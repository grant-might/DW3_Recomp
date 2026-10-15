/*
 * WSTAG974: Circuit Board, Underground: a deep tunnel with guides to both
 * servers, in the extra chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 750 starts it instead of WSTAG895.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x94B
#define STAGE_FILE_8 0x698
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x931
#define STAGE_FILE_8 0x688
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x13B00, 0x21000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1E;
    FIELDSTG_state.music = MUSIC(0x1E, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 6);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 7);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 5);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints1_1Point2 = { 0x2EB, 1, 1, 0x240, 160, 1, NULL };
StagePoint placePoints1_1Point1 = { 0x2ED, 1, 1, 0x350, 0x1F8, 5, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x2E8, 1, 1, 176, 0x168, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point2 = { 0x2EB, 1, 4, 0x240, 160, 1, NULL };
StagePoint placePoints1_2Point1 = { 0x2EC, 1, 4, 240, 0x1D8, 5, &placePoints1_2Point2 };
StagePoint placePoints1_2Point0 = { 0x2EC, 1, 5, 240, 0x1D8, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point2 = { 0x2EC, 1, 6, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_3Point1 = { 0x2ED, 1, 4, 0x350, 0x1F8, 5, &placePoints1_3Point2 };
StagePoint placePoints1_3Point0 = { 0x2EA, 1, 3, 224, 0x200, 5, &placePoints1_3Point1 };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point2 = { 0x2EB, 1, 6, 0x240, 160, 1, NULL };
StagePoint placePoints1_4Point1 = { 0x2ED, 1, 5, 0x350, 0x1F8, 5, &placePoints1_4Point2 };
StagePoint placePoints1_4Point0 = { 0x2EC, 1, 7, 240, 0x1D8, 5, &placePoints1_4Point1 };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints2_1Point2 = { 0x2ED, 2, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints2_1Point1 = { 0x2EA, 2, 1, 224, 0x200, 5, &placePoints2_1Point2 };
StagePoint placePoints2_1Point0 = { 0x2EA, 2, 2, 224, 0x200, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point2 = { 0x2ED, 2, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints2_2Point1 = { 0x2EA, 2, 4, 224, 0x200, 5, &placePoints2_2Point2 };
StagePoint placePoints2_2Point0 = { 0x2ED, 2, 1, 224, 192, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints2_3Point2 = { 0x2EC, 2, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints2_3Point1 = { 0x2ED, 2, 1, 0x350, 0x1F8, 5, &placePoints2_3Point2 };
StagePoint placePoints2_3Point0 = { 0x2ED, 2, 2, 224, 192, 5, &placePoints2_3Point1 };
StagePoints placePoints2_3 = { 2, 3, &placePoints2_3Point0 };
StagePoint placePoints2_4Point2 = { 0x2EC, 2, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints2_4Point1 = { 0x2ED, 2, 2, 0x350, 0x1F8, 5, &placePoints2_4Point2 };
StagePoint placePoints2_4Point0 = { 0x2EA, 2, 5, 224, 0x200, 5, &placePoints2_4Point1 };
StagePoints placePoints2_4 = { 2, 4, &placePoints2_4Point0 };
StagePoint placePoints2_5Point2 = { 0x2EE, 2, 7, 0x130, 200, 1, NULL };
StagePoint placePoints2_5Point1 = { 0x2EA, 2, 6, 224, 0x200, 5, &placePoints2_5Point2 };
StagePoint placePoints2_5Point0 = { 0x2ED, 2, 3, 224, 192, 5, &placePoints2_5Point1 };
StagePoints placePoints2_5 = { 2, 5, &placePoints2_5Point0 };
StagePoint placePoints2_6Point2 = { 0x2EE, 2, 8, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_6Point1 = { 0x2EC, 2, 1, 240, 0x1D8, 5, &placePoints2_6Point2 };
StagePoint placePoints2_6Point0 = { 0x2EC, 2, 2, 240, 0x1D8, 5, &placePoints2_6Point1 };
StagePoints placePoints2_6 = { 2, 6, &placePoints2_6Point0 };
StagePoint placePoints2_7Point2 = { 0x2EE, 2, 9, 0x130, 200, 1, NULL };
StagePoint placePoints2_7Point1 = { 0x2EE, 2, 5, 224, 0x240, 5, &placePoints2_7Point2 };
StagePoint placePoints2_7Point0 = { 0x2ED, 2, 4, 224, 192, 5, &placePoints2_7Point1 };
StagePoints placePoints2_7 = { 2, 7, &placePoints2_7Point0 };
StagePoint placePoints2_8Point2 = { 0x2EE, 2, 9, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_8Point1 = { 0x2ED, 2, 4, 0x350, 0x1F8, 5, &placePoints2_8Point2 };
StagePoint placePoints2_8Point0 = { 0x2EE, 2, 6, 224, 0x240, 5, &placePoints2_8Point1 };
StagePoints placePoints2_8 = { 2, 8, &placePoints2_8Point0 };
StagePoint placePoints2_9Point2 = { 0x2E9, 2, 1, 0x240, 240, 1, NULL };
StagePoint placePoints2_9Point1 = { 0x2EE, 2, 7, 224, 0x240, 5, &placePoints2_9Point2 };
StagePoint placePoints2_9Point0 = { 0x2EE, 2, 8, 224, 0x240, 5, &placePoints2_9Point1 };
StagePoints placePoints2_9 = { 2, 9, &placePoints2_9Point0 };
StagePoint placePoints3_1Point2 = { 0x2EC, 3, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_1Point1 = { 0x2EC, 3, 2, 240, 0x1D8, 5, &placePoints3_1Point2 };
StagePoint placePoints3_1Point0 = { 0x2EC, 3, 3, 240, 0x1D8, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point2 = { 0x2EE, 3, 3, 0x130, 200, 1, NULL };
StagePoint placePoints3_2Point1 = { 0x2EC, 3, 4, 240, 0x1D8, 5, &placePoints3_2Point2 };
StagePoint placePoints3_2Point0 = { 0x2ED, 3, 3, 224, 192, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point2 = { 0x2EC, 3, 8, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_3Point1 = { 0x2EE, 3, 2, 224, 0x240, 5, &placePoints3_3Point2 };
StagePoint placePoints3_3Point0 = { 0x2EC, 3, 6, 240, 0x1D8, 5, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints3_4Point2 = { 0x2EE, 3, 5, 0x130, 200, 1, NULL };
StagePoint placePoints3_4Point1 = { 0x2EC, 3, 7, 240, 0x1D8, 5, &placePoints3_4Point2 };
StagePoint placePoints3_4Point0 = { 0x2EA, 3, 1, 224, 0x200, 5, &placePoints3_4Point1 };
StagePoints placePoints3_4 = { 3, 4, &placePoints3_4Point0 };
StagePoint placePoints3_5Point2 = { 0x2E9, 3, 1, 0x240, 240, 1, NULL };
StagePoint placePoints3_5Point1 = { 0x2EE, 3, 4, 224, 0x240, 5, &placePoints3_5Point2 };
StagePoint placePoints3_5Point0 = { 0x2EC, 3, 8, 240, 0x1D8, 5, &placePoints3_5Point1 };
StagePoints placePoints3_5 = { 3, 5, &placePoints3_5Point0 };
StagePoint placePoints4_1Point2 = { 0x2ED, 4, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints4_1Point1 = { 0x2EC, 4, 1, 240, 0x1D8, 5, &placePoints4_1Point2 };
StagePoint placePoints4_1Point0 = { 0x2EC, 4, 2, 240, 0x1D8, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point2 = { 0x2EE, 4, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints4_2Point1 = { 0x2EC, 4, 4, 240, 0x1D8, 5, &placePoints4_2Point2 };
StagePoint placePoints4_2Point0 = { 0x2EC, 4, 5, 240, 0x1D8, 5, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints4_3Point2 = { 0x2E9, 4, 1, 0x240, 240, 1, NULL };
StagePoint placePoints4_3Point1 = { 0x2EC, 4, 7, 240, 0x1D8, 5, &placePoints4_3Point2 };
StagePoint placePoints4_3Point0 = { 0x2EE, 4, 2, 224, 0x240, 5, &placePoints4_3Point1 };
StagePoints placePoints4_3 = { 4, 3, &placePoints4_3Point0 };
StagePoint placePoints5_1Point2 = { 0x2EE, 5, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_1Point1 = { 0x2ED, 5, 3, 0x350, 0x1F8, 5, &placePoints5_1Point2 };
StagePoint placePoints5_1Point0 = { 0x2EC, 5, 2, 240, 0x1D8, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point2 = { 0x2EC, 6, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_2Point1 = { 0x2ED, 5, 5, 0x350, 0x1F8, 5, &placePoints5_2Point2 };
StagePoint placePoints5_2Point0 = { 0x2EE, 5, 1, 224, 0x240, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point2 = { 0x2EC, 5, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_3Point1 = { 0x2EA, 5, 1, 224, 0x200, 5, &placePoints5_3Point2 };
StagePoint placePoints5_3Point0 = { 0x2ED, 5, 6, 224, 192, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints5_4Point2 = { 0x2EC, 5, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_4Point1 = { 0x2EC, 5, 4, 240, 0x1D8, 5, &placePoints5_4Point2 };
StagePoint placePoints5_4Point0 = { 0x2EA, 5, 2, 224, 0x200, 5, &placePoints5_4Point1 };
StagePoints placePoints5_4 = { 5, 4, &placePoints5_4Point0 };
StagePoint placePoints5_5Point2 = { 0x2EC, 6, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_5Point1 = { 0x2EA, 5, 3, 224, 0x200, 5, &placePoints5_5Point2 };
StagePoint placePoints5_5Point0 = { 0x2EC, 5, 5, 240, 0x1D8, 5, &placePoints5_5Point1 };
StagePoints placePoints5_5 = { 5, 5, &placePoints5_5Point0 };
StagePoint placePoints6_1Point2 = { 0x2EE, 6, 2, 0x130, 200, 1, NULL };
StagePoint placePoints6_1Point1 = { 0x2ED, 6, 1, 0x350, 0x1F8, 5, &placePoints6_1Point2 };
StagePoint placePoints6_1Point0 = { 0x2ED, 6, 2, 224, 192, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point2 = { 0x2ED, 6, 6, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_2Point1 = { 0x2EE, 6, 1, 224, 0x240, 5, &placePoints6_2Point2 };
StagePoint placePoints6_2Point0 = { 0x2EC, 6, 3, 240, 0x1D8, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints6_3Point2 = { 0x2EE, 6, 5, 0x130, 200, 1, NULL };
StagePoint placePoints6_3Point1 = { 0x2ED, 6, 4, 0x350, 0x1F8, 5, &placePoints6_3Point2 };
StagePoint placePoints6_3Point0 = { 0x2ED, 6, 5, 224, 192, 5, &placePoints6_3Point1 };
StagePoints placePoints6_3 = { 6, 3, &placePoints6_3Point0 };
StagePoint placePoints6_4Point2 = { 0x2EE, 6, 5, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_4Point1 = { 0x2ED, 6, 5, 0x350, 0x1F8, 5, &placePoints6_4Point2 };
StagePoint placePoints6_4Point0 = { 0x2ED, 6, 6, 224, 192, 5, &placePoints6_4Point1 };
StagePoints placePoints6_4 = { 6, 4, &placePoints6_4Point0 };
StagePoint placePoints6_5Point2 = { 0x2EE, 6, 7, 0x130, 200, 1, NULL };
StagePoint placePoints6_5Point1 = { 0x2EE, 6, 3, 224, 0x240, 5, &placePoints6_5Point2 };
StagePoint placePoints6_5Point0 = { 0x2EE, 6, 4, 224, 0x240, 5, &placePoints6_5Point1 };
StagePoints placePoints6_5 = { 6, 5, &placePoints6_5Point0 };
StagePoint placePoints6_6Point2 = { 0x2EE, 6, 8, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_6Point1 = { 0x2ED, 6, 7, 0x350, 0x1F8, 5, &placePoints6_6Point2 };
StagePoint placePoints6_6Point0 = { 0x2EC, 6, 4, 240, 0x1D8, 5, &placePoints6_6Point1 };
StagePoints placePoints6_6 = { 6, 6, &placePoints6_6Point0 };
StagePoint placePoints6_7Point2 = { 0x2EE, 6, 9, 0x130, 200, 1, NULL };
StagePoint placePoints6_7Point1 = { 0x2EE, 6, 5, 224, 0x240, 5, &placePoints6_7Point2 };
StagePoint placePoints6_7Point0 = { 0x2ED, 6, 8, 224, 192, 5, &placePoints6_7Point1 };
StagePoints placePoints6_7 = { 6, 7, &placePoints6_7Point0 };
StagePoint placePoints6_8Point2 = { 0x2EE, 6, 9, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_8Point1 = { 0x2ED, 6, 8, 0x350, 0x1F8, 5, &placePoints6_8Point2 };
StagePoint placePoints6_8Point0 = { 0x2EE, 6, 6, 224, 0x240, 5, &placePoints6_8Point1 };
StagePoints placePoints6_8 = { 6, 8, &placePoints6_8Point0 };
StagePoint placePoints6_9Point2 = { 0x2E9, 6, 1, 0x240, 240, 1, NULL };
StagePoint placePoints6_9Point1 = { 0x2EE, 6, 7, 224, 0x240, 5, &placePoints6_9Point2 };
StagePoint placePoints6_9Point0 = { 0x2EE, 6, 8, 224, 0x240, 5, &placePoints6_9Point1 };
StagePoints placePoints6_9 = { 6, 9, &placePoints6_9Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints2_1, &placePoints2_2, &placePoints2_3, &placePoints2_4,
    &placePoints2_5, &placePoints2_6, &placePoints2_7, &placePoints2_8,
    &placePoints2_9, &placePoints3_1, &placePoints3_2, &placePoints3_3,
    &placePoints3_4, &placePoints3_5, &placePoints4_1, &placePoints4_2,
    &placePoints4_3, &placePoints5_1, &placePoints5_2, &placePoints5_3,
    &placePoints5_4, &placePoints5_5, &placePoints6_1, &placePoints6_2,
    &placePoints6_3, &placePoints6_4, &placePoints6_5, &placePoints6_6,
    &placePoints6_7, &placePoints6_8, &placePoints6_9, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x15E, 0x140, 0x78, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x14C, 0x140, 0x30, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x166, 0x140, 0x98, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x140, 0xB8, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x156, 0x140, 0x58, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x170, 0x1FE },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x140, 0x1FD },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x8E), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 2), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 2), 1, ITEM(2, 0x8A), 0, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 2), 1, ITEM(2, 0x8A), 1, CODES_END };
u16 actor0Talk3Actions[] = { ITEM(3, 0x8E), 1, ITEM(3, 0x8D), 0, ITEM(2, 0x8A), 0, CODES_END };
u16 actor1Talk0Conditions[] = { ITEM(3, 0x8F), 1, CODES_END };
u16 actor1Talk1Conditions[] = { ITEM(3, 0x8F), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk2Conditions[] = { ITEM(3, 0x8F), 0, FLAG(0, 0), 1, ITEM(2, 0x8B), 0, CODES_END };
u16 actor1Talk3Conditions[] = { ITEM(3, 0x8F), 0, FLAG(0, 0), 1, ITEM(2, 0x8B), 1, CODES_END };
u16 actor1Talk3Actions[] = { ITEM(3, 0x8F), 1, ITEM(3, 0x8E), 0, ITEM(2, 0x8B), 0, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(3, 0x90), 1, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 1, ITEM(2, 0x8C), 0, CODES_END };
u16 actor2Talk3Conditions[] = { ITEM(3, 0x90), 0, FLAG(0, 1), 1, ITEM(2, 0x8C), 1, CODES_END };
u16 actor2Talk3Actions[] = { ITEM(3, 0x90), 1, ITEM(3, 0x8F), 0, ITEM(2, 0x8C), 0, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(3, 0x67), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 1, ITEM(2, 0x63), 0, CODES_END };
u16 actor3Talk3Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 1, ITEM(2, 0x63), 1, CODES_END };
u16 actor3Talk3Actions[] = { ITEM(3, 0x67), 1, ITEM(3, 0x66), 0, ITEM(2, 0x63), 0, CODES_END };
u16 actor4Talk0Conditions[] = { ITEM(3, 0x81), 1, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(3, 0x81), 0, FLAG(0, 0), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk2Conditions[] = { ITEM(3, 0x81), 0, FLAG(0, 0), 1, ITEM(2, 0x7D), 0, CODES_END };
u16 actor4Talk3Conditions[] = { ITEM(3, 0x81), 0, FLAG(0, 0), 1, ITEM(2, 0x7D), 1, CODES_END };
u16 actor4Talk3Actions[] = { ITEM(3, 0x81), 1, ITEM(3, 0x80), 0, ITEM(2, 0x7D), 0, CODES_END };
u16 actor5Talk0Conditions[] = { ITEM(3, 0x77), 1, CODES_END };
u16 actor5Talk1Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 0), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk2Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 0), 1, ITEM(2, 0x73), 0, CODES_END };
u16 actor5Talk3Conditions[] = { ITEM(3, 0x77), 0, FLAG(0, 0), 1, ITEM(2, 0x73), 1, CODES_END };
u16 actor5Talk3Actions[] = { ITEM(3, 0x77), 1, ITEM(3, 0x76), 0, ITEM(2, 0x73), 0, CODES_END };
u16 actor6Talk0Conditions[] = { ITEM(3, 0x75), 1, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(3, 0x75), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk2Conditions[] = { ITEM(3, 0x75), 0, FLAG(0, 0), 1, ITEM(2, 0x71), 0, CODES_END };
u16 actor6Talk3Conditions[] = { ITEM(3, 0x75), 0, FLAG(0, 0), 1, ITEM(2, 0x71), 1, CODES_END };
u16 actor6Talk3Actions[] = { ITEM(2, 0x71), 0, ITEM(3, 0x75), 1, ITEM(3, 0x74), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0xA5 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0xA6 },
    { actor0Talk2Conditions, NULL, 0xA7 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0xA8 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0xB9 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0xBA },
    { actor1Talk2Conditions, NULL, 0xBB },
    { actor1Talk3Conditions, actor1Talk3Actions, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0xCD },
    { actor2Talk1Conditions, actor2Talk1Actions, 0xCE },
    { actor2Talk2Conditions, NULL, 0xCF },
    { actor2Talk3Conditions, actor2Talk3Actions, 0xD0 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x95 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x96 },
    { actor3Talk2Conditions, NULL, 0x97 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x98 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x99 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x9A },
    { actor4Talk2Conditions, NULL, 0x9B },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x9C },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0xC5 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0xC6 },
    { actor5Talk2Conditions, NULL, 0xC7 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0xC8 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x9D },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x9E },
    { actor6Talk2Conditions, NULL, 0x9F },
    { actor6Talk3Conditions, actor6Talk3Actions, 0xA0 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x23), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x8E), 0,
    ITEM(3, 0x8D), 1,
    CODES_END,
};
u16 actor1Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x21), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x8E), 1,
    ITEM(3, 0x8F), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x22), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x8F), 1,
    ITEM(3, 0x90), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x20), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x66), 1,
    ITEM(3, 0x67), 0,
    CODES_END,
};
u16 actor4Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x80), 1,
    ITEM(3, 0x81), 0,
    CODES_END,
};
u16 actor5Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x1F), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x76), 1,
    ITEM(3, 0x77), 0,
    CODES_END,
};
u16 actor6Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x25), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x74), 1,
    ITEM(3, 0x75), 0,
    CODES_END,
};
u16 actor8Conditions[] = { WARP_ARG(0), 1, FLAG(0, 8), 0, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(1), 1, FLAG(0, 8), 0, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(2), 1, FLAG(0, 8), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(3), 1, FLAG(0, 8), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(4), 1, FLAG(0, 8), 0, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(5), 1, FLAG(0, 8), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x20), 0, FLAG(0, 9), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x21), 1, FLAG(0, 9), 0, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x20), 1, FLAG(0, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x20), 0, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x1C, 4, 624, 304, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x1F, 5, 624, 304, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x1F, 5, 624, 304, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xA6, 6, 624, 304, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xAB, 7, 624, 304, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xAD, 8, 624, 304, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xAD, 8, 624, 304, 1 };
FieldActorEntry actor7 = { NULL, NULL, 0x146, 9, 0, 0, 0 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x148, 0xA, 448, 528, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x148, 0xA, 608, 512, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x148, 0xA, 704, 416, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x148, 0xA, 720, 544, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x148, 0xA, 280, 300, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x148, 0xA, 440, 324, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x15F, 0xB, 440, 324, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x15F, 0xB, 288, 544, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x15F, 0xB, 448, 528, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x15F, 0xB, 864, 448, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x15F, 0xB, 288, 544, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x15F, 0xB, 256, 224, 1 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x160, 0xC, 864, 448, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x160, 0xC, 784, 488, 1 };
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
AreaBattle place1Area0Battle0 = { 38, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 56, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 107, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 155, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 120, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 181, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 142, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 143, 10, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
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
AreaBattle place2Area0Battle0 = { 82, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 82, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 108, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 108, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 129, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 129, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 91, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 173, 10, MUSIC(2, 0) };
BattleList place2Area0Battles = {
    3,
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
AreaBattle place3Area0Battle0 = { 111, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 111, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 112, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 112, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 119, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 119, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 168, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 168, 10, MUSIC(2, 0) };
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
AreaBattle place4Area0Battle0 = { 117, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 117, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 184, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 184, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 187, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 187, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 187, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 187, 10, MUSIC(2, 0) };
BattleList place4Area0Battles = {
    3,
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
AreaBattle place5Area0Battle0 = { 171, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 171, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 171, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 172, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 172, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 87, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 87, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 87, 10, MUSIC(2, 0) };
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
AreaBattle place6Area0Battle0 = { 73, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 73, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 86, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 86, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 85, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 170, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 92, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 174, 10, MUSIC(2, 0) };
BattleList place6Area0Battles = {
    3,
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
FieldBattles stageBattles[] = {
    { 419, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 424, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 430, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 436, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 442, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 448, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
};
