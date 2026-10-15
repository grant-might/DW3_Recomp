/* CARDGAME's picks of cards from a hand or a pile, by the player or the
   computer's flags, then the tally, the step messages and the coin toss
   that picks the first player (one module: cut between them, the tally's
   jump table would lose its alignment). */

#include "cardgame/card_battle.h"

/* Starts a pick from pile's hand: flags (effectStep.eligible) the cards that can be
   played, and opens the player's panel */
void CARDGAME_startHandPick(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 i;

    if (pile->handCount != 0) {
        for (i = 0; i < 40; i++) {
            battle->effectStep.eligible[i] = 0;
            battle->effectStep.marked[i] = 0;
            if (i < pile->handCount) {
                if (CARDGAME_canPlayCard(battle, pile->points, pile->hand[i])) {
                    battle->effectStep.eligible[i] = 1;
                    battle->anim.dimmed[i] = 0;
                } else {
                    battle->anim.dimmed[i] = 1;
                }
            }
        }
    } else {
        battle->anim.dimmed[0] = 0;
    }
    screen->resetPanels(screen);
    if (pile->side == 0) {
        battle->anim.next = CARD_ANIM_SHOW_HAND;
        screen->openPanel(screen, pile->side);
    } else {
        battle->anim.next = CARD_ANIM_SHOW_OPPONENT_HAND;
    }
    battle->effectStep.nextState = 1;
    CARDGAME_clearCardInfo(battle, screen);
    battle->effectStep.cursor = 0;
    battle->effectStep.choice = -1;
}

/* Starts a pick of the cards (all when all > 0) in mode (anim.next) */
void CARDGAME_startPick(CardBattle *battle, CardScreen *screen, s32 all, s32 mode) {
    s32 i;

    if (all > 0) {
        for (i = 0; i < 40; i++) {
            battle->anim.dimmed[i] = 0;
        }
    } else {
        battle->anim.dimmed[0] = 0;
    }
    CARDGAME_clearCardInfo(battle, screen);
    battle->effectStep.nextState = 1;
    battle->anim.next = mode;
    battle->effectStep.cursor = 0;
    battle->effectStep.choice = -1;
}

/* Moves the selection by STEP among COUNT cards: the selected card is raised */
void CARDGAME_moveSelection(CardBattle *battle, CardScreen *screen, s32 count, s32 step) {
    SOUND.playSound(SOUND_MENU_MOVE);
    screen->startSlide(screen, battle->effectStep.cursor, 5, screen->getHandOffset(count, battle->effectStep.cursor) + 0x1800, 0x6100);
    screen->sprites[battle->effectStep.cursor].highlight &= ~1;
    screen->sprites[battle->effectStep.cursor].moving = 0;
    battle->effectStep.cursor += step;
    screen->startSlide(screen, battle->effectStep.cursor, 1, screen->getHandOffset(count, battle->effectStep.cursor) + 0x1800, 0x5C00);
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
}

/* Moves the selection by STEP among the cards of PILE's hand: the selected card is raised */
void CARDGAME_movePileSelection(CardBattle *battle, CardScreen *screen, CardPile *pile, s32 step) {
    SOUND.playSound(SOUND_MENU_MOVE);
    screen->startSlide(screen, battle->effectStep.cursor, 5, screen->getHandOffset(pile->handCount, battle->effectStep.cursor) + 0x1800, 0x6100);
    screen->sprites[battle->effectStep.cursor].highlight &= ~1;
    screen->sprites[battle->effectStep.cursor].moving = 0;
    battle->effectStep.cursor += step;
    screen->startSlide(screen, battle->effectStep.cursor, 1, screen->getHandOffset(pile->handCount, battle->effectStep.cursor) + 0x1800, 0x5C00);
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
}

/* Flags (effectStep.eligible) the cards of pile's hand that can be played and aren't
   picked (effectStep.marked), and dims the others */
void CARDGAME_flagPlayableCards(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 i;

    for (i = 0; i < pile->handCount; i++) {
        if (CARDGAME_canPlayCard(battle, pile->points, pile->hand[i]) && battle->effectStep.marked[i] == 0) {
            battle->effectStep.eligible[i] = 1;
            screen->sprites[i].dimmed = 0;
        } else {
            battle->effectStep.eligible[i] = 0;
            if (battle->effectStep.marked[i] == 0) {
                screen->sprites[i].dimmed = 1;
            } else {
                screen->sprites[i].dimmed = 0;
            }
        }
    }
}

