#ifndef STCRDDEK_H
#define STCRDDEK_H

/*
 * STCRDDEK: the deck editor, where the player builds the three decks from
 * the cards they own and renames them on the keyboard (name_entry.h). Its
 * modules are in src/cardgame/deck_editor/, and the functions below are by module, in
 * the order they link.
 */

/* The name of this overlay's copy of a function or type of src/shared/ */
#define OVL_NAME(name) STCRDDEK_##name

/* The overlay's NameEntry and the name entry's files (name_entry.h) */
#define NAME_ENTRY_HAS_UNUSED98 0
#define NAME_ENTRY_HAS_HIDE 0
#define NAME_ENTRY_SPRITES STCRDDEK_KEY_SPRITES
#define NAME_ENTRY_FILE_KEYBOARD STCRDDEK_FILE_KEYBOARD
#include "shared/name_entry.h"

#if VERSION_US || VERSION_JP
#define STCRDDEK_FILE_SPRITES 0x62E /* as the card shop's */
#define STCRDDEK_FILE_IMAGES 0x632
#define STCRDDEK_FILE_CARDS 0x7E7 /* the card images, the first of five */
#define STCRDDEK_FILE_KEYBOARD 0x762
#define STCRDDEK_FILE_KEY_SPRITES 0x761
#elif VERSION_EU
#define STCRDDEK_FILE_SPRITES 0x63E
#define STCRDDEK_FILE_IMAGES 0x642
#define STCRDDEK_FILE_CARDS 0x7F6
#define STCRDDEK_FILE_KEYBOARD 0x771
#define STCRDDEK_FILE_KEY_SPRITES 0x770
#endif
#define STCRDDEK_SPRITES (STCRDDEK_FILE_SPRITES << 16) /* sprite bank */
#define STCRDDEK_IMAGES (STCRDDEK_FILE_IMAGES << 16) /* TIM archive */
#define STCRDDEK_KEY_SPRITES (STCRDDEK_FILE_KEY_SPRITES << 16) /* the keyboard's and partners' sprites */

/* The screen's helpers */
typedef struct DeckFuncs {
    /* 0x00 */ void (*loadFiles)(void);
    /* 0x04 */ s32 (*filesLoading)(void);
    /* 0x08 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x0C */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x10 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x14 */ s32 (*updateLerp)(MenuLerp *lerp);
} DeckFuncs;

/* The deck being edited */
typedef struct DeckEditor {
    TASK_HEADER(DeckEditor);
    /* 0x050 */ struct DeckScreen *screen;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 deck; /* GAME.decks[] */
    /* 0x060 */ s32 column; /* of the deck's grid, 9 cards a row */
    /* 0x064 */ s32 row;
    /* 0x068 */ s32 cursorFrame;
    /* 0x06C */ s32 cursorTime;
    /* 0x070 */ s32 cursorShown;
    /* 0x074 */ s32 listCursor; /* in the list of the cards owned */
    /* 0x078 */ s32 listTop;
    /* 0x07C */ s32 listCount;
    /* 0x080 */ s32 arrowsShown;
    /* 0x084 */ s32 arrowTime;
    /* 0x088 */ s16 list[315]; /* the cards owned and not in the deck */
    /* 0x2FE */ s8 owned[315]; /* how many of each card are not in the deck */
    /* 0x43C */ s32 infoShown; /* the card's info panel */
    /* 0x440 */ s32 blinkTime;
    /* 0x444 */ PanelAnim panels[5];
} DeckEditor;

/* A line of the list of the cards owned */
typedef struct CardLine {
    /* 0x0 */ TextWindow *name;
    /* 0x4 */ TextWindow *label;
    /* 0x8 */ TextWindow *count;
} CardLine;

