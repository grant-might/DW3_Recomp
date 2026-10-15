/*
 * WSTAG235: Asuka Inn 1F, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x195
#define STAGE_ARCHIVE 0x3BE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x1A3
#define STAGE_ARCHIVE 0x3CE
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0xBC00, 0xB600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 7;
    FIELDSTG_state.music = MUSIC(7, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1200[] = {
    SCRIPT_WALK(2, 0xCF, 0xD0, 3),
    SCRIPT_PLACE(0x15, 0xAF, 0xC0),
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
    SCRIPT_LEAVE(0xC01, 0, 0, 0),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_JP
    0x9082,
#endif
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x168, 0x88, 0x68, 0x160, 0x1FF },
    { 0x140, 0x100, 0x140, 0x169, 0, 0x69, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x168, 0x68, 0x68, 0x160, 0x1FE },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x174, 0x16C, 0xD0, 0x6C, 0x170, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 8), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A1E, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 9), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(1), 0, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0x1A, 0x23), 0, CODES_END };
u16 actor12Talk0Actions[] = { FLAG(0x1A, 0x23), 1, CARD(0xB7), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0x1A, 0x23), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0x1C, 0x34), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0x1C, 0x34), 0, FLAG(0, 0), 0, CODES_END };
u16 actor13Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0x1C, 0x34), 0, FLAG(0, 0), 1, FLAG(0, 1), 0, CODES_END };
u16 actor13Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor13Talk3Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 0,
    CODES_END,
};
u16 actor13Talk3Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor13Talk4Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 0,
    CODES_END,
};
u16 actor13Talk4Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor13Talk5Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 1,
    FLAG(0, 4), 0,
    CODES_END,
};
u16 actor13Talk5Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor13Talk6Conditions[] = {
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 1,
    FLAG(0, 4), 1,
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    CODES_END,
};
u16 actor13Talk6Actions[] = { SPECIAL(0x8C), 1, FLAG(0x1C, 0x34), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0x1C, 0x34), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0x1C, 0x34), 0, FLAG(0, 0), 0, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0x1C, 0x34), 0, FLAG(0, 0), 1, FLAG(0, 1), 0, CODES_END };
u16 actor14Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor14Talk3Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 0,
    CODES_END,
};
u16 actor14Talk3Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor14Talk4Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 0,
    CODES_END,
};
u16 actor14Talk4Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor14Talk5Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 1,
    FLAG(0, 4), 0,
    FLAG(0x1C, 0x34), 0,
    CODES_END,
};
u16 actor14Talk5Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor14Talk6Conditions[] = {
    FLAG(0x1C, 0x34), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 1,
    FLAG(0, 4), 1,
    CODES_END,
};
u16 actor14Talk6Actions[] = { FLAG(0x1C, 0x34), 1, SPECIAL(0x8C), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x18C },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x18D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x18B },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x18A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x207 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x200 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x201 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x202 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x203 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x204 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x205 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1FE },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1FF },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0xB5 },
    { actor12Talk1Conditions, NULL, 0xB6 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0xB6 },
    { actor13Talk1Conditions, actor13Talk1Actions, 5 },
    { actor13Talk2Conditions, actor13Talk2Actions, 7 },
    { actor13Talk3Conditions, actor13Talk3Actions, 7 },
    { actor13Talk4Conditions, actor13Talk4Actions, 7 },
    { actor13Talk5Conditions, actor13Talk5Actions, 7 },
    { actor13Talk6Conditions, actor13Talk6Actions, 6 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0xB6 },
    { actor14Talk1Conditions, actor14Talk1Actions, 5 },
    { actor14Talk2Conditions, actor14Talk2Actions, 7 },
    { actor14Talk3Conditions, actor14Talk3Actions, 7 },
    { actor14Talk4Conditions, actor14Talk4Actions, 7 },
    { actor14Talk5Conditions, actor14Talk5Actions, 7 },
    { actor14Talk6Conditions, actor14Talk6Actions, 6 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x206 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor12Conditions[] = { FLAG(0x1A, 0x22), 1, FLAG(0x1A, 0x23), 0, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1A, 0x22), 0, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1A, 0x22), 1, FLAG(0x1A, 0x23), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 188, 154, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 175, 192, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x38, 6, 88, 332, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x38, 6, 88, 332, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x3F, 7, 145, 289, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x3F, 7, 145, 289, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x3F, 7, 145, 289, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9D, 8, 88, 332, 3 };
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
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 78, 77, 0, 0 },
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 97, 115, 0, 0 },
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 135, 231, 0, 0 },
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 141, 44, 0, 0 },
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 261, 63, 0, 0 },
    { 1, 0, 0x40, 2, 6, 2, 0, 1, 4, 0, 326, 95, 0, 0 },
    { 1, 0, 0x40, 2, 7, 2, 0, 1, 4, 0, 165, 76, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 256, 151, 184, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 215, 20, 86, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 144, 185, 200, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 125, 175, 194, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 148, 127, 167, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 240, 127, 162, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20C, 0x96, 0x114, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x2A0, 0x17A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20F, 0x15A, 0xBA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20B, 0x146, 0xBA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xC8, 0xEC, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xB8, 0x132, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1200, script1200, EVENT_TEXT(0x2A), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