/* Whether the pick is over: six cards picked (effectStep.count), or none left that can
   be played */
s32 CARDGAME_isHandPickDone(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 ok = 1;
    s32 i;

    if (battle->effectStep.count < 6) {
        for (i = 0; i < pile->handCount; i++) {
            if (battle->effectStep.eligible[i] != 0) {
                ok = 0;
                break;
            }
        }
    }
    return ok;
}

/* Adds a card's points to pile's points of its colour (takes them when add is
   0), on the panel */
void CARDGAME_addCardPoints(CardBattle *battle, CardScreen *screen, CardPile *pile, s32 add, s32 card) {
    CardDrawer drawer;

    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[card] + 1);
    if (drawer.card->color < 6) {
        if (add) {
            pile->points[drawer.card->color - 1] += drawer.card->points;
        } else {
            pile->points[drawer.card->color - 1] -= drawer.card->points;
        }
        screen->setPanelValue(screen, pile->side, drawer.card->color - 1, pile->points[drawer.card->color - 1]);
    }
}

/* Picks or puts back the selected card of PILE's hand: 1 picked, 2 put back, 0 neither */
s32 CARDGAME_togglePick(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 result = 0;

    if (battle->effectStep.eligible[battle->effectStep.cursor] != 0) {
        if (battle->effectStep.count < 6) {
            CARDGAME_addCardPoints(battle, screen, pile, 0, pile->hand[battle->effectStep.cursor]);
            battle->effectStep.marked[battle->effectStep.cursor] = 1;
            screen->startBlink(screen, battle->effectStep.cursor);
            screen->sprites[battle->effectStep.cursor].highlight |= 2;
            result = 1;
            battle->effectStep.count++;
        }
    } else if (battle->effectStep.marked[battle->effectStep.cursor] != 0) {
        CARDGAME_addCardPoints(battle, screen, pile, 1, pile->hand[battle->effectStep.cursor]);
        battle->effectStep.marked[battle->effectStep.cursor] = 0;
        screen->sprites[battle->effectStep.cursor].highlight &= ~2;
        result = 2;
        battle->effectStep.count--;
    }
    return result;
}

/* Reads the pad while a card of PILE's hand is being chosen: Left/Right move, Cross picks, Triangle cancels */
void CARDGAME_readChooseInput(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.choice = -1;
        battle->effectStep.nextState = 14;
    }
    if (PAD_PRESSED(PAD_CIRCLE)) {
        battle->effectStep.nextState = 11;
    }
    if (pile->handCount != 0) {
        if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) |
            (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
            if (battle->effectStep.cursor > 0) {
                CARDGAME_movePileSelection(battle, screen, pile, -1);
            }
        } else if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) |
                   (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
            if (battle->effectStep.cursor < pile->handCount - 1) {
                CARDGAME_movePileSelection(battle, screen, pile, 1);
            }
        } else if (PAD_PRESSED(PAD_CROSS) && battle->effectStep.eligible[battle->effectStep.cursor] != 0) {
            battle->effectStep.nextState = 5;
            battle->effectStep.choice = battle->effectStep.cursor;
            battle->effectStep.marked[battle->effectStep.choice] = 1;
        }
    }
}

/* Reads the pad while cards of PILE's hand are being picked: Left/Right move, Cross picks or puts back, Square and Circle end */
void CARDGAME_readPickInput(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
    if (PAD_PRESSED(PAD_SQUARE)) {
        battle->effectStep.nextState = 6;
    }
    if (PAD_PRESSED(PAD_CIRCLE)) {
        battle->effectStep.nextState = 11;
    }
    if (pile->handCount != 0) {
        if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) |
            (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
            if (battle->effectStep.cursor > 0) {
                CARDGAME_movePileSelection(battle, screen, pile, -1);
            }
        } else if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) |
                   (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
            if (battle->effectStep.cursor < pile->handCount - 1) {
                CARDGAME_movePileSelection(battle, screen, pile, 1);
            }
        } else if (PAD_PRESSED(PAD_CROSS) && CARDGAME_togglePick(battle, screen, pile)) {
            battle->effectStep.nextState = 4;
        }
    }
}

