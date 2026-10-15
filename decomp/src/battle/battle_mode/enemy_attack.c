/* FIGHTSTG's enemy attacks, which WFIGHTMN starts for the enemy's turns. */

#include "battle/battle_mode.h"

/* Queues the enemy's next turn and says the technique it uses (the weakened
   enemy's own one, or ends the attack while its weakness lasts), noting
   whether the player's fighter was asleep. */
static inline void announceEnemyTech(EnemyAttack *task, BattleChild *children, BattleTableEntry *entry) {
    FIGHTSTG_queueEnemyTurn(FIGHTSTG_events.funcs.getDelay(SIDE_ENEMY, 0));
    if (FIGHTSTG_battle.state.weakened != 0) {
        if (FIGHTSTG_events.funcs.first(EVENT_WEAKNESS_END) >= 0) {
            task->setState(task, 3);
        } else {
            children[0].message = FIGHTSTG_createMessage();
            task->lines[0] = 8;
            task->lines[1] = 0x10;
            task->tech = entry->techs[2];
            children[0].message->show(children[0].message, 2, task->lines);
            FIGHTSTG_battle.state.weakened = 0;
            FIGHTSTG_battle.state.hitCount = 0;
            task->resetIdleMotion = 1;
        }
    } else {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x10;
        task->lines[1] = FIGHTSTG_battle.state.tech != 0 ? FIGHTSTG_battle.state.tech : entry->techs[0];
        children[0].message->show(children[0].message, 3, task->lines);
        task->tech = entry->techs[0];
    }
    if ((FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0])->flags & FIGHTER_ASLEEP) {
        task->asleep = 1;
    }
}

/* Shows the technique's damage and takes it from the player's active
   fighter's HP (the European version's instant knockout takes it all). */
static inline void damagePlayer(EnemyAttack *task, BattleChild *children) {
    BattleFighter *player;

#if VERSION_EU || VERSION_JP
    player = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    /* the European version's instant knockout */
    if (FIGHTSTG_action.effects[TECH_EFFECT_KNOCK_OUT]) {
        player->hp = 0;
        children[0].task->state = 3;
    } else {
        task->lines[0] = 0;
        task->lines[1] = FIGHTSTG_action.damage;
        children[0].message->show(children[0].message, 4, task->lines);
        player->hp -= FIGHTSTG_action.damage;
        if (player->hp <= 0) {
            player->hp = 0;
        }
    }
#else
    task->lines[0] = 0;
    task->lines[1] = FIGHTSTG_action.damage;
    children[0].message->show(children[0].message, 4, task->lines);
    player = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    player->hp -= FIGHTSTG_action.damage;
    if (player->hp <= 0) {
        player->hp = 0;
    }
#endif
}

/* Wakes the player's sleeping FIGHTER up and says so, ending its sleep's
   event. */
static inline void wakePlayerUp(EnemyAttack *task, BattleChild *children, BattleFighter *fighter) {
    s32 index;

    task->lines[0] = 0x2B;
    task->lines[1] = 0;
    task->lines[2] = FIGHTSTG_battle.state.active[0];
    children[0].message = FIGHTSTG_createMessage();
    children[0].message->show(children[0].message, 7, task->lines);
    fighter->flags &= ~FIGHTER_ASLEEP;
    index = FIGHTSTG_events.funcs.find(EVENT_STATUS_END + 2, 0, FIGHTSTG_battle.state.active[0]);
    if (index >= 0) {
        FIGHTSTG_events.events[index].type = 0;
    }
}

/* The other attack's result: its damage (queueing the player's knockout at
   0 HP unless the attack was started without, else charging the enemy's
   gauge) or its miss message. */
static inline void showOtherAttackResult(EnemyAttack *task, BattleChild *children) {
    BattleFighter *struck;

    if (FIGHTSTG_action.hits[0]) {
        task->lines[0] = 0;
        task->lines[1] = FIGHTSTG_action.damage;
        children[0].message->show(children[0].message, 4, task->lines);
        struck = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        struck->hp -= FIGHTSTG_action.damage;
        if (struck->hp <= 0) {
            struck->hp = 0;
            if (task->noKnockout == 0) {
                FIGHTSTG_queueKnockOut(0);
            }
        } else {
            WFIGHTMN_chargeGauge(SIDE_ENEMY, FIGHTSTG_action.damage);
        }
    } else {
        task->lines[0] = 0x1D;
        task->lines[1] = 0;
        children[0].message->show(children[0].message, 2, task->lines);
    }
}

