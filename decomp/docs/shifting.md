# Shiftable build

The build only stays a working game when code changes size if every address
in the code and data is a symbol, which the linker moves with what it names.
Three checks guard it; [CONTRIBUTING.md](../CONTRIBUTING.md#shifting) says
how to fix what they find.

## shiftcheck

The links keep their relocations (`--emit-relocs`), and `make shiftcheck`
(`tools/shiftcheck.py`) reads them to find the addresses that are numbers:
a raw `0x8...` in C, a `.word` that splat didn't symbolize, a pointer in a
u32 table, a lui or a jal without a relocation, a symbol that a symbol file
or splat's `undefined_syms_auto` gives an absolute address:
```
# Fail on any address that config/<version>/shiftcheck.txt doesn't list
make VERSION=eu shiftcheck

# List every one, with what it points to and where it is
make VERSION=eu shiftreport
```
A binary that points into the binaries it loads links against their
symbols: the executable against the mode overlays (`MODE_ENTRY_POINTS`),
FIGHTSTG against WFIGHTMN and WFIGHTTS, FIELDSTG against the stages, whose
names it takes prefixed with the stage's (`WSTAG931_startStage`,
`include/field/stages.h`). `CHILDREN_<binary>` in the Makefile lists them;
`tools/link_imports.py` writes `build/<version>/<binary>_imports.ld` with
the names the binary uses and doesn't define, and the children link against
the binary's own symbols from a first link of it with placeholders for those
names (`build/<version>/layout/`).
`config/<version>/shiftcheck.txt` lists the addresses still to fix, and is
empty in both versions. FIELDSTG's event and script command tables, which
point into whichever stage is loaded, name the symbols of the stage each
entry belongs to: `WSTAG260_script1320`, one of the story events' scripts,
and `WSTAG780_createCommand800`, a function of the stage whose scripts run the
command. The list holds an address by where it is and what it holds, with the
names after a `#`, so that a rename leaves it as it is. `make shiftcheck` warns about those that are fixed (and
`tools/shiftcheck.py --strict` fails), which `tools/shiftcheck.py -v
<version> --update` drops.
The memory map (`EXE_VRAM`, `OVERLAY_VRAM` and `STAGE_VRAM` in
`mk/version/<version>.mk`, where the executable and the overlays load) is
fixed by design, and the binaries link at its symbols, not at numbers.

## padcheck

A padding build links every binary again from the same objects with the
memory map moved up, as if the executable had grown at its start, into
`build/<version>/pad<pad>/`; in it, a word may only change where it has a
relocation (`tools/padcheck.py`):
```
# Padding builds at +0x4 and +0x10004 (which carries into the upper half of
# every %hi/%lo pair), each checked against the build
make VERSION=eu padcheck
```
`make PAD=0x10004 smoke` boots a padding build in DuckStation, on your machine only ([CONTRIBUTING.md](../CONTRIBUTING.md#shifting)).

## inputcheck

Each link also checks what it took in (`tools/inputcheck.py`): only the
objects the Makefile built for the binary, as a linker script would link a
stale object that is still in `build/`, and no blob, whose words have no
relocations, but the executable's tail, a picture.

## In the CI

The CI runs `make padcheck`, and `tools/shiftcheck.py --strict --base
HEAD^`, which also fails when the list has an entry that is fixed, or one
that the parent commit's doesn't have: the list may only get shorter.
