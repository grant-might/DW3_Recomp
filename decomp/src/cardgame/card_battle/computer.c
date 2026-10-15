/* The fourth object of CARDGAME.PRO (see effect_script.c), from CARDGAME_checkComputerCondition
   (USA): its rodata starts at 0x80083294, 4 bytes past a multiple of 8. */

#include "cardgame/card_battle.h"

/* The colour of the last card played */
static inline s32 lastPlayColor(CardBattle *battle) {
    CardDrawer drawer;
    s32 card = battle->record.plays[battle->record.playCount - 1].card;

    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[card] + 1);
    return drawer.card->color;
}

/* Checks the condition id (0x70 to 0x96) of a card for the computer; returns 1 if it holds */
s32 CARDGAME_checkComputerCondition(CardBattle *battle, CardScreen *screen, s32 id) {
    s32 result = 0;

    switch (id) {
    case 0x70:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
        result = 1;
        break;
    case 0x80:
    case 0x81:
        if (CARDGAME_markPileCardsByColor(battle, screen, 1, 4, 0xFF)) {
            result = 1;
        }
        break;
    case 0x83:
        if (battle->record.playCount != 0) {
            result = 1;
        }
        break;
    case 0x84:
        if (battle->record.playCount != 0) {
            if (lastPlayColor(battle) == 6) {
                result = 1;
            }
        }
        break;
    case 0x72:
        if (CARDGAME_markPileCardsByColor(battle, screen, 0, 3, 0xFF)) {
            result = 1;
        }
        break;
    case 0x82:
        if (CARDGAME_markPileCardsByColor(battle, screen, 1, 3, 0xFF)) {
            result = 1;
        }
        break;
    case 0x71:
        if (CARDGAME_markPileCardsByColor(battle, screen, 1, 2, 0xFF)) {
            result = 1;
        }
        break;
    case 0x7F:
        if (CARDGAME_markPileCardsByColor(battle, screen, 0, 2, 0xFF)) {
            result = 1;
        }
        break;
    case 0x96:
        if (CARDGAME_markPileCardsByColor(battle, screen, 1, 2, 0xFD)) {
            result = 1;
        }
        break;
    case 0x85:
    case 0x8E:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x1FC)) {
            result = 1;
        }
        break;
    case 0x87:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x180)) {
            result = 1;
        }
        break;
    case 0x88:
    case 0x8F:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x2FC)) {
            result = 1;
        }
        break;
    case 0x89:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x2BC)) {
            result = 1;
        }
        break;
    case 0x8A:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x280)) {
            result = 1;
        }
        break;
    case 0x8B:
    case 0x90:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x3FC)) {
            result = 1;
        }
        break;
    case 0x8C:
    case 0x95:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x380)) {
            result = 1;
        }
        break;
    case 0x91:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x3F8)) {
            result = 1;
        }
        break;
    case 0x92:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x3F4)) {
            result = 1;
        }
        break;
    case 0x93:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x310)) {
            result = 1;
        }
        break;
    case 0x94:
        if (CARDGAME_markSlotsByColor(battle, screen, 1, 0x3DC)) {
            result = 1;
        }
        break;
    }
    return result;
}

