/* CARDGAME's card effects on the slots: shaking cards away, sending them
   off or back, their stats, and marking them by colour. */

#include "cardgame/card_battle.h"

/* Counts a card of the colour it has on a side's panel (at most 99); 1 if it has one */
s32 CARDGAME_addColorCount(CardBattle *battle, CardScreen *screen, s32 side, s32 card) {
    CardDrawer drawer;
    CardPile *pile = &battle->sides[side].pile;
    s32 color;
    s32 done = 0;

    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[card] + 1);
    color = drawer.card->color - 1;
    if (color < 5) {
        if (pile->points[color] < 99) {
            pile->points[color]++;
        }
        screen->setPanelValue(screen, side, color, pile->points[color]);
        done = 1;
    }
    return done;
}

/* Shakes sprite index, then shrinks it to nothing; 1 once done */
s32 CARDGAME_stepShakeAway(CardBattle *battle, CardScreen *screen, s32 index) {
    s32 done = 0;

    switch (battle->effectStep.time) {
    case 0:
    default:
        screen->startShake(screen, index);
        battle->effectStep.time = 1;
        break;
    case 1:
        if (screen->sprites[index].state == 1) {
            battle->effectStep.time = 2;
            screen->scaleSprite(screen, index, 5, 0, 0x1000);
        }
        break;
    case 2:
        if (screen->sprites[index].state == 1) {
            done = 1;
        }
        break;
    }
    return done;
}

/* Puts the card of side's slot index on its owner's discards, on
   the panel */
void CARDGAME_discardSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index) {
    switch (battle->players[side].slots[index].side) {
    case 0:
        battle->sides[0].pile.discards[battle->sides[0].pile.discardCount] = battle->players[side].slots[index].card;
        battle->sides[0].pile.discardCount++;
        screen->setPanelValue(screen, 0, CARD_PANEL_DISCARDS, battle->sides[0].pile.discardCount);
        break;
    case 1:
        battle->sides[1].pile.discards[battle->sides[1].pile.discardCount] = battle->players[side].slots[index].card;
        battle->sides[1].pile.discardCount++;
        screen->setPanelValue(screen, 1, CARD_PANEL_DISCARDS, battle->sides[1].pile.discardCount);
        break;
    }
}

/* Shakes side's slot index away, then CARDGAME_discardSlotCard; 1 once done */
s32 CARDGAME_stepDiscardSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index) {
    s32 ok = 0;

    if (CARDGAME_stepShakeAway(battle, screen, side != 0 ? index + 6 : index)) {
        CARDGAME_discardSlotCard(battle, screen, side, index);
        ok = 1;
    }
    return ok;
}

/* Moves sprite index off the left of the screen; 1 once it is there */
s32 CARDGAME_stepSendOff(CardBattle *battle, CardScreen *screen, s32 index) {
    s32 done = 0;

    switch (battle->effectStep.time) {
    case 0:
    default:
        battle->effectStep.time = 1;
        screen->sprites[index].moving = 1;
        screen->startMove(screen, index, 15, -0x5000, 0x6100);
        screen->setSpriteScale(screen, index, 0x1200, 0x1200);
        break;
    case 1:
        if (screen->sprites[index].state == 1) {
            screen->sprites[index].moving = 0;
            screen->sprites[index].scaleX = 0;
            done = 1;
        }
        break;
    }
    return done;
}

/* Puts the card of side's slot index back in its owner's hand, on the panel */
void CARDGAME_returnSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index) {
    switch (battle->players[side].slots[index].side) {
    case 0:
        battle->sides[0].pile.hand[battle->sides[0].pile.handCount] = battle->players[side].slots[index].card;
        battle->sides[0].pile.handCount++;
        screen->setPanelValue(screen, 0, CARD_PANEL_HAND, battle->sides[0].pile.handCount);
        break;
    case 1:
        battle->sides[1].pile.hand[battle->sides[1].pile.handCount] = battle->players[side].slots[index].card;
        battle->sides[1].pile.handCount++;
        screen->setPanelValue(screen, 1, CARD_PANEL_HAND, battle->sides[1].pile.handCount);
        break;
    }
}

/* Sends side's slot index off, then CARDGAME_returnSlotCard; 1 once done */
s32 CARDGAME_stepReturnSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index) {
    s32 ok = 0;

    if (CARDGAME_stepSendOff(battle, screen, side != 0 ? index + 6 : index)) {
        CARDGAME_returnSlotCard(battle, screen, side, index);
        ok = 1;
    }
    return ok;
}

/* Starts effect 3 (which 0) or 4 on the flagged cards in play, and plays a
   sound if there were any */
