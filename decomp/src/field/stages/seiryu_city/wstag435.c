/*
 * WSTAG435: Gale Tower, Seiryu City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x1B3
#define STAGE_ARCHIVE 0x3D0
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x1C1
#define STAGE_ARCHIVE 0x3E0
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0008 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x15100, 0x18800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x30;
    FIELDSTG_state.music = MUSIC(0x30, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script950[] = {
    SCRIPT_WALK(2, 0x131, 0x1B0, 1),
    SCRIPT_PLACE(0x3E, 0x100, 0x1C7),
    SCRIPT_POSE(0x3E, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 3, 0x3E, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 5, 0x3E, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xCF, 0x181, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xC8, 0x184, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x98, 0x1BC, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x91, 0x1C0, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x51, 0x1A0, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_FOLLOW(1, 0x3E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x3E, 2, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x185, 0x100, 0x1C8),
    SCRIPT_POSE(0x185, 0x56, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(2, 0x100, 0x149, 5),
    SCRIPT_POSE(0x185, 0x56, 1),
    SCRIPT_WAIT_ANIM(0x185),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x22E, 0x350, 0x230, 7),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x100, 0x88, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x172, 0x100, 0xC8, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x17A, 0x100, 0xE8, 0, 0x170, 0x1FF },
};
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 1), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 1), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(0x37), 1, FLAG(0x1A, 0xA), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x6E },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x10F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x111 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x110 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x16F },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x16F },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x16F },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x3C), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x3E, 4, 255, 456, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x3E, 4, 256, 455, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x67, 5, 288, 408, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x67, 5, 288, 408, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x67, 5, 288, 408, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x185, 6, 0, 0, 1 };
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
    { 1, 0, 0xD0, 2, 0x32, 0, 0, 0, 0, 0, 51, 95, 0, 0 },
    { 1, 0, 0xD0, 2, 0x32, 0, 0, 0, 0, 0, 300, 55, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22E, 0x350, 0x230, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 950, script950, EVENT_TEXT(0x2D), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
