/* FIGHTSTG's random rolls: hits, criticals, statuses and the other effects,
   running away and the statuses' own tests. */

#include "battle/battle_mode.h"

/* Whether a physical hit is critical: a chance of 4 in 128 with the
   technique's critical boost or the accessory's, more against the target's
   family and while the target is paralyzed, asleep or confused */
s32 FIGHTSTG_rollCritical(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    BattleFighter *fighter;
    s32 chance;
    s32 i;
    s32 j;
    s32 k;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    chance = 4;
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    if (entry->effect >= TECH_EFFECT_FIRST) {
        if (entry->effect == TECH_EFFECT_CRITICAL) {
            chance += entry->effectPower;
        }
    } else if (atk->criticalBonus != 0) {
        k = side != 0;
        fighter = &FIGHTSTG_battle.state.fighters[k][FIGHTSTG_battle.state.active[k]];
        if (fighter->special) {
            chance = atk->criticalBonus * 2 + 4;
        } else {
            chance = atk->criticalBonus + 4;
        }
    }
    if (entry->family >= FAMILY_FIRST) {
        if (entry->family == def->family) {
            chance += 0x3C;
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (atk->weaponFamilies[i] >= FAMILY_FIRST && atk->weaponFamilies[i] == def->family) {
                chance += 0x10;
                break;
            }
        }
    }
    j = side == 0;
    fighter = &FIGHTSTG_battle.state.fighters[j][FIGHTSTG_battle.state.active[j]];
    if (fighter->flags & FIGHTER_PARALYZED) {
        chance += fighter->paralysis >> 3;
    }
    if (fighter->flags & FIGHTER_ASLEEP) {
        chance += fighter->sleep >> 3;
    }
    if (fighter->flags & FIGHTER_CONFUSED) {
        chance += fighter->confusion >> 1;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a magic hit is critical: a chance from the technique's element
   power against the target's resistance, more against the target's family
   and while the target is paralyzed, asleep or confused */
s32 FIGHTSTG_rollMagicCritical(u8 side, s32 id) {
    BattleStats *def;
    TechData *entry;
    BattleFighter *fighter;
    s32 value;
    s32 chance;
    s32 j;

#if VERSION_US
    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
#elif VERSION_EU || VERSION_JP
    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
    }
#endif
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    value = entry->elementPower * 100 / def->resist[entry->element - ELEMENT_FIRST];
    if (value > 0x40) {
        value = 0x40;
    }
    chance = value + 4;
    if (entry->family >= FAMILY_FIRST && entry->family == def->family) {
        chance = value + 0x40;
    }
    j = side == 0;
    fighter = &FIGHTSTG_battle.state.fighters[j][FIGHTSTG_battle.state.active[j]];
    if (fighter->flags & FIGHTER_PARALYZED) {
        chance += fighter->paralysis >> 3;
    }
    if (fighter->flags & FIGHTER_ASLEEP) {
        chance += fighter->sleep >> 3;
    }
    if (fighter->flags & FIGHTER_CONFUSED) {
        chance += fighter->confusion >> 1;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a physical hit lands: the technique's accuracy with the speed and
   level differences and the accuracy and evasion accessories (the critical
   accessory costs the player some); the enemy hits at least 1 in 4, the
   player in BATTLE_KIND_FINAL_LAST 2 in 3 */
s32 FIGHTSTG_rollHit(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 chance;
    s32 level;
    s32 diff;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    if (side == 0) {
        if (FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
            return (RANDOM.next() & 0x7F) >= 0x2B;
        }
        diff = atk->stats[BATTLE_STAT_SPEED] + atk->accuracy - def->stats[BATTLE_STAT_SPEED];
        level = atk->level - def->level;
        if (entry->effect < TECH_EFFECT_FIRST) {
            chance = entry->accuracy + entry->accuracy * (diff / 8 + (level - atk->criticalBonus)) / 128;
        } else {
            chance = entry->accuracy + entry->accuracy * (diff / 8 + level) / 128;
        }
    } else {
        level = atk->level - def->level;
        diff = atk->stats[BATTLE_STAT_SPEED] - def->stats[BATTLE_STAT_SPEED] - def->evasion;
        chance = entry->accuracy + entry->accuracy * (diff / 8 + level) / 128;
        if (chance < 0x20) {
            chance = 0x20;
        }
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a magic hit lands: the technique's accuracy with the wisdom and
   level differences; the player in BATTLE_KIND_FINAL_LAST hits 2 in 3 */
s32 FIGHTSTG_rollMagicHit(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 chance;
    s32 level;
    s32 diff;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    if (side == 0 && FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
        return (RANDOM.next() & 0x7F) >= 0x2B;
    }
    diff = atk->stats[BATTLE_STAT_WISDOM] - def->stats[BATTLE_STAT_WISDOM];
    level = atk->level - def->level;
    chance = entry->accuracy + entry->accuracy * (diff / 8 + level) / 128;
    return (RANDOM.next() & 0x7F) < chance;
}

/* The poison a hit inflicts, or 0: the technique's or the accessory's chance
   and an eighth of the user's wisdom, less the target's resistances and
   wisdom (BATTLE_BLOCK_POISON keeps the player from it) */
s32 FIGHTSTG_rollPoison(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 chance;
    s32 result;
    s32 base;

    if (side == 0) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_POISON]) {
            return 0;
        }
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    if (entry->effect < TECH_EFFECT_FIRST) {
        result = atk->statuses[HIT_POISON].power;
        base = atk->statuses[HIT_POISON].chance + atk->stats[BATTLE_STAT_WISDOM] / 8;
    } else {
        result = entry->effectPower;
        base = entry->effectChance + atk->stats[BATTLE_STAT_WISDOM] / 8;
    }
    chance = base - (def->resist[RESIST_POISON] + def->resist[1] + def->stats[BATTLE_STAT_WISDOM] / 2) / 8;
    if (chance <= 0) {
        chance = 1;
    }
    if (chance >= 0x80) {
        chance = 0x7F;
    }
    if ((RANDOM.next() & 0x7F) < chance) {
        return result;
    }
    return 0;
}

/* Whether a hit inflicts status, like rollPoison without its power: BLOCK
   keeps the player from it, and the target resists it with RESIST and the
   resistance to ELEMENT */
static inline s32 rollStatus(u8 side, s32 id, s32 block, s32 status, s32 resist, s32 element) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 chance;
    s32 base;

    if (side == 0) {
        if (BATTLE_SETUP.blocks[block]) {
            return 0;
        }
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    if (entry->effect < TECH_EFFECT_FIRST) {
        base = atk->statuses[status].chance + atk->stats[BATTLE_STAT_WISDOM] / 8;
    } else {
        base = entry->effectChance + atk->stats[BATTLE_STAT_WISDOM] / 8;
    }
    chance = base - (def->resist[resist] + def->resist[element] + def->stats[BATTLE_STAT_WISDOM] / 2) / 8;
    if (chance <= 0) {
        chance = 1;
    }
    if (chance >= 0x80) {
        chance = 0x7F;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a hit paralyzes, like rollPoison (BATTLE_BLOCK_PARALYSIS keeps the
   player from it) */
s32 FIGHTSTG_rollParalysis(u8 side, s32 id) {
    return rollStatus(side, id, BATTLE_BLOCK_PARALYSIS, HIT_PARALYSIS, RESIST_PARALYSIS, 4);
}

/* Whether a hit confuses, like rollPoison (BATTLE_BLOCK_CONFUSION keeps the
   player from it) */
s32 FIGHTSTG_rollConfusion(u8 side, s32 id) {
    return rollStatus(side, id, BATTLE_BLOCK_CONFUSION, HIT_CONFUSION, RESIST_CONFUSION, 3);
}

/* Whether a hit puts the target to sleep, like rollPoison with the
   technique's chance (BATTLE_BLOCK_SLEEP keeps the player from it) */
s32 FIGHTSTG_rollSleep(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 chance;

    if (side == 0) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_SLEEP]) {
            return 0;
        }
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    entry = &TECHS[id - 1];
    chance = entry->effectChance + atk->stats[BATTLE_STAT_WISDOM] / 8 - (def->resist[RESIST_SLEEP] + def->resist[2] + def->stats[BATTLE_STAT_WISDOM] / 2) / 8;
    if (chance <= 0) {
        chance = 1;
    }
    if (chance >= 0x80) {
        chance = 0x7F;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a hit knocks the target out, like rollPoison
   (BATTLE_BLOCK_KNOCK_OUT keeps the player from it) */
s32 FIGHTSTG_rollKnockOut(u8 side, s32 id) {
    return rollStatus(side, id, BATTLE_BLOCK_KNOCK_OUT, HIT_KNOCK_OUT, RESIST_KNOCK_OUT, 6);
}

/* Whether the player steals the enemy's item: its itemChance in 1024 by the
   speed ratio (at most twice) and the technique's and the accessory's chance */
s32 FIGHTSTG_rollSteal(u8 side, s32 id) {
    BattleStats *atk;
    BattleStats *def;
    TechData *entry;
    s32 ratio;
    s32 chance;
    s32 power;
    s32 roll;

    if (side == 0) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_STEAL]) {
            return 0;
        }
        if (FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]].item <= 0) {
            return 0;
        }
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    if (side == 0) {
        atk = &FIGHTSTG_battleFuncs.stats[0];
        def = &FIGHTSTG_battleFuncs.stats[1];
        ratio = atk->stats[BATTLE_STAT_SPEED] * 100 / def->stats[BATTLE_STAT_SPEED];
        entry = &TECHS[id - 1];
        if (ratio > 200) {
            ratio = 200;
        }
        power = (entry->effectChance + atk->stealBonus) * 100 / 64;
        chance = FIGHTSTG_battleTableFunc(FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]].id)->itemChance * ratio * power / 10000;
        roll = RANDOM.next() % 1024;
        if (roll < chance) {
            return 1;
        }
    }
    return 0;
}

