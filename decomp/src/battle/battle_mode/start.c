/* FIGHTSTG's root task, which the mode starts (FIGHTSTG_start). */

#include "battle/battle_mode.h"

/* The mode's root task: loads WFIGHTTS (the battle test, with the mode's
   argument) or WFIGHTMN and starts it, then runs the battle's update */
void FIGHTSTG_updateRoot(Task *task, Task **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.funcs.getModeArg()) {
            OVERLAY_LOADER.loadSubOverlay(FILE_WFIGHTTS);
            BATTLE_SETUP.stage = FIGHTSTG_randomStage();
            children[0] = WFIGHTTS_start();
        } else {
            OVERLAY_LOADER.loadSubOverlay(FILE_WFIGHTMN);
            children[0] = WFIGHTMN_start();
        }
        FIGHTSTG_battle.setSpeed(0);
        task->nextState(task);
        break;
    case TASK_RUN:
        FIGHTSTG_battle.countFrames();
        break;
    case 2:
    case TASK_KILL:
        break;
    }
}

/* The mode's entry point (MODE_ENTRY_POINTS): starts the root task */
Task *FIGHTSTG_start(void) {
    return createTask(FIGHTSTG_updateRoot, sizeof(Task), sizeof(Task *));
}
