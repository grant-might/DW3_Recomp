#!/usr/bin/env python3
"""Boots a build in DuckStation and checks that the game reaches its first menu.

    VERSION=<version> smoke.py --disc IMAGE --bios DIR [--pad PAD]
                               [--build DIR] [--emulator DUCKSTATION]

`make smoke` runs it: an optional check, on your machine only, that the
build boots. It writes the build (build/<version>, or the padding build's
build/<version>/pad<pad>; with --build, another toolchain's, such as
build/<version>/gcc, whose files may be smaller) into a copy of the original
disc image (tools/patch_disc.py), boots that in DuckStation under xvfb-run,
with no window, and reads GAME.mode through DuckStation's GDB server until
the game waits in the first screen that needs a button: the title screen
(USA) or the language menu (Europe). Then it watches the game for HOLD
seconds: it passes if the game takes no exception that the kernel leaves
unresolved (all but interrupts and system calls), never jumps to 0, runs
only known code (the BIOS, the kernel, the build's binaries), keeps drawing
frames and stays in that screen, with the code of one of the build's mode
overlays where the executable loads them. A padding build moves every
binary, so a pass with PAD=0x10004 means that the code and data that moved
still work.

DuckStation runs with HOME in build/.../smoke/home, so that it reads the
settings written there and never yours, nor your memory cards. It reads the
BIOS from DIR and writes nothing there. It is stopped through its process
group, whatever happens.
"""
import argparse
import os
import shutil
import signal
import socket
import subprocess
import sys
import time
from pathlib import Path

from elftools.elf.elffile import ELFFile

from gdb_rsp import GdbClient
from patch_disc import patch
from version import BUILD_DIR, EXE_NAME, VERSION

# GAME.mode (include/engine/game_state.h), and the mode in which the game waits
# for a button on its first screen: STDWTITL's title screen, or the
# European language menu (CNTY_SEL), which comes before it
MODE_OFFSET = {"us": 0x26BC, "eu": 0x26C4}[VERSION]
FIRST_MENU = {"us": 0xE01, "eu": 0x1600}[VERSION]

# GFX.frameCount (include/engine/graphics.h): the frames that drawFrame ended
FRAME_COUNT_OFFSET = 0x08

TIMEOUT = 120  # seconds until the first menu
HOLD = 20  # seconds of watching it
POLL = 1
MIN_FRAMES = 60  # that the game must draw while it is watched
READ_CHUNK = 0x400  # bytes per memory read

# The kernel's exception handler gives every exception but interrupts and
# system calls to A(40h), SystemErrorUnresolvedException, which hangs
# (https://psx-spx.consoledev.net/ps1/kernelbios/function-summary/#a-functions-call-00a0h-with-function-number-in-r9-register):
# its address is in the A(nnh) jump table at 0x200
# (https://psx-spx.consoledev.net/ps1/kernelbios/memory-map/#bios-ram-map-1st-64kbytes-of-ram-fixed-addresses-mainly-in-1st-500h-bytes)
A0_TABLE = 0x80000200
UNRESOLVED_EXCEPTION = A0_TABLE + 4 * 0x40
# the exception codes in COP0's Cause register, bits 2-6
# (https://psx-spx.consoledev.net/ps1/cpu/cpuspecifications/#cop0r13-cause-read-only-except-bit8-9-are-rw)
EXCEPTIONS = {0: "Int", 4: "AdEL", 5: "AdES", 6: "IBE", 7: "DBE", 8: "Syscall", 9: "BP",
              10: "RI", 11: "CpU", 12: "Ov"}
# Known code, as physical addresses (KUSEG, KSEG0 and KSEG1 mirror them):
# the kernel in the first 64 KB of RAM and the BIOS ROM
# (https://psx-spx.consoledev.net/ps1/system/memorymap/)
PHYSICAL = 0x1FFFFFFF
KERNEL = (0, 0x10000)
BIOS_ROM = (0x1FC00000, 0x1FC80000)

# A minimal configuration: no audio, memory cards, achievements or update
# check; fast boot through the BIOS (BIOS_DIR) and the GDB server on PORT.
SETTINGS = """\
[Main]
SetupWizardIncomplete = false
ConfirmPowerOff = false
SaveStateOnExit = false
StartPaused = false
PauseOnFocusLoss = false
InhibitScreensaver = false
EnableDiscordPresence = false
[AutoUpdater]
CheckAtStartup = false
[BIOS]
SearchDirectory = {bios}
PatchFastBoot = true
[GPU]
Renderer = Software
[Audio]
Backend = Null
[MemoryCards]
Card1Type = None
Card2Type = None
[Cheevos]
Enabled = false
[Debug]
EnableGDBServer = true
GDBServerPort = {port}
"""

