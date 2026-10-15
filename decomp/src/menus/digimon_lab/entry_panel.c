/* The second screen's entry panel: a partner's entries and its three slots,
   where the picked entry goes */

#include "menus/digimon_lab.h"

/* Creates the panel's windows and cursors */
void STGDGLAB_createEntryPanelWindows(LabEntryPanel *panel, LabEntryPanelWindows *windows) {
    s32 i;
    TextWindow **items;

    for (i = 0; i < 10; i++) {
        windows->entries[i] = createTextWindow(panel->layer, 1, (i % 2) * 0x85 + 0x27, (i / 2) * 0xE + 0x16);
    }
    windows->digimonName = createTextWindow(panel->layer, 1, 0x5B, 0x6A);
    windows->levelLabel = createTextWindow(panel->layer, 1, 0xF5, 0x6A);
    windows->values[13] = createTextWindow(panel->layer, 1, 0x12D, 0x6A);
    for (i = 0; i < 6; i++) {
        windows->skills[i] = createTextWindow(panel->layer, 1, 0xC2, i * 0xE + 0x8B);
    }
    for (i = 0; i < 6; i++) {
        windows->values[i] = createTextWindow(panel->layer, 1, 0x7D, i * 0xE + 0x7F);
    }
    for (i = 0; i < 7; i++) {
        windows->values[i + 6] = createTextWindow(panel->layer, 1, 0xA7, i * 0xE + 0x7F);
    }
    for (i = 0, items = panel->children; i < panel->childCount - 3; i++, items++) {
        if (*items != NULL) {
            (*items)->setDepth(*items, panel->depth - 1);
        }
    }
    windows->cursor = createCursor(panel->layer, panel->depth - 1, 0x1A, 0x16);
    windows->cursor->setVisible(windows->cursor, 0);
    windows->optionCursor = createCursor(panel->layer, panel->depth - 3, 0x9A, 0x83);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    for (i = 0; i < 4; i++) {
        windows->options[i] = createTextWindow(panel->layer, 1, 0xA7, i * 0xE + 0x83);
    }
    windows->help = createTextWindow(panel->layer, 1, 0x14, 0xC3);
}

/* Shows the page of the partner's entries, and the stats and skills of the
   one under the cursor; or hides the panel's windows */
void STGDGLAB_showEntryPanel(LabEntryPanel *panel, LabEntryPanelWindows *windows, s32 show) {
    PartnerTotals totals;
    PartnerEntry entry;
    DigimonData *data;
    TextWindow **items;
    s32 id;
    s32 value;
    s32 i;
    s32 j;
    s16 *stats;

    if (show) {
        for (i = 0; i < 10; i++) {
            id = panel->entries[panel->scroll * 2 + i];
            if (id >= 3) {
                windows->entries[i]->setPalette(windows->entries[i], PALETTE_WHITE);
                for (j = 0; j < 3; j++) {
                    if (panel->slots[j] == id) {
                        windows->entries[i]->setPalette(windows->entries[i], PALETTE_GREY);
                        break;
                    }
                }
                windows->entries[i]->setString(windows->entries[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)),
                                               GET_DIGIMON(id)->nameId);
            } else {
                windows->entries[i]->setVisible(windows->entries[i], 0);
            }
        }
        id = panel->entries[(panel->scroll + panel->row) * 2 + panel->col];
        if (id != 0) {
            GAME.funcs.getPartnerEntry(panel->partner, id, &entry);
            GAME.funcs.computeStats(panel->partner, &totals);
            data = GET_DIGIMON(id);
            windows->digimonName->setString(windows->digimonName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            windows->levelLabel->setString(windows->levelLabel, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x13);
            windows->values[13]->setNumber(windows->values[13], 0, entry.level);
            stats = totals.stats;
            for (i = 0; i < 6; i++) {
                value = stats[i + 6] + data->battleStats[i];
                if (value >= 1000) {
                    value = 999;
                }
                windows->values[i]->setNumber(windows->values[i], 0, value);
            }
            if (totals.fields.lowered[0] != 0) {
                windows->values[0]->setPalette(windows->values[0], PALETTE_PURPLE);
            }
            if (totals.fields.lowered[1] != 0) {
                windows->values[1]->setPalette(windows->values[1], PALETTE_PURPLE);
            }
            if (totals.fields.lowered[2] != 0) {
                windows->values[4]->setPalette(windows->values[4], PALETTE_PURPLE);
            }
            stats = totals.stats;
            for (i = 0; i < 7; i++) {
                value = stats[i + 12] + data->resistances[i];
                if (value >= 1000) {
                    value = 999;
                }
                windows->values[i + 6]->setNumber(windows->values[i + 6], 0, value);
            }
            for (i = 0; i < 6; i++) {
                if (entry.skills[i] != 0) {
                    windows->skills[i]->setString(windows->skills[i], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)),
                                                  entry.skills[i] & SKILL_ID);
                    if (entry.skills[i] & SKILL_LAST) {
                        windows->skills[i]->setPalette(windows->skills[i], PALETTE_YELLOW);
                    } else if (entry.skills[i] & SKILL_MARKED) {
                        windows->skills[i]->setPalette(windows->skills[i], PALETTE_GREEN);
                    } else {
                        windows->skills[i]->setPalette(windows->skills[i], PALETTE_WHITE);
                    }
                } else {
                    windows->skills[i]->setVisible(windows->skills[i], 0);
                }
            }
            for (i = 0; i < 14; i++) {
                windows->values[i]->setRightAlign(windows->values[i], 1);
            }
        }
        windows->cursor->setVisible(windows->cursor, 1);
    } else {
        for (i = 0, items = panel->children; i < panel->childCount - 3; i++, items++) {
            if (*items != NULL) {
                (*items)->setVisible(*items, 0);
            }
        }
        windows->cursor->setVisible(windows->cursor, 0);
    }
}

