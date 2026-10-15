/* The seventh object of STSTATUS.PRO (see card_screen.c), the first screen's
   panels (STSTATUS_createItemList): its rodata starts at 0x80082B90 (USA). */

#include "menus/field_menu.h"

/* Creates the item list's windows: its title, two columns of 8 items, the page
   number, the page arrows and the cursor */
void STSTATUS_createItemListWindows(ItemList *panel, ItemListWindows *windows) {
    TextWindow *window;
    s32 i;
    s32 j;

    windows->title = createTextWindow(panel->layer, 1, 0x13, 0x14);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 2; j++) {
            window = createTextWindow(panel->layer, 1, j * 0x83 + 0x38, i * 14 + 0x25);
            windows->items[i][j] = window;
            window->setDepth(window, panel->depth - 1);
        }
    }
    windows->page = createTextWindow(panel->layer, 1, 0x9B, 0x9C);
    windows->pageSeparator = createTextWindow(panel->layer, 1, 0x9E, 0x9C);
    windows->pageCount = createTextWindow(panel->layer, 1, 0xB2, 0x9C);
    windows->prev = createTextWindow(panel->layer, 1, 0x2D, 0x97);
    windows->next = createTextWindow(panel->layer, 1, 0x102, 0x97);
    windows->cursor = createCursor(panel->layer, panel->depth - 1, (panel->cursor % 2) * 0x83 + 0x1E,
                                   (panel->cursor % 16) / 2 * 14 + 0x25);
    windows->cursor->setVisible(windows->cursor, 0);
}

/* Shows or hides the list's page and its arrows */
void STSTATUS_showItemListPage(ItemList *panel, ItemListWindows *windows, s32 show) {
    s32 first;
    s32 i;
    s32 j;

    if (show) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), panel->list + 0x16);
        first = panel->page * 16;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 2; j++) {
                if (panel->items[first + i * 2 + j] != 0) {
                    windows->items[i][j]->setString(windows->items[i][j], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)),
                                                    panel->items[first + i * 2 + j]);
                } else {
                    windows->items[i][j]->setVisible(windows->items[i][j], 0);
                }
            }
        }
        if (panel->page == 0) {
            panel->hasPrev = 0;
            windows->prev->setVisible(windows->prev, 0);
        } else {
            panel->hasPrev = 1;
            windows->prev->setString(windows->prev, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x22);
        }
        if (panel->page != panel->pageCount - 1) {
            panel->hasNext = 1;
            windows->next->setString(windows->next, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x23);
        } else {
            panel->hasNext = 0;
            windows->next->setVisible(windows->next, 0);
        }
    } else {
        windows->title->setVisible(windows->title, 0);
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 2; j++) {
                windows->items[i][j]->setVisible(windows->items[i][j], 0);
            }
        }
        panel->hasPrev = 0;
        windows->prev->setVisible(windows->prev, 0);
        panel->hasNext = 0;
        windows->next->setVisible(windows->next, 0);
    }
}

/* Shows the page with the arrows, the chosen item and the page number */
void STSTATUS_refreshItemList(ItemList *panel, ItemListWindows *windows) {
    if (panel->page == 0) {
        panel->hasPrev = 0;
        windows->prev->setVisible(windows->prev, 0);
    } else {
        panel->hasPrev = 1;
        windows->prev->setString(windows->prev, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x22);
    }
    if (panel->page == panel->pageCount - 1) {
        panel->hasNext = 0;
        windows->next->setVisible(windows->next, 0);
    } else {
        panel->hasNext = 1;
        windows->next->setString(windows->next, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x23);
    }
    STSTATUS_showItemListPage(panel, windows, 1);
    panel->screen->item = panel->items[panel->cursor];
    STSTATUS_showChosenItem(panel->screen, 1);
    STSTATUS_showItemHelp(panel->screen, 1);
    windows->page->setNumber(windows->page, 0, panel->page + 1);
    windows->page->setRightAlign(windows->page, 1);
    windows->pageSeparator->setString(windows->pageSeparator, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 4);
    windows->pageCount->setNumber(windows->pageCount, 0, panel->pageCount);
    windows->pageCount->setRightAlign(windows->pageCount, 1);
}

/* Moves the item list's cursor: L1 and R1 turn the pages; returns whether
   it moved */
