#ifndef CARDGAME_SCREEN_H
#define CARDGAME_SCREEN_H

/* CARDGAME's battle screen: its task, panels, card sprites, blinkers,
   gauges, numbers and windows, the fader, the preloader and the scene
   (screen.c, panels.c, sprites.c, windows.c, message_window.c, fader.c,
   preloader.c and scene.c). */

#include "cardgame/card_battle/types.h"

/* CARDGAME's own pictures, a TIM archive */
#if VERSION_US || VERSION_JP
#define FILE_CARDGAME_TIMS 0x24E
#elif VERSION_EU
#define FILE_CARDGAME_TIMS 0x25D
#endif

/* A file CARDGAME_tickPreloader reads; a text file is in each language */
typedef struct CardFileEntry {
    /* 0x0 */ s16 file; /* -2: the files before are enough to start, -1: the end */
    /* 0x2 */ s16 isText;
} CardFileEntry;

/* Reads CARDGAME's files into the file cache, one at a time */
typedef struct CardPreloader {
    TASK_HEADER(CardPreloader);
    /* 0x50 */ s32 ready; /* the files before the first -2 are in */
    /* 0x54 */ s16 index;
    /* 0x56 */ s16 file;
} CardPreloader;

/* Fades the screen to a colour: a POLY_F4 over it, blended (blend is the
   semi-transparency rate) */
typedef struct CardFader {
    TASK_HEADER(CardFader);
    /* 0x50 */ s32 mode; /* 0 idle, 1 fading, 2 done: the task ends */
    /* 0x54 */ s32 time; /* left */
    /* 0x58 */ s32 duration;
    /* 0x5C */ u8 blend;
    /* 0x5D */ u8 killWhenDone;
    /* 0x5E */ u8 target[3];
    /* 0x61 */ u8 from[3];
    /* 0x64 */ u8 color[3];
    /* 0x68 */ void (*setColor)(struct CardFader *fader, u8 r, u8 g, u8 b);
    /* 0x6C */ void (*start)(struct CardFader *fader, u8 r, u8 g, u8 b, s32 frames, s32 killWhenDone);
    /* 0x70 */ s32 (*isDone)(struct CardFader *fader);
    /* 0x74 */ void (*kill)(struct CardFader *fader);
} CardFader;

/* A fade colour and blend mode (CardBattle.fade - 1 picks one) */
typedef struct CardFadeColor {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 blend;
} CardFadeColor;

/* An offset from a window's corner */
typedef struct CardOffset {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} CardOffset;

/* How each of the six CardWindow windows is drawn */
typedef struct CardWindowLayout {
    /* 0x0 */ s16 x; /* its pivot, from the window's corner */
    /* 0x2 */ s16 y;
    /* 0x4 */ u8 sprite;
    /* 0x5 */ u8 kind; /* 2: a sprite of the fourth TIM, else of the third */
} CardWindowLayout;

/* A number drawn with the TIM archive's digits (CARDGAME_drawNumber) */
typedef struct CardNumber {
    /* 0x00 */ s16 value;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 scaleX;
    /* 0x08 */ s16 scaleY;
    /* 0x0A */ s16 pivotX;
    /* 0x0C */ s16 pivotY;
    /* 0x0E */ u8 digits;
    /* 0x0F */ u8 leadingZeros;
    /* 0x10 */ u8 depth;
} CardNumber;

/* The battle screen's task items */
typedef struct CardScreenItems {
    /* 0x00 */ Cursor *cursor;
    /* 0x04 */ TextWindow *texts[3];
    /* 0x10 */ TextWindow *panelTexts[2]; /* the label under each side's panel (CARDGAME_drawPanelIcon) */
    /* 0x18 */ TextWindow *moreTexts[8];
} CardScreenItems;

/* A panel's open (state 1) or close (state 3) scale, which goes over
   `duration` frames */
typedef struct CardPanelScale {
    /* 0x00 */ s16 value;
    /* 0x02 */ u8 pad2[2]; /* nothing uses it */
    /* 0x04 */ s16 time;
    /* 0x06 */ s16 duration;
    /* 0x08 */ u8 state;
    /* 0x09 */ u8 pad9[3]; /* nothing uses it */
} CardPanelScale;

