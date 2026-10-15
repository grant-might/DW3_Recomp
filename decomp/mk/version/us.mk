# Digimon World 3, USA (SLUS-01436)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the release, as the configs' headers name it
VERSION_NAME := Digimon World 3, USA (SLUS-01436)

# the executable, as it is on the disc
EXE_NAME := SLUS_014.36

# where the extracted disc is (tools/extract_disc.py): the executable and
# AAA/, whose PRO/ directory holds the overlays
DISK_DIR := disks/us

# The overlays the build links and checks, AAA/PRO/<FILE>.PRO: each one has
# its splat config, config/<version>/<name>.yaml. The stage overlays are
# added from config/<version>/stages.txt.
OVERLAYS := cardgame cnty_sel fieldstg fightstg shocktst soundtst stagslct \
	stcrdabm stcrddek stcrdshp stdgname stdwtitl stfgtrep stgdglab \
	stgmcard stgtrain stitshop stplnmet ststatus wfightmn wfightts

# where the executable loads (main.yaml)
EXE_VRAM := 0x80010000

# where the overlays load, after the executable's .bss, and where the stage
# overlays and WFIGHTMN/WFIGHTTS load, after CARDGAME, the largest
OVERLAY_VRAM := 0x80082448
STAGE_VRAM := 0x800A4CA4

# $gp, as crt0 sets it: main.yaml's gp_value, which tools/match_versions.py
# reads too (the overlays don't use $gp)
GP_VALUE := 0x8005C2F8

# The C files this version builds: the Makefile compiles these and nothing
# else under src/, and gives each binary the ones in its folder, its config's
# src_path (src/engine/, src/menus/item_shop/ for STITSHOP...; the stages
# theirs under src/field/stages/<area>/, tools/stage_areas.py). Every C file
# is the USA version's. A .inc.c file isn't built on its own: it is
# code that several binaries share, which their C files include
# (src/field/stages/common/, src/shared/).
C_SRC := $(shell find src -name '*.c' ! -name '*.inc.c' 2> /dev/null)
