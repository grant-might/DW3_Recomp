#ifndef STITSHOP_H
#define STITSHOP_H

/* STITSHOP.PRO: the item shop. The game mode's argument picks the shop
   (STITSHOP_shops, a list of the items it sells); the player buys items
   for the money, sells them, and equips what was bought on a partner.
   Its strings are in file 0x72. Its modules are in src/menus/item_shop/, and the
   functions below are by module, in the order they link. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STITSHOP_##name

/* The shop's sprite sheet; the next file is its texture archive */
#if VERSION_US || VERSION_JP
#define FILE_SHOP_SPRITES 0x3F2
#elif VERSION_EU
#define FILE_SHOP_SPRITES 0x402
#endif

/* The main task of the shop (STITSHOP_createShop) */
typedef struct ItemShop {
    TASK_HEADER(ItemShop);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 bgScroll;
    /* 0x5C */ s32 bgTick;
    /* 0x60 */ s32 shop; /* the game mode's argument */
    /* 0x64 */ s32 choice;
    /* 0x68 */ PanelAnim panels[4];
    /* 0xA8 */ void (*showMoney)(struct ItemShop *shop);
} ItemShop;

/* The children of the shop */
typedef struct ItemShopWindows {
    /* 0x00 */ TextWindow *title; /* the shop's name */
    /* 0x04 */ TextWindow *help;
    /* 0x08 */ TextWindow *money;
    /* 0x0C */ TextWindow *moneyLabel;
    /* 0x10 */ TextWindow *buy;
    /* 0x14 */ TextWindow *sell;
    /* 0x18 */ Cursor *cursor;
    /* 0x1C */ Task *dialog; /* ShopBuy or ShopSell */
    /* 0x20 */ ScreenFade *fade;
} ItemShopWindows;

/* What the details panel shows of a partner */
typedef struct ShopPartnerInfo {
    /* 0x00 */ StatBlock current;
    /* 0x2C */ StatBlock withItem; /* with the item equipped */
    /* 0x58 */ s32 slot;
    /* 0x5C */ s32 changes; /* how many of the stats would change */
    /* 0x60 */ s32 rows[8]; /* from 2: the stats that change (from 1) */
} ShopPartnerInfo;

/* A row of the details panel's stats */
typedef struct ShopStatRow {
    /* 0x00 */ s32 partner;
    /* 0x04 */ s32 stat; /* into STITSHOP_stats */
    /* 0x08 */ s32 compare; /* show the stat with the item equipped */
    /* 0x0C */ s32 skip;
    /* 0x10 */ s32 skip2; /* -1: none */
} ShopStatRow;

/* The panel with the selected item's details (STITSHOP_createInfo) */
typedef struct ShopInfo {
    TASK_HEADER(ShopInfo);
    /* 0x050 */ s32 layer;
    /* 0x054 */ s32 depth;
    /* 0x058 */ s32 selling;
    /* 0x05C */ s32 page; /* 0: the description, 1: the partners' stats */
    /* 0x060 */ s32 partyCount;
    /* 0x064 */ ShopPartnerInfo partners[3];
    /* 0x1E4 */ s32 item;
    /* 0x1E8 */ s32 quantity;
    /* 0x1EC */ s32 hasStats;
    /* 0x1F0 */ s32 shown;
    /* 0x1F4 */ PanelAnim panels[4];
    /* 0x234 */ void (*showItem)(struct ShopInfo *info, s32 item, s32 quantity);
    /* 0x238 */ void (*close)(struct ShopInfo *info);
    /* 0x23C */ void (*setKindVisible)(struct ShopInfo *info, s32 visible);
    /* 0x240 */ void (*turnPage)(struct ShopInfo *info);
    /* 0x244 */ void (*refreshPartner)(struct ShopInfo *info, s32 member);
} ShopInfo;

/* A partner's rows of the details panel's second page */
typedef struct ShopInfoPartner {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *stats[2];
    /* 0x0C */ TextWindow *changes[8];
} ShopInfoPartner;

