/* The screen that buys cards from a shop's stock */

#include "cardgame/card_packs.h"

/* Creates the screen's windows and its cursor */
void STCRDSHP_createBuyWindows(CardShopBuy *buy, CardShopBuyWindows *win) {
    win->name = createTextWindow(buy->layer, 1, 0x88, 0x80);
    win->pointsLabel = createTextWindow(buy->layer, 1, 0x115, 0x80);
    win->points = createTextWindow(buy->layer, 1, 0x126, 0x80);
    win->countLabel = createTextWindow(buy->layer, 1, 0x115, 0xA6);
    win->count = createTextWindow(buy->layer, 1, 0x12C, 0xA6);
    win->text = createTextWindow(buy->layer, 1, 0x50, 0x97);
    win->apLabel = createTextWindow(buy->layer, 1, 0xCE, 0x97);
    win->ap = createTextWindow(buy->layer, 1, 0xF0, 0x97);
    win->hpLabel = createTextWindow(buy->layer, 1, 0xCE, 0xA4);
    win->hp = createTextWindow(buy->layer, 1, 0xF0, 0xA4);
    win->price = createTextWindow(buy->layer, 3, 0x117, 0xC7);
    win->priceLabel = createTextWindow(buy->layer, 3, 0x11A, 0xC7);
    win->prev = createTextWindow(buy->layer, 1, 0x12, 0x67);
    win->next = createTextWindow(buy->layer, 1, 0x121, 0x67);
    win->next->setDepth(win->next, buy->depth);
    win->message = createTextWindow(buy->layer, 1, 0x9A, 0x39);
    win->yes = createTextWindow(buy->layer, 1, 0xC5, 0x56);
    win->no = createTextWindow(buy->layer, 1, 0xC5, 0x66);
    win->cursor = createCursor(buy->layer, buy->depth - 3, 0xB8, 0x56);
    win->cursor->setVisible(win->cursor, 0);
}

/* Shows (or hides) the card under the cursor's name, price, how many the
   player has and its text or numbers */
void STCRDSHP_showBuyCard(CardShopBuy *buy, CardShopBuyWindows *win, s32 show) {
    CardDrawer drawer;
    s32 card;

    card = buy->stock->cards[buy->page * 6 + buy->column];
    if (show) {
        initCardDrawer(&drawer);
        drawer.setCard(card);
        win->name->setString(win->name, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), card);
        win->priceLabel->setString(win->priceLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 3);
        win->price->setNumber(win->price, 0, STCRDSHP_funcs.getPrice(card));
        win->price->setRightAlign(win->price, 1);
        win->countLabel->setString(win->countLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
        win->count->setNumber(win->count, 0, GAME.cards[card]);
        win->count->setRightAlign(win->count, 1);
        if (drawer.getKind() != 0) {
            win->pointsLabel->setVisible(win->pointsLabel, 0);
            win->points->setVisible(win->points, 0);
            win->text->setString(win->text, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), card);
            win->apLabel->setVisible(win->apLabel, 0);
            win->ap->setVisible(win->ap, 0);
            win->hpLabel->setVisible(win->hpLabel, 0);
            win->hp->setVisible(win->hp, 0);
        } else {
            win->pointsLabel->setString(win->pointsLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
            win->points->setNumber(win->points, 0, drawer.card->points);
            win->points->setRightAlign(win->points, 1);
            if (card == 0x45 || card == 0x70 || card == 0x9B || card == 0xC6 || card == 0xF1) {
                win->text->setString(win->text, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), card);
                win->apLabel->setVisible(win->apLabel, 0);
                win->ap->setVisible(win->ap, 0);
                win->hpLabel->setVisible(win->hpLabel, 0);
                win->hp->setVisible(win->hp, 0);
            } else {
                win->text->setVisible(win->text, 0);
                win->apLabel->setString(win->apLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x11);
                win->ap->setNumber(win->ap, 0, drawer.card->ap);
                win->ap->setRightAlign(win->ap, 1);
                win->hpLabel->setString(win->hpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x12);
                win->hp->setNumber(win->hp, 0, drawer.card->hp);
                win->hp->setRightAlign(win->hp, 1);
            }
        }
    } else {
        win->name->setVisible(win->name, 0);
        win->priceLabel->setVisible(win->priceLabel, 0);
        win->price->setVisible(win->price, 0);
        win->countLabel->setVisible(win->countLabel, 0);
        win->count->setVisible(win->count, 0);
        win->pointsLabel->setVisible(win->pointsLabel, 0);
        win->points->setVisible(win->points, 0);
        win->text->setVisible(win->text, 0);
        win->apLabel->setVisible(win->apLabel, 0);
        win->ap->setVisible(win->ap, 0);
        win->hpLabel->setVisible(win->hpLabel, 0);
        win->hp->setVisible(win->hp, 0);
    }
}

