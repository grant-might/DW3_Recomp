/* FIGHTSTG's damage and healing. */

#include "battle/battle_mode.h"

/* the element whose boost (FIGHTSTG_getElementBoost) weakens each one */
s32 FIGHTSTG_opposedElements[] = {
    0, 0, 4, 2,
    5, 3, 7, 8,
    6,
};

/* What the battle's element boost adds to value for a technique of the
   given element: the boost's amount in 128ths of value for the boosted
   element, half as much taken off for the element it weakens, nothing
   otherwise */
s32 FIGHTSTG_getElementBoost(s32 value, s32 element) {
    ElementBoost *boost = &FIGHTSTG_battle.state.boost;

    if (boost->element < ELEMENT_FIRST) {
        return 0;
    }
    if (element == boost->element) {
        return value * boost->amount / 128;
    }
    if (FIGHTSTG_opposedElements[element] == boost->element) {
        return -(value * boost->amount / 256);
    }
    return 0;
}

/* A hit's damage from its base value: the element boost, half again against
   the target's family, the user's damage bonus, the element against the
   target's resistance, the triple hit's 0.4, a critical hit and the target's
   damage cut; at most 5 times value */
s32 FIGHTSTG_adjustDamage(u8 side, s32 id, s32 value) {
    TechData *tech = &TECHS[id - 1];
    BattleStats *user = &FIGHTSTG_battleFuncs.stats[0];
    BattleStats *target = &FIGHTSTG_battleFuncs.stats[1];
    s32 result = value;
    s32 i;

    result += FIGHTSTG_getElementBoost(result, tech->element);
    if (tech->family >= FAMILY_FIRST) {
        if (tech->family == target->family) {
            result += result / 2;
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (user->weaponFamilies[i] >= FAMILY_FIRST && user->weaponFamilies[i] == target->family) {
                result += result / 2;
                break;
            }
        }
    }
    if (user->damageBonus != 0) {
        result += result * user->damageBonus / 64;
    }
    if (tech->element >= ELEMENT_FIRST) {
        result += result * tech->elementPower * 2 / target->resist[tech->element - ELEMENT_FIRST];
    } else if ((tech->script < 11 || tech->script > 12) && user->element != 0) {
        result += result * user->elementPower * 2 / target->resist[user->element - ELEMENT_FIRST];
    }
    if (tech->effect < TECH_EFFECT_FIRST && user->tripleHit != 0 && (tech->script < 11 || tech->script > 12)) {
        result = result * 4 / 10;
    }
    if (FIGHTSTG_rollCritical(side, id) != 0) {
        result += result * ((RANDOM.next() & 0x3F) + 0x20) / 64;
    }
    if (target->damageCut != 0) {
        result -= target->damageCut;
        if (result <= 0) {
            result = 0;
        }
    }
    if (result > value * 5) {
        result = value * 5;
    }
#if VERSION_EU || VERSION_JP
    if (result >= 10000) {
        result = 9999;
    }
#endif
    return result;
}

/* A technique's power from the user's attack against the target's defense
 * (an enemy's scaled by its unkA / 16), passed on to FIGHTSTG_adjustDamage. The match
 * depends on the division in each branch. */
s32 FIGHTSTG_computeDamage(u8 side, s32 id) {
    TechData *tech;
    BattleStats *user;
    BattleStats *target;
    s32 value;
    s32 enemy;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    user = &FIGHTSTG_battleFuncs.stats[0];
    tech = &TECHS[id - 1];
    target = &FIGHTSTG_battleFuncs.stats[1];
    if (side == 0) {
        value = tech->power * user->stats[BATTLE_STAT_ATTACK] / target->stats[BATTLE_STAT_DEFENSE];
    } else {
        enemy = FIGHTSTG_battle.state.active[1]; /* the match depends on reading it first */
        value = tech->power * BATTLE_SETUP.enemies[enemy].strength / 16 * user->stats[BATTLE_STAT_ATTACK] /
                target->stats[BATTLE_STAT_DEFENSE];
    }
    return FIGHTSTG_adjustDamage(side, id, value);
}

/* A magic technique's damage: its power by the user's spirit against the
   target's (half to twice as much), the element boost and resistance, half
   again against the target's family, a critical hit and the target's damage
   cut; at most 5 times the power */
s32 FIGHTSTG_computeMagicDamage(u8 side, s32 id) {
    TechData *tech;
    BattleStats *user;
    BattleStats *target;
    s32 base;
    s32 enemy;
    s32 power;
    s32 result;
    s16 resist;

    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    user = &FIGHTSTG_battleFuncs.stats[0];
    tech = &TECHS[id - 1];
    target = &FIGHTSTG_battleFuncs.stats[1];
    if (side == 0) {
        base = tech->power;
    } else {
        enemy = FIGHTSTG_battle.state.active[1]; /* the match depends on reading it first */
        base = tech->power * BATTLE_SETUP.enemies[enemy].strength / 16;
    }
    power = base * (user->stats[BATTLE_STAT_SPIRIT] * 50 / target->stats[BATTLE_STAT_SPIRIT] + 50) / 100;
    if (power > base * 2) {
        power = base * 2;
    }
    if (power < base / 2) {
        power = base / 2;
    }
    result = power;
    result += FIGHTSTG_getElementBoost(result, tech->element);
    if (tech->element >= ELEMENT_FIRST) {
        resist = target->resist[tech->element - ELEMENT_FIRST];
        if (resist < 100) {
            result = result * (400 - resist * 3) / 100;
        } else if (resist >= 300) {
            result = result * (65 - resist / 20) / 100;
        } else {
            result = result * (125 - resist / 4) / 100;
        }
    }
    if (tech->family >= FAMILY_FIRST && tech->family == target->family) {
        result += result / 2;
    }
    if (FIGHTSTG_rollMagicCritical(side, id) != 0) {
        result += result * (RANDOM.next() % 65 + 0x20) / 64;
    }
    if (target->damageCut != 0) {
        result -= target->damageCut;
        if (result <= 0) {
            result = 0;
        }
    }
    if (result > power * 5) {
        result = power * 5;
    }
#if VERSION_EU || VERSION_JP
    if (result >= 10000) {
        result = 9999;
    }
#endif
    return result;
}