/* Reads the pad while one of COUNT cards is being chosen: Left/Right move, Triangle cancels */
void CARDGAME_readCountInput(CardBattle *battle, CardScreen *screen, s32 count) {
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.choice = -1;
        battle->effectStep.nextState = 10;
    }
    if (count != 0) {
        if (((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) |
             (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) &&
            battle->effectStep.cursor > 0) {
            CARDGAME_moveSelection(battle, screen, count, -1);
        }
        if (((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) |
             (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) &&
            battle->effectStep.cursor < count - 1) {
            CARDGAME_moveSelection(battle, screen, count, 1);
        }
    }
}

/* Scales the highlighted sprite up to 1.25 and back, 5 frames each; 1 once
   done */
s32 CARDGAME_stepPulseCard(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.time) {
    case 0:
    default:
        screen->scaleSprite(screen, battle->effectStep.cursor, 5, 0x1400, 0x1400);
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time++;
        break;
    case 1:
        battle->effectStep.vars[0] += GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[0] >= 5) {
            battle->effectStep.time++;
        }
        break;
    case 2:
        screen->scaleSprite(screen, battle->effectStep.cursor, 5, 0x1000, 0x1000);
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time++;
        break;
    case 3:
        battle->effectStep.vars[0] += GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[0] >= 5) {
            done = 1;
        }
        break;
    }
    return done;
}

/* Hides the hand and the choice's windows for the battle menu */
static inline void hideHandForMenu(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    battle->anim.next = battle->anim.hide;
    CARDGAME_promptText = screen->windows[0].value;
    battle->effectStep.time = battle->anim.hide;
    screen->closeWindow(screen, 4);
    screen->closeWindow(screen, 0);
    screen->closeWindow(screen, 1);
    screen->closeWindow(screen, 2);
    screen->closeWindow(screen, 3);
    if (battle->effectStep.id == 0x9A) {
        screen->closeWindow(screen, 5);
    }
    if (pile->side == 0) {
        screen->closePanel(screen, 0);
    }
}

/* Back from the battle menu: shows the hand again, the cards that can't be
   played dimmed, and reopens the choice's windows */
static inline void showHandAfterMenu(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 j;

    CARDGAME_promptText = 0x19;
    battle->effectStep.vars[0] = 0;
    battle->anim.next = battle->effectStep.time - 1;
    switch (battle->effectStep.id) {
    case 0x99:
    case 0x9A:
    case 0x9D:
        for (j = 0; j < pile->handCount; j++) {
            if (CARDGAME_canPlayCard(battle, pile->points, pile->hand[j])) {
                battle->anim.dimmed[j] = 0;
            } else {
                battle->anim.dimmed[j] = 1;
            }
        }
        break;
    }
    screen->openWindow(screen, 0, 0, CARDGAME_promptText, 0, 0x42);
    screen->openWindow(screen, 2, 1, 0, 0x82, 0xA5);
    screen->openWindow(screen, 4, 2, 0, 0x86, 0x31);
    screen->openWindow(screen, 3, 1, 0, 0x82, 0x90);
    screen->openWindow(screen, 1, 3, 0, 0xFD, 0x90);
    if (battle->effectStep.id == 0x9A) {
        screen->openWindow(screen, 5, 4, 0x24, 0, 0x14);
    }
    if (pile->side == 0) {
        screen->openPanel(screen, 0);
    }
}

/* Starts the state of the card choice effectStep.nextState asks for: its
   prompt, windows and dimmed cards */
