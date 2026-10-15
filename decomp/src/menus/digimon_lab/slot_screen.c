/* The main menu's second screen, which sets a party member's slots: it opens
   the list of the member's entries, then the entry panel or the skill panel */

#include "menus/digimon_lab.h"

/* Creates the second screen's windows and cursor */
void STGDGLAB_createSlotScreenWindows(LabSlotScreen *screen, LabSlotScreenWindows *windows) {
    s32 i;
    TextWindow **items;

    windows->title = createTextWindow(screen->layer, 1, 0xAE, 0x57);
    for (i = 0; i < 3; i++) {
        windows->slotNames[i] = createTextWindow(screen->layer, 1, 0xA7, i * 0xE + 0x16);
    }
    for (i = 0; i < 2; i++) {
        windows->options[i] = createTextWindow(screen->layer, 1, 0xA7, i * 0xE + 0x73);
    }
    for (i = 0; i < 6; i++) {
        windows->list[i] = createTextWindow(screen->layer, 1, 0x32, i * 0xE + 0x4F);
    }
    for (i = 0; i < 7; i++) {
        windows->list[i + 6] = createTextWindow(screen->layer, 1, 0x5B, i * 0xE + 0x4F);
    }
    for (i = 0, items = screen->children; i < screen->childCount - 4; i++, items++) {
        if (*items != NULL) {
            (*items)->setDepth(*items, screen->depth - 1);
        }
    }
    windows->cursor = createCursor(screen->layer, screen->depth - 1, 0x9A, 0x73);
    windows->cursor->setVisible(windows->cursor, 0);
}

/* Shows the party member's slots and stats, or hides the screen's windows */
void STGDGLAB_showSlotScreen(LabSlotScreen *screen, LabSlotScreenWindows *windows, s32 show) {
    PartnerTotals totals;
    TextWindow **items;
    Partner *partner;
    s32 member;
    s32 value;
    s32 i;
    s16 *stats;

    if (show) {
        member = GAME.funcs.getPartyMember(screen->lab->member);
        GAME.funcs.computeStats(member, &totals);
        partner = &GAME.partners[member];
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 1);
        for (i = 0; i < 3; i++) {
            if (screen->slots[i] < 2) {
                if (i == 0) {
                    windows->slotNames[0]->setString(windows->slotNames[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1B);
                } else {
                    windows->slotNames[i]->setVisible(windows->slotNames[i], 0);
                }
            } else {
                windows->slotNames[i]->setString(windows->slotNames[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)),
                                             GET_DIGIMON(screen->slots[i])->nameId);
                if (partner->battleDigivolve == screen->slots[i]) {
                    windows->slotNames[i]->setPalette(windows->slotNames[i], PALETTE_BLUE);
                } else {
                    windows->slotNames[i]->setPalette(windows->slotNames[i], PALETTE_WHITE);
                }
            }
        }
        for (i = 0; i < 2; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), i + 0x21);
        }
        stats = totals.stats;
        for (i = 0; i < 13; i++) {
            value = stats[i + 6];
            if (value >= 1000) {
                value = 999;
            }
            windows->list[i]->setNumber(windows->list[i], 0, value);
        }
        if (totals.fields.lowered[0] != 0) {
            windows->list[0]->setPalette(windows->list[0], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[1] != 0) {
            windows->list[1]->setPalette(windows->list[1], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[2] != 0) {
            windows->list[4]->setPalette(windows->list[4], PALETTE_PURPLE);
        }
        for (i = 0; i < 13; i++) {
            windows->list[i]->setRightAlign(windows->list[i], 1);
        }
        windows->cursor->setVisible(windows->cursor, 1);
    } else {
        for (i = 0, items = screen->children; i < screen->childCount - 4; i++, items++) {
            if (*items != NULL) {
                (*items)->setVisible(*items, 0);
            }
        }
        windows->cursor->setVisible(windows->cursor, 0);
    }
}

/* Reads the party member's slots and entries and fades the screen's panels in */
static inline void loadSlotScreen(LabSlotScreen *screen) {
    s32 member;

    member = GAME.funcs.getPartyMember(screen->lab->member);
    GAME.funcs.getPartnerSlots(member, screen->slots);
    screen->entryCount = GAME.funcs.listPartnerEntries(member, screen->entries);
    STGDGLAB_data.funcs.startFade(&screen->panels[0], 1);
    STGDGLAB_data.funcs.startFade(&screen->panels[1], 1);
    STGDGLAB_data.funcs.startFade(&screen->panels[2], 1);
    STGDGLAB_data.funcs.startFade(&screen->panels[3], 1);
    screen->substate++;
}

/* Up and down pick "Change digivolve type" or "Load technique"; cross opens
   the entry list (or says it can't, without enough entries), triangle leaves */
