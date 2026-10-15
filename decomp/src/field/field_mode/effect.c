/* The effects of up to four animations at a spot */

#include "field/field_mode.h"

/* Steps one of an effect's animations, catching up on up to 4 frames, and
   returns its frame, or 0xFF at its end */
s32 FIELDSTG_stepEffectAnim(EffectAnim *anim, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[anim->anim.index];
    s32 elapsed = GFX.funcs.getFrameTime();

    if (elapsed > 4) {
        elapsed = 4;
    }
    if (depth == 0) {
        anim->anim.timer -= elapsed;
    }
    if (anim->anim.timer <= 0) {
        frame++;
        anim->anim.index++;
        anim->anim.timer += frame->duration;
        if (frame->frame == 0xFF) {
            return 0xFF;
        }
        FIELDSTG_stepEffectAnim(anim, frames, depth + 1);
    }
    return frame->frame;
}

/* An effect's update: draws its animations at its spot until they end */
void FIELDSTG_updateEffect(FieldEffect *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    SpriteDrawer sprite;
    s32 i;
    s32 j;
    s32 frame;

    switch (task->state) {
        default:
        case TASK_INIT:
            for (i = 0; i < 4; i++) {
                task->anims[i].active = 1;
                task->anims[i].anim.index = 0;
                task->anims[i].anim.timer = FIELDSTG_effectAnims[task->set][i][0].duration;
            }
            task->nextState(task);
            break;
        case TASK_RUN:
            for (j = 0; j < 4; j++) {
                if (task->anims[j].active != 0) {
                    frame = FIELDSTG_stepEffectAnim(&task->anims[j], FIELDSTG_effectAnims[task->set][j], 0);
                    switch (frame) {
                        case 0x12C:
                            break;
                        case 0xFF:
                            task->anims[j].active = 0;
                            break;
                        default:
                            initSpriteDrawer(&sprite);
                            sprite.setTexture(FIELD_SPRITES2_X, FIELD_SPRITES_Y);
                            sprite.setLayer(layer, 0);
                            sprite.setClutRow(0);
                            sprite.draw(FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16 | 1), frame, task->x, task->y);
                            break;
                    }
                }
            }
            if (task->anims[0].active == 0 && task->anims[1].active == 0 && task->anims[2].active == 0
                && task->anims[3].active == 0) {
                task->setState(task, TASK_KILL);
            }
            break;
        case TASK_DONE:
        case TASK_KILL:
            break;
    }
}

/* Creates the effect of a set at (x, y) */
FieldEffect *FIELDSTG_createEffect(s32 x, s32 y, s32 set) {
    FieldEffect *task = createTask(FIELDSTG_updateEffect, sizeof(FieldEffect), 0);

    task->x = x;
    task->y = y;
    task->set = set;
    return task;
}