static inline void startChooseState(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 i;
    s32 j;

    switch (battle->effectStep.nextState) {
    case 1:
    case 2:
        switch (battle->effectStep.id) {
        case 0x99:
            if (pile->side != 0) {
                CARDGAME_promptText = 0x1C;
            } else {
                CARDGAME_promptText = 0x19;
            }
            break;
        case 0x9D:
            CARDGAME_promptText = 0x1C;
            break;
        case 0x9C:
            CARDGAME_promptText = 0x1B;
            break;
        case 0x9A:
        case 0x9B:
            CARDGAME_promptText = 0x19;
            break;
        }
        battle->effectStep.vars[1] = 0;
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        battle->effectStep.choice = 0;
        battle->effectStep.count = 0;
        break;
    case 5:
        SOUND.playSound(SOUND_MENU_CONFIRM);
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        break;
    case 4:
        SOUND.playSound(SOUND_MENU_CONFIRM);
        break;
    case 14:
        if (pile->side == 0) {
            battle->anim.next = CARD_ANIM_HIDE_HAND;
            screen->closePanel(screen, 0);
        } else {
            battle->anim.next = CARD_ANIM_HIDE_OPPONENT_HAND;
        }
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        screen->closeWindow(screen, 4);
        screen->closeWindow(screen, 0);
        screen->closeWindow(screen, 1);
        screen->closeWindow(screen, 2);
        screen->closeWindow(screen, 3);
        break;
    case 10:
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        battle->anim.next = battle->anim.hide;
        screen->closeWindow(screen, 4);
        screen->closeWindow(screen, 0);
        screen->closeWindow(screen, 1);
        screen->closeWindow(screen, 2);
        screen->closeWindow(screen, 3);
        break;
    case 6:
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        CARDGAME_promptText = screen->windows[0].value;
        for (i = 0; i < pile->handCount; i++) {
            if (battle->effectStep.marked[i] != 0) {
                screen->sprites[i].dimmed = 0;
                screen->sprites[i].highlight |= 4;
            } else {
                screen->sprites[i].dimmed = 1;
            }
        }
        break;
    case 7:
        battle->effectStep.choice = 0;
        battle->effectStep.time = 0;
        break;
    case 3:
    case 8:
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        break;
    case 11:
        hideHandForMenu(battle, screen, pile);
        break;
    case 13:
        showHandAfterMenu(battle, screen, pile);
        break;
    case 9:
        battle->effectStep.vars[0] = 0;
        battle->effectStep.time = 0;
        for (j = 0; j < pile->handCount; j++) {
            if (CARDGAME_canPlayCard(battle, pile->points, pile->hand[j])) {
                screen->sprites[j].dimmed = 0;
            } else {
                screen->sprites[j].dimmed = 1;
            }
            if (battle->effectStep.marked[j] != 0) {
                screen->sprites[j].dimmed = 0;
                screen->sprites[j].highlight &= ~4;
            }
        }
        break;
    }
}

/* Puts the highlighted card back in the hand and opens the confirm message */
static inline void askToConfirm(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    switch (battle->effectStep.time) {
    case 0:
        screen->closeWindow(screen, 4);
        screen->closeWindow(screen, 0);
        screen->closeWindow(screen, 5);
        screen->startMove(screen, battle->effectStep.cursor, 5, screen->getHandOffset(pile->handCount, battle->effectStep.cursor) + 0x1800, 0x6100);
        screen->sprites[battle->effectStep.cursor].highlight &= ~1;
        screen->sprites[battle->effectStep.cursor].moving = 0;
        break;
    case 6:
        screen->openMessage(screen, 15, 1, 0, 2);
        break;
    }
}

/* After a no to the confirm message: reopens the choice's windows, then
   lifts the highlighted card again and choosing goes on (state 3) */
static inline void backToChoosing(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    switch (battle->effectStep.time) {
    case 6:
        break;
    case 18:
        screen->openWindow(screen, 0, 0, CARDGAME_promptText, 0, 0x42);
        screen->openWindow(screen, 4, 2, 0, 0x86, 0x31);
        screen->openWindow(screen, 5, 4, 0x24, 0, 0x14);
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    }
    if (++battle->effectStep.time > 30) {
        battle->effectStep.nextState = 3;
        screen->startMove(screen, battle->effectStep.cursor, 5, screen->getHandOffset(pile->handCount, battle->effectStep.cursor) + 0x1800, 0x5C00);
        screen->sprites[battle->effectStep.cursor].highlight |= 1;
        screen->sprites[battle->effectStep.cursor].moving = 1;
    }
}

/* The answer to the confirm message: yes (effectStep.choice 0) ends the choice
   (state 8), no or triangle goes back to choosing (state 9) */
static inline void readConfirmInput(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_CROSS)) {
        screen->confirmMessage(screen);
        if (battle->effectStep.choice == 0) {
            battle->effectStep.nextState = 8;
        } else {
            battle->effectStep.nextState = 9;
        }
    } else if (PAD_PRESSED(PAD_UP) || PAD_PRESSED(PAD_DOWN)) {
        battle->effectStep.choice ^= 1;
        SOUND.playSound(SOUND_CURSOR);
        screen->setMessageChoice(screen, battle->effectStep.choice);
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.choice = 1;
        screen->setMessageChoice(screen, 1);
        screen->confirmMessage(screen);
        battle->effectStep.nextState = 9;
    }
}

