#ifndef STCRDSHP_H
#define STCRDSHP_H

/* STCRDSHP.PRO: the card packs (mode 0x1300). Opening a pack, an item, uses
   it up and draws six cards, one from each of its slots' lists
   (STCRDSHP_packs), which are then added to the player's cards. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STCRDSHP_##name
/* The card grid that src/shared's hide_cards and update_hiding take */
#define CARD_GRID CardPackGrid

/* A card pack: the item and the six lists of 16 cards its slots draw from */
typedef struct CardPack {
    /* 0x00 */ s32 item;
    /* 0x04 */ s32 *slots[6];
} CardPack;

/* The cards are drawn from 315 ids (1-314) */
#define CARD_PACK_IDS 315

/* getItemCategory's value for the card packs (the item type 28) */
#define CARD_PACK_CATEGORY 0x62

/* The sprite sheet of the screen, the shop's textures and the card data
   (five files, as STCRDABM loads them) */
#if VERSION_US || VERSION_JP
#define FILE_CARDSHOP_SPRITES 0x62E
#define FILE_CARDSHOP_IMAGES 0x632
#define FILE_CARD_DATA 0x7E7
#elif VERSION_EU
#define FILE_CARDSHOP_SPRITES 0x63E
#define FILE_CARDSHOP_IMAGES 0x642
#define FILE_CARD_DATA 0x7F6
#endif

/* Draws the six cards drawn from a pack, and turns them over */
typedef struct CardPackGrid {
    TASK_HEADER(CardPackGrid);
    /* 0x50 */ struct CardShop *owner; /* never read */
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 pad5C; /* never read or written (STCRDABM's grid: the page's first card) */
    /* 0x60 */ s32 pad60; /* never read or written (STCRDABM's grid: the page being turned's) */
    /* 0x64 */ s32 shown;  /* cards drawn */
    /* 0x68 */ s32 turned; /* slots turned so far */
    /* 0x6C */ s32 time;
    /* 0x70 */ s32 frame;
    /* 0x74 */ s32 pad74; /* never read or written */
    /* 0x78 */ s32 pad78; /* never read or written */
    /* 0x7C */ s32 cards[6];
    /* 0x94 */ s32 prevCards[6]; /* the cards being turned */
    /* 0xAC */ u8 padAC[0x10]; /* never read or written */
    /* 0xBC */ void (*setCards)(struct CardPackGrid *grid, s32 *cards);
    /* 0xC0 */ void (*hide)(struct CardPackGrid *grid);
} CardPackGrid;

/* The cards a shop sells */
typedef struct CardShopStock {
    /* 0x0 */ s32 shop;
    /* 0x4 */ s32 count;
    /* 0x8 */ s16 *cards; /* 0-terminated */
} CardShopStock;

/* A card's price */
typedef struct CardPrice {
    /* 0x0 */ s16 card;
    /* 0x2 */ s16 price;
} CardPrice;

/* The shop's title for the field it is opened from */
typedef struct CardShopTitle {
    /* 0x0 */ s32 mode; /* GAME.fieldMode; 0 ends the list */
    /* 0x4 */ s32 title; /* string of file 0x95 */
} CardShopTitle;

/* The card shop's main task (STCRDSHP_createShop): buy cards, open packs
   or edit the decks */
typedef struct CardShop {
    TASK_HEADER(CardShop);
    /* 0x050 */ s32 layer;
    /* 0x054 */ s32 depth;
    /* 0x058 */ s32 scroll; /* of the background */
    /* 0x05C */ s32 scrollWait; /* it moves every other frame */
    /* 0x060 */ s32 shop; /* the game mode's argument */
    /* 0x064 */ s32 title;
    /* 0x068 */ s32 toDeckEditor;
    /* 0x06C */ s32 cursor;
    /* 0x070 */ s16 items[0x194]; /* the bag's, to look for packs */
    /* 0x398 */ PanelAnim fades[3];
    /* 0x3C8 */ void (*showMoney)(struct CardShop *shop);
} CardShop;

/* The children of the shop */
typedef struct CardShopWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *help;
    /* 0x08 */ TextWindow *message;
    /* 0x0C */ TextWindow *moneyLabel;
    /* 0x10 */ TextWindow *money;
    /* 0x14 */ TextWindow *options[3];
    /* 0x20 */ Cursor *cursor;
    /* 0x24 */ Task *dialog; /* buying cards or opening a pack */
    /* 0x28 */ ScreenFade *fade;
} CardShopWindows;

/* The screen to open a pack (STCRDSHP_createPackOpen): the bag's packs, eight
   a page */
