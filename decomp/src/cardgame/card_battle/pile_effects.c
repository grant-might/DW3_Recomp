/* CARDGAME's card effects on the hands, decks and discards: discarding,
   drawing, taking cards and changing colour values. */

#include "cardgame/card_battle.h"

/* Starts a count of side's hand and discards (effectStep.vars[0] and effectStep.vars[1]) */
void CARDGAME_startHandCount(CardBattle *battle, CardScreen *screen, s32 side) {
    battle->effectStep.state = 1;
    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = battle->sides[side].pile.handCount;
    battle->effectStep.vars[1] = battle->sides[side].pile.discardCount;
    battle->effectStep.vars[2] = 0;
}

/* Counts a side's hand cards over to its discards on its panel, one every 7 frames, then moves them; 1 when done */
s32 CARDGAME_discardHand(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    s32 more;
    s32 i;

    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[2] += GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[2] >= 7) {
            more = 0;
            if (battle->effectStep.vars[0] > 0) {
                more = 1;
                battle->effectStep.vars[0]--;
                battle->effectStep.vars[1]++;
            }
            if (!more) {
                battle->effectStep.state = 2;
            }
            screen->setPanelValue(screen, side, CARD_PANEL_HAND, battle->effectStep.vars[0]);
            screen->setPanelValue(screen, side, CARD_PANEL_DISCARDS, battle->effectStep.vars[1]);
            battle->effectStep.vars[2] -= 7;
        }
        break;
    case 2:
        for (i = 0; i < battle->sides[side].pile.handCount; i++) {
            battle->sides[side].pile.discards[battle->sides[side].pile.discardCount] = battle->sides[side].pile.hand[i];
            battle->sides[side].pile.discardCount++;
        }
        battle->sides[side].pile.handCount = 0;
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_stepColorValue: amount (effectStep.vars[3], below 0 to take) one at a
   time, one each interval frames (effectStep.vars[2]) */
void CARDGAME_startColorChange(CardBattle *battle, CardScreen *screen, s32 amount, s32 interval) {
    battle->effectStep.vars[2] = interval;
    battle->effectStep.vars[3] = amount;
    battle->effectStep.nextState = 1;
}

