/* FIGHTSTG's technique action: the task that plays a technique, its messages
   and the boosts and statuses it gives. */

#include "battle/battle_mode.h"

/* the techniques that boost a stat of a fighter (FIGHTSTG_updateTechAction): the user's,
   or the enemy's for an amount below 0 */
TechBoost FIGHTSTG_techBoosts[] = {
    { 0xC6, 1, 0, 0x30 },
    { 0xC8, 1, 1, 0x31 },
    { 0xCA, 1, 2, 0x32 },
    { 0xCC, -1, 0, 0x33 },
    { 0xCD, -1, 1, 0x34 },
    { 0xCE, -1, 1, 0x34 },
    { 0xCF, -1, 2, 0x35 },
    { 0xD0, -1, 2, 0x35 },
    { -1, 0, 0, 0 },
};
/* and the ones that boost a stat of all the user's side */
TechBoost FIGHTSTG_sideBoosts[] = {
    { 0xC7, 1, 0, 0x30 },
    { 0xC9, 1, 1, 0x31 },
    { 0xCB, 1, 2, 0x32 },
    { -1, 0, 0, 0 },
};
/* techniques 0xBE-0xC5's four kinds of status (FIGHTSTG_updateTechAction): the flag of
   each in BattleFighter.flags, */
u8 FIGHTSTG_statusTechFlags[] = {
    FIGHTER_POISONED, FIGHTER_PARALYZED, FIGHTER_CONFUSED, 0x3F, /* all six */
};
/* the event types that the European version's FIGHTSTG_reviveFighter clears */
#if VERSION_EU || VERSION_JP
s32 FIGHTSTG_clearIds[] = {
    EVENT_STATUS_DAMAGE, EVENT_STATUS_END, EVENT_STATUS_END + 1, EVENT_STATUS_END + 2,
    EVENT_RESTRICTION_END, EVENT_RESTRICTION_END + 1,
};
#endif
/* the argument of message 9, which the odd techniques show, */
s32 FIGHTSTG_statusTechArgs[] = {
    1, 2, 3, 4,
};
/* and the message shown when the target has the status */
s32 FIGHTSTG_statusTechLines[] = {
    40, 41, 42, 44,
};

/* Starts the technique: its action for a physical or magic one, its message,
   and whether the other side's fighter is asleep */
static inline void announceTech(TechAction *task, BattleChild *children) {
    TechData *tech = &TECHS[task->tech - 1];

    if (tech->icon == TECH_PHYSICAL || tech->icon == TECH_MAGIC) {
        FIGHTSTG_action.start(task->side, task->tech);
    } else {
        HEAP.zero(&FIGHTSTG_action, BATTLE_ACTION_STATE_SIZE);
    }
    children[0].message = FIGHTSTG_createMessage();
    if (tech->script == 0xC) {
        task->lines[0] = 0x3A;
        task->lines[1] = task->side;
        children[0].message->show(children[0].message, 2, task->lines);
    } else {
        task->lines[0] = task->side;
        task->lines[1] = task->tech;
        children[0].message->show(children[0].message, 3, task->lines);
    }
    {
        s32 other = 1 - (task->side >> 4);
        BattleFighter *fighter = FIGHTSTG_getActiveFighter(other);

        if (fighter->flags & FIGHTER_ASLEEP) {
            task->asleep = 1;
        }
    }
    task->nextState(task);
}

/* Once the message is gone, starts the technique's script and spends the
   user's charge, or for an enemy-only effect ends the user's weakness */
static inline void startTechScript(TechAction *task, BattleChild *children) {
    if (children[0].task == NULL) {
        if (FIGHTSTG_action.effects[TECH_EFFECT_ENEMY_ONLY] == 0) {
            children[0].script = WFIGHTMN_startTech(task->side, task->tech);
            {
                BattleFighter *fighters = FIGHTSTG_battle.state.fighters[0];

                if (task->side != 0) {
                    fighters = FIGHTSTG_battle.state.fighters[1];
                }
                fighters[FIGHTSTG_battle.state.active[task->side != 0]].charge = 0;
            }
        } else {
            WFIGHTMN_endWeakness(task->side);
        }
        task->substate++;
    }
}

/* A physical or magic technique: deals its damage to the other side's fighter
   (or its hits', or knocks it out), or says it missed; one with
   TECH_EFFECT_ENEMY_ONLY is queued as the partner's technique instead */
