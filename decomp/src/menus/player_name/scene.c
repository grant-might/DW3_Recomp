/* STPLNMET's scene: the mode's root task */

#include "menus/player_name.h"

/* Puts the layer's origin at the center of the screen and gives it 5 callbacks */
void STPLNMET_centerLayer(Task *task, PlayerNameScreen **screen, Layer *layer, RECT *rect) {
    layer->setOffset(layer, rect->w / 2, rect->h / 2);
    layer->allocCallbacks(layer, 5);
}

/* The mode's root task: sets up the 320x240 display, a near-black layer and a
   second layer above it, then creates the name screen */
void STPLNMET_updateScene(Task *task, PlayerNameScreen **screen) {
    RECT rect;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        GFX.funcs.reset();
        GFX.funcs.allocPrimBuffers(0x19000);
        GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xF0;
        layer = GFX.funcs.createLayer(&rect, 1, SCREEN_LAYER);
        layer->setBgColor(layer, 1, 1, 1);
        STPLNMET_centerLayer(task, screen, layer, &rect);
        GFX.funcs.createLayer(&rect, 3, 0x1001);
        GFX.funcs.moveLayer(0x1001, 0x1000, 1);
        *screen = STPLNMET_createScreen();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The mode's entry point (MODE_ENTRY_POINTS): starts the root task */
Task *STPLNMET_start(void) {
    return createTask(STPLNMET_updateScene, sizeof(Task), sizeof(PlayerNameScreen *));
}
