/*
 * WSTAG540: Bug Maze, Network Break.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x6E9
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x6F9
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0017 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x15200, 0x1C000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x13;
    FIELDSTG_state.music = MUSIC(0x13, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 165, 10, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 165, 10, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 173, 10, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 173, 10, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 92, 10, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
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
    { 60, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x1D8, 0xD0, 0xD8, 0x140, 0x1FA },
    { 0x180, 0x100, 0x1A0, 0x168, 0x180, 0x68, 0x150, 0x1FA },
    { 0x180, 0x100, 0x1A6, 0x168, 0x198, 0x68, 0x160, 0x1FA },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x18), 1, ITEM(5, 0xFC), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x19), 1, ITEM(1, 0x2B), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor2Talk0Actions[] = { SPECIAL(0x8F), 1, FLAG(2, 0x1A), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x261 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x16D },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x32D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x18), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(2, 0x19), 0, CODES_END };
u16 actor2Conditions[] = { FLAG(2, 0x1A), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 369, 1218, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x4D, 5, 1072, 601, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x4E, 6, 1313, 1138, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x64, 2, 0xD, 0, 0, 0, 0, 0, 358, 319, 0, 0 },
    { 1, 0, 0x64, 2, 0xE, 0, 0, 0, 0, 0, 508, 332, 0, 0 },
    { 1, 0, 0x64, 2, 0xF, 0, 0, 0, 0, 0, 887, 399, 0, 0 },
    { 1, 0, 0x64, 2, 0xF, 0, 0, 0, 0, 0, 1007, 403, 0, 0 },
    { 1, 0, 0x64, 2, 0x10, 0, 0, 0, 0, 0, 800, 411, 0, 0 },
    { 1, 0, 0x64, 2, 0x10, 0, 0, 0, 0, 0, 1159, 424, 0, 0 },
    { 1, 0, 0x64, 2, 0x11, 0, 0, 0, 0, 0, 1126, 413, 0, 0 },
    { 1, 0, 0x50, 2, 0x12, 0, 0, 0, 0, 0, 1266, 453, 0, 0 },
    { 1, 0, 0xDC, 2, 0x13, 0, 0, 0, 0, 0, 1360, 483, 0, 0 },
    { 1, 0, 0x64, 2, 0, 0, 0, 0, 0, 0, 502, 965, 0, 0 },
    { 1, 0, 0x64, 2, 0, 0, 0, 0, 0, 0, 619, 747, 0, 0 },
    { 1, 0, 0x64, 2, 0, 0, 0, 0, 0, 0, 668, 882, 0, 0 },
    { 1, 0, 0x64, 2, 1, 0, 0, 0, 0, 0, 368, 1064, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 514, 921, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 538, 877, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 561, 834, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 631, 702, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 680, 838, 0, 0 },
    { 1, 0, 0x64, 2, 2, 0, 0, 0, 0, 0, 704, 794, 0, 0 },
    { 1, 0, 0x64, 2, 3, 0, 0, 0, 0, 0, 624, 866, 0, 0 },
    { 1, 0, 0x64, 2, 3, 0, 0, 0, 0, 0, 647, 822, 0, 0 },
    { 1, 0, 0x64, 2, 3, 0, 0, 0, 0, 0, 671, 779, 0, 0 },
    { 1, 0, 0x64, 2, 4, 0, 0, 0, 0, 0, 593, 850, 0, 0 },
    { 1, 0, 0x64, 2, 4, 0, 0, 0, 0, 0, 617, 806, 0, 0 },
    { 1, 0, 0x64, 2, 4, 0, 0, 0, 0, 0, 640, 762, 0, 0 },
    { 1, 0, 0x64, 2, 4, 0, 0, 0, 0, 0, 663, 719, 0, 0 },
    { 1, 0, 0xA0, 2, 5, 0, 0, 0, 0, 0, 687, 683, 0, 0 },
    { 1, 0, 0x8C, 2, 6, 0, 0, 0, 0, 0, 847, 715, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 939, 866, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 1035, 914, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 1155, 918, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 1297, 879, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 1394, 927, 0, 0 },
    { 1, 0, 0x64, 2, 7, 0, 0, 0, 0, 0, 1513, 931, 0, 0 },
    { 1, 0, 0x64, 2, 8, 0, 0, 0, 0, 0, 932, 807, 0, 0 },
    { 1, 0, 0x64, 2, 8, 0, 0, 0, 0, 0, 1004, 899, 0, 0 },
    { 1, 0, 0x64, 2, 8, 0, 0, 0, 0, 0, 1362, 912, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 920, 828, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 1111, 924, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 1255, 885, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 1351, 933, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 1470, 937, 0, 0 },
    { 1, 0, 0x64, 2, 0xA, 0, 0, 0, 0, 0, 1199, 891, 0, 0 },
    { 1, 0, 0x64, 2, 0xB, 0, 0, 0, 0, 0, 1226, 864, 0, 0 },
    { 1, 0, 0x64, 2, 0xC, 0, 0, 0, 0, 0, 521, 415, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x244, 0x40C, 0x1E4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x246, 0x98, 0xF4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x246, 0x98, 0x1E4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x246, 0x98, 0x2C4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x246, 0x98, 0x414, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x220, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x210, 0x1F8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x293, 0x98, 0x264, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