static inline void dealTechDamage(TechAction *task, BattleChild *children) {
    s32 other;

    children[0].message = FIGHTSTG_createMessage();
    other = task->side == 0;
    if (FIGHTSTG_action.effects[TECH_EFFECT_ENEMY_ONLY]) {
        FIGHTSTG_queuePartnerTech(task->tech);
        task->state = 3;
        return;
    }
    if (FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT]) {
        if (TECHS[task->tech - 1].effect == TECH_EFFECT_DOUBLE_MAGIC) {
            task->damage = FIGHTSTG_action.hitDamage[0] + FIGHTSTG_action.hitDamage[1];
            task->lines[0] = other << 4;
            task->lines[1] = task->damage;
            children[0].message->show(children[0].message, 4, task->lines);
        } else {
#if VERSION_US
            task->damage = FIGHTSTG_action.damage * FIGHTSTG_action.hitsLanded;
#endif
            task->lines[0] = other << 4;
            task->lines[1] = FIGHTSTG_action.damage;
            task->lines[2] = FIGHTSTG_action.hitsLanded;
            children[0].message->show(children[0].message, 0x10, task->lines);
#if VERSION_EU || VERSION_JP
            task->damage = FIGHTSTG_action.damage * FIGHTSTG_action.hitsLanded;
            if (task->damage >= 10000) {
                task->damage = 9999;
            }
#endif
        }
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_KNOCK_OUT]) {
        FIGHTSTG_getActiveFighter(other)->hp = 0;
        FIGHTSTG_queueKnockOut(other << 4);
        children[0].task->state = 3;
    } else if (FIGHTSTG_action.hits[0]) {
        task->lines[0] = other << 4;
        task->lines[1] = FIGHTSTG_action.damage;
        children[0].message->show(children[0].message, 4, task->lines);
        task->damage = FIGHTSTG_action.damage;
    } else {
        task->lines[0] = 0x1D;
        task->lines[1] = other << 4;
        children[0].message->show(children[0].message, 2, task->lines);
    }
    if (task->damage != 0) {
        BattleFighter *fighter = FIGHTSTG_getActiveFighter(other);

        fighter->hp -= task->damage;
        if (fighter->hp <= 0) {
            fighter->hp = 0;
            FIGHTSTG_queueKnockOut(other << 4);
            task->step = 1;
        }
    }
    task->substate++;
}

/* Techniques 0xBE-0xC5: cure one of the four kinds of status
   (FIGHTSTG_statusTechFlags), the odd ones on the user's whole side */
static inline void cureStatusTech(TechAction *task, BattleChild *children) {
    s32 other;
    s32 kind;
    BattleFighter *fighters;
    s32 i;

    other = task->side != 0;
    switch (task->tech) {
    case 0xBE:
    case 0xBF:
    default:
        kind = 0;
        break;
    case 0xC0:
    case 0xC1:
        kind = 1;
        break;
    case 0xC2:
    case 0xC3:
        kind = 2;
        break;
    case 0xC4:
    case 0xC5:
        kind = 3;
        break;
    }
    fighters = FIGHTSTG_battle.state.fighters[other];
    switch (task->step) {
    case 0:
    default:
        children[0].message = FIGHTSTG_createMessage();
        if (task->tech & 1) {
            task->lines[0] = task->side;
            task->lines[1] = FIGHTSTG_statusTechArgs[kind];
            children[0].message->show(children[0].message, 9, task->lines);
            task->step = 1;
            task->counter = 3;
            return;
        }
        if (fighters[FIGHTSTG_battle.state.active[other]].flags & FIGHTSTG_statusTechFlags[kind]) {
            task->lines[0] = FIGHTSTG_statusTechLines[kind];
            task->lines[1] = task->side;
            task->lines[2] = FIGHTSTG_battle.state.active[0];
            children[0].message->show(children[0].message, 2, task->lines);
            task->step = 1;
            task->counter = 1;
            return;
        }
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
        break;
    case 1:
        for (i = 0; i < task->counter; i++) {
            FIGHTSTG_events.funcs.cureStatus(task->side, i, task->tech);
        }
        break;
    }
    task->nextSubstate(task);
}

/* Technique 0x64: revives the partners knocked out to their max HP */
static inline void reviveSide(TechAction *task, BattleChild *children) {
    BattleFighter *fighter;
    s32 i;

    fighter = FIGHTSTG_battle.state.fighters[0];
    switch (task->step) {
    case 0:
    default:
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0;
        task->lines[1] = 5;
        children[0].message->show(children[0].message, 9, task->lines);
        task->step++;
        return;
    case 1:
        for (i = 0; i < 3; i++) {
            if (fighter[i].id != 0 && fighter[i].hp == 0) {
                fighter[i].hp = fighter[i].maxHp;
                WFIGHTMN_checkEquip(i);
            }
        }
        break;
    }
    task->nextSubstate(task);
}

