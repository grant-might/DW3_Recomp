/*
 * WSTAG973: Circuit Board, Underground: a deep tunnel with guides to both
 * servers, in the extra chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 749 starts it instead of WSTAG890.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x949
#define STAGE_FILE_8 0x695
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x92F
#define STAGE_FILE_8 0x685
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x18200, 0x7E00};
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

StagePoint placePoints1_1Point2 = { 0x2EB, 1, 2, 0x240, 160, 1, NULL };
StagePoint placePoints1_1Point1 = { 0x2EE, 1, 1, 0x130, 200, 1, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x2EC, 1, 1, 240, 0x1D8, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point2 = { 0x2EB, 1, 3, 0x240, 160, 1, NULL };
StagePoint placePoints1_2Point1 = { 0x2EC, 1, 2, 0x3B0, 120, 1, &placePoints1_2Point2 };
StagePoint placePoints1_2Point0 = { 0x2EC, 1, 3, 240, 0x1D8, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point2 = { 0x2EC, 1, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_3Point1 = { 0x2EC, 1, 4, 0x3B0, 120, 1, &placePoints1_3Point2 };
StagePoint placePoints1_3Point0 = { 0x2EA, 1, 1, 224, 0x200, 5, &placePoints1_3Point1 };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point2 = { 0x2EC, 1, 5, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_4Point1 = { 0x2EE, 1, 3, 0x130, 200, 1, &placePoints1_4Point2 };
StagePoint placePoints1_4Point0 = { 0x2EA, 1, 2, 224, 0x200, 5, &placePoints1_4Point1 };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints1_5Point2 = { 0x2EB, 1, 5, 0x240, 160, 1, NULL };
StagePoint placePoints1_5Point1 = { 0x2EE, 1, 4, 0x130, 200, 1, &placePoints1_5Point2 };
StagePoint placePoints1_5Point0 = { 0x2EC, 1, 6, 240, 0x1D8, 5, &placePoints1_5Point1 };
StagePoints placePoints1_5 = { 1, 5, &placePoints1_5Point0 };
StagePoint placePoints2_1Point2 = { 0x2EE, 2, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_1Point1 = { 0x2EE, 2, 3, 0x130, 200, 1, &placePoints2_1Point2 };
StagePoint placePoints2_1Point0 = { 0x2EE, 2, 1, 224, 0x240, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point2 = { 0x2EE, 2, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_2Point1 = { 0x2EE, 2, 4, 0x130, 200, 1, &placePoints2_2Point2 };
StagePoint placePoints2_2Point0 = { 0x2EA, 2, 3, 224, 0x200, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints2_3Point2 = { 0x2EE, 2, 5, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_3Point1 = { 0x2ED, 2, 4, 0x3A0, 128, 1, &placePoints2_3Point2 };
StagePoint placePoints2_3Point0 = { 0x2EE, 2, 2, 224, 0x240, 5, &placePoints2_3Point1 };
StagePoints placePoints2_3 = { 2, 3, &placePoints2_3Point0 };
StagePoint placePoints2_4Point2 = { 0x2EE, 2, 7, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_4Point1 = { 0x2EE, 2, 8, 0x130, 200, 1, &placePoints2_4Point2 };
StagePoint placePoints2_4Point0 = { 0x2ED, 2, 3, 0x350, 0x1F8, 5, &placePoints2_4Point1 };
StagePoints placePoints2_4 = { 2, 4, &placePoints2_4Point0 };
StagePoint placePoints3_1Point2 = { 0x2EC, 3, 3, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_1Point1 = { 0x2ED, 3, 2, 0x3A0, 128, 1, &placePoints3_1Point2 };
StagePoint placePoints3_1Point0 = { 0x2EC, 3, 1, 240, 0x1D8, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point2 = { 0x2EC, 3, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_2Point1 = { 0x2ED, 3, 3, 0x3A0, 128, 1, &placePoints3_2Point2 };
StagePoint placePoints3_2Point0 = { 0x2ED, 3, 1, 0x350, 0x1F8, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point2 = { 0x2EE, 3, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_3Point1 = { 0x2EC, 3, 6, 0x3B0, 120, 1, &placePoints3_3Point2 };
StagePoint placePoints3_3Point0 = { 0x2ED, 3, 2, 0x350, 0x1F8, 5, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints4_1Point2 = { 0x2EC, 4, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints4_1Point1 = { 0x2EC, 4, 3, 0x3B0, 120, 1, &placePoints4_1Point2 };
StagePoint placePoints4_1Point0 = { 0x2EA, 4, 1, 224, 0x200, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point2 = { 0x2EC, 4, 4, 0x3B0, 120, 1, NULL };
StagePoint placePoints4_2Point1 = { 0x2EC, 4, 5, 0x3B0, 120, 1, &placePoints4_2Point2 };
StagePoint placePoints4_2Point0 = { 0x2EC, 4, 3, 240, 0x1D8, 5, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints4_3Point2 = { 0x2EC, 4, 6, 0x3B0, 120, 1, NULL };
StagePoint placePoints4_3Point1 = { 0x2EC, 4, 7, 0x3B0, 120, 1, &placePoints4_3Point2 };
StagePoint placePoints4_3Point0 = { 0x2EE, 4, 1, 224, 0x240, 5, &placePoints4_3Point1 };
StagePoints placePoints4_3 = { 4, 3, &placePoints4_3Point0 };
StagePoint placePoints5_1Point2 = { 0x2EC, 5, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_1Point1 = { 0x2ED, 5, 2, 0x3A0, 128, 1, &placePoints5_1Point2 };
StagePoint placePoints5_1Point0 = { 0x2E8, 5, 1, 176, 0x168, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point2 = { 0x2EC, 5, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints5_2Point1 = { 0x2ED, 5, 4, 0x3A0, 128, 1, &placePoints5_2Point2 };
StagePoint placePoints5_2Point0 = { 0x2ED, 5, 1, 0x350, 0x1F8, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point2 = { 0x2ED, 5, 5, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_3Point1 = { 0x2EE, 5, 1, 0x130, 200, 1, &placePoints5_3Point2 };
StagePoint placePoints5_3Point0 = { 0x2EC, 5, 1, 240, 0x1D8, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints5_4Point2 = { 0x2EB, 5, 1, 0x240, 160, 1, NULL };
StagePoint placePoints5_4Point1 = { 0x2EC, 5, 3, 0x3B0, 120, 1, &placePoints5_4Point2 };
StagePoint placePoints5_4Point0 = { 0x2ED, 5, 2, 0x350, 0x1F8, 5, &placePoints5_4Point1 };
StagePoints placePoints5_4 = { 5, 4, &placePoints5_4Point0 };
StagePoint placePoints5_5Point2 = { 0x2EB, 5, 2, 0x240, 160, 1, NULL };
StagePoint placePoints5_5Point1 = { 0x2EE, 5, 2, 0x130, 200, 1, &placePoints5_5Point2 };
StagePoint placePoints5_5Point0 = { 0x2ED, 5, 3, 224, 192, 5, &placePoints5_5Point1 };
StagePoints placePoints5_5 = { 5, 5, &placePoints5_5Point0 };
StagePoint placePoints5_6Point2 = { 0x2EE, 5, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_6Point1 = { 0x2EB, 5, 3, 0x240, 160, 1, &placePoints5_6Point2 };
StagePoint placePoints5_6Point0 = { 0x2EC, 5, 3, 240, 0x1D8, 5, &placePoints5_6Point1 };
StagePoints placePoints5_6 = { 5, 6, &placePoints5_6Point0 };
StagePoint placePoints6_1Point2 = { 0x2EC, 6, 2, 0x3B0, 120, 1, NULL };
StagePoint placePoints6_1Point1 = { 0x2EE, 6, 1, 0x130, 200, 1, &placePoints6_1Point2 };
StagePoint placePoints6_1Point0 = { 0x2EC, 6, 1, 240, 0x1D8, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point2 = { 0x2EE, 6, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_2Point1 = { 0x2EC, 6, 3, 0x3B0, 120, 1, &placePoints6_2Point2 };
StagePoint placePoints6_2Point0 = { 0x2EA, 6, 1, 224, 0x200, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints6_3Point2 = { 0x2ED, 6, 4, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_3Point1 = { 0x2ED, 6, 5, 0x3A0, 128, 1, &placePoints6_3Point2 };
StagePoint placePoints6_3Point0 = { 0x2EC, 6, 2, 240, 0x1D8, 5, &placePoints6_3Point1 };
StagePoints placePoints6_3 = { 6, 3, &placePoints6_3Point0 };
StagePoint placePoints6_4Point2 = { 0x2EB, 6, 1, 0x240, 160, 1, NULL };
StagePoint placePoints6_4Point1 = { 0x2EE, 6, 3, 0x130, 200, 1, &placePoints6_4Point2 };
StagePoint placePoints6_4Point0 = { 0x2ED, 6, 3, 224, 192, 5, &placePoints6_4Point1 };
StagePoints placePoints6_4 = { 6, 4, &placePoints6_4Point0 };
StagePoint placePoints6_5Point2 = { 0x2EE, 6, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_5Point1 = { 0x2EE, 6, 4, 0x130, 200, 1, &placePoints6_5Point2 };
StagePoint placePoints6_5Point0 = { 0x2ED, 6, 3, 0x350, 0x1F8, 5, &placePoints6_5Point1 };
StagePoints placePoints6_5 = { 6, 5, &placePoints6_5Point0 };
StagePoint placePoints6_6Point2 = { 0x2EE, 6, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_6Point1 = { 0x2ED, 6, 7, 0x3A0, 128, 1, &placePoints6_6Point2 };
StagePoint placePoints6_6Point0 = { 0x2EE, 6, 2, 224, 0x240, 5, &placePoints6_6Point1 };
StagePoints placePoints6_6 = { 6, 6, &placePoints6_6Point0 };
StagePoint placePoints6_7Point2 = { 0x2ED, 6, 8, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_7Point1 = { 0x2EE, 6, 6, 0x130, 200, 1, &placePoints6_7Point2 };
StagePoint placePoints6_7Point0 = { 0x2ED, 6, 6, 0x350, 0x1F8, 5, &placePoints6_7Point1 };
StagePoints placePoints6_7 = { 6, 7, &placePoints6_7Point0 };
StagePoint placePoints6_8Point2 = { 0x2EE, 6, 7, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_8Point1 = { 0x2EE, 6, 8, 0x130, 200, 1, &placePoints6_8Point2 };
StagePoint placePoints6_8Point0 = { 0x2ED, 6, 7, 224, 192, 5, &placePoints6_8Point1 };
StagePoints placePoints6_8 = { 6, 8, &placePoints6_8Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints1_5, &placePoints2_1, &placePoints2_2, &placePoints2_3,
    &placePoints2_4, &placePoints3_1, &placePoints3_2, &placePoints3_3,
    &placePoints4_1, &placePoints4_2, &placePoints4_3, &placePoints5_1,
    &placePoints5_2, &placePoints5_3, &placePoints5_4, &placePoints5_5,
    &placePoints5_6, &placePoints6_1, &placePoints6_2, &placePoints6_3,
    &placePoints6_4, &placePoints6_5, &placePoints6_6, &placePoints6_7,
    &placePoints6_8, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16E, 0x170, 0xB8, 0x70, 0x140, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x140, 0xB8, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x152, 0x140, 0x48, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x160, 0x140, 0x80, 0x40, 0x170, 0x1FE },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x76), 1, ITEM(2, 0x8B), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x75), 1, ITEM(2, 0x7E), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 0x74), 1, ITEM(2, 0x97), 1, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(2, 0x79), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor18Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor18Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xA, 0xE), 1, CODES_END };
u16 actor19Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor19Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor19Talk1Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xA, 0xF), 1, CODES_END };
u16 actor20Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor20Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor20Talk1Actions[] = { FLAG(0xA, 0x11), 1, EVENT_BATTLE(2), 1, CODES_END };
u16 actor21Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor21Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor21Talk1Actions[] = { EVENT_BATTLE(3), 1, FLAG(0xA, 0x12), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0xE },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0xD },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0xC },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x11 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, actor18Talk0Actions, 0xF1 },
    { actor18Talk1Conditions, actor18Talk1Actions, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, actor19Talk0Actions, 0xF3 },
    { actor19Talk1Conditions, actor19Talk1Actions, 0xF4 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, actor20Talk0Actions, 0xF7 },
    { actor20Talk1Conditions, actor20Talk1Actions, 0xF8 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, actor21Talk0Actions, 0xF9 },
    { actor21Talk1Conditions, actor21Talk1Actions, 0xFA },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x22), 1, FLAG(2, 0x76), 0, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x23), 1, FLAG(2, 0x75), 0, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(5), 1, WARP_ARG(0x20), 1, FLAG(2, 0x74), 0, CODES_END };
u16 actor3Conditions[] = {
    WARP_ARG(5), 1,
    WARP_ARG(0x23), 1,
    FLAG(2, 0x79), 0,
    ITEM(0, 0x29), 0,
    CODES_END,
};
u16 actor5Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, FLAG(0, 8), 0, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1F), 1, FLAG(0, 8), 0, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x20), 1, FLAG(0, 8), 0, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x21), 1, FLAG(0, 8), 0, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x22), 1, FLAG(0, 8), 0, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(1), 1, FLAG(0, 8), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(4), 1, FLAG(0, 8), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(5), 1, FLAG(0, 8), 0, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x21), 1, FLAG(0, 9), 0, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x22), 1, FLAG(0, 9), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, FLAG(0xA, 0xE), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1F), 1, FLAG(0xA, 0xF), 0, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x21), 1, FLAG(0xA, 0x11), 0, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x22), 1, FLAG(0xA, 0x12), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 672, 216, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x21, 4, 672, 216, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x21, 4, 672, 216, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x21, 4, 672, 216, 1 };
FieldActorEntry actor4 = { NULL, NULL, 0x146, 5, 0, 0, 0 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x148, 6, 488, 164, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x148, 6, 488, 164, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x148, 6, 784, 200, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x148, 6, 784, 200, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x148, 6, 488, 164, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x148, 6, 768, 352, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x148, 6, 488, 164, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x148, 6, 456, 340, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x15F, 7, 448, 384, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x15F, 7, 880, 408, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x15F, 7, 880, 152, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x15F, 7, 784, 200, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x15F, 7, 272, 168, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x189, 8, 480, 256, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x18A, 9, 480, 256, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x18C, 0xA, 640, 416, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x18D, 0xB, 640, 416, 1 };
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0x3A0, 0x80, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0x350, 0x1F8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2ED, 0xE0, 0xC0, 1, 0, 0, 0 },
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
AreaBattle place1Area3Battle0 = { 316, 19, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle1 = { 317, 19, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle2 = { 319, 19, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle3 = { 321, 19, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
AreaBattle place2Area0Battle0 = { 81, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 81, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 165, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 165, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 166, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 166, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 169, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 169, 10, MUSIC(2, 0) };
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
AreaBattle place3Area0Battle0 = { 136, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 136, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 137, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 137, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 183, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 183, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 118, 10, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 118, 10, MUSIC(2, 0) };
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
AreaBattle place5Area0Battle0 = { 74, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 77, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 78, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 79, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 80, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 75, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 76, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 89, 10, MUSIC(2, 0) };
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
AreaBattle place6Area0Battle0 = { 110, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 110, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 71, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 71, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 121, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 121, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 182, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 182, 10, MUSIC(2, 0) };
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
    { 418, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 423, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 429, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 435, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 441, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 447, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
};