/* Draws the screen: the card under the cursor's frames, the cursor, the
   arrows and the title */
void STCRDSHP_drawBuy(CardShopBuy *buy) {
    SpriteDrawer sprite;
    CardDrawer drawer;
    s32 card;
    s32 kind;
    s32 frame;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(buy->layer, buy->depth);
    card = buy->stock->cards[buy->page * 6 + buy->column];
    if (buy->fades[0].level != 0) {
        initCardDrawer(&drawer);
        drawer.setCard(card);
        if (buy->fades[0].level != ONE) {
            sprite.setScale(buy->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x87);
        }
        kind = drawer.getKind();
        if (kind == 1) {
            frame = 0x12;
        } else if (kind == 2) {
            frame = 0x13;
        } else {
            frame = drawer.card->color + 0x13;
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), frame, 0x103, 0x7E);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xD, 0xFC, 0x7C);
        if (buy->fades[0].level != ONE) {
            sprite.setPivot(0x140, 0x87);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xA, 0x82, 0x7C);
        if (buy->fades[0].level != ONE) {
            sprite.setPivot(0x140, 0xAF);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x10, 0x103, 0xA4);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xD, 0xFC, 0xA2);
        if (buy->fades[0].level != ONE) {
            sprite.setPivot(0x140, 0xA5);
        }
        if (kind != 0) {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xB, 0x4A, 0x92);
        } else if (card == 0x45 || card == 0x70 || card == 0x9B || card == 0xC6 || card == 0xF1) {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xB, 0x4A, 0x92);
        } else {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xC, 0xC7, 0x92);
        }
        if (buy->fades[1].level != ONE) {
            sprite.setPivot(0x140, 0xC8);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x29, 0xD6, 0xBF);
    }
    if (buy->cursorShown != 0) {
        if (GFX.funcs.getTime() - buy->cursorTime >= 4) {
            buy->cursorTime = GFX.funcs.getTime();
            if (++buy->cursorFrame >= 6) {
                buy->cursorFrame = 0;
            }
        }
        sprite.setClutRow(STCRDSHP_cursorRows[buy->cursorFrame]);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 7, buy->column * 0x2A + 0x24, 0x44);
        sprite.setClutRow(0);
        if (buy->pages >= 2) {
            if (GFX.funcs.getTime() - buy->arrowsTime >= 0x11) {
                buy->arrowsTime = GFX.funcs.getTime();
                buy->arrowsShown = 1 - buy->arrowsShown;
            }
            if (buy->fades[0].level == ONE && buy->arrowsShown != 0) {
                if (buy->page > 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x1A, 0xE, 0x55);
                }
                if (buy->page < buy->pages - 1) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x1B, 0x121, 0x55);
                }
            }
        }
    }
    if (buy->fades[1].level != 0) {
        initSpriteDrawer(&sprite);
        sprite.setTexture(0x280, 0);
        sprite.setLayerId(buy->layer, buy->depth - 2);
        if (buy->fades[1].level != ONE) {
            sprite.setScale(buy->fades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x3F);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2C, 0x7B, 0x33);
        if (buy->fades[2].level != 0) {
            if (buy->fades[2].level != ONE) {
                sprite.setScale(buy->fades[2].level, ONE, ONE);
                sprite.setPivot(0x140, 0x64);
            }
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2D, 0xAF, 0x50);
        }
    }
}

/* Once the card's panel has opened: shows the cursor, the card under it and
   the R1 arrow's label when there is a next page */
static inline void STCRDSHP_openBuyCard(CardShopBuy *buy, CardShopBuyWindows *win) {
    if (STCRDSHP_funcs.updateFade(&buy->fades[0])) {
        buy->cursorShown = 1;
        STCRDSHP_showBuyCard(buy, win, 1);
        if (buy->pages >= 2) {
            if (buy->page < buy->pages - 1) {
                win->next->setString(win->next, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xB);
            } else {
                win->next->setVisible(win->next, 0);
            }
        }
        buy->substate++;
    }
}

/* L1/R1 turn the page: the cursor goes back to its first card, the grid
   lays out the page's cards and the arrows' labels show where there are more */
