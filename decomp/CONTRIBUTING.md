# Contributing

The conventions follow the [Digimon World decomp](https://github.com/jype0/dw_decomp)
and the [Digimon Digital Card Battle decomp](https://github.com/ReGame-Labs/dcb_decomp),
so that the Digimon decomps read the same way. The README explains how to
set up and build the project; this file is about the work itself.

## Decompiling a function

Every function that isn't C yet is an `INCLUDE_ASM` line in its file under
`src/`, which includes splat's disassembly of it:
```c
INCLUDE_ASM("main/nonmatchings/graphics", convertText);
INCLUDE_ASM("stages/nonmatchings/wstag210", func_800A4D38);
```
The folder is relative to `asm/<version>/`, so the function above is
`asm/us/main/nonmatchings/graphics/convertText.s` after `make generate`.

1. **Get a first draft.** m2c turns the `.s` into C to start from:
   ```
   python3 external/m2c/m2c.py asm/us/main/nonmatchings/graphics/convertText.s
   ```
   Give it `--context` with a preprocessed copy of the file's headers to
   get the project's types and names in the output. The draft is a starting
   point: give it the real types, the struct fields and the engine calls of
   the code around it (see [Code](#code)).
2. **Make it match on its own.** Put the draft in a file of its own that
   includes the same headers as its unit, and compare it with the original:
   ```
   tools/try_match.py draft.c convertText
   ```
   It compiles the draft with the project's compiler, finds the function in
   `asm/<version>/` and prints `MATCH`, or both versions side by side with the
   differing instructions marked `**`. Relocated fields are masked, so a
   different symbol name doesn't count as a difference. Set `UNIT=wstag210`
   (any part of the path) when several units have a function of that name, as
   the stages do; `VERSION` picks the version. For a `-G8` file (`G8_SRC` in the
   Makefile), pass the same flags as the Makefile: `CFLAGS='-O2 -G8 -fsigned-char -fno-builtin
   -fdollars-in-identifiers' MASPSXFLAGS='--aspsx-version=2.86 -G8'`.
3. **Search for a near miss.** When only register allocation or instruction
   order is left, try other source shapes first: types, the order of
   statements, a temporary, a loop written another way. Then the permuter
   can search for you:
   ```
   tools/permuter_import.py draft.c convertText
   python3 external/decomp-permuter/permuter.py permuter/convertText -j8
   ```
   The permuter's finds are hints, not answers: keep only what reads as C
   someone would write (see [Matching](#matching)).
4. **Put it in place.** Replace the `INCLUDE_ASM` line with the C, add the
   prototypes and types it needs to the headers, rebuild and check:
   ```
   make -j$(nproc) && make compare
   make VERSION=us -j$(nproc) && make VERSION=us compare
   make VERSION=jp -j$(nproc) && make VERSION=jp compare
   ```
   Every line of every version has to say `OK`.
5. **Check it in objdiff.** `make objdiff` and the objdiff GUI show the
   function and its data against the original inside the whole unit;
   `make report` writes the progress that decomp.dev will show.

Rodata is migrated into the functions that use it: a function's jump tables
and strings live in its own `.s` file, so its C version emits them itself, and
switch statements work as usual. Rodata shared by several functions stays
behind `INCLUDE_RODATA` until all of them are C. Data that splat left as
`.word`s becomes C with `tools/data_to_c.py`, which reproduces the bytes;
give it real types once the code that uses it is understood.

## Matching

- A change only counts if `make compare` still says `OK` for every binary of
  every version: the executable, the overlays and the stages have to stay
  byte for byte identical. Run it for `us`, `eu` and `jp` before opening a
  pull request; the CI runs them all.
- Only byte-identical matches go in. No `NON_MATCHING` code, no `#if 0`
  blocks, no inline assembly in place of C, and no tricks that wouldn't pass
  review. A function that doesn't match yet stays behind its `INCLUDE_ASM`; a
  draft that came close can go in the pull request's description, not in the
  source. `tools/hacks.py` fails the CI on `NON_MATCHING` (or `NONMATCHING`),
  on `#if 0`, on inline asm in a function's body, on a register variable
  pinned with `asm("$reg")` and on any top-level asm: data, padding
  included, is written in C.
- A fake match is the last resort, not a shortcut: only for a function that
  natural C has failed to match after a real search (other source shapes,
  types, statement order, the permuter's legitimate finds), a forced form is
  allowed, such as an empty `do {} while (0)` used as a CSE or scheduling
  barrier, a variable reused for an unrelated job, or a local that only
  shapes the stack frame. Mark the exact spot with a comment that starts with
  `/* fake match:` and gives, in one or two lines, what is forced and why:
  the compiler decision it reproduces. Example: `/* fake match: the empty
  loop ends a CSE block, so GFX's address is loaded again, as in the
  original */`. A forced form without its reason doesn't pass review, and
  the rest of the function stays readable C.
- A BEC form is a one-off exception the maintainer grants for a single
  function: a form these rules don't take elsewhere (such as a breakless
  `do { } while (0)`), allowed because Digimon World 2's matched source, by
  the same developer with the same compiler, has it. Its comment starts with
  `/* BEC form:`, says why the match needs it and names the Digimon World 2
  function with the same form; `tools/hacks.py` counts these apart from the
  fake matches.
- Two lesser workarounds have a standard marker of their own, so that
  `tools/hacks.py` can count them for the README's badge and the table in
  [docs/status.md](docs/status.md#fake-matches-and-hacks):
  - a local that nothing reads or writes, kept because the original's stack
    frame has room for it, ends its declaration with exactly
    `/* unused, but it is in the original stack frame */`
    (`MATRIX unused; /* unused, but it is in the original stack frame */` in
    `libgs_gs_131.c`). Anything more to say about it goes in a comment of its
    own above it. A local that makes no difference to the output is deleted
    instead.
  - C that only matches in one of several equivalent forms (an extra block,
    an `if` without braces, a copy of a variable, a type, one version's own
    form of a loop) has a comment that says the `match depends on` that
    form, and why: `/* kept on one line: the match depends on it, since GCC
    2.8.1's line notes decide where the index is computed */`.

  After adding or removing a fake match or one of these, run
  `tools/hacks.py` and update the README's badge and the table in
  `docs/status.md` to its counts: the CI runs
  `tools/hacks.py --check README.md docs/status.md`. `tools/hacks.py --list`
  lists them all.
- When a form recurs and has a likely origin, give it a name and say so
  once, as `COUNTDOWN_BORROW` in `include/field/stage.h` does: a statement macro
  for the timed stages' countdown, whose `do { } while (0)` the match
  depends on. Say it once for a form every copy shares too, as the comment
  above `StageInfo` does for the setup functions' `(Vec2){x, y}`.
- Give a function pointer the return type of the function it points to. A
  call through a pointer declared to return a value is a `call_value` whose
  result goes to `v0`, so the local allocator suggests `v0` for the
  register that holds the call's address. That moves registers well away
  from the call: `func_800928BC` was 68 diffs until `Battle.unkF0` was
  declared `void`, as `func_8009DA88` is. A callee that leaves nothing for
  its caller in `v0` at its `jr $ra` returns `void`.
- Code that was written in assembly, not compiled, stays as assembly. Say in
  a comment what shows it is hand-written (things no compiler emits). An
  object that is all assembly stays an `asm` segment of the splat config.

## Shifting

The build has to stay shiftable: every address in the code and data a
symbol, so that code that changes size still links into a working game.
Three checks guard it, and the CI runs them for both versions;
[docs/shifting.md](docs/shifting.md) describes them, and this section says
how to fix what they find:

- `make shiftcheck` (`tools/shiftcheck.py`) fails on an address that is a
  number ([docs/shifting.md](docs/shifting.md#shiftcheck) lists the
  kinds). It prints each one with where it is and what it points to;
  `make shiftreport` lists them all. Fix the source:
  name what the address points to (a declaration of the symbol, or a name
  in the symbol file so that splat writes a label), and for a binary that
  loads later, use its symbol as the binary imports it (`CHILDREN_<binary>`
  in the Makefile: `WSTAG931_startStage`, `include/field/stages.h`).
  `config/<version>/shiftcheck.txt`, the addresses left to fix, is empty and
  may only get shorter; when the check warns that a listed one is fixed,
  `tools/shiftcheck.py -v <version> --update` drops it.
- `make padcheck` (`tools/padcheck.py`) links every binary again with the
  memory map moved up by 0x4 and by 0x10004, and fails on a section whose
  size changed or a word that changed without a relocation, with the label
  before it. Something there depends on where the code is, such as a pad
  or an alignment: write it so that the layout is the same at any address.
  It also fails on a relocation's absolute symbol, with the name of
  something in a binary, that didn't move: a symbol file gives that name's
  unpadded address. Drop the name from the file, so that the binary takes
  it from the executable's or the parent's symbols.
- `tools/inputcheck.py`, which every link runs, fails on an object that
  isn't one of the binary's or on a blob. The first is a stale object in
  `build/` (`make regenerate` after changing a config) or one that a splat
  config names and the Makefile doesn't build (add its source to the
  version's `C_SRC`). A blob, a `.bin` or assembly that `.incbin`s one, has
  no relocations: make it data (C, or splat's words) so that its pointers
  get them. Only the executable's tail, a picture, may stay one.

`make smoke` (`tools/smoke.py`) is an optional check for your machine, never
the CI's: it writes the build into a copy of your disc image under `build/`
and boots it in DuckStation under `xvfb-run`, with settings of its own,
until the first menu (the title screen, or the European language menu),
then watches it for an exception or code that runs outside the BIOS, the
kernel and the build.
`make PAD=0x10004 smoke` boots a padding build, which shows that the code
and data that moved still work, and `make TOOLCHAIN=gcc smoke` the modern
GCC's ([docs/toolchain.md](docs/toolchain.md#modern-gcc-build)), whose
files may be smaller than the original's and are padded to them. It needs `DISC=` (the original image, a
`.bin`), `BIOS=` (a directory with a PlayStation BIOS) and, if DuckStation
isn't `duckstation-qt`, `DUCKSTATION=`, on the command line or in
`local.mk`.

## Versions

The same source builds every version of the game (`make VERSION=eu`, the
default, `us` or `jp`). The Makefile passes one `-DVERSION_<VERSION>`, and
`include/version.h` (through `common.h`) makes `VERSION_US`, `VERSION_EU`
and `VERSION_JP` all defined, each 0 or 1; the assembly gets the same names
from `--defsym`.

- Test a version with `#if`, never `#ifdef` or `defined()`: `#if VERSION_EU`.
  A misspelt name is then a `-Wundef` warning instead of silently false.
- A condition names the versions it is for: `#if VERSION_US || VERSION_EU`,
  not `#if !VERSION_US`, so that a version added later doesn't fall into a
  branch nobody checked for it. The versions have no order: no
  `VERSION >= ...` or "newer than" tests.
- Only name a version you have checked. While a file is still asm in some
  version, its `#if` blocks name only the versions that build it from C.
- Each version lists the C files it builds in `mk/version/<version>.mk`
  (`C_SRC`), and the Makefile builds nothing else. `us` builds every C file
  under `src/`; `eu` and `jp` the ones they share (all of them so far) and
  the extra stages'. Their executables and overlays are split into the USA
  files with the same names (`tools/split_version.py`), as asm segments
  until a file is C in that version.
- To build a file for `eu` or `jp` too: add it to the version's `C_SRC`,
  make its segments in `config/<version>/` `c` (with `.rodata` and `.data`
  when those are C in `us`), then name what the C uses at the version's
  addresses:

  ```
  tools/match_versions.py eu      # with both versions built: build/eu/version_pairs.tsv
  tools/version_symbols.py eu src/<folder>/<file>.c --write
  make VERSION=eu generate build/eu/src/<folder>/<file>.c.o
  tools/version_symbols.py eu src/<folder>/<file>.c --write
  make VERSION=eu regenerate && make VERSION=eu && make VERSION=eu compare
  ```

  The first run names the functions and rodata the C includes as asm, so
  that splat writes their files; the second, after the object builds, names
  every function and datum it reads from where the original has them.
  `version_symbols.py` lists what it can't name: a name whose reads
  disagree, or one of splat's names from `us` (`D_80081E20`) that the
  version's asm has at another address. For a stage, drop the `asm` mark of
  its line in `config/<version>/stages.txt` once it has a C file there.
- A file whose contents differ throughout between versions gets one copy per
  version instead of an `#if` around all of it, named after the version
  (`<module>_eu.c` next to `<module>.c`).
- A function only one version has needs its own line in that version's
  `config/<version>/symbols_<binary>.txt` when the C names it
  (`FIGHTSTG_startCameraShots = 0x800A246C; // type:func version-only`):
  without it the asm keeps splat's `func_` name, the binary still matches by
  checksum, but `make report` counts the function as unmatched.

## Code

Look at the C around you and write the same way. What the existing code
does:

- Four spaces, no tabs; the opening brace on the same line; braces around
  every `if`, `for` and `while` body, even a one-line one.
  `.clang-format` has this layout for clang-format 18:
  `make format FILES=<file>` lays out a file you've changed
  ([docs/toolchain.md](docs/toolchain.md#formatter)).
- Comments are `/* */`. A function gets a one-line comment above it that
  says what it does or returns (`/* Advances a panel animation; 1 once it has
  finished */`), and a block of code a comment for what isn't obvious from
  the code: a magic number, a file id, a mode.
- The game code uses the types of `include/common.h` (`s8`, `u8`, `s16`,
  `u16`, `s32`, `u32`) and the PsyQ types where it talks to the SDK (`RECT`,
  `CVECTOR`, `u_long`).
- Struct fields keep their offset in a comment, and fields that aren't
  understood yet are named by it:
  ```c
  typedef struct StageTask {
      TASK_HEADER(StageTask);
      /* 0x50 */ void *owner;
  } StageTask;
  ```
  A task's struct starts with `TASK_HEADER(Type)` (`include/engine/task.h`),
  and its state machine uses `TASK_INIT`, `TASK_RUN`, `TASK_DONE` and
  `TASK_KILL`.
- The engine is reached through its tables, as the original does:
  `SOUND.playSound(...)`, `GFX.funcs.createLayer(...)`,
  `FILE_CACHE.load(...)`, `GAME.funcs.getPartyMember(...)`, or a task's own
  methods, `task->nextState(task)`. Declare a global function pointer table
  as a struct or an array, never as a scalar `extern`: GCC 2.8 moves stores
  to struct fields past the load of a scalar's function pointer.
- In a `-G8` file, the small variables the code reads through `$gp` are
  declared `static` at the top of the file.
- Headers: `include/engine/game.h` includes the engine's headers, one per module in
  `include/engine/<module>.h`, each with its own types and prototypes;
  `include/<group>/<folder>.h` has an overlay's (in the groups of `src/`),
  and `include/field/stage.h` what the stages share. A large overlay's are
  split by module into `include/<group>/<folder>/*.h` (FIGHTSTG, CARDGAME):
  `types.h` first, with the types the others point to before they are
  defined, and `include/<group>/<folder>.h` includes them all. Every header has an `#ifndef <NAME>_H` guard, and most a
  comment at the top that says what the module or overlay is.
- Every function that isn't `static` has its prototype in the header of
  its module or overlay, next to those of the functions defined around it,
  even when only its own file calls it (`make lint` checks it). A function
  defined old-style is declared with the promoted types its callers pass
  (`s32` for an `s16`), the types it narrows to in a comment. The stages
  are the exception: a stage is one file, and its functions keep splat's
  names, the same in many stages, so it declares one only where a use
  comes first. A `.c` file keeps the externs only it uses at its top,
  after the includes; anything a second file needs moves to a header.
- A `case` that runs into the next one ends with `/* fallthrough */`, at the
  indentation of its statements. `make lint` checks this too, and that
  nothing declares a local or a label it doesn't use, except for the
  marked stack frame locals, which its list keeps.

## Layout

- One folder per binary under `src/`: `src/engine/` for the executable,
  `src/<group>/<overlay>/` for each overlay, grouped by the part of the game
  it runs (`field/`, `battle/`, `cardgame/`, `menus/`, `debug/`) and named
  after what it is: `src/menus/item_shop/` is STITSHOP.PRO
  ([src/README.md](src/README.md) lists them with their files on the disc).
  The disc's name stays the binary's: its config (`config/<version>/stitshop.yaml`,
  whose `src_path` gives the folder), its asm, its functions' prefix
  (`STITSHOP_`) and its unit in the report. `src/field/stages/` has one
  `wstag###.c` per stage, in the folder of the area FIELDSTG names when
  the player enters it (`src/field/stages/asuka_city/wstag210.c`): the 18 areas
  and their stages are listed in [src/field/stages/README.md](src/field/stages/README.md),
  which `tools/stage_areas.py` writes as it moves a stage there.
  `wstag260.c` (the story events' scripts, in no area) and `common/` stay in
  `src/field/stages/`. A stage includes the shared code as `"common/<file>.inc.c"`
  (`-Isrc/field/stages`).
- Code that several binaries have, each its own copy of the same C, is one
  `.inc.c` file that their C files include where the function is, named
  after it: `src/field/stages/common/` for the stages, `src/shared/` for the
  menu overlays (included as `"shared/<file>.inc.c"`, `-Isrc`) (and STFGTREP's and FIGHTSTG's copies of their functions). An overlay's copy keeps the overlay's prefix: its header
  defines `OVL_NAME(name)` (`STGMCARD_##name`), and the shared file names
  its functions with it (`void OVL_NAME(drawFader)(ScreenFade *task)` is
  `STGMCARD_drawFader` in STGMCARD). A copy that differs in a constant
  takes it from a macro that the including file defines just before the
  include (`FADER_DEPTH` for `create_fader.inc.c`).
- The executable is split into modules, a folder each under `src/engine/`:

  | Folder | What | Header (`include/engine/`) |
  |---|---|---|
  | `system/` | the boot and the frame loop (`main.c`), the heap, the random numbers, the modes' overlays | `overlay.h`, `heap.h`, `random.h` |
  | `task/` | the tasks' methods and creation, the task registry | `task.h` |
  | `file/` | the CD reader, the file cache, the disc's file table, the RLE decompressor | `file.h` |
  | `gfx/` | the display and the frame, the layers, the card, sprite and TIM drawers, the screen fade | `gfx.h` |
  | `text/` | the text windows, the cursor, the message and talk boxes, the font, the text tools | `text.h` |
  | `pad/` | the controllers and the demos | `pad.h` |
  | `sound/` | the sound banks | `sound.h` |
  | `memcard/` | the memory card saves | `memcard.h` |
  | `game/` | the game state and modes, the events, the party, the stats, the partners, the Digimon and the items | `game_state.h` |
  | `menu/` | the inn and the field menu | `menu.h` |

  The original's objects (`inn`, `system`, `memcard`, `game3`...) are cut
  into these files in their link order: each file is a run of the original's
  code, and its rodata and data are runs of the original's too, in the same
  order. `config/<version>/main.yaml` lists each file's `c`, `.rodata` and
  `.data` subsegments, so a file can only move or split where all three stay
  in order (and a datum that starts a file is word-aligned). The modules cut
  from a `-G8` object keep `-G8` (`G8_SRC` in the Makefile). Each module
  declares its types, functions and data once, in its header;
  `include/engine/game.h` includes them all.
- The SDK, Sony's code, isn't in `src/`: the build takes it from the
  original as splat's disassembly (`asm/<version>/main/psyq/`), and the C
  decompiled of it earlier is in this repository's history.
- An overlay's file `X_2.c` is the second half of an original object that
  the splat config splits in two; the report counts both halves as the unit `X`.
- The executable's data is in `src/engine/data/` until it moves to the module
  that defines it. Its European tables differ all over (file numbers,
  screen positions, overlay addresses) and splat names them at other
  addresses: they are `data_to_c.py`'s output for `asm/eu/` next to the USA
  ones, in `#if VERSION_US`/`#elif VERSION_EU` blocks for the objects that
  differ. Give a table the same name in both versions when you name it.
  A module's data moves from its first datum on, as a whole or up to a
  datum where the rest can wait: give it a `.data` subsegment
  of its own in both versions' `main.yaml` (`[0x2F1D8, .data, menu/inn]`).
  What is left on each side stays
  in `src/engine/data/`, one file per range, listed in address order in
  `GAME_DATA` (`tools/objdiff_generate.py`) and in `mk/version/eu.mk`. A
  struct that splat sees as several labels (the code reads some of its
  fields by their own names) needs its size in the symbol files, and those
  names become fields (`PAD_INIT[0]()` is `PAD.init()`). A `-G8` module
  can only take objects larger than 8 bytes: a smaller one would go to
  `.sdata`. So a small datum of a `-G8` module that the original has in
  `.data` is a field of a larger object: `GAME_MODE` is `GAME.mode`, and
  `GAME` (`GameState`) runs from the save data to its methods.
  A table that the code indexes from 1 starts at its first entry and is
  read as `&ITEMS[id - 1]`: GCC folds the -1 into the address, and splat
  names that address after the object it falls in. A size in the symbol
  files also makes splat read other data's words in that range as
  addresses: a constant that only looks like one gets an `ignore:true`
  entry in its overlay's symbol file (`SOUNDTST`'s sound ids).

## Names

A name has to come from evidence: the strings a function uses, the SDK calls
it makes, its callers, the data it reads. What isn't understood yet keeps its
address (`func_800A4D38`, `D_800A6EAC`, `unk14`).

| What | Style | Examples |
|---|---|---|
| Functions | camelCase, a verb that says what the function does | `createTextWindow`, `startPanel`, `updatePanel`, `tryAllocMem` |
| Globals and tables | UPPER_SNAKE | `FIELD_MENU_LAYOUT`, `PAGE_STATS`, `ROOT_TASK` |
| Engine modules | UPPER_SNAKE, a struct of state and methods | `GFX`, `HEAP`, `FILE_CACHE`, `PAD`, `SOUND`, `GAME` |
| Types | PascalCase | `StageTask`, `FieldMenuView`, `PanelAnim` |
| Struct fields | camelCase, with the offset comment kept | `/* 0x58 */ s32 cursor;` |
| Parameters and locals | camelCase, the same word for the same thing everywhere | `task`, `layer`, `win`, `id` |
| Overlay functions and data | the overlay's name in capitals first, then camelCase | `CNTY_SEL_tickScreen`, `STDWTITL_startLogoTask`, `CNTY_SEL_screenRect` |
| Strings | `[OVERLAY_]STR_` or `PATH_`, then the words | `STR_NULL_MESSAGE`, `SHOCKTST_STR_SLOW`, `SHOCKTST_PATH_DLSKDATA_TXT` |

Verbs: a task's per-frame update is `tick*`, the function that creates it
`start*Task`; `draw*` draws, `create*` sets up and returns an object,
`show*`, `init*`, `load*`, `get*`/`set*`, `is*`/`has*`.

Every name goes in the version's symbol file, so that splat's disassembly of
the original uses it too and objdiff keeps pairing the functions:
`config/us/symbols.txt` for the executable, `config/us/symbols_<overlay>.txt`
for an overlay, with `// type:func` for a function and `// size:0x..` for
data whose size splat can't tell. Group related symbols under a comment that
says what they are, as `symbols_cnty_sel.txt` does. Then
`make regenerate`, so that no file under the old name stays in `asm/us/`.

The stages are all loaded at the same address and many have functions at the
same addresses, so their functions keep splat's names for now; the
Makefile already reads a stage's own symbol file,
`config/<version>/stages/<stage>.txt`, when there is one. Their data is
named after where the tables of `include/field/stage.h` put it (`actor3Talks`,
`area0Battle2`, `script50`): `tools/name_stage_data.py` names it in the C
and in both versions' symbol files, and can be run again after new data
comes in.

The versions share their names: a function or datum is called the same in
every version, each at its own address, and a name in `eu`'s or `jp`'s
symbol files means what that name means in `us`'s. The CI runs
`tools/check_names.py`, which fails when a name in their symbol files isn't
`us`'s name in the same binary (a function there if it is one here), or is
named twice. splat's automatic names (`func_`, `D_`) and the binaries `us`
doesn't have (the stages `WSTAG920`-`974`) aren't checked. So a rename
touches every version's symbol files the same way: `tools/rename.py OLD NEW`
renames in every version's symbol files, `src/` and `include/` (an overlay's
name keeps its prefix), then `make VERSION=<version> regenerate` each
version.

- A name only one version has, for its own code, says so with
  `version-only` in its comment:
  `func_name = 0x80012345; // type:func version-only`.
- Code that `us` has in another binary is given `us`'s name, with
  `us-<binary>` in the comment (`us-main` for the executable, `us-cardgame`
  for an overlay, `us-wstag200` for a stage); check_names checks it against
  that binary's names in `us`:
  `drawWindow = 0x800A5123; // type:func us-main`.

The European and Japanese versions' names come from `us`:
`tools/match_versions.py eu` (or `jp`) pairs their functions with the USA
ones (by their instructions with the addresses masked, their order, their
calls and their strings) and the data they read, and `--seed` writes the USA
names of the confident pairs into `config/<version>/symbols*.txt`, between
the lines it marks, keeping the names written by hand. Name a function in
`us` and run it again to give the other versions the name too; a function
whose code differs too much to pair is named by hand, above the seeded
block.

## Clean code

Every change follows these rules, after the match: nothing goes in that
breaks a function or datum in either version.

- **Names tell the truth.** Name only what the evidence supports, and keep
  `unkXX`, `func_` and `D_` rather than guess. One word for one concept
  everywhere (`task`, `win`, `entry`).
- **No magic numbers.** Use the named constants (`PAD_*`, `TEXT_*`,
  `SOUND_*`, `STAT_*`) and `sizeof`. A number whose meaning is known gets a
  constant or enum in its owner's header; one that isn't stays as it is.
- **Types instead of casts.** Real struct members, unions for two views of
  the same memory, the right return types. A cast that stays says why.
- **One declaration per symbol**, in the header of the module that defines
  it, which its users include. No copies of externs or prototypes in `.c`
  files or in other modules' headers.
- **Helpers only where they exist.** A static inline function or a macro is
  fine when it matches and reads better; a matched function isn't
  restructured for style at the risk of its match.
- **Comments say why.** Each function has a short comment on what it does in
  the game; a "the match depends on" comment stays, short and precise.
  Stale comments, commented-out code and TODOs go.
- **No dead code or fake matches**: no junk temporaries, unread locals,
  `volatile`, an empty `do`/`while (0)` or one without a `break` used as a
  barrier, or branches with identical arms.
- **The surrounding style** (see Code), with `NULL` for pointers and `0` for
  integers.
- **Renames by script.** A rename is made with a script that can be run
  again, so that other branches can apply it after a rebase; each commit
  has one theme.

## Commits and pull requests

- Commit messages are one short sentence in the imperative, without a
  trailing period: "Decompile the stage progress setters", "Declare the
  FIELDSTG field state", "Let permuter_import pick the stage a function
  belongs to". Add a body when the change
  needs explaining: what was found, why a form matches. No trailers.
- A pull request is small and about one thing. Matches, renames and moving
  code around go in separate pull requests: decomp.dev shows a renamed or
  moved function as broken under its old name and new under the new one.
- A pull request that renames things lists every rename in a table
  (address, old name, new name).
- Before opening one, run `make compare` for both versions; both must print
  only `OK`. Run `tools/hacks.py --check README.md docs/status.md` and
  `tools/check_names.py` too, which the CI runs first. The CI builds and compares `eu` and `us`, and uploads both
  reports that decomp.dev reads.
- Pull requests are squash-merged, titled "Title (#N)": "Build the overlays
  and decompile CNTY_SEL (#12)". The title says what the pull request does,
  like a commit message.