/* Scores the cards in a side's slots, counting runs of the same card */
s32 CARDGAME_scoreHand(CardBattle *battle, s32 side, s32 mask) {
    CardSortEntry entries[6];
    CardSortEntry entry;
    CardDrawer drawer;
    s32 count;
    s32 total6;
    s32 total8;
    s32 rest6;
    s32 rest8;
    s32 sum6;
    s32 sum8;
    s32 skip;
    s32 i;
    s32 j;

    total6 = 0;
    rest6 = 0;
    rest8 = 0;
    count = battle->players[side].slotCount;
    initCardDrawer(&drawer);
    total8 = 0;
    for (i = 0; i < count; i++) {
        entries[i].card = battle->cards[battle->players[side].slots[i].card];
        entries[i].slot = i;
    }
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (entries[i].card > entries[j].card) {
                entry = entries[i];
                entries[i] = entries[j];
                entries[j] = entry;
            }
        }
    }
    j = 0;
    i = (mask >> entries[0].slot) & 1;
    sum6 = battle->players[side].slots[entries[0].slot].ap;
    sum8 = battle->players[side].slots[entries[0].slot].hp;
    for (skip = 0; i < count - 1; i++, skip = 0) {
        drawer.setCard(entries[i].card + 1);
        if ((mask >> entries[i + 1].slot) & 1) {
            if (i == count - 2) {
                break;
            }
            skip = 1;
        }
        if (drawer.card->comboCard != 0 && entries[i].card == entries[i + 1 + skip].card) {
            j++;
            sum6 += battle->players[side].slots[entries[i + skip].slot].ap;
            sum8 += battle->players[side].slots[entries[i + skip].slot].hp;
        } else if (j < 2) {
            j = 0;
            if (i + 1 + skip < count) {
                sum6 = battle->players[side].slots[entries[i + 1 + skip].slot].ap;
                sum8 = battle->players[side].slots[entries[i + 1 + skip].slot].hp;
            } else {
                sum6 = 0;
                sum8 = 0;
            }
        } else {
            total6 += sum6;
            total8 += sum8;
            if (i + 1 + skip < count) {
                sum6 = battle->players[side].slots[entries[i + 1 + skip].slot].ap;
                sum8 = battle->players[side].slots[entries[i + 1 + skip].slot].hp;
            } else {
                sum6 = 0;
                sum8 = 0;
            }
            if (j >= 3) {
                total6 += 20;
                total8 += 20;
                j = 0;
                break;
            }
            j = 0;
        }
        if (skip) {
            i++;
        }
    }
    if (j >= 2) {
        total6 += sum6;
        total8 += sum8;
        if (j >= 3) {
            total6 += 20;
            total8 += 20;
        }
    }
    for (i = 0; i < count; i++) {
        if (!((mask >> i) & 1)) {
            rest6 += battle->players[side].slots[i].ap;
            rest8 += battle->players[side].slots[i].hp;
        }
    }
    return rest6 + rest8 + total6 + total8;
}

/* Keeps the flag (effectStep.eligible) only on side's flagged slot that CARDGAME_scoreHand
   rates lowest */
void CARDGAME_keepLowestSlot(CardBattle *battle, s32 side) {
    s32 best = 0xFFF;
    s32 bestIndex = 0;
    s8 *flags;
    s32 i;
    s32 value;

    if (side == 1) {
        flags = &battle->effectStep.eligible[6];
    } else {
        flags = battle->effectStep.eligible;
    }
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            value = CARDGAME_scoreHand(battle, side, 1 << i);
            if (best >= value) {
                best = value;
                flags[bestIndex] = 0;
                bestIndex = i;
                flags[i] = 1;
            } else {
                flags[i] = 0;
            }
        }
    }
}

/* The next play targets (target) side's first flagged slot; whether
   there is one */
s32 CARDGAME_targetFlaggedSlot(CardBattle *battle, s32 side) {
    s32 found = 0;
    s8 *flags;
    s32 i;

    if (side == 1) {
        flags = &battle->effectStep.eligible[6];
    } else {
        flags = battle->effectStep.eligible;
    }
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            found = 1;
            battle->record.plays[battle->record.playCount].target = battle->players[side].slots[i].order;
            break;
        }
    }
    return found;
}

/* CARDGAME_targetFlaggedSlot among side's flagged slots of colour 3 */
s32 CARDGAME_targetColor3Slot(CardBattle *battle, s32 side) {
    CardDrawer drawer;
    s8 *flags;
    s32 i;

    initCardDrawer(&drawer);
    flags = battle->effectStep.eligible;
    if (side == 1) {
        flags = &battle->effectStep.eligible[6];
    }
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            drawer.setCard(battle->cards[battle->players[side].slots[i].card] + 1);
            if (drawer.card->color != 3) {
                flags[i] = 0;
            }
        }
    }
    return CARDGAME_targetFlaggedSlot(battle, side);
}

