/* CARDGAME's card choices: a card of a hand, of a pile, a previous card or
   the target slots, by the player or the computer. */

#include "cardgame/card_battle.h"

/* Sets up CARDGAME_chooseCard's choice of one of a side's cards: kind 0 among
   its hand, 1 and 2 among its deck, 3 among its discards */
void CARDGAME_setupCardChoice(CardBattle *battle, CardScreen *screen, s32 side, s32 kind) {
    s32 i;

    switch (kind) {
    case 0:
        battle->effectStep.vars[4] = side != 0 ? 11 : 5;
        battle->effectStep.vars[3] = battle->sides[side].pile.handCount;
        break;
    case 1:
    case 2:
        if (side == 0) {
            battle->effectStep.vars[4] = 7;
            battle->sortCards(battle, battle->sides[0].pile.deck, battle->sides[0].pile.deckTop | (40 << 16), 2);
        } else {
            battle->effectStep.vars[4] = 13;
            if (kind != 2) {
                battle->sortCards(battle, battle->sides[1].pile.deck, battle->sides[1].pile.deckTop | (40 << 16), 3);
            }
        }
        battle->effectStep.vars[3] = battle->sides[side].pile.deckCount;
        break;
    case 3:
        battle->effectStep.vars[4] = side != 0 ? 15 : 9;
        battle->effectStep.vars[3] = battle->sides[side].pile.discardCount;
        break;
    }
    for (i = 0; i < 40; i++) {
        if (i < battle->effectStep.vars[3]) {
            battle->effectStep.marked[i] = 0;
            if (battle->effectStep.eligible[i] != 0) {
                battle->anim.dimmed[i] = 0;
            } else {
                battle->anim.dimmed[i] = 1;
            }
        }
    }
    battle->effectStep.nextState = 1;
    CARDGAME_clearCardInfo(battle, screen);
    battle->effectStep.cursor = 0;
    battle->effectStep.choice = -1;
}

/* CARDGAME_setupCardChoice of side, kind 0 */
void CARDGAME_setupSideChoice(CardBattle *battle, CardScreen *screen, s32 side) {
    CARDGAME_setupCardChoice(battle, screen, side, 0);
}

/* Moves the highlight along the hand by delta, raising the highlighted card */
void CARDGAME_moveHandHighlight(CardBattle *battle, CardScreen *screen, s32 delta) {
    SOUND.playSound(SOUND_MENU_MOVE);
    screen->startSlide(screen, battle->effectStep.cursor, 5, screen->getHandOffset(battle->effectStep.vars[3], battle->effectStep.cursor) + 0x1800, 0x6100);
    screen->sprites[battle->effectStep.cursor].highlight &= ~1;
    screen->sprites[battle->effectStep.cursor].moving = 0;
    battle->effectStep.cursor += delta;
    screen->startSlide(screen, battle->effectStep.cursor, 1, screen->getHandOffset(battle->effectStep.vars[3], battle->effectStep.cursor) + 0x1800, 0x5C00);
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
}

/* Moves the highlight along the hand with left and right; cross picks a playable card */
void CARDGAME_browseHand(CardBattle *battle, CardScreen *screen) {
    screen->sprites[battle->effectStep.cursor].highlight |= 1;
    screen->sprites[battle->effectStep.cursor].moving = 1;
    if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveHandHighlight(battle, screen, -1);
        }
    } else if ((PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->effectStep.vars[3] - 1) {
            CARDGAME_moveHandHighlight(battle, screen, 1);
        }
    } else if (PAD_PRESSED(PAD_CROSS) && battle->effectStep.eligible[battle->effectStep.cursor] != 0) {
        battle->effectStep.nextState = 3;
    }
    battle->effectStep.flags = 0;
}

/* Shows the card being played (sprite 15) scaling in on the slot row */
static inline void showPlayedCard(CardBattle *battle, CardScreen *screen) {
#if PAL_VIDEO
    screen->addSprite(screen, 15, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0], CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1]);
#else
    screen->addSprite(screen, 15, 0x1800, 0x9000);
#endif
    screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
    screen->sprites[15].scaleX = 0;
    screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
}

/* Takes the picked card (effectStep.choice): one of the player's deck is
   swapped to its top and the rest shuffled, one of the opponent's draws
   gives its order */
