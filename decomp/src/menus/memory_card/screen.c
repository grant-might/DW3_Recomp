/* The screen's main task; the files it loads; and the panels' openings and
   moves */

#include "menus/memory_card.h"

/* The main task: loads the files, runs the save list, then fades out and
   requests the next mode */
void STGMCARD_updateScreen(MemCardScreen *screen, MemCardScreenTasks *tasks) {
    SpriteDrawer sprite;

    switch (screen->state) {
    case TASK_INIT:
    default:
        switch (screen->substate) {
        case 0:
        default:
            STGMCARD_funcs.loadFiles();
            screen->substate++;
            break;
        case 1:
            if (STGMCARD_funcs.filesLoading() == 0 && SOUND.isLoading() == 0) {
                SOUND.playSound(MUSIC(0x20, 0));
                screen->nextState(screen);
            }
            break;
        }
        break;
    case TASK_RUN:
        switch (screen->substate) {
        case 0:
        default:
            if (tasks->saves == NULL) {
                tasks->saves = STGMCARD_createSaves(screen);
            }
            screen->nextSubstate(screen);
            break;
        case 1:
            if (tasks->saves->state == 2) {
                tasks->fade = STGMCARD_createFader();
                tasks->fade->start(tasks->fade, 0, 30);
                screen->substate++;
            }
            break;
        case 2:
            if (tasks->fade->state == 2) {
                screen->state = TASK_KILL;
            }
            break;
        }
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, 3);
        sprite.setTexture(0x280, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 31, 25, 0);
        if (screen->bgTick != 0) {
            screen->bgScroll++;
            screen->bgScroll = screen->bgScroll < 96 ? screen->bgScroll : 0;
            screen->bgTick = 0;
        } else {
            screen->bgTick = 1;
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 30, screen->bgScroll, screen->bgScroll);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        SOUND.stopSound(MUSIC(0x20, 0));
        if (screen->step != 0) {
            if (screen->loading == 0) {
                GAME.funcs.requestMode(GAME.funcs.getPrevMode(), 0);
            } else {
                GAME.funcs.requestMode(MODE_TITLE, 0);
            }
        } else {
            GAME.funcs.requestMode(GAME.fieldMode, 0);
        }
        STGMCARD_funcs.freeBuffers();
        break;
    }
}

/* Creates the screen's main task, with what it needs from the mode it was
   opened from and the one it was opened for */
MemCardScreen *STGMCARD_createScreen(void) {
    MemCardScreen *screen = createTask(STGMCARD_updateScreen, sizeof(MemCardScreen), 8);
    s32 mode;
    s32 prev;
    s32 i;

    screen->layer = SCREEN_LAYER;
    mode = GAME.funcs.getMode() & 0xFF;
    screen->loading = (u32)GAME.funcs.getModeArg() >> 31 ^ 1;
    prev = GAME.funcs.getPrevMode();
    for (i = 0; STGMCARD_prevModes[i].mode != 0; i++) {
        if (STGMCARD_prevModes[i].mode == prev) {
            screen->area = STGMCARD_prevModes[i].area;
        }
    }
    screen->place = STGMCARD_places[mode];
    SOUND.loadBank(0x20);
    return screen;
}

/* Loads the screen's images, gives the memory card's save file its title and icon,
   allocates the buffers of a save and of the card's info section, and requests the
   strings */
void STGMCARD_loadFiles(void) {
    TimLoader loader;
    TextTools conv;
    char title[0x48];
    void *frames[3];
    CardClut *clut;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(FILE_GMCARD_SPRITES << 16));
    HEAP.zero(title, 0x41);
    initTextTools(&conv);
    conv.convert(title, conv.getString(FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x27), 0);
    clut = STGMCARD_saveIcon.clut;
    frames[0] = STGMCARD_saveIcon.frames[0];
    frames[1] = STGMCARD_saveIcon.frames[1];
    frames[2] = STGMCARD_saveIcon.frames[2];
    MEMCARD_FUNCS.setHeader(title, clut, 3, frames);
    if (STGMCARD_funcs.dataBuf != NULL) {
        HEAP.free(STGMCARD_funcs.dataBuf);
    }
    STGMCARD_funcs.dataSize = 0x2780;
    STGMCARD_funcs.dataBuf = HEAP.alloc(0x2780, 2);
    if (STGMCARD_funcs.infoBuf != NULL) {
        HEAP.free(STGMCARD_funcs.infoBuf);
    }
    STGMCARD_funcs.infoSize = 0x180;
    STGMCARD_funcs.infoBuf = HEAP.alloc(0x180, 2);
    FILE_CACHE.request(TEXT_FILE(TEXT_MEMORY_CARD));
    FILE_CACHE.request(TEXT_FILE(TEXT_AREA_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_SHOP_NAMES));
}

/* Whether the screen's strings are still loading */
s32 STGMCARD_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_MEMORY_CARD)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_AREA_NAMES)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_SHOP_NAMES)) != 0;
}

/* Frees the buffers of a save and of the card's info section, when leaving the screen */
void STGMCARD_freeBuffers(void) {
    if (STGMCARD_funcs.dataBuf != NULL) {
        HEAP.free(STGMCARD_funcs.dataBuf);
    }
    if (STGMCARD_funcs.infoBuf != NULL) {
        HEAP.free(STGMCARD_funcs.infoBuf);
    }
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
