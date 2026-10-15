/*
 * WSTAG970: Circuit Board, Underground: the Black Kingz' den, in the extra
 * chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 746 starts it instead of WSTAG875.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x943
#define STAGE_FILE_8 0x68B
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x929
#define STAGE_FILE_8 0x67B
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x20000, 0xB900};
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

StagePoint placePoints1_1Point0 = { 0x2ED, 1, 3, 0x3A0, 128, 1, NULL };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point0 = { 0x2ED, 1, 4, 0x3A0, 128, 1, NULL };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point0 = { 0x2EE, 1, 3, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints2_1Point0 = { 0x2EE, 2, 1, 0x130, 200, 1, NULL };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point0 = { 0x2EE, 2, 1, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints2_3Point0 = { 0x2ED, 2, 2, 0x3A0, 128, 1, NULL };
StagePoints placePoints2_3 = { 2, 3, &placePoints2_3Point0 };
StagePoint placePoints2_4Point0 = { 0x2EE, 2, 2, 0x130, 200, 1, NULL };
StagePoints placePoints2_4 = { 2, 4, &placePoints2_4Point0 };
StagePoint placePoints2_5Point0 = { 0x2EE, 2, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints2_5 = { 2, 5, &placePoints2_5Point0 };
StagePoint placePoints2_6Point0 = { 0x2EE, 2, 5, 0x130, 200, 1, NULL };
StagePoints placePoints2_6 = { 2, 6, &placePoints2_6Point0 };
StagePoint placePoints3_1Point0 = { 0x2EE, 3, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints4_1Point0 = { 0x2ED, 4, 1, 0x3A0, 128, 1, NULL };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point0 = { 0x2EE, 5, 3, 0x130, 200, 1, NULL };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point0 = { 0x2EE, 5, 4, 0x3A0, 0x1A0, 1, NULL };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point0 = { 0x2EE, 5, 5, 0x130, 200, 1, NULL };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints6_1Point0 = { 0x2ED, 6, 2, 0x3A0, 128, 1, NULL };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints2_1,
    &placePoints2_2, &placePoints2_3, &placePoints2_4, &placePoints2_5,
    &placePoints2_6, &placePoints3_1, &placePoints4_1, &placePoints5_1,
    &placePoints5_2, &placePoints5_3, &placePoints6_1, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x148, 0x180, 0x20, 0x80, 0x140, 0x1FF },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x150, 0x140, 0x40, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x160, 0x140, 0x80, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x160, 0x170, 0x80, 0x70, 0x140, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x170, 0xA8, 0x70, 0x150, 0x1FE },
    { 0x140, 0x100, 0x174, 0x170, 0xD0, 0x70, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x180, 0, 0x80, 0x170, 0x1FE },
    { 0x140, 0x100, 0x170, 0x140, 0xC0, 0x40, 0x140, 0x1FD },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x150, 0x1FD },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x170, 0x1FD },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x77), 1, ITEM(2, 0x64), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x7A), 1, ITEM(0, 0x2A), 1, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(0, 0x27), 1, CODES_END };
u16 actor2Talk0Actions[] = { SPECIAL(0x34), 1, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(0, 0x27), 0, FLAG(0, 0), 0, FLAG(0xA, 0x18), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(0, 0x27), 0, FLAG(0, 0), 1, FLAG(0xA, 0x18), 0, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xA, 0x18), 1, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x18), 1, ITEM(0, 0x27), 0, CODES_END };
u16 actor2Talk3Actions[] = { SPECIAL(0x34), 1, ITEM(0, 0x27), 1, CODES_END };
u16 actor2Talk4Conditions[] = { ITEM(0, 0x27), 0, FLAG(0, 0), 1, FLAG(0xA, 0x18), 1, CODES_END };
u16 actor2Talk4Actions[] = { ITEM(0, 0x27), 1, SPECIAL(0x34), 1, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(0, 0x23), 1, CODES_END };
u16 actor3Talk0Actions[] = { SPECIAL(0x32), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(0, 0x23), 0, FLAG(0, 0), 0, FLAG(0xA, 0x16), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(0, 0x23), 0, FLAG(0, 0), 1, FLAG(0xA, 0x16), 0, CODES_END };
u16 actor3Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xA, 0x16), 1, CODES_END };
u16 actor3Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x16), 1, ITEM(0, 0x23), 0, CODES_END };
u16 actor3Talk3Actions[] = { SPECIAL(0x32), 1, ITEM(0, 0x23), 1, CODES_END };
u16 actor3Talk4Conditions[] = { ITEM(0, 0x23), 0, FLAG(0, 0), 1, FLAG(0xA, 0x16), 1, CODES_END };
u16 actor3Talk4Actions[] = { ITEM(0, 0x23), 1, SPECIAL(0x32), 1, CODES_END };
u16 actor4Talk0Conditions[] = { ITEM(0, 0x13), 1, CODES_END };
u16 actor4Talk0Actions[] = { SPECIAL(0x37), 1, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(0, 0x13), 0, FLAG(0, 0), 0, FLAG(0xA, 0x19), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk2Conditions[] = { ITEM(0, 0x13), 0, FLAG(0, 0), 1, FLAG(0xA, 0x19), 0, CODES_END };
u16 actor4Talk2Actions[] = { EVENT_BATTLE(2), 1, FLAG(0xA, 0x19), 1, CODES_END };
u16 actor4Talk3Conditions[] = { ITEM(0, 0x13), 0, FLAG(0, 0), 0, FLAG(0xA, 0x19), 1, CODES_END };
u16 actor4Talk3Actions[] = { ITEM(0, 0x13), 1, SPECIAL(0x37), 1, CODES_END };
u16 actor4Talk4Conditions[] = { FLAG(0, 0), 1, FLAG(0xA, 0x19), 1, ITEM(0, 0x13), 0, CODES_END };
u16 actor4Talk4Actions[] = { SPECIAL(0x37), 1, ITEM(0, 0x13), 1, CODES_END };
u16 actor5Talk0Conditions[] = { ITEM(0, 0x18B), 1, CODES_END };
u16 actor5Talk0Actions[] = { SPECIAL(0x39), 1, CODES_END };
u16 actor5Talk1Conditions[] = { ITEM(0, 0x18B), 0, FLAG(0, 0), 0, FLAG(0xA, 0x1B), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk2Conditions[] = { ITEM(0, 0x18B), 0, FLAG(0, 0), 1, FLAG(0xA, 0x1B), 0, CODES_END };
u16 actor5Talk2Actions[] = { EVENT_BATTLE(3), 1, FLAG(0xA, 0x1B), 1, CODES_END };
u16 actor5Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x1B), 1, ITEM(0, 0x18B), 0, CODES_END };
u16 actor5Talk3Actions[] = { SPECIAL(0x39), 1, ITEM(0, 0x18B), 1, CODES_END };
u16 actor5Talk4Conditions[] = { ITEM(0, 0x18B), 0, FLAG(0, 0), 1, FLAG(0xA, 0x1B), 1, CODES_END };
u16 actor5Talk4Actions[] = { ITEM(0, 0x18B), 1, SPECIAL(0x39), 1, CODES_END };
u16 actor6Talk0Conditions[] = { ITEM(3, 0x82), 1, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(3, 0x82), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk2Conditions[] = { ITEM(3, 0x82), 0, FLAG(0, 0), 1, ITEM(2, 0x7E), 0, CODES_END };
u16 actor6Talk3Conditions[] = { ITEM(3, 0x82), 0, FLAG(0, 0), 1, ITEM(2, 0x7E), 1, CODES_END };
u16 actor6Talk3Actions[] = { ITEM(2, 0x7E), 0, ITEM(3, 0x82), 1, ITEM(3, 0x81), 0, CODES_END };
u16 actor7Talk0Conditions[] = { ITEM(3, 0x83), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(3, 0x83), 0, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(3, 0x83), 0, ITEM(2, 0x7F), 0, CODES_END };
u16 actor7Talk3Conditions[] = { ITEM(3, 0x83), 0, FLAG(0, 0), 1, ITEM(2, 0x7F), 1, CODES_END };
u16 actor7Talk3Actions[] = { ITEM(3, 0x83), 1, ITEM(2, 0x7F), 0, ITEM(3, 0x82), 0, CODES_END };
u16 actor8Talk0Conditions[] = { ITEM(3, 0x9A), 1, CODES_END };
u16 actor8Talk1Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk2Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 1, ITEM(2, 0x96), 0, CODES_END };
u16 actor8Talk3Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 1, ITEM(2, 0x96), 1, CODES_END };
u16 actor8Talk3Actions[] = { ITEM(3, 0x9A), 1, ITEM(3, 0x99), 0, ITEM(2, 0x96), 0, CODES_END };
u16 actor9Talk0Conditions[] = { ITEM(3, 0x9C), 1, CODES_END };
u16 actor9Talk1Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk2Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 1, ITEM(2, 0x98), 0, CODES_END };
u16 actor9Talk3Conditions[] = { ITEM(3, 0x9C), 0, FLAG(0, 0), 1, ITEM(2, 0x98), 1, CODES_END };
u16 actor9Talk3Actions[] = { ITEM(3, 0x9C), 1, ITEM(3, 0x9B), 0, ITEM(2, 0x98), 0, CODES_END };
u16 actor10Talk0Conditions[] = { ITEM(3, 0x69), 1, CODES_END };
u16 actor10Talk1Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 0), 0, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor10Talk2Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 0), 1, ITEM(2, 0x65), 0, CODES_END };
u16 actor10Talk3Conditions[] = { ITEM(3, 0x69), 0, FLAG(0, 0), 1, ITEM(2, 0x65), 1, CODES_END };
u16 actor10Talk3Actions[] = { ITEM(3, 0x69), 1, ITEM(3, 0x68), 0, ITEM(2, 0x65), 0, CODES_END };
u16 actor11Talk0Conditions[] = { ITEM(3, 0x68), 1, CODES_END };
u16 actor11Talk1Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 0, CODES_END };
u16 actor11Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor11Talk2Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 1, ITEM(2, 0x64), 0, CODES_END };
u16 actor11Talk3Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 1, ITEM(2, 0x64), 1, CODES_END };
u16 actor11Talk3Actions[] = { ITEM(3, 0x68), 1, ITEM(3, 0x67), 0, ITEM(2, 0x64), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0xF },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x12 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0xE1 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0xE2 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0xE3 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0xE4 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0xE4 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0xD9 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0xDA },
    { actor3Talk2Conditions, actor3Talk2Actions, 0xDB },
    { actor3Talk3Conditions, actor3Talk3Actions, 0xDC },
    { actor3Talk4Conditions, actor3Talk4Actions, 0xDC },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0xE5 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0xE6 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0xE7 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0xE8 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0xE8 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0xED },
    { actor5Talk1Conditions, actor5Talk1Actions, 0xEE },
    { actor5Talk2Conditions, actor5Talk2Actions, 0xEF },
    { actor5Talk3Conditions, actor5Talk3Actions, 0xF0 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0xF0 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0xAD },
    { actor6Talk1Conditions, actor6Talk1Actions, 0xAE },
    { actor6Talk2Conditions, NULL, 0xAF },
    { actor6Talk3Conditions, actor6Talk3Actions, 0xB0 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0xC1 },
    { actor7Talk1Conditions, actor7Talk1Actions, 0xC2 },
    { actor7Talk2Conditions, NULL, 0xC3 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0xC4 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0xA1 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0xA2 },
    { actor8Talk2Conditions, NULL, 0xA3 },
    { actor8Talk3Conditions, actor8Talk3Actions, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0xC9 },
    { actor9Talk1Conditions, actor9Talk1Actions, 0xCA },
    { actor9Talk2Conditions, NULL, 0xCB },
    { actor9Talk3Conditions, actor9Talk3Actions, 0xCC },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0xBD },
    { actor10Talk1Conditions, actor10Talk1Actions, 0xBE },
    { actor10Talk2Conditions, NULL, 0xBF },
    { actor10Talk3Conditions, actor10Talk3Actions, 0xC0 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0xA9 },
    { actor11Talk1Conditions, actor11Talk1Actions, 0xAA },
    { actor11Talk2Conditions, NULL, 0xAB },
    { actor11Talk3Conditions, actor11Talk3Actions, 0xAC },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(2), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x77), 0, CODES_END };
u16 actor1Conditions[] = {
    WARP_ARG(3), 1,
    WARP_ARG(0x1E), 1,
    FLAG(2, 0x7A), 0,
    ITEM(0, 0x2A), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    WARP_ARG(4), 1,
    WARP_ARG(0x1F), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0xE), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    WARP_ARG(4), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0xD), 0,
    CODES_END,
};
u16 actor4Conditions[] = {
    WARP_ARG(4), 1,
    WARP_ARG(0x20), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0x10), 0,
    CODES_END,
};
u16 actor5Conditions[] = {
    WARP_ARG(5), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0x12), 0,
    CODES_END,
};
u16 actor6Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x20), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x81), 1,
    ITEM(3, 0x82), 0,
    CODES_END,
};
u16 actor7Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x22), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x82), 1,
    ITEM(3, 0x83), 0,
    CODES_END,
};
u16 actor8Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x1F), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x99), 1,
    ITEM(3, 0x9A), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x23), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x9B), 1,
    ITEM(3, 0x9C), 0,
    CODES_END,
};
u16 actor10Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x68), 1,
    ITEM(3, 0x69), 0,
    CODES_END,
};
u16 actor11Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x21), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x67), 1,
    ITEM(3, 0x68), 0,
    CODES_END,
};
u16 actor13Conditions[] = { WARP_ARG(0), 1, FLAG(0, 8), 0, CODES_END };
u16 actor14Conditions[] = { WARP_ARG(0), 0, FLAG(0, 8), 0, CODES_END };
u16 actor15Conditions[] = { WARP_ARG(0), 1, FLAG(0, 9), 0, CODES_END };
u16 actor16Conditions[] = { WARP_ARG(0), 0, FLAG(0, 9), 0, WARP_ARG(0x1E), 1, CODES_END };
u16 actor17Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor18Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x20), 1, FLAG(0, 9), 0, CODES_END };
u16 actor19Conditions[] = { WARP_ARG(0), 0, WARP_ARG(0x1E), 1, FLAG(0, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 432, 144, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x21, 4, 432, 144, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x7C, 5, 432, 144, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x7F, 6, 432, 144, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x80, 7, 432, 144, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x81, 8, 432, 144, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xA7, 9, 432, 144, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xA7, 9, 432, 144, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0xA9, 0xA, 432, 144, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xA9, 0xA, 432, 144, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xAC, 0xB, 432, 144, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xAC, 0xB, 432, 144, 7 };
FieldActorEntry actor12 = { NULL, NULL, 0x146, 0xC, 0, 0, 0 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x148, 0xD, 272, 488, 1 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x148, 0xD, 392, 380, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x15F, 0xE, 368, 440, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x15F, 0xE, 464, 248, 1 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x15F, 0xE, 384, 336, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x15F, 0xE, 456, 300, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x160, 0xF, 528, 264, 1 };
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
AreaBattle place1Area0Battle0 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 141, 10, MUSIC(2, 0) };
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
AreaBattle place2Area0Battle0 = { 83, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 83, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 83, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 83, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 84, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 84, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 84, 10, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 84, 10, MUSIC(2, 0) };
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
AreaBattle place5Area0Battle0 = { 116, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 116, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 164, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 164, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 139, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 139, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 163, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 163, 10, MUSIC(2, 0) };
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
AreaBattle place5Area3Battle0 = { 303, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle1 = { 309, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle2 = { 310, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle3 = { 312, 10, MUSIC(0x22, 0) };
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
AreaBattle place6Area3Battle0 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle1 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle2 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle3 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle4 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle5 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle6 = { 312, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle7 = { 312, 10, MUSIC(0x22, 0) };
BattleList place6Area3Battles = {
    0,
    { &place6Area3Battle0, &place6Area3Battle1, &place6Area3Battle2, &place6Area3Battle3,
      &place6Area3Battle4, &place6Area3Battle5, &place6Area3Battle6, &place6Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 415, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 421, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 427, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 433, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 438, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 444, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
};
