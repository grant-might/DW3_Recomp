/*
 * WSTAG972: Circuit Board, Underground: a deep tunnel with Black Kingz and
 * Numemon, in the extra chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 748 starts it instead of WSTAG885.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x947
#define STAGE_FILE_8 0x692
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x92D
#define STAGE_FILE_8 0x682
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x37200, 0x1CE00};
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

StagePoint placePoints1_1Point1 = { 0x2ED, 1, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints1_1Point0 = { 0x2EC, 1, 2, 240, 0x1D8, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point1 = { 0x2EC, 1, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_2Point0 = { 0x2ED, 1, 2, 0x350, 0x1F8, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point1 = { 0x2ED, 1, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints1_3Point0 = { 0x2ED, 1, 3, 224, 192, 5, &placePoints1_3Point1 };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point1 = { 0x2EE, 1, 2, 0x130, 200, 1, NULL };
StagePoint placePoints1_4Point0 = { 0x2ED, 1, 3, 0x350, 0x1F8, 5, &placePoints1_4Point1 };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints1_5Point1 = { 0x2EE, 1, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints1_5Point0 = { 0x2ED, 1, 4, 224, 192, 5, &placePoints1_5Point1 };
StagePoints placePoints1_5 = { 1, 5, &placePoints1_5Point0 };
StagePoint placePoints1_6Point1 = { 0x2ED, 1, 5, 0x3A0, 128, 1, NULL };
StagePoint placePoints1_6Point0 = { 0x2EE, 1, 3, 224, 0x240, 5, &placePoints1_6Point1 };
StagePoints placePoints1_6 = { 1, 6, &placePoints1_6Point0 };
StagePoint placePoints1_7Point1 = { 0x2EE, 1, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints1_7Point0 = { 0x2EC, 1, 8, 240, 0x1D8, 5, &placePoints1_7Point1 };
StagePoints placePoints1_7 = { 1, 7, &placePoints1_7Point0 };
StagePoint placePoints1_8Point1 = { 0x2EC, 1, 7, 0x3B0, 120, 1, NULL };
StagePoint placePoints1_8Point0 = { 0x2E8, 1, 2, 176, 0x168, 5, &placePoints1_8Point1 };
StagePoints placePoints1_8 = { 1, 8, &placePoints1_8Point0 };
StagePoint placePoints2_1Point1 = { 0x2EE, 2, 6, 0x130, 200, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x2EE, 2, 3, 224, 0x240, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point1 = { 0x2EE, 2, 6, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints2_2Point0 = { 0x2EE, 2, 4, 224, 0x240, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints3_1Point1 = { 0x2ED, 3, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x2EC, 4, 6, 240, 0x1D8, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints3_2Point1 = { 0x2EE, 3, 1, 0x130, 200, 1, NULL };
StagePoint placePoints3_2Point0 = { 0x2E8, 3, 1, 176, 0x168, 5, &placePoints3_2Point1 };
StagePoints placePoints3_2 = { 3, 2, &placePoints3_2Point0 };
StagePoint placePoints3_3Point1 = { 0x2EE, 3, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_3Point0 = { 0x2ED, 3, 1, 224, 192, 5, &placePoints3_3Point1 };
StagePoints placePoints3_3 = { 3, 3, &placePoints3_3Point0 };
StagePoint placePoints3_4Point1 = { 0x2EE, 3, 2, 0x130, 200, 1, NULL };
StagePoint placePoints3_4Point0 = { 0x2ED, 3, 2, 224, 192, 5, &placePoints3_4Point1 };
StagePoints placePoints3_4 = { 3, 4, &placePoints3_4Point0 };
StagePoint placePoints3_5Point1 = { 0x2EC, 3, 7, 0x3B0, 120, 1, NULL };
StagePoint placePoints3_5Point0 = { 0x2EE, 3, 1, 224, 0x240, 5, &placePoints3_5Point1 };
StagePoints placePoints3_5 = { 3, 5, &placePoints3_5Point0 };
StagePoint placePoints3_6Point1 = { 0x2EE, 3, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_6Point0 = { 0x2ED, 3, 3, 0x350, 0x1F8, 5, &placePoints3_6Point1 };
StagePoints placePoints3_6 = { 3, 6, &placePoints3_6Point0 };
StagePoint placePoints3_7Point1 = { 0x2EE, 3, 4, 0x130, 200, 1, NULL };
StagePoint placePoints3_7Point0 = { 0x2EC, 3, 5, 240, 0x1D8, 5, &placePoints3_7Point1 };
StagePoints placePoints3_7 = { 3, 7, &placePoints3_7Point0 };
StagePoint placePoints3_8Point1 = { 0x2EE, 3, 5, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints3_8Point0 = { 0x2EE, 3, 3, 224, 0x240, 5, &placePoints3_8Point1 };
StagePoints placePoints3_8 = { 3, 8, &placePoints3_8Point0 };
StagePoint placePoints4_1Point1 = { 0x2EE, 4, 1, 0x130, 200, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x2E8, 4, 1, 176, 0x168, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints4_2Point1 = { 0x2EE, 4, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints4_2Point0 = { 0x2ED, 4, 1, 224, 192, 5, &placePoints4_2Point1 };
StagePoints placePoints4_2 = { 4, 2, &placePoints4_2Point0 };
StagePoint placePoints4_3Point1 = { 0x2ED, 4, 2, 0x3A0, 128, 1, NULL };
StagePoint placePoints4_3Point0 = { 0x2ED, 4, 1, 0x350, 0x1F8, 5, &placePoints4_3Point1 };
StagePoints placePoints4_3 = { 4, 3, &placePoints4_3Point0 };
StagePoint placePoints4_4Point1 = { 0x2EE, 4, 2, 0x130, 200, 1, NULL };
StagePoint placePoints4_4Point0 = { 0x2ED, 4, 2, 224, 192, 5, &placePoints4_4Point1 };
StagePoints placePoints4_4 = { 4, 4, &placePoints4_4Point0 };
StagePoint placePoints4_5Point1 = { 0x2EE, 4, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints4_5Point0 = { 0x2ED, 4, 2, 0x350, 0x1F8, 5, &placePoints4_5Point1 };
StagePoints placePoints4_5 = { 4, 5, &placePoints4_5Point0 };
StagePoint placePoints4_6Point1 = { 0x2EC, 3, 1, 0x3B0, 120, 1, NULL };
StagePoint placePoints4_6Point0 = { 0x2ED, 4, 3, 224, 192, 5, &placePoints4_6Point1 };
StagePoints placePoints4_6 = { 4, 6, &placePoints4_6Point0 };
StagePoint placePoints4_7Point1 = { 0x2EE, 4, 3, 0x130, 200, 1, NULL };
StagePoint placePoints4_7Point0 = { 0x2ED, 4, 3, 0x350, 0x1F8, 5, &placePoints4_7Point1 };
StagePoints placePoints4_7 = { 4, 7, &placePoints4_7Point0 };
StagePoint placePoints5_1Point1 = { 0x2ED, 5, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2ED, 5, 1, 224, 192, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point1 = { 0x2EE, 5, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_2Point0 = { 0x2ED, 5, 2, 224, 192, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point1 = { 0x2ED, 5, 6, 0x3A0, 128, 1, NULL };
StagePoint placePoints5_3Point0 = { 0x2ED, 5, 4, 0x350, 0x1F8, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints5_4Point1 = { 0x2EE, 5, 4, 0x130, 200, 1, NULL };
StagePoint placePoints5_4Point0 = { 0x2EE, 5, 3, 224, 0x240, 5, &placePoints5_4Point1 };
StagePoints placePoints5_4 = { 5, 4, &placePoints5_4Point0 };
StagePoint placePoints5_5Point1 = { 0x2EE, 5, 5, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints5_5Point0 = { 0x2EE, 5, 4, 224, 0x240, 5, &placePoints5_5Point1 };
StagePoints placePoints5_5 = { 5, 5, &placePoints5_5Point0 };
StagePoint placePoints6_1Point1 = { 0x2ED, 6, 1, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_1Point0 = { 0x2EE, 5, 2, 224, 0x240, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints6_2Point1 = { 0x2ED, 6, 3, 0x3A0, 128, 1, NULL };
StagePoint placePoints6_2Point0 = { 0x2ED, 6, 1, 224, 192, 5, &placePoints6_2Point1 };
StagePoints placePoints6_2 = { 6, 2, &placePoints6_2Point0 };
StagePoint placePoints6_3Point1 = { 0x2EE, 6, 2, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_3Point0 = { 0x2ED, 6, 2, 0x350, 0x1F8, 5, &placePoints6_3Point1 };
StagePoints placePoints6_3 = { 6, 3, &placePoints6_3Point0 };
StagePoint placePoints6_4Point1 = { 0x2EE, 6, 6, 0x3A0, 0x1A0, 1, NULL };
StagePoint placePoints6_4Point0 = { 0x2EE, 5, 5, 224, 0x240, 5, &placePoints6_4Point1 };
StagePoints placePoints6_4 = { 6, 4, &placePoints6_4Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints1_5, &placePoints1_6, &placePoints1_7, &placePoints1_8,
    &placePoints2_1, &placePoints2_2, &placePoints3_1, &placePoints3_2,
    &placePoints3_3, &placePoints3_4, &placePoints3_5, &placePoints3_6,
    &placePoints3_7, &placePoints3_8, &placePoints4_1, &placePoints4_2,
    &placePoints4_3, &placePoints4_4, &placePoints4_5, &placePoints4_6,
    &placePoints4_7, &placePoints5_1, &placePoints5_2, &placePoints5_3,
    &placePoints5_4, &placePoints5_5, &placePoints6_1, &placePoints6_2,
    &placePoints6_3, &placePoints6_4, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x178, 0x120, 0xE0, 0x20, 0x150, 0x1FF },
    { 0x140, 0x100, 0x172, 0x140, 0xC8, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x140, 0x68, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x14E, 0x140, 0x38, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x162, 0x140, 0x88, 0x40, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x140, 0xA8, 0x40, 0x170, 0x1FE },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x140, 0x1FD },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x150, 0x1FD },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x160, 0x1FD },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x78), 1, ITEM(2, 0x72), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x7B), 1, ITEM(3, 0x66), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 0x7C), 1, ITEM(3, 0x8D), 1, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x17, 1), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor3Talk4Conditions[] = {
    CARD(0x13), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Talk4Actions[] = {
    CARD(0x13), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor3Talk5Conditions[] = {
    CARD(0x13), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 1), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x1A, 1), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor4Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor4Talk4Conditions[] = {
    CARD(1), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor4Talk4Actions[] = {
    CARD(1), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 0,
    CODES_END,
};
u16 actor4Talk5Conditions[] = {
    CARD(1), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor4Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor5Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Talk2Actions[] = { CARD_BATTLE(6, 1), 1, CODES_END };
u16 actor5Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor5Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor5Talk4Conditions[] = {
    CARD(0xD), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor5Talk4Actions[] = {
    CARD(0xD), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor5Talk5Conditions[] = {
    CARD(0xD), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor5Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(0x21, 1), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor6Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk4Conditions[] = {
    CARD(0x19), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor6Talk4Actions[] = {
    CARD(0x19), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor6Talk5Conditions[] = {
    CARD(0x19), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor6Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor7Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(0x11, 1), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor7Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor7Talk4Conditions[] = {
    CARD(7), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor7Talk4Actions[] = {
    CARD(7), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor7Talk5Conditions[] = {
    CARD(7), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor7Talk5Actions[] = { FLAG(0, 0), 0, FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor12Talk2Actions[] = { CARD_BATTLE(0x4A, 0), 1, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor12Talk3Actions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk4Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CARD(0x21), 0,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor12Talk4Actions[] = {
    FLAG(0, 0), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    CARD(0x21), 1,
    CODES_END,
};
u16 actor12Talk5Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CARD(0x21), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor12Talk5Actions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor13Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor13Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor13Talk2Actions[] = { CARD_BATTLE(0x48, 0), 1, CODES_END };
u16 actor13Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor13Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor13Talk4Conditions[] = {
    CARD(0x22), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor13Talk4Actions[] = {
    CARD(0x22), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor13Talk5Conditions[] = {
    CARD(0x22), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor13Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor14Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor14Talk2Actions[] = { CARD_BATTLE(0x49, 0), 1, CODES_END };
u16 actor14Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor14Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor14Talk4Conditions[] = {
    CARD(0x23), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor14Talk4Actions[] = {
    CARD(0x23), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor14Talk5Conditions[] = {
    CARD(0x23), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor14Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor15Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor15Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor15Talk2Actions[] = { CARD_BATTLE(0x4B, 0), 1, CODES_END };
u16 actor15Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor15Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor15Talk4Conditions[] = {
    CARD(0x1F), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor15Talk4Actions[] = {
    CARD(0x1F), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor15Talk5Conditions[] = {
    CARD(0x1F), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor15Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor16Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(0x47, 0), 1, CODES_END };
u16 actor16Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor16Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor16Talk4Conditions[] = {
    CARD(0x20), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor16Talk4Actions[] = {
    CARD(0x20), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor16Talk5Conditions[] = {
    CARD(0x20), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor16Talk5Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor17Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 1), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor17Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor17Talk2Actions[] = { CARD_BATTLE(0x4F, 0), 1, CODES_END };
u16 actor17Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor17Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor17Talk4Conditions[] = {
    CARD(0xF1), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor17Talk4Actions[] = {
    CARD(0xF1), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 0,
    CODES_END,
};
u16 actor17Talk5Conditions[] = {
    CARD(0xF1), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 1), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor17Talk5Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor18Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor18Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor18Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor18Talk2Actions[] = { CARD_BATTLE(0x4E, 0), 1, CODES_END };
u16 actor18Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor18Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor18Talk4Conditions[] = {
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CARD(0x45), 0,
    CODES_END,
};
u16 actor18Talk4Actions[] = {
    CARD(0x45), 1,
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor18Talk5Conditions[] = {
    CARD(0x45), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor18Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor19Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor19Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor19Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor19Talk2Actions[] = { CARD_BATTLE(0x4D, 0), 1, CODES_END };
u16 actor19Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor19Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor19Talk4Conditions[] = {
    CARD(0x9B), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor19Talk4Actions[] = {
    CARD(0x9B), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor19Talk5Conditions[] = {
    CARD(0x9B), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor19Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor20Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor20Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor20Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor20Talk2Actions[] = { CARD_BATTLE(0x4C, 0), 1, CODES_END };
u16 actor20Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor20Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor20Talk4Conditions[] = {
    CARD(0xC6), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor20Talk4Actions[] = {
    CARD(0xC6), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor20Talk5Conditions[] = {
    CARD(0xC6), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor20Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor21Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor21Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor21Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor21Talk2Actions[] = { CARD_BATTLE(0x50, 0), 1, CODES_END };
u16 actor21Talk3Conditions[] = {
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor21Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor21Talk4Conditions[] = {
    CARD(0x70), 0,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor21Talk4Actions[] = {
    CARD(0x70), 1,
    FLAG(0, 0x10), 0,
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor21Talk5Conditions[] = {
    CARD(0x70), 1,
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor21Talk5Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor26Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor26Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor26Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor26Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xA, 0x10), 1, CODES_END };
u16 actor27Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor27Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor27Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor27Talk1Actions[] = { FLAG(0xA, 0x13), 1, EVENT_BATTLE(1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x10 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x13 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x14 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x10A },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x10A },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x10E },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x111 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x122 },
    { actor3Talk5Conditions, actor3Talk5Actions, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x107 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x107 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x10E },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x111 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x11F },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x109 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x109 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x10E },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x111 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0x121 },
    { actor5Talk5Conditions, actor5Talk5Actions, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x10B },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x10B },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x10E },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x111 },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x123 },
    { actor6Talk5Conditions, actor6Talk5Actions, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x108 },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x108 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x10E },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x111 },
    { actor7Talk4Conditions, actor7Talk4Actions, 0x120 },
    { actor7Talk5Conditions, actor7Talk5Actions, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0xFF },
    { actor12Talk1Conditions, actor12Talk1Actions, 0xFF },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x10C },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x10F },
    { actor12Talk4Conditions, actor12Talk4Actions, 0x117 },
    { actor12Talk5Conditions, actor12Talk5Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x100 },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x100 },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x10C },
    { actor13Talk3Conditions, actor13Talk3Actions, 0x10F },
    { actor13Talk4Conditions, actor13Talk4Actions, 0x118 },
    { actor13Talk5Conditions, actor13Talk5Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x101 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x101 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0x10C },
    { actor14Talk3Conditions, actor14Talk3Actions, 0x10F },
    { actor14Talk4Conditions, actor14Talk4Actions, 0x119 },
    { actor14Talk5Conditions, actor14Talk5Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0xFD },
    { actor15Talk1Conditions, actor15Talk1Actions, 0xFD },
    { actor15Talk2Conditions, actor15Talk2Actions, 0x10C },
    { actor15Talk3Conditions, actor15Talk3Actions, 0x10F },
    { actor15Talk4Conditions, actor15Talk4Actions, 0x115 },
    { actor15Talk5Conditions, actor15Talk5Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, NULL, 0xFE },
    { actor16Talk1Conditions, actor16Talk1Actions, 0xFE },
    { actor16Talk2Conditions, actor16Talk2Actions, 0x10C },
    { actor16Talk3Conditions, actor16Talk3Actions, 0x10F },
    { actor16Talk4Conditions, actor16Talk4Actions, 0x116 },
    { actor16Talk5Conditions, actor16Talk5Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, NULL, 0x106 },
    { actor17Talk1Conditions, actor17Talk1Actions, 0x106 },
    { actor17Talk2Conditions, actor17Talk2Actions, 0x10D },
    { actor17Talk3Conditions, actor17Talk3Actions, 0x110 },
    { actor17Talk4Conditions, actor17Talk4Actions, 0x11E },
    { actor17Talk5Conditions, actor17Talk5Actions, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0x102 },
    { actor18Talk1Conditions, actor18Talk1Actions, 0x102 },
    { actor18Talk2Conditions, actor18Talk2Actions, 0x10D },
    { actor18Talk3Conditions, actor18Talk3Actions, 0x110 },
    { actor18Talk4Conditions, actor18Talk4Actions, 0x11A },
    { actor18Talk5Conditions, actor18Talk5Actions, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0x104 },
    { actor19Talk1Conditions, actor19Talk1Actions, 0x104 },
    { actor19Talk2Conditions, actor19Talk2Actions, 0x10D },
    { actor19Talk3Conditions, actor19Talk3Actions, 0x110 },
    { actor19Talk4Conditions, actor19Talk4Actions, 0x11C },
    { actor19Talk5Conditions, actor19Talk5Actions, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, NULL, 0x105 },
    { actor20Talk1Conditions, actor20Talk1Actions, 0x105 },
    { actor20Talk2Conditions, actor20Talk2Actions, 0x10D },
    { actor20Talk3Conditions, actor20Talk3Actions, 0x110 },
    { actor20Talk4Conditions, actor20Talk4Actions, 0x11D },
    { actor20Talk5Conditions, actor20Talk5Actions, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0x103 },
    { actor21Talk1Conditions, actor21Talk1Actions, 0x103 },
    { actor21Talk2Conditions, actor21Talk2Actions, 0x10D },
    { actor21Talk3Conditions, actor21Talk3Actions, 0x110 },
    { actor21Talk4Conditions, actor21Talk4Actions, 0x11B },
    { actor21Talk5Conditions, actor21Talk5Actions, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { actor26Talk0Conditions, actor26Talk0Actions, 0xF5 },
    { actor26Talk1Conditions, actor26Talk1Actions, 0xF6 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { actor27Talk0Conditions, actor27Talk0Actions, 0xFB },
    { actor27Talk1Conditions, actor27Talk1Actions, 0xFC },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x25), 1, FLAG(2, 0x78), 0, CODES_END };
u16 actor1Conditions[] = {
    WARP_ARG(0), 1,
    WARP_ARG(0x25), 1,
    FLAG(2, 0x7B), 0,
    ITEM(3, 0x66), 0,
    ITEM(3, 0x67), 0,
    ITEM(3, 0x68), 0,
    ITEM(3, 0x69), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    WARP_ARG(0), 1,
    WARP_ARG(0x25), 1,
    FLAG(2, 0x7C), 0,
    ITEM(3, 0x8D), 0,
    ITEM(3, 0x8E), 0,
    ITEM(3, 0x8F), 0,
    ITEM(3, 0x90), 0,
    CODES_END,
};
u16 actor3Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x20), 1, CODES_END };
u16 actor4Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x24), 1, CODES_END };
u16 actor5Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x22), 1, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x24), 1, CODES_END };
u16 actor9Conditions[] = { WARP_ARG(0), 1, FLAG(0, 8), 0, CODES_END };
u16 actor10Conditions[] = { WARP_ARG(2), 1, FLAG(0, 8), 0, CODES_END };
u16 actor11Conditions[] = { WARP_ARG(4), 1, FLAG(0, 8), 0, CODES_END };
u16 actor12Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1F), 1, CODES_END };
u16 actor13Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x25), 1, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x21), 1, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x23), 1, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x21), 1, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x22), 1, CODES_END };
u16 actor20Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x23), 1, CODES_END };
u16 actor21Conditions[] = { WARP_ARG(3), 1, WARP_ARG(0x20), 1, CODES_END };
u16 actor22Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor23Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor24Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor25Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x21), 1, FLAG(0, 9), 0, CODES_END };
u16 actor26Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x21), 1, FLAG(0xA, 0x10), 0, CODES_END };
u16 actor27Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x25), 1, FLAG(0xA, 0x13), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 800, 344, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x4D, 5, 704, 352, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x4E, 6, 616, 272, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xC1, 7, 448, 200, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xC1, 7, 448, 200, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xC1, 7, 800, 344, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xC1, 7, 448, 200, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xC1, 7, 888, 460, 1 };
FieldActorEntry actor8 = { NULL, NULL, 0x146, 8, 0, 0, 0 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x148, 9, 736, 416, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x148, 9, 800, 448, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x148, 9, 736, 416, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x14D, 0xA, 448, 200, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x14D, 0xA, 888, 460, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x14D, 0xA, 888, 460, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x14D, 0xA, 448, 200, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x14D, 0xA, 448, 200, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x159, 0xB, 448, 200, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x159, 0xB, 448, 200, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x159, 0xB, 888, 460, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x159, 0xB, 888, 460, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x159, 0xB, 800, 344, 1 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x15F, 0xC, 784, 200, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x15F, 0xC, 304, 440, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0x15F, 0xC, 880, 152, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x15F, 0xC, 736, 272, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x18B, 0xD, 464, 424, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x18E, 0xE, 592, 440, 1 };
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
AreaBattle place1Area3Battle0 = { 318, 19, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle1 = { 320, 19, MUSIC(0x22, 0) };
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
AreaBattle place4Area0Battle0 = { 113, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 113, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 114, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 114, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 115, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 115, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 167, 10, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 167, 10, MUSIC(2, 0) };
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
AreaBattle place6Area0Battle0 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 135, 10, MUSIC(2, 0) };
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
    { 417, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 422, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 428, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 434, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 440, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 446, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
};
