/* FIGHTSTG's switch-in menu: which Digimon of the incoming partner comes in,
   and whether it does the pair technique. */

#include "battle/battle_mode.h"

/* Draws the switch-in menu's frames, the second one with the pair technique */
void FIGHTSTG_drawSwitchInMenu(SwitchInMenu *task) {
    SpriteDrawer drawer;
    void *sheet;

    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0x2B, 0xA7, 0x8F);
    if (task->tech != 0) {
        drawer.draw(sheet, 2, 0xA7, 0xB8);
        drawer.draw(sheet, 0x29, 0xA3, 0x21);
    }
}

/* Makes the switch-in menu's windows: the four names, the choices and the MP */
void FIGHTSTG_createSwitchInWindows(SwitchInMenu *task, SwitchInMenuWindows *w) {
    s32 i;

    for (i = 0; i < 4; i++) {
        w->names[i] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xBB, 0x92 + i * 0x13);
    }
    w->choices[4] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xAB, 0x92);
    w->choices[0] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xBB, 0xA5);
    w->choices[1] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xBB, 0xB8);
    w->choices[2] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x10E, 0xBA);
    w->choices[3] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x133, 0xBA);
    for (i = 0; i < 4; i++) {
        w->mp[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, FIGHTSTG_mpWindowX[i], 0x39);
    }
}

/* Shows the names of the incoming partner's Digimon, or hides them */
void FIGHTSTG_showSwitchInNames(SwitchInMenu *task, SwitchInMenuWindows *w, s32 visible) {
    DigimonData *data;
    s32 i;

    if (visible) {
        for (i = 0; i < task->count; i++) {
            data = GET_DIGIMON(task->ids[i]);
            if (data != NULL) {
                w->names[i]->setString(w->names[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (w->names[i] != NULL) {
                w->names[i]->setVisible(w->names[i], visible);
            }
        }
    }
}

/* Shows the picked Digimon's name and the choices, switching in palette 7
   when flag 0x10 bars it and the pair technique in palette 7 when it can't be
   done, with its MP cost and the active fighter's MP; or hides them */
void FIGHTSTG_showSwitchInChoices(SwitchInMenu *task, SwitchInMenuWindows *w, s32 visible) {
    char *text;
    BattleFighter *active;
    BattleFighter *other;
    DigimonData *data;
    s32 i;

    text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
    if (visible) {
        active = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        other = &FIGHTSTG_battle.state.fighters[0][task->fighter];
        data = GET_DIGIMON(task->ids[task->digimon]);
        w->choices[4]->setString(w->choices[4], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
        w->choices[0]->setString(w->choices[0], text, 0x1B);
        if ((active->flags & FIGHTER_NO_SWITCH) || (other->flags & FIGHTER_NO_SWITCH)) {
            w->choices[0]->setPalette(w->choices[0], PALETTE_GREY);
        } else {
            w->choices[0]->setPalette(w->choices[0], PALETTE_WHITE);
        }
        if (task->tech != 0) {
            w->choices[1]->setString(w->choices[1], text, 0x1C);
            w->choices[2]->setString(w->choices[2], text, 0xD);
            w->choices[3]->setNumber(w->choices[3], 0, TECHS[task->tech - 1].mp);
            w->choices[3]->setRightAlign(w->choices[3], 1);
            if (active->mp < TECHS[task->tech - 1].mp || other->mp < TECHS[task->tech - 1].mp) {
                w->choices[1]->setPalette(w->choices[1], PALETTE_GREY);
            } else if ((active->flags & FIGHTER_NO_DIGIVOLVE) || (other->flags & FIGHTER_NO_DIGIVOLVE) || (active->flags & FIGHTER_ASLEEP)) {
                w->choices[1]->setPalette(w->choices[1], PALETTE_GREY);
            } else if (active->hp <= 0 || other->hp <= 0) {
                w->choices[1]->setPalette(w->choices[1], PALETTE_GREY);
            } else {
                w->choices[1]->setPalette(w->choices[1], PALETTE_WHITE);
            }
            w->mp[0]->setString(w->mp[0], text, 0xD);
            w->mp[1]->setNumber(w->mp[1], 0, active->mp);
            w->mp[1]->setRightAlign(w->mp[1], 1);
            w->mp[2]->setString(w->mp[2], text, 0x10);
            w->mp[3]->setNumber(w->mp[3], 0, active->maxMp);
            w->mp[3]->setRightAlign(w->mp[3], 1);
        }
    } else {
        for (i = 0; i < 5; i++) {
            if (w->choices[i] != NULL) {
                w->choices[i]->setVisible(w->choices[i], visible);
            }
        }
        for (i = 0; i < 4; i++) {
            if (w->mp[i] != NULL) {
                w->mp[i]->setVisible(w->mp[i], visible);
            }
        }
    }
}

/* the cursors of FIGHTSTG_updateSwitchInMenu's two steps */
CursorLayout FIGHTSTG_pairCursors[2] = {
    { 4, 168, 147, 19, 36, 167, 146, 19 },
    { 2, 168, 166, 19, 34, 167, 165, 19 },
};
/* the x of FIGHTSTG_createSwitchInWindows's MP windows */
s32 FIGHTSTG_mpWindowX[4] = {
    0xAC, 0xD8, 0xD9, 0xFB,
};

/* Lists the partner and the Digimon in its slots, and makes the menu's
   windows. */
static inline void listSwitchInDigimon(SwitchInMenu *task, SwitchInMenuWindows *w) {
    s32 member;
    s32 count;
    s32 i;

    member = task->partner;
    GAME.funcs.getPartnerSlots(member, task->slots);
    count = 1;
    task->ids[0] = DIGIMON_DATA[member].id;
    for (i = 0; i < 3; i++) {
        if (task->slots[i] >= 3) {
            task->ids[count] = task->slots[i];
            count++;
        }
    }
    task->count = count;
    FIGHTSTG_pairCursors[0].count = count;
    FIGHTSTG_createSwitchInWindows(task, w);
    task->shownPage = -1;
}

/* Sets the pair technique of the active fighter's Digimon with the chosen
   one, 0 for none (a temporary active fighter keeps the one it has). */
static inline void findPairTech(SwitchInMenu *task) {
    DigimonData *data;
    DigimonData *partner;

    if (FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]].temporary == 0) {
        data = GET_DIGIMON(FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]].id);
        partner = GET_DIGIMON(task->ids[task->digimon]);
        if (data->pairPartner != 0 && data->pairPartner == partner->nameId) {
            task->tech = data->pairTech;
        } else {
            task->tech = 0;
        }
    }
}

