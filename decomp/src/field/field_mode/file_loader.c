/* The task that loads the field's files before it starts */

#include "field/field_mode.h"

/* The task (FIELDSTG_loadFieldFiles's) goes unused */
void FIELDSTG_requestInnNames(Task *task) {
    s32 mode = GAME.funcs.getMode();
    s32 found = 0;
    s32 i;

    for (i = 0; FIELDSTG_file5DModes[i] != 0; i++) {
        if (FIELDSTG_file5DModes[i] == (s16)mode) {
            found = 1;
            break;
        }
    }
    if (found) {
        FILE_CACHE.request(TEXT_FILE(TEXT_INN_NAMES));
    }
}

/* Requests the files of the actions the map's slots offer: climbs, drops
   and the gauge game. The task (FIELDSTG_loadFieldFiles's) goes unused. */
void FIELDSTG_requestSlotFiles(Task *task) {
    StageSlot *entry = FIELDSTG_state.slots;
    s32 climbs = 0;
    s32 drops = 0;
    s32 gauges = 0;

    for (; entry->type != 0; entry++) {
        switch (entry->type) {
        case SLOT_CLIMB_UP:
        case SLOT_CLIMB_DOWN:
            climbs = 1;
            break;
        case SLOT_DROP:
            drops = 1;
            break;
        case SLOT_GAUGE:
            gauges = 1;
            break;
        }
    }
    if (climbs) {
        FILE_CACHE.request(FIELD_CLIMB_FILE);
    }
    if (drops) {
        FILE_CACHE.request(FIELD_DROP_FILE);
    }
    if (gauges) {
        FILE_CACHE.request(FIELD_GAUGE_FILE);
    }
}

/* Loads the field's files, a step at a time: 1 once done */
s32 FIELDSTG_loadFieldFiles(Task *task) {
    TimLoader sprites;
    TimLoader loader;

    switch (task->step) {
    case 0:
        switch (task->counter) {
        case 0:
        default:
            FILE_CACHE.request(FIELD_SPRITES_FILE);
            task->tickCounter(task);
            /* fallthrough */
        case 1:
            if (FILE_CACHE.isLoading(FIELD_SPRITES_FILE) != 0) {
                return 0;
            }
            initTimLoader(&sprites);
            sprites.setImagePos(FIELD_SPRITES_X, FIELD_SPRITES_Y);
            sprites.loadArchive(FILE_CACHE.getEntry((FIELD_SPRITES_FILE << 16) | 2));
            sprites.setImagePos(FIELD_SPRITES2_X, FIELD_SPRITES_Y);
            sprites.loadArchive(FILE_CACHE.getEntry((FIELD_SPRITES_FILE << 16) | 3));
            task->nextStep(task);
            break;
        }
        break;
    case 1:
        switch (task->counter) {
        case 0:
        default:
            if (FIELDSTG_state.imageEntry == 0 && FIELDSTG_state.imageFile == 0) {
                task->nextStep(task);
                break;
            }
            if (FIELDSTG_state.imageEntry != 0) {
                FILE_CACHE.request(FIELDSTG_state.imageEntry >> 16);
            }
            if (FIELDSTG_state.sheetEntry != 0) {
                FILE_CACHE.request(FIELDSTG_state.sheetEntry >> 16);
            }
            if (FIELDSTG_state.imageFile != 0) {
                FILE_CACHE.request(FIELDSTG_state.imageFile);
            }
            task->tickCounter(task);
            /* fallthrough */
        case 1:
            if (FIELDSTG_state.imageEntry != 0) {
                if (FILE_CACHE.isLoading(FIELDSTG_state.imageEntry >> 16) != 0) {
                    return 0;
                }
                initTimLoader(&loader);
                loader.setClutPos(0, FIELD_OBJECTS_CLUT_Y);
                loader.setImagePos(FIELD_OBJECTS_X, FIELD_OBJECTS_Y);
                loader.loadArchive(FILE_CACHE.getEntry(FIELDSTG_state.imageEntry));
            }
            task->tickCounter(task);
            /* fallthrough */
        case 2:
            if (FIELDSTG_state.imageFile != 0) {
                if (FILE_CACHE.isLoading(FIELDSTG_state.imageFile) != 0) {
                    return 0;
                }
                initTimLoader(&loader);
                loader.setClutPos(0, FIELD_OBJECTS_CLUT_Y);
                loader.setImagePos(FIELD_OBJECTS_X, FIELD_OBJECTS_Y);
                loader.loadArchive(FILE_CACHE.load(FIELDSTG_state.imageFile));
            }
            task->tickCounter(task);
            /* fallthrough */
        case 3:
            if (FIELDSTG_state.sheetEntry != 0 && FILE_CACHE.isLoading(FIELDSTG_state.sheetEntry >> 16) != 0) {
                return 0;
            }
            task->nextStep(task);
            break;
        }
        break;
    case 2:
        if (FIELDSTG_state.imageFile != 0) {
            FILE_CACHE.free(FIELDSTG_state.imageFile);
        }
        task->nextStep(task);
        /* fallthrough */
    case 3:
        FILE_CACHE.request(FILE_MENU_SPRITES);
        FILE_CACHE.request(FIELDSTG_state.textFile);
        FILE_CACHE.request(TEXT_FILE(TEXT_STATUS));
        FIELDSTG_requestSlotFiles(task);
        FIELDSTG_requestInnNames(task);
        task->nextStep(task);
        return 1;
    default:
        return 1;
    }
    return 0;
}

/* The file loader's update: loads the field's files from its first step on
   (FIELDSTG_loadFieldFiles) */
void FIELDSTG_runFileLoader(Task *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->substate = task->key2;
        /* fallthrough */
    case TASK_RUN:
        if (FIELDSTG_loadFieldFiles(task) != 0) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the file loader, which starts at a step */
Task *FIELDSTG_createFileLoader(s32 step) {
    Task *task = createTask(FIELDSTG_runFileLoader, sizeof(Task) + 4, 0); /* 4 bytes more than it uses */

    task->key2 = step;
    return task;
}
