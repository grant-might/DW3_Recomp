/*
 * WSTAG731: Kulon Weapons, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG730.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateTileAnimsFrames[];

#include "common/step_looping_animation.inc.c"

#include "common/update_tile_anims1_once.inc.c"

#include "common/create_tile_anims_54.inc.c"

#include "common/update_stage_tile_anims.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x6B8
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x6C7
#endif
/* Sets the stage up: its map, actors and events, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xCE00, 0xDD00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1236[] = {
    SCRIPT_WALK(2, 0x1AD, 0xCE, 5),
    SCRIPT_PLACE(0x15, 0x1CD, 0xBE),
    SCRIPT_POSE(0x15, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 3),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC10, 0, 0, 0),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x350,
#elif VERSION_JP
    0x100,
#endif
};
AnimFrame updateTileAnimsFrames[] = {
    { 53, 8 }, { 54, 8 }, { 55, 8 }, { 56, 4 },
    { 57, 40 }, { 58, 8 }, { 255, 0 },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x172, 0x13C, 0xC8, 0x3C, 0x170, 0x1FC },
    { 0x140, 0x100, 0x160, 0x1D4, 0x80, 0xD4, 0x150, 0x1FB },
};
u16 actor0Talk0Actions[] = { START_EVENT(0x16), 1, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(3, 0x9C), 1, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 1, ITEM(2, 0x98), 0, CODES_END };
u16 actor2Talk3Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 1, ITEM(2, 0x98), 1, CODES_END };
u16 actor2Talk3Actions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x9B), 0,
    ITEM(2, 0x98), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor4Talk0Conditions[] = { ITEM(3, 0x90), 1, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk2Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 1, ITEM(2, 0x8C), 0, CODES_END };
u16 actor4Talk3Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 1, ITEM(2, 0x8C), 1, CODES_END };
u16 actor4Talk3Actions[] = {
    ITEM(3, 0x90), 1,
    ITEM(3, 0x8F), 0,
    ITEM(2, 0x8C), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor6Talk0Conditions[] = { ITEM(3, 0x77), 1, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 2), 0, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor6Talk2Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 2), 1, ITEM(2, 0x73), 0, CODES_END };
u16 actor6Talk3Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 2), 1, ITEM(2, 0x73), 1, CODES_END };
u16 actor6Talk3Actions[] = {
    ITEM(3, 0x77), 1,
    ITEM(3, 0x76), 0,
    ITEM(2, 0x73), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor8Talk0Conditions[] = { ITEM(3, 0x83), 1, CODES_END };
u16 actor8Talk1Conditions[] = { ITEM(3, 0x83), 0, FLAG(0, 3), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor8Talk2Conditions[] = { ITEM(3, 0x83), 0, FLAG(0, 3), 1, ITEM(2, 0x7F), 0, CODES_END };
u16 actor8Talk3Conditions[] = { ITEM(3, 0x83), 0, FLAG(0, 3), 1, ITEM(2, 0x7F), 1, CODES_END };
u16 actor8Talk3Actions[] = {
    ITEM(3, 0x83), 1,
    ITEM(3, 0x82), 0,
    ITEM(2, 0x7F), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor10Talk0Conditions[] = { ITEM(3, 0x69), 1, CODES_END };
u16 actor10Talk1Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 4), 0, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor10Talk2Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 4), 1, ITEM(2, 0x65), 0, CODES_END };
u16 actor10Talk3Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 4), 1, ITEM(2, 0x65), 1, CODES_END };
u16 actor10Talk3Actions[] = {
    ITEM(3, 0x69), 1,
    ITEM(3, 0x68), 0,
    ITEM(2, 0x65), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x2D1 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x30A },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x30B },
    { actor2Talk2Conditions, NULL, 0x30C },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x30D },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x30E },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x30F },
    { actor4Talk2Conditions, NULL, 0x310 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x311 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x317 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x312 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x313 },
    { actor6Talk2Conditions, NULL, 0x314 },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x318 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x319 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x31A },
    { actor8Talk2Conditions, NULL, 0x31B },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x31C },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x31D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0x31F },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x320 },
    { actor10Talk2Conditions, NULL, 0x321 },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x322 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x31E },
    { NULL, NULL, 0 },
};
u16 actor1Conditions[] = { ITEM(3, 0x9B), 0, ITEM(3, 0x9C), 0, CODES_END };
u16 actor2Conditions[] = { ITEM(3, 0x9B), 1, ITEM(3, 0x9C), 0, CODES_END };
u16 actor3Conditions[] = { ITEM(3, 0x8F), 0, ITEM(3, 0x9C), 1, ITEM(3, 0x90), 0, CODES_END };
u16 actor4Conditions[] = { ITEM(3, 0x9C), 1, ITEM(3, 0x8F), 1, ITEM(3, 0x90), 0, CODES_END };
u16 actor5Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x76), 0,
    ITEM(3, 0x77), 0,
    CODES_END,
};
u16 actor6Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x76), 1,
    ITEM(3, 0x77), 0,
    CODES_END,
};
u16 actor7Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x77), 1,
    ITEM(3, 0x82), 0,
    ITEM(3, 0x83), 0,
    CODES_END,
};
u16 actor8Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x77), 1,
    ITEM(3, 0x82), 1,
    ITEM(3, 0x83), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    ITEM(3, 0x68), 0,
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x77), 1,
    ITEM(3, 0x83), 1,
    ITEM(3, 0x69), 0,
    CODES_END,
};
u16 actor10Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x77), 1,
    ITEM(3, 0x83), 1,
    ITEM(3, 0x68), 1,
    ITEM(3, 0x69), 0,
    CODES_END,
};
u16 actor11Conditions[] = {
    ITEM(3, 0x9C), 1,
    ITEM(3, 0x90), 1,
    ITEM(3, 0x77), 1,
    ITEM(3, 0x83), 1,
    ITEM(3, 0x69), 1,
    CODES_END,
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x15, 4, 461, 190, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xC1, 5, 304, 176, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xC1, 5, 304, 176, 1 };
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
    { 1, 0, 0x40, 2, 0x34, 2, 0, 5, 6, 0, 124, 112, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 5, 6, 0, 187, 83, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 3, 6, 0, 470, 34, 0, 0 },
    { 1, 0, 0x56, 2, 1, 0, 0, 0, 0, 0, 335, 60, 0, 0 },
    { 1, 1, 0x80, 6, 0x35, 0, 0, 0, 0, 0, 518, 136, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 284, 28, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 286, 62, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 313, 43, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 315, 77, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 342, 57, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 345, 92, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 289, 26, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 291, 60, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 317, 40, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 319, 74, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 346, 54, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 348, 89, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 312, 144, 195, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2D0, 0x318, 0xAC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1236, script1236, EVENT_TEXT(5), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
