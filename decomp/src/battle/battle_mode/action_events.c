/* FIGHTSTG's action events: the task that runs the events an action queued. */

#include "battle/battle_mode.h"

/* Takes the enemy's item into the bag (up to 99) and says so. */
static inline void stealEnemyItem(ActionEvents *task, BattleChild *children) {
    BattleFighter *enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];

    task->args[0] = enemy->item;
    children[0].message->show(children[0].message, 0x11, task->args);
    GAME.items[enemy->item]++;
    if (GAME.items[enemy->item] >= 100) {
        GAME.items[enemy->item] = 99;
    }
    enemy->item = -1;
}

/* Heals FIGHTER by the drained HP (up to its max HP) and says so. */
static inline void drainHp(ActionEvents *task, BattleChild *children, BattleFighter *fighter) {
    task->args[0] = task->side;
    if (fighter->maxHp < fighter->hp + FIGHTSTG_action.drain) {
        task->args[1] = fighter->maxHp - fighter->hp;
    } else {
        task->args[1] = FIGHTSTG_action.drain;
    }
    task->args[2] = 0;
    children[0].message->show(children[0].message, 0x12, task->args);
    fighter->hp += task->args[1];
}

/* Moves the drained MP (as much as the partner has) from the partner to the
   enemy, up to its max MP, and says so. */
static inline void drainMp(ActionEvents *task, BattleChild *children) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    BattleFighter *enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];

    task->args[0] = 0x10;
    if (fighter->mp < FIGHTSTG_action.drain) {
        task->args[1] = fighter->mp;
    } else {
        task->args[1] = FIGHTSTG_action.drain;
    }
    task->args[2] = 1;
    children[0].message->show(children[0].message, 0x12, task->args);
    enemy->mp += task->args[1];
    if (enemy->maxMp < enemy->mp) {
        enemy->mp = enemy->maxMp;
    }
    fighter->mp -= task->args[1];
}

/* Says which status (bit 0, 1 or 2 of the event's flags) the technique
   raises. */
static inline void showRaisedStatus(ActionEvents *task, BattleChild *children) {
    task->args[0] = SIDE_ENEMY - task->side;
    if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_ONE_STATUS] & 1) {
        task->args[1] = 0;
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_ONE_STATUS] & 2) {
        task->args[1] = 1;
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_ONE_STATUS] & 4) {
        task->args[1] = 2;
    }
    children[0].message->show(children[0].message, 0x13, task->args);
}

/* Says the next status the technique raises and clears its bit, so the event
   runs again for the remaining ones. */
static inline void showNextRaisedStatus(ActionEvents *task, BattleChild *children) {
    task->args[0] = SIDE_ENEMY - task->side;
    if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_EACH_STATUS] & 1) {
        task->args[1] = 0;
        FIGHTSTG_action.effects[task->step] &= ~1;
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_EACH_STATUS] & 2) {
        task->args[1] = 1;
        FIGHTSTG_action.effects[task->step] &= ~2;
    } else if (FIGHTSTG_action.effects[TECH_EFFECT_RAISE_EACH_STATUS] & 4) {
        task->args[1] = 2;
        FIGHTSTG_action.effects[task->step] &= ~4;
    }
    children[0].message->show(children[0].message, 0x13, task->args);
}

/* Applies event I's status or battle effect (a poison, a lowered stat, an
   escape...) and shows its message. */
static inline void applyStatusEvent(ActionEvents *task, BattleChild *children, s32 i) {
    s32 other = task->side != 0;

    switch (i) {
    case TECH_EFFECT_POISON:
    default:
        FIGHTSTG_inflictPoison(SIDE_ENEMY - task->side, FIGHTSTG_battle.state.active[1 - other], FIGHTSTG_action.effects[i]);
        task->args[0] = 0x1E;
        break;
    case TECH_EFFECT_PARALYSIS:
        FIGHTSTG_inflictParalysis(SIDE_ENEMY - task->side, FIGHTSTG_action.tech, FIGHTSTG_action.effects[i]);
        task->args[0] = 0x1F;
        break;
    case TECH_EFFECT_CONFUSION:
        FIGHTSTG_inflictConfusion(SIDE_ENEMY - task->side, FIGHTSTG_action.tech, FIGHTSTG_action.effects[i]);
        task->args[0] = 0x20;
        break;
    case TECH_EFFECT_SLEEP:
        FIGHTSTG_inflictSleep(SIDE_ENEMY - task->side, FIGHTSTG_action.tech, FIGHTSTG_action.effects[i]);
        task->args[0] = 0x21;
        break;
    case TECH_EFFECT_NO_SWITCH:
        FIGHTSTG_startRestriction(0xD3);
        task->args[0] = 0x44;
        break;
    case TECH_EFFECT_LOWER_ATTACK:
        task->args[0] = 0x33;
        break;
    case TECH_EFFECT_LOWER_DEFENSE:
        task->args[0] = 0x34;
        break;
    case TECH_EFFECT_RAISE_ALL_STATUS:
        task->args[0] = 0x4C;
        break;
    case TECH_EFFECT_END_BATTLE:
        task->args[0] = 0x48;
        FIGHTSTG_endBattle(BATTLE_FLED);
        break;
    }
    task->args[1] = (task->side == 0) << 4;
    children[0].message->show(children[0].message, 2, task->args);
}

