/*
 * WSTAG928: Amaterasu Inn 1F, Amaterasu City, in the extra chapter. The
 * European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 633
 * starts it instead of WSTAG236.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1612, with Guardromon: applies 0x7C0C, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1612(void) {
    FLAGS_00.applyAction(0x7C0C, 1);
}

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8EF
#define STAGE_FILE_8 0x1A2
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8AD
#define STAGE_FILE_8 0x194
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xBC00, 0xB600};
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
extern s16 script1612[];

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14A, 0x18A, 0x28, 0x8A, 0x160, 0x1FF },
    { 0x140, 0x100, 0x152, 0x187, 0x48, 0x87, 0x170, 0x1FF },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x166, 0x18B, 0x98, 0x8B, 0x160, 0x1FE },
};
u16 actor0Talk0Actions[] = { 0x7A28, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x70), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x18, 5), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x18, 5), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0x18, 5), 0, FLAG(0, 0), 1, FLAG(0, 1), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0x18, 5), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0x18, 5), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 0,
    CODES_END,
};
u16 actor2Talk4Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor2Talk5Conditions[] = {
    FLAG(0x18, 5), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 4), 0,
    FLAG(0, 3), 1,
    CODES_END,
};
u16 actor2Talk5Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor2Talk6Conditions[] = {
    FLAG(0, 3), 1,
    FLAG(0x18, 5), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 4), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { SPECIAL(0x13), 1, FLAG(0x18, 5), 1, SPECIAL(0x92), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2B },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x31 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x2E },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x30 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x30 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x30 },
    { actor2Talk5Conditions, actor2Talk5Actions, 0x30 },
    { actor2Talk6Conditions, actor2Talk6Actions, 0x2F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x2D },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 188, 154, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 175, 192, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x3F, 6, 145, 289, 7 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x177, 7, 88, 332, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27B, 0x96, 0x114, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x2A0, 0x17A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27E, 0x15A, 0xBA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27A, 0x146, 0xBA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xC8, 0xEC, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xB8, 0x132, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1612, script1612, EVENT_TEXT(6), NULL, endEvent1612 },
    { -1, NULL, 0, NULL, NULL },
};
s16 script1612[] = {
    SCRIPT_WALK(2, 0xCF, 0xD0, 3),
    SCRIPT_PLACE(0x15, 0xAF, 0xC0),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x24),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC0A, 0, 0, 0),
    SCRIPT_END,
};