# DuckStation asks, in a dialog that nobody would answer, to create a
# launcher shortcut for an AppImage unless the shortcut exists
DESKTOP_FILE = ".local/share/applications/org.duckstation.DuckStation.desktop"


def free_port():
    with socket.socket() as s:
        s.bind(("127.0.0.1", 0))
        return s.getsockname()[1]


def symbol(elf_path, name):
    with open(elf_path, "rb") as f:
        for sym in ELFFile(f).get_section_by_name(".symtab").iter_symbols():
            if sym.name == name:
                return sym["st_value"]
    sys.exit(f"{elf_path} has no {name}")


def same_file(a, b):
    with open(a, "rb") as fa, open(b, "rb") as fb:
        while True:
            chunk = fa.read(1 << 20)
            if chunk != fb.read(1 << 20):
                return False
            if not chunk:
                return True


def isolated_home(out, bios, port):
    home = out / "home"
    settings = home / ".local/share/duckstation/settings.ini"
    settings.parent.mkdir(parents=True, exist_ok=True)
    settings.write_text(SETTINGS.format(bios=bios, port=port))
    (home / DESKTOP_FILE).parent.mkdir(parents=True, exist_ok=True)
    (home / DESKTOP_FILE).touch()
    return home


def code_span(linkdir):
    """Where the build's code is, as physical addresses."""
    starts, ends = [], []
    for path in linkdir.glob("*.elf"):
        with open(path, "rb") as f:
            for s in ELFFile(f).iter_sections():
                if s["sh_flags"] & 4 and s["sh_size"] and s["sh_addr"] >= 0x80000000:
                    starts.append(s["sh_addr"] & PHYSICAL)
                    ends.append((s["sh_addr"] + s["sh_size"]) & PHYSICAL)
    return min(starts), max(ends)


def mode_overlays(linkdir, address):
    """The code of the overlays that load at ADDRESS: {name: (the address of
    its code, its code)}, from splat's <name>_TEXT_START and _TEXT_END."""
    found = {}
    for path in sorted(linkdir.glob("*.elf")):
        with open(path, "rb") as f:
            elf = ELFFile(f)
            section = elf.get_section_by_name("." + path.stem)
            if section is None or section["sh_addr"] != address:
                continue
            names = {f"{path.stem}_TEXT_START", f"{path.stem}_TEXT_END"}
            bounds = {s.name: s["st_value"] for s in elf.get_section_by_name(".symtab").iter_symbols()
                      if s.name in names}
            start, end = bounds[f"{path.stem}_TEXT_START"], bounds[f"{path.stem}_TEXT_END"]
            found[path.stem] = (start, section.data()[start - address : end - address])
    return found


def read_memory(gdb, address, size):
    return b"".join(gdb.read(a, min(READ_CHUNK, address + size - a)) for a in range(address, address + size, READ_CHUNK))


def loaded_overlay(gdb, overlays):
    """The name of the overlay whose code is in memory, or None."""
    for name, (address, text) in overlays.items():
        if gdb.read(address, len(text[:READ_CHUNK])) == text[:READ_CHUNK] and \
           read_memory(gdb, address, len(text)) == text:
            return name
    return None


def wait_for_first_menu(gdb, mode_address, emulator):
    """Polls GAME.mode until it is FIRST_MENU, and returns how long that took,
    or None and the last mode."""
    start = time.monotonic()
    mode = None
    while time.monotonic() - start < TIMEOUT and emulator.poll() is None:
        mode = int.from_bytes(gdb.read(mode_address, 4), "little")
        if mode == FIRST_MENU:
            return time.monotonic() - start, mode
        gdb.cont()
        time.sleep(POLL)
        gdb.halt()
    return None, mode


def watch(gdb, mode_address, frames_address, code, overlays):
    """Runs the game for HOLD seconds, stopping it at an unresolved exception
    and at a jump to 0, and looking where it runs every POLL seconds; returns
    what went wrong, or None, the mode overlay in memory and the frames drawn."""
    def read_word(address):
        return int.from_bytes(gdb.read(address, 4), "little")

    known = (KERNEL, BIOS_ROM, code)
    first_frame = read_word(frames_address)
    breakpoints = (read_word(UNRESOLVED_EXCEPTION), 0)
    for address in breakpoints:
        gdb.set_breakpoint(address)
    deadline = time.monotonic() + HOLD
    while time.monotonic() < deadline:
        gdb.cont()
        stopped = gdb.wait(POLL) is not None
        if not stopped:
            gdb.halt()
        regs = gdb.registers()
        if stopped and regs["pc"] == 0:
            return f"a jump to 0, ra {regs['ra']:#010x}", None, None
        if stopped:
            number = (regs["cause"] >> 2) & 0x1F
            return (f"exception {EXCEPTIONS.get(number, number)}, BadVAddr "
                    f"{regs['badvaddr']:#010x}, ra {regs['ra']:#010x}"), None, None
        if not any(lo <= regs["pc"] & PHYSICAL < hi for lo, hi in known):
            return f"running unknown code at {regs['pc']:#010x}", None, None
    for address in breakpoints:
        gdb.remove_breakpoint(address)
    frames = read_word(frames_address) - first_frame
    if frames < MIN_FRAMES:
        return f"only {frames} frames in {HOLD} s", None, frames
    mode = read_word(mode_address)
    if mode != FIRST_MENU:
        return f"GAME.mode {mode:#x}", None, frames
    overlay = loaded_overlay(gdb, overlays)
    if overlay is None:
        return "no mode overlay's code where the executable loads them", None, frames
    return None, overlay, frames


