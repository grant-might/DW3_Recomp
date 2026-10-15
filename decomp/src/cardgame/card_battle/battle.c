/* CARDGAME's card battle (CARDGAME_createBattle): the opponent, the deck
   choice, the rounds, the plays and their results. */

#include "cardgame/card_battle.h"

/* Loads the opponent's data (FILE_CARDGAME_OPPONENTS, entry arg - 1): its deck and its card order */
void CARDGAME_loadOpponent(CardBattle *battle, CardBattleItems *items) {
    CardOpponent *opponent;
    s32 i;

    battle->opponents = FILE_CACHE.load(FILE_CARDGAME_OPPONENTS);
    opponent = &battle->opponents[battle->arg - 1];
    battle->opponentLevel = opponent->level;
    battle->prize = CARDGAME_prizeItems[opponent->prize];
    for (i = 0; i < 40; i++) {
        battle->opponentDraws[i].order = i;
        battle->opponentDraws[i].group = opponent->cards[i].group;
        battle->opponentPlans[i].kind = opponent->cards[i].kind;
        battle->opponentPlans[i].flagged = (opponent->cards[i].card & 0x8000) != 0;
        switch (battle->opponentPlans[i].kind) {
        case 1:
            battle->opponentPlans[i].priority = i + 400;
            break;
        case 2:
            battle->opponentPlans[i].priority = i + 300;
            break;
        case 3:
            battle->opponentPlans[i].priority = i + 500;
            break;
        case 4:
            battle->opponentPlans[i].priority = i + 200;
            break;
        case 5:
            battle->opponentPlans[i].priority = i + 600;
            break;
        case 6:
            battle->opponentPlans[i].priority = i + 100;
            break;
        case 7:
            battle->opponentPlans[i].priority = i + 700;
            break;
        }
    }
    for (i = 0; i < 27; i++) {
        if (opponent->counterCards[i] == 0) {
            battle->counterCards[i] = 7;
            battle->counterCards[i + 1] = 8;
            battle->counterCards[i + 2] = 24;
            battle->counterCards[i + 3] = 31;
            battle->counterCards[i + 4] = 32;
            battle->counterCards[i + 5] = 0;
            battle->counterCards[i + 6] = 6;
            battle->counterCards[i + 7] = 12;
            battle->counterCards[i + 8] = 18;
            battle->counterCards[i + 9] = 27;
            battle->counterCards[i + 10] = 30;
            battle->counterCards[i + 11] = 33;
            battle->counterCards[i + 12] = 37;
            battle->counterCards[i + 13] = 38;
            battle->counterCards[i + 14] = 0xFF;
            break;
        }
        battle->counterCards[i] = opponent->counterCards[i] - 1;
    }
    for (i = 0; i < 40; i++) {
        battle->opponentDeck[i] = (opponent->cards[i].card & 0xFFF) - 1;
    }
}

/* Sorts the opponent's hand by its cards' opponentPlans kind, then value */
void CARDGAME_sortOpponentHand(CardBattle *battle) {
    s32 i;
    s32 j;
    s32 swap;
    s32 card;
    s32 ka;
    s32 kb;
    s32 va;
    s32 vb;

    for (i = 0; i < battle->sides[1].pile.handCount - 1; i++) {
        swap = 0;
        for (j = i + 1; j < battle->sides[1].pile.handCount; j++, swap = 0) {
            ka = battle->opponentPlans[battle->sides[1].pile.hand[i] - 40].kind;
            kb = battle->opponentPlans[battle->sides[1].pile.hand[j] - 40].kind;
            va = battle->opponentPlans[battle->sides[1].pile.hand[i] - 40].priority;
            vb = battle->opponentPlans[battle->sides[1].pile.hand[j] - 40].priority;
            if (kb < ka || (ka == kb && vb < va)) {
                swap = 1;
            }
            if (swap) {
                card = battle->sides[1].pile.hand[i];
                battle->sides[1].pile.hand[i] = battle->sides[1].pile.hand[j];
                battle->sides[1].pile.hand[j] = card;
            }
        }
    }
}

/* drawEnd: the first card of the opponent's deck above the round's level
   (round); reserveStart: the end of its deck before the cards of kind 7 */
void CARDGAME_findOpponentDeckLimits(CardBattle *battle, CardBattleItems *items) {
    s32 i;

    for (i = battle->sides[1].pile.deckTop; i < 40; i++) {
        if (battle->round * 2 + 2 < battle->opponentDraws[i].group) {
            break;
        }
    }
    battle->drawEnd = i;
    for (i = 40; i > 0; i--) {
        if (battle->opponentDraws[i - 1].group != 7) {
            break;
        }
    }
    battle->reserveStart = i;
}

/* Puts side 1's hand back on its pile and renumbers the pile's cards */
void CARDGAME_returnOpponentHand(CardBattle *battle, CardBattleItems *items) {
    CardPile *pile = &battle->sides[1].pile;
    s32 start = pile->deckTop;
    s32 i;
    s32 j;
    s16 card;

    while (pile->handCount > 0) {
        pile->deck[--pile->deckTop] = pile->hand[--pile->handCount];
        pile->deckCount++;
    }
    if (start != pile->deckTop) {
        for (i = pile->deckTop; i < start; i++) {
            battle->opponentDraws[i].group = battle->round * 2 + 1;
        }
    }
    for (i = 0; i < 40; i++) {
        j = pile->deckTop;
        if (battle->opponentDraws[j].group > battle->round * 2 + 2) {
            break;
        }
        card = pile->deck[j];
        while (j < battle->reserveStart - 1) {
            pile->deck[j] = pile->deck[j + 1];
            battle->opponentDraws[j].group = battle->opponentDraws[j + 1].group;
            j++;
        }
        pile->deck[battle->reserveStart - 1] = card;
        battle->opponentDraws[battle->reserveStart - 1].group = 7;
    }
    for (i = 39; i >= 0; i--) {
        battle->opponentDraws[i].order = i;
    }
}

/* Lays out the cards of one of a side's piles (which: 0 the hand; 1 the discards; 2 the deck from deckTop) as sprites in a row */
void CARDGAME_layOutPile(CardBattle *battle, CardBattleItems *items, s32 side, s32 which) {
    CardScreen *screen = items->screen;
    CardAnim *anim = &battle->anim;
    s32 i;

    switch (which) {
    case 0:
        anim->count = battle->sides[side].pile.handCount;
        break;
    case 1:
        anim->count = battle->sides[side].pile.discardCount;
        break;
    case 2:
        anim->count = battle->sides[side].pile.deckCount;
        break;
    }
    anim->time = 0;
    anim->shown = 0;
    anim->stagger = 0;
    anim->duration = anim->count * 4 + 10;
    if (anim->count != 0) {
        for (i = 0; i < 40; i++) {
            if (i < anim->count) {
                screen->addSprite(screen, i, screen->getHandOffset(anim->count, i) + 0x1800, 0x6100);
                switch (which) {
                case 0:
                    screen->setSpriteCard(screen, i, battle->sides[side].pile.hand[i]);
                    break;
                case 1:
                    screen->setSpriteCard(screen, i, battle->sides[side].pile.discards[i]);
                    break;
                case 2:
                    screen->setSpriteCard(screen, i, battle->sides[side].pile.deck[battle->sides[side].pile.deckTop + i]);
                    break;
                }
                if (anim->faceDown != 0) {
                    screen->sprites[i].visible = 2;
                }
                screen->sprites[i].scaleX = 0;
                items->screen->sprites[i].dimmed = battle->anim.dimmed[i];
            } else {
                screen->removeSprite(screen, i);
            }
        }
        anim->faceDown = 0;
    } else {
        anim->faceDown = 0;
        screen->addSprite(screen, 0, screen->getHandOffset(anim->count, 0) + 0x1800, 0x6100);
        screen->sprites[0].visible = 3;
        screen->sprites[0].scaleX = 0;
        for (i = 1; i < 40; i++) {
            screen->removeSprite(screen, i);
        }
    }
}

/* Closes the panels once; 1 once they are closed */
s32 CARDGAME_stepHidePanels(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    if (battle->subState == 0) {
        battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        items->screen->closePanels(items->screen);
        battle->subState++;
    }
    if (items->screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
        done = 1;
    }
    return done;
}

/* Starts the count of side's hand, discards or deck (which 0-2) */
void CARDGAME_startPileCount(CardBattle *battle, CardBattleItems *items, s32 side, s32 which) {
    CardAnim *anim = &battle->anim;

    switch (which) {
    case 0:
        anim->count = battle->sides[side].pile.handCount;
        break;
    case 1:
        anim->count = battle->sides[side].pile.discardCount;
        break;
    case 2:
        anim->count = battle->sides[side].pile.deckCount;
        break;
    }
    anim->time = 0;
    anim->shown = 0;
    anim->stagger = 0;
    anim->duration = anim->count * 4 + 10;
}

/* Marks in each slot sprite's marks[play] whether play of record.plays
   applies to it, by the play's targetKind */
static inline void markPlayTargets(CardBattle *battle, CardScreen *screen, s32 play) {
    CardSlot *slot;
    CardSprite *sprite;
    s32 j;
    s32 first;
    s32 index;
    s32 card;

    switch (battle->record.plays[play].targetKind) {
    case 0:
        for (j = 0; j < 12; j++) {
            if (j < 6) {
                if (j >= battle->players[0].slotCount) {
                    continue;
                }
                slot = &battle->players[0].slots[j];
            } else {
                index = j - 6;
                if (index >= battle->players[1].slotCount) {
                    continue;
                }
                slot = &battle->players[1].slots[index];
            }
            if (slot->order == battle->record.plays[play].target) {
                screen->sprites[j].marks[play] = 1;
            }
        }
        break;
    case 1:
        first = 0;
        if (battle->record.plays[play].side != 0) {
            first = 6;
        }
        sprite = &screen->sprites[first];
        for (j = 0; j < 6; j++) {
            sprite[j].marks[play] = 1;
        }
        break;
    case 2:
        first = 0;
        if (battle->record.plays[play].side == 0) {
            first = 6;
        }
        sprite = &screen->sprites[first];
        for (j = 0; j < 6; j++) {
            sprite[j].marks[play] = 1;
        }
        break;
    case 3:
        for (j = 0; j < 12; j++) {
            screen->sprites[j].marks[play] = 1;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        for (j = 0; j < 12; j++) {
            if (j < 6) {
                card = battle->players[0].slots[j].card;
            } else {
                card = battle->players[1].slots[j - 6].card;
            }
            switch (battle->record.plays[play].targetKind) {
            case 4:
                if (screen->getCardColor(screen, card) != 1) {
                    screen->sprites[j].marks[play] = 1;
                }
                break;
            case 5:
                if (screen->getCardColor(screen, card) != 2) {
                    screen->sprites[j].marks[play] = 1;
                }
                break;
            case 6:
                if (screen->getCardColor(screen, card) == 3) {
                    screen->sprites[j].marks[play] = 1;
                }
                break;
            case 7:
                if (screen->getCardColor(screen, card) != 4) {
                    screen->sprites[j].marks[play] = 1;
                }
                break;
            case 8:
                if (screen->getCardColor(screen, card) == 6) {
                    screen->sprites[j].marks[play] = 1;
                }
                break;
            }
        }
        break;
    case 9:
        break;
    }
}

/* Lays out the slots as sprites (0-5 player 0's, 6-11 player 1's) and each
   play of record.plays (12 on), marking in marks[play] the slot sprites it
   applies to (markPlayTargets) */
void CARDGAME_layOutSlots(CardBattle *battle, CardBattleItems *items) {
    CardScreen *screen = items->screen;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i >= battle->players[0].slotCount) {
            break;
        }
#if PAL_VIDEO
        screen->addSprite(screen, i, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][0] + i * 0x2900, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][1]);
