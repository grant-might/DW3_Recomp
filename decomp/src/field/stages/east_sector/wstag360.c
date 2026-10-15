/*
 * WSTAG360: Forest Inn, East Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1457, with Guardromon: applies 0x7C14, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1457(void) {
    FLAGS_00.applyAction(0x7C14, 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x210
#define STAGE_ARCHIVE 0x3C6
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x21F
#define STAGE_ARCHIVE 0x3D6
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x19300, 0x14800};
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

s16 script1457[] = {
    SCRIPT_WALK(2, 0x99, 0xB5, 3),
    SCRIPT_PLACE(0x15, 0x79, 0xA5),
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
    SCRIPT_LEAVE(0xC12, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x166, 0, 0x66, 0x150, 0x1FB },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x160, 0x1FB },
    { 0x140, 0x100, 0x168, 0x169, 0xA0, 0x69, 0x170, 0x1FB },
    { 0x140, 0x100, 0x15A, 0x16C, 0x68, 0x6C, 0x140, 0x1FA },
    { 0x140, 0x100, 0x148, 0x16D, 0x20, 0x6D, 0x150, 0x1FA },
    { 0x140, 0x100, 0x150, 0x16D, 0x40, 0x6D, 0x160, 0x1FA },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 8), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A1F, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 9), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(0x11), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0x1C, 0x45), 0, CODES_END };
u16 actor12Talk0Actions[] = { FLAG(0x1C, 0x45), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0x1C, 0x45), 1, CODES_END };
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
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x1C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x23F },
    { actor3Talk1Conditions, NULL, 0x2F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x242 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x241 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x240 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x23F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x23E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x23D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x23C },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x23B },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0x2E6 },
    { actor12Talk1Conditions, NULL, 0x327 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x243 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x15), 1, PROGRESS(6), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor12Conditions[] = { ITEM(0, 0x18F), 0, FLAG(0x1C, 0x44), 1, PROGRESS(6), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 345, 308, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 121, 165, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 6, 177, 153, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x40, 7, 177, 153, 3 };
FieldActorEntry actor13 = { NULL, NULL, 0x78, 8, 374, 268, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9D, 9, 177, 153, 3 };
FieldActorEntry actor15 = { NULL, NULL, 0xDB, 0xA, 346, 325, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 484, 223, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 57, 39, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 89, 23, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 203, 224, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 272, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 292, 139, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 305, 376, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 337, 393, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 467, 220, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 4, 0, 344, 109, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 4, 0, 321, 107, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 370, 108, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 289, 326, 340, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 294, 310, 332, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 322, 310, 325, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 337, 303, 317, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 351, 282, 309, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 375, 277, 301, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 383, 270, 293, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 400, 271, 285, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 415, 270, 282, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 77, 127, 175, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 109, 113, 158, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 141, 97, 142, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 174, 81, 127, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 257, 176, 209, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 255, 245, 263, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 239, 252, 272, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 420, 237, 278, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x222, 0x80, 0x31C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x224, 0x8C, 0xC2, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0xFA, 0x126, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x10B, 0x188, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1457, script1457, EVENT_TEXT(0x33), NULL, endEvent1457 },
    { -1, NULL, 0, NULL, NULL },
};