/* CARDGAME_targetFlaggedSlot among side's flagged slots of colour 4 */
s32 CARDGAME_targetColor4Slot(CardBattle *battle, s32 side) {
    CardDrawer drawer;
    s8 *flags;
    s32 i;

    initCardDrawer(&drawer);
    flags = battle->effectStep.eligible;
    if (side == 1) {
        flags = &battle->effectStep.eligible[6];
    }
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            drawer.setCard(battle->cards[battle->players[side].slots[i].card] + 1);
            if (drawer.card->color != 4) {
                flags[i] = 0;
            }
        }
    }
    return CARDGAME_targetFlaggedSlot(battle, side);
}

/* Whether side's slot index is flagged and its hp is at most value */
s32 CARDGAME_isSlotWithin(CardBattle *battle, s32 side, s32 index, s32 value) {
    s32 result = 0;

    if (*(index + battle->effectStep.eligible) != 0) {
        result = value >= battle->players[side].slots[index].hp;
    }
    return result;
}

/* Unflags side's slots whose hp is over value; when none is left, 1 (and
   the last one flagged again, unless keep) */
s32 CARDGAME_unflagSlotsOver(CardBattle *battle, s32 side, s32 value, s32 keep) {
    s32 result = 0;
    s32 last = 0;
    s8 *flags;
    s32 i;
    s32 any;

    if (side == 1) {
        flags = &battle->effectStep.eligible[6];
    } else {
        flags = battle->effectStep.eligible;
    }
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            last = i;
            if (value < battle->players[side].slots[i].hp) {
                flags[i] = 0;
            }
        }
    }
    any = 0;
    for (i = 0; i < battle->players[side].slotCount; i++) {
        if (flags[i] != 0) {
            any = 1;
            break;
        }
    }
    if (!any) {
        if (keep == 0) {
            flags[last] = 1;
        }
        result = 1;
    }
    return result;
}

/* Picks the opponent's flagged card with the lowest CardImage.rank, the strongest; 1 if
   there is one */
s32 CARDGAME_pickLowestOpponentCard(CardBattle *battle) {
    CardDrawer drawer;
    s32 lowest = 500;
    s32 best = lowest;
    s8 *flags = battle->effectStep.eligible;
    s32 found;
    s32 i;

    found = 0;
    initCardDrawer(&drawer);
    for (i = 0; i < battle->sides[1].pile.handCount; i++) {
        if (flags[i] != 0) {
            drawer.setCard(battle->cards[battle->sides[1].pile.hand[i]] + 1);
            found = 1;
            if (drawer.card->rank < lowest) {
                lowest = drawer.card->rank;
                best = i;
            }
        }
    }
    if (found == 1) {
        battle->record.plays[battle->record.playCount].target = battle->sides[1].pile.hand[best];
    }
    return found;
}

/* The limit card id sets for CARDGAME_compareCards (0 for most) */
s32 CARDGAME_getCardLimit(CardBattle *battle, s32 id) {
    s32 value = 0;

    switch (id) {
    case 0x12:
        value = 60;
        break;
    case 0x14:
        value = 15;
        break;
    case 0x38:
        value = 10;
        break;
    case 0x15:
    case 0x2E:
        value = 30;
        break;
    }
    return value;
}

/* The bonus card id gives in CARDGAME_compareCards (0 for most) */
s32 CARDGAME_getCardBonus(CardBattle *battle, s32 id) {
    s32 value = 0;

    switch (id) {
    case 2:
        value = 15;
        break;
    case 12:
        value = 50;
        break;
    case 15:
        value = 20;
        break;
    case 3:
    case 40:
        value = 30;
        break;
    case 58:
        value = 10;
        break;
    }
    return value;
}

/* The computer's target for card id: a slot picked by the card's rule; 1 once
   there is one */
