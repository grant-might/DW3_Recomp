#!/usr/bin/env python3
"""
Renames the fields of StageSlot, StagePoint and StagePoints where the code uses them.

The two structs (include/field/field_map.h, include/field/stage.h) had their offsets'
names (unkA...). A stage's place points are copied into its exit slots
field by field (copyPlacePoints), so a point's fields are named after the
slot's they go to, and a place's two ids after the slot's too. This renames them in the code that reads them, through
the pointers listed below; the structs' declarations are changed by hand.
It changes no bytes, and can be run again at any time:

    tools/stage_slot_fields.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

SLOT = {
    "unkA": "arg",
    "unkC": "x",
    "unkE": "y",
    "unk10": "dir",
    "unk12": "hideAnim",
    "unk14": "place",
    "unk16": "placeArg",
}
POINT = {
    "unk0": "arg",
    "unk2": "place",
    "unk4": "placeArg",
    "unk6": "x",
    "unk8": "y",
    "unkA": "dir",
}

PLACE = {"unk0": "place", "unk2": "placeArg"}

# file: [(the pointer the fields are read through, the struct's renames)]
USES = {
    "src/field/stages/common/copy_place_points.inc.c": [(r"\bslots", SLOT), (r"\bpoint", POINT), (r"\b(?:place|entry)", PLACE)],
    "src/field/field_mode/trigger.c": [(r"\btask->entry", SLOT)],
}


def main():
    changed = 0
    for name, uses in USES.items():
        path = ROOT / name
        text = path.read_text()
        new = text
        for pointer, fields in uses:
            for old, field in fields.items():
                new = re.sub(f"({pointer}->){old}\\b", f"\\g<1>{field}", new)
        if new != text:
            path.write_text(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
