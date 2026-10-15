/* WFIGHTMN, the battle's sub-overlay that FIGHTSTG loads for a normal
   battle: the battle menu and the turn's events. Its first object is this
   file, the loader; the second is cut, in its link order, into setup.c,
   menu.c and techs.c, with its data in data/wfightmn_2.c. */

#include "battle/normal_battle.h"

/* Loads the battle menu's images into VRAM, one file a frame, then its
   other files, and ends */
void WFIGHTMN_loadFiles(BattleLoader *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            switch (task->step) {
            case 0:
            default:
                FILE_CACHE.request(FILE_BATTLE_IMAGES);
                FILE_CACHE.request(FILE_BATTLE_IMAGES_1);
                FILE_CACHE.request(FILE_BATTLE_IMAGES_2);
                FILE_CACHE.request(FILE_BATTLE_IMAGES_3);
                FILE_CACHE.request(FILE_BATTLE_IMAGES_4);
                task->nextStep(task);
                break;
            case 1:
                if (FILE_CACHE.isLoading(FILE_BATTLE_IMAGES) == 0) {
                    TimLoader loader;

                    initTimLoader(&loader);
                    loader.setImagePos(0x200, 0);
                    loader.loadArchive(FILE_CACHE.getEntry(FILE_BATTLE_IMAGES << 16 | 1));
                    loader.setImagePos(0, 0xF4);
                    loader.load(FILE_CACHE.getEntry(FILE_BATTLE_IMAGES << 16));
                    task->nextStep(task);
                }
                break;
            case 2:
                if (FILE_CACHE.isLoading(FILE_BATTLE_IMAGES_1) == 0) {
                    TimLoader loader;

                    initTimLoader(&loader);
                    loader.setImagePos(0x140, 0x100);
                    loader.loadArchive(FILE_CACHE.load(FILE_BATTLE_IMAGES_1));
                    task->nextStep(task);
                }
                break;
            case 3:
                if (FILE_CACHE.isLoading(FILE_BATTLE_IMAGES_2) == 0) {
                    TimLoader loader;

                    initTimLoader(&loader);
                    loader.setImagePos(0x1C0, 0x100);
                    loader.loadArchive(FILE_CACHE.load(FILE_BATTLE_IMAGES_2));
                    task->nextStep(task);
                }
                break;
            case 4:
                if (FILE_CACHE.isLoading(FILE_BATTLE_IMAGES_3) == 0) {
                    TimLoader loader;

                    initTimLoader(&loader);
                    loader.setImagePos(0x200, 0x100);
                    loader.loadArchive(FILE_CACHE.load(FILE_BATTLE_IMAGES_3));
                    task->nextStep(task);
                }
                break;
            case 5:
                if (FILE_CACHE.isLoading(FILE_BATTLE_IMAGES_4) == 0) {
                    TimLoader loader;

                    initTimLoader(&loader);
                    loader.setImagePos(0x240, 0x100);
                    loader.loadArchive(FILE_CACHE.load(FILE_BATTLE_IMAGES_4));
                    task->nextStep(task);
                }
                break;
            case 6:
                FILE_CACHE.free(FILE_BATTLE_IMAGES);
                FILE_CACHE.free(FILE_BATTLE_IMAGES_1);
                FILE_CACHE.free(FILE_BATTLE_IMAGES_2);
                FILE_CACHE.free(FILE_BATTLE_IMAGES_3);
                FILE_CACHE.free(FILE_BATTLE_IMAGES_4);
                task->nextSubstate(task);
                break;
            }
            break;
        case 1:
            switch (task->step) {
            case 0:
            default:
                FILE_CACHE.request(FILE_MENU_SPRITES);
                FILE_CACHE.request(FILE_BATTLE_MENU);
                FILE_CACHE.request(TEXT_FILE(TEXT_BATTLE_MENU));
                FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMES));
                FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_NAMES));
                FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_INFO));
                FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
                FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_INFO));
                task->nextStep(task);
                break;
            case 1:
                if (FILE_CACHE.isLoading(FILE_MENU_SPRITES) == 0 && FILE_CACHE.isLoading(FILE_BATTLE_MENU) == 0 &&
                    FILE_CACHE.isLoading(TEXT_FILE(TEXT_BATTLE_MENU)) == 0 && FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_NAMES)) == 0 &&
                    FILE_CACHE.isLoading(TEXT_FILE(TEXT_SKILL_NAMES)) == 0 && FILE_CACHE.isLoading(TEXT_FILE(TEXT_SKILL_INFO)) == 0 &&
                    FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_NAMES)) == 0 && FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_INFO)) == 0) {
                    task->setState(task, TASK_KILL);
                }
                break;
            }
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Starts loading the battle menu's files */
BattleLoader *WFIGHTMN_createLoader(void) {
    return createTask(WFIGHTMN_loadFiles, sizeof(BattleLoader), 0);
}