#else
        screen->addSprite(screen, i, 0x1800 + i * 0x2900, 0x9000);
#endif
        screen->setSpriteCard(screen, i, battle->players[0].slots[i].card);
        screen->sprites[i].ap = battle->players[0].slots[i].ap;
        screen->sprites[i].hp = battle->players[0].slots[i].hp;
        screen->sprites[i].scaleX = 0;
    }
    for (i = 0; i < 6; i++) {
        if (i >= battle->players[1].slotCount) {
            break;
        }
#if PAL_VIDEO
        screen->addSprite(screen, i + 6, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][2] + i * 0x2900, CARDGAME_slotRowPositions[SHIFT_PAL_SCREEN][3]);
#else
        screen->addSprite(screen, i + 6, 0x1800 + i * 0x2900, 0x3200);
#endif
        screen->setSpriteCard(screen, i + 6, battle->players[1].slots[i].card);
        screen->sprites[i + 6].ap = battle->players[1].slots[i].ap;
        screen->sprites[i + 6].hp = battle->players[1].slots[i].hp;
        screen->sprites[i + 6].scaleX = 0;
        if (battle->anim.faceDown != 0) {
            screen->sprites[i + 6].visible = 2;
        }
    }
    battle->anim.faceDown = 0;
    for (i = 0; i < battle->record.playCount; i++) {
        screen->addSprite(screen, i + 12, 0x5100 + i * 0x3200, 0x6100);
        screen->setSpriteCard(screen, i + 12, battle->record.plays[i].card);
        screen->sprites[i + 12].scaleX = 0;
        screen->sprites[i + 12].order = i + 1;
        screen->sprites[i + 12].marks[0] = 0;
        screen->sprites[i + 12].marks[1] = 0;
        screen->sprites[i + 12].marks[2] = 0;
        screen->sprites[i + 12].moving = 0;
        if (battle->record.plays[i].mark != 0) {
            screen->sprites[i + 12].marks[battle->record.plays[i].mark] = 1;
        }
        markPlayTargets(battle, screen, i);
    }
}

/* Starts the fade that fade asks for (CARDGAME_fadeColors) */
void CARDGAME_startStepFade(CardBattle *battle, CardBattleItems *items) {
    s32 frames;
    CardFader *fader;
    s32 i;

    if (battle->fade != 0) {
        i = battle->fade - 1;
        frames = 10;
        fader = CARDGAME_createFader(CARDGAME_fadeColors[i].blend);
        items->fader = fader;
        fader->setColor(fader, CARDGAME_fadeColors[i].r, CARDGAME_fadeColors[i].g, CARDGAME_fadeColors[i].b);
        if (CARDGAME_fadeColors[i].blend == 2) {
            frames = 15;
        }
        items->fader->start(items->fader, 0, 0, 0, frames, 1);
        battle->fade = 0;
    }
}

/* Sets the 15 sprites' anim.dimmed to whether anim.dimAll is 2, once it is
   set */
void CARDGAME_resetSpriteFlags(CardBattle *battle) {
    CardAnim *anim = &battle->anim;
    u8 state = anim->dimAll;
    s32 i;

    if (state != 0) {
        for (i = 0; i < 15; i++) {
            battle->anim.dimmed[i] = state == 2;
        }
        anim->dimAll = 0;
    }
}

