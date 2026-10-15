/* FIGHTSTG's technique effects and the start of an action. */

#include "battle/battle_mode.h"

/* TECH_EFFECT_POISON: poisons the target when rollPoison lets it, twice as
   strongly when the acting fighter is special */
void FIGHTSTG_tryPoison(void) {
    s32 side = FIGHTSTG_action.side != 0;
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
    s32 value = FIGHTSTG_battleFuncs.rollPoison(FIGHTSTG_action.side, FIGHTSTG_action.tech);

    if (value != 0) {
        if (fighter->special) {
            value *= 2;
        }
        FIGHTSTG_action.effects[TECH_EFFECT_POISON] = value;
    }
}

/* TECH_EFFECT_PARALYSIS: paralyzes the target when rollParalysis lets it, with
   the technique's effectPower or the accessory's, twice as strongly when the acting
   fighter is special */
void FIGHTSTG_tryParalysis(void) {
    BattleAction *action = &FIGHTSTG_action;
    BattleFuncs *funcs = &FIGHTSTG_battleFuncs;
    s32 side = action->side != 0;
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
    TechData *entry;
    s32 value;

    if (funcs->rollParalysis(action->side, action->tech)) {
        entry = &TECHS[action->tech - 1];
        if (entry->effect < TECH_EFFECT_FIRST) {
            value = funcs->stats[0].statuses[HIT_PARALYSIS].power;
        } else {
            value = entry->effectPower;
        }
        if (fighter->special) {
            value *= 2;
        }
        FIGHTSTG_action.effects[TECH_EFFECT_PARALYSIS] = value;
    }
}

/* TECH_EFFECT_CONFUSION: confuses the target when rollConfusion lets it, with
   the technique's effectPower or the accessory's, twice as strongly when the acting
   fighter is special */
void FIGHTSTG_tryConfusion(void) {
    BattleAction *action = &FIGHTSTG_action;
    BattleFuncs *funcs = &FIGHTSTG_battleFuncs;
    s32 side = action->side != 0;
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
    TechData *entry;
    s32 value;

    if (funcs->rollConfusion(action->side, action->tech)) {
        entry = &TECHS[action->tech - 1];
        if (entry->effect < TECH_EFFECT_FIRST) {
            value = funcs->stats[0].statuses[HIT_CONFUSION].power;
        } else {
            value = entry->effectPower;
        }
        if (fighter->special) {
            value *= 2;
        }
        FIGHTSTG_action.effects[TECH_EFFECT_CONFUSION] = value;
    }
}

/* TECH_EFFECT_SLEEP: puts the target to sleep when rollSleep lets it, twice
   as strongly when the acting fighter is special */
void FIGHTSTG_trySleep(void) {
    s32 side = FIGHTSTG_action.side != 0;
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
    TechData *entry;
    s32 value;

    if (FIGHTSTG_battleFuncs.rollSleep(FIGHTSTG_action.side, FIGHTSTG_action.tech)) {
        entry = &TECHS[FIGHTSTG_action.tech - 1];
        value = entry->effectPower;
        if (fighter->special) {
            value *= 2;
        }
        FIGHTSTG_action.effects[entry->effect] = value;
    }
}

/* TECH_EFFECT_KNOCK_OUT: knocks the target out when rollKnockOut lets it */
void FIGHTSTG_tryKnockOut(void) {
    BattleAction *action = &FIGHTSTG_action;

    if (FIGHTSTG_battleFuncs.rollKnockOut(action->side, action->tech)) {
        action->effects[TECH_EFFECT_KNOCK_OUT] = 1;
    }
}

/* TECH_EFFECT_MULTI_HIT and the triple-hit accessory: rolls the other hits, 3
   or the technique's hitCount in all; in BATTLE_KIND_FINAL_LAST the player's
   missed first hit makes them all miss */
