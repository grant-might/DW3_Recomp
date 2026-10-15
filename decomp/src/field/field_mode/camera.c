/* The camera, which follows an actor or looks at a spot, and shakes */

#include "field/field_mode.h"

/* Scrolls the map layer to the camera's center, kept inside the map, and
   shakes it while shaking with its sound */
void FIELDSTG_scrollCamera(Camera *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    MapStreamer *map;
    Vec2 *size;
    s32 x;
    s32 y;
    s32 shake;

    x = task->center.x - 0xA0;
    y = task->center.y - 0x8C;
    if (task->hasBounds == 0) {
        map = TASK_REGISTRY.funcs.find(FIELD_TASK_MAP, -1, -1);
        if (map != NULL) {
            if (map->state == TASK_RUN) {
                size = map->getSize(map);
                task->bounds = *size;
                task->hasBounds = 1;
            }
        } else if (GAME.funcs.getMode() != 0x2DE) {
            task->bounds.x = 0x7FFF;
            task->bounds.y = 0x7FFF;
        } else {
            task->bounds.x = 0x500;
            task->bounds.y = 0x400;
        }
    }
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (task->bounds.x - SCREEN_WIDTH < x) {
        x = task->bounds.x - SCREEN_WIDTH;
    }
    if (task->bounds.y - SCREEN_HEIGHT < y) {
        y = task->bounds.y - SCREEN_HEIGHT;
    }
    shake = 0;
    if (task->shaking != 0) {
        task->shake = (task->shake + 1) & 3;
        shake = task->shake + 1;
        if (task->voice == -1) {
            task->voice = SOUND.playSound(SOUND_COMCD203);
        }
    } else if (task->voice != -1) {
        SOUND.keyOff(SOUND_COMCD203, task->voice);
        task->voice = -1;
    }
    layer->setScroll(layer, (FIELDSTG_shakeOffsets[shake].x + x) << 8, (FIELDSTG_shakeOffsets[shake].y + y) << 8);
}

/* The camera's update: it follows its target actor (substate 0) or looks at
   a spot (1), panning there unless it snaps */
void FIELDSTG_updateCamera(Camera *task) {
    Vec2 delta;
    Vec2 sign;

    switch (task->state) {
        default:
        case TASK_INIT:
            task->target = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
            task->snap = 1;
            if (task->target != NULL) {
                task->nextState(task);
            }
            break;
        case TASK_RUN:
            switch (task->substate) {
                case 0:
                    task->center.x = task->target->tile.x;
                    task->center.y = task->target->tile.y - (task->target->z >> 8);
                    if ((task->step == 0) & (task->snap == 0)) {
                        task->nextStep(task);
                    }
                    break;
                case 1:
                    task->center.x = task->spotX;
                    task->center.y = task->spotY;
                    if ((task->step == 0) & (task->snap == 0)) {
                        task->nextStep(task);
                    }
                    break;
            }
            if (task->step == 1) {
                sign.x = 1;
                sign.y = 1;
                delta.x = task->center.x - task->pan.x;
                if (delta.x < 0) {
                    sign.x = -1;
                    delta.x = -delta.x;
                }
                delta.y = task->center.y - task->pan.y;
                if (delta.y < 0) {
                    sign.y = -1;
                    delta.y = -delta.y;
                }
                if (delta.x != 0 && delta.y != 0) {
                    if (delta.x > 4) {
                        delta.x /= 4;
                    } else if (delta.x > 2) {
                        delta.x /= 2;
                    } else {
                        delta.x = 1;
                    }
                    task->center.x = task->pan.x += delta.x * sign.x;
                    if (delta.y > 4) {
                        delta.y /= 4;
                    } else if (delta.y > 2) {
                        delta.y /= 2;
                    } else {
                        delta.y = 1;
                    }
                    task->center.y = task->pan.y += delta.y * sign.y;
                } else {
                    task->nextStep(task);
                }
            }
            FIELDSTG_scrollCamera(task);
            break;
        case TASK_DONE:
            break;
        case TASK_KILL:
            if (task->voice != -1) {
                SOUND.keyOff(SOUND_COMCD203, task->voice);
                task->voice = -1;
            }
            break;
    }
}

/* Creates the camera */
Camera *FIELDSTG_createCamera(void) {
    Camera *task = createTaskWithId(FIELDSTG_updateCamera, sizeof(Camera), 0, FIELD_TASK_CAMERA);

    task->voice = -1;
    return task;
}

/* Points the camera at the actor id, at once if snap */
void FIELDSTG_followWithCamera(s32 snap, s32 id) {
    Camera *task = TASK_REGISTRY.funcs.find(FIELD_TASK_CAMERA, -1, -1);

    if (task != NULL) {
        task->unk7C = 0;
        task->snap = snap;
        task->targetId = id;
        task->target = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, id, -1);
        task->pan = task->center;
        task->setSubstate(task, 0);
    }
}

/* Points the camera at the spot (x, y), at once if snap */
void FIELDSTG_pointCamera(s32 snap, s32 x, s32 y) {
    Camera *task = TASK_REGISTRY.funcs.find(FIELD_TASK_CAMERA, -1, -1);

    if (task != NULL) {
        task->unk7C = 0;
        task->snap = snap;
        task->spotX = x;
        task->spotY = y;
        task->pan = task->center;
        task->setSubstate(task, 1);
    }
}

/* Starts or stops the camera's shake */
void FIELDSTG_shakeCamera(s32 shaking) {
    Camera *task = TASK_REGISTRY.funcs.find(FIELD_TASK_CAMERA, -1, -1);

    if (task != NULL) {
        task->shaking = shaking;
    }
}
