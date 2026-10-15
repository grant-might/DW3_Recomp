/*
 * WSTAG396: Wind Prairie, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG395.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x581
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x591
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0006 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xFE00, 0xA600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2E;
    FIELDSTG_state.music = MUSIC(0x2E, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 151, 1, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 151, 1, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 151, 1, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 151, 1, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 94, 1, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 94, 1, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 94, 1, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 94, 1, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 151, 1, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 151, 1, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 151, 1, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 151, 1, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 94, 1, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 94, 1, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 94, 1, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 94, 1, MUSIC(2, 0) };
BattleList area1Battles = {
    1,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle1 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle2 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle3 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle4 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle5 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle6 = { 100, 4, MUSIC(2, 0) };
AreaBattle area2Battle7 = { 100, 4, MUSIC(2, 0) };
BattleList area2Battles = {
    3,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 227, 1, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 275, 1, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 329, 1, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 99, 1, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 65, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x140, 0x1FF },
    { 0x180, 0x100, 0x1A6, 0x100, 0x198, 0, 0x160, 0x1FF },
};
u16 actor0Talk0Actions[] = { SPECIAL(0x8F), 1, FLAG(2, 0x2A), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, PARTY_STAT(8), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x2B, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1E), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x1E), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1E), 1,
    CODES_END,
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, FLAG(0xE, 0x3F), 0, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x3F), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x3F), 1,
    PARTY_STAT(0xB), 0,
    CODES_END,
};
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x3F), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x2B, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x180 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x244 },
    { actor1Talk1Conditions, NULL, 0x245 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x246 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x247 },
    { actor1Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x24C },
    { actor2Talk1Conditions, NULL, 0x24E },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x24D },
    { actor2Talk3Conditions, NULL, 0x248 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x39C },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x249 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x2A), 0, CODES_END };
u16 actor1Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x19), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor3Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x19), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 1566, 529, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 1408, 936, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 1408, 936, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 1408, 936, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 1408, 936, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x78, 2, 0x12, 0, 0, 0, 0, 0, 1216, 594, 0, 0 },
    { 1, 0, 0x78, 2, 0x14, 0, 0, 0, 0, 0, 1172, 388, 0, 0 },
    { 1, 0, 0x78, 2, 0x16, 0, 0, 0, 0, 0, 998, 303, 0, 0 },
    { 1, 0, 0x78, 2, 0x17, 0, 0, 0, 0, 0, 953, 355, 0, 0 },
    { 1, 0, 0x40, 2, 8, 0, 0, 0, 0, 0, 84, 106, 0, 0 },
    { 1, 0, 0x40, 2, 8, 0, 0, 0, 0, 0, 619, 641, 0, 0 },
    { 1, 0, 0x40, 2, 8, 0, 0, 0, 0, 0, 1198, 301, 0, 0 },
    { 1, 0, 0x40, 2, 8, 0, 0, 0, 0, 0, 1556, 782, 0, 0 },
    { 1, 0, 0x78, 4, 0x13, 0, 0, 0, 0, 0, 1264, 489, 562, 0 },
    { 1, 0, 0x78, 4, 0x15, 0, 0, 0, 0, 0, 905, 338, 355, 0 },
    { 1, 0, 0x78, 4, 0x18, 0, 0, 0, 0, 0, 914, 450, 464, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 1115, 218, 262, 0 },
    { 1, 0, 0x44, 4, 1, 0, 0, 0, 0, 0, 828, 359, 400, 0 },
    { 1, 0, 0x60, 4, 2, 0, 0, 0, 0, 0, 476, 268, 321, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 1216, 649, 682, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1169, 434, 465, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 545, 557, 583, 0 },
    { 1, 0, 0x44, 4, 6, 0, 0, 0, 0, 0, 925, 722, 766, 0 },
    { 1, 0, 0x73, 4, 7, 0, 0, 0, 0, 0, 634, 658, 767, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 288, 133, 133, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 384, 135, 135, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 425, 155, 155, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 432, 311, 311, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 472, 179, 179, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 521, 203, 203, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 263, 263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 391, 391, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 574, 368, 368, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 607, 214, 214, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 617, 347, 347, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 648, 235, 235, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 695, 259, 259, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 744, 283, 283, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 792, 307, 307, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 840, 331, 331, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 799, 799, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1008, 791, 791, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1376, 751, 751, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1424, 775, 775, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1472, 799, 799, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1520, 967, 967, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x296, 0x4A2, 0x36A, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x299, 0xAA, 0xC3, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29C, 0x150, 0x268, 5, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 8, 3 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 7, 3 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
