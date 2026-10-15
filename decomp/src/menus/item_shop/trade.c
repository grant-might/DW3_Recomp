/* The buying and selling dialogs: how many of the item, at what total, and
   the purchase or the sale */

#include "menus/item_shop.h"

/* Creates the buying dialog's windows: the quantity, the total with its yes and
   no, and the cursor */
void STITSHOP_createBuyWindows(ShopBuy *buy, ShopBuyWindows *win) {
    win->quantityLabel = createTextWindow(buy->layer, 1, 0xB9, 0x3A);
    win->times = createTextWindow(buy->layer, 1, 0x103, 0x54);
    win->quantity = createTextWindow(buy->layer, 1, 0x11E, 0x54);
    win->total = createTextWindow(buy->layer, 1, 0x9A, 0x2C);
    win->yes = createTextWindow(buy->layer, 1, 0xC5, 0x49);
    win->no = createTextWindow(buy->layer, 1, 0xC5, 0x59);
    win->cursor = createCursor(buy->layer, buy->depth - 1, 0xB8, 0x49);
    win->cursor->setVisible(win->cursor, 0);
}

/* Shows the quantity to buy; or hides it */
void STITSHOP_showQuantity(ShopBuy *buy, ShopBuyWindows *win, s32 show) {
    if (show) {
        win->quantityLabel->setString(win->quantityLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x11);
        win->times->setString(win->times, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 8);
        win->quantity->setNumber(win->quantity, 0, buy->quantity);
        win->quantity->setRightAlign(win->quantity, 1);
    } else {
        win->quantityLabel->setVisible(win->quantityLabel, 0);
        win->times->setVisible(win->times, 0);
        win->quantity->setVisible(win->quantity, 0);
    }
}

/* Shows the total price with yes and no to confirm the purchase; or hides them */
void STITSHOP_showBuyTotal(ShopBuy *buy, ShopBuyWindows *win, s32 show) {
    if (show) {
        win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x12);
        win->total->setNumber(win->total, 1, ITEM_FUNCS.get(buy->item)->price * buy->quantity);
        win->yes->setString(win->yes, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x13);
        win->no->setString(win->no, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x14);
    } else {
        win->total->setVisible(win->total, 0);
        win->yes->setVisible(win->yes, 0);
        win->no->setVisible(win->no, 0);
    }
}

/* Draws the buying screen's frames, the quantity's blinking arrows and the
   marker over a partner */
void STITSHOP_drawBuy(ShopBuy *buy, ShopBuyWindows *win) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(buy->layer, buy->depth);
    sprite.setTexture(0x280, 0x100);
    if (buy->panels[0].level != 0) {
        sprite.setScale(buy->panels[0].level, ONE, ONE);
        if (buy->panels[0].level != ONE) {
            sprite.setPivot(0x140, 0x42);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xA, 0x9A, 0x34);
        if (buy->panels[0].level != ONE) {
            sprite.setPivot(0x140, 0x59);
        } else {
            if (GFX.funcs.getTime() - buy->blinkTime >= 9) {
                buy->blinkTime = GFX.funcs.getTime();
                buy->blink = 1 - buy->blink;
            }
            if (buy->blink) {
                if (buy->quantity < buy->max) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x12, 0x122, 0x51);
                }
                if (buy->quantity >= 2) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x13, 0x122, 0x5B);
                }
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xB, 0xF6, 0x4F);
    }
    if (buy->panels[1].level != 0) {
        if (buy->panels[1].level != ONE) {
            sprite.setScale(buy->panels[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x32);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x15, 0x7B, 0x26);
        if (buy->panels[1].level != ONE) {
            sprite.setPivot(0x140, 0x57);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xC, 0xAF, 0x43);
    }
    if (buy->panels[2].level != 0) {
        sprite.setLayerId(buy->layer, buy->depth - 2);
        if (buy->panels[2].level != ONE) {
            sprite.setScale(buy->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x32);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x15, 0x7B, 0x26);
    }
    if (buy->panels[3].level != 0) {
        sprite.setLayerId(buy->layer, buy->depth - 2);
        if (buy->panels[3].level != ONE) {
            sprite.setScale(buy->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x32);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x15, 0x7B, 0x26);
    }
    if (buy->markerShown) {
        if (GFX.funcs.getTime() - buy->markerTime >= 9) {
            buy->markerTime = GFX.funcs.getTime();
            if (++buy->markerFrame >= 8) {
                buy->markerFrame = 0;
            }
        }
        sprite.setLayerId(buy->layer, buy->depth - 2);
        sprite.setScale(ONE, ONE, ONE);
        sprite.setClutRow(buy->markerFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xF, buy->partner * 0x63 + 0x10, 0x9C);
    }
}

/* The quantity to buy: up/down change it by one, left/right by ten, up to
   what the money and the bag's 99 allow; cross goes on to the total,
   triangle back to the list and circle turns the details' page */
