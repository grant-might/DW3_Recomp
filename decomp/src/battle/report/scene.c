/* The fight report's mode: its scene, its entry point and the screen fader
   (src/shared). */

#include "battle/report.h"

/* The mode's scene: resets the graphics, makes the screen layer and creates
   the report */
void STFGTREP_updateScene(Task *task, FightReport **children) {
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
        children[0] = STFGTREP_createScreen();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The mode's entry point (MODE_ENTRY_POINTS) */
Task *STFGTREP_start(void) {
    return createTask(STFGTREP_updateScene, sizeof(Task), 4);
}

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 6
#include "shared/create_fader.inc.c"
