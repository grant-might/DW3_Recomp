#!/usr/bin/env python3
"""Write what a binary links with from the binaries that load after it.

    link_imports.py OUT OBJ... --from SYMS... [--linked LD...]
    link_imports.py --placeholders OUT OBJ...

A binary that points into binaries that load after it, its children (the
Makefile's CHILDREN_<name>: the executable's mode overlays, FIGHTSTG's
WFIGHTMN and WFIGHTTS), links against their symbols, which each child's
build/<version>/<child>_syms.ld lists. OUT takes the names that the objects
use and don't define, and that nothing else the binary links with (--linked:
the executable's and the parent's symbols, the hand-written and splat's)
defines, so that a child's symbol never replaces one the binary has. A name
that two children define with different addresses is an error: the binary
would point into whichever one the link took.

An overlay takes the rest of what it uses from splat's symbol files
(undefined_{syms,funcs}_auto_<name>.txt) the same way: --from them, --linked
with the executable's and the parent's symbols, so that OUT keeps only the
names those don't define. splat's files have the addresses of the unpadded
build, which must never replace the executable's or the parent's in a
padding build, whatever the order of the linker scripts.

The children link against the binary's own symbols, so those come from a
link without the children's: a binary's addresses don't depend on the values
of what it imports, as MIPS links don't relax. --placeholders writes that
layout link's PROVIDE of every name the objects use: a placeholder in the
same 256 MB as the code, which any jal reaches.
"""
import argparse
import re
import sys

from elftools.elf.elffile import ELFFile

PLACEHOLDER = 0x80000000
ASSIGNMENT = re.compile(r"^\s*(\w+)\s*=\s*([^;]+);", re.M)


def object_names(paths):
    """The global names the objects use and those they define"""
    used, defined = set(), set()
    for path in paths:
        with open(path, "rb") as f:
            symtab = ELFFile(f).get_section_by_name(".symtab")
            for s in symtab.iter_symbols() if symtab else ():
                if not s.name or s["st_info"]["bind"] == "STB_LOCAL":
                    continue
                (used if s["st_shndx"] == "SHN_UNDEF" else defined).add(s.name)
    return used - defined, defined


def assigned(path):
    """The names a linker script assigns, with what it assigns them"""
    text = re.sub(r"/\*.*?\*/", "", open(path).read(), flags=re.S)
    return {n: v.strip() for n, v in ASSIGNMENT.findall(text)}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("out")
    ap.add_argument("objects", nargs="+")
    ap.add_argument("--from", dest="children", nargs="*", default=[], help="the children's symbols")
    ap.add_argument("--linked", nargs="*", default=[], help="the binary's other linker scripts")
    ap.add_argument("--placeholders", action="store_true", help="write the layout link's placeholders")
    args = ap.parse_args()
    used, _ = object_names(args.objects)
    if args.placeholders:
        lines = ["PROVIDE(%s = 0x%08X);" % (n, PLACEHOLDER) for n in sorted(used)]
    else:
        for path in args.linked:
            used -= set(assigned(path))
        found = {}
        for path in args.children:
            for n, v in assigned(path).items():
                if n in used:
                    found.setdefault(n, {}).setdefault(v, path)
        clashes = {n: d for n, d in found.items() if len(d) > 1}
        for n, d in sorted(clashes.items()):
            print("%s: %s is in %s" % (args.out, n, ", ".join("%s (%s)" % (p, v) for v, p in d.items())),
                  file=sys.stderr)
        if clashes:
            sys.exit("%s: rename the children's symbols the binary uses" % args.out)
        lines = ["%s = %s; /* %s */" % (n, v, p) for n in sorted(found) for v, p in found[n].items()]
    with open(args.out, "w") as f:
        f.write("".join(line + "\n" for line in lines))


if __name__ == "__main__":
    main()
