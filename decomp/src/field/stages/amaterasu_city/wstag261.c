/*
 * WSTAG261: Cargo Tower, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG260.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern s16 tileLiftShake[];

#include "common/update_tile_lift.inc.c"

/* Runs a script command of the lift's: moves it up or down */
void handleCommand827(Lift *task, s32 id) {
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

/* Creates the lift (updateTileLift), raised when flag 0x1C3D is set */
Lift *createCommand827(s32 id) {
    Lift *task = createTaskWithId(updateTileLift, sizeof(Lift), 0, id);

    if (FLAGS_00.checkCondition(FLAG(0x1C, 0x3D), 1)) {
        task->raised = 1;
    } else {
        task->raised = 0;
    }
    return task;
}

/* The stage task: creates the object of map object 0x33B */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        children[0] = createCommand827(0x33B);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0 };

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x517
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x527
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0204 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10300, 0x18500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x42;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.music = MUSIC(0x42, 2);
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1321[] = {
    SCRIPT_WALK(2, 0xBF, 0x190, 3),
    SCRIPT_PLACE(0x3F, 0xB0, 0x188),
    SCRIPT_POSE(0x3F, 1, 0),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH02, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xEC, 0x178, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x108, 0x184, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_PLACE(0x3F, 0xB0, 0x109),
    SCRIPT_POSE(0x3F, 1, 0),
    SCRIPT_COMMAND(0x33B, LIFT_UP, 2),
    SCRIPT_WAIT(0x12C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1326[] = {
    SCRIPT_WALK(2, 0xBF, 0x111, 3),
    SCRIPT_PLACE(0x3F, 0xB0, 0x109),
    SCRIPT_POSE(0x3F, 1, 0),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH02, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xEF, 0xF9, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x108, 0x105, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_PLACE(0x3F, 0xB0, 0x188),
    SCRIPT_POSE(0x3F, 1, 0),
    SCRIPT_COMMAND(0x33B, LIFT_DOWN, 2),
    SCRIPT_WAIT(0x12C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 tileLiftShake[] = {
    1, 2, 1, 0, -1, -2, -1, 0,
    0x3E8, 0,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1C, 0x3D), 0, CODES_END };
u16 actor0Talk0Actions[] = { START_EVENT(0x62), 1, FLAG(0x1C, 0x3D), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1C, 0x3D), 1, CODES_END };
u16 actor0Talk1Actions[] = { START_EVENT(0x63), 1, FLAG(0x1C, 0x3D), 0, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1C, 0x3D), 0, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x62), 1, FLAG(0x1C, 0x3D), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1C, 0x3D), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(0x63), 1, FLAG(0x1C, 0x3D), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x203 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x204 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x203 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x204 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x1C, 0x3D), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(0x1C, 0x3D), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x3F, 4, 176, 392, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x3F, 4, 176, 265, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 1, 0x44, 2, 0x3A, 2, 0, 1, 4, 0, 312, 282, 0, 0 },
    { 1, 2, 0x40, 2, 0x32, 1, 0x32, 0x39, 4, 0, 135, 358, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 0, 0, 0, 0, 0, 328, 194, 0, 0 },
    { 1, 0, 0x40, 6, 1, 0, 0, 0, 0, 0, 192, 417, 0, 0 },
    { 1, 3, 0xCD, 6, 0, 0, 0, 0, 0, 0, 160, 338, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 0, 0, 0, 0, 0, 328, 272, 0, 0 },
    { 1, 0, 0x48, 0xA, 0x14, 0, 0, 0, 0, 0, 128, 202, 0, 0 },
    { 1, 0, 0x49, 0xA, 0x15, 0, 0, 0, 0, 0, 200, 183, 0, 0 },
    { 1, 0, 0x40, 4, 0x3D, 0, 0, 0, 0, 0, 336, 166, 193, 0 },
    { 1, 0, 0x40, 8, 0x3F, 0, 0, 0, 0, 0, 336, 244, 277, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x271, 0x368, 0xF4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x281, 0x2FE, 0xA5, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 8, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1321, script1321, EVENT_TEXT(0x2E), NULL, NULL },
    { 1326, script1326, EVENT_TEXT(0x2F), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
