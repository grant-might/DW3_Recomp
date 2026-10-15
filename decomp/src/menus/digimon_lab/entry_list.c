/* The entry list: a partner's slots or all its entries, with the stats and
   skills of the one picked */

#include "menus/digimon_lab.h"

/* Creates the panel's windows */
void STGDGLAB_createEntryListWindows(LabEntryList *panel, LabEntryListWindows *windows) {
    s32 i;
    TextWindow **items;

    windows->title = createTextWindow(panel->layer, 1, 0xAE, 0x15);
    for (i = 0; i < 3; i++) {
        windows->options[i] = createTextWindow(panel->layer, 1, 0xA7, i * 0xE + 0x31);
    }
    windows->digimonName = createTextWindow(panel->layer, 1, 0x5B, 0x6A);
    for (i = 0; i < 6; i++) {
        windows->values[i] = createTextWindow(panel->layer, 1, 0x7D, i * 0xE + 0x7F);
    }
    for (i = 0; i < 7; i++) {
        windows->values[i + 6] = createTextWindow(panel->layer, 1, 0xA7, i * 0xE + 0x7F);
    }
    windows->levelLabel = createTextWindow(panel->layer, 1, 0xEB, 0x6A);
    windows->values[13] = createTextWindow(panel->layer, 1, 0x12D, 0x6A);
    for (i = 0; i < 6; i++) {
        windows->skills[i] = createTextWindow(panel->layer, 1, 0xC2, i * 0xE + 0x8B);
    }
    for (i = 0, items = panel->children; i < panel->childCount - 2; i++, items++) {
        if (*items != NULL) {
            (*items)->setDepth(*items, panel->depth - 1);
        }
    }
}

/* Shows the list of ids and the stats and skills of the one under the
   cursor; or hides the panel's windows */
void STGDGLAB_showEntryList(LabEntryList *panel, LabEntryListWindows *windows, s32 show) {
    PartnerTotals totals;
    PartnerEntry entry;
    DigimonData *data;
    Partner *partner;
    TextWindow **items;
    s16 *stats;
    s32 id;
    s32 value;
    s32 i;

    if (show) {
        if (panel->count == 0) {
            windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1E);
        } else {
            windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1F);
        }
        partner = &GAME.partners[panel->partner];
        for (i = 0; i < 3; i++) {
            id = panel->ids[panel->scroll + i];
            if (id < 3) {
                if (i == 0) {
                    windows->options[0]->setString(windows->options[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1B);
                } else {
                    windows->options[i]->setVisible(windows->options[i], 0);
                }
            } else {
                windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)),
                                               GET_DIGIMON(id)->nameId);
                if (partner->battleDigivolve == id) {
                    windows->options[i]->setPalette(windows->options[i], PALETTE_BLUE);
                } else {
                    windows->options[i]->setPalette(windows->options[i], PALETTE_WHITE);
                }
            }
        }
        id = panel->ids[panel->scroll + panel->cursor];
        if (id != 0) {
            GAME.funcs.getPartnerEntry(panel->partner, id, &entry);
            GAME.funcs.computeStats(panel->partner, &totals);
            data = GET_DIGIMON(id);
            windows->digimonName->setString(windows->digimonName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            windows->levelLabel->setString(windows->levelLabel, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x13);
            windows->values[13]->setNumber(windows->values[13], 0, entry.level);
            for (i = 0; i < 6; i++) {
                value = totals.fields.battle[i] + data->battleStats[i];
                if (value >= 1000) {
                    value = 999;
                }
                windows->values[i]->setNumber(windows->values[i], 0, value);
                if ((i == 0 && totals.fields.lowered[0] != 0) || (i == 1 && totals.fields.lowered[1] != 0) ||
                    (i == 4 && totals.fields.lowered[2] != 0)) {
                    windows->values[i]->setPalette(windows->values[i], PALETTE_PURPLE);
                }
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
        } else {
            windows->digimonName->setVisible(windows->digimonName, 0);
            windows->levelLabel->setVisible(windows->levelLabel, 0);
            for (i = 0; i < 14; i++) {
                windows->values[i]->setVisible(windows->values[i], 0);
            }
        }
    } else {
        for (i = 0, items = panel->children; i < panel->childCount - 2; i++, items++) {
            if (*items != NULL) {
                (*items)->setVisible(*items, 0);
            }
        }
    }
}

