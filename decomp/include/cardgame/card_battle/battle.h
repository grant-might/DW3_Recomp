#ifndef CARDGAME_BATTLE_H
#define CARDGAME_BATTLE_H

/* CARDGAME's card battle: the slots, piles, plays, effects and steps, the
   opponent and the rounds (effect_script.c, battle.c, battle_steps.c, round.c,
   play_checks.c, pile_effects.c, slot_effects.c and computer.c). */

#include "cardgame/card_battle/types.h"
#include "cardgame/card_battle/screen.h"

/* The opponents of the card battles (CardOpponent) */
#if VERSION_US || VERSION_JP
#define FILE_CARDGAME_OPPONENTS 0x795
#elif VERSION_EU
#define FILE_CARDGAME_OPPONENTS 0x7A4
#endif

/* A card a player has out (CARDGAME_setSlot) */
typedef struct CardSlot {
    /* 0x0 */ s16 card; /* the index in the battle's card list */
    /* 0x2 */ s16 apBonus; /* what the effects added to its ap */
    /* 0x4 */ s16 hpBonus; /* and to its hp */
    /* 0x6 */ s16 ap; /* its attack points, 0 to 99 */
    /* 0x8 */ s16 hp; /* its hit points, 0 to 99: at 0 it is lost */
    /* 0xA */ u8 side;
    /* 0xB */ u8 owner;
    /* 0xC */ u8 order; /* CardBattle.slotCount when it was added */
} CardSlot;

/* A slot's card, sorted by id (CARDGAME_findCardSet) */
typedef struct CardSortEntry {
    /* 0x0 */ s16 slot;
    /* 0x2 */ s16 card; /* its id, minus one */
} CardSortEntry;

/* One of the two players of a card battle */
typedef struct CardPlayer {
    /* 0x00 */ u8 slotCount;
    /* 0x02 */ CardSlot slots[8];
} CardPlayer;

/* A card played in a round (CardRecord.plays), whose effect then runs */
typedef struct CardPlay {
    /* 0x0 */ s16 card; /* the index in the battle's card list */
    /* 0x2 */ s16 mark;
    /* 0x4 */ u8 side; /* who played it */
    /* 0x5 */ u8 targetKind; /* its effect's (CardEffect.target) */
    /* 0x6 */ u8 target; /* the slot's order, or the card, it was played on */
} CardPlay;

/* A card's effect (CARDGAME_cardEffects), read through
   CARDGAME_getEffectField */
typedef struct CardEffect {
    /* 0x0 */ u8 condition; /* for the computer (CARDGAME_checkComputerCondition) */
    /* 0x1 */ u8 playCondition; /* CARDGAME_checkPlayCondition's */
    /* 0x2 */ u8 message; /* what it shows when the play condition holds */
    /* 0x3 */ u8 target; /* the kind of target of its play (CardPlay.targetKind) */
    /* 0x4 */ u8 script[40]; /* the effect steps it runs one by one, up to a 0 */
} CardEffect;

/* How the computer plays a card of its deck (CardBattle.opponentPlans):
   kind first, then priority (CARDGAME_sortOpponentHand) */
typedef struct CardPlan {
    /* 0x0 */ u8 kind; /* 1-7 (CardDeckCard.kind) */
    /* 0x1 */ u8 flagged; /* bit 15 of the card in the opponent's deck */
    /* 0x2 */ s16 priority; /* by kind, then by place in the deck */
} CardPlan;

/* What CARDGAME_runBattle runs this frame (CardBattle.run) */
enum CardRun {
    CARD_RUN_PHASE,   /* the battle's phase (CARDGAME_runPhase) */
    CARD_RUN_STEP,    /* an effect step (CARDGAME_runEffectStep) */
    CARD_RUN_RESOLVE, /* the card just played (CARDGAME_resolveCard) */
    CARD_RUN_MENU     /* the battle menu (CARDGAME_runBattleMenu) */
};

