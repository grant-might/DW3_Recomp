/*
 * WSTAG934: Cargo Tower, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 639 starts it
 * instead of WSTAG261.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern s16 tileLiftShake[];

#include "common/update_tile_lift.inc.c"

/* Runs a script command of the lift's: moves it up or down */
void handleTileLift(Lift *task, s32 id) {
    if (task != NULL) {
        switch (id) {
        case LIFT_UP:
            task->setState(task, TASK_DONE);
            task->raised = 0;
            break;
        case LIFT_DOWN:
            task->setState(task, TASK_DONE);
            task->raised = 1;
            break;
        }
    }
}

/* Creates the lift (updateTileLift), raised when flag 0x1C3D is set; nothing in the game calls it */
Lift *createTileLift(s32 id) {
    Lift *task = createTaskWithId(updateTileLift, sizeof(Lift), 0, id);

    if (FLAGS_00.checkCondition(FLAG(0x1C, 0x3D), 1)) {
        task->raised = 1;
    } else {
        task->raised = 0;
    }
    return task;
}

#include "common/update_stage.inc.c"

/* Creates the stage task (updateStage) of the given owner */
StageTask *startStage(void *owner) {
    StageTask *task = createTask(updateStage, 0x58, 8);

    task->owner = owner;
    stageFuncs.setup();
    return task;
}

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0 };

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8FB
#define STAGE_FILE_8 0x1AC
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8BF
#define STAGE_FILE_8 0x19E
#endif
/* Sets the stage up: its map and actors, playing the ambience ENV_0204 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x6700, 0x12300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x42;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.music = MUSIC(0x42, 2);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

s16 tileLiftShake[] = {
    1, 2, 1, 0, -1, -2, -1, 0,
    0x3E8,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU
    5,
#elif VERSION_JP
    0x404,
#endif
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x18C, 0x1DD, 0x130, 0xDD, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1EA, 0x13C, 0x2A8, 0x3C, 0x170, 0x1FE },
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor1Talk1Actions[] = { ITEM(0, 0x192), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x41 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x3E },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x3F },
    { actor1Talk2Conditions, NULL, 0x40 },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x2D, 4, 271, 361, 5 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x67, 5, 191, 400, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 1, 0x44, 2, 0x52, 2, 0, 1, 4, 0, 312, 282, 0, 0 },
    { 1, 2, 0x40, 2, 0x4A, 1, 0x4A, 0x51, 4, 0, 135, 358, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x43, 4, 0, 357, 161, 0, 0 },
    { 1, 0, 0x40, 6, 1, 0, 0, 0, 0, 0, 192, 417, 0, 0 },
    { 1, 0, 0x40, 6, 2, 1, 2, 7, 4, 0, 357, 161, 0, 0 },
    { 1, 3, 0xCD, 6, 0, 0, 0, 0, 0, 0, 160, 338, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x49, 4, 0, 357, 241, 0, 0 },
    { 1, 0, 0x40, 0xA, 8, 1, 8, 0xD, 4, 0, 357, 241, 0, 0 },
    { 1, 0, 0x40, 0xA, 0xF, 1, 0xF, 0x13, 4, 0, 62, 326, 0, 0 },
    { 1, 0, 0x48, 0xA, 0x14, 0, 0, 0, 0, 0, 128, 202, 0, 0 },
    { 1, 0, 0x49, 0xA, 0x15, 0, 0, 0, 0, 0, 200, 183, 0, 0 },
    { 1, 0, 0x40, 4, 0x32, 1, 0x32, 0x37, 4, 0, 357, 161, 200, 0 },
    { 1, 0, 0x40, 8, 0x38, 1, 0x38, 0x3D, 4, 0, 357, 241, 280, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x271, 0x368, 0xF4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x281, 0x2FE, 0xA5, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 8, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
