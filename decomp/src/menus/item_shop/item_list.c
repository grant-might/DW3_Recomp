/* The list of the items to buy or sell, by pages */

#include "menus/item_shop.h"

/* Creates the item list's windows: 14 items in two columns, the page number, the
   L1 and R1 labels (lower when selling) and the cursor */
void STITSHOP_createItemListWindows(ShopItemList *list, ShopItemListWindows *win) {
    s32 i;
    s32 y = list->selling * 0x28;

    for (i = 0; i < 14; i++) {
        win->items[i] = createTextWindow(list->layer, 1, (i % 2) * 0x83 + 0x37, (i / 2) * 0xE + 0x24);
        win->items[i]->setDepth(win->items[i], list->depth - 1);
    }
    win->page = createTextWindow(list->layer, 1, 0x9B, y + 0x60);
    win->slash = createTextWindow(list->layer, 1, 0x9C, y + 0x60);
    win->pages = createTextWindow(list->layer, 1, 0xB0, y + 0x60);
    win->l1Label = createTextWindow(list->layer, 1, 0x2D, y + 0x5C + list->selling * 2);
    win->r1Label = createTextWindow(list->layer, 1, 0x102, y + 0x5C + list->selling * 2);
    win->cursor = createCursor(list->layer, list->depth - 1, 0x1D, 0x24);
    win->cursor->setVisible(win->cursor, 0);
}

/* Shows the page of items, the page number and the arrows' labels; or hides
   them */
void STITSHOP_showItemPage(ShopItemList *list, ShopItemListWindows *win, s32 show) {
    s32 index;
    s32 item;
    s32 i;

    if (show) {
        for (i = 0; i < list->pageSize; i++) {
            index = list->page * list->pageSize + i;
            if (list->selling == 0) {
                item = list->shopItems[index];
            } else {
                item = list->items[index];
            }
            if (index < list->count && item != 0) {
                win->items[i]->setString(win->items[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
            } else {
                win->items[i]->setVisible(win->items[i], 0);
            }
        }
        win->page->setNumber(win->page, 0, list->page + 1);
        win->page->setRightAlign(win->page, 1);
        win->slash->setString(win->slash, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x18);
        win->pages->setNumber(win->pages, 0, list->pages);
        win->pages->setRightAlign(win->pages, 1);
        if (list->pages >= 2) {
            if (list->page != 0) {
                win->l1Label->setString(win->l1Label, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x19);
            } else {
                win->l1Label->setVisible(win->l1Label, 0);
            }
            if (list->page < list->pages - 1) {
                win->r1Label->setString(win->r1Label, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0x1A);
                return;
            }
            win->r1Label->setVisible(win->r1Label, 0);
        }
    } else {
        for (i = 0; i < list->pageSize; i++) {
            win->items[i]->setVisible(win->items[i], 0);
        }
        win->page->setVisible(win->page, 0);
        win->slash->setVisible(win->slash, 0);
        win->pages->setVisible(win->pages, 0);
        win->l1Label->setVisible(win->l1Label, 0);
        win->r1Label->setVisible(win->r1Label, 0);
    }
}

/* Draws the list's frame, the items' icons and the blinking page arrows */
void STITSHOP_drawItemList(ShopItemList *list) {
    SpriteDrawer sprite;
    s32 index;
    s32 item;
    s32 x;
    s32 y;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(list->layer, list->depth);
    sprite.setTexture(0x280, 0x100);
    if (list->panel.level != 0) {
        if (list->panel.level != ONE) {
            sprite.setScale(ONE, list->panel.level, ONE);
            sprite.setPivot(0xA0, 0x47);
        } else {
            for (i = 0; i < list->pageSize; i++) {
                index = list->page * list->pageSize + i;
                if (list->selling == 0) {
                    item = list->shopItems[index];
                } else {
                    item = list->items[index];
                }
                if (index >= list->count || item == 0) {
                    break;
                }
                x = (i % 2) * 0x83 + 0x28;
                y = (i % list->pageSize) / 2 * 0xE + 0x24;
                sprite.setTexture(0x140, 0);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(item), x, y);
                sprite.setTexture(0x280, 0x100);
                sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x31, x, y);
            }
            if (list->pages >= 2) {
                if (GFX.funcs.getTime() - list->blinkTime >= 11) {
                    list->blinkTime = GFX.funcs.getTime();
                    if (++list->clutRow >= 4) {
                        list->clutRow = 0;
                    }
                }
                sprite.setTexture(0x280, 0x100);
                sprite.setClutRow(list->clutRow);
                if (list->page != 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x10, 0x1E, list->selling * 0x2A + 0x5F);
                }
                if (list->page < list->pages - 1) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x11, 0xFD, list->selling * 0x2A + 0x5F);
                }
                sprite.setClutRow(0);
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), list->selling + 6, 0, 0x1D);
    }
}

