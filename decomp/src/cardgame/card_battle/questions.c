/* CARDGAME's step windows, the yes/no question and the card info. */

#include "cardgame/card_battle.h"

/* A position of CARDGAME_stepWindowPositions */
s16 CARDGAME_getStepWindowPos(s32 a, s32 b, s32 c) {
#if PAL_VIDEO
    return CARDGAME_stepWindowPositions[SHIFT_PAL_SCREEN][a][b][c];
#else
    return CARDGAME_stepWindowPositions[0][a][b][c];
#endif
}

/* Opens the windows of the step CARDGAME_windowSteps[step] once its time is past; the next step */
s32 CARDGAME_openStepWindows(CardBattle *battle, CardScreen *screen, s32 layout, s32 text, s32 time, s32 step) {
    if (CARDGAME_windowSteps[step].time < time) {
        switch (CARDGAME_windowSteps[step].kind) {
        case 0:
            screen->openWindow(screen, 2, 1, 0, CARDGAME_getStepWindowPos(layout, 0, 0), CARDGAME_getStepWindowPos(layout, 0, 1));
            screen->openWindow(screen, 4, 2, 0, CARDGAME_getStepWindowPos(layout, 1, 0), CARDGAME_getStepWindowPos(layout, 1, 1));
            if (text != 0) {
                screen->openWindow(screen, 0, 0, text, 0, 0x42);
            }
            if (battle->effectStep.id == 0x9A) {
                screen->openWindow(screen, 5, 4, 0x24, 0, 0x14);
            }
            break;
        case 1:
            screen->openWindow(screen, 3, 1, 0, CARDGAME_getStepWindowPos(layout, 2, 0), CARDGAME_getStepWindowPos(layout, 2, 1));
            break;
        case 2:
            screen->openWindow(screen, 1, 3, 0, CARDGAME_getStepWindowPos(layout, 3, 0), CARDGAME_getStepWindowPos(layout, 3, 1));
            break;
        }
        if (step < 3) {
            step++;
        }
    }
    return step;
}

/* Opens the panels and asks message value; the European and Japanese versions
   start on the second choice when none of the player's hand can be played */
void CARDGAME_startQuestion(CardBattle *battle, CardScreen *screen, s32 value) {
#if VERSION_EU || VERSION_JP
    s32 i;
    s32 found;
#endif

    battle->effectStep.choice = 0;
    battle->effectStep.state = 0;
    if (screen->panels[0].state == 0) {
        battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
        battle->anim.next = CARD_ANIM_SHOW_SLOTS;
        screen->openPanels(screen);
    }
    battle->effectStep.time = 0;
    screen->message.message = value;
#if VERSION_US
    screen->message.choice = 0;
    screen->message.place = 0;
#elif VERSION_EU || VERSION_JP
    screen->message.place = 0;
    found = 0;
    for (i = 0; i < battle->sides[0].pile.handCount; i++) {
        if (CARDGAME_canPlayCard(battle, battle->sides[0].pile.points, battle->sides[0].pile.hand[i])) {
            found = 1;
            break;
        }
    }
    if (found) {
        screen->message.choice = 0;
        battle->effectStep.choice = 0;
        battle->effectStep.vars[4] = 2;
    } else {
        screen->message.choice = 1;
        battle->effectStep.choice = 1;
        battle->effectStep.vars[4] = 3;
    }
#endif
}

/* The yes/no window's buttons: cross answers, triangle answers the second
   choice, up and down change the choice, L1 or R1 moves the window and
   circle opens the battle menu */