/* Scales the slots in, a pair every 4 frames, then shows the plays */
static inline void showSlots(CardBattle *battle, CardBattleItems *items, CardAnim *anim) {
    if (anim->stagger >= 4) {
        if (anim->shown < battle->players[0].slotCount) {
            items->screen->scaleSprite(items->screen, anim->shown, 10, 0x1000, 0x1000);
        }
        if (anim->shown < battle->players[1].slotCount) {
            items->screen->scaleSprite(items->screen, anim->shown + 6, 10, 0x1000, 0x1000);
        }
        anim->shown++;
        anim->stagger -= 4;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
    if (anim->duration < anim->time) {
        anim->next = CARD_ANIM_SHOW_PLAYS;
    }
}

/* Scales the plays in, one every 4 frames, opening each one's gauge after it */
static inline void showPlays(CardBattle *battle, CardBattleItems *items, CardAnim *anim) {
    if (anim->stagger >= 4) {
        if (anim->shown < battle->record.playCount) {
            items->screen->scaleSprite(items->screen, anim->shown + 12, 10, 0x1000, 0x1000);
        }
        if (anim->shown - 1 < battle->record.playCount && anim->shown - 1 >= 0) {
            items->screen->openGauge(items->screen, anim->shown - 1, battle->record.plays[anim->shown - 1].side);
        }
        anim->shown++;
        anim->stagger -= 4;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
    if (anim->duration < anim->time) {
        anim->current = CARD_ANIM_NONE;
    }
}

/* Scales the slots out, a pair every 4 frames, then hides the plays */
static inline void hideSlots(CardBattle *battle, CardBattleItems *items, CardAnim *anim) {
    if (anim->stagger >= 4) {
        if (anim->shown < battle->players[0].slotCount) {
            items->screen->scaleSprite(items->screen, anim->shown, 5, 0, 0x1000);
        }
        if (anim->shown < battle->players[1].slotCount) {
            items->screen->scaleSprite(items->screen, anim->shown + 6, 5, 0, 0x1000);
        }
        anim->shown++;
        anim->stagger -= 4;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
    if (anim->duration < anim->time) {
        anim->next = CARD_ANIM_HIDE_PLAYS;
    }
}

/* Scales the plays out with their gauges, one every 4 frames */
static inline void hidePlays(CardBattle *battle, CardBattleItems *items, CardAnim *anim) {
    if (anim->stagger >= 4) {
        if (anim->shown < battle->record.playCount) {
            items->screen->scaleSprite(items->screen, anim->shown + 12, 5, 0, 0x1000);
            items->screen->closeGauge(items->screen, anim->shown);
        }
        anim->shown++;
        anim->stagger -= 4;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
    if (anim->duration < anim->time) {
        anim->current = CARD_ANIM_NONE;
    }
}

/* Scales a pile's cards in, one every 4 frames (at least one) */
static inline void showPile(CardAnim *anim, CardScreen *screen) {
    s32 count;

    count = anim->count;
    if (count == 0) {
        count = 1;
    }
    if (anim->shown < count && anim->stagger >= 4) {
        screen->scaleSprite(screen, anim->shown, 10, 0x1000, 0x1000);
        anim->shown++;
        anim->stagger -= 4;
    }
    if (anim->time > anim->duration) {
        anim->current = CARD_ANIM_NONE;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
}

/* Scales a pile's cards out, one every 4 frames (at least one) */
static inline void hidePile(CardAnim *anim, CardScreen *screen) {
    s32 count;

    count = anim->count;
    if (count == 0) {
        count = 1;
    }
    if (anim->shown < count && anim->stagger >= 4) {
        screen->scaleSprite(screen, anim->shown, 5, 0, 0x1000);
        anim->shown++;
        anim->stagger -= 4;
    }
    if (anim->time > anim->duration) {
        anim->current = CARD_ANIM_NONE;
    }
    anim->time += GFX.funcs.getFrameTime();
    anim->stagger += GFX.funcs.getFrameTime();
}

/* Starts (anim.next) and runs (anim.current) the screen's card animations: the hands', the record's and a pile's sprites scaling in or out */
void CARDGAME_updateCardAnims(CardBattle *battle, CardBattleItems *items) {
    CardAnim *anim = &battle->anim;
    CardScreen *screen = items->screen;
    s32 index = 0;
    s32 i;

    if (anim->next != CARD_ANIM_NONE) {
        switch (anim->next) {
        case CARD_ANIM_SHOW_SLOTS:
            CARDGAME_layOutSlots(battle, items);
            anim->time = 3;
            anim->shown = 0;
            anim->stagger = 4;
            anim->duration = (battle->players[0].slotCount > battle->players[1].slotCount ? battle->players[0].slotCount : battle->players[1].slotCount) * 8;
            for (i = 0; i < 40; i++) {
                items->screen->sprites[i].dimmed = battle->anim.dimmed[i];
            }
            break;
        case CARD_ANIM_SHOW_PLAYS:
            anim->time = 3;
            anim->shown = 0;
            anim->stagger = 4;
            anim->duration = battle->record.playCount * 10;
            break;
        case CARD_ANIM_HIDE_SLOTS:
            anim->time = 3;
            anim->shown = 0;
            anim->stagger = 4;
            anim->duration = (battle->players[0].slotCount > battle->players[1].slotCount ? battle->players[0].slotCount : battle->players[1].slotCount) * 5;
            break;
        case CARD_ANIM_HIDE_PLAYS:
            anim->time = 3;
            anim->shown = 0;
            anim->stagger = 4;
            anim->duration = battle->record.playCount * 5 + 22;
            break;
        case CARD_ANIM_SHOW_HAND:
        case CARD_ANIM_LAY_OUT_HAND:
            index++;
            /* fallthrough */
        case CARD_ANIM_SHOW_DECK:
            index++;
            /* fallthrough */
        case CARD_ANIM_SHOW_DISCARDS:
            index++;
            /* fallthrough */
        case CARD_ANIM_SHOW_OPPONENT_HAND:
        case CARD_ANIM_LAY_OUT_OPPONENT_HAND:
            index++;
            /* fallthrough */
        case CARD_ANIM_SHOW_OPPONENT_DECK:
            index++;
            /* fallthrough */
        case CARD_ANIM_SHOW_OPPONENT_DISCARDS:
            index++;
            CARDGAME_layOutPile(battle, items, CARDGAME_animPiles[index][0], CARDGAME_animPiles[index][1]);
            break;
        case CARD_ANIM_HIDE_HAND:
            CARDGAME_startPileCount(battle, items, 0, 0);
            break;
        case CARD_ANIM_HIDE_DISCARDS:
            CARDGAME_startPileCount(battle, items, 0, 1);
            break;
        case CARD_ANIM_HIDE_DECK:
            CARDGAME_startPileCount(battle, items, 0, 2);
            break;
        case CARD_ANIM_HIDE_OPPONENT_DISCARDS:
            CARDGAME_startPileCount(battle, items, 1, 1);
            break;
        case CARD_ANIM_HIDE_OPPONENT_DECK:
            CARDGAME_startPileCount(battle, items, 1, 2);
            break;
        case CARD_ANIM_HIDE_OPPONENT_HAND:
            CARDGAME_startPileCount(battle, items, 1, 0);
            break;
        }
        anim->hide = anim->next + 1;
        anim->current = anim->next;
        anim->next = CARD_ANIM_NONE;
    }
    switch (anim->current) {
    case CARD_ANIM_SHOW_SLOTS:
        showSlots(battle, items, anim);
        break;
    case CARD_ANIM_SHOW_PLAYS:
        showPlays(battle, items, anim);
        break;
    case CARD_ANIM_HIDE_SLOTS:
        hideSlots(battle, items, anim);
        break;
    case CARD_ANIM_HIDE_PLAYS:
        hidePlays(battle, items, anim);
        break;
    case CARD_ANIM_SHOW_HAND:
    case CARD_ANIM_SHOW_DECK:
    case CARD_ANIM_SHOW_DISCARDS:
    case CARD_ANIM_SHOW_OPPONENT_HAND:
    case CARD_ANIM_SHOW_OPPONENT_DECK:
    case CARD_ANIM_SHOW_OPPONENT_DISCARDS:
        showPile(anim, screen);
        break;
    case CARD_ANIM_HIDE_HAND:
    case CARD_ANIM_HIDE_DECK:
    case CARD_ANIM_HIDE_DISCARDS:
    case CARD_ANIM_HIDE_OPPONENT_HAND:
    case CARD_ANIM_HIDE_OPPONENT_DECK:
    case CARD_ANIM_HIDE_OPPONENT_DISCARDS:
        hidePile(anim, screen);
        break;
    case CARD_ANIM_LAY_OUT_HAND:
        anim->current = CARD_ANIM_NONE;
        anim->hide = CARD_ANIM_HIDE_HAND;
        break;
    case CARD_ANIM_LAY_OUT_OPPONENT_HAND:
        anim->current = CARD_ANIM_NONE;
        anim->hide = CARD_ANIM_HIDE_OPPONENT_HAND;
        break;
    }
}

/* Shows both sides' values on their panels: the points of each colour, the
   deck, hand and discards, AP and HP */
void CARDGAME_showPanelValues(CardBattle *battle, CardBattleItems *items) {
    s32 side;
    s32 i;

    for (side = 0; side < 2; side++) {
        for (i = 0; i < 5; i++) {
            items->screen->setPanelValue(items->screen, side, i, battle->sides[side].pile.points[i]);
        }
        items->screen->setPanelValue(items->screen, side, CARD_PANEL_DECK, battle->sides[side].pile.deckCount);
        items->screen->setPanelValue(items->screen, side, CARD_PANEL_HAND, battle->sides[side].pile.handCount);
        items->screen->setPanelValue(items->screen, side, CARD_PANEL_DISCARDS, battle->sides[side].pile.discardCount);
        items->screen->setPanelValue(items->screen, side, CARD_PANEL_AP, battle->sides[side].pile.apTotal);
        items->screen->setPanelValue(items->screen, side, CARD_PANEL_HP, battle->sides[side].pile.hpTotal);
    }
}

/* Sorts the cards list[from..to) (range: to << 16 | from) by their values,
   moving opponentDraws (flags bit 0) and effectStep.eligible (bit 1) with them */
void CARDGAME_sortCards(CardBattle *battle, s16 *list, s32 range, s32 flags) {
    CardDraw draw;
    s16 id;
    s8 eligible;
    s32 from;
    s32 to;
    s16 *ids;
    s32 i;
    s32 j;

    from = range & 0xFFFF;
    to = range >> 16;
    /* the match depends on this copy of list after from and to: list stays
       in $a1 until here, so from gets $s0 and leaves $a1 to flags & 1 */
    ids = list;
    for (i = from; i < to - 1; i++) {
        for (j = i + 1; j < to; j++) {
            if (battle->cards[ids[i]] > battle->cards[ids[j]]) {
                id = ids[i];
                ids[i] = ids[j];
                ids[j] = id;
                if (flags & 1) {
                    draw = battle->opponentDraws[i];
                    battle->opponentDraws[i] = battle->opponentDraws[j];
                    battle->opponentDraws[j] = draw;
                }
                if (flags & 2) {
                    eligible = battle->effectStep.eligible[i - from];
                    battle->effectStep.eligible[i - from] = battle->effectStep.eligible[j - from];
                    battle->effectStep.eligible[j - from] = eligible;
                }
            }
        }
    }
}

/* Shuffles the player's pile's deck[base..base + n) with 40 random swaps */
void CARDGAME_shufflePile(CardBattle *battle, s32 base, s32 n) {
    CardPile *pile = &battle->sides[0].pile;
    s32 i;
    s32 a;
    s32 b;
    s32 card;

    if (n >= 2) {
        for (i = 0; i < 40; i++) {
            a = RANDOM.next() % n + base;
            b = RANDOM.next() % n + base;
            card = pile->deck[a];
            pile->deck[a] = pile->deck[b];
            pile->deck[b] = card;
        }
    }
}

/* Goes to the next phaseStep after frames frames */
static inline void waitPhaseStep(CardBattle *battle, s32 frames) {
    battle->phaseTime += GFX.funcs.getFrameTime();
    if (battle->phaseTime >= frames) {
        battle->phaseTime = 0;
        battle->phaseStep++;
    }
}

/* Opens deck window index with its sound */
static inline void openDeckWindow(CardBattle *battle, CardBattleItems *items, s32 index) {
    SOUND.playSound(SOUND_MENU_OPEN);
    items->deckWindows[index] = CARDGAME_createDeckWindow(index, CARDGAME_deckWindowPos[index].x, CARDGAME_deckWindowPos[index].y);
    battle->phaseTime = 0;
    battle->phaseStep++;
}

/* Closes deck window index with its sound */
static inline void closeDeckWindow(CardBattle *battle, CardBattleItems *items, s32 index) {
    SOUND.playSound(SOUND_MENU_CLOSE);
    items->deckWindows[index]->close(items->deckWindows[index]);
    battle->phaseTime = 0;
    battle->phaseStep++;
}

/* Up and Down move the marker between the three decks; Cross picks the
   marked one, which blinks */
static inline void moveDeckMarker(CardBattle *battle, CardBattleItems *items) {
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_UP))) |
        (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_UP)))) {
        if (battle->deckChoice > 0) {
            battle->deckChoice--;
            SOUND.playSound(SOUND_MENU_MOVE);
        }
    } else if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_DOWN))) |
               (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_DOWN)))) {
        if (battle->deckChoice < 2) {
            battle->deckChoice++;
            SOUND.playSound(SOUND_MENU_MOVE);
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        battle->phaseTime = 0;
        battle->phaseStep++;
        items->deckWindows[battle->deckChoice]->setBlink(items->deckWindows[battle->deckChoice]);
        items->marker->setFast(items->marker);
    }
    items->marker->setPos(items->marker, CARDGAME_deckWindowPos[battle->deckChoice].x, CARDGAME_deckWindowPos[battle->deckChoice].y);
}

/* Loads the chosen deck (or the default one when it is empty) and the
   opponent's, fills both piles and shuffles the player's */
static inline void loadChosenDeck(CardBattle *battle, CardBattleItems *items) {
    CardPile *pile;
    s32 i;

    if (GAME.decks[battle->deckChoice].cards[0] != 0) {
        for (i = 0; i < 40; i++) {
            battle->playerDeck[i] = GAME.decks[battle->deckChoice].cards[i] - 1;
        }
    } else {
        for (i = 0; i < 40; i++) {
            battle->playerDeck[i] = CARDGAME_defaultDeck[i];
        }
    }
    battle->cardCount = items->screen->loadCardImages(battle->cards, battle->playerDeck, battle->opponentDeck);
    for (i = 0; i < 40; i++) {
        battle->sides[0].pile.deck[i] = i;
        battle->sides[1].pile.deck[i] = i + 40;
    }
    battle->round = 0;
    CARDGAME_findOpponentDeckLimits(battle, items);
    battle->sides[1].pile.deckCount = 40;
    battle->sides[0].pile.deckCount = 40;
    battle->sides[1].pile.deckTop = 0;
    battle->sides[0].pile.deckTop = 0;
    CARDGAME_showPanelValues(battle, items);
    pile = &battle->sides[0].pile;
    battle->shufflePile(battle, pile->deckTop, pile->deckCount);
}

