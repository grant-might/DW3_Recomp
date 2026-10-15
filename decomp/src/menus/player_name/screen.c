/* The screen: its pages' tabs, the steps between them and the title; the files
   it loads; and the panels' openings and moves */

#include "menus/player_name.h"

/* Creates the three tabs at the top (the name, the partners, the confirmation), hidden */
void STPLNMET_createTabs(PlayerNameScreen *screen, PlayerNameScreenChildren *children) {
    s32 i;

    for (i = 0; i < 3; i++) {
        children->tabs[i] = createTextWindow(screen->layer, 3, STPLNMET_tabX[i], 0x12);
        children->tabs[i]->setString(children->tabs[i], FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), i + 3);
        children->tabs[i]->setPalette(children->tabs[i], PALETTE_BLUE);
        children->tabs[i]->setVisible(children->tabs[i], 0);
    }
}

/* Shows the tabs, the current page's one highlighted, or hides them */
void STPLNMET_showTabs(PlayerNameScreen *screen, PlayerNameScreenChildren *children, s32 show) {
    s32 i;

    for (i = 0; i < 3; i++) {
        children->tabs[i]->setVisible(children->tabs[i], show);
        if (screen->page == i) {
            children->tabs[i]->setPalette(children->tabs[i], PALETTE_WHITE);
        } else {
            children->tabs[i]->setPalette(children->tabs[i], PALETTE_BLUE);
        }
    }
}

/* The screen's states: starts the sprites and the welcome, then goes through the
   name, the partners and the confirmation (triangle going back); once
   confirmed, saves the name in GAME.name, gives the party chosen and ends */
void STPLNMET_stepScreen(PlayerNameScreen *screen, PlayerNameScreenChildren *children) {
    switch (screen->substate) {
    case 0:
        children->scroll = STPLNMET_createScroll();
        children->scroll->load(children->scroll, 0x1C0, 0x100);
        children->scroll->setLayer(children->scroll, screen->layer, 6);
        children->sparkles = STPLNMET_createSparkles();
        children->shine = STPLNMET_createShine();
        children->dialog = STPLNMET_createWelcome(screen);
        screen->substate++;
        break;
    case 1:
        if (children->dialog == NULL) {
            STPLNMET_funcs.startFade(&screen->fade, 1);
            children->name = STPLNMET_createNameEntry(GAME.name);
            children->partners = STPLNMET_createChoice(screen);
            children->confirm = STPLNMET_createConfirm(screen);
            screen->page = 0;
            screen->nextSubstate(screen);
        }
        break;
    case 2:
        switch (screen->step) {
        case 0:
        default:
            if (STPLNMET_funcs.updateFade(&screen->fade) != 0) {
                STPLNMET_showTabs(screen, children, 1);
            }
            if (children->name->substate == 100) {
                children->name->hide(children->name, 1);
                children->partners->hide(children->partners, 0);
                screen->page = 1;
                STPLNMET_showTabs(screen, children, 1);
                screen->step++;
            }
            break;
        case 1:
            if (children->partners->substate == 100) {
                children->partners->hide(children->partners, 1);
                children->confirm->choice = children->partners->choice;
                children->confirm->name = children->name->name;
                children->confirm->hide(children->confirm, 0);
                screen->page = 2;
                STPLNMET_showTabs(screen, children, 1);
                screen->step++;
            } else if (children->partners->substate == 200) {
                children->partners->hide(children->partners, 1);
                children->name->hide(children->name, 0);
                screen->page = 0;
                STPLNMET_showTabs(screen, children, 1);
                screen->step--;
            }
            break;
        case 2:
            if (children->confirm->substate == 100) {
                children->confirm->close(children->confirm);
                STPLNMET_funcs.startFade(&screen->fade, 0);
                STPLNMET_showTabs(screen, children, 0);
                screen->step++;
            } else if (children->confirm->substate == 200) {
                children->confirm->hide(children->confirm, 1);
                children->partners->hide(children->partners, 0);
                screen->page = 1;
                STPLNMET_showTabs(screen, children, 1);
                screen->step--;
            }
            break;
        case 3:
            if (STPLNMET_funcs.updateFade(&screen->fade) != 0 && children->confirm == NULL) {
                children->name->getName(children->name, GAME.name);
                GAME.funcs.setParty(children->partners->choice);
                screen->state = TASK_KILL;
            }
            break;
        }
        break;
    }
}

/* Draws the page's title, opening and closing with the screen's fade */
void STPLNMET_drawTitle(PlayerNameScreen *screen) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(screen->layer, screen->depth);
    if (screen->fade.level != 0) {
        if (screen->fade.level != ONE) {
            sprite.setScale(screen->fade.level, ONE, ONE);
            sprite.setPivot(0x18, 0x20);
        }
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), screen->page + 12, 0x18, 0x15);
    }
}

/* The screen's task: loads its files and sound bank and creates the tabs, then
   starts its music and runs it; stops the music and returns to the field
   (mode 0x2D8) when it ends */
void STPLNMET_updateScreen(PlayerNameScreen *screen, PlayerNameScreenChildren *children) {
    switch (screen->state) {
    case TASK_INIT:
    default:
        switch (screen->substate) {
        case 0:
        default:
            STPLNMET_funcs.loadFiles();
            SOUND.loadBank(0x20);
            screen->substate++;
            break;
        case 1:
            if (STPLNMET_funcs.filesLoading() == 0) {
                STPLNMET_createTabs(screen, children);
                screen->fade.duration = 10;
                screen->setState(screen, TASK_DONE);
            }
            break;
        }
        break;
    case TASK_RUN:
        STPLNMET_stepScreen(screen, children);
        STPLNMET_drawTitle(screen);
        break;
    case TASK_DONE:
        if (SOUND.isLoading() == 0) {
            SOUND.playSound(MUSIC(0x20, 0));
            screen->setState(screen, TASK_RUN);
        }
        break;
    case TASK_KILL:
        SOUND.stopSound(MUSIC(0x20, 0));
        GAME.funcs.requestMode(0x2D8, 0);
        break;
    }
}

/* Creates the player's name screen (task) */
PlayerNameScreen *STPLNMET_createScreen(void) {
    PlayerNameScreen *screen = createTask(STPLNMET_updateScreen, sizeof(PlayerNameScreen), sizeof(PlayerNameScreenChildren));

    screen->layer = 0x1001;
    screen->depth = 5;
    return screen;
}

/* Loads the screen's sprites into VRAM and requests its texts and the keyboard */
void STPLNMET_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0x100);
    loader.loadArchive(FILE_CACHE.getEntry(FILE_PLNMET_SPRITES << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_ONLINE));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_NAME_ENTRY));
    FILE_CACHE.request(FILE_PLNMET_KEYBOARD);
}

/* Whether the screen's texts or the keyboard are still loading */
s32 STPLNMET_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ONLINE)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_NAME_ENTRY)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(FILE_PLNMET_KEYBOARD) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