/* Shows the panel's options (the names of its three slots, then two strings
   of file 0x3A), or hides them */
void STGDGLAB_showEntryPanelOptions(LabEntryPanel *panel, LabEntryPanelWindows *windows, s32 show) {
    s32 i;

    if (show) {
        for (i = 0; i < 3; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)),
                                           GET_DIGIMON(panel->slots[i])->nameId);
        }
        windows->options[3]->setString(windows->options[3], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x2C);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x2B);
    } else {
        for (i = 0; i < 4; i++) {
            windows->options[i]->setVisible(windows->options[i], 0);
        }
        windows->help->setVisible(windows->help, 0);
    }
}

/* Moves the cursor over the entries: the shoulder buttons scroll five rows,
   up and down move a row (scrolling at the edges) and left and right a column,
   skipping empty entries; 1 when it moved */
static inline s32 moveEntryCursor(LabEntryPanel *panel) {
    s32 scroll;
    s32 row;
    s32 moved;
    s32 rows;
    s32 n;

    scroll = panel->scroll;
    moved = 0;
    if (panel->entryCount >= 11) {
        if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
            panel->scroll -= 5;
            if (panel->scroll < 0) {
                panel->scroll = 0;
            }
        } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) ||
                   (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
            rows = panel->entryCount - panel->entryCount / 2;
            for (n = 0; n < 5; n++) {
                panel->scroll++;
                if (panel->scroll + 4 > rows - 1) {
                    panel->scroll = rows - 5;
                    break;
                }
            }
        }
        if (scroll != panel->scroll) {
            moved = 1;
            panel->row = 0;
        }
    }
    if (!moved) {
        row = panel->row;
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            panel->row--;
            if (panel->row < 0) {
                panel->row = 0;
                panel->scroll--;
                if (panel->scroll < 0) {
                    panel->scroll = 0;
                }
            }
            if (row != panel->row || scroll != panel->scroll) {
                moved = 1;
            }
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            if (panel->entryCount >= 10) {
                panel->row++;
                if (panel->row >= 5) {
                    panel->row = 4;
                    panel->scroll++;
                    if (panel->scroll > panel->entryCount - panel->entryCount / 2 - 1) {
                        panel->scroll = panel->entryCount - panel->entryCount / 2 - 1;
                    }
                }
            } else {
                panel->row++;
                if (panel->row > panel->entryCount - panel->entryCount / 2 - 1) {
                    panel->row = panel->entryCount - panel->entryCount / 2 - 1;
                }
            }
            if (scroll != panel->scroll || row != panel->row) {
                if (panel->col != 0) {
                    if (panel->entries[(panel->scroll + panel->row) * 2 + panel->col] >= 3) {
                        moved = 1;
                    } else if (panel->entries[(panel->scroll + panel->row) * 2] >= 3) {
                        panel->col = 0;
                        moved = 1;
                    }
                } else if (panel->entries[(panel->scroll + panel->row) * 2] >= 3) {
                    moved = 1;
                }
                if (!moved) {
                    panel->row = row;
                    panel->scroll = scroll;
                }
            }
        }
        if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
            if (panel->col != 0) {
                panel->col = 0;
                moved = 1;
            }
        } else if ((PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) && panel->col == 0 &&
                   panel->entries[(panel->scroll + panel->row) * 2 + 1] >= 3) {
            panel->col = 1;
            moved = 1;
        }
    }
    return moved;
}

