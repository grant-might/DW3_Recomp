/* FIGHTSTG's white flash over the screen. */

#include "battle/battle_mode.h"

/* Draws the white flash: a full-screen quad of the task's level, added to the
   screen on layer BATTLE_LAYER_FRONT */
void FIGHTSTG_drawWhiteFlash(WhiteFlash *task) {
    Layer *layer = GFX.funcs.getLayer(BATTLE_LAYER_FRONT);
    u_long *ot = layer->getOtEntry(layer, 0);
    POLY_F4 *poly = GFX.funcs.getPrim();
    DR_TPAGE *mode;

    poly->r0 = task->level;
    poly->g0 = task->level;
    poly->b0 = task->level;
    setlen(poly, 5);
    poly->code = 0x2A;
    /* the match depends on these chained stores, x3 before x1 */
    poly->x3 = poly->x1 = 320;
    poly->x2 = poly->x0 = 0;
    poly->y1 = poly->y0 = 0;
    poly->y3 = poly->y2 = 240;
    addPrim(ot, poly);
    mode = (DR_TPAGE *)(poly + 1);
    SetDrawTPage(mode, 0, 1, 0xA0);
    addPrim(ot, mode);
    /* the match depends on ++mode: with mode + 1, the OT and 0xFFFFFF
       outrank mode in the local allocator and take s2 and s3 */
    GFX.funcs.setPrim(++mode);
}

/* The white flash's task: brightens up to full white and holds it, then once
   ended (FIGHTSTG_endWhiteFlash) fades back and dies; draws itself each frame */
void FIGHTSTG_updateWhiteFlash(WhiteFlash *task) {
    switch (task->state) {
    case TASK_INIT: /* the match depends on this case, which default covers */
    default:
        task->nextState(task);
        task->level = 0;
        /* fallthrough */
    case TASK_RUN:
        if (task->substate == 0) {
            task->level += task->speed * GFX.funcs.getFrameTime();
            if (task->level >= 0xFF) {
                task->level = 0xFF;
                task->nextSubstate(task);
            }
        }
        break;
    case TASK_DONE:
        task->level -= task->speed * GFX.funcs.getFrameTime();
        if (task->level < 0) {
            task->level = 0;
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_KILL:
        break;
    }
    FIGHTSTG_drawWhiteFlash(task);
}

/* Fades the white flash back out over FRAMES */
void FIGHTSTG_endWhiteFlash(WhiteFlash *task, s32 frames) {
    task->speed = 0xFF / frames;
    task->setState(task, TASK_DONE);
}

/* Starts a white flash that whitens the screen over FRAMES */
WhiteFlash *FIGHTSTG_startWhiteFlash(s32 frames) {
    WhiteFlash *task = createTask(FIGHTSTG_updateWhiteFlash, sizeof(WhiteFlash), 0);

    task->speed = 0xFF / frames;
    return task;
}
