/* CARDGAME's plays and round: the slots swept, a card put, taken, moved,
   copied or turned over, a card played, the hands dealt, then the round's
   start, attacks, swaps, totals and end (one module: cut between them, the
   round's first jump table would lose its alignment). */

#include "cardgame/card_battle.h"

/* Starts CARDGAME_stepSlotSweep */
void CARDGAME_startSlotSweep(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.vars[0] = 0;
    battle->effectStep.state = 1;
}

/* Goes over the marked slots one by one, discarding their cards (which 0) or
   returning them to the hand (1); 1 once done */
s32 CARDGAME_stepSlotSweep(CardBattle *battle, CardScreen *screen, s32 which) {
    s32 done = 0;
    s32 i;
    s32 ok;

    switch (battle->effectStep.state) {
    case 1:
        for (i = battle->effectStep.vars[0]; i < 12; i++) {
            if (battle->effectStep.marked[i] != 0) {
                battle->effectStep.state = 2;
                battle->effectStep.time = 0;
                battle->effectStep.vars[0] = i;
                battle->effectStep.vars[1] = i >= 6;
                battle->effectStep.vars[2] = i;
                if (i >= 6) {
                    battle->effectStep.vars[2] = i - 6;
                }
                break;
            }
        }
        if (i >= 12) {
            battle->effectStep.state = 3;
        }
        break;
    case 2:
        if (which == 0) {
            ok = CARDGAME_stepDiscardSlotCard(battle, screen, battle->effectStep.vars[1], battle->effectStep.vars[2]);
        } else {
            ok = CARDGAME_stepReturnSlotCard(battle, screen, battle->effectStep.vars[1], battle->effectStep.vars[2]);
        }
        if (ok) {
            battle->effectStep.state = 1;
            battle->effectStep.vars[0]++;
        }
        break;
    case 3:
        done = 1;
        break;
    }
    return done;
}

/* Marks (marks) the plays whose effect reaches sprite index, by
   their target (targetKind): a side, both, or a colour; side is not used */
void CARDGAME_markRecordHits(CardBattle *battle, CardScreen *screen, s32 side, s32 index) {
    s32 i;
    s32 color;
    s32 n;
    n = battle->record.playCount - 1;
    color = screen->sprites[index].color + 1;
    for (i = 0; i < n; i++) {
        screen->sprites[index].marks[i] = 0;
        switch (battle->record.plays[i].targetKind) {
        case 1:
            if (battle->record.plays[i].side == 0) {
                if (index < 6) {
                    screen->sprites[index].marks[i] = 1;
                }
            } else if (index >= 6) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 2:
            if (battle->record.plays[i].side == 0) {
                if (index >= 6) {
                    screen->sprites[index].marks[i] = 1;
                }
            } else if (index < 6) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 3:
            screen->sprites[index].marks[i] = 1;
            break;
        case 4:
            if (color != 1) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 5:
            if (color != 2) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 6:
            if (color == 3) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 7:
            if (color != 4) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        case 8:
            if (color == 6) {
                screen->sprites[index].marks[i] = 1;
            }
            break;
        }
    }
}

/* Puts a card in the side's next slot (owned by side 2) and shows it */
void CARDGAME_putSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 card) {
    CardDrawer drawer;
    CardPlayer *player = &battle->players[side];
    s32 index = player->slotCount;
    s32 slot = index;

    if (side != 0) {
        index += 6;
    }
    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[card] + 1);
    player->slots[slot].ap = drawer.card->ap;
    player->slots[slot].hp = drawer.card->hp;
    player->slots[slot].apBonus = 0;
    player->slots[slot].hpBonus = 0;
    player->slots[slot].card = card;
    player->slots[slot].owner = side;
    player->slots[slot].side = 2;
    player->slots[slot].order = battle->slotCount++;
    screen->addSprite(screen, index, 0xE500, 0x6100);
    screen->setSpriteCard(screen, index, card);
    screen->sprites[index].ap = player->slots[slot].ap;
    screen->sprites[index].hp = player->slots[slot].hp;
    screen->sprites[index].scaleX = 0;
    CARDGAME_markRecordHits(battle, screen, side, index);
    battle->effectStep.state = 1;
    screen->scaleSprite(screen, index, 20, 0x1000, 0x1000);
}

/* Takes the card the last record names out of the side's hand into its next
   slot and shows it */
void CARDGAME_takeHandCard(CardBattle *battle, CardScreen *screen, s32 side) {
    CardPlayer *player = &battle->players[side];
    CardPile *pile = &battle->sides[side].pile;
    s32 index = player->slotCount;
    s32 slot = index;
    s32 i;
    s32 j;

    if (side != 0) {
        index += 6;
    }
    for (i = 0; i < pile->handCount; i++) {
        if (battle->record.plays[battle->record.playCount - 1].target == pile->hand[i]) {
            battle->addCard(battle, side, battle->record.plays[battle->record.playCount - 1].target);
            break;
        }
    }
    for (j = i; j < pile->handCount - 1; j++) {
        pile->hand[j] = pile->hand[j + 1];
    }
    pile->handCount--;
    player->slotCount--;
    screen->addSprite(screen, index, 0xE500, 0x6100);
    screen->setSpriteCard(screen, index, player->slots[slot].card);
    for (i = 0; i < 5; i++) {
        CARDGAME_setCountingCardValues(battle, &player->slots[slot], side, i);
    }
    screen->sprites[index].ap = player->slots[slot].ap;
    screen->sprites[index].hp = player->slots[slot].hp;
    screen->sprites[index].scaleX = 0;
    CARDGAME_markRecordHits(battle, screen, side, index);
    battle->effectStep.state = 1;
    screen->setPanelValue(screen, side, CARD_PANEL_HAND, pile->handCount);
    screen->scaleSprite(screen, index, 20, 0x1000, 0x1000);
}

