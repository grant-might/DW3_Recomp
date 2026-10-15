/* The fourth object of STSTATUS.PRO (see card_screen.c), the fifth screen's
   panel of STSTATUS_createDigivolvePanel: its rodata starts at 0x800827A0 (USA). */

#include "menus/field_menu.h"

/* Creates the digivolution panel's windows: the three slots, the question about
   a slot, the entry's techniques and stats, the help and the MP cost */
void STSTATUS_createDigivolveWindows(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    WindowPos *layout;
    WindowPos *pos;
    s32 i;

    for (i = 0; i < 3; i++) {
        windows->slots[i] = createTextWindow(panel->layer, 1, 0xBD, i * 14 + 0x31);
    }
    windows->slotCursor = createCursor(panel->layer, panel->depth - 1, 0xB0, panel->slot * 14 + 0x31);
    windows->slotCursor->setVisible(windows->slotCursor, 0);
    windows->partnerName = createTextWindow(panel->layer, 1, 0x5E, 0x4D);
    windows->choiceTitle = createTextWindow(panel->layer, 1, 0xB2, 0x2A);
    for (i = 0; i < 2; i++) {
        windows->options[i] = createTextWindow(panel->layer, 1, 0xBD, i * 14 + 0x3A);
    }
    windows->optionCursor = createCursor(panel->layer, panel->depth - 1, 0xB4, panel->option * 14 + 0x3A);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    windows->entryName = createTextWindow(panel->layer, 1, 0x4F, 0x68);
    windows->skillLabel = createTextWindow(panel->layer, 1, 0xEB, 0x68);
    windows->skillLevel = createTextWindow(panel->layer, 1, 0x12F, 0x68);
    for (i = 0; i < 6; i++) {
        windows->list[i] = createTextWindow(panel->layer, 1, 0x71, i * 14 + 0x7C);
    }
    for (i = 0; i < 7; i++) {
        windows->list[i + 6] = createTextWindow(panel->layer, 1, 0x9B, i * 14 + 0x7C);
    }
    for (i = 0; i < 6; i++) {
        windows->values[i] = createTextWindow(panel->layer, 1, 0xC4, i * 14 + 0x88);
    }
    windows->listCursor = createCursor(panel->layer, panel->depth - 1, 0xA9, 0x88);
    windows->listCursor->setVisible(windows->listCursor, 0);
    windows->title = createTextWindow(panel->layer, 1, 0x98, 0x13);
    layout = STSTATUS_data.layout;
    pos = &layout[12];
    windows->help = createTextWindow(panel->layer, 1, pos->x, pos->y);
    pos = &layout[13];
    windows->mpLabel = createTextWindow(panel->layer, 1, pos->x, pos->y);
    pos = &layout[14];
    windows->mp = createTextWindow(panel->layer, 1, pos->x, pos->y);
}

/* The Digimon in the partner's three slots, the one it is highlighted, and
   the partner's own name */
