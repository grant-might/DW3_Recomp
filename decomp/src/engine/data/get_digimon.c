#include "common.h"
#include "engine/game_state.h"

/*
 * getDigimon for the overlays, in the small data (.sdata) between
 * game/items.c's and file/file_table.c's. game/digimon.c, getDigimon's own
 * module, is linked before game/items.c, so it cannot hold it; nothing
 * shows which of the modules linked between the two does.
 */
DigimonData *(*GET_DIGIMON)(s32 id) = getDigimon;