/* Chooses the player's deck: opens the three deck windows and a marker, Up/Down/Cross pick one, then loads both decks' cards; 1 once done */
s32 CARDGAME_chooseDeck(CardBattle *battle, CardBattleItems *items) {
    CardPreloader *preloader;
    s32 done = 0;

    switch (battle->phaseStep) {
    case 0:
    default:
        preloader = items->preloader;
        if (preloader == NULL) {
            battle->phaseStep++;
        } else if (preloader->state == 1 && preloader->ready != 0) {
            battle->phaseStep++;
        }
        break;
    case 1:
        items->screen->openWindow(items->screen, 5, 5, 0x41, 0, 0x26);
        SOUND.playSound(SOUND_MENU_OPEN);
        items->deckWindows[0] = CARDGAME_createDeckWindow(0, CARDGAME_deckWindowPos[0].x, CARDGAME_deckWindowPos[0].y);
        battle->phaseTime = 0;
        battle->phaseStep++;
        break;
    case 2:
        waitPhaseStep(battle, 11);
        break;
    case 3:
        openDeckWindow(battle, items, 1);
        break;
    case 4:
        waitPhaseStep(battle, 11);
        break;
    case 5:
        openDeckWindow(battle, items, 2);
        break;
    case 6:
        waitPhaseStep(battle, 11);
        break;
    case 7:
        items->marker = CARDGAME_createMarker(CARDGAME_deckWindowPos[0].x, CARDGAME_deckWindowPos[0].y);
        battle->phaseTime = 0;
        battle->phaseStep++;
        break;
    case 8:
        battle->phaseTime += GFX.funcs.getFrameTime();
        if (battle->phaseTime >= 11) {
            battle->phaseTime = 0;
            battle->deckChoice = 0;
            battle->phaseStep++;
        }
        break;
    case 9:
        moveDeckMarker(battle, items);
        break;
    case 10:
        waitPhaseStep(battle, 15);
        break;
    case 11:
        items->marker->close(items->marker);
        battle->phaseTime = 0;
        battle->phaseStep++;
        break;
    case 13:
        closeDeckWindow(battle, items, 0);
        break;
    case 14:
        waitPhaseStep(battle, 4);
        break;
    case 15:
        closeDeckWindow(battle, items, 1);
        break;
    case 16:
        waitPhaseStep(battle, 4);
        break;
    case 17:
        closeDeckWindow(battle, items, 2);
        break;
    case 18:
        waitPhaseStep(battle, 4);
        break;
    case 19:
        items->screen->closeWindow(items->screen, 5);
        battle->phaseTime = 0;
        battle->phaseStep++;
        break;
    case 12:
    case 20:
        waitPhaseStep(battle, 6);
        break;
    case 21:
        loadChosenDeck(battle, items);
        done = 1;
        break;
    }
    return done;
}

/* Runs step 0xA7 (CARDGAME_startFirstPick) and then keeps who goes first
   (firstStarter); 1 once done */
s32 CARDGAME_runFirstPick(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    if (battle->phaseStep == 0) {
        battle->effectStep.next = 0xA7;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 1;
    } else {
        if (battle->effectStep.choice == 0) {
            battle->firstStarter = 0;
        } else {
            battle->firstStarter = 1;
        }
        done = 1;
    }
    return done;
}

/* Runs step 20 (CARDGAME_stepStart) and then returns its choice + 1: 1 dealt,
   2 the player's deck was short, 3 the opponent's */
s32 CARDGAME_runStartStep(CardBattle *battle, CardBattleItems *items) {
    s32 result = 0;

    if (battle->phaseStep == 0) {
        battle->effectStep.next = 0x14;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep++;
    } else {
        switch (battle->effectStep.choice) {
        case 0:
            result = 1;
            break;
        case 1:
            result = 2;
            break;
        case 2:
            result = 3;
            break;
        }
    }
    return result;
}

/* Takes the first flagged card out of the hand of side record.turnSide into the play record.plays[playCount]; returns its condition id */
s32 CARDGAME_takeFlaggedCard(CardBattle *battle, CardBattleItems *items) {
    s32 i;
    s32 j;
    s32 side;
    s32 card;
    s16 index;

    for (i = 0; i < 10; i++) {
        if (battle->effectStep.marked[i] != 0) {
            break;
        }
    }
    side = battle->record.turnSide;
    card = battle->cards[battle->sides[side].pile.hand[i]];
    battle->record.plays[battle->record.playCount].side = side;
    index = battle->sides[side].pile.hand[i];
    battle->record.plays[battle->record.playCount].card = index;
    battle->record.plays[battle->record.playCount].targetKind = CARDGAME_getEffectField(battle->cards[index], 3, 0);
    for (j = i; j < battle->sides[side].pile.handCount - 1; j++) {
        battle->sides[side].pile.hand[j] = battle->sides[side].pile.hand[j + 1];
    }
    battle->sides[side].pile.handCount--;
    return CARDGAME_getEffectField(card, 0, 0);
}

/* Flags (effectStep.marked) the slots that the current play's target kind (record.plays[playCount].targetKind) picks */
void CARDGAME_flagTargetSlots(CardBattle *battle, CardScreen *screen) {
    CardSlot *slot;
    s32 card;
    s32 i;

    for (i = 0; i < 12; i++) {
        battle->effectStep.marked[i] = 0;
        switch (battle->record.plays[battle->record.playCount].targetKind) {
        case 0:
            if (i < 6) {
                if (i >= battle->players[0].slotCount) {
                    break;
                }
                slot = &battle->players[0].slots[i];
            } else {
                if (i - 6 >= battle->players[1].slotCount) {
                    break;
                }
                slot = &battle->players[1].slots[i - 6];
            }
            if (slot->order == battle->record.plays[battle->record.playCount].target) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 2:
            if (i < 6 && i < battle->players[0].slotCount) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 1:
            if (i >= 6 && i - 6 < battle->players[1].slotCount) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 3:
            battle->effectStep.marked[i] = 1;
            break;
        case 4:
            if (i < 6) {
                card = battle->players[0].slots[i].card;
            } else {
                card = battle->players[1].slots[i - 6].card;
            }
            if (screen->getCardColor(screen, card) != 1) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 5:
            if (i < 6) {
                card = battle->players[0].slots[i].card;
            } else {
                card = battle->players[1].slots[i - 6].card;
            }
            if (screen->getCardColor(screen, card) != 2) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 6:
            if (i < 6) {
                card = battle->players[0].slots[i].card;
            } else {
                card = battle->players[1].slots[i - 6].card;
            }
            if (screen->getCardColor(screen, card) == 3) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 7:
            if (i < 6) {
                card = battle->players[0].slots[i].card;
            } else {
                card = battle->players[1].slots[i - 6].card;
            }
            if (screen->getCardColor(screen, card) != 4) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 8:
            if (i < 6) {
                card = battle->players[0].slots[i].card;
            } else {
                card = battle->players[1].slots[i - 6].card;
            }
            if (screen->getCardColor(screen, card) == 6) {
                battle->effectStep.marked[i] = 1;
            }
            break;
        case 9:
            break;
        }
    }
}

/* Starts the rounds: clears the record, shows the slots and opens the panels */
static inline void startRounds(CardBattle *battle, CardBattleItems *items) {
    battle->record.turnState = 16;
    battle->record.playCount = 0;
    battle->record.passes = 0;
    battle->record.plays[0].mark = 0;
    battle->record.plays[1].mark = 0;
    battle->record.plays[2].mark = 0;
    battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
    battle->record.starter = battle->record.turnSide = battle->firstStarter;
    items->screen->openPanels(items->screen);
}

/* Picks the side that plays next, by the starter and the plays so far, or
   once three cards are played resolves them */
static inline void startPlay(CardBattle *battle, CardBattleItems *items) {
    switch (battle->record.playCount) {
    case 0:
    case 2:
        if (battle->record.starter != 0) {
            battle->record.turnState = 3;
        } else {
            battle->record.turnState = 2;
        }
        break;
    case 1:
        if (battle->record.starter != 0) {
            battle->record.turnState = 2;
        } else {
            battle->record.turnState = 3;
        }
        break;
    default:
        battle->record.turnState = 12;
        break;
    }
    if (battle->record.playCount != 0) {
        battle->record.passes = 0;
    }
    battle->record.answer = -1;
    items->screen->setPanelValue(items->screen, 0, CARD_PANEL_HAND, battle->sides[0].pile.handCount);
    items->screen->setPanelValue(items->screen, 1, CARD_PANEL_HAND, battle->sides[1].pile.handCount);
}

/* The computer plays a card (CARDGAME_pickComputerCard); without one it
   passes when it would play first, else the plays resolve */
static inline void playComputerTurn(CardBattle *battle, CardBattleItems *items) {
    s32 condition;
    s32 i;

    CARDGAME_sortOpponentHand(battle);
    if (CARDGAME_pickComputerCard(battle, items->screen)) {
        condition = CARDGAME_takeFlaggedCard(battle, items);
        if (condition == 0x83 || condition == 0x84) {
            for (i = 0; i < 15; i++) {
                battle->effectStep.marked[i] = 0;
            }
            battle->effectStep.marked[battle->record.playCount + 11] = 1;
            battle->record.plays[battle->record.playCount - 1].mark = 1;
        }
        if (battle->record.passes != 0) {
            SOUND.playSound(SOUND_MENU_OPEN);
            items->screen->closePanelIcon(items->screen, 0);
        }
        battle->record.turnState = 9;
        CARDGAME_flagTargetSlots(battle, items->screen);
    } else if (battle->record.playCount == 0) {
        battle->record.turnState = 11;
        battle->record.waitTime = 20;
        battle->record.passes++;
        SOUND.playSound(SOUND_MENU_OPEN);
        items->screen->openPanelIcon(items->screen, 1);
    } else {
        battle->record.turnState = 12;
    }
}

/* The player picks a card to play (effect step 0x98); without one they pass
   when they would play first, else the plays resolve */
static inline void playPlayerTurn(CardBattle *battle, CardBattleItems *items) {
    if (battle->record.answer == -1) {
        battle->run = CARD_RUN_STEP;
        battle->effectStep.next = 0x98;
        battle->sortCards(battle, battle->sides[0].pile.hand, battle->sides[0].pile.handCount << 16, 0);
    } else if (battle->record.answer != 0) {
        battle->record.turnState = CARDGAME_turnStates[battle->record.turnSide + 2];
        battle->record.answer = -1;
        if (battle->record.passes != 0) {
            items->screen->panels[1].scale.state = 0;
        }
    } else if (battle->record.playCount == 0) {
        battle->record.turnState = 11;
        battle->record.waitTime = 20;
        battle->record.passes++;
        SOUND.playSound(SOUND_MENU_OPEN);
        items->screen->openPanelIcon(items->screen, 0);
    } else {
        battle->record.turnState = 12;
    }
}

/* Takes the picked card into the play and runs its condition's step: when it
   holds the card is played, else it goes back to the hand */
static inline void takePickedCard(CardBattle *battle, CardBattleItems *items) {
    s32 side;

    if (battle->record.answer == -1) {
        battle->effectStep.next = CARDGAME_takeFlaggedCard(battle, items);
        battle->run = CARD_RUN_STEP;
    } else if (battle->record.answer != 0) {
        battle->record.turnState = CARDGAME_turnStates[battle->record.turnSide + 6];
    } else {
        side = battle->record.turnSide;
        battle->sides[side].pile.hand[battle->sides[side].pile.handCount] = battle->record.plays[battle->record.playCount].card;
        battle->sides[side].pile.handCount++;
        battle->sortCards(battle, battle->sides[0].pile.hand, battle->sides[0].pile.handCount << 16, 0);
        battle->record.turnState = CARDGAME_turnStates[battle->record.turnSide + 2];
        battle->record.answer = -1;
    }
}

/* Starts the next turn, the other side starting; after two passes the rounds
   end (state 15) */
