/*
 * The darkness: the screen drawn black (a white quad subtracted from it,
 * blend mode 2) while in TASK_DONE (GAME.dark is set then)
 */
void updateDarkness(StageTask *task) {
    Layer *layer;
    u_long *ot;
    POLY_F4 *poly;
    DR_TPAGE *mode;

    if (task->state == TASK_INIT) {
        if (GAME.dark != 0) {
            task->setState(task, TASK_DONE);
        } else {
            task->setState(task, TASK_RUN);
        }
    }
    switch (task->state) {
    case TASK_RUN:
        if (task->key1 != 0) {
            if (task->substate != 5) {
                task->nextSubstate(task);
            } else {
                task->key1 = 0;
                task->setState(task, TASK_DONE);
                SOUND.playSound(SOUND_SWITCH02);
            }
        }
        GAME.dark = 0;
        break;
    case TASK_DONE:
        layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
        ot = (u_long *)layer->getOtEntry(layer, 6);
        poly = GFX.funcs.getPrim();
        setlen(poly, 5);
        poly->code = 0x2A;
        poly->r0 = poly->g0 = poly->b0 = 0xFF;
        poly->x1 = poly->x3 = 320;
        poly->x0 = poly->x2 = 0;
        poly->y0 = poly->y1 = 0;
        poly->y2 = poly->y3 = 256;
        addPrim(ot, poly);
        mode = (DR_TPAGE *)(poly + 1);
        setlen(mode, 1);
        mode->code[0] = 0xE1000245;
        addPrim(ot, mode);
        GFX.funcs.setPrim(mode + 1);
        GAME.dark = 1;
        break;
    case TASK_INIT:
    case TASK_KILL:
        break;
    }
}