void STSTATUS_showDigivolveSlots(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show) {
    Partner *partner;
    DigimonData *data;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(panel->member);
        partner = &GAME.partners[id];
        for (i = 0; i < 3; i++) {
            if (panel->slots[i] > 0) {
                data = GET_DIGIMON(panel->slots[i]);
                windows->slots[i]->setString(windows->slots[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
                if (partner->battleDigivolve == panel->slots[i]) {
                    windows->slots[i]->setPalette(windows->slots[i], PALETTE_BLUE);
                } else {
                    windows->slots[i]->setPalette(windows->slots[i], PALETTE_WHITE);
                }
            } else {
                windows->slots[i]->setVisible(windows->slots[i], 0);
            }
        }
        data = &DIGIMON_DATA[id];
        windows->partnerName->setString(windows->partnerName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
    } else {
        for (i = 0; i < 3; i++) {
            windows->slots[i]->setVisible(windows->slots[i], 0);
        }
        windows->partnerName->setVisible(windows->partnerName, 0);
    }
}

/* Shows or hides the question about the chosen slot: its Digimon's name, then
   to start battles as it (or to stop, when it already does) or its techniques */
void STSTATUS_showDigivolveChoices(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show) {
    Partner *partner;
    DigimonData *data;
    s32 i;

    if (show) {
        partner = &GAME.partners[GAME.funcs.getPartyMember(panel->member)];
        data = GET_DIGIMON(panel->slots[panel->slot]);
        windows->choiceTitle->setString(windows->choiceTitle, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
        if (partner->battleDigivolve == panel->slots[panel->slot]) {
            windows->options[0]->setString(windows->options[0], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3A);
        } else {
            windows->options[0]->setString(windows->options[0], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x39);
        }
        windows->options[1]->setString(windows->options[1], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3B);
    } else {
        windows->choiceTitle->setVisible(windows->choiceTitle, 0);
        for (i = 0; i < 2; i++) {
            windows->options[i]->setVisible(windows->options[i], 0);
        }
    }
}

/* The stats and techniques of the slot's entry (or of the partner itself),
   or hides them */
void STSTATUS_showDigivolveStats(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show) {
    PartnerTotals totals;
    PartnerEntry entry;
    DigimonData *data;
    s32 id;
    s32 slot;
    s32 value;
    s32 tech;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(panel->member);
        if (panel->fromEntry) {
            slot = panel->slots[panel->slot];
            GAME.funcs.getPartnerEntry(id, slot, &entry);
            GAME.funcs.computeStats(id, &totals);
            data = GET_DIGIMON(slot);
            windows->entryName->setString(windows->entryName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            windows->skillLabel->setString(windows->skillLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x38);
            windows->skillLevel->setNumber(windows->skillLevel, 0, entry.level);
            windows->skillLevel->setRightAlign(windows->skillLevel, 1);
            for (i = 0; i < 6; i++) {
                value = totals.stats[STSTATUS_panelStats[i]] + data->battleStats[i];
                if (value >= 1000) {
                    value = 999;
                }
                windows->list[i]->setNumber(windows->list[i], 0, value);
                if (i == 0) {
                    if (totals.fields.lowered[0]) {
                        windows->list[0]->setPalette(windows->list[0], PALETTE_PURPLE);
                    }
                } else if (i == 1) {
                    if (totals.fields.lowered[1]) {
                        windows->list[1]->setPalette(windows->list[1], PALETTE_PURPLE);
                    }
                } else if (i == 4 && totals.fields.lowered[2]) {
                    windows->list[4]->setPalette(windows->list[4], PALETTE_PURPLE);
                }
            }
            for (i = 0; i < 7; i++) {
                value = totals.stats[STSTATUS_panelStats[i + 6]] + data->resistances[i];
                if (value >= 1000) {
                    value = 999;
                }
                windows->list[i + 6]->setNumber(windows->list[i + 6], 0, value);
            }
            for (i = 0; i < 13; i++) {
                windows->list[i]->setRightAlign(windows->list[i], 1);
            }
            for (i = 0; i < 6; i++) {
                tech = entry.skills[i];
                if (tech != 0) {
                    windows->values[i]->setString(windows->values[i], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), tech & SKILL_ID);
                    if (tech & SKILL_LAST) {
                        windows->values[i]->setPalette(windows->values[i], PALETTE_YELLOW);
                    } else if (tech & SKILL_MARKED) {
                        windows->values[i]->setPalette(windows->values[i], PALETTE_GREEN);
                    } else {
                        windows->values[i]->setPalette(windows->values[i], PALETTE_WHITE);
                    }
                } else {
                    windows->values[i]->setVisible(windows->values[i], 0);
                }
            }
        } else {
            data = &DIGIMON_DATA[id];
            GAME.funcs.computeStats(id, &totals);
            windows->entryName->setString(windows->entryName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            windows->skillLabel->setString(windows->skillLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x38);
            windows->skillLevel->setString(windows->skillLevel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0xD);
            windows->skillLevel->setRightAlign(windows->skillLevel, 1);
            for (i = 0; i < 6; i++) {
                value = totals.stats[STSTATUS_panelStats[i]];
                if (value >= 1000) {
                    value = 999;
                }
                windows->list[i]->setNumber(windows->list[i], 0, value);
                if (i == 0) {
                    if (totals.fields.lowered[0]) {
                        windows->list[0]->setPalette(windows->list[0], PALETTE_PURPLE);
                    }
                } else if (i == 1) {
                    if (totals.fields.lowered[1]) {
                        windows->list[1]->setPalette(windows->list[1], PALETTE_PURPLE);
                    }
                } else if (i == 4 && totals.fields.lowered[2]) {
                    windows->list[4]->setPalette(windows->list[4], PALETTE_PURPLE);
                }
            }
            for (i = 0; i < 7; i++) {
                value = totals.stats[STSTATUS_panelStats[i + 6]];
                if (value >= 1000) {
                    value = 999;
                }
                windows->list[i + 6]->setNumber(windows->list[i + 6], 0, value);
            }
            for (i = 0; i < 13; i++) {
                windows->list[i]->setRightAlign(windows->list[i], 1);
            }
            for (i = 0; i < 6; i++) {
                if (data->skills[i + 1] != 0) {
                    windows->values[i]->setString(windows->values[i], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), data->skills[i + 1]);
                    windows->values[i]->setPalette(windows->values[i], PALETTE_YELLOW);
                } else {
                    windows->values[i]->setVisible(windows->values[i], 0);
                }
            }
        }
        panel->listShown = 1;
    } else {
        windows->entryName->setVisible(windows->entryName, 0);
        windows->skillLabel->setVisible(windows->skillLabel, 0);
        windows->skillLevel->setVisible(windows->skillLevel, 0);
        for (i = 0; i < 13; i++) {
            windows->list[i]->setVisible(windows->list[i], 0);
        }
        for (i = 0; i < 6; i++) {
            windows->values[i]->setVisible(windows->values[i], 0);
        }
        panel->listShown = 0;
    }
}

/* Moves the list to the panel's scroll position */
void STSTATUS_scrollTechList(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    s32 i;

    /* the match depends on the (s16) casts in the loops */
    windows->entryName->setPos(windows->entryName, 0x4F, panel->scroll.value + 0x68);
    windows->skillLabel->setPos(windows->skillLabel, 0xEB, panel->scroll.value + 0x68);
    windows->skillLevel->setPos(windows->skillLevel, 0x12F, panel->scroll.value + 0x68);
    for (i = 0; i < 6; i++) {
        windows->list[i]->setPos(windows->list[i], 0x71, i * 14 + (s16)(panel->scroll.value + 0x7C));
    }
    for (i = 0; i < 7; i++) {
        windows->list[i + 6]->setPos(windows->list[i + 6], 0x9B, i * 14 + (s16)(panel->scroll.value + 0x7C));
    }
    for (i = 0; i < 6; i++) {
        windows->values[i]->setPos(windows->values[i], 0xC4, i * 14 + (s16)(panel->scroll.value + 0x88));
    }
}

/* Shows the cost of the technique under the list's cursor */
void STSTATUS_showTechCost(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show) {
    PartnerEntry entry;
    DigimonData *data;
    s32 id;
    s32 tech;

    if (show) {
        id = GAME.funcs.getPartyMember(panel->member);
        if (panel->fromEntry) {
            GAME.funcs.getPartnerEntry(id, panel->slots[panel->slot], &entry);
            tech = entry.skills[panel->tech] & SKILL_ID;
            if (tech > 0) {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), tech);
                windows->mpLabel->setString(windows->mpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 3);
                windows->mp->setNumber(windows->mp, 0, TECHS[tech - 1].mp);
                windows->mp->setRightAlign(windows->mp, 1);
                return;
            }
        } else {
            data = &DIGIMON_DATA[id];
            tech = data->skills[panel->tech + 1];
            if (tech > 0) {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), tech);
                windows->mpLabel->setString(windows->mpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 3);
                windows->mp->setNumber(windows->mp, 0, TECHS[tech - 1].mp);
                windows->mp->setRightAlign(windows->mp, 1);
                return;
            }
        }
    }
    windows->help->setVisible(windows->help, 0);
    windows->mpLabel->setVisible(windows->mpLabel, 0);
    windows->mp->setVisible(windows->mp, 0);
}

