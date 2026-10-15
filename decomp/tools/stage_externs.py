#!/usr/bin/env python3
"""
Drops the declarations of a stage's own data and functions that nothing needs.

data_to_c.py writes an extern for every datum of a stage before its code, but
a datum or a function that is defined before every use of it needs no
declaration. This keeps an extern or a prototype only where something (a
function, a table before the datum, or a src/field/stages/common file included
before it) uses the name before its definition, and drops copies of the
same declaration. The declarations before the first function go in one
block, and each block gets a comment that says why it is there.
It changes no bytes, and can be run again at any time:

    tools/stage_externs.py [src/field/stages/central_sector/wstag200.c ...]

With no files it goes through every stage, src/field/stages/<area>/wstag###.c
(and src/field/stages/wstag260.c).
"""
import argparse
import glob
import re
import sys

EXTERN = re.compile(r"^extern\s+[^;=(]*?\b(\w+)\s*((?:\[[^\]]*\])*)\s*;\s*$")
PROTOTYPE = re.compile(r"^(?:extern\s+)?(?!return\b)\w+[\w\s]*?[\s*]\**(\w+)\([^()]*\);\s*$")
TOKEN = re.compile(r"\b\w+\b")
INCLUDE = re.compile(r'^#include "(common/\w+\.inc\.c)"')
# The comments this writes above a block of declarations
COMMENT = re.compile(r"^/\* (Defined below, after|Defined in common/)")
FUNCTION = re.compile(r"^(?!extern\b)(?:static\s+)?\w+(?:\s+\w+)*[\s*]*\b\w+\([^;]*$")


def tokens(line):
    """The names a line uses: an #include of src/field/stages/common uses the file's"""
    m = INCLUDE.match(line)
    if m:
        with open(f"src/field/stages/{m.group(1)}") as f:
            return TOKEN.findall(f.read())
    return TOKEN.findall(line)


def definition_line(lines, name):
    """The line where the file defines name (not an extern), or None"""
    pattern = re.compile(
        r"^(?!extern\b)(?:static\s+)?(?:const\s+)?\w+(?:\s+\w+)*[\s*]*\b"
        + re.escape(name)
        + r"\s*(?:\[[^\]]*\])*\s*(?:=|;)"
    )
    function = re.compile(
        r"^(?!extern\b)(?:static\s+)?\w+(?:\s+\w+)*[\s*]*\b" + re.escape(name) + r"\([^;]*$"
    )
    for i, line in enumerate(lines):
        if pattern.match(line) or function.match(line):
            return i
    return None


def prune(text):
    lines = text.split("\n")
    externs = {}
    for i, line in enumerate(lines):
        m = EXTERN.match(line) or PROTOTYPE.match(line)
        if m:
            externs.setdefault(m.group(1), []).append(i)
    if not externs:
        return text

    # Where each name is first used, other than in an extern of it or its definition
    first_use = {}
    definitions = {name: definition_line(lines, name) for name in externs}
    extern_lines = {i for idx in externs.values() for i in idx}
    for i, line in enumerate(lines):
        if i in extern_lines:
            continue
        for tok in tokens(line):
            if tok in externs and tok not in first_use and definitions[tok] != i:
                first_use[tok] = i

    drop = set()
    for name, idx in externs.items():
        definition = definitions[name]
        if definition is None:
            # Defined elsewhere: keep the first declaration
            drop.update(idx[1:])
            continue
        use = first_use.get(name)
        if use is None or use > definition:
            drop.update(idx)
        else:
            drop.update(idx[1:])

    out = []
    for i, line in enumerate(lines):
        if i in drop:
            continue
        # A blank line left after a block of dropped externs goes with it
        if line == "" and out and out[-1] == "" and i - 1 in drop:
            continue
        out.append(line)
    return "\n".join(out)


def declared(line):
    """The name a declaration line declares, or None"""
    m = EXTERN.match(line) or PROTOTYPE.match(line)
    return m.group(1) if m else None


def gather(lines):
    """Moves the declarations before the first function down into one block"""
    first = next((i for i, line in enumerate(lines) if FUNCTION.match(line)), len(lines))
    head = [i for i in range(first) if declared(lines[i])]
    if len(head) < 2:
        return lines
    last = head[-1]
    if head == list(range(head[0], last + 1)):
        return lines
    moved = [lines[i] for i in head]
    out = []
    for i, line in enumerate(lines):
        if i in head[:-1]:
            continue
        out.extend(moved if i == last else [line])
    return out


def item_kind(lines, i):
    """'code' if line i is in a function, 'data' if in a table"""
    for j in range(i, -1, -1):
        line = lines[j]
        if FUNCTION.match(line):
            return "code"
        if line and not line[0].isspace() and line[0] not in "}#/" and not line.startswith(" *"):
            return "data"
    return "data"


def why(lines, names):
    """The comment for a block of declarations of names"""
    included = []
    for name in names:
        if definition_line(lines, name) is None:
            for line in lines:
                m = INCLUDE.match(line)
                if m and name in tokens(line):
                    included.append(m.group(1))
                    break
    if included and len(included) == len(names):
        return f"/* Defined in {included[0]}, included below */" if len(set(included)) == 1 else None
    kinds = set()
    for name in names:
        for i, line in enumerate(lines):
            if declared(line) or definition_line(lines, name) == i:
                continue
            if name in tokens(line):
                kinds.add("data" if INCLUDE.match(line) is None and item_kind(lines, i) == "data" else "code")
                break
    if kinds == {"code"}:
        return "/* Defined below, after the code that uses them */"
    if kinds == {"data"}:
        return "/* Defined below, after the tables that use them */"
    return "/* Defined below, after their first use */"


def comment(text):
    """Writes the comment above each block of declarations"""
    lines = [line for line in text.split("\n") if not COMMENT.match(line)]
    lines = gather(lines)
    out = []
    i = 0
    while i < len(lines):
        if declared(lines[i]):
            j = i
            while j < len(lines) and declared(lines[j]):
                j += 1
            note = why(lines, [declared(line) for line in lines[i:j]])
            if note:
                if out and out[-1] != "" and not out[-1].startswith("#include"):
                    out.append("")
                out.append(note)
            out.extend(lines[i:j])
            i = j
        else:
            out.append(lines[i])
            i += 1
    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    parser.add_argument("files", nargs="*")
    args = parser.parse_args()
    files = args.files or sorted(glob.glob("src/field/stages/wstag*.c") + glob.glob("src/field/stages/*/wstag*.c"))
    changed = 0
    for path in files:
        with open(path) as f:
            text = f.read()
        new = comment(prune(text))
        if new != text:
            with open(path, "w") as f:
                f.write(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
