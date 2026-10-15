# Toolchain, layout and tools

## Toolchain

| | |
|---|---|
| Game code | GCC 2.8.1 (`-O2 -G0`; `-G8` for the executable's modules cut from the original's `inn`, `system`, `memcard`, `game3`, `game3_2`, `graphics`, `sound` and `overlay`, `G8_SRC` in the Makefile) + ASPSX 2.86, emulated with [maspsx](https://github.com/mkst/maspsx) |
| Splitting | [splat](https://github.com/ethteck/splat) 0.50.0 |
| Diffing | [objdiff](https://github.com/encounter/objdiff) 3.8.1, [decomp.dev](https://decomp.dev) |

- The compiler was identified by running m2c over every game function and
  building the output with several GCC versions: GCC 2.8.x `-O2` matches 82 of
  342 functions untouched, 2.7.2 matches 49 and 2.91.66 matches 46. GCC 2.8.0
  and 2.8.1 give the same results, and so do ASPSX 2.56 to 2.86.
- The game's divisions carry no divide-by-zero check, so maspsx runs without
  `--expand-div`.
- The modules cut from the original's `system` and `graphics` read their
  small variables through `$gp` (`ROOT_TASK` and `BOOT_IMAGE_RECT` in
  `system/main.c`, `CD_MODE` in `file/cd_reader.c`; `FLIP_PENDING` in
  `gfx/display.c`, the drawers and `TEXT_TOOLS` in theirs), so they are
  built with `-G8` in both GCC and maspsx (`SDATA_LIMIT` in the Makefile).
  Those variables are declared `static` in the C; maspsx emits them as
  common symbols that resolve to their definitions.
- `game3_2`'s and `overlay`'s modules read nothing through `$gp`, but match
  only at `-G8` too: GCC then leaves the address of a small extern
  (`GET_DIGIMON` in `game/partner.c`) to the assembler's macro, which loads
  it again for every read, and that changes the scheduling and the
  registers. `inn`'s, `memcard`'s and `game3`'s need it for the same reason
  in the European version only, for `LANGUAGE`. `sound/sound.c` is built
  with `-G8` as well, but its code is the same at `-G0`. Every module cut
  from a `-G8` object keeps it; the rest of the game uses `-G0`.
- The C includes the PsyQ 4.7 headers from
  [psyq_headers](https://github.com/Rivarux/psyq_headers) (a fork of
  [jype0's](https://github.com/jype0/psyq_headers)). `libgte.h` names
  some parameters `$2`, hence `-fdollars-in-identifiers`. The game uses
  signed `char` (`-fsigned-char`).
- Most global function pointers live in tables (the heap, `HEAP`, holds
  `free`, `alloc` and `zero`, for example) and must be called through a struct.
  GCC 2.8 assumes a struct field and a scalar global never alias, so with a
  scalar `extern` it moves stores to struct fields past the load of the
  function pointer. A store through a pointer to a field
  isn't a struct access to it: `WFIGHTMN_setStat` keeps the load of
  `FIGHTSTG_battleTableFunc` after the stores that way.

To use a different binutils or objdiff, create `local.mk`:
```
CROSS := /path/to/mipsel-linux-gnu-
OBJDIFF := /path/to/objdiff-cli
```

## Modern GCC build

`make TOOLCHAIN=gcc` builds the game's C with the binutils' own GCC
(`$(CROSS)gcc`, GCC 12 on Ubuntu 24.04) into `build/<version>/gcc/`, and
leaves the matching build (`TOOLCHAIN=gcc-psx`, the default) as it is. It
doesn't match: it links the same objects, in the same order and with the
same memory map, which is what makes it a working game. The PsyQ libraries
and the executable's header are the same assembly. `mk/toolchain/gcc.mk`
has its flags, each with its reason: `-Os -G0` for the R3000 with no
position-independent code, and what GCC 2.8.1 did that GCC 12 doesn't by
default (`-std=gnu89`, `-fno-strict-aliasing`, `-fwrapv`,
`-fno-delete-null-pointer-checks`, `-fno-aggressive-loop-optimizations`,
no divide-by-zero check, zero-initialized variables in `.data` because an
overlay's `.bss` is neither loaded nor cleared, `-fno-common`,
`-fno-toplevel-reorder`).

- It links with splat's scripts, rewritten into `build/<version>/gcc/ld/`
  for its objects and the sections GCC 12 splits from theirs
  (`.text.startup`, `.rodata.str1.4`), so they aren't discarded.
- The executable's tail, the boot image, stays at `OVERLAY_VRAM`, where
  `main()` reads it, and the executable is padded to whole sectors, which
  the BIOS reads (`EXE_TEXT_SIZE`, `config/<version>/undefined_syms.txt`).
- The CD reader loads each file as large as the original's
  (`getFileSectorCount`), so each binary must fit in its file on the disc,
  and FIELDSTG and FIGHTSTG before `STAGE_VRAM`, where the stages load:
  `make fitcheck` (`tools/fitcheck.py`), which this build runs, fails on
  one that doesn't, and `FITCHECK_ARGS=--all` lists every binary with its
  sizes and limits. The heap begins after the original's files
  (`tools/link_heap.py --disc`), no later than the original's.
- Two things in the C read differently with GCC 3 and later, in
  `include/common.h`, and keep the matching build's bytes:
  `FLEXIBLE`, the length of an array that ends a struct, whose
  initializers GCC 12 drops when it is zero-length (`SoundFiles.seps`), and
  `INTERRUPT_SHARED`, `volatile` for a field that an interrupt's callback
  writes while the game polls it, which GCC 12 would read once
  (`DecEnv.isdone`, the title movie's decoder).

`compare`, `objdiff`, `report`, `shiftcheck`, `padcheck` and the other
checks of the match work on the matching build only. `make TOOLCHAIN=gcc
smoke` boots this build ([CONTRIBUTING.md](../CONTRIBUTING.md#shifting));
the CI builds it for both versions without failing the run on it yet.

## Lint

GCC 2.8.1 builds a call to an undeclared function, a pointer of the wrong
type or a function that returns no value with at most a warning. `make lint`
(`tools/lint.py`) has a modern GCC (`LINT_CC`, `mipsel-linux-gnu-gcc` by
default) compile every C file the version builds, with the build's
preprocessor flags, and fails on any warning that
`config/<version>/lint.txt` doesn't list. It writes nothing (`-S -o
/dev/null`), so it never affects the match, and it gives the C its own
`__GNUC__`: the C is checked as a modern GCC sees it. Besides GCC's default
warnings, it turns on the ones that find plain mistakes (`WARNINGS` in the
script says what each finds): an undeclared function, a declaration with no
type, a pointer of the wrong type, an integer used as a pointer, a
non-void function that returns no value, an object written twice between
sequence points, a local read before it is set. It also checks the
declarations: a function that isn't `static` needs a prototype before its
definition, from its header (the stages, one file each, are exempt, in
`EXEMPT`), and a file may declare a thing only once. GCC 12 has no such
check for variables. Of `-Wall` and `-Wextra` it takes the warnings that
suit the period's C: a `case` that falls through with no `/* fallthrough */`
comment, a local, label or `static` function nothing uses, a misleading
precedence or indentation, a function cast to another type, a comparison
always true or false, and others that find nothing. It leaves out, with
why, those that would flag the original's own forms: unused parameters
(methods take their table's), partial table initializers, unsigned range
checks, Shift-JIS text as `u8`, old-style definitions and declarations
without a prototype. The defaults it turns off are in `QUIET`, with why
(STAGSLCT's two-byte character constants).

As with `shiftcheck.txt`, the CI runs it with `--strict --base HEAD^`, so
the list may only get shorter; the list's `# checked:` line names the
warnings it was written with, so that turning one on brings its own
entries. An entry that stays has a comment above it that says why, which
`--update` keeps:
```
make VERSION=eu lint
# Write what it finds as the list, once it is shorter
make VERSION=eu lint LINT_ARGS=--update
```

## Editor

[clangd](https://clangd.llvm.org) gives an editor the project's types and
declarations, with the build's own flags: `make compile_commands` writes
`compile_commands.json` (`tools/compile_commands.py`), every C file the
version builds with the Makefile's include paths and defines, `VERSION_EU`
or `VERSION_US` among them. It is for one version at a time, so
`make VERSION=us compile_commands` switches to the USA one, and it isn't in
git: write it again after adding a C file. `.clangd` adds what the editor
shouldn't warn about. A header gets the flags of a file that includes it;
an `.inc.c` file has no entry of its own, so the macros its including file
defines before the include are missing there.
To check the setup, `clangd --check=src/engine/task/task.c` parses one file
and prints what it finds.

For VS Code, `.vscode/` recommends the clangd extension (in place of
Microsoft's C/C++ one, whose IntelliSense would compete with it) and has
tasks that build, compare, lint, report and regenerate each version, write
objdiff's configuration and `compile_commands.json`, and format the open
file or check the formatting (Terminal > Run Task; the European build is the
default build task). They run make with the
Python of `.venv`. `settings.json` keeps clangd from adding an `#include` as it
completes a name: the headers a file includes are the project's choice.

## Formatter

`.clang-format` writes down the C's layout (CONTRIBUTING.md, Code) for
clang-format 18, and `tools/format.sh` runs it on the C files of `src/` and
`include/` that git has, or on the files it is given:

```
# Format the files in place
make format
make format FILES=src/engine/task/task.c
# List the files it would change, and fail if there are any
make format-check
```

`CLANG_FORMAT` names the program when it isn't `clang-format` (Ubuntu 24.04's
`clang-format` package, the CI image's system, is 18.1.3). There is no
column limit: a line breaks where it was written, as GCC 2.8.1's line notes
can depend on a break, and a formatted file is built and compared like any
other change. The script leaves alone what clang-format 18 would change
for the worse: the data, a top-level definition with an initializer to its
`;` (it would put every element of a list that ends in a comma on a line of
its own), and the files of its `IGNORE` list, each with the reason.
The tree hasn't been formatted with it as a whole, so `make format-check`
still lists the files that differ.

## Docker

The `Dockerfile` has the CI's build environment: Ubuntu 24.04 with the MIPS
binutils, Python with `requirements.txt`, and the compiler and tools that
`tools/dl_deps.sh` downloads. It holds no game data. `tools/docker.sh` builds
the image and runs a command in it, with the repository (`disks/` and
`external/` included) mounted at `/dw3`, as your own user. `VERSION` is passed
on when it is set; without a command it opens a shell. No `bin/` or `.venv`
is needed on the host: `BIN_DIR` points at the image's tools.
The prebuilt tools are x86 Linux binaries, so the image is `linux/amd64`.

`.devcontainer/devcontainer.json` opens the same image as a VS Code dev
container (Dev Containers: Reopen in Container), with the repository mounted
at `/dw3` as `tools/docker.sh` does and the clangd extension. It runs as
Ubuntu's own user with your user id, adds git, which the image doesn't
have, and writes `compile_commands.json` once it is created. The disc in
`disks/` comes from the repository, as with `tools/docker.sh`.

## API documentation

`make docs` builds the API documentation with
[Doxygen](https://www.doxygen.nl) and the
[Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css) theme, the
submodule `external/doxygen-awesome-css`, into `build/docs/html/`: the
European version's functions, types, globals and macros, with their source,
call graphs and include graphs, and these pages. Without graphviz it leaves
the graphs out:
```
sudo apt install doxygen graphviz
git submodule update --init external/doxygen-awesome-css
make docs          # build/docs/html/index.html, warnings in build/docs/warnings.log
make docs-clean
```
The C keeps its plain `/* */` comments: as doxygen reads a file,
`tools/doxygen_filter.py` turns the comment right above a function, type,
global or macro, and the one after a field on its line, into its
documentation, so the comments that [CONTRIBUTING.md](../CONTRIBUTING.md#code)
asks for are the documentation. The folders' descriptions come from
[src/README.md](../src/README.md) and
[the stages' list](../src/field/stages/README.md) (`tools/doxygen_dirs.py`).
None of it touches the game's build.

The CI (`.github/workflows/docs.yaml`) builds it on every push to `main`
and publishes it to GitHub Pages: <https://regame-labs.github.io/dw3_decomp/>.

## Layout

| Path | Contents |
|---|---|
| `src/engine/<module>/` | the executable's game code, one folder per module, cut from the original objects in their link order (see [binaries.md](binaries.md#the-games-binaries)) |
| `src/engine/data/` | the executable's data that no module can hold, each file saying why |
| `src/<group>/<folder>/` | each overlay's C, in a folder named after what it is ([src/README.md](../src/README.md) lists them with their files on the disc), grouped by the part of the game it runs: `field/` (FIELDSTG and the stages), `battle/` (FIGHTSTG, WFIGHTMN, STFGTREP), `cardgame/` (CARDGAME and the STCRD* menus), `menus/` (the other menus and screens) and `debug/` (the developers' test menus); `<overlay>_2.c` is the second half of an object split in two. `src/field/field_mode/` has a file per module instead, more than the original's five objects, split only where each jump table keeps its place, and its data in `data/fieldstg.c`; `src/battle/battle_mode/` has its third to sixth objects cut the same way into modules (`entrance.c` to `interp.c`, between `stage.c`, the second object, and `camera_task.c`, the last), each with its own data; `src/cardgame/card_battle/` has its second and third objects cut so too (`play_checks.c` to `round.c`, `battle_steps.c` to `fader.c`), with their data in `data/cardgame_2.c` and `data/cardgame_3.c`; `src/battle/normal_battle/` has its second object in `setup.c`, `menu.c` and `techs.c`, with its data in `data/wfightmn_2.c`, and `src/battle/report/` is cut into `scene.c` to `growth.c`, with its data in `data/stfgtrep.c` |
| `src/shared/` | the code several overlays share, each its own copy of the same C, which their files include as `"shared/<file>.inc.c"` |
| `src/field/stages/<area>/` | one C file per stage, `wstag###.c`, in the folder of its area (the list: [src/field/stages/README.md](../src/field/stages/README.md)); `src/field/stages/common/` holds the code the stages share, and `wstag260.c`, in no area, the story events' scripts |
| `include/engine/game.h`, `include/engine/` | types and declarations of the game code, one header per module of `src/engine/` (`task.h`, `heap.h`, `gfx.h`, `file.h`, `pad.h`, `random.h`, `sound.h`, `overlay.h`, `text.h`, `game_state.h`, `memcard.h`, `menu.h`) |
| `include/<group>/<folder>.h`, `include/field/stage.h` | the overlays' types and declarations, in the groups of `src/`, and the stages'; FIGHTSTG's and CARDGAME's are split by module into `include/battle/battle_mode/` and `include/cardgame/card_battle/`; `include/shared/` has the shared code's |
| `include/` | `common.h`, `version.h`, `include_asm.h` and the assembler macros |
| `config/<version>/` | the version's splat configs, symbols, stage list and checksums |
| `mk/version/` | each version's settings for the Makefile and the tools |
| `docs/` | the status, the binaries and versions, the toolchain and the shiftable build; `Doxyfile` and `doxygen_head.html`, the API documentation's settings and the theme's scripts |
| `tools/` | build helpers, matching helpers and the report generator (see [Tools](#tools)) |
| `external/` | submodules: maspsx, m2c, decomp-permuter, psyq_headers, doxygen-awesome-css |
| `.github/workflows/build.yaml` | the CI: checks the names and the hacks, builds and compares both versions, uploads their reports |
| `.github/workflows/docs.yaml` | builds the API documentation and publishes it to GitHub Pages |
| `Dockerfile`, `tools/docker.sh`, `.github/workflows/docker.yaml` | the build environment as a Docker image, the script that runs a command in it, and its CI |
| `.clangd`, `.vscode/`, `.devcontainer/`, `.clang-format` | the editor's settings: clangd's, VS Code's tasks and extensions (see [Editor](#editor)), the Docker image as a dev container, and the C's layout for clang-format (see [Formatter](#formatter)) |
| `asm/<version>/`, `build/<version>/`, `expected/<version>/`, `assets/<version>/`, `build/docs/` | generated; not in git |
| `disks/<version>/` | the extracted disc; not in git |

## Tools

| Tool | What it does |
|---|---|
| `tools/dl_deps.sh` | downloads the PSX GCC, objdiff-cli and mkpsxiso into `bin/` |
| `tools/extract_disc.py` | extracts a disc image, `AAA/` included |
| `tools/stage_yaml.py` | writes a stage's splat config from `config/<version>/stages.txt` |
| `tools/stage_areas.py` | moves each stage's C file into the folder of its area, `src/field/stages/<area>/`, from FIELDSTG's stage and area tables, and writes `src/field/stages/README.md` |
| `tools/stage_externs.py` | drops the declarations of a stage's own data and functions that are defined before every use, and says above each block that is left why it is needed |
| `tools/stage_struct_fields.py` | renames the fields of `StageTile` and `StageActor` where the stages and FIELDSTG read them through a pointer of that type |
| `tools/name_stage_data.py` | names the stages' data by its place in the stage's tables, in the C and every version's symbol files (then `make regenerate`) |
| `tools/stage_constants.py` | writes the stages' music, sound ids, story points and flag codes with the names of `include/engine/sound.h`, `include/field/stage.h`, `include/field/field_map.h` and `include/engine/game_state.h` |
| `tools/stage_task_sizes.py` | writes the sizes the stages give `createTask` as `sizeof` of the update's task and children types, where the compiler gives that size |
| `tools/stage_common.py` | includes a `src/field/stages/common/` file in place of a stage's copy of its code |
| `tools/try_match.py` | compiles a draft and compares each of its functions with the original |
| `tools/permuter_import.py` | sets up a [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) directory for one function |
| `tools/data_to_c.py` | turns a splat data file into C definitions that reproduce its bytes |
| `tools/objdiff_generate.py` | writes `objdiff.json` (`make objdiff`) |
| `tools/data_sizes.py` | gives compiled data symbols their ELF size, for objdiff (part of the build) |
| `tools/comm_align.py` | gives common symbols larger than a word a word's alignment, as their definitions have (part of the build) |
| `tools/hacks.py` | counts the fake matches and hacks (`--list`, `--check README.md docs/status.md`) and fails on `NON_MATCHING` code, `#if 0` and inline asm in place of C |
| `tools/check_names.py` | checks that every version's symbol files use the USA version's names |
| `tools/match_versions.py` | pairs a version's functions with the USA ones (`build/<version>/version_pairs.txt`) and, with `--seed`, writes their USA names into the version's symbol files |
| `tools/split_version.py` | splits a version's executable and overlays into the USA version's files, from the pairs |
| `tools/version_symbols.py` | names, at a version's addresses, what a USA C file uses, so that the version can build it |
| `tools/rename.py` | renames a symbol in every version's symbol files, `src/` and `include/` |
| `tools/rename_field.py` | renames a struct field in its definition and wherever the code uses it, from the command line or a spec file |
| `tools/docker.sh` | runs a command in the Docker build environment |
| `tools/doxygen_filter.py` | turns the C's comments that document something into doc comments as doxygen reads them (`make docs`) |
| `tools/doxygen_dirs.py` | describes the source's folders for doxygen, from `src/README.md` and `src/field/stages/README.md` (`make docs`) |
| `tools/version.py` | the version being worked on and its paths, for the other tools |
| `tools/shiftcheck.py` | finds the addresses in the code and data that aren't symbols (`make shiftcheck`) |
| `tools/link_imports.py` | writes what a binary links with from the binaries that load after it (part of the build) |
| `tools/padcheck.py` | checks a padding build: only the words with a relocation may change (`make padcheck`) |
| `tools/inputcheck.py` | checks that a link took in only the binary's objects and no blob (part of the build) |
| `tools/lint.py` | compiles the game C with a modern GCC and fails on its warnings (`make lint`) |
| `tools/compile_commands.py` | writes `compile_commands.json`, the build's flags for clangd (`make compile_commands`) |
| `tools/format.sh` | formats the C with clang-format and `.clang-format`, or lists the files it would change (`make format`, `make format-check`) |
| `tools/fitcheck.py` | checks that a build's binaries fit in the original's files and memory (`make fitcheck`, part of `TOOLCHAIN=gcc`) |
| `tools/smoke.py`, `tools/patch_disc.py` | write a build into a copy of the disc image and boot it in DuckStation (`make smoke`) |
