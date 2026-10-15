/*
 * WSTAG586: Duct Room 01, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG585.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5DB
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5EB
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0017 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12C00, 0x15400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x39;
    FIELDSTG_state.music = MUSIC(0x39, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 116, 12, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
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
    { 107, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x158, 0x100, 0x60, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x160, 0x100, 0x80, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x120, 0, 0x20, 0x170, 0x1FE },
    { 0x140, 0x100, 0x148, 0x120, 0x20, 0x20, 0x150, 0x1FD },
    { 0x140, 0x100, 0x150, 0x120, 0x40, 0x20, 0x160, 0x1FD },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x4C), 1, ITEM(1, 0x32), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Actions[] = {
    ITEM(1, 0x59), 1,
    ITEM(1, 0x5A), 1,
    ITEM(2, 0xA7), 1,
    ITEM(0, 7), 1,
    CODES_END,
};
u16 actor2Talk0Actions[] = {
    ITEM(3, 0x74), 1,
    ITEM(3, 0x80), 1,
    ITEM(3, 0x8D), 1,
    ITEM(3, 0x66), 1,
    ITEM(3, 0x99), 1,
    CODES_END,
};
u16 actor3Talk0Actions[] = {
    ITEM(2, 0x63), 1,
    ITEM(2, 0x96), 1,
    ITEM(2, 0x7D), 1,
    ITEM(2, 0x8A), 1,
    ITEM(2, 0x71), 1,
    CODES_END,
};
u16 actor4Talk0Actions[] = {
    ITEM(2, 0x97), 1,
    ITEM(2, 0x7E), 1,
    ITEM(2, 0x8B), 1,
    ITEM(2, 0x64), 1,
    ITEM(2, 0x72), 1,
    CODES_END,
};
u16 actor5Talk0Actions[] = {
    ITEM(2, 0x98), 1,
    ITEM(2, 0x7F), 1,
    ITEM(2, 0x8C), 1,
    ITEM(2, 0x65), 1,
    ITEM(2, 0x73), 1,
    CODES_END,
};
u16 actor7Talk0Actions[] = { SPECIAL(0x92), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x184 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x39F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x3A0 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x3A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x3A2 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x3A3 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x3A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x39E },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x4C), 0, PROGRESS(0), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 379, 318, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 209, 329, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x46, 6, 240, 313, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x47, 7, 272, 297, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x48, 8, 354, 401, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x49, 9, 385, 385, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x4A, 0xA, 415, 369, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x98, 0xB, 379, 317, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 6, 0, 277, 252, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 6, 0, 405, 317, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B7, 0x358, 0x37C, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