static inline void STITSHOP_chooseBuyQuantity(ShopBuy *buy, ShopBuyWindows *win) {
    s32 old;
    u16 price;

    old = buy->quantity;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        buy->quantity++;
        if (buy->quantity > buy->max) {
            buy->quantity = buy->max;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (--buy->quantity <= 0) {
            buy->quantity = 1;
        }
    } else if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        buy->quantity -= 10;
        if (buy->quantity < 10) {
            buy->quantity = 1;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        buy->quantity += 10;
        if (buy->quantity > buy->max) {
            buy->quantity = buy->max;
        }
    }
    if (old != buy->quantity) {
        price = ITEM_FUNCS.get(buy->item)->price;
        if (GAME.money < price * buy->quantity) {
            buy->quantity = GAME.money / price;
        }
        STITSHOP_showQuantity(buy, win, 1);
        win->info->showItem(win->info, buy->item, buy->quantity);
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        buy->nextSubstate(buy);
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->nextSubstate(buy);
        buy->step = 1;
        buy->quantity = 1;
        win->info->showItem(win->info, buy->item, 1);
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        if (win->info->substate == 3) {
            win->info->turnPage(win->info);
            buy->substate = 12;
        }
    }
}

/* The item list: cross picks the item under the cursor, unless the bag holds
   99 of it or the money is short (a message then), triangle closes the dialog
   and circle turns the details' page */
static inline void STITSHOP_pickBuyItem(ShopBuy *buy, ShopBuyWindows *win) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        buy->item = win->list->getSelected(win->list);
        if (GAME.items[buy->item] == 99) {
            win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x1C);
            win->total->setVisible(win->total, 0);
            buy->substate = 45;
        } else if (GAME.money < ITEM_FUNCS.get(buy->item)->price) {
            win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x1B);
            win->total->setVisible(win->total, 0);
            buy->substate = 45;
        } else {
            win->list->showCursor(win->list, 0);
            buy->substate = 5;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        win->list->close(win->list);
        win->info->close(win->info);
        buy->substate = 50;
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        if (win->info->substate == 3) {
            win->info->turnPage(win->info);
            win->list->freezeCursor(win->list, 1);
            buy->substate = 4;
        }
    }
}

/* Yes or no to the total: yes buys the items (at most 99 in the bag) and pays
   for them, and for equipment asks next whether to equip it; triangle cancels
   and circle turns the details' page */
static inline void STITSHOP_confirmBuy(ShopBuy *buy, ShopBuyWindows *win) {
    s32 old;

    old = buy->choice;
    if (PAD_PRESSED(PAD_UP)) {
        buy->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        buy->choice = 1;
    }
    if (old != buy->choice) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB8, buy->choice * 0x10 + 0x49);
    } else if (win->info->shown && PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        buy->setSubstate(buy, 20);
        if (buy->choice == 0) {
            if (ITEM_FUNCS.isKind(buy->item, ITEM_KIND_WEAPON) || ITEM_FUNCS.isKind(buy->item, ITEM_KIND_ARMOR) ||
                ITEM_FUNCS.isKind(buy->item, ITEM_KIND_ACCESSORY)) {
                buy->step = 1;
            }
            if (GAME.items[buy->item] + buy->quantity >= 100) {
                GAME.items[buy->item] = 99;
            } else {
                GAME.items[buy->item] += buy->quantity;
            }
            win->info->showItem(win->info, buy->item, buy->quantity);
            GAME.money -= ITEM_FUNCS.get(buy->item)->price * buy->quantity;
            buy->shop->showMoney(buy->shop);
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->setSubstate(buy, 20);
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        if (win->info->substate == 3) {
            win->info->turnPage(win->info);
            buy->substate = 18;
        }
    }
}

/* Once the total's panel has closed: asks whether to equip what was bought
   when a partner of the party can, else goes back to the list */
static inline void STITSHOP_endBuyTotal(ShopBuy *buy, ShopBuyWindows *win) {
    s32 i;
    s32 n;

    if (STITSHOP_funcs.updateFade(&buy->panels[1]) != 0) {
        if (buy->step != 0) {
            for (i = 0, n = 0; i < 3; i++) {
                if (STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(i), buy->item)) {
                    n++;
                }
            }
            if (n != 0) {
                buy->setSubstate(buy, 25);
            } else {
                buy->substate = 10;
                buy->step = 1;
            }
        } else {
            buy->substate = 10;
            buy->step = 1;
        }
    }
}

/* Yes or no to equipping what was bought: cross answers, triangle declines */
static inline void STITSHOP_confirmEquip(ShopBuy *buy, ShopBuyWindows *win) {
    s32 old;

    old = buy->choice;
    if (PAD_PRESSED(PAD_UP)) {
        buy->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        buy->choice = 1;
    }
    if (old != buy->choice) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB8, buy->choice * 0x10 + 0x49);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        buy->nextSubstate(buy);
        if (buy->choice == 0) {
            buy->step = 1;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->nextSubstate(buy);
    }
}

