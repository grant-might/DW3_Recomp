# Binaries and versions

## The game's binaries

The executable, `SLES_039.36` in the European release, `SLUS_014.36` in
the USA one and `SLPS_034.46` in the Japanese one, holds the engine and the
PsyQ libraries. The rest of the game
is in overlays, `AAA/PRO/*.PRO` on the disc, which load right after the
executable's `.bss`. Every engine module is a
global struct holding its state and a table of methods (`GFX`, `HEAP`,
`FILE_CACHE`, `PAD`, `SOUND`, `GAME`...), and every game object is a task
(`createTask`, `include/engine/task.h`); the overlays reach the engine through
those tables. Each overlay has its own splat config, source folder and symbol
prefix (`CNTY_SEL_`, `STDWTITL_`...):

| Overlay | Loads at (us) | Functions in C (us) | What it runs |
|---|---|---|---|
| `CARDGAME` | `0x80082448` | 306 / 306 | the card battle (mode `0x700`): the decks, the cards in play and the battle screen |
| `CNTY_SEL` | `0x80082448` | 26 / 26 | the country select screen |
| `FIELDSTG` | `0x80082448` | 222 / 222 | the field mode, where the player walks around the map; the stages load on top of it |
| `FIGHTSTG` | `0x80082448` | 297 / 297 | the battle: the fight stage and its lights, the fighters' models, faces and cameras, the battle camera and windows, the queue of battle events and the stat, hit and status checks |
| `SHOCKTST` | `0x80082448` | 17 / 17 | the debug vibration test |
| `SOUNDTST` | `0x80082448` | 8 / 8 | the debug sound test |
| `STAGSLCT` | `0x80082448` | 8 / 8 | the debug stage select, a menu of every scene of the game |
| `STCRDABM` | `0x80082448` | 29 / 29 | the card album |
| `STCRDDEK` | `0x80082448` | 55 / 55 | the decks, which it names with the on-screen keyboard (`include/shared/name_entry.h`) |
| `STCRDSHP` | `0x80082448` | 45 / 45 | the card packs (mode 0x1300): opening a pack uses it up and draws six cards, one from each slot's list in `STCRDSHP_packs` |
| `STDGNAME` | `0x80082448` | 32 / 32 | a name entry screen, a keyboard of character pages |
| `STDWTITL` | `0x80082448` | 91 / 91 | the title screen, the opening movies and a notice screen |
| `STFGTREP` | `0x80082448` | 36 / 36 | the report after a battle (mode 0x1400), which `WFIGHTMN` requests: the partners that went up a level |
| `STGDGLAB` | `0x80082448` | 70 / 70 | the partners' digivolutions, it seems: a menu of three screens that checks the recipes of `STGDGLAB_data` against a partner's entries and sets its three slots |
| `STGMCARD` | `0x80082448` | 45 / 45 | the memory card screen (mode 0xC00): the saves of a card, their details, and saving and loading |
| `STGTRAIN` | `0x80082448` | 94 / 94 | the gyms: a partner trains a stat, gaining some and losing others, with its sprites and the result windows |
| `STITSHOP` | `0x80082448` | 69 / 69 | the item shop, where the player buys and sells items and equips what was bought on a partner |
| `STPLNMET` | `0x80082448` | 53 / 53 | the player's name entry (mode 0x500), with a copy of `STDGNAME`'s keyboard |
| `STSTATUS` | `0x80082448` | 123 / 123 | the screens the field menu opens (`STSTATUS_screens`), such as the item list and the equipment |
| `WFIGHTMN` | `0x800A4CA4` | 42 / 42 | the battle's sub-overlay, which `FIGHTSTG` loads (file 0x1FA) for a normal battle: it checks the party and its equipment and ends the battle |
| `WFIGHTTS` | `0x800A4CA4` | 14 / 14 | the debug battle test, which `FIGHTSTG` loads (file 0x1FB) in place of `WFIGHTMN`: lists of fighters, motions, effects and stages |
| `WSTAG###` (238) | `0x800A4CA4` | 1,369 / 1,369 | the stages: small programs that load on top of `FIELDSTG` and call into it |

`SMDLDATA`, `SDIGIEDT` and `SFSTDATA` hold no code and aren't built.
`WSTAG260` has no code either, only the story events' scripts that
`FIELDSTG` runs, and builds from its C data. The disc's `AAA/DAT`, `AAA/PRO` and `AAA/STR` directories are only
reachable through the ISO 9660 path table, which is why
`tools/extract_disc.py` is needed: dumpsxiso doesn't see them.