def stop(emulator):
    """Stops xvfb-run, Xvfb and DuckStation, the process group that Popen
    began with xvfb-run, and waits until none of them is left."""
    for sig in (signal.SIGTERM, signal.SIGKILL):
        deadline = time.monotonic() + 10
        try:
            os.killpg(emulator.pid, sig)
            while time.monotonic() < deadline:
                emulator.poll()
                os.killpg(emulator.pid, 0)
                time.sleep(0.2)
        except ProcessLookupError:
            return


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--disc", required=True, help="the original disc image (.bin)")
    parser.add_argument("--bios", required=True, help="a directory with a PlayStation BIOS")
    parser.add_argument("--pad", default="0", help="the padding build to boot, as make's PAD")
    parser.add_argument("--build", type=Path, default=BUILD_DIR,
                        help="the build, as make's BUILDDIR (build/<version>/gcc for TOOLCHAIN=gcc)")
    parser.add_argument("--emulator", default="duckstation-qt", help="DuckStation (Qt)")
    args = parser.parse_args()

    if not shutil.which("xvfb-run"):
        sys.exit("the smoke test runs DuckStation under xvfb-run: install xvfb (apt install xvfb)")
    emulator_path = shutil.which(args.emulator)
    if not emulator_path:
        sys.exit(f"DuckStation isn't {args.emulator}: give its path with DUCKSTATION=")
    build = args.build.resolve()
    linkdir = build if args.pad == "0" else build / f"pad{args.pad}"
    out = linkdir / "smoke"
    disc = patch(args.disc, linkdir, out, fit=build != BUILD_DIR)
    if same_file(disc, args.disc):
        print("the image is the original disc")
    exe = linkdir / f"{EXE_NAME}.elf"
    mode_address = symbol(exe, "GAME") + MODE_OFFSET
    frames_address = symbol(exe, "GFX") + FRAME_COUNT_OFFSET
    code = code_span(linkdir)
    overlays = mode_overlays(linkdir, symbol(exe, "OVERLAY_VRAM"))

    port = free_port()
    home = isolated_home(out, os.path.abspath(args.bios), port)
    env = {k: v for k, v in os.environ.items() if not k.startswith("XDG_") or k == "XDG_RUNTIME_DIR"}
    env.update(HOME=str(home), QT_QPA_PLATFORM="xcb")
    command = ["xvfb-run", "--auto-servernum", "--server-args=-screen 0 640x480x24",
               emulator_path, "-batch", "-nogui", "-fastboot", "--", str(out / "disc.cue")]
    with open(out / "emulator.log", "wb") as log:
        emulator = subprocess.Popen(command, env=env, stdout=log, stderr=subprocess.STDOUT,
                                    stdin=subprocess.DEVNULL, start_new_session=True)
    problem = None
    try:
        gdb = GdbClient(port, timeout=60)
        gdb.stop_reason()
        seconds, mode = wait_for_first_menu(gdb, mode_address, emulator)
        if seconds is not None:
            problem, overlay, frames = watch(gdb, mode_address, frames_address, code, overlays)
        gdb.close()
    finally:
        stop(emulator)

    where = f"{VERSION}" + (f" {os.path.relpath(build, BUILD_DIR)}" if build != BUILD_DIR else "") + \
        (f" PAD={args.pad}" if args.pad != "0" else "")
    if seconds is None:
        last = "none" if mode is None else f"{mode:#x}"
        sys.exit(f"smoke {where}: no first menu ({FIRST_MENU:#x}) in {TIMEOUT} s, "
                 f"GAME.mode {last}; see {out / 'emulator.log'}")
    if problem:
        sys.exit(f"smoke {where}: the first menu ({FIRST_MENU:#x}) after {seconds:.0f} s, then {problem}")
    print(f"smoke {where}: the first menu ({FIRST_MENU:#x}) after {seconds:.0f} s, then {HOLD} s "
          f"without an error, {frames} frames, with {overlay.upper()}'s code in place")


if __name__ == "__main__":
    main()
