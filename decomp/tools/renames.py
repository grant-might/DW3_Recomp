#!/usr/bin/env python3
"""Apply lists of renames, again and again without harm.

    tools/renames.py LIST... [-n]

A list (tools/renames/<area>.txt) has a rename a line, OLD NEW, in the
order they were made; # starts a comment. Each one changes OLD to NEW as a
whole word in the files tools/rename.py changes (every version's symbol
files, src/ and include/) and in the docs (*.md, docs/*.md). A rename whose
OLD is gone is already done and changes nothing, so a branch that still has
the old names (or gets new code with them in a rebase) runs the lists again
to catch up:

    python3 tools/renames.py tools/renames/fieldstg.txt

A field of a global struct is renamed the same way, written with its
global, so that only that global's uses change (FIELDSTG_map.unk50
FIELDSTG_map.setFirstMap); the struct's declaration is changed by hand.

Unlike tools/rename.py, NEW may already exist (the part of the branch that
was renamed before) and an overlay's prefix is taken as written, so that
an overlay's datum with splat's name (D_8009A70C) can get the overlay's
prefix. -n only lists what would change. Afterwards, splat has to write the
assembly again with the new names: make VERSION=<version> regenerate, for
each version.
"""

import argparse
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rename import IDENT, ROOT, files, word  # noqa: E402


def is_name(text):
    """a C identifier, or a global's field (FIELDSTG_map.unk50)"""
    return all(IDENT.match(part) for part in text.split("."))


def read_list(path):
    """[(old, new)] of a list, in its order"""
    pairs = []
    for n, line in enumerate(Path(path).read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        if len(parts) != 2 or not all(is_name(p) for p in parts):
            sys.exit(f"{path}:{n}: expected OLD NEW, two C identifiers or two fields")
        old, new = (p.rpartition(".")[0] for p in parts)
        if old != new:
            sys.exit(f"{path}:{n}: a field keeps its global ({parts[0]}, {parts[1]})")
        pairs.append((parts[0], parts[1]))
    return pairs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("lists", nargs="+")
    ap.add_argument("-n", "--dry-run", action="store_true", help="only list what would change")
    args = ap.parse_args()
    pairs = [p for path in args.lists for p in read_list(path)]
    olds = Counter(old for old, _ in pairs)
    twice = [old for old, n in olds.items() if n > 1]
    if twice:
        sys.exit("renamed twice: " + ", ".join(twice))

    paths = files() + sorted(ROOT.glob("*.md")) + sorted((ROOT / "docs").glob("*.md"))
    texts = {p: p.read_text(errors="surrogateescape") for p in paths}
    changed = Counter()
    done = 0
    for old, new in pairs:
        old_re = word(old)
        hit = False
        for p, text in texts.items():
            text, n = old_re.subn(new, text)
            if n:
                texts[p] = text
                changed[p] += n
                hit = True
        done += hit
    for p, n in sorted(changed.items()):
        print(f"{p.relative_to(ROOT)}: {n}")
        if not args.dry_run:
            p.write_text(texts[p], errors="surrogateescape")
    print(f"{done} of {len(pairs)} renames changed something"
          + ("" if args.dry_run or not done else "; now make VERSION=<version> regenerate for each version"))


if __name__ == "__main__":
    main()
