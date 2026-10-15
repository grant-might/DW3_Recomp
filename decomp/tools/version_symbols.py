#!/usr/bin/env python3
"""Name, in another version (eu), the symbols a shared C file uses.

    tools/version_symbols.py <version> src/<folder>/<module>.c ... [--write]

A module that becomes C in a version links against the names its C uses:
every function and datum it calls or reads must have its name in that
version's symbol files, at that version's address. This reads them from the
module's object (build/<version>/src/<folder>/<module>.c.o, so build it
first: make VERSION=<version> build/<version>/src/<folder>/<module>.c.o, with the module a c
segment in the version's config and listed in its .mk) and from the
original: each relocation of the object (a jal, a %hi/%lo pair, a pointer in
its data) sits where the original has the same instruction or word, which
holds the version's address. The names the object defines itself get the
address of their section, where the version's config puts the module. A
string or table that only the module's data points to gets splat's
automatic name (D_80012345) in the binary's own symbol file, as us has
them, so that splat labels it and the report pairs the pointers. A datum
the C defines or uses under us's automatic name gets it at the version's
address too, unless the version's asm has that name at another address
(then the module can't be shared until the name is a real one). And a field
of the C's data that a module still in asm reads by splat's name
(D_80012345) gets its address in undefined_syms, for the link.

The functions and rodata the C still includes as asm (INCLUDE_ASM,
INCLUDE_RODATA) need their name before the object builds: splat writes the
file the C includes under it. Most have it already
(tools/match_versions.py --seed); one that hasn't, such as one with us's
automatic name (func_80012345, D_80012345), gets it at the address of its
confident pair in build/<version>/version_pairs.tsv
(tools/match_versions.py) or, when the module's code or rodata is as long
as us's, at the same offset in it; in the binary's own file. So a module takes two runs: one to name those, then make
VERSION=<version> generate and the object, and one for the rest.

A name goes in the version's files that have it in us (config/<v>/symbols.txt,
symbols_<overlay>.txt, stages/<stage>.txt or undefined_syms.txt), with us's
comment. A name an overlay's C defines that us has only in the executable
(code the version links into the overlay) goes in the overlay's file, marked
"us-main" for tools/check_names.py. It prints what it would add, and the
names whose reads disagree or whose address the version's files already
give another name; --write adds the rest. Then make VERSION=<version>
regenerate and compare: a relocation in a function that doesn't line up
with the original reads the wrong word (the reads of one name have to
agree, which catches most).
"""

import argparse
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from match_versions import ROOT, chain, is_stage  # noqa: E402
from version import unit_path  # noqa: E402

LINE = re.compile(r"^\s*(?:/\*.*\*/\s*)?([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(?://(.*)|/\*(.*)\*/)?\s*$")
AUTO_NAME = re.compile(r"^(?:[A-Z]+_)?(?:func|D|jtbl|jlabel)_[0-9A-F]{8}$")

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
HEADER = "names from the C of the modules this version builds (tools/version_symbols.py)"


def own_file(binary: str) -> str:
    """The symbol file of BINARY's own names, in config/<version>/."""
    if binary == "main":
        return "symbols.txt"
    return f"stages/{binary}.txt" if is_stage(binary) else f"symbols_{binary}.txt"


def symbol_files(version: str, binary: str) -> list:
    """The symbol files the link of BINARY reads, in the order the names
    are looked for, as paths in config/<version>/: the executable's, BINARY's
    and those of the overlays it loads on top of (a stage FIELDSTG's), and the
    executable's hand-written undefined_syms.txt last."""
    loaded = [b for b in reversed(chain(binary)) if b != "main"]
    return ["symbols.txt"] + [own_file(b) for b in loaded] + ["undefined_syms.txt"]


def config_path(version: str, binary: str) -> Path:
    """BINARY's splat config: a stage's is written by tools/stage_yaml.py."""
    if is_stage(binary):
        return ROOT / "build" / version / "generated" / "stages" / f"{binary}.yaml"
    return ROOT / "config" / version / f"{binary}.yaml"


def source_module(source: str) -> tuple:
    """(binary, module, object) of the C file SOURCE: src/engine/menu/inn.c is
    main's menu/inn, src/field/stages/central_sector/wstag200.c the stage
    wstag200's central_sector/wstag200."""
    path = Path(source).resolve()
    binary, module = unit_path(path).split("/", 1)
    if binary == "stages":
        binary = path.stem
    return binary, module, path.relative_to(ROOT / "src").with_suffix(".c.o")


