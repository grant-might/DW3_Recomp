"""The version of the game the tools work on, and where its files are.

VERSION comes from the environment, as the Makefile exports it (make
VERSION=us), and defaults to eu like the Makefile. Each version has its
settings in mk/version/<version>.mk (the executable's name and the disc),
its splat configs, symbols and checksums in config/<version>/; splat writes
its disassembly to asm/<version>/, the build goes to build/<version>/ and
objdiff's target objects to expected/<version>/.
"""

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

VERSION = os.environ.get("VERSION") or "eu"

_MK = ROOT / "mk" / "version" / f"{VERSION}.mk"
if not _MK.is_file():
    sys.exit(f"unsupported VERSION {VERSION}: there is no {_MK.relative_to(ROOT)}")
# the NAME := VALUE settings of the version's .mk
_SETTINGS = dict(re.findall(r"^(\w+)\s*:=\s*(.*?)\s*$", _MK.read_text(), re.M))

# the release, such as "Digimon World 3, USA (SLUS-01436)"
VERSION_NAME = _SETTINGS.get("VERSION_NAME", VERSION)
# the executable's name (SLUS_014.36) and the extracted disc it is on
EXE_NAME = _SETTINGS["EXE_NAME"]
DISK_DIR = ROOT / _SETTINGS["DISK_DIR"]

# where the overlays load, and the stage overlays (after CARDGAME)
OVERLAY_VRAM = int(_SETTINGS["OVERLAY_VRAM"], 16)
STAGE_VRAM = int(_SETTINGS["STAGE_VRAM"], 16)

# splat configs (<binary>.yaml), symbols and checksums
CONFIG_DIR = ROOT / "config" / VERSION
# splat's output (<binary>/...)
ASM_DIR = ROOT / "asm" / VERSION
# objects, ELFs, binaries and the report
BUILD_DIR = ROOT / "build" / VERSION
# objdiff's target objects
EXPECTED_DIR = ROOT / "expected" / VERSION
# the prebuilt compiler and tools (tools/dl_deps.sh), as the Makefile's BIN_DIR
BIN_DIR = Path(os.environ.get("BIN_DIR") or ROOT / "bin")

# where each binary's C is under src/: grouped by the part of the game it
# runs, each overlay in a folder named after what it is (the disc's name is
# the binary's: config/<version>/<binary>.yaml, asm/<version>/<binary>/);
# src/shared/ holds the code several overlays include
SRC = ROOT / "src"
SRC_DIRS = {
    "main": "engine",
    "fieldstg": "field/field_mode", "stages": "field/stages",
    "fightstg": "battle/battle_mode", "wfightmn": "battle/normal_battle", "stfgtrep": "battle/report",
    "cardgame": "cardgame/card_battle", "stcrddek": "cardgame/deck_editor",
    "stcrdshp": "cardgame/card_packs", "stcrdabm": "cardgame/album",
    "stdwtitl": "menus/title", "ststatus": "menus/field_menu", "stitshop": "menus/item_shop",
    "stgmcard": "menus/memory_card", "stgdglab": "menus/digimon_lab", "stgtrain": "menus/gym",
    "stplnmet": "menus/player_name", "stdgname": "menus/partner_name", "cnty_sel": "menus/country_select",
    "stagslct": "debug/stage_select", "wfightts": "debug/battle_test",
    "shocktst": "debug/vibration_test", "soundtst": "debug/sound_test",
}


def src_dir(binary: str) -> Path:
    """The folder of a binary's C ("stages" for the stage overlays)."""
    return SRC / SRC_DIRS[binary]


def unit_path(source) -> str:
    """A C file's path as splat's asm/<version>/ and objdiff's units name it,
    under its binary: src/engine/menu/inn.c is main/menu/inn,
    src/field/stages/x/wstag200.c stages/x/wstag200. Files outside a binary's
    folder (src/shared/) keep their path under src/."""
    rel = Path(source).resolve().relative_to(SRC).with_suffix("").as_posix()
    for binary, folder in SRC_DIRS.items():
        if rel.startswith(folder + "/"):
            return binary + rel[len(folder):]
    return rel


def stage_entries(version: str = VERSION) -> list:
    """The words of each line of config/<version>/stages.txt, without its
    comments: a stage's name, then the offsets of its code if it has any."""
    path = ROOT / "config" / version / "stages.txt"
    if not path.exists():
        return []
    lines = (line.split("#", 1)[0].split() for line in path.read_text().splitlines())
    return [words for words in lines if words]


def stage_path(stage: str) -> str:
    """Where a stage is under src/field/stages/ (and asm/<version>/stages/), without
    the .c: <area>/<stage> in the folder of its area (tools/stage_areas.py),
    or <stage> for one in no area (wstag260), as the Makefile's
    STAGE_PATH_<stage>."""
    found = sorted(src_dir("stages").glob(f"*/{stage}.c"))
    return f"{found[0].parent.name}/{stage}" if found else stage


def overlay_parents() -> dict:
    """{overlay: the overlay it loads on top of}, as the Makefile's
    OVL_PARENT_<name>, and "<stage>": every stage's (FIELDSTG)."""
    make = (ROOT / "Makefile").read_text()
    out = dict(re.findall(r"^OVL_PARENT_(\w+)\s*:=\s*(\w+)", make, re.M))
    m = re.search(r"OVL_PARENT_\$\(s\)\s*:=\s*(\w+)", make)
    out["<stage>"] = m[1] if m else "fieldstg"
    return out
