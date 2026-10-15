/*
 * WSTAG212: Main Lobby, Asuka City: the story events' copy. STAGSLCT's "Main
 * Lobby: demo": the first partner's download, the DO Guard, Kail asking how to
 * change into Digimon.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame *updateTileEffectFrames[];
extern s8 tileEffectSortY[];
extern s16 tileEffectPlaces[][2];
extern s32 tileEffectSounds[];

#include "common/step_animation_once.inc.c"

#include "common/reset_tile_effect.inc.c"

/* Moves its map objects to (x, y) and animates them when the substate is set to 1 */
void updateTileEffect(StageTileEffect *task) {
    StageTile *tile;
    StageTile *t;
    s32 i;
    s32 n;
    s32 j;
    s32 frame;
    s32 done;

    switch (task->state) {
    case TASK_INIT:
    default:
        resetTileEffect(task);
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            switch (task->step) {
            case 0:
                n = 0;
                for (t = FIELDSTG_state.objects; t->margin != 0; t++) {
                    if (t->anim >= 1 && t->anim <= 3) {
                        task->anims[n].tile = t;
                        t->x = task->x;
                        t->y = task->y;
                        t->sortY = task->y + tileEffectSortY[n];
                        n++;
                    }
                }
                task->nextStep(task);
                /* fallthrough */
            case 1:
                done = 0;
                for (i = 0; i < 3; i++) {
                    tile = task->anims[i].tile;
                    frame = stepAnimationOnce(&task->anims[i], updateTileEffectFrames[i], 0);
                    switch (frame) {
                    case 0xFF:
                        done++;
                        tile->visible = 0;
                        tile->frame = 0;
                        break;
                    case 0x12C:
                        tile->visible = 0;
                        tile->frame = 0;
                        break;
                    default:
                        tile->visible = 1;
                        tile->frame = frame;
                        break;
                    }
                }
                if (done < 3) {
                    break;
                }
                task->nextStep(task);
                /* fallthrough */
            case 2:
                for (j = 0; j < 3; j++) {
                    task->anims[j].tile->visible = 0;
                }
                resetTileEffect(task);
                task->setSubstate(task, 0);
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

/* Starts the effect at the place of map ID */
void handleCommand810(StageTileEffect *task, s32 id) {
    s32 i;

    if (task != NULL) {
        i = 0;
        switch (id) {
        case 0x33B:
            i = 1;
            /* fallthrough */
        case 0x33A:
            i++;
            /* fallthrough */
        case 0x32F:
            task->x = tileEffectPlaces[i][0];
            task->y = tileEffectPlaces[i][1];
            task->setSubstate(task, 1);
            SOUND.playSound(tileEffectSounds[i]);
            break;
        }
    }
}

/*
 * Command 810 of the event scripts: the effect of events 10 to 12, where the
 * Tamer Service downloads the partners
 */
void *createCommand810(s32 arg) {
    return createTaskWithId(updateTileEffect, sizeof(StageTileEffect), 0, arg);
}

/* Creates the event object of the story so far, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 2 && FLAGS_00.checkCondition(SPECIAL(0), 1)) {
            children[0] = FIELDSTG_startEvent(0xA);
        } else if (GAME.progress == 2 && FLAGS_00.checkCondition(SPECIAL(1), 1)) {
            children[0] = FIELDSTG_startEvent(0xB);
        } else if (GAME.progress == 2 && FLAGS_00.checkCondition(SPECIAL(2), 1)) {
            children[0] = FIELDSTG_startEvent(0xC);
        } else if (GAME.progress == 0xB && FLAGS_00.checkCondition(FLAG(0x40, 4), 0)) {
            children[0] = FIELDSTG_startEvent(0x10F);
        } else if (GAME.progress == 0xB && FLAGS_00.checkCondition(FLAG(0x40, 4), 1)) {
            children[0] = FIELDSTG_startEvent(0x111);
        } else if (GAME.progress == 0xD && FLAGS_00.checkCondition(FLAG(0x1C, 0xD), 1)) {
            children[0] = FIELDSTG_startEvent(0x160);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/*
 * After events 10 to 12, where the Tamer Service downloads the starter pack
 * the player chose (A, B or C): moves the story on to its step 3
 */
void endEvents10To12(void) {
    GAME.progress = 3;
}

/*
 * After event 271, with Maniac Phillip, Lazy Fei, Tamer Service, Kail, Lazy
 * Alan, Worker Jeff, Miss Misha, Game Master and Energetic Yuki: sets flag
 * 0x4005 and keeps it from playing again
 */
void endEvent271(void) {
    FLAGS_00.applyAction(FLAG(0x40, 5), 1);
    FLAGS_00.applyAction(FLAG(0x40, 4), 1);
}

/* After event 273, with Kail: moves the story on to its step 0xC */
void endEvent273(void) {
    GAME.progress = 0xC;
}

/*
 * After event 352, with Kail: lets event 40 of WSTAG285 start and moves the
 * story on to its step 0xE
 */
void endEvent352(void) {
    FLAGS_00.applyAction(FLAG(0x40, 5), 0);
    GAME.progress = 0xE;
}

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x4BE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x4CE
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0205 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x17700, 0x13F00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x43;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.music = MUSIC(0x43, 3);
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

s16 script10[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x160, 0x110),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(5, 0x3D8, 0x194),
    SCRIPT_POSE(5, 1, 3),
    SCRIPT_PLACE(6, 0x3F0, 0x1A1),
    SCRIPT_POSE(6, 1, 3),
    SCRIPT_PLACE(0x1B, 0x3A1, 0x179),
    SCRIPT_POSE(0x1B, 1, 7),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_PLACE(0x2D, 0x29F, 0x178),
    SCRIPT_POSE(0x2D, 1, 6),
    SCRIPT_PLACE(0x2E, 0x187, 0x167),
    SCRIPT_POSE(0x2E, 1, 1),
    SCRIPT_PLACE(0x31, 0x178, 0xD5),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x34, 0xBF, 0x10A),
    SCRIPT_POSE(0x34, 1, 7),
    SCRIPT_COMMAND(0x32A, 0x33C, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x20, 0),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x131, 0x128, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 1),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_PLACE(0xB, 0x1D5, 0x159),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_PLACE(0xC, 0x1E7, 0x162),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WALK(0xB, 0x180, 0x12E, 2),
    SCRIPT_WALK(0xC, 0x193, 0x138, 2),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 3, 0xC, 0),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_POSE(0x2E, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_POSE(0x2E, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 7, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x32F, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0x111, 0x117),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0xB6, 0xD9, 0x124),
    SCRIPT_POSE(0xB6, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33A, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x41, 0x101, 0x110),
    SCRIPT_POSE(0x41, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33B, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x42, 0x129, 0xFC),
    SCRIPT_POSE(0x42, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(0xB6, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0xB6, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB6, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x41, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_POSE(0x42, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x42, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x40, 3),
    SCRIPT_POSE(0x41, 0x40, 7),
    SCRIPT_POSE(0x42, 0x40, 7),
    SCRIPT_POSE(0xB6, 0x40, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0x41, 1, 7),
    SCRIPT_POSE(0x42, 1, 7),
    SCRIPT_POSE(0xB6, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0xE8, 0x14C, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WALK(0x41, 0x131, 0x128, 1),
    SCRIPT_WALK(0x42, 0x158, 0x114, 1),
    SCRIPT_WALK(0xB6, 0x108, 0x13C, 1),
    SCRIPT_WAIT_WALK(0x42),
    SCRIPT_WALK(0x41, 0x128, 0x12C, 1),
    SCRIPT_WALK(0x42, 0x148, 0x11C, 1),
    SCRIPT_POSE(0xB6, 1, 1),
    SCRIPT_WAIT_WALK(0x42),
    SCRIPT_WALK(1, 0x5A, 0x191, 1),
    SCRIPT_WALK(0x41, 0x5A, 0x191, 1),
    SCRIPT_WALK(0x42, 0x5A, 0x191, 1),
    SCRIPT_WALK(0xB6, 0x5A, 0x191, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
s16 script11[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x160, 0x110),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_PLACE(0x2E, 0x187, 0x167),
    SCRIPT_POSE(0x2E, 1, 1),
    SCRIPT_PLACE(0x31, 0x178, 0xD5),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x34, 0xBF, 0x10A),
    SCRIPT_POSE(0x34, 1, 7),
    SCRIPT_COMMAND(0x32A, 0x33C, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x20, 0),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x131, 0x128, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 1),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_PLACE(0xB, 0x1D5, 0x159),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_PLACE(0xC, 0x1E7, 0x162),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WALK(0xB, 0x180, 0x12E, 2),
    SCRIPT_WALK(0xC, 0x193, 0x138, 2),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 3, 0xC, 0),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_POSE(0x2E, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_POSE(0x2E, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 7, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x32F, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0x111, 0x117),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0xB5, 0xD9, 0x124),
    SCRIPT_POSE(0xB5, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33A, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x22, 0x101, 0x110),
    SCRIPT_POSE(0x22, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33B, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x41, 0x129, 0xFC),
    SCRIPT_POSE(0x41, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(0xB5, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0xB5, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB5, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x22, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_POSE(0x41, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x41, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x40, 3),
    SCRIPT_POSE(0x22, 0x40, 7),
    SCRIPT_POSE(0x41, 0x40, 7),
    SCRIPT_POSE(0xB5, 0x40, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0x22, 1, 7),
    SCRIPT_POSE(0x41, 1, 7),
    SCRIPT_POSE(0xB5, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0xE8, 0x14C, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WALK(0x22, 0x131, 0x128, 1),
    SCRIPT_WALK(0x41, 0x158, 0x114, 1),
    SCRIPT_WALK(0xB5, 0x108, 0x13C, 1),
    SCRIPT_WAIT_WALK(0x41),
    SCRIPT_WALK(0x22, 0x128, 0x12C, 1),
    SCRIPT_WALK(0x41, 0x148, 0x11C, 1),
    SCRIPT_POSE(0xB5, 1, 1),
    SCRIPT_WAIT_WALK(0x41),
    SCRIPT_WALK(1, 0x5A, 0x191, 1),
    SCRIPT_WALK(0x22, 0x5A, 0x191, 1),
    SCRIPT_WALK(0x41, 0x5A, 0x191, 1),
    SCRIPT_WALK(0xB5, 0x5A, 0x191, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
s16 script12[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x160, 0x110),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_PLACE(0x2E, 0x187, 0x167),
    SCRIPT_POSE(0x2E, 1, 1),
    SCRIPT_PLACE(0x31, 0x178, 0xD5),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x34, 0xBF, 0x10A),
    SCRIPT_POSE(0x34, 1, 7),
    SCRIPT_COMMAND(0x32A, 0x33C, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x20, 0),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x131, 0x128, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 1),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_PLACE(0xB, 0x1D5, 0x159),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_PLACE(0xC, 0x1E7, 0x162),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WALK(0xB, 0x180, 0x12E, 2),
    SCRIPT_WALK(0xC, 0x193, 0x138, 2),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 3, 0xC, 0),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_POSE(0x2E, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_POSE(0x2E, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 7, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x32F, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0x111, 0x117),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0xB4, 0xD9, 0x124),
    SCRIPT_POSE(0xB4, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33A, 0x40),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x40, 0x101, 0x110),
    SCRIPT_POSE(0x40, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32A, 0x33B, 0x42),
    SCRIPT_WAIT(0xB4),
    SCRIPT_PLACE(0x42, 0x129, 0xFC),
    SCRIPT_POSE(0x42, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(0xB4, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0xB4, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB4, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x40, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_POSE(0x42, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x42, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x40, 3),
    SCRIPT_POSE(0x40, 0x40, 7),
    SCRIPT_POSE(0x42, 0x40, 7),
    SCRIPT_POSE(0xB4, 0x40, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0x40, 1, 7),
    SCRIPT_POSE(0x42, 1, 7),
    SCRIPT_POSE(0xB4, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 1, 1),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0xC, 0),
    SCRIPT_POSE(0xC, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0xE8, 0x14C, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WALK(0x40, 0x131, 0x128, 1),
    SCRIPT_WALK(0x42, 0x158, 0x114, 1),
    SCRIPT_WALK(0xB4, 0x108, 0x13C, 1),
    SCRIPT_WAIT_WALK(0x42),
    SCRIPT_WALK(0x40, 0x128, 0x12C, 1),
    SCRIPT_WALK(0x42, 0x148, 0x11C, 1),
    SCRIPT_POSE(0xB4, 1, 1),
    SCRIPT_WAIT_WALK(0x42),
    SCRIPT_WALK(1, 0x5A, 0x191, 1),
    SCRIPT_WALK(0x40, 0x5A, 0x1D7, 1),
    SCRIPT_WALK(0x42, 0x5A, 0x191, 1),
    SCRIPT_WALK(0xB4, 0x5A, 0x191, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
s16 script271[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x50, 0x198),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0xD, 0x180, 0xFF),
    SCRIPT_POSE(0xD, 1, 1),
    SCRIPT_PLACE(0xE, 0x1B2, 0xF7),
    SCRIPT_POSE(0xE, 1, 7),
    SCRIPT_PLACE(0x2E, 0x18E, 0x144),
    SCRIPT_POSE(0x2E, 1, 2),
    SCRIPT_PLACE(0x31, 0x123, 0xE6),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x32, 0x160, 0x10F),
    SCRIPT_POSE(0x32, 1, 5),
    SCRIPT_PLACE(0x33, 0x172, 0x118),
    SCRIPT_POSE(0x33, 1, 5),
    SCRIPT_PLACE(0x34, 0x13F, 0xE5),
    SCRIPT_POSE(0x34, 1, 1),
    SCRIPT_PLACE(0x35, 0x108, 0xFE),
    SCRIPT_POSE(0x35, 1, 6),
    SCRIPT_PLACE(0x36, 0x12C, 0x105),
    SCRIPT_POSE(0x36, 1, 4),
    SCRIPT_PLACE(0x39, 0x16E, 0x144),
    SCRIPT_POSE(0x39, 1, 6),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_GAYALOOP, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x68, 0x18C, 5),
    SCRIPT_PLACE(0xB, 0x50, 0x198),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_WALK(1, 0x12C, 0x12A, 5),
    SCRIPT_WALK(0xB, 0x113, 0x136, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x32),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x33, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 2, 0x32, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xD, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 0xB, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x39),
    SCRIPT_COMMAND(0x324, 0x325, 0xE),
    SCRIPT_COMMAND(0x325, 0x325, 0x32),
    SCRIPT_COMMAND(0x326, 0x325, 0x35),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x39),
    SCRIPT_COMMAND(0x324, 0x326, 0x70),
    SCRIPT_COMMAND(0x325, 0x326, 0x32),
    SCRIPT_COMMAND(0x326, 0x326, 0x35),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x31),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_POSE(0xE, 1, 2),
    SCRIPT_POSE(0x2E, 1, 3),
    SCRIPT_POSE(0x31, 1, 3),
    SCRIPT_POSE(0x32, 1, 3),
    SCRIPT_POSE(0x33, 1, 3),
    SCRIPT_POSE(0x34, 1, 3),
    SCRIPT_POSE(0x35, 1, 3),
    SCRIPT_POSE(0x36, 1, 3),
    SCRIPT_POSE(0x39, 1, 3),
    SCRIPT_PLACE(0x6C, 0, 0),
    SCRIPT_POSE(0x6C, 1, 0),
    SCRIPT_PLACE(0x6D, 0xA0, 0x88),
    SCRIPT_POSE(0x6D, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(0x6D, 0xCD, 0x9F, 7),
    SCRIPT_PLACE(0x6E, 0xA0, 0x88),
    SCRIPT_POSE(0x6E, 1, 7),
    SCRIPT_WAIT_WALK(0x6D),
    SCRIPT_PLACE(0x6C, 0xA0, 0x88),
    SCRIPT_POSE(0x6C, 1, 7),
    SCRIPT_WALK(0x6D, 0xE3, 0x93, 7),
    SCRIPT_WALK(0x6E, 0xCD, 0x9F, 7),
    SCRIPT_WAIT_WALK(0x6E),
    SCRIPT_WALK(0x6C, 0xE6, 0xAA, 7),
    SCRIPT_POSE(0x6D, 1, 7),
    SCRIPT_WALK(0x6E, 0xBD, 0xA7, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_GAYALOOP, 1),
    SCRIPT_WAIT_WALK(0x6C),
    SCRIPT_POSE(0x6C, 1, 7),
    SCRIPT_POSE(0x6E, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x31, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 6, 0x35, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 7, 0x36, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 8, 0x6C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6C, 0xA0, 0x88, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(0x6D, 0xCD, 0x9F, 1),
    SCRIPT_WAIT_WALK(0x6D),
    SCRIPT_PLACE(0x6C, 0, 0),
    SCRIPT_POSE(0x6C, 1, 0),
    SCRIPT_WALK(0x6D, 0xA0, 0x88, 3),
    SCRIPT_WALK(0x6E, 0xCD, 0x9F, 5),
    SCRIPT_WAIT_WALK(0x6E),
    SCRIPT_WALK(0x6E, 0xA0, 0x88, 3),
    SCRIPT_WAIT_WALK(0x6E),
    SCRIPT_POSE(0xD, 1, 1),
    SCRIPT_POSE(0xE, 1, 7),
    SCRIPT_POSE(0x2E, 1, 2),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_POSE(0x32, 1, 7),
    SCRIPT_POSE(0x34, 1, 1),
    SCRIPT_POSE(0x35, 1, 6),
    SCRIPT_POSE(0x36, 1, 4),
    SCRIPT_POSE(0x39, 1, 6),
    SCRIPT_PLACE(0x6D, 0, 0),
    SCRIPT_POSE(0x6D, 1, 0),
    SCRIPT_PLACE(0x6E, 0, 0),
    SCRIPT_POSE(0x6E, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x31, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(1, 0xA, 0x35, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 0xB, 0x2E, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x29, 1),
    SCRIPT_COMMAND(0x323, 0x327, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 1, 3),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 0x3B, 5),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 0x11, 0xB, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 1, 3),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x10E, 0x11D, 3),
    SCRIPT_WALK(0xB, 0x12C, 0x12A, 3),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(1, 0xCD, 0xFD, 5),
    SCRIPT_WALK(0xB, 0xEA, 0x10B, 3),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(1, 0xE5, 0xF2, 5),
    SCRIPT_WALK(0xB, 0xCD, 0xFD, 5),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(1, 0x18B, 0xA0, 5),
    SCRIPT_WALK(0xB, 0x18B, 0xA0, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x214, 0x1E3, 0x2C6, 5),
    SCRIPT_END,
};
s16 script273[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x12A, 0x12B),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_PLACE(0xB, 0x111, 0x137),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_PLACE(0xD, 0x180, 0xFF),
    SCRIPT_POSE(0xD, 1, 1),
    SCRIPT_PLACE(0xE, 0x1B2, 0xF7),
    SCRIPT_POSE(0xE, 1, 7),
    SCRIPT_PLACE(0x2E, 0x18E, 0x144),
    SCRIPT_POSE(0x2E, 1, 2),
    SCRIPT_PLACE(0x31, 0x123, 0xE6),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x32, 0x160, 0x10F),
    SCRIPT_POSE(0x32, 1, 7),
    SCRIPT_PLACE(0x33, 0x172, 0x118),
    SCRIPT_POSE(0x33, 1, 3),
    SCRIPT_PLACE(0x34, 0x13F, 0xE5),
    SCRIPT_POSE(0x34, 1, 1),
    SCRIPT_PLACE(0x35, 0x108, 0xFE),
    SCRIPT_POSE(0x35, 1, 6),
    SCRIPT_PLACE(0x36, 0x12C, 0x105),
    SCRIPT_POSE(0x36, 1, 4),
    SCRIPT_PLACE(0x39, 0x16E, 0x144),
    SCRIPT_POSE(0x39, 1, 6),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x29, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xB, 1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xB, 0x6F, 0x187, 1),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_TALK(0, 4, 1, 3),
    SCRIPT_PLACE(0xB, 0, 0),
    SCRIPT_POSE(0xB, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(1, 0x77, 0x187, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
s16 script352[] = {
    SCRIPT_PLACE(2, 0x138, 0xCC),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0xB, 0x150, 0xC0),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(2), 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHOW_OBJECTS(2), 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xB, 0x150, 0xD6, 3),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0xB, 2),
    SCRIPT_POSE(0xB, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0xB, 0xE6, 0x10B, 7),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WALK(0xB, 0x127, 0x12C, 1),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(0xB, 0x60, 0x190, 1),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x118, 0xDC, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x118, 0x132, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
