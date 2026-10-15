/* The ninth object of STSTATUS.PRO (see card_screen.c), the screen of
   STSTATUS_createSortScreen: its rodata starts at 0x80082C88 (USA). */

#include "menus/field_menu.h"

/* Creates the party order screen's windows: the title, the help and the
   party's pages */
void STSTATUS_createSortWindows(SortScreen *screen, SortScreenWindows *windows) {
    s32 i;
    s32 j;
    WindowPos *pos;

    pos = &STSTATUS_data.layout[11];
    windows->title = createTextWindow(screen->layer, 1, pos->x, pos->y);
    pos = &STSTATUS_data.layout[12];
    windows->help = createTextWindow(screen->layer, 1, pos->x, pos->y);
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
}

/* As STSTATUS_showCardPage */
void STSTATUS_showSortPage(SortScreen *screen, SortScreenWindows *windows, s32 member, s32 show) {
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
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats9[i]]);
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

/* Moves the member cursor's palette on every 8 frames */
static inline void STSTATUS_stepSortCursor(SortScreen *screen) {
    if (GFX.funcs.getTime() - screen->cursorTime >= 8) {
        screen->cursorTime = GFX.funcs.getTime();
        screen->cursorFrame++;
        if (screen->cursorFrame >= 8) {
            screen->cursorFrame = 0;
        }
    }
}

/* Moves the partners' portraits to their next frames every 13 frames */
static inline void STSTATUS_animateSortPortraits(SortScreen *screen) {
    s32 id;
    s32 i;

    if (GFX.funcs.getTime() - screen->frameTime >= 13) {
        screen->frameTime = GFX.funcs.getTime();
        for (i = 0; i < screen->count; i++) {
            id = GAME.funcs.getPartyMember(i);
            screen->frames[i]++;
            if (STSTATUS_data.partnerAnims[id].frames[screen->frames[i]] == -1 || screen->frames[i] >= 7) {
                screen->frames[i] = 0;
            }
        }
    }
}

/* Moves the swap arrow's palette on every 11 frames */
static inline void STSTATUS_stepSortArrow(SortScreen *screen) {
    if (GFX.funcs.getTime() - screen->arrowTime >= 11) {
        screen->arrowTime = GFX.funcs.getTime();
        screen->arrowFrame++;
        if (screen->arrowFrame >= 8) {
            screen->arrowFrame = 0;
        }
    }
}

/* Draws the cursors, the partners' portraits and frames, the two being
   swapped squashed by the fade, and the swap's arrow */
void STSTATUS_drawSortScreen(SortScreen *screen) {
    SpriteDrawer sprite;
    s32 id;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    STSTATUS_stepSortCursor(screen);
    sprite.setTexture(0x280, 0x100);
    if (screen->cursorShown) {
        if (screen->substate == 6) {
            sprite.setClutRow(screen->cursorFrame);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->cursor * 0x2E + 0x11);
        } else if (screen->substate == 7) {
            sprite.setClutRow(screen->cursorFrame);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->second * 0x2E + 0x11);
            sprite.setClutRow(8);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->first * 0x2E + 0x11);
        } else if (screen->substate == 8) {
            sprite.setScale(ONE, screen->fade.level, ONE);
            sprite.setPivot(0, screen->first * 0x2E + 0x25);
            sprite.setClutRow(8);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->first * 0x2E + 0x11);
            sprite.setPivot(0, screen->second * 0x2E + 0x25);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->second * 0x2E + 0x11);
        }
        sprite.setClutRow(0);
    }
    STSTATUS_animateSortPortraits(screen);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
            } else if ((screen->substate == 8 || screen->substate == 9) && (i == screen->first || i == screen->second)) {
                sprite.setScale(ONE, screen->fade.level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            id = GAME.funcs.getPartyMember(i);
            /* the match depends on (*&...): FILE_CACHE.getEntry would keep the
               cache's address in a register through the loop */
            sprite.draw((*&FILE_CACHE.getEntry)(FILE_STATUS_SPRITES << 16),
                        STSTATUS_data.partnerAnims[id].frames[screen->frames[i]], 0x6B, i * 0x2E + 0x13);
        }
    }
    sprite.setTexture(0x140, 0);
    /* the match depends on the three loops sharing i and on the frame's
       plain draws written in both elses (merged after reload) */
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            } else if (screen->substate == 8 || screen->substate == 9) {
                if (i == screen->first || i == screen->second) {
                    sprite.setScale(ONE, screen->fade.level, ONE);
                    sprite.setPivot(0, i * 0x2E + 0x25);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                    sprite.setPivot(0x7C, i * 0x2E + 0x27);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                    sprite.setPivot(0, i * 0x2E + 0x25);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
                } else {
                    sprite.setScale(ONE, ONE, ONE);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
                }
            } else {
                sprite.setScale(ONE, ONE, ONE);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            }
        }
    }
    if (screen->fades[0].level != 0) {
        if (screen->fades[0].level != ONE) {
            sprite.setScale(screen->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x19);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
    if (screen->fades[1].level != 0) {
        if (screen->fades[1].level != ONE) {
            sprite.setScale(screen->fades[1].level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    if (screen->arrowShown) {
        STSTATUS_stepSortArrow(screen);
        sprite.setLayerId(screen->layer, 0);
        sprite.setClutRow(screen->arrowFrame);
        switch (screen->first) {
        case 0:
        default:
            if (screen->second == 1) {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x29, 0x93, 0x22);
            } else {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x28, 0x93, 0x22);
            }
            break;
        case 1:
            if (screen->second == 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x29, 0x93, 0x22);
            } else {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x29, 0x93, 0x4F);
            }
            break;
        case 2:
            if (screen->second == 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x28, 0x93, 0x22);
            } else {
                sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x29, 0x93, 0x4F);
            }
            break;
        }
    }
}