void FIGHTSTG_rollMultiHit(void) {
    s32 count = 3;
    BattleAction *action = &FIGHTSTG_action;
    Battle *battle = &FIGHTSTG_battle;
    TechData *entry = &TECHS[action->tech - 1];
    s32 i;
    s32 side;

    side = action->side;
    if (entry->effect >= TECH_EFFECT_FIRST) {
        count = entry->hitCount;
    }
#if VERSION_US
    action->hitsLanded = 0;
    action->hitCount = 0;
#elif VERSION_EU || VERSION_JP
    action->hitsLanded = action->hits[0];
    action->hitCount = 1;
#endif
    if (side == 0 && battle->state.kind == BATTLE_KIND_FINAL_LAST && action->hits[0] == 0) {
        action->hitCount = count;
    } else {
#if VERSION_US
        for (i = 0; i < count; i++) {
#elif VERSION_EU || VERSION_JP
        for (i = 1; i < count; i++) {
#endif
            if (FIGHTSTG_battleFuncs.rollHit(FIGHTSTG_action.side, FIGHTSTG_action.tech) != 0) {
                FIGHTSTG_action.hits[FIGHTSTG_action.hitCount++] = 1;
                FIGHTSTG_action.hitsLanded++;
            } else {
                FIGHTSTG_action.hits[FIGHTSTG_action.hitCount++] = 0;
            }
        }
    }
    FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT] = 1;
}

/* When rollDrain allows it, the action's drain becomes its damage times a 128th of
 * the technique's effectPower (effect 8) or the player's HIT_DRAIN power, doubled when the
 * acting fighter is special. The match depends on each branch doubling
 * and scaling its own value. */
void FIGHTSTG_tryDrain(void) {
    BattleAction *action = &FIGHTSTG_action;
    BattleFuncs *funcs = &FIGHTSTG_battleFuncs;
    s32 side = action->side != 0;
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
    TechData *entry;
    s32 value;

    if (funcs->rollDrain(action->side, action->tech)) {
        entry = &TECHS[action->tech - 1];
        if (entry->effect == TECH_EFFECT_DRAIN) {
            value = entry->effectPower;
            if (fighter->special) {
                value *= 2;
            }
            action->drain = action->damage * value / 128;
        } else {
            value = funcs->stats[0].statuses[HIT_DRAIN].power;
            if (fighter->special) {
                value *= 2;
            }
            action->drain = action->damage * value / 128;
        }
        FIGHTSTG_action.effects[TECH_EFFECT_DRAIN] = 1;
    }
}

/* Marks the action's technique's effect with the effect itself */
static inline void markTechEffect(void) {
    BattleAction *action = &FIGHTSTG_action;
    TechData *entry = &TECHS[action->tech - 1];

    action->effects[entry->effect] = entry->effect;
}

/* TECH_EFFECT_ENEMY_ONLY outside its own enemy's hands: marks the effect,
   which takes the place of the hit */
void FIGHTSTG_markEnemyOnly(void) {
    markTechEffect();
}

/* TECH_EFFECT_CRITICAL: marks the effect, which rollCritical has used */
void FIGHTSTG_markCritical(void) {
    markTechEffect();
}

/* TECH_EFFECT_STEAL: when the enemy's active fighter holds an
 * item and rollSteal lets the player take it, marks the effect and loads the item
 * names. The match depends on funcs being a const pointer: the front end puts
 * its value in the call, so only the %hi of its own initialization is left
 * for CSE to share with the call's address. */
void FIGHTSTG_trySteal(void) {
    BattleFuncs *const funcs = &FIGHTSTG_battleFuncs;
    BattleFighter *enemies = FIGHTSTG_battle.state.fighters[1];
    TechData *entry;

    if (enemies[FIGHTSTG_battle.state.active[1]].item > 0) {
        if (funcs->rollSteal(0, FIGHTSTG_action.tech) != 0) {
            entry = &TECHS[FIGHTSTG_action.tech - 1];
            FIGHTSTG_action.effects[entry->effect] = 1;
            FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
        }
    }
}

/* TECH_EFFECT_LOWER_ATTACK: lowers the attack of the other side's active
 * fighter by the technique's effectPower in 128ths and starts its event (not when
 * side 0 acts with BATTLE_BLOCK_LOWER_ATTACK set). The match depends on team
 * being a u8. */
