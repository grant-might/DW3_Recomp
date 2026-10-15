#ifndef CARDGAME_CHOICE_H
#define CARDGAME_CHOICE_H

/* CARDGAME's choices: the cards of a hand or a pile, the picks, the deck,
   the table views and the questions (card_choice.c, hand_pick.c,
   deck_choice.c, table_view.c and questions.c). */

#include "cardgame/card_battle/types.h"
#include "cardgame/card_battle/screen.h"
#include "cardgame/card_battle/battle.h"

/* A step of CARDGAME_windowSteps: once its time is past, CARDGAME_openStepWindows opens windows */
typedef struct CardBattleStep {
    /* 0x0 */ s16 time;
    /* 0x2 */ s16 kind;
} CardBattleStep;

/* A blinking marker that opens and closes by scaling (CARDGAME_createMarker):
   frame 0x47 of the TIM archive's sprite sheet */
struct CardMarker {
    TASK_HEADER(CardMarker);
    /* 0x50 */ s32 time; /* the blink */
    /* 0x54 */ s16 x;
    /* 0x56 */ s16 y;
    /* 0x58 */ s16 scaleX;
    /* 0x5A */ s16 scaleY;
    /* 0x5C */ u8 pad[6]; /* nothing uses it */
    /* 0x62 */ u8 phase; /* 0 opening, 1 open, 2 closing */
    /* 0x63 */ u8 fast; /* the blink's palette cycle */
    /* 0x64 */ s16 scaleTime;
    /* 0x66 */ s16 scaleDuration;
    /* 0x68 */ void (*setPos)(struct CardMarker *marker, s16 x, s16 y);
    /* 0x6C */ void (*close)(struct CardMarker *marker);
    /* 0x70 */ void (*setFast)(struct CardMarker *marker);
};

/* A deck's window (CARDGAME_createDeckWindow): its name and how many cards
   of each of the six colours it has. It opens and closes by scaling. */
struct CardDeckWindow {
    TASK_HEADER(CardDeckWindow);
    /* 0x50 */ s32 deck; /* in GAME.decks */
    /* 0x54 */ s32 time; /* the blink */
    /* 0x58 */ s16 x;
    /* 0x5A */ s16 y;
    /* 0x5C */ s16 scaleX;
    /* 0x5E */ s16 scaleY;
    /* 0x60 */ u8 counts[6];
    /* 0x66 */ u8 phase; /* 0 opening, 1 open, 2 closing */
    /* 0x67 */ u8 blink;
    /* 0x68 */ s16 scaleTime;
    /* 0x6A */ s16 scaleDuration;
    /* 0x6C */ void (*setBlink)(struct CardDeckWindow *window);
    /* 0x70 */ void (*close)(struct CardDeckWindow *window);
};

