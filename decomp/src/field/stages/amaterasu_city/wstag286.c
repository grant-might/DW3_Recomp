/*
 * WSTAG286: Admin Center 1F, Amaterasu City. On the Amaterasu server; its
 * Asuka server twin is WSTAG285.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x50F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x51F
#endif
/*
 * Sets the stage up: its map, actors and battles, playing KANRIBGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1F300, 0x27F00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
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
AreaBattle area3Battle0 = { 196, 15, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 197, 15, MUSIC(2, 0) };
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
    { 145, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x170, 0x130, 0xC0, 0x30, 0x160, 0x1FE },
    { 0x140, 0x100, 0x172, 0x1D9, 0xC8, 0xD9, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1B6, 0x178, 0x1D8, 0x78, 0x150, 0x1FD },
    { 0x140, 0x100, 0x172, 0x1B1, 0xC8, 0xB1, 0x160, 0x1FD },
    { 0x180, 0x100, 0x1B2, 0x100, 0x1C8, 0, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1B2, 0x128, 0x1C8, 0x28, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1B6, 0x150, 0x1D8, 0x50, 0x160, 0x1FC },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x5F), 1, ITEM(1, 0x2D), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0xC, 0x26), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0xC, 0x27), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0xC, 0x27), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0xC, 0x25), 1, EVENT_BATTLE(0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xA3 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xA5 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xA6 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x1BC },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x1BE },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x1BF },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0x1BD },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x5F), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0xC, 0x26), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(0xC, 0x27), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0xC, 0x27), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0xC, 0x25), 0, PROGRESS(0x25), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 825, 285, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x25, 5, 560, 673, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x26, 6, 371, 574, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x9D, 7, 560, 673, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9E, 8, 296, 532, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x12B, 9, 823, 819, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x12D, 0xA, 371, 574, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x12E, 0xB, 349, 586, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x12F, 0xC, 438, 244, 1 };
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
    { 1, 0, 0x40, 2, 0x36, 2, 0, 5, 8, 0, 437, 121, 0, 0 },
    { 1, 0, 0x78, 2, 0x32, 2, 0, 1, 4, 0, 121, 400, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 104, 383, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 212, 501, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 264, 240, 0, 0 },
    { 1, 0, 0x78, 6, 0x33, 2, 0, 1, 4, 0, 116, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 104, 334, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 212, 215, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 263, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 76, 291, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 152, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 203, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 284, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 748, 208, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 269, 421, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 348, 461, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 429, 501, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 509, 540, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 517, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 597, 201, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 653, 613, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 733, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 813, 693, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 837, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 893, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 2, 0, 5, 8, 0, 554, 542, 0, 0 },
    { 1, 0x64, 0x40, 6, 0xD, 0, 0, 0, 0, 0, 384, 133, 0, 0 },
    { 1, 0, 0x78, 4, 0x32, 2, 0, 1, 4, 0, 172, 373, 385, 0 },
    { 1, 0, 0x40, 4, 0x34, 2, 0, 1, 4, 0, 156, 358, 385, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 833, 737, 808, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 850, 744, 800, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 753, 706, 752, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 672, 666, 712, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 448, 554, 600, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 368, 514, 560, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 288, 474, 520, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 174, 336, 385, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 608, 250, 296, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 528, 210, 256, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 449, 161, 233, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 466, 168, 224, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 408, 135, 186, 0 },
    { 1, 0, 0x78, 4, 0x33, 2, 0, 1, 4, 0, 167, 255, 385, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x286, 0x318, 0x1DC, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0x14A, 0xC4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x284, 0xC8, 0x7C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