void FIGHTSTG_lowerAttack(void) {
    u8 team = FIGHTSTG_action.side != 0;
    TechData *entry;

    if (team == 0 && BATTLE_SETUP.blocks[BATTLE_BLOCK_LOWER_ATTACK] != 0) {
        return;
    }
    entry = &TECHS[FIGHTSTG_action.tech - 1];
    FIGHTSTG_battleFuncs.changeBoost(SIDE_ENEMY - FIGHTSTG_action.side, FIGHTSTG_battle.state.active[1 - team], 0, -entry->effectPower);
    FIGHTSTG_queueBoostEnd(SIDE_ENEMY - FIGHTSTG_action.side, FIGHTSTG_battle.state.active[1 - team], 0, FIGHTSTG_action.tech);
    FIGHTSTG_action.effects[entry->effect] = entry->effectPower;
}

/* TECH_EFFECT_LOWER_DEFENSE: lowers the defense of the other side's active fighter
   by the technique's effectPower in 128ths and starts its event */
void FIGHTSTG_lowerDefense(void) {
    s32 other = 1 - (FIGHTSTG_action.side != 0);
    TechData *entry = &TECHS[FIGHTSTG_action.tech - 1];

    FIGHTSTG_battleFuncs.changeBoost(SIDE_ENEMY - FIGHTSTG_action.side, FIGHTSTG_battle.state.active[other], 1, -entry->effectPower);
    FIGHTSTG_queueBoostEnd(SIDE_ENEMY - FIGHTSTG_action.side, FIGHTSTG_battle.state.active[other], 1, FIGHTSTG_action.tech);
    FIGHTSTG_action.effects[entry->effect] = entry->effectPower;
}

/* TECH_EFFECT_DRAIN_MP: takes the technique's effectPower in 128ths of the player's
   fighter's max MP, at most what it has, as the action's drain */
void FIGHTSTG_drainMp(void) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    TechData *entry = &TECHS[FIGHTSTG_action.tech - 1];

    if (fighter->mp != 0) {
        FIGHTSTG_action.drain = fighter->maxMp * entry->effectPower / 128;
        if (fighter->mp < FIGHTSTG_action.drain) {
            FIGHTSTG_action.drain = fighter->mp;
        }
        FIGHTSTG_action.effects[entry->effect] = FIGHTSTG_action.drain;
    }
}

/* TECH_EFFECT_RAISE_ONE_STATUS: when rollStatusRaise lets it, raises one of
   the partner's status values at random by the technique's effectPower */
void FIGHTSTG_raiseOneStatus(void) {
    s32 i = RANDOM.next() % 2;
    TechData *entry;
    PartnerStats *stats;

    if (FIGHTSTG_battleFuncs.rollStatusRaise(FIGHTSTG_action.side, FIGHTSTG_action.tech)) {
        entry = &TECHS[FIGHTSTG_action.tech - 1];
        stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]));
        stats->status[i] += entry->effectPower;
        FIGHTSTG_action.effects[entry->effect] = 1 << i;
    }
}

/* TECH_EFFECT_RAISE_EACH_STATUS: raises each of the partner's status values
   that rollStatusRaise lets by the technique's effectPower */
void FIGHTSTG_raiseEachStatus(void) {
    TechData *entry = &TECHS[FIGHTSTG_action.tech - 1];
    PartnerStats *stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]));
    s32 i;

    for (i = 0; i < 3; i++) {
        if (FIGHTSTG_battleFuncs.rollStatusRaise(FIGHTSTG_action.side, FIGHTSTG_action.tech)) {
            stats->status[i] += entry->effectPower;
            FIGHTSTG_action.effects[entry->effect] |= 1 << i;
        }
    }
}

/* TECH_EFFECT_RAISE_ALL_STATUS: when rollStatusRaise lets it, raises all of
   the partner's status values by the technique's effectPower */
void FIGHTSTG_raiseAllStatus(void) {
    BattleAction *action = &FIGHTSTG_action;
    TechData *entry = &TECHS[action->tech - 1];
    PartnerStats *stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]));
    s32 i;

    if (FIGHTSTG_battleFuncs.rollStatusRaise(action->side, action->tech)) {
        for (i = 0; i < 3; i++) {
            stats->status[i] += entry->effectPower;
        }
        FIGHTSTG_action.effects[entry->effect] = 7;
    }
}

/* TECH_EFFECT_NO_SWITCH: marks the effect when rollNoSwitch lets it */
void FIGHTSTG_tryNoSwitch(void) {
    TechData *entry;

    if (FIGHTSTG_battleFuncs.rollNoSwitch(FIGHTSTG_action.side, FIGHTSTG_action.tech)) {
        entry = &TECHS[FIGHTSTG_action.tech - 1];
        FIGHTSTG_action.effects[entry->effect] = 1;
    }
}