/* The phases of a card battle (CardBattle.phase, CARDGAME_runPhase) */
enum CardPhase {
    CARD_PHASE_NONE, /* for nextPhase: no switch */
    CARD_PHASE_FADE_IN,
    CARD_PHASE_CHOOSE_DECK,
    CARD_PHASE_FIRST_PICK,   /* who starts */
    CARD_PHASE_DEAL,         /* six cards to each side */
    CARD_PHASE_PLAYS_BEFORE, /* plays before the cards are put out: kind 2 cards only */
    CARD_PHASE_PUT_OUT,
    CARD_PHASE_PLAYS_AFTER, /* plays after: any kind but 0 */
    CARD_PHASE_END_ROUND,
    CARD_PHASE_NEXT_ROUND,
    CARD_PHASE_MESSAGE /* the message of the phase that ended (CARDGAME_battleMessages) */
};

/* What a phase does once it is over (CARDGAME_battleMessages): the step
   that shows its message, and the phase after it */
typedef struct CardBattleMessage {
    /* 0x0 */ u16 step; /* for CardStep.next */
    /* 0x2 */ u16 phase; /* for CardBattle.nextPhase */
} CardBattleMessage;

/* Where a card of the opponent's deck is drawn (CardBattle.opponentDraws):
   the computer draws the cards of group round * 2 + 2 (CardBattle.round),
   and holds the cards of group 7 back */
typedef struct CardDraw {
    /* 0x0 */ u8 order; /* its place in the deck */
    /* 0x1 */ u8 group;
} CardDraw;

/* A player's cards in a card battle (CardSide.pile) */
typedef struct CardPile {
    /* 0x00 */ s16 apTotal; /* the sum of its slot cards' ap, which comes off the other side's hpTotal */
    /* 0x02 */ s16 hpTotal; /* the sum of its slot cards' hp: the higher one wins the round */
    /* 0x04 */ s16 deckTop; /* the index of the deck's top card */
    /* 0x06 */ s16 discardCount;
    /* 0x08 */ s16 deckCount; /* the cards left in the deck */
    /* 0x0A */ s16 handCount;
    /* 0x0C */ u8 points[5]; /* one per card colour */
    /* 0x11 */ u8 side; /* its panel */
    /* 0x12 */ u8 wins; /* rounds won: two win the battle */
    /* 0x14 */ s16 deck[40]; /* deckTop on are left */
    /* 0x64 */ s16 hand[10]; /* handCount in use */
    /* 0x78 */ s16 discards[10]; /* the cards played and lost from the slots, discardCount in use */
} CardPile;

/* The turns of a round, in which the sides play cards one after the other
   (CARDGAME_playRounds), and the battle menu (CardBattle.record) */
typedef struct CardRecord {
    /* 0x00 */ u8 menuState; /* CARDGAME_runBattleMenu's state */
    /* 0x01 */ u8 menuCursor; /* CARDGAME_runBattleMenu's cursor, 0-4 */
    /* 0x02 */ u8 pad2[0xA]; /* nothing uses it */
    /* 0x0C */ u8 turnState; /* CARDGAME_playRounds' state */
    /* 0x0D */ s8 playCount; /* the plays in use */
    /* 0x0E */ u8 turns; /* the turns of plays this round */
    /* 0x0F */ u8 passes; /* the turns with no card played: two end the plays */
    /* 0x10 */ u8 starter; /* the side that starts the round */
    /* 0x11 */ u8 turnSide; /* the side whose turn it is */
    /* 0x12 */ s8 answer; /* a step's result: -1 while waiting, then 0 or 1 */
    /* 0x14 */ s32 waitTime;
    /* 0x18 */ CardPlay plays[3]; /* the cards played this turn, up to three */
    /* 0x30 */ u8 pad30[4]; /* nothing uses it */
} CardRecord;

/* The effect step that runs (CARDGAME_runEffectStep) and its values, which
   CARDGAME_runBattleMenu keeps a copy of while the menu is open */
