/* FIGHTSTG's fighter data, loaded into FIGHTSTG_fighterCache. */

#include "battle/battle_mode.h"

/* The part of the fighters' file at an offset from its start */
static inline void *getFightersPart(FightersFile *file, s32 offset) {
    return (u8 *)file + offset;
}

/* the match depends on reaching the cache through the symbol until a fighter
   is found, and on each branch storing and returning its own info */
FighterInfo *FIGHTSTG_getFighterInfo(s32 id) {
    FightersFile *file;
    FighterEntry *entry;
    FighterInfo *partners;
    FighterInfoEnemy *enemies;
    FighterCache *cache;
    FighterInfo *info;
    s32 i;

    if (id == FIGHTSTG_fighterCache.id) {
        if (FIGHTSTG_fighterCache.isEnemy) {
            return FIGHTSTG_fighterCache.enemyInfo;
        }
        return FIGHTSTG_fighterCache.partnerInfo;
    }
    file = FILE_CACHE.load(FILE_FIGHTERS);
    entry = getFightersPart(file, file->entries);
    partners = getFightersPart(file, file->partners);
    enemies = getFightersPart(file, file->enemies);
    while (entry->id != 0) {
        if (entry->id == id) {
            FIGHTSTG_fighterCache.id = id;
            cache = &FIGHTSTG_fighterCache;
            cache->index = i = entry->index;
            cache->kind = entry->kind;
            cache->isEnemy = entry->kind >= 0x3A;
            if (cache->isEnemy) {
                info = (FighterInfo *)&enemies[i];
                cache->partnerInfo = info;
                cache->enemyInfo = info;
                return info;
            } else {
                info = &partners[i];
                cache->partnerInfo = info;
                cache->enemyInfo = info;
                return info;
            }
        }
        entry++;
    }
    return NULL;
}

/* Loads entry index of the fighters file into FIGHTSTG_fighterCache: its id
   and kind, and its info, an enemy's from kind 0x3A on or else a partner's */
void FIGHTSTG_cacheFighter(s32 index) {
    FightersFile *file = FILE_CACHE.load(FILE_FIGHTERS);
    FighterEntry *entries = getFightersPart(file, file->entries);
    FighterInfo *partners = getFightersPart(file, file->partners);
    FighterInfoEnemy *enemies = getFightersPart(file, file->enemies);
    FighterEntry *entry = &entries[index];
    FighterInfo *info;
    s32 i;
    FighterCache *cache = &FIGHTSTG_fighterCache;

    cache->id = entry->id;
    cache->index = i = entry->index;
    cache->kind = entry->kind;
    cache->isEnemy = entry->kind >= 0x3A;
    if (cache->isEnemy == 0) {
        info = &partners[i];
    } else {
        info = (FighterInfo *)&enemies[i];
    }
    cache->partnerInfo = info;
    cache->enemyInfo = info;
}

/* Fighter id's face parts (FaceRect), in the fighters file */
FaceRect *FIGHTSTG_getFighterFace(s32 id) {
    FightersFile *file = FILE_CACHE.load(FILE_FIGHTERS);

    /* face assumes the file is at base: move it to where the file is */
    return (FaceRect *)(FIGHTSTG_getFighterInfo(id)->face - file->base + (s32)file);
}

/* The first and last entries of the fighters file that are enemies (enemy
   set) or partners */
void FIGHTSTG_getFighterRange(u32 enemy, s32 *min, s32 *max) {
    FightersFile *file = FILE_CACHE.load(FILE_FIGHTERS);
    FighterEntry *entry = getFightersPart(file, file->entries);
    s32 lo = 0xFF;
    s32 hi = 0;
    s32 i = 0;

    while (entry->id != 0) {
        if ((entry->kind >= 0x3A) == enemy) {
            if (i < lo) {
                lo = i;
            }
            if (hi < i) {
                hi = i;
            }
        }
        entry++;
        i++;
    }
    *min = lo;
    *max = hi;
}