/* Whether a hit drains HP: the technique's chance, or the accessory's for a
   technique without an effect */
s32 FIGHTSTG_rollDrain(u8 side, s32 id) {
    BattleStats *stats;
    TechData *entry;
    s32 chance;

    if (side == 0 && BATTLE_SETUP.blocks[BATTLE_BLOCK_DRAIN] != 0) {
        return 0;
    }
#if VERSION_US
    stats = &FIGHTSTG_battleFuncs.stats[0];
#elif VERSION_EU || VERSION_JP
    stats = FIGHTSTG_computeStats(side, 1, FIGHTSTG_battle.state.active[side >> 4]);
#endif
    entry = &TECHS[id - 1];
    if (entry->effect < TECH_EFFECT_FIRST) {
        chance = stats->statuses[HIT_DRAIN].chance;
    } else {
        chance = entry->effectChance;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* Rolls a technique's effect chance (TechData.effectChance, in 128ths) */
static inline s32 rollTechChance(s32 id) {
    TechData *entry = &TECHS[id - 1];
    s32 chance = entry->effectChance;

    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether a technique turns the player's fighter back: never when it is one
   of the first 8 Digimon, otherwise the technique's chance */
s32 FIGHTSTG_rollDedigivolve(s32 actor, s32 id) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (DIGIMON_DATA[i].id == FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]].id) {
            return 0;
        }
    }
    return rollTechChance(id);
}

