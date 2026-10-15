/* The keyboard to rename a deck: the name entry STPLNMET and STDGNAME share */

#include "cardgame/deck_editor.h"

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

/* Creates the name entry (task) to rename a deck, starting from its name: up to 10 characters */
NameEntry *STCRDDEK_createNameEntry(char *text) {
    NameEntry *entry = createTask(STCRDDEK_updateNameEntry, sizeof(NameEntry), sizeof(NameEntryWindows));

    entry->getName = STCRDDEK_getName;
    entry->close = STCRDDEK_closeNameEntry;
    entry->layer = SCREEN_LAYER;
    entry->depth = 3;
    entry->mode = 2;
    entry->maxLength = 10;
    entry->partner = -1;
    STCRDDEK_setName(entry, text);
    STCRDDEK_setNameVram(entry, 0x280, 0x100);
    return entry;
}
