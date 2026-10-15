/* FIGHTSTG's faces: a model's blinking eyes and its other face parts. */

#include "battle/battle_mode.h"

/* The eyes' frame for the model's motion: 2 (closed) for motions 4 to 11, 1
   for 2, else 0 (open) */
s32 FIGHTSTG_getEyesFrame(Face *task) {
    switch (task->model->motion) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return 2;
    case 2:
        return 1;
    default:
        return 0;
    }
}

/* Fills prim to copy part's frame of the face's texture over the part, in
   VRAM */
void FIGHTSTG_moveFacePart(Face *task, DR_MOVE *prim, s32 part, s32 frame) {
    RECT rect;
    Vec2 pos;

    rect.x = task->parts[part].rect.frames[frame][0] + task->texPos.x;
    rect.y = task->parts[part].rect.frames[frame][1] + task->texPos.y;
    rect.w = task->parts[part].rect.w;
    rect.h = task->parts[part].rect.h;
    pos.x = task->parts[part].rect.x + task->texPos.x;
    pos.y = task->parts[part].rect.y + task->texPos.y;
    SetDrawMove(prim, &rect, pos.x, pos.y);
}

/* A fighter's face task: each frame, copies the eyes' frame (blinking at
   random while open) and the other parts' frames (a three-frame cycle) over
   the model's texture, for the parts whose frame changed */
void FIGHTSTG_updateFace(Face *task) {
    Layer *layer;
    u_long *ot;
    DR_MOVE *prim;
    s32 eyes;
    s32 frame;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->partCount == 0) {
            task->setState(task, TASK_KILL);
        } else {
            task->nextState(task);
        }
        break;
    case TASK_RUN:
        eyes = FIGHTSTG_getEyesFrame(task);
        if (eyes == 0) {
            switch (task->blinkTimer >> 1) {
            case 0:
                task->blinkTimer = (RANDOM.next() & 0x7F) + 60;
                /* fallthrough */
            default:
                eyes = 0;
                break;
            case 1:
            case 2:
            case 5:
            case 6:
                eyes = 1;
                break;
            case 3:
            case 4:
                eyes = 2;
                break;
            }
            task->blinkTimer -= FIGHTSTG_battle.frames;
            if (task->blinkTimer < 0) {
                task->blinkTimer = 0;
            }
        } else {
            task->blinkTimer = 0;
        }
        task->time += FIGHTSTG_battle.frames;
        frame = task->time % 18 / 6;
        layer = GFX.funcs.getLayer(SCREEN_LAYER);
        ot = layer->getOtEntry(layer, 0);
        prim = GFX.funcs.getPrim();
        for (i = 0; i < 2; i++) {
            if (task->parts[i].used && task->parts[i].frame != eyes) {
                task->parts[i].frame = eyes;
                FIGHTSTG_moveFacePart(task, prim, i, eyes);
                addPrim(ot, prim);
                prim++;
            }
        }
        for (i = 2; i < 16; i++) {
            if (task->parts[i].used && task->parts[i].frame != frame) {
                task->parts[i].frame = frame;
                FIGHTSTG_moveFacePart(task, prim, i, frame);
                addPrim(ot, prim);
                prim++;
            }
        }
        GFX.funcs.setPrim(prim);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the face of fighter's model, with its parts
   (FIGHTSTG_fighterCache.getFace); none for fighter 0 */
Face *FIGHTSTG_createFace(Model *model, s32 fighter) {
    Face *task;
    FaceRect *rects;
    s32 i;

    if (fighter == 0) {
        return NULL;
    }
    task = createTask(FIGHTSTG_updateFace, sizeof(Face), 0);
    task->model = model;
    task->texPos = model->texPos;
    rects = FIGHTSTG_fighterCache.getFace(fighter);
    for (i = 15; i >= 0; i--) {
        task->parts[i].used = 0;
    }
    for (i = 0; i < 16; i++) {
        if (rects[i].x == 0xFF) {
            return task;
        }
        if (rects[i].w != 0) {
            task->parts[i].rect = rects[i];
            task->parts[i].used = 1;
            task->partCount++;
        }
    }
    return task;
}
