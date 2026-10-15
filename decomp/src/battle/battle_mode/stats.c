/* FIGHTSTG's battle stats (FIGHTSTG_computeStats). */

#include "battle/battle_mode.h"

/* Works out an enemy's stats from its table entry, its strength and the
   fighter's boosts */
static inline void computeEnemyStats(BattleStats *stats, BattleFighter *fighter, s32 index) {
    BattleTableEntry *entry;
    s32 j;

    entry = FIGHTSTG_battleTableFunc(fighter->id);
    stats->level = BATTLE_SETUP.enemies[index].level;
    for (j = 0; j < 5; j++) {
        stats->stats[j] = entry->stats[j] * BATTLE_SETUP.enemies[index].strength / 16;
    }
    for (j = 0; j < 12; j++) {
        stats->resist[j] = entry->resist[j];
    }
    stats->flags = fighter->flags;
    if (fighter->boosts[0] != 0) {
        stats->stats[BATTLE_STAT_ATTACK] += fighter->boosts[0];
    }
    if (fighter->boosts[1] != 0) {
        stats->stats[BATTLE_STAT_DEFENSE] += fighter->boosts[1];
    }
    if (fighter->boosts[2] != 0) {
        stats->stats[BATTLE_STAT_SPEED] += fighter->boosts[2];
    }
    if (fighter->boosts[3] != 0) {
        stats->stats[BATTLE_STAT_SPIRIT] += fighter->boosts[3];
    }
    stats->family = entry->family;
    stats->damageBonus = fighter->charge;
}

/* Works out a side's stats into its BattleStats: the partner's totals with
   the fighter's boosts, its resistances and what its equipment and
   accessories add, or the enemy's (computeEnemyStats). The match depends on
   one s16 pointer walking the totals, the accessories and the equipment
   alike: its extra sets give it the references that put it before found in
   the global allocator. */