/* The marker over the partner to equip: left/right move it to the next partner
   who can equip the item, cross equips it on them and triangle gives up */
static inline void STITSHOP_chooseEquipPartner(ShopBuy *buy, ShopBuyWindows *win) {
    s32 old;
    s32 partner;

    old = buy->partner;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        do {
            if (--buy->partner < 0) {
                buy->partner = 0;
                break;
            }
        } while (!STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(buy->partner), buy->item));
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        do {
            if (++buy->partner > win->info->partyCount - 1) {
                buy->partner = win->info->partyCount - 1;
                break;
            }
        } while (!STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(buy->partner), buy->item));
    }
    if (old != buy->partner) {
        if (STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(buy->partner), buy->item)) {
            SOUND.playSound(SOUND_MENU_MOVE);
        } else {
            buy->partner = old;
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        partner = GAME.funcs.getPartyMember(buy->partner);
        STITSHOP_funcs.equip(partner, STITSHOP_funcs.compareEquip(partner, buy->item), buy->item, 1);
        win->info->refreshPartner(win->info, buy->partner);
        buy->substate = 36;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        buy->substate++;
    }
}

/* Closes the list and works out the most of the item that can be bought: what
   the money pays for (an item priced 0 counts as 1), up to 99 in the bag */
static inline void STITSHOP_startBuyQuantity(ShopBuy *buy, ShopBuyWindows *win) {
    u16 price;

    win->list->close(win->list);
    price = ITEM_FUNCS.get(buy->item)->price;
    if (price == 0) {
        price = 1;
    }
    buy->max = GAME.money / price;
    if (buy->max + GAME.items[buy->item] >= 100) {
        buy->max = 99 - GAME.items[buy->item];
    }
    buy->substate++;
}

/* Once the total's panel has opened: shows the total with yes and no, the
   cursor on yes */
static inline void STITSHOP_openBuyTotal(ShopBuy *buy, ShopBuyWindows *win) {
    if (STITSHOP_funcs.updateFade(&buy->panels[1]) != 0) {
        STITSHOP_showBuyTotal(buy, win, 1);
        buy->choice = 0;
        win->cursor->setPos(win->cursor, 0xB8, 0x49);
        win->cursor->setVisible(win->cursor, 1);
        buy->substate++;
    }
}

/* Once the question's panels have opened: asks whether to equip what was
   bought, the cursor on yes */
static inline void STITSHOP_openEquipQuestion(ShopBuy *buy, ShopBuyWindows *win) {
    STITSHOP_funcs.updateFade(&buy->panels[1]);
    if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
        win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x15);
        win->yes->setString(win->yes, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x16);
        win->no->setString(win->no, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x17);
        buy->choice = 0;
        win->cursor->setPos(win->cursor, 0xB8, 0x49);
        win->cursor->setVisible(win->cursor, 1);
        buy->substate++;
    }
}

/* Hides the question about equipping and closes its panel, and the message's
   too when the answer was no */
static inline void STITSHOP_closeEquipQuestion(ShopBuy *buy, ShopBuyWindows *win) {
    win->total->setVisible(win->total, 0);
    win->yes->setVisible(win->yes, 0);
    win->no->setVisible(win->no, 0);
    win->cursor->setVisible(win->cursor, 0);
    STITSHOP_funcs.startFade(&buy->panels[1], 0);
    if (buy->step == 0) {
        STITSHOP_funcs.startFade(&buy->panels[2], 0);
    }
    buy->substate++;
}

/* Once the question's panel has closed: on yes goes on to pick the partner, on
   no back to the list */
static inline void STITSHOP_endEquipQuestion(ShopBuy *buy, ShopBuyWindows *win) {
    if (buy->step == 0) {
        STITSHOP_funcs.updateFade(&buy->panels[2]);
    }
    if (STITSHOP_funcs.updateFade(&buy->panels[1]) != 0) {
        if (buy->step != 0) {
            buy->setSubstate(buy, 30);
        } else {
            buy->substate = 10;
            buy->step = 1;
            win->info->nextSubstate(win->info);
        }
    }
}

/* Once the message's panel has opened: shows the marker over the first partner
   who can equip the item (or the last one picked) and asks who equips it */
static inline void STITSHOP_showEquipMarker(ShopBuy *buy, ShopBuyWindows *win) {
    if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
        if (buy->step == 0) {
            for (buy->partner = 0; buy->partner < 3; buy->partner++) {
                if (STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(buy->partner), buy->item)) {
                    break;
                }
            }
        }
        buy->markerShown = 1;
        win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x10);
        buy->substate++;
    }
}

/* Once the message that the item was equipped has closed: back to the list
   when none is left in the bag, else to picking a partner again */