/* Runs the queued battle events: the first flag set in FIGHTSTG_action.effects
   picks the step, which shows its message, applies its effect and clears
   the flag (stealEnemyItem, drainHp, drainMp, showRaisedStatus,
   showNextRaisedStatus or applyStatusEvent). The match depends on each event
   branch ending with its own flag clear and `break` (the copies are merged
   after reload, but they make the loop big enough that loop.c leaves 0x10 at
   each use), on `case 2` written next to `default` in applyStatusEvent's
   switch (it sets the jump table's base) and on drainMp setting its fighter
   and enemy before args[0]. */
void FIGHTSTG_updateActionEvents(ActionEvents *task, BattleChild *children) {
    BattleFighter *fighter;
    u32 side;
    s32 i;
    switch (task->state) {
    case 0:
    default:
        FIGHTSTG_action.effects[TECH_EFFECT_MULTI_HIT] = 0;
        FIGHTSTG_action.effects[TECH_EFFECT_ENEMY_ONLY] = 0;
        FIGHTSTG_action.effects[TECH_EFFECT_CRITICAL] = 0;
        task->nextState(task);
        break;
    case 1:
        switch (task->substate) {
        case 0:
        default:
            for (i = 0; i < 0x25; i++) {
                if (FIGHTSTG_action.effects[i] != 0) {
                    task->step = i;
                    side = task->side >> 4;
                    task->substate++;
                    fighter = &FIGHTSTG_battle.state.fighters[1 - side][FIGHTSTG_battle.state.active[1 - side]];
                    if (i == TECH_EFFECT_SLEEP && (fighter->flags & FIGHTER_ASLEEP)) {
                        FIGHTSTG_inflictSleep(SIDE_ENEMY - task->side, FIGHTSTG_action.tech, FIGHTSTG_action.effects[TECH_EFFECT_SLEEP]);
                        FIGHTSTG_action.effects[task->step] = 0;
                        break;
                    } else {
                        children[0].message = FIGHTSTG_createMessage();
                        if (i == TECH_EFFECT_STEAL) {
                            stealEnemyItem(task, children);
                            FIGHTSTG_action.effects[task->step] = 0;
                            break;
                        } else if (i == TECH_EFFECT_DRAIN) {
                            fighter = &FIGHTSTG_battle.state.fighters[side][FIGHTSTG_battle.state.active[side]];
                            drainHp(task, children, fighter);
                            FIGHTSTG_action.effects[task->step] = 0;
                            break;
                        } else if (i == TECH_EFFECT_DRAIN_MP) {
                            drainMp(task, children);
                            FIGHTSTG_action.effects[task->step] = 0;
                            break;
                        } else if (i == TECH_EFFECT_RAISE_ONE_STATUS) {
                            showRaisedStatus(task, children);
                            FIGHTSTG_action.effects[task->step] = 0;
                            break;
                        } else if (i == TECH_EFFECT_RAISE_EACH_STATUS) {
                            showNextRaisedStatus(task, children);
                            break;
                        } else {
                            applyStatusEvent(task, children, i);
                        }
                    }
                    FIGHTSTG_action.effects[task->step] = 0;
                    break;
                }
            }
            if (task->step == 0) {
                task->state = 3;
            }
            break;
        case 1:
            if (children[0].task == NULL) {
                task->setSubstate(task, 0);
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts running the current action's events for SIDE
   (FIGHTSTG_updateActionEvents) */
ActionEvents *FIGHTSTG_startActionEvents(s32 side) {
    ActionEvents *task = createTask(FIGHTSTG_updateActionEvents, sizeof(ActionEvents), sizeof(Task *));

    task->side = side;
    return task;
}