void CARDGAME_startSlotEffects(CardBattle *battle, CardScreen *screen, s32 which) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (battle->effectStep.marked[i] != 0) {
            if (i < 6) {
                if (i < battle->players[0].slotCount) {
                    screen->startEffect(screen, i, which);
                    found = 1;
                }
            } else if (i - 6 < battle->players[1].slotCount) {
                screen->startEffect(screen, i, which);
                found = 1;
            }
        }
    }
    if (found) {
        switch (which) {
        case 0:
        default:
            SOUND.playSound(0x9C0001);
            break;
        case 1:
            SOUND.playSound(0x9C0000);
            break;
        }
    }
    battle->effectStep.time = 0;
}

/* Counts effectStep.time up by the frame time; whether it passed duration */
s32 CARDGAME_waitFor(CardBattle *battle, CardScreen *screen, s32 duration) {
    battle->effectStep.time += GFX.funcs.getFrameTime();
    return duration < battle->effectStep.time;
}

/* Starts moving the marked slots (effectStep.marked) by an offset packed as x << 16 | y */
void CARDGAME_moveMarkedSlots(CardBattle *battle, CardScreen *screen, s32 offset, s32 mode) {
    s16 dx = offset >> 16;
    s16 dy = offset;
    s32 i;

    battle->effectStep.time = 0;
    battle->effectStep.vars[0] = dx;
    battle->effectStep.vars[1] = dy;
    battle->effectStep.vars[2] = dx;
    if (dx < 0) {
        battle->effectStep.vars[2] = -dx;
    }
    battle->effectStep.vars[3] = dy;
    if (dy < 0) {
        battle->effectStep.vars[3] = -dy;
    }
    for (i = 0; i < 12; i++) {
        if (battle->effectStep.marked[i] != 0) {
            if (i < 6) {
                if (i >= battle->players[0].slotCount) {
                    continue;
                }
                battle->players[0].slots[i].apBonus += dx;
                battle->players[0].slots[i].hpBonus += dy;
                if (mode == 2) {
                    battle->players[0].slots[i].apBonus = battle->players[0].slots[i].ap * -1;
                }
            } else {
                if (i - 6 >= battle->players[1].slotCount) {
                    continue;
                }
                battle->players[1].slots[i - 6].apBonus += dx;
                battle->players[1].slots[i - 6].hpBonus += dy;
                if (mode == 2) {
                    battle->players[1].slots[i - 6].apBonus = battle->players[1].slots[i - 6].ap * -1;
                }
            }
            if (mode == 0) {
                screen->startShake(screen, i);
            } else {
                screen->startRecovery(screen, i);
            }
        }
    }
}

/* Counts the marked slots' ap and hp up or down by one a frame (sign of effectStep.vars[0]/effectStep.vars[1], for effectStep.vars[2]/effectStep.vars[3] frames, clamped to 0..99) with their sprites; when both are over, marks the slots whose hp reached 0 */
s32 CARDGAME_stepSlotStats(CardBattle *battle, CardScreen *screen) {
    s32 done = 1;
    s32 i;

    if (battle->effectStep.time < battle->effectStep.vars[2]) {
        for (i = 0; i < 12; i++) {
            if (battle->effectStep.marked[i] != 0) {
                if (i < 6) {
                    if (i < battle->players[0].slotCount) {
                        if (battle->effectStep.vars[0] > 0) {
                            if (battle->players[0].slots[i].ap < 99) {
                                battle->players[0].slots[i].ap++;
                                screen->sprites[i].ap++;
                            }
                        } else if (battle->players[0].slots[i].ap > 0) {
                            battle->players[0].slots[i].ap--;
                            screen->sprites[i].ap--;
                        }
                    }
                } else if (i - 6 < battle->players[1].slotCount) {
                    if (battle->effectStep.vars[0] > 0) {
                        if (battle->players[1].slots[i - 6].ap < 99) {
                            battle->players[1].slots[i - 6].ap++;
                            screen->sprites[i].ap++;
                        }
                    } else if (battle->players[1].slots[i - 6].ap > 0) {
                        battle->players[1].slots[i - 6].ap--;
                        screen->sprites[i].ap--;
                    }
                }
            }
        }
        done = 0;
    }
    if (battle->effectStep.time < battle->effectStep.vars[3]) {
        for (i = 0; i < 12; i++) {
            if (battle->effectStep.marked[i] != 0) {
                if (i < 6) {
                    if (i < battle->players[0].slotCount) {
                        if (battle->effectStep.vars[1] > 0) {
                            if (battle->players[0].slots[i].hp < 99) {
                                battle->players[0].slots[i].hp++;
                                screen->sprites[i].hp++;
                            }
                        } else if (battle->players[0].slots[i].hp > 0) {
                            battle->players[0].slots[i].hp--;
                            screen->sprites[i].hp--;
                        }
                    }
                } else if (i - 6 < battle->players[1].slotCount) {
                    if (battle->effectStep.vars[1] > 0) {
                        if (battle->players[1].slots[i - 6].hp < 99) {
                            battle->players[1].slots[i - 6].hp++;
                            screen->sprites[i].hp++;
                        }
                    } else if (battle->players[1].slots[i - 6].hp > 0) {
                        battle->players[1].slots[i - 6].hp--;
                        screen->sprites[i].hp--;
                    }
                }
            }
        }
        done = 0;
    }
    battle->effectStep.time++;
    if (done != 0) {
        for (i = 0; i < 12; i++) {
            battle->effectStep.marked[i] = 0;
            if (i < 6) {
                if (i < battle->players[0].slotCount && battle->players[0].slots[i].hp <= 0) {
                    battle->effectStep.marked[i] = 1;
                }
            } else if (i - 6 < battle->players[1].slotCount && battle->players[1].slots[i - 6].hp <= 0) {
                battle->effectStep.marked[i] = 1;
            }
        }
    }
    return done;
}

