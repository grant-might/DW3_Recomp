#!/usr/bin/env python3
"""
Writes the numbers the stages give FIELDSTG with the names of field_map.h.

- a drawing layer of the field, the first argument of GFX.funcs.getLayer,
  createLayer's third, drawer.setLayerId, createTextWindow, createCursor
  and createScreenFade, becomes its FIELD_LAYER_ name;
- a task id of the field, the first argument of TASK_REGISTRY.funcs.find or
  the last of createTaskWithId, becomes its FIELD_TASK_ name (the stages' own
  ids stay numbers);
- the map given to FIELDSTG_map.setFile, getCell or files[] becomes its
  FIELD_MAP_ name;
- the type of a StageSlot, the word after its two conditions, becomes its
  SLOT_ name, and a flag of its conditions, FLAG(0, n), its FIELD_FLAG_ name;
- a field command of an event script, a script command (SCRIPT_COMMAND,
  or its word 0x101) of FIELD_TASK_COMMANDS, becomes its FIELD_COMMAND_ name.

It changes no bytes and only rewrites numbers, so it can be run again at any
time; a stage file it changes that has none of the field's headers gets
field_map.h after its first include. -n lists what it would change, file by file, without writing:

    tools/field_constants.py [-n] [src/field/stages/central_sector/wstag200.c ...]
"""
import argparse
import difflib
import glob
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEADER = ROOT / "include/field/field_map.h"
NUMBER = r"0x[0-9A-Fa-f]+|\d+"


def names(prefix):
    """{value: name} of field_map.h's defines with that prefix"""
    out = {}
    for m in re.finditer(rf"^#define ({prefix}\w+) ({NUMBER})\b", HEADER.read_text(), re.M):
        out.setdefault(int(m.group(2), 0), m.group(1))
    return out


LAYERS = names("FIELD_LAYER_")
TASKS = names("FIELD_TASK_")
MAPS = names("FIELD_MAP_")
SLOTS = names("SLOT_")
FLAGS = names("FIELD_FLAG_")


def commands():
    """{value: name} of the field commands, those of the object groups as calls"""
    out = names("FIELD_COMMAND_")
    text = HEADER.read_text()
    groups = int(re.search(rf"^#define FIELD_OBJECT_GROUPS ({NUMBER})\b", text, re.M).group(1), 0)
    for name, base in re.findall(rf"^#define (FIELD_COMMAND_\w+)\(n\) \(({NUMBER}) \+ \(n\)\)", text, re.M):
        for n in range(groups):
            out[int(base, 0) + n] = f"{name}({n})"
    return out


COMMANDS = commands()


def by(table):
    """a substitution that names group 'n' of a match by table, keeping the rest"""
    def sub(m):
        name = table.get(int(m.group("n"), 0))
        if name is None:
            return m.group(0)
        return m.group(0)[: m.start("n") - m.start(0)] + name + m.group(0)[m.end("n") - m.start(0):]
    return sub


N = rf"(?P<n>{NUMBER})\b"
RULES = [
    (re.compile(rf"\b(?:GFX\.funcs\.getLayer|setLayerId|createTextWindow|createCursor|createScreenFade)\({N}"), by(LAYERS)),
    (re.compile(rf"\bcreateLayer\([^,()]+, [^,()]+, {N}\)"), by(LAYERS)),
    (re.compile(rf"\bTASK_REGISTRY\.funcs\.find\({N}"), by(TASKS)),
    (re.compile(rf"\bcreateTaskWithId\([^;]*, {N}\);"), by(TASKS)),
    (re.compile(rf"\bFIELDSTG_map\.(?:setFile|getCell)\({N}"), by(MAPS)),
    (re.compile(rf"\bFIELDSTG_map\.files\[{N}\]"), by(MAPS)),
    (re.compile(rf"(?:\b0x101, |\bSCRIPT_COMMAND\()(?P<n>0x32D)\b"), by(TASKS)),
    (re.compile(rf"(?:\b0x101, |\bSCRIPT_COMMAND\()FIELD_TASK_COMMANDS, {N}"), by(COMMANDS)),
]
# a StageSlot table, and the type of each of its records
SLOT_TABLE = re.compile(r"^(?:static )?StageSlot \w+\[\w*\] = \{.*?^\};", re.M | re.S)
SLOT_TYPE = re.compile(rf"\{{ \{{ \{{ [^{{}}]+ \}}, \{{ [^{{}}]+ \}} \}}, {N},")
SLOT_FLAG = re.compile(rf"\bFLAG\(0, {N}\)")


HEADERS = re.compile(r'^#include "(?:stage|field_map|fieldstg)\.h"', re.M)
FIRST_INCLUDE = re.compile(r"^#include .*\n", re.M)


def rewrite(text):
    """text with the numbers named"""
    for pattern, sub in RULES:
        text = pattern.sub(sub, text)
    return SLOT_TABLE.sub(lambda m: SLOT_FLAG.sub(by(FLAGS), SLOT_TYPE.sub(by(SLOTS), m.group(0))), text)


def with_header(path, text):
    """text, with field_map.h included if a stage file (not an .inc.c) has no field header"""
    if path.endswith(".inc.c") or HEADERS.search(text):
        return text
    return FIRST_INCLUDE.sub(lambda m: m.group(0) + '#include "field_map.h"\n', text, count=1)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[1])
    ap.add_argument("files", nargs="*")
    ap.add_argument("-n", "--dry-run", action="store_true", help="only list what would change")
    args = ap.parse_args()
    paths = args.files or sorted(glob.glob(str(ROOT / "src/field/stages/**/*.c"), recursive=True))
    changed = 0
    for path in paths:
        old = Path(path).read_text()
        new = rewrite(old)
        if new == old:
            continue
        new = with_header(str(path), new)
        changed += 1
        if args.dry_run:
            name = Path(path).relative_to(ROOT) if Path(path).is_absolute() else path
            a, b = old.split("\n"), new.split("\n")
            for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
                if op == "replace":
                    for x, y in zip(a[i1:i2], b[j1:j2]):
                        print(f"{name}: {x.strip()}  ->  {y.strip()}")
                elif op == "insert":
                    for y in b[j1:j2]:
                        print(f"{name}: + {y.strip()}")
        else:
            Path(path).write_text(new)
    print(f"{changed} files {'would change' if args.dry_run else 'changed'}", file=sys.stderr)


if __name__ == "__main__":
    main()