The executable's game code is in modules (`src/engine/<module>/`, see
CONTRIBUTING.md), cut from its original objects in their link order:

| Original object | Files | Address (us) |
|---|---|---|
| `crt0` | `asm/<version>/main/crt0.s`: PsyQ startup (`2MBYTE.OBJ`), splat's disassembly | `0x80010EBC`-`0x80010F80` |
| `inn` | `menu/inn.c`, `gfx/screen_fade.c` | `0x80010F80`-`0x800120B8` |
| `system` | `menu/field_menu.c`, `game/digimon.c`, `game/items.c`, `file/cd_reader.c`, `file/file_cache.c`, `file/file_table.c`, `task/task.c`, `system/main.c` | `0x800120B8`-`0x80014818` |
| `memcard` | `memcard/memcard.c` (and `system`'s last function, `initMemCard`) | `0x80014818`-`0x800154F8` |
| `game3` | `game/events.c`, `game/state.c`, `game/party.c`, `game/play_time.c`, `game/stats.c` | `0x800154F8`-`0x800172E8` |
| `game3_2` | `game/partner.c`, `system/heap.c`, `task/registry.c` | `0x800172E8`-`0x80017FAC` |
| `pad` | `pad/pad.c`, `pad/demo.c`, `pad/port.c`, `system/random.c` | `0x80017FAC`-`0x80018FEC` |
| `text_window` | `text/text_window.c`, `text/cursor.c`, `file/decompressor.c`, `text/message_box.c`, `text/talk_box.c`, `text/font.c` | `0x80018FEC`-`0x8001D070` |
| `graphics` | `gfx/display.c`, `gfx/layer.c`, `gfx/card_drawer.c`, `gfx/sprite_drawer.c`, `text/text_tools.c`, `gfx/tim_loader.c` | `0x8001D070`-`0x8001FC68` |
| `sound` | `sound/sound.c` | `0x8001FC68`-`0x80020764` |
| `overlay` | `system/overlay.c` | `0x80020764`-`0x80020998` |
| `psyq` | `asm/<version>/main/psyq/`: the PsyQ libraries, splat's disassembly, one file per library object | `0x80020998`-`0x8003E9D8` |

Memory maps (psylink puts `.rodata` in front of `.text`):

| | `us` (`SLUS_014.36`) | `eu` (`SLES_039.36`) | `jp` (`SLPS_034.46`) |
|---|---|---|---|
| `.rodata` | `0x80010000`-`0x80010EBC` | `0x80010000`-`0x80010E88` | `0x80010000`-`0x80010E68` |
| `.text` | `0x80010EBC`-`0x8003E9D8` | `0x80010E88`-`0x8003EDCC` | `0x80010E68`-`0x8003EBF8` |
| `.data` | `0x8003E9D8`-`0x8005C480` | `0x8003EDCC`-`0x8005CCE8` | `0x8003EBF8`-`0x8005CA98` |
| `.bss` | `0x8005C480`-`0x80082448` | `0x8005CCE8`-`0x80082CB0` | `0x8005CA98`-`0x80082A60` |
| `$gp` | `0x8005C2F8` | `0x8005CB50` | `0x8005C90C` |
| Overlays (`OVERLAY_VRAM`) | `0x80082448` | `0x80082CB0` | `0x80082A60` |
| Stages, `WFIGHTMN`, `WFIGHTTS` (`STAGE_VRAM`) | `0x800A4CA4` | `0x800A5DE0` | `0x800A5770` |

A 320x480 16-bit TIM image is loaded together with the program, so the
executable stores `.bss` and the gap after it as zeros; splat keeps that tail
as `assets/<version>/tail.bin`. `STAGE_VRAM` is right after `CARDGAME`, the
largest overlay.

## How the versions are organised

One source tree builds every version, one at a time, picked with `VERSION`
(`eu` by default):

| `VERSION` | Release | Executable (SHA-1) | Disc image (SHA-1) | Overlays | Stages | C |
|---|---|---|---|---|---|---|
| `us` | *Digimon World 3*, USA, SLUS-01436 | `SLUS_014.36` (`444653259f78ddb483fd22af72cce9276f42f214`) | `Digimon World 3 (USA).bin` (`f0b022f9be53cbce14640abd8f01beaadcb35208`) | 21 | 238 | yes |
| `eu` | *Digimon World 2003*, Europe, SLES-03936 | `SLES_039.36` (`d1b7e4d646e3a9c2b88fdb25d20b5f7116bbb06d`) | `Digimon World 2003 (Europe).bin` (`457cb233349ba841e03b33d8060f8fbcadd45cb3`) | 21 | 293 | the USA version's, and its own stages' |
| `jp` | *Digimon World 3: Aratanaru Bouken no Tobira*, Japan, SLPS-03446 | `SLPS_034.46` (`0e5b6b9ee2f99a804ed8c4117c805482c10e260b`) | `Digimon World 3 - Aratanaru Bouken no Tobira (Japan).bin` (`8594fb44f601da64087a8996ffd89ab26abb11ea`) | 21 | 293 | none yet: splat's assembly |

- `mk/version/<version>.mk` has each version's settings: the release's name,
  the executable's name, the disc directory, the overlays, where they load
  (`OVERLAY_VRAM`, `STAGE_VRAM`) and the C files it builds (`C_SRC`). The
  Makefile builds nothing else. `tools/version.py` reads the same file, so
  every tool follows `VERSION` too.
- `config/<version>/` has its splat configs (`main.yaml`, `<overlay>.yaml`),
  symbols (`symbols.txt`, `symbols_<overlay>.txt`), the list of stages
  (`stages.txt`) and the checksums (`<executable>.sha1`, `overlays.sha1`,
  `stages.sha1`). The generated `asm/<version>/`, `build/<version>/`,
  `expected/<version>/` and `assets/<version>/` are kept apart too.
- The stages have no config file each: `config/<version>/stages.txt` lists
  them with the offsets where their code starts and ends, and
  `tools/stage_yaml.py` writes their splat configs into
  `build/<version>/generated/stages/`. A stage marked `asm` there keeps its
  code and data in asm segments until it has a C file in that version; one
  marked `asm-data` only its data: no stage of any version is marked
  either now.
  `c-rodata` takes the bytes before the code, a color or jump tables, from
  the stage's C file too. `head-word` takes `WSTAG924`'s first word from a
  C file of its own, `src/field/stages/amaterasu_city/wstag924_head.c`, linked
  before the stage's jump tables (which GCC would align to 8 bytes after
  it). `data`
  marks the stage with no code, `WSTAG260`, all of it its C file's data
  (`data asm`: splat's assembly). The last bytes of a stage that ends inside
  a word are a databin of their own while its data is asm, which
  `tools/inputcheck.py` lets link as it is shorter than a word.
- The C sees `VERSION_US`, `VERSION_EU` and `VERSION_JP`, each 0 or 1
  (`include/version.h`), and so does the assembly (`--defsym`). Code tests
  them with `#if VERSION_EU`, never `#ifdef`; CONTRIBUTING.md has the rules.
- `us` builds every C file under `src/`, and `eu` builds them all too
  (`C_SRC`), with `#if VERSION_EU` blocks where its code or data differ.
  Its executable and overlays are split into the USA version's files, so
  its asm lands at the same paths (`asm/eu/main/game/events.s` for
  `asm/us/main/game/events.s`). Its stages that the
  USA version has build from their C files (`config/eu/stages/<stage>.txt`
  gives their functions the USA names), and so do its own, whose functions
  are C where they are the code of a USA stage's C. The European release
  has the USA one's 21 overlays and 238 stages plus 55 stages of its own
  (`WSTAG920`-`974`).
- `jp` builds every C file too, with `VERSION_JP` in the tests of the
  blocks: the European release was made from the Japanese one, so the
  Japanese code and data are mostly the European ones (its 55 extra stages,
  the scripts the USA version left as they were), with `LANGUAGE` 0 and no
  PAL video, at the USA disc's file numbers (`include/engine/file.h`), its
  text the Japanese copies of the USA text files. Its executable and
  overlays are split into the USA version's files (`tools/split_version.py`),
  its `WSTAG920`-`974` take the European names
  (`config/jp/stages/wstag9NN.txt`). Only the PsyQ SDK and `libpress`
  (asm by policy) and `crt0` are assembly. The Japanese release has the
  same overlays and stages as the European one, each a little different in
  size.
- The versions share their names: the European and Japanese symbol files
  hold the USA names of the functions and data paired with the USA version's
  (`tools/match_versions.py`), and `tools/check_names.py` checks that they
  stay the same.
