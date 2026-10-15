/*
 * WSTAG290: Basement Stairs, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define STAGE_FILE 0x29D
#define STAGE_ARCHIVE 0x31F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define STAGE_FILE 0x2AC
#define STAGE_ARCHIVE 0x32E
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0003 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1E000, 0x1D400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 6;
    FIELDSTG_state.music = MUSIC(6, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area0Battles = {
    0,
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
AreaBattle area3Battle0 = { 189, 15, MUSIC(2, 0) };
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
    { 135, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x1B9, 0xD0, 0xB9, 0x140, 0x1F1 },
    { 0x180, 0x100, 0x18C, 0x100, 0x130, 0, 0x150, 0x1F1 },
    { 0x180, 0x100, 0x194, 0x100, 0x150, 0, 0x170, 0x1F1 },
};
u16 actor0Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 8), 1, CODES_END };
u16 actor1Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 9), 1, CODES_END };
u16 actor2Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0xA), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 8), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 9), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x132, 4, 209, 353, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x133, 5, 419, 250, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x134, 6, 544, 249, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x40, 2, 0, 5, 8, 0, 171, 28, 0, 0 },
    { 1, 0, 0x40, 2, 0x40, 2, 0, 5, 8, 0, 549, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 5, 8, 0, 264, 45, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 5, 8, 0, 361, 94, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 5, 8, 0, 614, 108, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 5, 8, 0, 699, 185, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 0xD, 0x10, 0, 467, 144, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 0xD, 0x10, 0, 467, 144, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 0xD, 0x10, 0, 676, 280, 0, 0 },
    { 1, 0, 0x40, 2, 0x35, 2, 0, 0xD, 0x10, 0, 676, 280, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 8, 0x10, 0, 81, 194, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 8, 0x10, 0, 81, 194, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 2, 0, 8, 0x10, 0, 789, 254, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 8, 0x10, 0, 789, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 166, 188, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 262, 236, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 294, 252, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 326, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 358, 284, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 390, 300, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 6, 0, 421, 316, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 2, 0, 3, 6, 0, 197, 203, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 2, 0, 3, 6, 0, 229, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 3, 6, 0, 451, 328, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 3, 6, 0, 487, 328, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 3, 6, 0, 517, 321, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 2, 0, 3, 6, 0, 549, 305, 0, 0 },
    { 1, 0x64, 0x40, 6, 5, 0, 0, 0, 0, 0, 81, 237, 0, 0 },
    { 1, 0x65, 0x40, 6, 6, 0, 0, 0, 0, 0, 161, 70, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 114, 71, 123, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 59, 239, 290, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 572, 463, 490, 0 },
    { 1, 0, 0x68, 4, 3, 0, 0, 0, 0, 0, 656, 281, 368, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 209, 283, 336, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x214, 0x386, 0x34A, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21A, 0x3A8, 0x13C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x205, 0x368, 0x18C, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
