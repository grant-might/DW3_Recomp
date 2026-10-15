/* The fade out from the screen's edges once an option is chosen */

#include "menus/title.h"

/* The edge fade's darkness after time frames: 0 to 255 along a quarter sine over 30 frames */
s32 STDWTITL_getEdgeFadeLevel(s32 time) {
    if (time >= 30) {
        return 255;
    }
    return rsin(time * 1024 / 30) * 30 / 4096 * 255 / 30;
}

/* Darkens the screen by level: four subtractive triangles from its centre to its
   edges, the edges twice as dark */
void STDWTITL_drawEdgeFade(s32 level) {
    Layer *layer = GFX.funcs.getLayer(STDWTITL_TITLE_LAYER);
    u_long *ot = layer->getOtEntry(layer, 0);
    POLY_G3 *poly = GFX.funcs.getPrim();
    DR_TPAGE *mode;
    s32 edge;
    s32 i;

    edge = level * 2;
    for (i = 0; i < 4; i++) {
        setPolyG3(poly);
        setSemiTrans(poly, 1);
        poly->r0 = poly->g0 = poly->b0 = level;
        if (edge >= 256) {
            edge = 255;
        }
        poly->r1 = poly->g1 = poly->b1 = poly->r2 = poly->g2 = poly->b2 = edge;
        /* From the centre of the screen to one of its edges */
        poly->x0 = 160;
        poly->y0 = 120;
        poly->x1 = STDWTITL_edgeFadeLines[i].x1;
        poly->y1 = STDWTITL_edgeFadeLines[i].y1;
        poly->x2 = STDWTITL_edgeFadeLines[i].x2;
        poly->y2 = STDWTITL_edgeFadeLines[i].y2;
        addPrim(ot, poly);
        poly++;
    }
    /* the texture page packet goes right after the triangles */
    mode = (DR_TPAGE *)poly;
    setDrawTPage(mode, 0, 1, getTPage(0, 2, 320, 0));
    addPrim(ot, mode);
    GFX.funcs.setPrim(mode + 1);
}

/* Starts the fade out from the screen's edges (the task's start) */
void STDWTITL_startEdgeFade(EdgeFadeTask *task) {
    task->done = 0;
    task->time = 0;
    task->setSubstate(task, 1);
}

/* 1 once the edge fade has ended (the task's isDone) */
s32 STDWTITL_isEdgeFadeDone(EdgeFadeTask *task) {
    return task->done;
}

/* The edge fade's task: once started, darkens the screen from its edges over 30
   frames and keeps it dark */
void STDWTITL_tickEdgeFade(EdgeFadeTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->level = 0;
        task->time = 0;
        task->done = 0;
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->level = STDWTITL_getEdgeFadeLevel(task->time++);
            if (task->time >= 30) {
                task->setSubstate(task, 0);
                task->done = 1;
            }
            break;
        }
        STDWTITL_drawEdgeFade(task->level);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the fade out from the screen's edges (task), once an option is chosen */
EdgeFadeTask *STDWTITL_startEdgeFadeTask(void) {
    EdgeFadeTask *task = createTask(STDWTITL_tickEdgeFade, sizeof(EdgeFadeTask), 0);

    task->start = STDWTITL_startEdgeFade;
    task->isDone = STDWTITL_isEdgeFadeDone;
    return task;
}