def read_names(path: Path) -> list:
    """[(name, address, comment)] of a symbol file."""
    out = []
    for line in path.read_text().splitlines():
        m = LINE.match(line)
        if m:
            out.append((m[1], int(m[2], 0), (m[3] or m[4] or "").strip()))
    return out


def module_layout(version: str, binary: str, module: str) -> tuple:
    """({section: (file offset, address)} of MODULE in the version's config
    of BINARY, the binary's bytes)."""
    config = yaml.safe_load(config_path(version, binary).read_text())
    code = next(s for s in config["segments"] if isinstance(s, dict) and s.get("type") == "code")
    out = {}
    for s in code["subsegments"]:
        if isinstance(s, list) and len(s) >= 3 and s[2] == module:
            section = {"c": ".text", ".rodata": ".rodata", ".data": ".data"}.get(s[1])
            if section:
                out[section] = (s[0], code["vram"] + s[0] - code["start"])
    return out, (ROOT / config["options"]["target_path"]).read_bytes()


def extent(version: str, binary: str, module: str, kinds: tuple) -> tuple:
    """(address, size) of MODULE's subsegment of one of KINDS (its code:
    c, asm; its rodata: .rodata, rodata) in the version's config of BINARY,
    or None."""
    config = yaml.safe_load(config_path(version, binary).read_text())
    code = next(s for s in config["segments"] if isinstance(s, dict) and s.get("type") == "code")
    subs = [s for s in code["subsegments"] if isinstance(s, list) and len(s) >= 2]
    for i, s in enumerate(subs):
        if len(s) >= 3 and s[2] == module and s[1] in kinds:
            end = subs[i + 1][0] if i + 1 < len(subs) else None
            return code["vram"] + s[0] - code["start"], (end - s[0]) if end is not None else None
    return None


def included_asm(version: str, source: str, named: set, us_names: dict) -> tuple:
    """([(address, name)], [problems]): the functions and rodata that
    SOURCE includes as asm and the version hasn't NAMED yet, at their pairs'
    addresses or at their offset in a module as long as us's (us has them
    at their automatic name's address or at US_NAMES')."""
    binary, module, _ = source_module(source)
    included = re.findall(r"^\s*INCLUDE_(ASM|RODATA)\(\s*\"[^\"]+\",\s*(\w+)\s*\)", Path(source).read_text(), re.M)
    kind = {n: k for k, n in included if n not in named}
    names = list(kind)
    if not names:
        return [], []
    pairs = ROOT / "build" / version / "version_pairs.tsv"
    if not pairs.exists():
        return [], [f"{pairs.relative_to(ROOT)} is missing, for {', '.join(names)}: run tools/match_versions.py {version}"]
    rows = [line.split("\t") for line in pairs.read_text().splitlines()[1:]]
    found = defaultdict(set)
    for addr, _, b, _, us_name, _, _, _, _, confidence in rows:
        if confidence == "confident" and b == binary:
            found[us_name].add(int(addr, 16))
    # failing that, the same offset in a module's code or rodata as long as
    # us's
    kinds = {"ASM": ("c", "asm"), "RODATA": (".rodata", "rodata")}
    out, problems = [], []
    for name in names:
        ours = extent(version, binary, module, kinds[kind[name]])
        theirs = extent("us", binary, module, kinds[kind[name]])
        same = ours and theirs and ours[1] is not None and ours[1] == theirs[1]
        us_addr = int(name[-8:], 16) if AUTO_NAME.match(name) else us_names.get(name)
        if len(found[name]) == 1:
            out.append((next(iter(found[name])), name))
        elif not found[name] and same and us_addr is not None and 0 <= us_addr - theirs[0] < theirs[1]:
            out.append((ours[0] + us_addr - theirs[0], name))
        else:
            problems.append(f"{name}: {len(found[name])} confident pairs in {binary}")
    return out, problems


def sext16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


