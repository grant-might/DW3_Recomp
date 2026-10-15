/* The still screen (SPLASH_FILE) */

#include "menus/title.h"

/* Draws the still screen at its fade (the European version has a picture for each
   group of languages) */
void STDWTITL_drawSplash(SplashTask *task) {
    SpriteDrawer sprite;
#if VERSION_EU || VERSION_JP
    /* the screen has a frame per group of languages */
    s32 frame = 0;

    switch (LANGUAGE) {
    case 0:
    case 1:
    case 2:
        frame = 0;
        break;
    case 3:
    case 6:
        frame = 2;
        break;
    case 4:
        frame = 3;
        break;
    case 5:
        frame = 1;
        break;
    }
#endif

    initSpriteDrawer(&sprite);
    sprite.setLayerId(STDWTITL_SPLASH_LAYER, 1);
    sprite.setTexture(0x280, 0);
    sprite.setClutRow(task->fade);
#if VERSION_US
    sprite.draw(FILE_CACHE.getEntry(SPLASH_SPRITES), 0, 0, 0);
#elif VERSION_EU || VERSION_JP
    sprite.draw(FILE_CACHE.getEntry(SPLASH_SPRITES), frame, 0, 0);
#endif
}

/* The still screen's task: fades it in, waits 2 seconds, then for Start or five
   minutes, fades it out and a second later goes on to MODE_OPENING */
void STDWTITL_tickSplash(SplashTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->fade = 15;
        task->timer = 0;
        SOUND.stopAll();
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            if (task->timer >= 2) {
                task->timer -= 2;
                if (--task->fade <= 0) {
                    task->setSubstate(task, 1);
                    task->timer = 0;
                }
            }
            break;
        case 1:
            if (task->timer >= 120) {
                task->nextSubstate(task);
                task->timer = 0;
            }
            break;
        case 2:
            if (PAD_PRESSED(PAD_START)) {
                task->timer = 18000;
            }
            if (task->timer >= 18000) {
                task->nextSubstate(task);
                task->timer = 0;
            }
            break;
        case 3:
            if (task->timer >= 2) {
                task->timer -= 2;
                if (++task->fade >= 15) {
                    task->nextSubstate(task);
                    task->timer = 0;
                }
            }
            break;
        case 4:
            if (task->timer >= 60) {
                task->nextSubstate(task);
                task->timer = 0;
            }
            break;
        case 5:
            GAME.funcs.requestMode(MODE_OPENING, 0);
            task->setState(task, TASK_KILL);
            break;
        }
        task->timer += GFX.funcs.getFrameTime();
        STDWTITL_drawSplash(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the still screen (task) */
SplashTask *STDWTITL_startSplashTask(void) {
    return createTask(STDWTITL_tickSplash, sizeof(SplashTask), 0);
}