static inline void takePickedCard(CardBattle *battle) {
    s32 card;

    switch (battle->effectStep.vars[4]) {
    case 7:
        /* the picked card goes to the top of the hand */
        card = battle->sides[0].pile.deck[battle->sides[0].pile.deckTop];

        battle->sides[0].pile.deck[battle->sides[0].pile.deckTop] = battle->sides[0].pile.deck[battle->sides[0].pile.deckTop + battle->effectStep.choice];
        battle->sides[0].pile.deck[battle->sides[0].pile.deckTop + battle->effectStep.choice] = card;
        battle->effectStep.choice = battle->sides[0].pile.deckTop;
        battle->shufflePile(battle, battle->sides[0].pile.deckTop + 1, battle->sides[0].pile.deckCount - 1);
        break;
    case 13:
        battle->effectStep.choice = battle->opponentDraws[battle->effectStep.choice + battle->sides[1].pile.deckTop].order;
        CARDGAME_sortOpponentCards(battle);
        break;
    }
}

/* Lets a player pick a card of a row (set up by CARDGAME_setupCardChoice): 2 once one is
   picked, 1 if the player backed out with triangle (mode 1), else 0 */
s32 CARDGAME_chooseCard(CardBattle *battle, CardScreen *screen, s32 mode) {
    s32 result = 0;

    if (battle->effectStep.nextState != 0) {
        switch (battle->effectStep.nextState) {
        case 1:
            battle->anim.next = battle->effectStep.vars[4];
            switch (battle->effectStep.vars[4]) {
            case 5:
                CARDGAME_selectionText = 0x19;
                break;
            case 7:
                CARDGAME_selectionText = 0x1A;
                break;
            case 13:
                CARDGAME_selectionText = 0x1D;
                break;
            case 9:
                CARDGAME_selectionText = 0x1B;
                break;
            case 11:
            case 15:
                CARDGAME_selectionText = 0x1C;
                break;
            }
            if (mode == 2) {
                screen->openPanel(screen, 0);
            }
            battle->effectStep.vars[1] = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
            battle->effectStep.choice = 0;
            battle->effectStep.count = 0;
            break;
        case 3:
            SOUND.playSound(SOUND_MENU_CONFIRM);
            /* fallthrough */
        case 2:
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
            break;
        case 4:
            if (mode == 1) {
                screen->scaleSprite(screen, 15, 8, 0, 0x1000);
            }
            battle->effectStep.vars[0] = 0;
            battle->effectStep.time = 0;
            battle->anim.next = battle->anim.hide;
            screen->closeWindow(screen, 4);
            screen->closeWindow(screen, 0);
            screen->closeWindow(screen, 1);
            screen->closeWindow(screen, 2);
            screen->closeWindow(screen, 3);
            break;
        }
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }
    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.vars[1] = CARDGAME_openStepWindows(battle, screen, 2, CARDGAME_selectionText, battle->effectStep.time, battle->effectStep.vars[1]);
        CARDGAME_showCardInfo(battle, screen, 0);
        if (battle->effectStep.time == 2 && mode == 1) {
            showPlayedCard(battle, screen);
        }
        if (battle->anim.count * 4 + 14 < battle->effectStep.time) {
            battle->effectStep.nextState = 2;
            screen->startSlide(screen, 0, 5, 0x1800, 0x5C00);
            screen->sprites[battle->effectStep.cursor].highlight |= 1;
            screen->sprites[battle->effectStep.cursor].moving = 1;
        }
        battle->effectStep.time++;
        break;
    case 2:
        CARDGAME_browseHand(battle, screen);
        if (mode == 1 && PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            battle->effectStep.flags = 1;
            battle->effectStep.nextState = 4;
        }
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    case 3:
        if (CARDGAME_stepPulseCard(battle, screen)) {
            battle->effectStep.flags = 2;
            battle->effectStep.nextState = 4;
            if (mode == 2) {
                screen->closePanel(screen, 0);
            }
        }
        CARDGAME_showCardInfo(battle, screen, 0);
        break;
    case 4:
        if (battle->effectStep.vars[3] * 4 + 5 < battle->effectStep.time++) {
            battle->effectStep.choice = battle->effectStep.cursor;
            takePickedCard(battle);
            battle->effectStep.marked[battle->effectStep.choice] = 1;
            result = battle->effectStep.flags;
            battle->effectStep.nextState = 0;
            battle->effectStep.next = 0;
            battle->effectStep.id = 0;
            battle->effectStep.next = 0;
        }
        break;
    }
    return result;
}

/* The computer marks (effectStep.marked) the cards of its hand it plays: up to six that
   can be played, but not its kind 5 ones, paying their points; 1 */