static inline void STCRDSHP_turnBuyPage(CardShopBuy *buy, CardShopBuyWindows *win) {
    s32 old;
    s32 i;

    old = buy->page;
    if (!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) {
        buy->page--;
        if (buy->page < 0) {
            buy->page = 0;
        }
    } else if (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) {
        buy->page++;
        if (buy->page > buy->pages - 1) {
            buy->page = buy->pages - 1;
        }
    }
    if (old != buy->page) {
        SOUND.playSound(SOUND_MENU_MOVE);
        buy->cursorShown = 0;
        buy->column = 0;
        for (i = 0; i < 6; i++) {
            buy->cards[i] = buy->stock->cards[buy->page * 6 + i];
        }
        win->grid->setCards(win->grid, buy->cards);
        if (buy->pages >= 2) {
            if (buy->page > 0) {
                win->prev->setString(win->prev, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xA);
            } else {
                win->prev->setVisible(win->prev, 0);
            }
            if (buy->page < buy->pages - 1) {
                win->next->setString(win->next, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xB);
            } else {
                win->next->setVisible(win->next, 0);
            }
        }
        buy->substate = 1;
    }
}

/* The cursor on the page's cards: left/right move it (only onto a card), L1/R1
   turn the page and lay out its cards; cross asks to buy the card, unless the
   money is short or the player has 9 of it (a message), triangle leaves */
static inline void STCRDSHP_chooseBuyCard(CardShopBuy *buy, CardShopBuyWindows *win) {
    CardDrawer drawer;
    s32 old;

    old = buy->column;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        buy->column--;
        if (buy->column < 0) {
            buy->column = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        buy->column++;
        if (buy->column >= 6) {
            buy->column = 5;
        }
    }
    if (old != buy->column) {
        if (buy->stock->cards[buy->page * 6 + buy->column] != 0) {
            SOUND.playSound(SOUND_MENU_MOVE);
            STCRDSHP_showBuyCard(buy, win, 1);
        } else {
            buy->column = old;
        }
    }
    STCRDSHP_turnBuyPage(buy, win);
    if (PAD_PRESSED(PAD_CROSS)) {
        initCardDrawer(&drawer);
        buy->chosen = buy->stock->cards[buy->page * 6 + buy->column];
        drawer.setCard(buy->chosen);
        buy->price = STCRDSHP_funcs.getPrice(buy->chosen);
        SOUND.playSound(SOUND_MENU_CONFIRM);
        if (GAME.money < buy->price) {
            buy->substate = 10;
            buy->step = 0x13;
        } else if (GAME.cards[buy->chosen] == 9) {
            buy->substate = 10;
            buy->step = 0x14;
        } else {
            buy->substate = 5;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->substate = 50;
    }
}

/* Once the total's panels have opened: shows the price with buy and no, and
   the cursor */
static inline void STCRDSHP_openBuyTotal(CardShopBuy *buy, CardShopBuyWindows *win) {
    STCRDSHP_funcs.updateFade(&buy->fades[2]);
    if (STCRDSHP_funcs.updateFade(&buy->fades[1])) {
        win->message->setString(win->message, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xC);
        win->message->setNumber(win->message, 1, buy->price);
        win->yes->setString(win->yes, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xD);
        win->no->setString(win->no, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xE);
        win->cursor->setPos(win->cursor, 0xB8, buy->choice * 0x10 + 0x56);
        win->cursor->setVisible(win->cursor, 1);
        buy->substate++;
    }
}

/* Buy or no to the price: buy adds the card and pays for it; triangle declines */
static inline void STCRDSHP_confirmBuyCard(CardShopBuy *buy, CardShopBuyWindows *win) {
    s32 old;

    old = buy->choice;
    if (PAD_PRESSED(PAD_UP)) {
        buy->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        buy->choice = 1;
    }
    if (old != buy->choice) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB8, buy->choice * 0x10 + 0x56);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (buy->choice == 0) {
            GAME.funcs.addCards(buy->chosen, 1);
            GAME.money -= buy->price;
            buy->shop->showMoney(buy->shop);
        }
        buy->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->substate++;
    }
}

/* Hides the total, its answers and the cursor, and closes the panel */
static inline void STCRDSHP_closeBuyTotal(CardShopBuy *buy, CardShopBuyWindows *win) {
    win->message->setVisible(win->message, 0);
    win->yes->setVisible(win->yes, 0);
    win->no->setVisible(win->no, 0);
    win->cursor->setVisible(win->cursor, 0);
    buy->choice = 0;
    STCRDSHP_funcs.startFade(&buy->fades[1], 0);
    buy->substate++;
}

