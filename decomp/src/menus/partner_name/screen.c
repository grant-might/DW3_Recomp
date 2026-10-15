/* The renaming screen's main task; the files it loads; and the panels'
   openings */

#include "menus/partner_name.h"

/* Runs the renaming screen: the partner menu, then the name entry for the chosen
   partner, whose new name is stored before going back to the menu; ends once
   the screen has faded out after leaving the menu */
void STDGNAME_stepScreen(ScreenTask *task, ScreenChildren *children) {
    s32 partner;

    switch (task->substate) {
    case 0:
    default:
        if (children->menu == NULL) {
            children->menu = STDGNAME_createMenu(task);
        }
        task->substate++;
        break;
    case 1:
        if (children->menu == NULL) {
            if (task->choice == -1) {
                task->substate = 5;
                break;
            }
            task->substate++;
        }
        break;
    case 2:
        if (children->name == NULL) {
            partner = GAME.funcs.getPartyMember(STDGNAME_funcs.partner);
            children->name = STDGNAME_createNameEntry(GAME.funcs.getPartnerStats(partner)->name, partner);
        }
        task->substate++;
        break;
    case 3:
        if (children->name->substate == 100) {
            children->name->getName(children->name,
                                    GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(STDGNAME_funcs.partner))->name);
            children->name->close(children->name);
            task->substate++;
        }
        break;
    case 4:
        if (children->name->state == TASK_DONE) {
            children->name->state = TASK_KILL;
            task->setSubstate(task, 0);
        }
        break;
    case 5:
        if (children->fade->state == TASK_DONE) {
            task->setState(task, TASK_KILL);
        }
        break;
    }
}

/* Draws the screen's background and its scrolling pattern */
void STDGNAME_drawBackground(ScreenTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layer, 7);
    sprite.setTexture(0x280, 0);
    sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), 0x31, 0, 0);
    /* Scroll one pixel every other frame, wrapping at 96 */
    if (task->bgTick) {
        task->bgScroll = task->bgScroll++ < 95 ? task->bgScroll : 0;
        task->bgTick = 0;
    } else {
        task->bgTick = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), 0x24, task->bgScroll, task->bgScroll);
}

/* The renaming screen's task: loads its files, then runs the screen and draws
   the background; goes back to the field when killed */
void STDGNAME_updateScreen(ScreenTask *task, ScreenChildren *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        switch (task->substate) {
        case 0:
        default:
            STDGNAME_funcs.loadFiles();
            task->substate++;
            break;
        case 1:
            if (STDGNAME_funcs.isLoading() == 0) {
                task->nextState(task);
            }
            break;
        }
        break;
    case TASK_RUN:
        STDGNAME_stepScreen(task, children);
        STDGNAME_drawBackground(task);
        break;
    case TASK_DONE:
        if (children->awaited == NULL) {
            task->setState(task, TASK_RUN);
        }
        STDGNAME_drawBackground(task);
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.fieldMode, 0);
        break;
    }
}

/* Fades the screen out over 30 frames (task->fadeOut), to leave it */
void STDGNAME_fadeOutScreen(ScreenTask *task) {
    ScreenChildren *children = task->children;
    ScreenFade *fade;

    children->fade = fade = STDGNAME_createFader();
    fade->start(fade, 0, 30);
}

/* Creates the renaming screen (task) */
ScreenTask *STDGNAME_createScreen(void) {
    ScreenTask *task = createTask(STDGNAME_updateScreen, sizeof(ScreenTask), sizeof(ScreenChildren));

    task->fadeOut = STDGNAME_fadeOutScreen;
    task->layer = SCREEN_LAYER;
    return task;
}

/* Loads the screen's textures and requests the keyboard and the strings; the
   chosen partner goes back to the first */
void STDGNAME_loadFiles(void) {
    TimLoader loader;

    HEAP.zero(&STDGNAME_funcs.partner, sizeof(STDGNAME_funcs.partner));
    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STDGNAME_FILE_IMAGES << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMING));
    FILE_CACHE.request(STDGNAME_FILE_KEYBOARD);
    FILE_CACHE.request(TEXT_FILE(TEXT_NAME_ENTRY));
}

/* Whether the keyboard or the strings are still loading */
s32 STDGNAME_filesLoading(void) {
    if (FILE_CACHE.isLoading(STDGNAME_FILE_KEYBOARD)) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_NAMING))) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_NAME_ENTRY)) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