static inline void startNextTurn(CardBattle *battle) {
    battle->record.turnState = 1;
    battle->record.playCount = 0;
    battle->record.plays[0].mark = 0;
    battle->record.plays[1].mark = 0;
    battle->record.plays[2].mark = 0;
    battle->record.starter ^= 1;
    battle->record.turnSide = battle->record.starter;
    battle->record.turns++;
    if (battle->record.passes >= 2) {
        battle->record.turnState = 15;
        battle->subState = 0;
    }
}

/* Runs the rounds of plays (state record.turnState): each side in turn plays a card, the computer through CARDGAME_pickComputerCard; 1 once over */
s32 CARDGAME_playRounds(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    switch (battle->record.turnState) {
    case 0:
    default:
        startRounds(battle, items);
        break;
    case 16:
        if (items->screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            battle->record.turnState = 17;
            items->screen->openWindow(items->screen, 5, 5, battle->phase == CARD_PHASE_PLAYS_BEFORE ? 0x3C : 0x3D, 0, 0x6E);
        }
        break;
    case 17:
        if (items->screen->windows[5].state == 2) {
            if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
                battle->record.turnState = 18;
                items->screen->closeWindow(items->screen, 5);
            }
        }
        break;
    case 18:
        if (items->screen->windows[5].state == 0) {
            battle->record.turnState = 1;
        }
        break;
    case 1:
        startPlay(battle, items);
        break;
    case 3:
        playComputerTurn(battle, items);
        break;
    case 2:
        playPlayerTurn(battle, items);
        break;
    case 4:
    case 5:
        if (battle->record.answer == -1) {
            battle->run = CARD_RUN_STEP;
            battle->effectStep.next = 0x99;
        } else if (battle->record.answer != 0) {
            battle->record.turnState = CARDGAME_turnStates[battle->record.turnSide + 4];
            battle->record.answer = -1;
        } else {
            battle->record.turnState = CARDGAME_turnStates[battle->record.turnSide];
            battle->record.answer = -1;
            if (battle->record.passes != 0) {
                items->screen->panels[1].scale.state = 2;
            }
        }
        break;
    case 6:
    case 7:
        takePickedCard(battle, items);
        break;
    case 9:
        battle->effectStep.next = 0x13;
        battle->run = CARD_RUN_STEP;
        battle->record.turnState = 10;
        break;
    case 8:
        battle->effectStep.next = 0x12;
        battle->run = CARD_RUN_STEP;
        battle->record.turnState = 10;
        break;
    case 10:
        battle->record.turnState = 1;
        battle->record.turnSide ^= 1;
        battle->record.playCount++;
        break;
    case 11:
        if (--battle->record.waitTime <= 0) {
            battle->record.turnState = 14;
        }
        break;
    case 12:
        if (battle->record.playCount > 0) {
            battle->run = CARD_RUN_RESOLVE;
            battle->record.turnState = 13;
            battle->resolveState = 0;
            battle->subTime = 0;
            battle->subState = 0;
            battle->playsToResolve = battle->record.playCount;
        } else {
            battle->record.turnState = 14;
        }
        break;
    case 13:
        battle->record.turnState = 12;
        break;
    case 14:
        startNextTurn(battle);
        break;
    case 15:
        items->screen->closePanelIcon(items->screen, 0);
        items->screen->closePanelIcon(items->screen, 1);
        if (CARDGAME_stepHidePanels(battle, items)) {
            done = 1;
        }
        break;
    }
    return done;
}

/* Clears record.turnState, shows the panel values and sorts the player's hand */
void CARDGAME_prepareTurn(CardBattle *battle, CardBattleItems *items) {
    battle->record.turnState = 0;
    CARDGAME_showPanelValues(battle, items);
    battle->sortCards(battle, battle->sides[0].pile.hand, battle->sides[0].pile.handCount << 16, 0);
}

/* Takes the marked cards (effectStep.marked) out of pile's hand */
void CARDGAME_removeMarkedHandCards(CardBattle *battle, CardPile *pile) {
    s32 i;
    s32 j;

    for (i = pile->handCount - 1; i >= 0; i--) {
        if (battle->effectStep.marked[i] != 0) {
            for (j = i; j < pile->handCount - 1; j++) {
                pile->hand[j] = pile->hand[j + 1];
            }
            pile->handCount--;
        }
    }
}

/* Puts the battle's card card in side's slot index, with its AP and HP and
   the order it came out in */
void CARDGAME_setSlot(CardBattle *battle, s32 side, s32 card, s32 index) {
    CardDrawer drawer;
    CardPlayer *player = &battle->players[side];

    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[card] + 1);
    player->slots[index].ap = drawer.card->ap;
    player->slots[index].hp = drawer.card->hp;
    player->slots[index].apBonus = 0;
    player->slots[index].hpBonus = 0;
    player->slots[index].card = card;
    player->slots[index].owner = side;
    player->slots[index].side = side;
    player->slots[index].order = battle->slotCount++;
}

/* CardBattle.addCard: puts card in side's next slot */
void CARDGAME_addCard(CardBattle *battle, s32 side, s32 card) {
    CardPlayer *player = &battle->players[side];

    CARDGAME_setSlot(battle, side, card, player->slotCount++);
}

/* Puts side's flagged cards of its pile into its slots, up to effectStep.count of them */
void CARDGAME_putOutCards(CardBattle *battle, s32 side) {
    CardPlayer *player = &battle->players[side];
    CardPile *pile = &battle->sides[side].pile;
    s32 i;

    player->slotCount = 0;
    for (i = 0; i < pile->handCount; i++) {
        if (player->slotCount < battle->effectStep.count && battle->effectStep.marked[i] != 0) {
            CARDGAME_addCard(battle, side, pile->hand[i]);
        }
    }
    CARDGAME_removeMarkedHandCards(battle, pile);
}

/* Sets the values of a slot that holds the pass-th card of CARDGAME_countingCards from the battle's counts (0 to 99) */
void CARDGAME_setCountingCardValues(CardBattle *battle, CardSlot *slot, s32 side, s32 pass) {
    CardSide *cardSide = &battle->sides[side];
    CardPlayer *player = &battle->players[side];
    s32 values[2];
    s32 i;
    s32 base;

    if (battle->cards[slot->card] == CARDGAME_countingCards[pass]) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                base = slot->apBonus;
            } else {
                base = slot->hpBonus;
            }
            switch (pass) {
            case 0:
                values[i] = player->slotCount * 20 + base;
                break;
            case 1:
                values[i] = cardSide->pile.handCount * 10 + 10 + base;
                break;
            case 2:
                values[i] = cardSide->pile.discardCount * 20 + 10 + base;
                break;
            case 3:
                values[i] = (battle->players[0].slotCount + battle->players[1].slotCount) * 10 + base;
                break;
            case 4:
                values[i] = (battle->sides[0].pile.discardCount + battle->sides[1].pile.discardCount) * 10 + 10 + base;
                break;
            }
            if (values[i] >= 99) {
                values[i] = 99;
            }
            if (values[i] <= 0) {
                values[i] = 0;
            }
        }
        slot->ap = values[0];
        slot->hp = values[1];
    }
}

/* Sets the values of the counting cards out, pass by pass
   (CARDGAME_setCountingCardValues); whether any card is out */
s32 CARDGAME_setAllCountingValues(CardBattle *battle) {
    s32 found = 0;
    s32 pass;
    s32 side;
    s32 i;
    CardPlayer *player;

    for (pass = 0; pass < 5; pass++) {
        for (side = 0; side < 2; side++) {
            player = &battle->players[side];
            for (i = 0; i < player->slotCount; i++) {
                found = 1;
                CARDGAME_setCountingCardValues(battle, &player->slots[i], side, pass);
            }
        }
    }
    return found;
}

/* Side's apTotal and hpTotal become the sums of its slot cards' ap and hp */
void CARDGAME_sumSlotValues(CardBattle *battle, s32 side) {
    s32 i;

    battle->sides[side].pile.apTotal = 0;
    battle->sides[side].pile.hpTotal = 0;
    for (i = 0; i < battle->players[side].slotCount; i++) {
        battle->sides[side].pile.apTotal += battle->players[side].slots[i].ap;
        battle->sides[side].pile.hpTotal += battle->players[side].slots[i].hp;
    }
}

/* A battle step: each side puts out its flagged cards (putOutCards) between messages 0x9A, 0x9D and 0x9E; 1 once done */
s32 CARDGAME_runPutOut(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    switch (battle->phaseStep) {
    case 0:
        battle->effectStep.next = 0x9A;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 1;
        break;
    case 1:
        battle->putOutCards(battle, 0);
        battle->phaseStep = 2;
        items->screen->setPanelValue(items->screen, 0, CARD_PANEL_HAND, battle->sides[0].pile.handCount);
        break;
    case 2:
        CARDGAME_sortOpponentHand(battle);
        battle->effectStep.next = 0x9D;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 3;
        break;
    case 3:
        battle->putOutCards(battle, 1);
        battle->phaseStep = 4;
        CARDGAME_setAllCountingValues(battle);
        CARDGAME_sumSlotValues(battle, 1);
        CARDGAME_sumSlotValues(battle, 0);
        items->screen->setPanelValue(items->screen, 1, CARD_PANEL_HAND, battle->sides[1].pile.handCount);
        break;
    case 4:
        battle->effectStep.next = 0x9E;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 5;
        break;
    case 5:
        done = 1;
        break;
    }
    return done;
}

/* Finds three or more of one card (with a CardImage.comboCard) among a side's slots, by id from start: flags them in effectStep.eligible; the index after them or -1 */
s32 CARDGAME_findCardSet(CardBattle *battle, s32 side, s32 start) {
    CardSortEntry entries[6];
    CardSortEntry entry;
    CardDrawer drawer;
    s32 count;
    s32 value;
    s32 result;
    s32 i;
    s32 j;
    s32 k;

    result = -1;
    value = 0x51;
    count = battle->players[side].slotCount;
    initCardDrawer(&drawer);
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
    for (i = start; i < count - 1; i++) {
        drawer.setCard(entries[i].card + 1);
        if (drawer.card->comboCard != 0 && entries[i].card == entries[i + 1].card) {
            value = drawer.card->comboCard;
            j++;
        } else if (j < 2) {
            j = 0;
        } else {
            break;
        }
    }
    if (j >= 2) {
        for (k = 0; k < count; k++) {
            battle->effectStep.eligible[k] = 0;
        }
        for (; j >= 0; j--) {
            battle->effectStep.eligible[entries[i - j].slot] = 1;
        }
        battle->effectStep.vars[4] = value;
        result = i + 1;
    }
    return result;
}