static inline void STITSHOP_endEquipped(ShopBuy *buy, ShopBuyWindows *win) {
    if (STITSHOP_funcs.updateFade(&buy->panels[3]) != 0) {
        if (GAME.items[buy->item] <= 0) {
            buy->substate = 35;
        } else {
            buy->substate = 30;
            buy->step = 1;
        }
    }
}

/* The buying dialog: pick the item in the list, how many, confirm, then
   (equipment) whether to equip it and on which partner */
void STITSHOP_runBuy(ShopBuy *buy, ShopBuyWindows *win) {
    switch (buy->substate) {
    case 0:
    default:
        if (win->list == NULL) {
            /* ShopBuy starts as a ShopDialog */
            win->list = STITSHOP_createItemList((ShopDialog *)buy, buy->shop->shop, 0);
        }
        buy->substate++;
        break;
    case 1:
        if (win->list->state == TASK_DONE) {
            if (win->info == NULL) {
                win->info = STITSHOP_createInfo(0, win->list->getSelected(win->list));
            }
            buy->substate++;
        }
        break;
    case 2:
        if (win->info->substate == 3) {
            win->list->showCursor(win->list, 1);
            buy->substate++;
        }
        break;
    case 3:
        STITSHOP_pickBuyItem(buy, win);
        break;
    case 4:
        if (win->info->substate == 3) {
            win->list->freezeCursor(win->list, 0);
            buy->substate = 3;
        }
        break;
    case 5:
        STITSHOP_startBuyQuantity(buy, win);
        break;
    case 6:
        if (win->list->state == TASK_DONE) {
            STITSHOP_funcs.startFade(&buy->panels[0], 1);
            buy->substate++;
        }
        break;
    case 7:
        if (STITSHOP_funcs.updateFade(&buy->panels[0]) != 0) {
            STITSHOP_showQuantity(buy, win, 1);
            buy->substate++;
        }
        break;
    case 8:
        STITSHOP_chooseBuyQuantity(buy, win);
        break;
    case 9:
        STITSHOP_showQuantity(buy, win, 0);
        STITSHOP_funcs.startFade(&buy->panels[0], 0);
        buy->substate++;
        break;
    case 10:
        if (STITSHOP_funcs.updateFade(&buy->panels[0]) != 0) {
            if (buy->step != 0) {
                win->list->start(win->list);
                buy->nextSubstate(buy);
            } else {
                buy->substate = 15;
            }
        }
        break;
    case 11:
        if (win->list->state == TASK_DONE) {
            win->list->showCursor(win->list, 1);
            buy->substate = 2;
        }
        break;
    case 12:
        if (win->info->substate == 3) {
            buy->substate = 8;
        }
        break;
    case 15:
        STITSHOP_funcs.startFade(&buy->panels[1], 1);
        buy->substate++;
        break;
    case 16:
        STITSHOP_openBuyTotal(buy, win);
        break;
    case 17:
        STITSHOP_confirmBuy(buy, win);
        break;
    case 18:
        if (win->info->substate == 3) {
            buy->substate = 17;
        }
        break;
    case 20:
        buy->quantity = 1;
        win->info->showItem(win->info, buy->item, 1);
        STITSHOP_showBuyTotal(buy, win, 0);
        win->cursor->setVisible(win->cursor, 0);
        STITSHOP_funcs.startFade(&buy->panels[1], 0);
        buy->substate++;
        break;
    case 21:
        STITSHOP_endBuyTotal(buy, win);
        break;
    case 25:
        if (win->info->shown) {
            win->info->nextSubstate(win->info);
            STITSHOP_funcs.startFade(&buy->panels[1], 1);
            STITSHOP_funcs.startFade(&buy->panels[2], 1);
            buy->substate++;
        }
        break;
    case 26:
        STITSHOP_openEquipQuestion(buy, win);
        break;
    case 27:
        STITSHOP_confirmEquip(buy, win);
        break;
    case 28:
        STITSHOP_closeEquipQuestion(buy, win);
        break;
    case 29:
        STITSHOP_endEquipQuestion(buy, win);
        break;
    case 30:
        STITSHOP_funcs.startFade(&buy->panels[2], 1);
        buy->substate++;
        break;
    case 31:
        STITSHOP_showEquipMarker(buy, win);
        break;
    case 32:
        STITSHOP_chooseEquipPartner(buy, win);
        break;
    case 33:
        STITSHOP_funcs.startFade(&buy->panels[2], 0);
        win->total->setVisible(win->total, 0);
        buy->substate++;
        break;
    case 34:
        if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
            buy->substate++;
        }
        break;
    case 35:
        buy->substate = 10;
        buy->step = 1;
        win->info->nextSubstate(win->info);
        buy->markerShown = 0;
        break;
    case 36:
        buy->markerShown = 0;
        STITSHOP_funcs.startFade(&buy->panels[2], 0);
        win->total->setVisible(win->total, 0);
        buy->substate++;
        break;
    case 37:
        if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
            STITSHOP_funcs.startFade(&buy->panels[3], 1);
            buy->substate++;
        }
        break;
    case 38:
        if (STITSHOP_funcs.updateFade(&buy->panels[3]) != 0) {
            win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 9);
            buy->substate++;
        }
        break;
    case 39:
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            STITSHOP_funcs.startFade(&buy->panels[3], 0);
            win->total->setVisible(win->total, 0);
            buy->substate++;
        }
        break;
    case 40:
        STITSHOP_endEquipped(buy, win);
        break;
    case 45:
        STITSHOP_funcs.startFade(&buy->panels[2], 1);
        win->list->freezeCursor(win->list, 1);
        buy->substate++;
        break;
    case 46:
        if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
            win->total->setVisible(win->total, 1);
            buy->substate++;
        }
        break;
    case 47:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            win->total->setVisible(win->total, 0);
            STITSHOP_funcs.startFade(&buy->panels[2], 0);
            buy->substate++;
        }
        break;
    case 48:
        if (STITSHOP_funcs.updateFade(&buy->panels[2]) != 0) {
            win->list->freezeCursor(win->list, 0);
            buy->substate = 3;
        }
        break;
    case 50:
        if (win->info == NULL) {
            buy->state = TASK_KILL;
        }
        break;
    }
}

