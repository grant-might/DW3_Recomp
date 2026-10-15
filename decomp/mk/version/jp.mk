# Digimon World 3: Aratanaru Bouken no Tobira, Japan (SLPS-03446)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the release, as the configs' headers name it
VERSION_NAME := Digimon World 3: Aratanaru Bouken no Tobira, Japan (SLPS-03446)

# the executable, as it is on the disc
EXE_NAME := SLPS_034.46

# where the extracted disc is (tools/extract_disc.py): the executable and
# AAA/, whose PRO/ directory holds the overlays
DISK_DIR := disks/jp

# The overlays the build links and checks, AAA/PRO/<FILE>.PRO: each one has
# its splat config, config/<version>/<name>.yaml. The stage overlays are
# added from config/<version>/stages.txt: the USA version's and the 55 more
# the European version has (WSTAG920-974).
OVERLAYS := cardgame cnty_sel fieldstg fightstg shocktst soundtst stagslct \
	stcrdabm stcrddek stcrdshp stdgname stdwtitl stfgtrep stgdglab \
	stgmcard stgtrain stitshop stplnmet ststatus wfightmn wfightts

# where the executable loads (main.yaml)
EXE_VRAM := 0x80010000

# where the overlays load, after the executable's .bss, and where the stage
# overlays and WFIGHTMN/WFIGHTTS load, after CARDGAME, the largest
OVERLAY_VRAM := 0x80082A60
STAGE_VRAM := 0x800A5770

# $gp, as crt0 sets it: main.yaml's gp_value, which tools/match_versions.py
# reads too (the overlays don't use $gp)
GP_VALUE := 0x8005C90C

# The C files this version builds, from the same C as the other versions at
# this version's addresses (tools/version_symbols.py names what they use).
# The executable and the overlays are split into the USA version's modules
# (tools/split_version.py), so that their asm lands at the same paths under
# asm/jp/ as under asm/us/; the rest is still splat's assembly.

# game: every module of the executable, and its data
C_SRC := $(shell find src/engine -name '*.c')

# the overlays whose directory holds all their modules and data, one
# directory each
OVERLAY_DIRS := src/debug/sound_test src/debug/vibration_test \
	src/menus/country_select src/menus/item_shop src/menus/digimon_lab \
	src/menus/field_menu src/menus/memory_card src/menus/player_name \
	src/menus/partner_name src/cardgame/album src/cardgame/card_packs \
	src/cardgame/deck_editor src/battle/report src/battle/normal_battle \
	src/debug/battle_test src/battle/battle_mode src/field/field_mode \
	src/menus/gym src/cardgame/card_battle src/menus/title \
	src/debug/stage_select
C_SRC += $(shell find $(OVERLAY_DIRS) -name '*.c')

# The stages: the USA version's and the European version's 55 more
C_SRC += $(sort $(shell find src/field/stages -name '*.c' ! -name '*.inc.c'))
