/*
 * WSTAG695: Snow Mountain, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x75F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x76E
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0007 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x33200, 0xD500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2F;
    FIELDSTG_state.music = MUSIC(0x2F, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 113, 14, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 113, 14, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 113, 14, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 113, 14, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 114, 14, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 114, 14, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 114, 14, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 114, 14, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 218, 14, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 333, 14, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 114, 14, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 82, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x156, 0x1C8, 0x58, 0xC8, 0x170, 0x1FD },
    { 0x140, 0x100, 0x176, 0x188, 0xD8, 0x88, 0x170, 0x1FC },
    { 0x140, 0x100, 0x176, 0x1D0, 0xD8, 0xD0, 0x160, 0x1FB },
    { 0x140, 0x100, 0x176, 0x1B0, 0xD8, 0xB0, 0x170, 0x1FB },
};
u16 actor0Talk0Actions[] = { SPECIAL(0x90), 1, FLAG(2, 0x22), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { PARTY_STAT(6), 0, FLAG(0, 0), 1, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(0, 0x14), 0, PARTY_STAT(6), 1, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x1C, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 0,
    CODES_END,
};
u16 actor1Talk4Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x12), 1, CODES_END };
u16 actor1Talk5Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(0x1C, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(0, 0x14), 0, FLAG(0, 0), 1, PARTY_STAT(6), 1, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x1C, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor3Talk4Conditions[] = {
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 0,
    CODES_END,
};
u16 actor3Talk4Actions[] = { FLAG(0xE, 0x12), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor3Talk5Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(0x1C, 1), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(2, 0x23), 1, ITEM(4, 0xA0), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(0x1C, 0), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor6Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x12), 0,
    CODES_END,
};
u16 actor6Talk4Actions[] = { FLAG(0xE, 0x12), 1, CODES_END };
u16 actor6Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor6Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x12), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor6Talk6Actions[] = { CARD_BATTLE(0x1C, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x32E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0xD },
    { actor1Talk1Conditions, NULL, 0x11 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x12 },
    { actor1Talk3Conditions, NULL, 0x13 },
    { actor1Talk4Conditions, actor1Talk4Actions, 0x14 },
    { actor1Talk5Conditions, NULL, 0x15 },
    { actor1Talk6Conditions, actor1Talk6Actions, 0x16 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0xD },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x17 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x18 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0xE },
    { actor3Talk1Conditions, NULL, 0x11 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x12 },
    { actor3Talk3Conditions, NULL, 0x13 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x14 },
    { actor3Talk5Conditions, NULL, 0x15 },
    { actor3Talk6Conditions, actor3Talk6Actions, 0x16 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x295 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x268 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0xF },
    { actor6Talk1Conditions, NULL, 0xF },
    { actor6Talk2Conditions, actor6Talk2Actions, 0xF },
    { actor6Talk3Conditions, NULL, 0xF },
    { actor6Talk4Conditions, actor6Talk4Actions, 0xF },
    { actor6Talk5Conditions, NULL, 0xF },
    { actor6Talk6Conditions, actor6Talk6Actions, 0xF },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x22), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(0, 0x11), 0, SPECIAL(4), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, SPECIAL(9), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor5Conditions[] = { FLAG(2, 0x23), 0, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 384, 453, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x34, 5, 657, 408, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x34, 5, 657, 408, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x34, 5, 657, 408, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x34, 5, 657, 408, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x4D, 6, 961, 241, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 7, 657, 408, 1 };
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
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 6, 0, 472, 329, 0, 0 },
    { 1, 0, 0xC8, 2, 0x34, 1, 0x34, 0x37, 8, 0, 542, 52, 0, 0 },
    { 1, 0, 0x40, 2, 0x3B, 2, 0, 3, 8, 0, 412, 670, 0, 0 },
    { 1, 0, 0x40, 2, 2, 1, 2, 7, 4, 0, 93, 305, 0, 0 },
    { 1, 0, 0x40, 2, 2, 1, 2, 7, 4, 0, 585, 772, 0, 0 },
    { 1, 0, 0x40, 2, 2, 1, 2, 7, 4, 0, 1033, 514, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xD, 6, 0, 579, 336, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 3, 8, 0, 557, 494, 0, 0 },
    { 1, 0, 0x50, 6, 0x39, 2, 0, 3, 8, 0, 549, 528, 0, 0 },
    { 1, 0, 0x50, 6, 0x3A, 2, 0, 3, 8, 0, 493, 598, 0, 0 },
    { 1, 0x64, 0x40, 6, 1, 0, 0, 0, 0, 0, 546, 340, 0, 0 },
    { 1, 0, 0x48, 4, 0, 0, 0, 0, 0, 0, 517, 329, 393, 0 },
    { 1, 0, 0x53, 4, 8, 0, 0, 0, 0, 0, 448, 290, 368, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 192, 783, 783, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 400, 263, 263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 672, 175, 175, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 720, 199, 199, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x261, 0x500, 0x290, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x261, 0x500, 0x300, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x264, 0x238, 0x444, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x263, 0x108, 0x12C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x1CE, 0xAA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x1BF, 0x112, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x3B0, 0x16A, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x3BF, 0x1D0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x380, 0x253, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x38F, 0x2EA, 0, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x13, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 7, 2 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