typedef struct CardPackOpen {
    TASK_HEADER(CardPackOpen);
    /* 0x050 */ struct CardShop *shop;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 cursor; /* the pack under the cursor, in packs[] */
    /* 0x060 */ s32 page;
    /* 0x064 */ s32 pages;
    /* 0x068 */ s32 arrowFrame; /* the page arrows' palette */
    /* 0x06C */ s32 arrowTime; /* compared with the time but never updated */
    /* 0x070 */ s32 cards[6]; /* drawn from the pack opened */
    /* 0x088 */ s32 card; /* the one under the cursor, in cards[] */
    /* 0x08C */ s32 glowShown; /* the glow of the card under the cursor */
    /* 0x090 */ s32 glowFrame;
    /* 0x094 */ s32 glowTime;
    /* 0x098 */ s16 packs[0x194]; /* the bag's card packs */
    /* 0x3C0 */ s32 packCount;
    /* 0x3C4 */ s16 items[0x194]; /* the bag's items */
    /* 0x6EC */ s32 pad; /* never read or written */
    /* 0x6F0 */ PanelAnim fades[4];
} CardPackOpen;

/* The children of the screen to open a pack */
typedef struct CardPackOpenWindows {
    /* 0x00 */ TextWindow *packs[8]; /* the page's */
    /* 0x20 */ Cursor *cursor;
    /* 0x24 */ TextWindow *name; /* the pack's */
    /* 0x28 */ TextWindow *countLabel;
    /* 0x2C */ TextWindow *count; /* how many the bag holds */
    /* 0x30 */ TextWindow *help[2];
    /* 0x38 */ TextWindow *prev; /* by the arrows */
    /* 0x3C */ TextWindow *next;
    /* 0x40 */ TextWindow *page;
    /* 0x44 */ TextWindow *slash;
    /* 0x48 */ TextWindow *pages;
    /* 0x4C */ TextWindow *cardName; /* the card under the cursor's */
    /* 0x50 */ TextWindow *pointsLabel;
    /* 0x54 */ TextWindow *points;
    /* 0x58 */ TextWindow *cardCountLabel;
    /* 0x5C */ TextWindow *cardCount; /* how many the player has */
    /* 0x60 */ TextWindow *cardText;
    /* 0x64 */ TextWindow *apLabel;
    /* 0x68 */ TextWindow *ap;
    /* 0x6C */ TextWindow *hpLabel;
    /* 0x70 */ TextWindow *hp;
    /* 0x74 */ CardPackGrid *grid;
} CardPackOpenWindows;

/* The screen to buy cards (STCRDSHP_createBuy): the shop's cards, six a page */
typedef struct CardShopBuy {
    TASK_HEADER(CardShopBuy);
    /* 0x50 */ struct CardShop *shop;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 column; /* the card under the cursor, on the page */
    /* 0x60 */ s32 cursorShown;
    /* 0x64 */ s32 cursorFrame;
    /* 0x68 */ s32 cursorTime;
    /* 0x6C */ s32 page;
    /* 0x70 */ s32 pages;
    /* 0x74 */ s32 cards[6]; /* the page's */
    /* 0x8C */ s32 arrowsShown;
    /* 0x90 */ s32 arrowsTime;
    /* 0x94 */ s32 choice;
    /* 0x98 */ s32 chosen;
    /* 0x9C */ s32 price;
    /* 0xA0 */ s32 shopId; /* the shop's game mode argument */
    /* 0xA4 */ s32 count;
    /* 0xA8 */ struct CardShopStock *stock;
    /* 0xAC */ PanelAnim fades[3];
} CardShopBuy;

typedef struct CardShopBuyWindows {
    /* 0x00 */ TextWindow *name; /* the card under the cursor's */
    /* 0x04 */ TextWindow *pointsLabel;
    /* 0x08 */ TextWindow *points;
    /* 0x0C */ TextWindow *countLabel;
    /* 0x10 */ TextWindow *count; /* how many the player has */
    /* 0x14 */ TextWindow *text; /* its effect (TEXT_CARD_EFFECTS) */
    /* 0x18 */ TextWindow *apLabel;
    /* 0x1C */ TextWindow *ap;
    /* 0x20 */ TextWindow *hpLabel;
    /* 0x24 */ TextWindow *hp;
    /* 0x28 */ TextWindow *priceLabel;
    /* 0x2C */ TextWindow *price;
    /* 0x30 */ TextWindow *prev; /* by the arrows */
    /* 0x34 */ TextWindow *next;
    /* 0x38 */ TextWindow *message; /* the total, or why the card can't be bought */
    /* 0x3C */ TextWindow *yes;
    /* 0x40 */ TextWindow *no;
    /* 0x44 */ Cursor *cursor;
    /* 0x48 */ CardPackGrid *grid;
} CardShopBuyWindows;

