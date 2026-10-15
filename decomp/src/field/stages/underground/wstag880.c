/*
 * WSTAG880: Circuit Board, Underground: reached only from the other tunnels. A
 * tunnel only WSTAG865 and 885 to 895 reach.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x680
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x690
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xEB00, 0x1F500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1E;
    FIELDSTG_state.music = MUSIC(0x1E, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints1_1Point0 = { 0x2ED, 1, 1, 224, 192, 5, NULL };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints4_1Point0 = { 0x2ED, 4, 2, 224, 192, 5, NULL };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints7_1Point0 = { 0x2EE, 7, 2, 224, 0x240, 5, NULL };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints12_1Point0 = { 0x2EC, 12, 3, 240, 0x1D8, 5, NULL };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints12_2Point0 = { 0x2EC, 12, 4, 240, 0x1D8, 5, NULL };
StagePoints placePoints12_2 = { 12, 2, &placePoints12_2Point0 };
StagePoint placePoints13_1Point0 = { 0x2EC, 13, 5, 240, 0x1D8, 5, NULL };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints13_2Point0 = { 0x2EC, 13, 6, 240, 0x1D8, 5, NULL };
StagePoints placePoints13_2 = { 13, 2, &placePoints13_2Point0 };
StagePoint placePoints14_1Point0 = { 0x2ED, 14, 1, 224, 192, 5, NULL };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints14_2Point0 = { 0x2ED, 14, 2, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints14_2 = { 14, 2, &placePoints14_2Point0 };
StagePoint placePoints15_1Point0 = { 0x2E8, 15, 1, 176, 0x168, 5, NULL };
StagePoints placePoints15_1 = { 15, 1, &placePoints15_1Point0 };
StagePoint placePoints16_1Point0 = { 0x2ED, 16, 1, 224, 192, 5, NULL };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints16_2Point0 = { 0x2ED, 16, 1, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints16_2 = { 16, 2, &placePoints16_2Point0 };
StagePoint placePoints17_1Point0 = { 0x2E8, 17, 1, 176, 0x168, 5, NULL };
StagePoints placePoints17_1 = { 17, 1, &placePoints17_1Point0 };
StagePoint placePoints18_1Point0 = { 0x2EC, 18, 1, 240, 0x1D8, 5, NULL };
StagePoints placePoints18_1 = { 18, 1, &placePoints18_1Point0 };
StagePoint placePoints19_1Point0 = { 0x2ED, 19, 2, 224, 192, 5, NULL };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints19_2Point0 = { 0x2ED, 19, 2, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints19_2 = { 19, 2, &placePoints19_2Point0 };
StagePoint placePoints19_3Point0 = { 0x2ED, 19, 3, 224, 192, 5, NULL };
StagePoints placePoints19_3 = { 19, 3, &placePoints19_3Point0 };
StagePoint placePoints19_4Point0 = { 0x2EC, 19, 5, 240, 0x1D8, 5, NULL };
StagePoints placePoints19_4 = { 19, 4, &placePoints19_4Point0 };
StagePoint placePoints20_1Point0 = { 0x2ED, 20, 2, 224, 192, 5, NULL };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints20_2Point0 = { 0x2ED, 20, 3, 224, 192, 5, NULL };
StagePoints placePoints20_2 = { 20, 2, &placePoints20_2Point0 };
StagePoint placePoints20_3Point0 = { 0x2ED, 20, 4, 224, 192, 5, NULL };
StagePoints placePoints20_3 = { 20, 3, &placePoints20_3Point0 };
StagePoint placePoints20_4Point0 = { 0x2EC, 20, 6, 240, 0x1D8, 5, NULL };
StagePoints placePoints20_4 = { 20, 4, &placePoints20_4Point0 };
StagePoint placePoints20_5Point0 = { 0x2EC, 20, 8, 240, 0x1D8, 5, NULL };
StagePoints placePoints20_5 = { 20, 5, &placePoints20_5Point0 };
StagePoint placePoints20_6Point0 = { 0x2EC, 20, 9, 240, 0x1D8, 5, NULL };
StagePoints placePoints20_6 = { 20, 6, &placePoints20_6Point0 };
StagePoint placePoints20_7Point0 = { 0x2ED, 20, 6, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints20_7 = { 20, 7, &placePoints20_7Point0 };
StagePoint placePoints21_1Point0 = { 0x2EE, 21, 1, 224, 0x240, 5, NULL };
StagePoints placePoints21_1 = { 21, 1, &placePoints21_1Point0 };
StagePoint placePoints22_1Point0 = { 0x2EC, 22, 3, 240, 0x1D8, 5, NULL };
StagePoints placePoints22_1 = { 22, 1, &placePoints22_1Point0 };
StagePoint placePoints23_1Point0 = { 0x2EE, 23, 1, 224, 0x240, 5, NULL };
StagePoints placePoints23_1 = { 23, 1, &placePoints23_1Point0 };
StagePoint placePoints24_1Point0 = { 0x2EC, 24, 5, 240, 0x1D8, 5, NULL };
StagePoints placePoints24_1 = { 24, 1, &placePoints24_1Point0 };
StagePoint placePoints25_1Point0 = { 0x2ED, 25, 1, 224, 192, 5, NULL };
StagePoints placePoints25_1 = { 25, 1, &placePoints25_1Point0 };
StagePoint placePoints25_2Point0 = { 0x2ED, 25, 1, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints25_2 = { 25, 2, &placePoints25_2Point0 };
StagePoint placePoints26_1Point0 = { 0x2E8, 26, 1, 176, 0x168, 5, NULL };
StagePoints placePoints26_1 = { 26, 1, &placePoints26_1Point0 };
StagePoint placePoints27_1Point0 = { 0x2ED, 27, 1, 224, 192, 5, NULL };
StagePoints placePoints27_1 = { 27, 1, &placePoints27_1Point0 };
StagePoint placePoints27_2Point0 = { 0x2ED, 27, 2, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints27_2 = { 27, 2, &placePoints27_2Point0 };
StagePoint placePoints28_1Point0 = { 0x2EC, 28, 3, 240, 0x1D8, 5, NULL };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints28_2Point0 = { 0x2EC, 28, 4, 240, 0x1D8, 5, NULL };
StagePoints placePoints28_2 = { 28, 2, &placePoints28_2Point0 };
StagePoint placePoints28_3Point0 = { 0x2EE, 28, 2, 224, 0x240, 5, NULL };
StagePoints placePoints28_3 = { 28, 3, &placePoints28_3Point0 };
StagePoint placePoints29_1Point0 = { 0x2ED, 29, 1, 224, 192, 5, NULL };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints29_2Point0 = { 0x2ED, 29, 2, 224, 192, 5, NULL };
StagePoints placePoints29_2 = { 29, 2, &placePoints29_2Point0 };
StagePoint placePoints29_3Point0 = { 0x2ED, 29, 2, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints29_3 = { 29, 3, &placePoints29_3Point0 };
StagePoint placePoints30_1Point0 = { 0x2EC, 30, 1, 240, 0x1D8, 5, NULL };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoint placePoints30_2Point0 = { 0x2EE, 30, 2, 224, 0x240, 5, NULL };
StagePoints placePoints30_2 = { 30, 2, &placePoints30_2Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints4_1, &placePoints7_1, &placePoints12_1,
    &placePoints12_2, &placePoints13_1, &placePoints13_2, &placePoints14_1,
    &placePoints14_2, &placePoints15_1, &placePoints16_1, &placePoints16_2,
    &placePoints17_1, &placePoints18_1, &placePoints19_1, &placePoints19_2,
    &placePoints19_3, &placePoints19_4, &placePoints20_1, &placePoints20_2,
    &placePoints20_3, &placePoints20_4, &placePoints20_5, &placePoints20_6,
    &placePoints20_7, &placePoints21_1, &placePoints22_1, &placePoints23_1,
    &placePoints24_1, &placePoints25_1, &placePoints25_2, &placePoints26_1,
    &placePoints27_1, &placePoints27_2, &placePoints28_1, &placePoints28_2,
    &placePoints28_3, &placePoints29_1, &placePoints29_2, &placePoints29_3,
    &placePoints30_1, &placePoints30_2, NULL,
};
AreaBattle place1Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    1,
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
AreaBattle place4Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place4Area0Battles = {
    1,
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
AreaBattle place7Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle6 = { 182, 10, MUSIC(2, 0) };
AreaBattle place7Area0Battle7 = { 182, 10, MUSIC(2, 0) };
BattleList place7Area0Battles = {
    1,
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
AreaBattle place12Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle4 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place12Area0Battles = {
    1,
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
AreaBattle place13Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place13Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place13Area0Battles = {
    1,
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
AreaBattle place14Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place14Area0Battles = {
    4,
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
AreaBattle place15Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place15Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place15Area0Battles = {
    5,
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
AreaBattle place16Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place16Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place16Area0Battles = {
    4,
    { &place16Area0Battle0, &place16Area0Battle1, &place16Area0Battle2, &place16Area0Battle3,
      &place16Area0Battle4, &place16Area0Battle5, &place16Area0Battle6, &place16Area0Battle7 },
};
AreaBattle place16Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area1Battles = {
    0,
    { &place16Area1Battle0, &place16Area1Battle1, &place16Area1Battle2, &place16Area1Battle3,
      &place16Area1Battle4, &place16Area1Battle5, &place16Area1Battle6, &place16Area1Battle7 },
};
AreaBattle place16Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area2Battles = {
    0,
    { &place16Area2Battle0, &place16Area2Battle1, &place16Area2Battle2, &place16Area2Battle3,
      &place16Area2Battle4, &place16Area2Battle5, &place16Area2Battle6, &place16Area2Battle7 },
};
AreaBattle place16Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area3Battles = {
    0,
    { &place16Area3Battle0, &place16Area3Battle1, &place16Area3Battle2, &place16Area3Battle3,
      &place16Area3Battle4, &place16Area3Battle5, &place16Area3Battle6, &place16Area3Battle7 },
};
AreaBattle place17Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place17Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place17Area0Battles = {
    5,
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
AreaBattle place18Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place18Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place18Area0Battles = {
    5,
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
AreaBattle place19Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle4 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place19Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place19Area0Battles = {
    1,
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
AreaBattle place20Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place20Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place20Area0Battles = {
    1,
    { &place20Area0Battle0, &place20Area0Battle1, &place20Area0Battle2, &place20Area0Battle3,
      &place20Area0Battle4, &place20Area0Battle5, &place20Area0Battle6, &place20Area0Battle7 },
};
AreaBattle place20Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place20Area1Battles = {
    0,
    { &place20Area1Battle0, &place20Area1Battle1, &place20Area1Battle2, &place20Area1Battle3,
      &place20Area1Battle4, &place20Area1Battle5, &place20Area1Battle6, &place20Area1Battle7 },
};
AreaBattle place20Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place20Area2Battles = {
    0,
    { &place20Area2Battle0, &place20Area2Battle1, &place20Area2Battle2, &place20Area2Battle3,
      &place20Area2Battle4, &place20Area2Battle5, &place20Area2Battle6, &place20Area2Battle7 },
};
AreaBattle place20Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place20Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place20Area3Battles = {
    0,
    { &place20Area3Battle0, &place20Area3Battle1, &place20Area3Battle2, &place20Area3Battle3,
      &place20Area3Battle4, &place20Area3Battle5, &place20Area3Battle6, &place20Area3Battle7 },
};
AreaBattle place21Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle4 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place21Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place21Area0Battles = {
    3,
    { &place21Area0Battle0, &place21Area0Battle1, &place21Area0Battle2, &place21Area0Battle3,
      &place21Area0Battle4, &place21Area0Battle5, &place21Area0Battle6, &place21Area0Battle7 },
};
AreaBattle place21Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place21Area1Battles = {
    0,
    { &place21Area1Battle0, &place21Area1Battle1, &place21Area1Battle2, &place21Area1Battle3,
      &place21Area1Battle4, &place21Area1Battle5, &place21Area1Battle6, &place21Area1Battle7 },
};
AreaBattle place21Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place21Area2Battles = {
    0,
    { &place21Area2Battle0, &place21Area2Battle1, &place21Area2Battle2, &place21Area2Battle3,
      &place21Area2Battle4, &place21Area2Battle5, &place21Area2Battle6, &place21Area2Battle7 },
};
AreaBattle place21Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place21Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place21Area3Battles = {
    0,
    { &place21Area3Battle0, &place21Area3Battle1, &place21Area3Battle2, &place21Area3Battle3,
      &place21Area3Battle4, &place21Area3Battle5, &place21Area3Battle6, &place21Area3Battle7 },
};
AreaBattle place22Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle4 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place22Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place22Area0Battles = {
    3,
    { &place22Area0Battle0, &place22Area0Battle1, &place22Area0Battle2, &place22Area0Battle3,
      &place22Area0Battle4, &place22Area0Battle5, &place22Area0Battle6, &place22Area0Battle7 },
};
AreaBattle place22Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place22Area1Battles = {
    0,
    { &place22Area1Battle0, &place22Area1Battle1, &place22Area1Battle2, &place22Area1Battle3,
      &place22Area1Battle4, &place22Area1Battle5, &place22Area1Battle6, &place22Area1Battle7 },
};
AreaBattle place22Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place22Area2Battles = {
    0,
    { &place22Area2Battle0, &place22Area2Battle1, &place22Area2Battle2, &place22Area2Battle3,
      &place22Area2Battle4, &place22Area2Battle5, &place22Area2Battle6, &place22Area2Battle7 },
};
AreaBattle place22Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place22Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place22Area3Battles = {
    0,
    { &place22Area3Battle0, &place22Area3Battle1, &place22Area3Battle2, &place22Area3Battle3,
      &place22Area3Battle4, &place22Area3Battle5, &place22Area3Battle6, &place22Area3Battle7 },
};
AreaBattle place23Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place23Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place23Area0Battles = {
    2,
    { &place23Area0Battle0, &place23Area0Battle1, &place23Area0Battle2, &place23Area0Battle3,
      &place23Area0Battle4, &place23Area0Battle5, &place23Area0Battle6, &place23Area0Battle7 },
};
AreaBattle place23Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place23Area1Battles = {
    0,
    { &place23Area1Battle0, &place23Area1Battle1, &place23Area1Battle2, &place23Area1Battle3,
      &place23Area1Battle4, &place23Area1Battle5, &place23Area1Battle6, &place23Area1Battle7 },
};
AreaBattle place23Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place23Area2Battles = {
    0,
    { &place23Area2Battle0, &place23Area2Battle1, &place23Area2Battle2, &place23Area2Battle3,
      &place23Area2Battle4, &place23Area2Battle5, &place23Area2Battle6, &place23Area2Battle7 },
};
AreaBattle place23Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place23Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place23Area3Battles = {
    0,
    { &place23Area3Battle0, &place23Area3Battle1, &place23Area3Battle2, &place23Area3Battle3,
      &place23Area3Battle4, &place23Area3Battle5, &place23Area3Battle6, &place23Area3Battle7 },
};
AreaBattle place24Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place24Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place24Area0Battles = {
    5,
    { &place24Area0Battle0, &place24Area0Battle1, &place24Area0Battle2, &place24Area0Battle3,
      &place24Area0Battle4, &place24Area0Battle5, &place24Area0Battle6, &place24Area0Battle7 },
};
AreaBattle place24Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place24Area1Battles = {
    0,
    { &place24Area1Battle0, &place24Area1Battle1, &place24Area1Battle2, &place24Area1Battle3,
      &place24Area1Battle4, &place24Area1Battle5, &place24Area1Battle6, &place24Area1Battle7 },
};
AreaBattle place24Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place24Area2Battles = {
    0,
    { &place24Area2Battle0, &place24Area2Battle1, &place24Area2Battle2, &place24Area2Battle3,
      &place24Area2Battle4, &place24Area2Battle5, &place24Area2Battle6, &place24Area2Battle7 },
};
AreaBattle place24Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place24Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place24Area3Battles = {
    0,
    { &place24Area3Battle0, &place24Area3Battle1, &place24Area3Battle2, &place24Area3Battle3,
      &place24Area3Battle4, &place24Area3Battle5, &place24Area3Battle6, &place24Area3Battle7 },
};
AreaBattle place25Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place25Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place25Area0Battles = {
    4,
    { &place25Area0Battle0, &place25Area0Battle1, &place25Area0Battle2, &place25Area0Battle3,
      &place25Area0Battle4, &place25Area0Battle5, &place25Area0Battle6, &place25Area0Battle7 },
};
AreaBattle place25Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place25Area1Battles = {
    0,
    { &place25Area1Battle0, &place25Area1Battle1, &place25Area1Battle2, &place25Area1Battle3,
      &place25Area1Battle4, &place25Area1Battle5, &place25Area1Battle6, &place25Area1Battle7 },
};
AreaBattle place25Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place25Area2Battles = {
    0,
    { &place25Area2Battle0, &place25Area2Battle1, &place25Area2Battle2, &place25Area2Battle3,
      &place25Area2Battle4, &place25Area2Battle5, &place25Area2Battle6, &place25Area2Battle7 },
};
AreaBattle place25Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place25Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place25Area3Battles = {
    0,
    { &place25Area3Battle0, &place25Area3Battle1, &place25Area3Battle2, &place25Area3Battle3,
      &place25Area3Battle4, &place25Area3Battle5, &place25Area3Battle6, &place25Area3Battle7 },
};
AreaBattle place26Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place26Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place26Area0Battles = {
    5,
    { &place26Area0Battle0, &place26Area0Battle1, &place26Area0Battle2, &place26Area0Battle3,
      &place26Area0Battle4, &place26Area0Battle5, &place26Area0Battle6, &place26Area0Battle7 },
};
AreaBattle place26Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place26Area1Battles = {
    0,
    { &place26Area1Battle0, &place26Area1Battle1, &place26Area1Battle2, &place26Area1Battle3,
      &place26Area1Battle4, &place26Area1Battle5, &place26Area1Battle6, &place26Area1Battle7 },
};
AreaBattle place26Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place26Area2Battles = {
    0,
    { &place26Area2Battle0, &place26Area2Battle1, &place26Area2Battle2, &place26Area2Battle3,
      &place26Area2Battle4, &place26Area2Battle5, &place26Area2Battle6, &place26Area2Battle7 },
};
AreaBattle place26Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place26Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place26Area3Battles = {
    0,
    { &place26Area3Battle0, &place26Area3Battle1, &place26Area3Battle2, &place26Area3Battle3,
      &place26Area3Battle4, &place26Area3Battle5, &place26Area3Battle6, &place26Area3Battle7 },
};
AreaBattle place27Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle6 = { 170, 10, MUSIC(2, 0) };
AreaBattle place27Area0Battle7 = { 170, 10, MUSIC(2, 0) };
BattleList place27Area0Battles = {
    2,
    { &place27Area0Battle0, &place27Area0Battle1, &place27Area0Battle2, &place27Area0Battle3,
      &place27Area0Battle4, &place27Area0Battle5, &place27Area0Battle6, &place27Area0Battle7 },
};
AreaBattle place27Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place27Area1Battles = {
    0,
    { &place27Area1Battle0, &place27Area1Battle1, &place27Area1Battle2, &place27Area1Battle3,
      &place27Area1Battle4, &place27Area1Battle5, &place27Area1Battle6, &place27Area1Battle7 },
};
AreaBattle place27Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place27Area2Battles = {
    0,
    { &place27Area2Battle0, &place27Area2Battle1, &place27Area2Battle2, &place27Area2Battle3,
      &place27Area2Battle4, &place27Area2Battle5, &place27Area2Battle6, &place27Area2Battle7 },
};
AreaBattle place27Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place27Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place27Area3Battles = {
    0,
    { &place27Area3Battle0, &place27Area3Battle1, &place27Area3Battle2, &place27Area3Battle3,
      &place27Area3Battle4, &place27Area3Battle5, &place27Area3Battle6, &place27Area3Battle7 },
};
AreaBattle place28Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place28Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place28Area0Battles = {
    1,
    { &place28Area0Battle0, &place28Area0Battle1, &place28Area0Battle2, &place28Area0Battle3,
      &place28Area0Battle4, &place28Area0Battle5, &place28Area0Battle6, &place28Area0Battle7 },
};
AreaBattle place28Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place28Area1Battles = {
    0,
    { &place28Area1Battle0, &place28Area1Battle1, &place28Area1Battle2, &place28Area1Battle3,
      &place28Area1Battle4, &place28Area1Battle5, &place28Area1Battle6, &place28Area1Battle7 },
};
AreaBattle place28Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place28Area2Battles = {
    0,
    { &place28Area2Battle0, &place28Area2Battle1, &place28Area2Battle2, &place28Area2Battle3,
      &place28Area2Battle4, &place28Area2Battle5, &place28Area2Battle6, &place28Area2Battle7 },
};
AreaBattle place28Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place28Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place28Area3Battles = {
    0,
    { &place28Area3Battle0, &place28Area3Battle1, &place28Area3Battle2, &place28Area3Battle3,
      &place28Area3Battle4, &place28Area3Battle5, &place28Area3Battle6, &place28Area3Battle7 },
};
AreaBattle place29Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place29Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place29Area0Battles = {
    1,
    { &place29Area0Battle0, &place29Area0Battle1, &place29Area0Battle2, &place29Area0Battle3,
      &place29Area0Battle4, &place29Area0Battle5, &place29Area0Battle6, &place29Area0Battle7 },
};
AreaBattle place29Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place29Area1Battles = {
    0,
    { &place29Area1Battle0, &place29Area1Battle1, &place29Area1Battle2, &place29Area1Battle3,
      &place29Area1Battle4, &place29Area1Battle5, &place29Area1Battle6, &place29Area1Battle7 },
};
AreaBattle place29Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place29Area2Battles = {
    0,
    { &place29Area2Battle0, &place29Area2Battle1, &place29Area2Battle2, &place29Area2Battle3,
      &place29Area2Battle4, &place29Area2Battle5, &place29Area2Battle6, &place29Area2Battle7 },
};
AreaBattle place29Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place29Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place29Area3Battles = {
    0,
    { &place29Area3Battle0, &place29Area3Battle1, &place29Area3Battle2, &place29Area3Battle3,
      &place29Area3Battle4, &place29Area3Battle5, &place29Area3Battle6, &place29Area3Battle7 },
};
AreaBattle place30Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place30Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place30Area0Battles = {
    1,
    { &place30Area0Battle0, &place30Area0Battle1, &place30Area0Battle2, &place30Area0Battle3,
      &place30Area0Battle4, &place30Area0Battle5, &place30Area0Battle6, &place30Area0Battle7 },
};
AreaBattle place30Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place30Area1Battles = {
    0,
    { &place30Area1Battle0, &place30Area1Battle1, &place30Area1Battle2, &place30Area1Battle3,
      &place30Area1Battle4, &place30Area1Battle5, &place30Area1Battle6, &place30Area1Battle7 },
};
AreaBattle place30Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place30Area2Battles = {
    0,
    { &place30Area2Battle0, &place30Area2Battle1, &place30Area2Battle2, &place30Area2Battle3,
      &place30Area2Battle4, &place30Area2Battle5, &place30Area2Battle6, &place30Area2Battle7 },
};
AreaBattle place30Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place30Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place30Area3Battles = {
    0,
    { &place30Area3Battle0, &place30Area3Battle1, &place30Area3Battle2, &place30Area3Battle3,
      &place30Area3Battle4, &place30Area3Battle5, &place30Area3Battle6, &place30Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 232, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 251, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 267, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 293, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 301, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 303, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
    { 306, 15, 0, { &place15Area0Battles, &place15Area1Battles, &place15Area2Battles, &place15Area3Battles } },
    { 311, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 313, 17, 0, { &place17Area0Battles, &place17Area1Battles, &place17Area2Battles, &place17Area3Battles } },
    { 316, 18, 0, { &place18Area0Battles, &place18Area1Battles, &place18Area2Battles, &place18Area3Battles } },
    { 322, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 328, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 333, 21, 0, { &place21Area0Battles, &place21Area1Battles, &place21Area2Battles, &place21Area3Battles } },
    { 337, 22, 0, { &place22Area0Battles, &place22Area1Battles, &place22Area2Battles, &place22Area3Battles } },
    { 341, 23, 0, { &place23Area0Battles, &place23Area1Battles, &place23Area2Battles, &place23Area3Battles } },
    { 344, 24, 0, { &place24Area0Battles, &place24Area1Battles, &place24Area2Battles, &place24Area3Battles } },
    { 349, 25, 0, { &place25Area0Battles, &place25Area1Battles, &place25Area2Battles, &place25Area3Battles } },
    { 351, 26, 0, { &place26Area0Battles, &place26Area1Battles, &place26Area2Battles, &place26Area3Battles } },
    { 354, 27, 0, { &place27Area0Battles, &place27Area1Battles, &place27Area2Battles, &place27Area3Battles } },
    { 361, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 367, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 373, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x152, 0x140, 0x48, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x158, 0x140, 0x60, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x15E, 0x140, 0x78, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x164, 0x140, 0x90, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x140, 0xA8, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x170, 0x140, 0xC0, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x176, 0x140, 0xD8, 0x40, 0x170, 0x1FE },
    { 0x140, 0x100, 0x14C, 0x160, 0x30, 0x60, 0x140, 0x1FD },
    { 0x140, 0x100, 0x152, 0x160, 0x48, 0x60, 0x150, 0x1FD },
    { 0x140, 0x100, 0x158, 0x160, 0x60, 0x60, 0x160, 0x1FD },
    { 0x140, 0x100, 0x15E, 0x160, 0x78, 0x60, 0x170, 0x1FD },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x140, 0x1FC },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FC },
    { 0x140, 0x100, 0x164, 0x160, 0x90, 0x60, 0x160, 0x1FC },
    { 0x140, 0x100, 0x16A, 0x160, 0xA8, 0x60, 0x170, 0x1FC },
    { 0x140, 0x100, 0x170, 0x160, 0xC0, 0x60, 0x140, 0x1FB },
    { 0x140, 0x100, 0x176, 0x160, 0xD8, 0x60, 0x150, 0x1FB },
    { 0x140, 0x100, 0x140, 0x170, 0, 0x70, 0x160, 0x1FB },
    { 0x140, 0x100, 0x146, 0x170, 0x18, 0x70, 0x170, 0x1FB },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x140, 0x1FA },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x150, 0x1FA },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x2D), 1, ITEM(1, 0x2F), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x2E), 1, ITEM(1, 0x30), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 0x2F), 1, ITEM(1, 0x31), 1, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(2, 0x30), 1, ITEM(1, 0x32), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(2, 0x31), 1, ITEM(1, 0x33), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(2, 0x32), 1, ITEM(1, 0x34), 1, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(2, 0x33), 1, ITEM(1, 0x35), 1, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(2, 0x34), 1, ITEM(1, 0x37), 1, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(2, 0x35), 1, ITEM(1, 0x38), 1, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(2, 0x36), 1, ITEM(1, 0x39), 1, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(2, 0x37), 1, ITEM(1, 0x3A), 1, CODES_END };
u16 actor11Talk0Actions[] = { FLAG(2, 0x56), 1, ITEM(1, 0x3B), 1, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(2, 0x57), 1, ITEM(1, 0x3C), 1, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(2, 0x58), 1, ITEM(1, 0x3D), 1, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(2, 0x59), 1, ITEM(2, 0x70), 1, CODES_END };
u16 actor18Talk0Actions[] = { FLAG(2, 0x5A), 1, ITEM(5, 0x11B), 1, CODES_END };
u16 actor19Talk0Actions[] = { FLAG(2, 0x5B), 1, ITEM(5, 0x11C), 1, CODES_END };
u16 actor20Talk0Actions[] = { FLAG(2, 0x5C), 1, ITEM(5, 0xFF), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x38E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x388 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x389 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x38A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x38B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x38C },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x38D },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x38F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0x390 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, actor9Talk0Actions, 0x391 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, actor10Talk0Actions, 0x392 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, actor11Talk0Actions, 0x393 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, actor15Talk0Actions, 0x394 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, actor16Talk0Actions, 0x395 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, actor17Talk0Actions, 0x396 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, actor18Talk0Actions, 0x397 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, actor19Talk0Actions, 0x398 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, actor20Talk0Actions, 0x399 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0x15), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x2D), 0, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(0x12), 1, WARP_ARG(0x21), 1, FLAG(2, 0x2E), 0, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x2F), 0, CODES_END };
u16 actor3Conditions[] = { WARP_ARG(0x18), 1, WARP_ARG(0x1F), 1, FLAG(2, 0x30), 0, CODES_END };
u16 actor4Conditions[] = { WARP_ARG(0xE), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x31), 0, CODES_END };
u16 actor5Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x32), 0, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(0x11), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x33), 0, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(0xF), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x34), 0, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0x1C), 1, WARP_ARG(0x1F), 1, FLAG(2, 0x35), 0, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0xD), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x36), 0, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(0x10), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x37), 0, CODES_END };
u16 actor11Conditions[] = { FLAG(2, 0x56), 0, WARP_ARG(0xC), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(0x18), 1, FLAG(0, 8), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0x1B), 1, FLAG(0, 8), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(6), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x57), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0x13), 1, WARP_ARG(0x20), 1, FLAG(2, 0x58), 0, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0x13), 1, WARP_ARG(0x23), 1, FLAG(2, 0x59), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0x19), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x5A), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0x17), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x5B), 0, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0x1B), 1, WARP_ARG(0x1F), 1, FLAG(2, 0x5C), 0, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor22Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor23Conditions[] = { WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor24Conditions[] = { WARP_ARG(0x21), 1, FLAG(0, 9), 0, CODES_END };
u16 actor25Conditions[] = { WARP_ARG(0), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor26Conditions[] = { WARP_ARG(0xB), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor27Conditions[] = { WARP_ARG(0xC), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor28Conditions[] = { WARP_ARG(0xD), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor29Conditions[] = { WARP_ARG(0xF), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor30Conditions[] = { WARP_ARG(0x10), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor31Conditions[] = { WARP_ARG(0x12), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor32Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor33Conditions[] = { WARP_ARG(0x17), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor34Conditions[] = { WARP_ARG(0x18), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor35Conditions[] = { WARP_ARG(0x1A), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor36Conditions[] = { WARP_ARG(0x1B), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor37Conditions[] = { WARP_ARG(0x1C), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor38Conditions[] = { WARP_ARG(0x1D), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 240, 600, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x4D, 5, 240, 600, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x4E, 6, 240, 600, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x4F, 7, 240, 600, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x50, 8, 240, 600, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x51, 9, 260, 600, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x52, 0xA, 240, 600, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x53, 0xB, 240, 600, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x54, 0xC, 240, 600, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x55, 0xD, 240, 600, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x56, 0xE, 240, 600, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x57, 0xF, 240, 600, 1 };
FieldActorEntry actor12 = { NULL, NULL, 0x146, 0x10, 0, 0, 0 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x148, 0x11, 336, 312, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x148, 0x11, 400, 344, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x154, 0x12, 240, 600, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x155, 0x13, 240, 600, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x156, 0x14, 240, 600, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x157, 0x15, 240, 600, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x158, 0x16, 240, 600, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x15B, 0x17, 240, 600, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x15F, 0x18, 288, 400, 1 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x15F, 0x18, 208, 440, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x15F, 0x18, 160, 464, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0x15F, 0x18, 480, 208, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x160, 0x19, 192, 544, 1 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x160, 0x19, 480, 208, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x160, 0x19, 352, 272, 1 };
FieldActorEntry actor28 = { actor28Conditions, NULL, 0x160, 0x19, 336, 312, 1 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x160, 0x19, 240, 488, 1 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x160, 0x19, 336, 376, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x160, 0x19, 352, 272, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x160, 0x19, 400, 344, 1 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x160, 0x19, 352, 272, 1 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0x160, 0x19, 192, 544, 1 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x160, 0x19, 240, 488, 1 };
FieldActorEntry actor36 = { actor36Conditions, NULL, 0x160, 0x19, 480, 208, 1 };
FieldActorEntry actor37 = { actor37Conditions, NULL, 0x160, 0x19, 352, 272, 1 };
FieldActorEntry actor38 = { actor38Conditions, NULL, 0x160, 0x19, 336, 376, 1 };
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
    &actor12,
    &actor13,
    &actor14,
    &actor15,
    &actor16,
    &actor17,
    &actor18,
    &actor19,
    &actor20,
    &actor21,
    &actor22,
    &actor23,
    &actor24,
    &actor25,
    &actor26,
    &actor27,
    &actor28,
    &actor29,
    &actor30,
    &actor31,
    &actor32,
    &actor33,
    &actor34,
    &actor35,
    &actor36,
    &actor37,
    &actor38,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EB, 0x240, 0xA0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
