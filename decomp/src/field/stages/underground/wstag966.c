/*
 * WSTAG966: Seabed, Underground: the junction of the Seabed tunnels, in the
 * extra chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 742 starts it instead of WSTAG855.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x93B
#define STAGE_FILE_8 0x6F0
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x919
#define STAGE_FILE_8 0x6E0
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x13300, 0x13B00};
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

StagePoint placePoints2_1Point1 = { 0x2E4, 2, 1, 0x340, 160, 1, NULL };
StagePoint placePoints2_1Point0 = { 0x2E0, 2, 1, 176, 0x178, 5, &placePoints2_1Point1 };
StagePoints placePoints2_1 = { 2, 1, &placePoints2_1Point0 };
StagePoint placePoints2_2Point1 = { 0x2E4, 2, 2, 0x340, 160, 1, NULL };
StagePoint placePoints2_2Point0 = { 0x2E4, 2, 1, 192, 0x180, 5, &placePoints2_2Point1 };
StagePoints placePoints2_2 = { 2, 2, &placePoints2_2Point0 };
StagePoint placePoints2_3Point1 = { 0x2E4, 2, 4, 0x340, 160, 1, NULL };
StagePoint placePoints2_3Point0 = { 0x2E4, 2, 3, 192, 0x180, 5, &placePoints2_3Point1 };
StagePoints placePoints2_3 = { 2, 3, &placePoints2_3Point0 };
StagePoint placePoints3_1Point1 = { 0x2E7, 3, 1, 0x250, 232, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x2E5, 3, 1, 224, 0x120, 5, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints4_1Point1 = { 0x2E4, 4, 1, 0x340, 160, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x2E3, 4, 1, 160, 0x180, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point1 = { 0x2E4, 5, 2, 0x340, 160, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2E4, 5, 1, 192, 0x180, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoints *placePoints[] = {
    &placePoints2_1, &placePoints2_2, &placePoints2_3, &placePoints3_1,
    &placePoints4_1, &placePoints5_1, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x170, 0x1BC, 0xC0, 0xBC, 0x150, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 0x147, 4, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 155, 230, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 440, 261, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 797, 447, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 1127, 463, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 312, 203, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 657, 342, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 916, 467, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 768, 289, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 215, 158, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 591, 261, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 983, 460, 0, 0 },
    { 1, 0, 0x50, 4, 0, 0, 0, 0, 0, 0, 192, 257, 340, 0 },
    { 1, 0, 0x65, 4, 1, 0, 0, 0, 0, 0, 160, 241, 334, 0 },
    { 1, 0, 0x5E, 4, 2, 0, 0, 0, 0, 0, 144, 237, 324, 0 },
    { 1, 0, 0x91, 4, 3, 0, 0, 0, 0, 0, 368, 184, 306, 0 },
    { 1, 0, 0x77, 4, 4, 0, 0, 0, 0, 0, 336, 204, 322, 0 },
    { 1, 0, 0x89, 4, 5, 0, 0, 0, 0, 0, 304, 218, 347, 0 },
    { 1, 0, 0x82, 4, 6, 0, 0, 0, 0, 0, 272, 238, 354, 0 },
    { 1, 0, 0x5E, 4, 7, 0, 0, 0, 0, 0, 464, 278, 368, 0 },
    { 1, 0, 0x66, 4, 8, 0, 0, 0, 0, 0, 432, 283, 375, 0 },
    { 1, 0, 0x52, 4, 9, 0, 0, 0, 0, 0, 414, 298, 380, 0 },
    { 1, 0, 0x51, 4, 0xA, 0, 0, 0, 0, 0, 544, 316, 390, 0 },
    { 1, 0, 0x60, 4, 0xB, 0, 0, 0, 0, 0, 512, 320, 405, 0 },
    { 1, 0, 0x52, 4, 0xC, 0, 0, 0, 0, 0, 494, 336, 415, 0 },
    { 1, 0, 0x93, 4, 0xD, 0, 0, 0, 0, 0, 720, 311, 435, 0 },
    { 1, 0, 0x79, 4, 0xE, 0, 0, 0, 0, 0, 688, 331, 451, 0 },
    { 1, 0, 0x8B, 4, 0xF, 0, 0, 0, 0, 0, 656, 345, 476, 0 },
    { 1, 0, 0x89, 4, 0x10, 0, 0, 0, 0, 0, 624, 360, 483, 0 },
    { 1, 0, 0x5D, 4, 0x11, 0, 0, 0, 0, 0, 752, 421, 506, 0 },
    { 1, 0, 0x67, 4, 0x12, 0, 0, 0, 0, 0, 720, 424, 515, 0 },
    { 1, 0, 0x50, 4, 0x13, 0, 0, 0, 0, 0, 702, 441, 527, 0 },
    { 1, 0, 0x52, 4, 0x14, 0, 0, 0, 0, 0, 1104, 521, 606, 0 },
    { 1, 0, 0x65, 4, 0x15, 0, 0, 0, 0, 0, 1072, 505, 598, 0 },
    { 1, 0, 0x5D, 4, 0x16, 0, 0, 0, 0, 0, 1057, 501, 587, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E6, 0x450, 0x248, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E6, 0xA0, 0x150, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
AreaBattle place2Area0Battle0 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 62, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 106, 11, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 106, 11, MUSIC(2, 0) };
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
AreaBattle place3Area0Battle0 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 105, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 105, 11, MUSIC(2, 0) };
BattleList place3Area0Battles = {
    5,
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
AreaBattle place4Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 61, 11, MUSIC(2, 0) };
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
AreaBattle place5Area0Battle0 = { 185, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 185, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 185, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 185, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 185, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 178, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 178, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 178, 11, MUSIC(2, 0) };
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
    { 400, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
    { 404, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 408, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 412, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
};
