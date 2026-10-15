#!/usr/bin/env python3
"""Write the splat config of a stage overlay (AAA/PRO/WSTAG###.PRO).

The stage overlays all have the same layout, so instead of a config file
each, config/<version>/stages.txt lists them with the offsets where their code starts
and ends:

    WSTAG200 0x4 0x2B8 [asm | asm-data | head-word | c-rodata]
    WSTAG260 data [asm]

A stage loads at STAGE_VRAM (mk/version/<version>.mk; 0x800A4CA4 in us), after
the largest main overlay (CARDGAME), on top of FIELDSTG, whose functions it calls. Most stages start right with code; the
others with the color that their setup function copies to D_800990B4.unk38
(0x808080, 0x966754 or 0x1966754), or with the jump tables of their switch
statements. From the end of the code on, the file is data: C, from the
stage's C file, unless the stage is marked "asm" (its code and data are still
splat's assembly in this version) or "asm-data" (only its data is).
"c-rodata" takes the bytes before the code (the color, or the jump tables)
from the stage's C file too (it can follow "asm-data"). "head-word" before it
takes a color that comes before the jump tables from a C file of its own,
<name>_head.c next to the stage's, linked first: in the stage's C file GCC
would align the jump tables to 8 bytes after it (WSTAG924).

The stage's C file is src/field/stages/<area>/<name>.c, in the folder of its area
(tools/stage_areas.py), or src/field/stages/<name>.c for one in no area: the
subsegments are named <area>/<name>, so that splat finds it there and
writes its assembly to asm/<version>/stages/<area>/.

A stage with no code, "data" (WSTAG260, the story events' scripts that
FIELDSTG runs), is all data, from its C file, or splat's assembly ("data
asm").

A stage file that ends inside a word (seven European ones) has its last
bytes in its C's data too, and no section after them is aligned; one whose
data is still asm keeps them in a databin of their own ({name}_end), as
splat's data segment drops them (a blob, which tools/inputcheck.py lets
the stage link as it is shorter than a word).

usage: stage_yaml.py wstag200 build/us/generated/stages/wstag200.yaml
(VERSION, as for make, picks the version; eu by default)
"""

import hashlib
import os
import sys

import version


TEMPLATE = """\
# {version_name}: {name}, written by tools/stage_yaml.py
name: {name}
sha1: {sha1}
options:
  basename: {name}
  target_path: {target}
  base_path: {base}
  platform: psx
  compiler: GCC

  asm_path: asm/{version}/stages
  asset_path: asm/{version}/stages/bin
  src_path: src/field/stages
  build_path: build/{version}

  ld_script_path: build/{version}/generated/{name}.ld

  global_vram_start: 0x80000000
  global_vram_end: 0x80200000

  find_file_boundaries: True

  generate_asm_macros_files: False

  section_order: [".rodata", ".text", ".data", ".bss"]

  symbol_addrs_path:
{symbols}
  undefined_funcs_auto_path: build/{version}/generated/undefined_funcs_auto_{name}.txt
  undefined_syms_auto_path: build/{version}/generated/undefined_syms_auto_{name}.txt

  string_encoding: ASCII
  data_string_encoding: ASCII
  rodata_string_guesser_level: 2
  data_string_guesser_level: 2

  ld_bss_is_noload: {odd_end}
  ld_align_section_vram_end: {align_end}

  migrate_rodata_to_functions: False
  create_data_pads: False

segments:
  - name: {name}
    type: code
    start: 0x0
    vram: 0x{vram:X}
    vram_symbol: STAGE_VRAM
    align: 4
    subalign: 4
    subsegments:
{subsegments}{tail}  - [0x{size:X}]
"""

# the bytes after the last word, which splat's data segment would pad
TAIL = """\
  - name: {name}_end
    type: databin
    start: 0x{start:X}
    vram: 0x{vram:X}
    follows_vram: {name}
    subalign: 4
"""