/* Where CARDGAME_drawPanel draws the parts of a side's panel (CARDGAME_panelLayouts), relative to
   CardPanel.x and y (or the box's, boxX and boxY) */
typedef struct CardPanelLayout {
    /* 0x00 */ u8 unk0; /* 1 in both layouts; nothing reads it */
    /* 0x01 */ u8 frame; /* the panel's sprite */
    /* 0x02 */ u8 winsSprite; /* plus CardPanel.wins: the sprite at winsX, winsY */
    /* 0x03 */ u8 boxLabelSprite;
    /* 0x04 */ CardOffset flagIcons; /* five, 0x2A apart */
    /* 0x08 */ CardOffset flagNumbers; /* CardPanel.points, 0x2A apart */
    /* 0x0C */ CardOffset deckCount;
    /* 0x10 */ CardOffset handCount;
    /* 0x14 */ CardOffset discardCount; /* from the box */
    /* 0x18 */ CardOffset apBar;
    /* 0x1C */ CardOffset apTotal;
    /* 0x20 */ CardOffset hpBar;
    /* 0x24 */ CardOffset hpTotal;
    /* 0x28 */ CardOffset flag6Part;
    /* 0x2C */ CardOffset flag7Part;
    /* 0x30 */ CardOffset boxLabel; /* from the box */
    /* 0x34 */ CardOffset flagLights; /* five, 0x2A apart */
    /* 0x38 */ CardOffset flag6Light;
    /* 0x3C */ CardOffset flag7Light;
    /* 0x40 */ CardOffset boxLight; /* from the box */
    /* 0x44 */ CardOffset apLight;
    /* 0x48 */ CardOffset hpLight;
} CardPanelLayout;

/* One side's panel on the battle screen (CardScreen.panels): 0 is the
   player's, 1 the opponent's */
typedef struct CardPanel {
    /* 0x00 */ s16 time;
    /* 0x02 */ s16 duration;
    /* 0x04 */ s16 state;
    /* 0x06 */ s16 event; /* for a frame, until CARDGAME_slidePanel clears it: 2 the panel started sliding in, 1 it slid out; nothing reads it */
    /* 0x08 */ s32 blinkTime; /* a frame counter for the blinking (CARDGAME_drawPanel) */
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 y;
    /* 0x10 */ s16 apTotal; /* the values shown (CARDGAME_setPanelValue) */
    /* 0x12 */ s16 hpTotal;
    /* 0x14 */ u8 pad14[4]; /* nothing uses it */
    /* 0x18 */ u8 points[5]; /* one per card colour */
    /* 0x1D */ u8 deckCount;
    /* 0x1E */ u8 handCount;
    /* 0x20 */ s16 boxX; /* the discards' box, which slides on its own */
    /* 0x22 */ s16 boxY;
    /* 0x24 */ u8 pad24[4]; /* nothing uses it */
    /* 0x28 */ s32 discardCount;
    /* 0x2C */ u8 pad2C[4]; /* nothing uses it */
    /* 0x30 */ u8 flags[10];
    /* 0x3A */ u8 pad3A[2]; /* nothing uses it */
    /* 0x3C */ s16 winsX; /* where the rounds won are shown */
    /* 0x3E */ s16 winsY;
    /* 0x40 */ u8 pad40[4]; /* nothing uses it */
    /* 0x44 */ s32 wins; /* the rounds won shown */
    /* 0x48 */ CardPanelScale scale;
} CardPanel;

