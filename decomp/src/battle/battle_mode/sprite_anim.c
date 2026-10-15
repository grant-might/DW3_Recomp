/* FIGHTSTG's sprite animations, the frames of a 2D effect. */

#include "battle/battle_mode.h"

/* Empty, and nothing calls it */
void FIGHTSTG_nop(void) {
}

/* A layer callback that draws a sprite animation's frame: at its projected
   position, or at a screen position when its z is 0x7FFF or -1, with its
   scale, CLUT row and rotation. The match depends on taking the task as
   void * */
void FIGHTSTG_drawSpriteAnim(void *arg, Layer *layer) {
    SpriteAnim *task = arg;
    ShortVec3 screen;
    SpriteDrawer drawer;

    if (task->pos.vz != 0x7FFF && task->pos.vz != -1) {
        FIGHTSTG_battle.project(layer, &task->pos, &screen);
    } else {
        screen.x = task->pos.vx;
        screen.y = task->pos.vy;
        if (task->pos.vz != 0x7FFF) {
            screen.z = 0xFFF;
        } else {
            screen.z = 0;
        }
    }
    screen.x += task->x;
    screen.y += task->y;
    initSpriteDrawer(&drawer);
    drawer.setLayer(layer, screen.z);
    drawer.setTexture(task->texPos.x, task->texPos.y);
    if (task->scale.both != 0x10001000) {
        drawer.setScale(task->scale.v[0], task->scale.v[1], 0);
    }
    if (task->clutRow != 0) {
        drawer.setClutRow(task->clutRow);
    }
    if (task->rot.both != 0 || task->rotZ != 0) {
        drawer.setRotation(task->rot.v[0], task->rot.v[1], task->rotZ);
    }
    drawer.setPivot(screen.x, screen.y);
    drawer.draw(FILE_CACHE.getEntry(task->sheet), task->frame, screen.x, screen.y);
}

/* A frame-by-frame animation of a 2D effect: reads its flags, stride,
   duration and first values from data, then each frame the values its flags
   pick (frame, CLUT row, offset, scale, rotation) and has
   FIGHTSTG_drawSpriteAnim draw it, until its duration is over */
void FIGHTSTG_updateSpriteAnim(SpriteAnim *task) {
    s16 *data;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->flags = *task->data++;
        task->stride = *task->data++;
        task->duration = *task->data++;
        for (i = 0; i < 9; i++) {
            (&task->frame)[i] = *task->data++;
        }
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        data = task->data + task->time * task->stride;
        if (task->flags & 1) {
            task->frame = *data++;
        }
        if (task->flags & 2) {
            task->clutRow = *data++;
        }
        if (task->flags & 4) {
            task->x = *data++;
        }
        if (task->flags & 8) {
            task->y = *data++;
        }
        if (task->flags & 0x10) {
            task->scale.v[0] = *data++;
        }
        if (task->flags & 0x20) {
            task->scale.v[1] = *data++;
        }
        if (task->flags & 0x40) {
            task->rot.v[0] = *data++;
        }
        if (task->flags & 0x80) {
            task->rot.v[1] = *data++;
        }
        if (task->flags & 0x100) {
            task->rotZ = *data;
        }
        if (task->scale.v[0] != 0 && task->scale.v[1] != 0) {
            Layer *layer = GFX.funcs.getLayer(task->layerId);

            layer->addCallback(layer, FIGHTSTG_drawSpriteAnim, task);
        }
        task->time += FIGHTSTG_battle.frames;
        if (task->time >= task->duration) {
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_DONE:
        task->nextState(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates a sprite animation from data at pos, with frames of sheet
   (textures at texPos) on layer layerId */
SpriteAnim *FIGHTSTG_createSpriteAnim(s16 *data, SVECTOR *pos, s32 sheet, Vec2 *texPos, s32 layerId) {
    SpriteAnim *task = createTask(FIGHTSTG_updateSpriteAnim, sizeof(SpriteAnim), 0);

    task->data = data;
    task->pos = *pos;
    task->sheet = sheet;
    task->texPos = *texPos;
    task->layerId = layerId;
    return task;
}
