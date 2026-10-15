/* The first object of CARDGAME.PRO, from the start of the code to
   CARDGAME_runEffectStep (USA). CARDGAME.PRO was four objects:
   each one's jump tables are aligned to 8 from the start of its own rodata,
   and they are 4 bytes past a multiple of 8 from 0x80082A04 (USA) to
   0x80082E30 and from 0x80083294 on. Where each object's code starts is only
   known to be between the function with the last jump table of the object
   before and the one with its first. The second and third objects are cut
   into modules, in their link order (play_checks.c to round.c, then
   battle_steps.c to fader.c), each with its own rodata, a cut only where
   each jump table keeps its place. The data is with the objects that read
   it: effect_script.c's table, then the second object's (data/cardgame_2.c) and
   the third's (data/cardgame_3.c), whose variables at the end the second
   object reads too. */

#include "cardgame/card_battle.h"

/* One field of card effect index (CardEffect): 0 its condition, 1 its play
   condition, 2 its message, 3 its target, 4 its script's step at offset */
s32 CARDGAME_getEffectField(s32 index, u32 field, s32 offset) {
    u8 value = 0;

    switch (field) {
    case 0:
        value = CARDGAME_cardEffects[index].condition;
        break;
    case 1:
        value = CARDGAME_cardEffects[index].playCondition;
        break;
    case 2:
        value = CARDGAME_cardEffects[index].message;
        break;
    case 3:
        value = CARDGAME_cardEffects[index].target;
        break;
    case 4:
        value = CARDGAME_cardEffects[index].script[offset];
        break;
    }
    return value;
}

/* The effect script jumps offset steps (effectPos) when the last card played was side value's */
void CARDGAME_jumpIfLastSide(CardBattle *battle, CardScreen *screen, s32 value, s32 offset) {
    if (battle->record.plays[battle->record.playCount - 1].side == value) {
        battle->effectPos += offset;
    }
}

/* The effect script jumps offset steps (effectPos) when side holds at most value cards */
void CARDGAME_jumpIfFewInHand(CardBattle *battle, s32 side, s32 value, s32 offset) {
    if (value >= battle->sides[side].pile.handCount) {
        battle->effectPos += offset;
    }
}

/* The effect script jumps offset steps (effectPos) when side holds more than value cards */
void CARDGAME_jumpIfManyInHand(CardBattle *battle, s32 side, s32 value, s32 offset) {
    if (value < battle->sides[side].pile.handCount) {
        battle->effectPos += offset;
    }
}

/* The effect script jumps offset steps (effectPos) when side's deck has cards left */
void CARDGAME_jumpIfDeckLeft(CardBattle *battle, s32 side, s32 offset) {
    if (battle->sides[side].pile.deckCount > 0) {
        battle->effectPos += offset;
    }
}

/* The effect script jumps offset steps (effectPos) when side has fewer than six cards out */
void CARDGAME_jumpIfSlotFree(CardBattle *battle, s32 side, s32 offset) {
    if (battle->players[side].slotCount < 6) {
        battle->effectPos += offset;
    }
}

/* Counts effectStep.time down by the frame time; whether it ran out */
s32 CARDGAME_countDown(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.time -= GFX.funcs.getFrameTime();
    return battle->effectStep.time <= 0;
}

/* effectStep.choice becomes the card side which draws next: the player's deck
   top, or the opponent's first card held back (reserveStart) */
void CARDGAME_chooseNextDraw(CardBattle *battle, CardScreen *screen, s32 which) {
    if (which == 0) {
        battle->effectStep.choice = battle->sides[0].pile.deckTop;
    } else {
        battle->effectStep.choice = battle->reserveStart;
    }
}

/* Marks (effectStep.marked) the slots whose order is the last card played's target */
void CARDGAME_markSlotsOfOrder(CardBattle *battle, CardScreen *screen) {
    s32 entry = battle->record.playCount - 1;
    CardSlot *slot;
    s32 i;

    for (i = 0; i < 12; i++) {
        battle->effectStep.marked[i] = 0;
        if (i < 6) {
            if (i >= battle->players[0].slotCount) {
                continue;
            }
            slot = &battle->players[0].slots[i];
        } else {
            if (i - 6 >= battle->players[1].slotCount) {
                continue;
            }
            slot = &battle->players[1].slots[i - 6];
        }
        if (slot->order == battle->record.plays[entry].target) {
            battle->effectStep.marked[i] = 1;
        }
    }
}

/* Marks the opponent's slot that scoreHand rates highest, and keeps it in effectStep.choice */
void CARDGAME_markBestOpponentSlot(CardBattle *battle) {
    s32 best = 0;
    s32 bestIndex = 6;
    s32 value;
    s32 i;

    for (i = 0; i < 12; i++) {
        battle->effectStep.marked[i] = 0;
        if (i >= 6) {
            value = battle->scoreHand(battle, 1, 1 << (i - 6));
            if (best < value) {
                best = value;
                battle->effectStep.marked[bestIndex] = 0;
                bestIndex = i;
                battle->effectStep.choice = i;
                battle->effectStep.marked[i] = 1;
            }
        }
    }
}

