/* The second screen's skill panel, where a slot's Digimon's skills are marked */

#include "menus/digimon_lab.h"

/* Creates the panel's windows and cursors */
void STGDGLAB_createSkillPanelWindows(LabSkillPanel *panel, LabSkillPanelWindows *windows) {
    s32 i;
    TextWindow **items;

    windows->digimonName = createTextWindow(panel->layer, 1, 0x9A, 0x46);
    windows->learnedLabel = createTextWindow(panel->layer, 1, 0x122, 0x46);
    windows->learned = createTextWindow(panel->layer, 1, 0x121, 0x46);
    for (i = 0; i < 6; i++) {
        windows->left[i] = createTextWindow(panel->layer, 1, 0x6B, i * 0x11 + 0x58);
        windows->right[i] = createTextWindow(panel->layer, 1, 0xF1, i * 0x11 + 0x58);
    }
    windows->message = createTextWindow(panel->layer, 1, 0x9A, 0x1A);
    windows->yes = createTextWindow(panel->layer, 1, 0xC3, 0x38);
    windows->no = createTextWindow(panel->layer, 1, 0xC3, 0x48);
    windows->help = createTextWindow(panel->layer, 1, 0x14, 0xC3);
    windows->help->setLines(windows->help, 2);
    windows->mpLabel = createTextWindow(panel->layer, 1, 0x109, 0xD1);
    windows->mp = createTextWindow(panel->layer, 1, 0x12C, 0xD1);
    for (i = 0, items = panel->children; i < panel->childCount - 2; i++, items++) {
        if (*items != NULL) {
            (*items)->setDepth(*items, panel->depth - 1);
        }
    }
    windows->yes->setDepth(windows->yes, 0);
    windows->no->setDepth(windows->no, 0);
    windows->optionCursor = createCursor(panel->layer, 0, 0xB8, 0x38);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    windows->cursor = createCursor(panel->layer, panel->depth - 1, 0x4C, 0x58);
    windows->cursor->setVisible(windows->cursor, 0);
}

/* Shows the slot's Digimon's skills, each marked or not and in the palette of
   marked, last or unknown ones, how many are marked, and the cursor's skill's
   description and MP; or hides the panel's windows */
