/*
 * WSTAG964: Seabed, Underground: Seehomon and Depthmon's forge, in the extra
 * chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 740 starts it instead of WSTAG845.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x937
#define STAGE_FILE_8 0x704
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x915
#define STAGE_FILE_8 0x6F4
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xE500, 0x16900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1D;
    FIELDSTG_state.music = MUSIC(0x1D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 6);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 7);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 5);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints2_1Point1 = { 0x2E6, 2, 2, 0x450, 0x248, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x2E6, 2, 1, 160, 0x150, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point1 = { 0x2E4, 2, 3, 0x340, 160, 1, NULL };
StagePoint placePoints2_2Point0 = { 0x2E6, 2, 2, 160, 0x150, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints2_3Point1 = { 0x2E6, 2, 3, 0x450, 0x248, 1, NULL };
StagePoint placePoints2_3Point0 = { 0x2E4, 2, 2, 192, 0x180, 5, &placePoints2_3Point1 };
StagePoints placePoints2_3 = { 2, 3, &placePoints2_3Point0 };
StagePoint placePoints2_4Point1 = { 0x2E2, 2, 1, 0x330, 248, 1, NULL };
StagePoint placePoints2_4Point0 = { 0x2E6, 2, 3, 160, 0x150, 5, &placePoints2_4Point1 };
StagePoints placePoints2_4 = { 2, 4, &placePoints2_4Point0 };
StagePoint placePoints3_1Point1 = { 0x2E5, 3, 1, 0x3B0, 216, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x2E0, 3, 1, 176, 0x178, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints4_1Point1 = { 0x2E7, 4, 1, 0x250, 232, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x2E6, 4, 1, 160, 0x150, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point1 = { 0x2E6, 5, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2E0, 5, 1, 176, 0x178, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point1 = { 0x2E4, 5, 3, 0x340, 160, 1, NULL };
StagePoint placePoints5_2Point0 = { 0x2E6, 5, 1, 160, 0x150, 5, &placePoints5_2Point1 };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point1 = { 0x2E7, 5, 1, 0x250, 232, 1, NULL };
StagePoint placePoints5_3Point0 = { 0x2E4, 5, 2, 192, 0x180, 5, &placePoints5_3Point1 };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoints *placePoints[] = {
    &placePoints2_1, &placePoints2_2, &placePoints2_3, &placePoints2_4,
    &placePoints3_1, &placePoints4_1, &placePoints5_1, &placePoints5_2,
    &placePoints5_3, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14A, 0x1A0, 0x28, 0xA0, 0x150, 0x1FF },
    { 0x180, 0x100, 0x1B0, 0x178, 0x1C0, 0x78, 0x160, 0x1FF },
    { 0x140, 0x100, 0x170, 0x1D0, 0xC0, 0xD0, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x140, 0x1FE },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x73), 1, ITEM(1, 0x34), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x6F), 1, ITEM(2, 0x7D), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 0x70), 1, ITEM(2, 0x8A), 1, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(3, 0x76), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 1, ITEM(2, 0x72), 0, CODES_END };
u16 actor3Talk3Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 1, ITEM(2, 0x72), 1, CODES_END };
u16 actor3Talk3Actions[] = { ITEM(3, 0x76), 1, ITEM(3, 0x75), 0, ITEM(2, 0x72), 0, CODES_END };
u16 actor4Talk0Conditions[] = { ITEM(3, 0x9B), 1, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk2Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 0, CODES_END };
u16 actor4Talk3Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 1, CODES_END };
u16 actor4Talk3Actions[] = { ITEM(3, 0x9B), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x97), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 7 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 8 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0xB1 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0xB2 },
    { actor3Talk2Conditions, NULL, 0xB3 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0xB4 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0xB5 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0xB6 },
    { actor4Talk2Conditions, NULL, 0xB7 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0xB8 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(1), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x73), 0, CODES_END };
u16 actor1Conditions[] = { WARP_ARG(1), 1, WARP_ARG(0x21), 1, FLAG(2, 0x6F), 0, CODES_END };
u16 actor2Conditions[] = { WARP_ARG(4), 1, WARP_ARG(0x1E), 1, FLAG(2, 0x70), 0, CODES_END };
u16 actor3Conditions[] = {
    WARP_ARG(1), 1,
    WARP_ARG(0x1F), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x75), 1,
    ITEM(3, 0x76), 0,
    CODES_END,
};
u16 actor4Conditions[] = {
    WARP_ARG(3), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x55), 1,
    SPECIAL(0x95), 1,
    ITEM(3, 0x9A), 1,
    ITEM(3, 0x9B), 0,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 544, 240, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x21, 4, 544, 240, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x21, 4, 544, 240, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xA8, 5, 544, 240, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xAA, 6, 544, 240, 1 };
FieldActorEntry actor5 = { NULL, NULL, 0x147, 7, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 200, 265, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 560, 185, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 857, 33, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 395, 180, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 500, 39, 0, 0 },
    { 1, 0, 0xB6, 6, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 659, 0, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 176, 128, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 758, 41, 0, 0 },
    { 1, 0, 0x8A, 4, 3, 0, 0, 0, 0, 0, 416, 191, 327, 0 },
    { 1, 0, 0x52, 4, 4, 0, 0, 0, 0, 0, 384, 174, 311, 0 },
    { 1, 0, 0x83, 4, 5, 0, 0, 0, 0, 0, 352, 169, 295, 0 },
    { 1, 0, 0x8E, 4, 6, 0, 0, 0, 0, 0, 320, 154, 279, 0 },
    { 1, 0, 0x54, 4, 8, 0, 0, 0, 0, 0, 672, 177, 268, 0 },
    { 1, 0, 0x66, 4, 9, 0, 0, 0, 0, 0, 640, 162, 251, 0 },
    { 1, 0, 0x60, 4, 0xA, 0, 0, 0, 0, 0, 624, 156, 242, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E4, 0x340, 0xA0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E4, 0xC0, 0x180, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
AreaBattle place2Area0Battle0 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 276, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 276, 11, MUSIC(2, 0) };
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
AreaBattle place3Area0Battle0 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 64, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 64, 11, MUSIC(2, 0) };
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
AreaBattle place4Area0Battle0 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 159, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 159, 11, MUSIC(2, 0) };
BattleList place4Area0Battles = {
    5,
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
AreaBattle place5Area0Battle0 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 104, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 104, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 104, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 104, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 104, 11, MUSIC(2, 0) };
BattleList place5Area0Battles = {
    4,
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
FieldBattles stageBattles[] = {
    { 399, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 402, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 407, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 411, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
};
