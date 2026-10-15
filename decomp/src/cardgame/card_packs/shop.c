/* The card shop's main task, which opens the pack screen or the buy screen;
   the files it loads; its helpers; and the shops' stock and prices */

#include "cardgame/card_packs.h"

/* Creates the shop's windows and its cursor */
void STCRDSHP_createShopWindows(CardShop *shop, CardShopWindows *win) {
    TextWindow **windows;
    s32 i;
    s32 j;

    win->title = createTextWindow(shop->layer, 1, 0x1D, 0x16);
    win->help = createTextWindow(shop->layer, 1, 0xD3, 0xCC);
    win->money = createTextWindow(shop->layer, 3, 0x117, 0x1C);
    win->moneyLabel = createTextWindow(shop->layer, 3, 0x11A, 0x1C);
    for (i = 0; i < 3; i++) {
        win->options[i] = createTextWindow(shop->layer, 1, 0xA7, i * 14 + 0x35);
    }
    j = 0; /* the match depends on setting it here and on the loop's form */
    win->cursor = createCursor(shop->layer, shop->depth - 2, 0xA7, shop->cursor * 14 + 0x35);
    win->cursor->setVisible(win->cursor, 0);
    win->message = createTextWindow(shop->layer, 1, 0x9A, 0x71);
    windows = shop->children;
    while (j < shop->childCount - 3) {
        j++;
        (*windows)->setDepth(*windows, shop->depth - 2);
        windows++;
    }
}

/* Shows or hides the shop's title and the money */
void STCRDSHP_showTitle(CardShop *shop, CardShopWindows *win, s32 show) {
    if (show) {
        win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_SHOP_NAMES)), shop->title);
        win->moneyLabel->setString(win->moneyLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 3);
        win->money->setNumber(win->money, 0, GAME.money);
        win->money->setRightAlign(win->money, 1);
    } else {
        win->title->setVisible(win->title, 0);
        win->moneyLabel->setVisible(win->moneyLabel, 0);
        win->money->setVisible(win->money, 0);
    }
}

/* Shows or hides the options (buy cards, open a pack, edit the decks) and the help */
void STCRDSHP_showOptions(CardShop *shop, CardShopWindows *win, s32 show) {
    s32 i;

    if (show) {
        for (i = 0; i < 3; i++) {
            win->options[i]->setString(win->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), i + 5);
        }
        win->help->setString(win->help, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 4);
    } else {
        for (i = 0; i < 3; i++) {
            win->options[i]->setVisible(win->options[i], 0);
        }
        win->help->setVisible(win->help, 0);
    }
}

/* Draws the scrolling background and the panels as they open */
void STCRDSHP_drawShop(CardShop *shop) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(shop->layer, 7);
    sprite.setTexture(0x280, 0);
    if (shop->scrollWait != 0) {
        shop->scroll++;
        shop->scroll = shop->scroll < 0x60 ? shop->scroll : 0;
        shop->scrollWait = 0;
    } else {
        shop->scrollWait = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 8, shop->scroll, shop->scroll);
    sprite.setLayerId(shop->layer, shop->depth - 1);
    if (shop->fades[0].level != 0) {
        if (shop->fades[0].level != ONE) {
            sprite.setScale(shop->fades[0].level, ONE, ONE);
            sprite.setPivot(0x57, 0x1B);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x28, 0x16, 0x14);
        if (shop->fades[0].level != ONE) {
            sprite.setPivot(0x140, 0x1D);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x29, 0xD6, 0x15);
    }
    if (shop->fades[1].level != 0) {
        if (shop->fades[1].level != ONE) {
            sprite.setScale(shop->fades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x49);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2F, 0x92, 0x2E);
        if (shop->fades[1].level != ONE) {
            sprite.setPivot(0x140, 0xD2);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xF, 0xC6, 0xC4);
    }
    if (shop->fades[2].level != 0) {
        if (shop->fades[2].level != ONE) {
            sprite.setScale(shop->fades[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x77);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2C, 0x7B, 0x6B);
    }
}

/* Up/down choose buy one card, open booster or edit folder; cross starts it,
   triangle leaves the shop */
static inline void STCRDSHP_chooseOption(CardShop *shop, CardShopWindows *win) {
    s32 cursor;

    cursor = shop->cursor;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--shop->cursor < 0) {
            shop->cursor = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++shop->cursor >= 3) {
            shop->cursor = 2;
        }
    }
    if (cursor != shop->cursor) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0x9A, shop->cursor * 14 + 0x35);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        shop->substate = 10;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        shop->setSubstate(shop, 50);
    }
}

/* Starts what was chosen: the screen to buy cards, the one to open a pack when
   the bag has one (else a message), or the deck editor once the shop has
   closed */