/* Technique 0x177: revives the partners and restores them whole, statuses
   included */
static inline void restoreSide(TechAction *task, BattleChild *children) {
#if VERSION_US
    BattleFighter *fighter;
#endif
    s32 i;

#if VERSION_US
    fighter = FIGHTSTG_battle.state.fighters[0];
#endif
    switch (task->step) {
    case 0:
    default:
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0;
        task->lines[1] = 6;
        children[0].message->show(children[0].message, 9, task->lines);
        task->step++;
        return;
    case 1:
#if VERSION_US
        for (i = 0; i < 3; i++) {
            if (fighter[i].id != 0) {
                if (fighter[i].hp == 0) {
                    WFIGHTMN_checkEquip(i);
                }
                fighter[i].flags = 0;
                fighter[i].hp = fighter[i].maxHp;
                FIGHTSTG_queueBoostEnd(SIDE_PLAYER, i, 1, task->tech);
            }
        }
#else
        for (i = 0; i < 3; i++) {
            FIGHTSTG_reviveFighter(0x177, i);
        }
#endif
        break;
    }
    task->nextSubstate(task);
}

/* The other item techniques: heal the user, or its whole side for 0xBB and
   above, by FIGHTSTG_computeHeal, or say they do nothing */
static inline void healWithTech(TechAction *task, BattleChild *children) {
    BattleFighter *fighters;
    s32 kind;

    fighters = FIGHTSTG_battle.state.fighters[0];
    if (task->side != 0) {
        fighters = FIGHTSTG_battle.state.fighters[1];
    }
    switch (task->step) {
    case 0:
    default: {
        s32 i;
        s32 most;

        kind = -1;
        task->heal = FIGHTSTG_battleFuncs.computeHeal(task->side, task->tech);
        most = 0;
        if (task->tech >= 0xBB) {
            for (i = 0; i < 3; i++) {
                if (fighters[i].id != 0 && fighters[i].hp != 0) {
                    s32 lost = fighters[i].maxHp - fighters[i].hp;

                    if (most < lost) {
                        most = lost;
                    }
                }
            }
            if (most != 0) {
                kind = 9;
                if (task->heal < most) {
                    most = task->heal;
                }
                task->lines[1] = 0;
                task->lines[2] = most;
            }
        } else {
            s32 lost;

            i = FIGHTSTG_battle.state.active[task->side != 0];
            lost = fighters[i].maxHp - fighters[i].hp;
            if (most < lost) {
                most = lost;
            }
            if (most != 0) {
                kind = 8;
                if (task->heal < most) {
                    most = task->heal;
                }
                task->heal = most;
                task->lines[1] = most;
            }
        }
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = task->side;
        if (kind != -1) {
            s32 i;

            children[0].message->show(children[0].message, kind, task->lines);
            if (task->tech >= 0xBB) {
                for (i = 0; i < 3; i++) {
                    if (fighters[i].id != 0 && fighters[i].hp != 0) {
                        if (fighters[i].hp + task->heal > fighters[i].maxHp) {
                            fighters[i].hp = fighters[i].maxHp;
                        } else {
                            fighters[i].hp += task->heal;
                        }
                    }
                }
            } else {
                i = FIGHTSTG_battle.state.active[task->side != 0];
                if (fighters[i].hp + task->heal > fighters[i].maxHp) {
                    fighters[i].hp = fighters[i].maxHp;
                } else {
                    fighters[i].hp += task->heal;
                }
            }
            task->nextSubstate(task);
        } else {
            task->lines[0] = 0x2F;
            task->lines[1] = task->side;
            children[0].message->show(children[0].message, 2, task->lines);
            task->nextSubstate(task);
        }
        break;
    }
    case 1:
        return;
    }
}