/* The entry list's close: fades it out and hides its windows and cursor */
void STGDGLAB_closeEntryList(LabEntryList *panel) {
    LabEntryListWindows *windows = panel->children;

    STGDGLAB_data.funcs.startFade(&panel->fade, 0);
    STGDGLAB_showEntryList(panel, windows, 0);
    windows->cursor->setVisible(windows->cursor, 0);
    panel->substate++;
}

/* The panel's states: fades in, moves the cursor through the ids and
   scrolls the list; triangle closes it */
void STGDGLAB_runEntryList(LabEntryList *panel, LabEntryListWindows *windows) {
    s32 cursor;
    s32 scroll;

    switch (panel->substate) {
    case 0:
    default:
        STGDGLAB_data.funcs.startFade(&panel->fade, 1);
        panel->substate++;
        break;
    case 1:
        if (STGDGLAB_data.funcs.updateFade(&panel->fade)) {
            STGDGLAB_showEntryList(panel, windows, 1);
            windows->cursor->setVisible(windows->cursor, 1);
            if (panel->count >= 4) {
                windows->scrollBar = STGDGLAB_createScrollBar();
                windows->scrollBar->setX(windows->scrollBar, 0x122, 0xC);
                windows->scrollBar->setRange(windows->scrollBar, 0x35, 0x55);
                windows->scrollBar->setCount(windows->scrollBar, 3, panel->count);
                windows->scrollBar->setPos(windows->scrollBar, panel->cursor);
            }
            panel->substate++;
        }
        break;
    case 2:
        if (panel->count > 0) {
            cursor = panel->cursor;
            scroll = panel->scroll;
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
                if (panel->count >= 3) {
                    panel->cursor++;
                    if (panel->cursor >= 3) {
                        panel->cursor = 2;
                        panel->scroll++;
                        if (panel->scroll > panel->count - 3) {
                            panel->scroll = panel->count - 3;
                        }
                    }
                } else {
                    panel->cursor++;
                    if (panel->cursor > panel->count - 1) {
                        panel->cursor = panel->count - 1;
                    }
                }
            }
#if VERSION_EU || VERSION_JP
            if (windows->scrollBar != NULL && scroll != panel->scroll) {
                windows->scrollBar->setPos(windows->scrollBar, panel->scroll);
            }
#endif
            if (cursor != panel->cursor || scroll != panel->scroll) {
                SOUND.playSound(SOUND_CURSOR);
                STGDGLAB_showEntryList(panel, windows, 1);
                windows->cursor->setPos(windows->cursor, 0x9A, panel->cursor * 0xE + 0x31);
#if VERSION_US
                if (windows->scrollBar != NULL) {
                    windows->scrollBar->setPos(windows->scrollBar, panel->cursor + panel->scroll);
                }
#endif
            }
        }
        if (panel->closable != 0 && PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            STGDGLAB_closeEntryList(panel);
            if (windows->scrollBar != NULL) {
                windows->scrollBar->state = TASK_KILL;
            }
        }
        break;
    case 3:
        if (STGDGLAB_data.funcs.updateFade(&panel->fade)) {
            panel->setState(panel, TASK_KILL);
        }
        break;
    case 4:
        windows->cursor->setVisible(windows->cursor, 0);
        break;
    case 5:
        windows->cursor->setVisible(windows->cursor, 1);
        panel->substate = 2;
        break;
    }
    panel->titlePanel.level = panel->listPanel.level = panel->fade.level;
}

/* Draws the panel's frames, its scroll arrows and the icons of the skills
   of the id under the cursor */
