/*
 * WSTAG495: South Cape, South Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x3B7
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x3C7
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0014 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10300, 0x1EC00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x36;
    FIELDSTG_state.music = MUSIC(0x36, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 59, 2, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 59, 2, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 152, 2, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 152, 2, MUSIC(2, 0) };
BattleList area1Battles = {
    3,
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
AreaBattle area3Battle0 = { 214, 2, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 328, 2, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 59, 2, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 21, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16C, 0x130, 0xB0, 0x30, 0x150, 0x1FF },
    { 0x140, 0x100, 0x174, 0x130, 0xD0, 0x30, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x100, 0xB0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x158, 0xB0, 0x58, 0x140, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { PARTY_STAT(2), 0, FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor0Talk2Actions[] = { CARD_BATTLE(0x18, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xE), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor0Talk6Actions[] = { CARD_BATTLE(0x18, 1), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x18, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xE), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xE), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor1Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(0x18, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PARTY_STAT(2), 0, FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(4), 0, PARTY_STAT(2), 1, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x18, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xE), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor3Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xE), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xE), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor3Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(0x18, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 0, CODES_END };
u16 actor9Talk0Actions[] = { 0x9402, 1, CODES_END };
u16 actor9Talk1Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor9Talk1Actions[] = { 0x9406, 1, CODES_END };
u16 actor10Talk0Actions[] = { 0x940D, 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor11Talk2Actions[] = { CARD_BATTLE(0x18, 0), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 0,
    CODES_END,
};
u16 actor11Talk3Actions[] = { FLAG(0xE, 0xE), 1, CODES_END };
u16 actor11Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor11Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor11Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xE), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor11Talk6Actions[] = { CARD_BATTLE(0x18, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0xB2 },
    { actor0Talk1Conditions, NULL, 0xB6 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0xB7 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0xB8 },
    { actor0Talk4Conditions, NULL, 0xB9 },
    { actor0Talk5Conditions, NULL, 0xBA },
    { actor0Talk6Conditions, actor0Talk6Actions, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0xB3 },
    { actor1Talk1Conditions, NULL, 0xB6 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0xB7 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0xB8 },
    { actor1Talk4Conditions, NULL, 0xB9 },
    { actor1Talk5Conditions, NULL, 0xBA },
    { actor1Talk6Conditions, actor1Talk6Actions, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xB2 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0xB4 },
    { actor3Talk1Conditions, NULL, 0xB6 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0xB7 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0xB8 },
    { actor3Talk4Conditions, NULL, 0xB9 },
    { actor3Talk5Conditions, NULL, 0xBA },
    { actor3Talk6Conditions, actor3Talk6Actions, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0xB2 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0xBC },
    { actor4Talk2Conditions, actor4Talk2Actions, 0xBD },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x332 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x332 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x332 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x332 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x23 },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, actor10Talk0Actions, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0xB5 },
    { actor11Talk1Conditions, NULL, 0xB5 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0xB5 },
    { actor11Talk3Conditions, actor11Talk3Actions, 0xB5 },
    { actor11Talk4Conditions, NULL, 0xB5 },
    { actor11Talk5Conditions, NULL, 0xB5 },
    { actor11Talk6Conditions, actor11Talk6Actions, 0xB5 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(4), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actor10Conditions[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x35, 4, 480, 480, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x35, 4, 480, 480, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x35, 4, 480, 480, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 4, 480, 480, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 4, 480, 480, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x65, 5, 192, 496, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x65, 5, 192, 496, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x65, 5, 192, 496, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x65, 5, 192, 496, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x86, 6, 204, 412, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x86, 6, 204, 412, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 7, 480, 480, 7 };
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
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 427, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 182, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 267, 201, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 377, 286, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 400, 136, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 456, 92, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 211, 228, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 347, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 445, 194, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 48, 497, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 369, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 496, 331, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 667, 251, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 712, 378, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 142, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 356, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 541, 241, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 564, 420, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 640, 332, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 730, 597, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 839, 559, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 34, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 138, 506, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 19, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 214, 575, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 324, 588, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 445, 584, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 587, 528, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 739, 511, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 18, 509, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 182, 191, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 232, 601, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 262, 206, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 361, 453, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 417, 288, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 467, 189, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 479, 600, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 516, 331, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 558, 244, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 648, 406, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 697, 378, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 716, 634, 0, 0 },
    { 1, 0, 0x64, 4, 6, 0, 0, 0, 0, 0, 209, 351, 391, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23C, 0x88, 0x1BE, 7, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 9, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x50, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