/* The buying dialog's task: creates its windows, starting at a quantity of 1,
   then runs and draws it */
void STITSHOP_updateBuy(ShopBuy *buy, ShopBuyWindows *win) {
    switch (buy->state) {
    case TASK_INIT:
    default:
        buy->nextState(buy);
        STITSHOP_createBuyWindows(buy, win);
        buy->panels[0].duration = 10;
        buy->panels[1].duration = 10;
        buy->panels[2].duration = 10;
        buy->panels[3].duration = 10;
        buy->quantity = 1;
        break;
    case TASK_RUN:
        STITSHOP_runBuy(buy, win);
        STITSHOP_drawBuy(buy, win);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Shows the item selected in the list, and the quantity, in the details panel
   (buy->showItem) */
void STITSHOP_showBuyItem(ShopBuy *buy, s32 item, s32 quantity) {
    ShopBuyWindows *win = buy->children;

    win->info->showItem(win->info, item, quantity);
}

/* Creates the buying dialog (task), when buy is chosen in the shop */
ShopBuy *STITSHOP_createBuy(ItemShop *shop) {
    ShopBuy *buy = createTask(STITSHOP_updateBuy, sizeof(ShopBuy), sizeof(ShopBuyWindows));

    buy->showItem = STITSHOP_showBuyItem;
    buy->layer = SCREEN_LAYER;
    buy->depth = 4;
    buy->shop = shop;
    return buy;
}

/* Creates the selling dialog's windows: the cursor, the four kinds of sale, the
   quantity, the total with its yes and no, and the message */
void STITSHOP_createSellWindows(ShopSell *sell, ShopSellWindows *win) {
    s32 i;

    win->cursor = createCursor(sell->layer, sell->depth - 1, 0, 0xB0);
    win->cursor->setVisible(win->cursor, 0);
    for (i = 0; i < 4; i++) {
        win->types[i] = createTextWindow(sell->layer, 1, 0xBE, i * 0xE + 0x2F);
    }
    win->quantityLabel = createTextWindow(sell->layer, 1, 0xB9, 0x3A);
    win->times = createTextWindow(sell->layer, 1, 0x103, 0x54);
    win->quantity = createTextWindow(sell->layer, 1, 0x11E, 0x54);
    win->total = createTextWindow(sell->layer, 1, 0x9A, 0x2C);
    win->yes = createTextWindow(sell->layer, 1, 0xC5, 0x49);
    win->no = createTextWindow(sell->layer, 1, 0xC5, 0x59);
    win->message = createTextWindow(sell->layer, 1, 0x9A, 0x8A);
}

/* Draws the selling screen's frames, and the quantity's blinking arrows */
void STITSHOP_drawSell(ShopSell *sell, ShopSellWindows *win) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    if (sell->panels[3].level != 0) {
        sprite.setLayerId(sell->layer, 1);
        if (sell->panels[3].level != ONE) {
            sprite.setScale(sell->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x41);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xD, 0xA0, 0x24);
    }
    sprite.setLayerId(sell->layer, sell->depth);
    if (sell->panels[0].level != 0) {
        if (sell->panels[0].level != ONE) {
            sprite.setScale(sell->panels[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x4F);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 4, 0xA9, 0x26);
    }
    if (sell->panels[1].level != 0) {
        if (sell->panels[1].level != ONE) {
            sprite.setScale(sell->panels[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x42);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xA, 0x9A, 0x34);
        if (sell->panels[1].level != ONE) {
            sprite.setPivot(0x140, 0x59);
        } else {
            if (GFX.funcs.getTime() - sell->blinkTime >= 9) {
                sell->blinkTime = GFX.funcs.getTime();
                sell->blink = 1 - sell->blink;
            }
            if (sell->blink) {
                if (sell->quantity < sell->max) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x12, 0x122, 0x51);
                }
                if (sell->quantity >= 2) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x13, 0x122, 0x5B);
                }
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xB, 0xF6, 0x4F);
    }
    if (sell->panels[2].level != 0) {
        if (sell->panels[2].level != ONE) {
            sprite.setScale(sell->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x32);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x15, 0x7B, 0x26);
        if (sell->panels[2].level != ONE) {
            sprite.setPivot(0x140, 0x57);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xC, 0xAF, 0x43);
    }
    if (sell->panels[4].level != 0) {
        if (sell->panels[4].level != ONE) {
            sprite.setScale(sell->panels[4].level, ONE, ONE);
            sprite.setPivot(0x140, 0x8F);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x15, 0x7B, 0x84);
    }
}