typedef struct CardStep {
    /* 0x00 */ u8 id; /* the step running */
    /* 0x01 */ u8 next; /* the step to start: CARDGAME_runEffectStep sets it up first */
    /* 0x02 */ u8 state; /* the step's own state */
    /* 0x03 */ u8 nextState; /* the state to set up first, if not 0 */
    /* 0x04 */ s32 time; /* a frame count, or the step's own sub-state */
    /* 0x08 */ s32 vars[5]; /* values each step uses its own way */
    /* 0x1C */ s32 cursor; /* the card or slot highlighted */
    /* 0x20 */ s32 choice; /* the card, slot or answer chosen; -1 for none */
    /* 0x24 */ u8 count; /* the cards picked, or to draw */
    /* 0x25 */ u8 flags; /* the step's own: the rows to pick from, the deck run out... */
    /* 0x26 */ s8 eligible[0x29]; /* the cards or slots that can be picked */
    /* 0x4F */ s8 marked[40]; /* the cards or slots picked */
} CardStep;

/* Each player's side of a card battle (CardBattle.sides) */
typedef struct CardSide {
    /* 0x00 */ CardPile pile;
    /* 0x8C */ u8 pad[0x3C]; /* nothing uses it */
} CardSide;

/* The animations of the cards on the table (CardBattle.anim): the slots,
   the plays or a pile laid out as sprites that scale in, then out */
enum CardAnimId {
    CARD_ANIM_NONE,
    CARD_ANIM_SHOW_SLOTS,
    CARD_ANIM_HIDE_SLOTS,
    CARD_ANIM_SHOW_PLAYS,
    CARD_ANIM_HIDE_PLAYS,
    CARD_ANIM_SHOW_HAND, /* the player's piles... */
    CARD_ANIM_HIDE_HAND,
    CARD_ANIM_SHOW_DECK,
    CARD_ANIM_HIDE_DECK,
    CARD_ANIM_SHOW_DISCARDS,
    CARD_ANIM_HIDE_DISCARDS,
    CARD_ANIM_SHOW_OPPONENT_HAND, /* ...and the opponent's */
    CARD_ANIM_HIDE_OPPONENT_HAND,
    CARD_ANIM_SHOW_OPPONENT_DECK,
    CARD_ANIM_HIDE_OPPONENT_DECK,
    CARD_ANIM_SHOW_OPPONENT_DISCARDS,
    CARD_ANIM_HIDE_OPPONENT_DISCARDS,
    CARD_ANIM_LAY_OUT_HAND, /* the hand laid out at once */
    CARD_ANIM_LAY_OUT_OPPONENT_HAND
};

/* CardAnim.dimAll */
#define CARD_ANIM_UNDIM_ALL 1
#define CARD_ANIM_DIM_ALL 2

/* The running animation of the cards on the table (CARDGAME_updateCardAnims) */
typedef struct CardAnim {
    /* 0x00 */ u8 current; /* a CardAnimId */
    /* 0x01 */ u8 next; /* the one to start */
    /* 0x02 */ u8 pad; /* nothing uses it */
    /* 0x03 */ u8 hide; /* the one that hides what the last one showed */
    /* 0x04 */ u8 faceDown; /* the next pile or slots are laid out face down */
    /* 0x05 */ u8 dimAll; /* sets dimmed of the first 15 sprites once */
    /* 0x06 */ u8 dimmed[40]; /* by sprite */
    /* 0x30 */ s32 time;
    /* 0x34 */ s32 shown; /* the sprites scaled so far */
    /* 0x38 */ s32 stagger; /* one more sprite scales every 4 frames */
    /* 0x3C */ s32 count; /* the pile's cards */
    /* 0x40 */ s32 duration;
} CardAnim;

