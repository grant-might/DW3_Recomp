/* CARDGAME's opponent card order and the checks of whether a card can be
   played. */

#include "cardgame/card_battle.h"

/* Sorts the opponent's cards by opponentDraws[].order, then swaps card effectStep.choice to reserveStart */
void CARDGAME_sortOpponentCards(CardBattle *battle) {
    CardDraw draw;
    s16 *cards = battle->sides[1].pile.deck;
    s16 card;
    s32 i;
    s32 j;
    s32 target;

    for (i = battle->sides[1].pile.deckTop; i < 39; i++) {
        for (j = i + 1; j < 40; j++) {
            if (battle->opponentDraws[i].order > battle->opponentDraws[j].order) {
                card = cards[i];
                cards[i] = cards[j];
                cards[j] = card;
                draw = battle->opponentDraws[i];
                battle->opponentDraws[i] = battle->opponentDraws[j];
                battle->opponentDraws[j] = draw;
            }
        }
    }
    target = battle->reserveStart;
    draw = battle->opponentDraws[battle->effectStep.choice];
    battle->opponentDraws[battle->effectStep.choice] = battle->opponentDraws[target];
    battle->opponentDraws[target] = draw;
    card = cards[battle->effectStep.choice];
    cards[battle->effectStep.choice] = cards[target];
    cards[target] = card;
    battle->effectStep.choice = target;
}

/* Whether card index can be played in this phase by its kind: any kind but 0
   in CARD_PHASE_PLAYS_AFTER, kind 2 in CARD_PHASE_PLAYS_BEFORE */
s32 CARDGAME_canPlayCardKind(CardBattle *battle, s32 index) {
    CardDrawer drawer;
    s32 result = 0;
    s32 kind;

    initCardDrawer(&drawer);
    drawer.setCard(battle->cards[index] + 1);
    kind = drawer.getKind();
    if (kind != 0) {
        if (battle->phase == CARD_PHASE_PLAYS_AFTER || (battle->phase == CARD_PHASE_PLAYS_BEFORE && kind == 2)) {
            result = 1;
        }
    }
    return result;
}

/* Whether a card can be played: in CARD_PHASE_PUT_OUT a kind 0x10 card needs as many
   points of its colour (points), otherwise CARDGAME_canPlayCardKind */
s32 CARDGAME_canPlayCard(CardBattle *battle, u8 *points, s32 card) {
    CardDrawer drawer;
    s32 result = 0;

    if (battle->phase == CARD_PHASE_PUT_OUT) {
        initCardDrawer(&drawer);
        drawer.setCard(battle->cards[card] + 1);
        if (drawer.card->kind == 0x10) {
            result = points[drawer.card->color - 1] >= drawer.card->points;
        }
    } else {
        result = CARDGAME_canPlayCardKind(battle, card);
    }
    return result;
}