/* An item technique: an auto-recovery, a cure, a revival or a heal */
static inline void useItemTech(TechAction *task, BattleChild *children) {
    task->lines[0] = task->side;
    switch (task->tech) {
    case 0xBD:
        FIGHTSTG_queueAutoRecoverEnd(task->side);
        FIGHTSTG_queueRecovery(task->side, FIGHTSTG_battle.state.active[task->side != 0], task->tech);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x27;
        task->lines[1] = task->side;
        children[0].message->show(children[0].message, 2, task->lines);
        task->nextSubstate(task);
        break;
    case 0xBE:
    case 0xBF:
    case 0xC0:
    case 0xC1:
    case 0xC2:
    case 0xC3:
    case 0xC4:
    case 0xC5:
        cureStatusTech(task, children);
        break;
    case 0x64:
        reviveSide(task, children);
        break;
    case 0x177:
        restoreSide(task, children);
        break;
    default:
        healWithTech(task, children);
        break;
    }
}

/* Techniques 0xC6-0xD0 (FIGHTSTG_techBoosts): boost a stat of the user, or
   lower the other side's, which the battle can block for the partner's */
static inline void boostFighterStat(TechAction *task, BattleChild *children, TechData *tech) {
    TechBoost *boost;
    s32 other;
    s32 ok;

    for (boost = FIGHTSTG_techBoosts; boost->tech != -1; boost++) {
        if (boost->tech == task->tech) {
            break;
        }
    }
    ok = 1;
    other = task->side >> 4;
    if (boost->amount <= 0) {
        other ^= 1;
        if (task->side == 0 && BATTLE_SETUP.blocks[boost->stat + BATTLE_BLOCK_LOWER_ATTACK]) {
            task->lines[0] = 0x2F;
            children[0].message->show(children[0].message, ok, task->lines);
            ok = 0;
        }
    }
    if (ok == 0) {
        return;
    }
    FIGHTSTG_battleFuncs.changeBoost(other << 4, FIGHTSTG_battle.state.active[other], boost->stat, boost->amount * tech->effectPower);
    FIGHTSTG_queueBoostEnd(other << 4, FIGHTSTG_battle.state.active[other], boost->stat, task->tech);
    task->lines[0] = boost->line;
    task->lines[1] = other << 4;
    task->lines[2] = FIGHTSTG_battle.state.active[other];
    children[0].message->show(children[0].message, 2, task->lines);
}

/* Techniques 0xC7, 0xC9 and 0xCB (FIGHTSTG_sideBoosts): boost a stat of all
   the user's side */
static inline void boostSideStat(TechAction *task, BattleChild *children, TechData *tech) {
    TechBoost *boost;
    s32 i;

    for (boost = FIGHTSTG_sideBoosts; boost->tech != -1; boost++) {
        if (boost->tech == task->tech) {
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (FIGHTSTG_battle.state.fighters[task->side >> 4][i].id != 0 && FIGHTSTG_battle.state.fighters[task->side >> 4][i].hp > 0) {
            FIGHTSTG_battleFuncs.changeBoost(task->side, i, boost->stat, boost->amount * tech->effectPower);
            FIGHTSTG_queueBoostEnd(task->side, i, boost->stat, task->tech);
        }
    }
    task->lines[0] = task->side;
    switch (boost->stat) {
    case 0:
        task->lines[1] = 7;
        break;
    case 1:
        task->lines[1] = 8;
        break;
    case 2:
        task->lines[1] = 9;
        break;
    }
    children[0].message->show(children[0].message, 9, task->lines);
}

/* Techniques 0xD1 and 0xD2: charge the user by the technique's power */
static inline void chargeUser(TechAction *task, BattleChild *children, TechData *tech) {
    s32 other = task->side >> 4;

    FIGHTSTG_getActiveFighter(other)->charge = tech->effectPower;
    task->lines[0] = 0x36;
    task->lines[1] = task->side;
    children[0].message->show(children[0].message, 2, task->lines);
}

/* Technique 0x188: the enemy drains the partner's MP by the technique's power
   in 128ths of its max MP */
static inline void drainMp(TechAction *task, BattleChild *children, TechData *tech) {
    BattleFighter *to = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
    BattleFighter *from = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    s32 amount;

    if (from->mp != 0) {
        amount = from->maxMp * tech->effectPower / 128;
        if (from->mp < amount) {
            amount = from->mp;
        }
        to->mp += amount;
        from->mp -= amount;
        if (to->mp > to->maxMp) {
            to->mp = to->maxMp;
        }
        if (from->mp < 0) {
            from->mp = 0;
        }
        task->lines[0] = 0x10;
        task->lines[1] = amount;
        task->lines[2] = 1;
        children[0].message->show(children[0].message, 0x12, task->lines);
    } else {
        task->lines[0] = 0x2F;
        task->lines[1] = 0x10;
        children[0].message->show(children[0].message, 2, task->lines);
    }
}

/* Technique 0x190: heals the enemy by FIGHTSTG_computeHeal and charges it */
static inline void healEnemy(TechAction *task, BattleChild *children) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];

    task->heal = FIGHTSTG_battleFuncs.computeHeal(task->side, 0x190);
    fighter->hp += task->heal;
    if (fighter->hp > fighter->maxHp) {
        fighter->hp = fighter->maxHp;
    }
    fighter->charge = 0x20;
    task->lines[0] = 0x46;
    task->lines[1] = 0x10;
    children[0].message->show(children[0].message, 2, task->lines);
}