/* Draws the panel's frames, the technique icons and the help arrow */
void STSTATUS_drawDigivolvePanel(DigivolvePanel *panel) {
    SpriteDrawer sprite;
    PartnerEntry entry;
    DigimonData *data;
    s32 id;
    s32 tech;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(panel->layer, panel->depth);
    sprite.setTexture(0x280, 0x100);
    if (panel->fades[1].level != 0) {
        if (panel->fades[1].level != ONE) {
            sprite.setScale(panel->fades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x44);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x24, 0xA8, 0x28);
    }
    if (panel->fades[2].level != 0) {
        if (panel->fades[2].level != ONE) {
            sprite.setScale(panel->fades[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x41);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x26, 0xA8, 0x28);
    }
    if (panel->fades[3].level != 0) {
        if (panel->fades[3].level != ONE) {
            sprite.setScale(panel->fades[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0xA2);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x14, 0x4F, panel->scroll.value + 0x7C);
        if (panel->listShown) {
            id = GAME.funcs.getPartyMember(panel->member);
            if (panel->fromEntry) {
                GAME.funcs.getPartnerEntry(id, panel->slots[panel->slot], &entry);
                for (i = 0; i < 6; i++) {
                    tech = entry.skills[i] & SKILL_ID;
                    if (tech != 0) {
                        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), TECHS[tech - 1].icon + 0x37, 0xB6,
                                    panel->scroll.value + 0x88 + i * 14);
                    }
                }
            } else {
                data = &DIGIMON_DATA[id];
                for (i = 0; i < 6; i++) {
                    tech = data->skills[i + 1];
                    if (tech > 0) {
                        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), TECHS[tech - 1].icon + 0x37, 0xB6,
                                    panel->scroll.value + 0x88 + i * 14);
                    }
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x27, 0x48, panel->scroll.value + 0x61);
    }
    if (panel->fades[4].level != 0) {
        if (panel->blink) {
            if (GFX.funcs.getTime() - panel->time >= 4) {
                panel->time = GFX.funcs.getTime();
                panel->blinkFrame++;
                if (panel->blinkFrame >= 5) {
                    panel->blinkFrame = 0;
                }
            }
            sprite.setTexture(0x140, 0);
            sprite.setClutRow(panel->blinkFrame);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xA, 0x123, 0xD6);
            sprite.setClutRow(0);
        }
        if (panel->fades[4].level != ONE) {
            sprite.setScale(panel->fades[4].level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    if (panel->fades[0].level != 0) {
        if (panel->fades[0].level != ONE) {
            sprite.setScale(panel->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0xD);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
}

/* Once the title's panel is in, shows the title and starts the slots' panel */
static inline void STSTATUS_openDigivolveTitle(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[0])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x37);
        STSTATUS_data.funcs.startFade(&panel->fades[1], 1);
        panel->substate++;
    }
}

/* Once the slots' panel is in, shows the slots and starts the stats' panel */
static inline void STSTATUS_openDigivolveSlots(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
        STSTATUS_showDigivolveSlots(panel, windows, 1);
        STSTATUS_data.funcs.startFade(&panel->fades[3], 1);
        panel->substate++;
    }
}

