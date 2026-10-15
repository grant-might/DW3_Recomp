/* FIGHTSTG's command menu, the six commands of the player's turn. */

#include "battle/battle_mode.h"

/* Shows the six commands once, greying out attack, techniques and digivolve
   when the active fighter is asleep and digivolve when it has flag 0x20 (the
   first window's setPalette is called for each) */
void FIGHTSTG_showCommands(CommandMenu *task) {
    CommandMenuWindows *w = task->children;
    BattleFighter *fighter;
    char *text;
    s32 i;

    if (w->lines[0] == NULL) {
        text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
        for (i = 0; i < BATTLE_COMMAND_COUNT; i++) {
            w->lines[i] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x24, 0x6D + i * 0x13);
            w->lines[i]->setString(w->lines[i], text, i + 1);
        }
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        if (fighter->flags & FIGHTER_ASLEEP) {
            for (i = 0; i < BATTLE_COMMAND_SWITCH; i++) {
                w->lines[0]->setPalette(w->lines[i], PALETTE_GREY);
            }
        }
        if (fighter->flags & FIGHTER_NO_DIGIVOLVE) {
            w->lines[0]->setPalette(w->lines[BATTLE_COMMAND_DIGIVOLVE], PALETTE_GREY);
        }
    }
}

/* the battle commands' cursor (FIGHTSTG_createCursor makes the menus' cursors from
   these layouts) */
CursorLayout FIGHTSTG_commandCursor = {
    6, 17, 110, 19, 34, 16, 109, 19,
};

/* The command menu's task: puts a cursor (FIGHTSTG_commandCursor) on the
 * lines, starting on start; cross picks the line into *result and locks the
 * cursor, but not the greyed-out ones (FIGHTSTG_showCommands).
 * The match depends on the early exits being breaks out of a do/while. */
void FIGHTSTG_updateCommandMenu(CommandMenu *task, CommandMenuWindows *w) {
    BattleFighter *fighter;

    switch (task->state) {
    case TASK_INIT:
    default:
        w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_commandCursor);
        w->cursor->sel = task->start;
        FIGHTSTG_showCommands(task);
        task->nextState(task);
        break;
    case TASK_RUN:
        do {
            if (!(PAD.getPressed(0) & (1 << PAD_CROSS))) {
                break;
            }
            SOUND.playSound(SOUND_MENU_CONFIRM);
            fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
            if ((fighter->flags & FIGHTER_ASLEEP) && w->cursor->sel < BATTLE_COMMAND_SWITCH) {
                break;
            }
            if ((fighter->flags & FIGHTER_NO_DIGIVOLVE) && w->cursor->sel == BATTLE_COMMAND_DIGIVOLVE) {
                break;
            }
            *task->result = w->cursor->sel;
            task->setState(task, 3);
            w->cursor->locked = 1;
        } while (0);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Opens the battle's command menu with the cursor on start; *result gets
   the command (PlayerTurn.command) */
CommandMenu *FIGHTSTG_createCommandMenu(s32 start, s32 *result) {
    CommandMenu *task = createTask(FIGHTSTG_updateCommandMenu, sizeof(CommandMenu), sizeof(CommandMenuWindows));

    task->result = result;
    *result = -1;
    task->start = start;
    return task;
}
