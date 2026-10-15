/*
 * WSTAG636: Mobius Desert, West Sector: the way in, on the Amaterasu server.
 * On the Amaterasu server; its Asuka server twin is WSTAG635.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x627
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x637
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x11F00, 0x2F100};
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

StagePoint placePoints1_1Point3 = { 0x2C2, 1, 2, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_1Point2 = { 0x2C2, 1, 3, 0x358, 252, 1, &placePoints1_1Point3 };
StagePoint placePoints1_1Point1 = { 0x2C2, 1, 1, 0x120, 0x100, 7, &placePoints1_1Point2 };
StagePoint placePoints1_1Point0 = { 0x2C0, 0, 0, 0x100, 0x278, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point3 = { 0x2C2, 1, 1, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_2Point2 = { 0x2C2, 1, 4, 0x358, 252, 1, &placePoints1_2Point3 };
StagePoint placePoints1_2Point1 = { 0x2C2, 1, 2, 0x120, 0x100, 7, &placePoints1_2Point2 };
StagePoint placePoints1_2Point0 = { 0x2C0, 0, 0, 0x100, 0x278, 5, &placePoints1_2Point1 };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point3 = { 0x2C2, 1, 3, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_3Point2 = { 0x2C2, 1, 5, 0x358, 252, 1, &placePoints1_3Point3 };
StagePoint placePoints1_3Point1 = { 0x2C2, 1, 4, 0x120, 0x100, 7, &placePoints1_3Point2 };
StagePoint placePoints1_3Point0 = { 0x2C2, 1, 1, 0x118, 0x2EC, 5, &placePoints1_3Point1 };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point3 = { 0x2C2, 1, 4, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_4Point2 = { 0x2C2, 1, 6, 0x358, 252, 1, &placePoints1_4Point3 };
StagePoint placePoints1_4Point1 = { 0x2C2, 1, 3, 0x120, 0x100, 7, &placePoints1_4Point2 };
StagePoint placePoints1_4Point0 = { 0x2C2, 1, 2, 0x118, 0x2EC, 5, &placePoints1_4Point1 };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints1_5Point3 = { 0x2C2, 1, 6, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_5Point2 = { 0x2C2, 1, 7, 0x358, 252, 1, &placePoints1_5Point3 };
StagePoint placePoints1_5Point1 = { 0x2C2, 1, 5, 0x120, 0x100, 7, &placePoints1_5Point2 };
StagePoint placePoints1_5Point0 = { 0x2C2, 1, 3, 0x118, 0x2EC, 5, &placePoints1_5Point1 };
StagePoints placePoints1_5 = { 1, 5, &placePoints1_5Point0 };
StagePoint placePoints1_6Point3 = { 0x2C3, 0, 0, 0x2F8, 0x244, 3, NULL };
StagePoint placePoints1_6Point2 = { 0x2C2, 1, 8, 0x358, 252, 1, &placePoints1_6Point3 };
StagePoint placePoints1_6Point1 = { 0x2C2, 1, 6, 0x120, 0x100, 7, &placePoints1_6Point2 };
StagePoint placePoints1_6Point0 = { 0x2C2, 1, 4, 0x118, 0x2EC, 5, &placePoints1_6Point1 };
StagePoints placePoints1_6 = { 1, 6, &placePoints1_6Point0 };
StagePoint placePoints1_7Point3 = { 0x2C2, 1, 7, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_7Point2 = { 0x2C2, 1, 1, 0x358, 252, 1, &placePoints1_7Point3 };
StagePoint placePoints1_7Point1 = { 0x2C2, 1, 8, 0x120, 0x100, 7, &placePoints1_7Point2 };
StagePoint placePoints1_7Point0 = { 0x2C2, 1, 5, 0x118, 0x2EC, 5, &placePoints1_7Point1 };
StagePoints placePoints1_7 = { 1, 7, &placePoints1_7Point0 };
StagePoint placePoints1_8Point3 = { 0x2C2, 1, 8, 0x350, 0x2F8, 3, NULL };
StagePoint placePoints1_8Point2 = { 0x2C2, 1, 2, 0x358, 252, 1, &placePoints1_8Point3 };
StagePoint placePoints1_8Point1 = { 0x2C2, 1, 7, 0x120, 0x100, 7, &placePoints1_8Point2 };
StagePoint placePoints1_8Point0 = { 0x2C2, 1, 6, 0x118, 0x2EC, 5, &placePoints1_8Point1 };
StagePoints placePoints1_8 = { 1, 8, &placePoints1_8Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints1_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints1_5, &placePoints1_6, &placePoints1_7, &placePoints1_8,
    &placePoints0_0, NULL,
};
AreaBattle area0Battle0 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 162, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 162, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 162, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 162, 5, MUSIC(2, 0) };
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
    { 102, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x1A0, 0x88, 0xA0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x158, 0x1A0, 0x60, 0xA0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x162, 0x1C0, 0x88, 0xC0, 0x160, 0x1FF },
};
u16 actor1Talk0Conditions[] = { ITEM(3, 0x82), 1, CODES_END };
u16 actor1Talk1Conditions[] = { ITEM(3, 0x82), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk2Conditions[] = { ITEM(3, 0x82), 0, FLAG(0, 0), 1, ITEM(2, 0x7E), 0, CODES_END };
u16 actor1Talk3Conditions[] = { FLAG(0, 0), 1, ITEM(2, 0x7E), 1, ITEM(3, 0x82), 0, CODES_END };
u16 actor1Talk3Actions[] = {
    ITEM(3, 0x82), 1,
    ITEM(3, 0x81), 0,
    ITEM(2, 0x7E), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x3C9 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x350 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x351 },
    { actor1Talk2Conditions, NULL, 0x352 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x353 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3CA },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x1E), 1, CODES_END };
u16 actor1Conditions[] = {
    SPECIAL(0x43), 1,
    SPECIAL(0x4B), 1,
    ITEM(3, 0x81), 1,
    ITEM(3, 0x82), 0,
    WARP_ARG(0), 1,
    WARP_ARG(0x1F), 1,
    CODES_END,
};
u16 actor2Conditions[] = { WARP_ARG(0), 1, WARP_ARG(0x23), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x40, 4, 408, 364, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0xA7, 5, 480, 696, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xB5, 6, 480, 696, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 7, 0, 0, 0, 0, 0, 568, 143, 0, 0 },
    { 1, 0, 0x40, 2, 7, 0, 0, 0, 0, 0, 904, 503, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 417, 278, 329, 0 },
    { 1, 0, 0x48, 4, 1, 0, 0, 0, 0, 0, 348, 278, 342, 0 },
    { 1, 0, 0x4A, 4, 2, 0, 0, 0, 0, 0, 529, 611, 680, 0 },
    { 1, 0, 0x5A, 4, 3, 0, 0, 0, 0, 0, 422, 570, 656, 0 },
    { 1, 0, 0x58, 4, 4, 0, 0, 0, 0, 0, 384, 635, 719, 0 },
    { 1, 0, 0x64, 4, 5, 0, 0, 0, 0, 0, 918, 517, 608, 0 },
    { 1, 0, 0x64, 4, 6, 0, 0, 0, 0, 0, 582, 156, 248, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x340, 0xF0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x368, 0x2EC, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x128, 0x2F4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x110, 0x108, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