/* Once the stats' panel is in, shows the partner's stats and the cursor on it */
static inline void STSTATUS_openDigivolveStats(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[3])) {
        STSTATUS_showDigivolveStats(panel, windows, 1);
        windows->slotCursor->setPos(windows->slotCursor, 0x50, 0x4D);
        windows->slotCursor->setVisible(windows->slotCursor, 1);
        panel->substate++;
    }
}

/* Choosing the partner or a slot: right and left move between them, up and
   down between the slots, cross asks about the slot (or shows the partner's
   own technique), triangle closes the panel */
static inline void STSTATUS_chooseDigivolveSlot(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    s32 slot;

    if (panel->fromEntry) {
        slot = panel->slot;
        if (PAD_PRESSED(PAD_LEFT)) {
            panel->fromEntry = 0;
            STSTATUS_showDigivolveStats(panel, windows, 1);
            windows->slotCursor->setPos(windows->slotCursor, 0x50, 0x4D);
            SOUND.playSound(SOUND_CURSOR);
            return;
        }
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            panel->slot--;
            if (panel->slot < 0) {
                panel->slot = 0;
            }
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            panel->slot++;
            if (panel->slot > panel->slotCount - 1) {
                panel->slot = panel->slotCount - 1;
            }
        }
        if (slot != panel->slot) {
            STSTATUS_showDigivolveStats(panel, windows, 1);
            windows->slotCursor->setPos(windows->slotCursor, 0xB0, panel->slot * 0xE + 0x31);
            SOUND.playSound(SOUND_CURSOR);
        } else if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            panel->substate = 10;
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            panel->substate = 0x32;
        }
    } else if (panel->slotCount > 0 && PAD_PRESSED(PAD_RIGHT)) {
        panel->fromEntry = 1;
        STSTATUS_showDigivolveStats(panel, windows, 1);
        windows->slotCursor->setPos(windows->slotCursor, 0xB0, panel->slot * 0xE + 0x31);
        SOUND.playSound(SOUND_CURSOR);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        panel->substate = 0x3C;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        panel->substate = 0x32;
    }
}

/* Starts closing the slots' panel for the question about the slot */
static inline void STSTATUS_closeSlotsForQuestion(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[1], 0);
    STSTATUS_showDigivolveSlots(panel, windows, 0);
    windows->slotCursor->setVisible(windows->slotCursor, 0);
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2C);
    panel->substate++;
}

/* Once the slots' panel is closed, starts the question's panel */
static inline void STSTATUS_openDigivolveQuestion(DigivolvePanel *panel) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
        STSTATUS_data.funcs.startFade(&panel->fades[2], 1);
        panel->substate++;
    }
}

