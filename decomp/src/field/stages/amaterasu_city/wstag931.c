/*
 * WSTAG931: Wedge's Shop, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 636 starts it
 * instead of WSTAG246.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8F5
#define STAGE_FILE_8 0x1A6
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8B6
#define STAGE_FILE_8 0x198
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x15A00, 0x18E00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 7;
    FIELDSTG_state.music = MUSIC(7, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

/* Defined below, after the tables that use them */
extern s16 script1614[];

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F4, 0x140, 0x2D0, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x162, 0x1DC, 0x88, 0xDC, 0x160, 0x1FF },
    { 0x1C0, 0x100, 0x1CE, 0x1A4, 0x238, 0xA4, 0x170, 0x1FF },
    { 0x140, 0x100, 0x174, 0x1DC, 0xD0, 0xDC, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1D6, 0x1A9, 0x258, 0xA9, 0x160, 0x1FE },
};
u16 actor0Talk0Actions[] = { 0x7A11, 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7A0F, 1, CODES_END };
u16 actor3Talk0Actions[] = { 0x7A10, 1, CODES_END };
u16 actor4Talk0Conditions[] = { ITEM(0, 0x25), 0, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(0, 0x25), 1, FLAG(0x18, 7), 0, CODES_END };
u16 actor4Talk1Actions[] = { START_EVENT(0x71), 1, FLAG(0x18, 7), 1, CODES_END };
u16 actor4Talk2Conditions[] = { ITEM(0, 0x25), 1, FLAG(0x18, 7), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x33 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x32 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x86 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x83 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x84 },
    { actor4Talk2Conditions, NULL, 0x85 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x85 },
    { NULL, NULL, 0 },
};
u16 actor4Conditions[] = { FLAG(0x18, 7), 0, CODES_END };
u16 actor5Conditions[] = { FLAG(0x18, 7), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 449, 281, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 304, 369, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x34, 6, 287, 432, 3 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0xB7, 7, 359, 308, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xFF, 8, 152, 397, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xFF, 8, 144, 352, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 384, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 492, 137, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 548, 165, 0, 0 },
    { 1, 0, 0x40, 6, 3, 1, 3, 8, 4, 0, 485, 145, 0, 0 },
    { 1, 0, 0x40, 6, 9, 1, 9, 0xE, 4, 0, 541, 172, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x12, 0, 0, 0, 0, 0, 304, 99, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x13, 0, 0, 0, 0, 0, 64, 285, 0, 0 },
    { 1, 0, 0xA0, 4, 0, 0, 0, 0, 0, 0, 144, 257, 380, 0 },
    { 1, 0, 0xA0, 4, 1, 0, 0, 0, 0, 0, 256, 257, 337, 0 },
    { 1, 0, 0xA0, 4, 2, 0, 0, 0, 0, 0, 384, 261, 287, 0 },
    { 1, 0, 0x60, 4, 0xF, 0, 0, 0, 0, 0, 240, 73, 150, 0 },
    { 1, 0, 0x60, 4, 0x10, 0, 0, 0, 0, 0, 0, 255, 335, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x1E0, 0x1DA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x240, 0x1AA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27D, 0x218, 0xE4, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27D, 0x128, 0x19C, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x11F, 0xBA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x110, 0xFE, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1614, script1614, EVENT_TEXT(7), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
s16 script1614[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xB0, 0x198, 3),
    SCRIPT_PLACE(0xFF, 0x98, 0x18D),
    SCRIPT_POSE(0xFF, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xFF),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xFF),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xFF, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xFF, 0x68, 0x174, 3),
    SCRIPT_WAIT_WALK(0xFF),
    SCRIPT_WALK(0xFF, 0x90, 0x160, 5),
    SCRIPT_WAIT_WALK(0xFF),
    SCRIPT_POSE(0xFF, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
