/*
 * WSTAG346: Plug Cape, Central Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG345.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x569
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x579
#endif
/*
 * Sets the stage up: its map, actors and battles, playing FIELDBGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xCA00, 0x8200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 9;
    FIELDSTG_state.music = MUSIC(9, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

AreaBattle area0Battle0 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 93, 13, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 98, 4, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 98, 4, MUSIC(2, 0) };
BattleList area1Battles = {
    3,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle1 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle2 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle3 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle4 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle5 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle6 = { 179, 2, MUSIC(2, 0) };
AreaBattle area2Battle7 = { 179, 2, MUSIC(2, 0) };
BattleList area2Battles = {
    3,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 329, 13, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 330, 2, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 93, 13, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 180, 2, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 95, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14A, 0x173, 0x28, 0x73, 0x150, 0x1FF },
    { 0x140, 0x100, 0x172, 0x167, 0xC8, 0x67, 0x160, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x16A, 0x68, 0x6A, 0x170, 0x1FF },
    { 0x140, 0x100, 0x152, 0x173, 0x48, 0x73, 0x140, 0x1FE },
    { 0x140, 0x100, 0x162, 0x182, 0x88, 0x82, 0x150, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x182, 0xA8, 0x82, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x18B, 0, 0x8B, 0x170, 0x1FE },
    { 0x140, 0x100, 0x172, 0x18F, 0xC8, 0x8F, 0x140, 0x1FD },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x44 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x48 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3E },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x46 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x47 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x41 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x40 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x42 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x43 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x45 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x3D },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x3F },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2E, 4, 657, 648, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 4, 657, 648, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x33, 5, 316, 322, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 6, 316, 322, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 7, 677, 303, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3A, 8, 641, 161, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 9, 641, 161, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 9, 641, 161, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9E, 0xA, 657, 648, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9E, 0xA, 657, 648, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9F, 0xB, 316, 322, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9F, 0xB, 316, 322, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 328, 528, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 920, 952, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 149, 246, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 343, 123, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 413, 862, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 91, 124, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 228, 455, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 592, 982, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 674, 776, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 865, 514, 0, 0 },
    { 1, 0, 0xFF, 6, 8, 0, 0, 0, 0, 0, 703, 232, 0, 0 },
    { 1, 0, 0x80, 4, 9, 0, 0, 0, 0, 0, 631, 378, 433, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 593, 792, 804, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 623, 849, 846, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 333, 235, 235, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 381, 259, 259, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 429, 283, 283, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 481, 532, 532, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 486, 775, 775, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 557, 557, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 533, 751, 751, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 541, 699, 699, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 574, 580, 580, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1014, 879, 879, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1054, 899, 899, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28C, 0x1D2, 0x92, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 0xC, 0x231, 0xB0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 0xC, 0x221, 0x178, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 0xD, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 9, 3 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x1D, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x50, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFA2, 0x10, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x28, 0x20, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x38, 0x29, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 0xD, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
