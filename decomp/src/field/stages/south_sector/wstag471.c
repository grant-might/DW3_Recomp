/*
 * WSTAG471: Swamp Inn, South Sector. On the Amaterasu server; its Asuka server
 * twin is WSTAG470.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1461, with Guardromon: applies 0x7C18, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1461(void) {
    FLAGS_00.applyAction(0x7C18, 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x56D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x57D
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1D400, 0x15400};
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

s16 script1461[] = {
    SCRIPT_WALK(2, 0xA0, 0x148, 3),
    SCRIPT_PLACE(0x15, 0x80, 0x138),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC16, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x170, 0x158, 0xC0, 0x58, 0x160, 0x1FF },
    { 0x140, 0x100, 0x172, 0x100, 0xC8, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x170, 0x178, 0xC0, 0x78, 0x160, 0x1FE },
    { 0x140, 0x100, 0x172, 0x130, 0xC8, 0x30, 0x170, 0x1FE },
    { 0x140, 0x100, 0x150, 0x196, 0x40, 0x96, 0x160, 0x1FD },
    { 0x140, 0x100, 0x158, 0x196, 0x60, 0x96, 0x170, 0x1FD },
    { 0x140, 0x100, 0x170, 0x198, 0xC0, 0x98, 0x160, 0x1FC },
    { 0x140, 0x100, 0x160, 0x19C, 0x80, 0x9C, 0x170, 0x1FC },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Actions[] = { 0x7A2B, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x67), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2CE },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xA7 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xAA },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xAC },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xAD },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xA6 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xB0 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xA8 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xAF },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 357, 278, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 128, 312, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 6, 496, 321, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 7, 557, 151, 1 };
FieldActorEntry actor4 = { NULL, NULL, 0x78, 8, 368, 232, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x79, 9, 289, 338, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x79, 9, 289, 338, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x79, 9, 289, 338, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x79, 9, 289, 338, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x79, 9, 289, 338, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 0xA, 557, 151, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 0xA, 557, 151, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 0xB, 496, 321, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9E, 0xB, 496, 321, 1 };
FieldActorEntry actor14 = { NULL, NULL, 0xDB, 0xC, 368, 296, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 249, 225, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 311, 194, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 472, 215, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 6, 0, 193, 297, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 6, 0, 257, 265, 0, 0 },
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A4, 0x1BE, 0xBA, 7, 0, 0, 0 },
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
    { 1461, script1461, EVENT_TEXT(0x31), NULL, endEvent1461 },
    { -1, NULL, 0, NULL, NULL },
};
