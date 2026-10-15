/* FIGHTSTG's first technique, the one a side starts the battle with. */

#include "battle/battle_mode.h"

/* Picks the side's first technique and says it is used (or, for an enemy
   without the MP, that it is not), noting whether the other side's fighter
   was asleep. */
static inline void announceFirstTech(FirstTech *task, BattleChild *children) {
    BattleFighter *fighter;
    TechData *tech;

    if (task->side == 0) {
        task->tech = GET_DIGIMON(FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]].id)->skills[0];
    } else {
        task->tech = FIGHTSTG_battleTableFunc(FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]].id)->techs[0];
    }
    fighter = (FIGHTSTG_battle.state.fighters[1] + FIGHTSTG_battle.state.active[1]);
    tech = &TECHS[task->tech - 1];
    if (task->side != 0 && tech->mp > fighter->mp) {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x8D;
        task->lines[1] = task->side;
        children[0].message->show(children[0].message, 2, task->lines);
        task->substate = 2;
    } else {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 8;
        task->lines[1] = task->side;
        children[0].message->show(children[0].message, 2, task->lines);
        FIGHTSTG_action.start(task->side, task->tech);
    }
    {
        s32 other = 1 - (task->side >> 4);

        fighter = FIGHTSTG_getActiveFighter(other);
    }
    if (fighter->flags & FIGHTER_ASLEEP) {
        task->asleep = 1;
    }
}

/* Shows the technique's result (its hits, its knockout, the 70 percent of
   the partner's HP that TECH_EFFECT_END_BATTLE takes, its damage or its
   miss) and keeps the damage to take. */
static inline void showFirstTechResult(FirstTech *task, BattleChild *children) {
    children[0].message = FIGHTSTG_createMessage();
    if (FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT]) {
        task->lines[0] = SIDE_ENEMY - task->side;
        task->lines[1] = FIGHTSTG_action.damage;
        task->lines[2] = FIGHTSTG_action.hitsLanded;
        children[0].message->show(children[0].message, 0x10, task->lines);
        task->damage = FIGHTSTG_action.hitsLanded * FIGHTSTG_action.damage;
#if VERSION_EU || VERSION_JP
        if (task->damage >= 10000) {
            task->damage = 9999;
        }
#endif
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_KNOCK_OUT]) {
        s32 other = task->side == 0;

        FIGHTSTG_getActiveFighter(other)->hp = 0;
        FIGHTSTG_queueKnockOut(other << 4);
        children[0].task->state = 3;
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_END_BATTLE]) {
        task->damage = (FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0])->hp * 7 / 10;
        task->lines[0] = 0;
        task->lines[1] = task->damage;
        children[0].message->show(children[0].message, 4, task->lines);
    } else if (FIGHTSTG_action.hits[0]) {
        task->lines[0] = (task->side == 0) << 4;
        task->lines[1] = FIGHTSTG_action.damage;
        children[0].message->show(children[0].message, 4, task->lines);
        task->damage = FIGHTSTG_action.damage;
    } else {
        task->lines[0] = 0x1D;
        task->lines[1] = (task->side == 0) << 4;
        children[0].message->show(children[0].message, 2, task->lines);
    }
}

/* Takes the damage from the other side's active fighter, queueing its
   knockout at 0 HP (and setting step, which ends the technique). */
static inline void takeFirstTechDamage(FirstTech *task) {
    BattleFighter *struck;

    if (task->damage != 0) {
        s32 other = task->side == 0;

        struck = FIGHTSTG_getActiveFighter(other);
        struck->hp -= task->damage;
        if (struck->hp <= 0) {
            struck->hp = 0;
            FIGHTSTG_queueKnockOut((task->side == 0) << 4);
            task->step = 1;
        }
    }
}

/* Wakes up COUNTERED, the sleeping fighter the technique struck in row
   INDEX, says so and ends its sleep's event. */
static inline void wakeCounteredUp(FirstTech *task, BattleChild *children, BattleFighter *countered, s32 index) {
    task->lines[0] = 0x2B;
    task->lines[1] = SIDE_ENEMY - task->side;
    task->lines[2] = FIGHTSTG_battle.state.active[index];
    children[0].message = FIGHTSTG_createMessage();
    children[0].message->show(children[0].message, 7, task->lines);
    countered->flags &= ~FIGHTER_ASLEEP;
    {
        s32 event = FIGHTSTG_events.funcs.find(0xC, SIDE_ENEMY - task->side, FIGHTSTG_battle.state.active[index]);

        if (event >= 0) {
            FIGHTSTG_events.events[event].type = 0;
        }
    }
}