/* Moves the side's next slot card into place; 1 once it is there */
s32 CARDGAME_moveSlotCard(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    CardPlayer *player = &battle->players[side];
    s32 index = player->slotCount;
    s32 x;
    s32 y;

    if (side != 0) {
        index += 6;
    }
#if PAL_VIDEO
    if (side == 0) {
        x = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + player->slotCount * 0x2900;
    } else {
        x = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + player->slotCount * 0x2900;
    }
    if (side == 0) {
        y = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1];
    } else {
        y = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3];
    }
#else
    x = player->slotCount * 0x2900 + 0x1800;
    y = 0x3200;
    if (side == 0) {
        y = 0x9000;
    }
#endif
    switch (battle->effectStep.state) {
    case 1:
        if (screen->sprites[index].state == 1) {
            screen->startBlink(screen, index);
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (screen->sprites[index].state == 1) {
            screen->startMove(screen, index, 20, x, y);
            screen->sprites[index].moving = 1;
            battle->effectStep.state = 3;
        }
        break;
    case 3:
        if (screen->sprites[index].state == 1) {
            battle->effectStep.state = 4;
            screen->sprites[index].moving = 0;
            player->slotCount++;
        }
        break;
    case 4:
        done = 1;
        break;
    }
    return done;
}

/* Copies the first marked slot card (effectStep.marked) into the side's next slot */
void CARDGAME_copySlotCard(CardBattle *battle, CardScreen *screen, s32 side) {
    CardPlayer *player = &battle->players[side];
    s32 index = player->slotCount;
    s32 slot = index;
    CardSlot *src;
    s32 from;
    s32 i;

    if (side != 0) {
        index += 6;
    }
    from = 0;
    src = &player->slots[slot];
    for (i = 0; i < 12; i++) {
        if (i < 6) {
            if (i >= battle->players[0].slotCount) {
                continue;
            }
            src = &battle->players[0].slots[i];
        } else {
            if (i - 6 >= battle->players[1].slotCount) {
                continue;
            }
            src = &battle->players[1].slots[i - 6];
        }
        if (battle->effectStep.marked[i] != 0) {
            from = i;
            break;
        }
    }
    player->slots[slot] = *src;
    screen->addSprite(screen, index, 0xE500, 0x6100);
    screen->setSpriteCard(screen, index, player->slots[slot].card);
    screen->sprites[index] = screen->sprites[from];
    screen->sprites[index].scaleX = 0;
    CARDGAME_markRecordHits(battle, screen, side, index);
    for (i = 0; i < battle->record.playCount - 1; i++) {
        if (battle->record.plays[i].targetKind == 0 && screen->sprites[from].marks[i] != 0) {
            screen->sprites[index].marks[i] = 1;
        }
    }
    battle->effectStep.state = 1;
    screen->scaleSprite(screen, from, 5, 0, 0x1000);
    battle->effectStep.time = 7;
}

/* Waits effectStep.time frames, then flips the side's next slot card over and moves it
   into place; 1 once it is there */
s32 CARDGAME_flipSlotCard(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    CardPlayer *player = &battle->players[side];
    s32 index = player->slotCount;
    s32 x;
    s32 y;

    if (side != 0) {
        index += 6;
    }
#if PAL_VIDEO
    if (side == 0) {
        x = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + player->slotCount * 0x2900;
    } else {
        x = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + player->slotCount * 0x2900;
    }
    if (side == 0) {
        y = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1];
    } else {
        y = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3];
    }
#else
    x = player->slotCount * 0x2900 + 0x1800;
    y = 0x3200;
    if (side == 0) {
        y = 0x9000;
    }
#endif
    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.time -= GFX.funcs.getFrameTime();
        if (battle->effectStep.time <= 0) {
            screen->scaleSprite(screen, index, 5, 0x1000, 0x1000);
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (screen->sprites[index].state == 1) {
            screen->startBlink(screen, index);
            battle->effectStep.state = 3;
        }
        break;
    case 3:
        if (screen->sprites[index].state == 1) {
            screen->startMove(screen, index, 20, x, y);
            screen->sprites[index].moving = 1;
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        if (screen->sprites[index].state == 1) {
            battle->effectStep.state = 5;
            screen->sprites[index].moving = 0;
            player->slotCount++;
        }
        break;
    case 5:
        done = 1;
        break;
    }
    return done;
}

/* Shows the card being played as sprite 15 (opening the panels for side 0)
   and starts moving it to its place in the record */
void CARDGAME_startPlayCard(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 i;
    s32 j;

    if (side == 0 || screen->panels[0].state == 0) {
        if (screen->panels[0].state == 0) {
            screen->resetPanels(screen);
            battle->effectStep.choice = 0;
            screen->addSprite(screen, 15, 0xE500, 0x6100);
            screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
            screen->sprites[15].scaleX = 0;
            screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
            screen->openPanels(screen);
            battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
            battle->anim.dimmed[15] = 0;
            if (side == 0) {
                for (j = 0; j < 15; j++) {
                    battle->effectStep.marked[j] = 0;
                }
            }
            battle->anim.next = CARD_ANIM_SHOW_SLOTS;
            battle->effectStep.state = 1;
        } else {
            screen->startMove(screen, 15, 10, CARDGAME_recordCardPositions[battle->record.playCount][0], CARDGAME_recordCardPositions[battle->record.playCount][1]);
#if VERSION_US
            battle->effectStep.state = 3;
#elif VERSION_EU || VERSION_JP
            battle->effectStep.state = 5;
#endif
            battle->effectStep.time = 0;
        }
    } else {
        screen->addSprite(screen, 15, 0xE500, 0x6100);
        screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
        screen->sprites[15].scaleX = 0;
        screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
        battle->effectStep.state = 2;
        battle->effectStep.time = 0;
#if VERSION_EU || VERSION_JP
        battle->effectStep.vars[0] = 0;
#endif
    }
    screen->clearPanelFlags(screen);
    for (i = 0; i < 15; i++) {
        screen->sprites[i].dimmed = 0;
    }
}

