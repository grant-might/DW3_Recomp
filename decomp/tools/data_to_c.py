#!/usr/bin/env python3
"""Turn a splat data file (.word lines) into C definitions.

usage: data_to_c.py [--sizes | --bss] asm/<version>/<ovl>/data/<ovl>.data.s > data.c

Each data symbol becomes an s32 (one word) or an s32 array; words that splat
wrote as a symbol become that symbol's address. Symbols of .short or .byte
become u16 or u8 arrays, and symbols that mix sizes u8 arrays of their bytes.
GCC places each object at the next multiple of its alignment (a scalar's size;
a word for any array, as MIPS word-aligns them), so an object must land on its
symbol's address: `data_to_c.py --sizes FILE` prints the symbol sizes (for the
splat symbol file) that merge the pieces that wouldn't into the object before. The declarations those need
come first. This is only a starting point that reproduces the bytes: give the
data real types once the code that uses it is understood.
"""

import re
import sys

VALUE = re.compile(r"^\s+/\* [0-9A-F]+ ([0-9A-F]{8})(?: [0-9A-F]{8})? \*/\s+\.(word|short|byte)\s+(\S+(?: [+-] \S+)?)")
LABEL = re.compile(r"^dlabel (\w+)")
END = re.compile(r"^enddlabel (\w+)")
STRING = re.compile(r"^\s+/\* [0-9A-F]+ [0-9A-F]{8} \*/\s+\.(ascii|asciz)\s")
RAW = re.compile(r"^\s+/\* ([0-9A-F]+) \*/\s*$")  # the bytes of the string above
OTHER = re.compile(r"^\s+/\* .*\*/\s+\.(half|space)\b")
TYPES = {"word": "s32", "short": "u16", "byte": "u8"}
SIZES = {"word": 4, "short": 2, "byte": 1}


def size(kind, value):
    return len(value) // 2 if kind == "raw" else SIZES[kind]


def parse(path):
    """[(name, address, [(kind, value)])] of the data file."""
    symbols = []
    string = False
    closed = True  # data after an enddlabel belongs to no symbol: name it
    for line in open(path):
        if m := LABEL.match(line):
            symbols.append([m.group(1), None, []])
            closed = False
            continue
        if END.match(line):
            closed = True
            continue
        if closed and (m := VALUE.match(line) or (STRING.match(line) and re.match(r"^\s+/\* [0-9A-F]+ ([0-9A-F]{8})", line))):
            symbols.append([f"D_{m.group(1)}", None, []])
            closed = False
        if STRING.match(line):
            # splat's string guesser: take the string's bytes (with the
            # padding up to the next word) from the line after it
            string = True
            if symbols[-1][1] is None:
                symbols[-1][1] = int(line.split()[2], 16)
        elif string and (m := RAW.match(line)):
            symbols[-1][2].append(("raw", m.group(1)))
            string = False
        elif m := VALUE.match(line):
            if symbols[-1][1] is None:
                symbols[-1][1] = int(m.group(1), 16)
            symbols[-1][2].append((m.group(2), m.group(3)))
        elif OTHER.match(line):
            sys.exit(f"unsupported data: {line.strip()}")
    return symbols


def objsize(values):
    return sum(size(k, v) for k, v in values)


def align(values):
    """Alignment GCC gives the C object: a scalar its size, an array (or the
    byte array of mixed data) a word, since MIPS word-aligns every array."""
    if len(values) == 1 and values[0][0] != "raw":
        return SIZES[values[0][0]]
    return 4


def misplaced(symbols):
    """Indexes of the symbols whose C object would not land on their address
    after the object before them."""
    bad, cur = [], None
    for i, (name, addr, values) in enumerate(symbols):
        a = align(values)
        placed = addr if cur is None else (cur + a - 1) // a * a
        if placed != addr:
            bad.append(i)
        cur = addr + objsize(values)
    return bad