/* A side's first technique (FIGHTSTG_startFirstTech): the partner's skills[0] or the
   enemy's battle table techs[0], its message (announceFirstTech) and WFIGHTMN's
   WFIGHTMN_startTech, then the damage to the other side's active fighter
   (showFirstTechResult, takeFirstTechDamage: TECH_EFFECT_KNOCK_OUT knocks it out,
   TECH_EFFECT_END_BATTLE takes 70 percent of the partner's HP), its waking up when
   that fighter is asleep (substate 4, wakeCounteredUp) and FIGHTSTG_startEnemyAttack
   when the partner misses in BATTLE_KIND_FINAL_LAST. The match depends on the other
   side's fighter being found as its row's offset, other * 0x60, added as an int to its
   slot in the first row, each in a variable of its own in a block of its own (as an
   index, gcc folds the row into (other * 3 + active) * 32); on the blocks
   of their own of substate 0's row and of the event that wakeCounteredUp finds;
   on announceFirstTech's MP test being written tech->mp > fighter->mp; on
   TECH_EFFECT_END_BATTLE's damage being stored before lines[0] and on the pointer sum
   of its HP. */
void FIGHTSTG_updateFirstTech(FirstTech *task, BattleChild *children) {
    BattleFighter *countered;
    s32 index;

    switch (task->state) {
    case TASK_INIT:
    default:
        announceFirstTech(task, children);
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            if (children[0].task == NULL) {
                children[0].script = WFIGHTMN_startTech(task->side, task->tech);
                {
                    BattleFighter *fighters = FIGHTSTG_battle.state.fighters[0];

                    if (task->side != 0) {
                        fighters = FIGHTSTG_battle.state.fighters[1];
                    }
                    fighters[FIGHTSTG_battle.state.active[task->side != 0]].charge = 0;
                }
                task->substate++;
            }
            break;
        case 1:
            if (children[0].task == NULL) {
                showFirstTechResult(task, children);
                takeFirstTechDamage(task);
                task->substate++;
            }
            break;
        case 2:
            if (children[0].task == NULL) {
                if (FIGHTSTG_action.effects[TECH_EFFECT_END_BATTLE]) {
                    children[0].events = FIGHTSTG_startActionEvents(task->side);
                    task->nextSubstate(task);
                } else if (task->step != 0) {
                    task->state = 3;
                } else if (task->damage == 0) {
                    if (task->side != 0 || FIGHTSTG_battle.state.kind != BATTLE_KIND_FINAL_LAST) {
                        task->state = 3;
                    } else {
                        task->setSubstate(task, 6);
                    }
                } else {
                    children[0].events = FIGHTSTG_startActionEvents(task->side);
                    task->nextSubstate(task);
                }
            }
            break;
        case 3:
            if (children[0].task == NULL) {
                task->nextSubstate(task);
            }
            break;
        case 4:
            index = task->side == 0;
            countered = (FIGHTSTG_battle.state.fighters[index] + FIGHTSTG_battle.state.active[index]);
            if (countered->flags & FIGHTER_ASLEEP) {
                if (task->asleep != 0 && FIGHTSTG_battleFuncs.testWakeUp(SIDE_ENEMY - task->side, task->damage) != 0) {
                    wakeCounteredUp(task, children, countered, index);
                    task->substate = 8;
                } else {
                    task->state = 3;
                }
            } else {
                children[0].counter = FIGHTSTG_startCounterattack(index << 4, task->damage, 0);
                task->substate++;
            }
            break;
        case 5:
            if (children[0].task != NULL) {
                if (children[0].task->state != 2) {
                    break;
                }
                WFIGHTMN_chargeGauge(task->side, task->damage);
            }
            task->state = 3;
            break;
        case 6:
            if (children[0].task == NULL) {
                children[0].enemyAttack = FIGHTSTG_startEnemyAttack(1, 0);
                task->nextSubstate(task);
            }
            break;
        case 7:
            if (children[0].task == NULL) {
                task->setState(task, 3);
            }
            break;
        case 8:
            if (children[0].task == NULL) {
                children[0].counter = FIGHTSTG_startCounterattack((task->side == 0) << 4, task->damage, 0);
                task->substate = 5;
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts side's first technique (FIGHTSTG_updateFirstTech) */
FirstTech *FIGHTSTG_startFirstTech(s32 side) {
    FirstTech *task = createTask(FIGHTSTG_updateFirstTech, sizeof(FirstTech), sizeof(Task *));

    task->side = side;
    return task;
}
