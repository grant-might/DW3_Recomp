/* The third object of STSTATUS.PRO (see card_screen.c), the fifth screen's
   panel of STSTATUS_createEquipPanel: its rodata starts at 0x800826CC (USA). */

#include "menus/field_menu.h"

/* Creates the equipment panel's windows: the six slots, the list of items that
   fit the chosen one, their counts, the help and the slot's item */
void STSTATUS_createEquipWindows(EquipPanel *panel, EquipPanelWindows *windows) {
    s32 i;

    windows->title = createTextWindow(panel->layer, 1, 0x98, 0x13);
    windows->cursor = createCursor(panel->layer, panel->depth - 1, 0xA5, 0x31);
    windows->cursor->setVisible(windows->cursor, 0);
    for (i = 0; i < 6; i++) {
        windows->slots[i] = createTextWindow(panel->layer, 1, 0xC0, i * 14 + 0x31);
    }
    windows->listTitle = createTextWindow(panel->layer, 1, 0x7F, 0x3B);
    windows->listCursor = createCursor(panel->layer, panel->depth - 1, 0x86, 0x4B);
    windows->listCursor->setVisible(windows->listCursor, 0);
    for (i = 0; i < 8; i++) {
        windows->rows[i].name = createTextWindow(panel->layer, 1, 0xA0, i * 14 + 0x4B);
        windows->rows[i].times = createTextWindow(panel->layer, 1, 0x10D, i * 14 + 0x4B);
        windows->rows[i].count = createTextWindow(panel->layer, 1, 0x120, i * 14 + 0x4B);
    }
    windows->help = createTextWindow(panel->layer, 1, 0x14, 0xC6);
    windows->kind = createTextWindow(panel->layer, 1, 0x14, 0xD5);
    windows->slotTitle = createTextWindow(panel->layer, 1, 0xA7, 0x13);
    windows->slotItem = createTextWindow(panel->layer, 1, 0xC0, 0x23);
}

/* The partner's equipment, or the slots' names where it has none */
void STSTATUS_showEquipment(EquipPanel *panel, EquipPanelWindows *windows, s32 show) {
    PartnerStats *stats;
    s16 item;
    s32 i;

    if (show) {
        stats = GAME.funcs.getPartnerStats(panel->partner);
        for (i = 0; i < 6; i++) {
            item = stats->equip[i];
            if (item > 0) {
                windows->slots[i]->setString(windows->slots[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
            } else {
                windows->slots[i]->setString(windows->slots[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_slotStrings[i]);
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            windows->slots[i]->setVisible(windows->slots[i], 0);
        }
    }
}

/* An item's name and, for slots 2 and 3, its kind (none: string 0x51) */
void STSTATUS_showEquipItem(EquipPanel *panel, EquipPanelWindows *windows, s32 item) {
    ItemData *data;

    if (item > 0) {
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_INFO)), item);
        if (panel->slot == 2 || panel->slot == 3) {
            data = ITEM_FUNCS.get(item)->data.record;
            windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_kindStrings[data->weapon.kind - 1]);
            return;
        }
    } else {
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x51);
    }
    windows->kind->setVisible(windows->kind, 0);
}

