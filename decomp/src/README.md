# The source

One folder per binary, grouped by the part of the game it runs, each named
after what it is. The disc's name stays the binary's: its splat config
(`config/<version>/<binary>.yaml`, whose `src_path` is the folder), its asm
(`asm/<version>/<binary>/`), its functions' prefix (`STITSHOP_`) and its unit
in the report. `include/` has the same folders.

| Folder | On the disc | What it is |
|---|---|---|
| `engine/` | SLES_039.36, SLUS_014.36 | the executable: one folder per module |
| `field/field_mode/` | FIELDSTG.PRO | the field mode, where the player walks around the map |
| `field/stages/` | WSTAG###.PRO | the stage overlays, in the folders of their areas ([the list](field/stages/README.md)) |
| `battle/battle_mode/` | FIGHTSTG.PRO | the battle |
| `battle/normal_battle/` | WFIGHTMN.PRO | the battle's sub-overlay for a normal battle: the battle menu and the turn's events |
| `battle/report/` | STFGTREP.PRO | the report after a battle: the partners that went up a level |
| `cardgame/card_battle/` | CARDGAME.PRO | the card battle |
| `cardgame/deck_editor/` | STCRDDEK.PRO | the deck editor |
| `cardgame/card_packs/` | STCRDSHP.PRO | opening card packs |
| `cardgame/album/` | STCRDABM.PRO | the card album |
| `menus/title/` | STDWTITL.PRO | the title screen, the opening movies and a notice screen |
| `menus/field_menu/` | STSTATUS.PRO | the field menu's screens |
| `menus/item_shop/` | STITSHOP.PRO | the item shop |
| `menus/memory_card/` | STGMCARD.PRO | the memory card screen |
| `menus/digimon_lab/` | STGDGLAB.PRO | the Digimon Lab, where the partners' digivolutions are set |
| `menus/gym/` | STGTRAIN.PRO | the gyms, where the partners train their stats |
| `menus/player_name/` | STPLNMET.PRO | the player's name entry |
| `menus/partner_name/` | STDGNAME.PRO | the screen where the partners are renamed |
| `menus/country_select/` | CNTY_SEL.PRO | the country select screen |
| `debug/stage_select/` | STAGSLCT.PRO | the debug stage select |
| `debug/battle_test/` | WFIGHTTS.PRO | the battle test, which FIGHTSTG loads in place of WFIGHTMN |
| `debug/vibration_test/` | SHOCKTST.PRO | the vibration test |
| `debug/sound_test/` | SOUNDTST.PRO | the sound test |
| `shared/` | | the code several overlays have, each its own copy, which their files include as `"shared/<file>.inc.c"` |

An overlay's `scene.c` is its root task (and the screen fade); `data/` holds
data that is the original's own object (`data/wfightmn_2.c`, the second
object's), named after it.
