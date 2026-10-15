/*
 * WSTAG226: Login Room, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG225.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame *updateTileEffectFrames[];
extern s8 tileEffectSortY[];
extern s8 tileEffectY[];
extern s16 tileEffectPlaces[][2];

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
                        t->y = task->y + tileEffectY[n];
                        t->sortY = task->y + tileEffectSortY[n];
                        n++;
                    }
                }
                resetTileEffect(task);
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

/* Starts the effect at the place of map 0 or 1 */
void startTileEffect(StageTileEffect *task, s32 id) {
    s32 i;

    if (task != NULL) {
        i = 0;
        switch (id) {
        case 1:
            i = 1;
            /* fallthrough */
        case 0:
            task->x = tileEffectPlaces[i][0];
            task->y = tileEffectPlaces[i][1];
            task->setSubstate(task, 1);
            break;
        }
    }
}

#include "common/create_tile_effect.inc.c"

/* Creates the effect of map objects that startTileEffect sets off (updateTileEffect) */
void *createTileEffectTask(void) {
    return createTask(updateTileEffect, sizeof(StageTileEffect), 0);
}

/* Creates the stage helper task and the event object of progress 0x25 when flag 0x4060 is set */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[1] = createTileEffectTask();
        do {
            if (GAME.progress == 0x25 && FLAGS_00.checkCondition(FLAG(0x40, 0x60), 1)) {
                children[0] = FIELDSTG_startEvent(0x3A7);
                break;
            }
        } while (0);
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

/* After event 935, with Teddy, Kail and Kurt: moves the story on to its step 0x26 */
void endEvent935(void) {
    GAME.progress = 0x26;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x4A8
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x4B8
#endif
/*
 * Sets the stage up: its map, actors and events, playing KANRIBGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10100, 0x16D00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script935[] = {
    SCRIPT_PLACE(1, 0x58, 0x1CC),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0x9D, 0x109, 0x18D),
    SCRIPT_POSE(0x9D, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x78, 0x1BC, 5),
    SCRIPT_PLACE(0xC, 0x58, 0x1CC),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_WALK(1, 0xA0, 0x1A8, 5),
    SCRIPT_WALK(0xC, 0x80, 0x1B8, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0xF0, 0x180, 5),
    SCRIPT_WALK(0xC, 0xD0, 0x190, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(1, 0x3A, 7),
    SCRIPT_WALK(0xC, 0xD0, 0x178, 4),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(1, 0x2F, 7),
    SCRIPT_WALK(0xC, 0xFE, 0x161, 5),
    SCRIPT_COMMAND(0x323, 0x327, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(0xC, 0x3A, 3),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 1, 3),
    SCRIPT_POSE(0xC, 0x3A, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_TALK(0, 2, 0xC, 0),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_POSE(0xC, 7, 0),
    SCRIPT_POSE(0x9D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_PLACE(0xB, 0x58, 0x1CC),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_POSE(0xC, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xB, 0x78, 0x1BC, 5),
    SCRIPT_PLACE(0x13D, 0x58, 0x1CC),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(0xB, 0xD0, 0x190, 5),
    SCRIPT_POSE(0x9D, 1, 2),
    SCRIPT_WALK(0x13D, 0xB0, 0x1A0, 5),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xB),
    SCRIPT_TALK(0, 0xB, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_POSE(0x9D, 1, 1),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0xEA, 0x175),
    SCRIPT_TALK(1, 3, 1, 3),
    SCRIPT_TALK(0, 4, 0xC, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WALK(0xB, 0xD0, 0x162, 4),
    SCRIPT_POSE(0x9D, 1, 3),
    SCRIPT_WALK(0x13D, 0xD0, 0x190, 5),
    SCRIPT_WAIT_WALK(0x13D),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WALK(0x13D, 0xD0, 0x180, 6),
    SCRIPT_WAIT_WALK(0x13D),
    SCRIPT_TALK(0, 5, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 6),
    SCRIPT_POSE(0x9D, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0xB, 0),
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(0xB, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x13D, 3),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0xC, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_POSE(0xC, 7, 1),
    SCRIPT_POSE(0x9D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0xC, 2),
    SCRIPT_POSE(0xC, 7, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_POSE(0x9D, 1, 3),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x58, 0x1CC, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_POSE(0x9D, 1, 1),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LEAVE(0x270, 0x3E8, 0xEC, 1),
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
    0, -0x2F, -0x2F, 0,
};
s16 tileEffectPlaces[][2] = {
    { 0x150, 0x112 }, { 112, 0x112 },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x160, 0x180, 0x80, 0x80, 0x170, 0x1FE },
    { 0x140, 0x100, 0x168, 0x180, 0xA0, 0x80, 0x160, 0x1FD },
    { 0x140, 0x100, 0x160, 0x160, 0x80, 0x60, 0x170, 0x1FD },
    { 0x140, 0x100, 0x152, 0x198, 0x48, 0x98, 0x150, 0x1FC },
    { 0x140, 0x100, 0x15A, 0x198, 0x68, 0x98, 0x160, 0x1FC },
    { 0x140, 0x100, 0x168, 0x160, 0xA0, 0x60, 0x170, 0x1FC },
    { 0x140, 0x100, 0x14A, 0x190, 0x28, 0x90, 0x160, 0x1FB },
    { 0x140, 0x100, 0x14A, 0x170, 0x28, 0x70, 0x170, 0x1FB },
};
u16 actor3Talk0Conditions[] = { FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0x1A, 0xA), 1, CODES_END };
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x1B4 },
    { actor3Talk1Conditions, NULL, 0x209 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x5B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x5D },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x201 },
    { actor6Talk1Conditions, NULL, 0x202 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x5A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x5C },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x1FA },
    { actor9Talk1Conditions, NULL, 0x208 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1FB },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x25), 1, FLAG(0x40, 0x60), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x25), 1, FLAG(0x40, 0x60), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x25), 1, FLAG(0x40, 0x60), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(4, 0x2B), 1, PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1C), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x25), 1, FLAG(0x40, 0x60), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0xC, 6, 0, 0, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xC, 6, 208, 376, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x20, 7, 265, 397, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x20, 7, 265, 397, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x68, 8, 255, 352, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 9, 265, 397, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 9, 265, 397, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xB2, 0xA, 277, 363, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x13D, 0xB, 255, 352, 7 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x13D, 0xB, 0, 0, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 1, 0x50, 6, 0x46, 0, 0, 0, 0, 0, 336, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 116, 258, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 228, 282, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 340, 258, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x42, 6, 0, 355, 346, 0, 0 },
    { 0, 2, 0x50, 4, 0x48, 0, 2, 0, 0, 0, 336, 227, 301, 0 },
    { 0, 3, 0x50, 4, 0x55, 0, 0, 0, 0, 0, 336, 227, 294, 0 },
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
FieldEvent stageEvents[] = {
    { 935, script935, EVENT_TEXT(0x29), NULL, endEvent935 },
    { -1, NULL, 0, NULL, NULL },
};
