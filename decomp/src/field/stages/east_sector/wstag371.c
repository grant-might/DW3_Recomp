/*
 * WSTAG371: Protocol Forest, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG370.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x6AF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x6BE
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV10_00 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10600, 0x2BC00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x32;
    FIELDSTG_state.music = MUSIC(0x32, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 96, 3, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 96, 3, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 99, 8, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 99, 8, MUSIC(2, 0) };
BattleList area1Battles = {
    1,
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
    { 67, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x160, 0x100, 0x80, 0, 0x140, 0x1FF },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x9A), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(3, 0x9A), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x96), 0, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 0), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x96), 1, CODES_END };
u16 actor0Talk3Actions[] = {
    ITEM(3, 0x9A), 1,
    ITEM(3, 0x99), 0,
    ITEM(2, 0x96), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x2F2 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x2F3 },
    { actor0Talk2Conditions, NULL, 0x2F4 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x2F5 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x46), 1,
    SPECIAL(0x4E), 1,
    ITEM(3, 0x99), 1,
    ITEM(3, 0x9A), 0,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0xA9, 4, 734, 417, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 1024, 448, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 1088, 448, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 896, 576, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 960, 576, 0, 0 },
    { 1, 0, 0x40, 2, 4, 0, 0, 0, 0, 0, 1024, 576, 0, 0 },
    { 1, 0, 0x40, 2, 5, 0, 0, 0, 0, 0, 1088, 576, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 1024, 640, 0, 0 },
    { 1, 0, 0x40, 2, 7, 0, 0, 0, 0, 0, 1088, 640, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x295, 0x5E, 0x40E, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x291, 0x374, 0x9B, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
