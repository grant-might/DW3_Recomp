/* The title screen's task, which runs its parts; its images; and the panels'
   fades */

#include "menus/title.h"

/* Once the fade out ends, starts the chosen mode; returns 1 then */
s32 STDWTITL_leaveTitle(TitleTask *task, TitleChildren *children) {
    s32 left = 0;

    if (children->fade->isDone(children->fade)) {
        switch (task->choice) {
        case 0:
            break;
        case 1:
            left = 1;
            GAME.funcs.newGame();
            GAME.funcs.resetPlayTime();
            BATTLE_SETUP.clearGauges();
            MEMCARD_FUNCS.setFileName();
            GAME.funcs.requestMode(MODE_NEW_GAME, 0);
            break;
        case 2:
            left = 1;
            GAME.funcs.newGame();
            BATTLE_SETUP.clearGauges();
            MEMCARD_FUNCS.setFileName();
            GAME.funcs.requestMode(MODE_CONTINUE, 0);
            break;
        case 3:
#if VERSION_US
            GAME.funcs.requestMode(MODE_OPENING, 0);
#elif VERSION_EU || VERSION_JP
            if (LANGUAGE == 0) {
                if (GAME.funcs.getPrevMode() == MODE_OPENING) {
                    GAME.funcs.requestMode(MODE_OPENING_2, 0);
                } else {
                    GAME.funcs.requestMode(MODE_OPENING, 0);
                }
            } else {
                GAME.funcs.requestMode(MODE_OPENING_2, 0);
            }
#endif
            left = 1;
            break;
        }
    }
    return left;
}

/* Brings in the titles, the glint, the logo and the menu, then waits for an
   option; returns 1 once the chosen mode starts */
s32 STDWTITL_stepTitle(TitleTask *task, TitleChildren *children) {
    s32 left = 0;

    switch (task->substate) {
    case 0:
    default:
        children->title0->show(children->title0);
        children->title1->show(children->title1);
        task->timer = 0;
        task->setSubstate(task, 2);
        break;
    case 1:
        if (task->timer++ >= 0) {
            task->timer = 0;
            children->title1->show(children->title1);
            task->nextSubstate(task);
        }
        break;
    case 2:
        if (task->timer >= 10) {
            task->timer = 0;
            SOUND.playSound(STDWTITL_TITLE_MUSIC);
            children->glint->show(children->glint);
            task->nextSubstate(task);
        }
        task->timer += GFX.funcs.getFrameTime();
        break;
    case 3:
        if (task->timer >= 12) {
            task->timer = 0;
            task->nextSubstate(task);
        }
        task->timer += GFX.funcs.getFrameTime();
        break;
    case 4:
        if (task->timer >= 12) {
            task->timer = 0;
            children->logo->show(children->logo);
            task->nextSubstate(task);
        }
        task->timer += GFX.funcs.getFrameTime();
        break;
    case 5:
        if (task->timer >= 12) {
            task->timer = 0;
            children->menu->show(children->menu);
            children->background->animate(children->background);
            task->nextSubstate(task);
        }
        task->timer += GFX.funcs.getFrameTime();
        break;
    case 6:
        task->choice = children->menu->getChoice(children->menu);
        if (task->choice != 0) {
            task->nextSubstate(task);
            children->fade = STDWTITL_startEdgeFadeTask();
            if (task->choice == 1) {
#if VERSION_US || VERSION_JP
                FILE_CACHE.request(0x158);
#elif VERSION_EU
                FILE_CACHE.request(0x166);
#endif
            }
        }
        break;
    case 7:
        children->fade->start(children->fade);
        task->nextSubstate(task);
        break;
    case 8:
        left = STDWTITL_leaveTitle(task, children);
        break;
    }
    return left;
}

/* The title screen's task: creates its background, title parts, glint, logo and
   menu (the Alt ones for the European version's language 0), then runs it until an option
   is taken or it times out; stops the title music when killed */
void STDWTITL_tickTitle(TitleTask *task, TitleChildren *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        children->background = STDWTITL_startBackgroundTask(0);
#if VERSION_US
        children->title0 = STDWTITL_startTitle0Task(0);
        children->title1 = STDWTITL_startTitle1Task(0);
        children->glint = STDWTITL_startGlintTask(0);
        children->logo = STDWTITL_startLogoTask(0);
        children->menu = STDWTITL_startMenuTask(0);
#elif VERSION_EU || VERSION_JP
        if (LANGUAGE == 0) {
            children->title0 = STDWTITL_startTitle0AltTask(0);
            children->title1 = STDWTITL_startTitle1AltTask(0);
            children->logo = STDWTITL_startLogoTask(0);
            children->glint = STDWTITL_startGlintAltTask(0);
            children->menu = STDWTITL_startMenuTask(0);
        } else {
            children->title0 = STDWTITL_startTitle0Task(0);
            children->title1 = STDWTITL_startTitle1Task(0);
            children->glint = STDWTITL_startGlintTask(0);
            children->logo = STDWTITL_startLogoTask(0);
            children->menu = STDWTITL_startMenuTask(0);
        }
#endif
        task->timer = 0;
        break;
    case TASK_RUN:
        if (STDWTITL_stepTitle(task, children)) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        SOUND.stopSound(STDWTITL_TITLE_MUSIC);
        break;
    }
}

/* Creates the title screen (task), under its title loader parent */
TitleTask *STDWTITL_startTitleTask(TitleLoaderTask *parent) {
    TitleTask *task = createTask(STDWTITL_tickTitle, sizeof(TitleTask), sizeof(TitleChildren));

    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->parent = parent;
    return task;
}

/* Loads the title's images to VRAM: the sprites (by the language in the European
   version) and the background's four TIM archives */
void STDWTITL_loadTitleImages(void) {
    TimLoader loader;

    initTimLoader(&loader);
#if VERSION_US
    loader.setImagePos(0x280, 0);
    loader.setClutPos(0, 0x1F0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_titleImages[1].images));
    STDWTITL_spriteBank = STDWTITL_titleImages[1].sprites;
#elif VERSION_EU || VERSION_JP
    loader.setImagePos(0x280, STDWTITL_TEXTURE_Y);
    loader.setClutPos(0, 0x1F0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_titleImages[LANGUAGE].images));
    STDWTITL_spriteBank = STDWTITL_titleImages[LANGUAGE].sprites;
#endif
    loader.setClutPos(0, 0x1F3);
    loader.setImagePos(0x300, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(4)));
    loader.setImagePos(0x340, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(5)));
    loader.setImagePos(0x380, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(6)));
    loader.setImagePos(0x3C0, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STDWTITL_BACKGROUND(7)));
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