# a stage that is still one blob (listed by name alone)
BLOB_TEMPLATE = """\
# {version_name}: {name}, written by tools/stage_yaml.py
name: {name}
sha1: {sha1}
options:
  basename: {name}
  target_path: {target}
  base_path: {base}
  platform: psx
  compiler: GCC

  asm_path: asm/{version}/stages
  asset_path: asm/{version}/stages/bin
  src_path: src/field/stages
  build_path: build/{version}

  ld_script_path: build/{version}/generated/{name}.ld

  symbol_addrs_path:
{symbols}
  undefined_funcs_auto_path: build/{version}/generated/undefined_funcs_auto_{name}.txt
  undefined_syms_auto_path: build/{version}/generated/undefined_syms_auto_{name}.txt

  generate_asm_macros_files: False
  create_c_files: False

segments:
  - name: {name}
    type: databin
    start: 0x0
    vram: 0x{vram:X}
    vram_symbol: STAGE_VRAM
    subalign: 4
  - [0x{size:X}]
"""


def main():
    name, out = sys.argv[1], sys.argv[2]
    root = str(version.ROOT)
    config = f"config/{version.VERSION}"
    found = blob = data_only = False
    rodata_type = "rodata"
    head_word = False
    for line in open(os.path.join(root, config, "stages.txt")):
        words = line.split("#", 1)[0].split()
        if words and words[0].lower() == name:
            found = True
            # "data": the stage has no code, its C file holds it all ("data
            # asm": splat's assembly does, in this version)
            data_only = words[1:2] == ["data"]
            if len(words) == 1 or data_only:
                blob = not data_only
                text_start = text_end = 0
                marks = words[2:]
            else:
                text_start, text_end = int(words[1], 16), int(words[2], 16)
                # "asm": the code and data are still splat's assembly in this
                # version; "asm-data": the data is; "c-rodata": the jump tables
                # before the code are the C file's
                marks = words[3:]
            if marks != ["asm"] and [m for m in ("asm-data", "head-word", "c-rodata") if m in marks] != marks or (
                    "head-word" in marks and "c-rodata" not in marks) or (data_only and marks not in ([], ["asm"])):
                sys.exit(f"{config}/stages.txt: unknown mark in {line.strip()}")
            code = "asm" if marks == ["asm"] else "c"
            data_type = "data" if "asm" in marks or "asm-data" in marks else ".data"
            rodata_type = ".rodata" if "c-rodata" in marks else "rodata"
            head_word = "head-word" in marks
    if not found:
        sys.exit(f"{name} is not in {config}/stages.txt")

    disk = os.path.relpath(version.DISK_DIR, root)
    target = f"{disk}/AAA/PRO/{name.upper()}.PRO"
    data = open(os.path.join(root, target), "rb").read()
    symbols = [f"{config}/symbols.txt"]
    if os.path.exists(os.path.join(root, f"{config}/symbols_fieldstg.txt")):
        symbols.append(f"{config}/symbols_fieldstg.txt")
    if os.path.exists(os.path.join(root, f"{config}/stages/{name}.txt")):
        symbols.append(f"{config}/stages/{name}.txt")

    # the stage's C file is src/field/stages/<path>.c (and its head <path>_head.c)
    path = version.stage_path(name)
    if data_only:
        subsegments = f"      - [0x0, {data_type}, {path}]\n"
    else:
        subsegments = ((f"      - [0x0, .rodata, {path}_head]\n" if head_word else "")
                       + (f"      - [0x{4 if head_word else 0:X}, {rodata_type}, {path}]\n" if text_start else "")
                       + f"      - [0x{text_start:X}, {code}, {path}]\n"
                       + f"      - [0x{text_end:X}, {data_type}, {path}]\n")

    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w") as f:
        f.write(
            (BLOB_TEMPLATE if blob else TEMPLATE).format(
                name=name,
                version=version.VERSION,
                version_name=version.VERSION_NAME,
                sha1=hashlib.sha1(data).hexdigest(),
                target=target,
                base=os.path.relpath(root, os.path.dirname(os.path.abspath(out))),
                symbols="".join(f"    - {s}\n" for s in symbols),
                vram=version.STAGE_VRAM,
                subsegments=subsegments,
                # splat's data drops the bytes after the last word: the C's
                # data holds them, asm data needs them apart
                tail=TAIL.format(name=name, start=len(data) & ~3, vram=version.STAGE_VRAM + (len(data) & ~3))
                if len(data) % 4 and data_type == "data" else "",
                # and the file ends there: no section after the data, nor the
                # data's end, is aligned to a word (as in CNTY_SEL)
                odd_end=bool(len(data) % 4 and data_type != "data"),
                align_end=not (len(data) % 4 and data_type != "data"),
                size=len(data),
            )
        )


if __name__ == "__main__":
    main()