/* getDamage: the damage of an event (args: the side, the fighter and the
 * base damage) to the fighter if it is its side's active one, less a tenth of
 * its resist[1] and resist[RESIST_POISON], as a part of its max HP: at least 1, at most
 * half the max HP. The match depends on the fighter pointer. */
s32 FIGHTSTG_getDamage(s32 *args) {
    u8 side;
    u8 team; /* the match depends on a u8 */
    s32 index;
    s32 damage;
    BattleStats *stats;
    s16 maxHp;
    BattleFighter *fighter;
    s32 result;

    side = args[0];
    index = args[1];
    damage = args[2];
    team = side != 0;
    if (index != FIGHTSTG_battle.state.active[team]) {
        return 0;
    }
    FIGHTSTG_computeStats(side, 0, index);
    stats = &FIGHTSTG_battleFuncs.stats[1];
    fighter = &FIGHTSTG_battle.state.fighters[team][index];
    maxHp = fighter->maxHp;
    result = (damage / 2 - (stats->resist[RESIST_POISON] + stats->resist[1]) / 10) * maxHp / 100;
    if (result <= 0) {
        result = 1;
    }
    if (result > maxHp / 2) {
        result = maxHp / 2;
    }
    return result;
}

/* The damage of a counterattack on value: the partner's first skill deals
   it as one hit (the triple hit set aside), another technique its power in
   64ths of value (32nds when the fighter is special) */
s32 FIGHTSTG_computeCounterDamage(u8 side, s32 id, s32 value) {
    BattleFighter *fighter;
    s32 own;
    u8 saved;
    s32 result;
    TechData *entry;

    if (side == 0) {
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        own = id == GET_DIGIMON(fighter->id)->skills[0];
    } else {
        fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        FIGHTSTG_battleTableFunc(fighter->id);
        own = 0;
    }
    if (side == 0) {
        FIGHTSTG_computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    } else {
        FIGHTSTG_computeStats(SIDE_PLAYER, 0, FIGHTSTG_battle.state.active[0]);
        FIGHTSTG_computeStats(SIDE_ENEMY, 1, FIGHTSTG_battle.state.active[1]);
    }
    if (own == 1) {
        saved = FIGHTSTG_battleFuncs.stats[0].tripleHit;
        FIGHTSTG_battleFuncs.stats[0].tripleHit = 0;
        result = FIGHTSTG_adjustDamage(side, id, value);
        FIGHTSTG_battleFuncs.stats[0].tripleHit = saved;
        return result;
    }
    entry = &TECHS[id - 1];
    if (fighter->special) {
        result = value * entry->effectPower / 32;
    } else {
        result = value * entry->effectPower / 64;
    }
    return FIGHTSTG_adjustDamage(side, id, result);
}

/* The HP a healing technique restores: 64 times its power, and an eighth of
   it per point of the user's wisdom */
s32 FIGHTSTG_computeHeal(u8 side, s32 id) {
    BattleStats *stats;
    TechData *entry;
    u16 value;
    s32 flag;
    s32 fighter;
#if VERSION_EU || VERSION_JP
    s32 result;
#endif

    if (side == 0) {
        fighter = FIGHTSTG_battle.state.active[0];
        flag = SIDE_PLAYER;
    } else {
        flag = SIDE_ENEMY;
        fighter = FIGHTSTG_battle.state.active[1];
    }
    FIGHTSTG_computeStats(flag, 1, fighter);
    stats = &FIGHTSTG_battleFuncs.stats[0];
    entry = &TECHS[id - 1];
    value = entry->power;
#if VERSION_US
    return (value << 6) + stats->stats[BATTLE_STAT_WISDOM] * value / 8;
#elif VERSION_EU || VERSION_JP
    result = (value << 6) + stats->stats[BATTLE_STAT_WISDOM] * value / 8;
    if (result > 9999) {
        result = 9999;
    }
    return result;
#endif
}

/* The HP a fighter regains: 8 to 16 128ths of its max HP when big, 4 to 8
   otherwise */
s32 FIGHTSTG_getHeal(u8 side, s32 index, s32 big) {
    BattleFighter *fighter;
    s32 value;

    if (side == 0) {
        fighter = &FIGHTSTG_battle.state.fighters[0][index];
    } else {
        fighter = &FIGHTSTG_battle.state.fighters[1][index];
    }
    if (big) {
        value = fighter->maxHp * (RANDOM.next() % 9 + 8) / 128;
    } else {
        value = fighter->maxHp * (RANDOM.next() % 5 + 4) / 128;
    }
#if VERSION_EU || VERSION_JP
    if (value > 9999) {
        value = 9999;
    }
#endif
    return value;
}