/* The chosen cards pulse with a sound, then the hand and its windows close */
static inline void showChosenCards(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    /* the match depends on a loop variable of its own for most loops:
       sharing them gives GCC 2.8.1 other registers */
    s32 k;
    s32 m;
    s32 count;

    switch (battle->effectStep.time) {
    case 20:
        count = 0;
        for (k = 0; k < pile->handCount; k++) {
            if (battle->effectStep.marked[k] != 0) {
                screen->scaleSprite(screen, k, 6, 0x1400, 0x1400);
                count++;
            }
        }
        if (count != 0) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
        }
        break;
    case 25:
        for (m = 0; m < pile->handCount; m++) {
            if (battle->effectStep.marked[m] != 0) {
                screen->scaleSprite(screen, m, 6, 0x1000, 0x1000);
            }
        }
        break;
    case 35:
        if (pile->side == 0) {
            battle->anim.next = CARD_ANIM_HIDE_HAND;
            screen->closePanel(screen, 0);
        } else {
            battle->anim.next = CARD_ANIM_HIDE_OPPONENT_HAND;
        }
        screen->closeWindow(screen, 1);
        screen->closeWindow(screen, 2);
        screen->closeWindow(screen, 3);
        break;
    }
}

/* Back from the battle menu: once the hand is shown again, choosing goes on
   with the picked cards highlighted */
static inline void resumeChoosing(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 n;

    if (((pile->side == 0 && screen->panels[0].state == 2) || (pile->side != 0 && ++battle->effectStep.vars[0] > 10)) &&
        battle->anim.current == CARD_ANIM_NONE) {
        battle->effectStep.nextState = 3;
        screen->startSlide(screen, battle->effectStep.cursor, 5, screen->getHandOffset(pile->handCount, battle->effectStep.cursor) + 0x1800, 0x5C00);
        screen->sprites[battle->effectStep.cursor].highlight |= 1;
        screen->sprites[battle->effectStep.cursor].moving = 1;
        for (n = 0; n < pile->handCount; n++) {
            if (battle->effectStep.marked[n] == 1) {
                screen->sprites[n].dimmed = 0;
                screen->sprites[n].highlight |= 2;
            }
        }
    }
}