/* Marks in effectStep.marked the slots out that the last card played applies to, by its target (record.plays[].targetKind): a side, both, or a card colour */
void CARDGAME_markTargetSlots(CardBattle *battle, CardScreen *screen) {
    s32 i;
    s32 n;
    s32 owner;
    s32 card;

    n = battle->record.playCount - 1;
    owner = battle->record.plays[n].side;
    for (i = 0; i < 12; i++) {
        battle->effectStep.marked[i] = 0;
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
        switch (battle->record.plays[n].targetKind) {
        case 1:
            if (owner == 0) {
                if (i < 6) {
                    battle->effectStep.marked[i] = 1;
                }
            } else if (i >= 6) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 2:
            if (owner == 0) {
                if (i >= 6) {
                    battle->effectStep.marked[i] = 1;
                }
            } else if (i < 6) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 3:
            battle->effectStep.marked[i] = 1;
            break;
        case 4:
            if (screen->getCardColor(screen, card) != 1) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 5:
            if (screen->getCardColor(screen, card) != 2) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 6:
            if (screen->getCardColor(screen, card) == 3) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 7:
            if (screen->getCardColor(screen, card) != 4) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 8:
            if (screen->getCardColor(screen, card) == 6) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        }
    }
}

/* Starts a step's first state */
void CARDGAME_beginStep(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.state = 1;
}

/* Removes the slots marked in effectStep.marked: slides the cards after them left on screen, then moves their slots and sprites down and shrinks slotCount; 1 when over */
s32 CARDGAME_removeMarkedSlots(CardBattle *battle, CardScreen *screen) {
    s32 result = 0;
    s32 side, i, j, count;
    /* the match depends on case 3 having variables of its own */
    s32 player, k;
    s32 from, to;
    s8 *marks;
    s8 mark;
    CardSprite sprite;

    switch (battle->effectStep.state) {
    case 1:
        CARDGAME_markedCounts[0] = CARDGAME_markedCounts[1] = 0;
        CARDGAME_slotMoveCounts[0] = CARDGAME_slotMoveCounts[1] = 0;
        for (side = 0; side < 2; side++) {
            marks = &battle->effectStep.marked[side * 6];
            for (i = 0; i < battle->players[side].slotCount; i++) {
                if (marks[i] == 1) {
                    CARDGAME_markedCounts[side]++;
                }
            }
            count = 0;
            for (i = 0; i < battle->players[side].slotCount - 1; i++) {
                if (marks[i] == 1) {
                    for (j = i + 1; j < battle->players[side].slotCount; j++) {
                        if (marks[j] == 0) {
                            mark = marks[i];
                            marks[i] = marks[j];
                            marks[j] = mark;
                            CARDGAME_slotMoves[side][count][0] = i;
                            CARDGAME_slotMoves[side][count][1] = j;
                            count++;
                            break;
                        }
                    }
                }
            }
            CARDGAME_slotMoveCounts[side] = count;
            if (side == 0) {
                for (i = 0; i < count; i++) {
#if PAL_VIDEO
                    screen->startMove(screen, CARDGAME_slotMoves[0][i][1], 10,
                                   CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + CARDGAME_slotMoves[0][i][0] * 0x2900,
                                   CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1]);
#else
                    screen->startMove(screen, CARDGAME_slotMoves[0][i][1], 10, CARDGAME_slotMoves[0][i][0] * 0x2900 + 0x1800, 0x9000);
#endif
                }
            } else {
                for (i = 0; i < count; i++) {
#if PAL_VIDEO
                    screen->startMove(screen, CARDGAME_slotMoves[1][i][1] + 6, 10,
                                   CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + CARDGAME_slotMoves[1][i][0] * 0x2900,
                                   CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3]);
#else
                    screen->startMove(screen, CARDGAME_slotMoves[1][i][1] + 6, 10, CARDGAME_slotMoves[1][i][0] * 0x2900 + 0x1800, 0x3200);
#endif
                }
            }
        }
        if (CARDGAME_markedCounts[0] + CARDGAME_markedCounts[1] != 0) {
            battle->effectStep.state = 2;
            battle->effectStep.time = 20;
        } else {
            battle->effectStep.state = 4;
        }
        break;
    case 2:
        battle->effectStep.time -= GFX.funcs.getFrameTime();
        if (battle->effectStep.time <= 0) {
            battle->effectStep.state = 3;
        }
        break;
    case 3:
        for (player = 0; player < 2; player++) {
            for (k = 0; k < CARDGAME_slotMoveCounts[player]; k++) {
                from = CARDGAME_slotMoves[player][k][0];
                to = CARDGAME_slotMoves[player][k][1];
                battle->players[player].slots[from] = battle->players[player].slots[to];
                from += player * 6;
                to += player * 6;
                sprite = screen->sprites[from];
                screen->sprites[from] = screen->sprites[to];
                screen->sprites[to] = sprite;
            }
            battle->players[player].slotCount -= CARDGAME_markedCounts[player];
            for (k = 0; k < 6; k++) {
                if (k >= battle->players[player].slotCount) {
                    screen->removeSprite(screen, player * 6 + k);
                    screen->sprites[player * 6 + k].scaleX = 0;
                }
            }
        }
        battle->effectStep.state = 4;
        break;
    case 4:
        result = 1;
        break;
    }
    return result;
}