/* Fills the list of the items that fit the slot */
void STSTATUS_showEquipList(EquipPanel *panel, EquipPanelWindows *windows, s32 show) {
    s32 i;
    s32 row;
    s32 item;

    if (show) {
        windows->listTitle->setString(windows->listTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_slotStrings[panel->slot]);
        for (i = 0; i < 8; i++) {
            row = panel->scroll + i;
            if (row > panel->count - 1) {
                break;
            }
            item = panel->items[row];
            if (item > 0) {
                if (STSTATUS_data.funcs.canEquip(panel->partner, panel->slot, item)) {
                    windows->rows[i].name->setPalette(windows->rows[i].name, PALETTE_WHITE);
                } else {
                    windows->rows[i].name->setPalette(windows->rows[i].name, PALETTE_GREY);
                }
                windows->rows[i].name->setString(windows->rows[i].name, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
                windows->rows[i].times->setString(windows->rows[i].times, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x40);
                windows->rows[i].times->setRightAlign(windows->rows[i].times, 1);
                windows->rows[i].count->setNumber(windows->rows[i].count, 0, GAME.items[item]);
                windows->rows[i].count->setRightAlign(windows->rows[i].count, 1);
            } else {
                if (item == -1) {
                    windows->rows[i].name->setString(windows->rows[i].name, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x45);
                    windows->rows[i].name->setPalette(windows->rows[i].name, PALETTE_WHITE);
                } else {
                    windows->rows[i].name->setVisible(windows->rows[i].name, 0);
                }
                windows->help->setVisible(windows->help, 0);
                windows->kind->setVisible(windows->kind, 0);
                windows->rows[i].times->setVisible(windows->rows[i].times, 0);
                windows->rows[i].count->setVisible(windows->rows[i].count, 0);
            }
        }
        STSTATUS_showEquipItem(panel, windows, panel->items[panel->cursor + panel->scroll]);
    } else {
        windows->listTitle->setVisible(windows->listTitle, 0);
        for (i = 0; i < 8; i++) {
            windows->rows[i].name->setVisible(windows->rows[i].name, 0);
            windows->rows[i].times->setVisible(windows->rows[i].times, 0);
            windows->rows[i].count->setVisible(windows->rows[i].count, 0);
        }
        windows->help->setVisible(windows->help, 0);
        windows->kind->setVisible(windows->kind, 0);
    }
}

/* The item in the slot being changed, or the slot's name if it's empty */
void STSTATUS_showSlotItem(EquipPanel *panel, EquipPanelWindows *windows, s32 show) {
    PartnerStats *stats;
    s32 item;

    if (show) {
        stats = GAME.funcs.getPartnerStats(panel->partner);
        item = *(stats->equip + panel->slot); /* the match depends on this form */
        windows->slotTitle->setString(windows->slotTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x20);
        if (item > 0) {
            windows->slotItem->setString(windows->slotItem, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
        } else {
            windows->slotItem->setString(windows->slotItem, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_slotStrings[panel->slot]);
        }
    } else {
        windows->slotTitle->setVisible(windows->slotTitle, 0);
        windows->slotItem->setVisible(windows->slotItem, 0);
    }
}

/* Draws the equipment panels: the partner's equipment, the list of items
   that fit the slot and the slot's item */
void STSTATUS_drawEquipPanel(EquipPanel *panel) {
    SpriteDrawer sprite;
    PartnerStats *stats;
    s32 level;
    s32 item;
    s32 index;
    s32 i;

    stats = GAME.funcs.getPartnerStats(panel->partner);
    initSpriteDrawer(&sprite);
    if (panel->panels[0].level != 0) {
        sprite.setTexture(0x140, 0);
        sprite.setLayerId(panel->layer, 6);
        level = panel->panels[0].level;
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0x140, 0x19);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
    sprite.setLayerId(panel->layer, panel->depth);
    level = panel->panels[1].level;
    if (level != 0) {
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0x140, 0x58);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            sprite.setTexture(0x140, 0);
            for (i = 0; i < 6; i++) {
                item = stats->equip[i];
                if (item > 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(item), 0xB2, i * 14 + 0x31);
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x2A, 0x9D, 0x28);
    }
    level = panel->panels[2].level;
    if (level != 0) {
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0x140, 0x89);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            for (i = 0; i < 8; i++) {
                index = panel->scroll + i;
                if (index < panel->count) {
                    item = panel->items[index];
                    if (item == -1 || item > 0) {
                        sprite.setTexture(0x280, 0x100);
                        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x31, 0x91, i * 14 + 0x4B);
                        if (item > 0) {
                            sprite.setTexture(0x140, 0);
                            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(item), 0x91, i * 14 + 0x4B);
                        }
                    }
                }
            }
            if (panel->count > 8) {
                if (GFX.funcs.getTime() - panel->arrowTime >= 9) {
                    panel->arrowTime = GFX.funcs.getTime();
                    panel->arrowShown = 1 - panel->arrowShown;
                }
                sprite.setTexture(0x280, 0x100);
                if (panel->arrowShown) {
                    if (panel->scroll > 0) {
                        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x32, 0x126, 0x45);
                    }
                    if (panel->scroll < panel->count - 8) {
                        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x33, 0x126, 0xB3);
                    }
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x2B, 0x77, 0x39);
        if (panel->panels[2].level != ONE) {
            sprite.setPivot(0, 0xD3);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    level = panel->panels[3].level;
    if (level != 0) {
        if (level != ONE) {
            sprite.setScale(level, ONE, ONE);
            sprite.setPivot(0x140, 0x20);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            if (panel->showSlot) {
                item = *(stats->equip + panel->slot); /* the match depends on this form */
                if (item > 0) {
                    sprite.setTexture(0x140, 0);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(item), 0xB2, 0x23);
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x2C, 0xA0, 0x11);
    }
}

/* Once the title's panel is in, shows the title and starts the slots' panel */
static inline void STSTATUS_openEquipTitle(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[0])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3F);
        STSTATUS_data.funcs.startFade(&panel->panels[1], 1);
        panel->substate++;
    }
}