s32 STSTATUS_moveItemListCursor(ItemList *panel, ItemListWindows *windows) {
    s32 page;
    s32 cursor;
    s32 first;
    s32 last;

    page = panel->page;
    cursor = panel->cursor;
    if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
        panel->page--;
        if (panel->page < 0) {
            panel->page = 0;
        }
    } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
        panel->page++;
        if (panel->page > panel->pageCount - 1) {
            panel->page = panel->pageCount - 1;
        }
    }
    if (page != panel->page) {
        SOUND.playSound(SOUND_CURSOR);
        panel->cursor = panel->page * 16;
        /* the column is always 0: the match depends on computing it */
        windows->cursor->setPos(windows->cursor, (panel->cursor % 2) * 0x83 + 0x1E, (panel->cursor % 16) / 2 * 14 + 0x25);
        STSTATUS_refreshItemList(panel, windows);
        return 1;
    }
    first = page * 16;
    /* the match depends on this form, which CSE doesn't share with first */
    last = (page + 1) * 16 - 1;
    if (last > panel->count - 1) {
        last = panel->count - 1;
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        panel->cursor -= 2;
        if (panel->cursor < first) {
            panel->cursor = first;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        panel->cursor += 2;
        if (panel->cursor > last) {
            panel->cursor = last;
        }
    }
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        if (!PAD_HELD(PAD_DOWN)) {
            panel->cursor--;
            if (panel->cursor < first) {
                panel->cursor = first;
            }
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        if (!PAD_HELD(PAD_UP)) {
            panel->cursor++;
            if (panel->cursor > last) {
                panel->cursor = last;
            }
        }
    }
    if (cursor != panel->cursor) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, (panel->cursor % 2) * 0x83 + 0x1E, (panel->cursor % 16) / 2 * 14 + 0x25);
        panel->screen->item = panel->items[panel->cursor];
        STSTATUS_showChosenItem(panel->screen, 1);
        STSTATUS_showItemHelp(panel->screen, 1);
        return 1;
    }
    return 0;
}

/* The item list's update: fades in, lets an item be chosen, fades out */
void STSTATUS_runItemList(ItemList *panel, ItemListWindows *windows) {
    switch (panel->substate) {
    case 0:
    default:
        panel->fades[0].duration = 10;
        panel->fades[1].duration = 10;
        STSTATUS_data.funcs.startFade(&panel->fades[0], 1);
        STSTATUS_data.funcs.startFade(&panel->fades[1], 1);
        panel->substate++;
        break;
    case 1:
        STSTATUS_data.funcs.updateFade(&panel->fades[0]);
        if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
            windows->cursor->setVisible(windows->cursor, 1);
            STSTATUS_refreshItemList(panel, windows);
            windows->cursor->setPos(windows->cursor, (panel->cursor % 2) * 0x83 + 0x1E,
                                    (panel->cursor % 16) / 2 * 14 + 0x25);
            panel->active = 1;
            panel->substate++;
        }
        break;
    case 2:
        if (STSTATUS_moveItemListCursor(panel, windows) == 0) {
            if (PAD_PRESSED(PAD_CROSS)) {
                if (panel->list == 0) {
                    panel->screen->itemIndex = panel->cursor;
                    panel->screen->item = panel->items[panel->cursor];
                    if (ITEM_FUNCS.get(panel->screen->item)->data.effect->flags & ITEM_USE_MENU) {
                        SOUND.playSound(SOUND_SELECT);
                        STSTATUS_showItemHelp(panel->screen, -1);
                        panel->substate++;
                    }
                }
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                panel->screen->itemIndex = -1;
                STSTATUS_fadeItemInfo(panel->screen, 0);
                panel->substate++;
            }
        }
        break;
    case 3:
        STSTATUS_showItemListPage(panel, windows, 0);
        windows->cursor->setVisible(windows->cursor, 0);
        windows->page->setVisible(windows->page, 0);
        windows->pageSeparator->setVisible(windows->pageSeparator, 0);
        windows->pageCount->setVisible(windows->pageCount, 0);
        STSTATUS_data.funcs.startFade(&panel->fades[0], 0);
        STSTATUS_data.funcs.startFade(&panel->fades[1], 0);
        panel->hasPrev = 0;
        panel->hasNext = 0;
        panel->active = 0;
        panel->substate++;
        break;
    case 4:
        STSTATUS_itemInfoFaded(panel->screen);
        STSTATUS_data.funcs.updateFade(&panel->fades[0]);
        if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
            panel->state = TASK_KILL;
        }
        break;
    }
}

