/* FIGHTSTG's counterattack, and the European version's revival of a fighter. */

#include "battle/battle_mode.h"

/* Starts the battle script of the counterattack's technique, with its
   element's stage, and its hit result, and empties the countering fighter's
   charge. */
static inline void playCounterTech(Counterattack *task, BattleChild *children) {
    TechData *tech = &TECHS[task->tech - 1];
    s32 other = task->side != 0;
    BattleFighter *row;
    s32 element;

    children[0].script = FIGHTSTG_createBattleScript();
    children[0].script->enemy = task->side;
    if (task->side == 0 && tech->script == 5) {
        children[0].script->index = 6;
    } else {
        children[0].script->index = tech->script;
    }
    if (tech->scriptSound != 0) {
        children[0].script->sound = tech->scriptSound;
    }
    {
        BattleStats *stats = FIGHTSTG_battleFuncs.computeStats(task->side, 1, FIGHTSTG_battle.state.active[other]);

        if (tech->element >= ELEMENT_FIRST || stats->element >= ELEMENT_FIRST) {
            if (tech->element >= ELEMENT_FIRST) {
                element = tech->element - ELEMENT_FIRST;
            } else {
                element = stats->element - ELEMENT_FIRST;
            }
            element = element * 3 + 0x21;
            if (stats->elementPower >= 0x40) {
                children[0].script->stage = element + 1;
            } else {
                children[0].script->stage = element;
            }
        } else {
            children[0].script->stage = -1;
        }
    }
    if (tech->scriptEffect != 0) {
        children[0].script->effect = tech->scriptEffect;
    } else if (tech->script == 5) {
        children[0].script->effect = 0x2E;
        children[0].script->sound = 0x1E;
    }
    row = FIGHTSTG_battle.state.fighters[1 - other];
    if (task->hit != 0) {
        if (row[FIGHTSTG_battle.state.active[1 - other]].hp - task->damage <= 0) {
            children[0].script->hits[3] = 2;
        } else {
            children[0].script->hits[3] = 1;
            WFIGHTMN_countHit(task->side, task->damage);
            WFIGHTMN_setIdleMotion(SIDE_ENEMY - task->side, task->damage);
        }
    } else {
        children[0].script->hits[3] = 3;
    }
    row = FIGHTSTG_battle.state.fighters[0];
    if (task->side != 0) {
        row = FIGHTSTG_battle.state.fighters[1];
    }
    row[FIGHTSTG_battle.state.active[task->side != 0]].charge = 0;
}

/* Shows whether the counterattack hit and for how much, and takes the damage
   from the countered fighter's HP (queueing its knockout at 0). */
static inline void showCounterResult(Counterattack *task, BattleChild *children) {
    children[0].message = FIGHTSTG_createMessage();
    if (task->hit != 0) {
        task->lines[0] = (task->side == 0) << 4;
        task->lines[1] = task->damage;
        children[0].message->show(children[0].message, 4, task->lines);
    } else {
        task->lines[0] = 0x1D;
        task->lines[1] = (task->side == 0) << 4;
        children[0].message->show(children[0].message, 2, task->lines);
    }
    if (task->damage != 0) {
        s32 index = task->side == 0;
        BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[index][FIGHTSTG_battle.state.active[index]];

        fighter->hp -= task->damage;
        if (fighter->hp <= 0) {
            fighter->hp = 0;
            if (task->noKnockOutEvent == 0) {
                FIGHTSTG_queueKnockOut(index << 4);
            }
        }
    }
}

/* A counterattack (FIGHTSTG_startCounterattack): the player's from an event of type 8 or
   the partner's first technique, the enemy's from its table entry; substate
   0 plays the technique (playCounterTech), 1 shows the damage and takes the
   HP (showCounterResult), 2 hands received to WFIGHTMN_chargeGauge. The
   match depends on playCounterTech's technique and side being declared in
   it, on the event index and the stats being variables of their own, on the
   stage being written element * 3 + 0x21 and one more, on the enemy's
   fighter being found as a pointer sum and on one row variable for both of
   playCounterTech's rows. */
