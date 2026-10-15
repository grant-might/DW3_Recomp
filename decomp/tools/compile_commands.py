#!/usr/bin/env python3
"""Write compile_commands.json for clangd and other C tooling.

    make [VERSION=us] compile_commands

One entry per C file the version builds (CC_SRC from the Makefile, its
ALL_C_SRC), with the build's own preprocessor flags (CC_CPPFLAGS, its
CPPFLAGS: the include paths, -nostdinc and the defines, VERSION_<VERSION>
among them), so that the editor sees the code the build compiles and never
drifts from it. To those it adds what a modern clang needs to parse GCC
2.8.1's C as the build does: a 32-bit MIPS target, GCC 2.8.1's dialect
(gnu89, old-style definitions included) and the game's -fno-builtin and
-fsigned-char. Nothing here reaches the build.

The file goes to the top of the repository, where clangd looks for it, and
is for one version at a time: running it again for another version replaces
it. A header gets the flags of a C file that includes it. An .inc.c file
isn't compiled on its own (the stages' common/ and src/shared/ code), so it
has no entry: the macros its including file defines before the include
(OVL_NAME, FADER_DEPTH...) are missing there.
"""
import json
import os
import shlex
import sys

import version

# what tools/lint.py gives a modern compiler too, and the target: s32 and
# pointers are 32 bits wide, as on the PlayStation
CLANG_FLAGS = ["--target=mipsel-linux-gnu", "-std=gnu89", "-fno-builtin", "-fsigned-char"]


def main() -> None:
    cppflags = shlex.split(os.environ.get("CC_CPPFLAGS", ""))
    files = sorted(set(os.environ.get("CC_SRC", "").split()))
    if not cppflags or not files:
        sys.exit("no files or flags: run it as make VERSION=%s compile_commands" % version.VERSION)
    root = str(version.ROOT)
    entries = [{
        "directory": root,
        "file": path,
        "arguments": ["clang"] + CLANG_FLAGS + cppflags + ["-c", path,
                      "-o", "build/%s/%s.o" % (version.VERSION, path)],
    } for path in files]
    out = version.ROOT / "compile_commands.json"
    with open(out, "w") as f:
        json.dump(entries, f, indent=1)
        f.write("\n")
    print("wrote %s: %s, %d files" % (out.name, version.VERSION, len(entries)))


if __name__ == "__main__":
    main()