static inline void readYesNoInput(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_CROSS)) {
        screen->confirmMessage(screen);
        battle->effectStep.state = 7;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        battle->effectStep.choice = 1;
        screen->setMessageChoice(screen, 1);
        screen->confirmMessage(screen);
        battle->effectStep.state = 7;
#if VERSION_US
    } else if (PAD_PRESSED(PAD_UP) || PAD_PRESSED(PAD_DOWN)) {
#elif VERSION_EU || VERSION_JP
    } else if ((PAD_PRESSED(PAD_UP) || PAD_PRESSED(PAD_DOWN)) && battle->effectStep.vars[4] == 2) {
#endif
        SOUND.playSound(SOUND_CURSOR);
        battle->effectStep.choice ^= 1;
        screen->setMessageChoice(screen, battle->effectStep.choice);
    } else if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1))) {
        screen->closeMessage(screen);
        battle->effectStep.state = 6;
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        screen->closeMessage(screen);
        battle->effectStep.time = 0;
        battle->effectStep.state = 3;
    }
}

/* Closes the panels and opens the battle menu once they are closed */
static inline void hideForMenu(CardBattle *battle, CardScreen *screen) {
    switch (battle->effectStep.time) {
    case 0:
        if (screen->message.state == 0) {
            battle->anim.next = CARD_ANIM_HIDE_SLOTS;
            screen->closePanels(screen);
            battle->effectStep.time = 1;
        }
        break;
    case 1:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            battle->run = CARD_RUN_MENU;
            battle->record.menuState = 0;
            battle->effectStep.state = 4;
        }
        break;
    }
}

/* Back from the battle menu: reopens the yes/no window once the panels are open again */
static inline void showAfterMenu(CardBattle *battle, CardScreen *screen) {
    switch (battle->effectStep.time) {
    case 0:
        if (battle->anim.current == CARD_ANIM_NONE && screen->panels[0].state == 2) {
#if VERSION_US
            screen->openMessage(screen, screen->message.message, 1, screen->message.choice, screen->message.place);
#elif VERSION_EU || VERSION_JP
            screen->openMessage(screen, screen->message.message, battle->effectStep.vars[4], screen->message.choice, screen->message.place);
#endif
            battle->effectStep.time = 1;
        }
        break;
    case 1:
        if (screen->message.state == 2) {
            battle->effectStep.state = 2;
        }
        break;
    }
}

/* Runs the yes/no window that CARDGAME_startQuestion sets up: 1 for the first choice, 2 for the second, 0 until then */
s32 CARDGAME_stepYesNo(CardBattle *battle, CardScreen *screen) {
    s32 result = 0;

    switch (battle->effectStep.state) {
    case 0:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 36 && battle->anim.current == CARD_ANIM_NONE) {
#if VERSION_US
            battle->effectStep.state = 1;
            battle->effectStep.time = 0;
            screen->openMessage(screen, screen->message.message, 1, screen->message.choice, screen->message.place);
#elif VERSION_EU || VERSION_JP
            battle->effectStep.time = 0;
            if (battle->record.playCount != 0 && battle->effectStep.vars[4] == 3) {
                battle->effectStep.state = 8;
            } else {
                battle->effectStep.state = 1;
                screen->openMessage(screen, screen->message.message, battle->effectStep.vars[4], screen->message.choice, screen->message.place);
            }
#endif
        }
        break;
    case 2:
        readYesNoInput(battle, screen);
        break;
    case 6:
        if (screen->message.state == 0) {
            screen->message.place ^= 2;
#if VERSION_US
            screen->openMessage(screen, screen->message.message, 1, screen->message.choice, screen->message.place);
#elif VERSION_EU || VERSION_JP
            screen->openMessage(screen, screen->message.message, battle->effectStep.vars[4], screen->message.choice, screen->message.place);
#endif
            battle->effectStep.state = 1;
        }
        break;
    case 7:
        if (screen->message.state == 0) {
            if (battle->effectStep.choice == 0) {
                battle->effectStep.state = 9;
                battle->anim.next = CARD_ANIM_HIDE_SLOTS;
                screen->closePanels(screen);
            } else {
                battle->effectStep.state = 8;
            }
        }
        break;
    case 3:
        hideForMenu(battle, screen);
        break;
    case 4:
        battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
        battle->anim.next = CARD_ANIM_SHOW_SLOTS;
        screen->openPanels(screen);
        battle->effectStep.state = 5;
        battle->effectStep.time = 0;
        break;
    case 5:
        showAfterMenu(battle, screen);
        break;
    case 1:
        if (screen->message.state == 2) {
            battle->effectStep.state = 2;
        }
        break;
    case 9:
        if (screen->panels[0].state != 0 || battle->anim.current != CARD_ANIM_NONE) {
            break;
        }
        /* fallthrough */
    case 8:
        result = 2;
        if (battle->effectStep.choice == 0) {
            result = 1;
        }
        break;
    }
    return result;
}