/* Opens the panels for a phase */
void CARDGAME_openPanelsPhase(CardBattle *battle, CardBattleItems *items) {
    items->screen->openPanels(items->screen);
    battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
    battle->anim.next = CARD_ANIM_SHOW_SLOTS;
    battle->phaseStep = 0;
}

/* Runs effect step 0x19 for each set of three or more of one card among the
   player's slots (CARDGAME_findCardSet), then goes on to the opponent's */
static inline void runPlayerCardSets(CardBattle *battle) {
    battle->phaseTime = CARDGAME_findCardSet(battle, 0, battle->phaseTime);
    if (battle->phaseTime == -1) {
        battle->phaseStep = 2;
        battle->phaseTime = 0;
    } else {
        battle->effectStep.next = 0x19;
        battle->run = CARD_RUN_STEP;
    }
}

/* Runs effect step 0x1A for each set of three or more of one card among the
   opponent's slots (CARDGAME_findCardSet) */
static inline void runOpponentCardSets(CardBattle *battle) {
    battle->phaseTime = CARDGAME_findCardSet(battle, 1, battle->phaseTime);
    if (battle->phaseTime == -1) {
        battle->phaseStep = 8;
    } else {
        battle->effectStep.next = 0x1A;
        battle->run = CARD_RUN_STEP;
    }
}

/* Runs effect step 0x1B for the kept card (keptCard) until keptCount runs
   out */
static inline void runKeptCardSteps(CardBattle *battle) {
    if (battle->keptCard != 0) {
        battle->effectStep.next = 0x1B;
        battle->run = CARD_RUN_STEP;
        battle->keptCount--;
    }
    if (battle->keptCount == 0) {
        battle->phaseStep = 9;
    } else {
        battle->phaseStep = 8;
    }
}

/* When a side has no slots left, the round ends at once, the higher total
   winning; else the round's result window opens */
static inline void checkEmptyRows(CardBattle *battle, CardScreen *screen) {
    if (battle->players[0].slotCount == 0 || battle->players[1].slotCount == 0) {
        if (battle->sides[0].pile.hpTotal > battle->sides[1].pile.hpTotal) {
            battle->roundWinner = 0;
        } else {
            battle->roundWinner = 1;
        }
        battle->phaseStep = 11;
        if (battle->players[battle->roundWinner].slotCount == 0 && battle->players[battle->roundWinner ^ 1].slotCount != 0) {
            battle->phaseStep = 10;
            battle->phaseTime = 0;
        }
    } else {
        battle->phaseStep = 3;
        screen->openWindow(screen, 5, 5, 13, 0, 110);
    }
}

/* Gives the round to the side with the higher total (pile.hpTotal) and runs
   its win's effect step (0x18 the player's, 0x17 the opponent's) */
static inline void pickRoundWinner(CardBattle *battle) {
    if (battle->sides[0].pile.hpTotal > battle->sides[1].pile.hpTotal) {
        battle->effectStep.next = 0x18;
        battle->roundWinner = 0;
    } else {
        battle->effectStep.next = 0x17;
        battle->roundWinner = 1;
    }
    battle->run = CARD_RUN_STEP;
    battle->phaseStep = 11;
    battle->phaseTime = 0;
}

/* Blinks the round winner's wins on its panel with a flash, then counts the
   win and shows the round's result */
static inline void countRoundWin(CardBattle *battle, CardBattleItems *items, CardScreen *screen) {
    CardFader *fader;
    s32 side;

    side = battle->roundWinner;
    if (battle->phaseTime & 1) {
        items->screen->panels[side].wins = battle->sides[side].pile.wins + 1;
    } else {
        items->screen->panels[side].wins = battle->sides[side].pile.wins;
    }
    if (battle->phaseTime == 0) {
        SOUND.playSound(0x9C0002);
        fader = CARDGAME_createFader(1);
        items->fader = fader;
        fader->setColor(fader, 0x80, 0x80, 0x80);
        items->fader->start(items->fader, 0, 0, 0, 10, 1);
    }
    battle->phaseTime += GFX.funcs.getFrameTime();
    if (battle->phaseTime >= 51) {
        items->screen->panels[side].wins = battle->sides[side].pile.wins + 1;
        battle->sides[side].pile.wins++;
        battle->phaseStep = 12;
        if (battle->roundWinner == 0) {
            screen->openWindow(screen, 5, 5, 16, 0, 110);
        } else if (battle->sides[0].pile.hpTotal != battle->sides[1].pile.hpTotal) {
            screen->openWindow(screen, 5, 5, 17, 0, 110);
        } else {
            screen->openMessage(screen, 18, 0, 0, 1);
        }
    }
}

/* Waits for the round's result: its window, or the draw message when the
   totals are equal */
static inline void waitRoundResultShown(CardBattle *battle, CardScreen *screen) {
    s32 done;

    if (battle->sides[0].pile.hpTotal != battle->sides[1].pile.hpTotal) {
        done = screen->windows[5].state == 2;
    } else {
        done = screen->message.state == 2;
    }
    if (done) {
        battle->phaseStep = 13;
    }
}

/* Cross or Triangle closes the round's result window or message */
static inline void closeRoundResult(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
        if (battle->sides[0].pile.hpTotal != battle->sides[1].pile.hpTotal) {
            screen->closeWindow(screen, 5);
        } else {
            screen->closeMessage(screen);
        }
        battle->phaseStep = 14;
    }
}

/* Once the round's result is closed, opens message 19 */
static inline void waitRoundResultClosed(CardBattle *battle, CardScreen *screen) {
    s32 done;

    if (battle->sides[0].pile.hpTotal != battle->sides[1].pile.hpTotal) {
        done = screen->windows[5].state == 0;
    } else {
        done = screen->message.state == 0;
    }
    if (done) {
        battle->phaseStep = 15;
        screen->openMessage(screen, 19, 0, 0, 1);
    }
}

/* Once a side has two wins, shows who won the battle; else goes on to the
   next round */
static inline void checkBattleWinner(CardBattle *battle, CardScreen *screen) {
    if (screen->message.state == 0) {
        if (battle->sides[0].pile.wins >= 2) {
            SOUND.playSound(SOUND_WIN_JINGLE);
            screen->openWindow(screen, 5, 5, 21, 0, 110);
            battle->phaseStep = 19;
            battle->playerLost = 0;
        } else if (battle->sides[1].pile.wins >= 2) {
            screen->openWindow(screen, 5, 5, 22, 0, 110);
            battle->phaseStep = 19;
            battle->playerLost = 1;
        } else {
            battle->phaseStep = 18;
        }
    }
}

/* Runs the round's closing effect step: 0x1C after the player's win, 0x1D
   after the opponent's */
static inline void runRoundEndStep(CardBattle *battle) {
    if (battle->roundWinner == 0) {
        battle->effectStep.next = 0x1C;
    } else {
        battle->effectStep.next = 0x1D;
    }
    battle->run = CARD_RUN_STEP;
    battle->phaseStep = 26;
}

/* Cross or Triangle closes the battle's result: the player's win goes on to
   the prize, a loss ends it */
static inline void closeBattleResult(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
        if (battle->roundWinner == 0) {
            screen->closeWindow(screen, 5);
            battle->phaseStep = 21;
            battle->subState = 0;
        } else {
            battle->phaseStep = 25;
        }
    }
}

/* The end of a round: shows both sides' cards, gives the round to the higher total (pile.hpTotal) and the battle to the first side with two rounds; 1 for the next round, 2 once the battle is over */
s32 CARDGAME_endRound(CardBattle *battle, CardBattleItems *items) {
    CardScreen *screen = items->screen;
    s32 result = 0;

    switch (battle->phaseStep) {
    case 0:
        if (items->screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
            battle->phaseStep = 1;
            battle->phaseTime = 0;
        }
        break;
    case 1:
        runPlayerCardSets(battle);
        break;
    case 2:
        runOpponentCardSets(battle);
        break;
    case 8:
        runKeptCardSteps(battle);
        break;
    case 9:
        checkEmptyRows(battle, screen);
        break;
    case 3:
        if (screen->windows[5].state == 2) {
            battle->phaseStep = 4;
        }
        break;
    case 4:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->phaseStep = 5;
            screen->closeWindow(screen, 5);
        }
        break;
    case 5:
        if (screen->windows[5].state == 0) {
            battle->phaseStep = 6;
        }
        break;
    case 6:
        battle->effectStep.next = 0x15;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 7;
        break;
    case 7:
        battle->effectStep.next = 0x16;
        battle->run = CARD_RUN_STEP;
        battle->phaseStep = 10;
        battle->phaseTime = 0;
        break;
    case 10:
        pickRoundWinner(battle);
        break;
    case 11:
        countRoundWin(battle, items, screen);
        break;
    case 12:
        waitRoundResultShown(battle, screen);
        break;
    case 13:
        closeRoundResult(battle, screen);
        break;
    case 14:
        waitRoundResultClosed(battle, screen);
        break;
    case 15:
        if (screen->message.state == 2) {
            battle->phaseStep = 16;
        }
        break;
    case 16:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            screen->closeMessage(screen);
            battle->phaseStep = 17;
        }
        break;
    case 17:
        checkBattleWinner(battle, screen);
        break;
    case 18:
        runRoundEndStep(battle);
        break;
    case 19:
        if (screen->windows[5].state == 2) {
            battle->phaseStep = 20;
        }
        break;
    case 20:
        closeBattleResult(battle, screen);
        break;
    case 21:
        if (screen->windows[5].state == 0 && CARDGAME_stepHidePanels(battle, items)) {
            battle->phaseStep = 22;
            screen->openMessage(screen, battle->prize, 0, 0, 3);
        }
        break;
    case 22:
        if (screen->message.state == 2) {
            battle->phaseStep = 23;
        }
        break;
    case 23:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->phaseStep = 25;
        }
        break;
    case 25:
        result = 2;
        break;
    case 26:
        result = 1;
        break;
    }
    return result;
}

/* Puts pile's hand back on top of its deck */
void CARDGAME_returnHandToDeck(CardPile *pile) {
    while (pile->handCount > 0) {
        pile->deck[--pile->deckTop] = pile->hand[--pile->handCount];
        pile->deckCount++;
    }
}

/* Puts both hands back in the decks, shuffles the player's, clears the
   round's values and moves on to the next round (round) */