typedef struct DeckEditorChildren {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *hint;
    /* 0x08 */ TextWindow *deckName;
    /* 0x0C */ TextWindow *kinds[6];
    /* 0x24 */ TextWindow *cardName;
    /* 0x28 */ TextWindow *pointsLabel;
    /* 0x2C */ TextWindow *points;
    /* 0x30 */ TextWindow *apLabel;
    /* 0x34 */ TextWindow *ap;
    /* 0x38 */ TextWindow *hpLabel;
    /* 0x3C */ TextWindow *hp;
    /* 0x40 */ TextWindow *effect;
    /* 0x44 */ TextWindow *listCardName;
    /* 0x48 */ TextWindow *listPointsLabel;
    /* 0x4C */ TextWindow *listPoints;
    /* 0x50 */ TextWindow *listApLabel;
    /* 0x54 */ TextWindow *listAp;
    /* 0x58 */ TextWindow *listHpLabel;
    /* 0x5C */ TextWindow *listHp;
    /* 0x60 */ TextWindow *listEffect;
    /* 0x64 */ CardLine lines[8];
    /* 0xC4 */ Cursor *cursor;
    /* 0xC8 */ TextWindow *message;
    /* 0xCC */ struct DeckCards *cards;
    /* 0xD0 */ ScrollBar *scrollBar;
} DeckEditorChildren;

/* Draws the deck's cards, appearing one per frame */
typedef struct DeckCards {
    TASK_HEADER(DeckCards);
    /* 0x50 */ DeckEditor *editor;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 count; /* cards shown so far */
    /* 0x60 */ u8 pad[0x10]; /* never read or written */
} DeckCards;

/* A task that waits for triangle; nothing creates it */
typedef struct DeckIdle {
    TASK_HEADER(DeckIdle);
    /* 0x50 */ void *parent;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ u8 pad[0x10]; /* never read or written */
} DeckIdle;

/* A deck's row on the screen: its name and how many cards of each kind it has */
typedef struct DeckRow {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *counts[6];
} DeckRow;

typedef struct DeckScreenChildren {
    /* 0x00 */ union {
        DeckEditor *editor;
        NameEntry *name; /* renaming */
    } child; /* NULL once it is gone */
    /* 0x04 */ TextWindow *title;
    /* 0x08 */ DeckRow rows[3];
    /* 0x5C */ TextWindow *options[2];
    /* 0x64 */ Cursor *cursor;
    /* 0x68 */ ScreenFade *fader;
} DeckScreenChildren;

/* The screen's controller: the three decks to choose from */
typedef struct DeckScreen {
    TASK_HEADER(DeckScreen);
    /* 0x050 */ s32 layer;
    /* 0x054 */ s32 depth;
    /* 0x058 */ s32 scroll; /* of the background */
    /* 0x05C */ s32 tick;
    /* 0x060 */ s32 deck; /* under the cursor */
    /* 0x064 */ s32 chosen;
    /* 0x068 */ s32 cursorFrame;
    /* 0x06C */ s32 cursorTime;
    /* 0x070 */ s32 renaming;
    /* 0x074 */ s32 kinds[3][6]; /* how many cards of each kind a deck has */
    /* 0x0BC */ PanelAnim panels[3];
    /* 0x0EC */ PanelAnim rowPanels[3];
    /* 0x11C */ void (*countKinds)(struct DeckScreen *task); /* STCRDDEK_countCardKinds */
} DeckScreen;

/* scene.c */
void STCRDDEK_updateScene(Task *task, DeckScreen **child);
void STCRDDEK_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STCRDDEK_drawFader(ScreenFade *task);
void STCRDDEK_updateFader(ScreenFade *task);
ScreenFade *STCRDDEK_createFader(void);

/* deck_cards.c */
void STCRDDEK_drawDeckCards(DeckCards *task);
void STCRDDEK_loadNextCard(DeckCards *task);
void STCRDDEK_updateDeckCards(DeckCards *task);
DeckCards *STCRDDEK_createDeckCards(DeckEditor *editor);

