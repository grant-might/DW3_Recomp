/* FIGHTSTG's enemy turn: the enemy's pick of its action from its battle
   table entry, and its conditions. */

#include "battle/battle_mode.h"

/* The enemy that the enemy's turn (FIGHTSTG_updateEnemyTurn) switches to:
   task->target's, or one of the others at random (not the active one), -1
   for none */
s32 FIGHTSTG_pickEnemySwitch(EnemyTurn *task) {
    BattleFighter *enemies = FIGHTSTG_battle.state.fighters[1];
    s32 found[2];
    s32 count;
    s32 active;
    s32 i;

    switch (task->target) {
    case -2:
        if (enemies[0].id == enemies[FIGHTSTG_battle.state.active[1]].id) {
            break;
        }
        if (enemies[0].hp != 0) {
            return 0;
        }
        break;
    case -3:
        if (enemies[1].id == enemies[FIGHTSTG_battle.state.active[1]].id) {
            break;
        }
        if (enemies[1].hp != 0) {
            return 1;
        }
        break;
    case -4:
        if (enemies[2].id == enemies[FIGHTSTG_battle.state.active[1]].id) {
            break;
        }
        if (enemies[2].hp != 0) {
            return 2;
        }
        break;
    default:
        count = 0;
        found[0] = -1;
        found[1] = -1;
        active = FIGHTSTG_battle.state.active[1];
        for (i = 0; i < 3; i++) {
            enemies = &FIGHTSTG_battle.state.fighters[1][i];
            if (active != i && enemies->id != 0 && enemies->hp != 0) {
                found[count++] = i;
            }
        }
        /* the match depends on case 0 and on reusing enemies */
        switch (count) {
        case 0:
            break;
        case 1:
            return found[0];
        case 2:
            return found[RANDOM.next() & 1];
        }
        break;
    }
    return -1;
}

/* In BATTLE_KIND_ESCAPE, the enemy flees once its HP is under a tenth;
   otherwise its turn goes on to the next state. */
static inline void fleeWhenWeak(EnemyTurn *task, BattleChild *children) {
    if (FIGHTSTG_battle.state.kind == BATTLE_KIND_ESCAPE) {
        BattleFighter *enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];

        if (enemy->hp < (s16)(enemy->maxHp / 10)) {
            children->message = FIGHTSTG_createMessage();
            task->lines[0] = 0x5F;
            task->lines[1] = 0x10;
            children->message->show(children->message, 2, task->lines);
            FIGHTSTG_endBattle(BATTLE_FLED);
            task->substate++;
        } else {
            task->nextState(task);
        }
    } else {
        task->nextState(task);
    }
}

/* Shows the enemy's run-away event, if it has one, and clears it before its
   turn goes on. */
static inline void showRunAway(EnemyTurn *task, BattleChild *children) {
    s32 index;

    switch (task->step) {
    case 0:
    default:
        index = FIGHTSTG_events.funcs.find(EVENT_RUN_AWAY, 0x10, FIGHTSTG_battle.state.active[1]);
        if (index >= 0) {
            children->message = FIGHTSTG_createMessage();
            task->lines[0] = 0x60;
            task->lines[1] = 0x10;
            children->message->show(children->message, 2, task->lines);
            FIGHTSTG_events.events[index].type = 0;
            task->step++;
        } else {
            task->nextSubstate(task);
        }
        break;
    case 1:
        if (children->message == NULL) {
            task->nextSubstate(task);
        }
        break;
    }
}

/* Starts the enemy's attack (target 1) or technique TARGET, paying its MP,
   or shows the message for an enemy without the MP for it. */
static inline void useEnemyTech(EnemyTurn *task, BattleChild *children) {
    BattleFighter *fighter;
    TechData *tech;

    if (task->target == 1) {
        children->attack = FIGHTSTG_startFirstTech(0x10);
    } else {
        fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        tech = &TECHS[task->target - 1];
        if (fighter->mp >= tech->mp) {
            children->tech = FIGHTSTG_startTechAction(SIDE_ENEMY, task->target);
            fighter->mp -= tech->mp;
        } else {
            children->message = FIGHTSTG_createMessage();
            task->lines[0] = 0x8D;
            task->lines[1] = 0x10;
            children->message->show(children->message, 2, task->lines);
        }
    }
}

/* Says the name of the enemy that came in. */
static inline void showEnemyName(EnemyTurn *task, BattleChild *children) {
    BattleFighter *enemies = FIGHTSTG_battle.state.fighters[1];
    BattleTableEntry *enemy = FIGHTSTG_battleTableFunc(enemies[FIGHTSTG_battle.state.active[1]].id);

    children->message = FIGHTSTG_createMessage();
    task->lines[0] = enemy->nameId;
    children->message->show(children->message, 0xD, task->lines);
}

