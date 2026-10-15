/*
 * WSTAG890: Circuit Board, Underground: a deep tunnel with guides to both
 * servers. A tunnel only the other tunnels reach (WSTAG865 to 895), with
 * guides to both servers' exits.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x686
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x696
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x18200, 0x7E00};
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

StagePoint placePoints1_1Point2 = { 0x2EB, 1, 1, 0x240, 160, 1, NULL };
StagePoint placePoints1_1Point1 = { 0x2EE, 1, 1, 0x130, 200, 1, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x2E8, 1, 2, 176, 0x168, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point2 = { 0x2EE, 1, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints1_2Point1 = { 0x2E9, 1, 1, 0x240, 240, 1, &placePoints1_2Point2 };
StagePoint placePoints1_2Point0 = { 0x2EC, 1, 1, 240, 0x1D8, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints2_1Point2 = { 0x2EE, 2, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_1Point1 = { 0x2E9, 2, 1, 0x240, 240, 1, &placePoints2_1Point2 };
StagePoint placePoints2_1Point0 = { 0x2EA, 2, 1, 224, 0x200, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point2 = { 0x2EE, 2, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_2Point1 = { 0x2E9, 2, 2, 0x240, 240, 1, &placePoints2_2Point2 };
StagePoint placePoints2_2Point0 = { 0x2EE, 2, 1, 224, 0x240, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints3_1Point2 = { 0x2EE, 3, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_1Point1 = { 0x2EE, 3, 2, 0x130, 200, 1, &placePoints3_1Point2 };
StagePoint placePoints3_1Point0 = { 0x2E8, 3, 2, 176, 0x168, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point2 = { 0x2EC, 3, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_2Point1 = { 0x2EE, 3, 3, 0x130, 200, 1, &placePoints3_2Point2 };
StagePoint placePoints3_2Point0 = { 0x2EE, 3, 1, 224, 0x240, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point2 = { 0x2EE, 3, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_3Point1 = { 0x2EE, 3, 4, 0x130, 200, 1, &placePoints3_3Point2 };
StagePoint placePoints3_3Point0 = { 0x2EE, 3, 2, 224, 0x240, 5, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints3_4Point2 = { 0x2EE, 3, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_4Point1 = { 0x2EE, 3, 5, 0x130, 200, 1, &placePoints3_4Point2 };
StagePoint placePoints3_4Point0 = { 0x2E8, 3, 4, 176, 0x168, 5, &placePoints3_4Point1 };
StagePoints placePoints3_4 = { 3, 4, &placePoints3_4Point0 };
StagePoint placePoints4_1Point2 = { 0x2EE, 4, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints4_1Point1 = { 0x2E9, 4, 1, 0x240, 240, 1, &placePoints4_1Point2 };
StagePoint placePoints4_1Point0 = { 0x2EE, 4, 1, 224, 0x240, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point2 = { 0x2EB, 4, 1, 0x240, 160, 1, NULL };
StagePoint placePoints4_2Point1 = { 0x2E9, 4, 2, 0x240, 240, 1, &placePoints4_2Point2 };
StagePoint placePoints4_2Point0 = { 0x2EE, 4, 2, 224, 0x240, 5, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints5_1Point2 = { 0x2EE, 5, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_1Point1 = { 0x2EE, 5, 2, 0x130, 200, 1, &placePoints5_1Point2 };
StagePoint placePoints5_1Point0 = { 0x2EA, 5, 1, 224, 0x200, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point2 = { 0x2E9, 5, 1, 0x240, 240, 1, NULL };
StagePoint placePoints5_2Point1 = { 0x2EE, 5, 3, 0x130, 200, 1, &placePoints5_2Point2 };
StagePoint placePoints5_2Point0 = { 0x2EE, 5, 1, 224, 0x240, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point2 = { 0x2EE, 5, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_3Point1 = { 0x2EE, 5, 4, 0x130, 200, 1, &placePoints5_3Point2 };
StagePoint placePoints5_3Point0 = { 0x2EE, 5, 2, 224, 0x240, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints5_4Point2 = { 0x2EE, 5, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_4Point1 = { 0x2EC, 5, 1, 0x3B0, 120, 1, &placePoints5_4Point2 };
StagePoint placePoints5_4Point0 = { 0x2E8, 5, 3, 176, 0x168, 5, &placePoints5_4Point1 };
StagePoints placePoints5_4 = { 5, 4, &placePoints5_4Point0 };
StagePoint placePoints6_1Point2 = { 0x2EE, 6, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_1Point1 = { 0x2EE, 6, 2, 0x130, 200, 1, &placePoints6_1Point2 };
StagePoint placePoints6_1Point0 = { 0x2E8, 6, 1, 176, 0x168, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point2 = { 0x2EE, 6, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_2Point1 = { 0x2EC, 6, 2, 0x3B0, 120, 1, &placePoints6_2Point2 };
StagePoint placePoints6_2Point0 = { 0x2EA, 6, 2, 224, 0x200, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints8_1Point2 = { 0x2E9, 8, 1, 0x240, 240, 1, NULL };
StagePoint placePoints8_1Point1 = { 0x2EE, 8, 1, 0x130, 200, 1, &placePoints8_1Point2 };
StagePoint placePoints8_1Point0 = { 0x2E8, 8, 1, 176, 0x168, 5, &placePoints8_1Point1 };
StagePoints placePoints8_1 = { 8, 1, &placePoints8_1Point0 };
StagePoint placePoints8_2Point2 = { 0x2EE, 8, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints8_2Point1 = { 0x2E9, 8, 3, 0x240, 240, 1, &placePoints8_2Point2 };
StagePoint placePoints8_2Point0 = { 0x2EC, 8, 1, 240, 0x1D8, 5, &placePoints8_2Point1 };
StagePoints placePoints8_2 = { 8, 2, &placePoints8_2Point0 };
StagePoint placePoints9_1Point2 = { 0x2EE, 9, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints9_1Point1 = { 0x2E9, 9, 1, 0x240, 240, 1, &placePoints9_1Point2 };
StagePoint placePoints9_1Point0 = { 0x2EA, 9, 1, 224, 0x200, 5, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints9_2Point2 = { 0x2EE, 9, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints9_2Point1 = { 0x2E9, 9, 3, 0x240, 240, 1, &placePoints9_2Point2 };
StagePoint placePoints9_2Point0 = { 0x2EE, 9, 1, 224, 0x240, 5, &placePoints9_2Point1 };
StagePoints placePoints9_2 = { 9, 2, &placePoints9_2Point0 };
StagePoint placePoints10_1Point2 = { 0x2EE, 10, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints10_1Point1 = { 0x2EC, 10, 2, 0x3B0, 120, 1, &placePoints10_1Point2 };
StagePoint placePoints10_1Point0 = { 0x2E8, 10, 2, 176, 0x168, 5, &placePoints10_1Point1 };
StagePoints placePoints10_1 = { 10, 1, &placePoints10_1Point0 };
StagePoint placePoints11_1Point2 = { 0x2EE, 11, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints11_1Point1 = { 0x2EC, 11, 2, 0x3B0, 120, 1, &placePoints11_1Point2 };
StagePoint placePoints11_1Point0 = { 0x2E8, 11, 2, 176, 0x168, 5, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints12_1Point2 = { 0x2EC, 12, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints12_1Point1 = { 0x2EE, 12, 1, 0x130, 200, 1, &placePoints12_1Point2 };
StagePoint placePoints12_1Point0 = { 0x2E8, 12, 1, 176, 0x168, 5, &placePoints12_1Point1 };
StagePoints placePoints12_1 = { 12, 1, &placePoints12_1Point0 };
StagePoint placePoints12_2Point2 = { 0x2EE, 12, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints12_2Point1 = { 0x2EE, 12, 2, 0x130, 200, 1, &placePoints12_2Point2 };
StagePoint placePoints12_2Point0 = { 0x2EC, 12, 1, 240, 0x1D8, 5, &placePoints12_2Point1 };
StagePoints placePoints12_2 = { 12, 2, &placePoints12_2Point0 };
StagePoint placePoints13_1Point2 = { 0x2EC, 13, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints13_1Point1 = { 0x2EE, 13, 1, 0x130, 200, 1, &placePoints13_1Point2 };
StagePoint placePoints13_1Point0 = { 0x2EA, 13, 2, 224, 0x200, 5, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints13_2Point2 = { 0x2EE, 13, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints13_2Point1 = { 0x2EE, 13, 2, 0x130, 200, 1, &placePoints13_2Point2 };
StagePoint placePoints13_2Point0 = { 0x2EC, 13, 1, 240, 0x1D8, 5, &placePoints13_2Point1 };
StagePoints placePoints13_2 = { 13, 2, &placePoints13_2Point0 };
StagePoint placePoints14_1Point2 = { 0x2EB, 14, 1, 0x240, 160, 1, NULL };
StagePoint placePoints14_1Point1 = { 0x2ED, 14, 2, 0x3A0, 128, 1, &placePoints14_1Point2 };
StagePoint placePoints14_1Point0 = { 0x2EA, 14, 1, 224, 0x200, 5, &placePoints14_1Point1 };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints14_2Point2 = { 0x2E9, 14, 1, 0x240, 240, 1, NULL };
StagePoint placePoints14_2Point1 = { 0x2EB, 14, 2, 0x240, 160, 1, &placePoints14_2Point2 };
StagePoint placePoints14_2Point0 = { 0x2ED, 14, 1, 0x350, 0x1F8, 5, &placePoints14_2Point1 };
StagePoints placePoints14_2 = { 14, 2, &placePoints14_2Point0 };
StagePoint placePoints16_1Point2 = { 0x2EB, 16, 1, 0x240, 160, 1, NULL };
StagePoint placePoints16_1Point1 = { 0x2EB, 16, 2, 0x240, 160, 1, &placePoints16_1Point2 };
StagePoint placePoints16_1Point0 = { 0x2EE, 16, 1, 224, 0x240, 5, &placePoints16_1Point1 };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints19_1Point2 = { 0x2EC, 19, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints19_1Point1 = { 0x2ED, 19, 2, 0x3A0, 128, 1, &placePoints19_1Point2 };
StagePoint placePoints19_1Point0 = { 0x2EE, 19, 2, 224, 0x240, 5, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoint placePoints19_2Point2 = { 0x2EB, 19, 1, 0x240, 160, 1, NULL };
StagePoint placePoints19_2Point1 = { 0x2EB, 19, 2, 0x240, 160, 1, &placePoints19_2Point2 };
StagePoint placePoints19_2Point0 = { 0x2ED, 19, 1, 0x350, 0x1F8, 5, &placePoints19_2Point1 };
StagePoints placePoints19_2 = { 19, 2, &placePoints19_2Point0 };
StagePoint placePoints19_3Point2 = { 0x2EB, 19, 3, 0x240, 160, 1, NULL };
StagePoint placePoints19_3Point1 = { 0x2EC, 19, 5, 0x3B0, 120, 1, &placePoints19_3Point2 };
StagePoint placePoints19_3Point0 = { 0x2EC, 19, 4, 240, 0x1D8, 5, &placePoints19_3Point1 };
StagePoints placePoints19_3 = { 19, 3, &placePoints19_3Point0 };
StagePoint placePoints20_1Point2 = { 0x2EC, 20, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints20_1Point1 = { 0x2ED, 20, 2, 0x3A0, 128, 1, &placePoints20_1Point2 };
StagePoint placePoints20_1Point0 = { 0x2EE, 20, 2, 224, 0x240, 5, &placePoints20_1Point1 };
StagePoints placePoints20_1 = { 20, 1, &placePoints20_1Point0 };
StagePoint placePoints20_2Point2 = { 0x2EB, 20, 1, 0x240, 160, 1, NULL };
StagePoint placePoints20_2Point1 = { 0x2EC, 20, 4, 0x3B0, 120, 1, &placePoints20_2Point2 };
StagePoint placePoints20_2Point0 = { 0x2ED, 20, 1, 0x350, 0x1F8, 5, &placePoints20_2Point1 };
StagePoints placePoints20_2 = { 20, 2, &placePoints20_2Point0 };
StagePoint placePoints20_3Point2 = { 0x2EB, 20, 2, 0x240, 160, 1, NULL };
StagePoint placePoints20_3Point1 = { 0x2EC, 20, 5, 0x3B0, 120, 1, &placePoints20_3Point2 };
StagePoint placePoints20_3Point0 = { 0x2EC, 20, 3, 240, 0x1D8, 5, &placePoints20_3Point1 };
StagePoints placePoints20_3 = { 20, 3, &placePoints20_3Point0 };
StagePoint placePoints20_4Point2 = { 0x2EB, 20, 3, 0x240, 160, 1, NULL };
StagePoint placePoints20_4Point1 = { 0x2EC, 20, 6, 0x3B0, 120, 1, &placePoints20_4Point2 };
StagePoint placePoints20_4Point0 = { 0x2EC, 20, 4, 240, 0x1D8, 5, &placePoints20_4Point1 };
StagePoints placePoints20_4 = { 20, 4, &placePoints20_4Point0 };
StagePoint placePoints20_5Point2 = { 0x2EC, 20, 7, 0x3B0, 120, 1, NULL };
StagePoint placePoints20_5Point1 = { 0x2EC, 20, 8, 0x3B0, 120, 1, &placePoints20_5Point2 };
StagePoint placePoints20_5Point0 = { 0x2EC, 20, 5, 240, 0x1D8, 5, &placePoints20_5Point1 };
StagePoints placePoints20_5 = { 20, 5, &placePoints20_5Point0 };
StagePoint placePoints20_6Point2 = { 0x2EC, 20, 9, 0x3B0, 120, 1, NULL };
StagePoint placePoints20_6Point1 = { 0x2EB, 20, 7, 0x240, 160, 1, &placePoints20_6Point2 };
StagePoint placePoints20_6Point0 = { 0x2EC, 20, 7, 240, 0x1D8, 5, &placePoints20_6Point1 };
StagePoints placePoints20_6 = { 20, 6, &placePoints20_6Point0 };
StagePoint placePoints25_1Point2 = { 0x2EB, 25, 1, 0x240, 160, 1, NULL };
StagePoint placePoints25_1Point1 = { 0x2EB, 25, 2, 0x240, 160, 1, &placePoints25_1Point2 };
StagePoint placePoints25_1Point0 = { 0x2EE, 25, 1, 224, 0x240, 5, &placePoints25_1Point1 };
StagePoints placePoints25_1 = { 25, 1, &placePoints25_1Point0 };
StagePoint placePoints27_1Point2 = { 0x2EB, 27, 1, 0x240, 160, 1, NULL };
StagePoint placePoints27_1Point1 = { 0x2ED, 27, 2, 0x3A0, 128, 1, &placePoints27_1Point2 };
StagePoint placePoints27_1Point0 = { 0x2EA, 27, 1, 224, 0x200, 5, &placePoints27_1Point1 };
StagePoints placePoints27_1 = { 27, 1, &placePoints27_1Point0 };
StagePoint placePoints27_2Point2 = { 0x2E9, 27, 1, 0x240, 240, 1, NULL };
StagePoint placePoints27_2Point1 = { 0x2EB, 27, 2, 0x240, 160, 1, &placePoints27_2Point2 };
StagePoint placePoints27_2Point0 = { 0x2ED, 27, 1, 0x350, 0x1F8, 5, &placePoints27_2Point1 };
StagePoints placePoints27_2 = { 27, 2, &placePoints27_2Point0 };
StagePoint placePoints28_1Point2 = { 0x2EC, 28, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints28_1Point1 = { 0x2EE, 28, 1, 0x130, 200, 1, &placePoints28_1Point2 };
StagePoint placePoints28_1Point0 = { 0x2E8, 28, 1, 176, 0x168, 5, &placePoints28_1Point1 };
StagePoints placePoints28_1 = { 28, 1, &placePoints28_1Point0 };
StagePoint placePoints28_2Point2 = { 0x2EE, 28, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints28_2Point1 = { 0x2EE, 28, 2, 0x130, 200, 1, &placePoints28_2Point2 };
StagePoint placePoints28_2Point0 = { 0x2EC, 28, 1, 240, 0x1D8, 5, &placePoints28_2Point1 };
StagePoints placePoints28_2 = { 28, 2, &placePoints28_2Point0 };
StagePoint placePoints29_1Point2 = { 0x2EB, 29, 1, 0x240, 160, 1, NULL };
StagePoint placePoints29_1Point1 = { 0x2ED, 29, 2, 0x3A0, 128, 1, &placePoints29_1Point2 };
StagePoint placePoints29_1Point0 = { 0x2EE, 29, 1, 224, 0x240, 5, &placePoints29_1Point1 };
StagePoints placePoints29_1 = { 29, 1, &placePoints29_1Point0 };
StagePoint placePoints29_2Point2 = { 0x2EB, 29, 2, 0x240, 160, 1, NULL };
StagePoint placePoints29_2Point1 = { 0x2EB, 29, 3, 0x240, 160, 1, &placePoints29_2Point2 };
StagePoint placePoints29_2Point0 = { 0x2ED, 29, 1, 0x350, 0x1F8, 5, &placePoints29_2Point1 };
StagePoints placePoints29_2 = { 29, 2, &placePoints29_2Point0 };
StagePoint placePoints30_1Point2 = { 0x2EC, 30, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints30_1Point1 = { 0x2EE, 30, 1, 0x130, 200, 1, &placePoints30_1Point2 };
StagePoint placePoints30_1Point0 = { 0x2E8, 30, 1, 176, 0x168, 5, &placePoints30_1Point1 };
StagePoints placePoints30_1 = { 30, 1, &placePoints30_1Point0 };
StagePoint placePoints30_2Point2 = { 0x2EE, 30, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints30_2Point1 = { 0x2EE, 30, 2, 0x130, 200, 1, &placePoints30_2Point2 };
StagePoint placePoints30_2Point0 = { 0x2E8, 30, 2, 176, 0x168, 5, &placePoints30_2Point1 };
StagePoints placePoints30_2 = { 30, 2, &placePoints30_2Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints2_1, &placePoints2_2,
    &placePoints3_1, &placePoints3_2, &placePoints3_3, &placePoints3_4,
    &placePoints4_1, &placePoints4_2, &placePoints5_1, &placePoints5_2,
    &placePoints5_3, &placePoints5_4, &placePoints6_1, &placePoints6_2,
    &placePoints8_1, &placePoints8_2, &placePoints9_1, &placePoints9_2,
    &placePoints10_1, &placePoints11_1, &placePoints12_1, &placePoints12_2,
    &placePoints13_1, &placePoints13_2, &placePoints14_1, &placePoints14_2,
    &placePoints16_1, &placePoints19_1, &placePoints19_2, &placePoints19_3,
    &placePoints20_1, &placePoints20_2, &placePoints20_3, &placePoints20_4,
    &placePoints20_5, &placePoints20_6, &placePoints25_1, &placePoints27_1,
    &placePoints27_2, &placePoints28_1, &placePoints28_2, &placePoints29_1,
    &placePoints29_2, &placePoints30_1, &placePoints30_2, NULL,
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
FieldBattles stageBattles[] = {
    { 231, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 238, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 243, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 249, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 254, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 261, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 270, 8, 0, { &place8Area0Battles, &place8Area1Battles, &place8Area2Battles, &place8Area3Battles } },
    { 276, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 281, 10, 0, { &place10Area0Battles, &place10Area1Battles, &place10Area2Battles, &place10Area3Battles } },
    { 286, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 291, 12, 0, { &place12Area0Battles, &place12Area1Battles, &place12Area2Battles, &place12Area3Battles } },
    { 298, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 302, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
    { 310, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 321, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
    { 327, 20, 0, { &place20Area0Battles, &place20Area1Battles, &place20Area2Battles, &place20Area3Battles } },
    { 348, 25, 0, { &place25Area0Battles, &place25Area1Battles, &place25Area2Battles, &place25Area3Battles } },
    { 353, 27, 0, { &place27Area0Battles, &place27Area1Battles, &place27Area2Battles, &place27Area3Battles } },
    { 359, 28, 0, { &place28Area0Battles, &place28Area1Battles, &place28Area2Battles, &place28Area3Battles } },
    { 366, 29, 0, { &place29Area0Battles, &place29Area1Battles, &place29Area2Battles, &place29Area3Battles } },
    { 369, 30, 0, { &place30Area0Battles, &place30Area1Battles, &place30Area2Battles, &place30Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x170, 0x140, 0xC0, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x168, 0x140, 0xA0, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x148, 0x140, 0x20, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x150, 0x140, 0x40, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x158, 0x140, 0x60, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x160, 0x140, 0x80, 0x40, 0x170, 0x1FE },
    { 0x140, 0x100, 0x174, 0x120, 0xD0, 0x20, 0x140, 0x1FD },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FD },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x170, 0x1FD },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x3B2 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x3B3 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3BA },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x359 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x3B6 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x3B7 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x3B9 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x355 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x3B8 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x3BB },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x35C },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x35A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x3B4 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x3BC },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x357 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x356 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x35D },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x358 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x35B },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(0x21), 1, WARP_ARG(0x13), 1, CODES_END };
u16 actor3Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor4Conditions[] = { WARP_ARG(0xB), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor5Conditions[] = { WARP_ARG(0xC), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(0x12), 1, WARP_ARG(0x20), 1, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0xD), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0x13), 1, WARP_ARG(0x22), 1, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(1), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x21), 1, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(7), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(0x1B), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x20), 1, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(8), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0x13), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x21), 1, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(5), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor22Conditions[] = { WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor23Conditions[] = { WARP_ARG(0xB), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor24Conditions[] = { WARP_ARG(0x12), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor25Conditions[] = { WARP_ARG(0x13), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor26Conditions[] = { WARP_ARG(0x1A), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor27Conditions[] = { WARP_ARG(0x1B), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor28Conditions[] = { WARP_ARG(0), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor29Conditions[] = { WARP_ARG(2), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor30Conditions[] = { WARP_ARG(3), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor31Conditions[] = { WARP_ARG(4), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor32Conditions[] = { WARP_ARG(7), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x23, 4, 984, 448, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x23, 4, 984, 448, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x23, 4, 672, 216, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x23, 4, 672, 216, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x40, 5, 672, 216, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x40, 5, 384, 100, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x40, 5, 672, 216, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x40, 5, 672, 216, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x41, 6, 672, 216, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x41, 6, 672, 216, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x41, 6, 672, 216, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xB4, 7, 672, 216, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xB5, 8, 984, 448, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xB5, 8, 384, 100, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xB5, 8, 672, 216, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xE7, 9, 672, 216, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0xEB, 0xA, 672, 216, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0xEF, 0xB, 672, 216, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0xF1, 0xC, 672, 216, 7 };
FieldActorEntry actor19 = { NULL, NULL, 0x146, 0xD, 0, 0, 0 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x15F, 0xE, 768, 352, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x15F, 0xE, 456, 340, 1 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x15F, 0xE, 448, 384, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x160, 0xF, 272, 168, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0x160, 0xF, 880, 152, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x160, 0xF, 480, 256, 1 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x160, 0xF, 640, 416, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x160, 0xF, 784, 200, 1 };
FieldActorEntry actor28 = { actor28Conditions, NULL, 0x160, 0xF, 784, 200, 1 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x160, 0xF, 488, 164, 1 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x160, 0xF, 784, 200, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x160, 0xF, 880, 408, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x160, 0xF, 480, 256, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0x3A0, 0x80, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0x350, 0x1F8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0xE0, 0xC0, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