/* A card of an opponent's deck (CardOpponent.cards) */
typedef struct CardDeckCard {
    /* 0x0 */ s16 card; /* the id plus one in the low 12 bits; bit 15 a flag */
    /* 0x2 */ u8 group; /* when the computer draws it (CardDraw.group) */
    /* 0x3 */ u8 kind; /* how it plays it (CardPlan.kind) */
} CardDeckCard;

/* An opponent of the card battle: an entry of file FILE_CARDGAME_OPPONENTS, picked by
   CardBattle.arg (CARDGAME_loadOpponent) */
typedef struct CardOpponent {
    /* 0x00 */ CardDeckCard cards[40];
    /* 0xA0 */ s16 counterCards[20]; /* from 1, up to a 0: CardBattle.counterCards before the usual ones */
    /* 0xC8 */ u8 level; /* shown after "LV" (card text 0x3E), 4-92 */
    /* 0xCC */ s32 prize; /* its index in CARDGAME_prizeItems */
} CardOpponent;

/* The card battle (CARDGAME_createBattle). Its seven task items: the battle
   screen is the last one (CardBattleItems). */
struct CardBattle {
    TASK_HEADER(CardBattle);
    /* 0x050 */ s16 cards[250]; /* card ids, minus one (CARDGAME_loadCardImages) */
    /* 0x244 */ u8 cardCount; /* in cards */
    /* 0x245 */ u8 pad245[3]; /* nothing uses it */
    /* 0x248 */ s16 opponentDeck[40]; /* the opponent's deck */
    /* 0x298 */ s16 playerDeck[40]; /* the player's deck */
    /* 0x2E8 */ u8 arg; /* the mode argument: the opponent, from 1 */
    /* 0x2E9 */ u8 opponentLevel; /* CardOpponent.level */
    /* 0x2EA */ s16 deckChoice; /* the player's deck (GAME.decks) */
    /* 0x2EC */ s32 prize; /* the item the player wins (GAME.items) */
    /* 0x2F0 */ CardOpponent *opponents; /* file FILE_CARDGAME_OPPONENTS */
    /* 0x2F4 */ u8 run; /* what runs this frame (CARD_RUN_*) */
    /* 0x2F5 */ u8 firstStarter; /* the side that starts the first round */
    /* 0x2F6 */ u8 prevPhase;
    /* 0x2F7 */ u8 nextPhase; /* the phase to switch to, if not 0 */
    /* 0x2F8 */ u8 phase; /* a CARD_PHASE_* */
    /* 0x2F9 */ u8 phaseStep; /* the phase's own state */
    /* 0x2FC */ s32 phaseTime; /* a frame count, or the phase's own counter */
    /* 0x300 */ u8 round; /* from 0 */
    /* 0x301 */ u8 roundWinner; /* the side that won the round */
    /* 0x302 */ u8 playerLost; /* 0 once the player has won the battle */
    /* 0x303 */ u8 result; /* 2 once the battle is over */
    /* 0x304 */ u8 keptCard; /* the card CARDGAME_keepLastCard kept, plus one */
    /* 0x305 */ u8 keptCount; /* and how many times */
    /* 0x306 */ u8 fade; /* the fade to start (CARDGAME_fadeColors), plus one */
    /* 0x307 */ u8 pad307; /* nothing uses it */
    /* 0x308 */ u8 slotCount; /* the slots added so far */
    /* 0x309 */ u8 pad309; /* nothing uses it */
    /* 0x30A */ CardDraw opponentDraws[40]; /* by the opponent's deck card */
    /* 0x35A */ u8 pad35A[2]; /* nothing uses it */
    /* 0x35C */ CardPlan opponentPlans[40]; /* one per card of side 1 (40-79) */
    /* 0x3FC */ u8 pad3FC[4]; /* nothing uses it */
    /* 0x400 */ u8 counterCards[0x1B]; /* the cards the computer answers with a kind 4 card, up to an 0xFF */
    /* 0x41B */ u8 drawEnd; /* the first card of the opponent's deck above the round's group */
    /* 0x41C */ u8 reserveStart; /* the first card of the opponent's deck held back (group 7) */
    /* 0x420 */ CardStep effectStep;
    /* 0x498 */ CardAnim anim;
    /* 0x4DC */ u8 resolveState; /* CARDGAME_resolveCard's */
    /* 0x4DD */ u8 prevDiscarded; /* the card before the last one played went off too */
    /* 0x4DE */ u8 playsToResolve; /* record.playCount, as CARDGAME_resolveCard starts; nothing reads it */
    /* 0x4E0 */ s16 effectPos; /* the step of the card's effect script to run next */
    /* 0x4E2 */ s16 loopCount; /* the effect script's loop: the runs left */
    /* 0x4E4 */ s16 loopStart; /* and where it starts again */
    /* 0x4E8 */ s32 subState; /* the state of a phase's helpers (CARDGAME_showPlayedCard...) */
    /* 0x4EC */ s32 subTime; /* and their frame count */
    /* 0x4F0 */ CardStep savedStep; /* the step, while the battle menu is open */
    /* 0x568 */ CardRecord record;
    /* 0x59C */ CardSide sides[2];
    /* 0x72C */ CardPlayer players[2];
    /* 0x810 */ void (*shufflePile)(struct CardBattle *battle, s32 base, s32 n);
    /* 0x814 */ void (*sortCards)(struct CardBattle *battle, s16 *list, s32 range, s32 flags);
    /* 0x818 */ void (*putOutCards)(struct CardBattle *battle, s32 side);
    /* 0x81C */ void (*addCard)(struct CardBattle *battle, s32 side, s32 card);
    /* 0x820 */ s32 (*scoreHand)(struct CardBattle *battle, s32 side, s32 mask);
};