void FIGHTSTG_updateCounterattack(Counterattack *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleTableEntry *entry;
    s32 other;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->received == 0) {
            task->state = TASK_DONE;
            break;
        }
        other = task->side != 0;
        fighter = &FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]];
        if ((fighter->flags & FIGHTER_PARALYZED) && FIGHTSTG_battleFuncs.testParalysis(task->side) != 0) {
            task->state = TASK_DONE;
            break;
        }
        if (task->side == 0) {
            s32 index = FIGHTSTG_events.funcs.find(EVENT_PARTNER_TECH, 0, FIGHTSTG_battle.state.active[0]);

            if (index >= 0) {
                task->tech = FIGHTSTG_events.events[index].args[2];
                task->substate = 0;
                FIGHTSTG_events.events[index].type = 0;
#if VERSION_US
            } else if (FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0])->counter != 0) {
#elif VERSION_EU || VERSION_JP
            } else if (FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0])->counter != 0
                       && FIGHTSTG_battleFuncs.rollCounter(0, task->received) != 0) {
#endif
                task->tech = GET_DIGIMON(fighter->id)->skills[0];
                task->substate = 1;
            } else {
                task->state = TASK_DONE;
                break;
            }
        } else {
            entry = FIGHTSTG_battleTableFunc(fighter->id);
            if (entry->counter.condition == 0) {
                task->state = TASK_DONE;
                break;
            }
            if (FIGHTSTG_testEnemyCondition(entry->counter.condition, entry->counter.conditionArg) == 0) {
                task->state = TASK_DONE;
                break;
            }
            task->tech = FIGHTSTG_getEnemyAction(entry->counter.target);
            if (task->tech == 1) {
                task->tech = FIGHTSTG_battleTableFunc((FIGHTSTG_battle.state.fighters[1] + FIGHTSTG_battle.state.active[1])->id)->techs[0];
                task->substate = 1;
            }
        }
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = task->side;
        task->lines[1] = task->tech;
        children[0].message->show(children[0].message, task->substate + 5, task->lines);
        task->hit = FIGHTSTG_battleFuncs.rollHit(task->side, task->tech);
        if (task->hit != 0) {
            task->damage = FIGHTSTG_battleFuncs.computeCounterDamage(task->side, task->tech, task->received);
            if (FIGHTSTG_battle.state.kind != BATTLE_KIND_NORMAL) {
                task->damage = WFIGHTMN_limitDamage(task->side, task->damage, 0);
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            if (children[0].message == NULL) {
                playCounterTech(task, children);
                task->substate++;
            }
            break;
        case 1:
            if (children[0].script == NULL) {
                showCounterResult(task, children);
                task->substate++;
            }
            break;
        case 2:
            if (children[0].message == NULL) {
                WFIGHTMN_chargeGauge(task->side, task->received);
                task->state = TASK_KILL;
            }
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Starts SIDE's counterattack (FIGHTSTG_updateCounterattack) to RECEIVED
   damage; NOKNOCKOUTEVENT keeps a knockout from queueing its event */
Counterattack *FIGHTSTG_startCounterattack(s32 side, s32 received, s32 noKnockOutEvent) {
    Counterattack *task = createTask(FIGHTSTG_updateCounterattack, sizeof(Counterattack), 4);

    task->side = side;
    task->received = received;
    task->noKnockOutEvent = noKnockOutEvent;
    return task;
}

#if VERSION_EU || VERSION_JP
/* Revives one of the player's fighters at full HP, clearing its status
   events, and raises its defense by tech's effectPower (FIGHTSTG_battleFuncs.changeBoost) */
void FIGHTSTG_reviveFighter(s32 tech, s32 fighter) {
    BattleFighter *fighters = FIGHTSTG_battle.state.fighters[0];
    TechData *entry = &TECHS[tech - 1];
    s32 index;
    s32 i;

    if (fighters[fighter].id == 0) {
        return;
    }
    for (i = 0; i < 6; i++) {
        index = FIGHTSTG_events.funcs.find(FIGHTSTG_clearIds[i], 0, fighter);
        if (index >= 0) {
            FIGHTSTG_events.events[index].type = 0;
        }
    }
    if (fighters[fighter].hp == 0) {
        WFIGHTMN_checkEquip(fighter);
    }
    fighters[fighter].flags = 0;
    fighters[fighter].hp = fighters[fighter].maxHp;
    FIGHTSTG_battleFuncs.changeBoost(0, fighter, 1, entry->effectPower);
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, fighter, 1, tech);
}
#endif
