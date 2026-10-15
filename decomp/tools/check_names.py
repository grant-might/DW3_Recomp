#!/usr/bin/env python3
"""Check that every version names its symbols as us does.

The versions share their names: a name in eu's symbol files means the same
function or datum as that name in us's. The symbol files are the mapping
between versions: tools/match_versions.py --seed writes the us names of the
functions it pairs, and nothing else needs keeping. So a name of another
version must still be one of us's:

- in us's file of the same binary: config/us/symbols.txt for the
  executable, symbols_<overlay>.txt for an overlay, stages/<stage>.txt for
  a stage overlay;
- a function there if it is a function here (type:func), data if data.

A rename made in us only (or in one version only) leaves the old name
behind somewhere, and this fails; tools/rename.py renames in every version.
Not checked: splat's automatic names (func_80012345, D_80012345), names in a
binary us doesn't have (one of eu's own stage overlays), and a name that is
a version's own, with "version-only" in its comment:

    func_name = 0x80012345; // type:func version-only

A version that has, in one binary, code that us has in another names it with
us's name and "us-<binary>" in the comment ("us-main" for us's executable,
"us-cardgame" for an overlay, "us-wstag200" for a stage); that name is
checked against that binary's names in us instead:

    drawWindow = 0x800A5123; // type:func us-main
"""

import re
import sys
from collections import Counter
from pathlib import Path

from version import stage_entries

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config"
LINE = re.compile(r"^\s*([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(?://(.*))?$")
AUTO_NAME = re.compile(r"^(?:[A-Z0-9]+_)*(?:func|D|jtbl|jlabel)_[0-9A-F]{8}$")


def symbol_files(vdir):
    """[(binary, path)] of a version's symbol files: main for symbols.txt,
    the overlay for symbols_<overlay>.txt, the stage for stages/<stage>.txt."""
    out = []
    for path in sorted(vdir.glob("symbols*.txt")):
        m = re.match(r"symbols_(\w+)\.txt$", path.name)
        out.append((m[1] if m else "main", path))
    for path in sorted(vdir.glob("stages/*.txt")):
        out.append((path.stem, path))
    return out


def binaries(vdir):
    """The binaries a version builds: main, the overlays that have a splat
    config, and the stage overlays in stages.txt."""
    names = {p.stem for p in vdir.glob("*.yaml")}
    names.update(words[0].lower() for words in stage_entries(vdir.name))
    return names


def read(path):
    """[(line number, name, is a function, comment)] of a symbol file."""
    out = []
    for n, line in enumerate(path.read_text().splitlines(), 1):
        m = LINE.match(line)
        if m:
            comment = m[3] or ""
            out.append((n, m[1], "type:func" in comment.split(), comment))
    return out


def main():
    us_dir = CONFIG / "us"
    us_binaries = binaries(us_dir)
    # us's names by binary: {binary: {name: is a function}}
    us = {}
    for binary, path in symbol_files(us_dir):
        for _, name, func, _ in read(path):
            us.setdefault(binary, {})[name] = func
    where = {name: b for b, names in us.items() for name in names}

    errors = []
    checked = 0
    for vdir in sorted(p for p in CONFIG.iterdir() if p.is_dir() and p.name != "us"):
        seen = Counter()
        for binary, path in symbol_files(vdir):
            rel = path.relative_to(ROOT)
            for n, name, func, comment in read(path):
                seen[(binary, name)] += 1
                if seen[(binary, name)] == 2:
                    errors.append(f"{rel}:{n}: {name} is named twice")
                if binary not in us_binaries or AUTO_NAME.match(name) or "version-only" in comment.split():
                    continue
                checked += 1
                home = next((w[3:] for w in comment.split() if w.startswith("us-") and w[3:] in us_binaries),
                            binary)
                if name not in us.get(home, {}):
                    if name in where:
                        errors.append(f"{rel}:{n}: {name} is {where[name]}'s in us, not {home}'s")
                    else:
                        errors.append(f"{rel}:{n}: us has no {name} (renamed in one version only? "
                                      "tools/rename.py renames in all)")
                elif us[home][name] != func:
                    kind = "a function" if us[home][name] else "data"
                    errors.append(f"{rel}:{n}: {name} is {kind} in us")
    for e in errors:
        print(e)
    if errors:
        sys.exit(f"{len(errors)} names differ from us's")
    print(f"{checked} names of the other versions are us's")


if __name__ == "__main__":
    main()