s32 CARDGAME_pickComputerCards(CardBattle *battle, CardScreen *screen) {
    s32 i;
    s32 card;

    for (i = 0; i < battle->sides[1].pile.handCount; i++) {
        battle->effectStep.marked[i] = 0;
        if (battle->effectStep.count < 6 && CARDGAME_canPlayCard(battle, battle->sides[1].pile.points, battle->sides[1].pile.hand[i])) {
            card = battle->sides[1].pile.hand[i];
            if (battle->opponentPlans[card - 40].kind != 5) {
                CARDGAME_addCardPoints(battle, screen, &battle->sides[1].pile, 0, card);
                battle->effectStep.marked[i] = 1;
                battle->effectStep.count++;
            }
        }
    }
    return 1;
}

/* Sets effectStep.choice to the effectStep.eligible-marked card of the pile effectStep.vars[4] picks with the highest CardImage.rank, the weakest (the strongest when lowest != 0), or -1 */
void CARDGAME_pickBestPileCard(CardBattle *battle, CardScreen *screen, s32 lowest) {
    CardDrawer drawer;
    CardImage *header;
    CardImage *current;
    s32 count = 0;
    s32 card;
    s32 best;
    s32 i;

    /* the match depends on cases 11 and 13 having bodies of their own in
       both switches */
    switch (battle->effectStep.vars[4]) {
    case 5:
        count = battle->sides[0].pile.handCount;
        break;
    case 7:
        count = battle->sides[0].pile.deckCount;
        break;
    case 11:
        count = battle->sides[1].pile.handCount;
        break;
    case 13:
        count = battle->sides[1].pile.handCount;
        break;
    case 9:
        count = battle->sides[0].pile.discardCount;
        break;
    case 15:
        count = battle->sides[1].pile.discardCount;
        break;
    }
    header = NULL;
    card = 0;
    initCardDrawer(&drawer);
    best = -1;
    for (i = 0; i < count; i++) {
        if (battle->effectStep.eligible[i] != 0) {
            switch (battle->effectStep.vars[4]) {
            case 5:
                card = battle->sides[0].pile.hand[i];
                break;
            case 7:
                card = battle->sides[0].pile.deck[i];
                break;
            case 11:
                card = battle->sides[1].pile.hand[i];
                break;
            case 13:
                card = battle->sides[1].pile.hand[i];
                break;
            case 9:
                card = battle->sides[0].pile.discards[i];
                break;
            case 15:
                card = battle->sides[1].pile.discards[i];
                break;
            }
            drawer.setCard(battle->cards[card] + 1);
            current = drawer.card;
            if (best != -1) {
                if (lowest == 0) {
                    if (current->rank >= header->rank) {
                        best = i;
                        header = current;
                    }
                } else if (current->rank < header->rank) {
                    best = i;
                    header = current;
                }
            } else {
                best = i;
                header = current;
            }
        }
    }
    battle->effectStep.choice = best;
}

/* The computer's pick from its deck: the first card flagged (effectStep.eligible) from its
   top up to drawEnd, or else the last one flagged */
void CARDGAME_pickComputerDeckCard(CardBattle *battle, CardScreen *screen) {
    s32 i;
    s32 found = 0;

    for (i = battle->sides[1].pile.deckTop; i < battle->drawEnd; i++) {
        if (battle->effectStep.eligible[i - battle->sides[1].pile.deckTop] != 0) {
            battle->effectStep.choice = i;
            found = 1;
            break;
        }
    }
    if (!found) {
        for (i = 39; battle->sides[1].pile.deckTop < i; i--) {
            if (battle->effectStep.eligible[i - battle->sides[1].pile.deckTop] != 0) {
                break;
            }
        }
        battle->effectStep.choice = i;
    }
    battle->effectStep.marked[battle->effectStep.choice] = 1;
}

/* Picks the player's card with the lowest CardImage.rank, the strongest */
void CARDGAME_pickLowestPlayerCard(CardBattle *battle, CardScreen *screen) {
    CardDrawer drawer;
    s32 best = battle->sides[0].pile.deckTop;
    s32 lowest;
    s32 i;

    initCardDrawer(&drawer);
    lowest = 400;
    for (i = battle->sides[0].pile.deckTop; i < 40; i++) {
        drawer.setCard(battle->cards[battle->sides[0].pile.deck[i]] + 1);
        if (drawer.card->rank < lowest) {
            lowest = drawer.card->rank;
            best = i;
        }
    }
    battle->effectStep.choice = best;
}