/* The card battle's task items */
typedef struct CardBattleItems {
    /* 0x00 */ CardPreloader *preloader; /* the card game's files' (CARDGAME_startPreloader) */
    /* 0x04 */ CardFader *fader;
    /* 0x08 */ CardMarker *marker; /* the deck choice's */
    /* 0x0C */ CardDeckWindow *deckWindows[3];
    /* 0x18 */ CardScreen *screen;
} CardBattleItems;

/* The steps CARDGAME_stepPlayCard has before its card goes off: the European and
   Japanese versions show the card's information first, and wait for a button */
#if VERSION_US
#define CARD_INFO_STEPS 0
#elif VERSION_EU || VERSION_JP
#define CARD_INFO_STEPS 2
#endif

/* The functions and data the overlay's files share */
extern CardEffect CARDGAME_cardEffects[]; /* the effects of the first 60 cards, by card id minus one: conditions, target and messages */
CardBattle *CARDGAME_createBattle(s32 arg);
void CARDGAME_showPanelValues(CardBattle *battle, CardBattleItems *items);
extern s16 CARDGAME_animPiles[7][2]; /* the side and pile CARDGAME_updateCardAnims passes to CARDGAME_layOutPile */
/* the deck used when the chosen one is empty: its first 40 cards, as card
   ids - 1 (CardBattle.playerDeck); nothing reads the 40 after them */