/* The functions and data the overlay's files share */
extern CardOffset CARDGAME_deckCountOffsets[];
extern s32 CARDGAME_rowSpriteOffsets[]; /* the sprite offset of each row of CARDGAME_viewTable */
extern s32 CARDGAME_rowSteps[]; /* the step of each row of CARDGAME_viewTable */
extern s16 CARDGAME_savedPanelScales[2]; /* the panels' scale states, kept by CARDGAME_viewTable */
extern s32 CARDGAME_selectionText; /* the message of CARDGAME_chooseCard's selection */
extern s16 CARDGAME_stepWindowPositions[2][3][4][2]; /* CARDGAME_openStepWindows's window positions, by layout and window, then the same moved for SHIFT_PAL_SCREEN */
s32 CARDGAME_getTableHeight(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_viewTable(CardBattle *battle, CardScreen *screen);
void CARDGAME_moveTableHighlight(CardBattle *battle, CardScreen *screen, s32 kind, s32 delta);
void CARDGAME_setupCardChoice(CardBattle *battle, CardScreen *screen, s32 side, s32 kind);
extern u8 CARDGAME_stepMessages[][2]; /* CARDGAME_startStepMessage's text for window 5 and what screen->openMessage shows, by message */
extern CardBattleStep CARDGAME_windowSteps[];
extern s32 CARDGAME_coinCardPositions[][2]; /* the two cards' x, y (CARDGAME_startFirstPick) */
extern s32 CARDGAME_promptText; /* the text of window 0 */
void CARDGAME_showCardInfo(CardBattle *battle, CardScreen *screen, s32 offset);
void CARDGAME_addCardPoints(CardBattle *battle, CardScreen *screen, CardPile *pile, s32 arg3, s32 card);
s16 CARDGAME_getStepWindowPos(s32 a, s32 b, s32 c);
s32 CARDGAME_openStepWindows(CardBattle *battle, CardScreen *screen, s32 layout, s32 text, s32 time, s32 step);
void CARDGAME_startQuestion(CardBattle *battle, CardScreen *screen, s32 value);
s32 CARDGAME_stepYesNo(CardBattle *battle, CardScreen *screen);
void CARDGAME_clearCardInfo(CardBattle *battle, CardScreen *screen);
void CARDGAME_showTableCardInfo(CardBattle *battle, CardScreen *screen, s32 kind);
void CARDGAME_startHandPick(CardBattle *battle, CardScreen *screen, CardPile *pile);
void CARDGAME_startPick(CardBattle *battle, CardScreen *screen, s32 all, s32 mode);
void CARDGAME_moveSelection(CardBattle *battle, CardScreen *screen, s32 count, s32 step);
void CARDGAME_movePileSelection(CardBattle *battle, CardScreen *screen, CardPile *pile, s32 step);
void CARDGAME_flagPlayableCards(CardBattle *battle, CardScreen *screen, CardPile *pile);
s32 CARDGAME_isHandPickDone(CardBattle *battle, CardScreen *screen, CardPile *pile);
s32 CARDGAME_togglePick(CardBattle *battle, CardScreen *screen, CardPile *pile);
void CARDGAME_readChooseInput(CardBattle *battle, CardScreen *screen, CardPile *pile);
void CARDGAME_readPickInput(CardBattle *battle, CardScreen *screen, CardPile *pile);
void CARDGAME_readCountInput(CardBattle *battle, CardScreen *screen, s32 count);
s32 CARDGAME_stepPulseCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_startPanelsStep(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_stepMessage(CardBattle *battle, CardScreen *screen);
void CARDGAME_startStepMessage(CardBattle *battle, CardScreen *screen, s32 index);
void CARDGAME_switchCoinCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_startFirstPick(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_drawFirstPlayer(CardBattle *battle, CardScreen *screen);
void CARDGAME_startViewTable(CardBattle *battle, CardScreen *screen);
void CARDGAME_startPickTableCard(CardBattle *battle, CardScreen *screen, s32 cancelable);
s32 CARDGAME_getPickTableHeight(CardBattle *battle, CardScreen *screen);
void CARDGAME_movePickHighlight(CardBattle *battle, CardScreen *screen, s32 kind, s32 delta);
s32 CARDGAME_pickTableCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_setupSideChoice(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_moveHandHighlight(CardBattle *battle, CardScreen *screen, s32 delta);
void CARDGAME_browseHand(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_chooseCard(CardBattle *battle, CardScreen *screen, s32 mode);
s32 CARDGAME_pickComputerCards(CardBattle *battle, CardScreen *screen);
void CARDGAME_pickComputerDeckCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_startPileChoice(CardBattle *battle, CardScreen *screen, s32 flags);

/* The steps CARDGAME_runEffectStep starts (CardBattle.effectStep.next) and runs (effectStep.id) */
s32 CARDGAME_stepChooseCards(CardBattle *battle, CardScreen *screen, CardPile *pile);
s32 CARDGAME_stepTally(CardBattle *battle, CardScreen *screen);
void CARDGAME_pickBestPileCard(CardBattle *battle, CardScreen *screen, s32 arg2);
void CARDGAME_pickLowestPlayerCard(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_stepPileChoice(CardBattle *battle, CardScreen *screen);
void CARDGAME_startPreviousCardChoice(CardBattle *battle, CardScreen *screen, s32 arg2);
s32 CARDGAME_stepPreviousCardChoice(CardBattle *battle, CardScreen *screen);
void CARDGAME_showTargetSlots(CardBattle *battle, CardScreen *screen, s32 side, s32 arg3);
s32 CARDGAME_stepTargetSlots(CardBattle *battle, CardScreen *screen);
void CARDGAME_drawMarker(CardMarker *marker);
void CARDGAME_updateMarker(CardMarker *marker);
void CARDGAME_setMarkerPos(CardMarker *marker, s16 x, s16 y);
void CARDGAME_setMarkerFast(CardMarker *marker);
void CARDGAME_closeMarker(CardMarker *marker);
CardMarker *CARDGAME_createMarker(s16 x, s16 y);
void CARDGAME_drawDeckWindow(CardDeckWindow *window);
void CARDGAME_updateDeckWindow(CardDeckWindow *window, TextWindow **texts);
void CARDGAME_setDeckWindowBlink(CardDeckWindow *window);
void CARDGAME_closeDeckWindow(CardDeckWindow *window);
CardDeckWindow *CARDGAME_createDeckWindow(s32 deck, s32 x, s32 y);

#endif /* CARDGAME_CHOICE_H */