def addresses(version: str, source: str) -> tuple:
    """({name: {address: reads}}, [problems]) for the C file SOURCE."""
    binary, module, rel = source_module(source)
    obj = ROOT / "build" / version / "src" / rel
    if not obj.exists():
        sys.exit(f"{obj.relative_to(ROOT)} is missing: make VERSION={version} {obj.relative_to(ROOT)}")
    layout, original = module_layout(version, binary, module)
    if ".text" not in layout:
        sys.exit(f"{module} is not a c segment in {config_path(version, binary).relative_to(ROOT)}")
    found = defaultdict(lambda: defaultdict(int))
    problems = []
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        symtab = elf.get_section_by_name(".symtab")
        sections = [s.name for s in elf.iter_sections()]
        # the names the object defines
        for sym in symtab.iter_symbols():
            if not sym.name or sym["st_info"]["bind"] != "STB_GLOBAL" or sym["st_shndx"] in ("SHN_UNDEF", "SHN_ABS"):
                continue
            if sym["st_shndx"] == "SHN_COMMON":
                continue
            section = sections[sym["st_shndx"]]
            if section in layout:
                found[sym.name][layout[section][1] + sym["st_value"]] += 1
        # the names it uses
        for section in (".text", ".rodata", ".data"):
            rel_section = elf.get_section_by_name(".rel" + section)
            if rel_section is None or section not in layout:
                continue
            ours = elf.get_section_by_name(section).data()
            off, base = layout[section]
            theirs = original[off:off + len(ours)]
            relocs = list(rel_section.iter_relocations())
            # the %hi's of each name, by offset: gas moves a %hi's
            # relocation next to the first %lo it pairs with, so they are
            # found by the register the %lo's instruction uses
            his = defaultdict(dict)
            for r in relocs:
                if r["r_info_type"] == R_MIPS_HI16:
                    his[symtab.get_symbol(r["r_info_sym"]).name][r["r_offset"]] = True
            for r in relocs:
                sym = symtab.get_symbol(r["r_info_sym"])
                o = r["r_offset"]
                if sym["st_info"]["type"] == "STT_SECTION":
                    # a pointer in the data to the module's own literals: the
                    # original's word is the address of a string splat only
                    # labels if a name says so
                    if (r["r_info_type"] == R_MIPS_32 and section != ".text"
                            and sections[sym["st_shndx"]] in (".rodata", ".data") and o + 4 <= len(theirs)):
                        found[None][struct.unpack_from("<I", theirs, o)[0]] += 1
                    continue
                if not sym.name:
                    continue
                if o + 4 > len(theirs):
                    continue
                a = struct.unpack_from("<I", ours, o)[0]
                b = struct.unpack_from("<I", theirs, o)[0]
                t = r["r_info_type"]
                if t == R_MIPS_32:
                    found[sym.name][(b - a) & 0xFFFFFFFF] += 1
                    continue
                if (a >> 26) != (b >> 26):
                    problems.append(f"{module}: the instruction at {section}+{o:#x} ({sym.name}) differs")
                    continue
                if t == R_MIPS_26:
                    pc = (base + o) & 0xF0000000
                    found[sym.name][(pc | ((b & 0x3FFFFFF) << 2)) - ((a & 0x3FFFFFF) << 2)] += 1
                elif t == R_MIPS_LO16:
                    # the closest %hi before it that loads its base register
                    # (or, failing that, the closest %hi of the name)
                    base_reg = (a >> 21) & 31
                    cands = sorted((h for h in his[sym.name] if h < o), reverse=True)
                    h = next((h for h in cands
                              if (struct.unpack_from("<I", ours, h)[0] >> 16) & 31 == base_reg), None)
                    if h is None:
                        h = cands[0] if cands else None
                    if h is None:
                        problems.append(f"{module}: %lo({sym.name}) at {section}+{o:#x} has no %hi")
                        continue
                    our_hi = struct.unpack_from("<I", ours, h)[0] & 0xFFFF
                    their_hi = struct.unpack_from("<I", theirs, h)[0] & 0xFFFF
                    addend = (our_hi << 16) + sext16(a & 0xFFFF)
                    found[sym.name][((their_hi << 16) + sext16(b & 0xFFFF) - addend) & 0xFFFFFFFF] += 1
        # what the object covers, and the names it defines
        covered = [(layout[n][1], layout[n][1] + elf.get_section_by_name(n).data_size)
                   for n in (".rodata", ".data") if n in layout and elf.get_section_by_name(n)]
        defined = {sym.name for sym in symtab.iter_symbols()
                   if sym.name and sym["st_shndx"] not in ("SHN_UNDEF",)}
    return found, problems, covered, defined