/* Card 15 goes off and the marked slot cards turn over; it then lands in
   sprite 12 + CardRecord.playCount. 1 once done */
s32 CARDGAME_stepPlayCard(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 i;
    s32 j;
#if VERSION_EU || VERSION_JP
    s32 card;
#endif

    switch (battle->effectStep.state) {
#if VERSION_US
    case 2:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 21) {
            screen->startMove(screen, 15, 10, CARDGAME_recordCardPositions[battle->record.playCount][0], CARDGAME_recordCardPositions[battle->record.playCount][1]);
            battle->effectStep.state = 3;
            battle->effectStep.time = 0;
        }
        break;
#elif VERSION_EU || VERSION_JP
    case 2:
        screen->windows[1].showCount = 0;
        screen->windows[1].value = 0;
        screen->windows[2].value = 0;
        screen->windows[3].value = 0;
        screen->windows[4].value = 0;
        battle->effectStep.vars[0] = CARDGAME_openStepWindows(battle, screen, 2, 0, battle->effectStep.time, battle->effectStep.vars[0]);
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 21) {
            battle->effectStep.time = 0;
            battle->effectStep.vars[0] = 0;
            battle->effectStep.state = 3;
        }
        break;
    case 3:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 4;
        }
        card = battle->cards[battle->record.plays[battle->record.playCount].card] + 1;
        screen->windows[4].numbers[2] = 0;
        screen->windows[2].value = card;
        screen->windows[4].value = card;
        break;
    case 4:
        screen->startMove(screen, 15, 10, CARDGAME_recordCardPositions[battle->record.playCount][0], CARDGAME_recordCardPositions[battle->record.playCount][1]);
        screen->closeWindow(screen, 4);
        screen->closeWindow(screen, 1);
        screen->closeWindow(screen, 2);
        screen->closeWindow(screen, 3);
        battle->effectStep.state = 5;
        battle->effectStep.time = 0;
        battle->effectStep.vars[0] = 0;
        break;
#endif
    case 1:
        if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            screen->startMove(screen, 15, 10, CARDGAME_recordCardPositions[battle->record.playCount][0], CARDGAME_recordCardPositions[battle->record.playCount][1]);
            battle->effectStep.state = 3 + CARD_INFO_STEPS;
            battle->effectStep.time = 0;
        }
        break;
    case 3 + CARD_INFO_STEPS:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 12) {
            battle->effectStep.time = 0;
            battle->effectStep.state = 4 + CARD_INFO_STEPS;
            screen->startBlink(screen, 15);
            for (i = 0; i < 15; i++) {
                if (battle->effectStep.marked[i] != 0) {
                    screen->startBlink(screen, i);
                }
            }
        }
        break;
    case 4 + CARD_INFO_STEPS:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 21) {
            battle->effectStep.state = 5 + CARD_INFO_STEPS;
            battle->effectStep.time = 0;
            screen->scaleSprite(screen, 15, 5, 0, 0x1000);
            for (j = 0; j < 15; j++) {
                if (battle->effectStep.marked[j] != 0) {
                    screen->sprites[j].marks[battle->record.playCount] = 1;
                }
            }
        }
        break;
    case 5 + CARD_INFO_STEPS:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 7) {
            battle->effectStep.time = 0;
            screen->sprites[15].order = battle->record.playCount + 1;
            screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
            battle->effectStep.state = 6 + CARD_INFO_STEPS;
            screen->openGauge(screen, battle->record.playCount, battle->record.turnSide);
        }
        break;
    case 6 + CARD_INFO_STEPS:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 11) {
            battle->effectStep.time = 0;
            battle->effectStep.state = 7 + CARD_INFO_STEPS;
            screen->sprites[battle->record.playCount + 12] = screen->sprites[15];
            screen->removeSprite(screen, 15);
            for (j = 0; j < 12; j++) {
                screen->sprites[j].highlight &= ~1;
            }
        }
        break;
    case 7 + CARD_INFO_STEPS:
        done = 1;
        break;
    }
    return done;
}

/* Empties both hands, on the panels with the decks */
void CARDGAME_clearHands(CardBattle *battle, CardScreen *screen) {
    battle->sides[0].pile.handCount = 0;
    battle->sides[1].pile.handCount = 0;
    screen->setPanelValue(screen, 0, CARD_PANEL_HAND, battle->sides[0].pile.handCount);
    screen->setPanelValue(screen, 1, CARD_PANEL_HAND, battle->sides[1].pile.handCount);
    screen->setPanelValue(screen, 0, CARD_PANEL_DECK, battle->sides[0].pile.deckCount);
    screen->setPanelValue(screen, 1, CARD_PANEL_DECK, battle->sides[1].pile.deckCount);
    battle->effectStep.choice = 0;
    battle->effectStep.nextState = 1;
}

/* Deals six cards from each side's deck into its hand and puts them on the
   table, face down */
