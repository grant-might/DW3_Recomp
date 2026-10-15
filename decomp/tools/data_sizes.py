#!/usr/bin/env python3
"""Give the global data symbols of a compiled object their ELF size.

GCC's MIPS output labels data (`D_800100E4:` followed by `.ascii`/`.word`...)
without `.size`, so the symbol has size 0 and objdiff stretches it up to the
next symbol, swallowing anonymous data such as a switch's jump table (`$L65:`,
`.word $L60`...) that follows it: memcard/memcard.c's save file names then compared
against the names plus two jump tables. This ends each global data label with
`.size X, . - X` where its data ends: at the next label, `.align` or section
change. The bytes of the object don't change, only its symbol table.

usage: data_sizes.py < in.s > out.s
"""

import re
import sys

LABEL = re.compile(r"^([A-Za-z_$.][\w$.]*):\s*$")
DATA_SECTIONS = (".rodata", ".rdata", ".data", ".sdata", ".bss", ".sbss")


def main():
    lines = sys.stdin.read().split("\n")
    if lines[-1] == "":
        lines.pop()  # the newline that ends the input, given back at the end
    out = []
    section = ".text"
    globl = set()
    open_sym = None  # data label whose size is still pending

    def close():
        nonlocal open_sym
        if open_sym is not None:
            out.append(f"\t.size\t{open_sym}, . - {open_sym}")
            open_sym = None

    for line in lines:
        code = line.split("#", 1)[0].strip()
        words = code.split()
        if words:
            head = words[0]
            if head in (".section", ".text", ".data", ".rdata", ".bss", ".sdata", ".sbss"):
                close()
                section = words[1].rstrip(",") if head == ".section" else head
            elif head == ".align":
                close()
            elif head == ".globl" and len(words) > 1:
                globl.add(words[1])
            elif LABEL.match(code):
                close()
                name = LABEL.match(code).group(1)
                if section.startswith(DATA_SECTIONS) and name in globl:
                    open_sym = name
        out.append(line)
    close()
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
