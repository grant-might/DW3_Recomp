/* The title screen's background, and its eight looping sprite animations */

#include "menus/title.h"

/* Steps one of the background's looping animations by a frame and returns its sprite
   frame (300: nothing drawn); callers pass depth 0, it recurses past entries already over */
s32 STDWTITL_stepLoopingAnimation(AnimState *anim, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[anim->index];

    if (depth == 0) {
        anim->timer--;
    }
    if (anim->timer <= 0) {
        frame++;
        anim->index++;
        anim->timer += frame->duration;
        if (frame->frame == 0xFF) {
            /* Back to the first frame */
            frame = frames;
            anim->index = 0;
            anim->timer += frame->duration;
        }
        STDWTITL_stepLoopingAnimation(anim, frames, depth + 1);
    }
    return frame->frame;
}

/* Draws the title screen's background, from entries 0-2 of its file */
void STDWTITL_drawBackground(TitleBackgroundTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setAltClut(0, 0x1F3);
    sprite.setTexture(0x300, 0);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(0)), 0, 0, 0);
    sprite.setTexture(0x340, 0);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(1)), 0, 0, 0);
    sprite.setTexture(0x380, 0);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(2)), 0, 0, 0);
}

/* Steps and draws the background's eight looping sprite animations; the last two at
   the positions picked for them */
void STDWTITL_drawBackgroundSprites(TitleBackgroundTask *task) {
    SpriteDrawer sprite;
    s32 frame0 = STDWTITL_stepLoopingAnimation(&task->anims[0], STDWTITL_backgroundAnim0, 0);
    s32 frame1 = STDWTITL_stepLoopingAnimation(&task->anims[1], STDWTITL_backgroundAnim1, 0);
    s32 frame2 = STDWTITL_stepLoopingAnimation(&task->anims[2], STDWTITL_backgroundAnim2, 0);
    s32 frame3 = STDWTITL_stepLoopingAnimation(&task->anims[3], STDWTITL_backgroundAnim3, 0);
    s32 frame4 = STDWTITL_stepLoopingAnimation(&task->anims[4], STDWTITL_backgroundAnim4, 0);
    s32 frame5 = STDWTITL_stepLoopingAnimation(&task->anims[5], STDWTITL_backgroundAnim5, 0);
    s32 frame6 = STDWTITL_stepLoopingAnimation(&task->anims[6], STDWTITL_backgroundAnim6, 0);
    s32 frame7 = STDWTITL_stepLoopingAnimation(&task->anims[7], STDWTITL_backgroundAnim7, 0);

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setTexture(0x3C0, 0);
    if (frame0 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(3)), frame0, 0, 0);
    }
    if (frame1 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(3)), frame1, 0, 0);
    }
    if (frame2 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(3)), frame2, 0, 0);
    }
    if (frame3 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(3)), frame3, 0, 0);
    }
    if (frame4 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(8)), frame4, 0, 0);
    }
    if (frame5 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(8)), frame5, 0, 0);
    }
    if (frame6 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(8)), frame6, task->pos6.x, task->pos6.y);
    }
    if (frame7 != 300) {
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(8)), frame7, task->pos7.x, task->pos7.y);
    }
}

/* The background's task: draws the background; once animated, also its eight
   sprite animations, the last two with a random delay and position */
void STDWTITL_tickBackground(TitleBackgroundTask *task) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->anims[0].index = 0;
            task->anims[0].timer = STDWTITL_backgroundAnim0[0].duration;
            task->anims[1].index = 0;
            task->anims[1].timer = STDWTITL_backgroundAnim1[0].duration;
            task->anims[2].index = 0;
            task->anims[2].timer = STDWTITL_backgroundAnim2[0].duration;
            task->anims[3].index = 0;
            task->anims[3].timer = STDWTITL_backgroundAnim3[0].duration;
            task->anims[4].index = 0;
            task->anims[4].timer = STDWTITL_backgroundAnim4[0].duration;
            task->anims[5].index = 0;
            task->anims[5].timer = STDWTITL_backgroundAnim5[0].duration;
            task->anims[6].index = 0;
            task->anims[6].timer = STDWTITL_backgroundAnim6[0].duration + RANDOM.next() % 300;
            task->anims[7].index = 0;
            task->anims[7].timer = STDWTITL_backgroundAnim7[0].duration + RANDOM.next() % 240;
            i = RANDOM.next() % 3;
            task->pos6.x = STDWTITL_background6Positions[i].x;
            task->pos6.y = STDWTITL_background6Positions[i].y;
            i = RANDOM.next() % 5;
            task->pos7.x = STDWTITL_background7Positions[i].x;
            task->pos7.y = STDWTITL_background7Positions[i].y;
            task->setSubstate(task, 1);
        }
        STDWTITL_drawBackgroundSprites(task);
        /* fallthrough */
    case TASK_RUN:
        STDWTITL_drawBackground(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Starts the background's sprite animations (the task's animate) */
void STDWTITL_animateBackground(TitleBackgroundTask *task) {
    if (task->state == TASK_RUN) {
        task->setState(task, TASK_DONE);
    }
}

/* Creates the title screen's background (task); skip is not used */
TitleBackgroundTask *STDWTITL_startBackgroundTask(s32 skip) {
    TitleBackgroundTask *task = createTask(STDWTITL_tickBackground, sizeof(TitleBackgroundTask), 0);

    task->animate = STDWTITL_animateBackground;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}