AnimFrame updateTileEffectFrames_0[] = {
    { 92, 5 }, { 93, 5 }, { 94, 3 }, { 95, 3 },
    { 94, 211 }, { 95, 3 }, { 94, 3 }, { 93, 4 },
    { 92, 4 }, { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_1[] = {
    { 0x12C, 80 }, { 81, 3 }, { 82, 3 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 83, 2 },
    { 84, 2 }, { 85, 2 }, { 86, 2 }, { 87, 2 },
    { 84, 2 }, { 88, 2 }, { 86, 2 }, { 87, 2 },
    { 84, 2 }, { 88, 2 }, { 89, 2 }, { 87, 2 },
    { 90, 2 }, { 88, 2 }, { 89, 2 }, { 87, 2 },
    { 90, 2 }, { 91, 2 }, { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_2[] = {
    { 0x12C, 32 }, { 50, 3 }, { 51, 3 }, { 52, 3 },
    { 53, 3 }, { 54, 3 }, { 55, 3 }, { 56, 3 },
    { 57, 3 }, { 58, 3 }, { 59, 3 }, { 60, 3 },
    { 61, 3 }, { 62, 8 }, { 63, 3 }, { 64, 3 },
    { 65, 3 }, { 66, 3 }, { 67, 3 }, { 68, 3 },
    { 69, 3 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 2 }, { 70, 2 }, { 71, 2 }, { 68, 2 },
    { 69, 4 }, { 70, 4 }, { 71, 4 }, { 72, 3 },
    { 73, 3 }, { 74, 3 }, { 75, 2 }, { 76, 2 },
    { 77, 3 }, { 78, 3 }, { 79, 3 }, { 78, 3 },
    { 79, 4 }, { 80, 4 }, { 255, 0x3E7 },
};
AnimFrame *updateTileEffectFrames[] = {
    updateTileEffectFrames_0, updateTileEffectFrames_1, updateTileEffectFrames_2,
};
s8 tileEffectSortY[] = {
    -1, 1, 2, 0,
};
s16 tileEffectPlaces[][2] = {
    { 217, 0x124 }, { 0x101, 0x110 }, { 0x129, 252 },
};
/* The sound of each of the three partners' download effects */
s32 tileEffectSounds[] = {
    SOUND_DIGI_EN0, SOUND_DIGI_EN1, SOUND_DIGI_EN2,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1E0, 0x198, 0x280, 0x98, 0x160, 0x1FB },
    { 0x1C0, 0x100, 0x1E8, 0x198, 0x2A0, 0x98, 0x170, 0x1FB },
    { 0x1C0, 0x100, 0x1F0, 0x198, 0x2C0, 0x98, 0x150, 0x1FA },
    { 0x1C0, 0x100, 0x1D8, 0x120, 0x260, 0x20, 0x160, 0x1FA },
    { 0x1C0, 0x100, 0x1E0, 0x120, 0x280, 0x20, 0x170, 0x1FA },
    { 0x1C0, 0x100, 0x1E8, 0x120, 0x2A0, 0x20, 0x150, 0x1F9 },
    { 0x1C0, 0x100, 0x1C0, 0x1A0, 0x200, 0xA0, 0x160, 0x1F9 },
    { 0x1C0, 0x100, 0x1F0, 0x120, 0x2C0, 0x20, 0x170, 0x1F9 },
    { 0x1C0, 0x100, 0x1C8, 0x1A8, 0x220, 0xA8, 0x150, 0x1F8 },
    { 0x1C0, 0x100, 0x1C0, 0x128, 0x200, 0x28, 0x160, 0x1F8 },
    { 0x1C0, 0x100, 0x1C8, 0x128, 0x220, 0x28, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1D0, 0x128, 0x240, 0x28, 0x140, 0x1F7 },
    { 0x1C0, 0x100, 0x1D8, 0x148, 0x260, 0x48, 0x150, 0x1F7 },
    { 0x1C0, 0x100, 0x1E0, 0x148, 0x280, 0x48, 0x160, 0x1F7 },
    { 0x1C0, 0x100, 0x1E8, 0x148, 0x2A0, 0x48, 0x170, 0x1F7 },
    { 0x1C0, 0x100, 0x1E0, 0x1B8, 0x280, 0xB8, 0x140, 0x1F6 },
    { 0x1C0, 0x100, 0x1E8, 0x1B8, 0x2A0, 0xB8, 0x150, 0x1F6 },
    { 0x1C0, 0x100, 0x1F0, 0x1B8, 0x2C0, 0xB8, 0x160, 0x1F6 },
    { 0x1C0, 0x100, 0x1C0, 0x1C0, 0x200, 0xC0, 0x170, 0x1F6 },
    { 0x140, 0x100, 0x178, 0x179, 0xE0, 0x79, 0x140, 0x1F5 },
    { 0x1C0, 0x100, 0x1F0, 0x148, 0x2C0, 0x48, 0x150, 0x1F5 },
    { 0x1C0, 0x100, 0x1C0, 0x150, 0x200, 0x50, 0x160, 0x1F5 },
    { 0x1C0, 0x100, 0x1D0, 0x1C0, 0x240, 0xC0, 0x170, 0x1F5 },
    { 0x1C0, 0x100, 0x1D8, 0x1C0, 0x260, 0xC0, 0x140, 0x1F4 },
    { 0x1C0, 0x100, 0x1C8, 0x1C8, 0x220, 0xC8, 0x150, 0x1F4 },
};
u16 actor0Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(2), 1, SPECIAL(1), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(2), 1, SPECIAL(2), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(2), 1, SPECIAL(0), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(2), 1, SPECIAL(1), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(2), 1, SPECIAL(0), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(2), 1, SPECIAL(2), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor40Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(2), 1, SPECIAL(2), 1, CODES_END };
u16 actor43Conditions[] = { PROGRESS(2), 1, SPECIAL(1), 1, CODES_END };
u16 actor44Conditions[] = { PROGRESS(2), 1, SPECIAL(0), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0xB, 5, 0, 0, 0 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0xC, 6, 0, 0, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0xD, 7, 0, 0, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0xD, 7, 384, 255, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0xE, 8, 0, 0, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0xE, 8, 434, 247, 7 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x20, 9, 0, 0, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x20, 9, 384, 255, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x22, 0xA, 0, 0, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x24, 0xB, 0, 0, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x24, 0xB, 434, 247, 7 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x2E, 0xC, 0, 0, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x2E, 0xC, 0, 0, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x2E, 0xC, 684, 363, 7 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x31, 0xD, 0, 0, 7 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x31, 0xD, 0, 0, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x31, 0xD, 361, 221, 7 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x31, 0xD, 361, 221, 7 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x32, 0xE, 0, 0, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0x33, 0xF, 0, 0, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x34, 0x10, 0, 0, 1 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x34, 0x10, 0, 0, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x35, 0x11, 0, 0, 1 };
FieldActorEntry actor28 = { actor28Conditions, NULL, 0x35, 0x11, 328, 260, 6 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x36, 0x12, 0, 0, 1 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x36, 0x12, 535, 307, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x39, 0x13, 0, 0, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x40, 0x14, 0, 0, 1 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x41, 0x15, 0, 0, 1 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0x41, 0x15, 0, 0, 1 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x41, 0x15, 305, 353, 5 };
FieldActorEntry actor36 = { actor36Conditions, NULL, 0x41, 0x15, 305, 353, 5 };
FieldActorEntry actor37 = { actor37Conditions, NULL, 0x42, 0x16, 0, 0, 1 };
FieldActorEntry actor38 = { actor38Conditions, NULL, 0x42, 0x16, 0, 0, 1 };
FieldActorEntry actor39 = { actor39Conditions, NULL, 0x6C, 0x17, 0, 0, 1 };
FieldActorEntry actor40 = { actor40Conditions, NULL, 0x6D, 0x18, 0, 0, 1 };
FieldActorEntry actor41 = { actor41Conditions, NULL, 0x6E, 0x19, 0, 0, 1 };
FieldActorEntry actor42 = { actor42Conditions, NULL, 0xB4, 0x1A, 0, 0, 1 };
FieldActorEntry actor43 = { actor43Conditions, NULL, 0xB5, 0x1B, 0, 0, 1 };
FieldActorEntry actor44 = { actor44Conditions, NULL, 0xB6, 0x1C, 0, 0, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    &actor8,
    &actor9,
    &actor10,
    &actor11,
    &actor12,
    &actor13,
    &actor14,
    &actor15,
    &actor16,
    &actor17,
    &actor18,
    &actor19,
    &actor20,
    &actor21,
    &actor22,
    &actor23,
    &actor24,
    &actor25,
    &actor26,
    &actor27,
    &actor28,
    &actor29,
    &actor30,
    &actor31,
    &actor32,
    &actor33,
    &actor34,
    &actor35,
    &actor36,
    &actor37,
    &actor38,
    &actor39,
    &actor40,
    &actor41,
    &actor42,
    &actor43,
    &actor44,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x1A, 2, 0, 3, 0x16, 0, 339, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x1A, 2, 0, 3, 0x16, 0, 372, 196, 0, 0 },
    { 1, 0, 0x40, 2, 0x1A, 2, 0, 3, 0x16, 0, 403, 244, 0, 0 },
    { 1, 0, 0x40, 2, 0x1A, 2, 0, 3, 0x16, 0, 435, 228, 0, 0 },
    { 1, 0, 0x40, 2, 0x60, 2, 0, 7, 0x12, 0, 386, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x60, 2, 0, 7, 0x12, 0, 546, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x61, 2, 0, 7, 0x12, 0, 573, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x61, 2, 0, 7, 0x12, 0, 613, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x62, 2, 0, 7, 0x12, 0, 151, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x62, 2, 0, 7, 0x12, 0, 411, 111, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 10, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 42, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 74, 137, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 290, 69, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 322, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 354, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 482, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 7, 0x12, 0, 514, 181, 0, 0 },
    { 1, 0, 0x40, 6, 0x62, 2, 0, 7, 0x12, 0, 119, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 135, 232, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 155, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 175, 212, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 195, 202, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 215, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 235, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 255, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 275, 162, 0, 0 },
    { 1, 0, 0x40, 6, 0x63, 2, 0, 7, 0x12, 0, 295, 152, 0, 0 },
    { 1, 0, 0x40, 6, 0xE, 0, 0, 0, 0, 0, 335, 141, 0, 0 },
    { 1, 0, 0x40, 6, 0xF, 0, 0, 0, 0, 0, 511, 229, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 2, 0, 3, 4, 0, 114, 179, 0, 0 },
    { 1, 0, 0x40, 6, 0x1C, 2, 0, 3, 4, 0, 264, 130, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x1D, 1, 0x1D, 0x1F, 0xA, 0, 28, 519, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x1D, 1, 0x1D, 0x1F, 0xA, 0, 81, 492, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x1D, 1, 0x1D, 0x1F, 0xA, 0, 183, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x1D, 1, 0x1D, 0x1F, 0xA, 0, 237, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 0xA, 0, 55, 506, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 0xA, 0, 100, 493, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 0xA, 0, 151, 461, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 0xA, 0, 202, 445, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 0xA, 0, 296, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 0xA, 0, 259, 428, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 0xA, 0, 366, 433, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 0xA, 0, 582, 411, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x26, 1, 0x26, 0x28, 0xA, 0, 530, 355, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x26, 1, 0x26, 0x28, 0xA, 0, 546, 354, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x29, 1, 0x29, 0x2B, 0xA, 0, 87, 303, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x29, 1, 0x29, 0x2B, 0xA, 0, 130, 322, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2C, 1, 0x2C, 0x2E, 0xA, 0, 61, 292, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2C, 1, 0x2C, 0x2E, 0xA, 0, 113, 328, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0xA, 0, 437, 427, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0xA, 0, 455, 421, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0xA, 0, 608, 405, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0xA, 0, 626, 396, 0, 0 },
    { 0, 1, 0x40, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 2, 0x40, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 3, 0x40, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 384, 259, 273, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 401, 250, 271, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 368, 251, 271, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 417, 243, 263, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 352, 242, 263, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 433, 233, 255, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 336, 229, 255, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 449, 227, 247, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 320, 227, 247, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 337, 219, 240, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 353, 208, 231, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 369, 203, 223, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 385, 192, 215, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 401, 187, 207, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 144, 110, 160, 0 },
    { 1, 0, 0x50, 4, 0x11, 0, 0, 0, 0, 0, 359, 148, 192, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 535, 236, 280, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 10, script10, EVENT_TEXT(3), NULL, endEvents10To12 },
    { 11, script11, EVENT_TEXT(4), NULL, endEvents10To12 },
    { 12, script12, EVENT_TEXT(5), NULL, endEvents10To12 },
    { 271, script271, EVENT_TEXT(0x18), NULL, endEvent271 },
    { 273, script273, EVENT_TEXT(0x1A), NULL, endEvent273 },
    { 352, script352, EVENT_TEXT(0x20), NULL, endEvent352 },
    { -1, NULL, 0, NULL, NULL },
};