/* A card on the battle screen (CardScreen.sprites) */
typedef struct CardSprite {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 targetX;
    /* 0x0C */ s32 targetY;
    /* 0x10 */ s32 startX;
    /* 0x14 */ s32 startY;
    /* 0x18 */ s16 scaleX;
    /* 0x1A */ s16 scaleY;
    /* 0x1C */ s16 targetScaleX;
    /* 0x1E */ s16 targetScaleY;
    /* 0x20 */ s16 startScaleX;
    /* 0x22 */ s16 startScaleY;
    /* 0x24 */ s16 slot;
    /* 0x26 */ s16 moving;
    /* 0x28 */ s32 time;
    /* 0x2C */ s32 duration;
    /* 0x30 */ s32 growTime; /* the frames it grows for before it flies (CARDGAME_flySprite) */
    /* 0x34 */ s32 effectTime;
    /* 0x38 */ s16 color; /* the card's colour, from 0 */
    /* 0x3A */ s16 index; /* in the battle's card list */
    /* 0x3C */ s16 isKind16;
    /* 0x3E */ u8 marks[3]; /* the plays that apply to the card, drawn as marks */
    /* 0x41 */ u8 points; /* the card's */
    /* 0x42 */ u8 state;
    /* 0x43 */ u8 ap; /* the values shown, which count towards the slot's */
    /* 0x44 */ u8 hp;
    /* 0x45 */ u8 visible;
    /* 0x46 */ u8 order; /* a played card's place in the record, from 1 */
    /* 0x47 */ u8 effect; /* 1-4, drawn by CARDGAME_drawSpriteEffect */
    /* 0x48 */ u8 highlight; /* bit 0 the cursor, bit 1 picked, bit 2 glowing */
    /* 0x49 */ u8 dimmed;
} CardSprite;

/* What CARDGAME_setPanelValue sets: 0-4 a colour's points, then these */
enum CardPanelValue {
    CARD_PANEL_DECK = 5,
    CARD_PANEL_HAND,
    CARD_PANEL_DISCARDS,
    CARD_PANEL_AP,
    CARD_PANEL_HP
};

/* A blinking marker on the battle screen (CardScreen.blinkers), which can
   move to a target */
typedef struct CardBlinker {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 startX;
    /* 0x06 */ s16 startY;
    /* 0x08 */ s16 targetX;
    /* 0x0A */ s16 targetY;
    /* 0x0C */ s16 blinkTime;
    /* 0x0E */ u8 time;
    /* 0x0F */ u8 duration;
    /* 0x10 */ u8 state;
} CardBlinker;

/* A value that goes from `from` to `to` (0x1000 is 1) over `duration` frames */
typedef struct CardGauge {
    /* 0x00 */ s16 from;
    /* 0x02 */ s16 to;
    /* 0x04 */ s16 time;
    /* 0x06 */ s16 duration;
    /* 0x08 */ s16 state;
    /* 0x0A */ s16 value;
} CardGauge;

/* A window that opens (its scale goes from `from` to `to`) */
typedef struct CardWindow {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 from;
    /* 0x06 */ s16 to;
    /* 0x08 */ s16 time;
    /* 0x0A */ s16 duration;
    /* 0x0C */ s16 layout; /* in CARDGAME_windowLayouts */
    /* 0x0E */ s16 showCount;
    /* 0x10 */ s32 value; /* by its layout: a string, a card, or points | colour << 4 */
    /* 0x14 */ u8 numbers[3]; /* two numbers, and whether they are shown */
    /* 0x17 */ u8 state;
} CardWindow;

/* The battle screen's message window (CardScreen.message), which
   CARDGAME_runBattleMenu keeps a copy of (CARDGAME_savedScreenState) */
typedef struct CardMessageWindow {
    /* 0x00 */ s32 place; /* in CARDGAME_messageWindowPositions */
    /* 0x04 */ s32 message;
    /* 0x08 */ s32 pad8; /* nothing uses it */
    /* 0x0C */ s16 time;
    /* 0x0E */ s16 duration;
    /* 0x10 */ s16 choice; /* the cursor's row */
    /* 0x12 */ s16 pad12[2]; /* nothing uses it */
    /* 0x16 */ u8 state; /* 1 opening, 2 open, 4 confirmed, 5 closing */
    /* 0x17 */ u8 prompt; /* what it asks: 0 nothing, else a choice with the cursor */
} CardMessageWindow;

/* The battle screen (created by CARDGAME_createScreen): the two panels, the cards on
   the table and the windows. Its 14 task items are a cursor and 13 text
   windows. */