/* Starts a TECH_EFFECT_DOUBLE_MAGIC technique: two magic hits, each with the
   damage of technique 0x1B9 or 0x1BA */
void FIGHTSTG_startDoubleMagic(void) {
#if VERSION_EU || VERSION_JP
    s32 files[2] = { 0x1B9, 0x1BA };
#endif
    s32 i;

    FIGHTSTG_action.hitsLanded = 0;
    FIGHTSTG_action.hitCount = 2;
    for (i = 0; i < 2; i++) {
        if (FIGHTSTG_battleFuncs.rollMagicHit(FIGHTSTG_action.side, FIGHTSTG_action.tech)) {
            FIGHTSTG_action.hits[i] = 1;
#if VERSION_EU || VERSION_JP
            FIGHTSTG_action.hitDamage[i] = FIGHTSTG_battleFuncs.computeMagicDamage(FIGHTSTG_action.side, files[i]);
#endif
            FIGHTSTG_action.hitsLanded++;
        }
    }
#if VERSION_US
    FIGHTSTG_action.hitDamage[0] = FIGHTSTG_battleFuncs.computeMagicDamage(FIGHTSTG_action.side, 0x1B9);
    FIGHTSTG_action.hitDamage[1] = FIGHTSTG_battleFuncs.computeMagicDamage(FIGHTSTG_action.side, 0x1BA);
#endif
    FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT] = 1;
}

/* Starts a TECH_EFFECT_END_BATTLE technique: in BATTLE_KIND_UNK2 its effect,
   elsewhere a plain physical hit */
void FIGHTSTG_startEndBattle(void) {
    TechData *entry;

    if (FIGHTSTG_battle.state.kind == BATTLE_KIND_UNK2) {
        entry = &TECHS[FIGHTSTG_action.tech - 1];
        FIGHTSTG_action.effects[entry->effect] = 1;
    } else {
        FIGHTSTG_action.hits[0] = FIGHTSTG_battleFuncs.rollHit(FIGHTSTG_action.side, FIGHTSTG_action.tech);
        FIGHTSTG_action.hitCount++;
        FIGHTSTG_action.damage = FIGHTSTG_battleFuncs.computeDamage(FIGHTSTG_action.side, FIGHTSTG_action.tech);
    }
}

/* Carries out the effect of a technique whose hit landed */
void FIGHTSTG_applyTechEffect(void) {
    TechData *entry = &TECHS[FIGHTSTG_action.tech - 1];

    switch (entry->effect) {
    case TECH_EFFECT_POISON:
        FIGHTSTG_tryPoison();
        break;
    case TECH_EFFECT_PARALYSIS:
        FIGHTSTG_tryParalysis();
        break;
    case TECH_EFFECT_CONFUSION:
        FIGHTSTG_tryConfusion();
        break;
    case TECH_EFFECT_SLEEP:
        FIGHTSTG_trySleep();
        break;
    case TECH_EFFECT_KNOCK_OUT:
        FIGHTSTG_tryKnockOut();
        break;
    case TECH_EFFECT_DRAIN:
        FIGHTSTG_tryDrain();
        break;
    case TECH_EFFECT_STEAL:
        FIGHTSTG_trySteal();
        break;
    case TECH_EFFECT_LOWER_ATTACK:
        FIGHTSTG_lowerAttack();
        break;
    case TECH_EFFECT_CRITICAL:
        FIGHTSTG_markCritical();
        break;
    case TECH_EFFECT_LOWER_DEFENSE:
        FIGHTSTG_lowerDefense();
        break;
    case TECH_EFFECT_DRAIN_MP:
        FIGHTSTG_drainMp();
        break;
    case TECH_EFFECT_RAISE_ONE_STATUS:
        FIGHTSTG_raiseOneStatus();
        break;
    case TECH_EFFECT_RAISE_EACH_STATUS:
        FIGHTSTG_raiseEachStatus();
        break;
    case TECH_EFFECT_RAISE_ALL_STATUS:
        FIGHTSTG_raiseAllStatus();
        break;
    case TECH_EFFECT_NO_SWITCH:
        FIGHTSTG_tryNoSwitch();
        break;
    }
}