static inline void STCRDSHP_startOption(CardShop *shop, CardShopWindows *win) {
    s32 count;
    s32 i;

    shop->step = 1;
    switch (shop->cursor) {
    case 0:
    default:
        win->dialog = (Task *)STCRDSHP_createBuy(shop, shop->shop);
        shop->substate = 50;
        break;
    case 1:
        count = ITEM_FUNCS.list(1, shop->items);
        for (i = 0; i < count; i++) {
            if (ITEM_FUNCS.getCategory(shop->items[i]) == CARD_PACK_CATEGORY) {
                win->dialog = (Task *)STCRDSHP_createPackOpen(shop);
                shop->substate = 50;
                break;
            }
        }
        if (win->dialog == NULL) {
            shop->setSubstate(shop, 20);
        }
        break;
    case 2:
        shop->substate = 100;
        shop->toDeckEditor = 1;
        break;
    }
}

/* Hides the options and the cursor and closes their panel; leaving the shop
   (step unset), also fades the screen out */
static inline void STCRDSHP_closeOptions(CardShop *shop, CardShopWindows *win) {
    if (shop->step == 0) {
        win->fade = STCRDSHP_createFader();
        win->fade->start(win->fade, 0, 0x1E);
        win->fade->depth = 6;
    }
    STCRDSHP_showOptions(shop, win, 0);
    win->cursor->setVisible(win->cursor, 0);
    STCRDSHP_funcs.startFade(&shop->fades[1], 0);
    shop->substate++;
}

/* Once the options' panel has closed: waits for the screen chosen, or closes
   the title's panel to leave */
static inline void STCRDSHP_endOptions(CardShop *shop, CardShopWindows *win) {
    if (STCRDSHP_funcs.updateFade(&shop->fades[1]) != 0) {
        if (shop->step == 0) {
            STCRDSHP_showTitle(shop, win, 0);
            STCRDSHP_funcs.startFade(&shop->fades[0], 0);
            shop->substate++;
        } else {
            shop->substate = 11;
        }
    }
}

/* Opens the panels, moves the cursor and starts what was chosen */
void STCRDSHP_runShop(CardShop *shop, CardShopWindows *win) {
    switch (shop->substate) {
    case 0:
    default:
        STCRDSHP_funcs.startFade(&shop->fades[0], 1);
        shop->substate++;
        break;
    case 1:
        if (STCRDSHP_funcs.updateFade(&shop->fades[0]) != 0) {
            STCRDSHP_showTitle(shop, win, 1);
            STCRDSHP_funcs.startFade(&shop->fades[1], 1);
            shop->substate++;
        }
        break;
    case 2:
        if (STCRDSHP_funcs.updateFade(&shop->fades[1]) != 0) {
            STCRDSHP_showOptions(shop, win, 1);
            win->cursor->setPos(win->cursor, 0x9A, shop->cursor * 14 + 0x35);
            win->cursor->setVisible(win->cursor, 1);
            shop->substate++;
        }
        break;
    case 3:
        STCRDSHP_chooseOption(shop, win);
        break;
    case 10:
        STCRDSHP_startOption(shop, win);
        break;
    case 11:
        if (win->dialog == NULL) {
            shop->substate = 1;
        }
        break;
    case 20:
        win->cursor->setStill(win->cursor, 1);
        win->cursor->setPalette(win->cursor, PALETTE_GREY);
        STCRDSHP_funcs.startFade(&shop->fades[2], 1);
        shop->substate++;
        break;
    case 21:
        if (STCRDSHP_funcs.updateFade(&shop->fades[2]) != 0) {
            win->message->setString(win->message, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x15);
            shop->substate++;
        }
        break;
    case 22:
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            win->message->setVisible(win->message, 0);
            STCRDSHP_funcs.startFade(&shop->fades[2], 0);
            shop->substate++;
        }
        break;
    case 23:
        if (STCRDSHP_funcs.updateFade(&shop->fades[2]) != 0) {
            win->cursor->setStill(win->cursor, 0);
            win->cursor->setPalette(win->cursor, PALETTE_WHITE);
            shop->substate = 3;
        }
        break;
    case 50:
        STCRDSHP_closeOptions(shop, win);
        break;
    case 51:
        STCRDSHP_endOptions(shop, win);
        break;
    case 52:
        if (STCRDSHP_funcs.updateFade(&shop->fades[0]) != 0) {
            shop->substate = 101;
        }
        break;
    case 100:
        win->fade = STCRDSHP_createFader();
        win->fade->start(win->fade, 0, 10);
        shop->substate++;
        break;
    case 101:
        if (win->fade->state == TASK_DONE) {
            shop->state = TASK_KILL;
        }
        break;
    }
}