/* Shows the card being played as sprite 15 and the panels with flags, and
   starts CARDGAME_stepPileChoice's choice */
void CARDGAME_startPileChoice(CardBattle *battle, CardScreen *screen, s32 flags) {
    s32 i;

    screen->resetPanels(screen);
    screen->setPanelFlags(screen, flags);
    battle->effectStep.choice = 0;
    battle->effectStep.state = 1;
    battle->effectStep.vars[4] = flags;
    screen->addSprite(screen, 15, 0xE500, 0x6100);
    screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
    screen->sprites[15].scaleX = 0;
    screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
    screen->openPanels(screen);
    battle->anim.dimAll = CARD_ANIM_DIM_ALL;
    battle->anim.dimmed[15] = 0;
    for (i = 0; i < 15; i++) {
        battle->effectStep.marked[i] = 0;
    }
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
}

/* The steps of CARDGAME_startPileChoice: confirming is allowed when the pile the panel flags (effectStep.vars[4]) pick has cards; 1 then, 0 on leaving with triangle, -1 until then */
s32 CARDGAME_stepPileChoice(CardBattle *battle, CardScreen *screen) {
    s32 result = -1;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_CROSS)) {
            switch (battle->effectStep.vars[4]) {
            case 0x400:
            case 0x1400:
                if (battle->sides[0].pile.discardCount != 0) {
                    result = 1;
                }
                break;
            case 0x1000:
                if (battle->sides[0].pile.deckCount != 0) {
                    result = 1;
                }
                break;
            case 0x4000:
                if (battle->sides[0].pile.handCount != 0) {
                    result = 1;
                }
                break;
            case 0x2000:
                if (battle->sides[1].pile.deckCount != 0) {
                    result = 1;
                }
                break;
            case 0x8000:
                if (battle->sides[1].pile.handCount != 0) {
                    result = 1;
                }
                break;
            default:
                result = 1;
                break;
            }
            if (result == 1) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            screen->closePanels(screen);
            battle->effectStep.choice = 1;
            battle->effectStep.state = 3;
            screen->scaleSprite(screen, 15, 4, 0, 0x1000);
            battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        }
        break;
    case 3:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            screen->removeSprite(screen, 15);
            screen->clearPanelFlags(screen);
            result = 0;
        }
        break;
    }
    return result;
}

/* Shows the last card played as sprite 15; with mode 0, or mode 1 and a colour 6 card before it, the card before it can be picked too */
void CARDGAME_startPreviousCardChoice(CardBattle *battle, CardScreen *screen, s32 mode) {
    CardDrawer drawer;
    s32 i;
    s32 j;
    s32 ok;
    s32 card;
    s32 sprite;

    screen->resetPanels(screen);
    battle->effectStep.choice = 0;
    battle->effectStep.state = 1;
    battle->effectStep.vars[4] = 0;
    screen->addSprite(screen, 15, 0xE500, 0x6100);
    screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
    screen->sprites[15].scaleX = 0;
    screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
    screen->openPanels(screen);
    for (i = 0; i < 15; i++) {
        battle->anim.dimmed[i] = 1;
    }
    battle->anim.dimmed[15] = 0;
    for (j = 0; j < 15; j++) {
        battle->effectStep.marked[j] = 0;
    }
    if (battle->record.playCount != 0) {
        ok = 0;
        if (mode == 0) {
            ok = 1;
        } else if (mode == 1) {
            card = battle->record.plays[battle->record.playCount - 1].card;
            initCardDrawer(&drawer);
            drawer.setCard(battle->cards[card] + 1);
            if (drawer.card->color == 6) {
                ok = 1;
            }
        }
        if (ok) {
            sprite = battle->record.playCount + 11;
            screen->sprites[sprite].highlight |= 1;
            battle->anim.dimmed[sprite] = 0;
            battle->effectStep.vars[4] = 1;
        }
    }
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
}

/* The steps of CARDGAME_startPreviousCardChoice: confirming takes the card before the last one played, if it can be taken; 1 then, 0 on leaving with triangle, -1 until then */
s32 CARDGAME_stepPreviousCardChoice(CardBattle *battle, CardScreen *screen) {
    s32 result = -1;
    s32 sprite;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 2;
            if (battle->record.playCount != 0) {
                screen->sprites[battle->record.playCount + 11].highlight |= 1;
            }
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_CROSS)) {
            if (battle->effectStep.vars[4] == 1) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                sprite = battle->record.playCount + 11;
                screen->sprites[sprite].highlight &= ~1;
                battle->effectStep.marked[sprite] = battle->record.playCount + 1;
                result = 1;
                battle->record.plays[battle->record.playCount - 1].mark = battle->record.playCount;
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            screen->closePanels(screen);
            battle->effectStep.choice = 1;
            battle->effectStep.state = 3;
            screen->scaleSprite(screen, 15, 4, 0, 0x1000);
            battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        }
        break;
    case 3:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            screen->removeSprite(screen, 15);
            screen->clearPanelFlags(screen);
            result = 0;
        }
        break;
    }
    return result;
}