/* A support technique: a boost, a charge, a restriction or digidevolving the
   enemy, or the enemy's drain of MP or heal */
static inline void useSupportTech(TechAction *task, BattleChild *children, TechData *tech) {
    children[0].message = FIGHTSTG_createMessage();
    switch (task->tech) {
    case 0xC6:
    case 0xC8:
    case 0xCA:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xD0:
        boostFighterStat(task, children, tech);
        break;
    case 0xC7:
    case 0xC9:
    case 0xCB:
        boostSideStat(task, children, tech);
        break;
    case 0xD1:
    case 0xD2:
        chargeUser(task, children, tech);
        break;
    case 0xD3:
        if (FIGHTSTG_battleFuncs.rollNoSwitch(SIDE_ENEMY, 0xD3)) {
            FIGHTSTG_startRestriction(0xD3);
            task->lines[0] = 0x44;
            task->lines[1] = 0;
            children[0].message->show(children[0].message, 2, task->lines);
        } else {
            task->lines[0] = 0x2F;
            children[0].message->show(children[0].message, 1, task->lines);
        }
        break;
    case 0xD4:
        if (FIGHTSTG_battleFuncs.rollNoDigivolve(SIDE_ENEMY, 0xD4)) {
            FIGHTSTG_startRestriction(0xD4);
            task->lines[0] = 0x45;
            task->lines[1] = 0;
            children[0].message->show(children[0].message, 2, task->lines);
        } else {
            task->lines[0] = 0x2F;
            children[0].message->show(children[0].message, 1, task->lines);
        }
        break;
    case 0x187:
        if (FIGHTSTG_battleFuncs.rollDedigivolve(SIDE_ENEMY, 0x187)) {
            FIGHTSTG_queueDigidevolve();
            children[0].task->state = 3;
        } else {
            task->lines[0] = 0x2F;
            children[0].message->show(children[0].message, 1, task->lines);
        }
        break;
    case 0x188:
        drainMp(task, children, tech);
        break;
    case 0x190:
        healEnemy(task, children);
        break;
    }
    task->nextSubstate(task);
}

/* Icon 6's techniques: set the field's boosted element and its amount (the
   user's spirit / 10 plus the technique's power, at most 0x7F), cleared after
   a time set by the spirit. The element boosted is the technique's icon, so
   always 6, while the message names the technique's own element. */
static inline void boostFieldElement(TechAction *task, BattleChild *children, TechData *tech) {
    BattleStats *stats;
    s16 spiritBonus;
    s32 amount;

    children[0].message = FIGHTSTG_createMessage();
    stats = FIGHTSTG_battleFuncs.computeStats(task->side, 1, FIGHTSTG_battle.state.active[task->side == 0x10]);
    FIGHTSTG_battle.state.boost.element = tech->icon;
    spiritBonus = stats->stats[BATTLE_STAT_SPIRIT] / 10;
    amount = spiritBonus + tech->effectPower;
    if (amount >= 0x80) {
        amount = 0x7F;
    }
    FIGHTSTG_battle.state.boost.amount = amount;
    FIGHTSTG_queueClearField(stats->stats[BATTLE_STAT_SPIRIT] * 12 + 1000);
    task->lines[0] = tech->element + 0x61;
    children[0].message->show(children[0].message, 1, task->lines);
    task->nextSubstate(task);
}

/* After the technique: wakes the other side's fighter when it was asleep and
   the damage wakes it (or ends), else starts the motion of the damage */
