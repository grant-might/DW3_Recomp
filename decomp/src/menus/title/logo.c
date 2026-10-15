/* The title screen's logo */

#include "menus/title.h"

/* Draws the logo once it is visible: sprite 6, and sprites 3 and 5 at their
   animations' current CLUT rows */
void STDWTITL_drawLogo(LogoTask *task) {
    SpriteDrawer sprite;

    if (task->visible) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 6, 29, 209);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->anims[0].frame);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 3, 8, 28);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->anims[1].frame);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 5, 20, 202);
    }
}

/* The logo's task: waits until shown, plays its two animations once
   (STDWTITL_logoFrames), then keeps it drawn; with skip it starts complete */
void STDWTITL_tickLogo(LogoTask *task) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->nextState(task);
        } else {
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->visible = 1;
            task->nextSubstate(task);
            task->anims[0].index = 0;
            task->anims[1].index = 0;
            /* fallthrough */
        case 2:
            for (i = 0; i < 2; i++) {
                if (!task->anims[i].done) {
                    if (STDWTITL_logoFrames[i][task->anims[i].index] == -1) {
                        task->anims[i].done = 1;
                        task->anims[i].index--;
                    }
                    task->anims[i].frame = STDWTITL_logoFrames[i][task->anims[i].index];
                    task->anims[i].index++;
                }
            }
            if (task->anims[0].done + task->anims[1].done == 2) {
                task->setSubstate(task, 2);
            }
            STDWTITL_drawLogo(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->anims[0].frame = 11;
            task->anims[1].frame = 7;
            task->visible = 1;
            task->nextSubstate(task);
        }
        STDWTITL_drawLogo(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Starts the logo's animations (the task's show), only while it waits */
void STDWTITL_showLogo(LogoTask *task) {
    if (task->state == TASK_RUN) {
        task->setSubstate(task, 1);
    }
}

/* Creates the title screen's logo (task); skip: shown complete from the start */
LogoTask *STDWTITL_startLogoTask(s32 skip) {
    LogoTask *task = createTask(STDWTITL_tickLogo, sizeof(LogoTask), 0);

    task->show = STDWTITL_showLogo;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}
