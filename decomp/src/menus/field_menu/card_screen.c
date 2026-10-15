/* The first object of STSTATUS.PRO, the screen with the party's pages of
   STSTATUS_createCardScreen and the fader. STSTATUS.PRO was ten objects, about one per
   screen: each one's jump tables are aligned to 8 from the start of its own
   rodata, and three of them start 4 bytes past a multiple of 8. Where each
   object's code starts is only known to be between the function with the
   last jump table of the object before and the one with its first (or the
   screen's create function). The data follows the objects' order: each
   object's tables are with it, and the ones they share with the last. */

#include "menus/field_menu.h"

#define SCENE_TASK Task
#define SCENE_CHILD FieldMenuScreen
#define SCENE_CREATE OVL_NAME(createMenu)
#define SCENE_PRIM_BUFFERS 0x14000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

/* Creates the windows of a screen with the party's pages */
void STSTATUS_createCardWindows(PartyScreen *screen, PartyScreenWindows *windows) {
    s32 i;
    s32 j;
    WindowPos *pos;
    TextWindow **children;

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
    children = screen->children;
    for (i = 0; i < screen->childCount - 2; i++, children++) {
        (*children)->setDepth(*children, screen->depth - 1);
    }
}

/* Shows or hides a party member's page: name and five stats (as showPartnerPage) */
void STSTATUS_showCardPage(PartyScreen *screen, PartyScreenWindows *windows, s32 member, s32 show) {
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
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats[i]]);
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

/* Shows or hides the help and the two options with their cursor */
void STSTATUS_showCardChoices(PartyScreen *screen, PartyScreenWindows *windows, s32 show) {
    s32 i;

    if (show) {
        windows->choiceTitle->setString(windows->choiceTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0xB);
        for (i = 0; i < 2; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), i + 0x4A);
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

/* Draws the party's portraits and their pages' panels */
void STSTATUS_drawCardScreen(PartyScreen *screen) {
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
static inline void STSTATUS_startCardScreen(PartyScreen *screen) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
    if (screen->count == 1) {
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
    }
    screen->substate = screen->count;
}

/* One member: once the page and the panels are in, shows the page, the help,
   the hint and the options */
static inline void STSTATUS_openCardScreenAlone(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showCardPage(screen, windows, 0, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x4C);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showCardChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Two members: once the first page is in, shows it and starts the second
   page and the help's panel */
static inline void STSTATUS_openFirstOfTwoCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showCardPage(screen, windows, 0, 1);
        screen->substate = 4;
    }
}

/* Two members: once the second page and the panels are in, shows the page,
   the help, the hint and the options */
static inline void STSTATUS_openSecondOfTwoCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showCardPage(screen, windows, 1, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x4C);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showCardChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Three members: once the first page is in, shows it and starts the second */
static inline void STSTATUS_openFirstOfThreeCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_showCardPage(screen, windows, 0, 1);
        screen->substate = 5;
    }
}

/* Three members: once the second page is in, shows it and starts the third
   and the help's panel */
static inline void STSTATUS_openSecondOfThreeCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showCardPage(screen, windows, 1, 1);
        screen->substate++;
    }
}

/* Three members: once the third page and the panels are in, shows the page,
   the help, the hint and the options */
static inline void STSTATUS_openThirdCardPage(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showCardPage(screen, windows, 2, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x4C);
        windows->hint->setString(windows->hint, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showCardChoices(screen, windows, 1);
        screen->substate = 10;
    }
}

/* Choosing an option: up and down move the cursor, cross picks the card
   album or the deck editor and fades the screen out, triangle closes it */
static inline void STSTATUS_chooseCardOption(PartyScreen *screen, PartyScreenWindows *windows) {
    s32 choice;

    choice = screen->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->choice = 1;
    }
    if (choice != screen->choice) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, 0xB8, screen->choice * 14 + 0x3A);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        screen->setSubstate(screen, 100);
        if (screen->choice == 0) {
            screen->step = 1;
        } else {
            screen->step = 2;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->setSubstate(screen, 0x32);
    }
}

/* Creates the fader that hides the screen */
static inline void STSTATUS_createCardFader(PartyScreen *screen, PartyScreenWindows *windows) {
    if (windows->fader == NULL) {
        windows->fader = STSTATUS_createFader();
    }
    screen->substate++;
}

/* Starts closing the screen: the last page, the panels, the help, the hint
   and the options */