/* Once the first page and its panel are in, shows the page and, with one
   member, the help, else the title and starts the second page */
static inline void STSTATUS_openFirstSortPage(SortScreen *screen, SortScreenWindows *windows) {
    if (screen->count == 1) {
        STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    } else {
        STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    }
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        STSTATUS_showSortPage(screen, windows, 0, 1);
        if (screen->count == 1) {
            screen->substate = 15;
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x10);
        } else {
            screen->substate++;
            windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x11);
            STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        }
    }
}

/* Once the second page is in, shows it and, with two members, the help, else
   starts the third page */
static inline void STSTATUS_openSecondSortPage(SortScreen *screen, SortScreenWindows *windows) {
    if (screen->count == 2) {
        STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    }
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_showSortPage(screen, windows, 1, 1);
        if (screen->count == 2) {
            screen->substate = 5;
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0xF);
        } else {
            screen->substate++;
            STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        }
    }
}

/* Once the third page and the help's panel are in, shows the page and the
   help */
static inline void STSTATUS_openThirdSortPage(SortScreen *screen, SortScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showSortPage(screen, windows, 2, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0xF);
        screen->substate = 5;
    }
}

/* Choosing the first member to swap: up and down move the cursor (wrapping),
   cross picks the member and moves the arrow to the next one, triangle closes
   the screen */
static inline void STSTATUS_chooseFirstToSwap(SortScreen *screen, SortScreenWindows *windows) {
    s32 old;

    old = screen->cursor;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->cursor--;
        if (screen->cursor < 0) {
            screen->cursor = screen->count - 1;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->cursor++;
        if (screen->cursor > screen->count - 1) {
            screen->cursor = 0;
        }
    }
    if (old != screen->cursor) {
        SOUND.playSound(SOUND_MENU_MOVE);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->arrowShown = 1;
        screen->substate++;
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x12);
        screen->first = screen->second = screen->cursor;
        do {
            screen->second++;
            if (screen->second > screen->count - 1) {
                screen->second = 0;
            }
        } while (screen->first == screen->second);
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->cursorShown = 0;
        screen->substate = 0x14;
    }
}

/* Choosing whom to swap the first member with: up and down move the arrow
   (wrapping, past the first), cross hides both pages, triangle goes back */
static inline void STSTATUS_chooseSecondToSwap(SortScreen *screen, SortScreenWindows *windows) {
    s32 old;

    old = screen->second;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        do {
            screen->second--;
            if (screen->second < 0) {
                screen->second = screen->count - 1;
            }
        } while (screen->first == screen->second);
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        do {
            screen->second++;
            if (screen->second > screen->count - 1) {
                screen->second = 0;
            }
        } while (screen->first == screen->second);
    }
    if (old != screen->second) {
        SOUND.playSound(SOUND_MENU_MOVE);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        STSTATUS_showSortPage(screen, windows, screen->first, 0);
        STSTATUS_showSortPage(screen, windows, screen->second, 0);
        STSTATUS_data.funcs.startFade(&screen->fade, 0);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x11);
        screen->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->arrowShown = 0;
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x11);
        screen->substate--;
    }
}

/* Once both pages are hidden, swaps the two members in the party and starts
   their portraits again */
static inline void STSTATUS_swapMembers(SortScreen *screen) {
    s32 member;

    if (STSTATUS_data.funcs.updateFade(&screen->fade)) {
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
        screen->arrowShown = 0;
        member = GAME.funcs.getPartyMember(screen->first);
        GAME.party[screen->first] = GAME.funcs.getPartyMember(screen->second);
        GAME.party[screen->second] = member;
        screen->frames[screen->first] = 0;
        screen->frames[screen->second] = 0;
        screen->substate++;
    }
}

