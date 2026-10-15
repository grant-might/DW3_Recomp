/* FIELDSTG's entry point, which the executable starts the field mode with */

#include "field/field_mode.h"

/* FIELDSTG's root task: notes whether the mode is new (GAME.clearTempFlags)
   and creates the field */
void FIELDSTG_updateRoot(Task *task, Task **children) {
    s32 mode;

    switch (task->state) {
        default:
        case TASK_INIT:
            mode = GAME.funcs.getMode();
            if (GAME.lastFieldMode != mode) {
                GAME.lastFieldMode = mode;
                GAME.clearTempFlags = 1;
#if VERSION_EU || VERSION_JP
                GAME.randomGauges = 0x10;
#endif
            } else {
                GAME.clearTempFlags = 0;
            }
            children[0] = FIELDSTG_createField();
            task->nextState(task);
            break;
        case TASK_RUN:
        case TASK_DONE:
        case TASK_KILL:
            break;
    }
}

/* FIELDSTG's entry point: creates its root task */
Task *FIELDSTG_start(void) {
    return createTask(FIELDSTG_updateRoot, sizeof(Task), 0xC);
}