/* Once the question's panel is in, shows the question and its cursor */
static inline void STSTATUS_enterDigivolveQuestion(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[2])) {
        STSTATUS_showDigivolveChoices(panel, windows, 1);
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        panel->substate++;
    }
}

/* The question: up and down move the cursor, cross makes the slot's entry
   the partner's battle digivolution (or no longer) or shows its techniques,
   triangle goes back to the slots */
static inline void STSTATUS_answerDigivolveQuestion(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    Partner *partner;
    s32 option;

    option = panel->option;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        panel->option = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        panel->option = 1;
    }
    if (option != panel->option) {
        SOUND.playSound(SOUND_CURSOR);
        windows->optionCursor->setPos(windows->optionCursor, 0xB4, panel->option * 0xE + 0x3A);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (panel->option == 0) {
            partner = &GAME.partners[GAME.funcs.getPartyMember(panel->member)];
            if (partner->battleDigivolve == panel->slots[panel->slot]) {
                partner->battleDigivolve = 0;
            } else {
                partner->battleDigivolve = panel->slots[panel->slot];
            }
            panel->substate = 0x19;
        } else {
            panel->substate = 0x28;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        panel->substate = 0x14;
    }
}

/* Starts closing the question's panel, back to the slots */
static inline void STSTATUS_closeDigivolveQuestion(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[2], 0);
    STSTATUS_showDigivolveChoices(panel, windows, 0);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x37);
    panel->substate++;
}

/* Once the question's panel is closed, starts the slots' panel */
static inline void STSTATUS_reopenDigivolveSlots(DigivolvePanel *panel) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[2])) {
        STSTATUS_data.funcs.startFade(&panel->fades[1], 1);
        panel->substate++;
    }
}

/* Starts closing the title's and the question's panels for the battle
   digivolution's message */
static inline void STSTATUS_closeQuestionForBattle(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[0], 0);
    windows->title->setVisible(windows->title, 0);
    STSTATUS_data.funcs.startFade(&panel->fades[2], 0);
    STSTATUS_showDigivolveChoices(panel, windows, 0);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    panel->substate++;
}

/* Once the question's panel is closed, starts scrolling the stats up */
static inline void STSTATUS_scrollUpForBattle(DigivolvePanel *panel) {
    STSTATUS_data.funcs.updateFade(&panel->fades[0]);
    if (STSTATUS_data.funcs.updateFade(&panel->fades[2])) {
        STSTATUS_data.funcs.startLerp(&panel->scroll, 0, -0x22, 4);
        panel->substate++;
    }
}

/* Scrolls the stats; once up, starts the info panel */
static inline void STSTATUS_openBattleMessage(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateLerp(&panel->scroll)) {
        STSTATUS_data.funcs.startFade(&panel->fades[4], 1);
        panel->substate++;
    }
    STSTATUS_scrollTechList(panel, windows);
}

/* Once the info panel is in, says whether the partner now digivolves in
   battle */
static inline void STSTATUS_showBattleMessage(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    Partner *partner;

    if (STSTATUS_data.funcs.updateFade(&panel->fades[4])) {
        partner = &GAME.partners[GAME.funcs.getPartyMember(panel->member)];
        if (partner->battleDigivolve != 0) {
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3C);
        } else {
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3D);
        }
        panel->blink = 1;
        panel->substate++;
    }
}

/* Cross takes the message away */
static inline void STSTATUS_waitBattleMessage(DigivolvePanel *panel) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        panel->blink = 0;
        panel->substate++;
    }
}

/* Starts closing the info panel and the stats' panel */
static inline void STSTATUS_closeBattleMessage(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[4], 0);
    windows->help->setVisible(windows->help, 0);
    STSTATUS_data.funcs.startFade(&panel->fades[3], 0);
    STSTATUS_showDigivolveStats(panel, windows, 0);
    panel->substate++;
}

/* Once the stats' panel is closed, scrolls the stats back and starts the
   panel again */
static inline void STSTATUS_endBattleMessage(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.updateFade(&panel->fades[4]);
    if (STSTATUS_data.funcs.updateFade(&panel->fades[3])) {
        panel->fromEntry = 0;
        STSTATUS_data.funcs.startLerp(&panel->scroll, 0, -0x22, 4);
        STSTATUS_scrollTechList(panel, windows);
        panel->substate = 0;
    }
}