void CARDGAME_dealHands(CardBattle *battle, CardScreen *screen) {
    s32 side;
    s32 i;
    s32 j;

    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = 0;
    battle->effectStep.vars[3] = 0;
    battle->effectStep.vars[1] = battle->sides[0].pile.deckCount;
    battle->effectStep.vars[2] = battle->sides[1].pile.deckCount;
    for (side = 0; side < 2; side++) {
        for (j = 0; j < 6; j++) {
            battle->sides[side].pile.hand[j] = battle->sides[side].pile.deck[battle->sides[side].pile.deckTop];
            battle->sides[side].pile.handCount++;
            battle->sides[side].pile.deckTop++;
            battle->sides[side].pile.deckCount--;
        }
    }
    for (i = 0; i < 6; i++) {
#if PAL_VIDEO
        screen->addSprite(screen, i, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + 0x14800, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1]);
#else
        screen->addSprite(screen, i, 0x16000, 0x9000);
#endif
        screen->setSpriteCard(screen, i, battle->sides[0].pile.hand[i]);
        screen->sprites[i].visible = 2;
        screen->sprites[i].scaleY = 0;
        screen->sprites[i].scaleX = 0;
    }
    for (i = 0; i < 6; i++) {
#if PAL_VIDEO
        screen->addSprite(screen, i + 6, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + 0x14800, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3]);
#else
        screen->addSprite(screen, i + 6, 0x16000, 0x3200);
#endif
        screen->setSpriteCard(screen, i + 6, battle->sides[1].pile.hand[i]);
        screen->sprites[i + 6].visible = 2;
        screen->sprites[i + 6].scaleY = 0;
        screen->sprites[i + 6].scaleX = 0;
    }
}

/* Deals the next card of each side to its slot, and counts it off the deck
   on the panels */
static inline void dealNextCards(CardBattle *battle, CardScreen *screen) {
    if (battle->effectStep.vars[0] < 6) {
#if PAL_VIDEO
        screen->startMove(screen, battle->effectStep.vars[0], 20, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + battle->effectStep.vars[0] * 0x2900, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1]);
#else
        screen->startMove(screen, battle->effectStep.vars[0], 20, battle->effectStep.vars[0] * 0x2900 + 0x1800, 0x9000);
#endif
        screen->setSpriteScale(screen, battle->effectStep.vars[0], 0x1000, 0x1000);
#if PAL_VIDEO
        screen->startMove(screen, battle->effectStep.vars[0] + 6, 20, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + battle->effectStep.vars[0] * 0x2900, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3]);
#else
        screen->startMove(screen, battle->effectStep.vars[0] + 6, 20, battle->effectStep.vars[0] * 0x2900 + 0x1800, 0x3200);
#endif
        screen->setSpriteScale(screen, battle->effectStep.vars[0] + 6, 0x1000, 0x1000);
        battle->effectStep.vars[0]++;
        battle->effectStep.vars[1]--;
        battle->effectStep.vars[2]--;
        screen->setPanelValue(screen, 0, CARD_PANEL_HAND, battle->effectStep.vars[0]);
        screen->setPanelValue(screen, 1, CARD_PANEL_HAND, battle->effectStep.vars[0]);
        screen->setPanelValue(screen, 0, CARD_PANEL_DECK, battle->effectStep.vars[1]);
        screen->setPanelValue(screen, 1, CARD_PANEL_DECK, battle->effectStep.vars[2]);
    }
}

/* Counts the colours of the next card of each side; the cards that add to
   one blink */
static inline void countNextColors(CardBattle *battle, CardScreen *screen) {
    if (battle->effectStep.vars[0] < 6) {
        if (CARDGAME_addColorCount(battle, screen, 0, battle->sides[0].pile.hand[battle->effectStep.vars[0]])) {
            screen->startBlink(screen, battle->effectStep.vars[0]);
        }
        if (CARDGAME_addColorCount(battle, screen, 1, battle->sides[1].pile.hand[battle->effectStep.vars[0]])) {
            screen->startBlink(screen, battle->effectStep.vars[0] + 6);
        }
        battle->effectStep.vars[0]++;
    }
}

/* Starts the state of the battle start effectStep.nextState asks for */
static inline void startDealState(CardBattle *battle, CardScreen *screen) {
    switch (battle->effectStep.nextState) {
    case 1:
        screen->openPanels(screen);
        break;
    case 2:
        CARDGAME_dealHands(battle, screen);
        break;
    case 3:
    case 4:
        battle->effectStep.time = 0;
        battle->effectStep.vars[0] = 0;
        battle->effectStep.vars[3] = 0;
        break;
    case 5:
        battle->effectStep.time = 0;
        battle->effectStep.vars[0] = 0;
        battle->effectStep.vars[3] = 0;
        screen->closePanels(screen);
        break;
    case 6:
        screen->setPanelFlags(screen, 0x1000);
        screen->openMessage(screen, 0x21, 0, 1, 1);
        battle->effectStep.choice = 1;
        break;
    case 7:
        SOUND.playSound(SOUND_WIN_JINGLE);
        screen->setPanelFlags(screen, 0x2000);
        screen->openMessage(screen, 0x22, 0, 1, 1);
        battle->effectStep.choice = 2;
        break;
    case 9:
        battle->effectStep.time = 0;
        battle->effectStep.vars[0] = 0;
        battle->effectStep.vars[3] = 0;
        screen->closePanels(screen);
        screen->closeMessage(screen);
        break;
    case 11:
        screen->openMessage(screen, battle->prize, 0, 0, 3);
        break;
    case 8:
    case 10:
    case 12:
        /* nothing to set up */
        break;
    }
}