void CARDGAME_startNextRound(CardBattle *battle, CardBattleItems *items) {
    CardPile *pile = &battle->sides[0].pile;

    CARDGAME_returnHandToDeck(pile);
    CARDGAME_returnOpponentHand(battle, items);
#if VERSION_US
    CARDGAME_findOpponentDeckLimits(battle, items);
#endif
    battle->shufflePile(battle, pile->deckTop, pile->deckCount);
    battle->keptCard = 0;
    battle->keptCount = 0;
    battle->sides[0].pile.apTotal = 0;
    battle->sides[0].pile.hpTotal = 0;
    battle->sides[1].pile.apTotal = 0;
    battle->sides[1].pile.hpTotal = 0;
    CARDGAME_showPanelValues(battle, items);
    battle->round++;
#if VERSION_EU || VERSION_JP
    CARDGAME_findOpponentDeckLimits(battle, items);
#endif
}

/* Starts the step of battle message prevPhase (CARDGAME_battleMessages) */
void CARDGAME_startBattleMessage(CardBattle *battle, CardBattleItems *items) {
    s32 i = battle->prevPhase;

    battle->run = CARD_RUN_STEP;
    battle->effectStep.next = CARDGAME_battleMessages[i].step;
    battle->nextPhase = CARDGAME_battleMessages[i].phase;
}

/* Sets up a card battle: the sides' panels, no result, CARD_PHASE_FADE_IN */
void CARDGAME_initBattle(CardBattle *battle, CardBattleItems *items) {
    battle->sides[0].pile.side = 0;
    battle->sides[1].pile.side = 1;
    battle->result = 0;
    battle->playerLost = 1;
    battle->phase = CARD_PHASE_FADE_IN;
}

/* Runs the battle's current phase (phase), after switching to the one asked for in nextPhase; 1 once the battle is over */
s32 CARDGAME_runPhase(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    if (battle->nextPhase != CARD_PHASE_NONE) {
        switch (battle->nextPhase) {
        case CARD_PHASE_FADE_IN:
        case CARD_PHASE_CHOOSE_DECK:
            battle->phaseStep = 0;
            battle->phaseTime = 0;
            break;
        case CARD_PHASE_FIRST_PICK:
        case CARD_PHASE_DEAL:
        case CARD_PHASE_PUT_OUT:
            battle->phaseStep = 0;
            break;
        case CARD_PHASE_PLAYS_BEFORE:
        case CARD_PHASE_PLAYS_AFTER:
            CARDGAME_prepareTurn(battle, items);
            break;
        case CARD_PHASE_END_ROUND:
            CARDGAME_openPanelsPhase(battle, items);
            break;
        case CARD_PHASE_NEXT_ROUND:
            CARDGAME_startNextRound(battle, items);
            break;
        }
        battle->prevPhase = battle->phase;
        battle->phase = battle->nextPhase;
        battle->nextPhase = CARD_PHASE_NONE;
    }
    switch (battle->phase) {
    case CARD_PHASE_FADE_IN:
        switch (battle->phaseStep) {
        case 0:
        default:
            items->screen->fadeState = 1;
            battle->phaseStep = 1;
            break;
        case 1:
            if (items->screen->fadeState == 2) {
                battle->nextPhase = CARD_PHASE_MESSAGE;
            }
            break;
        }
        break;
    case CARD_PHASE_CHOOSE_DECK:
        if (CARDGAME_chooseDeck(battle, items)) {
            battle->nextPhase = CARD_PHASE_MESSAGE;
        }
        break;
    case CARD_PHASE_FIRST_PICK:
        if (CARDGAME_runFirstPick(battle, items)) {
            battle->nextPhase = CARD_PHASE_MESSAGE;
        }
        break;
    case CARD_PHASE_DEAL:
        switch (CARDGAME_runStartStep(battle, items)) {
        case 1:
            battle->nextPhase = CARD_PHASE_MESSAGE;
            break;
        case 2:
            battle->playerLost = 1;
            done = 1;
            break;
        case 3:
            battle->playerLost = 0;
            done = 1;
            break;
        }
        break;
    case CARD_PHASE_PLAYS_BEFORE:
    case CARD_PHASE_PLAYS_AFTER:
        if (CARDGAME_playRounds(battle, items)) {
            battle->nextPhase = CARD_PHASE_MESSAGE;
        }
        break;
    case CARD_PHASE_PUT_OUT:
        if (CARDGAME_runPutOut(battle, items)) {
            battle->nextPhase = CARD_PHASE_MESSAGE;
        }
        break;
    case CARD_PHASE_END_ROUND:
        switch (CARDGAME_endRound(battle, items)) {
        case 1:
            battle->nextPhase = CARD_PHASE_NEXT_ROUND;
            break;
        case 2:
            done = 1;
            break;
        }
        break;
    case CARD_PHASE_NEXT_ROUND:
        battle->nextPhase = CARD_PHASE_MESSAGE;
        break;
    case CARD_PHASE_MESSAGE:
        CARDGAME_startBattleMessage(battle, items);
        break;
    }
    return done;
}

/* Opens the panels once; 1 once they are open */
s32 CARDGAME_stepShowPanels(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    if (battle->subState == 0) {
        battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
        battle->anim.next = CARD_ANIM_SHOW_SLOTS;
        items->screen->openPanels(items->screen);
        battle->subState++;
    }
    if (items->screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
        done = 1;
    }
    return done;
}

/* Shows the last card played (record.plays) in window 2 for 35 frames (Cross cuts it short), then scales its sprite away; 1 once done */
s32 CARDGAME_showPlayedCard(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;
    s32 place;
    s32 i;

    switch (battle->subState) {
    case 0:
        battle->subState = 1;
        battle->subTime = 0;
        place = battle->record.plays[battle->record.playCount - 1].side;
#if PAL_VIDEO
        items->screen->openWindow(items->screen, 2, 1, battle->cards[battle->record.plays[battle->record.playCount - 1].card] + 1,
                              CARDGAME_playedCardPos[SHIFT_PAL_SCREEN][place].x, CARDGAME_playedCardPos[SHIFT_PAL_SCREEN][place].y);
#else
        items->screen->openWindow(items->screen, 2, 1, battle->cards[battle->record.plays[battle->record.playCount - 1].card] + 1,
                              CARDGAME_playedCardPos[0][place].x, CARDGAME_playedCardPos[0][place].y);
#endif
        for (i = 0; i < 15; i++) {
            items->screen->sprites[i].moving = 0;
        }
        break;
    case 1:
        if (battle->subTime >= 20 && PAD_HELD(PAD_CROSS)) {
            battle->subTime = 35;
        }
        battle->subTime += GFX.funcs.getFrameTime();
        if (battle->subTime >= 35) {
            items->screen->closeWindow(items->screen, 2);
            items->screen->scaleSprite(items->screen, battle->record.playCount + 11, 8, 0x1400, 0x1400);
            items->screen->closeGauge(items->screen, battle->record.playCount - 1);
            battle->subState = 2;
            battle->subTime = 0;
            SOUND.playSound(0x8004603C);
        }
        break;
    case 2:
        battle->subTime += GFX.funcs.getFrameTime();
        if (battle->subTime >= 8) {
            items->screen->startBlink(items->screen, battle->record.playCount + 11);
            battle->subState = 3;
            battle->subTime = 0;
        }
        break;
    case 3:
        battle->subTime += GFX.funcs.getFrameTime();
        if (battle->subTime >= 15) {
            items->screen->scaleSprite(items->screen, battle->record.playCount + 11, 4, 0, 0);
            battle->subState = 5;
            battle->subTime = 0;
            SOUND.playSound(0x8004603C);
        }
        break;
    case 5:
        battle->subTime += GFX.funcs.getFrameTime();
        if (battle->subTime >= 15) {
            done = 1;
        }
        break;
    }
    return done;
}

/* When the played card's play condition holds, shows its message instead of
   its effects; else starts its effects */
static inline void checkPlayedCard(CardBattle *battle, CardBattleItems *items) {
    s16 card;

    card = battle->cards[battle->record.plays[battle->record.playCount - 1].card];
    if (CARDGAME_checkPlayCondition(battle, items->screen, CARDGAME_getEffectField(card, 1, 0))) {
        items->screen->openMessage(items->screen, CARDGAME_getEffectField(card, 2, 0), 0, 1, 1);
        battle->resolveState = 8;
    } else {
        battle->effectPos = 0;
        battle->resolveState = 7;
    }
}

/* Runs the played card's effect steps one after another, then clears its
   marks */
static inline void runCardEffects(CardBattle *battle, CardBattleItems *items) {
    s32 i;
    s32 text;

    text = CARDGAME_getEffectField(battle->cards[battle->record.plays[battle->record.playCount - 1].card], 4, battle->effectPos);
    if (text != 0) {
        battle->effectStep.next = text;
        battle->run = CARD_RUN_STEP;
        battle->effectPos++;
    } else {
        if (battle->record.playCount >= 2) {
            battle->record.plays[battle->record.playCount - 2].mark = 0;
        }
        for (i = 0; i < 12; i++) {
            items->screen->sprites[i].marks[battle->record.playCount - 1] = 0;
        }
        battle->resolveState = 10;
    }
}

/* Puts the played card in its side's discards (unless it is card 13) and
   takes it (and the one before when that was discarded) off the record */
static inline void discardPlayedCard(CardBattle *battle) {
    s32 index;
    s32 side;
    s8 count;

    index = battle->record.plays[battle->record.playCount - 1].card;
    side = battle->record.plays[battle->record.playCount - 1].side;
    if (battle->cards[index] != 13) {
        battle->sides[side].pile.discards[battle->sides[side].pile.discardCount] = index;
        battle->sides[side].pile.discardCount++;
    }
    count = battle->record.playCount;
    battle->record.playCount = count - 1;
    if (battle->prevDiscarded != 0) {
        battle->record.playCount = count - 2;
    }
    battle->resolveState = 13;
    battle->subTime = 0;
    battle->subState = 0;
}