/* Once the panel has opened: shows the four kinds of items to sell (items,
   weapons, armor, accessories) and the cursor on the last one chosen */
static inline void STITSHOP_openSellTypes(ShopSell *sell, ShopSellWindows *win) {
    s32 i;

    if (STITSHOP_funcs.updateFade(&sell->panels[0]) != 0) {
        for (i = 0; i < 4; i++) {
            win->types[i]->setString(win->types[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), i + 0x1D);
        }
        win->cursor->setPos(win->cursor, 0xB0, sell->type * 0xE + 0x2F);
        win->cursor->setVisible(win->cursor, 1);
        sell->substate++;
    }
}

/* Up/down choose the kind of items to sell; cross lists the bag's items of
   that kind, triangle closes the dialog */
static inline void STITSHOP_chooseSellType(ShopSell *sell, ShopSellWindows *win) {
    s32 old;

    old = sell->type;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--sell->type < 0) {
            sell->type = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++sell->type >= 4) {
            sell->type = 3;
        }
    }
    if (old != sell->type) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB0, sell->type * 0xE + 0x2F);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        win->cursor->setVisible(win->cursor, 0);
        if (win->list != NULL) {
            win->list->state = TASK_KILL;
        }
        sell->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        sell->setSubstate(sell, 50);
    }
}

/* Once the quantity's panel has opened: shows how many to sell, up to all of
   the item the bag holds */
static inline void STITSHOP_openSellQuantity(ShopSell *sell, ShopSellWindows *win) {
    if (STITSHOP_funcs.updateFade(&sell->panels[1]) != 0) {
        win->quantityLabel->setString(win->quantityLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x21);
        win->times->setString(win->times, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 8);
        win->quantity->setNumber(win->quantity, 0, sell->quantity);
        win->quantity->setRightAlign(win->quantity, 1);
        sell->item = win->list->getSelected(win->list);
        sell->max = GAME.items[sell->item];
        sell->substate++;
    }
}

/* The quantity to sell: up/down change it by one, left/right by ten, up to
   what the bag holds; cross goes on to the total, triangle back to the list */
static inline void STITSHOP_chooseSellQuantity(ShopSell *sell, ShopSellWindows *win) {
    s32 old;

    old = sell->quantity;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        sell->quantity++;
        if (sell->quantity > sell->max) {
            sell->quantity = sell->max;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (--sell->quantity <= 0) {
            sell->quantity = 1;
        }
    } else if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        sell->quantity -= 10;
        if (sell->quantity < 10) {
            sell->quantity = 1;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        sell->quantity += 10;
        if (sell->quantity > sell->max) {
            sell->quantity = sell->max;
        }
    }
    if (old != sell->quantity) {
        ITEM_FUNCS.get(sell->item); /* its result is unused */
        win->quantity->setNumber(win->quantity, 0, sell->quantity);
        win->quantity->setRightAlign(win->quantity, 1);
        win->info->showItem(win->info, sell->item, sell->quantity);
        SOUND.playSound(SOUND_MENU_MOVE);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        sell->step = 0;
        sell->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        sell->quantity = 1;
        win->info->showItem(win->info, sell->item, 1);
        sell->step = 1;
        sell->substate++;
    }
}

/* Hides the quantity and closes its panel, then goes on to the total, or back
   to the list after triangle */
static inline void STITSHOP_closeSellQuantity(ShopSell *sell, ShopSellWindows *win) {
    STITSHOP_funcs.startFade(&sell->panels[1], 0);
    win->quantityLabel->setVisible(win->quantityLabel, 0);
    win->times->setVisible(win->times, 0);
    win->quantity->setVisible(win->quantity, 0);
    if (sell->step != 0) {
        win->list->start(win->list);
        sell->substate++;
    } else {
        sell->setSubstate(sell, 30);
    }
}