/* Starts closing the question's panel for the entry's techniques */
static inline void STSTATUS_closeQuestionForTechs(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[2], 0);
    STSTATUS_showDigivolveChoices(panel, windows, 0);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2A);
    panel->substate++;
}

/* Once the question's panel is closed, starts scrolling the stats up */
static inline void STSTATUS_scrollUpForTechs(DigivolvePanel *panel) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[2])) {
        STSTATUS_data.funcs.startLerp(&panel->scroll, 0, -0x22, 4);
        panel->substate++;
    }
}

/* Scrolls the stats; once up, starts the info panel */
static inline void STSTATUS_openEntryTechs(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateLerp(&panel->scroll)) {
        STSTATUS_data.funcs.startFade(&panel->fades[4], 1);
        panel->substate++;
    }
    STSTATUS_scrollTechList(panel, windows);
}

/* Starts closing the info panel and scrolling the stats back */
static inline void STSTATUS_closeEntryTechs(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2C);
    windows->listCursor->setVisible(windows->listCursor, 0);
    STSTATUS_showTechCost(panel, windows, 0);
    STSTATUS_data.funcs.startLerp(&panel->scroll, -0x22, 0, 4);
    STSTATUS_data.funcs.startFade(&panel->fades[4], 0);
    panel->substate++;
}

/* Scrolls the stats back; once the info panel is closed, starts the
   question's panel */
static inline void STSTATUS_reopenDigivolveQuestion(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.updateLerp(&panel->scroll);
    if (STSTATUS_data.funcs.updateFade(&panel->fades[4])) {
        STSTATUS_data.funcs.startFade(&panel->fades[2], 1);
        panel->substate++;
    }
    STSTATUS_scrollTechList(panel, windows);
}

/* Once the question's panel is in, shows the question and its cursor again */
static inline void STSTATUS_enterQuestionAgain(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[2])) {
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        STSTATUS_showDigivolveChoices(panel, windows, 1);
        panel->substate = 13;
    }
}

/* Starts closing the panel: the title, the slots and the stats */
static inline void STSTATUS_closeDigivolvePanel(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[0], 0);
    STSTATUS_data.funcs.startFade(&panel->fades[1], 0);
    STSTATUS_data.funcs.startFade(&panel->fades[3], 0);
    STSTATUS_showDigivolveSlots(panel, windows, 0);
    STSTATUS_showDigivolveStats(panel, windows, 0);
    windows->slotCursor->setVisible(windows->slotCursor, 0);
    windows->title->setVisible(windows->title, 0);
    panel->substate++;
}

/* Ends the panel once its panels are closed */
static inline void STSTATUS_endDigivolvePanel(DigivolvePanel *panel) {
    STSTATUS_data.funcs.updateFade(&panel->fades[0]);
    STSTATUS_data.funcs.updateFade(&panel->fades[1]);
    if (STSTATUS_data.funcs.updateFade(&panel->fades[3])) {
        panel->state = 3;
    }
}

/* Starts closing the slots' panel for the partner's own technique */
static inline void STSTATUS_closeSlotsForPartnerTech(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.startFade(&panel->fades[1], 0);
    STSTATUS_showDigivolveSlots(panel, windows, 0);
    windows->slotCursor->setVisible(windows->slotCursor, 0);
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2A);
    panel->substate++;
}

/* Once the slots' panel is closed, starts scrolling the stats up */
static inline void STSTATUS_scrollUpForPartnerTech(DigivolvePanel *panel) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
        STSTATUS_data.funcs.startLerp(&panel->scroll, 0, -0x22, 4);
        panel->substate++;
    }
}

/* Scrolls the stats; once up, starts the info panel */
static inline void STSTATUS_openPartnerTech(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateLerp(&panel->scroll)) {
        STSTATUS_data.funcs.startFade(&panel->fades[4], 1);
        panel->substate++;
    }
    STSTATUS_scrollTechList(panel, windows);
}

/* Once the info panel is in, puts the cursor on the partner's own technique
   (its seventh skill) and shows its description and MP cost */
static inline void STSTATUS_showPartnerTech(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    DigimonData *data;
    s32 tech;

    if (STSTATUS_data.funcs.updateFade(&panel->fades[4])) {
        data = &DIGIMON_DATA[GAME.funcs.getPartyMember(panel->member)];
        tech = data->skills[6];
        panel->tech = 5;
        windows->listCursor->setPos(windows->listCursor, 0xA9, panel->scroll.value + 0xCE);
        windows->listCursor->setVisible(windows->listCursor, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), tech);
        windows->mpLabel->setString(windows->mpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 3);
        windows->mp->setNumber(windows->mp, 0, TECHS[tech - 1].mp);
        windows->mp->setRightAlign(windows->mp, 1);
        panel->substate++;
    }
}