typedef struct ShopInfoWindows {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *equippedLabel;
    /* 0x08 */ TextWindow *equipped; /* GAME.equippedItems */
    /* 0x0C */ TextWindow *ownedLabel;
    /* 0x10 */ TextWindow *owned; /* GAME.items */
    /* 0x14 */ TextWindow *priceLabel;
    /* 0x18 */ TextWindow *price;
    /* 0x1C */ TextWindow *desc;
    /* 0x20 */ TextWindow *kind; /* the weapons' */
    /* 0x24 */ ShopInfoPartner partners[3];
    /* 0xA8 */ TextWindow *pageHint;
} ShopInfoWindows;

/* What ShopBuy and ShopSell start with, and are passed to the item list as:
   the list shows its selection in it */
typedef struct ShopDialog {
    TASK_HEADER(ShopDialog);
    /* 0x50 */ void (*showItem)(struct ShopDialog *dialog, s32 item, s32 quantity);
} ShopDialog;

/* The list of the items to buy or sell (STITSHOP_createItemList) */
typedef struct ShopItemList {
    TASK_HEADER(ShopItemList);
    /* 0x050 */ ShopDialog *dialog; /* ShopBuy or ShopSell */
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 selling; /* 0: the shop's items, 1: the bag's */
    /* 0x060 */ s32 type; /* the shop, or the item type to sell */
    /* 0x064 */ s16 items[0x194]; /* the bag's items that can be sold */
    /* 0x38C */ s16 bag[0x194];
    /* 0x6B4 */ s16 *shopItems;
    /* 0x6B8 */ s32 active;
    /* 0x6BC */ s32 selection;
    /* 0x6C0 */ s32 count;
    /* 0x6C4 */ s32 page;
    /* 0x6C8 */ s32 pages;
    /* 0x6CC */ s32 clutRow; /* the arrows' blink */
    /* 0x6D0 */ s32 blinkTime;
    /* 0x6D4 */ s32 pageSize;
    /* 0x6D8 */ PanelAnim panel;
    /* 0x6E8 */ void (*start)(struct ShopItemList *list);
    /* 0x6EC */ void (*close)(struct ShopItemList *list);
    /* 0x6F0 */ s32 (*getSelected)(struct ShopItemList *list);
    /* 0x6F4 */ void (*showCursor)(struct ShopItemList *list, s32 visible);
    /* 0x6F8 */ void (*freezeCursor)(struct ShopItemList *list, s32 frozen);
    /* 0x6FC */ void (*refresh)(struct ShopItemList *list);
    /* 0x700 */ void (*listBag)(struct ShopItemList *list);
} ShopItemList;

typedef struct ShopItemListWindows {
    /* 0x00 */ TextWindow *items[14];
    /* 0x38 */ TextWindow *page;
    /* 0x3C */ TextWindow *slash;
    /* 0x40 */ TextWindow *pages;
    /* 0x44 */ TextWindow *l1Label;
    /* 0x48 */ TextWindow *r1Label;
    /* 0x4C */ Cursor *cursor;
} ShopItemListWindows;

/* The dialog to buy an item (STITSHOP_createBuy) */
typedef struct ShopBuy {
    TASK_HEADER(ShopBuy);
    /* 0x50 */ void (*showItem)(struct ShopBuy *buy, s32 item, s32 quantity);
    /* 0x54 */ struct ItemShop *shop;
    /* 0x58 */ s32 layer;
    /* 0x5C */ s32 depth;
    /* 0x60 */ s32 item;
    /* 0x64 */ s32 quantity;
    /* 0x68 */ s32 max;
    /* 0x6C */ s32 blink;
    /* 0x70 */ s32 blinkTime;
    /* 0x74 */ s32 choice;
    /* 0x78 */ s32 partner; /* the one the marker is over */
    /* 0x7C */ s32 markerShown;
    /* 0x80 */ s32 markerFrame; /* the marker's clut row, 0-7 */
    /* 0x84 */ s32 markerTime;
    /* 0x88 */ PanelAnim panels[4];
} ShopBuy;

typedef struct ShopBuyWindows {
    /* 0x00 */ ShopItemList *list;
    /* 0x04 */ ShopInfo *info;
    /* 0x08 */ TextWindow *quantityLabel;
    /* 0x0C */ TextWindow *times;
    /* 0x10 */ TextWindow *quantity;
    /* 0x14 */ TextWindow *total;
    /* 0x18 */ TextWindow *yes;
    /* 0x1C */ TextWindow *no;
    /* 0x20 */ Cursor *cursor;
} ShopBuyWindows;