/* Whether a status raise works: the technique's chance and an eighth of
   what the enemy's wisdom has over the partner's */
s32 FIGHTSTG_rollStatusRaise(s32 actor, s32 id) {
    TechData *entry;
    s32 chance;

    FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
    FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    entry = &TECHS[id - 1];
    chance = entry->effectChance + (FIGHTSTG_battleFuncs.stats[0].stats[BATTLE_STAT_WISDOM] - FIGHTSTG_battleFuncs.stats[1].stats[BATTLE_STAT_WISDOM]) / 8;
    return (RANDOM.next() & 0x7F) < chance;
}

/* Whether TECH_EFFECT_NO_SWITCH works: the technique's chance */
s32 FIGHTSTG_rollNoSwitch(s32 actor, s32 id) {
    return rollTechChance(id);
}

/* Whether TECH_EFFECT_NO_DIGIVOLVE works: the technique's chance */
s32 FIGHTSTG_rollNoDigivolve(s32 actor, s32 id) {
    return rollTechChance(id);
}

#if VERSION_EU || VERSION_JP
/* FIGHTSTG_battleFuncs.rollCounter: a random test, likelier the bigger damage is next
   to the player's fighter's max HP; side is not used */
s32 FIGHTSTG_rollCounter(s32 side, s32 damage) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    s32 chance = (damage << 6) / fighter->maxHp + 32;

    return (RANDOM.next() & 0x7F) < chance;
}
#endif

/* FIGHTSTG_battleFuncs.testRunAway: a random test for side running away, from
   both sides' stats; the player can't while BATTLE_BLOCK_RUN_AWAY is set, and
   in the European version neither can the enemy while its fighter is asleep.
   The match depends on the battle's runAttempts being read through a pointer
   to its state, taken before side is tested again, and on the flags being
   tested with family as a halfword. */