/* Starts side's technique: rolls its hits and damage, then its effect or, for
   the player's plain techniques, the accessories' effects; outside normal
   battles WFIGHTMN_limitDamage caps the damage */
void FIGHTSTG_startAction(u8 side, s32 tech) {
    TechData *entry;

    HEAP.zero(&FIGHTSTG_action, BATTLE_ACTION_STATE_SIZE);
    entry = &TECHS[tech - 1];
    FIGHTSTG_action.side = side;
    FIGHTSTG_action.tech = tech;
    if (entry->effect == TECH_EFFECT_DOUBLE_MAGIC) {
        FIGHTSTG_startDoubleMagic();
    } else if (entry->effect == TECH_EFFECT_END_BATTLE) {
        FIGHTSTG_startEndBattle();
    } else {
        /* the match depends on the fighter's pointer sum */
        if (entry->effect == TECH_EFFECT_ENEMY_ONLY &&
            (side == 0 || FIGHTSTG_battleTableFunc((FIGHTSTG_battle.state.fighters[1] + FIGHTSTG_battle.state.active[1])->id)->techs[0] != tech)) {
            FIGHTSTG_markEnemyOnly();
            return;
        }
        switch (entry->icon) {
        case TECH_PHYSICAL:
            FIGHTSTG_action.hits[0] = FIGHTSTG_battleFuncs.rollHit(side, tech);
            FIGHTSTG_action.hitCount++;
            FIGHTSTG_action.damage = FIGHTSTG_battleFuncs.computeDamage(side, tech);
            /* the match depends on the second test of TECH_EFFECT_MULTI_HIT,
               which the compiler merges with the first and with case 3's */
            if (side == 0) {
                if (entry->effect < TECH_EFFECT_FIRST) {
                    if (entry->script == 11 || entry->script == 12) {
                        break;
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].tripleHit) {
                        FIGHTSTG_rollMultiHit();
                        break;
                    }
                    if (FIGHTSTG_action.hits[0] == 0) {
                        break;
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].statuses[HIT_POISON].chance) {
                        FIGHTSTG_tryPoison();
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].statuses[HIT_PARALYSIS].chance) {
                        FIGHTSTG_tryParalysis();
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].statuses[HIT_CONFUSION].chance) {
                        FIGHTSTG_tryConfusion();
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].statuses[HIT_KNOCK_OUT].chance) {
                        FIGHTSTG_tryKnockOut();
                    }
                    if (FIGHTSTG_battleFuncs.stats[0].statuses[HIT_DRAIN].chance) {
                        FIGHTSTG_tryDrain();
                    }
                } else if (entry->effect == TECH_EFFECT_MULTI_HIT) {
                    FIGHTSTG_rollMultiHit();
                } else if (FIGHTSTG_action.hits[0]) {
                    FIGHTSTG_applyTechEffect();
                }
            } else if (entry->effect >= TECH_EFFECT_FIRST) {
                if (entry->effect == TECH_EFFECT_MULTI_HIT) {
                    FIGHTSTG_rollMultiHit();
                } else if (FIGHTSTG_action.hits[0]) {
                    FIGHTSTG_applyTechEffect();
                }
            }
            break;
        case TECH_MAGIC:
            FIGHTSTG_action.hits[0] = FIGHTSTG_battleFuncs.rollMagicHit(side, tech);
            FIGHTSTG_action.hitCount++;
            FIGHTSTG_action.damage = FIGHTSTG_battleFuncs.computeMagicDamage(side, tech);
            if (entry->effect < TECH_EFFECT_FIRST) {
                break;
            }
            if (entry->effect == TECH_EFFECT_MULTI_HIT) {
                FIGHTSTG_rollMultiHit();
            } else if (FIGHTSTG_action.hits[0]) {
                FIGHTSTG_applyTechEffect();
            }
            break;
        }
    }
    if (FIGHTSTG_battle.state.kind != BATTLE_KIND_NORMAL) {
        FIGHTSTG_action.damage = WFIGHTMN_limitDamage(side, FIGHTSTG_action.damage, FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT] ? FIGHTSTG_action.hitsLanded : 0);
    }
}
