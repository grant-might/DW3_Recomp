"""The two native recompilation builds this launcher runs, and how it starts them.

Nothing here is absolute: Builds/ and Discs/ sit beside the launcher itself
(paths.launcher_root()), so the whole install can be moved or copied to another machine. Each
regional build is self-contained — its exe, its game.toml, its bios/ and its input files all live
in one folder:

    Builds/EUR — Digimon World 2003, PAL,    id SLES-03936   (exe Digimon_World_2003_Recompiled.exe)
    Builds/USA — Digimon World 3,   NTSC-U,  id SLUS-01436   (exe Digimon_World_3_Recompiled.exe)

`BUILDS` lists the USA build first: every place that walks the tuple (the Play tab's buttons, its
labels and the disc router) then shows USA before Europe, in one consistent order.

The launcher does not ship these builds — the player makes them from their own disc with the
bundled recompiler (see builder.py). This module only describes where a finished build lives, how
a build's own `game.toml` is pointed at a disc image, and how it is launched.

The runtime resolves game.toml, bios/, input.ini and card1.mcd from its WORKING DIRECTORY, so
`launch` starts the exe with cwd set to the build folder. The one file the launcher rewrites is
that build's own game.toml, and only its `disc = ` line — through tomlkit, so comments, key order
and every other value survive the write.
"""
from __future__ import annotations

import os
import pathlib
import re
import subprocess
import sys
from dataclasses import dataclass

from . import disc, paths

try:
    import tomlkit
except ImportError:  # pragma: no cover - surfaced by the caller with a clear message
    tomlkit = None  # type: ignore[assignment]

# The variable the runtime reads to merge the keyboard and every connected controller onto
# Player 1, so a pad works without the player rebinding anything.
DEV_INPUT_VAR = "PSX_DEV_INPUT"

# Re-exported so callers do not need disc.REGION_* just to name a build.
REGION_EU = disc.REGION_EU
REGION_US = disc.REGION_US


@dataclass(frozen=True)
class BuildSpec:
    region: str                    # disc.REGION_EU / disc.REGION_US
    folder: str                    # the folder under Builds/
    label: str                     # what the UI calls this build
    exe_name: str                  # the executable inside that folder
    disc_name: str                 # a default image name for the Discs/ hint
    title: str                     # the runtime's window title / recompiler --name


BUILDS = (
    BuildSpec(REGION_US, "USA", "USA", "Digimon_World_3_Recompiled.exe",
              "Digimon World 3 (USA).bin", "Digimon World 3"),
    BuildSpec(REGION_EU, "EUR", "Europe", "Digimon_World_2003_Recompiled.exe",
              "Digimon World 2003 (Europe).bin", "Digimon World 2003"),
)

_BY_REGION = {b.region: b for b in BUILDS}


def spec(region: str) -> BuildSpec:
    try:
        return _BY_REGION[region]
    except KeyError:
        raise KeyError(f"unknown build region {region!r}") from None


# ---------------------------------------------------------------- discovery (relative to the app)

def builds_dir() -> pathlib.Path:
    return paths.launcher_root() / "Builds"


def discs_dir() -> pathlib.Path:
    return paths.launcher_root() / "Discs"


def build_dir(region: str) -> pathlib.Path:
    return builds_dir() / spec(region).folder


def exe_path(region: str) -> pathlib.Path:
    return build_dir(region) / spec(region).exe_name


def game_toml(region: str) -> pathlib.Path:
    return build_dir(region) / "game.toml"


def build_status(region: str) -> tuple[bool, pathlib.Path]:
    exe = exe_path(region)
    return exe.is_file(), exe


def card_paths(region: str) -> list[pathlib.Path]:
    """The two memory cards this build streams saves from (standard 128 KiB PSX cards)."""
    d = build_dir(region)
    return [d / "card1.mcd", d / "card2.mcd"]


def card_dirs() -> list[pathlib.Path]:
    """Every build folder, so the Memory Card tab can find its per-build cards."""
    return [build_dir(b.region) for b in BUILDS]


def input_files(region: str) -> dict[str, pathlib.Path]:
    d = build_dir(region)
    return {"input.ini": d / "input.ini", "keybinds.ini": d / "keybinds.ini"}


def run_log_path(region: str) -> pathlib.Path:
    """Where a launch tees the runtime's own console output, so the tab can show it honestly."""
    return build_dir(region) / "logs" / "runtime.log"


# ---------------------------------------------------------------- the disc router

def images() -> list[pathlib.Path]:
    """Disc images dropped in Discs/, a .bin preferred over its .cue companion."""
    found = disc.candidates_in(discs_dir())
    bins = [p for p in found if p.suffix.lower() == ".bin"]
    return bins or found


# Fix for `disc_name`-style callers: images() returns absolute paths already.
def classify(image: pathlib.Path) -> str:
    """The region this image's own serial says it is (PAL / NTSC-U / NTSC-J / unknown)."""
    return disc.region_of_image(image)