/* Moves a side's colour value (pile.points[color]) one step each effectStep.vars[2] frames towards using up effectStep.vars[3] (up to 99, down to 0), on its panel; 1 when done */
s32 CARDGAME_stepColorValue(CardBattle *battle, CardScreen *screen, s32 side, s32 color) {
    s32 done = 0;

    if (battle->effectStep.nextState != 0) {
        switch (battle->effectStep.nextState) {
        case 1:
            screen->setPanelFlags(screen, 1 << (color * 2) << side);
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = 0;
            break;
        case 2:
            screen->clearPanelFlags(screen);
            break;
        }
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }
    switch (battle->effectStep.state) {
    case 1:
        if (battle->effectStep.time == battle->effectStep.vars[2] / 2) {
            if (battle->effectStep.vars[3] > 0) {
                if (battle->sides[side].pile.points[color] < 99) {
                    battle->sides[side].pile.points[color]++;
                }
                battle->effectStep.vars[3]--;
                SOUND.playSound(SOUND_COUNT);
            } else {
                if (battle->sides[side].pile.points[color] != 0) {
                    battle->sides[side].pile.points[color]--;
                }
                battle->effectStep.vars[3]++;
                SOUND.playSound(SOUND_COUNT);
            }
            screen->setPanelValue(screen, side, color, battle->sides[side].pile.points[color]);
        }
        battle->effectStep.time++;
        if (battle->effectStep.time > battle->effectStep.vars[2]) {
            if (battle->effectStep.vars[3] == 0 || battle->sides[side].pile.points[color] == 0) {
                battle->effectStep.nextState = 2;
            } else {
                battle->effectStep.nextState = 1;
            }
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_drainColorValues */
void CARDGAME_startColorDrain(CardBattle *battle, CardScreen *screen) {
    CARDGAME_startColorChange(battle, screen, -0x80, 0x10);
    battle->effectStep.vars[4] = 0;
}

/* Counts both sides' colour values (pile.points) down by one each effectStep.vars[2] frames, on the panels, until they are all 0; 1 when done */
s32 CARDGAME_drainColorValues(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 side;
    s32 i;
    s32 any;
    s32 j;
    s32 k;

    if (battle->effectStep.nextState != 0) {
        switch (battle->effectStep.nextState) {
        case 1:
            screen->setPanelFlags(screen, 0x3FF);
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = 0;
            break;
        case 2:
            screen->clearPanelFlags(screen);
            break;
        }
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }
    switch (battle->effectStep.state) {
    case 1:
        if (battle->effectStep.time == battle->effectStep.vars[2] / 2) {
            for (side = 0; side < 2; side++) {
                for (i = 0; i < 5; i++) {
                    if (battle->sides[side].pile.points[i] != 0) {
                        battle->sides[side].pile.points[i]--;
                    }
                    screen->setPanelValue(screen, side, i, battle->sides[side].pile.points[i]);
                }
            }
        }
        battle->effectStep.time++;
        if (battle->effectStep.time > battle->effectStep.vars[2]) {
            any = 0;
            for (j = 0; j < 2; j++) {
                for (k = 0; k < 5; k++) {
                    if (battle->sides[j].pile.points[k] != 0) {
                        any = 1;
                    }
                }
            }
            if (any) {
                battle->effectStep.nextState = 1;
            } else {
                battle->effectStep.nextState = 2;
            }
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* Closes the gauge of the card before the last one */
void CARDGAME_startCloseGauge(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.time = 0;
    battle->effectStep.state = 1;
    screen->closeGauge(screen, battle->record.playCount - 2);
}

/* Puts the card played before the last one (record.plays[playCount - 2]) on its side's discards once its sprite has turned; 1 when done */
s32 CARDGAME_discardPrevCard(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 side;
    s32 n;
    s32 i;

    switch (battle->effectStep.state) {
    case 1:
        if (CARDGAME_stepShakeAway(battle, screen, battle->record.playCount + 10)) {
            battle->effectStep.vars[0] = 10;
            battle->effectStep.state = 2;
            side = battle->record.plays[battle->record.playCount - 2].side;
            battle->sides[side].pile.discards[battle->sides[side].pile.discardCount] = battle->record.plays[battle->record.playCount - 2].card;
            battle->sides[side].pile.discardCount++;
            screen->setPanelValue(screen, 0, CARD_PANEL_DISCARDS, battle->sides[0].pile.discardCount);
            screen->setPanelValue(screen, 1, CARD_PANEL_DISCARDS, battle->sides[1].pile.discardCount);
        }
        break;
    case 2:
        battle->effectStep.vars[0] -= GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[0] <= 0) {
            battle->effectStep.state = 3;
            n = battle->record.playCount - 1;
            if (n >= 2) {
                battle->record.plays[battle->record.playCount - 3].mark = 0;
            }
            for (i = 0; i < 15; i++) {
                screen->sprites[i].marks[n - 1] = 0;
            }
        }
        break;
    case 3:
        done = 1;
        break;
    }
    return done;
}

/* Keeps the last card played in keptCard (as card + 1) and counts it (keptCount);
   1 */
s32 CARDGAME_keepLastCard(CardBattle *battle, CardScreen *screen) {
    s32 card = battle->record.plays[battle->record.playCount - 1].card;

    battle->keptCount++;
    battle->keptCard = card + 1;
    return 1;
}

/* Shows side's card at effectStep.choice as sprite 17: of its hand (which 0) or its deck
   (1) */
void CARDGAME_showPileCard(CardBattle *battle, CardScreen *screen, s32 side, s32 which) {
    s32 card;

    screen->addSprite(screen, 17, 0xE500, 0x6100);
    card = 0;
    screen->sprites[17].scaleX = 0;
    switch (which) {
    case 0:
        card = battle->sides[side].pile.hand[battle->effectStep.choice];
        battle->effectStep.vars[4] = card;
        break;
    case 1:
        card = battle->sides[side].pile.deck[battle->effectStep.choice];
        battle->effectStep.vars[4] = card;
        break;
    }
    screen->setSpriteCard(screen, 17, card);
    screen->scaleSprite(screen, 17, 8, 0x1000, 0x1000);
    battle->effectStep.time = 0;
    battle->effectStep.state = 1;
}

/* Moves sprite 17 to the side's spot, then puts card effectStep.vars[4] on the side's discards and takes entry effectStep.choice out of the pile it came from (from 0: the hand, from 1: the deck after deckTop); 1 when over */
s32 CARDGAME_discardPickedCard(CardBattle *battle, CardScreen *screen, s32 side, s32 from) {
    s32 done = 0;
    /* the match depends on the loops of from 1 having a variable of their own */
    s32 i, j;

    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 21) {
            screen->startMove(screen, 17, 10, CARDGAME_discardPositions[side][0], CARDGAME_discardPositions[side][1]);
            screen->setSpriteScale(screen, 17, 0, 0);
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (screen->sprites[17].state == 1) {
            battle->effectStep.state = 3;
            battle->effectStep.time = 0;
            battle->sides[side].pile.discards[battle->sides[side].pile.discardCount] = battle->effectStep.vars[4];
            battle->sides[side].pile.discardCount++;
            switch (from) {
            case 0:
                for (i = battle->effectStep.choice; i < battle->sides[side].pile.handCount - 1; i++) {
                    battle->sides[side].pile.hand[i] = battle->sides[side].pile.hand[i + 1];
                }
                screen->setPanelValue(screen, side, CARD_PANEL_HAND, --battle->sides[side].pile.handCount);
                break;
            case 1:
                if (side == 0) {
                    for (i = battle->effectStep.choice; i >= battle->sides[side].pile.deckTop + 1; i--) {
                        battle->sides[side].pile.deck[i] = battle->sides[side].pile.deck[i - 1];
                    }
                    battle->sides[side].pile.deckTop++;
                    battle->sides[side].pile.deckCount--;
                } else {
                    for (j = battle->effectStep.choice; j >= battle->sides[side].pile.deckTop + 1; j--) {
                        battle->sides[side].pile.deck[j] = battle->sides[side].pile.deck[j - 1];
                        battle->opponentDraws[j] = battle->opponentDraws[j - 1];
                    }
                    if (++battle->drawEnd >= 40) {
                        battle->drawEnd = 39;
                    }
                    if (++battle->reserveStart >= 40) {
                        battle->reserveStart = 39;
                    }
                    battle->sides[side].pile.deckTop++;
                    battle->sides[side].pile.deckCount--;
                    for (j = 39; j >= 0; j--) {
                        battle->opponentDraws[j].order = j;
                    }
                }
                screen->setPanelValue(screen, side, CARD_PANEL_DECK, battle->sides[side].pile.deckCount);
                break;
            }
            screen->setPanelValue(screen, side, CARD_PANEL_DISCARDS, battle->sides[side].pile.discardCount);
        }
        break;
    case 3:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 46) {
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        done = 1;
        break;
    }
    return done;
}

/* Adds card effectStep.choice of a side's discards (which 4) or deck to its hand */
void CARDGAME_takeCardToHand(CardBattle *battle, CardScreen *screen, s32 side, s32 which) {
    s32 card;
    s32 i;

    battle->effectStep.vars[4] = battle->sides[side].pile.handCount;
    if (which == 4) {
        card = battle->sides[side].pile.discards[battle->effectStep.choice];
    } else {
        card = battle->sides[side].pile.deck[battle->effectStep.choice];
    }
    battle->sides[side].pile.hand[battle->sides[side].pile.handCount] = card;
    battle->sides[side].pile.handCount++;
    if (battle->effectStep.vars[4] != 0) {
        if (side == 0) {
            battle->anim.next = CARD_ANIM_SHOW_HAND;
        } else {
            battle->anim.next = CARD_ANIM_SHOW_OPPONENT_HAND;
            battle->anim.faceDown = 1;
        }
    } else if (side == 0) {
        battle->anim.next = CARD_ANIM_LAY_OUT_HAND;
    } else {
        battle->anim.next = CARD_ANIM_LAY_OUT_OPPONENT_HAND;
        battle->anim.faceDown = 1;
    }
    screen->openPanel(screen, side);
    for (i = 0; i < 40; i++) {
        battle->anim.dimmed[i] = 0;
    }
    battle->effectStep.state = 1;
}

/* Shows both sides' deck, hand and discard counts on their panels */
static inline void showPileCounts(CardBattle *battle, CardScreen *screen) {
    screen->setPanelValue(screen, 0, CARD_PANEL_DECK, battle->sides[0].pile.deckCount);
    screen->setPanelValue(screen, 0, CARD_PANEL_HAND, battle->sides[0].pile.handCount);
    screen->setPanelValue(screen, 0, CARD_PANEL_DISCARDS, battle->sides[0].pile.discardCount);
    screen->setPanelValue(screen, 1, CARD_PANEL_DECK, battle->sides[1].pile.deckCount);
    screen->setPanelValue(screen, 1, CARD_PANEL_HAND, battle->sides[1].pile.handCount);
    screen->setPanelValue(screen, 1, CARD_PANEL_DISCARDS, battle->sides[1].pile.discardCount);
}

/* Takes card effectStep.choice out of side's used pile, shifting the cards
   after it up */
static inline void takeChosenDiscard(CardBattle *battle, s32 side) {
    s32 i;

    for (i = battle->effectStep.choice; i < battle->sides[side].pile.discardCount - 1; i++) {
        battle->sides[side].pile.discards[i] = battle->sides[side].pile.discards[i + 1];
    }
    battle->sides[side].pile.discardCount--;
}

/* Takes card effectStep.choice out of side's deck (for the computer, a
   reserve card goes from the bottom), shifting the cards above it down */
static inline void takeChosenCard(CardBattle *battle, s32 side) {
    s32 i;
    s32 j;
    s32 k;
    s32 swap;

    if (side == 0) {
        for (j = battle->effectStep.choice; j >= battle->sides[0].pile.deckTop + 1; j--) {
            battle->sides[0].pile.deck[j] = battle->sides[0].pile.deck[j - 1];
        }
    } else {
        if (battle->effectStep.choice < battle->drawEnd) {
            for (i = battle->effectStep.choice; i >= battle->sides[1].pile.deckTop + 1; i--) {
                battle->sides[1].pile.deck[i] = battle->sides[1].pile.deck[i - 1];
                battle->opponentDraws[i] = battle->opponentDraws[i - 1];
            }
        } else {
            i = battle->effectStep.choice;
            if (battle->opponentDraws[i].group == 7) {
                for (; i >= battle->sides[1].pile.deckTop + 1; i--) {
                    battle->sides[1].pile.deck[i] = battle->sides[1].pile.deck[i - 1];
                    battle->opponentDraws[i] = battle->opponentDraws[i - 1];
                }
                battle->drawEnd++;
                battle->reserveStart++;
            } else {
                swap = battle->sides[1].pile.deck[i];
                battle->sides[1].pile.deck[i] = battle->sides[1].pile.deck[39];
                battle->sides[1].pile.deck[39] = swap;
                for (i = 39; i >= battle->sides[1].pile.deckTop + 1; i--) {
                    battle->sides[1].pile.deck[i] = battle->sides[1].pile.deck[i - 1];
                    battle->opponentDraws[i] = battle->opponentDraws[i - 1];
                }
                battle->drawEnd++;
                battle->reserveStart++;
            }
        }
        for (k = 39; k >= 0; k--) {
            battle->opponentDraws[k].order = k;
        }
    }
    battle->sides[side].pile.deckTop++;
    battle->sides[side].pile.deckCount--;
}

/* Moves the card just put in the hand into place, then takes card
   effectStep.choice out of the deck (takeChosenCard), or out of the used pile
   when which is 4 (takeChosenDiscard) */
s32 CARDGAME_drawFromDeck(CardBattle *battle, CardScreen *screen, s32 side, s32 which) {
    CardDrawer drawer;
    s32 done = 0;
    /* the match depends on a variable of its own for most states */
    s32 ready;
    s32 closed;
    s32 index;
    s32 top;
    s32 last;
    s32 card;
    s32 drawn;
    s32 x;

    switch (battle->effectStep.state) {
    case 1:
        ready = 0;
        if (side == 0) {
            ready = screen->panels[0].state == 2;
        } else if (screen->panels[1].state == 2) {
            ready = 1;
        }
        last = battle->sides[side].pile.handCount - 1;
        screen->sprites[last].x = 0x14A00;
        if (ready && battle->anim.current == CARD_ANIM_NONE) {
            x = screen->getHandOffset(battle->sides[side].pile.handCount, last);
            screen->sprites[last].scaleX = 0x1000;
            screen->startMove(screen, last, 15, x + 0x1800, 0x6100);
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        index = battle->sides[side].pile.handCount - 1;
        if (screen->sprites[index].state == 1) {
            if (which == 4) {
                takeChosenDiscard(battle, side);
                battle->effectStep.state = 4;
                battle->effectStep.time = 45;
            } else {
                takeChosenCard(battle, side);
                card = battle->sides[side].pile.hand[index];
                initCardDrawer(&drawer);
                drawer.setCard(battle->cards[card] + 1);
                if (drawer.card->color < 6) {
                    battle->effectStep.state = 3;
                    screen->startBlink(screen, index);
                } else {
                    battle->effectStep.state = 4;
                    battle->effectStep.time = 45;
                }
            }
            showPileCounts(battle, screen);
        }
        break;
    case 3:
        top = battle->sides[side].pile.handCount - 1;
        if (screen->sprites[top].state == 1) {
            battle->effectStep.state = 4;
            battle->effectStep.time = 45;
            drawn = battle->sides[side].pile.hand[top];
            initCardDrawer(&drawer);
            drawer.setCard(battle->cards[drawn] + 1);
            if (drawer.card->color < 6) {
                if (battle->sides[side].pile.points[drawer.card->color - 1] < 99) {
                    battle->sides[side].pile.points[drawer.card->color - 1]++;
                }
                screen->setPanelValue(screen, side, drawer.card->color - 1, battle->sides[side].pile.points[drawer.card->color - 1]);
            }
        }
        break;
    case 4:
        battle->effectStep.time -= GFX.funcs.getFrameTime();
        if (battle->effectStep.time <= 0) {
            battle->effectStep.state = 5;
            screen->closePanel(screen, side);
            battle->anim.next = battle->anim.hide;
        }
        break;
    case 5:
        closed = 0;
        if (side == 0) {
            closed = screen->panels[0].state == 0;
        } else if (screen->panels[1].state == 0) {
            closed = 1;
        }
        if (closed && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 6;
        }
        break;
    case 6:
        done = 1;
        break;
    }
    return done;
}

/* Starts a count of side's discards and deck (effectStep.vars[0] and effectStep.vars[1]) */
void CARDGAME_startDiscardCount(CardBattle *battle, CardScreen *screen, s32 side) {
    battle->effectStep.time = 0;
    battle->effectStep.vars[3] = 0;
    battle->effectStep.vars[0] = battle->sides[side].pile.discardCount;
    battle->effectStep.vars[1] = battle->sides[side].pile.deckCount;
    battle->effectStep.state = 1;
}

/* Counts the panel's discards down into its deck, then puts the side's discards back
   into its deck; 1 once done */
s32 CARDGAME_returnUsedCards(CardBattle *battle, CardScreen *screen, s32 side) {
    CardPile *pile;
    s16 *deck;
    s16 *used;
    s32 done = 0;
    s32 more;
    s32 i;
    s32 j;

    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[3] >= 7) {
            more = 0;
            if (battle->effectStep.vars[0] > 0) {
                more = 1;
                battle->effectStep.vars[0]--;
                battle->effectStep.vars[1]++;
            }
            if (!more) {
                battle->effectStep.state = 2;
            }
            screen->setPanelValue(screen, side, CARD_PANEL_DISCARDS, battle->effectStep.vars[0]);
            screen->setPanelValue(screen, side, CARD_PANEL_DECK, battle->effectStep.vars[1]);
            battle->effectStep.vars[3] -= 7;
        }
        break;
    case 2:
        pile = &battle->sides[side].pile;
        deck = pile->deck;
        used = pile->discards;
        if (side == 0) {
            j = pile->deckTop - 1;
            for (i = pile->discardCount - 1; i >= 0; i--) {
                deck[j--] = used[i];
                pile->deckTop--;
                pile->deckCount++;
            }
            pile->discardCount = 0;
            battle->shufflePile(battle, pile->deckTop, pile->deckCount);
        } else {
            for (i = pile->discardCount - 1; i >= 0; i--) {
                pile->deckTop--;
                pile->deckCount++;
                for (j = pile->deckTop; j < 40; j++) {
                    deck[j] = deck[j + 1];
                    battle->opponentDraws[j] = battle->opponentDraws[j + 1];
                }
                battle->drawEnd--;
                battle->reserveStart--;
                deck[39] = used[i];
                battle->opponentDraws[39].group = 7;
            }
            for (i = 39; i >= 0; i--) {
                battle->opponentDraws[i].order = i;
            }
            pile->discardCount = 0;
        }
        done = 1;
        break;
    }
    return done;
}

/* Starts drawing count cards (effectStep.count) */
void CARDGAME_startDraw(CardBattle *battle, CardScreen *screen, s32 count) {
    s32 i;

    for (i = 0; i < 40; i++) {
        battle->effectStep.marked[i] = 0;
    }
    battle->effectStep.count = count;
    battle->effectStep.flags = 0;
}

/* Marks (effectStep.marked) the cards side draws from its deck: from its top for the
   player, by the round's level (round) for the computer; effectStep.flags when the
   deck runs out; 1 */
s32 CARDGAME_markDrawnCards(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 i;
    u8 n;
    s32 k;
    s32 m;

    if (battle->effectStep.count != 0) {
        if (side == 0) {
            n = 0;
            for (i = battle->sides[0].pile.deckTop; i < battle->sides[0].pile.deckTop + battle->effectStep.count; i++) {
                if (i >= 40) {
                    battle->effectStep.flags = 1;
                    battle->effectStep.count = n;
                    break;
                }
                battle->effectStep.marked[i] = 1;
                n++;
            }
        } else {
            k = battle->sides[1].pile.deckTop;
            m = 39;
            if (battle->effectStep.count > battle->sides[1].pile.deckCount) {
                battle->effectStep.flags = 1;
                battle->effectStep.count = battle->sides[1].pile.deckCount;
            }
            for (i = 0; i < battle->effectStep.count; i++) {
                if (battle->opponentDraws[k].group == battle->round * 2 + 2) {
                    battle->effectStep.marked[k] = 1;
                    k++;
                } else {
                    battle->effectStep.marked[m] = 1;
                    m--;
                }
            }
        }
    }
    return 1;
}

/* Moves side's marked deck cards to its hand and opens its panel */
void CARDGAME_drawMarkedCards(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 i;
    s32 card;
    s32 j;

    battle->effectStep.vars[4] = battle->sides[side].pile.handCount;
#if VERSION_EU || VERSION_JP
    battle->effectStep.vars[1] = 0;
#endif
    for (i = battle->sides[side].pile.deckTop; i < 40; i++) {
        if (battle->effectStep.marked[i] != 0) {
            card = battle->sides[side].pile.deck[i];
            battle->sides[side].pile.hand[battle->sides[side].pile.handCount] = card;
            battle->sides[side].pile.handCount++;
#if VERSION_EU || VERSION_JP
            battle->effectStep.vars[1] = 1;
#endif
        }
    }
    for (j = 0; j < 40; j++) {
        battle->anim.dimmed[j] = 0;
    }
    if (battle->effectStep.vars[4] != 0) {
        if (side == 0) {
            battle->anim.next = CARD_ANIM_SHOW_HAND;
        } else {
            battle->anim.next = CARD_ANIM_SHOW_OPPONENT_HAND;
            battle->anim.faceDown = 1;
        }
    } else {
        if (side == 0) {
            battle->anim.next = CARD_ANIM_LAY_OUT_HAND;
        } else {
            battle->anim.next = CARD_ANIM_LAY_OUT_OPPONENT_HAND;
            battle->anim.faceDown = 1;
        }
    }
    screen->openPanel(screen, side);
    battle->effectStep.state = 1;
}

/* Goes on to the next drawn card, or once all are in place to the deck's
   empty message or the wait before the panel closes */
static inline void goToNextCard(CardBattle *battle, CardScreen *screen, s32 side) {
    battle->effectStep.vars[0]++;
    if (battle->effectStep.vars[0] < battle->sides[side].pile.handCount) {
        battle->effectStep.state = 2;
    } else if (battle->effectStep.flags != 0) {
        screen->openMessage(screen, 0x35, 0, 0, side == 0 ? 2 : 0);
        battle->effectStep.state = 5;
    } else {
        battle->effectStep.time = 45;
        battle->effectStep.state = 8;
    }
}

/* Takes the last marked card out of side's deck (for the computer, a reserve
   card goes from the bottom), shifting the cards above it down */
static inline void takeMarkedCard(CardBattle *battle, s32 side) {
    s32 found;
    /* the match depends on a loop variable of its own for most loops */
    s32 i;
    s32 j;
    s32 m;
    s32 n;
    s32 swap;

    for (i = 39, found = 0; i >= 0; i--) {
        if (battle->effectStep.marked[i] != 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        battle->effectStep.marked[i] = 0;
        if (side == 0) {
            for (m = i; m >= battle->sides[0].pile.deckTop + 1; m--) {
                battle->sides[0].pile.deck[m] = battle->sides[0].pile.deck[m - 1];
                battle->effectStep.marked[m] = battle->effectStep.marked[m - 1];
            }
        } else {
            if (i < battle->drawEnd) {
                for (j = i; j >= battle->sides[1].pile.deckTop + 1; j--) {
                    battle->sides[1].pile.deck[j] = battle->sides[1].pile.deck[j - 1];
                    battle->effectStep.marked[j] = battle->effectStep.marked[j - 1];
                    battle->opponentDraws[j] = battle->opponentDraws[j - 1];
                }
            } else {
                if (battle->opponentDraws[i].group == 7) {
                    for (j = i; j >= battle->sides[1].pile.deckTop + 1; j--) {
                        battle->sides[1].pile.deck[j] = battle->sides[1].pile.deck[j - 1];
                        battle->effectStep.marked[j] = battle->effectStep.marked[j - 1];
                        battle->opponentDraws[j] = battle->opponentDraws[j - 1];
                    }
                    battle->drawEnd++;
                    battle->reserveStart++;
                } else {
                    swap = battle->sides[1].pile.deck[i];
                    battle->sides[1].pile.deck[i] = battle->sides[1].pile.deck[39];
                    battle->sides[1].pile.deck[39] = swap;
                    for (j = 39; j >= battle->sides[1].pile.deckTop + 1; j--) {
                        battle->sides[1].pile.deck[j] = battle->sides[1].pile.deck[j - 1];
                        battle->effectStep.marked[j] = battle->effectStep.marked[j - 1];
                        battle->opponentDraws[j] = battle->opponentDraws[j - 1];
                    }
                    battle->drawEnd++;
                    battle->reserveStart++;
                }
            }
            for (n = 39; n >= 0; n--) {
                battle->opponentDraws[n].order = n;
            }
        }
        battle->sides[side].pile.deckTop++;
        battle->sides[side].pile.deckCount--;
    }
}

/* Moves the cards drawn into the hand (from effectStep.vars[4] on) into place
   one at a time (goToNextCard), counting their colours and taking each flagged
   card (effectStep.marked) out of the deck (takeMarkedCard) */
s32 CARDGAME_drawNewCards(CardBattle *battle, CardScreen *screen, s32 side) {
    CardDrawer drawer;
    s32 done = 0;
    s32 ready;
    s32 closed;
    s32 card;
    s32 k;
    s32 x;

    switch (battle->effectStep.state) {
    case 1:
        ready = 0;
        if (side == 0) {
            ready = screen->panels[0].state == 2;
        } else if (screen->panels[1].state == 2) {
            ready = 1;
        }
        for (k = battle->effectStep.vars[4]; k < battle->sides[side].pile.handCount; k++) {
            screen->sprites[k].x = 0x14A00;
        }
        if (ready && battle->anim.current == CARD_ANIM_NONE) {
#if VERSION_US
            battle->effectStep.state = 2;
#elif VERSION_EU || VERSION_JP
            if (battle->effectStep.vars[1] == 0) {
                screen->openMessage(screen, 0x35, 0, 0, side == 0 ? 2 : 0);
                battle->effectStep.state = 5;
            } else {
                battle->effectStep.state = 2;
            }
#endif
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = battle->effectStep.vars[4];
        }
        break;
    case 2:
        x = screen->getHandOffset(battle->sides[side].pile.handCount, battle->effectStep.vars[0]);
        screen->sprites[battle->effectStep.vars[0]].scaleX = 0x1000;
        screen->startMove(screen, battle->effectStep.vars[0], 15, x + 0x1800, 0x6100);
        battle->effectStep.state = 3;
        break;
    case 3:
        if (screen->sprites[battle->effectStep.vars[0]].state == 1) {
            card = battle->sides[side].pile.hand[battle->effectStep.vars[0]];
            initCardDrawer(&drawer);
            drawer.setCard(battle->cards[card] + 1);
            if (drawer.card->color < 6) {
                if (battle->sides[side].pile.points[drawer.card->color - 1] < 99) {
                    battle->sides[side].pile.points[drawer.card->color - 1]++;
                }
                screen->setPanelValue(screen, side, drawer.card->color - 1, battle->sides[side].pile.points[drawer.card->color - 1]);
                screen->startBlink(screen, battle->effectStep.vars[0]);
                battle->effectStep.state = 4;
            } else {
                goToNextCard(battle, screen, side);
            }
            takeMarkedCard(battle, side);
        }
        break;
    case 4:
        if (screen->sprites[battle->effectStep.vars[0]].state == 1) {
            goToNextCard(battle, screen, side);
        }
        break;
    case 5:
        if (screen->message.state == 2) {
            battle->effectStep.state = 6;
        }
        break;
    case 6:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 7;
            screen->closeMessage(screen);
        }
        break;
    case 7:
        if (screen->message.state == 0) {
            battle->effectStep.time = 45;
            battle->effectStep.state = 8;
        }
        break;
    case 8:
        showPileCounts(battle, screen);
        battle->effectStep.time -= GFX.funcs.getFrameTime();
        if (battle->effectStep.time <= 0) {
            battle->effectStep.state = 9;
            screen->closePanel(screen, side);
            battle->anim.next = battle->anim.hide;
        }
        break;
    case 9:
        closed = 0;
        if (side == 0) {
            closed = screen->panels[0].state == 0;
        } else if (screen->panels[1].state == 0) {
            closed = 1;
        }
        if (closed && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 10;
        }
        break;
    case 10:
        done = 1;
        break;
    }
    return done;
}
