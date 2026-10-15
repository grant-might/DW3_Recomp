/* FIGHTSTG's item menu, with the cursors of the technique and switch menus
   too, which the original has here. */

#include "battle/battle_mode.h"

/* Draws the page's item icons, the page arrows (blinking) and the frame */
void FIGHTSTG_drawItemMenu(ItemMenu *task) {
    SpriteDrawer drawer;
    void *sheet;
    s32 index;
    s32 i;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x140, 0);
    sheet = FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16);
    for (i = 0; i < ITEM_MENU_LINES; i++) {
        index = task->page * ITEM_MENU_LINES + i;
        if (index > task->count - 1) {
            break;
        }
        drawer.draw(sheet, ITEM_FUNCS.getCategory(task->usable[index]), 0x1D, 0x45 + i * 0xE);
    }
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    if (GFX.funcs.getTime() & 0x10) {
        if (task->page > 0) {
            drawer.draw(sheet, 0x1F, 0x10, 0xA9);
        }
        if (task->page < task->pageCount - 1) {
            drawer.draw(sheet, 0x20, 0x90, 0xA9);
        }
    }
    drawer.draw(sheet, 0x27, 8, 0x3E);
    drawer.draw(sheet, 0x28, 0xA6, 0xA0);
    drawer.draw(sheet, 0x31, 0xB, 0xBC);
}

/* Makes the item menu's windows: the page buttons, the count's label, the
   seven names, the description and the count */
void FIGHTSTG_createItemWindows(ItemMenu *task, ItemMenuWindows *w) {
    char *text;
    s32 i;

    text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
    w->prevButton = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x1A, 0xA9);
    w->prevButton->setString(w->prevButton, text, 0x11);
    w->prevButton->setPalette(w->prevButton, PALETTE_DARK_BLUE);
    w->nextButton = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x80, 0xA9);
    w->nextButton->setString(w->nextButton, text, 0x12);
    w->nextButton->setPalette(w->nextButton, PALETTE_DARK_BLUE);
    w->countLabel = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xAC, 0xA5);
    w->countLabel->setString(w->countLabel, text, 0xC);
    for (i = 0; i < ITEM_MENU_LINES; i++) {
        w->names[i] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x2A, 0x45 + i * 0xE);
    }
    w->message = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xC2);
    w->count = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xC6, 0xA5);
}

/* Shows the page's names and the description and count of the item under
   the cursor, or text 0x15 and 0x1A when there are none */
void FIGHTSTG_showItemPage(ItemMenu *task, ItemMenuWindows *w) {
    s32 index;
    s32 item;
    s32 i;

    if (task->count != 0) {
        for (i = 0; i < ITEM_MENU_LINES; i++) {
            index = task->page * ITEM_MENU_LINES + i;
            if (index > task->count - 1) {
                w->names[i]->setVisible(w->names[i], 0);
            } else {
                item = task->usable[index];
                if (item != 0) {
                    w->names[i]->setString(w->names[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
                }
            }
        }
        item = task->usable[task->page * ITEM_MENU_LINES + w->cursor->sel];
        w->message->setString(w->message, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_INFO)), item);
        w->count->setNumber(w->count, 0, GAME.items[item]);
    } else {
        w->message->setString(w->message, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x15);
        w->count->setString(w->count, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x1A);
    }
    w->count->setRightAlign(w->count, 1);
}

/* the cursors of the items' (FIGHTSTG_updateItemMenu), the techniques'
   (FIGHTSTG_updateTechMenu) and the partner switch's (FIGHTSTG_updateSwitchMenu) menus */
CursorLayout FIGHTSTG_itemCursor = {
    1, 14, 69, 14, -1, 0, 0, 0,
};
CursorLayout FIGHTSTG_techCursor = {
    1, 14, 69, 14, -1, 0, 0, 0,
};
CursorLayout FIGHTSTG_switchCursor = {
    2, 17, 78, 32, 47, 16, 77, 32,
};

