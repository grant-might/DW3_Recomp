# Digimon World 2003, Europe (SLES-03936)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the release, as the configs' headers name it
VERSION_NAME := Digimon World 2003, Europe (SLES-03936)

# the executable, as it is on the disc
EXE_NAME := SLES_039.36

# where the extracted disc is (tools/extract_disc.py): the executable and
# AAA/, whose PRO/ directory holds the overlays
DISK_DIR := disks/eu

# The overlays the build links and checks, AAA/PRO/<FILE>.PRO: each one has
# its splat config, config/<version>/<name>.yaml. The stage overlays are
# added from config/<version>/stages.txt: the USA version's and 55 more.
OVERLAYS := cardgame cnty_sel fieldstg fightstg shocktst soundtst stagslct \
	stcrdabm stcrddek stcrdshp stdgname stdwtitl stfgtrep stgdglab \
	stgmcard stgtrain stitshop stplnmet ststatus wfightmn wfightts

# where the executable loads (main.yaml)
EXE_VRAM := 0x80010000

# where the overlays load, after the executable's .bss, and where the stage
# overlays and WFIGHTMN/WFIGHTTS load, after CARDGAME, the largest
OVERLAY_VRAM := 0x80082CB0
STAGE_VRAM := 0x800A5DE0

# $gp, as crt0 sets it: main.yaml's gp_value, which tools/match_versions.py
# reads too (the overlays don't use $gp)
GP_VALUE := 0x8005CB50

# The C files this version builds: every module of the USA version, from the
# same C at this version's addresses (tools/version_symbols.py names what they
# use), and the stages only this version has. The executable and the overlays
# are split into the USA version's modules (tools/split_version.py), so that
# their asm lands at the same paths under asm/eu/ as under asm/us/; only the
# PsyQ libraries and the functions behind INCLUDE_ASM stay asm.
C_SRC := src/debug/sound_test/sound_test.c

# game: every module of the executable, and its data
C_SRC += $(shell find src/engine -name '*.c')

# menus
C_SRC += src/menus/item_shop/scene.c src/menus/digimon_lab/scene.c src/menus/field_menu/card_screen.c
# stitshop's modules, and its data
C_SRC += $(addprefix src/menus/item_shop/, trade.c item_list.c info.c shop.c equip.c data/stitshop.c)
# stgdglab's modules
C_SRC += $(addprefix src/menus/digimon_lab/, recipe_screen.c entry_panel.c slot_screen.c menu.c scroll_bar.c)
C_SRC += $(addprefix src/menus/digimon_lab/, party_screen.c skill_panel.c entry_list.c lab.c)
# ststatus's modules
C_SRC += $(addprefix src/menus/field_menu/, demo_screen.c equip_panel.c digivolve_panel.c status_screen.c item_screen.c item_list.c tech_screen.c sort_screen.c map_screen.c menu.c helpers.c)

# cardgame
C_SRC += src/cardgame/card_battle/effect_script.c
# cardgame's other objects and modules (effect_script.c is above), and the
# second and third objects' data
C_SRC += $(filter-out src/cardgame/card_battle/effect_script.c,$(wildcard src/cardgame/card_battle/*.c))
C_SRC += src/cardgame/card_battle/data/cardgame_2.c src/cardgame/card_battle/data/cardgame_3.c

# fightstg's other objects and modules (model_mesh.c is with the overlays)
C_SRC += $(filter-out src/battle/battle_mode/model_mesh.c,$(wildcard src/battle/battle_mode/*.c))

# small overlays
C_SRC += src/menus/memory_card/scene.c src/battle/report/scene.c src/battle/report/partner.c src/battle/report/report.c src/battle/report/fade.c src/battle/report/growth.c src/battle/report/data/stfgtrep.c src/battle/normal_battle/loader.c src/battle/normal_battle/setup.c src/battle/normal_battle/menu.c src/battle/normal_battle/techs.c src/battle/normal_battle/data/wfightmn_2.c src/cardgame/card_packs/scene.c src/menus/player_name/scene.c src/debug/battle_test/battle_test.c
# stcrdshp's modules
C_SRC += $(addprefix src/cardgame/card_packs/, pack_open.c fader.c card_grid.c buy.c shop.c)
# stgmcard's modules, and its data
C_SRC += $(addprefix src/menus/memory_card/, info.c panel.c menu.c saves.c screen.c data/stgmcard.c)
# stplnmet's modules, and its data
C_SRC += $(addprefix src/menus/player_name/, backdrop.c welcome.c name_entry.c confirm.c choice.c screen.c data/stplnmet.c)

# overlays
C_SRC += src/debug/vibration_test/vibration_test.c src/menus/country_select/country_select.c src/cardgame/album/album.c
C_SRC += src/debug/stage_select/stage_select.c src/menus/partner_name/scene.c
# stdgname's modules, and its data
C_SRC += $(addprefix src/menus/partner_name/, name_entry.c menu.c screen.c data/stdgname.c)
C_SRC += src/menus/title/scene.c
# stdwtitl's modules, and its data
C_SRC += $(addprefix src/menus/title/, logo.c movie.c glint.c splash.c title_loader.c slides.c menu.c)
C_SRC += $(addprefix src/menus/title/, edge_fade.c background.c title.c)
C_SRC += $(addprefix src/menus/title/data/, stdwtitl.c title.c movie.c)
C_SRC += src/cardgame/deck_editor/scene.c src/menus/gym/scene.c src/battle/battle_mode/model_mesh.c
# stcrddek's modules, and its data
C_SRC += $(addprefix src/cardgame/deck_editor/, deck_cards.c editor.c name_entry.c scroll_bar.c screen.c data/stcrddek.c)
# fieldstg's modules, and its data
C_SRC += $(wildcard src/field/field_mode/*.c) src/field/field_mode/data/fieldstg.c
# stgtrain's modules
C_SRC += $(addprefix src/menus/gym/, sprite.c screen.c result.c session.c actor.c menu.c files.c)

# The stages: the USA version's, built from its C, and those it doesn't have,
# each in the folder of its area (tools/stage_areas.py), with WSTAG924's
# color in a file of its own before the stage's jump tables (wstag924_head.c,
# tools/stage_yaml.py)
C_SRC += $(sort $(shell find src/field/stages -name '*.c' ! -name '*.inc.c'))