s32 CARDGAME_pickComputerTarget(CardBattle *battle, CardScreen *screen, s32 id) {
    s32 done = 0;

    switch (id) {
    case 0x27:
        if (CARDGAME_pickLowestOpponentCard(battle)) {
            done = 1;
        }
        break;
    case 0x07:
    case 0x09:
    case 0x1A:
    case 0x37:
    case 0x39:
        CARDGAME_keepLowestSlot(battle, 0);
        if (CARDGAME_targetFlaggedSlot(battle, 0)) {
            done = 1;
        }
        break;
    case 0x15:
    case 0x2E:
        CARDGAME_unflagSlotsOver(battle, 0, 30, 0);
        CARDGAME_keepLowestSlot(battle, 0);
        if (CARDGAME_targetFlaggedSlot(battle, 0)) {
            done = 1;
        }
        break;
    case 0x38:
        CARDGAME_unflagSlotsOver(battle, 0, 10, 0);
        CARDGAME_keepLowestSlot(battle, 0);
        if (CARDGAME_targetFlaggedSlot(battle, 0)) {
            done = 1;
        }
        break;
    case 0x28:
        if (CARDGAME_targetFlaggedSlot(battle, 1)) {
            done = 1;
        }
        break;
    case 0x03:
    case 0x0F:
    case 0x3A:
    case 0x3B:
        if (battle->players[1].slotCount != 0) {
            battle->record.plays[battle->record.playCount].target = battle->players[1].slots[0].order;
            done = 1;
        }
        break;
    default:
        done = 1;
        break;
    }
    return done;
}

/* Compares two cards by the rule of the current play; returns 1 if the second one wins */
s32 CARDGAME_compareCards(CardBattle *battle, s32 id1, s32 id2, s32 flip) {
    s32 index = 0;
    s32 result = 0;
    s32 value;
    s32 bonus;
    s32 i;

    if (flip == 0) {
        value = CARDGAME_getCardLimit(battle, id2);
        bonus = CARDGAME_getCardBonus(battle, id1);
    } else {
        value = CARDGAME_getCardLimit(battle, id1);
        bonus = CARDGAME_getCardBonus(battle, id2);
    }
    switch (battle->record.plays[battle->record.playCount - 1].targetKind) {
    case 0:
        for (i = 0; i < 12; i++) {
            if (i < 6) {
                if (battle->record.plays[battle->record.playCount - 1].target == battle->players[0].slots[i].order) {
                    index = i;
                    break;
                }
            } else {
                if (battle->record.plays[battle->record.playCount - 1].target == battle->players[1].slots[i - 6].order) {
                    index = i - 6;
                    break;
                }
            }
        }
        if (flip == 0) {
            if (CARDGAME_isSlotWithin(battle, 0, index, value)) {
                result = 1;
                battle->record.plays[battle->record.playCount].target = battle->players[0].slots[index].order;
            }
        } else {
            if (CARDGAME_isSlotWithin(battle, 1, index, value) && !CARDGAME_isSlotWithin(battle, 1, index, value - bonus)) {
                result = 1;
                battle->record.plays[battle->record.playCount].target = battle->players[1].slots[index].order;
            }
        }
        break;
    case 1:
        if (flip == 0) {
            CARDGAME_unflagSlotsOver(battle, 0, value, 0);
            CARDGAME_keepLowestSlot(battle, 0);
            if (CARDGAME_targetFlaggedSlot(battle, 0)) {
                result = 1;
            }
        }
        break;
    case 3:
        if (flip == 0) {
            CARDGAME_unflagSlotsOver(battle, 0, value, 0);
            CARDGAME_keepLowestSlot(battle, 0);
            if (CARDGAME_targetFlaggedSlot(battle, 0)) {
                result = 1;
            }
        } else {
            if (CARDGAME_unflagSlotsOver(battle, 1, value, 0) && CARDGAME_targetFlaggedSlot(battle, 1)) {
                result = 1;
            }
        }
        break;
    case 6:
        if (flip == 0) {
            CARDGAME_unflagSlotsOver(battle, 0, value, 0);
            CARDGAME_keepLowestSlot(battle, 0);
            if (CARDGAME_targetColor3Slot(battle, 0)) {
                result = 1;
            }
        }
        break;
    case 7:
        if (flip != 0) {
            CARDGAME_keepLowestSlot(battle, 1);
            if (CARDGAME_unflagSlotsOver(battle, 1, value, 0) && CARDGAME_targetColor4Slot(battle, 1)) {
                result = 1;
            }
        }
        break;
    }
    return result;
}

/* The index of the first card of the opponent's hand of opponentPlans kind (its
   count when none) */