/* Choosing cards of PILE's hand to play: effectStep.nextState queues the next step (startChooseState). -1 while it runs, then 0 or 1 */
s32 CARDGAME_stepChooseCards(CardBattle *battle, CardScreen *screen, CardPile *pile) {
    s32 result = -1;

    if (battle->effectStep.nextState != 0) {
        startChooseState(battle, screen, pile);
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }

    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.vars[1] = CARDGAME_openStepWindows(battle, screen, 2, CARDGAME_promptText, battle->effectStep.time, battle->effectStep.vars[1]);
        CARDGAME_showCardInfo(battle, screen, 0);
        if (battle->anim.count * 4 + 14 < battle->effectStep.time) {
            battle->effectStep.nextState = 3;
            screen->startSlide(screen, 0, 5, 0x1800, 0x5C00);
            screen->sprites[battle->effectStep.cursor].highlight |= 1;
            screen->sprites[battle->effectStep.cursor].moving = 1;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        break;
    case 2:
        screen->scaleSprite(screen, 0, 5, 0x1000, 0x1000);
        if (battle->effectStep.time > 10) {
            battle->effectStep.nextState = 3;
            screen->startSlide(screen, 0, 5, 0x1800, 0x5C00);
            screen->sprites[battle->effectStep.cursor].highlight |= 1;
            screen->sprites[battle->effectStep.cursor].moving = 1;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        break;
    case 3:
        switch (battle->effectStep.id) {
        case 0x99:
            CARDGAME_readChooseInput(battle, screen, pile);
            break;
        case 0x9A:
        case 0x9D:
            CARDGAME_readPickInput(battle, screen, pile);
            break;
        case 0x9B:
        case 0x9C:
            CARDGAME_readCountInput(battle, screen, battle->anim.count);
            break;
        }
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    case 4:
        if (screen->sprites[battle->effectStep.cursor].state == 1) {
            CARDGAME_flagPlayableCards(battle, screen, pile);
            if (CARDGAME_isHandPickDone(battle, screen, pile)) {
                battle->effectStep.nextState = 6;
            } else {
                battle->effectStep.nextState = 3;
            }
        }
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    case 5:
        if (CARDGAME_stepPulseCard(battle, screen)) {
            battle->effectStep.nextState = 14;
        }
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    case 14:
        if (pile->handCount * 4 + 5 < battle->effectStep.time) {
            battle->effectStep.nextState = 0;
            battle->effectStep.next = 0;
            battle->effectStep.id = 0;
            battle->effectStep.next = 0;
            result = battle->effectStep.choice != -1;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        break;
    case 10:
        if (battle->anim.count * 4 + 14 < battle->effectStep.time) {
            result = 0;
            battle->effectStep.nextState = 0;
            battle->effectStep.next = 0;
            battle->effectStep.id = 0;
            battle->effectStep.next = 0;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        break;
    case 6:
        askToConfirm(battle, screen, pile);
        if (++battle->effectStep.time > 18) {
            battle->effectStep.nextState = 7;
        }
        break;
    case 7:
        readConfirmInput(battle, screen);
        break;
    case 8:
        showChosenCards(battle, screen, pile);
        if (++battle->effectStep.time > 45 && battle->anim.current == CARD_ANIM_NONE) {
            result = 1;
        }
        break;
    case 9:
        backToChoosing(battle, screen, pile);
        break;
    case 11:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.nextState = 12;
            battle->record.menuState = 0;
            battle->run = CARD_RUN_MENU;
        }
        break;
    case 12:
        battle->effectStep.nextState = 13;
        break;
    case 13:
        resumeChoosing(battle, screen, pile);
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    }
    return result;
}

/* Opens the panels for a step */
void CARDGAME_startPanelsStep(CardBattle *battle, CardScreen *screen) {
    screen->resetPanels(screen);
    battle->effectStep.choice = 0;
    battle->effectStep.state = 1;
    screen->openPanels(screen);
    battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
    battle->anim.faceDown = 1;
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
}

/* Turns the opponent's slot cards over, then counts the panels' ap and hp up to each side's pile apTotal and hpTotal; 1 when done */
s32 CARDGAME_stepTally(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 counting = 0;
    s32 step;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            if (battle->players[0].slotCount + battle->players[1].slotCount == 0) {
                battle->effectStep.state = 3;
                battle->effectStep.time = 0;
                battle->effectStep.vars[0] = 0;
                battle->effectStep.vars[1] = 0;
                battle->effectStep.vars[2] = 0;
                battle->effectStep.vars[3] = 0;
            } else {
                battle->effectStep.state = 2;
                battle->effectStep.time = 0;
                battle->effectStep.vars[0] = 0;
                battle->effectStep.vars[3] = 0;
                if (battle->players[0].slotCount > battle->players[1].slotCount) {
                    battle->effectStep.vars[1] = battle->players[0].slotCount;
                } else {
                    battle->effectStep.vars[1] = battle->players[1].slotCount;
                }
                battle->effectStep.vars[1] = battle->effectStep.vars[1] * 6 + 18;
            }
        }
        break;
    case 2:
        if (battle->effectStep.vars[3] >= 6) {
            if (battle->effectStep.vars[0] < battle->players[1].slotCount) {
                screen->startFlip(screen, battle->effectStep.vars[0] + 6);
            }
            battle->effectStep.vars[0]++;
            battle->effectStep.vars[3] -= 6;
        }
        if (battle->effectStep.time > battle->effectStep.vars[1]) {
            battle->effectStep.state = 3;
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.vars[1] = 0;
            battle->effectStep.vars[2] = 0;
            battle->effectStep.vars[3] = 0;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        break;
    case 3:
        step = battle->sides[0].pile.apTotal / 60;
        battle->effectStep.vars[0] += step != 0 ? step : 1;
        if (battle->effectStep.vars[0] > battle->sides[0].pile.apTotal) {
            battle->effectStep.vars[0] = battle->sides[0].pile.apTotal;
        } else {
            counting = 1;
        }
        step = battle->sides[0].pile.hpTotal / 60;
        battle->effectStep.vars[1] += step != 0 ? step : 1;
        if (battle->effectStep.vars[1] > battle->sides[0].pile.hpTotal) {
            battle->effectStep.vars[1] = battle->sides[0].pile.hpTotal;
        } else {
            counting = 1;
        }
        step = battle->sides[1].pile.apTotal / 60;
        battle->effectStep.vars[2] += step != 0 ? step : 1;
        if (battle->effectStep.vars[2] > battle->sides[1].pile.apTotal) {
            battle->effectStep.vars[2] = battle->sides[1].pile.apTotal;
        } else {
            counting = 1;
        }
        step = battle->sides[1].pile.hpTotal / 60;
        battle->effectStep.vars[3] += step != 0 ? step : 1;
        if (battle->effectStep.vars[3] > battle->sides[1].pile.hpTotal) {
            battle->effectStep.vars[3] = battle->sides[1].pile.hpTotal;
        } else {
            counting = 1;
        }
        if (battle->effectStep.time++ > 60) {
            battle->effectStep.state = 4;
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = battle->sides[0].pile.apTotal;
            battle->effectStep.vars[1] = battle->sides[0].pile.hpTotal;
            battle->effectStep.vars[2] = battle->sides[1].pile.apTotal;
            battle->effectStep.vars[3] = battle->sides[1].pile.hpTotal;
        } else if (counting) {
            SOUND.playSound(SOUND_COUNT);
        }
        screen->setPanelValue(screen, 0, CARD_PANEL_AP, battle->effectStep.vars[0]);
        screen->setPanelValue(screen, 0, CARD_PANEL_HP, battle->effectStep.vars[1]);
        screen->setPanelValue(screen, 1, CARD_PANEL_AP, battle->effectStep.vars[2]);
        screen->setPanelValue(screen, 1, CARD_PANEL_HP, battle->effectStep.vars[3]);
        break;
    case 4:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.time = 90;
        }
        if (++battle->effectStep.time > 90) {
            screen->closePanels(screen);
            battle->effectStep.state = 5;
            battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        }
        break;
    case 5:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            done = 1;
        }
        break;
    }
    return done;
}