s32 FIGHTSTG_testRunAway(u8 side) {
    BattleStats *atk;
    BattleStats *def;
    s32 chance;
    BattleState *state;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    state = &FIGHTSTG_battle.state;
    atk = &FIGHTSTG_battleFuncs.stats[0];
    def = &FIGHTSTG_battleFuncs.stats[1];
    if (side == 0) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_RUN_AWAY]) {
            return 0;
        }
        if (*(u16 *)&atk->flags & (FIGHTER_ASLEEP | FIGHTER_NO_SWITCH)) {
            return 0;
        }
        chance = (state->runAttempts + 1) * 8;
        if (atk->flags & FIGHTER_PARALYZED) {
            chance >>= 1;
        }
        if (atk->level > def->level) {
            chance += atk->level - def->level;
        }
        if (atk->stats[BATTLE_STAT_SPEED] > def->stats[BATTLE_STAT_SPEED]) {
            chance += (atk->stats[BATTLE_STAT_SPEED] - def->stats[BATTLE_STAT_SPEED]) / 10;
        }
        chance += atk->runAwayBonus;
    } else {
#if VERSION_EU || VERSION_JP
        if (atk->flags & FIGHTER_ASLEEP) {
            return 0;
        }
#endif
        if (def->runAwayGuard) {
            chance = 0x20;
        } else {
            chance = 0x40;
        }
        if (atk->flags & FIGHTER_PARALYZED) {
            chance >>= 1;
        }
        chance += (atk->stats[BATTLE_STAT_SPEED] - def->stats[BATTLE_STAT_SPEED]) / 10;
    }
    return (RANDOM.next() & 0x7F) < chance;
}

/* FIGHTSTG_battleFuncs.testWakeUp: whether a hit of value wakes side's
   fighter up, likelier the bigger value is next to the fighter's defense,
   less likely the stronger its sleep and the later its end (EVENT_STATUS_END
   + 2) is due. The match depends on the 64 being set apart from sleep's half and on the sum being
   written in one statement. */
s32 FIGHTSTG_testWakeUp(u8 side, s32 value) {
    s32 team;
    s32 index;
    s32 chance;
    s16 delay;
    s32 luck;
    BattleStats *def;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
    } else {
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    }
    team = side != 0;
    def = &FIGHTSTG_battleFuncs.stats[1];
    index = FIGHTSTG_events.funcs.find(EVENT_STATUS_END + 2, side, FIGHTSTG_battle.state.active[team]);
    chance = (value << 7) / def->stats[BATTLE_STAT_DEFENSE];
    delay = FIGHTSTG_events.events[index].time / 100;
    luck = 64;
    luck -= FIGHTSTG_battle.state.fighters[team][FIGHTSTG_battle.state.active[team]].sleep >> 1;
    chance = chance + luck - delay;
    return (RANDOM.next() & 0x7F) < chance;
}

/* FIGHTSTG_battleFuncs.testConfusion: a random test of side's confusion
   against its resists 9 and 3 */
s32 FIGHTSTG_testConfusion(u8 side) {
    BattleStats *stats;
    s32 other;
    s32 flag;
    s32 fighter;
    s32 chance;

    if (side == 0) {
        fighter = FIGHTSTG_battle.state.active[0];
        flag = SIDE_PLAYER;
    } else {
        flag = SIDE_ENEMY;
        fighter = FIGHTSTG_battle.state.active[1];
    }
    FIGHTSTG_computeStats(flag, 0, fighter);
    stats = &FIGHTSTG_battleFuncs.stats[1];
    other = side != 0;
    chance = FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]].confusion - (stats->resist[RESIST_CONFUSION] + stats->resist[3]) / 8;
    return (RANDOM.next() & 0x7F) < chance;
}

/* FIGHTSTG_battleFuncs.testParalysis: whether side's paralysis costs it the
   turn, a random test of its strength against resists 8 and 4 (at least a
   chance of 32 in 128) */
s32 FIGHTSTG_testParalysis(u8 side) {
    BattleStats *stats;
    s32 other;
    s32 flag;
    s32 fighter;
    s32 chance;

    if (side == 0) {
        fighter = FIGHTSTG_battle.state.active[0];
        flag = SIDE_PLAYER;
    } else {
        flag = SIDE_ENEMY;
        fighter = FIGHTSTG_battle.state.active[1];
    }
    FIGHTSTG_computeStats(flag, 0, fighter);
    stats = &FIGHTSTG_battleFuncs.stats[1];
    other = side != 0;
    chance = FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]].paralysis - (stats->resist[RESIST_PARALYSIS] + stats->resist[4]) / 8;
    if (chance < 0x20) {
        chance = 0x20;
    }
    return (RANDOM.next() & 0x7F) < chance;
}
