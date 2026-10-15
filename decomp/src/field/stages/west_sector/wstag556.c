/*
 * WSTAG556: Noise Desert, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG555.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5D3
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5E3
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10000, 0x50300};
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

AreaBattle area0Battle0 = { 134, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 134, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 134, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 134, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 172, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 172, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 172, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 172, 5, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 235, 5, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 283, 5, MUSIC(3, 0) };
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
    { 99, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x158, 0x16A, 0x60, 0x6A, 0x140, 0x1FF },
    { 0x140, 0x100, 0x160, 0x16A, 0x80, 0x6A, 0x150, 0x1FF },
    { 0x180, 0x100, 0x190, 0x100, 0x140, 0, 0x160, 0x1FF },
    { 0x180, 0x100, 0x198, 0x100, 0x160, 0, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x140, 0x1FE },
    { 0x180, 0x100, 0x190, 0x128, 0x140, 0x28, 0x150, 0x1FE },
    { 0x180, 0x100, 0x198, 0x128, 0x160, 0x28, 0x160, 0x1FE },
    { 0x180, 0x100, 0x1A0, 0x128, 0x180, 0x28, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1A8, 0x128, 0x1A0, 0x28, 0x140, 0x1FD },
    { 0x180, 0x100, 0x1A8, 0x100, 0x1A0, 0, 0x150, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x100, 0x1C0, 0, 0x160, 0x1FD },
};
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x33, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x26), 0,
    CODES_END,
};
u16 actor4Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x26), 1, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x26), 1,
    CODES_END,
};
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, FLAG(0xE, 0x48), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0xE, 0x48), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x48), 1,
    PARTY_STAT(0xC), 0,
    CODES_END,
};
u16 actor7Talk4Conditions[] = {
    PARTY_STAT(0xC), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x48), 1,
    CODES_END,
};
u16 actor7Talk4Actions[] = { CARD_BATTLE(0x33, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x198 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x196 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x19C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x19A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x244 },
    { actor4Talk1Conditions, NULL, 0x245 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x246 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x247 },
    { actor4Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x39B },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x249 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x24C },
    { actor7Talk1Conditions, NULL, 0x24E },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x24D },
    { actor7Talk3Conditions, NULL, 0x248 },
    { actor7Talk4Conditions, actor7Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x19D },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x19D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x19B },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x19B },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x199 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x199 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x197 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x197 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x328 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x329 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor4Conditions[] = {
    SPECIAL(0x19), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor5Conditions[] = { SPECIAL(0x19), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor7Conditions[] = {
    ITEM(0, 0x14), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x19), 1,
    CODES_END,
};
u16 actor8Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x20), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x20), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2F, 4, 720, 777, 5 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x31, 5, 992, 1209, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x36, 6, 1072, 433, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x37, 7, 320, 769, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 8, 401, 577, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 8, 401, 577, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x45, 8, 401, 577, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x45, 8, 401, 577, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 9, 1072, 433, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 9, 1072, 433, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9E, 0xA, 320, 769, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 0xA, 320, 769, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9F, 0xB, 720, 777, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9F, 0xB, 720, 777, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xA0, 0xC, 992, 1209, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xA0, 0xC, 992, 1209, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x132, 0xD, 411, 158, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x133, 0xE, 389, 147, 1 };
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
    &actor13,
    &actor14,
    &actor15,
    &actor16,
    &actor17,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 6, 0, 0, 0, 0, 0, 874, 1273, 0, 0 },
    { 1, 0, 0x80, 6, 7, 0, 0, 0, 0, 0, 856, 1221, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 506, 475, 544, 0 },
    { 1, 0, 0x50, 4, 1, 0, 0, 0, 0, 0, 317, 423, 488, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 225, 615, 666, 0 },
    { 1, 0, 0x48, 4, 3, 0, 0, 0, 0, 0, 476, 647, 713, 0 },
    { 1, 0, 0x49, 4, 4, 0, 0, 0, 0, 0, 592, 815, 880, 0 },
    { 1, 0, 0x78, 4, 5, 0, 0, 0, 0, 0, 289, 127, 230, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C6, 0xC8, 0x3BC, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B3, 0x7A, 0xEA, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B1, 0xB0, 0x90, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C0, 0x320, 0x88, 1, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x1E, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 8, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