/* Cross or triangle go back */
static inline void STSTATUS_waitPartnerTech(DigivolvePanel *panel) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        panel->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        panel->substate++;
    }
}

/* Starts closing the info panel and scrolling the stats back */
static inline void STSTATUS_closePartnerTech(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x37);
    windows->listCursor->setVisible(windows->listCursor, 0);
    STSTATUS_showTechCost(panel, windows, 0);
    STSTATUS_data.funcs.startLerp(&panel->scroll, -0x22, 0, 4);
    STSTATUS_data.funcs.startFade(&panel->fades[4], 0);
    panel->substate++;
}

/* Scrolls the stats back; once the info panel is closed, starts the slots'
   panel */
static inline void STSTATUS_reopenSlotsAfterTech(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    STSTATUS_data.funcs.updateLerp(&panel->scroll);
    if (STSTATUS_data.funcs.updateFade(&panel->fades[4])) {
        STSTATUS_data.funcs.startFade(&panel->fades[1], 1);
        panel->substate++;
    }
    STSTATUS_scrollTechList(panel, windows);
}

/* Once the slots' panel is in, shows the slots and the cursor, back to
   choosing a slot */
static inline void STSTATUS_enterSlotsAgain(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&panel->fades[1])) {
        STSTATUS_showDigivolveSlots(panel, windows, 1);
        windows->slotCursor->setVisible(windows->slotCursor, 1);
        panel->substate = 4;
    }
}

/* The panel's update: picks a slot, then shows its entry's techniques or
   makes the battle start as it */
void STSTATUS_runDigivolvePanel(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    PartnerEntry entry;
    s32 old;
    s32 i;

    switch (panel->substate) {
    case 0:
    default:
        STSTATUS_data.funcs.startFade(&panel->fades[0], 1);
        panel->substate++;
        break;
    case 1:
        STSTATUS_openDigivolveTitle(panel, windows);
        break;
    case 2:
        STSTATUS_openDigivolveSlots(panel, windows);
        break;
    case 3:
        STSTATUS_openDigivolveStats(panel, windows);
        break;
    case 4:
        STSTATUS_chooseDigivolveSlot(panel, windows);
        break;
    case 10:
        STSTATUS_closeSlotsForQuestion(panel, windows);
        break;
    case 11:
        STSTATUS_openDigivolveQuestion(panel);
        break;
    case 12:
        STSTATUS_enterDigivolveQuestion(panel, windows);
        break;
    case 13:
        STSTATUS_answerDigivolveQuestion(panel, windows);
        break;
    case 0x14:
        STSTATUS_closeDigivolveQuestion(panel, windows);
        break;
    case 0x15:
        STSTATUS_reopenDigivolveSlots(panel);
        break;
    case 0x19:
        STSTATUS_closeQuestionForBattle(panel, windows);
        break;
    case 0x1A:
        STSTATUS_scrollUpForBattle(panel);
        break;
    case 0x1B:
        STSTATUS_openBattleMessage(panel, windows);
        break;
    case 0x1C:
        STSTATUS_showBattleMessage(panel, windows);
        break;
    case 0x1D:
        STSTATUS_waitBattleMessage(panel);
        break;
    case 0x1E:
        STSTATUS_closeBattleMessage(panel, windows);
        break;
    case 0x1F:
        STSTATUS_endBattleMessage(panel, windows);
        break;
    case 0x28:
        STSTATUS_closeQuestionForTechs(panel, windows);
        break;
    case 0x29:
        STSTATUS_scrollUpForTechs(panel);
        break;
    case 0x2A:
        STSTATUS_openEntryTechs(panel, windows);
        break;
    case 0x2B:
        if (STSTATUS_data.funcs.updateFade(&panel->fades[4])) {
            GAME.funcs.getPartnerEntry(GAME.funcs.getPartyMember(panel->member), panel->slots[panel->slot], &entry);
            panel->tech = -1;
            panel->techCount = 0;
            for (i = 0; i < 6; i++) {
                if (entry.skills[i] & SKILL_ID) {
                    panel->techCount++;
                    if (panel->tech == -1) {
                        panel->tech = i;
                    }
                }
            }
            if (panel->techCount != 0) {
                /* the match depends on this order of the terms */
                windows->listCursor->setPos(windows->listCursor, 0xA9, panel->tech * 0xE + 0x88 + panel->scroll.value);
                windows->listCursor->setVisible(windows->listCursor, 1);
                STSTATUS_showTechCost(panel, windows, 1);
                panel->substate++; /* in both branches: the match depends on it */
            } else {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x3E);
                panel->substate++;
            }
        }
        break;
    case 0x2C:
        if (panel->techCount != 0) {
            old = panel->tech;
            GAME.funcs.getPartnerEntry(GAME.funcs.getPartyMember(panel->member), panel->slots[panel->slot], &entry);
            if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
                do {
                    panel->tech--;
                    if (panel->tech < 0) {
                        panel->tech = old;
                        break;
                    }
                } while ((entry.skills[panel->tech] & SKILL_ID) <= 0);
            } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
                do {
                    panel->tech++;
                    if (panel->tech >= 6) {
                        panel->tech = old;
                        break;
                    }
                } while ((entry.skills[panel->tech] & SKILL_ID) <= 0);
            }
            if (old != panel->tech) {
                SOUND.playSound(SOUND_CURSOR);
                windows->listCursor->setPos(windows->listCursor, 0xA9, panel->tech * 0xE + 0x88 + panel->scroll.value);
                STSTATUS_showTechCost(panel, windows, 1);
            }
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            panel->substate++;
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            panel->substate++;
        }
        break;
    case 0x2D:
        STSTATUS_closeEntryTechs(panel, windows);
        break;
    case 0x2E:
        STSTATUS_reopenDigivolveQuestion(panel, windows);
        break;
    case 0x2F:
        STSTATUS_enterQuestionAgain(panel, windows);
        break;
    case 0x32:
        STSTATUS_closeDigivolvePanel(panel, windows);
        break;
    case 0x33:
        STSTATUS_endDigivolvePanel(panel);
        break;
    case 0x3C:
        STSTATUS_closeSlotsForPartnerTech(panel, windows);
        break;
    case 0x3D:
        STSTATUS_scrollUpForPartnerTech(panel);
        break;
    case 0x3E:
        STSTATUS_openPartnerTech(panel, windows);
        break;
    case 0x3F:
        STSTATUS_showPartnerTech(panel, windows);
        break;
    case 0x40:
        STSTATUS_waitPartnerTech(panel);
        break;
    case 0x41:
        STSTATUS_closePartnerTech(panel, windows);
        break;
    case 0x42:
        STSTATUS_reopenSlotsAfterTech(panel, windows);
        break;
    case 0x16:
    case 0x43:
        STSTATUS_enterSlotsAgain(panel, windows);
        break;
    }
}