/* Once the total's panel has opened: shows what the items sell for with sell
   and no, the cursor on sell */
static inline void STITSHOP_openSellTotal(ShopSell *sell, ShopSellWindows *win) {
    if (STITSHOP_funcs.updateFade(&sell->panels[2]) != 0) {
        win->total->setString(win->total, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x12);
        win->total->setNumber(win->total, 1, ITEM_FUNCS.get(sell->item)->sellPrice * sell->quantity);
        win->yes->setString(win->yes, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x22);
        win->no->setString(win->no, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x14);
        sell->choice = 0;
        win->cursor->setPos(win->cursor, 0xB8, 0x49);
        win->cursor->setVisible(win->cursor, 1);
        sell->substate++;
    }
}

/* Sell or no to the total: sell takes the items from the bag and adds their
   price to the money (up to 9999999); the list is refilled either way */
static inline void STITSHOP_confirmSell(ShopSell *sell, ShopSellWindows *win) {
    s32 old;

    old = sell->choice;
    if (PAD_PRESSED(PAD_UP)) {
        sell->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        sell->choice = 1;
    }
    if (old != sell->choice) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB8, sell->choice * 0x10 + 0x49);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (sell->choice == 0) {
            GAME.money += ITEM_FUNCS.get(sell->item)->sellPrice * sell->quantity;
            if (GAME.money > 9999999) {
                GAME.money = 9999999;
            }
            GAME.items[sell->item] -= sell->quantity;
            if (GAME.items[sell->item] < 0) {
                GAME.items[sell->item] = 0;
            }
            sell->shop->showMoney(sell->shop);
        }
        win->list->listBag(win->list);
        sell->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        sell->substate++;
    }
}

/* Hides the total and closes its panel; back to the kinds of items when none
   of the kind is left, else back to the list */
static inline void STITSHOP_closeSellTotal(ShopSell *sell, ShopSellWindows *win) {
    STITSHOP_funcs.startFade(&sell->panels[2], 0);
    win->total->setVisible(win->total, 0);
    win->yes->setVisible(win->yes, 0);
    win->no->setVisible(win->no, 0);
    win->cursor->setVisible(win->cursor, 0);
    if (win->list->count <= 0 && GAME.items[sell->item] == 0) {
        sell->quantity = 1;
        win->list->state = TASK_KILL;
        win->info->close(win->info);
        sell->substate = 39;
    } else {
        win->list->start(win->list);
        sell->substate++;
    }
}

/* Brings the list back while the total's panel closes: its cursor and page
   move up when the last row sold out, and the details show the item now under
   it; then back to the list */
static inline void STITSHOP_reopenSellList(ShopSell *sell, ShopSellWindows *win) {
    if (win->list == NULL) {
        /* ShopSell starts as a ShopDialog */
        win->list = STITSHOP_createItemList((ShopDialog *)sell, STITSHOP_sellLists[sell->type], 1);
    } else if (win->list->substate == 100) {
        win->list->substate = 1;
        if (win->list->selection > win->list->count - 1) {
            win->list->selection--;
        }
        if (win->list->page > win->list->pages - 1) {
            win->list->page--;
        }
        sell->item = win->list->getSelected(win->list);
        sell->quantity = 1;
        win->info->showItem(win->info, sell->item, 1);
    }
    if (STITSHOP_funcs.updateFade(&sell->panels[2]) != 0) {
        sell->substate = 26;
    }
}

/* Hides the kinds of items and the cursor and closes their panel */
static inline void STITSHOP_closeSellTypes(ShopSell *sell, ShopSellWindows *win) {
    s32 i;

    win->cursor->setVisible(win->cursor, 0);
    for (i = 0; i < 4; i++) {
        win->types[i]->setVisible(win->types[i], 0);
    }
    STITSHOP_funcs.startFade(&sell->panels[0], 0);
    sell->substate++;
}

/* The selling dialog: pick the kind of item, then the item in the list, how
   many, and confirm */
