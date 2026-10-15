/* STGTRAIN's scene: the mode's root task */

#include "menus/gym.h"

/* The mode's root task: sets up the display and starts the screen */
void STGTRAIN_updateRoot(Task *task, Task **children) {
    RECT rect;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        GFX.funcs.reset();
        GFX.funcs.allocPrimBuffers(0x14000);
        GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xF0;
        layer = GFX.funcs.createLayer(&rect, 3, SCREEN_LAYER);
        layer->setBgColor(layer, 0, 0, 0);
        rect.x = 0x7B;
        rect.y = 0x53;
        rect.w = 0xA8;
        rect.h = 0x58;
        GFX.funcs.createLayer(&rect, 3, 0x1001);
        GFX.funcs.moveLayer(0x1001, 0x1000, 1);
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xF0;
        GFX.funcs.createLayer(&rect, 3, 0x1002);
        GFX.funcs.moveLayer(0x1002, 0x1001, 1);
        children[0] = (Task *)STGTRAIN_createScreen();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The mode's entry point (MODE_ENTRY_POINTS): starts the root task */
Task *STGTRAIN_start(void) {
    return createTask(STGTRAIN_updateRoot, sizeof(Task), 4);
}