void STGDGLAB_drawEntryList(LabEntryList *panel, LabEntryListWindows *windows) {
    SpriteDrawer sprite;
    PartnerEntry entry;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(panel->layer, panel->depth);
    if (panel->titlePanel.level != ONE) {
        sprite.setScale(panel->titlePanel.level, ONE, ONE);
        sprite.setPivot(0x140, 0x15);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x20, 0x92, 0xF);
    if (panel->listPanel.level != ONE) {
        sprite.setScale(panel->listPanel.level, ONE, ONE);
        sprite.setPivot(0x140, 0x45);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    if (panel->listPanel.level == ONE) {
        if (GFX.funcs.getTime() - panel->blinkTime >= 8) {
            panel->blinkTime = GFX.funcs.getTime();
            panel->blink = 1 - panel->blink;
        }
        if (panel->blink) {
            if (panel->scroll != 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x34, 0x123, 0x2D);
            }
            if (panel->scroll < panel->count - 3) {
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x35, 0x123, 0x55);
            }
        }
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x22, 0x92, 0x2A);
    if (panel->count > 0) {
        if (panel->fade.level != ONE) {
            sprite.setScale(panel->listPanel.level, ONE, ONE);
            sprite.setPivot(0x140, 0xA4);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2A, 0x54, 0x64);
        sprite.setTexture(0x140, 0);
        sprite.setLayerId(panel->layer, panel->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x14, 0x5B, 0x7F);
        GAME.funcs.getPartnerEntry(panel->partner, panel->ids[panel->scroll + panel->cursor], &entry);
        for (i = 0; i < 6; i++) {
            if (entry.skills[i] != 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16),
                            TECHS[(entry.skills[i] & SKILL_ID) - 1].icon + 0x37, 0xB4, i * 0xE + 0x8B);
            }
        }
    }
}

/* The entry list's task: lists the partner's slots and, with allEntries, its
   other entries, creates the windows, then runs and draws the list */
void STGDGLAB_updateEntryList(LabEntryList *panel, LabEntryListWindows *windows) {
    s16 slots[4];
    s16 entries[44];
    s32 i;

    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        GAME.funcs.getPartnerSlots(panel->partner, slots);
        for (i = 0; i < 3; i++) {
            if (slots[i] < 3) {
                break;
            }
            panel->ids[panel->count] = slots[i];
            panel->count++;
        }
        if (panel->allEntries != 0) {
            GAME.funcs.listPartnerEntries(panel->partner, entries);
            for (i = 0; i < 44; i++) {
                if (entries[i] >= 3 && panel->ids[0] != entries[i] && panel->ids[1] != entries[i] &&
                    panel->ids[2] != entries[i]) {
                    panel->ids[panel->count] = entries[i];
                    panel->count++;
                }
            }
        }
        STGDGLAB_createEntryListWindows(panel, windows);
        windows->cursor = createCursor(panel->layer, panel->depth - 1, 0x9A, panel->cursor * 0xE + 0x31);
        windows->cursor->setVisible(windows->cursor, 0);
        panel->titlePanel.duration = 10;
        panel->listPanel.duration = 10;
        panel->fade.duration = 10;
        break;
    case TASK_RUN:
        STGDGLAB_runEntryList(panel, windows);
        STGDGLAB_drawEntryList(panel, windows);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the list of a partner's entries (task): only its slots unless
   ALLENTRIES; CLOSABLE lets triangle close it */
LabEntryList *STGDGLAB_createEntryList(s32 partner, s32 allEntries, s32 closable) {
    LabEntryList *panel = createTask(STGDGLAB_updateEntryList, sizeof(LabEntryList), 0x70);

    panel->close = STGDGLAB_closeEntryList;
    panel->layer = SCREEN_LAYER;
    panel->depth = 2;
    panel->allEntries = allEntries;
    panel->partner = partner;
    panel->closable = closable;
    return panel;
}