/* Marks in effectStep.eligible the cards of a side's hand (kind 2), deck (3) or discards (4) whose colour has its bit (CARDGAME_colorFlags) in flags (kind 16 cards only with flags bit 0, the others with bit 1); 1 if any */
s32 CARDGAME_markPileCardsByColor(CardBattle *battle, CardScreen *screen, s32 side, s32 kind, s32 flags) {
    CardDrawer drawer;
    s32 found;
    s32 count = 0;
    s32 card;
    s32 i;
    s32 ok;
    s32 j;
    s32 id;

    found = 0;
    switch (kind) {
    case 2:
        count = battle->sides[side].pile.handCount;
        break;
    case 3:
        count = battle->sides[side].pile.deckCount;
        break;
    case 4:
        count = battle->sides[side].pile.discardCount;
        break;
    }
    card = 0;
    for (i = 0; i < count; i++) {
        battle->effectStep.eligible[i] = 0;
        switch (kind) {
        case 2:
            card = battle->sides[side].pile.hand[i];
            break;
        case 3:
            card = battle->sides[side].pile.deck[i + battle->sides[side].pile.deckTop];
            break;
        case 4:
            card = battle->sides[side].pile.discards[i];
            break;
        }
        id = battle->cards[card];
        ok = 0;
        initCardDrawer(&drawer);
        drawer.setCard(id + 1);
        if (drawer.card->kind == 0x10) {
            ok = flags & 1;
        } else if (flags & 2) {
            ok = 1;
        }
        if (ok) {
            for (j = 0; j < 6; j++) {
                if ((flags & CARDGAME_colorFlags[j]) && drawer.card->color == j + 1) {
                    battle->effectStep.eligible[i] = 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    return found;
}

/* Marks in effectStep.eligible the slots of the sides flags picks (0x100 side's own, 0x200 the other) whose card colour has its bit (CARDGAME_colorFlags) in flags; 1 if any */
s32 CARDGAME_markSlotsByColor(CardBattle *battle, CardScreen *screen, s32 side, s32 flags) {
    CardDrawer drawer;
    s32 found = 0;
    s32 card = 0;
    s32 i;
    s32 skip;
    s32 j;

    initCardDrawer(&drawer);
    battle->effectStep.flags = 0;
    for (i = 0, skip = 0; i < 15; i++, skip = 0) {
        battle->effectStep.eligible[i] = 0;
        if (i < 6) {
            if ((side == 0 && (flags & 0x100)) || (side != 0 && (flags & 0x200))) {
                battle->effectStep.flags |= 1;
                if (i < battle->players[0].slotCount) {
                    card = battle->players[0].slots[i].card;
                } else {
                    skip = 1;
                }
            } else {
                skip = 1;
            }
        } else if (i < 12) {
            if ((side == 0 && (flags & 0x200)) || (side != 0 && (flags & 0x100))) {
                battle->effectStep.flags |= 2;
                if (i - 6 < battle->players[1].slotCount) {
                    card = battle->players[1].slots[i - 6].card;
                } else {
                    skip = 1;
                }
            } else {
                skip = 1;
            }
        } else {
            skip = 1;
        }
        if (!skip) {
            drawer.setCard(battle->cards[card] + 1);
            for (j = 0; j < 6; j++) {
                if ((flags & CARDGAME_colorFlags[j]) && drawer.card->color == j + 1) {
                    battle->effectStep.eligible[i] = 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    return found;
}