/* Once the fade is back, shows both pages again and goes back to choosing */
static inline void STSTATUS_showSwappedPages(SortScreen *screen, SortScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->fade)) {
        STSTATUS_showSortPage(screen, windows, screen->first, 1);
        STSTATUS_showSortPage(screen, windows, screen->second, 1);
        screen->substate = 6;
    }
}

/* Starts closing the screen: the help, the last page and its panel */
static inline void STSTATUS_closeSortScreen(SortScreen *screen, SortScreenWindows *windows) {
    switch (screen->count) {
    case 1:
    default:
        windows->help->setVisible(windows->help, 0);
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showSortPage(screen, windows, 0, 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        break;
    case 2:
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        windows->help->setVisible(windows->help, 0);
        STSTATUS_showSortPage(screen, windows, 1, 0);
        break;
    case 3:
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        windows->help->setVisible(windows->help, 0);
        STSTATUS_showSortPage(screen, windows, 2, 0);
        break;
    }
    screen->substate = screen->count + 0x14;
}

/* One member: ends the screen once the page is closed */
static inline void STSTATUS_closeSortScreenAlone(SortScreen *screen) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        screen->state = 3;
    }
    STSTATUS_data.funcs.updateFade(&screen->fades[1]);
}

/* Two members: once the second page and the help's panel are closed, starts
   closing the first and the title */
static inline void STSTATUS_closeSecondOfTwoSortPages(SortScreen *screen, SortScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_showSortPage(screen, windows, 0, 0);
        screen->substate = 0x18;
    }
}

/* Two or three members: ends the screen once the first page and the title
   are closed */
static inline void STSTATUS_closeFirstSortPage(SortScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        screen->state = 3;
    }
}

/* Three members: once the third page and the help's panel are closed, starts
   closing the second */
static inline void STSTATUS_closeThirdSortPage(SortScreen *screen, SortScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showSortPage(screen, windows, 1, 0);
        screen->substate = 0x19;
    }
}

/* Three members: once the second page is closed, starts closing the first
   and the title */
static inline void STSTATUS_closeSecondOfThreeSortPages(SortScreen *screen, SortScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_showSortPage(screen, windows, 0, 0);
        screen->substate = 0x18;
    }
}

/* The party order screen's steps: the pages fade in, two members are chosen
   and swapped, then the pages fade out */
void STSTATUS_runSortScreen(SortScreen *screen, SortScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_openFirstSortPage(screen, windows);
        break;
    case 1:
        STSTATUS_openSecondSortPage(screen, windows);
        break;
    case 2:
        STSTATUS_openThirdSortPage(screen, windows);
        break;
    case 5:
        screen->cursorShown = 1;
        screen->substate++;
        break;
    case 6:
        STSTATUS_chooseFirstToSwap(screen, windows);
        break;
    case 7:
        STSTATUS_chooseSecondToSwap(screen, windows);
        break;
    case 8:
        STSTATUS_swapMembers(screen);
        break;
    case 9:
        STSTATUS_showSwappedPages(screen, windows);
        break;
    case 15:
        screen->cursorShown = 1;
        screen->substate++;
        break;
    case 16:
        if (PAD_PRESSED(PAD_TRIANGLE)) {
            screen->substate = 0x14;
        }
        break;
    case 0x14:
        STSTATUS_closeSortScreen(screen, windows);
        break;
    case 0x15:
        STSTATUS_closeSortScreenAlone(screen);
        break;
    case 0x16:
        STSTATUS_closeSecondOfTwoSortPages(screen, windows);
        break;
    case 0x18:
        STSTATUS_closeFirstSortPage(screen);
        break;
    case 0x17:
        STSTATUS_closeThirdSortPage(screen, windows);
        break;
    case 0x19:
        STSTATUS_closeSecondOfThreeSortPages(screen, windows);
        break;
    }
}

/* The party order screen's task: counts the party, starts the fades and creates
   the windows, then runs and draws the screen */
void STSTATUS_updateSortScreen(SortScreen *screen, SortScreenWindows *windows) {
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
        }
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
        for (i = 0; i < 2; i++) {
            screen->fades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->fades[i], 1);
            screen->fade.duration = 8;
            STSTATUS_data.funcs.startFade(&screen->fade, 0);
        }
        STSTATUS_createSortWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runSortScreen(screen, windows);
        STSTATUS_drawSortScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the field menu's party order screen (task), where two members swap
   places */
Task *STSTATUS_createSortScreen(FieldMenuScreen *menu, s32 extra) {
    SortScreen *screen = createTask(STSTATUS_updateSortScreen, sizeof(SortScreen), sizeof(SortScreenWindows));

    screen->layer = SCREEN_LAYER;
    screen->depth = 2;
    screen->menu = menu;
    return (Task *)screen;
}

s32 STSTATUS_pageStats9[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