s32 CARDGAME_findOpponentHandKind(CardBattle *battle, s32 kind) {
    s32 i;

    for (i = 0; i < battle->sides[1].pile.handCount; i++) {
        if (battle->opponentPlans[battle->sides[1].pile.hand[i] - 40].kind == kind) {
            break;
        }
    }
    return i;
}

/* Picks the computer's next card, flagging it in effectStep.marked; returns 1 if one was found */
s32 CARDGAME_pickComputerCard(CardBattle *battle, CardScreen *screen) {
    CardDrawer drawer;
    s32 found = 0;
    CardImage *prevCard;
    CardImage *header;
    s32 prev;
    s32 listed;
    s32 kind;
    s32 flip;
    s32 start;
    s32 index;
    s16 card;
    s32 playerScore;
    s32 computerScore;
    s32 i;

    for (i = 0; i < battle->sides[1].pile.handCount; i++) {
        battle->effectStep.marked[i] = 0;
    }
    if (battle->record.playCount == 0) {
        if (battle->phase == CARD_PHASE_PLAYS_BEFORE) {
            start = CARDGAME_findOpponentHandKind(battle, 1);
            kind = 1;
        } else {
            start = CARDGAME_findOpponentHandKind(battle, 3);
            playerScore = CARDGAME_scoreHand(battle, 0, 0);
            computerScore = CARDGAME_scoreHand(battle, 1, 0);
            kind = 3;
            if ((battle->keptCount & 1) || playerScore < computerScore) {
                return 0;
            }
        }
        for (i = start; i < battle->sides[1].pile.handCount; i++) {
            index = battle->sides[1].pile.hand[i];
            if (battle->opponentPlans[index - 40].kind == kind && CARDGAME_canPlayCardKind(battle, index)) {
                card = battle->cards[index];
                if (CARDGAME_checkComputerCondition(battle, screen, CARDGAME_getEffectField(card, 0, 0)) && CARDGAME_pickComputerTarget(battle, screen, card)) {
                    battle->effectStep.marked[i] = 1;
                    found = 1;
                    break;
                }
            }
        }
    } else {
        listed = 0;
        for (i = 0; battle->counterCards[i] != 0xFF; i++) {
            if (battle->counterCards[i] == battle->cards[battle->record.plays[battle->record.playCount - 1].card]) {
                listed = 1;
                break;
            }
        }
        if (listed) {
            for (i = CARDGAME_findOpponentHandKind(battle, 4); i < battle->sides[1].pile.handCount; i++) {
                index = battle->sides[1].pile.hand[i];
                if (CARDGAME_canPlayCardKind(battle, index)) {
                    card = battle->cards[index];
                    if (CARDGAME_checkComputerCondition(battle, screen, CARDGAME_getEffectField(card, 0, 0)) && CARDGAME_pickComputerTarget(battle, screen, card)) {
                        battle->effectStep.marked[i] = 1;
                        found = 1;
                        break;
                    }
                }
            }
        }
        if (found == 0) {
            initCardDrawer(&drawer);
            prev = battle->record.plays[battle->record.playCount - 1].card;
            drawer.setCard(battle->cards[prev] + 1);
            prevCard = drawer.card;
            if (prevCard->kind == 3 || prevCard->kind == 9) {
                start = CARDGAME_findOpponentHandKind(battle, 3);
                kind = 3;
                for (i = start; i < battle->sides[1].pile.handCount; i++) {
                    index = battle->sides[1].pile.hand[i];
                    if ((battle->opponentPlans[index - 40].kind == kind || battle->opponentPlans[index - 40].kind == 4) && battle->opponentPlans[index - 40].flagged != 0) {
                        drawer.setCard(battle->cards[index] + 1);
                        header = drawer.card;
                        if (prevCard->kind == 3) {
                            if (header->kind != 9) {
                                continue;
                            }
                            flip = 0;
                        } else {
                            if (header->kind != 3) {
                                continue;
                            }
                            flip = 1;
                        }
                        if (CARDGAME_canPlayCardKind(battle, index) && CARDGAME_checkComputerCondition(battle, screen, CARDGAME_getEffectField(battle->cards[index], 0, 0)) && CARDGAME_compareCards(battle, prev, index, flip)) {
                            battle->effectStep.marked[i] = 1;
                            found = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    return found;
}