static inline void STSTATUS_closeCardScreen(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[screen->count - 1], 0);
    STSTATUS_showCardPage(screen, windows, screen->count - 1, 0);
    STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
    windows->help->setVisible(windows->help, 0);
    windows->hint->setVisible(windows->hint, 0);
    STSTATUS_data.funcs.startFade(&screen->fade, 0);
    STSTATUS_showCardChoices(screen, windows, 0);
    screen->substate = screen->count + 0x32;
}

/* One member: once the page and the panels are closed, goes on to the end */
static inline void STSTATUS_closeCardScreenAlone(PartyScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        screen->substate = 0x39;
    }
}

/* Two members: once the second page and the panels are closed, starts
   closing the first */
static inline void STSTATUS_closeSecondOfTwoCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showCardPage(screen, windows, 0, 0);
        screen->substate = 0x36;
    }
}

/* Three members: once the third page and the panels are closed, starts
   closing the second */
static inline void STSTATUS_closeThirdCardPage(PartyScreen *screen, PartyScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    STSTATUS_data.funcs.updateFade(&screen->fade);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showCardPage(screen, windows, 1, 0);
        screen->substate = 0x37;
    }
}

/* Three members: once the second page is closed, starts closing the first */
static inline void STSTATUS_closeSecondOfThreeCardPages(PartyScreen *screen, PartyScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showCardPage(screen, windows, 0, 0);
        screen->substate++;
    }
}

/* The end: goes to the chosen card album or deck editor, or ends the screen */
static inline void STSTATUS_endCardScreen(PartyScreen *screen) {
    if (screen->step == 1) {
        GAME.funcs.requestMode(MODE_CARD_ALBUM, 0);
    } else if (screen->step == 2) {
        GAME.funcs.requestMode(MODE_DECK_EDITOR, 0);
    } else {
        screen->state = 3;
    }
}

/* Runs the screen: the pages come in one after the other, the cursor picks
   one of the two options, and the pages go out in reverse */
void STSTATUS_runCardScreen(PartyScreen *screen, PartyScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_startCardScreen(screen);
        break;
    case 1:
        STSTATUS_openCardScreenAlone(screen, windows);
        break;
    case 2:
        STSTATUS_openFirstOfTwoCardPages(screen, windows);
        break;
    case 4:
        STSTATUS_openSecondOfTwoCardPages(screen, windows);
        break;
    case 3:
        STSTATUS_openFirstOfThreeCardPages(screen, windows);
        break;
    case 5:
        STSTATUS_openSecondOfThreeCardPages(screen, windows);
        break;
    case 6:
        STSTATUS_openThirdCardPage(screen, windows);
        break;
    case 10:
        STSTATUS_chooseCardOption(screen, windows);
        break;
    case 100:
        STSTATUS_createCardFader(screen, windows);
        break;
    case 101:
        windows->fader->start(windows->fader, 0, 10);
        screen->substate++;
        break;
    case 102:
        if (windows->fader->state == 2) {
            screen->substate = 0x39;
        }
        break;
    case 0x32:
        STSTATUS_closeCardScreen(screen, windows);
        break;
    case 0x33:
        STSTATUS_closeCardScreenAlone(screen);
        break;
    case 0x34:
        STSTATUS_closeSecondOfTwoCardPages(screen, windows);
        break;
    case 0x35:
        STSTATUS_closeThirdCardPage(screen, windows);
        break;
    case 0x37:
        STSTATUS_closeSecondOfThreeCardPages(screen, windows);
        break;
    case 0x36:
    case 0x38:
        if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
            screen->substate = 0x39;
        }
        break;
    case 0x39:
        STSTATUS_endCardScreen(screen);
        break;
    }
}

/* The screen's update: counts the party and opens the pages */
void STSTATUS_updateCardScreen(PartyScreen *screen, PartyScreenWindows *windows) {
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
        STSTATUS_createCardWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runCardScreen(screen, windows);
        STSTATUS_drawCardScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the screen with the party's pages (task) whose two options go to the
   card album (MODE_CARD_ALBUM) or the deck editor (MODE_DECK_EDITOR) */
Task *STSTATUS_createCardScreen(FieldMenuScreen *menu, s32 extra) {
    PartyScreen *screen = createTask(STSTATUS_updateCardScreen, sizeof(PartyScreen), sizeof(PartyScreenWindows));

    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->menu = menu;
    return (Task *)screen;
}

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 0
#include "shared/create_fader.inc.c"

/* The stats a party member's page shows, in PartnerTotals.stats (as
   STSTATUS_pageStats2, 4, 0, 8 and 9 on the other screens) */
s32 STSTATUS_pageStats[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