/* Once the slots' panel is in, shows the equipment and the slot cursor */
static inline void STSTATUS_openEquipSlots(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[1])) {
        STSTATUS_showEquipment(panel, windows, 1);
        windows->cursor->setVisible(windows->cursor, 1);
        panel->substate++;
    }
}

/* Choosing a slot: up and down move the cursor, cross lists the owned items
   for the slot ("none" first, then those the partner can equip, then the
   others), triangle closes the panel */
static inline void STSTATUS_chooseEquipSlot(EquipPanel *panel, EquipPanelWindows *windows) {
    s32 oldSlot;
    s32 count;
    s32 n;
    s32 i;

    oldSlot = panel->slot;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        panel->slot--;
        if (panel->slot < 0) {
            panel->slot = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        panel->slot++;
        if (panel->slot >= 6) {
            panel->slot = 5;
        }
    }
    if (oldSlot != panel->slot) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, 0xA5, panel->slot * 14 + 0x31);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        n = 1;
        count = STSTATUS_data.funcs.listItems(STSTATUS_slotLists[panel->slot], panel->owned);
        for (i = 0; i < count; i++) {
            if (STSTATUS_data.funcs.canEquip(panel->partner, panel->slot, panel->owned[i])) {
                panel->items[n++] = panel->owned[i];
            }
        }
        for (i = 0; i < count; i++) {
            if (!STSTATUS_data.funcs.canEquip(panel->partner, panel->slot, panel->owned[i])) {
                panel->items[n++] = panel->owned[i];
            }
        }
        panel->count = count + 1;
        panel->items[0] = -1;
        panel->substate = 10;
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        panel->substate = 0x32;
    }
}

/* Starts closing the slots' panel for the item list */
static inline void STSTATUS_closeEquipSlots(EquipPanel *panel, EquipPanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->panels[1], 0);
    STSTATUS_showEquipment(panel, windows, 0);
    windows->cursor->setVisible(windows->cursor, 0);
    panel->cursor = 0;
    panel->scroll = 0;
    panel->substate++;
}

/* Once the title's panel is closed, starts the slot's item panel */
static inline void STSTATUS_openSlotItem(EquipPanel *panel) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[0])) {
        STSTATUS_data.funcs.startFade(&panel->panels[3], 1);
        panel->substate++;
    }
}

/* Once the slot's item panel is in, shows the slot's item and starts the
   list's panel */
static inline void STSTATUS_openEquipList(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[3])) {
        panel->showSlot = 1;
        STSTATUS_showSlotItem(panel, windows, 1);
        STSTATUS_data.funcs.startFade(&panel->panels[2], 1);
        panel->substate++;
    }
}

/* Once the list's panel is in, shows the list, its cursor, the stats with
   the slot emptied and, past 8 items, the scroll bar */
static inline void STSTATUS_enterEquipList(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[2])) {
        STSTATUS_showEquipList(panel, windows, 1);
        windows->listCursor->setPos(windows->listCursor, 0x89, 0x4B);
        windows->listCursor->setVisible(windows->listCursor, 1);
        panel->screen->previewStats(panel->screen, panel->slot, 0);
        if (panel->count >= 9) {
            windows->scrollBar = STSTATUS_createScrollBar();
            windows->scrollBar->setX(windows->scrollBar, 0x125, 0xC);
            windows->scrollBar->setRange(windows->scrollBar, 0x4E, 0xB3);
            windows->scrollBar->setCount(windows->scrollBar, 8, panel->count);
            windows->scrollBar->setPos(windows->scrollBar, 0);
        }
        panel->substate++;
    }
}

