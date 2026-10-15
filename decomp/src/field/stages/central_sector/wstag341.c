/*
 * WSTAG341: Shell Beach, Central Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG340.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x565
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x575
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
    FIELDSTG_state.start = (Vec2){0x1F500, 0x10300};
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

AreaBattle area0Battle0 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 126, 1, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 179, 2, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 179, 2, MUSIC(2, 0) };
BattleList area1Battles = {
    3,
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
    { 94, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x15C, 0x19F, 0x70, 0x9F, 0x150, 0x1FF },
    { 0x140, 0x100, 0x164, 0x1A7, 0x90, 0xA7, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x1A7, 0xB0, 0xA7, 0x170, 0x1FF },
    { 0x140, 0x100, 0x174, 0x1A7, 0xD0, 0xA7, 0x140, 0x1FE },
    { 0x140, 0x100, 0x176, 0x151, 0xD8, 0x51, 0x150, 0x1FE },
    { 0x180, 0x100, 0x18A, 0x100, 0x128, 0, 0x160, 0x1FE },
    { 0x180, 0x100, 0x192, 0x100, 0x148, 0, 0x170, 0x1FE },
    { 0x180, 0x100, 0x19A, 0x100, 0x168, 0, 0x140, 0x1FD },
    { 0x180, 0x100, 0x1A2, 0x100, 0x188, 0, 0x150, 0x1FD },
    { 0x140, 0x100, 0x15C, 0x177, 0x70, 0x77, 0x160, 0x1FD },
};
u16 actor12Talk0Conditions[] = { ITEM(3, 0x9A), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(3, 0x9A), 0, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x96), 0, CODES_END };
u16 actor12Talk3Conditions[] = { FLAG(0, 0), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x96), 1, CODES_END };
u16 actor12Talk3Actions[] = {
    ITEM(3, 0x9A), 1,
    ITEM(3, 0x99), 0,
    ITEM(2, 0x96), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x38 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x3A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x3C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x3B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x35 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x32 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x37 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x36 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x31 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x33 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x2EE },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x2EF },
    { actor12Talk2Conditions, NULL, 0x2F0 },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x2F1 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = {
    SPECIAL(0x45), 1,
    SPECIAL(0x4D), 1,
    ITEM(3, 0x99), 1,
    ITEM(3, 0x9A), 0,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x32, 4, 299, 282, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x34, 5, 299, 282, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x36, 6, 288, 424, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x38, 7, 481, 520, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 8, 288, 424, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3A, 9, 481, 520, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 0xA, 299, 282, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 0xA, 299, 282, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9E, 0xB, 288, 424, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9E, 0xB, 288, 424, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9F, 0xC, 481, 520, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9F, 0xC, 481, 520, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xA9, 0xD, 640, 593, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x50, 2, 5, 0, 0, 0, 0, 0, 392, 90, 0, 0 },
    { 1, 0, 0x50, 2, 5, 0, 0, 0, 0, 0, 696, 287, 0, 0 },
    { 1, 0, 0x50, 2, 0xB, 0, 0, 0, 0, 0, 688, 97, 0, 0 },
    { 1, 0, 0x72, 2, 0xC, 0, 0, 0, 0, 0, 784, 47, 0, 0 },
    { 1, 0, 0x80, 2, 0xD, 0, 0, 0, 0, 0, 640, 128, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 98, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 257, 640, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 642, 721, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 307, 524, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 457, 693, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 794, 50, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 496, 558, 575, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 509, 517, 543, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 514, 471, 489, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 276, 499, 511, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 676, 390, 407, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 224, 271, 271, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 239, 239, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 320, 255, 255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 390, 243, 243, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 247, 247, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 231, 231, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 343, 343, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 656, 375, 375, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28C, 0x102, 0x3AC, 5, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 0xE, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x3C, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x40, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
