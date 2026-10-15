#!/usr/bin/env python3
"""
Includes the stages' shared code from src/field/stages/common/ in place of copies of it.

A stage function written exactly as a src/field/stages/common/*.inc.c writes its
code (the file without its leading comment) becomes an #include of that
file, where the copy was. It changes no bytes, and can be run again at any
time:

    tools/stage_common.py [src/field/stages/central_sector/wstag200.c ...]

With no files it goes through every stage, src/field/stages/<area>/wstag###.c
(and src/field/stages/wstag260.c).
"""
import argparse
import glob
import os
import re
import sys

COMMON = "src/field/stages/common"


def shared_code():
    """{include line: code} of the files in src/field/stages/common/"""
    out = {}
    for path in sorted(glob.glob(f"{COMMON}/*.inc.c")):
        text = open(path).read()
        code = re.sub(r"\A/\*.*?\*/\n", "", text, flags=re.S).strip("\n")
        out[f'#include "common/{os.path.basename(path)}"'] = code
    return out


COMMENT_BEFORE = re.compile(r"\n/\*(?:[^*]|\*(?!/))*\*/\n\Z")


def include_shared(text, shared):
    for include, code in shared.items():
        start = text.find("\n" + code + "\n")
        if start < 0 or include in text:
            continue
        end = start + 1 + len(code)
        comment = COMMENT_BEFORE.search(text[: start + 1])
        if comment:
            start = comment.start()
        text = text[: start + 1] + include + text[end:]
    return text


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    parser.add_argument("files", nargs="*")
    args = parser.parse_args()
    files = args.files or sorted(glob.glob("src/field/stages/wstag*.c") + glob.glob("src/field/stages/*/wstag*.c"))
    shared = shared_code()
    changed = 0
    for path in files:
        text = open(path).read()
        new = include_shared(text, shared)
        if new != text:
            open(path, "w").write(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
