/* The area name banner, with the names of the area and the place. Its
   rodata, at 0x800825CC (USA), starts FIELDSTG.PRO's fourth object (see
   data/fieldstg.c). */

#include "field/field_mode.h"

/* Opens the banner's windows with the names of the mode's area and place
   (FIELDSTG_areaNames) */
void FIELDSTG_showAreaName(AreaBanner *task, AreaNameWindows *windows) {
    s16 mode = GAME.funcs.getMode();
    s32 i;

    for (i = 0; FIELDSTG_areaNames[i].mode != 0; i++) {
        if (FIELDSTG_areaNames[i].mode == mode) {
            windows->area = createTextWindow(FIELD_LAYER_BANNER, 1, 0x80, 0x1A);
            windows->area->setString(windows->area, FILE_CACHE.load(TEXT_FILE(TEXT_AREA_NAMES)), FIELDSTG_areaNames[i].area);
            windows->area->setTypeDelay(windows->area, 5);
            windows->place = createTextWindow(FIELD_LAYER_BANNER, 1, 0x28, 0x44);
            windows->place->setString(windows->place, FILE_CACHE.load(TEXT_FILE(TEXT_STAGE_NAMES)), FIELDSTG_areaNames[i].place);
            windows->place->setTypeDelay(windows->place, 5);
            break;
        }
    }
}

/* Stretches a box toward from-to along one axis. The match depends on each
   case having its own variables. */
void FIELDSTG_stretchBannerBox(AreaBanner *task, BannerBox *box) {
    switch (box->stretch) {
    case 1: {
        s32 start = box->pos.vx;
        s32 end = start + box->size.vx;
        s32 from = box->from;
        s32 to = box->to;
        s32 speed = box->speed;

        if (start < from) {
            start += speed;
            if (start > from) {
                start = from;
            }
        } else if (start > from) {
            start -= speed;
            if (start < from) {
                start = from;
            }
        }
        if (end < to) {
            end += speed;
            if (end > to) {
                end = to;
            }
        } else if (end > to) {
            end -= speed;
            if (end < to) {
                end = to;
            }
        }
        box->pos.vx = start;
        box->size.vx = end - start;
        if (start == from && end == to) {
            box->stretch = 0;
        }
        break;
    }
    case 2: {
        s32 start = box->pos.vy;
        s32 end = start + box->size.vy;
        s32 from = box->from;
        s32 to = box->to;
        s32 speed = box->speed;

        if (start < from) {
            start += speed;
            if (start > from) {
                start = from;
            }
        } else if (start > from) {
            start -= speed;
            if (start < from) {
                start = from;
            }
        }
        if (end < to) {
            end += speed;
            if (end > to) {
                end = to;
            }
        } else if (end > to) {
            end -= speed;
            if (end < to) {
                end = to;
            }
        }
        box->pos.vy = start;
        box->size.vy = end - start;
        if (start == from && end == to) {
            box->stretch = 0;
        }
        break;
    }
    }
}

/* Draws one of the banner's boxes, a flat quad of a color */
void FIELDSTG_drawBannerBox(AreaBanner *task, u_long *ot, DVECTOR pos, DVECTOR size, s32 color) {
    POLY_F4 *poly = GFX.funcs.getPrim();

    setlen(poly, 5);
    *(s32 *)&poly->r0 = color; /* r0, g0 and b0 in one store; code follows */
    poly->code = 0x28;
    poly->x0 = pos.vx;
    poly->x1 = pos.vx + size.vx;
    poly->x2 = pos.vx;
    poly->x3 = pos.vx + size.vx;
    poly->y0 = pos.vy;
    poly->y1 = pos.vy;
    poly->y2 = pos.vy + size.vy;
    poly->y3 = pos.vy + size.vy;
    addPrim(ot, poly);
    GFX.funcs.setPrim(poly + 1);
}

/*
 * The area name banner: in TASK_INIT it copies its ten boxes from
 * FIELDSTG_bannerBoxes and opens the area and place windows
 * (FIELDSTG_showAreaName); in TASK_RUN the boxes appear and stretch in turn
 * until both windows have finished; in TASK_DONE it closes the layer's clip
 * from the top and the bottom.
 */
void FIELDSTG_updateBanner(AreaBanner *task, AreaNameWindows *windows) {
    Layer *layer;
    u_long *ot;
    s32 i;

    switch (task->state) {
    default:
    case TASK_INIT:
        if (task->key1 != 0) {
            s32 j;

            for (j = 0; j < 10; j++) {
                task->boxes[j] = FIELDSTG_bannerBoxes[j];
            }
            FIELDSTG_showAreaName(task, windows);
            task->nextState(task);
        } else {
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        default:
        case 0:
            task->boxes[0].visible = 1;
            task->boxes[5].visible = 1;
            if (task->boxes[5].stretch != 0) {
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            task->boxes[8].visible = 1;
            task->boxes[9].visible = 1;
            task->boxes[6].visible = 1;
            task->boxes[1].visible = 1;
            task->boxes[2].visible = 1;
            task->boxes[7].visible = 1;
            task->boxes[3].visible = 1;
            if (task->boxes[9].stretch != 0) {
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            task->boxes[4].visible = 1;
            if (task->boxes[4].stretch == 0) {
                task->nextSubstate(task);
            }
            break;
        case 4:
            if (windows->area->isFinished(windows->area) && windows->place->isFinished(windows->place)) {
                task->setState(task, TASK_DONE);
            }
            break;
        }
        break;
    case TASK_DONE:
        layer = GFX.funcs.getLayer(FIELD_LAYER_BANNER);
        /* the match depends on the empty case 0 */
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->step += GFX.funcs.getFrameTime();
            if (task->step < 60) {
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->clip.w = 320;
            task->clip.x = 0;
            task->clip.y = 0;
            task->clip.h = 240;
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            task->clip.y += 8;
            task->clip.h -= 16;
            layer->setClipPos(layer, task->clip.x, task->clip.y);
            layer->setClipSize(layer, task->clip.w, task->clip.h);
            if (task->clip.h == 0) {
                task->setState(task, TASK_KILL);
            }
            break;
        }
        break;
    case TASK_KILL:
        FIELDSTG_state.frozen = 0;
        break;
    }
    if (task->state >= 1 && task->state <= 2 && task->key1 != 0) {
        Layer *top = GFX.funcs.getLayer(FIELD_LAYER_BANNER);

        ot = (u_long *)top->getOtEntry(top, 1);
        for (i = 0; i < 10; i++) {
            if (task->boxes[i].visible) {
                FIELDSTG_stretchBannerBox(task, &task->boxes[i]);
                FIELDSTG_drawBannerBox(task, ot, task->boxes[i].pos, task->boxes[i].size, task->boxes[i].color);
            }
        }
    }
}

/* Creates the area name banner, which closes at once unless show; the
   player is frozen until it ends */
Task *FIELDSTG_createBanner(s32 show) {
    Task *task = createTaskWithId(FIELDSTG_updateBanner, sizeof(AreaBanner), 8, FIELD_TASK_BANNER);

    task->key1 = show;
    FIELDSTG_state.frozen = 1;
    return task;
}