/* Cross on an entry: one already in a slot is refused; otherwise it goes in
   the slot, and the options' panel opens when the slot held the entry the
   partner digivolves to in battle, else the panel closes */
static inline void putPickedEntry(LabEntryPanel *panel, LabEntryPanelWindows *windows) {
    Partner *partner;
    s32 id;
    s32 k;

    id = panel->entries[(panel->scroll + panel->row) * 2 + panel->col];
    for (k = 0; k < 3; k++) {
        if (panel->slots[k] == id) {
            id = 0;
            break;
        }
    }
    if (id == 0) {
        SOUND.playSound(SOUND_MENU_CANCEL);
    } else {
        SOUND.playSound(SOUND_SELECT);
        partner = &GAME.partners[panel->partner];
        if (panel->slots[panel->slot] == partner->battleDigivolve) {
            panel->substate = 2;
        } else {
            STGDGLAB_showEntryPanel(panel, windows, 0);
            STGDGLAB_data.funcs.startFade(panel->fades, 0);
            panel->substate = 0;
            panel->step = 1;
        }
        panel->slots[panel->slot] = id;
        GAME.funcs.setPartnerSlots(panel->partner, panel->slots);
#if VERSION_US
        if (windows->scrollBar != NULL) {
            windows->scrollBar->state = TASK_KILL;
        }
#endif
    }
}

/* The options' panel: up and down pick the slot to digivolve to in battle, or
   none; cross sets it as the partner's, triangle sets none */
static inline void pickBattleDigivolve(LabEntryPanel *panel, LabEntryPanelWindows *windows) {
    Partner *partner;
    s32 old;

    old = panel->option;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        panel->option--;
        if (panel->option < 0) {
            panel->option = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        panel->option++;
        if (panel->option >= 4) {
            panel->option = 3;
        }
    }
    if (old != panel->option) {
        SOUND.playSound(SOUND_CURSOR);
        windows->optionCursor->setPos(windows->optionCursor, 0x9A, panel->option * 0xE + 0x83);
    }
    partner = &GAME.partners[panel->partner];
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (panel->option < 3) {
            partner->battleDigivolve = panel->slots[panel->option];
        } else {
            partner->battleDigivolve = 0;
        }
        panel->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        partner->battleDigivolve = 0;
        panel->substate++;
    }
}

/* Draws the panel: the entries' marks, the blinking scroll arrows, the frames,
   the picked entry's skill icons and, while it is shown, the options' panel */
static inline void drawEntryPanel(LabEntryPanel *panel) {
    SpriteDrawer sprite;
    PartnerEntry entry;
    s32 i;
    s32 j;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(panel->layer, panel->depth);
    sprite.setTexture(0x280, 0x100);
    if (panel->fades[0].level != ONE) {
        sprite.setScale(panel->fades[0].level, ONE, ONE);
        sprite.setPivot(0x140, 0x37);
    } else {
        for (j = 0; j < 10; j++) {
            if (panel->entries[panel->scroll * 2 + j] >= 3) {
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3E, (j % 2) * 0x85 + 0x18,
                            (j / 2) * 0xE + 0x16);
            }
        }
    }
    if (GFX.funcs.getTime() - panel->blinkTime >= 8) {
        panel->blinkTime = GFX.funcs.getTime();
        panel->blink = 1 - panel->blink;
    }
    if (panel->fades[0].level == ONE && panel->blink) {
        if (panel->scroll > 0) {
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x34, 0x123, 0x13);
        }
        if (panel->entryCount >= 10 && panel->scroll < panel->entryCount - panel->entryCount / 2 - 5) {
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x35, 0x123, 0x55);
        }
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x29, 0x12, 0xF);
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2A, 0x54, 0x64);
    sprite.setTexture(0x140, 0);
    sprite.setLayerId(panel->layer, panel->depth - 1);
    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x14, 0x5B, 0x7F);
    GAME.funcs.getPartnerEntry(panel->partner, panel->entries[(panel->scroll + panel->row) * 2 + panel->col],
                               &entry);
    for (i = 0; i < 6; i++) {
        if (entry.skills[i] != 0) {
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16),
                        TECHS[(entry.skills[i] & SKILL_ID) - 1].icon + 0x37, 0xB4, i * 0xE + 0x8B);
        }
    }
    if (panel->fades[1].level != 0) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(panel->layer, panel->depth - 2);
        sprite.setTexture(0x280, 0x100);
        if (panel->fades[1].level != ONE) {
            sprite.setScale(panel->fades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x9D);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2C, 0x95, 0x7C);
        if (panel->fades[1].level != ONE) {
            sprite.setPivot(0, 0xD0);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2B, 0, 0xBF);
    }
}