void STGDGLAB_showSkillPanel(LabSkillPanel *panel, LabSkillPanelWindows *windows, s32 show) {
    TextWindow **items;
    u16 skill;
    s32 id;
    s32 i;

    if (show) {
        if (panel->entry.id >= 3) {
            windows->digimonName->setString(windows->digimonName, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)),
                                     GET_DIGIMON(panel->entry.id)->nameId);
            panel->learned = 0;
            for (i = 0; i < panel->skillCount; i++) {
                skill = panel->entry.skills[i];
                if (panel->entry.skills[i] != 0) {
                    windows->left[i]->setString(windows->left[i], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), skill & SKILL_ID);
                    if (skill & SKILL_MARKED) {
                        windows->right[i]->setString(windows->right[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x11);
                        panel->learned++;
                    } else {
                        windows->right[i]->setString(windows->right[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x12);
                    }
                    if (skill & SKILL_MARKED) {
                        windows->left[i]->setPalette(windows->left[i], PALETTE_GREEN);
                        windows->right[i]->setPalette(windows->right[i], PALETTE_GREEN);
                    } else if (skill & SKILL_LAST) {
                        windows->left[i]->setPalette(windows->left[i], PALETTE_YELLOW);
                        windows->right[i]->setPalette(windows->right[i], PALETTE_YELLOW);
                    } else if (!(skill & SKILL_KNOWN)) {
                        windows->left[i]->setPalette(windows->left[i], PALETTE_GREY);
                        windows->right[i]->setPalette(windows->right[i], PALETTE_GREY);
                    } else {
                        windows->left[i]->setPalette(windows->left[i], PALETTE_WHITE);
                        windows->right[i]->setPalette(windows->right[i], PALETTE_WHITE);
                    }
                } else {
                    windows->left[i]->setVisible(windows->left[i], 0);
                    windows->right[i]->setVisible(windows->right[i], 0);
                }
            }
            windows->learnedLabel->setString(windows->learnedLabel, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x10);
            windows->learned->setNumber(windows->learned, 0, panel->learned);
            windows->learned->setRightAlign(windows->learned, 1);
            id = panel->entry.skills[panel->cursor] & SKILL_ID;
            if (id != 0) {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), id);
                windows->mpLabel->setString(windows->mpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xE);
                windows->mp->setNumber(windows->mp, 0, TECHS[id - 1].mp);
                windows->mp->setRightAlign(windows->mp, 1);
            } else {
                windows->help->setVisible(windows->help, 0);
                windows->mpLabel->setVisible(windows->mpLabel, 0);
                windows->mp->setVisible(windows->mp, 0);
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

/* The panel of a partner's Digimon's skills: up and down pick one, cross
   marks it to pass on or unmarks it, after a yes/no, or says why it can't be
   marked, and triangle saves the entry and closes the panel */
void STGDGLAB_updateSkillPanel(LabSkillPanel *panel, LabSkillPanelWindows *windows) {
    SpriteDrawer sprite;
    s32 skill;
    s32 id;
    s32 old;
    s32 i;

    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        panel->fade.duration = 10;
        panel->confirm.duration = 10;
        STGDGLAB_data.funcs.startFade(&panel->fade, 1);
        GAME.funcs.getPartnerSlots(panel->member, panel->slots);
        GAME.funcs.getPartnerEntry(panel->member, panel->slots[panel->slot], &panel->entry);
        /* BEC form: the match needs the store in a block of its own, whose loop
           notes keep it before the call's arguments past sched2; as in Digimon
           World 2's Stg10_TitleUpdate and Stg20_ShopListUpdate */
        do {
            panel->skillCount = 6;
        } while (0);
        STGDGLAB_createSkillPanelWindows(panel, windows);
        break;
    case TASK_RUN:
        switch (panel->substate) {
        default:
            panel->setState(panel, TASK_KILL);
            /* fallthrough */
        case 0:
            if (STGDGLAB_data.funcs.updateFade(&panel->fade)) {
                STGDGLAB_showSkillPanel(panel, windows, 1);
                windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x26);
                windows->cursor->setPos(windows->cursor, 0x4C, panel->cursor * 0x11 + 0x58);
                windows->cursor->setVisible(windows->cursor, 1);
                windows->cursor->setPalette(windows->cursor, PALETTE_WHITE);
                windows->optionCursor->setVisible(windows->optionCursor, 0);
                panel->substate++;
            }
            break;
        case 1:
            old = panel->cursor;
            if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
                panel->cursor--;
                if (panel->cursor < 0) {
                    panel->cursor = 0;
                }
            } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
                panel->cursor++;
                if (panel->cursor > panel->skillCount - 1) {
                    panel->cursor = panel->skillCount - 1;
                }
            }
            if (old != panel->cursor) {
                SOUND.playSound(SOUND_CURSOR);
                STGDGLAB_showSkillPanel(panel, windows, 1);
                windows->cursor->setPos(windows->cursor, 0x4C, panel->cursor * 0x11 + 0x58);
            } else {
                skill = panel->entry.skills[old];
                if (PAD_PRESSED(PAD_CROSS)) {
                    SOUND.playSound(SOUND_SELECT);
                    if (skill != 0) {
                        if (skill & SKILL_MARKED) {
                            panel->substate = 5;
                            panel->step = 10;
                        } else if (skill & SKILL_LAST) {
                            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xB);
                            panel->substate = 30;
                        } else if (!(skill & SKILL_KNOWN)) {
                            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xA);
                            panel->substate = 30;
                        } else if (panel->learned >= 3) {
                            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 9);
                            panel->substate = 30;
                        } else {
                            panel->substate = 5;
                            panel->step = 20;
                        }
                        windows->cursor->setPalette(windows->cursor, PALETTE_GREY);
                        windows->cursor->setStill(windows->cursor, 1);
                    }
                } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                    SOUND.playSound(SOUND_MENU_CANCEL);
                    panel->setSubstate(panel, 40);
                    STGDGLAB_data.funcs.startFade(&panel->fade, 0);
                    windows->optionCursor->setVisible(windows->optionCursor, 0);
                    windows->cursor->setVisible(windows->cursor, 0);
                    STGDGLAB_showSkillPanel(panel, windows, 0);
                }
            }
            break;
        case 5:
            STGDGLAB_data.funcs.startFade(&panel->confirm, 1);
            panel->substate++;
            break;
        case 6:
            if (STGDGLAB_data.funcs.updateFade(&panel->confirm)) {
                panel->substate = panel->step;
                panel->setStep(panel, 0);
            }
            break;
        case 7:
            windows->optionCursor->setVisible(windows->optionCursor, 0);
            STGDGLAB_data.funcs.startFade(&panel->confirm, 0);
            panel->substate = 6;
            break;
        case 10:
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 7);
            windows->yes->setString(windows->yes, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xC);
            windows->no->setString(windows->no, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xD);
            panel->choice = 0;
            windows->optionCursor->setVisible(windows->optionCursor, 1);
            windows->optionCursor->setPos(windows->optionCursor, 0xB8, panel->choice * 0x10 + 0x38);
            panel->substate++;
            break;
        case 11:
            old = panel->choice;
            if (PAD_PRESSED(PAD_UP)) {
                panel->choice = 0;
            } else if (PAD_PRESSED(PAD_DOWN)) {
                panel->choice = 1;
            }
            if (old != panel->choice) {
                SOUND.playSound(SOUND_CURSOR);
                windows->optionCursor->setPos(windows->optionCursor, 0xB8, panel->choice * 0x10 + 0x38);
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                if (panel->choice == 0) {
                    panel->entry.skills[panel->cursor] &= ~SKILL_MARKED;
                }
                windows->yes->setVisible(windows->yes, 0);
                windows->no->setVisible(windows->no, 0);
                windows->cursor->setStill(windows->cursor, 0);
                panel->setSubstate(panel, 7);
                panel->step = 0;
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                windows->yes->setVisible(windows->yes, 0);
                windows->no->setVisible(windows->no, 0);
                windows->cursor->setStill(windows->cursor, 0);
                panel->setSubstate(panel, 7);
                panel->step = 0;
            }
            break;
        case 20:
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 8);
            windows->yes->setString(windows->yes, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xC);
            windows->no->setString(windows->no, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0xD);
            panel->choice = 0;
            windows->optionCursor->setVisible(windows->optionCursor, 1);
            windows->optionCursor->setPos(windows->optionCursor, 0xB8, panel->choice * 0x10 + 0x38);
            panel->substate++;
            break;
        case 21:
            old = panel->choice;
            if (PAD_PRESSED(PAD_UP)) {
                panel->choice = 0;
            } else if (PAD_PRESSED(PAD_DOWN)) {
                panel->choice = 1;
            }
            if (old != panel->choice) {
                SOUND.playSound(SOUND_CURSOR);
                windows->optionCursor->setPos(windows->optionCursor, 0xB8, panel->choice * 0x10 + 0x38);
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                if (panel->choice == 0) {
                    panel->entry.skills[panel->cursor] |= SKILL_MARKED;
                }
                windows->yes->setVisible(windows->yes, 0);
                windows->no->setVisible(windows->no, 0);
                windows->cursor->setStill(windows->cursor, 0);
                panel->setSubstate(panel, 7);
                panel->step = 0;
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                windows->yes->setVisible(windows->yes, 0);
                windows->no->setVisible(windows->no, 0);
                windows->cursor->setStill(windows->cursor, 0);
                panel->setSubstate(panel, 7);
                panel->step = 0;
            }
            break;
        case 30:
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                windows->cursor->setStill(windows->cursor, 0);
                panel->setSubstate(panel, 0);
            }
            break;
        case 40:
            if (STGDGLAB_data.funcs.updateFade(&panel->fade)) {
                GAME.funcs.setPartnerEntry(panel->member, panel->slots[panel->slot], &panel->entry);
                panel->setState(panel, TASK_KILL);
            }
            break;
        }
        initSpriteDrawer(&sprite);
        sprite.setLayerId(panel->layer, panel->depth);
        if (panel->fade.level != ONE) {
            sprite.setScale(panel->fade.level, ONE, ONE);
            sprite.setPivot(0x140, 0x7D);
        } else {
            sprite.setTexture(0x140, 0);
            for (i = 0; i < 6; i++) {
                if (panel->entry.skills[i] != 0) {
                    id = panel->entry.skills[i] & SKILL_ID;
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), TECHS[id - 1].icon + 0x37, 0x5D,
                                i * 0x11 + 0x58);
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x28, 0x46, 0x41);
        if (panel->fade.level != ONE) {
            sprite.setPivot(0, 0xD0);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2B, 0, 0xBF);
        if (panel->fade.level != ONE) {
            sprite.setPivot(0x140, 0x1F);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x24, 0x92, 0x13);
        if (panel->confirm.level != 0) {
            if (panel->confirm.level != ONE) {
                sprite.setScale(panel->confirm.level, ONE, ONE);
                sprite.setPivot(0x140, 0x46);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            sprite.setLayerId(panel->layer, 1);
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x23, 0xAE, 0x32);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the second screen's skill panel (task) for a slot of a partner (the
   MEMBER parameter is a partner id, from getPartyMember) */
LabSkillPanel *STGDGLAB_createSkillPanel(s32 member, s32 slot) {
    LabSkillPanel *panel = createTask(STGDGLAB_updateSkillPanel, sizeof(LabSkillPanel), 0x5C);

    panel->layer = SCREEN_LAYER;
    panel->depth = 2;
    panel->member = member;
    panel->slot = slot;
    return panel;
}
