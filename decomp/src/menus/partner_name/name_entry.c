/* The keyboard for the partner's new name: the name entry STPLNMET and
   STCRDDEK share */

#include "menus/partner_name.h"

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

/* Creates the name entry (task) for a partner, starting from the name given;
   up to 8 characters (5 in language 0, in the European version) */
NameEntry *STDGNAME_createNameEntry(char *name, s32 partner) {
    NameEntry *task = createTask(STDGNAME_updateNameEntry, sizeof(NameEntry), sizeof(NameEntryWindows));

    task->getName = STDGNAME_getName;
    task->close = STDGNAME_closeNameEntry;
    task->layer = SCREEN_LAYER;
    task->depth = 3;
    task->mode = 1;
    task->partner = partner;
#if VERSION_US
    task->maxLength = 8;
#elif VERSION_EU || VERSION_JP
    /* a name of language 0 is shorter */
    if (LANGUAGE == 0) {
        task->maxLength = 5;
    } else {
        task->maxLength = 8;
    }
#endif
    STDGNAME_setName(task, name);
    STDGNAME_setNameVram(task, 0x280, 0x100);
    return task;
}