/* Confirms the switch (unless either fighter has FIGHTER_NO_SWITCH) or the
   pair technique, if both fighters can use it, paying its MP from both. */
static inline void confirmSwitchIn(SwitchInMenu *task, SwitchInMenuWindows *w) {
    BattleFighter *active;
    BattleFighter *other;

    active = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    other = &FIGHTSTG_battle.state.fighters[0][task->fighter];
    SOUND.playSound(SOUND_MENU_CONFIRM);
    if (w->techCursor->sel == 0) {
        if (!(active->flags & FIGHTER_NO_SWITCH) && !(other->flags & FIGHTER_NO_SWITCH)) {
            *task->result = 0;
            *task->result |= task->ids[task->digimon] << 4;
            task->setSubstate(task, 0);
            FIGHTSTG_showSwitchInChoices(task, w, 0);
            w->techCursor->locked = 1;
        }
    } else if (active->mp >= TECHS[task->tech - 1].mp && other->mp >= TECHS[task->tech - 1].mp &&
               !(active->flags & FIGHTER_NO_DIGIVOLVE) && !(other->flags & FIGHTER_NO_DIGIVOLVE) && !(active->flags & FIGHTER_ASLEEP) && active->hp > 0 &&
               other->hp > 0) {
        active->mp -= TECHS[task->tech - 1].mp;
        other->mp -= TECHS[task->tech - 1].mp;
        *task->result = w->techCursor->sel;
        *task->result |= task->ids[task->digimon] << 4;
        *task->techResult = task->tech;
        task->setSubstate(task, 0);
        FIGHTSTG_showSwitchInChoices(task, w, 0);
        w->techCursor->locked = 1;
    }
}

/* Shows the PartnerInfo page again when the page or the chosen Digimon
   changed, closing the shown one. */
static inline void updatePartnerInfoPage(SwitchInMenu *task, SwitchInMenuWindows *w) {
    s32 changed;

    if (task->shownPage != task->page) {
        changed = 1;
    } else if (w->cursor == NULL) {
        changed = 0;
    } else if (task->digimon != w->cursor->sel) {
        task->digimon = w->cursor->sel;
        changed = 1;
    } else {
        changed = 0;
    }
    if (changed) {
        task->shownPage = task->page;
        if (w->info[0] != NULL) {
            w->info[0]->setState(w->info[0], 2);
            w->info[1] = FIGHTSTG_createPartnerInfo(task->partner, task->page, task->digimon);
        } else {
            if (w->info[1] != NULL) {
                w->info[1]->setState(w->info[1], 2);
            }
            w->info[0] = FIGHTSTG_createPartnerInfo(task->partner, task->page, task->digimon);
        }
    }
}

