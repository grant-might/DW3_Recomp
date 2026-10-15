/* The second object of STSTATUS.PRO (see card_screen.c), the second screen
   with the party's pages (STSTATUS_createDemoScreen): its rodata starts at 0x800825E4
   (USA). */

#include "menus/field_menu.h"

/* As STSTATUS_createCardWindows, without moving the children above the screen */
void STSTATUS_createDemoWindows(PartyScreen *screen, PartyScreenWindows *windows) {
    s32 i;
    s32 j;
    WindowPos *pos;

    for (j = 0; j < 3; j++) {
        pos = &STSTATUS_data.layout[0];
        windows->pages[j].name = createTextWindow(screen->layer, 1, pos->x, pos->y + j * 46);
        pos = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].labels[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
        pos = &STSTATUS_data.layout[6];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].values[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
    }
    pos = &STSTATUS_data.layout[12];
    windows->help = createTextWindow(screen->layer, 1, pos->x, pos->y);
    windows->help->setLines(windows->help, 2);
    windows->hint = createTextWindow(screen->layer, 1, pos->x, pos->y + 14);
    windows->choiceTitle = createTextWindow(screen->layer, 1, 0xB2, 0x2A);
    for (i = 0; i < 2; i++) {
        windows->options[i] = createTextWindow(screen->layer, 1, 0xC5, 0x3A + i * 14);
    }
    windows->cursor = createCursor(screen->layer, screen->depth - 1, 0xB8, 0x3A);
    windows->cursor->setVisible(windows->cursor, 0);
}

/* As STSTATUS_showCardPage */
void STSTATUS_showDemoPage(PartyScreen *screen, PartyScreenWindows *windows, s32 member, s32 show) {
    PartnerTotals stats;
    WindowPos *layout;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(member);
        GAME.funcs.computeStats(id, &stats);
        windows->pages[member].name->setString(windows->pages[member].name, GAME.funcs.getPartnerStats(id), -1);
        layout = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, layout++) {
            windows->pages[member].labels[i]->setString(windows->pages[member].labels[i],
                                                        FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), layout->string);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats2[i]]);
            windows->pages[member].values[i]->setRightAlign(windows->pages[member].values[i], 1);
        }
    } else {
        windows->pages[member].name->setVisible(windows->pages[member].name, 0);
        for (i = 0; i < 5; i++) {
            windows->pages[member].labels[i]->setVisible(windows->pages[member].labels[i], 0);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setVisible(windows->pages[member].values[i], 0);
        }
    }
}

/* As STSTATUS_showCardChoices, for the second screen: the options are GAME.digivolveDemo's two
   values, the current one highlighted */
void STSTATUS_showDemoChoices(PartyScreen *screen, PartyScreenWindows *windows, s32 show) {
    s32 i;

    if (show) {
        windows->choiceTitle->setString(windows->choiceTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x46);
        for (i = 0; i < 2; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), i + 0x47);
        }
        screen->choice = 1 - GAME.digivolveDemo;
        windows->cursor->setPos(windows->cursor, 0xB8, screen->choice * 14 + 0x3A);
        if (screen->choice == 0) {
            windows->options[0]->setPalette(windows->options[0], PALETTE_BLUE);
            windows->options[1]->setPalette(windows->options[1], PALETTE_WHITE);
        } else {
            windows->options[0]->setPalette(windows->options[0], PALETTE_WHITE);
            windows->options[1]->setPalette(windows->options[1], PALETTE_BLUE);
        }
        windows->cursor->setVisible(windows->cursor, 1);
    } else {
        windows->choiceTitle->setVisible(windows->choiceTitle, 0);
        for (i = 0; i < 2; i++) {
            windows->options[i]->setVisible(windows->options[i], 0);
        }
        windows->cursor->setVisible(windows->cursor, 0);
    }
}