static inline void wakeUpOrCounter(TechAction *task, BattleChild *children) {
    s32 other = task->side == 0;
    BattleFighter *fighter = FIGHTSTG_getActiveFighter(other);
    TechData *tech;

    tech = &TECHS[task->tech - 1];
    if (fighter->flags & FIGHTER_ASLEEP) {
        if (task->asleep != 0 && FIGHTSTG_battleFuncs.testWakeUp(other << 4, task->damage) != 0) {
            task->lines[0] = 0x2B;
            task->lines[1] = SIDE_ENEMY - task->side;
            task->lines[2] = FIGHTSTG_battle.state.active[other];
            children[0].message = FIGHTSTG_createMessage();
            children[0].message->show(children[0].message, 7, task->lines);
            fighter->flags &= ~FIGHTER_ASLEEP;
            {
                s32 event = FIGHTSTG_events.funcs.find(EVENT_STATUS_END + 2, SIDE_ENEMY - task->side, FIGHTSTG_battle.state.active[other]);

                if (event >= 0) {
                    FIGHTSTG_events.events[event].type = 0;
                }
            }
            task->substate = 9;
        } else {
            task->state = 3;
        }
    } else if (tech->icon == TECH_PHYSICAL) {
        children[0].counter = FIGHTSTG_startCounterattack((task->side == 0) << 4, task->damage, tech->script == 0xC);
        task->substate = 6;
    } else {
        task->substate = 5;
    }
}

/* A side's technique or item (FIGHTSTG_startTechAction): state 0 shows its
   message (announceTech), substate 1 does what it does by its kind (2 and 3
   deal damage (dealTechDamage), 4 is an item's cure, revival or healing
   (useItemTech), 5 a boost, a drain or a heal of the enemy (useSupportTech),
   6 a boost of the field's element (boostFieldElement)), then
   the wake-up when the other side's fighter is asleep (substate 4,
   wakeUpOrCounter) and the motion of the damage
   (FIGHTSTG_startCounterattack). The match depends on the other side's
   fighter being found as its row's offset added as an int to its slot (as
   in FIGHTSTG_updateFirstTech), on each case's variables being its own, on substate
   1's cases advancing substate themselves (case 5 once, after its switch)
   and on the heal tests being written hp + heal > maxHp. */
void FIGHTSTG_updateTechAction(TechAction *task, BattleChild *children) {
    TechData *tech;

    switch (task->state) {
    case TASK_INIT:
    default:
        announceTech(task, children);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            startTechScript(task, children);
            break;
        case 1:
            if (children[0].task != NULL) {
                break;
            }
            tech = &TECHS[task->tech - 1];
            switch (tech->icon) {
            default:
                return;
            case TECH_PHYSICAL:
            case TECH_MAGIC:
                dealTechDamage(task, children);
                return;
            case 4:
                useItemTech(task, children);
                break;
            case 5:
                useSupportTech(task, children, tech);
                break;
            case 6:
                boostFieldElement(task, children, tech);
                return;
            }
            break;
        case 2:
            if (children[0].task == NULL) {
                TechData *tech = &TECHS[task->tech - 1];

                if ((tech->icon == TECH_PHYSICAL || tech->icon == TECH_MAGIC) && task->step == 0) {
                    if (task->damage == 0) {
                        if (task->side == 0 && FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
                            task->setSubstate(task, 7);
                            break;
                        }
                    } else {
                        children[0].events = FIGHTSTG_startActionEvents(task->side);
                        task->nextSubstate(task);
                        break;
                    }
                }
                task->state = 3;
            }
            break;
        case 3:
            if (children[0].task == NULL) {
                task->substate++;
            }
            break;
        case 4:
            wakeUpOrCounter(task, children);
            break;
        case 6:
            if (children[0].task != NULL) {
                if (children[0].task->state != 2) {
                    break;
                }
            case 5:
                WFIGHTMN_chargeGauge(task->side, task->damage);
            }
            task->state = 3;
            break;
        case 7:
            if (children[0].task == NULL) {
                TechData *tech = &TECHS[task->tech - 1];

                children[0].enemyAttack = FIGHTSTG_startEnemyAttack(1, tech->script == 0xC);
                task->substate++;
            }
            break;
        case 8:
            if (children[0].task == NULL) {
                task->setState(task, 3);
            }
            break;
        case 9:
            if (children[0].task == NULL) {
                TechData *tech = &TECHS[task->tech - 1];

                if (tech->icon == TECH_PHYSICAL) {
                    children[0].counter = FIGHTSTG_startCounterattack((task->side == 0) << 4, task->damage, tech->script == 0xC);
                    task->substate = 6;
                } else {
                    task->substate = 5;
                }
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts side's (0 or 0x10) technique or item tech */
TechAction *FIGHTSTG_startTechAction(s32 side, s32 tech) {
    TechAction *task = createTask(FIGHTSTG_updateTechAction, sizeof(TechAction), sizeof(Task *));

    task->side = side;
    task->tech = tech;
    return task;
}
