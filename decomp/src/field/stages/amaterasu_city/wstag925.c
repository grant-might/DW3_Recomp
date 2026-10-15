/*
 * WSTAG925: Login Room, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 630 starts it
 * instead of WSTAG226.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame *updateTileEffectFrames[];
extern s8 tileEffectSortY[];
extern s8 tileEffectY[];

/* Creates the event object of flag 0x40CD while it is clear */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (FLAGS_00.checkCondition(FLAG(0x40, 0xCD), 0)) {
            children[0] = FIELDSTG_startEvent(0x640);
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
 * After event 1600, with the Tour Leader (Digimon Online's practice): heals
 * the party's HP and MP (the European special action 0x53) and keeps it from
 * playing again
 */
void endEvent1600(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xCD), 1);
    FLAGS_00.applyAction(SPECIAL(0x53), 1);
}

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8E9
#define STAGE_FILE_8 0x19E
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8A4
#define STAGE_FILE_8 0x190
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0011 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xFD00, 0x17700};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x33;
    FIELDSTG_state.music = MUSIC(0x33, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/step_animation_once.inc.c"

#include "common/reset_tile_effect.inc.c"

/* Moves the map objects with animations 1 to 3 to (x, y) and plays their animations once with a sound, then hides them and kills itself */
void updateTileEffect(StageTileEffect *task) {
    StageTile *object;
    StageTile *tile;
    s32 i;
    s32 j;
    s32 k;
    s32 done;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        resetTileEffect(task);
        task->nextState(task);
        task->setSubstate(task, 1);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            switch (task->step) {
            case 0:
                i = 0;
                for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
                    if (object->anim >= 1 && object->anim <= 3) {
                        task->anims[i].tile = object;
                        object->x = task->x;
                        object->y = task->y + tileEffectY[i];
                        object->sortY = task->y + tileEffectSortY[i];
                        i++;
                    }
                }
                SOUND.playSound(SOUND_LOGINEF);
                task->nextStep(task);
                /* fallthrough */
            case 1:
                done = 0;
                for (j = 0; j < 3; j++) {
                    tile = task->anims[j].tile;
                    frame = stepAnimationOnce(&task->anims[j], updateTileEffectFrames[j], 0);
                    switch (frame) {
                    case 0xFF:
                        done++;
                        /* fallthrough */
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
                for (k = 0; k < 3; k++) {
                    task->anims[k].tile->visible = 0;
                }
                resetTileEffect(task);
                task->setState(task, TASK_KILL);
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

/* Creates the StageTileEffect of updateTileEffect at (0x150, 0x112) with the given id */
void *createCommand855(s32 id) {
    StageTileEffect *task = createTaskWithId(updateTileEffect, sizeof(StageTileEffect), 0, id);

    task->x = 0x150;
    task->y = 0x112;
    return task;
}

/* Defined below, after the tables that use them */
extern s16 script1600[];

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14A, 0x1B8, 0x28, 0xB8, 0x170, 0x1FE },
    { 0x140, 0x100, 0x170, 0x1B0, 0xC0, 0xB0, 0x160, 0x1FD },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x26 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x40, 0xCD), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0xD, 5, 265, 397, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 1, 0x50, 6, 0x46, 0, 0, 0, 0, 0, 112, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 116, 258, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 228, 282, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 340, 258, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 355, 346, 0, 0 },
    { 0, 2, 0x50, 4, 0x48, 0, 0, 0, 0, 0, 112, 227, 301, 0 },
    { 0, 3, 0x50, 4, 0x55, 0, 0, 0, 0, 0, 112, 227, 294, 0 },
    { 1, 0, 0x40, 4, 0x3B, 1, 0x3B, 0x3E, 6, 0, 116, 274, 300, 0 },
    { 1, 0, 0x40, 4, 0x3B, 1, 0x3B, 0x3E, 6, 0, 228, 298, 324, 0 },
    { 1, 0, 0x40, 4, 0x3B, 1, 0x3B, 0x3E, 6, 0, 340, 274, 300, 0 },
    { 1, 0, 0x40, 4, 0x3B, 1, 0x3B, 0x3E, 6, 0, 355, 362, 388, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0x2DA, 0x17E, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1600, script1600, EVENT_TEXT(0), NULL, endEvent1600 },
    { -1, NULL, 0, NULL, NULL },
};
s16 script1600[] = {
    SCRIPT_LOOK_AT(1, 0x11E, 0x127),
    SCRIPT_PLACE(1, 0, 0),
    SCRIPT_POSE(1, 1, 0),
    SCRIPT_PLACE(0xD, 0x10F, 0x191),
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_WAIT(0x78),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x357, 0x335, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_WAIT(0x5A),
    SCRIPT_PLACE(1, 0x170, 0x11F),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x150, 0x130, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_TALK(0, 1, 1, 0),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x130, 0x160, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WALK(1, 0xF0, 0x180, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0xD, 2),
    SCRIPT_POSE(0xD, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 1, 1),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x50, 0x1D0, 1),
    SCRIPT_POSE(0xD, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0x273, 0x2DA, 0x17E, 1),
    SCRIPT_END,
};
AnimFrame updateTileEffectFrames_0[] = {
    { 70, 4 }, { 71, 4 }, { 70, 4 }, { 71, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_2[] = {
    { 0x12C, 56 }, { 85, 6 }, { 86, 6 }, { 87, 4 },
    { 88, 4 }, { 89, 4 }, { 90, 4 }, { 88, 4 },
    { 89, 4 }, { 90, 4 }, { 88, 4 }, { 89, 4 },
    { 90, 4 }, { 88, 4 }, { 89, 4 }, { 90, 4 },
    { 88, 4 }, { 89, 4 }, { 90, 4 }, { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_1[] = {
    { 0x12C, 16 }, { 72, 4 }, { 73, 4 }, { 74, 4 },
    { 73, 4 }, { 75, 6 }, { 76, 6 }, { 77, 14 },
    { 78, 114 }, { 79, 6 }, { 80, 6 }, { 81, 6 },
    { 82, 6 }, { 83, 6 }, { 84, 8 }, { 255, 0x3E7 },
};
AnimFrame *updateTileEffectFrames[] = {
    updateTileEffectFrames_0, updateTileEffectFrames_1, updateTileEffectFrames_2,
};
s8 tileEffectSortY[] = {
    30, 30, 30, 0,
};
s8 tileEffectY[] = {
    0, -47, -47,
};