/* Shows CARDGAME_stepMessages[index]: its message and window 5 */
void CARDGAME_startStepMessage(CardBattle *battle, CardScreen *screen, s32 index) {
    u8 *entry = CARDGAME_stepMessages[index];

    screen->openMessage(screen, entry[1], 0, 0, 1);
    screen->openWindow(screen, 5, 5, entry[0], 0, 0x42);
    battle->effectStep.state = 1;
}

/* Waits for window 5 to open, then for Cross or Triangle to close it; 1 once it has closed */
s32 CARDGAME_stepMessage(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->windows[5].state == 2) {
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 3;
            screen->closeWindow(screen, 5);
            screen->closeMessage(screen);
        }
        break;
    case 3:
        if (screen->windows[5].state == 0) {
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        done = 1;
        break;
    }
    return done;
}

/* Moves the selection to the other one of the two cards that CARDGAME_startFirstPick lays out */
void CARDGAME_switchCoinCard(CardBattle *battle, CardScreen *screen) {
    SOUND.playSound(SOUND_MENU_MOVE);
    screen->startSlide(screen, battle->effectStep.cursor, 5, CARDGAME_coinCardPositions[battle->effectStep.cursor][0], CARDGAME_coinCardPositions[battle->effectStep.cursor][1]);
    screen->sprites[battle->effectStep.cursor].highlight &= ~1;
    screen->sprites[battle->effectStep.cursor].moving = 0;
    battle->effectStep.cursor ^= 1;
    screen->startSlide(screen, battle->effectStep.cursor, 1, CARDGAME_coinCardPositions[battle->effectStep.cursor][0], CARDGAME_coinCardPositions[battle->effectStep.cursor][1] - 0x500);
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
}

