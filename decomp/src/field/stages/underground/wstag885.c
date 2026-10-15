/*
 * WSTAG885: Circuit Board, Underground: a deep tunnel with Black Kingz and
 * Numemon. A tunnel only the other tunnels reach (WSTAG865 to 895), with Black
 * Kingz and Numemon battles.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x683
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x693
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x37200, 0x1CE00};
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

StagePoint placePoints1_1Point1 = { 0x2ED, 1, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints1_1Point0 = { 0x2E8, 1, 1, 176, 0x168, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints2_1Point1 = { 0x2EE, 2, 1, 0x130, 200, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x2E8, 2, 1, 176, 0x168, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints3_1Point1 = { 0x2EE, 3, 1, 0x130, 200, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x2E8, 3, 1, 176, 0x168, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point1 = { 0x2E9, 3, 1, 0x240, 240, 1, NULL };
StagePoint placePoints3_2Point0 = { 0x2ED, 3, 2, 224, 192, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints5_1Point1 = { 0x2E9, 5, 4, 0x240, 240, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2ED, 5, 4, 0x350, 0x1F8, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints6_1Point1 = { 0x2EE, 6, 1, 0x130, 200, 1, NULL };
StagePoint placePoints6_1Point0 = { 0x2EA, 6, 1, 224, 0x200, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point1 = { 0x2EE, 6, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_2Point0 = { 0x2ED, 6, 2, 0x350, 0x1F8, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints7_1Point1 = { 0x2EE, 7, 1, 0x130, 200, 1, NULL };
StagePoint placePoints7_1Point0 = { 0x2E8, 7, 1, 176, 0x168, 5, &placePoints7_1Point1 };
StagePoints placePoints7_1 = { 7, 1, &placePoints7_1Point0 };
StagePoint placePoints7_2Point1 = { 0x2EE, 7, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints7_2Point0 = { 0x2E8, 7, 2, 176, 0x168, 5, &placePoints7_2Point1 };
StagePoints placePoints7_2 = { 7, 2, &placePoints7_2Point0 };
StagePoint placePoints7_3Point1 = { 0x2EE, 7, 2, 0x130, 200, 1, NULL };
StagePoint placePoints7_3Point0 = { 0x2EE, 7, 1, 224, 0x240, 5, &placePoints7_3Point1 };
StagePoints placePoints7_3 = { 7, 3, &placePoints7_3Point0 };
StagePoint placePoints8_1Point1 = { 0x2ED, 8, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints8_1Point0 = { 0x2E8, 8, 2, 176, 0x168, 5, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints9_1Point1 = { 0x2EE, 9, 1, 0x130, 200, 1, NULL };
StagePoint placePoints9_1Point0 = { 0x2E8, 9, 1, 176, 0x168, 5, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints10_1Point1 = { 0x2EE, 10, 1, 0x130, 200, 1, NULL };
StagePoint placePoints10_1Point0 = { 0x2E8, 10, 1, 176, 0x168, 5, &placePoints10_1Point1 };
StagePoints placePoints10_1 = { 10, 1, &placePoints10_1Point0 };
StagePoint placePoints10_2Point1 = { 0x2E9, 10, 2, 0x240, 240, 1, NULL };
StagePoint placePoints10_2Point0 = { 0x2ED, 10, 1, 0x350, 0x1F8, 5, &placePoints10_2Point1 };
StagePoints placePoints10_2 = { 10, 2, &placePoints10_2Point0 };
StagePoint placePoints11_1Point1 = { 0x2EE, 11, 1, 0x130, 200, 1, NULL };
StagePoint placePoints11_1Point0 = { 0x2E8, 11, 1, 176, 0x168, 5, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints11_2Point1 = { 0x2E9, 11, 2, 0x240, 240, 1, NULL };
StagePoint placePoints11_2Point0 = { 0x2ED, 11, 1, 0x350, 0x1F8, 5, &placePoints11_2Point1 };
StagePoints placePoints11_2 = { 11, 2, &placePoints11_2Point0 };
StagePoint placePoints12_1Point1 = { 0x2ED, 12, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints12_1Point0 = { 0x2EA, 12, 1, 224, 0x200, 5, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints12_2Point1 = { 0x2EE, 12, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints12_2Point0 = { 0x2EA, 12, 2, 224, 0x200, 5, &placePoints12_2Point1 };
StagePoints placePoints12_2 = { 12, 2, &placePoints12_2Point0 };
StagePoint placePoints12_3Point1 = { 0x2EB, 12, 1, 0x240, 160, 1, NULL };
StagePoint placePoints12_3Point0 = { 0x2ED, 12, 1, 224, 192, 5, &placePoints12_3Point1 };
StagePoints placePoints12_3 = { 12, 3, &placePoints12_3Point0 };
StagePoint placePoints12_4Point1 = { 0x2EB, 12, 2, 0x240, 160, 1, NULL };
StagePoint placePoints12_4Point0 = { 0x2EE, 12, 1, 224, 0x240, 5, &placePoints12_4Point1 };
StagePoints placePoints12_4 = { 12, 4, &placePoints12_4Point0 };
StagePoint placePoints12_5Point1 = { 0x2E9, 12, 1, 0x240, 240, 1, NULL };
StagePoint placePoints12_5Point0 = { 0x2EE, 12, 2, 224, 0x240, 5, &placePoints12_5Point1 };
StagePoints placePoints12_5 = { 12, 5, &placePoints12_5Point0 };
StagePoint placePoints13_1Point1 = { 0x2ED, 13, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints13_1Point0 = { 0x2EA, 13, 1, 224, 0x200, 5, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints13_2Point1 = { 0x2EE, 13, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints13_2Point0 = { 0x2E8, 13, 1, 176, 0x168, 5, &placePoints13_2Point1 };
StagePoints placePoints13_2 = { 13, 2, &placePoints13_2Point0 };
StagePoint placePoints13_3Point1 = { 0x2EC, 13, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints13_3Point0 = { 0x2ED, 13, 1, 224, 192, 5, &placePoints13_3Point1 };
StagePoints placePoints13_3 = { 13, 3, &placePoints13_3Point0 };
StagePoint placePoints13_4Point1 = { 0x2E9, 13, 1, 0x240, 240, 1, NULL };
StagePoint placePoints13_4Point0 = { 0x2EC, 13, 3, 240, 0x1D8, 5, &placePoints13_4Point1 };
StagePoints placePoints13_4 = { 13, 4, &placePoints13_4Point0 };
StagePoint placePoints13_5Point1 = { 0x2EB, 13, 1, 0x240, 160, 1, NULL };
StagePoint placePoints13_5Point0 = { 0x2EE, 13, 1, 224, 0x240, 5, &placePoints13_5Point1 };
StagePoints placePoints13_5 = { 13, 5, &placePoints13_5Point0 };
StagePoint placePoints13_6Point1 = { 0x2EB, 13, 2, 0x240, 160, 1, NULL };
StagePoint placePoints13_6Point0 = { 0x2EE, 13, 2, 224, 0x240, 5, &placePoints13_6Point1 };
StagePoints placePoints13_6 = { 13, 6, &placePoints13_6Point0 };
StagePoint placePoints18_1Point1 = { 0x2EB, 18, 1, 0x240, 160, 1, NULL };
StagePoint placePoints18_1Point0 = { 0x2E8, 18, 1, 176, 0x168, 5, &placePoints18_1Point1 };
StagePoints placePoints18_1 = { 18, 1, &placePoints18_1Point0 };
StagePoint placePoints19_1Point1 = { 0x2EE, 19, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints19_1Point0 = { 0x2EA, 19, 1, 224, 0x200, 5, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints19_2Point1 = { 0x2EE, 19, 2, 0x130, 200, 1, NULL };
StagePoint placePoints19_2Point0 = { 0x2E8, 19, 1, 176, 0x168, 5, &placePoints19_2Point1 };
StagePoints placePoints19_2 = { 19, 2, &placePoints19_2Point0 };
StagePoint placePoints19_3Point1 = { 0x2EE, 19, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints19_3Point0 = { 0x2EE, 19, 1, 224, 0x240, 5, &placePoints19_3Point1 };
StagePoints placePoints19_3 = { 19, 3, &placePoints19_3Point0 };
StagePoint placePoints19_4Point1 = { 0x2ED, 19, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints19_4Point0 = { 0x2ED, 19, 1, 224, 192, 5, &placePoints19_4Point1 };
StagePoints placePoints19_4 = { 19, 4, &placePoints19_4Point0 };
StagePoint placePoints19_5Point1 = { 0x2EB, 19, 4, 0x240, 160, 1, NULL };
StagePoint placePoints19_5Point0 = { 0x2ED, 19, 3, 0x350, 0x1F8, 5, &placePoints19_5Point1 };
StagePoints placePoints19_5 = { 19, 5, &placePoints19_5Point0 };
StagePoint placePoints20_1Point1 = { 0x2EE, 20, 2, 0x130, 200, 1, NULL };
StagePoint placePoints20_1Point0 = { 0x2EA, 20, 2, 224, 0x200, 5, &placePoints20_1Point1 };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints20_2Point1 = { 0x2EE, 20, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints20_2Point0 = { 0x2EE, 20, 1, 224, 0x240, 5, &placePoints20_2Point1 };
StagePoints placePoints20_2 = { 20, 2, &placePoints20_2Point0 };
StagePoint placePoints20_3Point1 = { 0x2ED, 20, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints20_3Point0 = { 0x2ED, 20, 1, 224, 192, 5, &placePoints20_3Point1 };
StagePoints placePoints20_3 = { 20, 3, &placePoints20_3Point0 };
StagePoint placePoints20_4Point1 = { 0x2ED, 20, 4, 0x3A0, 128, 1, NULL };
StagePoint placePoints20_4Point0 = { 0x2ED, 20, 2, 0x350, 0x1F8, 5, &placePoints20_4Point1 };
StagePoints placePoints20_4 = { 20, 4, &placePoints20_4Point0 };
StagePoint placePoints20_5Point1 = { 0x2ED, 20, 5, 0x3A0, 128, 1, NULL };
StagePoint placePoints20_5Point0 = { 0x2ED, 20, 3, 0x350, 0x1F8, 5, &placePoints20_5Point1 };
StagePoints placePoints20_5 = { 20, 5, &placePoints20_5Point0 };
StagePoint placePoints20_6Point1 = { 0x2EB, 20, 4, 0x240, 160, 1, NULL };
StagePoint placePoints20_6Point0 = { 0x2ED, 20, 4, 0x350, 0x1F8, 5, &placePoints20_6Point1 };
StagePoints placePoints20_6 = { 20, 6, &placePoints20_6Point0 };
StagePoint placePoints20_7Point1 = { 0x2ED, 20, 6, 0x3A0, 128, 1, NULL };
StagePoint placePoints20_7Point0 = { 0x2ED, 20, 5, 224, 192, 5, &placePoints20_7Point1 };
StagePoints placePoints20_7 = { 20, 7, &placePoints20_7Point0 };
StagePoint placePoints20_8Point1 = { 0x2EB, 20, 5, 0x240, 160, 1, NULL };
StagePoint placePoints20_8Point0 = { 0x2ED, 20, 5, 0x350, 0x1F8, 5, &placePoints20_8Point1 };
StagePoints placePoints20_8 = { 20, 8, &placePoints20_8Point0 };
StagePoint placePoints20_9Point1 = { 0x2EB, 20, 6, 0x240, 160, 1, NULL };
StagePoint placePoints20_9Point0 = { 0x2ED, 20, 6, 224, 192, 5, &placePoints20_9Point1 };
StagePoints placePoints20_9 = { 20, 9, &placePoints20_9Point0 };
StagePoint placePoints21_1Point1 = { 0x2EC, 21, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints21_1Point0 = { 0x2EA, 21, 1, 224, 0x200, 5, &placePoints21_1Point1 };
StagePoints placePoints21_1 = { 21, 1, &placePoints21_1Point0 };
StagePoint placePoints21_2Point1 = { 0x2EE, 21, 1, 0x130, 200, 1, NULL };
StagePoint placePoints21_2Point0 = { 0x2EC, 21, 1, 240, 0x1D8, 5, &placePoints21_2Point1 };
StagePoints placePoints21_2 = { 21, 2, &placePoints21_2Point0 };
StagePoint placePoints22_1Point1 = { 0x2EE, 22, 1, 0x130, 200, 1, NULL };
StagePoint placePoints22_1Point0 = { 0x2E8, 22, 1, 176, 0x168, 5, &placePoints22_1Point1 };
StagePoints placePoints22_1 = { 22, 1, &placePoints22_1Point0 };
StagePoint placePoints22_2Point1 = { 0x2EE, 22, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints22_2Point0 = { 0x2E8, 22, 2, 176, 0x168, 5, &placePoints22_2Point1 };
StagePoints placePoints22_2 = { 22, 2, &placePoints22_2Point0 };
StagePoint placePoints22_3Point1 = { 0x2EB, 22, 1, 0x240, 160, 1, NULL };
StagePoint placePoints22_3Point0 = { 0x2EE, 22, 1, 224, 0x240, 5, &placePoints22_3Point1 };
StagePoints placePoints22_3 = { 22, 3, &placePoints22_3Point0 };
StagePoint placePoints23_1Point1 = { 0x2EC, 23, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints23_1Point0 = { 0x2E8, 23, 1, 176, 0x168, 5, &placePoints23_1Point1 };
StagePoints placePoints23_1 = { 23, 1, &placePoints23_1Point0 };
StagePoint placePoints23_2Point1 = { 0x2EC, 23, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints23_2Point0 = { 0x2EC, 23, 1, 240, 0x1D8, 5, &placePoints23_2Point1 };
StagePoints placePoints23_2 = { 23, 2, &placePoints23_2Point0 };
StagePoint placePoints23_3Point1 = { 0x2EE, 23, 1, 0x130, 200, 1, NULL };
StagePoint placePoints23_3Point0 = { 0x2EC, 23, 2, 240, 0x1D8, 5, &placePoints23_3Point1 };
StagePoints placePoints23_3 = { 23, 3, &placePoints23_3Point0 };
StagePoint placePoints24_1Point1 = { 0x2EC, 24, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints24_1Point0 = { 0x2E8, 24, 1, 176, 0x168, 5, &placePoints24_1Point1 };
StagePoints placePoints24_1 = { 24, 1, &placePoints24_1Point0 };
StagePoint placePoints24_2Point1 = { 0x2EC, 24, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints24_2Point0 = { 0x2EC, 24, 1, 240, 0x1D8, 5, &placePoints24_2Point1 };
StagePoints placePoints24_2 = { 24, 2, &placePoints24_2Point0 };
StagePoint placePoints24_3Point1 = { 0x2EC, 24, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints24_3Point0 = { 0x2EC, 24, 2, 240, 0x1D8, 5, &placePoints24_3Point1 };
StagePoints placePoints24_3 = { 24, 3, &placePoints24_3Point0 };
StagePoint placePoints24_4Point1 = { 0x2EC, 24, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints24_4Point0 = { 0x2EC, 24, 3, 240, 0x1D8, 5, &placePoints24_4Point1 };
StagePoints placePoints24_4 = { 24, 4, &placePoints24_4Point0 };
StagePoint placePoints24_5Point1 = { 0x2EB, 24, 1, 0x240, 160, 1, NULL };
StagePoint placePoints24_5Point0 = { 0x2EC, 24, 4, 240, 0x1D8, 5, &placePoints24_5Point1 };
StagePoints placePoints24_5 = { 24, 5, &placePoints24_5Point0 };
StagePoint placePoints28_1Point1 = { 0x2ED, 28, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints28_1Point0 = { 0x2EA, 28, 1, 224, 0x200, 5, &placePoints28_1Point1 };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints28_2Point1 = { 0x2EE, 28, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints28_2Point0 = { 0x2EA, 28, 2, 224, 0x200, 5, &placePoints28_2Point1 };
StagePoints placePoints28_2 = { 28, 2, &placePoints28_2Point0 };
StagePoint placePoints28_3Point1 = { 0x2EB, 28, 1, 0x240, 160, 1, NULL };
StagePoint placePoints28_3Point0 = { 0x2ED, 28, 1, 224, 192, 5, &placePoints28_3Point1 };
StagePoints placePoints28_3 = { 28, 3, &placePoints28_3Point0 };
StagePoint placePoints28_4Point1 = { 0x2EB, 28, 2, 0x240, 160, 1, NULL };
StagePoint placePoints28_4Point0 = { 0x2EE, 28, 1, 224, 0x240, 5, &placePoints28_4Point1 };
StagePoints placePoints28_4 = { 28, 4, &placePoints28_4Point0 };
StagePoint placePoints29_1Point1 = { 0x2EE, 29, 1, 0x130, 200, 1, NULL };
StagePoint placePoints29_1Point0 = { 0x2E8, 29, 1, 176, 0x168, 5, &placePoints29_1Point1 };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints29_2Point1 = { 0x2EE, 29, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints29_2Point0 = { 0x2EA, 29, 1, 224, 0x200, 5, &placePoints29_2Point1 };
StagePoints placePoints29_2 = { 29, 2, &placePoints29_2Point0 };
StagePoint placePoints30_1Point1 = { 0x2EB, 30, 1, 0x240, 160, 1, NULL };
StagePoint placePoints30_1Point0 = { 0x2ED, 30, 1, 224, 192, 5, &placePoints30_1Point1 };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints2_1, &placePoints3_1, &placePoints3_2,
    &placePoints5_1, &placePoints6_1, &placePoints6_2, &placePoints7_1,
    &placePoints7_2, &placePoints7_3, &placePoints8_1, &placePoints9_1,
    &placePoints10_1, &placePoints10_2, &placePoints11_1, &placePoints11_2,
    &placePoints12_1, &placePoints12_2, &placePoints12_3, &placePoints12_4,
    &placePoints12_5, &placePoints13_1, &placePoints13_2, &placePoints13_3,
    &placePoints13_4, &placePoints13_5, &placePoints13_6, &placePoints18_1,
    &placePoints19_1, &placePoints19_2, &placePoints19_3, &placePoints19_4,
    &placePoints19_5, &placePoints20_1, &placePoints20_2, &placePoints20_3,
    &placePoints20_4, &placePoints20_5, &placePoints20_6, &placePoints20_7,
    &placePoints20_8, &placePoints20_9, &placePoints21_1, &placePoints21_2,
    &placePoints22_1, &placePoints22_2, &placePoints22_3, &placePoints23_1,
    &placePoints23_2, &placePoints23_3, &placePoints24_1, &placePoints24_2,
    &placePoints24_3, &placePoints24_4, &placePoints24_5, &placePoints28_1,
    &placePoints28_2, &placePoints28_3, &placePoints28_4, &placePoints29_1,
    &placePoints29_2, &placePoints30_1, NULL,
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
    { 230, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 237, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 242, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 257, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 260, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 265, 7, 0, { &place7Area0Battles, &place7Area1Battles, &place7Area2Battles, &place7Area3Battles } },
    { 269, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 275, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 280, 10, 0, { &place10Area0Battles, &place10Area1Battles, &place10Area2Battles, &place10Area3Battles } },
    { 285, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 290, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 296, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 315, 18, 0, { &place18Area0Battles, &place18Area1Battles, &place18Area2Battles, &place18Area3Battles } },
    { 318, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 326, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 330, 21, 0, { &place21Area0Battles, &place21Area1Battles, &place21Area2Battles, &place21Area3Battles } },
    { 335, 22, 0, { &place22Area0Battles, &place22Area1Battles, &place22Area2Battles, &place22Area3Battles } },
    { 339, 23, 0, { &place23Area0Battles, &place23Area1Battles, &place23Area2Battles, &place23Area3Battles } },
    { 343, 24, 0, { &place24Area0Battles, &place24Area1Battles, &place24Area2Battles, &place24Area3Battles } },
    { 358, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 364, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 371, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x174, 0x140, 0xD0, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x154, 0x140, 0x50, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x15C, 0x140, 0x70, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x164, 0x140, 0x90, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16C, 0x140, 0xB0, 0x40, 0x170, 0x1FE },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x140, 0x1FD },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x150, 0x1FD },
};
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x23), 0, CODES_END };
u16 actor6Talk0Actions[] = { CARD_BATTLE(0x49, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x23), 1, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor6Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x23), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0x21), 0, CODES_END };
u16 actor7Talk0Actions[] = { CARD_BATTLE(0x4A, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x21), 1, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x21), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CARD(0xC6), 0, CODES_END };
u16 actor8Talk0Actions[] = { CARD_BATTLE(0x4C, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0xC6), 1, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0xC6), 1, CODES_END };
u16 actor9Talk0Conditions[] = { CARD(0x9B), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk0Actions[] = { CARD_BATTLE(0x4D, 0), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x11), 0, CARD(0x9B), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CARD(0x9B), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x354 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x3B0 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3B1 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x3B5 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x367 },
    { actor6Talk1Conditions, NULL, 0x368 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x369 },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x36C },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x367 },
    { actor7Talk1Conditions, NULL, 0x368 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x369 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x36D },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x377 },
    { actor8Talk1Conditions, NULL, 0x378 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x379 },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x37A },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x377 },
    { actor9Talk1Conditions, NULL, 0x378 },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x379 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x37B },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0x17), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(7), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor3Conditions[] = { WARP_ARG(8), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor5Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 8), 0, CODES_END };
u16 actor6Conditions[] = {
    CARD(0x23), 0,
    WARP_ARG(0xB), 1,
    WARP_ARG(0x1E), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor7Conditions[] = {
    WARP_ARG(0xC), 1,
    WARP_ARG(0x1E), 1,
    ITEM(0, 0x14), 1,
    CARD(0x21), 0,
    CODES_END,
};
u16 actor8Conditions[] = {
    WARP_ARG(0x17), 1,
    WARP_ARG(0x20), 1,
    ITEM(0, 0x14), 1,
    CARD(0xC6), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    WARP_ARG(0x16), 1,
    WARP_ARG(0x1F), 1,
    ITEM(0, 0x14), 1,
    CARD(0x9B), 0,
    CODES_END,
};
u16 actor10Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(5), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(6), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0xB), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0xC), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0x12), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0x15), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0x16), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(0x17), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor22Conditions[] = { WARP_ARG(0x1B), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor23Conditions[] = { WARP_ARG(0x1C), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x23, 4, 800, 344, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x23, 4, 616, 272, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x23, 4, 800, 344, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x41, 5, 616, 272, 7 };
FieldActorEntry actor4 = { NULL, NULL, 0x146, 6, 0, 0, 0 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x148, 7, 400, 248, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x151, 8, 800, 344, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x152, 9, 888, 460, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x159, 0xA, 448, 200, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x15A, 0xB, 888, 460, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x15F, 0xC, 464, 424, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x15F, 0xC, 704, 352, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x15F, 0xC, 736, 272, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x160, 0xD, 880, 152, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x160, 0xD, 304, 440, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x160, 0xD, 736, 416, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x160, 0xD, 400, 248, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x160, 0xD, 592, 440, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x160, 0xD, 800, 448, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x160, 0xD, 784, 200, 1 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x160, 0xD, 320, 288, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x160, 0xD, 800, 448, 1 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x160, 0xD, 880, 152, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x160, 0xD, 736, 416, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EC, 0x3B0, 0x78, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EC, 0xF0, 0x1D8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
