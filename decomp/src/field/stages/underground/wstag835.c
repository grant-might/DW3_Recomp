/*
 * WSTAG835: Seabed, Underground: below Divermon's Lake and South and West
 * Sector. A tunnel under Divermon's Lake, Ether Jungle, Suzaku City, South
 * Badland, Pelche Oasis and North Badland E; it joins WSTAG825, 850 and 855.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places_last.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x6D6
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x6E5
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xC700, 0x16D00};
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

StagePoint placePoints3_1Point1 = { 0x2E6, 3, 1, 160, 0x150, 5, NULL };
StagePoint placePoints3_1Point0 = { 0x24B, 0, 0, 0x2D0, 0x320, 0, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints4_1Point1 = { 0x2E5, 4, 1, 224, 0x120, 5, NULL };
StagePoint placePoints4_1Point0 = { 0x23C, 0, 0, 160, 0x3E8, 0, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point1 = { 0x2E6, 5, 1, 160, 0x150, 5, NULL };
StagePoint placePoints5_1Point0 = { 0x249, 0, 0, 208, 0x200, 0, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints7_1Point1 = { 0x2E0, 7, 1, 176, 0x178, 5, NULL };
StagePoint placePoints7_1Point0 = { 0x23E, 0, 0, 0x420, 0x1E8, 0, &placePoints7_1Point1 };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints8_1Point1 = { 0x2E0, 8, 1, 176, 0x178, 5, NULL };
StagePoint placePoints8_1Point0 = { 0x227, 0, 0, 0x240, 0x200, 0, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints9_1Point1 = { 0x2E6, 9, 1, 160, 0x150, 5, NULL };
StagePoint placePoints9_1Point0 = { 0x247, 0, 0, 0x350, 0x3F0, 0, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints13_1Point1 = { 0x2E6, 13, 1, 160, 0x150, 5, NULL };
StagePoint placePoints13_1Point0 = { 0x2B5, 0, 0, 0x2D0, 0x320, 0, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints14_1Point1 = { 0x2E5, 14, 1, 224, 0x120, 5, NULL };
StagePoint placePoints14_1Point0 = { 0x2A9, 0, 0, 160, 0x3E8, 0, &placePoints14_1Point1 };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints15_1Point1 = { 0x2E6, 15, 1, 160, 0x150, 5, NULL };
StagePoint placePoints15_1Point0 = { 0x2B3, 0, 0, 208, 0x200, 0, &placePoints15_1Point1 };
StagePoints placePoints15_1 = { 15, 1, &placePoints15_1Point0 };
StagePoint placePoints17_1Point1 = { 0x2E0, 17, 1, 176, 0x178, 5, NULL };
StagePoint placePoints17_1Point0 = { 0x2AB, 0, 0, 0x420, 0x1E8, 0, &placePoints17_1Point1 };
StagePoints placePoints17_1 = { 17, 1, &placePoints17_1Point0 };
StagePoint placePoints18_1Point1 = { 0x2E0, 18, 1, 176, 0x178, 5, NULL };
StagePoint placePoints18_1Point0 = { 0x296, 0, 0, 0x240, 0x200, 0, &placePoints18_1Point1 };
StagePoints placePoints18_1 = { 18, 1, &placePoints18_1Point0 };
StagePoint placePoints19_1Point1 = { 0x2E6, 19, 1, 160, 0x150, 5, NULL };
StagePoint placePoints19_1Point0 = { 0x2B1, 0, 0, 0x350, 0x3F0, 0, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints3_1Point0 };
StagePoints *placePoints[] = {
    &placePoints3_1, &placePoints4_1, &placePoints5_1, &placePoints7_1,
    &placePoints8_1, &placePoints9_1, &placePoints13_1, &placePoints14_1,
    &placePoints15_1, &placePoints17_1, &placePoints18_1, &placePoints19_1,
    &placePoints0_0, NULL,
};
AreaBattle place3Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 61, 11, MUSIC(2, 0) };
BattleList place3Area0Battles = {
    3,
    { &place3Area0Battle0, &place3Area0Battle1, &place3Area0Battle2, &place3Area0Battle3,
      &place3Area0Battle4, &place3Area0Battle5, &place3Area0Battle6, &place3Area0Battle7 },
};
AreaBattle place3Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area1Battles = {
    0,
    { &place3Area1Battle0, &place3Area1Battle1, &place3Area1Battle2, &place3Area1Battle3,
      &place3Area1Battle4, &place3Area1Battle5, &place3Area1Battle6, &place3Area1Battle7 },
};
AreaBattle place3Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area2Battles = {
    0,
    { &place3Area2Battle0, &place3Area2Battle1, &place3Area2Battle2, &place3Area2Battle3,
      &place3Area2Battle4, &place3Area2Battle5, &place3Area2Battle6, &place3Area2Battle7 },
};
AreaBattle place3Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area3Battles = {
    0,
    { &place3Area3Battle0, &place3Area3Battle1, &place3Area3Battle2, &place3Area3Battle3,
      &place3Area3Battle4, &place3Area3Battle5, &place3Area3Battle6, &place3Area3Battle7 },
};
AreaBattle place4Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 62, 11, MUSIC(2, 0) };
BattleList place4Area0Battles = {
    2,
    { &place4Area0Battle0, &place4Area0Battle1, &place4Area0Battle2, &place4Area0Battle3,
      &place4Area0Battle4, &place4Area0Battle5, &place4Area0Battle6, &place4Area0Battle7 },
};
AreaBattle place4Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area1Battles = {
    0,
    { &place4Area1Battle0, &place4Area1Battle1, &place4Area1Battle2, &place4Area1Battle3,
      &place4Area1Battle4, &place4Area1Battle5, &place4Area1Battle6, &place4Area1Battle7 },
};
AreaBattle place4Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area2Battles = {
    0,
    { &place4Area2Battle0, &place4Area2Battle1, &place4Area2Battle2, &place4Area2Battle3,
      &place4Area2Battle4, &place4Area2Battle5, &place4Area2Battle6, &place4Area2Battle7 },
};
AreaBattle place4Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area3Battles = {
    0,
    { &place4Area3Battle0, &place4Area3Battle1, &place4Area3Battle2, &place4Area3Battle3,
      &place4Area3Battle4, &place4Area3Battle5, &place4Area3Battle6, &place4Area3Battle7 },
};
AreaBattle place5Area0Battle0 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 63, 11, MUSIC(2, 0) };
BattleList place5Area0Battles = {
    3,
    { &place5Area0Battle0, &place5Area0Battle1, &place5Area0Battle2, &place5Area0Battle3,
      &place5Area0Battle4, &place5Area0Battle5, &place5Area0Battle6, &place5Area0Battle7 },
};
AreaBattle place5Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area1Battles = {
    0,
    { &place5Area1Battle0, &place5Area1Battle1, &place5Area1Battle2, &place5Area1Battle3,
      &place5Area1Battle4, &place5Area1Battle5, &place5Area1Battle6, &place5Area1Battle7 },
};
AreaBattle place5Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area2Battles = {
    0,
    { &place5Area2Battle0, &place5Area2Battle1, &place5Area2Battle2, &place5Area2Battle3,
      &place5Area2Battle4, &place5Area2Battle5, &place5Area2Battle6, &place5Area2Battle7 },
};
AreaBattle place5Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area3Battles = {
    0,
    { &place5Area3Battle0, &place5Area3Battle1, &place5Area3Battle2, &place5Area3Battle3,
      &place5Area3Battle4, &place5Area3Battle5, &place5Area3Battle6, &place5Area3Battle7 },
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
AreaBattle place9Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle4 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle5 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle6 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle7 = { 61, 11, MUSIC(2, 0) };
BattleList place9Area0Battles = {
    3,
    { &place9Area0Battle0, &place9Area0Battle1, &place9Area0Battle2, &place9Area0Battle3,
      &place9Area0Battle4, &place9Area0Battle5, &place9Area0Battle6, &place9Area0Battle7 },
};
AreaBattle place9Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area1Battles = {
    0,
    { &place9Area1Battle0, &place9Area1Battle1, &place9Area1Battle2, &place9Area1Battle3,
      &place9Area1Battle4, &place9Area1Battle5, &place9Area1Battle6, &place9Area1Battle7 },
};
AreaBattle place9Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area2Battles = {
    0,
    { &place9Area2Battle0, &place9Area2Battle1, &place9Area2Battle2, &place9Area2Battle3,
      &place9Area2Battle4, &place9Area2Battle5, &place9Area2Battle6, &place9Area2Battle7 },
};
AreaBattle place9Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area3Battles = {
    0,
    { &place9Area3Battle0, &place9Area3Battle1, &place9Area3Battle2, &place9Area3Battle3,
      &place9Area3Battle4, &place9Area3Battle5, &place9Area3Battle6, &place9Area3Battle7 },
};
AreaBattle place13Area0Battle0 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle1 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle2 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle3 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle4 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle5 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle6 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle7 = { 104, 11, MUSIC(2, 0) };
BattleList place13Area0Battles = {
    3,
    { &place13Area0Battle0, &place13Area0Battle1, &place13Area0Battle2, &place13Area0Battle3,
      &place13Area0Battle4, &place13Area0Battle5, &place13Area0Battle6, &place13Area0Battle7 },
};
AreaBattle place13Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area1Battles = {
    0,
    { &place13Area1Battle0, &place13Area1Battle1, &place13Area1Battle2, &place13Area1Battle3,
      &place13Area1Battle4, &place13Area1Battle5, &place13Area1Battle6, &place13Area1Battle7 },
};
AreaBattle place13Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area2Battles = {
    0,
    { &place13Area2Battle0, &place13Area2Battle1, &place13Area2Battle2, &place13Area2Battle3,
      &place13Area2Battle4, &place13Area2Battle5, &place13Area2Battle6, &place13Area2Battle7 },
};
AreaBattle place13Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area3Battles = {
    0,
    { &place13Area3Battle0, &place13Area3Battle1, &place13Area3Battle2, &place13Area3Battle3,
      &place13Area3Battle4, &place13Area3Battle5, &place13Area3Battle6, &place13Area3Battle7 },
};
AreaBattle place14Area0Battle0 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle1 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle2 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle3 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle4 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle5 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle6 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle7 = { 103, 11, MUSIC(2, 0) };
BattleList place14Area0Battles = {
    2,
    { &place14Area0Battle0, &place14Area0Battle1, &place14Area0Battle2, &place14Area0Battle3,
      &place14Area0Battle4, &place14Area0Battle5, &place14Area0Battle6, &place14Area0Battle7 },
};
AreaBattle place14Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area1Battles = {
    0,
    { &place14Area1Battle0, &place14Area1Battle1, &place14Area1Battle2, &place14Area1Battle3,
      &place14Area1Battle4, &place14Area1Battle5, &place14Area1Battle6, &place14Area1Battle7 },
};
AreaBattle place14Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area2Battles = {
    0,
    { &place14Area2Battle0, &place14Area2Battle1, &place14Area2Battle2, &place14Area2Battle3,
      &place14Area2Battle4, &place14Area2Battle5, &place14Area2Battle6, &place14Area2Battle7 },
};
AreaBattle place14Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area3Battles = {
    0,
    { &place14Area3Battle0, &place14Area3Battle1, &place14Area3Battle2, &place14Area3Battle3,
      &place14Area3Battle4, &place14Area3Battle5, &place14Area3Battle6, &place14Area3Battle7 },
};
AreaBattle place15Area0Battle0 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle1 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle2 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle3 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle4 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle5 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle6 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle7 = { 178, 11, MUSIC(2, 0) };
BattleList place15Area0Battles = {
    3,
    { &place15Area0Battle0, &place15Area0Battle1, &place15Area0Battle2, &place15Area0Battle3,
      &place15Area0Battle4, &place15Area0Battle5, &place15Area0Battle6, &place15Area0Battle7 },
};
AreaBattle place15Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area1Battles = {
    0,
    { &place15Area1Battle0, &place15Area1Battle1, &place15Area1Battle2, &place15Area1Battle3,
      &place15Area1Battle4, &place15Area1Battle5, &place15Area1Battle6, &place15Area1Battle7 },
};
AreaBattle place15Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area2Battles = {
    0,
    { &place15Area2Battle0, &place15Area2Battle1, &place15Area2Battle2, &place15Area2Battle3,
      &place15Area2Battle4, &place15Area2Battle5, &place15Area2Battle6, &place15Area2Battle7 },
};
AreaBattle place15Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area3Battles = {
    0,
    { &place15Area3Battle0, &place15Area3Battle1, &place15Area3Battle2, &place15Area3Battle3,
      &place15Area3Battle4, &place15Area3Battle5, &place15Area3Battle6, &place15Area3Battle7 },
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
AreaBattle place19Area0Battle0 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle1 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle2 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle3 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle4 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle5 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle6 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle7 = { 104, 11, MUSIC(2, 0) };
BattleList place19Area0Battles = {
    3,
    { &place19Area0Battle0, &place19Area0Battle1, &place19Area0Battle2, &place19Area0Battle3,
      &place19Area0Battle4, &place19Area0Battle5, &place19Area0Battle6, &place19Area0Battle7 },
};
AreaBattle place19Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area1Battles = {
    0,
    { &place19Area1Battle0, &place19Area1Battle1, &place19Area1Battle2, &place19Area1Battle3,
      &place19Area1Battle4, &place19Area1Battle5, &place19Area1Battle6, &place19Area1Battle7 },
};
AreaBattle place19Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area2Battles = {
    0,
    { &place19Area2Battle0, &place19Area2Battle1, &place19Area2Battle2, &place19Area2Battle3,
      &place19Area2Battle4, &place19Area2Battle5, &place19Area2Battle6, &place19Area2Battle7 },
};
AreaBattle place19Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area3Battles = {
    0,
    { &place19Area3Battle0, &place19Area3Battle1, &place19Area3Battle2, &place19Area3Battle3,
      &place19Area3Battle4, &place19Area3Battle5, &place19Area3Battle6, &place19Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 182, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 186, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 190, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 195, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 197, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 200, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 210, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 214, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
    { 218, 15, 0, { &place15Area0Battles, &place15Area1Battles, &place15Area2Battles, &place15Area3Battles } },
    { 223, 17, 0, { &place17Area0Battles, &place17Area1Battles, &place17Area2Battles, &place17Area3Battles } },
    { 225, 18, 0, { &place18Area0Battles, &place18Area1Battles, &place18Area2Battles, &place18Area3Battles } },
    { 228, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x152, 0x198, 0x48, 0x98, 0x160, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 0x147, 4, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 254, 275, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 621, 244, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 852, 128, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 384, 171, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 689, 166, 0, 0 },
    { 1, 0, 0xFF, 2, 0x33, 2, 0, 7, 0x18, 0, 140, -36, 0, 0 },
    { 1, 0, 0x98, 2, 5, 0, 0, 0, 0, 0, 432, 254, 0, 0 },
    { 1, 0, 0x8A, 2, 6, 0, 0, 0, 0, 0, 408, 268, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 475, 113, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 169, 206, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 750, 142, 0, 0 },
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x18, 0, 140, 92, 336, 0 },
    { 1, 0, 0x92, 4, 1, 0, 0, 0, 0, 0, 560, 179, 319, 0 },
    { 1, 0, 0x79, 4, 2, 0, 0, 0, 0, 0, 528, 198, 335, 0 },
    { 1, 0, 0x88, 4, 3, 0, 0, 0, 0, 0, 496, 216, 351, 0 },
    { 1, 0, 0x86, 4, 4, 0, 0, 0, 0, 0, 464, 232, 368, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E2, 0xB0, 0x148, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E2, 0x330, 0xF8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