/* As STSTATUS_drawCardScreen, for the second screen */
void STSTATUS_drawDemoScreen(PartyScreen *screen) {
    SpriteDrawer sprite;
    s32 member;
    s32 level;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    if (GFX.funcs.getTime() - screen->frameTime >= 13) {
        screen->frameTime = GFX.funcs.getTime();
        for (i = 0; i < screen->count; i++) {
            member = GAME.funcs.getPartyMember(i);
            screen->frames[i]++;
            if (STSTATUS_data.partnerAnims[member].frames[screen->frames[i]] == -1 || screen->frames[i] >= 7) {
                screen->frames[i] = 0;
            }
        }
    }
    for (i = 0; i < screen->count; i++) {
        level = screen->pageFades[i].level;
        if (level != 0) {
            if (level != ONE) {
                sprite.setScale(level, level, ONE);
                sprite.setPivot(0x7C, i * 46 + 0x27);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            member = GAME.funcs.getPartyMember(i);
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16),
                        STSTATUS_data.partnerAnims[member].frames[screen->frames[i]], 0x6B, i * 46 + 0x13);
        }
    }
    sprite.setTexture(0x140, 0);
    for (i = 0; i < screen->count; i++) {
        level = screen->pageFades[i].level;
        if (level != 0) {
            /* both branches draw the frame's last part: the match depends on it */
            if (level != ONE) {
                sprite.setScale(level, ONE, ONE);
                sprite.setPivot(0, i * 46 + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 46 + 0x11);
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 46 + 0x27);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 46 + 0x13);
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 46 + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 46 + 0x11);
            } else {
                sprite.setScale(ONE, ONE, ONE);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 46 + 0x11);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 46 + 0x13);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 46 + 0x11);
            }
        }
    }
    level = screen->fades[1].level;
    if (level != 0) {
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    if (screen->fade.level != 0) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth);
        sprite.setTexture(0x280, 0x100);
        level = screen->fade.level;
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0x140, 0x40);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x3A, 0xA8, 0x28);
    }
}

/* Starts the fade of the first page (and, with one party member, of the
   panels); the next substate is the party's size */
static inline void STSTATUS_startDemoScreen(PartyScreen *screen) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
    if (screen->count == 1) {
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
    }
    screen->substate = screen->count;
}

/* One member: once the page and the panels are in, shows the page, the help,
   the hint and the options */
static inline void STSTATUS_openDemoScreenAlone(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showDemoPage(screen, windows, 0, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x49);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showDemoChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Two members: once the first page is in, shows it and starts the second
   page and the help's panel */
static inline void STSTATUS_openFirstOfTwoDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showDemoPage(screen, windows, 0, 1);
        screen->substate = 4;
    }
}

/* Two members: once the second page and the panels are in, shows the page,
   the help, the hint and the options */
static inline void STSTATUS_openSecondOfTwoDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showDemoPage(screen, windows, 1, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x49);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showDemoChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Three members: once the first page is in, shows it and starts the second */
static inline void STSTATUS_openFirstOfThreeDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_showDemoPage(screen, windows, 0, 1);
        screen->substate = 5;
    }
}

/* Three members: once the second page is in, shows it and starts the third
   and the help's panel */
static inline void STSTATUS_openSecondOfThreeDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showDemoPage(screen, windows, 1, 1);
        screen->substate++;
    }
}

/* Three members: once the third page and the panels are in, shows the page,
   the help, the hint and the options */
static inline void STSTATUS_openThirdDemoPage(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showDemoPage(screen, windows, 2, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x49);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showDemoChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Choosing an option: up and down move the cursor, cross turns the
   digivolution demo on or off, both cross and triangle close the screen */
static inline void STSTATUS_chooseDemoOption(PartyScreen *screen, PartyScreenWindows *windows) {
    s32 choice;

    choice = screen->choice;
    if (PAD_PRESSED(PAD_UP)) {
        screen->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        screen->choice = 1;
    }
    if (choice != screen->choice) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, 0xB8, screen->choice * 14 + 0x3A);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (screen->choice == 0) {
            GAME.digivolveDemo = 1;
        } else {
            GAME.digivolveDemo = 0;
        }
        screen->substate = 0x32;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->substate = 0x32;
    }
}

/* Starts closing the screen: the last page, the panels, the help, the hint
   and the options */