/* Empties the card information windows 1-4 */
void CARDGAME_clearCardInfo(CardBattle *battle, CardScreen *screen) {
    screen->windows[1].showCount = 0;
    screen->windows[1].value = 0;
    screen->windows[2].value = 0;
    screen->windows[3].value = 0;
    screen->windows[4].value = 0;
}

/* Fills the information windows with the card of sprite offset + effectStep.cursor (the
   highlighted one): its colour, name, picture or values, and kind */
void CARDGAME_showCardInfo(CardBattle *battle, CardScreen *screen, s32 offset) {
    CardDrawer drawer;
    s32 i;
    s32 found;
    s32 card;
    s32 id;

    CARDGAME_clearCardInfo(battle, screen);
    if (screen->sprites[offset + battle->effectStep.cursor].isKind16 != 0) {
        screen->windows[1].value |= screen->sprites[offset + battle->effectStep.cursor].points;
        screen->windows[1].value |= screen->sprites[offset + battle->effectStep.cursor].color << 4;
        screen->windows[1].showCount = 1;
    }
    found = 0;
    if (screen->sprites[offset + battle->effectStep.cursor].visible != 3) {
        card = battle->cards[screen->sprites[offset + battle->effectStep.cursor].index] + 1;
        screen->windows[2].value = card;
        for (i = 0; i < 5; i++) {
            if (CARDGAME_countingCards[i] == battle->cards[screen->sprites[offset + battle->effectStep.cursor].index]) {
                found = 1;
            }
        }
        if (screen->sprites[offset + battle->effectStep.cursor].isKind16 != 0 && found == 0) {
            screen->windows[4].value = 500;
            screen->windows[4].numbers[2] = 1;
            screen->windows[4].numbers[0] = screen->sprites[offset + battle->effectStep.cursor].ap;
            screen->windows[4].numbers[1] = screen->sprites[offset + battle->effectStep.cursor].hp;
        } else {
            screen->windows[4].numbers[2] = 0;
            screen->windows[4].value = card;
        }
        id = battle->cards[screen->sprites[offset + battle->effectStep.cursor].index];
        initCardDrawer(&drawer);
        drawer.setCard(id + 1);
        switch (drawer.card->level) {
        case 0:
            screen->windows[3].value = 0;
            break;
        case 1:
            screen->windows[3].value = 0x25;
            break;
        case 2:
            screen->windows[3].value = 0x26;
            break;
        case 3:
            screen->windows[3].value = 0x27;
            break;
        case 4:
            screen->windows[3].value = 0x28;
            break;
        case 5:
            screen->windows[3].value = 0x29;
            break;
        }
    } else {
        screen->windows[4].numbers[2] = 0;
        screen->windows[4].value = 500;
    }
}

/* CARDGAME_showCardInfo for the highlighted card of a row (kind 0, 1 or 2, as
   CARDGAME_moveTableHighlight) */
void CARDGAME_showTableCardInfo(CardBattle *battle, CardScreen *screen, s32 kind) {
    s32 offset = 0;

    switch (kind) {
    case 1:
        offset = 6;
        break;
    case 2:
        offset = 12;
        break;
    }
    CARDGAME_showCardInfo(battle, screen, offset);
}