/* Leaving: hides the cursor, the arrows' labels and the card, and closes its
   panel */
static inline void STCRDSHP_closeBuy(CardShopBuy *buy, CardShopBuyWindows *win) {
    buy->cursorShown = 0;
    win->prev->setVisible(win->prev, 0);
    win->next->setVisible(win->next, 0);
    STCRDSHP_showBuyCard(buy, win, 0);
    STCRDSHP_funcs.startFade(&buy->fades[0], 0);
    buy->substate++;
}

/* The states of the screen to buy cards: picking a card (left/right, L1/R1
   for the pages), the yes/no to buy it and the messages when the player
   can't */
void STCRDSHP_runBuy(CardShopBuy *buy, CardShopBuyWindows *win) {
    if (win->grid->state != TASK_RUN) {
        return;
    }
    switch (buy->substate) {
    case 0:
    default:
        STCRDSHP_funcs.startFade(&buy->fades[0], 1);
        buy->substate++;
        break;
    case 1:
        STCRDSHP_openBuyCard(buy, win);
        break;
    case 2:
        STCRDSHP_chooseBuyCard(buy, win);
        break;
    case 5:
        STCRDSHP_funcs.startFade(&buy->fades[1], 1);
        STCRDSHP_funcs.startFade(&buy->fades[2], 1);
        buy->substate++;
        break;
    case 6:
        STCRDSHP_openBuyTotal(buy, win);
        break;
    case 7:
        STCRDSHP_confirmBuyCard(buy, win);
        break;
    case 8:
        STCRDSHP_closeBuyTotal(buy, win);
        break;
    case 10:
        STCRDSHP_funcs.startFade(&buy->fades[1], 1);
        buy->fades[2].level = 0;
        buy->substate++;
        break;
    case 11:
        if (STCRDSHP_funcs.updateFade(&buy->fades[1])) {
            win->message->setString(win->message, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), buy->step);
            buy->substate++;
        }
        break;
    case 12:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            win->message->setVisible(win->message, 0);
            STCRDSHP_funcs.startFade(&buy->fades[1], 0);
            buy->substate++;
        }
        break;
    case 9:
    case 13:
        if (STCRDSHP_funcs.updateFade(&buy->fades[1])) {
            buy->substate = 1;
        }
        break;
    case 50:
        STCRDSHP_closeBuy(buy, win);
        break;
    case 51:
        if (STCRDSHP_funcs.updateFade(&buy->fades[0])) {
            win->grid->hide(win->grid);
            buy->state = TASK_DONE;
        }
        break;
    }
}

/* The buy screen's update: takes the shop's stock, six cards a page. The
   match depends on pages holding the remainder first and the partial page
   being a variable of its own. */
void STCRDSHP_updateBuy(CardShopBuy *buy, CardShopBuyWindows *win) {
    s32 i;
    s32 extra;
    s32 pages;

    switch (buy->state) {
    case TASK_INIT:
    default:
        buy->nextState(buy);
        buy->stock = STCRDSHP_funcs.getStock(buy->shopId);
        for (i = 0; i < 6; i++) {
            buy->cards[i] = buy->stock->cards[i];
        }
        buy->count = buy->stock->count;
        pages = buy->count % 6;
        extra = pages != 0;
        pages = buy->count / 6 + extra;
        buy->pages = pages;
        win->grid = STCRDSHP_createGrid(buy->shop, buy->cards);
        STCRDSHP_createBuyWindows(buy, win);
        buy->fades[0].duration = 10;
        buy->fades[1].duration = 10;
        buy->fades[2].duration = 10;
        break;
    case TASK_RUN:
        STCRDSHP_runBuy(buy, win);
        STCRDSHP_drawBuy(buy);
        break;
    case TASK_DONE:
        if (win->grid == NULL) {
            buy->state = TASK_KILL;
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the screen to buy the cards of a shop */
CardShopBuy *STCRDSHP_createBuy(CardShop *shop, s32 shopId) {
    CardShopBuy *buy = createTask(STCRDSHP_updateBuy, sizeof(CardShopBuy), sizeof(CardShopBuyWindows));

    buy->layer = SCREEN_LAYER;
    buy->depth = 6;
    buy->shop = shop;
    buy->shopId = shopId;
    return buy;
}

/* The CLUT rows of the cursor's frames */
s32 STCRDSHP_cursorRows[] = {
    0, 1, 2, 3,
    2, 1,
};