def sizes(symbols):
    """Symbols that must be one object with the ones after them for every C
    object to land on its address. Printed as splat symbol_addrs lines; splat
    then writes BASE+offset for the rest."""
    groups = [[s] for s in symbols]
    changed = True
    while changed:
        changed = False
        flat = [[g[0][0], g[0][1], [v for s in g for v in s[2]]] for g in groups]
        for i in misplaced(flat):
            if i:
                groups[i - 1] += groups.pop(i)
                changed = True
                break
    for g in groups:
        if len(g) > 1:
            name, addr = g[0][0], g[0][1]
            end = g[-1][1] + objsize(g[-1][2])
            print(f"{name} = 0x{addr:08X}; // size:0x{end - addr:X}")


def main():
    symbols = parse(sys.argv[-1])
    if "--sizes" in sys.argv:
        return sizes(symbols)
    if "--bss" in sys.argv:
        # uninitialized definitions, in order (build with -fno-common)
        if bad := misplaced(symbols):
            sys.exit(f"{symbols[bad[0]][0]} would not land on its address: run with --sizes")
        for name, _, values in symbols:
            kinds = {k for k, _ in values}
            t = TYPES[kinds.pop()] if len(kinds) == 1 and "raw" not in kinds else "u8"
            n = objsize(values) // (4 if t == "s32" else 2 if t == "u16" else 1)
            print(f"{t} {name};" if len(values) == 1 and t != "u8" or (t == "u8" and objsize(values) == 1) else f"{t} {name}[{n}];")
        return
    if bad := misplaced(symbols):
        sys.exit(f"{symbols[bad[0]][0]} would not land on its address: run with --sizes")
    kinds = {}
    for name, addr, values in symbols:
        kinds[name] = values[0][0] if len({k for k, _ in values}) == 1 else "bytes"
        if kinds[name] == "raw":
            kinds[name] = "bytes"
    scalars = {name for name, _, values in symbols if len(values) == 1 and kinds[name] != "bytes"}

    def value(v):
        if re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", v):
            n = int(v, 16) if "0x" in v else int(v)
            n &= 0xFFFFFFFF
            return str(n - (1 << 32) if n & 0x80000000 else n) if n < 0x10000 or n > 0xFFFF0000 else f"0x{n:X}"
        sym, _, off = v.partition(" ")
        addr = f"&{sym}" if sym in scalars else sym
        return f"(s32){addr}" + (f" {off}" if off else "")

    out, decls = [], []
    for name, _, values in symbols:
        kind = kinds[name]
        if kind == "bytes":
            # mixed sizes: its bytes, little-endian (only plain numbers can be split)
            data = b""
            for k, v in values:
                if k == "raw":
                    data += bytes.fromhex(v)
                    continue
                if not re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", v):
                    sys.exit(f"{name} mixes sizes and holds {v}: give it a real type by hand")
                data += (int(v, 0) & ((1 << (8 * SIZES[k])) - 1)).to_bytes(SIZES[k], "little")
            vals = [f"0x{b:02X}" for b in data]
            rows = [", ".join(vals[i:i + 8]) + "," for i in range(0, len(vals), 8)]
            out.append(f"u8 {name}[] = {{\n" + "\n".join("    " + r for r in rows) + "\n};")
            continue
        vs = [v for _, v in values]
        for v in vs:
            if not re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", v):
                v = v.split(" ")[0]
                t = TYPES.get(kinds.get(v), "u8") if v in kinds else "s32"
                d = f"void {v}();" if v.startswith("func_") else (
                    f"extern {t} {v};" if v in scalars else f"extern {t} {v}[];")
                if d not in decls:
                    decls.append(d)
        t = TYPES[kind]
        if len(vs) == 1:
            out.append(f"{t} {name} = {value(vs[0])};")
        else:
            vals = [value(v) if kind == "word" else v for v in vs]
            n = 4 if kind == "word" else 8
            rows = [", ".join(vals[i:i + n]) + "," for i in range(0, len(vals), n)]
            out.append(f"{t} {name}[] = {{\n" + "\n".join("    " + r for r in rows) + "\n};")
    print("\n".join(decls))
    if decls:
        print()
    print("\n".join(out))


if __name__ == "__main__":
    main()