extern u16 CARDGAME_defaultDeck[];
extern CardOffset CARDGAME_deckWindowPos[3]; /* the three deck windows */
extern u8 CARDGAME_turnStates[]; /* the next record.turnState state, by step and side */
extern CardBattleMessage CARDGAME_battleMessages[]; /* the step and nextPhase of CARDGAME_startBattleMessage, by prevPhase */
#if PAL_VIDEO
extern s32 CARDGAME_slotRowPositions[2][4]; /* sprite positions, by SHIFT_PAL_SCREEN */
extern CardOffset CARDGAME_panelIconPositions[2][2][2]; /* CARDGAME_drawPanelIcon's icon and label positions, by SHIFT_PAL_SCREEN and side (EU) */
#endif
void CARDGAME_startColorChange(CardBattle *battle, CardScreen *screen, s32 amount, s32 interval);
extern s16 CARDGAME_cardWindowPositions[2][2][2]; /* two positions, then the same moved for SHIFT_PAL_SCREEN */
extern s16 CARDGAME_colorFlags[]; /* a flag bit per card colour */
extern s32 CARDGAME_discardPositions[][2]; /* x, y per side */
extern u8 CARDGAME_slotMoves[2][6][2]; /* per side, the slot moves (from, to) of CARDGAME_removeMarkedSlots */
extern u8 CARDGAME_slotMoveCounts[]; /* per side, the entries of CARDGAME_slotMoves */
extern u8 CARDGAME_markedCounts[2]; /* per side, the marked slots */
void CARDGAME_returnOpponentHand(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_canPlayCard(CardBattle *battle, u8 *points, s32 card);
s32 CARDGAME_stepColorValue(CardBattle *battle, CardScreen *screen, s32 side, s32 color);
s32 CARDGAME_stepShakeAway(CardBattle *battle, CardScreen *screen, s32 index);
void CARDGAME_discardSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index);
s32 CARDGAME_stepDiscardSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index);
s32 CARDGAME_stepSendOff(CardBattle *battle, CardScreen *screen, s32 index);
void CARDGAME_returnSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index);
void CARDGAME_sortCards(CardBattle *battle, s16 *list, s32 range, s32 flags);
void CARDGAME_shufflePile(CardBattle *battle, s32 base, s32 n);
void CARDGAME_setCountingCardValues(CardBattle *battle, CardSlot *slot, s32 side, s32 pass);
extern CardFadeColor CARDGAME_fadeColors[]; /* CARDGAME_startStepFade's fade colours and blends, by fade - 1 */
extern s32 CARDGAME_recordCardPositions[][2]; /* where card 15 goes, by CardRecord.playCount */
extern s16 CARDGAME_countingCards[]; /* five card ids, minus one */
extern CardOffset CARDGAME_playedCardPos[2][2]; /* then the same moved for SHIFT_PAL_SCREEN */
extern CardMessageWindow CARDGAME_savedScreenState;
void CARDGAME_setSlot(CardBattle *battle, s32 side, s32 card, s32 index);
void CARDGAME_addCard(CardBattle *battle, s32 side, s32 card);
void CARDGAME_putOutCards(CardBattle *battle, s32 side);
void CARDGAME_updateCardAnims(CardBattle *battle, CardBattleItems *items);
void CARDGAME_runEffectStep(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_runPhase(CardBattle *battle, CardBattleItems *items);
u8 CARDGAME_resolveCard(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_runBattleMenu(CardBattle *battle, CardBattleItems *items);
void CARDGAME_updateBattle(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_scoreHand(CardBattle *battle, s32 side, s32 mask);
s32 CARDGAME_unflagSlotsOver(CardBattle *battle, s32 side, s32 value, s32 keep);
s32 CARDGAME_pickLowestOpponentCard(CardBattle *battle);
s32 CARDGAME_getEffectField(s32 index, u32 field, s32 offset);
void CARDGAME_jumpIfLastSide(CardBattle *battle, CardScreen *screen, s32 value, s32 offset);
void CARDGAME_jumpIfFewInHand(CardBattle *battle, s32 side, s32 value, s32 offset);
void CARDGAME_jumpIfManyInHand(CardBattle *battle, s32 side, s32 value, s32 offset);
void CARDGAME_jumpIfDeckLeft(CardBattle *battle, s32 side, s32 offset);
void CARDGAME_jumpIfSlotFree(CardBattle *battle, s32 side, s32 offset);
s32 CARDGAME_countDown(CardBattle *battle, CardScreen *screen);
void CARDGAME_chooseNextDraw(CardBattle *battle, CardScreen *screen, s32 which);
void CARDGAME_markSlotsOfOrder(CardBattle *battle, CardScreen *screen);
void CARDGAME_markBestOpponentSlot(CardBattle *battle);
s32 CARDGAME_stepEffectColorValue(CardBattle *battle, CardScreen *screen);
void CARDGAME_sortOpponentCards(CardBattle *battle);
s32 CARDGAME_canPlayCardKind(CardBattle *battle, s32 index);
s32 CARDGAME_addColorCount(CardBattle *battle, CardScreen *screen, s32 side, s32 card);
s32 CARDGAME_stepReturnSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 index);
s32 CARDGAME_waitFor(CardBattle *battle, CardScreen *screen, s32 duration);
void CARDGAME_beginStep(CardBattle *battle, CardScreen *screen);
void CARDGAME_startHandCount(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_startColorDrain(CardBattle *battle, CardScreen *screen);
void CARDGAME_startCloseGauge(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_keepLastCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_showPileCard(CardBattle *battle, CardScreen *screen, s32 side, s32 which);
s32 CARDGAME_drawFromDeck(CardBattle *battle, CardScreen *screen, s32 side, s32 which);
void CARDGAME_startDiscardCount(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_returnUsedCards(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_startDraw(CardBattle *battle, CardScreen *screen, s32 count);
s32 CARDGAME_markDrawnCards(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_drawMarkedCards(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_drawNewCards(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_startSlotSweep(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_stepSlotSweep(CardBattle *battle, CardScreen *screen, s32 which);
void CARDGAME_markRecordHits(CardBattle *battle, CardScreen *screen, s32 side, s32 index);
void CARDGAME_putSlotCard(CardBattle *battle, CardScreen *screen, s32 side, s32 card);
void CARDGAME_takeHandCard(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_moveSlotCard(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_copySlotCard(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_flipSlotCard(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_startPlayCard(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_stepPlayCard(CardBattle *battle, CardScreen *screen);
void CARDGAME_clearHands(CardBattle *battle, CardScreen *screen);
void CARDGAME_dealHands(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_stepStart(CardBattle *battle, CardScreen *screen);
void CARDGAME_startAttack(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_stepAttack(CardBattle *battle, CardScreen *screen, s32 side);
void CARDGAME_startDiscardAllSlots(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_stepDiscardAllSlots(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_interpolate(s32 to, s32 from, s32 duration, s32 time);
void CARDGAME_startSwap(CardBattle *battle, CardScreen *screen);
void CARDGAME_startTotals(CardBattle *battle, CardScreen *screen);
void CARDGAME_startRoundEnd(CardBattle *battle, CardScreen *screen);
void CARDGAME_startMessage(CardBattle *battle, CardScreen *screen, s32 text);
void CARDGAME_startClosePanels(CardBattle *battle, CardScreen *screen, s32 force);
s32 CARDGAME_stepClosePanels(CardBattle *battle, CardScreen *screen);
void CARDGAME_startOpenPanels(CardBattle *battle, CardScreen *screen, s32 force);
s32 CARDGAME_stepOpenPanels(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_checkPlayCondition(CardBattle *battle, CardScreen *screen, s32 kind);

/* The steps CARDGAME_runEffectStep starts (CardBattle.effectStep.next) and runs (effectStep.id) */
void CARDGAME_startSlotEffects(CardBattle *battle, CardScreen *screen, s32 which);
void CARDGAME_moveMarkedSlots(CardBattle *battle, CardScreen *screen, s32 arg2, s32 arg3);
s32 CARDGAME_stepSlotStats(CardBattle *battle, CardScreen *screen);
void CARDGAME_markTargetSlots(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_removeMarkedSlots(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_markPileCardsByColor(CardBattle *battle, CardScreen *screen, s32 side, s32 kind, s32 flags);
s32 CARDGAME_markSlotsByColor(CardBattle *battle, CardScreen *screen, s32 side, s32 flags);
s32 CARDGAME_discardHand(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_drainColorValues(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_discardPrevCard(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_discardPickedCard(CardBattle *battle, CardScreen *screen, s32 side, s32 arg3);
void CARDGAME_takeCardToHand(CardBattle *battle, CardScreen *screen, s32 side, s32 arg3);
s32 CARDGAME_stepSwap(CardBattle *battle, CardScreen *screen);
void CARDGAME_showSlotTotal(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_stepSlotTotal(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_stepTotals(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_stepRoundEnd(CardBattle *battle, CardScreen *screen, s32 side);
s32 CARDGAME_waitMessage(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_countSlotValues(CardBattle *battle, CardScreen *screen);
void CARDGAME_loadOpponent(CardBattle *battle, CardBattleItems *items);
extern s16 CARDGAME_prizeItems[]; /* the item an opponent gives for a win (CardBattle.prize), by its CardOpponent.prize */
void CARDGAME_sortOpponentHand(CardBattle *battle);
void CARDGAME_findOpponentDeckLimits(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_stepHidePanels(CardBattle *battle, CardBattleItems *items);
void CARDGAME_startPileCount(CardBattle *battle, CardBattleItems *items, s32 side, s32 which);
void CARDGAME_layOutPile(CardBattle *battle, CardBattleItems *items, s32 side, s32 which);
void CARDGAME_layOutSlots(CardBattle *battle, CardBattleItems *items);
void CARDGAME_startStepFade(CardBattle *battle, CardBattleItems *items);
void CARDGAME_resetSpriteFlags(CardBattle *battle);
s32 CARDGAME_runFirstPick(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_runStartStep(CardBattle *battle, CardBattleItems *items);
void CARDGAME_prepareTurn(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_chooseDeck(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_playRounds(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_runPutOut(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_endRound(CardBattle *battle, CardBattleItems *items);
void CARDGAME_removeMarkedHandCards(CardBattle *battle, CardPile *pile);
s32 CARDGAME_setAllCountingValues(CardBattle *battle);
void CARDGAME_sumSlotValues(CardBattle *battle, s32 side);
s32 CARDGAME_findCardSet(CardBattle *battle, s32 side, s32 index);
void CARDGAME_openPanelsPhase(CardBattle *battle, CardBattleItems *items);
void CARDGAME_returnHandToDeck(CardPile *pile);
void CARDGAME_startNextRound(CardBattle *battle, CardBattleItems *items);
void CARDGAME_startBattleMessage(CardBattle *battle, CardBattleItems *items);
void CARDGAME_initBattle(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_stepShowPanels(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_showPlayedCard(CardBattle *battle, CardBattleItems *items);
s32 CARDGAME_runBattle(CardBattle *battle, CardBattleItems *items);
void CARDGAME_keepLowestSlot(CardBattle *battle, s32 side);
s32 CARDGAME_targetFlaggedSlot(CardBattle *battle, s32 side);
s32 CARDGAME_targetColor3Slot(CardBattle *battle, s32 side);
s32 CARDGAME_targetColor4Slot(CardBattle *battle, s32 side);
s32 CARDGAME_isSlotWithin(CardBattle *battle, s32 side, s32 index, s32 value);
s32 CARDGAME_getCardLimit(CardBattle *battle, s32 id);
s32 CARDGAME_getCardBonus(CardBattle *battle, s32 id);
s32 CARDGAME_pickComputerTarget(CardBattle *battle, CardScreen *screen, s32 id);
s32 CARDGAME_findOpponentHandKind(CardBattle *battle, s32 kind);
s32 CARDGAME_checkComputerCondition(CardBattle *battle, CardScreen *screen, s32 id);
s32 CARDGAME_compareCards(CardBattle *battle, s32 id1, s32 id2, s32 flip);
s32 CARDGAME_pickComputerCard(CardBattle *battle, CardScreen *screen);
s32 CARDGAME_takeFlaggedCard(CardBattle *battle, CardBattleItems *items);
void CARDGAME_flagTargetSlots(CardBattle *battle, CardScreen *screen);

#endif /* CARDGAME_BATTLE_H */
