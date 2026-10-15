/*
 * WSTAG646: Mirage Tower, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG645.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* After event 735, with Tamer Heinrich and Kenny: keeps it from playing again */
void endEvent735(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x49), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x61B
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x62B
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x2C900, 0x23B00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x38;
    FIELDSTG_state.music = MUSIC(0x38, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script735[] = {
    SCRIPT_WALK(2, 0x248, 0x11C, 5),
    SCRIPT_PLACE(0x2D, 0x270, 0x108),
    SCRIPT_POSE(0x2D, 1, 1),
    SCRIPT_PLACE(0x31, 0x288, 0x115),
    SCRIPT_POSE(0x31, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x258, 0x114, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_COMMAND(0x323, 0x325, 0x2D),
    SCRIPT_COMMAND(0x324, 0x325, 0x31),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x2D),
    SCRIPT_COMMAND(0x324, 0x326, 0x31),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x31, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x2D),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x2D, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x2D, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x2D, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x2D, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x2D, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x2D, 0x250, 0xF8, 3),
    SCRIPT_WAIT_WALK(0x2D),
    SCRIPT_PLACE(0x2D, 0, 0),
    SCRIPT_POSE(0x2D, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 72, 5, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area1Battles = {
    0,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area2Battles = {
    0,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 239, 5, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 287, 5, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 104, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x13D, 0, 0x3D, 0x150, 0x1FF },
    { 0x140, 0x100, 0x150, 0x15D, 0x40, 0x5D, 0x160, 0x1FF },
    { 0x140, 0x100, 0x148, 0x13D, 0x20, 0x3D, 0x170, 0x1FF },
    { 0x140, 0x100, 0x150, 0x13D, 0x40, 0x3D, 0x150, 0x1FE },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x8E), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 1), 0, ITEM(3, 0x8E), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 1), 1, ITEM(3, 0x8E), 0, ITEM(2, 0x8A), 0, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 1), 1, ITEM(3, 0x8E), 0, ITEM(2, 0x8A), 1, CODES_END };
u16 actor0Talk3Actions[] = {
    ITEM(3, 0x8E), 1,
    ITEM(3, 0x8D), 0,
    ITEM(2, 0x8A), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0xB), 1, START_EVENT(0x46), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x11), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk4Conditions[] = { FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor2Talk4Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk5Conditions[] = { FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk5Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor4Talk3Actions[] = { CARD_BATTLE(0x37, 0), 1, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 0,
    CODES_END,
};
u16 actor4Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2A), 0,
    CODES_END,
};
u16 actor4Talk5Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x2A), 1, CODES_END };
u16 actor4Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2A), 1,
    PARTY_STAT(0xB), 0,
    CODES_END,
};
u16 actor4Talk7Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2A), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor4Talk7Actions[] = { CARD_BATTLE(0x37, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x2FA },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x2FB },
    { actor0Talk2Conditions, NULL, 0x2FC },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x2FD },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1D9 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x1E8 },
    { actor2Talk1Conditions, NULL, 0x13 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x1E6 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x14 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x1E7 },
    { actor2Talk5Conditions, actor2Talk5Actions, 0x15 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x1E8 },
    { actor3Talk1Conditions, NULL, 0x13 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x1DE },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x13 },
    { actor4Talk2Conditions, NULL, 0x16 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x1E1 },
    { actor4Talk4Conditions, NULL, 0x1E2 },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x1E3 },
    { actor4Talk6Conditions, NULL, 0x1E4 },
    { actor4Talk7Conditions, actor4Talk7Actions, 0x1E5 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1DD },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x49), 1,
    SPECIAL(0x51), 1,
    ITEM(3, 0x8D), 1,
    ITEM(3, 0x8E), 0,
    CODES_END,
};
u16 actor1Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x40, 0x49), 0, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor3Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x1C, 4, 720, 393, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 5, 624, 264, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x31, 6, 648, 277, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x31, 6, 648, 277, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x31, 6, 648, 277, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9D, 7, 648, 277, 1 };
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
    { 1, 0, 0x40, 6, 4, 0, 0, 0, 0, 0, 562, 254, 0, 0 },
    { 1, 0, 0x40, 4, 0x53, 2, 0, 0xF, 8, 0, 604, 194, 256, 0 },
    { 1, 0, 0x40, 4, 0x54, 2, 0, 0xF, 8, 0, 604, 194, 252, 0 },
    { 1, 0, 0x40, 4, 0x55, 2, 0, 0xF, 8, 0, 604, 194, 248, 0 },
    { 1, 0, 0x40, 4, 0x56, 2, 0, 0xF, 8, 0, 604, 194, 244, 0 },
    { 1, 0, 0x40, 4, 0x57, 2, 0, 0xF, 8, 0, 604, 194, 240, 0 },
    { 1, 0, 0x40, 4, 0x58, 2, 0, 0xF, 8, 0, 604, 194, 236, 0 },
    { 1, 0, 0x40, 4, 0x59, 2, 0, 0xF, 8, 0, 604, 194, 232, 0 },
    { 1, 0, 0x40, 4, 0x5A, 2, 0, 0xF, 8, 0, 604, 194, 228, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 566, 208, 264, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C4, 0x27A, 0x256, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x110, 0x108, 7, 0, 1, 6 },
    { { { FLAG(0x40, 0x49), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2DF, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 735, script735, EVENT_TEXT(0x1D), NULL, endEvent735 },
    { -1, NULL, 0, NULL, NULL },
};
