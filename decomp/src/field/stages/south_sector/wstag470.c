/*
 * WSTAG470: Swamp Inn, South Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x226
#define STAGE_ARCHIVE 0x2C6
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x235
#define STAGE_ARCHIVE 0x2D5
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 2;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1DB00, 0x13C00};
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

s16 script1215[] = {
    SCRIPT_WALK(2, 0xA0, 0x148, 3),
    SCRIPT_PLACE(0x15, 0x80, 0x138),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC03, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x195, 0xD8, 0x95, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B2, 0x173, 0x1C8, 0x73, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1B4, 0x147, 0x1D0, 0x47, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x1A3, 0x1C0, 0xA3, 0x170, 0x1FC },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 8), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A21, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 9), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(0xE), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0x1C, 0x43), 1, FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x16A },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x16B },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x169 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x168 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x21 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x111 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x115 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x116 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x118 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x119 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x11A },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x117 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x117 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x21 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, NULL, 0x115 },
    { actor16Talk1Conditions, NULL, 0x55 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0x326 },
    { actor17Talk1Conditions, NULL, 0x329 },
    { NULL, NULL, 0 },
};
u16 actor3Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(8), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 357, 278, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 128, 312, 7 };
FieldActorEntry actor2 = { NULL, NULL, 0x78, 6, 368, 232, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x79, 7, 289, 338, 7 };
FieldActorEntry actor18 = { NULL, NULL, 0xDB, 8, 368, 296, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 249, 225, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 311, 194, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 472, 215, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 6, 0, 193, 297, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 6, 0, 257, 265, 0, 0 },
    { 1, 0, 0xFF, 2, 0xD, 0, 0, 0, 0, 0, 133, 187, 0, 0 },
    { 1, 0, 0xFF, 2, 0xD, 0, 0, 0, 0, 0, 277, 116, 0, 0 },
    { 1, 0, 0xFF, 2, 0xE, 0, 0, 0, 0, 0, 421, 44, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 265, 384, 412, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 367, 333, 361, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 288, 299, 318, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 288, 305, 327, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 306, 299, 320, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 321, 290, 311, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 338, 282, 303, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 356, 275, 295, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 370, 266, 287, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 386, 258, 279, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 402, 250, 271, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 418, 242, 263, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 434, 235, 255, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x237, 0x1BE, 0xBA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x243, 0xC0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x252, 0x116, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0xB1, 0x149, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0xC0, 0x17E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1215, script1215, EVENT_TEXT(0x12), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