/* Effect steps 30-40: CARDGAME_stepColorValue on a colour of the last card's
   side (30-35) or of the other side (36-40) */
s32 CARDGAME_stepEffectColorValue(CardBattle *battle, CardScreen *screen) {
    u8 a = 0;
    u8 b = 0;
    s32 state;

    switch (battle->effectStep.id) {
    case 30:
    case 36:
        b = 0;
        break;
    case 31:
    case 37:
        b = 1;
        break;
    case 32:
    case 33:
    case 38:
        b = 2;
        break;
    case 34:
    case 39:
        b = 3;
        break;
    case 35:
    case 40:
        b = 4;
        break;
    }
    state = battle->effectStep.id;
    if (state >= 30) {
        if (state < 36) {
            a = battle->record.plays[battle->record.playCount - 1].side;
        } else if (state < 41) {
            a = battle->record.plays[battle->record.playCount - 1].side ^ 1;
        }
    }
    return CARDGAME_stepColorValue(battle, screen, a, b);
}

/* Starts the effect step battle->effectStep.next asks for: what it shows,
   moves or waits for before it runs */
static inline void startEffectStep(CardBattle *battle, CardScreen *screen, s32 side, s32 nextSide, s32 other) {
    s32 count;

    switch (battle->effectStep.next) {
    case 1:
        battle->effectStep.time = 45;
        break;
    case 18:
        CARDGAME_startPlayCard(battle, screen, 0);
        break;
    case 19:
        CARDGAME_startPlayCard(battle, screen, 1);
        break;
    case 20:
        CARDGAME_clearHands(battle, screen);
        break;
    case 27:
        CARDGAME_startSwap(battle, screen);
        break;
    case 21:
        CARDGAME_startAttack(battle, screen, 0);
        break;
    case 22:
        CARDGAME_startAttack(battle, screen, 1);
        break;
    case 23:
        CARDGAME_startDiscardAllSlots(battle, screen, 0);
        break;
    case 24:
        CARDGAME_startDiscardAllSlots(battle, screen, 1);
        break;
    case 25:
        CARDGAME_showSlotTotal(battle, screen, 0);
        break;
    case 26:
        CARDGAME_showSlotTotal(battle, screen, 1);
        break;
    case 28:
    case 29:
        CARDGAME_startRoundEnd(battle, screen);
        break;
    case 30:
    case 31:
    case 32:
    case 34:
    case 35:
        CARDGAME_startColorChange(battle, screen, 1, 30);
        break;
    case 33:
        CARDGAME_startColorChange(battle, screen, 2, 30);
        break;
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        CARDGAME_startColorChange(battle, screen, -2, 30);
        break;
    case 41:
        CARDGAME_startColorDrain(battle, screen);
        break;
    case 47:
        CARDGAME_startMessage(battle, screen, 0x39);
        break;
    case 48:
        CARDGAME_startMessage(battle, screen, 0x35);
        break;
    case 49:
        if (side == 0) {
            CARDGAME_startMessage(battle, screen, 0x3B);
        }
        break;
    case 43:
        CARDGAME_startClosePanels(battle, screen, 0);
        break;
    case 45:
        CARDGAME_startClosePanels(battle, screen, 1);
        break;
    case 44:
        CARDGAME_startOpenPanels(battle, screen, 0);
        break;
    case 46:
        CARDGAME_startOpenPanels(battle, screen, 1);
        break;
    case 62:
        CARDGAME_showPileCard(battle, screen, other, 0);
        break;
    case 63:
        CARDGAME_showPileCard(battle, screen, side, 0);
        break;
    case 64:
        CARDGAME_showPileCard(battle, screen, other, 1);
        break;
    case 65:
        CARDGAME_startCloseGauge(battle, screen);
        break;
    case 66:
        CARDGAME_takeCardToHand(battle, screen, side, 3);
        break;
    case 67:
        CARDGAME_takeCardToHand(battle, screen, side, 4);
        break;
    case 68:
        CARDGAME_startDiscardCount(battle, screen, side);
        break;
    case 69:
        CARDGAME_startDraw(battle, screen, 2);
        break;
    case 70:
        /* up to three, less what the pile already holds: the match depends
           on the subtraction being a statement of its own */
        count = 3;
        count -= battle->sides[side].pile.handCount;
        if (count <= 0) {
            count = 0;
        }
        CARDGAME_startDraw(battle, screen, count);
        break;
    case 71:
        CARDGAME_startDraw(battle, screen, 6);
        break;
    case 72:
        CARDGAME_drawMarkedCards(battle, screen, side);
        break;
    case 73:
        CARDGAME_startHandCount(battle, screen, side);
        break;
    case 76:
        CARDGAME_beginStep(battle, screen);
        break;
    case 77:
    case 78:
        CARDGAME_startSlotSweep(battle, screen);
        break;
    case 93:
        battle->effectStep.state = 1;
        break;
    case 90:
        CARDGAME_startTotals(battle, screen);
        break;
    case 79:
        CARDGAME_putSlotCard(battle, screen, side, 0x50);
        break;
    case 80:
        CARDGAME_putSlotCard(battle, screen, side, 0x51);
        break;
    case 81:
        CARDGAME_putSlotCard(battle, screen, side, 0x52);
        break;
    case 82:
        CARDGAME_putSlotCard(battle, screen, side, 0x53);
        break;
    case 83:
        CARDGAME_putSlotCard(battle, screen, side, 0x54);
        break;
    case 84:
        CARDGAME_putSlotCard(battle, screen, side, 0x55);
        break;
    case 85:
        CARDGAME_putSlotCard(battle, screen, side, 0x56);
        break;
    case 86:
        CARDGAME_takeHandCard(battle, screen, side);
        break;
    case 87:
        CARDGAME_copySlotCard(battle, screen, side);
        break;
    case 88:
        CARDGAME_startSlotEffects(battle, screen, 0);
        break;
    case 89:
        CARDGAME_startSlotEffects(battle, screen, 1);
        break;
    case 94:
        CARDGAME_moveMarkedSlots(battle, screen, 0x000A000A, 1);
        break;
    case 95:
        CARDGAME_moveMarkedSlots(battle, screen, 0x0000001E, 1);
        break;
    case 96:
        CARDGAME_moveMarkedSlots(battle, screen, 0x00320032, 1);
        break;
    case 97:
        CARDGAME_moveMarkedSlots(battle, screen, 0x00140014, 1);
        break;
    case 98:
        CARDGAME_moveMarkedSlots(battle, screen, 0x001E001E, 1);
        break;
    case 99:
        CARDGAME_moveMarkedSlots(battle, screen, 0x0000000A, 1);
        break;
    case 100:
        CARDGAME_moveMarkedSlots(battle, screen, 0x000A0000, 1);
        break;
    case 101:
        CARDGAME_moveMarkedSlots(battle, screen, 0x0000FFC4, 0);
        break;
    case 102:
        CARDGAME_moveMarkedSlots(battle, screen, 0x0000FFF1, 0);
        break;
    case 103:
        CARDGAME_moveMarkedSlots(battle, screen, 0x0000FFE2, 0);
        break;
    case 104:
        CARDGAME_moveMarkedSlots(battle, screen, 0x000AFFF6, 0);
        break;
    case 105:
        CARDGAME_moveMarkedSlots(battle, screen, 0xFF9D0000, 2);
        break;
    case 106:
        battle->fade = 1;
        battle->effectStep.time = 10;
        break;
    case 107:
        battle->fade = 2;
        battle->effectStep.time = 10;
        break;
    case 108:
        battle->fade = 3;
        battle->effectStep.time = 10;
        break;
    case 109:
        battle->fade = 4;
        battle->effectStep.time = 10;
        break;
    case 110:
        battle->fade = 5;
        battle->effectStep.time = 15;
        break;
    case 111:
        battle->fade = 6;
        battle->effectStep.time = 10;
        break;
    case 152:
        CARDGAME_startQuestion(battle, screen, 14);
        break;
    case 155:
        CARDGAME_startPick(battle, screen, battle->sides[0].pile.handCount, 5);
        break;
    case 156:
        CARDGAME_startPick(battle, screen, battle->sides[0].pile.discardCount, 9);
        break;
    case 168:
        CARDGAME_startViewTable(battle, screen);
        break;
    case 154:
        CARDGAME_startHandPick(battle, screen, &battle->sides[0].pile);
        break;
    case 157:
        battle->effectStep.count = 0;
        break;
    case 158:
        CARDGAME_startPanelsStep(battle, screen);
        break;
    case 153:
        CARDGAME_startHandPick(battle, screen, &battle->sides[battle->record.turnSide].pile);
        break;
    case 159:
    case 160:
        CARDGAME_startStepMessage(battle, screen, 0);
        break;
    case 161:
        CARDGAME_startStepMessage(battle, screen, 1);
        break;
    case 162:
        CARDGAME_startStepMessage(battle, screen, 2);
        break;
    case 163:
        CARDGAME_startStepMessage(battle, screen, 3);
        break;
    case 164:
        CARDGAME_startStepMessage(battle, screen, 4);
        break;
    case 165:
        CARDGAME_startStepMessage(battle, screen, 5);
        break;
    case 166:
        CARDGAME_startStepMessage(battle, screen, 6);
        break;
    case 167:
        CARDGAME_startFirstPick(battle, screen);
        break;
    case 172:
        CARDGAME_setupCardChoice(battle, screen, side, 2);
        break;
    case 173:
        CARDGAME_setupCardChoice(battle, screen, other, 1);
        break;
    case 169:
        CARDGAME_setupCardChoice(battle, screen, other, 0);
        break;
    case 170:
        CARDGAME_setupCardChoice(battle, screen, side, 0);
        break;
    case 171:
        CARDGAME_setupCardChoice(battle, screen, side, 3);
        break;
    case 174:
        CARDGAME_startPickTableCard(battle, screen, 0);
        break;
    case 112:
        CARDGAME_startPileChoice(battle, screen, 0);
        break;
    case 113:
        CARDGAME_startPileChoice(battle, screen, 0x4000);
        break;
    case 114:
        CARDGAME_startPileChoice(battle, screen, 0x2000);
        break;
    case 115:
        CARDGAME_startPileChoice(battle, screen, 0x1);
        break;
    case 116:
        CARDGAME_startPileChoice(battle, screen, 0x4);
        break;
    case 117:
        CARDGAME_startPileChoice(battle, screen, 0x10);
        break;
    case 118:
        CARDGAME_startPileChoice(battle, screen, 0x40);
        break;
    case 119:
        CARDGAME_startPileChoice(battle, screen, 0x100);
        break;
    case 120:
        CARDGAME_startPileChoice(battle, screen, 0x2);
        break;
    case 121:
        CARDGAME_startPileChoice(battle, screen, 0x8);
        break;
    case 122:
        CARDGAME_startPileChoice(battle, screen, 0x20);
        break;
    case 123:
        CARDGAME_startPileChoice(battle, screen, 0x80);
        break;
    case 124:
        CARDGAME_startPileChoice(battle, screen, 0x200);
        break;
    case 125:
        CARDGAME_startPileChoice(battle, screen, 0x3FF);
        break;
    case 126:
        CARDGAME_startPileChoice(battle, screen, 0xF0000);
        break;
    case 127:
        CARDGAME_startPileChoice(battle, screen, 0x8000);
        break;
    case 129:
        CARDGAME_startPileChoice(battle, screen, 0x400);
        break;
    case 128:
        CARDGAME_startPileChoice(battle, screen, 0x1400);
        break;
    case 130:
        CARDGAME_startPileChoice(battle, screen, 0x1000);
        break;
    case 131:
        CARDGAME_startPreviousCardChoice(battle, screen, 0);
        break;
    case 132:
        CARDGAME_startPreviousCardChoice(battle, screen, 1);
        break;
    case 141:
        CARDGAME_startPickTableCard(battle, screen, 1);
        break;
    case 142:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 1);
        break;
    case 143:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 2);
        break;
    case 144:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 3);
        break;
    case 145:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 4);
        break;
    case 146:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 5);
        break;
    case 147:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 6);
        break;
    case 148:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 7);
        break;
    case 149:
        CARDGAME_showTargetSlots(battle, screen, nextSide, 8);
        break;
    case 151:
        CARDGAME_setupSideChoice(battle, screen, nextSide);
        break;
    }
}