static inline void pickSlotOption(LabSlotScreen *screen, LabSlotScreenWindows *windows) {
    s32 old;

    old = screen->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->choice--;
        if (screen->choice < 0) {
            screen->choice = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->choice++;
        if (screen->choice >= 2) {
            screen->choice = 1;
        }
    }
    if (old != screen->choice) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, 0x9A, screen->choice * 0xE + 0x72);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if ((screen->choice == 0 && screen->entryCount < 4) || (screen->choice == 1 && screen->entryCount == 0)) {
            screen->setSubstate(screen, 30);
            STGDGLAB_data.funcs.startFade(&screen->panels[4], 1);
            windows->cursor->setVisible(windows->cursor, 0);
        } else {
            screen->step = 0;
            screen->substate++;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->step = 1;
        screen->substate++;
    }
}

/* Once the screen's panels have faded out, goes on to the entry list for the
   picked option, or ends the screen */
static inline void openSlotOption(LabSlotScreen *screen) {
    STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
    STGDGLAB_data.funcs.updateFade(&screen->panels[1]);
    STGDGLAB_data.funcs.updateFade(&screen->panels[2]);
    if (STGDGLAB_data.funcs.updateFade(&screen->panels[3])) {
        if (screen->step == 0) {
            screen->picked = 0;
            screen->setSubstate(screen, 40);
            switch (screen->choice) {
            case 0:
            default:
                screen->step = 10;
                break;
            case 1:
                screen->step = 20;
                break;
            case 2:
                screen->setState(screen, TASK_KILL);
                break;
            }
        } else {
            screen->setState(screen, TASK_KILL);
        }
    }
}

/* Cross closes the message: fades its panel out and hides it */
static inline void closeMessage(LabSlotScreen *screen, LabSlotScreenWindows *windows, s32 panel) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        STGDGLAB_data.funcs.startFade(&screen->panels[panel], 0);
        windows->message->setVisible(windows->message, 0);
        screen->substate++;
    }
}

/* The second screen's states: picks between the entries and the skills,
   opens the list of the member's entries and then the picked one's panel */
void STGDGLAB_runSlotScreen(LabSlotScreen *screen, LabSlotScreenWindows *windows) {
    s16 slots[4];
    PartnerEntry entry;
    s32 found;
    s32 i;

    switch (screen->substate) {
    case 0:
    default:
        loadSlotScreen(screen);
        break;
    case 1:
        STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
        STGDGLAB_data.funcs.updateFade(&screen->panels[1]);
        STGDGLAB_data.funcs.updateFade(&screen->panels[2]);
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[3])) {
            STGDGLAB_showSlotScreen(screen, windows, 1);
            screen->substate++;
        }
        break;
    case 2:
        pickSlotOption(screen, windows);
        break;
    case 3:
        STGDGLAB_data.funcs.startFade(&screen->panels[0], 0);
        STGDGLAB_data.funcs.startFade(&screen->panels[1], 0);
        STGDGLAB_data.funcs.startFade(&screen->panels[2], 0);
        STGDGLAB_data.funcs.startFade(&screen->panels[3], 0);
        STGDGLAB_showSlotScreen(screen, windows, 0);
        screen->substate++;
        break;
    case 4:
        openSlotOption(screen);
        break;
    case 40:
        if (windows->entryList == NULL) {
            windows->entryList = STGDGLAB_createEntryList(GAME.funcs.getPartyMember(screen->lab->member), 0, 0);
        }
        windows->entryList->cursor = screen->picked;
        screen->substate++;
        break;
    case 41:
        if (windows->entryList == NULL || windows->entryList->substate >= 2) {
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                if (screen->step == 20) {
                    found = GAME.funcs.getPartyMember(screen->lab->member);
                    GAME.funcs.getPartnerSlots(found, slots);
                    GAME.funcs.getPartnerEntry(found, slots[windows->entryList->cursor], &entry);
                    found = 0; /* the match depends on reusing the member's variable */
                    for (i = 0; i < 6; i++) {
                        if (entry.skills[i] != 0) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        screen->setSubstate(screen, 50);
                        STGDGLAB_data.funcs.startFade(&screen->panels[5], 1);
                        windows->cursor->setVisible(windows->cursor, 0);
                        windows->entryList->substate = 4;
                        break;
                    }
                }
                screen->picked = windows->entryList->cursor;
                screen->substate = screen->step;
                screen->setStep(screen, 0);
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                windows->entryList->close(windows->entryList);
                screen->substate++;
            }
        }
        break;
    case 42:
        if (windows->entryList == NULL) {
            screen->setSubstate(screen, 0);
        }
        break;
    case 10:
        screen->lab->closeMenu(screen->lab);
        windows->entryList->close(windows->entryList);
        screen->substate++;
        break;
    case 11:
        if (screen->lab->menuOpen(screen->lab) && windows->entryList == NULL) {
            if (windows->entryPanel == NULL) {
                windows->entryPanel = STGDGLAB_createEntryPanel(GAME.funcs.getPartyMember(screen->lab->member), screen->picked);
            }
            screen->substate++;
        }
        break;
    case 12:
        if (windows->entryPanel == NULL) {
            screen->lab->openMenu(screen->lab);
            screen->setSubstate(screen, 40);
            screen->step = 10;
        }
        break;
    case 20:
        windows->entryList->close(windows->entryList);
        screen->substate++;
        break;
    case 21:
        if (windows->entryList == NULL) {
            if (windows->skillsPanel == NULL) {
                windows->skillsPanel = STGDGLAB_createSkillPanel(GAME.funcs.getPartyMember(screen->lab->member), screen->picked);
            }
            screen->substate++;
        }
        break;
    case 22:
        if (windows->skillsPanel == NULL) {
            screen->setSubstate(screen, 40);
            screen->step = 20;
        }
        break;
    case 30:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4])) {
            if (windows->message == NULL) {
                windows->message = createTextWindow(screen->layer, 1, 0xA4, 0xB8);
            }
            windows->message->setDepth(windows->message, screen->depth - 1);
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), screen->choice + 0x24);
            screen->substate++;
        }
        break;
    case 31:
        closeMessage(screen, windows, 4);
        break;
    case 32:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4])) {
            windows->cursor->setVisible(windows->cursor, 1);
            screen->setSubstate(screen, 2);
        }
        break;
    case 50:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[5])) {
            if (windows->message == NULL) {
                windows->message = createTextWindow(screen->layer, 1, 0xA4, 0xB8);
            }
            windows->message->setDepth(windows->message, 0);
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x25);
            screen->substate++;
        }
        break;
    case 51:
        closeMessage(screen, windows, 5);
        break;
    case 52:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[5])) {
            screen->setSubstate(screen, 41);
            screen->step = 20;
            windows->entryList->substate = 5;
        }
        break;
    }
}