typedef struct CardScreen {
    TASK_HEADER(CardScreen);
    /* 0x050 */ s16 *cards; /* the battle's card list */
    /* 0x054 */ s32 spriteFlags; /* this frame: bit 0 a sprite is animated, bit 1 a flying one landed */
    /* 0x058 */ u32 time;
    /* 0x05C */ s16 opponent; /* CardBattle.arg */
    /* 0x05E */ s16 opponentLevel; /* CardBattle.opponentLevel */
    /* 0x060 */ CardPanel panels[2];
    /* 0x108 */ CardSprite sprites[40];
    /* 0xCE8 */ CardBlinker blinkers[12];
    /* 0xDC0 */ CardGauge gauges[3];
    /* 0xDE4 */ CardMessageWindow message;
    /* 0xDFC */ u8 padDFC[4]; /* nothing uses it */
    /* 0xE00 */ s16 menuTime;
    /* 0xE02 */ s16 menuDuration;
    /* 0xE04 */ s16 menuRow;
    /* 0xE06 */ u8 padE06[4]; /* nothing uses it */
    /* 0xE0A */ s16 menuState;
    /* 0xE0C */ CardWindow windows[6];
    /* 0xE9C */ u8 fadeRow; /* the background's palette row, 0 to 11 */
    /* 0xE9D */ u8 fadeTime; /* four frames a row */
    /* 0xE9E */ u8 fadeState; /* 0 dark, 1 fading in, 2 shown */
    /* 0xEA0 */ void (*setPanelValue)(struct CardScreen *screen, s32 side, u32 which, s32 value);
    /* 0xEA4 */ void (*openGauge)(struct CardScreen *screen, s32 index, s16 value);
    /* 0xEA8 */ void (*closeGauge)(struct CardScreen *screen, s32 index);
    /* 0xEAC */ void (*openWindow)(struct CardScreen *screen, s32 index, s16 layout, s32 value, s32 x, s32 y);
    /* 0xEB0 */ void (*closeWindow)(struct CardScreen *screen, s32 index);
    /* 0xEB4 */ void (*clearPanelFlags)(struct CardScreen *screen);
    /* 0xEB8 */ void (*setPanelFlags)(struct CardScreen *screen, s32 bits);
    /* 0xEBC */ void (*closePanel)(struct CardScreen *screen, s32 side);
    /* 0xEC0 */ void (*openPanel)(struct CardScreen *screen, s32 side);
    /* 0xEC4 */ void (*closePanels)(struct CardScreen *screen);
    /* 0xEC8 */ void (*openPanels)(struct CardScreen *screen);
    /* 0xECC */ void (*resetPanels)(struct CardScreen *screen);
    /* 0xED0 */ void (*openPanelIcon)(struct CardScreen *screen, s32 side);
    /* 0xED4 */ void (*closePanelIcon)(struct CardScreen *screen, s32 side);
    /* 0xED8 */ s32 (*getHandOffset)(s32 count, s32 index);
    /* 0xEDC */ s32 (*showBlinker)(struct CardScreen *screen, s32 index, s16 x, s16 y);
    /* 0xEE0 */ s32 (*startBlinkerMove)(struct CardScreen *screen, s32 index, u8 duration, s16 x, s32 y);
    /* 0xEE4 */ void (*openMessage)(struct CardScreen *screen, s32 message, s32 prompt, s16 choice, s32 place);
    /* 0xEE8 */ void (*closeMessage)(struct CardScreen *screen);
    /* 0xEEC */ void (*confirmMessage)(struct CardScreen *screen);
    /* 0xEF0 */ void (*setMessageChoice)(struct CardScreen *screen, s32 choice);
    /* 0xEF4 */ void (*openMenu)(struct CardScreen *screen, s16 row);
    /* 0xEF8 */ void (*closeMenu)(struct CardScreen *screen);
    /* 0xEFC */ void (*confirmMenu)(struct CardScreen *screen);
    /* 0xF00 */ void (*setMenuRow)(struct CardScreen *screen, s16 row);
    /* 0xF04 */ void (*dealSprites)(struct CardScreen *screen, s16 duration, s16 count, s32 x, s32 y);
    /* 0xF08 */ void (*startMove)(struct CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
    /* 0xF0C */ void (*startSlide)(struct CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
    /* 0xF10 */ s32 (*startFly)(struct CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
    /* 0xF14 */ s32 (*addSprite)(struct CardScreen *screen, s32 index, s32 x, s32 y);
    /* 0xF18 */ s32 (*removeSprite)(struct CardScreen *screen, s32 index);
    /* 0xF1C */ s32 (*startBlink)(struct CardScreen *screen, s32 index);
    /* 0xF20 */ void (*setSpriteScale)(struct CardScreen *screen, s32 index, s32 scaleX, s32 scaleY);
    /* 0xF24 */ void (*scaleSprite)(struct CardScreen *screen, s32 index, s32 duration, s32 scaleX, s32 scaleY);
    /* 0xF28 */ s32 (*startFlip)(struct CardScreen *screen, s32 index);
    /* 0xF2C */ s32 (*startJitter)(struct CardScreen *screen, s32 index);
    /* 0xF30 */ s32 (*startShake)(struct CardScreen *screen, s32 index);
    /* 0xF34 */ s32 (*startRecovery)(struct CardScreen *screen, s32 index);
    /* 0xF38 */ s32 (*startEffect)(struct CardScreen *screen, s32 index, s32 which);
    /* 0xF3C */ void (*setSpriteCard)(struct CardScreen *screen, s32 sprite, s32 index);
    /* 0xF40 */ s32 (*getCardColor)(struct CardScreen *screen, s32 index);
    /* 0xF44 */ s32 (*loadCardImages)(s16 *dst, s16 *player, s16 *opponent);
} CardScreen;

/* The functions and data the overlay's files share */
extern RECT CARDGAME_screenRect;
extern RECT CARDGAME_fadeRect;
extern CardFileEntry CARDGAME_preloadFiles[];
extern s16 CARDGAME_otherCards[]; /* the 100 cards whose images CARDGAME_loadCardImages loads after the decks (CARDGAME_getOtherCard) */
extern s16 CARDGAME_commonCards[]; /* the cards everyone has, loaded after the two decks */
extern CardOffset CARDGAME_gaugePositions[]; /* where the three CardGauge gauges are */
extern u16 CARDGAME_windowTextOffsets[]; /* a window's text x, by CARDGAME_drawWindowText's index */
extern CardWindowLayout CARDGAME_windowLayouts[];
void CARDGAME_drawBackground(CardScreen *screen);
void CARDGAME_updateMessageWindow(CardScreen *screen, CardScreenItems *items);
void CARDGAME_updateWindow(CardScreen *screen, CardScreenItems *items, CardWindow *window, s32 index);
void CARDGAME_updateMenuWindow(CardScreen *screen, CardScreenItems *items);
void CARDGAME_updateBlinkers(CardScreen *screen, CardScreenItems *items);
void CARDGAME_updatePanels(CardScreen *screen, CardScreenItems *items);
void CARDGAME_updateSprites(CardScreen *screen, CardScreenItems *items);
void CARDGAME_moveBlinker(CardBlinker *blinker);
void CARDGAME_drawBlinker(CardBlinker *blinker);
void CARDGAME_drawPanelIcon(CardScreen *screen, CardScreenItems *items, s32 side, CardPanelScale *scale);
void CARDGAME_updatePanelIcon(CardScreen *screen, CardScreenItems *items, s32 side, CardPanelScale *scale);
void CARDGAME_slidePanel(CardPanel *panel, CardScreenItems *items, s32 side);
void CARDGAME_drawPanel(CardPanel *panel, CardScreenItems *items, s32 side);
void CARDGAME_drawSpriteHighlights(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteMarks(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteOrder(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteEffect(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteDim(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteCard(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSpriteBlink(CardScreen *screen, CardSprite *sprite);
void CARDGAME_animateSprite(CardScreen *screen, CardScreenItems *items, CardSprite *sprite);
extern s16 CARDGAME_shakeOffsets[]; /* CARDGAME_shakeSprite's x offsets, by time */
extern s16 CARDGAME_jitterOffsets[]; /* CARDGAME_jitterSprite's x and y offsets */

/* The steps CARDGAME_runEffectStep starts (CardBattle.effectStep.next) and runs (effectStep.id) */
void CARDGAME_updateScene(Task *task, CardBattle **items);
void CARDGAME_drawNumber(CardNumber *number, s32 scaled);
void CARDGAME_drawGauge(CardScreen *screen, CardScreenItems *items, s32 index, CardGauge *p);
void CARDGAME_updateGauge(CardScreen *screen, CardScreenItems *items, s32 index, CardGauge *p);
void CARDGAME_updateGauges(CardScreen *screen, CardScreenItems *items);
void CARDGAME_drawWindowFrame(CardScreen *screen, CardScreenItems *items, CardWindow *window);
void CARDGAME_drawWindowNumbers(CardScreen *screen, CardScreenItems *items, CardWindow *window);
void CARDGAME_drawWindowText(CardScreen *screen, CardWindow *window, TextWindow *text, s32 file, s32 index);
void CARDGAME_drawCenteredText(CardScreen *screen, CardScreenItems *items, CardWindow *window, TextWindow *text, s32 file);
void CARDGAME_updateWindows(CardScreen *screen, CardScreenItems *items);
void CARDGAME_drawMessageFrame(CardScreen *screen, CardScreenItems *items, s16 scale, s32 x, s32 y);
void CARDGAME_drawMessageMark(CardScreen *screen, CardScreenItems *items, s32 x, s32 y);
void CARDGAME_drawMenuFrame(CardScreen *screen, CardScreenItems *items, s16 scale, s32 x, s32 y);
s32 CARDGAME_getHandOffset(s32 count, s32 index);
s32 CARDGAME_shakeSprite(CardScreen *screen, CardSprite *sprite);
s32 CARDGAME_holdSpriteThenBlink(CardScreen *screen, CardSprite *sprite);
s32 CARDGAME_holdSprite(CardScreen *screen, CardSprite *sprite, s32 duration);
s32 CARDGAME_jitterSprite(CardScreen *screen, CardSprite *sprite);
s32 CARDGAME_flipSprite(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawSprite(CardScreen *screen, CardSprite *sprite);
void CARDGAME_updateScreen(CardScreen *screen, CardScreenItems *items);
void CARDGAME_openGauge(CardScreen *screen, s32 index, s16 value);
void CARDGAME_closeGauge(CardScreen *screen, s32 index);
void CARDGAME_openWindow(CardScreen *screen, s32 index, s16 layout, s32 value, s32 x, s32 y);
void CARDGAME_closeWindow(CardScreen *screen, s32 index);
s32 CARDGAME_showBlinker(CardScreen *screen, s32 index, s16 x, s16 y);
s32 CARDGAME_startBlinkerMove(CardScreen *screen, s32 index, u8 duration, s16 x, s32 y);
void CARDGAME_openMessage(CardScreen *screen, s32 arg1, s32 arg2, s16 arg3, s32 arg4);
void CARDGAME_closeMessage(CardScreen *screen);
void CARDGAME_confirmMessage(CardScreen *screen);
void CARDGAME_setMessageChoice(CardScreen *screen, s32 value);
void CARDGAME_openMenuWindow(CardScreen *screen, s16 value);
void CARDGAME_closeMenuWindow(CardScreen *screen);
void CARDGAME_confirmMenuWindow(CardScreen *screen);
void CARDGAME_setMenuWindowRow(CardScreen *screen, s16 value);
void CARDGAME_resetPanels(CardScreen *screen);
void CARDGAME_openPanels(CardScreen *screen);
void CARDGAME_closePanels(CardScreen *screen);
void CARDGAME_openPanel(CardScreen *screen, s32 side);
void CARDGAME_closePanel(CardScreen *screen, s32 side);
void CARDGAME_openPanelIcon(CardScreen *screen, s32 side);
void CARDGAME_closePanelIcon(CardScreen *screen, s32 side);
s32 CARDGAME_addSprite(CardScreen *screen, s32 index, s32 x, s32 y);
s32 CARDGAME_startSpriteFlip(CardScreen *screen, s32 index);
s32 CARDGAME_startSpriteJitter(CardScreen *screen, s32 index);
s32 CARDGAME_startSpriteShake(CardScreen *screen, s32 index);
s32 CARDGAME_startSpriteRecovery(CardScreen *screen, s32 index);
s32 CARDGAME_startSpriteEffect(CardScreen *screen, s32 index, s32 which);
s32 CARDGAME_setSpriteScaleTarget(CardScreen *screen, s32 index, s32 duration, s32 scaleX, s32 scaleY, s32 instant);
void CARDGAME_setSpriteScale(CardScreen *screen, s32 index, s32 scaleX, s32 scaleY);
void CARDGAME_scaleSprite(CardScreen *screen, s32 index, s32 duration, s32 scaleX, s32 scaleY);
void CARDGAME_setSpriteMove(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
void CARDGAME_startSpriteMove(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
void CARDGAME_startSpriteSlide(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
s32 CARDGAME_startSpriteFly(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y);
void CARDGAME_setPanelValue(CardScreen *screen, s32 side, u32 which, s32 value);
void CARDGAME_setPanelFlags(CardScreen *screen, s32 bits);
void CARDGAME_clearPanelFlags(CardScreen *screen);
s32 CARDGAME_removeSprite(CardScreen *screen, s32 index);
s32 CARDGAME_startSpriteBlink(CardScreen *screen, s32 index);
void CARDGAME_dealSprites(CardScreen *screen, s16 duration, s16 count, s32 x, s32 y);
s32 CARDGAME_getCardColor(CardScreen *screen, s32 index);
void CARDGAME_setSpriteCard(CardScreen *screen, s32 sprite, s32 index);
void CARDGAME_loadCardImage(TimLoader *loader, u8 *image, s32 slot);
s32 CARDGAME_getOtherCard(s32 index);
s32 CARDGAME_loadCardImages(s16 *dst, s16 *player, s16 *opponent);
CardScreen *CARDGAME_createScreen(s16 *cards);
void CARDGAME_tickPreloader(CardPreloader *task);
CardPreloader *CARDGAME_startPreloader(void);
void CARDGAME_moveSprite(CardScreen *screen, CardSprite *sprite);
s32 CARDGAME_flySprite(CardScreen *screen, CardSprite *sprite);
void CARDGAME_drawCardPicture(CardSprite *sprite);
extern s32 CARDGAME_pictureSheet[]; /* the sprite sheet CARDGAME_drawCardPicture draws a card's picture with */
extern u8 CARDGAME_loopFrames[]; /* the frames of a sprite's effect 1, which loops */
extern u8 CARDGAME_onceFrames[]; /* the frames of its effect 2, which plays once */
extern u8 CARDGAME_highlightCluts[]; /* a palette cycle: 0, 1, 2, 3, 2, 1 */
extern s32 CARDGAME_messageWindowPositions[][2]; /* the message window's places (x, y), by CardScreen.message.place */
extern CardPanelLayout CARDGAME_panelLayouts[2]; /* the panels' layouts, by side */
extern u8 CARDGAME_panelLightCluts[8]; /* the clut rows of the panel's blinking lights: 0, 1, 2, 3, 2, 1 */
extern u8 CARDGAME_panelBarSprites[8][2]; /* the sprites of the panel's two bars, by LANGUAGE (USA: 1) */
extern u8 CARDGAME_flag7Frames[]; /* the frames of the panel's sprites for flags 7, 6 and 5 and its five flag icons (CARDGAME_flagIconSprites), when the flag is set: 0, 1, 2, 1, 0 */
extern u8 CARDGAME_flag6Frames[];
extern u8 CARDGAME_boxFrames[];
extern u8 CARDGAME_flagIconSprites[]; /* the sprites of the panel's five flag icons */
extern u8 CARDGAME_flagIconFrames[];
void CARDGAME_drawFader(CardFader *fader, RECT rect);
void CARDGAME_stepFader(CardFader *fader);
void CARDGAME_setFaderColor(CardFader *fader, u8 r, u8 g, u8 b);
void CARDGAME_startFade(CardFader *fader, u8 r, u8 g, u8 b, s32 frames, s32 killWhenDone);
s32 CARDGAME_isFadeDone(CardFader *fader);
void CARDGAME_killFader(CardFader *fader);
void CARDGAME_tickFader(CardFader *fader);
CardFader *CARDGAME_createFader(u8 blend);

#endif /* CARDGAME_SCREEN_H */
