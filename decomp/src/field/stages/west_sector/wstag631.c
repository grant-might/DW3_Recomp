/*
 * WSTAG631: S Noise Desert, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG630.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x60F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x61F
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x16000, 0x23100};
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

AreaBattle area0Battle0 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 72, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 72, 5, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 238, 5, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 286, 5, MUSIC(3, 0) };
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
    { 101, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x100, 0x88, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x16A, 0x100, 0xA8, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x172, 0x100, 0xC8, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x162, 0x128, 0x88, 0x28, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16A, 0x128, 0xA8, 0x28, 0x140, 0x1FE },
    { 0x140, 0x100, 0x172, 0x128, 0xC8, 0x28, 0x150, 0x1FE },
    { 0x140, 0x100, 0x162, 0x150, 0x88, 0x50, 0x160, 0x1FE },
    { 0x140, 0x100, 0x172, 0x178, 0xC8, 0x78, 0x170, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x150, 0xA8, 0x50, 0x140, 0x1FD },
    { 0x140, 0x100, 0x172, 0x150, 0xC8, 0x50, 0x150, 0x1FD },
    { 0x140, 0x100, 0x162, 0x178, 0x88, 0x78, 0x160, 0x1FD },
    { 0x140, 0x100, 0x16A, 0x178, 0xA8, 0x78, 0x170, 0x1FD },
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x36, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x29), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x29), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x29), 1,
    CODES_END,
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, FLAG(0xE, 0x4A), 0, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x4A), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x4A), 1,
    PARTY_STAT(0xC), 0,
    CODES_END,
};
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x4A), 1,
    PARTY_STAT(0xC), 1,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x36, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x1C6 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x244 },
    { actor1Talk1Conditions, NULL, 0x245 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x246 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x247 },
    { actor1Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x24C },
    { actor2Talk1Conditions, NULL, 0x24E },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x24D },
    { actor2Talk3Conditions, NULL, 0x248 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x39B },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x249 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x32B },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x32C },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x32D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 0,
    SPECIAL(0x19), 1,
    CODES_END,
};
u16 actor2Conditions[] = {
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor3Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x25), 0, PROGRESS(0x26), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x32, 4, 752, 289, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 432, 273, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 432, 273, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 432, 273, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 432, 273, 7 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x46, 6, 143, 672, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x47, 7, 177, 689, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x48, 8, 207, 657, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x49, 9, 232, 620, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x4A, 0xA, 263, 628, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 0xB, 752, 289, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 0xB, 752, 289, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0xA4, 0xC, 280, 645, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x132, 0xD, 272, 601, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x133, 0xE, 303, 607, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x134, 0xF, 321, 624, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 541, 275, 326, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 364, 459, 509, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B2, 0x90, 0x540, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C1, 0x340, 0xF0, 1, 0, 1, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
