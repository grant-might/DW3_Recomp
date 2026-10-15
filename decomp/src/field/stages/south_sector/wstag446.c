/*
 * WSTAG446: Bulk Swamp, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG445.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x596
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5A6
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0012 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10300, 0x13800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x34;
    FIELDSTG_state.music = MUSIC(0x34, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 105, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 105, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 229, 8, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 277, 8, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 331, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 177, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 70, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16E, 0x170, 0xB8, 0x70, 0x150, 0x1FF },
    { 0x140, 0x100, 0x166, 0x13E, 0x98, 0x3E, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x148, 0xB8, 0x48, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x148, 0xD8, 0x48, 0x140, 0x1FE },
    { 0x140, 0x100, 0x166, 0x166, 0x98, 0x66, 0x150, 0x1FE },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(5), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(5), 1, PARTY_STAT(8), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x2D, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x20), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x20), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x20), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, FLAG(0xE, 0x41), 0, CODES_END };
u16 actor3Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x41), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x41), 1,
    PARTY_STAT(0xB), 0,
    CODES_END,
};
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x41), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x2D, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0xCD },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xCA },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x244 },
    { actor2Talk1Conditions, NULL, 0x245 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x246 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x247 },
    { actor2Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x24C },
    { actor3Talk1Conditions, NULL, 0x24E },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24D },
    { actor3Talk3Conditions, NULL, 0x248 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x39C },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x249 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xCC },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xCE },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xC9 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xCB },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 611, 171, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x33, 5, 595, 475, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 6, 232, 316, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 6, 232, 316, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 6, 232, 316, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 6, 232, 316, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 7, 611, 171, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 7, 611, 171, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9E, 8, 595, 475, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9E, 8, 595, 475, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 89, 30, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 694, 46, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 185, 413, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 428, 276, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 405, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 706, 428, 0, 0 },
    { 1, 0x64, 0x40, 6, 1, 0, 0, 0, 0, 0, 490, 108, 0, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 464, 77, 161, 0 },
    { 1, 0, 0x80, 4, 0xD, 0, 0, 0, 0, 0, 358, 119, 155, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 223, 167, 167, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 248, 140, 140, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 295, 115, 115, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 151, 151, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 599, 123, 123, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 656, 313, 313, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 690, 375, 375, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A2, 0x668, 0xB4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A0, 0x190, 0x218, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