/* The item menu's task: lists the items usable in battle (flag 2), then L1
   and R1 turn the pages, cross picks the item under the cursor into *result
   and triangle gives -2; frees the list when killed */
void FIGHTSTG_updateItemMenu(ItemMenu *task, ItemMenuWindows *w) {
    s32 total;
    s32 pressed;
    s32 page;
    s32 rows;
    s32 n;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        total = ITEM_FUNCS.list(1, task->items);
        task->count = 0;
        for (i = 0; i < total; i++) {
            if (task->items[i] == 0) {
                break;
            }
            if (ITEM_FUNCS.get(task->items[i])->data.effect->flags & ITEM_USE_BATTLE) {
                task->count++;
            }
        }
        if (task->count != 0) {
            task->usable = HEAP.alloc(task->count * 2, 2);
            n = 0;
            for (i = 0; i < total; i++) {
                if (task->items[i] == 0) {
                    break;
                }
                if (ITEM_FUNCS.get(task->items[i])->data.effect->flags & ITEM_USE_BATTLE) {
                    task->usable[n++] = task->items[i];
                }
            }
            if (task->count % ITEM_MENU_LINES != 0) {
                task->pageCount = task->count / ITEM_MENU_LINES + 1;
            } else {
                task->pageCount = task->count / ITEM_MENU_LINES;
            }
            if (task->count != 0) {
                if (task->count > ITEM_MENU_LINES) {
                    FIGHTSTG_itemCursor.count = ITEM_MENU_LINES;
                } else {
                    FIGHTSTG_itemCursor.count = task->count;
                }
            }
        }
        w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_itemCursor);
        FIGHTSTG_createItemWindows(task, w);
        FIGHTSTG_showItemPage(task, w);
        task->nextState(task);
        break;
    case TASK_RUN:
        FIGHTSTG_drawItemMenu(task);
        pressed = PAD.getPressed(0);
        page = task->page;
        if (task->pageCount != 0) {
            if (pressed & (1 << PAD_L1)) {
                if (--task->page < 0) {
                    task->page = 0;
                }
            } else if (pressed & (1 << PAD_R1)) {
                if (++task->page > task->pageCount - 1) {
                    task->page = task->pageCount - 1;
                }
            }
        }
        if (page != task->page) {
            w->cursor->sel = 0;
            rows = task->count - task->page * ITEM_MENU_LINES;
            if (rows > ITEM_MENU_LINES) {
                rows = ITEM_MENU_LINES;
            }
            w->cursor->params.count = rows;
            FIGHTSTG_showItemPage(task, w);
            SOUND.playSound(SOUND_MENU_MOVE);
        } else if (task->sel != w->cursor->sel) {
            FIGHTSTG_showItemPage(task, w);
        } else if (pressed & (1 << PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            if (task->count != 0) {
                *task->result = task->usable[task->page * ITEM_MENU_LINES + w->cursor->sel];
                task->setState(task, 3);
                w->cursor->locked = 1;
                break;
            }
        } else if (pressed & (1 << PAD_TRIANGLE)) {
            /* the match depends on the goto, which puts the cancel after the
               cursor's line */
            goto cancel;
        }
        task->sel = w->cursor->sel;
        break;
    cancel:
        SOUND.playSound(SOUND_MENU_CANCEL);
        *task->result = -2;
        task->setState(task, 3);
        break;
    case 2:
        break;
    case 3:
        if (task->usable != NULL) {
            HEAP.free(task->usable);
        }
        break;
    }
}

/* Opens the battle's item menu; *result gets the item picked, or -2 to go
   back */
ItemMenu *FIGHTSTG_createItemMenu(s32 *result) {
    ItemMenu *task = createTask(FIGHTSTG_updateItemMenu, sizeof(ItemMenu), sizeof(ItemMenuWindows));

    task->result = result;
    *result = -1;
    return task;
}
