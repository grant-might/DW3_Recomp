/*
 * WSTAG571: North Badland E, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG570.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5CA
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5DA
#endif
/*
 * Sets the stage up: its map, actors and battles, playing BGM_0018; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xC900, 0xEB00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x14;
    FIELDSTG_state.music = MUSIC(0x14, 0);
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

AreaBattle area0Battle0 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 133, 3, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 133, 3, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 169, 3, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 169, 3, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 237, 3, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 285, 3, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 334, 2, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 180, 2, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 96, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x166, 0x190, 0x98, 0x90, 0x150, 0x1FF },
    { 0x140, 0x100, 0x176, 0x190, 0xD8, 0x90, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x190, 0xB8, 0x90, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x178, 0x68, 0x78, 0x140, 0x1FE },
    { 0x140, 0x100, 0x154, 0x1A8, 0x50, 0xA8, 0x150, 0x1FE },
    { 0x140, 0x100, 0x15C, 0x1A8, 0x70, 0xA8, 0x160, 0x1FE },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x35, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x28), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { FLAG(0xE, 0x28), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x28), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, FLAG(0xE, 0x49), 0, CODES_END };
u16 actor3Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x49), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x49), 1,
    PARTY_STAT(0xC), 0,
    CODES_END,
};
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x49), 1,
    PARTY_STAT(0xC), 1,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x35, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk0Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 0, CODES_END };
u16 actor6Talk0Actions[] = { 0x9409, 1, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor6Talk1Actions[] = { 0x940A, 1, CODES_END };
u16 actor7Talk0Actions[] = { 0x940D, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x1C0 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x1BE },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x244 },
    { actor2Talk1Conditions, NULL, 0x245 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x246 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x247 },
    { actor2Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x24C },
    { actor3Talk1Conditions, NULL, 0x24E },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24D },
    { actor3Talk3Conditions, NULL, 0x248 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x39D },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x249 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x2D9 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x2D9 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x2D9 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x1BF },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x1BF },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1C1 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1C1 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x19), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    SPECIAL(0x19), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actor7Conditions[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x32, 4, 748, 281, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x3A, 5, 537, 413, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 6, 333, 716, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 6, 333, 716, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 6, 333, 716, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 6, 333, 716, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x8B, 7, 730, 440, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x8B, 7, 730, 440, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 8, 537, 413, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 8, 537, 413, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9E, 9, 748, 281, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 9, 748, 281, 7 };
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
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 153, 305, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 875, 481, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 391, 837, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 693, 860, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 826, 679, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 290, 854, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 602, 785, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 817, 793, 0, 0 },
    { 1, 0, 0x40, 6, 2, 0, 0, 0, 0, 0, 541, 351, 0, 0 },
    { 1, 0, 0x40, 6, 3, 0, 0, 0, 0, 0, 480, 468, 0, 0 },
    { 1, 0, 0x5D, 4, 0, 0, 0, 0, 0, 0, 621, 413, 490, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B4, 0x570, 0x3C0, 3, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0xB, 2 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0xD, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x1C0, 0x272, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x1CF, 0x2D8, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFE7, 0x46, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x19, 0x46, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0xD, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