/* Plays out the last card played (record.plays): its effect and its messages, then puts it in its side's discards (pile.discards); 1 once done */
u8 CARDGAME_resolveCard(CardBattle *battle, CardBattleItems *items) {
    u8 done = 0;

    switch (battle->resolveState) {
    case 0:
    default:
        if (CARDGAME_setAllCountingValues(battle)) {
            battle->effectStep.next = 0x5D;
            battle->run = CARD_RUN_STEP;
            battle->resolveState = 1;
            break;
        }
        battle->resolveState = 3;
        /* fallthrough */
    case 3:
        if (items->screen->panels[0].state == 0) {
            battle->resolveState = 4;
        } else {
            battle->resolveState = 5;
            battle->subTime = 0;
            battle->subState = 0;
            battle->prevDiscarded = 0;
        }
        break;
    case 1:
        battle->effectStep.next = 0x4C;
        battle->run = CARD_RUN_STEP;
        battle->resolveState = 2;
        break;
    case 2:
        battle->effectStep.next = 0x5A;
        battle->run = CARD_RUN_STEP;
        battle->resolveState = 3;
        break;
    case 4:
        if (CARDGAME_stepShowPanels(battle, items)) {
            battle->resolveState = 5;
            battle->subTime = 0;
            battle->subState = 0;
            battle->prevDiscarded = 0;
        }
        break;
    case 5:
        if (CARDGAME_showPlayedCard(battle, items)) {
            battle->resolveState = 6;
            battle->subTime = 0;
            battle->subState = 0;
        }
        break;
    case 6:
        checkPlayedCard(battle, items);
        break;
    case 7:
        runCardEffects(battle, items);
        break;
    case 8:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            items->screen->closeMessage(items->screen);
            battle->resolveState = 9;
        }
        break;
    case 9:
        if (items->screen->message.state == 0) {
            battle->resolveState = 12;
        }
        break;
    case 10:
        battle->effectStep.next = 0x5A;
        battle->run = CARD_RUN_STEP;
        battle->resolveState = 11;
        break;
    case 11:
        battle->resolveState = 12;
        break;
    case 12:
        discardPlayedCard(battle);
        break;
    case 13:
        CARDGAME_showPanelValues(battle, items);
        battle->resolveState = 14;
        break;
    case 14:
        if (CARDGAME_setAllCountingValues(battle)) {
            battle->effectStep.next = 0x5D;
            battle->run = CARD_RUN_STEP;
            battle->resolveState = 15;
        } else {
            done = 1;
        }
        break;
    case 15:
        battle->effectStep.next = 0x4C;
        battle->run = CARD_RUN_STEP;
        battle->resolveState = 16;
        break;
    case 16:
        battle->effectStep.next = 0x5A;
        battle->run = CARD_RUN_STEP;
        battle->resolveState = 17;
        break;
    case 17:
        done = 1;
        break;
    }
    return done;
}

/* Up and Down move the menu's cursor (wrapping round its five rows), Cross
   picks the row, Triangle closes the menu */
static inline void moveMenuCursor(CardBattle *battle, CardBattleItems *items) {
    s32 cursor;

    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_UP))) |
        (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_UP)))) {
        cursor = battle->record.menuCursor - 1;
        if (cursor < 0) {
            cursor = 4;
        }
        battle->record.menuCursor = cursor;
        SOUND.playSound(SOUND_CURSOR);
    } else if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_DOWN))) |
               (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_DOWN)))) {
        battle->record.menuCursor = (u8)(battle->record.menuCursor + 1) % 5;
        SOUND.playSound(SOUND_CURSOR);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        items->screen->confirmMenu(items->screen);
        battle->record.menuState = 4;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        items->screen->closeMenu(items->screen);
        battle->record.menuState = 4;
        battle->record.menuCursor = 3;
    }
    items->screen->setMenuRow(items->screen, battle->record.menuCursor);
}

/* Once the menu is closed, goes to the picked row's help, back, or the quit
   question */
static inline void openMenuChoice(CardBattle *battle, CardBattleItems *items) {
    if (items->screen->menuState == 0) {
        switch (battle->record.menuCursor) {
        case 0:
            battle->record.menuState = 5;
            break;
        case 1:
            battle->record.menuState = 6;
            break;
        case 2:
            battle->record.menuState = 7;
            break;
        case 3:
            battle->record.menuState = 15;
            break;
        case 4:
            items->screen->openMessage(items->screen, 0x2C, 1, 0, 1);
            battle->record.menuState = 8;
            break;
        }
    }
}

/* The quit question: Up and Down choose yes or no, Cross answers, Triangle
   answers no */
static inline void answerQuitQuestion(CardBattle *battle, CardBattleItems *items) {
    if (PAD_PRESSED(PAD_CROSS)) {
        items->screen->confirmMessage(items->screen);
        battle->record.menuState = 10;
    } else if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_UP))) |
               (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_UP)))) {
        if (items->screen->message.choice != 0) {
            SOUND.playSound(SOUND_CURSOR);
        }
        items->screen->message.choice = 0;
    } else if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_DOWN))) |
               (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_DOWN)))) {
        if (items->screen->message.choice != 1) {
            SOUND.playSound(SOUND_CURSOR);
        }
        items->screen->message.choice = 1;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        items->screen->message.choice = 1;
        items->screen->confirmMessage(items->screen);
        items->screen->message.choice = 1;
        battle->record.menuState = 10;
    }
}

/* The battle menu (record.menuState its state): three help messages, back, and quitting after a yes/no question; 1 when closed, 2 to quit */
s32 CARDGAME_runBattleMenu(CardBattle *battle, CardBattleItems *items) {
    s32 result = 0;

    switch (battle->record.menuState) {
    case 1:
    default:
        /* save the battle state and the screen's, restored in case 15 */
        battle->savedStep = battle->effectStep;
        battle->record.menuCursor = 0;
        CARDGAME_savedScreenState = items->screen->message;
        items->screen->openMenu(items->screen, 0);
        battle->record.menuState = 2;
        break;
    case 2:
        if (items->screen->menuState == 2) {
            battle->record.menuState = 3;
        }
        break;
    case 3:
        moveMenuCursor(battle, items);
        break;
    case 4:
        openMenuChoice(battle, items);
        break;
    case 5:
        battle->effectStep.next = 0x9B;
        battle->run = CARD_RUN_STEP;
        battle->record.menuState = 11;
        break;
    case 6:
        battle->effectStep.next = 0xA8;
        battle->run = CARD_RUN_STEP;
        battle->record.menuState = 12;
        break;
    case 7:
        battle->effectStep.next = 0x9C;
        battle->run = CARD_RUN_STEP;
        battle->record.menuState = 13;
        break;
    case 8:
        if (items->screen->message.state == 2) {
            battle->record.menuState = 9;
        }
        break;
    case 9:
        answerQuitQuestion(battle, items);
        break;
    case 10:
        if (items->screen->message.state == 0) {
            if (items->screen->message.choice == 0) {
                result = 2;
            } else {
                battle->record.menuState = 14;
            }
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        items->screen->openMenu(items->screen, battle->record.menuCursor);
        battle->record.menuState = 2;
        break;
    case 15:
        battle->effectStep = battle->savedStep;
        result = 1;
        items->screen->message = CARDGAME_savedScreenState;
        battle->record.menuState = 0;
        break;
    }
    return result;
}

/* A frame of the card battle: the sprites' flags, the card animations, the
   fades, then the phase, the effect step, the card's resolution or the battle
   menu; 1 once it is over */
s32 CARDGAME_runBattle(CardBattle *battle, CardBattleItems *items) {
    s32 done = 0;

    CARDGAME_resetSpriteFlags(battle);
    CARDGAME_updateCardAnims(battle, items);
    CARDGAME_startStepFade(battle, items);
    switch (battle->run) {
    case CARD_RUN_PHASE:
    default:
        if (CARDGAME_runPhase(battle, items)) {
            done = 1;
        }
        break;
    case CARD_RUN_STEP:
        CARDGAME_runEffectStep(battle, items->screen);
        break;
    case CARD_RUN_RESOLVE:
        if (CARDGAME_resolveCard(battle, items)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case CARD_RUN_MENU:
        switch (CARDGAME_runBattleMenu(battle, items)) {
        case 1:
            battle->run = CARD_RUN_STEP;
            break;
        case 2:
            done = 1;
            break;
        }
        break;
    }
    return done;
}

/* The card battle task: sets up the battle and fades in, runs it (CARDGAME_runBattle), then fades out and gives the player the prize item for a win; result 2 once over */
void CARDGAME_updateBattle(CardBattle *battle, CardBattleItems *items) {
    TimLoader tim;
    CardFader *fader;

    switch (battle->state) {
    case 0:
    default:
        if (SOUND.isLoading() == 0) {
            FILE_CACHE.load(FILE_CARDGAME_TIMS);
            CARDGAME_initBattle(battle, items);
            CARDGAME_loadOpponent(battle, items);
            items->screen = CARDGAME_createScreen(battle->cards);
            items->preloader = CARDGAME_startPreloader();
            items->screen->resetPanels(items->screen);
            items->screen->opponentLevel = battle->opponentLevel;
            items->screen->opponent = battle->arg;
            initTimLoader(&tim);
            tim.setImagePos(0x280, 0);
            tim.loadArchive(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16));
            fader = CARDGAME_createFader(2);
            items->fader = fader;
            fader->setColor(fader, 0xFF, 0xFF, 0xFF);
            items->fader->start(items->fader, 0, 0, 0, 100, 0);
            battle->setState(battle, 2);
            SOUND.playSound(0x609C0004);
        }
        break;
    case 1:
        if (CARDGAME_runBattle(battle, items)) {
            fader = CARDGAME_createFader(2);
            items->fader = fader;
            fader->setColor(fader, 0, 0, 0);
            items->fader->start(items->fader, 0xFF, 0xFF, 0xFF, 100, 0);
            battle->setState(battle, 2);
            battle->result = 1;
        }
        break;
    case 2:
        if (battle->substate == 0 && items->fader->isDone(items->fader)) {
            items->fader->kill(items->fader);
            if (battle->result != 0) {
                if (battle->result != 2) {
                    if (battle->playerLost == 0) {
                        if (GAME.items[battle->prize] < 99) {
                            GAME.items[battle->prize]++;
                        }
                        FLAGS_00.pendingFlag10 = 1;
                    } else {
                        FLAGS_00.pendingFlag10 = 0;
                    }
                }
                battle->result = 2;
            } else {
                battle->setState(battle, 1);
            }
            battle->substate = 1;
        }
        break;
    case 3:
        if (battle->playerLost == 0) {
            SOUND.stopSound(SOUND_WIN_JINGLE);
        } else {
            SOUND.stopSound(0x609C0004);
        }
        break;
    }
}

/* Creates the card battle against opponent arg (CARDGAME_loadOpponent), with
   its functions, and loads its sound bank */
CardBattle *CARDGAME_createBattle(s32 arg) {
    CardBattle *battle = createTask(CARDGAME_updateBattle, sizeof(CardBattle), 7 * 4);

    battle->shufflePile = CARDGAME_shufflePile;
    battle->sortCards = CARDGAME_sortCards;
    battle->putOutCards = CARDGAME_putOutCards;
    battle->addCard = CARDGAME_addCard;
    battle->scoreHand = CARDGAME_scoreHand;
    battle->arg = arg;
    SOUND.loadBank(0x27);
    return battle;
}
