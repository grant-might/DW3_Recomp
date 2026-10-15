/* FIGHTSTG's player turn: the command menu and the menus it opens, until the
   turn's action is picked. */

#include "battle/battle_mode.h"

/* Opens the command menu with the HUD, the partner view and the shot camera,
   then goes to the menu of the command picked */
static inline void runCommandMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        if (children[0].task == NULL) {
            children[0].hud = FIGHTSTG_createHud();
        }
        if (children[1].task == NULL) {
            children[1].commandMenu = FIGHTSTG_createCommandMenu(task->command, &task->result);
        }
        if (children[2].task == NULL) {
            children[2].partnerView = FIGHTSTG_createPartnerView();
        }
        if (children[6].task == NULL) {
            children[6].shotCamera = FIGHTSTG_createShotCamera();
        }
        task->result = -1;
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        if (task->result != -1) {
            task->command = task->result;
            switch (task->result) {
            case BATTLE_COMMAND_ATTACK:
                task->action = 0;
                task->setSubstate(task, 0);
                break;
            case BATTLE_COMMAND_TECH:
                task->setStep(task, 3);
                break;
            case BATTLE_COMMAND_DIGIVOLVE:
                task->setStep(task, 1);
                break;
            case BATTLE_COMMAND_SWITCH:
                task->setStep(task, 4);
                task->switchLine = 0;
                break;
            case BATTLE_COMMAND_ITEM:
                task->setStep(task, 2);
                break;
            case BATTLE_COMMAND_RUN:
                task->action = 1;
                task->setSubstate(task, 0);
                break;
            }
        }
        break;
    }
}

/* Opens the digivolve menu for the active partner and, once a Digimon is
   picked, makes digivolving the action */
static inline void runDigivolveMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        task->result = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
        children[3].digivolveMenu = FIGHTSTG_createDigivolveMenu(&task->result);
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setSubstate(task, 2);
            break;
        default:
            task->action = 2;
            task->arg = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
            task->digimon = task->result;
            task->setSubstate(task, 0);
            break;
        }
        break;
    }
}

/* Opens the item menu and, once an item is picked, makes using it the
   action */
static inline void runItemMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        children[3].itemMenu = FIGHTSTG_createItemMenu(&task->result);
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setSubstate(task, 2);
            break;
        default:
            task->action = 3;
            task->arg = task->result;
            task->setSubstate(task, 0);
            break;
        }
        break;
    }
}

/* Opens the technique menu and, once a technique is picked, makes using it
   the action */
static inline void runTechMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        children[3].techMenu = FIGHTSTG_createTechMenu(&task->result);
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setSubstate(task, 2);
            break;
        default:
            task->action = 4;
            task->arg = task->result;
            task->setSubstate(task, 0);
            break;
        }
        break;
    }
}

/* Opens the switch menu and then the switch-in menu with the pair technique
   for the Digimon picked, and makes the switch the action */
static inline void runSwitchMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        children[3].switchMenu = FIGHTSTG_createSwitchMenu(&task->result, &task->switchLine, 1);
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setSubstate(task, 2);
            break;
        default:
            task->arg = task->result;
            children[4].switchInMenu = FIGHTSTG_createPairSwitchMenu(&task->result, &task->pairTech);
            task->tickCounter(task);
            break;
        }
        break;
    case 2:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setCounter(task, 0);
            break;
        default:
            if ((task->result & 0xF) == 0) {
                task->action = 5;
                task->digimon = task->result >> 4;
                task->setSubstate(task, 0);
                task->switchLine = 0;
            } else {
                task->action = 6;
                task->digimon = task->result >> 4;
                task->setSubstate(task, 0);
                task->switchLine = 0;
            }
            break;
        }
        break;
    }
}

/* Opens the confused menu with the partner view and the shot camera, and
   makes its pick the action */
static inline void runConfusedMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        if (children[2].task == NULL) {
            children[2].partnerView = FIGHTSTG_createPartnerView();
        }
        if (children[6].task == NULL) {
            children[6].shotCamera = FIGHTSTG_createShotCamera();
        }
        if (children[1].task == NULL) {
            children[1].confusedMenu = FIGHTSTG_createConfusedMenu(&task->result, children[2].task, children[6].task);
        }
        task->result = -1;
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        switch (task->result) {
        case -1:
            break;
        case 0:
            task->action = 0;
            task->setSubstate(task, 0);
            break;
        default:
            task->action = -1;
            task->setSubstate(task, 0);
            break;
        }
        break;
    }
}

