/*
 * WSTAG830: Seabed, Underground: below the bridges. A tunnel under the Asuka
 * and Amaterasu Bridges (WSTAG205, 206); it joins WSTAG825.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x6F1
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x701
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xDB00, 0x12300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1D;
    FIELDSTG_state.music = MUSIC(0x1D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints2_1Point2 = { 0x2E0, 2, 1, 176, 0x178, 5, NULL };
StagePoint placePoints2_1Point1 = { 0x202, 0, 0, 0x420, 0x1F0, 0, &placePoints2_1Point2 };
StagePoint placePoints2_1Point0 = { 0x202, 0, 0, 0x150, 0x148, 0, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints12_1Point2 = { 0x2E0, 12, 1, 176, 0x178, 5, NULL };
StagePoint placePoints12_1Point1 = { 0x272, 0, 0, 0x420, 0x1F0, 0, &placePoints12_1Point2 };
StagePoint placePoints12_1Point0 = { 0x272, 0, 0, 0x150, 0x148, 0, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints2_1Point0 };
StagePoints *placePoints[] = {
    &placePoints2_1, &placePoints12_1, &placePoints0_0, NULL,
};
AreaBattle place2Area0Battle0 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 62, 11, MUSIC(2, 0) };
BattleList place2Area0Battles = {
    4,
    { &place2Area0Battle0, &place2Area0Battle1, &place2Area0Battle2, &place2Area0Battle3,
      &place2Area0Battle4, &place2Area0Battle5, &place2Area0Battle6, &place2Area0Battle7 },
};
AreaBattle place2Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place2Area1Battles = {
    0,
    { &place2Area1Battle0, &place2Area1Battle1, &place2Area1Battle2, &place2Area1Battle3,
      &place2Area1Battle4, &place2Area1Battle5, &place2Area1Battle6, &place2Area1Battle7 },
};
AreaBattle place2Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place2Area2Battles = {
    0,
    { &place2Area2Battle0, &place2Area2Battle1, &place2Area2Battle2, &place2Area2Battle3,
      &place2Area2Battle4, &place2Area2Battle5, &place2Area2Battle6, &place2Area2Battle7 },
};
AreaBattle place2Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place2Area3Battles = {
    0,
    { &place2Area3Battle0, &place2Area3Battle1, &place2Area3Battle2, &place2Area3Battle3,
      &place2Area3Battle4, &place2Area3Battle5, &place2Area3Battle6, &place2Area3Battle7 },
};
AreaBattle place12Area0Battle0 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle1 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle2 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle3 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle4 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle5 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle6 = { 178, 11, MUSIC(2, 0) };
AreaBattle place12Area0Battle7 = { 178, 11, MUSIC(2, 0) };
BattleList place12Area0Battles = {
    4,
    { &place12Area0Battle0, &place12Area0Battle1, &place12Area0Battle2, &place12Area0Battle3,
      &place12Area0Battle4, &place12Area0Battle5, &place12Area0Battle6, &place12Area0Battle7 },
};
AreaBattle place12Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place12Area1Battles = {
    0,
    { &place12Area1Battle0, &place12Area1Battle1, &place12Area1Battle2, &place12Area1Battle3,
      &place12Area1Battle4, &place12Area1Battle5, &place12Area1Battle6, &place12Area1Battle7 },
};
AreaBattle place12Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place12Area2Battles = {
    0,
    { &place12Area2Battle0, &place12Area2Battle1, &place12Area2Battle2, &place12Area2Battle3,
      &place12Area2Battle4, &place12Area2Battle5, &place12Area2Battle6, &place12Area2Battle7 },
};
AreaBattle place12Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place12Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place12Area3Battles = {
    0,
    { &place12Area3Battle0, &place12Area3Battle1, &place12Area3Battle2, &place12Area3Battle3,
      &place12Area3Battle4, &place12Area3Battle5, &place12Area3Battle6, &place12Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 179, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 207, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x160, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 0x147, 4, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 550, 238, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 239, 158, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 661, 204, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 896, 119, 0, 0 },
    { 1, 0, 0xFF, 2, 0x33, 2, 0, 7, 0x18, 0, 188, -92, 0, 0 },
    { 1, 0, 0xFF, 2, 0x33, 2, 0, 7, 0x18, 0, 428, -20, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 128, 276, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 192, 297, 0, 0 },
    { 1, 0, 0x52, 2, 2, 0, 0, 0, 0, 0, 256, 292, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 320, 320, 0, 0 },
    { 1, 0, 0x48, 2, 4, 0, 0, 0, 0, 0, 768, 312, 0, 0 },
    { 1, 0, 0x54, 2, 5, 0, 0, 0, 0, 0, 832, 270, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 512, 335, 0, 0 },
    { 1, 0, 0x44, 2, 7, 0, 0, 0, 0, 0, 576, 316, 0, 0 },
    { 1, 0, 0x40, 2, 8, 0, 0, 0, 0, 0, 640, 337, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 704, 338, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 460, 182, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 348, 194, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 761, 156, 0, 0 },
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x18, 0, 188, 36, 280, 0 },
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x18, 0, 428, 108, 352, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E1, 0xE0, 0x110, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E1, 0x1D0, 0x154, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E1, 0x390, 0x108, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
