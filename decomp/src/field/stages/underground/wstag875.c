/*
 * WSTAG875: Circuit Board, Underground: the Black Kingz' den. A tunnel only
 * WSTAG885 to 895 reach: the Black Kingz and their leader, who has Etemon's
 * Mic.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x67C
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x68C
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x20000, 0xB900};
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

StagePoint placePoints2_1Point0 = { 0x2ED, 2, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point0 = { 0x2EE, 2, 2, 0x130, 200, 1, NULL };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints4_1Point0 = { 0x2EE, 4, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point0 = { 0x2ED, 5, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints6_1Point0 = { 0x2EC, 6, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point0 = { 0x2ED, 6, 2, 0x3A0, 128, 1, NULL };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints9_1Point0 = { 0x2ED, 9, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints9_2Point0 = { 0x2EE, 9, 2, 0x130, 200, 1, NULL };
StagePoints placePoints9_2 = { 9, 2, &placePoints9_2Point0 };
StagePoint placePoints12_1Point0 = { 0x2EC, 12, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints12_2Point0 = { 0x2EC, 12, 2, 0x3B0, 120, 1, NULL };
StagePoints placePoints12_2 = { 12, 2, &placePoints12_2Point0 };
StagePoint placePoints13_1Point0 = { 0x2EC, 13, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints13_2Point0 = { 0x2ED, 13, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints13_2 = { 13, 2, &placePoints13_2Point0 };
StagePoint placePoints14_1Point0 = { 0x2ED, 14, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints16_1Point0 = { 0x2EE, 16, 1, 0x130, 200, 1, NULL };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints19_1Point0 = { 0x2EC, 19, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints19_2Point0 = { 0x2EE, 19, 1, 0x130, 200, 1, NULL };
StagePoints placePoints19_2 = { 19, 2, &placePoints19_2Point0 };
StagePoint placePoints20_1Point0 = { 0x2EE, 20, 1, 0x130, 200, 1, NULL };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints20_2Point0 = { 0x2EC, 20, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints20_2 = { 20, 2, &placePoints20_2Point0 };
StagePoint placePoints21_1Point0 = { 0x2EC, 21, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints21_1 = { 21, 1, &placePoints21_1Point0 };
StagePoint placePoints25_1Point0 = { 0x2EE, 25, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints25_1 = { 25, 1, &placePoints25_1Point0 };
StagePoint placePoints27_1Point0 = { 0x2ED, 27, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints27_1 = { 27, 1, &placePoints27_1Point0 };
StagePoint placePoints28_1Point0 = { 0x2EC, 28, 1, 0x3B0, 120, 1, NULL };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints28_2Point0 = { 0x2EC, 28, 2, 0x3B0, 120, 1, NULL };
StagePoints placePoints28_2 = { 28, 2, &placePoints28_2Point0 };
StagePoint placePoints29_1Point0 = { 0x2EC, 29, 2, 0x3B0, 120, 1, NULL };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints30_1Point0 = { 0x2EE, 30, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoints *placePoints[] = {
    &placePoints2_1, &placePoints2_2, &placePoints4_1, &placePoints5_1,
    &placePoints6_1, &placePoints6_2, &placePoints9_1, &placePoints9_2,
    &placePoints12_1, &placePoints12_2, &placePoints13_1, &placePoints13_2,
    &placePoints14_1, &placePoints16_1, &placePoints19_1, &placePoints19_2,
    &placePoints20_1, &placePoints20_2, &placePoints21_1, &placePoints25_1,
    &placePoints27_1, &placePoints28_1, &placePoints28_2, &placePoints29_1,
    &placePoints30_1, NULL,
};
AreaBattle place2Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place2Area0Battles = {
    1,
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
AreaBattle place5Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 170, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place5Area0Battles = {
    1,
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
AreaBattle place6Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 170, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 110, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 110, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place6Area0Battles = {
    1,
    { &place6Area0Battle0, &place6Area0Battle1, &place6Area0Battle2, &place6Area0Battle3,
      &place6Area0Battle4, &place6Area0Battle5, &place6Area0Battle6, &place6Area0Battle7 },
};
AreaBattle place6Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area1Battles = {
    0,
    { &place6Area1Battle0, &place6Area1Battle1, &place6Area1Battle2, &place6Area1Battle3,
      &place6Area1Battle4, &place6Area1Battle5, &place6Area1Battle6, &place6Area1Battle7 },
};
AreaBattle place6Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area2Battles = {
    0,
    { &place6Area2Battle0, &place6Area2Battle1, &place6Area2Battle2, &place6Area2Battle3,
      &place6Area2Battle4, &place6Area2Battle5, &place6Area2Battle6, &place6Area2Battle7 },
};
AreaBattle place6Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area3Battles = {
    0,
    { &place6Area3Battle0, &place6Area3Battle1, &place6Area3Battle2, &place6Area3Battle3,
      &place6Area3Battle4, &place6Area3Battle5, &place6Area3Battle6, &place6Area3Battle7 },
};
AreaBattle place9Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place9Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place9Area0Battles = {
    1,
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
    5,
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
AreaBattle place12Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place12Area0Battle7 = { 71, 10, MUSIC(2, 0) };
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
AreaBattle place14Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place14Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place14Area0Battles = {
    1,
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
FieldBattles stageBattles[] = {
    { 236, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 247, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 252, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 258, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 274, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 295, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 307, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 317, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 323, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 329, 21, 0, { &place21Area0Battles, &place21Area1Battles, &place21Area2Battles, &place21Area3Battles } },
    { 346, 25, 0, { &place25Area0Battles, &place25Area1Battles, &place25Area2Battles, &place25Area3Battles } },
    { 352, 27, 0, { &place27Area0Battles, &place27Area1Battles, &place27Area2Battles, &place27Area3Battles } },
    { 356, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 363, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 370, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
    { 377, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 378, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x154, 0x140, 0x50, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15C, 0x140, 0x70, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x164, 0x140, 0x90, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x16C, 0x140, 0xB0, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x174, 0x140, 0xD0, 0x40, 0x170, 0x1FE },
    { 0x140, 0x100, 0x14C, 0x160, 0x30, 0x60, 0x140, 0x1FD },
    { 0x140, 0x100, 0x154, 0x160, 0x50, 0x60, 0x150, 0x1FD },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x170, 0x1FD },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x3F), 0, CODES_END };
u16 actor2Talk0Actions[] = { CARD_BATTLE(0x45, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x3F), 1, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0x1C, 0x3F), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1C, 0x40), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk0Actions[] = { CARD_BATTLE(0x46, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1C, 0x40), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0x1C, 0x40), 1,
    ITEM(0, 0x18D), 1,
    CODES_END,
};
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x20), 0, CODES_END };
u16 actor4Talk0Actions[] = { CARD_BATTLE(0x47, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x20), 1, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x20), 1, CODES_END };
u16 actor5Talk0Conditions[] = { CARD(0x22), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk0Actions[] = { CARD_BATTLE(0x48, 0), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x22), 1, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x22), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x1F), 0, CODES_END };
u16 actor6Talk0Actions[] = { CARD_BATTLE(0x4B, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x1F), 1, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor6Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x1F), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x45), 0, CODES_END };
u16 actor7Talk0Actions[] = { CARD_BATTLE(0x4E, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x45), 1, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x45), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0xF1), 0, CODES_END };
u16 actor8Talk0Actions[] = { CARD_BATTLE(0x4F, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0xF1), 1, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0xF1), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x70), 0, CODES_END };
u16 actor9Talk0Actions[] = { CARD_BATTLE(0x50, 0), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x70), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x70), 1, CODES_END };
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x37F },
    { actor2Talk1Conditions, NULL, 0x380 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x381 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x382 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x383 },
    { actor3Talk1Conditions, NULL, 0x384 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x385 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x386 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x367 },
    { actor4Talk1Conditions, NULL, 0x368 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x369 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x36A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x367 },
    { actor5Talk1Conditions, NULL, 0x368 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x369 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x36B },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x367 },
    { actor6Talk1Conditions, NULL, 0x368 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x369 },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x36E },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x377 },
    { actor7Talk1Conditions, NULL, 0x378 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x379 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x37C },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x377 },
    { actor8Talk1Conditions, NULL, 0x378 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x379 },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x37D },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x377 },
    { actor9Talk1Conditions, NULL, 0x378 },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x379 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x37E },
    { NULL, NULL, 0 },
};
u16 actor1Conditions[] = { WARP_ARG(8), 1, FLAG(0, 8), 0, CODES_END };
u16 actor2Conditions[] = {
    WARP_ARG(0xD), 1,
    WARP_ARG(0x1E), 1,
    FLAG(0x1C, 0x42), 1,
    FLAG(0x1C, 0x3F), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    WARP_ARG(0x1B), 1,
    WARP_ARG(0x1F), 1,
    FLAG(0x1C, 0x3F), 1,
    FLAG(0x1C, 0x40), 0,
    CODES_END,
};
u16 actor4Conditions[] = {
    ITEM(0, 0x14), 1,
    CARD(0x20), 0,
    WARP_ARG(0x1A), 1,
    WARP_ARG(0x1E), 1,
    CODES_END,
};
u16 actor5Conditions[] = {
    WARP_ARG(5), 1,
    WARP_ARG(0x1F), 1,
    ITEM(0, 0x14), 1,
    CARD(0x22), 0,
    CODES_END,
};
u16 actor6Conditions[] = {
    WARP_ARG(0x1C), 1,
    WARP_ARG(0x1E), 1,
    ITEM(0, 0x14), 1,
    CARD(0x1F), 0,
    CODES_END,
};
u16 actor7Conditions[] = {
    WARP_ARG(0x14), 1,
    WARP_ARG(0x1E), 1,
    ITEM(0, 0x14), 1,
    CARD(0x45), 0,
    CODES_END,
};
u16 actor8Conditions[] = {
    WARP_ARG(0x12), 1,
    WARP_ARG(0x1E), 1,
    ITEM(0, 0x14), 1,
    CARD(0xF1), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    WARP_ARG(8), 1,
    WARP_ARG(0x1F), 1,
    ITEM(0, 0x14), 1,
    CARD(0x70), 0,
    CODES_END,
};
u16 actor10Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(1), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(5), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(8), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0xC), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0xD), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0x12), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0x14), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0x1B), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { NULL, NULL, 0x146, 4, 0, 0, 0 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0x148, 5, 368, 440, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x14D, 6, 432, 144, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x14E, 7, 432, 144, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x14F, 8, 432, 144, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x150, 9, 432, 144, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x153, 0xA, 432, 144, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x15C, 0xB, 432, 144, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x15D, 0xC, 432, 144, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x15E, 0xD, 432, 144, 7 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x15F, 0xE, 384, 336, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x15F, 0xE, 272, 488, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x160, 0xF, 392, 380, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x160, 0xF, 320, 368, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x160, 0xF, 544, 176, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x160, 0xF, 528, 264, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x160, 0xF, 520, 220, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x160, 0xF, 464, 248, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x160, 0xF, 456, 300, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x160, 0xF, 480, 192, 1 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x160, 0xF, 520, 220, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EA, 0xE0, 0x200, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