/* Shows the card being played as sprite 15 and lets the slots out that its target picks (a side of owner's, both, or a card colour) be chosen: effectStep.marked marks them, effectStep.vars[4] is 1 if there are any */
void CARDGAME_showTargetSlots(CardBattle *battle, CardScreen *screen, s32 owner, s32 target) {
    s32 i;
    s32 ok;
    s32 card;
    s32 j;

    screen->resetPanels(screen);
    battle->effectStep.choice = 0;
    battle->effectStep.state = 1;
    screen->addSprite(screen, 15, 0xE500, 0x6100);
    screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
    screen->sprites[15].scaleX = 0;
    screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
    screen->openPanels(screen);
    battle->effectStep.vars[4] = 0;
    for (i = 0, ok = 0; i < 12; i++, ok = 0) {
        battle->effectStep.marked[i] = 0;
        battle->anim.dimmed[i] = 1;
        screen->sprites[i].highlight &= ~1;
        if (i < 6) {
            if (i >= battle->players[0].slotCount) {
                continue;
            }
            card = battle->players[0].slots[i].card;
        } else {
            if (i - 6 >= battle->players[1].slotCount) {
                continue;
            }
            card = battle->players[1].slots[i - 6].card;
        }
        switch (target) {
        case 1:
            if (owner == 0) {
                if (i < 6) {
                    ok = 1;
                }
            } else if (i >= 6) {
                ok = 1;
            }
            break;
        case 2:
            if (owner == 0) {
                if (i >= 6) {
                    ok = 1;
                }
            } else if (i < 6) {
                ok = 1;
            }
            break;
        case 3:
            ok = 1;
            break;
        case 4:
            if (screen->getCardColor(screen, card) != 1) {
                ok = 1;
            }
            break;
        case 5:
            if (screen->getCardColor(screen, card) != 2) {
                ok = 1;
            }
            break;
        case 6:
            if (screen->getCardColor(screen, card) == 3) {
                ok = 1;
            }
            break;
        case 7:
            if (screen->getCardColor(screen, card) != 4) {
                ok = 1;
            }
            break;
        case 8:
            if (screen->getCardColor(screen, card) == 6) {
                ok = 1;
            }
            break;
        }
        if (ok) {
            battle->effectStep.marked[i] = 1;
            battle->anim.dimmed[i] = 0;
            battle->effectStep.vars[4] = 1;
        }
    }
    for (j = 0; j < 3; j++) {
        battle->anim.dimmed[j + 12] = 1;
    }
    battle->anim.dimmed[15] = 0;
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
}

/* The steps of CARDGAME_showTargetSlots: highlights the marked slots (effectStep.marked); confirming needs one (effectStep.vars[4]); 1 then, 0 on leaving with triangle, -1 until then */
s32 CARDGAME_stepTargetSlots(CardBattle *battle, CardScreen *screen) {
    s32 result = -1;
    s32 i;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 2;
            for (i = 0; i < 12; i++) {
                if (battle->effectStep.marked[i] != 0) {
                    screen->sprites[i].highlight |= 1;
                }
            }
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_CROSS)) {
            if (battle->effectStep.vars[4] != 0) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                result = 1;
                for (i = 0; i < 12; i++) {
                    if (battle->effectStep.marked[i] != 0) {
                        screen->sprites[i].highlight &= ~1;
                    }
                }
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            screen->closePanels(screen);
            battle->effectStep.choice = 1;
            battle->effectStep.state = 3;
            screen->scaleSprite(screen, 15, 4, 0, 0x1000);
            battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        }
        break;
    case 3:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            screen->removeSprite(screen, 15);
            screen->clearPanelFlags(screen);
            result = 0;
            for (i = 0; i < 12; i++) {
                if (battle->effectStep.marked[i] != 0) {
                    screen->sprites[i].highlight &= ~1;
                }
            }
        }
        break;
    }
    return result;
}