/* Shows the money left (after buying) */
void STCRDSHP_showMoney(CardShop *shop) {
    CardShopWindows *win = shop->children;

    win->money->setNumber(win->money, 0, GAME.money);
    win->money->setRightAlign(win->money, 1);
}

/* The shop's update: loads its files, then runs it; leaves to the field or
   the deck editor */
void STCRDSHP_updateShop(CardShop *shop, CardShopWindows *win) {
    switch (shop->state) {
    case TASK_INIT:
    default:
        switch (shop->substate) {
        case 0:
        default:
            STCRDSHP_funcs.loadFiles();
            shop->substate++;
            break;
        case 1:
            if (STCRDSHP_funcs.filesLoading() == 0) {
                STCRDSHP_createShopWindows(shop, win);
                shop->fades[0].duration = 10;
                shop->fades[1].duration = 10;
                shop->fades[2].duration = 10;
                shop->nextState(shop);
            }
            break;
        }
        break;
    case TASK_RUN:
        STCRDSHP_runShop(shop, win);
        STCRDSHP_drawShop(shop);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        if (shop->toDeckEditor != 0) {
            GAME.funcs.requestMode(MODE_DECK_EDITOR, GAME.funcs.getModeArg());
        } else {
            GAME.funcs.requestMode(GAME.fieldMode, 0);
        }
        break;
    }
}

/* Creates the card shop (task) for the shop of the mode's argument, titled for
   the field it is opened from; the cursor starts on the deck editor's option
   when coming back from it. Requests the card data and strings */
CardShop *STCRDSHP_createShop(void) {
    CardShop *shop = createTask(STCRDSHP_updateShop, sizeof(CardShop), sizeof(CardShopWindows));
    s32 mode;
    s32 i;

    shop->showMoney = STCRDSHP_showMoney;
    shop->layer = SCREEN_LAYER;
    shop->depth = 7;
    shop->shop = GAME.funcs.getModeArg();
    mode = GAME.fieldMode;
    for (i = 0; STCRDSHP_titles[i].mode != 0; i++) {
        if (STCRDSHP_titles[i].mode == mode) {
            shop->title = STCRDSHP_titles[i].title;
        }
    }
    if (shop->title == 0) {
        shop->title = 0x1F;
    }
    if (GAME.funcs.getPrevMode() == MODE_DECK_EDITOR) {
        shop->cursor = 2;
    }
    FILE_CACHE.request(FILE_CARD_DATA);
    FILE_CACHE.request(FILE_CARD_DATA + 1);
    FILE_CACHE.request(FILE_CARD_DATA + 2);
    FILE_CACHE.request(FILE_CARD_DATA + 3);
    FILE_CACHE.request(FILE_CARD_DATA + 4);
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_EFFECTS));
    return shop;
}

/* Loads the shop's textures and requests its strings */
void STCRDSHP_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(FILE_CARDSHOP_IMAGES << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_SHOP));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_EFFECTS));
    FILE_CACHE.request(TEXT_FILE(TEXT_SHOP_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
}

/* Whether the shop's strings are still loading */
s32 STCRDSHP_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_SHOP)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_EFFECTS)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_SHOP_NAMES)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_NAMES)) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"

/* The stock of a shop (the first one's if it has none), counting its cards */
CardShopStock *STCRDSHP_getStock(s32 shop) {
    s32 found = -1;
    s32 i;

    for (i = 0; STCRDSHP_stocks[i].shop != -1; i++) {
        if (STCRDSHP_stocks[i].shop == shop) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        found = 0;
    }
    STCRDSHP_stocks[found].count = 0;
    for (i = 0; STCRDSHP_stocks[found].cards[i] != 0; i++) {
        STCRDSHP_stocks[found].count++;
    }
    return &STCRDSHP_stocks[found];
}

/* The price of a card */
s16 STCRDSHP_getPrice(s32 card) {
    s32 i;

    for (i = 0; STCRDSHP_prices[i].card != 0; i++) {
        if (STCRDSHP_prices[i].card == card) {
            return STCRDSHP_prices[i].price;
        }
    }
    return 1;
}

