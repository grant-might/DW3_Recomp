#!/usr/bin/env python3
"""Finds the addresses that would not follow the code if it moved.

    shiftcheck.py [-v VERSION] [--all [--numbers]] [--update] [--strict] [--base REV] [ELF...]

A build is shiftable when every address in its code and data is a symbol:
then adding or removing bytes anywhere still links into a working game. The
build links every binary with --emit-relocs, which keeps a relocation on each
word the linker filled in (build/<version>/*.elf). Whatever holds an address
but has no relocation, or has one against an absolute symbol, stays put when
the code moves:

  word     a word of data that holds an address without an R_MIPS_32: a
           number in C, a `.word 0x8...` that splat didn't symbolize, a
           pointer in a u32 table. Only a value at a label (or at a string
           in a block of rodata) counts: the rest of the words in the range
           are numbers (sound ids, pixels), which --all --numbers lists
  lui      a lui of an address's upper half without an R_MIPS_HI16 (a
           lui/ori pair makes a number, not an address)
  jal      a j/jal to an address without an R_MIPS_26
  abs      a relocation against an absolute symbol in RAM that the binary
           doesn't get from the binaries it links with (the executable, its
           parent overlay, its children) or, for the executable, from where
           the overlays end (heap.ld): splat's undefined_syms_auto entries and the
           addresses in the symbol files, which the linker never moves.
           `own` when the address is inside the binary (a label that is
           missing), `other` when it is in another binary
  header   a word of the executable's header

RAM is 0x80010000-0x801FFF00: the kernel's below and the stack above are
fixed by the hardware. The memory map's addresses (the *_VRAM of
mk/version/<version>.mk, where the overlays load) are fixed by design.

config/<version>/shiftcheck.txt lists what is known and left to fix by
address: the binary, the address of the word or the instruction and the
address it holds, with the names after a #, which a rename doesn't make
stale. The script exits with 1 on anything it doesn't list, and warns about
what it lists that is fixed (fails, with --strict). --update rewrites it.
The list may only get shorter: --base fails on an entry that the list of the
git revision REV doesn't have (the CI's is the parent commit, which for a
pull request is the base branch's).
"""
import argparse
import bisect
import collections
import pathlib
import re
import subprocess
import sys

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

from version import overlay_parents, stage_entries

RAM_LO = 0x80010000
RAM_HI = 0x801FFF00  # the stack is at the top of the 2 MB

R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

SEG_MARK = re.compile(r"^(\w+?)_(TEXT|DATA|RODATA|BSS|VRAM|ROM)(_START|_END|_SIZE)?$")


def in_ram(value):
    return RAM_LO <= value < RAM_HI


