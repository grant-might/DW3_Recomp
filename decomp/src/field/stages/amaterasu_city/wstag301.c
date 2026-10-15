/*
 * WSTAG301: Admin Center 2F, Amaterasu City. On the Amaterasu server; its
 * Asuka server twin is WSTAG300.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x424
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x434
#endif
/*
 * Sets the stage up: its map, actors and battles, playing KANRIBGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x49D00, 0x20300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
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
    { 147, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B8, 0x100, 0x1E0, 0, 0x170, 0x1F9 },
    { 0x140, 0x100, 0x172, 0x1B1, 0xC8, 0xB1, 0x170, 0x1F8 },
    { 0x180, 0x100, 0x1B0, 0x100, 0x1C0, 0, 0x170, 0x1F7 },
    { 0x180, 0x100, 0x180, 0x17F, 0x100, 0x7F, 0x170, 0x1F6 },
    { 0x180, 0x100, 0x188, 0x17F, 0x120, 0x7F, 0x150, 0x1F5 },
    { 0x180, 0x100, 0x1B0, 0x130, 0x1C0, 0x30, 0x160, 0x1F5 },
    { 0x180, 0x100, 0x1B0, 0x158, 0x1C0, 0x58, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x1A0, 0x167, 0x180, 0x67, 0x150, 0x1F4 },
    { 0x180, 0x100, 0x1A8, 0x167, 0x1A0, 0x67, 0x160, 0x1F4 },
    { 0x180, 0x100, 0x190, 0x174, 0x140, 0x74, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x198, 0x174, 0x160, 0x74, 0x150, 0x1F3 },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x60), 1, CODES_END };
u16 actor5Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x2B), 1, CODES_END };
u16 actor6Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0x30), 1, CODES_END };
u16 actor7Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0x31), 1, CODES_END };
u16 actor8Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x2C), 1, CODES_END };
u16 actor9Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x2D), 1, CODES_END };
u16 actor10Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x2E), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16F },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xAD },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xAF },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xB0 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x1C3 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x1C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x1C8 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0x1C4 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, actor9Talk0Actions, 0x1C5 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, actor10Talk0Actions, 0x1C6 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x60), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x2B), 0, CODES_END };
u16 actor6Conditions[] = { FLAG(0xC, 0x30), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0xC, 0x31), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x2C), 0, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x2D), 0, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x2E), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 700, 283, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x25, 5, 961, 217, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x26, 6, 896, 185, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x9D, 7, 961, 217, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9E, 8, 896, 185, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x12B, 9, 1120, 281, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x12D, 0xA, 992, 282, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x12E, 0xB, 769, 169, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x12F, 0xC, 936, 477, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x130, 0xD, 1225, 429, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x131, 0xE, 321, 201, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x36, 2, 0, 9, 8, 0, 918, 96, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 9, 0xA, 0, 918, 96, 0, 0 },
    { 1, 0, 0xFF, 2, 0xB, 0, 0, 0, 0, 0, 992, 126, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 166, 118, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 541, 90, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 1115, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 332, 103, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1009, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1211, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1294, 168, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 5, 8, 0, 860, 89, 0, 0 },
    { 1, 0, 0x80, 6, 0x35, 3, 0, 0xF, 0xE, 0, 660, 434, 0, 0 },
    { 1, 0x64, 0x40, 6, 0, 0, 0, 0, 0, 0, 929, 148, 0, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 768, 107, 151, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 992, 218, 264, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 1088, 218, 264, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1152, 186, 230, 0 },
    { 1, 0, 0x60, 4, 5, 0, 0, 0, 0, 0, 816, 333, 432, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 945, 441, 462, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 960, 190, 231, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 480, 321, 342, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 497, 256, 272, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 953, 144, 200, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x274, 0x158, 0xA4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x287, 0x56, 0x18A, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0xC8, 0x9C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x283, 0x177, 0xBD, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x274, 0x372, 0x132, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
