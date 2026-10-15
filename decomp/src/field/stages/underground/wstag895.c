/*
 * WSTAG895: Circuit Board, Underground: a deep tunnel with guides to both
 * servers. A tunnel only the other tunnels reach (WSTAG865 to 890), with
 * guides to both servers' exits.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x689
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x699
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x13B00, 0x21000};
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

StagePoint placePoints1_1Point2 = { 0x2E9, 1, 2, 0x240, 240, 1, NULL };
StagePoint placePoints1_1Point1 = { 0x2ED, 1, 1, 0x350, 0x1F8, 5, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x2ED, 1, 2, 224, 192, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints2_1Point2 = { 0x2ED, 2, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints2_1Point1 = { 0x2EC, 2, 1, 240, 0x1D8, 5, &placePoints2_1Point2 };
StagePoint placePoints2_1Point0 = { 0x2ED, 2, 1, 224, 192, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point2 = { 0x2E9, 2, 3, 0x240, 240, 1, NULL };
StagePoint placePoints2_2Point1 = { 0x2EA, 2, 2, 224, 0x200, 5, &placePoints2_2Point2 };
StagePoint placePoints2_2Point0 = { 0x2ED, 2, 2, 224, 192, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints3_1Point2 = { 0x2ED, 3, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints3_1Point1 = { 0x2EC, 3, 1, 240, 0x1D8, 5, &placePoints3_1Point2 };
StagePoint placePoints3_1Point0 = { 0x2ED, 3, 1, 224, 192, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point2 = { 0x2ED, 3, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints3_2Point1 = { 0x2ED, 3, 1, 0x350, 0x1F8, 5, &placePoints3_2Point2 };
StagePoint placePoints3_2Point0 = { 0x2E8, 3, 3, 176, 0x168, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point2 = { 0x2E9, 3, 2, 0x240, 240, 1, NULL };
StagePoint placePoints3_3Point1 = { 0x2ED, 3, 2, 0x350, 0x1F8, 5, &placePoints3_3Point2 };
StagePoint placePoints3_3Point0 = { 0x2ED, 3, 3, 224, 192, 5, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints3_4Point2 = { 0x2E9, 3, 3, 0x240, 240, 1, NULL };
StagePoint placePoints3_4Point1 = { 0x2ED, 3, 3, 0x350, 0x1F8, 5, &placePoints3_4Point2 };
StagePoint placePoints3_4Point0 = { 0x2ED, 3, 4, 224, 192, 5, &placePoints3_4Point1 };
StagePoints placePoints3_4 = { 3, 4, &placePoints3_4Point0 };
StagePoint placePoints3_5Point2 = { 0x2E9, 3, 4, 0x240, 240, 1, NULL };
StagePoint placePoints3_5Point1 = { 0x2ED, 3, 4, 0x350, 0x1F8, 5, &placePoints3_5Point2 };
StagePoint placePoints3_5Point0 = { 0x2E8, 3, 5, 176, 0x168, 5, &placePoints3_5Point1 };
StagePoints placePoints3_5 = { 3, 5, &placePoints3_5Point0 };
StagePoint placePoints4_1Point2 = { 0x2ED, 4, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints4_1Point1 = { 0x2E8, 4, 1, 176, 0x168, 5, &placePoints4_1Point2 };
StagePoint placePoints4_1Point0 = { 0x2EA, 4, 1, 224, 0x200, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point2 = { 0x2ED, 4, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints4_2Point1 = { 0x2E8, 4, 2, 176, 0x168, 5, &placePoints4_2Point2 };
StagePoint placePoints4_2Point0 = { 0x2ED, 4, 1, 224, 192, 5, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints5_1Point2 = { 0x2ED, 5, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_1Point1 = { 0x2E8, 5, 1, 176, 0x168, 5, &placePoints5_1Point2 };
StagePoint placePoints5_1Point0 = { 0x2ED, 5, 1, 224, 192, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point2 = { 0x2ED, 5, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_2Point1 = { 0x2ED, 5, 1, 0x350, 0x1F8, 5, &placePoints5_2Point2 };
StagePoint placePoints5_2Point0 = { 0x2E8, 5, 2, 176, 0x168, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point2 = { 0x2E9, 5, 2, 0x240, 240, 1, NULL };
StagePoint placePoints5_3Point1 = { 0x2ED, 5, 2, 0x350, 0x1F8, 5, &placePoints5_3Point2 };
StagePoint placePoints5_3Point0 = { 0x2ED, 5, 3, 224, 192, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints5_4Point2 = { 0x2E9, 5, 3, 0x240, 240, 1, NULL };
StagePoint placePoints5_4Point1 = { 0x2ED, 5, 3, 0x350, 0x1F8, 5, &placePoints5_4Point2 };
StagePoint placePoints5_4Point0 = { 0x2ED, 5, 4, 224, 192, 5, &placePoints5_4Point1 };
StagePoints placePoints5_4 = { 5, 4, &placePoints5_4Point0 };
StagePoint placePoints6_1Point2 = { 0x2E9, 6, 1, 0x240, 240, 1, NULL };
StagePoint placePoints6_1Point1 = { 0x2EC, 6, 1, 240, 0x1D8, 5, &placePoints6_1Point2 };
StagePoint placePoints6_1Point0 = { 0x2ED, 6, 1, 224, 192, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point2 = { 0x2EE, 6, 3, 0x130, 200, 1, NULL };
StagePoint placePoints6_2Point1 = { 0x2ED, 6, 1, 0x350, 0x1F8, 5, &placePoints6_2Point2 };
StagePoint placePoints6_2Point0 = { 0x2ED, 6, 2, 224, 192, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints6_3Point2 = { 0x2E9, 6, 2, 0x240, 240, 1, NULL };
StagePoint placePoints6_3Point1 = { 0x2EE, 6, 2, 224, 0x240, 5, &placePoints6_3Point2 };
StagePoint placePoints6_3Point0 = { 0x2EC, 6, 2, 240, 0x1D8, 5, &placePoints6_3Point1 };
StagePoints placePoints6_3 = { 6, 3, &placePoints6_3Point0 };
StagePoint placePoints7_1Point2 = { 0x2EC, 7, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints7_1Point1 = { 0x2EC, 7, 1, 240, 0x1D8, 5, &placePoints7_1Point2 };
StagePoint placePoints7_1Point0 = { 0x2EC, 7, 2, 240, 0x1D8, 5, &placePoints7_1Point1 };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints7_2Point2 = { 0x2EB, 7, 1, 0x240, 160, 1, NULL };
StagePoint placePoints7_2Point1 = { 0x2EC, 7, 3, 240, 0x1D8, 5, &placePoints7_2Point2 };
StagePoint placePoints7_2Point0 = { 0x2E8, 7, 3, 176, 0x168, 5, &placePoints7_2Point1 };
StagePoints placePoints7_2 = { 7, 2, &placePoints7_2Point0 };
StagePoint placePoints8_1Point2 = { 0x2E9, 8, 2, 0x240, 240, 1, NULL };
StagePoint placePoints8_1Point1 = { 0x2ED, 8, 1, 0x350, 0x1F8, 5, &placePoints8_1Point2 };
StagePoint placePoints8_1Point0 = { 0x2ED, 8, 2, 224, 192, 5, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints9_1Point2 = { 0x2ED, 9, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints9_1Point1 = { 0x2EC, 9, 1, 240, 0x1D8, 5, &placePoints9_1Point2 };
StagePoint placePoints9_1Point0 = { 0x2ED, 9, 1, 224, 192, 5, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints9_2Point2 = { 0x2E9, 9, 2, 0x240, 240, 1, NULL };
StagePoint placePoints9_2Point1 = { 0x2EA, 9, 2, 224, 0x200, 5, &placePoints9_2Point2 };
StagePoint placePoints9_2Point0 = { 0x2ED, 9, 2, 224, 192, 5, &placePoints9_2Point1 };
StagePoints placePoints9_2 = { 9, 2, &placePoints9_2Point0 };
StagePoint placePoints10_1Point2 = { 0x2E9, 10, 1, 0x240, 240, 1, NULL };
StagePoint placePoints10_1Point1 = { 0x2EC, 10, 1, 240, 0x1D8, 5, &placePoints10_1Point2 };
StagePoint placePoints10_1Point0 = { 0x2ED, 10, 1, 224, 192, 5, &placePoints10_1Point1 };
StagePoints placePoints10_1 = { 10, 1, &placePoints10_1Point0 };
StagePoint placePoints11_1Point2 = { 0x2E9, 11, 1, 0x240, 240, 1, NULL };
StagePoint placePoints11_1Point1 = { 0x2EC, 11, 1, 240, 0x1D8, 5, &placePoints11_1Point2 };
StagePoint placePoints11_1Point0 = { 0x2ED, 11, 1, 224, 192, 5, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints12_1Point2 = { 0x2EC, 12, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints12_1Point1 = { 0x2ED, 12, 1, 0x350, 0x1F8, 5, &placePoints12_1Point2 };
StagePoint placePoints12_1Point0 = { 0x2ED, 12, 2, 224, 192, 5, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints12_2Point2 = { 0x2EC, 12, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints12_2Point1 = { 0x2ED, 12, 2, 0x350, 0x1F8, 5, &placePoints12_2Point2 };
StagePoint placePoints12_2Point0 = { 0x2EC, 12, 2, 240, 0x1D8, 5, &placePoints12_2Point1 };
StagePoints placePoints12_2 = { 12, 2, &placePoints12_2Point0 };
StagePoint placePoints13_1Point2 = { 0x2EC, 13, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints13_1Point1 = { 0x2ED, 13, 1, 0x350, 0x1F8, 5, &placePoints13_1Point2 };
StagePoint placePoints13_1Point0 = { 0x2ED, 13, 2, 224, 192, 5, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints13_2Point2 = { 0x2EC, 13, 6, 0x3B0, 120, 1, NULL };
StagePoint placePoints13_2Point1 = { 0x2ED, 13, 2, 0x350, 0x1F8, 5, &placePoints13_2Point2 };
StagePoint placePoints13_2Point0 = { 0x2EC, 13, 2, 240, 0x1D8, 5, &placePoints13_2Point1 };
StagePoints placePoints13_2 = { 13, 2, &placePoints13_2Point0 };
StagePoint placePoints16_1Point2 = { 0x2ED, 16, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints16_1Point1 = { 0x2EA, 16, 1, 224, 0x200, 5, &placePoints16_1Point2 };
StagePoint placePoints16_1Point0 = { 0x2E8, 16, 1, 176, 0x168, 5, &placePoints16_1Point1 };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints19_1Point2 = { 0x2EC, 19, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints19_1Point1 = { 0x2EA, 19, 2, 224, 0x200, 5, &placePoints19_1Point2 };
StagePoint placePoints19_1Point0 = { 0x2EC, 19, 1, 240, 0x1D8, 5, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints19_2Point2 = { 0x2ED, 19, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints19_2Point1 = { 0x2EC, 19, 2, 240, 0x1D8, 5, &placePoints19_2Point2 };
StagePoint placePoints19_2Point0 = { 0x2EC, 19, 3, 240, 0x1D8, 5, &placePoints19_2Point1 };
StagePoints placePoints19_2 = { 19, 2, &placePoints19_2Point0 };
StagePoint placePoints20_1Point2 = { 0x2EC, 20, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints20_1Point1 = { 0x2EA, 20, 1, 224, 0x200, 5, &placePoints20_1Point2 };
StagePoint placePoints20_1Point0 = { 0x2E8, 20, 1, 176, 0x168, 5, &placePoints20_1Point1 };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints20_2Point2 = { 0x2ED, 20, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints20_2Point1 = { 0x2EC, 20, 1, 240, 0x1D8, 5, &placePoints20_2Point2 };
StagePoint placePoints20_2Point0 = { 0x2EC, 20, 2, 240, 0x1D8, 5, &placePoints20_2Point1 };
StagePoints placePoints20_2 = { 20, 2, &placePoints20_2Point0 };
StagePoint placePoints21_1Point2 = { 0x2EB, 21, 1, 0x240, 160, 1, NULL };
StagePoint placePoints21_1Point1 = { 0x2EC, 21, 2, 240, 0x1D8, 5, &placePoints21_1Point2 };
StagePoint placePoints21_1Point0 = { 0x2E8, 21, 1, 176, 0x168, 5, &placePoints21_1Point1 };
StagePoints placePoints21_1 = { 21, 1, &placePoints21_1Point0 };
StagePoint placePoints22_1Point2 = { 0x2EC, 22, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints22_1Point1 = { 0x2EC, 22, 1, 240, 0x1D8, 5, &placePoints22_1Point2 };
StagePoint placePoints22_1Point0 = { 0x2EC, 22, 2, 240, 0x1D8, 5, &placePoints22_1Point1 };
StagePoints placePoints22_1 = { 22, 1, &placePoints22_1Point0 };
StagePoint placePoints23_1Point2 = { 0x2EB, 23, 1, 0x240, 160, 1, NULL };
StagePoint placePoints23_1Point1 = { 0x2EC, 23, 3, 240, 0x1D8, 5, &placePoints23_1Point2 };
StagePoint placePoints23_1Point0 = { 0x2E8, 23, 2, 176, 0x168, 5, &placePoints23_1Point1 };
StagePoints placePoints23_1 = { 23, 1, &placePoints23_1Point0 };
StagePoint placePoints25_1Point2 = { 0x2ED, 25, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints25_1Point1 = { 0x2E8, 25, 1, 176, 0x168, 5, &placePoints25_1Point2 };
StagePoint placePoints25_1Point0 = { 0x2EA, 25, 1, 224, 0x200, 5, &placePoints25_1Point1 };
StagePoints placePoints25_1 = { 25, 1, &placePoints25_1Point0 };
StagePoint placePoints28_1Point2 = { 0x2EC, 28, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints28_1Point1 = { 0x2ED, 28, 1, 0x350, 0x1F8, 5, &placePoints28_1Point2 };
StagePoint placePoints28_1Point0 = { 0x2ED, 28, 2, 224, 192, 5, &placePoints28_1Point1 };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints28_2Point2 = { 0x2EB, 28, 3, 0x240, 160, 1, NULL };
StagePoint placePoints28_2Point1 = { 0x2ED, 28, 2, 0x350, 0x1F8, 5, &placePoints28_2Point2 };
StagePoint placePoints28_2Point0 = { 0x2EC, 28, 2, 240, 0x1D8, 5, &placePoints28_2Point1 };
StagePoints placePoints28_2 = { 28, 2, &placePoints28_2Point0 };
StagePoint placePoints29_1Point2 = { 0x2ED, 29, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints29_1Point1 = { 0x2EC, 29, 1, 240, 0x1D8, 5, &placePoints29_1Point2 };
StagePoint placePoints29_1Point0 = { 0x2EC, 29, 2, 240, 0x1D8, 5, &placePoints29_1Point1 };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints30_1Point2 = { 0x2E9, 30, 1, 0x240, 240, 1, NULL };
StagePoint placePoints30_1Point1 = { 0x2ED, 30, 1, 0x350, 0x1F8, 5, &placePoints30_1Point2 };
StagePoint placePoints30_1Point0 = { 0x2ED, 30, 2, 224, 192, 5, &placePoints30_1Point1 };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoint placePoints30_2Point2 = { 0x2EB, 30, 2, 0x240, 160, 1, NULL };
StagePoint placePoints30_2Point1 = { 0x2ED, 30, 2, 0x350, 0x1F8, 5, &placePoints30_2Point2 };
StagePoint placePoints30_2Point0 = { 0x2EA, 30, 1, 224, 0x200, 5, &placePoints30_2Point1 };
StagePoints placePoints30_2 = { 30, 2, &placePoints30_2Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints2_1, &placePoints2_2, &placePoints3_1,
    &placePoints3_2, &placePoints3_3, &placePoints3_4, &placePoints3_5,
    &placePoints4_1, &placePoints4_2, &placePoints5_1, &placePoints5_2,
    &placePoints5_3, &placePoints5_4, &placePoints6_1, &placePoints6_2,
    &placePoints6_3, &placePoints7_1, &placePoints7_2, &placePoints8_1,
    &placePoints9_1, &placePoints9_2, &placePoints10_1, &placePoints11_1,
    &placePoints12_1, &placePoints12_2, &placePoints13_1, &placePoints13_2,
    &placePoints16_1, &placePoints19_1, &placePoints19_2, &placePoints20_1,
    &placePoints20_2, &placePoints21_1, &placePoints22_1, &placePoints23_1,
    &placePoints25_1, &placePoints28_1, &placePoints28_2, &placePoints29_1,
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
    { 233, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 239, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 244, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 248, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 255, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 262, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 266, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 272, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 277, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 282, 10, 0, { &place10Area0Battles, &place10Area1Battles, &place10Area2Battles, &place10Area3Battles } },
    { 287, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 292, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 299, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 309, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 320, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 325, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 332, 21, 0, { &place21Area0Battles, &place21Area1Battles, &place21Area2Battles, &place21Area3Battles } },
    { 336, 22, 0, { &place22Area0Battles, &place22Area1Battles, &place22Area2Battles, &place22Area3Battles } },
    { 340, 23, 0, { &place23Area0Battles, &place23Area1Battles, &place23Area2Battles, &place23Area3Battles } },
    { 347, 25, 0, { &place25Area0Battles, &place25Area1Battles, &place25Area2Battles, &place25Area3Battles } },
    { 360, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 365, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 372, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x154, 0x140, 0x50, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x15C, 0x140, 0x70, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x164, 0x140, 0x90, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x140, 0xB0, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x174, 0x140, 0xD0, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x14C, 0x160, 0x30, 0x60, 0x160, 0x1FE },
    { 0x140, 0x100, 0x154, 0x160, 0x50, 0x60, 0x170, 0x1FE },
    { 0x140, 0x100, 0x15C, 0x160, 0x70, 0x60, 0x140, 0x1FD },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x150, 0x1FD },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x140, 0x1FC },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x35E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x3BD },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3BF },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x3C0 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x3C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x361 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x3C3 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x3C4 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x3C5 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x3C6 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x364 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x3BE },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x3C1 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x3C2 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x3C8 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x35F },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x360 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x362 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x363 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x365 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x366 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(1), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor3Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor4Conditions[] = { WARP_ARG(0x1B), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor5Conditions[] = { WARP_ARG(6), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(0xB), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(0xC), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0x12), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0x13), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(7), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(8), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0x1C), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x20), 1, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0x16), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(9), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0x1D), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0xA), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0x15), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor22Conditions[] = { WARP_ARG(3), 1, FLAG(0, 8), 0, CODES_END };
u16 actor23Conditions[] = { WARP_ARG(5), 1, FLAG(0, 8), 0, CODES_END };
u16 actor24Conditions[] = { WARP_ARG(0xB), 1, FLAG(0, 8), 0, CODES_END };
u16 actor25Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor26Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor27Conditions[] = { WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor28Conditions[] = { WARP_ARG(2), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor29Conditions[] = { WARP_ARG(3), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor30Conditions[] = { WARP_ARG(4), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor31Conditions[] = { WARP_ARG(5), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor32Conditions[] = { WARP_ARG(8), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor33Conditions[] = { WARP_ARG(0xB), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor34Conditions[] = { WARP_ARG(0xC), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor35Conditions[] = { WARP_ARG(0x12), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor36Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor37Conditions[] = { WARP_ARG(0x1D), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x22, 4, 456, 424, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x22, 4, 168, 256, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x22, 4, 840, 520, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x22, 4, 168, 256, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x22, 4, 456, 424, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x40, 5, 456, 424, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x40, 5, 456, 424, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x40, 5, 840, 520, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x40, 5, 168, 256, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x40, 5, 840, 520, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xB4, 6, 456, 424, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xB4, 6, 840, 520, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xB4, 6, 840, 520, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xB4, 6, 168, 256, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xB4, 6, 168, 256, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xE2, 7, 456, 424, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0xE3, 8, 456, 424, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0xE7, 9, 456, 424, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0xE8, 0xA, 456, 424, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0xF1, 0xB, 456, 424, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0xF2, 0xC, 456, 424, 1 };
FieldActorEntry actor21 = { NULL, NULL, 0x146, 0xD, 0, 0, 0 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x148, 0xE, 256, 224, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x148, 0xE, 448, 528, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0x148, 0xE, 704, 416, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x15F, 0xF, 624, 304, 1 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x15F, 0xF, 720, 544, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x15F, 0xF, 784, 488, 1 };
FieldActorEntry actor28 = { actor28Conditions, NULL, 0x160, 0x10, 608, 512, 1 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x160, 0x10, 864, 448, 1 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x160, 0x10, 288, 544, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x160, 0x10, 440, 324, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x160, 0x10, 288, 544, 1 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x160, 0x10, 280, 300, 1 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0x160, 0x10, 864, 448, 1 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x160, 0x10, 608, 512, 1 };
FieldActorEntry actor36 = { actor36Conditions, NULL, 0x160, 0x10, 256, 224, 1 };
FieldActorEntry actor37 = { actor37Conditions, NULL, 0x160, 0x10, 280, 300, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EE, 0x3A0, 0x1A0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EE, 0x130, 0xC8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EE, 0xE0, 0x240, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