/* The digivolution panel's task: creates its windows and counts the partner's
   filled slots, then runs and draws the panel */
void STSTATUS_updateDigivolvePanel(DigivolvePanel *panel, DigivolvePanelWindows *windows) {
    s32 i;

    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        STSTATUS_createDigivolveWindows(panel, windows);
        GAME.funcs.getPartnerSlots(GAME.funcs.getPartyMember(panel->member), panel->slots);
        for (i = 0; i < 3; i++) {
            if (panel->slots[i] >= 4) {
                panel->slotCount++;
            }
        }
        panel->fades[0].duration = 10;
        panel->fades[1].duration = 10;
        panel->fades[2].duration = 10;
        panel->fades[3].duration = 10;
        panel->fades[4].duration = 10;
        break;
    case TASK_RUN:
        STSTATUS_runDigivolvePanel(panel, windows);
        STSTATUS_drawDigivolvePanel(panel);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the fifth screen's digivolution panel (task) for its chosen party
   member */
DigivolvePanel *STSTATUS_createDigivolvePanel(StatsScreen *screen) {
    DigivolvePanel *panel = createTask(STSTATUS_updateDigivolvePanel, sizeof(DigivolvePanel), sizeof(DigivolvePanelWindows));

    panel->layer = SCREEN_LAYER;
    panel->depth = 6;
    panel->screen = screen;
    panel->member = screen->member;
    return panel;
}

/* The stats the fifth screen's panel shows: six battle stats, then the
   resistances */
s32 STSTATUS_panelStats[] = {
    STAT_STRENGTH, STAT_DEFENSE, STAT_SPIRIT, STAT_WISDOM,
    STAT_SPEED, STAT_CHARISMA, STAT_RESISTS, STAT_RESISTS + 1,
    STAT_RESISTS + 2, STAT_RESISTS + 3, STAT_RESISTS + 4, STAT_RESISTS + 5,
    STAT_RESISTS + 6,
};