/* The shop's title for each field it is opened from, up to mode 0 */
CardShopTitle STCRDSHP_titles[] = {
    { 529, 31 }, { 640, 32 }, { 559, 33 }, { 669, 34 }, { 574, 35 },
    { 683, 36 }, { 606, 37 }, { 711, 38 }, { 623, 39 }, { 726, 40 },
    { 0, 31 },
};
/* The shops' helpers */
CardShopFuncs STCRDSHP_funcs = {
    STCRDSHP_loadFiles, STCRDSHP_filesLoading, STCRDSHP_startFade, STCRDSHP_updateFade,
    STCRDSHP_startLerp, STCRDSHP_updateLerp, STCRDSHP_getStock, STCRDSHP_getPrice,
};
/* The cards each shop sells (STCRDSHP_stocks), up to 0 */
s16 STCRDSHP_shopCards[24][8] = {
    { 65, 235, 277, 194, 204, 59, 0, 0 },
    { 10, 210, 80, 168, 30, 59, 0, 0 },
    { 209, 167, 82, 128, 30, 59, 0, 0 },
    { 75, 202, 164, 121, 27, 59, 0, 0 },
    { 115, 157, 73, 204, 249, 27, 0, 0 },
    { 201, 116, 153, 284, 27, 30, 0, 0 },
    { 96, 226, 140, 185, 265, 259, 0, 0 },
    { 209, 167, 252, 82, 128, 259, 0, 0 },
    { 72, 115, 157, 73, 204, 249, 0, 0 },
    { 72, 115, 157, 73, 204, 285, 0, 0 },
    { 96, 10, 210, 80, 168, 30, 0, 0 },
    { 209, 167, 252, 82, 128, 259, 0, 0 },
    { 209, 72, 201, 116, 204, 249, 0, 0 },
    { 72, 115, 157, 73, 204, 285, 0, 0 },
    { 209, 167, 252, 82, 128, 259, 0, 0 },
    { 72, 115, 157, 73, 204, 249, 0, 0 },
    { 209, 72, 201, 116, 204, 249, 0, 0 },
    { 72, 201, 116, 157, 153, 284, 0, 0 },
    { 65, 106, 150, 194, 277, 235, 0, 0 },
    { 30, 75, 202, 164, 121, 27, 0, 0 },
    { 156, 75, 202, 109, 153, 285, 0, 0 },
    { 115, 157, 73, 204, 249, 30, 0, 0 },
    { 156, 70, 200, 109, 285, 27, 0, 0 },
    { 116, 285, 66, 106, 150, 194, 0, 0 },
};
/* The cards the shops sell and their prices */
CardPrice STCRDSHP_prices[] = {
    { 10, 2200 }, { 27, 11000 }, { 30, 4000 }, { 59, 1000 }, { 65, 10000 }, { 66, 10000 },
    { 70, 7000 }, { 72, 7700 }, { 73, 7700 }, { 75, 6600 }, { 80, 2200 }, { 82, 3000 },
    { 96, 500 }, { 106, 10000 }, { 109, 8000 }, { 115, 7700 }, { 116, 7700 }, { 121, 4000 },
    { 128, 3000 }, { 140, 500 }, { 150, 10000 }, { 153, 8000 }, { 156, 7000 }, { 157, 7700 },
    { 164, 4000 }, { 167, 3300 }, { 168, 2200 }, { 185, 500 }, { 194, 10000 }, { 200, 7000 },
    { 201, 7700 }, { 202, 6600 }, { 204, 5500 }, { 209, 4400 }, { 210, 2200 }, { 226, 500 },
    { 235, 13000 }, { 249, 6600 }, { 252, 4400 }, { 259, 1000 }, { 265, 500 }, { 277, 10000 },
    { 284, 9000 }, { 285, 9000 }, { 0, 1 },
};
/* Each shop's cards */
CardShopStock STCRDSHP_stocks[] = {
    { 49, 0, STCRDSHP_shopCards[0] },
    { 50, 0, STCRDSHP_shopCards[1] },
    { 51, 0, STCRDSHP_shopCards[2] },
    { 52, 0, STCRDSHP_shopCards[3] },
    { 53, 0, STCRDSHP_shopCards[4] },
    { 54, 0, STCRDSHP_shopCards[5] },
    { 55, 0, STCRDSHP_shopCards[6] },
    { 56, 0, STCRDSHP_shopCards[7] },
    { 57, 0, STCRDSHP_shopCards[8] },
    { 58, 0, STCRDSHP_shopCards[9] },
    { 59, 0, STCRDSHP_shopCards[10] },
    { 60, 0, STCRDSHP_shopCards[11] },
    { 61, 0, STCRDSHP_shopCards[12] },
    { 62, 0, STCRDSHP_shopCards[13] },
    { 63, 0, STCRDSHP_shopCards[14] },
    { 64, 0, STCRDSHP_shopCards[15] },
    { 65, 0, STCRDSHP_shopCards[16] },
    { 66, 0, STCRDSHP_shopCards[17] },
    { 67, 0, STCRDSHP_shopCards[18] },
    { 70, 0, STCRDSHP_shopCards[19] },
    { 71, 0, STCRDSHP_shopCards[20] },
    { 72, 0, STCRDSHP_shopCards[21] },
    { 73, 0, STCRDSHP_shopCards[22] },
    { 74, 0, STCRDSHP_shopCards[23] },
    { -1, 0, NULL },
};