/* The list's states: lists the shop's items or the bag's, fades in, then
   fades out when closed */
void STITSHOP_runItemList(ShopItemList *list, ShopItemListWindows *win) {
    switch (list->substate) {
    case 0:
    default:
        if (list->selling == 0) {
            list->shopItems = STITSHOP_funcs.getShopItems(list->type);
            list->count = STITSHOP_funcs.count;
            list->pageSize = 8;
            list->substate++;
        } else {
            STITSHOP_listSellable(list);
            list->pageSize = 14;
            list->substate = 100;
        }
        list->pages = list->count / list->pageSize + (list->count % list->pageSize != 0);
        break;
    case 1:
        STITSHOP_funcs.startFade(&list->panel, 1);
        list->substate++;
        break;
    case 2:
        if (STITSHOP_funcs.updateFade(&list->panel)) {
            STITSHOP_showItemPage(list, win, 1);
            list->setState(list, TASK_DONE);
        }
        break;
    case 50:
        STITSHOP_funcs.startFade(&list->panel, 0);
        STITSHOP_showItemPage(list, win, 0);
        list->substate++;
        break;
    case 51:
        if (STITSHOP_funcs.updateFade(&list->panel)) {
            list->setState(list, TASK_DONE);
        }
        break;
    case 100:
        break;
    }
}

/* The item list: L1 and R1 turn its pages, the d-pad moves in the page, and
   the dialog shows the selected item */
void STITSHOP_updateItemList(ShopItemList *list, ShopItemListWindows *win) {
    s32 page;
    s32 first;
    s32 last;
    s32 old;
    s32 item;

    switch (list->state) {
    case TASK_INIT:
    default:
        list->nextState(list);
        STITSHOP_createItemListWindows(list, win);
        list->panel.duration = 10;
        break;
    case TASK_RUN:
        STITSHOP_runItemList(list, win);
        STITSHOP_drawItemList(list);
        break;
    case TASK_DONE:
        if (list->active) {
            page = list->page;
            if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
                if (--list->page < 0) {
                    list->page = 0;
                }
            } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
                if (++list->page > list->pages - 1) {
                    list->page = list->pages - 1;
                }
            }
            if (page != list->page) {
                SOUND.playSound(SOUND_CURSOR);
                list->selection = list->page * list->pageSize;
                if (list->selling == 0) {
                    item = list->shopItems[list->selection];
                } else {
                    item = list->items[list->selection];
                }
                list->dialog->showItem(list->dialog, item, 1);
                STITSHOP_showItemPage(list, win, 1);
                win->cursor->setPos(win->cursor, (list->selection % 2) * 0x83 + 0x1D,
                                    (list->selection % list->pageSize) / 2 * 0xE + 0x24);
            } else {
                first = page * list->pageSize;
                old = list->selection;
                last = (page + 1) * list->pageSize - 1;
                if (last > list->count - 1) {
                    last = list->count - 1;
                }
                if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
                    list->selection -= 2;
                    if (list->selection < first) {
                        list->selection = first;
                    }
                } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
                    list->selection += 2;
                    if (list->selection > last) {
                        list->selection = last;
                    }
                }
                if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
                    if (!PAD_HELD(PAD_DOWN)) {
                        if (--list->selection < first) {
                            list->selection = first;
                        }
                    }
                } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
                    if (!PAD_HELD(PAD_UP)) {
                        if (++list->selection > last) {
                            list->selection = last;
                        }
                    }
                }
                if (old != list->selection) {
                    if (list->selling == 0) {
                        item = list->shopItems[list->selection];
                    } else {
                        item = list->items[list->selection];
                    }
                    list->dialog->showItem(list->dialog, item, 1);
                        win->cursor->setPos(win->cursor, (list->selection % 2) * 0x83 + 0x1D,
                                    (list->selection % list->pageSize) / 2 * 0xE + 0x24);
                    SOUND.playSound(SOUND_CURSOR);
                }
            }
        }
        STITSHOP_drawItemList(list);
        break;
    case TASK_KILL:
        break;
    }
}

