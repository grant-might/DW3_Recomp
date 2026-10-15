/* STDWTITL's scene: the mode's root task, which starts the title screen, a
   movie or the still screen; and the still screen's loader */

#include "menus/title.h"

/* The still screen's loader task: sets up the display and its layer, loads the
   still screen's images and creates the still screen, then idles */
void STDWTITL_tickSplashLoader(Task *task, SplashTask **splash) {
    TimLoader loader;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        GFX.funcs.reset();
        GFX.funcs.allocPrimBuffers(0xA000);
        GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
        initTimLoader(&loader);
        loader.setImagePos(0x280, 0);
        loader.loadArchive(FILE_CACHE.getEntry(SPLASH_IMAGES));
        layer = GFX.funcs.createLayer(&STDWTITL_screenRect, 2, STDWTITL_SPLASH_LAYER);
        layer->setBgColor(layer, 0x1F, 0x1F, 0x1F);
        *splash = STDWTITL_startSplashTask();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the still screen's loader (task), for STDWTITL_SPLASH_MODE */
Task *STDWTITL_startSplashLoaderTask(void) {
    return createTask(STDWTITL_tickSplashLoader, sizeof(Task), sizeof(SplashTask *));
}

/* The overlay's main task: by the low byte of the game mode, starts the title screen
   (0), the still screen (STDWTITL_SPLASH_MODE) or movie n-1 (any other) */
void STDWTITL_tickScreen(Task *task, TitleModeChildren *children) {
    RECT rect;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        switch (GAME.funcs.getMode() & 0xFF) {
        case 0:
            GFX.funcs.reset();
            GFX.funcs.allocPrimBuffers(0x14000);
            GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
            rect.x = 0;
            rect.y = 0;
            rect.w = 320;
            rect.h = 240;
            layer = GFX.funcs.createLayer(&rect, 2, STDWTITL_TITLE_LAYER);
            layer->setBgColor(layer, 0, 0, 0);
            children->title = STDWTITL_startTitleLoaderTask();
            break;
        case STDWTITL_SPLASH_MODE:
            children->splash = STDWTITL_startSplashLoaderTask();
            break;
        default:
            children->movie = STDWTITL_startMovieTask((GAME.funcs.getMode() & 0xFF) - 1);
            break;
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The overlay's entry: creates its main task (STDWTITL_tickScreen) */
Task *STDWTITL_start(void) {
    return createTask(STDWTITL_tickScreen, sizeof(Task), sizeof(TitleModeChildren));
}
