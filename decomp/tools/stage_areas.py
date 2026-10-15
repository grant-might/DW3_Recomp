#!/usr/bin/env python3
"""
Moves each stage's C file into the folder of its area: src/field/stages/<area>/.

FIELDSTG starts a stage for a mode (FIELDSTG_stages, and the European
version's extra chapter's FIELDSTG_extraStages: {mode, file,
WSTAGnnn_startStage}), and shows the name of
the mode's area as the player enters (FIELDSTG_areaNames: {area, place,
mode}; the area is a string of text file 0xAA, STAREA). Every mode of a
stage is in one area, whose name in lower_snake_case is its folder:
src/field/stages/wstag231.c goes to src/field/stages/amaterasu_city/wstag231.c, with
its head if it has one (wstag924_head.c). WSTAG260, the story events'
scripts, which FIELDSTG starts with FIELDSTG_createStoryEvents, stays in
src/field/stages/, and so does common/.

It also writes src/field/stages/README.md, the list of the areas and their
stages, with each stage's name from tools/stage_names_doc.py. The build finds a stage wherever it is under src/field/stages/, so the
bytes don't change. It can be run again at any time:

    tools/stage_areas.py [-n]

-n lists what it would move without moving it.
"""
import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STAGES = ROOT / "src" / "field" / "stages"
FIELDSTG_DATA = ROOT / "src" / "field" / "field_mode" / "data" / "fieldstg.c"
# STAREA, in either version's English text
AREA_TEXT = [ROOT / "disks/us/AAA/DAT/COUNTRY/USA/USSTAREA.BIN",
             ROOT / "disks/eu/AAA/DAT/COUNTRY/ENG/ESSTAREA.BIN"]
STAGE = re.compile(r"^wstag\d{3}$")


def text_strings(path):
    """The strings of a text file: a word count, the offsets, and the strings,
    each up to a 0 (1 a space, 2-0xB the digits, 0xE and 0x28 the letters)"""
    data = path.read_bytes()
    count = struct.unpack_from("<I", data)[0]
    offsets = struct.unpack_from(f"<{count}I", data, 4)
    out = []
    for start in offsets:
        chars = []
        for b in data[start:data.index(b"\0", start)]:
            if b == 1:
                chars.append(" ")
            elif 2 <= b <= 0xB:
                chars.append(str(b - 2))
            elif 0xE <= b < 0xE + 26:
                chars.append(chr(ord("A") + b - 0xE))
            elif 0x28 <= b < 0x28 + 26:
                chars.append(chr(ord("a") + b - 0x28))
            else:
                chars.append("?")
        out.append("".join(chars))
    return out


def tables(text, declaration):
    """The bodies of every definition of a table (both versions' #if copies)"""
    return re.findall(rf"^{re.escape(declaration)}\[\] = \{{\n(.*?)^\}};", text, re.M | re.S)


def stage_areas():
    """{stage: area name}, from FIELDSTG's tables and STAREA"""
    text = FIELDSTG_DATA.read_text()
    modes = {}
    for body in tables(text, "StageEntry FIELDSTG_stages") + tables(text, "StageEntry FIELDSTG_extraStages"):
        for mode, stage in re.findall(r"\{(\d+), \w+, WSTAG(\d+)_startStage\}", body):
            modes.setdefault(f"wstag{stage}", set()).add(int(mode))
    mode_areas = {}
    for body in tables(text, "AreaName FIELDSTG_areaNames"):
        for area, _, mode in re.findall(r"\{(\d+), (\d+), (\d+)\}", body):
            mode_areas.setdefault(int(mode), set()).add(int(area))
    path = next((p for p in AREA_TEXT if p.exists()), None)
    if path is None:
        sys.exit("no STAREA text: extract a disc first (tools/extract_disc.py)")
    names = [" ".join(s.split()) for s in text_strings(path)]
    out = {}
    for stage, stage_modes in modes.items():
        areas = {names[a] for m in stage_modes for a in mode_areas.get(m, ())}
        if len(areas) != 1:
            sys.exit(f"{stage}: its modes {sorted(stage_modes)} are in the areas {sorted(areas)}")
        out[stage] = areas.pop()
    return out


def folder(area):
    """An area's folder: its name in lower_snake_case"""
    return re.sub(r"[^a-z0-9]+", "_", area.lower()).strip("_")


def stage_files():
    """{stage: its C file}, wherever it is under src/field/stages/"""
    out = {}
    for path in sorted(STAGES.glob("wstag*.c")) + sorted(STAGES.glob("*/wstag*.c")):
        name = path.stem[: -len("_head")] if path.stem.endswith("_head") else path.stem
        if STAGE.match(name):
            out.setdefault(name, []).append(path)
    return out


def readme(areas):
    """src/field/stages/README.md: the areas and their stages, with their names"""
    from stage_names_doc import STAGES as NAMES

    by_area = {}
    for stage, area in sorted(areas.items()):
        by_area.setdefault(area, []).append(stage[len("wstag"):])
    by_area = sorted(by_area.items(), key=lambda kv: kv[1][0])
    rows = "".join(f"| `{folder(a)}/` | {a} | {len(s)} | {', '.join(s)} |\n" for a, s in by_area)
    lists = "".join(f"\n## {a} (`{folder(a)}/`)\n\n"
                    + "".join(f"- `wstag{n}`: {NAMES[int(n)][0]}\n" for n in s)
                    for a, s in by_area)
    return f"""# The stage overlays

Each stage overlay, AAA/PRO/WSTAG###.PRO, is one C file, `wstag###.c`, in
the folder of the area that FIELDSTG names when the player enters it
(FIELDSTG_stages gives a stage its modes, FIELDSTG_areaNames a mode its
area, a string of text file 0xAA). `tools/stage_areas.py` moves them and
writes this list.

- `common/` holds the code that several stages share, which they include.
- `wstag260.c`, the story events' scripts, is the file mode 528 (Asuka
  City's Cargo Tower) loads, but FIELDSTG starts that mode with its own
  FIELDSTG_createStoryEvents, so the stage has no code and stays here.

The cities have two copies of their stages, one for each server (Asuka and
Amaterasu, Seiryu and Qing Long, Suzaku and Zhu Que, Byakko and Bai Hu),
and the European version adds stages of its own (920 and up) to the areas.

Each stage's file starts with a comment that says which map or event it is
(`tools/stage_names_doc.py` writes them, and the names below).

| Folder | Area | Stages | WSTAG |
|---|---|---|---|
{rows}{lists}"""


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    ap.add_argument("-n", "--dry-run", action="store_true", help="only list what would move")
    args = ap.parse_args()
    areas = stage_areas()
    moved = 0
    for stage, paths in stage_files().items():
        if stage not in areas:
            continue
        for path in paths:
            dest = STAGES / folder(areas[stage]) / path.name
            if path == dest:
                continue
            moved += 1
            if args.dry_run:
                print(f"{path.relative_to(ROOT)} -> {dest.relative_to(ROOT)}")
                continue
            dest.parent.mkdir(exist_ok=True)
            path.rename(dest)
    text = readme(areas)
    path = STAGES / "README.md"
    if not args.dry_run and (not path.exists() or path.read_text() != text):
        path.write_text(text)
    print(f"{moved} files {'would move' if args.dry_run else 'moved'}", file=sys.stderr)


if __name__ == "__main__":
    main()