def disc_for_region(region: str) -> pathlib.Path | None:
    """The image in Discs/ whose serial classifies as `region`; None when there is none."""
    for img in images():
        if disc.region_of_image(img) == region:
            return img
    return None


_DISC_LINE = re.compile(r'^\s*disc\s*=\s*"([^"]*)"', re.M)


def baked_disc(region: str) -> pathlib.Path | None:
    """The image this build's own game.toml points at, when it still exists.

    A build can be made from a disc the player keeps anywhere; after a build the launcher bakes
    that path into the build's game.toml, so a launch does not depend on the image living in
    Discs/. Returns None when there is no file there any more.
    """
    p = game_toml(region)
    if not p.is_file():
        return None
    try:
        m = _DISC_LINE.search(p.read_text(encoding="utf-8-sig", errors="replace"))
    except OSError:
        return None
    if not m:
        return None
    img = pathlib.Path(m.group(1))
    return img if img.is_file() else None


def resolve_image(region: str) -> pathlib.Path | None:
    """The image to play this region with: one in Discs/ if present, else the build's baked one."""
    return disc_for_region(region) or baked_disc(region)


# ---------------------------------------------------------------- rewriting a build's game.toml

def set_disc(region: str, image: pathlib.Path) -> pathlib.Path:
    """Point this build's `disc = ` line at `image`, keeping every comment and other key.

    A plain toml round-trip would drop the runtime author's comments and reorder the file, so
    the edit goes through tomlkit and touches one key.
    """
    if tomlkit is None:
        raise RuntimeError("tomlkit is not installed (pip install tomlkit)")
    p = game_toml(region)
    if not p.is_file():
        raise FileNotFoundError(f"game.toml not found at {p}")
    doc = tomlkit.parse(p.read_text(encoding="utf-8-sig"))
    if "game" not in doc:
        doc["game"] = tomlkit.table()
    doc["game"]["disc"] = str(pathlib.Path(image).resolve())
    p.write_text(tomlkit.dumps(doc), encoding="utf-8")
    return p


# ---------------------------------------------------------------- launching

def _minimized_startupinfo():
    """Start the game minimized: the owner objects to windows flashing on their desktop."""
    if sys.platform != "win32":
        return None
    si = subprocess.STARTUPINFO()
    si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    si.wShowWindow = 6      # SW_MINIMIZE
    return si


def launch(region: str, extra_env: dict[str, str] | None = None,
           log_path: pathlib.Path | None = None) -> subprocess.Popen:
    """Start a regional build from its own folder, with the dev-input merge switched on.

    cwd is load-bearing: the runtime resolves game.toml, bios/, input.ini and card1.mcd from the
    working directory. When `log_path` is given the runtime's own stdout/stderr are teed there —
    the exe is a GUI-subsystem binary, so without a redirect its `psxrecomp: ...` progress lines
    (main() entered, disc region, executing from PC=0x...) go nowhere a player could read them.
    """
    exe = exe_path(region)
    if not exe.is_file():
        raise FileNotFoundError(f"{spec(region).label} build executable not found: {exe}")
    d = build_dir(region)
    if not d.is_dir():
        raise FileNotFoundError(f"{spec(region).label} build folder not found: {d}")
    env = dict(os.environ)
    env[DEV_INPUT_VAR] = "1"
    env.update(extra_env or {})
    creation = getattr(subprocess, "CREATE_NEW_PROCESS_GROUP", 0) if sys.platform == "win32" else 0
    out = None
    if log_path is not None:
        log_path.parent.mkdir(parents=True, exist_ok=True)
        out = open(log_path, "w", encoding="utf-8", errors="replace")  # noqa: SIM115 (handed to Popen)
    try:
        return subprocess.Popen([str(exe)], cwd=str(d), env=env, creationflags=creation,
                                stdout=out, stderr=subprocess.STDOUT,
                                startupinfo=_minimized_startupinfo(), close_fds=True)
    finally:
        if out is not None:
            out.close()      # the child holds its own duplicate of the handle


def tail_log(region: str, max_lines: int = 40) -> list[str]:
    """The last lines of a build's runtime log, for showing honest progress in the tab."""
    p = run_log_path(region)
    if not p.is_file():
        return []
    try:
        lines = p.read_text(encoding="utf-8", errors="replace").splitlines()
    except OSError:
        return []
    return lines[-max_lines:]


def boots_to_bios(lines: list[str]) -> bool:
    """True when a run log shows the runtime reaching its recompiled BIOS entry point.

    The engine prints `psxrecomp runtime: executing from PC=0xBFC00000` when it starts executing
    the LLE BIOS; that is the honest 'it really booted' signal, not merely a live process.
    """
    return any("executing from PC=0xBFC00000" in ln for ln in lines)