/* The shop's helpers (STCRDSHP_funcs) */
typedef struct CardShopFuncs {
    /* 0x00 */ void (*loadFiles)(void);
    /* 0x04 */ s32 (*filesLoading)(void);
    /* 0x08 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x0C */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x10 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x14 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x18 */ struct CardShopStock *(*getStock)(s32 shop);
    /* 0x1C */ s16 (*getPrice)(s32 card);
} CardShopFuncs;

/* STCRDSHP's data, in its order: the pack screen's, the buy screen's and the
   shop's */
extern s32 STCRDSHP_slotCards[123][16]; /* the cards each slot of a pack draws from */
extern s32 STCRDSHP_slotCards123[];
extern s32 STCRDSHP_slotCards124[86][16];
extern CardPack STCRDSHP_packs[];
extern s32 STCRDSHP_glowRows[]; /* the CLUT rows of the drawn card's glow */
extern s32 STCRDSHP_cursorRows[]; /* the CLUT rows of the cursor's frames */
extern CardShopTitle STCRDSHP_titles[];
extern CardShopFuncs STCRDSHP_funcs;
extern s16 STCRDSHP_shopCards[24][8]; /* the cards each shop sells */
extern CardPrice STCRDSHP_prices[];
extern CardShopStock STCRDSHP_stocks[];

/* pack_open.c */
void STCRDSHP_createPackOpenWindows(CardPackOpen *open, CardPackOpenWindows *win);
void STCRDSHP_showPackPage(CardPackOpen *open, CardPackOpenWindows *win, s32 show);
void STCRDSHP_showPack(CardPackOpen *open, CardPackOpenWindows *win, s32 show);
void STCRDSHP_showPackCard(CardPackOpen *open, CardPackOpenWindows *win, s32 show);
void STCRDSHP_drawPackOpen(CardPackOpen *open);
void STCRDSHP_listPacks(CardPackOpen *open);
void STCRDSHP_runPackOpen(CardPackOpen *open, CardPackOpenWindows *win);
void STCRDSHP_updatePackOpen(CardPackOpen *open, CardPackOpenWindows *win);
CardPackOpen *STCRDSHP_createPackOpen(CardShop *shop);

/* fader.c */
void STCRDSHP_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STCRDSHP_drawFader(ScreenFade *task);
void STCRDSHP_updateFader(ScreenFade *task);
ScreenFade *STCRDSHP_createFader(void);

/* card_grid.c */
void STCRDSHP_loadIcons(CardPackGrid *grid);
void STCRDSHP_setCards(CardPackGrid *grid, s32 *cards);
void STCRDSHP_hideCards(CardPackGrid *grid);
void STCRDSHP_drawCards(CardPackGrid *grid, s32 previous);
void STCRDSHP_drawTurningSlots(CardPackGrid *grid);
void STCRDSHP_updateHiding(CardPackGrid *grid);
void STCRDSHP_updateGrid(CardPackGrid *grid);
CardPackGrid *STCRDSHP_createGrid(CardShop *owner, s32 *cards);

/* buy.c */
void STCRDSHP_createBuyWindows(CardShopBuy *buy, CardShopBuyWindows *win);
void STCRDSHP_showBuyCard(CardShopBuy *buy, CardShopBuyWindows *win, s32 show);
void STCRDSHP_drawBuy(CardShopBuy *buy);
void STCRDSHP_runBuy(CardShopBuy *buy, CardShopBuyWindows *win);
void STCRDSHP_updateBuy(CardShopBuy *buy, CardShopBuyWindows *win);
CardShopBuy *STCRDSHP_createBuy(CardShop *shop, s32 shopId);

/* scene.c */
void STCRDSHP_updateScene(Task *task, CardShop **child);

/* shop.c */
void STCRDSHP_createShopWindows(CardShop *shop, CardShopWindows *win);
void STCRDSHP_showTitle(CardShop *shop, CardShopWindows *win, s32 show);
void STCRDSHP_showOptions(CardShop *shop, CardShopWindows *win, s32 show);
void STCRDSHP_drawShop(CardShop *shop);
void STCRDSHP_runShop(CardShop *shop, CardShopWindows *win);
void STCRDSHP_showMoney(CardShop *shop);
void STCRDSHP_updateShop(CardShop *shop, CardShopWindows *win);
CardShop *STCRDSHP_createShop(void);
void STCRDSHP_loadFiles(void);
s32 STCRDSHP_filesLoading(void);
void STCRDSHP_startFade(PanelAnim *fade, s32 fadeIn);
s32 STCRDSHP_updateFade(PanelAnim *fade);
void STCRDSHP_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STCRDSHP_updateLerp(MenuLerp *lerp);
CardShopStock *STCRDSHP_getStock(s32 shop);
s16 STCRDSHP_getPrice(s32 card);

#endif