/* The effect script goes back four steps (effectPos) when side other holds a
   card of another colour than 5 */
static inline void jumpBackForOtherColors(CardBattle *battle, s32 other) {
    CardDrawer drawer;
    s32 i;

    initCardDrawer(&drawer);
    for (i = 0; i < battle->sides[other].pile.handCount; i++) {
        drawer.setCard(battle->cards[battle->sides[other].pile.hand[i]] + 1);
        if (drawer.card->color != 5) {
            battle->effectPos -= 4;
            break;
        }
    }
}

/* effectStep.choice becomes the first card of side other's hand that can be picked (effectStep.eligible) */
static inline void chooseFirstEligible(CardBattle *battle, s32 other) {
    s32 i;

    for (i = 0; i < battle->sides[other].pile.handCount; i++) {
        if (battle->effectStep.eligible[i] != 0) {
            battle->effectStep.choice = i;
            break;
        }
    }
}

/* Ends a step that has an answer: result, once it is no longer -1 */
static inline void answerOnceDone(CardBattle *battle, s32 result) {
    if (result != -1) {
        battle->record.answer = result;
        battle->run = CARD_RUN_PHASE;
    }
}

/* Effect step 141: the player picks a card on the table; the answer is 0 when
   there was none or the player backed out, or 1 with the picked slot's order as the next play's target */