class Binary:
    def __init__(self, path, imported, name):
        self.path = pathlib.Path(path)
        self.name = name
        f = open(path, "rb")
        self.elf = ELFFile(f)
        self.syms = []  # (addr, name) of the labels, for locations
        self.symtab = self.elf.get_section_by_name(".symtab")
        self.text = []  # [start, end) of the code
        marks = {}
        for s in self.symtab.iter_symbols():
            if SEG_MARK.match(s.name):
                marks[s.name] = s["st_value"]
            elif s.name and s["st_shndx"] != "SHN_ABS" and s["st_info"]["type"] != "STT_SECTION" \
                    and not s.name.endswith(".NON_MATCHING") and s.name != "gcc2_compiled.":
                self.syms.append((s["st_value"], s.name))
        for k, v in marks.items():
            m = SEG_MARK.match(k)
            if m.group(2) == "TEXT" and m.group(3) == "_START" and m.group(1) + "_TEXT_END" in marks:
                self.text.append((v, marks[m.group(1) + "_TEXT_END"]))
        self.syms.sort()
        self.imported = imported
        self.objects = self.read_map()

    def read_map(self):
        """The input sections' ranges from the linker's map"""
        out = []
        mp = self.path.with_suffix(".map")
        if not mp.exists():
            return out
        rx = re.compile(r"^ (\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (\S+)$")
        for line in mp.read_text(errors="replace").splitlines():
            m = rx.match(line)
            if m and int(m.group(3), 16):
                out.append((int(m.group(2), 16), int(m.group(2), 16) + int(m.group(3), 16), m.group(4), m.group(1)))
        out.sort()
        return out

    def where(self, addr):
        i = bisect.bisect_right(self.syms, (addr, "\x7f")) - 1
        sym = "%s+0x%X" % (self.syms[i][1], addr - self.syms[i][0]) if i >= 0 else "?"
        obj = "?"
        j = bisect.bisect_right(self.objects, (addr, 0xFFFFFFFF)) - 1
        if j >= 0 and self.objects[j][0] <= addr < self.objects[j][1]:
            obj = self.objects[j][2]
        return obj, sym

    def byte_at(self, addr):
        for sec in self.elf.iter_sections():
            if sec["sh_type"] == "SHT_PROGBITS" and sec["sh_addr"] <= addr < sec["sh_addr"] + sec["sh_size"]:
                return sec.data()[addr - sec["sh_addr"]]
        return None

    def section_at(self, addr):
        """The kind of input section at addr (.text, .rodata, ...), from the map"""
        j = bisect.bisect_right(self.objects, (addr, 0xFFFFFFFF)) - 1
        if j >= 0 and self.objects[j][0] <= addr < self.objects[j][1]:
            return self.objects[j][3]
        return None

    def is_text(self, addr):
        return any(a <= addr < b for a, b in self.text)

    def range(self):
        """The binary's own addresses"""
        lo, hi = None, None
        for sec in self.elf.iter_sections():
            if sec["sh_flags"] & 2 and sec["sh_addr"] >= RAM_LO and sec.name != ".tail":
                a, b = sec["sh_addr"], sec["sh_addr"] + sec["sh_size"]
                lo = a if lo is None else min(lo, a)
                hi = b if hi is None else max(hi, b)
        return lo, hi

    @staticmethod
    def first_use(data, base, o, reg):
        """The opcode of the first instruction from o that reads reg, following
        the j's; None when it's written over first, 0 when the function
        returns without using it"""
        jump = None
        for _ in range(32):
            if not 0 <= o <= len(data) - 4:
                return None
            w = int.from_bytes(data[o:o + 4], "little")
            op = w >> 26
            if (w >> 21) & 31 == reg and op not in (2, 3):
                return op
            if op and op not in (2, 3, 0x2B, 0x29, 0x28, 0x04, 0x05) and (w >> 16) & 31 == reg:
                return None  # written over
            if not op and (w >> 11) & 31 == reg and w:
                return None
            if jump is not None:
                if jump == "ret":
                    return 0
                o, jump = jump, None
                continue
            if op == 2:
                jump = (((w & 0x3FFFFFF) << 2) | (base & 0xF0000000)) - base
            elif w == 0x03E00008:  # jr ra
                jump = "ret"
            o += 4
        return None

    @classmethod
    def is_number(cls, data, base, off, reg):
        """Whether the lui at off makes a number, not an address: its first
        use is an ori (an address takes an addiu, a load or a store), or
        there is none. The compiler shares the ori after a j, and puts the
        lui in a branch's delay slot for the ori at its target"""
        starts = [off + 4]
        if off >= 4:
            w = int.from_bytes(data[off - 4:off], "little")
            if 4 <= w >> 26 <= 7 or w >> 26 == 1:
                starts.append(off + ((w & 0xFFFF) ^ 0x8000) * 4 - 0x20000)
        return any(cls.first_use(data, base, o, reg) in (0x0D, 0) for o in starts)

    def scan(self):
        lo, hi = self.range()
        found = []
        sym_of = {}
        for sec in self.elf.iter_sections():
            if not isinstance(sec, RelocationSection):
                continue
            for r in sec.iter_relocations():
                addr = r["r_offset"]  # in an executable, the address itself
                s = self.symtab.get_symbol(r["r_info_sym"])
                sym_of[addr] = (r["r_info_type"], s)
        for addr, (rtype, s) in sym_of.items():
            if s["st_shndx"] != "SHN_ABS" or not in_ram(s["st_value"]) or s.name in self.imported \
                    or s.name in LAYOUT:
                continue
            where = "own" if lo <= s["st_value"] < hi else "other"
            found.append(("abs", where, addr, (s.name, s["st_value"])))
        for sec in self.elf.iter_sections():
            if sec["sh_type"] != "SHT_PROGBITS" or not sec["sh_flags"] & 2:
                continue
            data = sec.data()
            base = sec["sh_addr"]
            kind = {".header": "header", ".tail": "tail"}.get(sec.name)
            for off in range(0, len(data) - 3, 4):
                addr = base + off
                w = int.from_bytes(data[off:off + 4], "little")
                rel = sym_of.get(addr, (None,))[0]
                if self.is_text(addr) and kind is None:
                    op = w >> 26
                    if op == 0x0F and rel is None:
                        hi16 = w & 0xFFFF
                        if (RAM_LO >> 16) <= hi16 <= (RAM_HI >> 16) and (w >> 21) & 31 == 0 \
                                and not self.is_number(data, base, off, (w >> 16) & 31):
                            found.append(("lui", None, addr, hi16 << 16))
                    elif op in (2, 3) and rel is None:
                        t = ((w & 0x3FFFFFF) << 2) | (addr & 0xF0000000)
                        if in_ram(t):
                            found.append(("jal", None, addr, t))
                    continue
                if rel is None and in_ram(w):
                    found.append((kind or "word", None, addr, w))
        return found


