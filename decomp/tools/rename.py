#!/usr/bin/env python3
"""Rename a symbol in every version at once.

    tools/rename.py OLD NEW [-n]

The versions share their names: a function or datum has the same name in us
and eu (tools/match_versions.py gave eu the us names, and
tools/check_names.py checks they stay the same). So a rename changes OLD to
NEW, as a whole word, in

- every version's symbols and undefined symbols: config/<version>/*.txt
  (not stages.txt) and config/<version>/stages/*.txt;
- the C, headers and assembly under src/ and include/.

An overlay's names carry its prefix, its name in capitals (CNTY_SEL_ for
CNTY_SEL, STDWTITL_ for STDWTITL). When OLD has an overlay's prefix, NEW
gets the same one if it doesn't have it; a NEW with another overlay's
prefix is refused, and so is an overlay's prefix for a name OLD doesn't have
it in (an executable symbol, or one of the SDK's in an overlay, such as
STDWTITL's DecDCTReset). So is a NEW that already exists anywhere, or an OLD
that doesn't.

-n only lists what would change. Afterwards, splat has to write the
assembly again with the new name: make VERSION=<version> regenerate, for
each version.
"""

import argparse
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config"
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
# the files symbols are written in: splat's symbol files and the
# hand-written undefined symbols (not stages.txt, the stages' offsets)
CONFIG_FILES = ("symbols*.txt", "undefined_syms*.txt", "stages/*.txt")
SOURCE_SUFFIXES = {".c", ".h", ".s", ".inc"}


def word(name):
    return re.compile(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"(?![A-Za-z0-9_])")


def prefixes():
    """{prefix: overlay} for every overlay of every version (the stages have
    no prefix)."""
    return {y.stem.upper() + "_": y.stem for y in CONFIG.glob("*/*.yaml") if y.stem != "main"}


def files():
    """Every file a symbol's name can be in."""
    out = []
    for version in sorted(p for p in CONFIG.iterdir() if p.is_dir()):
        for pattern in CONFIG_FILES:
            out += sorted(version.glob(pattern))
    for top in ("src", "include"):
        out += sorted(p for p in (ROOT / top).rglob("*") if p.is_file() and p.suffix in SOURCE_SUFFIXES)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("old")
    ap.add_argument("new")
    ap.add_argument("-n", "--dry-run", action="store_true", help="only list what would change")
    args = ap.parse_args()
    old, new = args.old, args.new
    for name in (old, new):
        if not IDENT.match(name):
            sys.exit(f"{name} is not a C identifier")

    # the longest prefix first: CNTY_SEL_ before a shorter one it starts with
    known = sorted(prefixes(), key=len, reverse=True)
    paths = files()
    old_re = word(old)
    found = {p: p.read_text(errors="surrogateescape") for p in paths}
    hits = [p for p, text in found.items() if old_re.search(text)]
    if not hits:
        sys.exit(f"{old} is in no symbol file, nor in src/ or include/")

    # NEW keeps OLD's prefix, and only OLD's
    want = next((p for p in known if old.startswith(p)), None)
    new_prefix = next((p for p in known if new.startswith(p)), None)
    if want:
        if new_prefix and new_prefix != want:
            sys.exit(f"{old} is {want[:-1]}'s, but {new} has {new_prefix[:-1]}'s prefix")
        if not new_prefix:
            print(f"{new} -> {want}{new}: {old} is an overlay's symbol, so it keeps the prefix")
            new = want + new
    elif new_prefix:
        sys.exit(f"{old} has no overlay's prefix, but {new} has {new_prefix[:-1]}'s")

    new_re = word(new)
    clash = [p for p, text in found.items() if new_re.search(text)]
    if clash:
        sys.exit(f"{new} already exists: " + ", ".join(str(p.relative_to(ROOT)) for p in clash[:5]))

    counts = Counter()
    for p in hits:
        text, n = old_re.subn(new, found[p])
        counts[p] = n
        if not args.dry_run:
            p.write_text(text, errors="surrogateescape")
    for p, n in counts.items():
        print(f"{p.relative_to(ROOT)}: {n}")
    versions = sorted({p.relative_to(CONFIG).parts[0] for p in hits if CONFIG in p.parents})
    if not args.dry_run:
        print(f"{old} -> {new}; now make VERSION=<version> regenerate for each version"
              + (f" ({', '.join(versions)} name it)" if versions else ""))


if __name__ == "__main__":
    main()
