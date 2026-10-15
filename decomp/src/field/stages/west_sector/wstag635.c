/*
 * WSTAG635: Mobius Desert, West Sector: the way in. Between S Noise Desert
 * (WSTAG630) and the Mirage Tower (645); it leads on to WSTAG640.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateTileAnimsFrames0[];
extern AnimFrame updateTileAnimsFrames1[];
extern AnimFrame updateTileAnimsFrames2[];

#include "common/step_looping_animation.inc.c"

#include "common/update_tile_anims3_once.inc.c"

#include "common/create_tile_anims_5c.inc.c"

#include "common/copy_place_points.inc.c"

#include "common/update_stage_tile_anims_places.inc.c"

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x48C
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x49C
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x20900, 0x31B00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x38;
    FIELDSTG_state.music = MUSIC(0x38, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AnimFrame updateTileAnimsFrames0[] = {
    { 50, 8 }, { 51, 8 }, { 52, 8 }, { 53, 8 },
    { 54, 8 }, { 55, 8 }, { 56, 8 }, { 57, 8 },
    { 58, 8 }, { 59, 8 }, { 60, 8 }, { 82, 160 },
    { 255, 0 },
};
AnimFrame updateTileAnimsFrames1[] = {
    { 61, 8 }, { 62, 8 }, { 63, 8 }, { 64, 8 },
    { 65, 8 }, { 66, 8 }, { 67, 8 }, { 68, 8 },
    { 69, 8 }, { 70, 8 }, { 71, 8 }, { 82, 160 },
    { 255, 0 },
};
AnimFrame updateTileAnimsFrames2[] = {
    { 72, 8 }, { 73, 8 }, { 74, 8 }, { 75, 8 },
    { 76, 8 }, { 77, 8 }, { 78, 8 }, { 79, 8 },
    { 80, 8 }, { 81, 8 }, { 82, 160 }, { 255, 0 },
};
StagePoint placePoints1_1Point3 = { 0x259, 1, 2, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_1Point2 = { 0x259, 1, 3, 0x358, 252, 1, &placePoints1_1Point3 };
StagePoint placePoints1_1Point1 = { 0x259, 1, 1, 0x120, 0x100, 7, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x257, 0, 0, 0x100, 0x278, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point3 = { 0x259, 1, 1, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_2Point2 = { 0x259, 1, 4, 0x358, 252, 1, &placePoints1_2Point3 };
StagePoint placePoints1_2Point1 = { 0x259, 1, 2, 0x120, 0x100, 7, &placePoints1_2Point2 };
StagePoint placePoints1_2Point0 = { 0x257, 0, 0, 0x100, 0x278, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point3 = { 0x259, 1, 3, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_3Point2 = { 0x259, 1, 5, 0x358, 252, 1, &placePoints1_3Point3 };
StagePoint placePoints1_3Point1 = { 0x259, 1, 4, 0x120, 0x100, 7, &placePoints1_3Point2 };
StagePoint placePoints1_3Point0 = { 0x259, 1, 1, 0x118, 0x2EC, 5, &placePoints1_3Point1 };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point3 = { 0x259, 1, 4, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_4Point2 = { 0x259, 1, 6, 0x358, 252, 1, &placePoints1_4Point3 };
StagePoint placePoints1_4Point1 = { 0x259, 1, 3, 0x120, 0x100, 7, &placePoints1_4Point2 };
StagePoint placePoints1_4Point0 = { 0x259, 1, 2, 0x118, 0x2EC, 5, &placePoints1_4Point1 };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints1_5Point3 = { 0x259, 1, 6, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_5Point2 = { 0x259, 1, 7, 0x358, 252, 1, &placePoints1_5Point3 };
StagePoint placePoints1_5Point1 = { 0x259, 1, 5, 0x120, 0x100, 7, &placePoints1_5Point2 };
StagePoint placePoints1_5Point0 = { 0x259, 1, 3, 0x118, 0x2EC, 5, &placePoints1_5Point1 };
StagePoints placePoints1_5 = { 1, 5, &placePoints1_5Point0 };
StagePoint placePoints1_6Point3 = { 0x25A, 0, 0, 0x2F8, 0x244, 3, NULL };
StagePoint placePoints1_6Point2 = { 0x259, 1, 8, 0x358, 252, 1, &placePoints1_6Point3 };
StagePoint placePoints1_6Point1 = { 0x259, 1, 6, 0x120, 0x100, 7, &placePoints1_6Point2 };
StagePoint placePoints1_6Point0 = { 0x259, 1, 4, 0x118, 0x2EC, 5, &placePoints1_6Point1 };
StagePoints placePoints1_6 = { 1, 6, &placePoints1_6Point0 };
StagePoint placePoints1_7Point3 = { 0x259, 1, 7, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_7Point2 = { 0x259, 1, 1, 0x358, 252, 1, &placePoints1_7Point3 };
StagePoint placePoints1_7Point1 = { 0x259, 1, 8, 0x120, 0x100, 7, &placePoints1_7Point2 };
StagePoint placePoints1_7Point0 = { 0x259, 1, 5, 0x118, 0x2EC, 5, &placePoints1_7Point1 };
StagePoints placePoints1_7 = { 1, 7, &placePoints1_7Point0 };
StagePoint placePoints1_8Point3 = { 0x259, 1, 8, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_8Point2 = { 0x259, 1, 2, 0x358, 252, 1, &placePoints1_8Point3 };
StagePoint placePoints1_8Point1 = { 0x259, 1, 7, 0x120, 0x100, 7, &placePoints1_8Point2 };
StagePoint placePoints1_8Point0 = { 0x259, 1, 6, 0x118, 0x2EC, 5, &placePoints1_8Point1 };
StagePoints placePoints1_8 = { 1, 8, &placePoints1_8Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints1_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints1_5, &placePoints1_6, &placePoints1_7, &placePoints1_8,
    &placePoints0_0, NULL,
};
AreaBattle area0Battle0 = { 73, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 73, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 73, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 74, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 74, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 74, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 156, 5, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area1Battles = {
    0,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area2Battles = {
    0,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 37, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x160, 0xD8, 0x60, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x188, 0xD8, 0x88, 0x140, 0x1FE },
    { 0x180, 0x100, 0x1B0, 0x188, 0x1C0, 0x88, 0x150, 0x1FE },
    { 0x180, 0x100, 0x188, 0x194, 0x120, 0x94, 0x160, 0x1FE },
    { 0x180, 0x100, 0x180, 0x19C, 0x100, 0x9C, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x140, 0x1FD },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 0x2F), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 0x2F), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 0x2F), 1, SPECIAL(0x2E), 0, CODES_END };
u16 actor0Talk2Conditions[] = {
    FLAG(0x1A, 0x2F), 1,
    SPECIAL(0x2E), 1,
    SPECIAL(0x27), 0,
    CODES_END,
};
u16 actor0Talk3Conditions[] = {
    FLAG(0x1A, 0x2F), 1,
    SPECIAL(0x2E), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { FLAG(6, 4), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0x1A, 0x2F), 1,
    SPECIAL(0x2E), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 1,
    CODES_END,
};
u16 actor1Talk0Conditions[] = { SPECIAL(0x10), 0, CODES_END };
u16 actor1Talk0Actions[] = { SPECIAL(0x37), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk1Conditions[] = { SPECIAL(0x10), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x30), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1A, 0x30), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0x30), 1, SPECIAL(0x30), 0, CODES_END };
u16 actor2Talk2Conditions[] = {
    FLAG(0x1A, 0x30), 1,
    SPECIAL(0x30), 1,
    SPECIAL(0x27), 0,
    CODES_END,
};
u16 actor2Talk3Conditions[] = {
    FLAG(0x1A, 0x30), 1,
    SPECIAL(0x30), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { FLAG(6, 5), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0x1A, 0x30), 1,
    SPECIAL(0x30), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { SPECIAL(0x12), 0, CODES_END };
u16 actor3Talk0Actions[] = { SPECIAL(0x39), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor3Talk1Conditions[] = { SPECIAL(0x12), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0x1A, 0x31), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0x1A, 0x31), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0x1A, 0x31), 1, SPECIAL(0x2F), 0, CODES_END };
u16 actor7Talk2Conditions[] = {
    FLAG(0x1A, 0x31), 1,
    SPECIAL(0x2F), 1,
    SPECIAL(0x27), 0,
    CODES_END,
};
u16 actor7Talk3Conditions[] = {
    FLAG(0x1A, 0x31), 1,
    SPECIAL(0x2F), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 0,
    CODES_END,
};
u16 actor7Talk3Actions[] = { FLAG(6, 6), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0x1A, 0x31), 1,
    SPECIAL(0x2F), 1,
    SPECIAL(0x27), 1,
    SPECIAL(0x29), 1,
    CODES_END,
};
u16 actor8Talk0Conditions[] = { SPECIAL(0x11), 0, CODES_END };
u16 actor8Talk0Actions[] = { SPECIAL(0x38), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor8Talk1Conditions[] = { SPECIAL(0x11), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x33D },
    { actor0Talk1Conditions, NULL, 0x343 },
    { actor0Talk2Conditions, NULL, 0x33E },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x33F },
    { actor0Talk4Conditions, NULL, 0x344 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x340 },
    { actor1Talk1Conditions, NULL, 0x342 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x345 },
    { actor2Talk1Conditions, NULL, 0x34B },
    { actor2Talk2Conditions, NULL, 0x346 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x347 },
    { actor2Talk4Conditions, NULL, 0x34C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x348 },
    { actor3Talk1Conditions, NULL, 0x34A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x341 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x349 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x351 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x34D },
    { actor7Talk1Conditions, NULL, 0x353 },
    { actor7Talk2Conditions, NULL, 0x34E },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x34F },
    { actor7Talk4Conditions, NULL, 0x354 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x350 },
    { actor8Talk1Conditions, NULL, 0x352 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x13), 0,
    WARP_ARG(0), 1,
    WARP_ARG(0x1E), 1,
    CODES_END,
};
u16 actor1Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x13), 1,
    WARP_ARG(0x1E), 1,
    WARP_ARG(0), 1,
    CODES_END,
};
u16 actor2Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x18B), 0,
    WARP_ARG(0x21), 1,
    WARP_ARG(0), 1,
    CODES_END,
};
u16 actor3Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x18B), 1,
    WARP_ARG(0x21), 1,
    WARP_ARG(0), 1,
    CODES_END,
};
u16 actor4Conditions[] = { SPECIAL(0x1A), 1, WARP_ARG(0x1E), 1, WARP_ARG(0), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1A), 1, WARP_ARG(0x21), 1, WARP_ARG(0), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, WARP_ARG(0), 1, WARP_ARG(0x24), 1, CODES_END };
u16 actor7Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x168), 0,
    WARP_ARG(0x24), 1,
    WARP_ARG(0), 1,
    CODES_END,
};
u16 actor8Conditions[] = {
    SPECIAL(0x93), 1,
    SPECIAL(0x1A), 0,
    ITEM(0, 0x168), 1,
    WARP_ARG(0x24), 1,
    WARP_ARG(0), 1,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2B, 4, 480, 696, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2B, 4, 480, 696, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2C, 5, 800, 520, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2C, 5, 800, 520, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9D, 6, 480, 696, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9E, 7, 800, 520, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9F, 8, 408, 364, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xA3, 9, 408, 364, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0xA3, 9, 408, 364, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 3, 0xC8, 2, 0x48, 0, 0, 0, 0, 0, 50, 640, 0, 0 },
    { 1, 3, 0xC8, 2, 0x48, 0, 0, 0, 0, 0, 348, 103, 0, 0 },
    { 1, 3, 0xC8, 2, 0x48, 0, 0, 0, 0, 0, 369, 851, 0, 0 },
    { 1, 0, 0x40, 2, 7, 1, 7, 0xC, 4, 0, 570, 141, 0, 0 },
    { 1, 0, 0x40, 2, 7, 1, 7, 0xC, 4, 0, 906, 499, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 56, 957, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 92, 232, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 113, 400, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 340, 812, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 891, 290, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 1096, 196, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 417, 278, 329, 0 },
    { 1, 0, 0x48, 4, 1, 0, 0, 0, 0, 0, 348, 278, 342, 0 },
    { 1, 0, 0x4A, 4, 2, 0, 0, 0, 0, 0, 529, 611, 680, 0 },
    { 1, 0, 0x5A, 4, 3, 0, 0, 0, 0, 0, 422, 570, 656, 0 },
    { 1, 0, 0x58, 4, 4, 0, 0, 0, 0, 0, 384, 635, 719, 0 },
    { 1, 0, 0x60, 4, 5, 0, 0, 0, 0, 0, 919, 519, 608, 0 },
    { 1, 0, 0x5E, 4, 6, 0, 0, 0, 0, 0, 583, 159, 248, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x258, 0x340, 0xF0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x258, 0x368, 0x2EC, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x258, 0x128, 0x2F4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x258, 0x110, 0x108, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