BattleStats *FIGHTSTG_computeStats(u8 side, s32 which, s32 index) {
    PartnerTotals totals;
    BattleStats *stats;
    BattleFighter *fighter;
    DigimonData *digimon;
    DigimonData *other;
    PartnerStats *partner;
    ItemInfo *info;
    ItemData *data;
    AccessoryData *acc;
    s32 member;
    s16 *values;
    s32 count;
    s32 found;
    s32 i;

    if (which) {
        stats = &FIGHTSTG_battleFuncs.stats[0];
    } else {
        stats = &FIGHTSTG_battleFuncs.stats[1];
    }
    HEAP.zero(stats, sizeof(BattleStats));
    if (side == 0) {
        fighter = &FIGHTSTG_battle.state.fighters[0][index];
        member = GAME.funcs.getPartyMember(index);
        digimon = &DIGIMON_DATA[member];
        GAME.funcs.computeStats(member, &totals);
        for (i = RESIST_POISON; i < RESIST_COUNT; i++) {
            stats->resist[i] = digimon->statusResists[i - RESIST_POISON];
        }
        if (digimon->id != fighter->id) {
            other = GET_DIGIMON(fighter->id);
            for (i = 0; i < 5; i++) {
                totals.fields.battle[i] += other->battleStats[i];
            }
            for (i = 0; i < 7; i++) {
                totals.fields.resist[i] += other->resistances[i];
            }
            for (i = RESIST_POISON; i < RESIST_COUNT; i++) {
                stats->resist[i] += other->statusResists[i - RESIST_POISON];
            }
        }
        if (fighter->boosts[0] != 0) {
            totals.fields.battle[0] += fighter->boosts[0];
        }
        if (fighter->boosts[1] != 0) {
            totals.fields.battle[1] += fighter->boosts[1];
        }
        if (fighter->boosts[2] != 0) {
            totals.fields.battle[4] += fighter->boosts[2];
        }
        stats->level = totals.fields.level;
        values = totals.fields.battle;
        for (i = 0; i < 5; i++) {
#if VERSION_EU || VERSION_JP
            if (values[i] > 0) {
                stats->stats[i] = values[i];
            } else {
                stats->stats[i] = 1;
            }
#else
            stats->stats[i] = values[i];
#endif
        }
        values = totals.fields.resist;
        for (i = 0; i < 7; i++) {
#if VERSION_EU || VERSION_JP
            if (values[i] > 0) {
                stats->resist[i] = values[i];
            } else {
                stats->resist[i] = 1;
            }
#else
            stats->resist[i] = values[i];
#endif
        }
        stats->flags = fighter->flags;
        partner = GAME.funcs.getPartnerStats(member);
        values = &partner->equip[4];
        for (i = 0; i < 2; i++) {
            if (values[i] != 0) {
                acc = ITEM_FUNCS.get(values[i])->data.acc;
                if (acc->stat == 17) {
                    stats->resist[RESIST_POISON] = acc->amount;
                } else if (acc->stat == 18) {
                    stats->resist[RESIST_PARALYSIS] = acc->amount;
                } else if (acc->stat == 19) {
                    stats->resist[RESIST_CONFUSION] = acc->amount;
                } else if (acc->stat == 20) {
                    stats->resist[RESIST_SLEEP] = acc->amount;
                } else if (acc->stat == 21) {
                    stats->resist[RESIST_KNOCK_OUT] = acc->amount;
                }
            }
        }
        values = partner->equip;
        stats->family = GET_DIGIMON(fighter->id)->family;
        stats->damageBonus = fighter->charge;
        count = 0;
        for (i = 0; i < 4; i++) {
            if (values[i] > 0) {
                info = ITEM_FUNCS.get(values[i]);
                if (info->type >= 2 && info->type <= 14) {
                    data = info->data.record;
                    stats->accuracy += data->weapon.accuracy;
                    if (data->weapon.family >= FAMILY_FIRST) {
                        stats->weaponFamilies[count++] = data->weapon.family;
                    }
                } else {
                    data = info->data.record;
                    if (data->armor.evasion != 0) {
                        stats->evasion += data->armor.evasion;
                    }
                }
            }
        }
        found = 0;
        for (i = 0; i < 4; i++) {
            if (i != 1 && values[i] > 0) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                if (values[i] == 0x97) {
                    stats->statuses[HIT_POISON].chance = data->weapon.statusChance;
                    stats->statuses[HIT_POISON].power = data->weapon.statusPower;
                    found = 1;
                } else if (values[i] == 0xD2) {
                    stats->statuses[HIT_PARALYSIS].chance = data->weapon.statusChance;
                    stats->statuses[HIT_PARALYSIS].power = data->weapon.statusPower;
                    found = 1;
                } else if (values[i] == 0xB4 || values[i] == 0xC2) {
                    stats->statuses[HIT_CONFUSION].chance = data->weapon.statusChance;
                    stats->statuses[HIT_CONFUSION].power = data->weapon.statusPower;
                    found = 1;
                } else if (values[i] == 0x6D || values[i] == 0xBA) {
                    stats->statuses[HIT_KNOCK_OUT].chance = data->weapon.statusChance;
                    stats->statuses[HIT_KNOCK_OUT].power = data->weapon.statusPower;
                    found = 1;
                } else if (values[i] == 0x5E || values[i] == 0x93 || values[i] == 0xAD) {
                    stats->statuses[HIT_DRAIN].chance = data->weapon.statusChance;
                    stats->statuses[HIT_DRAIN].power = data->weapon.statusPower;
                    found = 1;
                } else if (values[i] == 0x96 || values[i] == 0xBF) {
                    stats->criticalBonus = data->weapon.statusPower;
                    found = 1;
                }
            }
        }
        if (!found) {
            values = &partner->equip[4];
            for (i = 0; i < 2; i++) {
                if (values[i] == 0x13C) {
                    stats->tripleHit = 1;
                } else if (values[i] == 0x13D) {
                    data = ITEM_FUNCS.get(0x13D)->data.record;
                    stats->criticalBonus = data->acc.amount;
                } else if (values[i] == 0x13E) {
                    data = ITEM_FUNCS.get(0x13E)->data.record;
                    stats->counter = data->acc.amount;
                }
            }
        }
        values = &partner->equip[4];
        for (i = 0; i < 2; i++) {
            if (values[i] >= 0x153 && values[i] <= 0x167) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                if (values[i] < 0x156) {
                    stats->element = 2;
                } else if (values[i] < 0x159) {
                    stats->element = 3;
                } else if (values[i] < 0x15C) {
                    stats->element = 4;
                } else if (values[i] < 0x15F) {
                    stats->element = 5;
                } else if (values[i] < 0x162) {
                    stats->element = 6;
                } else if (values[i] < 0x165) {
                    stats->element = 7;
                } else if (values[i] < 0x168) {
                    stats->element = 8;
                }
                stats->elementPower = data->acc.amount;
            } else if (values[i] >= 0x145 && values[i] <= 0x146) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->damageCut = data->acc.amount;
            } else if (values[i] >= 0x14B && values[i] <= 0x14C) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->accuracy += data->acc.amount;
            } else if (values[i] >= 0x14D && values[i] <= 0x14E) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->evasion += data->acc.amount;
            } else if (values[i] >= 0x14F && values[i] <= 0x150) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->runAwayBonus = data->acc.amount;
            } else if (values[i] == 0x13F) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->runAwayGuard = 1;
            } else if (values[i] >= 0x147 && values[i] <= 0x148) {
                data = ITEM_FUNCS.get(values[i])->data.record;
                stats->stealBonus = data->acc.amount;
            }
        }
    } else {
        fighter = &FIGHTSTG_battle.state.fighters[1][index];
        computeEnemyStats(stats, fighter, index);
    }
    if (which) {
        return &FIGHTSTG_battleFuncs.stats[0];
    }
    return &FIGHTSTG_battleFuncs.stats[1];
}
