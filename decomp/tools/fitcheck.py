#!/usr/bin/env python3
"""Checks that a build's binaries fit where the original's go.

    VERSION=<version> fitcheck.py [--build DIR] [--all]

`make fitcheck` runs it on the build of the toolchain (build/<version>, or
build/<version>/gcc for TOOLCHAIN=gcc), whose binaries may be larger or
smaller than the original's (docs/toolchain.md). The game keeps the
original's memory map (EXE_VRAM, OVERLAY_VRAM, STAGE_VRAM) and its files'
places on the disc, so each binary must fit:

- in its file on the disc (disks/<version>), which a boot test writes the
  build into (tools/patch_disc.py): the CD reader loads as many sectors as
  the executable's file table gives (getFileSectorCount,
  src/engine/file/file_table.c), the original files';
- in memory: the executable's .bss before OVERLAY_VRAM, where the overlays
  load (config/<version>/undefined_syms.txt checks it too), and an overlay
  that others load on top of (FIELDSTG, FIGHTSTG) before STAGE_VRAM, where
  they load;
- and the heap, which begins after every overlay as it is loaded
  (tools/link_heap.py), no later than the original's (HEAP_BASE in
  config/<version>/symbols_main.txt), so that it isn't smaller.

It fails on a binary that doesn't fit; --all lists every binary with its
sizes and limits.
"""
import argparse
import re
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

from version import BUILD_DIR, CONFIG_DIR, DISK_DIR, EXE_NAME, VERSION, overlay_parents


def symbols(path, names):
    with open(path, "rb") as f:
        found = {s.name: s["st_value"] for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols()
                 if s.name in names}
    missing = set(names) - set(found)
    if missing:
        sys.exit(f"{path} has no {' '.join(sorted(missing))}")
    return found


def span(path):
    """Where a binary is in memory, .bss included: (start, end)."""
    with open(path, "rb") as f:
        sections = [s for s in ELFFile(f).iter_sections() if s["sh_flags"] & 2 and s["sh_size"]]
    return min(s["sh_addr"] for s in sections), max(s["sh_addr"] + s["sh_size"] for s in sections)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--build", type=Path, default=BUILD_DIR, help="the build, as make's BUILDDIR")
    parser.add_argument("--all", action="store_true", help="list every binary")
    args = parser.parse_args()

    exe = args.build / f"{EXE_NAME}.elf"
    syms = symbols(exe, {"main_BSS_END", "OVERLAY_VRAM", "STAGE_VRAM", "HEAP_BASE"})
    original_heap = int(re.search(r"^HEAP_BASE = (0x[0-9A-Fa-f]+);", (CONFIG_DIR / "symbols_main.txt").read_text(),
                                  re.M)[1], 16)
    parents = set(overlay_parents().values())

    # (name, file size, its limit, end in memory, its limit)
    rows = [(EXE_NAME, (args.build / EXE_NAME).stat().st_size, (DISK_DIR / EXE_NAME).stat().st_size,
             syms["main_BSS_END"], syms["OVERLAY_VRAM"])]
    for path in sorted(args.build.glob("*.elf")):
        if path == exe:
            continue
        name = f"{path.stem.upper()}.PRO"
        disc_file = DISK_DIR / "AAA" / "PRO" / name
        if not disc_file.exists():
            sys.exit(f"{disc_file} doesn't exist: extract the disc (tools/extract_disc.py)")
        end = span(path)[1]
        rows.append((name, (args.build / "AAA" / "PRO" / name).stat().st_size, disc_file.stat().st_size,
                     end, syms["STAGE_VRAM"] if path.stem in parents else None))

    failed = 0
    if args.all:
        print(f"{'binary':<14} {'file':>8} {'on disc':>8} {'free':>7}   {'end':>10} {'limit':>10} {'free':>7}")
    for name, size, size_limit, end, end_limit in rows:
        bad = size > size_limit or (end_limit is not None and end > end_limit)
        failed += bad
        if args.all or bad:
            memory = f"{end:#010x} {end_limit:#010x} {end_limit - end:7d}" if end_limit is not None \
                else f"{end:#010x} {'':>10} {'':>7}"
            print(f"{name:<14} {size:8d} {size_limit:8d} {size_limit - size:7d}   {memory}"
                  + ("  doesn't fit" if bad else ""))
    heap_bad = syms["HEAP_BASE"] > original_heap
    if args.all or heap_bad:
        print(f"the heap begins at {syms['HEAP_BASE']:#010x}, the original's at {original_heap:#010x}"
              + (": it is smaller" if heap_bad else ""))
    failed += heap_bad

    where = f"{VERSION} {args.build}"
    if failed:
        sys.exit(f"fitcheck {where}: {failed} don't fit")
    print(f"fitcheck {where}: the {len(rows)} binaries fit")


if __name__ == "__main__":
    main()
