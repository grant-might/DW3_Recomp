#include "engine/game.h"

/* Starts fading the screen to black (fadeIn 0) or back, over `duration` frames */
void screenFadeStart(ScreenFade *task, s32 fadeIn, s32 duration) {
    task->setState(task, TASK_RUN);
    task->substate = 1;
    task->fadeIn = fadeIn;
    if (fadeIn == 0) {
        task->level = 0;
        task->levelStep = FADE_LEVEL_MAX / duration;
    } else {
        task->level = FADE_LEVEL_MAX;
        task->levelStep = -(FADE_LEVEL_MAX / duration);
    }
}

/* The fade's primitives, one after the other in the primitive buffer */
typedef struct FadePrims {
    POLY_F4 poly;
    DR_TPAGE mode;
} FadePrims;

/* A full-screen POLY_F4 with subtractive blending (tpage 0xE1000245) */
void drawScreenFade(ScreenFade *task) {
    Layer *layer = GFX.funcs.getLayer(task->layerId);
    u_long *ot = layer->getOtEntry(layer, task->depth);
    FadePrims *prims = GFX.funcs.getPrim();
    POLY_F4 *poly = &prims->poly;
    DR_TPAGE *mode = &prims->mode;

    setlen(poly, 5);
    poly->code = 0x2A;
    poly->r0 = poly->g0 = poly->b0 = task->level >> 8;
    poly->x0 = poly->x2 = 0;
    poly->x1 = poly->x3 = SCREEN_WIDTH;
    poly->y0 = poly->y1 = 0;
    poly->y2 = poly->y3 = 256;
    addPrim(ot, poly);
    setlen(mode, 1);
    mode->code[0] = 0xE1000245;
    addPrim(ot, mode);
    GFX.funcs.setPrim(prims + 1);
}

/* Moves the fade one frame on and draws it; TASK_DONE at the end, still drawn */
void updateScreenFade(ScreenFade *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        if (task->substate == 0) {
            break;
        }
        task->level += task->levelStep;
        if (task->fadeIn == 0) {
            if (task->level > FADE_LEVEL_MAX) {
                task->level = FADE_LEVEL_MAX;
                task->state = TASK_DONE;
            }
        } else if (task->level < 0) {
            task->level = 0;
            task->state = TASK_DONE;
        }
        /* fallthrough */
    case TASK_DONE:
        drawScreenFade(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* A screen fade task on a layer, idle until started */
ScreenFade *createScreenFade(s32 layerId) {
    ScreenFade *task = createTask(updateScreenFade, sizeof(ScreenFade), 0);

    task->start = screenFadeStart;
    task->layerId = layerId;
    task->depth = 0;
    return task;
}
