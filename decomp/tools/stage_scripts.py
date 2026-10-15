#!/usr/bin/env python3
"""
Writes the stages' event scripts with the SCRIPT_ macros of field_map.h.

An event script (FieldEvent.script) is an array of 16-bit words that
FIELDSTG_runEvent reads: each command a word of its kind << 8 | its variant,
then a fixed number of arguments (field_map.h lists them). The data writes
one command a line; this rewrites each such line as its macro:

    0x101, 2, 1, 5,           ->  SCRIPT_POSE(2, 1, 5),
    0x101, 0x323, 0x325, 0xC2 ->  SCRIPT_COMMAND(0x323, 0x325, 0xC2),
    0x300, 0x1E,              ->  SCRIPT_WAIT(0x1E),

A pose word (0x101) whose first argument is a script command's id (0x320 and
up, FIELDSTG_setEventPose) is SCRIPT_COMMAND, else SCRIPT_POSE. A line that
isn't exactly one command, such as the halfword of padding some scripts have
after their end, stays as it is. The macros expand to the same words, so no
byte changes, and the lines already written with them are left alone: it can
be run again at any time.

    tools/stage_scripts.py [-n] [src/field/stages/central_sector/wstag200.c ...]
"""
import argparse
import glob
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
NUMBER = re.compile(r"-?(?:0x[0-9A-Fa-f]+|\d+)")
COMMAND_IDS = 0x320  # FIELDSTG_findEventActor: the actors' ids are below
TASK_NAMES = {"FIELD_TASK_COMMANDS"}  # the script command ids written by name

# opcode: (macro, number of arguments)
MACROS = {
    0x000: ("SCRIPT_END", 0),
    0x100: ("SCRIPT_PLACE", 3),
    0x101: ("SCRIPT_POSE", 3),
    0x102: ("SCRIPT_WALK", 4),
    0x200: ("SCRIPT_TALK", 4),
    0x300: ("SCRIPT_WAIT", 1),
    0x301: ("SCRIPT_WAIT_BOX", 0),
    0x302: ("SCRIPT_WAIT_WALK", 1),
    0x303: ("SCRIPT_WAIT_ANIM", 1),
    0x304: ("SCRIPT_LEAVE", 4),
    0x600: ("SCRIPT_FOLLOW", 2),
    0x601: ("SCRIPT_LOOK_AT", 3),
}
SCRIPT = re.compile(r"^((?:static )?s16 script\w*\[\w*\] = \{\n)(.*?)(^\};)", re.M | re.S)
LINE = re.compile(r"^(\s*)(.*?),\s*$")


def split(items):
    """the comma-separated items of a line, keeping the commas inside calls"""
    out, depth, cur = [], 0, ""
    for c in items:
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
            continue
        depth += c == "("
        depth -= c == ")"
        cur += c
    out.append(cur.strip())
    return out


def is_command_id(item):
    if item in TASK_NAMES:
        return True
    return NUMBER.fullmatch(item) is not None and int(item, 0) >= COMMAND_IDS


def rewrite_line(line, ended):
    """the line as a macro, or as it is; and whether the script has ended"""
    m = LINE.match(line)
    if ended or not m:
        return line, ended
    items = split(m.group(2))
    if not NUMBER.fullmatch(items[0]):
        return line, items[0] == "SCRIPT_END"
    op = int(items[0], 0)
    if op not in MACROS or len(items) != MACROS[op][1] + 1:
        return line, ended
    name = MACROS[op][0]
    if op == 0x101 and is_command_id(items[1]):
        name = "SCRIPT_COMMAND"
    if MACROS[op][1] == 0:
        return f"{m.group(1)}{name},", op == 0
    return f"{m.group(1)}{name}({', '.join(items[1:])}),", False


def rewrite_body(body):
    """a script's lines with the macros; the lines after its end (padding) stay"""
    out, ended = [], False
    # the versions' blocks each end the script on their own line: track the end per line only
    for line in body.split("\n"):
        stripped = line.strip()
        if stripped.startswith("#") or stripped.startswith("/*"):
            out.append(line)
            continue
        new, ended = rewrite_line(line, ended)
        out.append(new)
    return "\n".join(out)


def rewrite(text):
    return SCRIPT.sub(lambda m: m.group(1) + rewrite_body(m.group(2)) + m.group(3), text)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[1])
    ap.add_argument("files", nargs="*")
    ap.add_argument("-n", "--dry-run", action="store_true", help="only count what would change")
    args = ap.parse_args()
    paths = args.files or sorted(glob.glob(str(ROOT / "src/field/**/*.c"), recursive=True))
    changed = 0
    for path in paths:
        old = Path(path).read_text()
        new = rewrite(old)
        if new != old:
            changed += 1
            if not args.dry_run:
                Path(path).write_text(new)
    print(f"{changed} files {'would change' if args.dry_run else 'changed'}", file=sys.stderr)


if __name__ == "__main__":
    main()