/* Draws the second screen's frames */
void STGDGLAB_drawSlotScreen(LabSlotScreen *screen, void *children) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    if (screen->panels[5].level != 0) {
        sprite.setLayerId(screen->layer, 1);
        if (screen->panels[5].level != ONE) {
            sprite.setScale(screen->panels[5].level, ONE, ONE);
            sprite.setPivot(0x140, 0xBE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x25, 0x92, 0xB0);
    }
    if (screen->panels[0].level != ONE) {
        sprite.setScale(screen->panels[0].level, ONE, ONE);
        sprite.setPivot(0, 0x7F);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.setTexture(0x140, 0);
    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xF, 0, 0x4B);
    sprite.setTexture(0x280, 0x100);
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x1F, 0, 0x4B);
    if (screen->panels[1].level != ONE) {
        sprite.setScale(screen->panels[1].level, ONE, ONE);
        sprite.setPivot(0x140, 0x2A);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x22, 0x92, 0xF);
    if (screen->panels[2].level != ONE) {
        sprite.setScale(screen->panels[3].level, ONE, ONE);
        sprite.setPivot(0x140, 0x5C);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x20, 0x92, 0x51);
    if (screen->panels[3].level != ONE) {
        sprite.setScale(screen->panels[3].level, ONE, ONE);
        sprite.setPivot(0x140, 0x87);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x23, 0x92, 0x6C);
    if (screen->panels[4].level != 0) {
        if (screen->panels[4].level != ONE) {
            sprite.setScale(screen->panels[4].level, ONE, ONE);
            sprite.setPivot(0x140, 0xBE);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x25, 0x92, 0xB0);
    }
}

/* The second screen's task: creates its windows, then runs and draws it */
void STGDGLAB_updateSlotScreen(LabSlotScreen *screen, void *children) {
    switch (screen->state) {
    case TASK_INIT:
    default:
        screen->nextState(screen);
        screen->panels[0].duration = 8;
        screen->panels[1].duration = 10;
        screen->panels[2].duration = 10;
        screen->panels[3].duration = 10;
        screen->panels[4].duration = 10;
        screen->panels[5].duration = 8;
        STGDGLAB_createSlotScreenWindows(screen, children);
        break;
    case TASK_RUN:
        STGDGLAB_runSlotScreen(screen, children);
        STGDGLAB_drawSlotScreen(screen, children);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the main menu's second screen (task), the party member's three slots */
Task *STGDGLAB_createSlotScreen(Lab *lab) {
    LabSlotScreen *screen = createTask(STGDGLAB_updateSlotScreen, sizeof(LabSlotScreen), 0x60);

    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->lab = lab;
    return (Task *)screen;
}