void STITSHOP_runSell(ShopSell *sell, ShopSellWindows *win) {
    switch (sell->substate) {
    case 0:
    default:
        STITSHOP_funcs.startFade(&sell->panels[0], 1);
        sell->substate++;
        break;
    case 1:
        STITSHOP_openSellTypes(sell, win);
        break;
    case 2:
        STITSHOP_chooseSellType(sell, win);
        break;
    case 3:
        if (win->list == NULL) {
            /* ShopSell starts as a ShopDialog */
            win->list = STITSHOP_createItemList((ShopDialog *)sell, STITSHOP_sellLists[sell->type], 1);
            sell->substate++;
        } else {
            win->list->state = TASK_KILL;
        }
        break;
    case 4:
        if (win->list->substate == 100) {
            if (win->list->count > 0) {
                sell->setSubstate(sell, 50);
                sell->step = 1;
            } else {
                sell->substate = 200;
            }
        }
        break;
    case 100:
        win->list->substate = 1;
        sell->substate = 101;
        break;
    case 101:
        if (win->list->state == TASK_DONE) {
            if (win->info == NULL) {
                win->info = STITSHOP_createInfo(1, win->list->getSelected(win->list));
            }
            sell->substate = 5;
        }
        break;
    case 5:
        if (win->info->substate == 3) {
            win->list->showCursor(win->list, 1);
            sell->substate++;
        }
        break;
    case 6:
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            sell->substate = 20;
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            win->list->close(win->list);
            win->info->close(win->info);
            sell->substate = 40;
        }
        break;
    case 20:
        win->list->close(win->list);
        sell->substate++;
        break;
    case 21:
        if (win->list->state == TASK_DONE) {
            STITSHOP_funcs.startFade(&sell->panels[1], 1);
            sell->substate++;
        }
        break;
    case 22:
        STITSHOP_openSellQuantity(sell, win);
        break;
    case 23:
        STITSHOP_chooseSellQuantity(sell, win);
        break;
    case 24:
        STITSHOP_closeSellQuantity(sell, win);
        break;
    case 25:
        if (win->list->substate == 100) {
            win->list->substate = 1;
        }
        if (STITSHOP_funcs.updateFade(&sell->panels[1]) != 0) {
            sell->substate++;
        }
        break;
    case 26:
        if (win->list->state == TASK_DONE) {
            win->list->showCursor(win->list, 1);
            sell->substate = 6;
        }
        break;
    case 30:
        if (STITSHOP_funcs.updateFade(&sell->panels[1]) != 0) {
            STITSHOP_funcs.startFade(&sell->panels[2], 1);
            sell->substate++;
        }
        break;
    case 31:
        STITSHOP_openSellTotal(sell, win);
        break;
    case 32:
        STITSHOP_confirmSell(sell, win);
        break;
    case 33:
        STITSHOP_closeSellTotal(sell, win);
        break;
    case 34:
        STITSHOP_reopenSellList(sell, win);
        break;
    case 39:
        if (STITSHOP_funcs.updateFade(&sell->panels[2]) != 0) {
            sell->substate = 40;
        }
        break;
    case 40:
        if (win->info == NULL) {
            sell->substate++;
        }
        break;
    case 41:
        sell->substate = 0;
        break;
    case 200:
        STITSHOP_funcs.startFade(&sell->panels[4], 1);
        sell->substate++;
        break;
    case 201:
        if (STITSHOP_funcs.updateFade(&sell->panels[4]) != 0) {
            win->message->setString(win->message, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 1);
            win->message->setSubString(win->message, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), sell->type + 0x1D, 1);
            sell->substate++;
        }
        break;
    case 202:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            win->message->setVisible(win->message, 0);
            STITSHOP_funcs.startFade(&sell->panels[4], 0);
            sell->substate++;
        }
        break;
    case 203:
        if (STITSHOP_funcs.updateFade(&sell->panels[4]) != 0) {
            win->cursor->setVisible(win->cursor, 1);
            sell->substate = 2;
        }
        break;
    case 50:
        STITSHOP_closeSellTypes(sell, win);
        break;
    case 51:
        if (STITSHOP_funcs.updateFade(&sell->panels[0]) != 0) {
            if (sell->step != 0) {
                sell->setSubstate(sell, 100);
            } else {
                sell->state = TASK_KILL;
            }
        }
        break;
    }
}

/* The selling dialog's task: creates its windows, starting at a quantity of 1,
   then runs and draws it */
void STITSHOP_updateSell(ShopSell *sell, ShopSellWindows *win) {
    switch (sell->state) {
    case TASK_INIT:
    default:
        sell->nextState(sell);
        STITSHOP_createSellWindows(sell, win);
        sell->panels[0].duration = 10;
        sell->panels[1].duration = 10;
        sell->panels[2].duration = 10;
        sell->panels[3].duration = 10;
        sell->panels[4].duration = 10;
        sell->quantity = 1;
        break;
    case TASK_RUN:
        STITSHOP_runSell(sell, win);
        STITSHOP_drawSell(sell, win);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Shows the item selected in the list, and the quantity, in the details panel
   (sell->showItem) */
void STITSHOP_showSellItem(ShopSell *sell, s32 item, s32 quantity) {
    ShopSellWindows *win = sell->children;

    win->info->showItem(win->info, item, quantity);
}

/* Creates the selling dialog (task), when sell is chosen in the shop */
ShopSell *STITSHOP_createSell(ItemShop *shop) {
    ShopSell *sell = createTask(STITSHOP_updateSell, sizeof(ShopSell), sizeof(ShopSellWindows));

    sell->showItem = STITSHOP_showSellItem;
    sell->layer = SCREEN_LAYER;
    sell->depth = 4;
    sell->shop = shop;
    return sell;
}