/* Draws the item list's icons, page arrows and frames */
void STSTATUS_drawItemList(ItemList *panel) {
    SpriteDrawer sprite;
    ItemInfo *info;
    s32 first;
    s32 i;
    s32 j;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(panel->layer, panel->depth);
    if (panel->pageCount >= 2) {
        if (GFX.funcs.getTime() - panel->time >= 11) {
            panel->time = GFX.funcs.getTime();
            panel->arrowFrame++;
            if (panel->arrowFrame >= 4) {
                panel->arrowFrame = 0;
            }
        }
        sprite.setClutRow(panel->arrowFrame);
    }
    if (panel->hasPrev) {
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x34, 0x1E, 0x98);
    }
    if (panel->hasNext) {
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x35, 0xFD, 0x98);
    }
    sprite.setClutRow(0);
    if (panel->active) {
        info = ITEM_FUNCS.get(panel->items[panel->cursor]);
        if ((info->type == 25 || info->type == 26) && (info->data.effect->flags & ITEM_USE_MENU)) {
            sprite.setTexture(0x140, 0);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x40, (panel->cursor % 2) * 0x83 + 0x29,
                        (panel->cursor % 16) / 2 * 14 + 0x25);
        }
        first = panel->page * 16;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 2; j++) {
                if (panel->items[first + i * 2 + j]) {
                    sprite.setTexture(0x140, 0);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16),
                                ITEM_FUNCS.getCategory(panel->items[first + i * 2 + j]), j * 0x83 + 0x29,
                                i * 14 + 0x25);
                    sprite.setTexture(0x280, 0x100);
                    sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x31, j * 0x83 + 0x29, i * 14 + 0x25);
                }
            }
        }
    }
    if (panel->fades[1].level != ONE) {
        sprite.setScale(panel->fades[1].level, ONE, ONE);
        sprite.setPivot(0xC, 0x17);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x30, 0xC, 0x12);
    if (panel->fades[0].level != ONE) {
        sprite.setScale(ONE, panel->fades[0].level, ONE);
        sprite.setPivot(0xA0, 0x60);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x2E, 0, 0x1E);
    if (panel->fades[2].level) {
        sprite.setLayerId(panel->layer, panel->depth - 2);
        if (panel->fades[2].level != ONE) {
            sprite.setScale(panel->fades[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x26);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x26, 0xA0, 0x12);
    }
}

/* The item list's task: fills the list, counts its pages of 16 and puts the
   cursor on the start item (else where the last chosen one was), then runs and
   draws the list */
void STSTATUS_updateItemList(ItemList *panel, ItemListWindows *windows) {
    s32 found;
    s32 last;
    s32 i;

    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        STSTATUS_fillItemList(panel);
        if (panel->count / 16 != 0) {
            panel->pageCount = panel->count / 16 + ((panel->count & 15) != 0);
        } else {
            panel->pageCount = 1;
        }
        if (panel->startItem != 0) {
            found = 0;
            for (i = 0; panel->items[i] != 0; i++) {
                if (panel->startItem == panel->items[i]) {
                    panel->cursor = i;
                    panel->page = i / 16;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                last = panel->screen->itemIndex;
                if (panel->items[last] > 0) {
                    panel->cursor = last;
                    panel->page = panel->screen->itemIndex / 16;
                } else if (last - 1 > 0) {
                    panel->cursor = last - 1;
                    panel->page = (last - 1) / 16;
                }
            }
        }
        STSTATUS_createItemListWindows(panel, windows);
        break;
    case TASK_RUN:
        STSTATUS_runItemList(panel, windows);
        STSTATUS_drawItemList(panel);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the first screen's list of the items of a list (task), the cursor on
   ITEM when it is not 0 */
ItemList *STSTATUS_createItemList(ItemScreen *screen, s32 list, s32 item) {
    ItemList *panel = createTask(STSTATUS_updateItemList, sizeof(ItemList), sizeof(ItemListWindows));

    panel->layer = SCREEN_LAYER;
    panel->depth = 3;
    panel->screen = screen;
    if (item != 0) {
        panel->startItem = item;
    }
    panel->list = list;
    return panel;
}
