/*
 * WSTAG491: Ether Jungle, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG490.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5AA
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5BA
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0013 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x3A900, 0x37D00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x35;
    FIELDSTG_state.music = MUSIC(0x35, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 109, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 109, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 176, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 176, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 95, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 95, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 95, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 95, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle3 = { 331, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 332, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 177, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 106, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 76, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1EC, 0x100, 0x2B0, 0, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1F4, 0x100, 0x2D0, 0, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1E2, 0x118, 0x288, 0x18, 0x170, 0x1FE },
    { 0x1C0, 0x100, 0x1EA, 0x128, 0x2A8, 0x28, 0x150, 0x1FD },
    { 0x1C0, 0x100, 0x1C8, 0x118, 0x220, 0x18, 0x160, 0x1FD },
};
u16 actor6Talk0Conditions[] = { ITEM(3, 0x67), 1, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk2Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 1, ITEM(2, 0x63), 0, CODES_END };
u16 actor6Talk3Conditions[] = { ITEM(3, 0x67), 0, FLAG(0, 0), 1, ITEM(2, 0x63), 1, CODES_END };
u16 actor6Talk3Actions[] = {
    ITEM(3, 0x67), 1,
    ITEM(3, 0x66), 0,
    ITEM(2, 0x63), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x104 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x107 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x106 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x108 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x103 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x2DE },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x2DF },
    { actor6Talk2Conditions, NULL, 0x2E0 },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x2E1 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor6Conditions[] = {
    SPECIAL(0x42), 1,
    SPECIAL(0x4A), 1,
    ITEM(3, 0x66), 1,
    ITEM(3, 0x67), 0,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2F, 4, 561, 458, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x37, 5, 1049, 365, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x9D, 6, 1049, 365, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x9D, 6, 1049, 365, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9E, 7, 561, 458, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9E, 7, 561, 458, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xA6, 8, 449, 1034, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x70, 2, 0xA, 0, 0, 0, 0, 0, 896, 548, 0, 0 },
    { 1, 0, 0x78, 2, 0xB, 0, 0, 0, 0, 0, 768, 265, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 323, 130, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 548, 651, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 1011, 794, 0, 0 },
    { 1, 0, 0x40, 6, 0x62, 2, 0, 0xD, 0xA, 0, 902, 118, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 324, 437, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 480, 1007, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 1091, 911, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 259, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 452, 984, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 1034, 904, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 1, 0x34, 0x43, 0xA, 0, 121, 863, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x4E, 0xA, 0, 231, 1080, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x4E, 0xA, 0, 643, 1068, 0, 0 },
    { 1, 0, 0x64, 6, 0x14, 0, 0, 0, 0, 0, 177, 889, 0, 0 },
    { 1, 0, 0x64, 6, 0x15, 0, 0, 0, 0, 0, 31, 789, 0, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 103, 696, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 287, 790, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 319, 634, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 331, 860, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 425, 935, 1152, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 566, 586, 673, 0 },
    { 1, 0, 0x50, 4, 1, 0, 0, 0, 0, 0, 1013, 384, 452, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 959, 427, 502, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 310, 860, 900, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 448, 513, 569, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 592, 744, 782, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 992, 151, 200, 0 },
    { 1, 0, 0x64, 4, 0x12, 0, 0, 0, 0, 0, 445, 977, 1021, 0 },
    { 1, 0, 0x64, 4, 0x13, 0, 0, 0, 0, 0, 896, 756, 810, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 80, 759, 759, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 224, 191, 191, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 223, 223, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 247, 247, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 368, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 447, 647, 647, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 439, 439, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 615, 615, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 496, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 512, 559, 559, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 613, 879, 879, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 624, 839, 839, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 640, 447, 447, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 487, 487, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 887, 887, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 704, 543, 543, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 752, 935, 935, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 871, 871, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 816, 823, 823, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AA, 0x372, 0x272, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AF, 0x90, 0x128, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A8, 0xA0, 0x80, 7, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 4, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 8, 2 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0xE, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x48, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x20, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0xE, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
