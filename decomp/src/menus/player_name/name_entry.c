/* The keyboard for the player's name: the name entry STCRDDEK and STDGNAME
   share */

#include "menus/player_name.h"

#define START_FADE OVL_NAME(startTween)
#include "shared/start_fade.inc.c"
#define UPDATE_FADE OVL_NAME(updateTween)
#include "shared/update_fade.inc.c"
#include "shared/name_entry/create_name_windows.inc.c"
#include "shared/name_entry/show_name_windows.inc.c"
#include "shared/name_entry/draw_keyboard.inc.c"
#include "shared/name_entry/update_keyboard.inc.c"
#include "shared/name_entry/update_name_entry.inc.c"
#include "shared/name_entry/set_name_vram.inc.c"
#include "shared/name_entry/set_name.inc.c"
#include "shared/name_entry/get_name.inc.c"
#include "shared/name_entry/close_name_entry.inc.c"

/* Hides the name entry, putting its cursor after the name's last character, or
   shows it again (name->hide) as the screen leaves or comes back to its page */
void STPLNMET_hideNameEntry(NameEntry *task, s32 hide) {
    NameEntryWindows *windows = task->children;
    s32 i;

    if (hide != 0) {
        task->state = TASK_DONE;
        STPLNMET_showNameWindows(task, windows, 0);
        for (i = task->maxLength - 1; i >= 0 && task->name[i] == SJIS_SPACE; i--) {
        }
        if (i == task->maxLength - 1) {
            task->cursor = i;
        } else {
            task->cursor = i + 1;
        }
    } else {
        task->state = TASK_RUN;
        task->substate = 2;
        STPLNMET_showNameWindows(task, windows, 1);
    }
}

/* Creates the name entry (task) for the player's name, starting from name: up to
   8 characters (5 in Japanese) */
NameEntry *STPLNMET_createNameEntry(char *name) {
    NameEntry *task = createTask(STPLNMET_updateNameEntry, sizeof(NameEntry), sizeof(NameEntryWindows));

    task->getName = STPLNMET_getName;
    task->hide = STPLNMET_hideNameEntry;
    task->close = STPLNMET_closeNameEntry;
    task->layer = 0x1001;
    task->depth = 6;
    task->mode = 0;
    task->partner = -1;
#if VERSION_US
    task->maxLength = 8;
#elif VERSION_EU || VERSION_JP
    /* five full-width characters in Japanese */
    if (LANGUAGE == 0) {
        task->maxLength = 5;
    } else {
        task->maxLength = 8;
    }
#endif
    STPLNMET_setName(task, name);
    STPLNMET_setNameVram(task, 0x140, 0x100);
    return task;
}