/* Opens the switch menu after a knockout and then the switch-in menu, and
   makes the switch the action */
static inline void runKnockOutSwitchMenu(PlayerTurn *task, PlayerTurnChild *children) {
    switch (task->counter) {
    case 0:
    default:
        if (children[6].task == NULL) {
            children[6].shotCamera = FIGHTSTG_createShotCamera();
        }
        if (children[3].task == NULL) {
            children[3].switchMenu = FIGHTSTG_createSwitchMenu(&task->result, &task->switchLine, 0);
        }
        task->tickCounter(task);
        /* fallthrough */
    case 1:
        if (task->result != -1) {
            task->arg = task->result;
            children[4].switchInMenu = FIGHTSTG_createSwitchInMenu(&task->result);
            task->tickCounter(task);
        }
        break;
    case 2:
        switch (task->result) {
        case -1:
            break;
        case -2:
            task->setCounter(task, 0);
            break;
        default:
            if ((task->result & 0xF) == 0) {
                task->action = 5;
                task->digimon = task->result >> 4;
                task->setSubstate(task, 0);
                task->switchLine = 0;
            }
            break;
        }
        break;
    }
}

/* The player's turn: substate 2 opens the command menu (runCommandMenu)
   and then the menu of its command (runDigivolveMenu, runItemMenu,
   runTechMenu, runSwitchMenu), 3 the confused menu (runConfusedMenu) and 4
   the switch menu after a knockout (runKnockOutSwitchMenu); each sets action
   and goes back to substate 0, which closes the children. The match depends on the menus'
   results being switches with case -1 first and on each of the two endings
   of the command menu's last step being written out. */
void FIGHTSTG_updatePlayerTurn(PlayerTurn *task, PlayerTurnChild *children) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            switch (task->step) {
            case 0:
            default:
                task->command = BATTLE_COMMAND_ATTACK;
                for (i = 1; i < 7; i++) {
                    if (children[i].task != NULL) {
                        children[i].task->setState(children[i].task, 3);
                    }
                }
                task->nextStep(task);
                break;
            case 1:
                break;
            }
            break;
        case 1:
            if (children[0].task == NULL) {
                children[0].hud = FIGHTSTG_createHud();
            }
            task->setSubstate(task, 0);
            break;
        case 2:
            switch (task->step) {
            case 0:
            default:
                runCommandMenu(task, children);
                break;
            case 1:
                runDigivolveMenu(task, children);
                break;
            case 2:
                runItemMenu(task, children);
                break;
            case 3:
                runTechMenu(task, children);
                break;
            case 4:
                runSwitchMenu(task, children);
                break;
            }
            break;
        case 3:
            runConfusedMenu(task, children);
            break;
        case 4:
            runKnockOutSwitchMenu(task, children);
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts the player's turn (FIGHTSTG_updatePlayerTurn, id 0xE) */
PlayerTurn *FIGHTSTG_startPlayerTurn(void) {
    return createTaskWithId(FIGHTSTG_updatePlayerTurn, sizeof(PlayerTurn), 7 * sizeof(Task *), BATTLE_TASK_PLAYER_TURN);
}

/* Sets the player's turn's substate, while it runs (4 picks the next fighter) */
void FIGHTSTG_setPlayerTurnStep(s32 substate) {
    Task *task = TASK_REGISTRY.funcs.find(BATTLE_TASK_PLAYER_TURN, -1, -1);

    if (task != NULL && task->state == TASK_RUN) {
        task->setSubstate(task, substate);
    }
}

/* Whether the player's turn has a menu open (its substate isn't 0) */
s32 FIGHTSTG_isPlayerChoosing(void) {
    PlayerTurn *turn = TASK_REGISTRY.funcs.find(BATTLE_TASK_PLAYER_TURN, -1, -1);

    return turn->substate != 0;
}
