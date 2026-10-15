/*
 * WSTAG825: Seabed, Underground: below the Sewers, Duel Island, Kicking Forest
 * and the UG Lake. A tunnel under the Asuka and Amaterasu Sewers, Duel Island,
 * Kicking Forest and the UG Lake; it joins WSTAG830, 835 and 855.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places_last.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x690
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x6A0
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x9D00, 0x17F00};
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

StagePoint placePoints1_1Point1 = { 0x2E6, 1, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints1_1Point0 = { 0x22A, 0, 0, 0x32C, 0x232, 0, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints2_1Point1 = { 0x2E1, 2, 1, 0x390, 0x108, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x21B, 0, 0, 128, 0x1E0, 0, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints7_1Point1 = { 0x2E2, 7, 1, 0x330, 248, 1, NULL };
StagePoint placePoints7_1Point0 = { 0x241, 0, 0, 208, 0x400, 0, &placePoints7_1Point1 };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints8_1Point1 = { 0x2E2, 8, 1, 0x330, 248, 1, NULL };
StagePoint placePoints8_1Point0 = { 0x228, 0, 0, 0x210, 0x370, 0, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints11_1Point1 = { 0x2E6, 11, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints11_1Point0 = { 0x299, 0, 0, 0x32C, 0x232, 0, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints12_1Point1 = { 0x2E1, 12, 1, 0x390, 0x108, 1, NULL };
StagePoint placePoints12_1Point0 = { 0x28A, 0, 0, 128, 0x1E0, 0, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints17_1Point1 = { 0x2E2, 17, 1, 0x330, 248, 1, NULL };
StagePoint placePoints17_1Point0 = { 0x2AE, 0, 0, 208, 0x400, 0, &placePoints17_1Point1 };
StagePoints placePoints17_1 = { 17, 1, &placePoints17_1Point0 };
StagePoint placePoints18_1Point1 = { 0x2E2, 18, 1, 0x330, 248, 1, NULL };
StagePoint placePoints18_1Point0 = { 0x297, 0, 0, 0x210, 0x370, 0, &placePoints18_1Point1 };
StagePoints placePoints18_1 = { 18, 1, &placePoints18_1Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints1_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints2_1, &placePoints7_1, &placePoints8_1,
    &placePoints11_1, &placePoints12_1, &placePoints17_1, &placePoints18_1,
    &placePoints0_0, NULL,
};
AreaBattle place1Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 62, 11, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    2,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area1Battles = {
    0,
    { &place1Area1Battle0, &place1Area1Battle1, &place1Area1Battle2, &place1Area1Battle3,
      &place1Area1Battle4, &place1Area1Battle5, &place1Area1Battle6, &place1Area1Battle7 },
};
AreaBattle place1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area2Battles = {
    0,
    { &place1Area2Battle0, &place1Area2Battle1, &place1Area2Battle2, &place1Area2Battle3,
      &place1Area2Battle4, &place1Area2Battle5, &place1Area2Battle6, &place1Area2Battle7 },
};
AreaBattle place1Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
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
AreaBattle place7Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle4 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle5 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle6 = { 61, 11, MUSIC(2, 0) };
AreaBattle place7Area0Battle7 = { 61, 11, MUSIC(2, 0) };
BattleList place7Area0Battles = {
    4,
    { &place7Area0Battle0, &place7Area0Battle1, &place7Area0Battle2, &place7Area0Battle3,
      &place7Area0Battle4, &place7Area0Battle5, &place7Area0Battle6, &place7Area0Battle7 },
};
AreaBattle place7Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place7Area1Battles = {
    0,
    { &place7Area1Battle0, &place7Area1Battle1, &place7Area1Battle2, &place7Area1Battle3,
      &place7Area1Battle4, &place7Area1Battle5, &place7Area1Battle6, &place7Area1Battle7 },
};
AreaBattle place7Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place7Area2Battles = {
    0,
    { &place7Area2Battle0, &place7Area2Battle1, &place7Area2Battle2, &place7Area2Battle3,
      &place7Area2Battle4, &place7Area2Battle5, &place7Area2Battle6, &place7Area2Battle7 },
};
AreaBattle place7Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place7Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place7Area3Battles = {
    0,
    { &place7Area3Battle0, &place7Area3Battle1, &place7Area3Battle2, &place7Area3Battle3,
      &place7Area3Battle4, &place7Area3Battle5, &place7Area3Battle6, &place7Area3Battle7 },
};
AreaBattle place8Area0Battle0 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle1 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle2 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle3 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle4 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle5 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle6 = { 64, 11, MUSIC(2, 0) };
AreaBattle place8Area0Battle7 = { 64, 11, MUSIC(2, 0) };
BattleList place8Area0Battles = {
    4,
    { &place8Area0Battle0, &place8Area0Battle1, &place8Area0Battle2, &place8Area0Battle3,
      &place8Area0Battle4, &place8Area0Battle5, &place8Area0Battle6, &place8Area0Battle7 },
};
AreaBattle place8Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place8Area1Battles = {
    0,
    { &place8Area1Battle0, &place8Area1Battle1, &place8Area1Battle2, &place8Area1Battle3,
      &place8Area1Battle4, &place8Area1Battle5, &place8Area1Battle6, &place8Area1Battle7 },
};
AreaBattle place8Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place8Area2Battles = {
    0,
    { &place8Area2Battle0, &place8Area2Battle1, &place8Area2Battle2, &place8Area2Battle3,
      &place8Area2Battle4, &place8Area2Battle5, &place8Area2Battle6, &place8Area2Battle7 },
};
AreaBattle place8Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place8Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place8Area3Battles = {
    0,
    { &place8Area3Battle0, &place8Area3Battle1, &place8Area3Battle2, &place8Area3Battle3,
      &place8Area3Battle4, &place8Area3Battle5, &place8Area3Battle6, &place8Area3Battle7 },
};
AreaBattle place11Area0Battle0 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle1 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle2 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle3 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle4 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle5 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle6 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle7 = { 103, 11, MUSIC(2, 0) };
BattleList place11Area0Battles = {
    2,
    { &place11Area0Battle0, &place11Area0Battle1, &place11Area0Battle2, &place11Area0Battle3,
      &place11Area0Battle4, &place11Area0Battle5, &place11Area0Battle6, &place11Area0Battle7 },
};
AreaBattle place11Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area1Battles = {
    0,
    { &place11Area1Battle0, &place11Area1Battle1, &place11Area1Battle2, &place11Area1Battle3,
      &place11Area1Battle4, &place11Area1Battle5, &place11Area1Battle6, &place11Area1Battle7 },
};
AreaBattle place11Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area2Battles = {
    0,
    { &place11Area2Battle0, &place11Area2Battle1, &place11Area2Battle2, &place11Area2Battle3,
      &place11Area2Battle4, &place11Area2Battle5, &place11Area2Battle6, &place11Area2Battle7 },
};
AreaBattle place11Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area3Battles = {
    0,
    { &place11Area3Battle0, &place11Area3Battle1, &place11Area3Battle2, &place11Area3Battle3,
      &place11Area3Battle4, &place11Area3Battle5, &place11Area3Battle6, &place11Area3Battle7 },
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
AreaBattle place17Area0Battle0 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle1 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle2 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle3 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle4 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle5 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle6 = { 103, 11, MUSIC(2, 0) };
AreaBattle place17Area0Battle7 = { 103, 11, MUSIC(2, 0) };
BattleList place17Area0Battles = {
    2,
    { &place17Area0Battle0, &place17Area0Battle1, &place17Area0Battle2, &place17Area0Battle3,
      &place17Area0Battle4, &place17Area0Battle5, &place17Area0Battle6, &place17Area0Battle7 },
};
AreaBattle place17Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place17Area1Battles = {
    0,
    { &place17Area1Battle0, &place17Area1Battle1, &place17Area1Battle2, &place17Area1Battle3,
      &place17Area1Battle4, &place17Area1Battle5, &place17Area1Battle6, &place17Area1Battle7 },
};
AreaBattle place17Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place17Area2Battles = {
    0,
    { &place17Area2Battle0, &place17Area2Battle1, &place17Area2Battle2, &place17Area2Battle3,
      &place17Area2Battle4, &place17Area2Battle5, &place17Area2Battle6, &place17Area2Battle7 },
};
AreaBattle place17Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place17Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place17Area3Battles = {
    0,
    { &place17Area3Battle0, &place17Area3Battle1, &place17Area3Battle2, &place17Area3Battle3,
      &place17Area3Battle4, &place17Area3Battle5, &place17Area3Battle6, &place17Area3Battle7 },
};
AreaBattle place18Area0Battle0 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle1 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle2 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle3 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle4 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle5 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle6 = { 64, 11, MUSIC(2, 0) };
AreaBattle place18Area0Battle7 = { 64, 11, MUSIC(2, 0) };
BattleList place18Area0Battles = {
    4,
    { &place18Area0Battle0, &place18Area0Battle1, &place18Area0Battle2, &place18Area0Battle3,
      &place18Area0Battle4, &place18Area0Battle5, &place18Area0Battle6, &place18Area0Battle7 },
};
AreaBattle place18Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place18Area1Battles = {
    0,
    { &place18Area1Battle0, &place18Area1Battle1, &place18Area1Battle2, &place18Area1Battle3,
      &place18Area1Battle4, &place18Area1Battle5, &place18Area1Battle6, &place18Area1Battle7 },
};
AreaBattle place18Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place18Area2Battles = {
    0,
    { &place18Area2Battle0, &place18Area2Battle1, &place18Area2Battle2, &place18Area2Battle3,
      &place18Area2Battle4, &place18Area2Battle5, &place18Area2Battle6, &place18Area2Battle7 },
};
AreaBattle place18Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place18Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place18Area3Battles = {
    0,
    { &place18Area3Battle0, &place18Area3Battle1, &place18Area3Battle2, &place18Area3Battle3,
      &place18Area3Battle4, &place18Area3Battle5, &place18Area3Battle6, &place18Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 173, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 178, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 194, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 196, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 201, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 206, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 222, 17, 0, { &place17Area0Battles, &place17Area1Battles, &place17Area2Battles, &place17Area3Battles } },
    { 224, 18, 0, { &place18Area0Battles, &place18Area1Battles, &place18Area2Battles, &place18Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x15D, 0x1C0, 0x5D, 0x160, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 0x147, 4, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 346, 191, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 653, 112, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 179, 218, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 563, 120, 0, 0 },
    { 1, 0, 0xA0, 2, 0xB, 0, 0, 0, 0, 0, 448, 198, 0, 0 },
    { 1, 0, 0x95, 2, 0xC, 0, 0, 0, 0, 0, 480, 210, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 266, 189, 0, 0 },
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x18, 0, 540, -12, 244, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 240, 273, 351, 0 },
    { 1, 0, 0x68, 4, 1, 0, 0, 0, 0, 0, 208, 259, 345, 0 },
    { 1, 0, 0x58, 4, 2, 0, 0, 0, 0, 0, 196, 254, 332, 0 },
    { 1, 0, 0x58, 4, 3, 0, 0, 0, 0, 0, 416, 180, 305, 0 },
    { 1, 0, 0x93, 4, 4, 0, 0, 0, 0, 0, 384, 157, 299, 0 },
    { 1, 0, 0x79, 4, 5, 0, 0, 0, 0, 0, 352, 151, 270, 0 },
    { 1, 0, 0x94, 4, 6, 0, 0, 0, 0, 0, 320, 131, 277, 0 },
    { 1, 0, 0x5C, 4, 8, 0, 0, 0, 0, 0, 528, 180, 268, 0 },
    { 1, 0, 0x63, 4, 9, 0, 0, 0, 0, 0, 496, 164, 260, 0 },
    { 1, 0, 0x5D, 4, 0xA, 0, 0, 0, 0, 0, 485, 159, 250, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E0, 0x240, 0xD8, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E0, 0xB0, 0x178, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
