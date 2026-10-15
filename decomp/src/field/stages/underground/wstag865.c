/*
 * WSTAG865: Circuit Board, Underground: entered from the sectors' maps. A
 * tunnel entered from maps of Central, East, South, West and North Sector; it
 * joins WSTAG880 to 895.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x6A0
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x6B0
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xF900, 0x14000};
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

StagePoint placePoints1_1Point1 = { 0x2EC, 1, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_1Point0 = { 0x21D, 0, 0, 0x410, 0x400, 0, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point1 = { 0x2ED, 1, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints1_2Point0 = { 0x248, 0, 0, 0x320, 0x518, 0, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints2_1Point1 = { 0x2EC, 2, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x261, 0, 0, 176, 0x2EE, 0, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints3_1Point1 = { 0x2EC, 3, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x2CD, 0, 0, 0x1C0, 0x206, 0, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point1 = { 0x2ED, 3, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints3_2Point0 = { 0x2A7, 0, 0, 0x560, 0x188, 0, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point1 = { 0x2EE, 3, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_3Point0 = { 0x21D, 0, 0, 0x110, 0x290, 0, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints3_4Point1 = { 0x2ED, 3, 4, 0x3A0, 128, 1, NULL };
StagePoint placePoints3_4Point0 = { 0x28C, 0, 0, 0x110, 0x290, 0, &placePoints3_4Point1 };
StagePoints placePoints3_4 = { 3, 4, &placePoints3_4Point0 };
StagePoint placePoints3_5Point1 = { 0x2EE, 3, 5, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_5Point0 = { 0x2B1, 0, 0, 0x3D0, 0x100, 0, &placePoints3_5Point1 };
StagePoints placePoints3_5 = { 3, 5, &placePoints3_5Point0 };
StagePoint placePoints4_1Point1 = { 0x2EE, 4, 1, 0x130, 200, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x23C, 0, 0, 192, 224, 0, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point1 = { 0x2EE, 4, 2, 0x130, 200, 1, NULL };
StagePoint placePoints4_2Point0 = { 0x2A9, 0, 0, 192, 224, 0, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints5_1Point1 = { 0x2EE, 5, 1, 0x130, 200, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2CD, 0, 0, 0x4C0, 200, 0, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point1 = { 0x2EE, 5, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_2Point0 = { 0x2C9, 0, 0, 0x4B0, 0x13E, 0, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point1 = { 0x2ED, 5, 4, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_3Point0 = { 0x2CC, 0, 0, 192, 0x214, 0, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints6_1Point1 = { 0x2ED, 6, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_1Point0 = { 0x247, 0, 0, 0x190, 0x1C0, 0, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints7_1Point1 = { 0x2EC, 7, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints7_1Point0 = { 0x2CA, 0, 0, 0x3BE, 0x2EE, 0, &placePoints7_1Point1 };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints7_2Point1 = { 0x2EC, 7, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints7_2Point0 = { 0x262, 0, 0, 0x3BE, 0x2EE, 0, &placePoints7_2Point1 };
StagePoints placePoints7_2 = { 7, 2, &placePoints7_2Point0 };
StagePoint placePoints7_3Point1 = { 0x2EE, 7, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints7_3Point0 = { 0x298, 0, 0, 0x560, 0x238, 0, &placePoints7_3Point1 };
StagePoints placePoints7_3 = { 7, 3, &placePoints7_3Point0 };
StagePoint placePoints8_1Point1 = { 0x2ED, 8, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints8_1Point0 = { 0x2B2, 0, 0, 0x320, 0x518, 0, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints8_2Point1 = { 0x2EC, 8, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints8_2Point0 = { 0x28C, 0, 0, 0x410, 0x400, 0, &placePoints8_2Point1 };
StagePoints placePoints8_2 = { 8, 2, &placePoints8_2Point0 };
StagePoint placePoints9_1Point1 = { 0x2EC, 9, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints9_1Point0 = { 0x2C9, 0, 0, 176, 0x2EE, 0, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints10_1Point1 = { 0x2EC, 10, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints10_1Point0 = { 0x265, 0, 0, 0x4AE, 0x1EF, 0, &placePoints10_1Point1 };
StagePoints placePoints10_1 = { 10, 1, &placePoints10_1Point0 };
StagePoint placePoints10_2Point1 = { 0x2ED, 10, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints10_2Point0 = { 0x264, 0, 0, 0x100, 0x2A4, 0, &placePoints10_2Point1 };
StagePoints placePoints10_2 = { 10, 2, &placePoints10_2Point0 };
StagePoint placePoints11_1Point1 = { 0x2EC, 11, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints11_1Point0 = { 0x2CD, 0, 0, 0x4AE, 0x1EF, 0, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints11_2Point1 = { 0x2ED, 11, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints11_2Point0 = { 0x2CC, 0, 0, 0x100, 0x2A4, 0, &placePoints11_2Point1 };
StagePoints placePoints11_2 = { 11, 2, &placePoints11_2Point0 };
StagePoint placePoints12_1Point1 = { 0x2ED, 12, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints12_1Point0 = { 0x261, 0, 0, 0x2F0, 0x30E, 0, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints13_1Point1 = { 0x2EC, 13, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints13_1Point0 = { 0x2C9, 0, 0, 0x2F0, 0x30E, 0, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints15_1Point1 = { 0x2EB, 15, 1, 0x240, 160, 1, NULL };
StagePoint placePoints15_1Point0 = { 0x220, 0, 0, 0x398, 0x270, 0, &placePoints15_1Point1 };
StagePoints placePoints15_1 = { 15, 1, &placePoints15_1Point0 };
StagePoint placePoints16_1Point1 = { 0x2EE, 16, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints16_1Point0 = { 0x24C, 0, 0, 0x200, 0x178, 0, &placePoints16_1Point1 };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints17_1Point1 = { 0x2EB, 17, 1, 0x240, 160, 1, NULL };
StagePoint placePoints17_1Point0 = { 0x21D, 0, 0, 0x290, 0x200, 0, &placePoints17_1Point1 };
StagePoints placePoints17_1 = { 17, 1, &placePoints17_1Point0 };
StagePoint placePoints18_1Point1 = { 0x2EC, 18, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints18_1Point0 = { 0x2B4, 0, 0, 0x3F0, 0x330, 0, &placePoints18_1Point1 };
StagePoints placePoints18_1 = { 18, 1, &placePoints18_1Point0 };
StagePoint placePoints19_1Point1 = { 0x2EC, 19, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints19_1Point0 = { 0x262, 0, 0, 190, 166, 0, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints20_1Point1 = { 0x2EE, 20, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints20_1Point0 = { 0x2CA, 0, 0, 190, 166, 0, &placePoints20_1Point1 };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints21_1Point1 = { 0x2EE, 21, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints21_1Point0 = { 0x264, 0, 0, 0x3A0, 180, 0, &placePoints21_1Point1 };
StagePoints placePoints21_1 = { 21, 1, &placePoints21_1Point0 };
StagePoint placePoints22_1Point1 = { 0x2EC, 22, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints22_1Point0 = { 0x265, 0, 0, 0x23F, 0x317, 0, &placePoints22_1Point1 };
StagePoints placePoints22_1 = { 22, 1, &placePoints22_1Point0 };
StagePoint placePoints22_2Point1 = { 0x2EC, 22, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints22_2Point0 = { 0x261, 0, 0, 0x1A0, 0x166, 0, &placePoints22_2Point1 };
StagePoints placePoints22_2 = { 22, 2, &placePoints22_2Point0 };
StagePoint placePoints23_1Point1 = { 0x2EC, 23, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints23_1Point0 = { 0x2CD, 0, 0, 0x23F, 0x317, 0, &placePoints23_1Point1 };
StagePoints placePoints23_1 = { 23, 1, &placePoints23_1Point0 };
StagePoint placePoints23_2Point1 = { 0x2EE, 23, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints23_2Point0 = { 0x2C9, 0, 0, 0x1A0, 0x166, 0, &placePoints23_2Point1 };
StagePoints placePoints23_2 = { 23, 2, &placePoints23_2Point0 };
StagePoint placePoints24_1Point1 = { 0x2EC, 24, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints24_1Point0 = { 0x2CC, 0, 0, 0x3A0, 180, 0, &placePoints24_1Point1 };
StagePoints placePoints24_1 = { 24, 1, &placePoints24_1Point0 };
StagePoint placePoints25_1Point1 = { 0x2EE, 25, 1, 0x130, 200, 1, NULL };
StagePoint placePoints25_1Point0 = { 0x2B6, 0, 0, 0x200, 0x178, 0, &placePoints25_1Point1 };
StagePoints placePoints25_1 = { 25, 1, &placePoints25_1Point0 };
StagePoint placePoints26_1Point1 = { 0x2EB, 26, 1, 0x240, 160, 1, NULL };
StagePoint placePoints26_1Point0 = { 0x2A7, 0, 0, 0x1C1, 0x37A, 0, &placePoints26_1Point1 };
StagePoints placePoints26_1 = { 26, 1, &placePoints26_1Point0 };
StagePoint placePoints28_1Point1 = { 0x2ED, 28, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints28_1Point0 = { 0x28C, 0, 0, 0x290, 0x200, 0, &placePoints28_1Point1 };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints29_1Point1 = { 0x2EC, 29, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints29_1Point0 = { 0x28F, 0, 0, 0x398, 0x270, 0, &placePoints29_1Point1 };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints30_1Point1 = { 0x2ED, 30, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints30_1Point0 = { 0x2B4, 0, 0, 0x2C0, 0x2A8, 0, &placePoints30_1Point1 };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoint placePoints30_2Point1 = { 0x2ED, 30, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints30_2Point0 = { 0x2B2, 0, 0, 0x2A0, 0x138, 0, &placePoints30_2Point1 };
StagePoints placePoints30_2 = { 30, 2, &placePoints30_2Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints2_1, &placePoints3_1,
    &placePoints3_2, &placePoints3_3, &placePoints3_4, &placePoints3_5,
    &placePoints4_1, &placePoints4_2, &placePoints5_1, &placePoints5_2,
    &placePoints5_3, &placePoints6_1, &placePoints7_1, &placePoints7_2,
    &placePoints7_3, &placePoints8_1, &placePoints8_2, &placePoints9_1,
    &placePoints10_1, &placePoints10_2, &placePoints11_1, &placePoints11_2,
    &placePoints12_1, &placePoints13_1, &placePoints15_1, &placePoints16_1,
    &placePoints17_1, &placePoints18_1, &placePoints19_1, &placePoints20_1,
    &placePoints21_1, &placePoints22_1, &placePoints22_2, &placePoints23_1,
    &placePoints23_2, &placePoints24_1, &placePoints25_1, &placePoints26_1,
    &placePoints28_1, &placePoints29_1, &placePoints30_1, &placePoints30_2,
    NULL,
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
AreaBattle place3Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 182, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 182, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place3Area0Battles = {
    1,
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
AreaBattle place8Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place8Area0Battle7 = { 71, 10, MUSIC(2, 0) };
BattleList place8Area0Battles = {
    1,
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
AreaBattle place10Area0Battle0 = { 174, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle1 = { 174, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle2 = { 170, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle3 = { 170, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle4 = { 170, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle5 = { 110, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle6 = { 110, 10, MUSIC(2, 0) };
AreaBattle place10Area0Battle7 = { 110, 10, MUSIC(2, 0) };
BattleList place10Area0Battles = {
    2,
    { &place10Area0Battle0, &place10Area0Battle1, &place10Area0Battle2, &place10Area0Battle3,
      &place10Area0Battle4, &place10Area0Battle5, &place10Area0Battle6, &place10Area0Battle7 },
};
AreaBattle place10Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place10Area1Battles = {
    0,
    { &place10Area1Battle0, &place10Area1Battle1, &place10Area1Battle2, &place10Area1Battle3,
      &place10Area1Battle4, &place10Area1Battle5, &place10Area1Battle6, &place10Area1Battle7 },
};
AreaBattle place10Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place10Area2Battles = {
    0,
    { &place10Area2Battle0, &place10Area2Battle1, &place10Area2Battle2, &place10Area2Battle3,
      &place10Area2Battle4, &place10Area2Battle5, &place10Area2Battle6, &place10Area2Battle7 },
};
AreaBattle place10Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place10Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place10Area3Battles = {
    0,
    { &place10Area3Battle0, &place10Area3Battle1, &place10Area3Battle2, &place10Area3Battle3,
      &place10Area3Battle4, &place10Area3Battle5, &place10Area3Battle6, &place10Area3Battle7 },
};
AreaBattle place11Area0Battle0 = { 182, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle1 = { 182, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle2 = { 182, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle3 = { 182, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle4 = { 71, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle5 = { 71, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle6 = { 71, 10, MUSIC(2, 0) };
AreaBattle place11Area0Battle7 = { 71, 10, MUSIC(2, 0) };
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
    { 229, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 235, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 241, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 246, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 253, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 259, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 264, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 268, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 273, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 279, 10, 0, { &place10Area0Battles, &place10Area1Battles, &place10Area2Battles, &place10Area3Battles } },
    { 284, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 289, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 297, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 305, 15, 0, { &place15Area0Battles, &place15Area1Battles, &place15Area2Battles, &place15Area3Battles } },
    { 308, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 312, 17, 0, { &place17Area0Battles, &place17Area1Battles, &place17Area2Battles, &place17Area3Battles } },
    { 314, 18, 0, { &place18Area0Battles, &place18Area1Battles, &place18Area2Battles, &place18Area3Battles } },
    { 319, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 324, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 331, 21, 0, { &place21Area0Battles, &place21Area1Battles, &place21Area2Battles, &place21Area3Battles } },
    { 334, 22, 0, { &place22Area0Battles, &place22Area1Battles, &place22Area2Battles, &place22Area3Battles } },
    { 338, 23, 0, { &place23Area0Battles, &place23Area1Battles, &place23Area2Battles, &place23Area3Battles } },
    { 342, 24, 0, { &place24Area0Battles, &place24Area1Battles, &place24Area2Battles, &place24Area3Battles } },
    { 345, 25, 0, { &place25Area0Battles, &place25Area1Battles, &place25Area2Battles, &place25Area3Battles } },
    { 350, 26, 0, { &place26Area0Battles, &place26Area1Battles, &place26Area2Battles, &place26Area3Battles } },
    { 357, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 362, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 368, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x174, 0x120, 0xD0, 0x20, 0x160, 0x1FF },
    { 0x140, 0x100, 0x160, 0x140, 0x80, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x160, 0x100, 0x80, 0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x170, 0x1FE },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x3A6 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x3A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x3A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x3A9 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(7), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor3Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor4Conditions[] = { WARP_ARG(0x19), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(2), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(4), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(7), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(9), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0xA), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0x15), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x42, 4, 648, 232, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x42, 4, 648, 232, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xB6, 5, 648, 232, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xB6, 5, 648, 232, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xB6, 5, 648, 232, 1 };
FieldActorEntry actor5 = { NULL, NULL, 0x146, 6, 0, 0, 0 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x15F, 7, 224, 336, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x15F, 7, 320, 320, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x15F, 7, 480, 304, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x160, 8, 432, 328, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x160, 8, 368, 344, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x160, 8, 528, 280, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x160, 8, 528, 280, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x160, 8, 368, 344, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x160, 8, 272, 312, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x160, 8, 368, 344, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x14, 0, 552, 100, 213, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E8, 0x240, 0xD0, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E8, 0xB0, 0x168, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