def asm_reads(version: str, binary: str, covered: list, defined: set) -> dict:
    """{name: address} of splat's automatic names, in the binary's asm, of
    addresses inside COVERED that the C doesn't define: a field of the C's
    data that a module still in asm reads by its own name."""
    config = config_path(version, binary).read_text()
    c_modules = set(re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", config, re.M))
    base = ROOT / "asm" / version / ("stages" if is_stage(binary) else binary)
    out = {}
    for path in base.rglob("*.s"):
        rel = path.relative_to(base)
        if rel.parts[0] in ("nonmatchings", "matchings"):
            continue
        # the stages share their directory: only this one's files
        if is_stage(binary) and path.name.split(".")[0] != binary:
            continue
        # the C modules' own disassembly (objdiff's targets) isn't linked
        module = re.sub(r"\.(rodata|data|bss)$", "", rel.with_suffix("").as_posix())
        if (module[len("data/"):] if module.startswith("data/") else module) in c_modules:
            continue
        for name in set(re.findall(r"\b((?:[A-Z]+_)?D_([0-9A-F]{8}))\b", path.read_text())):
            addr = int(name[1], 16)
            if name[0] not in defined and any(lo <= addr < hi for lo, hi in covered):
                out[name[0]] = addr
    return out


def own_labels(version: str, binary: str) -> dict:
    """{label: address} of splat's automatic labels in the version's asm of
    BINARY (as last generated)."""
    base = ROOT / "asm" / version / ("stages" if is_stage(binary) else binary)
    label = re.compile(r"^\s*(?:glabel|dlabel|jlabel|alabel)\s+(\w+)")
    word = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")
    out = {}
    for path in base.rglob("*.s"):
        if is_stage(binary) and path.relative_to(base).parts[0] not in ("nonmatchings", "matchings") \
                and path.name.split(".")[0] != binary:
            continue
        pending = []
        for line in path.read_text().splitlines():
            m = label.match(line)
            if m:
                pending.append(m[1])
                continue
            m = word.match(line)
            if m:
                for name in pending:
                    if AUTO_NAME.match(name):
                        out[name] = int(m[1], 16)
                pending = []
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("version")
    parser.add_argument("sources", nargs="+")
    parser.add_argument("--write", action="store_true", help="add the names to the version's symbol files")
    args = parser.parse_args()

    problems = []
    add = defaultdict(list)
    by_binary = defaultdict(list)
    for source in args.sources:
        by_binary[source_module(source)[0]].append(source)
    for binary, sources in by_binary.items():
        # the version's names, by file
        have = {}
        by_address = {}
        for file in symbol_files(args.version, binary):
            path = ROOT / "config" / args.version / file
            have[file] = {}
            by_address[file] = defaultdict(set)
            for name, addr, _ in read_names(path) if path.exists() else []:
                have[file][name] = addr
                by_address[file][addr].add(name)
        own = own_file(binary)

        # where us has each name (for this binary's link)
        us_files = defaultdict(list)
        us_comment = {}
        us_addr = {}
        for file in symbol_files("us", binary):
            path = ROOT / "config" / "us" / file
            for name, addr, comment in read_names(path) if path.exists() else []:
                us_files[name].append(file)
                us_comment.setdefault(name, comment)
                us_addr.setdefault(name, addr)
        # the functions the C includes as asm: their names first, for the
        # object to build
        missing = False
        labels = own_labels(args.version, binary)
        for source in sources:
            included, p = included_asm(args.version, source, set().union(*have.values()), us_addr)
            problems += p
            for addr, name in included:
                others = {n for f in by_address for n in by_address[f].get(addr, ())}
                if others:
                    problems.append(f"{name}: {addr:#010x} is {', '.join(sorted(others))}")
                    continue
                if labels.get(name, addr) != addr:
                    problems.append(f"{name} = {addr:#010x}: the version's {binary} has its own {name}")
                    continue
                comment = us_comment.get(name, "type:func" if name.startswith("func_") else "")
                add[own].append((addr, name, comment))
                missing = True
        if missing:
            problems.append(f"{binary}: name the functions the C includes as asm (--write), then make "
                            f"VERSION={args.version} generate and the objects, and run this again")
            continue

        found = defaultdict(lambda: defaultdict(int))
        covered, defined = [], set()
        labels = own_labels(args.version, binary)
        for source in sources:
            f, p, c, d = addresses(args.version, source)
            problems += p
            covered += c
            defined |= d
            for name, addrs in f.items():
                for a, n in addrs.items():
                    found[name][a] += n

        # the literals only the data points to get splat's automatic name,
        # in the binary's own file (as in us)
        for addr in sorted(found.pop(None, {})):
            name = f"D_{addr:08X}"
            if any(name in names for names in have.values()) or (addr, name, "") in add[own]:
                continue
            others = {n for f in by_address for n in by_address[f].get(addr, ())}
            if others:
                continue
            add[own].append((addr, name, ""))

        # the fields of the C's data that modules still in asm read by
        # splat's names: the link needs their addresses
        undefined = "undefined_syms.txt"
        for name, addr in sorted(asm_reads(args.version, binary, covered, defined).items(),
                                 key=lambda x: x[1]):
            if name not in have.get(undefined, {}):
                add[undefined].append((addr, name, ""))

        for name in sorted(found):
            addrs = found[name]
            if "." in name:
                # INCLUDE_ASM's NAME.NON_MATCHING aliases of a function
                continue
            if AUTO_NAME.match(name):
                # the asm the C includes reads the version's own labels
                # (splat's func_80012345 at 0x80012345)
                addrs = {a: n for a, n in addrs.items() if a != int(name[-8:], 16)}
                if not addrs:
                    continue
            if len(addrs) > 1:
                listed = ", ".join(f"{a:#010x} ({n}x)" for a, n in sorted(addrs.items(), key=lambda x: -x[1]))
                problems.append(f"{name}: the reads disagree: {listed}")
                continue
            addr = next(iter(addrs))
            if name not in us_files:
                if any(name in names for names in have.values()):
                    continue
                if AUTO_NAME.match(name):
                    # a datum the C defines or uses under us's automatic
                    # name (splat's label in us): the version's address
                    # gets the same name, for the link and so that the
                    # report pairs it, unless the version labels another
                    # address so
                    if labels.get(name, addr) != addr:
                        problems.append(f"{name} = {addr:#010x}: the version's {binary} has its own {name}")
                        continue
                    if (addr, name, "") not in add[own]:
                        add[own].append((addr, name, ""))
                    continue
                problems.append(f"{name} = {addr:#010x}: no us symbol file of {binary} has it")
                continue
            # code us has in the executable that this version links into
            # the overlay: its own names go in the overlay's file, and its
            # other C there uses them from that file
            main_files = {"symbols.txt", "undefined_syms.txt"}
            if binary != "main" and name in have[own] and set(us_files[name]) <= main_files:
                if have[own][name] != addr:
                    problems.append(f"{name}: {own} has it at {have[own][name]:#010x}, the C reads {addr:#010x}")
                continue
            if binary != "main" and name in defined and set(us_files[name]) <= main_files:
                comment = " ".join(["us-main"] + ([us_comment[name]] if us_comment[name] else []))
                if name in have[own]:
                    if have[own][name] != addr:
                        problems.append(f"{name}: {own} has it at {have[own][name]:#010x}, the C reads {addr:#010x}")
                elif (addr, name, comment) not in add[own]:
                    add[own].append((addr, name, comment))
                continue
            # in each of the files us has it in (a label in an overlay's
            # symbols and its address in undefined_syms for the link)
            for file in us_files[name]:
                if name in have[file]:
                    if have[file][name] != addr:
                        problems.append(f"{name}: {file} has it at {have[file][name]:#010x}, the C reads {addr:#010x}")
                    continue
                others = {n for n in by_address[file].get(addr, ()) if not AUTO_NAME.match(n)}
                if others:
                    problems.append(f"{name}: {addr:#010x} is {', '.join(sorted(others))} in {file}")
                    continue
                if (addr, name, us_comment[name]) not in add[file]:
                    add[file].append((addr, name, us_comment[name]))

    for p in problems:
        print("!", p)
    for file, entries in sorted(add.items()):
        print(f"config/{args.version}/{file}:")
        lines = []
        for addr, name, comment in sorted(entries):
            if file.startswith("undefined_syms"):
                line = f"{name} = {addr:#010X};".replace("0X", "0x")
            else:
                line = f"{name} = {addr:#010X};".replace("0X", "0x") + (f" // {comment}" if comment else "")
            lines.append(line)
            print("  " + line)
        if args.write:
            path = ROOT / "config" / args.version / file
            path.parent.mkdir(parents=True, exist_ok=True)
            text = path.read_text() if path.exists() else ""
            if file.startswith("undefined_syms"):
                header = ("/* from the C of the modules this version builds (tools/version_symbols.py):\n"
                          "   what it calls by address, and the fields of its data that the modules\n"
                          "   still in asm read by splat's names */")
                if not text:
                    text = "/* addresses the executable uses that none of its objects defines */\n"
            else:
                header = f"// {HEADER}"
            if header not in text:
                text = text.rstrip("\n") + ("\n\n" if text else "") + header + "\n"
            path.write_text(text.rstrip("\n") + "\n" + "\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