LAYOUT = set()  # the memory map's addresses, from mk/version/<version>.mk


def syms_of_ld(path):
    names = set()
    if path.exists():
        for line in path.read_text().splitlines():
            m = re.match(r"(\S+) = ", line)
            if m:
                names.add(m.group(1))
    return names


def category(name, obj, stages):
    """What kind of source the finding is in"""
    if name in stages:
        return "stages"
    # PsyQ: the libraries, their startup and STDWTITL's libpress
    if "/psyq" in obj or "/libpress" in obj or obj.endswith("crt0.s.o"):
        return "psyq"
    if name != "main":
        return "overlays"
    if "/data/" in obj or obj == "header":
        return "game data"
    return "game C"


def known_entries(text):
    """The entries of a known list by their key: what is before the #"""
    known = {}
    for line in text.splitlines():
        key = line.split("#", 1)[0].strip()
        if key:
            known[key] = line
    return known


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("-v", "--version", default="eu")
    ap.add_argument("--all", action="store_true", help="list every address, with what it holds and where")
    ap.add_argument("--numbers", action="store_true", help="with --all, also the words that are likely numbers")
    ap.add_argument("--update", action="store_true", help="write what is found as the known list")
    ap.add_argument("--strict", action="store_true", help="also fail on what the known list has that is fixed")
    ap.add_argument("--base", metavar="REV", help="also fail on what the known list has that REV's doesn't")
    ap.add_argument("elfs", nargs="*")
    args = ap.parse_args()
    build = pathlib.Path("build") / args.version
    known_path = pathlib.Path("config") / args.version / "shiftcheck.txt"
    mk = pathlib.Path("mk/version/%s.mk" % args.version).read_text()
    exe = re.search(r"^EXE_NAME := (\S+)", mk, re.M).group(1)
    LAYOUT.update(re.findall(r"^(\w+_VRAM) := ", mk, re.M))
    every = sorted(build.glob("*.elf"))
    elfs = [pathlib.Path(e) for e in args.elfs] or every
    if not elfs:
        sys.exit("no ELFs in %s: run make VERSION=%s" % (build, args.version))
    main_syms = syms_of_ld(build / "main_syms.ld")
    par = overlay_parents()
    stages = set(words[0].lower() for words in stage_entries(args.version))
    binaries = []
    for e in sorted(set(elfs) | set(every)):
        name = "main" if e.stem == exe else e.stem
        # the executable's heap begins after the overlays (tools/link_heap.py)
        imported = syms_of_ld(build / "heap.ld") if name == "main" else set()
        if name != "main":
            imported = set(main_syms)
            p = par.get(name, par["<stage>"] if name in stages else None)
            if p:
                imported |= syms_of_ld(build / ("%s_syms.ld" % p))
        # and its children's (the Makefile's CHILDREN_<name>)
        imported |= syms_of_ld(build / ("%s_imports.ld" % name))
        b = Binary(e, imported, name)
        # what the binary's hand-written symbols (undefined_syms*.txt) make
        # of the imported ones and its own labels follows them
        hand = pathlib.Path("config") / args.version / (
            "undefined_syms.txt" if name == "main" else "undefined_syms_%s.txt" % name)
        if hand.exists():
            known_names = imported | set(n for _, n in b.syms)
            text = re.sub(r"/\*.*?\*/", "", hand.read_text(), flags=re.S)
            for n, expr in re.findall(r"^\s*(\w+)\s*=\s*([^;]+);", text, re.M):
                ids = set(re.findall(r"\b[A-Za-z_]\w*", expr))
                if ids and ids <= known_names:
                    imported.add(n)
        binaries.append((e in elfs, b))
    # the labels of every binary, to tell a pointer from a number
    labels = {}
    for _, b in binaries:
        for a, n in b.syms:
            if not n.endswith(".NON_MATCHING") and not SEG_MARK.match(n):
                labels.setdefault(a, []).append((b.name, n))

    def target(b, value):
        """The label at value: the binary's own, or another's"""
        cands = labels.get(value)
        if not cands:
            # inside a label: an address when 4-aligned into the rodata, where
            # what the label holds starts (a string in a block of them); the
            # data has too many numbers that look like its addresses. What
            # starts there follows a 0, as a string does
            if value & 3:
                return None
            for _, o in binaries:
                sec = o.section_at(value)
                if sec == ".rodata" and o.byte_at(value - 1) == 0:
                    name = o.where(value)[1]
                    if o is b:
                        return name
                    return "stages:0x%08X" % value if o.name in stages else "%s:%s" % (o.name, name)
            return None
        own = [n for bn, n in cands if bn == b.name]
        if own:
            return own[0]
        if all(bn in stages for bn, _ in cands):
            return "stages:0x%08X" % value  # a label of whichever stage is loaded
        # overlays share addresses: an entry point first
        bn, n = min(cands, key=lambda c: (not c[1].endswith("_start"), c[0] in stages, c))
        return "%s:%s" % (bn, n)

    # the addresses, the names after the # are for reading
    known = known_entries(known_path.read_text()) if known_path.exists() and not args.update else {}
    added = []
    if args.base:
        base = subprocess.run(["git", "show", "%s:%s" % (args.base, known_path.as_posix())],
                              capture_output=True, text=True)
        if base.returncode:
            sys.exit("no %s at %s: %s" % (known_path, args.base, base.stderr.strip()))
        if "\n# count, " in base.stdout:
            # the list as it was before it was kept by address: compare totals
            total = sum(int(k.split()[0]) for k in known_entries(base.stdout))
            if len(known) > total:
                sys.exit("error: %d in %s, more than the %d at %s"
                         % (len(known), known_path, total, args.base))
        else:
            base_known = known_entries(base.stdout)
            added = sorted(k for k in known if k not in base_known)
    found = {}
    per_cat = collections.Counter()
    for chosen, b in binaries:
        if not chosen:
            continue
        for kind, sub, addr, what in sorted(b.scan(), key=lambda x: x[2]):
            obj, sym = b.where(addr)
            if kind == "header":
                obj = "header"
            if kind in ("word", "header", "tail", "lui", "jal"):
                t = target(b, what)
                if t is None and kind in ("word", "tail"):
                    if args.all and args.numbers:
                        print("%-9s %08X %-10s %-34s %s  %s" % (b.name, addr, "number", "0x%08X" % what, sym, obj))
                    continue
                shown = "0x%08X" % what + (" " + t if t else "")
                t = t or "0x%08X" % what
                value = what
            else:
                t, value = what
                shown = t
            key = kind + ("/" + sub if sub else "")
            if kind == "tail":
                continue  # the leftover bytes after .bss: nothing to symbolize
            if args.all:
                print("%-9s %08X %-10s %-34s %s  %s" % (b.name, addr, key, shown, sym, obj))
            o = re.sub(r"^build/\w+/", "", obj)
            k = "%s %s 0x%08X 0x%08X" % (b.name, key, addr, value)
            found[k] = "%-41s # %s -> %s (%s)" % (k, sym, t, o)
            per_cat[(category(b.name, obj, stages), key)] += 1
    if args.update:
        lines = ["# Addresses that don't move with the code (tools/shiftcheck.py --update):",
                 "# binary, kind, where, what it points to; then the names and the object"]
        lines += [found[k] for k in sorted(found)]
        known_path.write_text("\n".join(lines) + "\n")
        print("wrote %s: %d" % (known_path, len(found)))
        return
    print("%s: %s" % (args.version, ", ".join("%s %s %d" % (c, k, n) for (c, k), n in sorted(per_cat.items()))
                      or "nothing found"))
    new = sorted(k for k in found if k not in known)
    # only a check of every binary knows what is fixed
    gone = sorted(k for k in known if k not in found) if not args.elfs else []
    for k in new:
        print("new: %s" % found[k])
    for k in gone:
        print("fixed: %s" % known[k])
    for k in added:
        print("added since %s: %s" % (args.base, known[k]))
    if added:
        print("error: %d added to %s since %s, which may only get shorter" % (len(added), known_path, args.base))
    if gone:
        print("%s: %d fixed, which tools/shiftcheck.py -v %s --update drops from %s"
              % ("error" if args.strict else "warning", len(gone), args.version, known_path))
    sys.exit(1 if new or added or (gone and args.strict) else 0)


if __name__ == "__main__":
    main()
