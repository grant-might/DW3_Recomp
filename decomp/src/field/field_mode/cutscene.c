/* The animations of a cutscene over the field */

#include "field/field_mode.h"

/* A cutscene's animation over the field (FIELDSTG_createCutsceneAnim): kind 0 plays once,
   kind 1 loops two animations until 160 ticks of frame time pass. The match
   depends on the if/else around the file calls (not a ?: in their argument)
   and on case 0 next to default. */
void FIELDSTG_playCutsceneAnim(CutsceneAnim *task) {
    TimLoader loader;
    SpriteDrawer drawer;
    s32 loading;

    switch (task->state) {
    case TASK_INIT:
    default:
        switch (task->substate) {
        case 0:
        default:
            if (task->kind) {
                FILE_CACHE.request(FIELD_ANIM_FILE + 1);
            } else {
                FILE_CACHE.request(FIELD_ANIM_FILE);
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            if (task->kind) {
                loading = FILE_CACHE.isLoading(FIELD_ANIM_FILE + 1);
            } else {
                loading = FILE_CACHE.isLoading(FIELD_ANIM_FILE);
            }
            if (loading == 0) {
                initTimLoader(&loader);
                loader.setImagePos(FIELD_CUTSCENE_X, 0);
                loader.setClutPos(0, FIELD_CUTSCENE_CLUT_Y);
                if (task->kind) {
                    loader.loadArchive(FILE_CACHE.getEntry(((FIELD_ANIM_FILE + 1) << 16) | 1));
                    SOUND.playSound(SOUND_DIG_DEMO);
                } else {
                    loader.loadArchive(FILE_CACHE.getEntry((FIELD_ANIM_FILE << 16) | 1));
                    SOUND.playSound(SOUND_SUB_DEMO);
                }
                task->nextState(task);
            }
            break;
        }
        break;
    case TASK_RUN:
        if (task->kind == 0) {
            if (task->timer <= 0) {
                task->index++;
                task->frame = FIELDSTG_cutsceneAnim[task->index].frame;
                task->timer = FIELDSTG_cutsceneAnim[task->index].duration;
            } else {
                task->timer -= GFX.funcs.getFrameTime();
            }
            if (task->frame == -1) {
                task->setState(task, TASK_DONE);
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }

    if (task->state == TASK_RUN || task->state == TASK_DONE) {
        if (task->kind) {
            while (1) {
                if (task->timer <= 0) {
                    task->index++;
                    task->frame = FIELDSTG_cutsceneLoopAnim[task->index].frame;
                    task->timer = FIELDSTG_cutsceneLoopAnim[task->index].duration;
                } else {
                    task->timer -= GFX.funcs.getFrameTime();
                }
                if (task->frame != -1) {
                    break;
                }
                task->index = task->timer;
                task->timer = -1;
            }
            while (1) {
                if (task->timer2 <= 0) {
                    task->index2++;
                    task->frame2 = FIELDSTG_cutsceneLoopAnim2[task->index2].frame;
                    task->timer2 = FIELDSTG_cutsceneLoopAnim2[task->index2].duration;
                } else {
                    task->timer2 -= GFX.funcs.getFrameTime();
                }
                if (task->frame2 != -1) {
                    break;
                }
                task->index2 = task->timer2;
                task->timer2 = -1;
            }
            task->counter += GFX.funcs.getFrameTime();
            if (task->counter > 0xA0) {
                task->setState(task, TASK_DONE);
            }
        }
        initSpriteDrawer(&drawer);
        drawer.setFollowScroll(0);
        drawer.setLayerId(FIELD_LAYER_MAP, 7);
        drawer.setTexture(FIELD_CUTSCENE_X, 0);
        drawer.setAltClut(0, FIELD_CUTSCENE_CLUT_Y);
        if (task->kind) {
            drawer.draw(FILE_CACHE.getEntry((FIELD_ANIM_FILE + 1) << 16), task->frame, 0, 0);
            if (task->frame2 != 0) {
                drawer.draw(FILE_CACHE.getEntry((FIELD_ANIM_FILE + 1) << 16), task->frame2, 0, 0);
            }
        } else if (task->state == TASK_RUN) {
            drawer.draw(FILE_CACHE.getEntry(FIELD_ANIM_FILE << 16), task->frame, 0, 0);
        }
        if (task->kind) {
            drawer.draw(FILE_CACHE.getEntry((FIELD_ANIM_FILE + 1) << 16), 0, 0, 0);
        } else {
            drawer.draw(FILE_CACHE.getEntry(FIELD_ANIM_FILE << 16), 10, 0, 0);
        }
    }
}

/* Creates a cutscene animation of a kind */
CutsceneAnim *FIELDSTG_createCutsceneAnim(s32 kind) {
    CutsceneAnim *task = createTask(FIELDSTG_playCutsceneAnim, sizeof(CutsceneAnim), 0);

    task->kind = kind;
    return task;
}