static inline void answerTableCardPick(CardBattle *battle, CardScreen *screen) {
    s32 index;

    switch (CARDGAME_pickTableCard(battle, screen)) {
    case 1:
        battle->record.answer = 0;
        battle->run = CARD_RUN_PHASE;
        break;
    case 2:
        index = battle->effectStep.choice;
        if (index < 6) {
            battle->record.plays[battle->record.playCount].target = battle->players[0].slots[index].order;
        } else {
            index -= 6;
            battle->record.plays[battle->record.playCount].target = battle->players[1].slots[index].order;
        }
        battle->record.answer = 1;
        battle->run = CARD_RUN_PHASE;
        break;
    }
}

/* Effect step 151: a card is picked from nextSide's hand; the answer is 0 when
   the player backed out, or 1 with the picked card as the next play's target */
static inline void answerHandCardPick(CardBattle *battle, CardScreen *screen, s32 nextSide) {
    switch (CARDGAME_chooseCard(battle, screen, 1)) {
    case 1:
        battle->record.answer = 0;
        battle->run = CARD_RUN_PHASE;
        break;
    case 2:
        if (nextSide == 0) {
            battle->record.plays[battle->record.playCount].target = battle->sides[0].pile.hand[battle->effectStep.choice];
        } else {
            battle->record.plays[battle->record.playCount].target = battle->sides[1].pile.hand[battle->effectStep.choice];
        }
        battle->record.answer = 1;
        battle->run = CARD_RUN_PHASE;
        break;
    }
}

