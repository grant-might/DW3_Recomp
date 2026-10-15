#!/usr/bin/env python3
"""
Writes the sizes the stages give createTask and createTaskWithId as sizeof.

createTask(update, size, childrenSize) takes the size of the task and of the
block of its children. Where the update's first parameter is a pointer to a
type of include/field/stage.h (or include/field/field_map.h) of that size, the size
becomes sizeof(Type); and where its second parameter is a pointer to such a
type of the children's size, so does that one. The sizes come from compiling
a probe of the headers with the game's compiler (make's CROSS, CC1). A
number that is not the size of the type stays as it is. It changes no bytes,
and can be run again at any time:

    tools/stage_task_sizes.py [src/field/stages/central_sector/wstag200.c ...]
"""
import argparse
import glob
import os
import re
import subprocess
import sys
import tempfile

from name_stage_funcs import functions

HEADERS = ("include/field/stage.h", "include/field/field_map.h")
CREATE = re.compile(r"\b(createTask(?:WithId)?\()(\w+), (0x[0-9A-Fa-f]+|\d+), (0x[0-9A-Fa-f]+|\d+)")
INCLUDE = re.compile(r'^#include "(common/\w+\.inc\.c)"', re.M)
POINTER = re.compile(r"^\s*(?:const\s+)?(?:struct\s+)?(\w+)\s*\*\s*\w*\s*$")


def type_sizes():
    """{type: its size} of the headers' typedef'd structs, from the compiler,
    where every version has the same"""
    us, eu = version_sizes("US"), version_sizes("EU")
    return {t: n for t, n in us.items() if eu.get(t) == n}


def version_sizes(version):
    names = set()
    for path in HEADERS:
        names |= set(re.findall(r"^\} (\w+);", open(path).read(), re.M))
    names = sorted(names)
    probe = '#include "common.h"\n#include "stage.h"\n'
    probe += "".join(f"int size_{n} = sizeof({n});\n" for n in names)
    toolchain = os.environ.get("CROSS", "mipsel-linux-gnu-")
    cc1 = os.environ.get("CC1", "bin/gcc-2.8.1-psx/cc1")
    with tempfile.TemporaryDirectory() as tmp:
        c, i, s = (os.path.join(tmp, "probe" + ext) for ext in (".c", ".i", ".s"))
        with open(c, "w") as f:
            f.write(probe)
        subprocess.run([toolchain + "cpp", "-Iinclude", "-Iexternal/psyq_headers/psyq_lib47/include", "-undef",
                        "-nostdinc", "-D__GNUC__=2", "-D__GNUC_MINOR__=8", "-Dmips", "-D_LANGUAGE_C", "-DLANGUAGE_C",
                        f"-DVERSION_{version}", f"-DASM_DIR=\"asm/{version.lower()}\"", c, "-o", i], check=True, capture_output=True)
        subprocess.run([cc1, "-quiet", "-O2", "-G0", "-mips1", "-mgas", "-o", s, i], check=True, capture_output=True)
        text = open(s).read()
    out = {}
    for m in re.finditer(r"^size_(\w+):\s*\n\s*\.word\s+(\d+)", text, re.M):
        out[m.group(1)] = int(m.group(2))
    return out


def parameter_types(text):
    """{function: [the type each pointer parameter points to, or None]}, with
    the functions of the src/field/stages/common files the text includes"""
    out = {}
    sources = [text] + [open(f"src/field/stages/{m.group(1)}").read() for m in INCLUDE.finditer(text)]
    for source in sources:
        for name, (params, _) in functions(source).items():
            types = []
            for param in params.split(","):
                m = POINTER.match(param)
                types.append(m.group(1) if m else None)
            out[name] = types
    return out


def rewrite(text, sizes):
    params = parameter_types(text)

    def size(m):
        types = params.get(m.group(2))
        if not types:
            return m.group(0)
        args = [m.group(3), m.group(4)]
        for k in range(2):
            t = types[k] if k < len(types) else None
            if t in sizes and sizes[t] == int(args[k], 0):
                args[k] = f"sizeof({t})"
        return f"{m.group(1)}{m.group(2)}, {args[0]}, {args[1]}"

    return CREATE.sub(size, text)


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    parser.add_argument("files", nargs="*")
    args = parser.parse_args()
    files = args.files or sorted(glob.glob("src/field/stages/wstag*.c") + glob.glob("src/field/stages/*/wstag*.c"))
    sizes = type_sizes()
    changed = 0
    for path in files:
        text = open(path).read()
        new = rewrite(text, sizes)
        if new != text:
            open(path, "w").write(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