/* Who goes first: cards 0x57 and 0x58 face down, in a random order */
void CARDGAME_startFirstPick(CardBattle *battle, CardScreen *screen) {
    s32 first = RANDOM.next() & 1;

    battle->effectStep.vars[3] = first;
    battle->effectStep.vars[2] = 0;
    battle->effectStep.vars[1] = 0;
    battle->effectStep.vars[0] = 0;
    battle->effectStep.time = 0;
    screen->addSprite(screen, 0, 0x7400, 0x6100);
    screen->addSprite(screen, 1, 0xA400, 0x6100);
    if (first != 0) {
        screen->setSpriteCard(screen, 0, 0x57);
        screen->setSpriteCard(screen, 1, 0x58);
    } else {
        screen->setSpriteCard(screen, 1, 0x57);
        screen->setSpriteCard(screen, 0, 0x58);
    }
    screen->sprites[0].visible = 2;
    screen->sprites[1].visible = 2;
    screen->sprites[0].scaleX = 0;
    screen->sprites[0].dimmed = 0;
    screen->sprites[0].highlight = 0;
    screen->sprites[1].scaleX = 0;
    screen->sprites[1].dimmed = 0;
    screen->sprites[1].highlight = 0;
    battle->effectStep.cursor = 0;
    battle->effectStep.choice = 0;
    battle->effectStep.state = 1;
}

/* The draw for who goes first: the two face-down cards open, the player picks
   one with left/right and cross and both turn over; 1 once it is over, with
   effectStep.choice 1 if the player won the draw */
s32 CARDGAME_drawFirstPlayer(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        switch (battle->effectStep.time) {
        case 0:
            screen->scaleSprite(screen, 0, 10, 0x1000, 0x1000);
            break;
        case 4:
            screen->scaleSprite(screen, 1, 10, 0x1000, 0x1000);
            break;
        }
        battle->effectStep.time++;
        if (battle->effectStep.time >= 15) {
            battle->effectStep.state = 2;
            screen->startSlide(screen, 0, 5, 0x7400, 0x5C00);
            screen->sprites[0].moving = 1;
            screen->sprites[0].highlight |= 1;
        }
        break;
    case 2:
        screen->sprites[battle->effectStep.cursor].highlight |= 1;
        screen->sprites[battle->effectStep.cursor].moving = 1;
        if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) ||
            (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
            CARDGAME_switchCoinCard(battle, screen);
        } else if (PAD_PRESSED(PAD_CROSS)) {
            battle->effectStep.state = 3;
            battle->effectStep.choice = battle->effectStep.cursor;
            battle->effectStep.marked[battle->effectStep.choice] = 1;
            battle->effectStep.vars[1] = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
            SOUND.playSound(SOUND_MENU_CONFIRM);
        }
        break;
    case 3:
        if (CARDGAME_stepPulseCard(battle, screen)) {
            battle->effectStep.state = 4;
            battle->effectStep.vars[1] = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
            screen->startFlip(screen, battle->effectStep.cursor);
        }
        break;
    case 4:
        if (battle->effectStep.time == 20) {
            screen->startFlip(screen, battle->effectStep.cursor ^ 1);
#if VERSION_EU || VERSION_JP
            screen->openWindow(screen, 5, 5, battle->effectStep.vars[3] == battle->effectStep.choice ? 0x44 : 0x43, 0, 0x42);
            SOUND.playSound(SOUND_MENU_OPEN);
#endif
        }
        /* the result stays up for a while; cross or triangle skips it */
        if (battle->effectStep.time >= 31 && (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE))) {
#if VERSION_US
            battle->effectStep.time = 60;
#elif VERSION_EU || VERSION_JP
            battle->effectStep.time = 90;
#endif
        }
#if VERSION_US
        battle->effectStep.time++;
        if (battle->effectStep.time >= 61) {
#elif VERSION_EU || VERSION_JP
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 91) {
#endif
            battle->effectStep.state = 5;
            battle->effectStep.vars[1] = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
#if VERSION_EU || VERSION_JP
            screen->closeWindow(screen, 5);
#endif
        }
        break;
    case 5:
        switch (battle->effectStep.time) {
        case 0:
            screen->scaleSprite(screen, 0, 5, 0, 0x1000);
            break;
        case 4:
            screen->scaleSprite(screen, 1, 5, 0, 0x1000);
            break;
        }
        battle->effectStep.time++;
        if (battle->effectStep.time >= 15) {
            battle->effectStep.state = 6;
        }
        break;
    case 6:
        if (battle->effectStep.vars[3] == battle->effectStep.choice) {
            battle->effectStep.choice = 1;
        } else {
            battle->effectStep.choice = 0;
        }
        done = 1;
        break;
    }
    return done;
}