/* The dialog to sell an item (STITSHOP_createSell) */
typedef struct ShopSell {
    TASK_HEADER(ShopSell);
    /* 0x50 */ void (*showItem)(struct ShopSell *sell, s32 item, s32 quantity);
    /* 0x54 */ struct ItemShop *shop;
    /* 0x58 */ s32 layer;
    /* 0x5C */ s32 depth;
    /* 0x60 */ s32 type;
    /* 0x64 */ s32 item;
    /* 0x68 */ s32 quantity;
    /* 0x6C */ s32 max;
    /* 0x70 */ s32 blink; /* the quantity's arrows are shown */
    /* 0x74 */ s32 blinkTime;
    /* 0x78 */ s32 choice;
    /* 0x7C */ PanelAnim panels[5];
} ShopSell;

typedef struct ShopSellWindows {
    /* 0x00 */ ShopItemList *list;
    /* 0x04 */ ShopInfo *info;
    /* 0x08 */ Cursor *cursor;
    /* 0x0C */ TextWindow *types[4];
    /* 0x1C */ TextWindow *quantityLabel;
    /* 0x20 */ TextWindow *times;
    /* 0x24 */ TextWindow *quantity;
    /* 0x28 */ TextWindow *total;
    /* 0x2C */ TextWindow *yes;
    /* 0x30 */ TextWindow *no;
    /* 0x34 */ TextWindow *message;
} ShopSellWindows;

/* A shop: the items it sells */
typedef struct ShopList {
    /* 0x0 */ s32 count;
    /* 0x4 */ s16 *items;
} ShopList;

/* The shop's helpers (STITSHOP_funcs) */
typedef struct ItemShopFuncs {
    /* 0x00 */ s32 count; /* the items of the shop getShopItems returned */
    /* 0x04 */ void (*loadFiles)(void);
    /* 0x08 */ s32 (*filesLoading)(void);
    /* 0x0C */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x10 */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x14 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x18 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x1C */ s16 *(*getShopItems)(s32 shop);
    /* 0x20 */ s32 (*canEquip)(s32 partner, s32 item);
    /* 0x24 */ s32 (*compareEquip)(s32 partner, s32 item);
    /* 0x28 */ void (*equip)(s32 partner, s32 slot, s32 item, s32 fromBag);
} ItemShopFuncs;

/* scene.c */
void STITSHOP_updateScene(Task *task, ItemShop **child);
void STITSHOP_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STITSHOP_drawFader(ScreenFade *task);
void STITSHOP_updateFader(ScreenFade *task);
ScreenFade *STITSHOP_createFader(void);

/* trade.c */
void STITSHOP_createBuyWindows(ShopBuy *buy, ShopBuyWindows *win);
void STITSHOP_showQuantity(ShopBuy *buy, ShopBuyWindows *win, s32 show);
void STITSHOP_showBuyTotal(ShopBuy *buy, ShopBuyWindows *win, s32 show);
void STITSHOP_drawBuy(ShopBuy *buy, ShopBuyWindows *win);
void STITSHOP_runBuy(ShopBuy *buy, ShopBuyWindows *win);
void STITSHOP_updateBuy(ShopBuy *buy, ShopBuyWindows *win);
void STITSHOP_showBuyItem(ShopBuy *buy, s32 item, s32 quantity);
ShopBuy *STITSHOP_createBuy(ItemShop *shop);
void STITSHOP_createSellWindows(ShopSell *sell, ShopSellWindows *win);
void STITSHOP_drawSell(ShopSell *sell, ShopSellWindows *win);
void STITSHOP_runSell(ShopSell *sell, ShopSellWindows *win);
void STITSHOP_updateSell(ShopSell *sell, ShopSellWindows *win);
void STITSHOP_showSellItem(ShopSell *sell, s32 item, s32 quantity);
ShopSell *STITSHOP_createSell(ItemShop *shop);