/* The start of a card battle: deals six cards to each side (dealNextCards), turns them over and counts their colours (countNextColors) (a deck with fewer than six cards shows a message instead); 1 when it ends */
s32 CARDGAME_stepStart(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    if (battle->effectStep.nextState != 0) {
        startDealState(battle, screen);
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 2) {
            if (battle->sides[0].pile.deckCount < 6) {
                battle->effectStep.nextState = 6;
            } else if (battle->sides[1].pile.deckCount < 6) {
                battle->effectStep.nextState = 7;
            } else {
                battle->effectStep.nextState = 2;
            }
        }
        break;
    case 2:
        if (battle->effectStep.vars[3] >= 7) {
            dealNextCards(battle, screen);
            battle->effectStep.vars[3] -= 7;
        }
        if (battle->effectStep.time > 60) {
            battle->effectStep.nextState = 3;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        break;
    case 3:
        if (battle->effectStep.vars[3] >= 7) {
            if (battle->effectStep.vars[0] < 6) {
                screen->startFlip(screen, battle->effectStep.vars[0]);
                battle->effectStep.vars[0]++;
            }
            battle->effectStep.vars[3] -= 7;
        }
        if (battle->effectStep.time > 65) {
            battle->effectStep.nextState = 4;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        break;
    case 4:
        if (battle->effectStep.vars[3] >= 7) {
            countNextColors(battle, screen);
            battle->effectStep.vars[3] -= 7;
        }
        if (battle->effectStep.time > 80) {
            battle->effectStep.nextState = 5;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        break;
    case 5:
        if (battle->effectStep.vars[3] >= 3) {
            if (battle->effectStep.vars[0] < 6) {
                screen->scaleSprite(screen, battle->effectStep.vars[0], 5, 0, 0x1000);
                screen->scaleSprite(screen, battle->effectStep.vars[0] + 6, 5, 0, 0x1000);
                battle->effectStep.vars[0]++;
            }
            battle->effectStep.vars[3] -= 3;
        }
        if (battle->effectStep.time > 40) {
            battle->effectStep.nextState = 10;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[3] += GFX.funcs.getFrameTime();
        break;
    case 6:
    case 7:
        if (screen->message.state == 2) {
            battle->effectStep.nextState = 8;
        }
        break;
    case 8:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.nextState = 9;
        }
        break;
    case 9:
        if (screen->message.state == 0 && screen->panels[0].state == 0) {
            battle->effectStep.nextState = 10;
        }
        break;
    case 10:
        if (battle->effectStep.choice == 2) {
            battle->effectStep.nextState = 11;
        } else {
            done = 1;
        }
        break;
    case 11:
        if (screen->message.state == 2) {
            battle->effectStep.nextState = 12;
        }
        break;
    case 12:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            done = 1;
        }
        break;
    }
    return done;
}

/* Starts CARDGAME_stepAttack: side's apTotal comes off the other side's hpTotal
   (down to 0), counted down over its cards out */
void CARDGAME_startAttack(CardBattle *battle, CardScreen *screen, s32 side) {
    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = 0;
    battle->effectStep.vars[1] = battle->sides[side].pile.apTotal;
    battle->effectStep.vars[2] = battle->sides[side ^ 1].pile.hpTotal << 8;
    battle->sides[side ^ 1].pile.hpTotal -= battle->sides[side].pile.apTotal;
    if (battle->sides[side ^ 1].pile.hpTotal < 0) {
        battle->sides[side ^ 1].pile.hpTotal = 0;
    }
    if (battle->players[side].slotCount != 0) {
        battle->effectStep.vars[3] = (battle->effectStep.vars[2] - (battle->sides[side ^ 1].pile.hpTotal << 8)) / (battle->players[side].slotCount * 28 - 16);
        if (battle->effectStep.vars[3] == 0) {
            battle->effectStep.vars[3] = 1;
        }
    } else {
        battle->effectStep.vars[3] = 1;
    }
    battle->effectStep.state = 1;
}

/* The side's slot cards go over one by one (every 27 frames) while the panels
   count its attack (CARD_PANEL_AP) and the other side's HP (CARD_PANEL_HP)
   down; 1 once done */
s32 CARDGAME_stepAttack(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    s32 other = side ^ 1;
    s32 index;
    s32 value;
#if PAL_VIDEO
    s32 x;
#endif
    s32 i;

    switch (battle->effectStep.state) {
    case 1:
        if (battle->effectStep.time % 27 == 0 && battle->effectStep.vars[0] < battle->players[side].slotCount) {
            index = battle->effectStep.vars[0];
            if (side != 0) {
                index += 6;
            }
#if PAL_VIDEO
            x = CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + (battle->players[other].slotCount - 1) * 0x1480;
            if (SHIFT_PAL_SCREEN) {
                screen->startFly(screen, index, 6, x, side != 0 ? 0x6D00 : 0x5500);
            } else {
                screen->startFly(screen, index, 6, x, 0x6100);
            }
#else
            screen->startFly(screen, index, 6, (battle->players[other].slotCount - 1) * 0x1480 + 0x1800, 0x6100);
#endif
            battle->effectStep.vars[0]++;
            screen->sprites[index].moving = 1;
        }
        if (battle->effectStep.time >= 16) {
            value = 0;
            if (battle->effectStep.time < battle->players[side].slotCount * 28) {
                value = battle->effectStep.vars[1] - (battle->effectStep.vars[1] / (battle->players[side].slotCount * 56 + 1) + 1) * battle->effectStep.time;
                if (value < 0) {
                    value = 0;
                }
            }
            screen->setPanelValue(screen, side, CARD_PANEL_AP, value);
            if (battle->effectStep.time < battle->players[side].slotCount * 28) {
                battle->effectStep.vars[2] -= battle->effectStep.vars[3];
                if (battle->effectStep.vars[2] < battle->sides[other].pile.hpTotal << 8) {
                    battle->effectStep.vars[2] = battle->sides[other].pile.hpTotal << 8;
                }
            } else {
                battle->effectStep.vars[2] = battle->sides[other].pile.hpTotal << 8;
            }
            screen->setPanelValue(screen, other, CARD_PANEL_HP, battle->effectStep.vars[2] >> 8);
        }
        if (screen->spriteFlags & 2) {
            for (i = 0; i < battle->players[other].slotCount; i++) {
                screen->startJitter(screen, side == 0 ? i + 6 : i);
            }
        }
        battle->effectStep.time++;
        if (battle->effectStep.time > battle->players[side].slotCount * 28 + 25) {
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_stepDiscardAllSlots from side's last slot */
void CARDGAME_startDiscardAllSlots(CardBattle *battle, CardScreen *screen, s32 side) {
    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = battle->players[side].slotCount - 1;
    battle->effectStep.state = 1;
}

/* Discards side's slot cards from the last to the first; 1 once done */
s32 CARDGAME_stepDiscardAllSlots(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        if (CARDGAME_stepDiscardSlotCard(battle, screen, side, battle->effectStep.vars[0])) {
            battle->effectStep.time = 0;
            if (--battle->effectStep.vars[0] < 0) {
                battle->effectStep.state = 2;
                battle->players[side].slotCount = 0;
            }
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* The value going from FROM to TO in DURATION, at TIME, never past TO */
s32 CARDGAME_interpolate(s32 to, s32 from, s32 duration, s32 time) {
    s32 delta = to - from;
    s32 inRange;

    if (time == duration || from == to) {
        return to;
    }
    from += delta * time / duration;
    if (delta > 0) {
        inRange = from < to;
    } else {
        inRange = from > to;
    }
    if (!inRange) {
        from = to;
    }
    return from;
}

/* Shows the kept card (keptCard) as sprite 17 for CARDGAME_stepSwap */
void CARDGAME_startSwap(CardBattle *battle, CardScreen *screen) {
    screen->addSprite(screen, 0x11, 0x8300, 0x6100);
    screen->setSpriteCard(screen, 0x11, battle->keptCard - 1);
    screen->sprites[0x11].scaleX = 0;
    screen->scaleSprite(screen, 0x11, 10, 0x1000, 0x1000);
    battle->effectStep.state = 1;
}

/* Swaps the two sides' pile apTotal and hpTotal, counting the panels' ap and hp over to each other, then shows message 0x2D until cross or triangle; 1 once done */
s32 CARDGAME_stepSwap(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 ap;
    s32 hp;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->sprites[0x11].state == 1) {
            screen->startBlink(screen, 0x11);
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (screen->sprites[0x11].state == 1) {
            battle->effectStep.state = 3;
            battle->effectStep.time = 0;
        }
        break;
    case 3:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time < 15) {
            screen->setPanelValue(screen, 0, CARD_PANEL_AP, CARDGAME_interpolate(battle->sides[1].pile.apTotal, battle->sides[0].pile.apTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 0, CARD_PANEL_HP, CARDGAME_interpolate(battle->sides[1].pile.hpTotal, battle->sides[0].pile.hpTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 1, CARD_PANEL_AP, CARDGAME_interpolate(battle->sides[0].pile.apTotal, battle->sides[1].pile.apTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 1, CARD_PANEL_HP, CARDGAME_interpolate(battle->sides[0].pile.hpTotal, battle->sides[1].pile.hpTotal, 15, battle->effectStep.time));
        } else {
            ap = battle->sides[0].pile.apTotal;
            battle->sides[0].pile.apTotal = battle->sides[1].pile.apTotal;
            battle->sides[1].pile.apTotal = ap;
            hp = battle->sides[0].pile.hpTotal;
            battle->sides[0].pile.hpTotal = battle->sides[1].pile.hpTotal;
            battle->sides[1].pile.hpTotal = hp;
            screen->setPanelValue(screen, 0, CARD_PANEL_AP, battle->sides[0].pile.apTotal);
            screen->setPanelValue(screen, 0, CARD_PANEL_HP, battle->sides[0].pile.hpTotal);
            screen->setPanelValue(screen, 1, CARD_PANEL_AP, battle->sides[1].pile.apTotal);
            screen->setPanelValue(screen, 1, CARD_PANEL_HP, battle->sides[1].pile.hpTotal);
            screen->scaleSprite(screen, 0x11, 10, 0, 0x1000);
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        if (screen->sprites[0x11].state == 1) {
            battle->effectStep.state = 5;
            screen->openMessage(screen, 0x2D, 0, 0, 1);
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
            battle->effectStep.state = 8;
        }
        break;
    case 8:
        done = 1;
        break;
    }
    return done;
}

/* Adds up the marked slots' ap and hp (at most 99, 20 more for four or more slots) and shows them on sprite 0x11 with card effectStep.vars[4], looked for among the card list's last 100 cards */
void CARDGAME_showSlotTotal(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 i;
    s32 card;
    s32 slot;
    s32 count = 0;
    s32 index;
    s32 base;

    battle->effectStep.vars[1] = 0;
    battle->effectStep.vars[2] = 0;
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (battle->effectStep.eligible[i] != 0) {
            battle->effectStep.vars[1] += battle->players[side].slots[i].ap;
            if (battle->effectStep.vars[1] >= 100) {
                battle->effectStep.vars[1] = 99;
            }
            battle->effectStep.vars[2] += battle->players[side].slots[i].hp;
            if (battle->effectStep.vars[2] >= 100) {
                battle->effectStep.vars[2] = 99;
            }
            count++;
        }
    }
    if (count >= 4) {
        battle->effectStep.vars[1] += 20;
        if (battle->effectStep.vars[1] >= 100) {
            battle->effectStep.vars[1] = 99;
        }
        battle->effectStep.vars[2] += 20;
        if (battle->effectStep.vars[2] >= 100) {
            battle->effectStep.vars[2] = 99;
        }
    }
    screen->addSprite(screen, 0x11, 0x8300, 0x6100);
    index = 0;
    for (card = 89; card < battle->cardCount; card++) {
        if (battle->cards[card] == battle->effectStep.vars[4] - 1) {
            index = card;
            break;
        }
    }
    if (index == 0) {
        index = 87;
        battle->effectStep.vars[4] = 0x13B;
    }
    screen->setSpriteCard(screen, 0x11, index);
    screen->sprites[0x11].color = 5;
    screen->sprites[0x11].ap = battle->effectStep.vars[1];
    screen->sprites[0x11].hp = battle->effectStep.vars[2];
    base = 0;
    if (side != 0) {
        base = 6;
    }
    for (slot = 0; slot < battle->players[side].slotCount; slot++) {
        if (battle->effectStep.eligible[slot] != 0) {
            screen->sprites[base + slot].highlight |= 4;
        }
    }
    screen->sprites[0x11].scaleX = 0;
    battle->effectStep.time = 0;
    battle->effectStep.state = 1;
}

/* Takes the marked slots' sprites away, shows card effectStep.vars[4] in window 2 for 90 frames (or until cross or triangle), then adds effectStep.vars[1] and effectStep.vars[2] to the side's pile apTotal and hpTotal, counting the panel values up; 1 once done */
s32 CARDGAME_stepSlotTotal(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    s32 base;
    s32 i;
    s32 first;
    s32 j;

    switch (battle->effectStep.state) {
    case 1:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 21) {
            battle->effectStep.state = 2;
            base = 0;
            if (side != 0) {
                base = 6;
            }
            for (i = 0; i < battle->players[side].slotCount; i++) {
                if (battle->effectStep.eligible[i] != 0) {
                    screen->startBlink(screen, base + i);
                    battle->effectStep.vars[3] = base + i;
                }
            }
        }
        break;
    case 2:
        if (screen->sprites[battle->effectStep.vars[3]].state == 1) {
            screen->scaleSprite(screen, 0x11, 10, 0x1000, 0x1000);
            battle->effectStep.state = 3;
            first = 0;
            if (side != 0) {
                first = 6;
            }
            for (j = 0; j < battle->players[side].slotCount; j++) {
                if (battle->effectStep.eligible[j] != 0) {
                    screen->sprites[first + j].highlight &= ~4;
                }
            }
        }
        break;
    case 3:
        if (screen->sprites[0x11].state == 1) {
            battle->effectStep.state = 4;
#if PAL_VIDEO
            screen->openWindow(screen, 2, 1, battle->effectStep.vars[4], CARDGAME_cardWindowPositions[SHIFT_PAL_SCREEN][side][0], CARDGAME_cardWindowPositions[SHIFT_PAL_SCREEN][side][1]);
#else
            screen->openWindow(screen, 2, 1, battle->effectStep.vars[4], CARDGAME_cardWindowPositions[0][side][0], CARDGAME_cardWindowPositions[0][side][1]);
#endif
        }
        break;
    case 4:
        if (screen->windows[2].state == 2) {
            battle->effectStep.state = 5;
            battle->effectStep.time = 90;
        }
        break;
    case 5:
        battle->effectStep.time -= GFX.funcs.getFrameTime();
        if (battle->effectStep.time <= 0 || PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 6;
            screen->closeWindow(screen, 2);
        }
        break;
    case 6:
        if (screen->windows[2].state == 0) {
            screen->startBlink(screen, 0x11);
            battle->effectStep.state = 7;
        }
        break;
    case 7:
        if (screen->sprites[0x11].state == 1) {
            battle->effectStep.state = 8;
            battle->effectStep.time = 0;
        }
        break;
    case 8:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time < 20) {
            screen->setPanelValue(screen, side, CARD_PANEL_AP, CARDGAME_interpolate(battle->sides[side].pile.apTotal + battle->effectStep.vars[1], battle->sides[side].pile.apTotal, 20, battle->effectStep.time));
            screen->setPanelValue(screen, side, CARD_PANEL_HP, CARDGAME_interpolate(battle->sides[side].pile.hpTotal + battle->effectStep.vars[2], battle->sides[side].pile.hpTotal, 20, battle->effectStep.time));
        } else {
            battle->sides[side].pile.apTotal += battle->effectStep.vars[1];
            battle->sides[side].pile.hpTotal += battle->effectStep.vars[2];
            screen->setPanelValue(screen, side, CARD_PANEL_AP, battle->sides[side].pile.apTotal);
            screen->setPanelValue(screen, side, CARD_PANEL_HP, battle->sides[side].pile.hpTotal);
            screen->scaleSprite(screen, 0x11, 10, 0, 0x1000);
            battle->effectStep.state = 9;
        }
        break;
    case 9:
        if (screen->sprites[0x11].state == 1) {
            battle->effectStep.state = 10;
        }
        break;
    case 10:
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_stepTotals */
void CARDGAME_startTotals(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.state = 1;
    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = 0;
    battle->effectStep.vars[1] = 0;
    battle->effectStep.vars[2] = 0;
    battle->effectStep.vars[3] = 0;
}

/* Adds up both players' slots' ap and hp and counts the panels' ap and hp over to the totals, which become the sides' pile apTotal and hpTotal; 1 once done */
s32 CARDGAME_stepTotals(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 i;

    switch (battle->effectStep.state) {
    case 1:
        for (i = 0; i < battle->players[0].slotCount; i++) {
            battle->effectStep.vars[0] += battle->players[0].slots[i].ap;
            battle->effectStep.vars[1] += battle->players[0].slots[i].hp;
        }
        for (i = 0; i < battle->players[1].slotCount; i++) {
            battle->effectStep.vars[2] += battle->players[1].slots[i].ap;
            battle->effectStep.vars[3] += battle->players[1].slots[i].hp;
        }
        if (battle->sides[0].pile.apTotal == battle->effectStep.vars[0] && battle->sides[0].pile.hpTotal == battle->effectStep.vars[1] &&
            battle->sides[1].pile.apTotal == battle->effectStep.vars[2] && battle->sides[1].pile.hpTotal == battle->effectStep.vars[3]) {
            battle->effectStep.state = 4;
        } else {
            battle->effectStep.time = 0;
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        if (battle->effectStep.time < 15) {
            SOUND.playSound(SOUND_COUNT);
            screen->setPanelValue(screen, 0, CARD_PANEL_AP, CARDGAME_interpolate(battle->effectStep.vars[0], battle->sides[0].pile.apTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 0, CARD_PANEL_HP, CARDGAME_interpolate(battle->effectStep.vars[1], battle->sides[0].pile.hpTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 1, CARD_PANEL_AP, CARDGAME_interpolate(battle->effectStep.vars[2], battle->sides[1].pile.apTotal, 15, battle->effectStep.time));
            screen->setPanelValue(screen, 1, CARD_PANEL_HP, CARDGAME_interpolate(battle->effectStep.vars[3], battle->sides[1].pile.hpTotal, 15, battle->effectStep.time));
        } else {
            battle->sides[0].pile.apTotal = battle->effectStep.vars[0];
            battle->sides[0].pile.hpTotal = battle->effectStep.vars[1];
            battle->sides[1].pile.apTotal = battle->effectStep.vars[2];
            battle->sides[1].pile.hpTotal = battle->effectStep.vars[3];
            screen->setPanelValue(screen, 0, CARD_PANEL_AP, battle->sides[0].pile.apTotal);
            screen->setPanelValue(screen, 0, CARD_PANEL_HP, battle->sides[0].pile.hpTotal);
            screen->setPanelValue(screen, 1, CARD_PANEL_AP, battle->sides[1].pile.apTotal);
            screen->setPanelValue(screen, 1, CARD_PANEL_HP, battle->sides[1].pile.hpTotal);
            battle->effectStep.state = 3;
        }
        break;
    case 3:
        battle->effectStep.state = 4;
        break;
    case 4:
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_stepRoundEnd */
void CARDGAME_startRoundEnd(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.state = 1;
    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = 0;
    battle->effectStep.vars[1] = 0;
    battle->effectStep.vars[2] = 0;
    battle->effectStep.vars[3] = 0;
    battle->effectStep.vars[4] = 0;
}

/* Takes the side's slots away one by one, then moves both sides' hands back into their decks one at a time (the panels' hand and deck) and shows window 5 until cross or triangle; 1 once done */
s32 CARDGAME_stepRoundEnd(CardBattle *battle, CardScreen *screen, s32 side) {
    s32 done = 0;
    s32 index;
    s32 counting;

    switch (battle->effectStep.state) {
    case 1:
        if (battle->effectStep.vars[4] >= 3) {
            if (battle->effectStep.vars[0] < battle->players[side].slotCount) {
                index = battle->effectStep.vars[0];
                if (side != 0) {
                    index += 6;
                }
                screen->scaleSprite(screen, index, 4, 0, 0x1000);
                CARDGAME_discardSlotCard(battle, screen, side, battle->effectStep.vars[0]);
                battle->effectStep.vars[0]++;
            }
            battle->effectStep.vars[4] -= 3;
        }
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[4] += GFX.funcs.getFrameTime();
        if (battle->effectStep.time >= 61) {
            battle->players[side].slotCount = 0;
            battle->effectStep.time = 0;
            battle->effectStep.state = 2;
            battle->effectStep.vars[4] = 0;
            battle->effectStep.vars[0] = battle->sides[0].pile.handCount;
            battle->effectStep.vars[1] = battle->sides[0].pile.deckCount;
            battle->effectStep.vars[2] = battle->sides[1].pile.handCount;
            battle->effectStep.vars[3] = battle->sides[1].pile.deckCount;
        }
        break;
    case 2:
        battle->effectStep.time += GFX.funcs.getFrameTime();
        battle->effectStep.vars[4] += GFX.funcs.getFrameTime();
        if (battle->effectStep.vars[4] >= 3) {
            counting = 0;
            if (battle->effectStep.vars[0] > 0) {
                counting = 1;
                battle->effectStep.vars[0]--;
                battle->effectStep.vars[1]++;
            }
            if (battle->effectStep.vars[2] > 0) {
                counting = 1;
                battle->effectStep.vars[2]--;
                battle->effectStep.vars[3]++;
            }
            if (!counting) {
                battle->effectStep.state = 4;
                screen->openWindow(screen, 5, 5, 20, 0, 110);
            } else {
                SOUND.playSound(SOUND_COUNT);
            }
            screen->setPanelValue(screen, 0, CARD_PANEL_HAND, battle->effectStep.vars[0]);
            screen->setPanelValue(screen, 0, CARD_PANEL_DECK, battle->effectStep.vars[1]);
            screen->setPanelValue(screen, 1, CARD_PANEL_HAND, battle->effectStep.vars[2]);
            screen->setPanelValue(screen, 1, CARD_PANEL_DECK, battle->effectStep.vars[3]);
            battle->effectStep.vars[4] -= 3;
        }
        break;
    case 3:
        if (screen->windows[5].state == 2) {
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 5;
            screen->closeWindow(screen, 5);
        }
        break;
    case 5:
        if (screen->windows[5].state == 0) {
            battle->effectStep.state = 6;
            screen->closePanels(screen);
        }
        break;
    case 6:
        if (screen->panels[0].state == 0) {
            battle->effectStep.state = 7;
        }
        break;
    case 7:
        done = 1;
        break;
    }
    return done;
}