/* Choosing the item: up and down move the cursor, L1 and R1 scroll a page,
   the stats show the item's effect, cross equips it, triangle goes back */
static inline void STSTATUS_chooseEquipItem(EquipPanel *panel, EquipPanelWindows *windows) {
    s32 oldCursor;
    s32 oldScroll;
    s32 item;

    oldCursor = panel->cursor;
    oldScroll = panel->scroll;
    if (panel->count >= 9) {
        if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
            panel->scroll -= 8;
            if (panel->scroll < 0) {
                panel->scroll = 0;
            }
        } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
            panel->scroll += 8;
            if (panel->scroll > panel->count - 8) {
                panel->scroll = panel->count - 8;
            }
        }
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        panel->cursor--;
        if (panel->cursor < 0) {
            panel->cursor = 0;
            panel->scroll--;
            if (panel->scroll < 0) {
                panel->scroll = 0;
            }
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (panel->count < 8) {
            panel->cursor++;
            if (panel->cursor > panel->count - 1) {
                panel->cursor = panel->count - 1;
            }
        } else {
            panel->cursor++;
            if (panel->cursor >= 8) {
                panel->cursor = 7;
                panel->scroll++;
                if (panel->scroll > panel->count - 8) {
                    panel->scroll = panel->count - 8;
                }
            }
        }
    }
    if (oldCursor != panel->cursor) {
        SOUND.playSound(SOUND_CURSOR);
        windows->listCursor->setPos(windows->listCursor, 0x86, panel->cursor * 14 + 0x4B);
#if VERSION_US
        if (windows->scrollBar != NULL) {
            windows->scrollBar->setPos(windows->scrollBar, panel->cursor + panel->scroll);
        }
#endif
    }
    if (oldScroll != panel->scroll) {
        SOUND.playSound(SOUND_CURSOR);
        STSTATUS_showEquipList(panel, windows, 1);
        if (windows->scrollBar != NULL) {
#if VERSION_US
            /* the USA version's bar follows the chosen item, the European and
               Japanese ones the page */
            windows->scrollBar->setPos(windows->scrollBar, panel->cursor + panel->scroll);
#else
            windows->scrollBar->setPos(windows->scrollBar, panel->scroll);
#endif
        }
    }
    item = panel->items[panel->cursor + panel->scroll];
    if (oldCursor != panel->cursor || oldScroll != panel->scroll) {
        if (STSTATUS_data.funcs.canEquip(panel->partner, panel->slot, item)) {
            panel->screen->previewStats(panel->screen, panel->slot, item);
        } else {
            panel->screen->previewStats(panel->screen, -1, 0);
        }
        STSTATUS_showEquipItem(panel, windows, item);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        if (STSTATUS_data.funcs.canEquip(panel->partner, panel->slot, item)) {
            STSTATUS_data.funcs.equip(panel->partner, panel->slot, item);
            panel->substate = 0x14;
            panel->showSlot = 0;
            panel->screen->previewStats(panel->screen, panel->slot, item);
            if (windows->scrollBar != NULL) {
                windows->scrollBar->state = 3;
            }
            SOUND.playSound(SOUND_SELECT);
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        panel->screen->previewStats(panel->screen, -1, 0);
        panel->substate = 0x14;
        panel->showSlot = 0;
        if (windows->scrollBar != NULL) {
            windows->scrollBar->state = 3;
        }
    }
}

/* Starts closing the list's panel */
static inline void STSTATUS_closeEquipList(EquipPanel *panel, EquipPanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->panels[2], 0);
    STSTATUS_showEquipList(panel, windows, 0);
    windows->listCursor->setVisible(windows->listCursor, 0);
    panel->substate++;
}

/* Once the list's panel is closed, starts closing the slot's item panel */
static inline void STSTATUS_closeSlotItem(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[2])) {
        STSTATUS_data.funcs.startFade(&panel->panels[3], 0);
        STSTATUS_showSlotItem(panel, windows, 0);
        panel->substate++;
    }
}

