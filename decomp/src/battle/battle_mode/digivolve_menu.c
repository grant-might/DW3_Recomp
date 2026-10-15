/* FIGHTSTG's digivolve menu, the Digimon the active partner can digivolve
   into. */

#include "battle/battle_mode.h"

/* Shows the names of the Digimon the menu's partner can digivolve into, its
   current one in palette 7 */
void FIGHTSTG_showDigivolveNames(DigivolveMenu *task) {
    TextWindow **windows = task->children;
    BattleFighter *fighter;
    DigimonData *data;
    s32 index;
    s32 current;
    s32 i;

    /* the match depends on setting index in the loop's init */
    for (i = 0, index = -1; i < 3; i++) {
        if (task->partner == GAME.funcs.getPartyMember(i)) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    fighter = &FIGHTSTG_battle.state.fighters[0][index];
    if (fighter->temporary) {
        current = fighter->prevId;
    } else {
        current = fighter->id;
    }
    for (i = 0; i < task->count; i++) {
        if (windows[i + 1] == NULL) {
            windows[i + 1] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xBB, 0x92 + i * 0x13);
        }
        data = GET_DIGIMON(task->ids[i]);
        if (data != NULL) {
            windows[i + 1]->setString(windows[i + 1], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data->nameId);
            if (current == task->ids[i]) {
                windows[i + 1]->setPalette(windows[i + 1], PALETTE_GREY);
            } else {
                windows[i + 1]->setPalette(windows[i + 1], PALETTE_WHITE);
            }
        }
    }
}

/* the cursor of the Digimon a partner can change into (FIGHTSTG_updateDigivolveMenu sets
   its count) */
CursorLayout FIGHTSTG_changeCursor = {
    0, 168, 147, 19, 36, 167, 146, 19,
};

/* The digivolve menu's task: L1 and R1 turn the PartnerInfo's page, cross
   picks one other than the fighter's current Digimon into *result,
   triangle gives -2. The match depends on taking the children as the
   update's void * and copying them: as a parameter of their own type they
   are equivalent to their argument slot, whose doubled live length gives
   their register to changed */
void FIGHTSTG_updateDigivolveMenu(DigivolveMenu *task, void *children) {
    DigivolveMenuWindows *w = children;
    BattleFighter *fighter;
    s32 pressed;
    s32 changed;
    s32 show;
    s32 current;
    s32 id;
    s32 count;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        id = task->partner;
        GAME.funcs.getPartnerSlots(id, task->slots);
        task->ids[0] = DIGIMON_DATA[id].id;
        count = 1;
        for (i = 0; i < 3; i++) {
            if (task->slots[i] >= 3) {
                task->ids[count] = task->slots[i];
                count++;
            }
        }
        task->count = count;
        FIGHTSTG_changeCursor.count = count;
        w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_changeCursor);
        w->info[0] = FIGHTSTG_createPartnerInfo(task->partner, task->page, w->cursor->sel);
        FIGHTSTG_showDigivolveNames(task);
        task->sel = -1;
        task->nextState(task);
        break;
    case TASK_RUN:
        changed = 0;
        show = 1; /* never cleared, but the original tests it */
        /* the match depends on the do-while and its breaks, the stages' early exit */
        do {
            pressed = PAD.getPressed(0);
            if (pressed & (1 << PAD_R1)) {
                if (++task->page == 3) {
                    task->page = 0;
                }
                changed = 1;
                SOUND.playSound(SOUND_MENU_MOVE);
                break;
            }
            if (pressed & (1 << PAD_L1)) {
                if (--task->page < 0) {
                    task->page = 2;
                }
                changed = 1;
                SOUND.playSound(SOUND_MENU_MOVE);
                break;
            }
            if (pressed & (1 << PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
                if (fighter->temporary) {
                    current = fighter->prevId;
                } else {
                    current = fighter->id;
                }
                if (current != task->ids[w->cursor->sel]) {
                    *task->result = task->ids[w->cursor->sel];
                    task->setState(task, 3);
                    w->cursor->locked = 1;
                    break;
                }
            }
            if (pressed & (1 << PAD_TRIANGLE)) {
                *task->result = -2;
                task->setState(task, 3);
                SOUND.playSound(SOUND_MENU_CANCEL);
            }
        } while (0);
        if (w->cursor->sel != task->sel) {
            task->sel = w->cursor->sel;
            changed = 1;
        }
        if (changed) {
            if (show) {
                if (w->info[0] != NULL) {
                    w->info[0]->setState(w->info[0], 2);
                    w->info[1] = FIGHTSTG_createPartnerInfo(task->partner, task->page, w->cursor->sel);
                } else {
                    if (w->info[1] != NULL) {
                        w->info[1]->setState(w->info[1], 2);
                    }
                    w->info[0] = FIGHTSTG_createPartnerInfo(task->partner, task->page, w->cursor->sel);
                }
            } else {
                if (w->info[0] != NULL) {
                    w->info[0]->setState(w->info[0], 2);
                }
                if (w->info[1] != NULL) {
                    w->info[1]->setState(w->info[1], 2);
                }
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Opens the menu of the Digimon the partner *result can digivolve into;
   *result gets the one picked, or -2 to go back */
DigivolveMenu *FIGHTSTG_createDigivolveMenu(s32 *result) {
    DigivolveMenu *task = createTask(FIGHTSTG_updateDigivolveMenu, sizeof(DigivolveMenu), sizeof(DigivolveMenuWindows));

    task->result = result;
    task->partner = *result;
    *result = -1;
    return task;
}
