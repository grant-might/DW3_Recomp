/* FIGHTSTG's battle table lookups. */

#include "battle/battle_mode.h"

/* Enemy Digimon id's index in the battle table (FILE_BATTLE_TABLE: each
   enemy's stats, techniques and actions), or -1 */
s32 FIGHTSTG_findBattleTableIndex(s32 id) {
    BattleTableEntry *table = FILE_CACHE.load(FILE_BATTLE_TABLE);
    s32 i;

    for (i = 0; table[i].id != 0; i++) {
        if (table[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Enemy Digimon id's entry in the battle table, or NULL */
BattleTableEntry *FIGHTSTG_getBattleTableEntry(s32 id) {
    BattleTableEntry *table = FILE_CACHE.load(FILE_BATTLE_TABLE);
    s32 i = FIGHTSTG_findBattleTableIndex(id);

    if (i >= 0) {
        return &table[i];
    }
    return NULL;
}
