#!/usr/bin/env python3
"""Writes the folders' descriptions for doxygen (make docs), from the tables
of src/README.md and src/field/stages/README.md.

    tools/doxygen_dirs.py OUT.dox

Each binary's folder, in src/ and include/, gets its file on the disc and
what it is (STITSHOP.PRO: the item shop), each group (src/menus/...) its
binaries' files, and each stage area its name and its number of stages.
"""
import os
import re
import sys

from doxygen_filter import escape

ROW = re.compile(r"^\| `([\w/]+)/` \|(.*)\|\s*$")
# a link, such as "([the list](field/stages/README.md))", is relative to its README
LINK = re.compile(r"\s*\(?\[[^]]*\]\([^)]*\)\)?")


def rows(path):
    with open(path, encoding="utf-8") as f:
        for line in f:
            m = ROW.match(line)
            if m:
                yield m.group(1), [cell.strip() for cell in m.group(2).split("|")]


def entry(folder, brief):
    return f"/** \\dir {folder}\n    \\brief {escape(LINK.sub('', brief))} */\n"


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__.split("\n\n")[1])
    out = []
    groups = {}
    for folder, (disc, what) in rows("src/README.md"):
        brief = f"{disc}: {what}" if disc else what
        for root in ("src", "include"):
            if os.path.isdir(f"{root}/{folder}"):
                out.append(entry(f"{root}/{folder}", brief))
        group = folder.split("/")[0]
        if group != folder:
            groups.setdefault(group, []).extend(d.strip() for d in disc.split(","))
    for group, discs in groups.items():
        for root in ("src", "include"):
            out.append(entry(f"{root}/{group}", ", ".join(discs)))
    for folder, (area, count, _) in rows("src/field/stages/README.md"):
        out.append(entry(f"src/field/stages/{folder}", f"{area}: {count} stages"))
    with open(sys.argv[1], "w", encoding="utf-8") as f:
        f.writelines(out)


if __name__ == "__main__":
    main()