/* Effect step 152: the yes/no question; the answer is 1 for the first choice, 0 for the second */
static inline void answerYesNo(CardBattle *battle, CardScreen *screen) {
    switch (CARDGAME_stepYesNo(battle, screen)) {
    case 0:
        break;
    case 1:
        battle->record.answer = 1;
        battle->run = CARD_RUN_PHASE;
        break;
    case 2:
        battle->record.answer = 0;
        battle->run = CARD_RUN_PHASE;
        break;
    }
}

/* Starts the effect step battle->effectStep.next asks for (startEffectStep), then runs the current one (battle->effectStep.id) */
void CARDGAME_runEffectStep(CardBattle *battle, CardScreen *screen) {
    s32 side = battle->record.plays[battle->record.playCount - 1].side;
    s32 nextSide = battle->record.plays[battle->record.playCount].side;
    s32 other = side ^ 1;
    s32 result;

    if (battle->effectStep.next != 0) {
        startEffectStep(battle, screen, side, nextSide, other);
        battle->effectStep.id = battle->effectStep.next;
        battle->effectStep.next = 0;
    }

    switch (battle->effectStep.id) {
    case 0:
        break;
    case 1:
        if (CARDGAME_countDown(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 2:
        battle->run = CARD_RUN_RESOLVE;
        battle->effectPos += 2;
        break;
    case 3:
        battle->loopCount = 2;
        battle->run = CARD_RUN_RESOLVE;
        battle->loopStart = battle->effectPos;
        break;
    case 4:
        battle->loopCount = 3;
        battle->run = CARD_RUN_RESOLVE;
        battle->loopStart = battle->effectPos;
        break;
    case 5:
        battle->loopCount = 5;
        battle->run = CARD_RUN_RESOLVE;
        battle->loopStart = battle->effectPos;
        break;
    case 6:
        if (--battle->loopCount > 0) {
            battle->effectPos = battle->loopStart;
        }
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 7:
        CARDGAME_jumpIfLastSide(battle, screen, 1, 1);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 8:
        CARDGAME_jumpIfLastSide(battle, screen, 1, 2);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 15:
        jumpBackForOtherColors(battle, other);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 9:
        CARDGAME_jumpIfFewInHand(battle, side, 10, 9);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 10:
        CARDGAME_jumpIfManyInHand(battle, side, 10, -9);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 11:
        CARDGAME_jumpIfFewInHand(battle, side, 3, 9);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 12:
        CARDGAME_jumpIfManyInHand(battle, side, 3, -9);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 16:
        CARDGAME_jumpIfSlotFree(battle, side, 5);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 13:
        CARDGAME_jumpIfManyInHand(battle, side, 2, 4);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 14:
        CARDGAME_jumpIfDeckLeft(battle, other, 2);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 17:
        battle->record.plays[battle->record.playCount - 1].side ^= 1;
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 18:
    case 19:
        if (CARDGAME_stepPlayCard(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 20:
        if (CARDGAME_stepStart(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 27:
        if (CARDGAME_stepSwap(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 21:
        if (CARDGAME_stepAttack(battle, screen, 0)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 22:
        if (CARDGAME_stepAttack(battle, screen, 1)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 23:
        if (CARDGAME_stepDiscardAllSlots(battle, screen, 0)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 24:
        if (CARDGAME_stepDiscardAllSlots(battle, screen, 1)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 25:
        if (CARDGAME_stepSlotTotal(battle, screen, 0)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 26:
        if (CARDGAME_stepSlotTotal(battle, screen, 1)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 28:
        if (CARDGAME_stepRoundEnd(battle, screen, 0)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 29:
        if (CARDGAME_stepRoundEnd(battle, screen, 1)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        if (CARDGAME_stepEffectColorValue(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 41:
        if (CARDGAME_drainColorValues(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 42:
        if (CARDGAME_keepLastCard(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 43:
    case 45:
        if (CARDGAME_stepClosePanels(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 44:
    case 46:
        if (CARDGAME_stepOpenPanels(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 47:
        if (side != 0) {
            battle->run = CARD_RUN_RESOLVE;
        } else if (CARDGAME_waitMessage(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 48:
        if (CARDGAME_waitMessage(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 49:
        if (side != 0) {
            battle->run = CARD_RUN_RESOLVE;
        } else if (CARDGAME_waitMessage(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 52:
        CARDGAME_markPileCardsByColor(battle, screen, side, 3, 0xFD);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 53:
        CARDGAME_markPileCardsByColor(battle, screen, side, 3, 0xFE);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 50:
        CARDGAME_markPileCardsByColor(battle, screen, side, 3, 0xFF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 51:
        CARDGAME_markPileCardsByColor(battle, screen, other, 3, 0xFF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 55:
        CARDGAME_markPileCardsByColor(battle, screen, side, 2, 0xFF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 54:
        CARDGAME_markPileCardsByColor(battle, screen, other, 2, 0xFF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 56:
        CARDGAME_markPileCardsByColor(battle, screen, other, 2, 0x81);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 57:
        CARDGAME_markPileCardsByColor(battle, screen, other, 2, 0xBF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 58:
        CARDGAME_markPileCardsByColor(battle, screen, side, 4, 0xFF);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 59:
        CARDGAME_markSlotsByColor(battle, screen, side, 0x3FC);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 60:
        CARDGAME_markSlotsByColor(battle, screen, side, 0x2FC);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 61:
        CARDGAME_markSlotsByColor(battle, screen, side, 0x1FC);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 62:
        if (CARDGAME_discardPickedCard(battle, screen, other, 0)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 63:
        if (CARDGAME_discardPickedCard(battle, screen, side, 0)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 64:
        if (CARDGAME_discardPickedCard(battle, screen, other, 1)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 65:
        if (CARDGAME_discardPrevCard(battle, screen)) {
            battle->prevDiscarded = 1;
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 66:
        if (CARDGAME_drawFromDeck(battle, screen, side, 3)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 67:
        if (CARDGAME_drawFromDeck(battle, screen, side, 4)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 68:
        if (CARDGAME_returnUsedCards(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 69:
    case 70:
    case 71:
        if (CARDGAME_markDrawnCards(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 72:
        if (CARDGAME_drawNewCards(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 73:
        if (CARDGAME_discardHand(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 74:
        chooseFirstEligible(battle, other);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 75:
        CARDGAME_chooseNextDraw(battle, screen, other);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 76:
        if (CARDGAME_removeMarkedSlots(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 77:
        if (CARDGAME_stepSlotSweep(battle, screen, 0)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 78:
        if (CARDGAME_stepSlotSweep(battle, screen, 1)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 86:
        if (CARDGAME_moveSlotCard(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 87:
        if (CARDGAME_flipSlotCard(battle, screen, side)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 88:
        if (CARDGAME_waitFor(battle, screen, 36)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 89:
        if (CARDGAME_waitFor(battle, screen, 28)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 90:
        if (CARDGAME_stepTotals(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 91:
        CARDGAME_markSlotsOfOrder(battle, screen);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 92:
        CARDGAME_markTargetSlots(battle, screen);
        battle->run = CARD_RUN_RESOLVE;
        break;
    case 93:
        result = CARDGAME_countSlotValues(battle, screen);
        if (result == 1) {
            battle->run = CARD_RUN_RESOLVE;
        } else if (result == 2) {
            battle->effectStep.next = 77;
        }
        break;
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
        if (CARDGAME_stepSlotStats(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 106:
    case 107:
    case 108:
    case 109:
    case 110:
    case 111:
        if (CARDGAME_countDown(battle, screen)) {
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 119:
    case 120:
    case 121:
    case 122:
    case 123:
    case 124:
    case 125:
    case 126:
        answerOnceDone(battle, CARDGAME_stepPileChoice(battle, screen));
        break;
    case 127:
        answerOnceDone(battle, CARDGAME_stepPileChoice(battle, screen));
        break;
    case 128:
    case 129:
    case 130:
        answerOnceDone(battle, CARDGAME_stepPileChoice(battle, screen));
        break;
    case 131:
    case 132:
        answerOnceDone(battle, CARDGAME_stepPreviousCardChoice(battle, screen));
        break;
    case 135:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x180);
        battle->effectStep.next = 141;
        break;
    case 134:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x1BC);
        battle->effectStep.next = 141;
        break;
    case 133:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x1FC);
        battle->effectStep.next = 141;
        break;
    case 138:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x280);
        battle->effectStep.next = 141;
        break;
    case 137:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x2BC);
        battle->effectStep.next = 141;
        break;
    case 136:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x2FC);
        battle->effectStep.next = 141;
        break;
    case 139:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x3FC);
        battle->effectStep.next = 141;
        break;
    case 140:
        CARDGAME_markSlotsByColor(battle, screen, nextSide, 0x380);
        battle->effectStep.next = 141;
        break;
    case 141:
        answerTableCardPick(battle, screen);
        break;
    case 152:
        answerYesNo(battle, screen);
        break;
    case 153:
        answerOnceDone(battle, CARDGAME_stepChooseCards(battle, screen, &battle->sides[battle->record.turnSide].pile));
        break;
    case 154:
        if (CARDGAME_stepChooseCards(battle, screen, &battle->sides[0].pile) != -1) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 157:
        if (CARDGAME_pickComputerCards(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 155:
    case 156:
        if (CARDGAME_stepChooseCards(battle, screen, &battle->sides[0].pile) != -1) {
            battle->run = CARD_RUN_MENU;
        }
        break;
    case 168:
        if (CARDGAME_viewTable(battle, screen)) {
            battle->run = CARD_RUN_MENU;
        }
        break;
    case 158:
        if (CARDGAME_stepTally(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 159:
    case 160:
    case 161:
    case 162:
    case 163:
    case 164:
    case 165:
    case 166:
        if (CARDGAME_stepMessage(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 167:
        if (CARDGAME_drawFirstPlayer(battle, screen)) {
            battle->run = CARD_RUN_PHASE;
        }
        break;
    case 172:
        if (side == 0) {
            if (CARDGAME_chooseCard(battle, screen, 2)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_pickComputerDeckCard(battle, screen);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 173:
        if (side == 0) {
            if (CARDGAME_chooseCard(battle, screen, 0)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_pickLowestPlayerCard(battle, screen);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 169:
        if (side == 0) {
            if (CARDGAME_chooseCard(battle, screen, 0)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_pickBestPileCard(battle, screen, 1);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 170:
        if (side == 0) {
            if (CARDGAME_chooseCard(battle, screen, 0)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_pickBestPileCard(battle, screen, 0);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 171:
        if (side == 0) {
            if (CARDGAME_chooseCard(battle, screen, 0)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_pickBestPileCard(battle, screen, 1);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 174:
        if (side == 0) {
            if (CARDGAME_pickTableCard(battle, screen)) {
                battle->run = CARD_RUN_RESOLVE;
            }
        } else {
            CARDGAME_markBestOpponentSlot(battle);
            battle->run = CARD_RUN_RESOLVE;
        }
        break;
    case 142:
    case 143:
    case 144:
    case 145:
    case 146:
    case 147:
    case 148:
    case 149:
        answerOnceDone(battle, CARDGAME_stepTargetSlots(battle, screen));
        break;
    case 150:
        CARDGAME_markPileCardsByColor(battle, screen, nextSide, 2, 0xFD);
        battle->effectStep.next = 151;
        break;
    case 151:
        answerHandCardPick(battle, screen, nextSide);
        break;
    }
}

/* The data: the cards' effects, this object's one table */
CardEffect CARDGAME_cardEffects[60] = {
    { 0x91, 0x0A, 0x2F, 0x04, { 0x6A, 0x5C, 0x59, 0x4D, 0x4C } },
    { 0x70, 0x00, 0x00, 0x09, { 0x03, 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x4F, 0x06 } },
    { 0x8E, 0x0A, 0x2F, 0x01, { 0x5C, 0x5E } },
    { 0x85, 0x09, 0x2F, 0x00, { 0x5B, 0x5F } },
    { 0x73, 0x00, 0x00, 0x09, { 0x1E } },
    { 0x70, 0x00, 0x00, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x50 } },
    { 0x92, 0x0A, 0x2F, 0x05, { 0x6B, 0x5C, 0x4E, 0x4C, 0x03, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A, 0x11, 0x06 } },
    { 0x88, 0x09, 0x2F, 0x00, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x5B, 0x57, 0x4C } },
    { 0x83, 0x00, 0x00, 0x09, { 0x41 } },
    { 0x8B, 0x09, 0x2F, 0x00, { 0x5B, 0x4E, 0x4C, 0x03, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A, 0x11, 0x06 } },
    { 0x74, 0x00, 0x00, 0x09, { 0x1F } },
    { 0x70, 0x00, 0x00, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x51 } },
    { 0x93, 0x0A, 0x2F, 0x06, { 0x6C, 0x5C, 0x60 } },
    { 0x81, 0x04, 0x00, 0x09, { 0x2D, 0x3A, 0xAB, 0x43, 0x2E } },
    { 0x75, 0x00, 0x00, 0x09, { 0x21 } },
    { 0x85, 0x09, 0x2F, 0x00, { 0x5B, 0x61 } },
    { 0x75, 0x00, 0x00, 0x09, { 0x20 } },
    { 0x70, 0x00, 0x00, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x52 } },
    { 0x94, 0x0A, 0x2F, 0x07, { 0x6D, 0x5C, 0x58, 0x65, 0x4D, 0x4C } },
    { 0x95, 0x0A, 0x2F, 0x08, { 0x5C, 0x59, 0x4D, 0x4C } },
    { 0x90, 0x0A, 0x2F, 0x03, { 0x5C, 0x58, 0x66, 0x4D, 0x4C } },
    { 0x88, 0x09, 0x2F, 0x00, { 0x5B, 0x58, 0x67, 0x4D, 0x4C } },
    { 0x76, 0x00, 0x00, 0x09, { 0x22 } },
    { 0x70, 0x00, 0x00, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x53 } },
    { 0x7F, 0x03, 0x3A, 0x09, { 0x6E, 0x39, 0x4A, 0x3E, 0x0F } },
    { 0x82, 0x05, 0x35, 0x09, { 0x2D, 0x32, 0xAC, 0x42, 0x2E, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A } },
    { 0x89, 0x09, 0x2F, 0x00, { 0x5B, 0x59, 0x4D, 0x4C } },
    { 0x7F, 0x01, 0x33, 0x09, { 0x07, 0x2F, 0x07, 0x2B, 0x36, 0xA9, 0x07, 0x2C, 0x3E } },
    { 0x77, 0x00, 0x00, 0x09, { 0x23 } },
    { 0x70, 0x00, 0x00, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x54 } },
    { 0x90, 0x0A, 0x2F, 0x03, { 0x6F, 0x5C, 0x59, 0x4D, 0x4C } },
    { 0x7E, 0x00, 0x00, 0x09, { 0x6F, 0x2A } },
    { 0x72, 0x06, 0x35, 0x09, { 0x6F, 0x04, 0x08, 0x2F, 0x2B, 0x33, 0xAD, 0x07, 0x2C, 0x40, 0x0E, 0x30, 0x02, 0x06 } },
    { 0x82, 0x05, 0x35, 0x09, { 0x6F, 0x49, 0x2D, 0x47, 0x48, 0x2E } },
    { 0x7D, 0x00, 0x00, 0x09, { 0x6F, 0x29 } },
    { 0x80, 0x04, 0x00, 0x09, { 0x44 } },
    { 0x70, 0x00, 0x00, 0x09, { 0x0D, 0x2D, 0x46, 0x48, 0x2E, 0x0B, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0C, 0x11, 0x0D, 0x2D, 0x46, 0x48, 0x2E, 0x0B, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0C, 0x11 } },
    { 0x72, 0x06, 0x35, 0x09, { 0x05, 0x33, 0x4B, 0x40, 0x0E, 0x30, 0x02, 0x06 } },
    { 0x8F, 0x0A, 0x2F, 0x02, { 0x5C, 0x69 } },
    { 0x96, 0x0B, 0x2F, 0x09, { 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x56 } },
    { 0x87, 0x09, 0x2F, 0x00, { 0x5B, 0x62 } },
    { 0x82, 0x05, 0x35, 0x09, { 0x2D, 0x45, 0x48, 0x2E, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A } },
    { 0x82, 0x07, 0x36, 0x09, { 0x2D, 0x34, 0xAC, 0x42, 0x2E, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A } },
    { 0x82, 0x08, 0x37, 0x09, { 0x2D, 0x35, 0xAC, 0x42, 0x2E, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A } },
    { 0x82, 0x05, 0x35, 0x09, { 0x2D, 0x32, 0xAC, 0x42, 0x2E, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F } },
    { 0x72, 0x06, 0x35, 0x09, { 0x03, 0x33, 0x4B, 0x40, 0x0E, 0x30, 0x02, 0x06 } },
    { 0x8A, 0x09, 0x2F, 0x00, { 0x5B, 0x58, 0x67, 0x4D, 0x4C } },
    { 0x84, 0x00, 0x00, 0x09, { 0x41 } },
    { 0x7F, 0x02, 0x38, 0x09, { 0x07, 0x2F, 0x07, 0x2B, 0x38, 0xA9, 0x07, 0x2C, 0x3E } },
    { 0x70, 0x00, 0x00, 0x09, { 0x03, 0x10, 0x31, 0x3D, 0xAE, 0x4D, 0x4C, 0x55, 0x06 } },
    { 0x78, 0x00, 0x00, 0x09, { 0x24 } },
    { 0x79, 0x00, 0x00, 0x09, { 0x25 } },
    { 0x7A, 0x00, 0x00, 0x09, { 0x26 } },
    { 0x7B, 0x00, 0x00, 0x09, { 0x27 } },
    { 0x7C, 0x00, 0x00, 0x09, { 0x28 } },
    { 0x8C, 0x09, 0x2F, 0x00, { 0x5B, 0x4E, 0x4C, 0x03, 0x09, 0x08, 0x2F, 0x2B, 0x37, 0xAA, 0x07, 0x2C, 0x3F, 0x0A, 0x11, 0x06 } },
    { 0x8B, 0x09, 0x2F, 0x00, { 0x5B, 0x58, 0x68, 0x4D, 0x4C } },
    { 0x8A, 0x09, 0x2F, 0x00, { 0x5B, 0x4D, 0x4C } },
    { 0x85, 0x09, 0x2F, 0x00, { 0x5B, 0x63 } },
    { 0x85, 0x09, 0x2F, 0x00, { 0x5B, 0x64 } },
};