/* Picks the enemy to switch in (FIGHTSTG_pickEnemySwitch) and shows the
   switch's message, or the one for no enemy to switch in. */
static inline void switchEnemyOut(EnemyTurn *task, BattleChild *children) {
    s32 pick = FIGHTSTG_pickEnemySwitch(task);

    if (pick != -1) {
        task->switchTo = pick;
        children->message = FIGHTSTG_createMessage();
        task->lines[0] = 0x4D;
        children->message->show(children->message, 1, task->lines);
        task->substate = 5;
    } else {
        children->message = FIGHTSTG_createMessage();
        task->lines[0] = 0x8E;
        task->lines[1] = 0x10;
        children->message->show(children->message, 2, task->lines);
        task->substate = 3;
    }
}

/* The enemy's turn: a message when its HP is under a tenth, then one when it is asleep,
   paralyzed or confused, or else the first action of its battle table entry whose condition holds,
   and what its target makes it do: attack, a technique, a switch to another enemy
   (FIGHTSTG_pickEnemySwitch) or a message. The match depends on the goto into the confusion
   branch, the case -1 next to default, and the enemies pointers of showEnemyName and
   substate 5. */
void FIGHTSTG_updateEnemyTurn(EnemyTurn *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleTableEntry *entry;
    s32 message;
    s32 i;

    switch (task->state) {
    case 0:
    default:
        switch (task->substate) {
        case 0:
        default:
            fleeWhenWeak(task, children);
            break;
        case 1:
            if (children->message == NULL) {
                task->state = 3;
            }
            break;
        }
        break;
    case 1:
        switch (task->substate) {
        case 0:
        default:
            showRunAway(task, children);
            break;
        case 1:
            fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
            if (fighter->flags & FIGHTER_ASLEEP) {
                children->message = FIGHTSTG_createMessage();
                message = 0x90;
                goto show;
            }
            if ((fighter->flags & FIGHTER_PARALYZED) && FIGHTSTG_battleFuncs.testParalysis(SIDE_ENEMY) != 0) {
                children->message = FIGHTSTG_createMessage();
                message = 0x56;
                goto show;
            }
            if (fighter->flags & FIGHTER_CONFUSED) {
                children->message = FIGHTSTG_createMessage();
                message = RANDOM.next() % 8 + 0x83;
            show:
                task->lines[0] = message;
                task->lines[1] = 0x10;
                children->message->show(children->message, 2, task->lines);
                task->setSubstate(task, 3);
                break;
            }
            i = 0;
            entry = FIGHTSTG_battleTableFunc(fighter->id);
            for (; i < BATTLE_TABLE_FALLBACK; i++) {
                if (FIGHTSTG_testEnemyCondition(entry->actions[i].condition, entry->actions[i].conditionArg) != 0) {
                    break;
                }
            }
            task->target = FIGHTSTG_getEnemyAction(entry->actions[i].target);
            task->substate++;
            break;
        case 2:
            if (task->target > 0) {
                useEnemyTech(task, children);
                task->substate = 3;
            } else if (task->target < 0) {
                switch (task->target) {
                case -1:
                default:
                    FIGHTSTG_queueRunAway(0x10);
                    children->message = FIGHTSTG_createMessage();
                    task->lines[0] = 0x5E;
                    task->lines[1] = 0x10;
                    children->message->show(children->message, 2, task->lines);
                    task->substate = 3;
                    break;
                case -2:
                case -3:
                case -4:
                case -5:
                    switchEnemyOut(task, children);
                    break;
                }
            }
            break;
        case 3:
            if (children->message == NULL) {
                task->state = 3;
                FIGHTSTG_queueEnemyTurn(FIGHTSTG_events.funcs.getDelay(SIDE_ENEMY, 0));
            }
            break;
        case 4:
            if (children->message == NULL) {
                showEnemyName(task, children);
                task->substate = 3;
            }
            break;
        case 5:
            if (children->message == NULL) {
                BattleFighter *enemies = FIGHTSTG_battle.state.fighters[1];

                children->entrance = FIGHTSTG_startEntrance(enemies[task->switchTo].id, 1, 0);
                WFIGHTMN_setIdleMotion(SIDE_ENEMY, 0);
                task->substate = 6;
            }
            break;
        case 6:
            if (children->entrance->done) {
                FIGHTSTG_battle.state.active[1] = task->switchTo;
                task->substate = 4;
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts the enemy's turn (FIGHTSTG_updateEnemyTurn) */
EnemyTurn *FIGHTSTG_startEnemyTurn(void) {
    return createTask(FIGHTSTG_updateEnemyTurn, sizeof(EnemyTurn), 2 * sizeof(Task *));
}

/* Whether the condition of an enemy's action holds (BattleTableAction): always, a
   chance in 128, the enemy's or the partner's HP over or under a share, its MP, the
   partner's Digimon or flags, the other enemies, the battle and the enemy's boosts.
   The match depends on the one fighter pointer that the cases set as they need it,
   on case 1 keeping its roll in the same variable as the HP share, and on case 10's
   own loop counter. */
s32 FIGHTSTG_testEnemyCondition(u8 condition, s16 arg) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
    s32 result = 0;
    s32 percent;
    s32 j;
    DigimonData *digimon;
    s32 i;

    switch (condition) {
    case 0:
    default:
        result = 1;
        break;
    case 1:
        percent = RANDOM.next() % 128;
        if (percent < arg) {
            result = 1;
        }
        break;
    case 2:
        percent = arg * 100 / 128;
        if (fighter->hp < (s16)(fighter->maxHp / 100) * percent) {
            result = 1;
        }
        break;
    case 3:
        percent = arg * 100 / 128;
        if (fighter->hp >= (s16)(fighter->maxHp / 100) * percent) {
            result = 1;
        }
        break;
    case 4:
        if (fighter->mp < arg) {
            result = 1;
        }
        break;
    case 5:
        if (fighter->mp >= arg) {
            result = 1;
        }
        break;
    case 6:
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        percent = arg * 100 / 128;
        if (fighter->hp < (s16)(fighter->maxHp / 100) * percent) {
            result = 1;
        }
        break;
    case 7:
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        percent = arg * 100 / 128;
        if (fighter->hp >= (s16)(fighter->maxHp / 100) * percent) {
            result = 1;
        }
        break;
    case 8:
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        for (i = 0, digimon = DIGIMON_DATA; i < 8; i++, digimon++) {
            if (fighter->id == digimon->id) {
                result = 1;
                break;
            }
        }
        break;
    case 9:
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        if (fighter->flags & FIGHTER_ASLEEP) {
            result = 1;
        }
        break;
    case 10:
        fighter = FIGHTSTG_battle.state.fighters[1];
        if (arg == 0) {
            for (j = 0; j < 3; j++) {
                if (j != FIGHTSTG_battle.state.active[1] && fighter[j].id != 0 && fighter[j].hp != 0) {
                    result = 1;
                    break;
                }
            }
        } else {
            for (j = 0; j < 3; j++) {
                if (j != FIGHTSTG_battle.state.active[1] && fighter[j].id == arg && fighter[j].hp != 0) {
                    result = 1;
                    break;
                }
            }
        }
        break;
    case 11:
        if (FIGHTSTG_battle.state.boost.element == arg) {
            result = 1;
        }
        break;
    case 12:
        if (BATTLE_SETUP.battle == arg) {
            result = 1;
        }
        break;
    case 13:
        if (BATTLE_SETUP.encounterKind == arg) {
            result = 1;
        }
        break;
    case 14:
        if (fighter->charge != 0) {
            result = 1;
        }
        break;
    case 15:
        if (fighter->boosts[0] < 0) {
            result = 1;
        }
        break;
    case 16:
        if (fighter->boosts[1] < 0) {
            result = 1;
        }
        break;
    case 17:
        if (fighter->boosts[2] < 0) {
            result = 1;
        }
        break;
    case 18:
        if (fighter->turns % arg == 0) {
            result = 1;
        }
        break;
    }
    return result;
}

/* What an enemy's action target makes it do (EnemyTurn.target): 1 its first
   technique, its battle table entry's second or third technique, or -1 to -5
   for the other actions */
s32 FIGHTSTG_getEnemyAction(u8 kind) {
    BattleFighter *enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
    s32 value = 0;
    BattleTableEntry *entry = FIGHTSTG_battleTableFunc(enemy->id);

    switch (kind) {
    case 1:
        value = 1;
        break;
    case 2:
        value = entry->techs[1];
        break;
    case 3:
        value = entry->techs[2];
        break;
    case 4:
        value = -1;
        break;
    case 5:
        value = -2;
        break;
    case 6:
        value = -3;
        break;
    case 7:
        value = -4;
        break;
    case 8:
        value = -5;
        break;
    }
    return value;
}
