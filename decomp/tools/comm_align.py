#!/usr/bin/env python3
"""Give the common symbols of a compiled object the alignment of a word.

maspsx writes a static variable of a -G8 unit as `.comm X,size` (Makefile:
SDATA_LIMIT), so that it resolves to its definition in src/engine/data, and
leaves its alignment to the assembler, which takes it from the size: 8 for
system/main.c's 8-byte BOOT_IMAGE_RECT. The definitions are only word-aligned,
as the original linker laid them (the European BOOT_IMAGE_RECT is at
0x8005CCB4), and ld warns about the difference. This writes
`.comm X,size,4` for a common larger than a word; the bytes of the object
don't change, only its symbol table.

usage: comm_align.py < in.s > out.s
"""

import re
import sys

COMM = re.compile(r"^(\s*\.comm\s+[\w$.]+\s*,\s*(\d+))\s*$")
WORD = 4


def main():
    for line in sys.stdin:
        line = line.rstrip("\n")
        m = COMM.match(line)
        if m and int(m.group(2)) > WORD:
            line = f"{m.group(1)},{WORD}"
        print(line)


if __name__ == "__main__":
    main()