/* The switch-in menu's task: L1 and R1 turn the PartnerInfo's page, cross
   picks the Digimon, then switching (not with flag 0x10 on either fighter)
   or the pair technique (with the MP, HP and none of flag 0x20 or sleep),
   which spends both fighters' MP; triangle goes back a step */
void FIGHTSTG_updateSwitchInMenu(SwitchInMenu *task, SwitchInMenuWindows *w) {
    s32 pressed;

    switch (task->state) {
    case TASK_INIT:
    default:
        listSwitchInDigimon(task, w);
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            switch (task->step) {
            case 0:
            default:
                w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_pairCursors[0]);
                w->cursor->sel = task->digimon;
                if (w->techCursor != NULL) {
                    w->techCursor->setState(w->techCursor, 3);
                }
                task->nextStep(task);
                break;
            case 1:
                /* the match depends on the do-while and its breaks, the stages' early
                   exit, here and in the technique menu below */
                do {
                    pressed = PAD.getPressed(0);
                    if (pressed & (1 << PAD_TRIANGLE)) {
                        SOUND.playSound(SOUND_MENU_CANCEL);
                        *task->result = -2;
                        task->setState(task, 3);
                        break;
                    }
                    if (pressed & (1 << PAD_CROSS)) {
                        SOUND.playSound(SOUND_MENU_CONFIRM);
                        task->nextSubstate(task);
                        FIGHTSTG_showSwitchInNames(task, w, 0);
                        w->cursor->locked = 1;
                        break;
                    }
                    if (pressed & (1 << PAD_R1)) {
                        if (++task->page == 3) {
                            task->page = 0;
                        }
                        SOUND.playSound(SOUND_MENU_MOVE);
                        break;
                    }
                    if (pressed & (1 << PAD_L1)) {
                        if (--task->page < 0) {
                            task->page = 2;
                        }
                        SOUND.playSound(SOUND_MENU_MOVE);
                        break;
                    }
                    FIGHTSTG_showSwitchInNames(task, w, 1);
                    FIGHTSTG_showSwitchInChoices(task, w, 0);
                } while (0);
                break;
            }
            break;
        case 1:
            switch (task->step) {
            case 0:
            default:
                findPairTech(task);
                /* the match depends on the ?: */
                FIGHTSTG_pairCursors[1].count = task->tech != 0 ? 2 : 1;
                w->techCursor = FIGHTSTG_createCursor(&FIGHTSTG_pairCursors[1]);
                if (w->cursor != NULL) {
                    w->cursor->setState(w->cursor, 3);
                }
                task->nextStep(task);
                break;
            case 1:
                do {
                    pressed = PAD.getPressed(0);
                    if (pressed & (1 << PAD_CROSS)) {
                        confirmSwitchIn(task, w);
                        break;
                    }
                    if (pressed & (1 << PAD_TRIANGLE)) {
                        SOUND.playSound(SOUND_MENU_CANCEL);
                        task->setSubstate(task, 0);
                        FIGHTSTG_showSwitchInChoices(task, w, 0);
                        break;
                    }
                    if (pressed & (1 << PAD_R1)) {
                        if (++task->page == 3) {
                            task->page = 0;
                        }
                        SOUND.playSound(SOUND_MENU_MOVE);
                        break;
                    }
                    if (pressed & (1 << PAD_L1)) {
                        if (--task->page < 0) {
                            task->page = 2;
                        }
                        SOUND.playSound(SOUND_MENU_MOVE);
                        break;
                    }
                    FIGHTSTG_showSwitchInNames(task, w, 0);
                    FIGHTSTG_showSwitchInChoices(task, w, 1);
                } while (0);
                break;
            }
            FIGHTSTG_drawSwitchInMenu(task);
            break;
        }
        updatePartnerInfoPage(task, w);
        break;
    case 2:
    case 3:
        break;
    }
}

/* Opens the switch-in menu for fighter *result, without the pair technique */
SwitchInMenu *FIGHTSTG_createSwitchInMenu(s32 *result) {
    SwitchInMenu *task = createTask(FIGHTSTG_updateSwitchInMenu, sizeof(SwitchInMenu), sizeof(SwitchInMenuWindows));

    task->result = result;
    task->fighter = *result;
    task->partner = GAME.funcs.getPartyMember(*result);
    *result = -1;
    return task;
}

/* Opens the switch-in menu for fighter *result, with the pair technique,
   which goes into *techResult */
SwitchInMenu *FIGHTSTG_createPairSwitchMenu(s32 *result, s32 *techResult) {
    SwitchInMenu *task = FIGHTSTG_createSwitchInMenu(result);

    task->techResult = techResult;
    return task;
}