/* The panel's task: lists the partner's entries two by two, with the
   shoulder buttons scrolling five rows at a time; cross puts the picked
   entry in the slot and, when the slot held the partner's current entry,
   asks for the new one; triangle closes it */
void STGDGLAB_updateEntryPanel(LabEntryPanel *panel, LabEntryPanelWindows *windows) {
    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        panel->fades[0].duration = 10;
        STGDGLAB_data.funcs.startFade(panel->fades, 1);
        GAME.funcs.getPartnerSlots(panel->partner, panel->slots);
        panel->entryCount = GAME.funcs.listPartnerEntries(panel->partner, panel->entries);
        STGDGLAB_createEntryPanelWindows(panel, windows);
        panel->fades[1].duration = 10;
        break;
    case TASK_RUN:
        switch (panel->substate) {
        case 0:
        default:
            if (STGDGLAB_data.funcs.updateFade(panel->fades)) {
                if (panel->step) {
                    panel->setState(panel, TASK_KILL);
                } else {
#if VERSION_US
                    if (panel->entryCount >= 11) {
                        windows->scrollBar = STGDGLAB_createScrollBar();
                        windows->scrollBar->setX(windows->scrollBar, 0x122, 0xC);
                        windows->scrollBar->setRange(windows->scrollBar, 0x1B, 0x55);
                        windows->scrollBar->setCount(windows->scrollBar, 10, panel->entryCount / 2);
                        windows->scrollBar->setPos(windows->scrollBar, 0);
                    }
#endif
                    STGDGLAB_showEntryPanel(panel, windows, 1);
                    panel->substate++;
                }
            }
            break;
        case 1:
            if (moveEntryCursor(panel)) {
                SOUND.playSound(SOUND_CURSOR);
                windows->cursor->setPos(windows->cursor, panel->col * 0x85 + 0x1A, panel->row * 0xE + 0x16);
                STGDGLAB_showEntryPanel(panel, windows, 1);
#if VERSION_US
                if (windows->scrollBar != NULL) {
                    windows->scrollBar->setPos(windows->scrollBar, panel->scroll);
                }
#endif
            } else if (PAD_PRESSED(PAD_CROSS)) {
                putPickedEntry(panel, windows);
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                STGDGLAB_showEntryPanel(panel, windows, 0);
                STGDGLAB_data.funcs.startFade(panel->fades, 0);
                panel->substate = 0;
                panel->step = 1;
#if VERSION_US
                if (windows->scrollBar != NULL) {
                    windows->scrollBar->state = TASK_KILL;
                }
#endif
            }
            break;
        case 2:
            panel->fades[1].duration = 10;
            STGDGLAB_data.funcs.startFade(&panel->fades[1], 1);
            windows->cursor->setPalette(windows->cursor, PALETTE_GREY);
            windows->cursor->setStill(windows->cursor, 1);
            panel->substate++;
            break;
        case 3:
            if (STGDGLAB_data.funcs.updateFade(&panel->fades[1])) {
                STGDGLAB_showEntryPanelOptions(panel, windows, 1);
                panel->option = 0;
                windows->optionCursor->setPos(windows->optionCursor, 0x9A, 0x83);
                windows->optionCursor->setVisible(windows->optionCursor, 1);
                panel->substate++;
            }
            break;
        case 4:
            pickBattleDigivolve(panel, windows);
            break;
        case 5:
            STGDGLAB_data.funcs.startFade(&panel->fades[1], 0);
            STGDGLAB_showEntryPanelOptions(panel, windows, 0);
            windows->optionCursor->setVisible(windows->optionCursor, 0);
            panel->substate++;
            break;
        case 6:
            if (STGDGLAB_data.funcs.updateFade(&panel->fades[1])) {
                panel->substate = 0;
                panel->step = 1;
            }
            break;
        }
        drawEntryPanel(panel);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the second screen's panel (task) where one of the partner's entries
   is picked for one of its three slots */
LabEntryPanel *STGDGLAB_createEntryPanel(s32 partner, s32 slot) {
    LabEntryPanel *panel = createTask(STGDGLAB_updateEntryPanel, sizeof(LabEntryPanel), 0xA0);

    panel->layer = SCREEN_LAYER;
    panel->depth = 6;
    panel->partner = partner;
    panel->slot = slot;
    return panel;
}
