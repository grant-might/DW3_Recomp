# TODO

The game is all C in both versions: the executable's game code and data,
the 21 overlays and the 293 European and 238 USA stages build byte for byte
from the same source (3,606 functions in the European version, 3,380 in the
USA one, code and data 100 %), with no fake matches. The PsyQ libraries,
Sony's code, stay splat's disassembly and out of the progress
([docs/status.md](docs/status.md)). What is left is making the C read well,
and the tooling around it. The counts below are from commit `f5763800`.

## Names and types

- [ ] 30 struct fields in `include/` keep an `unk` name: `fieldstg.h` (7),
  `stgtrain.h` and `stage.h` (5 each), `name_entry.h` and `fightstg/model.h`
  (4 each), `fightstg/effect.h` (2), and one each in `ststatus.h`,
  `field_map.h` and `cardgame/screen.h`. Every other field has a name or is
  padding (`pad`, "never read or written"). These are written or copied but
  never read, or hold a constant, and their comments give what is known
  (the values on the disc, who sets them); name one when a reader turns up.
- [ ] 179 pointer casts are left in `src/` (72 in FIGHTSTG, 33 in the
  executable, 14 in FIELDSTG, the rest a few per overlay). Each one says
  why it stays (another form changes the code) or is plain, like a byte
  offset, a PsyQ primitive or a `Task *`.
- [ ] 33 functions are longer than 120 lines. Each was split where a named
  static inline keeps the bytes; the rest of each resists, because a
  split changes the register allocation or the stack frame (the draw
  functions, `FIELDSTG_runActorAction`, `CARDGAME_checkPlayCondition`,
  `spriteDrawerDraw`, `textWindowDraw`...), or is a flat switch of short
  cases (`FIGHTSTG_showMessage`, `FIGHTSTG_testEnemyCondition`).
- [ ] Each stage's file says which map or event it is, and
  [src/field/stages/README.md](src/field/stages/README.md) lists the names
  (`tools/stage_names_doc.py` writes both). The 30 Underground tunnels
  (WSTAG825 to 895, and the European 960 to 974) only have the game's names
  for their region, Seabed or Circuit Board, and the maps that lead to them:
  name them better if their maps or events tell more.

## Shared code

- [ ] Near duplicates are left as copies, because their code differs:
  - in the stages, `isFarFromHome`, `moveWanderer` / `moveWanderPair`, the
    `handleCommand*` sets, the `fadeOut*` pairs, `updateTileSixW` (its
    task types differ between WSTAG310 and 311) and the list menus' drawing
    (their steps are `common/list_menu.inc.c`);
  - the `stepTileAnimationN` copies, whose recursive calls name each copy.
  Share one when a parameter or a macro can give each copy its difference
  with the bytes unchanged.

## Data

- [x] The non-zero padding halfword the original has after 39 event scripts
  in 34 stage files (56 top-level `__asm__` statements, one for each version
  that has it) is now the script's last element, under the same `#if`: no
  data is written as asm any more.
- [ ] The stages' data repeats between stages, and between the versions as
  `#if VERSION_US` / `VERSION_EU` rows: write what is the same once, where
  the bytes allow it.
- [ ] `src/engine/data/` holds the executable's data no module can hold yet,
  each file with the reason: `matrices.c` (nothing in the executable reads
  it, between `system/random.c`'s data and `text/text_window.c`'s),
  `heap_start.c`, `get_digimon.c`, `eu_settings.c` and `all_files_pattern.c`
  (small data whose readers reach it through its address, so not theirs,
  and whose place in the link order names no module), and `game_bss.c`
  (commons that the original's linker laid out 8 bytes apart, and
  `CD_SECTOR_HEADER`, which needs `file/cd_reader.c`'s commons to leave
  them). Move a piece when new evidence names its module.

## Tooling and docs

- [ ] The README's status table and the overlay table in
  [docs/binaries.md](docs/binaries.md) are written by hand from `make
  report`. A script in `tools/` should write them and the CI should check
  them, as it does the hacks badge (`tools/hacks.py --check`).
- [ ] decomp.dev's data total counts the `.bss` (139,344 bytes in the
  European version): decide whether the report should leave it out.
- [ ] Pull requests from forks only run the `names` job (`check_names.py`
  and `hacks.py`): the builds need the private repository with the original
  files.
- [ ] `objdiff.json` holds one version at a time: the last one `make
  objdiff` was run for.
- [ ] `make PAD=0x10004 smoke` ([docs/shifting.md](docs/shifting.md)) boots a
  shifted build in DuckStation; run it again on the current code.