/* Starts closing the panel: the slots' panel, the equipment and the cursor */
static inline void STSTATUS_closeEquipPanel(EquipPanel *panel, EquipPanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->panels[1], 0);
    STSTATUS_showEquipment(panel, windows, 0);
    windows->cursor->setVisible(windows->cursor, 0);
    panel->substate++;
}

/* Once the slots' panel is closed, starts closing the title's panel */
static inline void STSTATUS_closeEquipTitle(EquipPanel *panel, EquipPanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->panels[1])) {
        windows->title->setVisible(windows->title, 0);
        STSTATUS_data.funcs.startFade(&panel->panels[0], 0);
        panel->substate++;
    }
}

/* The equipment panel's steps: a slot is chosen, then an item from the list
   of those that fit it, the ones the partner can equip first */
void STSTATUS_runEquipPanel(EquipPanel *panel, EquipPanelWindows *windows) {
    switch (panel->substate) {
    case 0:
    default:
        STSTATUS_data.funcs.startFade(&panel->panels[0], 1);
        panel->substate++;
        break;
    case 1:
        STSTATUS_openEquipTitle(panel, windows);
        break;
    case 2:
        STSTATUS_openEquipSlots(panel, windows);
        break;
    case 3:
        STSTATUS_chooseEquipSlot(panel, windows);
        break;
    case 10:
        STSTATUS_closeEquipSlots(panel, windows);
        break;
    case 12:
        STSTATUS_openSlotItem(panel);
        break;
    case 13:
        STSTATUS_openEquipList(panel, windows);
        break;
    case 14:
        STSTATUS_enterEquipList(panel, windows);
        break;
    case 15:
        STSTATUS_chooseEquipItem(panel, windows);
        break;
    case 0x14:
        STSTATUS_closeEquipList(panel, windows);
        break;
    case 0x15:
        STSTATUS_closeSlotItem(panel, windows);
        break;
    case 0x16:
        if (STSTATUS_data.funcs.updateFade(&panel->panels[3])) {
            panel->substate = 0;
        }
        break;
    case 0x32:
        STSTATUS_closeEquipPanel(panel, windows);
        break;
    case 11:
    case 0x33:
        STSTATUS_closeEquipTitle(panel, windows);
        break;
    case 0x34:
        if (STSTATUS_data.funcs.updateFade(&panel->panels[0])) {
            panel->state = 3;
        }
        break;
    }
}

/* The equipment panel's task: creates its windows, then runs and draws it; when
   killed it clears the fifth screen's unk70 */
void STSTATUS_updateEquipPanel(EquipPanel *panel, EquipPanelWindows *windows) {
    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        panel->scroll = 0;
        STSTATUS_createEquipWindows(panel, windows);
        panel->panels[3].duration = 10;
        panel->panels[1].duration = 10;
        panel->panels[2].duration = 10;
        panel->panels[0].duration = 10;
        break;
    case TASK_RUN:
        STSTATUS_runEquipPanel(panel, windows);
        STSTATUS_drawEquipPanel(panel);
        break;
    case TASK_KILL:
        panel->screen->unk70 = 0;
        /* fallthrough */
    case TASK_DONE:
        break;
    }
}

/* Creates the fifth screen's equipment panel (task) for its chosen party member */
EquipPanel *STSTATUS_createEquipPanel(StatsScreen *screen) {
    EquipPanel *panel = createTask(STSTATUS_updateEquipPanel, sizeof(EquipPanel), sizeof(EquipPanelWindows));

    panel->layer = SCREEN_LAYER;
    panel->depth = 4;
    panel->screen = screen;
    panel->partner = GAME.funcs.getPartyMember(screen->member);
    return panel;
}

/* The strings of the empty equipment slots */
s32 STSTATUS_slotStrings[] = {
    65, 66, 67, 77,
    68, 68,
};
/* The strings of the item kinds (WeaponData.kind), from 1 */
s32 STSTATUS_kindStrings[] = {
    67, 77, 79, 65,
    66, 68, 80, 68,
};
/* The item list of each equipment slot */
s32 STSTATUS_slotLists[] = {
    6, 7, 5, 5,
    4, 4,
};
