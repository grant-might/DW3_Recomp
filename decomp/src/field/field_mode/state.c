/* FieldState's methods: the file and width of a character, the stage of
   the mode and the battles of a place */

#include "field/field_mode.h"

/* The file entry index of FIELDSTG_fileEntries */
s32 FIELDSTG_getFileEntry(s32 index) {
    return FIELDSTG_fileEntries[index];
}

/* The width of character index */
s32 FIELDSTG_getActorWidth(s32 index) {
    return FIELDSTG_actorWidths[index];
}

/* Picks the stage overlay of the mode (FIELDSTG_stages, in the European
   version's extra chapter FIELDSTG_extraStages and then those): its file and
   its setup; hangs if there is none */
void FIELDSTG_pickStage(void) {
    StageEntry *entry;
    s32 mode;

#if VERSION_US
    entry = FIELDSTG_stages;
#elif VERSION_EU || VERSION_JP
    if (GAME.progress != FIELD_PROGRESS_EXTRA) {
        entry = FIELDSTG_stages;
    } else {
        entry = FIELDSTG_extraStages;
    }
#endif
    mode = GAME.funcs.getMode();
    HEAP.zero(&FIELDSTG_state, 100);
    while (1) {
        if (entry->mode == mode) {
            FIELDSTG_state.stageFile = entry->file;
            FIELDSTG_state.stageInit = entry->init;
            break;
        }
        if ((++entry)->mode == 0) {
            break;
        }
    }
    if (entry->mode == 0) {
        while (1) {
        }
    }
}

/* The battles of the place id in a stage's list, or NULL */
FieldBattles *FIELDSTG_findBattles(FieldBattles *list, s32 id) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (list->id == id) {
            return list;
        }
        list++;
    }
    return NULL;
}