/* Freezes the list's cursor (still, in palette 7) and its input, or lets them go
   again (list->freezeCursor): while the details panel turns its page */
void STITSHOP_freezeListCursor(ShopItemList *list, s32 frozen) {
    ShopItemListWindows *win = list->children;

    if (frozen) {
        win->cursor->setPalette(win->cursor, PALETTE_GREY);
        win->cursor->setStill(win->cursor, 1);
        list->active = 0;
    } else {
        win->cursor->setPalette(win->cursor, PALETTE_WHITE);
        win->cursor->setStill(win->cursor, 0);
        list->active = 1;
    }
}

/* Shows the list's cursor at the selection and takes the d-pad, or hides it and
   stops (list->showCursor) */
void STITSHOP_showListCursor(ShopItemList *list, s32 visible) {
    ShopItemListWindows *win = list->children;
    s32 row;

    list->active = visible;
    row = (list->selection % list->pageSize) / 2;
    win->cursor->setPos(win->cursor, (list->selection % 2) * 0x83 + 0x1D, row * 0xE + 0x24);
    win->cursor->setVisible(win->cursor, visible);
}

/* Runs the list's states again (list->start), to reopen it after the quantity
   and the yes/no */
void STITSHOP_startItemList(ShopItemList *list) {
    list->setState(list, TASK_RUN);
}

/* Closes the list (list->close): hides the cursor and fades it out */
void STITSHOP_closeItemList(ShopItemList *list) {
    list->setState(list, TASK_RUN);
    list->substate = 50;
    STITSHOP_showListCursor(list, 0);
}

/* The item at the list's selection, of the shop's or of the bag's sellable ones
   (list->getSelected) */
s32 STITSHOP_getSelectedItem(ShopItemList *list) {
    if (!list->selling) {
        return list->shopItems[list->selection];
    }
    return list->items[list->selection];
}

/* Lists the sellable items again, shows the page and the selection in the
   dialog (list->refresh) */
void STITSHOP_refreshItemList(ShopItemList *list) {
    ShopItemListWindows *win = list->children;

    STITSHOP_listSellable(list);
    STITSHOP_showItemPage(list, win, 1);
    list->dialog->showItem(list->dialog, list->items[list->selection], 1);
}

/* Lists the bag's items of the list's type that can be sold, those with a sell
   price (list->listBag, again after a sale) */
void STITSHOP_listSellable(ShopItemList *list) {
    s32 i;
    s32 n;

    list->count = 0;
    n = ITEM_FUNCS.list(list->type, list->bag);
    for (i = 0; i < n; i++) {
        if (ITEM_FUNCS.get(list->bag[i])->sellPrice != 0) {
            list->items[list->count++] = list->bag[i];
        }
    }
}

/* Creates the item list (task) of a dialog: the items of the shop type when
   buying, the bag's of the item type when selling */
ShopItemList *STITSHOP_createItemList(ShopDialog *dialog, s32 type, s32 selling) {
    ShopItemList *list = createTask(STITSHOP_updateItemList, sizeof(ShopItemList), 0x50);

    list->start = STITSHOP_startItemList;
    list->close = STITSHOP_closeItemList;
    list->getSelected = STITSHOP_getSelectedItem;
    list->showCursor = STITSHOP_showListCursor;
    list->freezeCursor = STITSHOP_freezeListCursor;
    list->refresh = STITSHOP_refreshItemList;
    list->listBag = STITSHOP_listSellable;
    list->layer = SCREEN_LAYER;
    list->depth = 4;
    list->dialog = dialog;
    list->selling = selling;
    list->type = type;
    return list;
}
