/* FIGHTSTG's turn of BATTLE_KIND_FINAL_SECOND's enemy, which leaves the
   player's fighter at 1 HP. */

#include "battle/battle_mode.h"

/* BATTLE_KIND_FINAL_SECOND's enemy turn: the enemy's message and its battle
   table entry's second technique, then the player's fighter is left at 1 HP
   with a message of the damage, and the battle goes on */
void FIGHTSTG_updateOneHpTurn(OneHpTurn *task, BattleChild *children) {
    BattleTableEntry *entry;
    BattleFighter *fighter;

    switch (task->state) {
    case TASK_INIT:
    default:
        entry = FIGHTSTG_battleTableFunc(FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]].id);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x10;
        task->lines[1] = entry->techs[1];
        children[0].message->show(children[0].message, 3, task->lines);
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            if (children[0].task == NULL) {
                children[0].script = WFIGHTMN_startTech(0x10, task->lines[1]);
                task->substate++;
            }
            break;
        case 1:
            if (children[0].task == NULL) {
                fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
                children[0].message = FIGHTSTG_createMessage();
                task->lines[0] = 0;
                task->lines[1] = fighter->hp - 1;
                children[0].message->show(children[0].message, 4, task->lines);
                fighter->hp = 1;
                task->substate++;
            }
            break;
        case 2:
            if (children[0].task == NULL) {
                FIGHTSTG_queueEnemyTurn(FIGHTSTG_events.funcs.getDelay(SIDE_ENEMY, 0));
                FIGHTSTG_queueLastEnemy();
                task->state = TASK_KILL;
            }
            break;
        }
        break;
    case 2:
    case TASK_KILL:
        break;
    }
}

/* Starts BATTLE_KIND_FINAL_SECOND's enemy turn */
OneHpTurn *FIGHTSTG_startOneHpTurn(void) {
    return createTask(FIGHTSTG_updateOneHpTurn, sizeof(OneHpTurn), sizeof(Task *));
}