/* An attack on the player's active fighter (FIGHTSTG_startEnemyAttack): state 1 is the
   enemy's technique, 2 another attack, each with its messages, motion and
   damage. The match depends on each case advancing substate itself, on a
   fighter variable of its own in each case and on the pointer sum of
   announceEnemyTech's FIGHTER_ASLEEP test. */
void FIGHTSTG_updateEnemyAttack(EnemyAttack *task, BattleChild *children) {
    BattleTableEntry *entry = FIGHTSTG_battleTableFunc(FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]].id);
    BattleFighter *fighter;
    BattleFighter *target;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->setState(task, task->kind + 1);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            announceEnemyTech(task, children, entry);
            task->substate++;
            break;
        case 1:
            if (children[0].task == NULL) {
                FIGHTSTG_action.start(0x10, task->tech);
                children[0].script = WFIGHTMN_startTech(0x10, task->tech);
                if (task->resetIdleMotion != 0) {
                    WFIGHTMN_setIdleMotion(SIDE_ENEMY, 0);
                    task->resetIdleMotion = 0;
                }
                task->substate++;
            }
            break;
        case 2:
            if (children[0].task == NULL) {
                children[0].message = FIGHTSTG_createMessage();
                if (FIGHTSTG_action.hits[0]) {
                    damagePlayer(task, children);
                    task->substate++;
                } else {
                    task->lines[0] = 0x1D;
                    task->lines[1] = 0;
                    children[0].message->show(children[0].message, 2, task->lines);
                    task->substate = 5;
                }
            }
            break;
        case 3:
            if (children[0].task == NULL) {
                target = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
#if VERSION_EU || VERSION_JP
                if (FIGHTSTG_action.effects[TECH_EFFECT_KNOCK_OUT]) {
                    target->hp = 0;
                    FIGHTSTG_queueKnockOut(0);
                    task->state = 3;
                } else
#endif
                if (target->hp <= 0) {
                    target->hp = 0;
                    FIGHTSTG_queueKnockOut(0);
                    task->state = 3;
                } else {
                    children[0].events = FIGHTSTG_startActionEvents(0x10);
                    task->substate++;
                }
            }
            break;
        case 4:
            if (children[0].task == NULL) {
                fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
                if (fighter->flags & FIGHTER_ASLEEP) {
                    if (task->asleep != 0 && FIGHTSTG_battleFuncs.testWakeUp(0, FIGHTSTG_action.damage) != 0) {
                        wakePlayerUp(task, children, fighter);
                        task->substate = 5;
                    } else {
                        task->state = 3;
                    }
                } else {
                    task->substate = 5;
                }
            }
            break;
        case 5:
            if (children[0].task == NULL) {
                WFIGHTMN_chargeGauge(SIDE_ENEMY, FIGHTSTG_action.damage);
                task->state = 3;
            }
            break;
        }
        break;
    case 2:
        if (children[0].task == NULL) {
            switch (task->substate) {
            case 0:
            default:
                children[0].message = FIGHTSTG_createMessage();
                task->lines[0] = 0x10;
                children[0].message->show(children[0].message, 6, task->lines);
                task->substate++;
                break;
            case 1:
                FIGHTSTG_action.start(0x10, entry->techs[1]);
                children[0].script = WFIGHTMN_startTech(0x10, entry->techs[1]);
                task->substate++;
                break;
            case 2:
                children[0].message = FIGHTSTG_createMessage();
                showOtherAttackResult(task, children);
                task->substate++;
                break;
            case 3:
                FIGHTSTG_endEnemyWeakness();
                task->state = 3;
                break;
            }
        }
        break;
    case 3:
        break;
    }
}

/* Starts an attack on the player's active fighter (FIGHTSTG_updateEnemyAttack):
   kind 0 for the enemy's technique, 1 for another attack */
EnemyAttack *FIGHTSTG_startEnemyAttack(s32 kind, s32 noKnockout) {
    EnemyAttack *task = createTask(FIGHTSTG_updateEnemyAttack, sizeof(EnemyAttack), sizeof(Task *));

    task->kind = kind;
    task->noKnockout = noKnockout;
    return task;
}