/* item_list.c */
void STITSHOP_createItemListWindows(ShopItemList *list, ShopItemListWindows *win);
void STITSHOP_showItemPage(ShopItemList *list, ShopItemListWindows *win, s32 show);
void STITSHOP_drawItemList(ShopItemList *list);
void STITSHOP_runItemList(ShopItemList *list, ShopItemListWindows *win);
void STITSHOP_updateItemList(ShopItemList *list, ShopItemListWindows *win);
void STITSHOP_freezeListCursor(ShopItemList *list, s32 frozen);
void STITSHOP_showListCursor(ShopItemList *list, s32 visible);
void STITSHOP_startItemList(ShopItemList *list);
void STITSHOP_closeItemList(ShopItemList *list);
s32 STITSHOP_getSelectedItem(ShopItemList *list);
void STITSHOP_refreshItemList(ShopItemList *list);
void STITSHOP_listSellable(ShopItemList *list);
ShopItemList *STITSHOP_createItemList(ShopDialog *dialog, s32 type, s32 selling);

/* info.c */
void STITSHOP_computeStats(s32 partner, StatBlock *out);
void STITSHOP_addStat(s16 *p, s32 stat, s32 delta);
void STITSHOP_showStat(ShopInfo *info, TextWindow *win, ShopStatRow *row);
void STITSHOP_colorStat(ShopInfo *info, TextWindow *win, ShopStatRow *row);
void STITSHOP_showOtherChanges(ShopInfo *info, TextWindow **win, ShopStatRow *row);
void STITSHOP_fillPartnerRows(ShopInfo *info, ShopInfoWindows *win, s32 member);
void STITSHOP_createInfoWindows(ShopInfo *info, ShopInfoWindows *win);
void STITSHOP_showItemRows(ShopInfo *info, ShopInfoWindows *win, s32 show);
void STITSHOP_showItemDesc(ShopInfo *info, ShopInfoWindows *win, s32 show);
void STITSHOP_showPartnerStats(ShopInfo *info, ShopInfoWindows *win, s32 show);
void STITSHOP_drawStatsPage(ShopInfo *info, ShopInfoWindows *win);
void STITSHOP_runBuyInfo(ShopInfo *info, ShopInfoWindows *win);
void STITSHOP_runSellInfo(ShopInfo *info, ShopInfoWindows *win);
void STITSHOP_updateInfo(ShopInfo *info, ShopInfoWindows *win);
void STITSHOP_setInfoItem(ShopInfo *info, s32 item, s32 quantity);
void STITSHOP_closeInfo(ShopInfo *info);
void STITSHOP_setKindVisible(ShopInfo *info, s32 visible);
void STITSHOP_turnInfoPage(ShopInfo *info);
void STITSHOP_refreshPartner(ShopInfo *info, s32 member);
ShopInfo *STITSHOP_createInfo(s32 selling, s32 item);

/* shop.c */
void STITSHOP_createShopWindows(ItemShop *shop, ItemShopWindows *win);
void STITSHOP_drawShop(ItemShop *shop, ItemShopWindows *win);
void STITSHOP_runShop(ItemShop *shop, ItemShopWindows *win);
void STITSHOP_updateShop(ItemShop *shop, ItemShopWindows *win);
void STITSHOP_showMoney(ItemShop *shop);
ItemShop *STITSHOP_createShop(void);
void STITSHOP_loadFiles(void);
s32 STITSHOP_filesLoading(void);
void STITSHOP_startFade(PanelAnim *fade, s32 fadeIn);
s32 STITSHOP_updateFade(PanelAnim *fade);
void STITSHOP_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STITSHOP_updateLerp(MenuLerp *lerp);

/* equip.c */
s16 *STITSHOP_getShopItems(s32 shop);
s32 STITSHOP_canEquip(s32 partner, s32 item);
s32 STITSHOP_compareEquip(s32 partner, s32 item);
void STITSHOP_equip(s32 partner, s32 slot, s32 item, s32 fromBag);

/* STITSHOP's data (data/stitshop.c), in its order */
extern s32 STITSHOP_sellLists[]; /* the item list each kind of sale shows */
extern Vec2 STITSHOP_changePos[];
extern s32 STITSHOP_stats[]; /* the stats the info shows */
extern s32 STITSHOP_kindStrings[]; /* the strings of the item kinds */
extern s32 STITSHOP_shopNames[];
/* STITSHOP_stock0 to 30, the items of each shop, are only STITSHOP_shops' */
extern ShopList STITSHOP_shops[31];
extern ItemShopFuncs STITSHOP_funcs;

#endif /* STITSHOP_H */