static inline void STSTATUS_closeDemoScreen(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[screen->count - 1], 0);
    STSTATUS_showDemoPage(screen, windows, screen->count - 1, 0);
    STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
    windows->help->setVisible(windows->help, 0);
    windows->hint->setVisible(windows->hint, 0);
    STSTATUS_data.funcs.startFade(&screen->fade, 0);
    STSTATUS_showDemoChoices(screen, windows, 0);
    screen->substate = screen->count + 0x32;
}

/* One member: once the page and the panels are closed, goes on to the end */
static inline void STSTATUS_closeDemoScreenAlone(PartyScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        screen->substate = 0x39;
    }
}

/* Two members: once the second page and the panels are closed, starts
   closing the first */
static inline void STSTATUS_closeSecondOfTwoDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showDemoPage(screen, windows, 0, 0);
        screen->substate = 0x36;
    }
}

/* Three members: once the third page and the panels are closed, starts
   closing the second */
static inline void STSTATUS_closeThirdDemoPage(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showDemoPage(screen, windows, 1, 0);
        screen->substate = 0x37;
    }
}

/* Three members: once the second page is closed, starts closing the first */
static inline void STSTATUS_closeSecondOfThreeDemoPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showDemoPage(screen, windows, 0, 0);
        screen->substate++;
    }
}

/* As STSTATUS_runCardScreen, for the second screen: the choice sets GAME.digivolveDemo */
void STSTATUS_runDemoScreen(PartyScreen *screen, PartyScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_startDemoScreen(screen);
        break;
    case 1:
        STSTATUS_openDemoScreenAlone(screen, windows);
        break;
    case 2:
        STSTATUS_openFirstOfTwoDemoPages(screen, windows);
        break;
    case 4:
        STSTATUS_openSecondOfTwoDemoPages(screen, windows);
        break;
    case 3:
        STSTATUS_openFirstOfThreeDemoPages(screen, windows);
        break;
    case 5:
        STSTATUS_openSecondOfThreeDemoPages(screen, windows);
        break;
    case 6:
        STSTATUS_openThirdDemoPage(screen, windows);
        break;
    case 10:
        STSTATUS_chooseDemoOption(screen, windows);
        break;
    case 0x32:
        STSTATUS_closeDemoScreen(screen, windows);
        break;
    case 0x33:
        STSTATUS_closeDemoScreenAlone(screen);
        break;
    case 0x34:
        STSTATUS_closeSecondOfTwoDemoPages(screen, windows);
        break;
    case 0x35:
        STSTATUS_closeThirdDemoPage(screen, windows);
        break;
    case 0x37:
        STSTATUS_closeSecondOfThreeDemoPages(screen, windows);
        break;
    case 0x36:
    case 0x38:
        if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
            screen->substate = 0x39;
        }
        break;
    case 0x39:
        screen->state = 3;
        break;
    }
}

/* As STSTATUS_updateCardScreen, for the second screen */
void STSTATUS_updateDemoScreen(PartyScreen *screen, PartyScreenWindows *windows) {
    s32 i;

    switch (screen->state) {
    case TASK_INIT:
    default:
        screen->nextState(screen);
        for (i = 0; i < 3; i++) {
            if (GAME.funcs.getPartyMember(i) >= 0) {
                screen->count++;
            }
        }
        for (i = 0; i < screen->count; i++) {
            screen->pageFades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->pageFades[i], 1);
        }
        for (i = 0; i < 2; i++) {
            screen->fades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->fades[i], 1);
        }
        screen->fade.duration = 10;
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
        STSTATUS_createDemoWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runDemoScreen(screen, windows);
        STSTATUS_drawDemoScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the second screen with the party's pages (task), whose two options
   set GAME.digivolveDemo */
Task *STSTATUS_createDemoScreen(FieldMenuScreen *menu, s32 extra) {
    /* its windows are PartyScreenWindows without the fader */
    PartyScreen *screen = createTask(STSTATUS_updateDemoScreen, sizeof(PartyScreen), 0x9C);

    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->menu = menu;
    return (Task *)screen;
}

s32 STSTATUS_pageStats2[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