/* editor.c */
void STCRDDEK_createEditorWindows(DeckEditor *task, DeckEditorChildren *children);
void STCRDDEK_showEditorWindows(DeckEditor *task, DeckEditorChildren *children, s32 show);
void STCRDDEK_showCardList(DeckEditor *task, DeckEditorChildren *children, s32 show);
void STCRDDEK_showListCard(DeckEditor *task, DeckEditorChildren *children, s32 show);
void STCRDDEK_buildCardList(DeckEditor *task);
void STCRDDEK_drawEditor(DeckEditor *task);
void STCRDDEK_stepEditor(DeckEditor *task, DeckEditorChildren *children);
void STCRDDEK_updateEditor(DeckEditor *task, DeckEditorChildren *children);
DeckEditor *STCRDDEK_createEditor(DeckScreen *screen, s32 deck);
void STCRDDEK_initIdle(DeckIdle *task, void *children);
void STCRDDEK_drawIdle(DeckIdle *task);
void STCRDDEK_stepIdle(DeckIdle *task, void *children);
void STCRDDEK_updateIdle(DeckIdle *task, void *children);
DeckIdle *STCRDDEK_createIdle(void *parent);

/* name_entry.c */
void STCRDDEK_startTween(PanelAnim *fade, s32 fadeIn);
s32 STCRDDEK_updateTween(PanelAnim *fade);
void STCRDDEK_createNameWindows(NameEntry *task, NameEntryWindows *windows);
void STCRDDEK_showNameWindows(NameEntry *task, NameEntryWindows *windows, s32 show);
void STCRDDEK_drawKeyboard(NameEntry *task);
void STCRDDEK_updateKeyboard(NameEntry *task, NameEntryWindows *windows);
void STCRDDEK_updateNameEntry(NameEntry *task, NameEntryWindows *windows);
void STCRDDEK_setNameVram(NameEntry *task, s32 x, s32 y);
void STCRDDEK_setName(NameEntry *task, char *name);
void STCRDDEK_getName(NameEntry *task, char *out);
void STCRDDEK_closeNameEntry(NameEntry *task);
NameEntry *STCRDDEK_createNameEntry(char *text);

/* scroll_bar.c */
void STCRDDEK_setScrollBarX(ScrollBar *bar, s32 x, s32 width);
void STCRDDEK_setScrollBarRange(ScrollBar *bar, s32 top, s32 bottom);
void STCRDDEK_setScrollBarCount(ScrollBar *bar, s32 pageSize, s32 count);
void STCRDDEK_setScrollBarPos(ScrollBar *bar, s32 pos);
void STCRDDEK_updateScrollBar(ScrollBar *bar);
ScrollBar *STCRDDEK_createScrollBar(void);

/* screen.c */
void STCRDDEK_createScreenWindows(DeckScreen *task, DeckScreenChildren *children);
void STCRDDEK_showDeckRow(DeckScreen *task, DeckScreenChildren *children, s32 deck, s32 show);
void STCRDDEK_drawScreen(DeckScreen *task);
void STCRDDEK_countCardKinds(DeckScreen *task);
void STCRDDEK_stepScreen(DeckScreen *task, DeckScreenChildren *children);
void STCRDDEK_updateScreen(DeckScreen *task, DeckScreenChildren *children);
DeckScreen *STCRDDEK_createScreen(void);
void STCRDDEK_loadFiles(void);
s32 STCRDDEK_filesLoading(void);
void STCRDDEK_startFade(PanelAnim *fade, s32 fadeIn);
s32 STCRDDEK_updateFade(PanelAnim *fade);
void STCRDDEK_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STCRDDEK_updateLerp(MenuLerp *lerp);

/* STCRDDEK's data (data/stcrddek.c), in its order */
extern s32 STCRDDEK_cursorCluts[];
extern Glyph STCRDDEK_glyphs[]; /* the name's font */
extern Glyph STCRDDEK_icons[];
extern KeyTabs STCRDDEK_keyPagesJp[];
extern KeyPage STCRDDEK_keyCharsJp[];
extern KeyTabs STCRDDEK_keyPages[];
extern KeyPage STCRDDEK_keyChars[];
extern TextStyle STCRDDEK_nameStyle;
extern s32 STCRDDEK_nameAnims[][7];
extern BigKey STCRDDEK_bigKeys[];
extern s32 STCRDDEK_keyArrowCluts[];
extern DeckFuncs STCRDDEK_funcs;
extern NameKeyboard STCRDDEK_keyboard;

#endif /* STCRDDEK_H */
