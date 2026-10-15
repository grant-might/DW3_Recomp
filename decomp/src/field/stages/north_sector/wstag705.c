/*
 * WSTAG705: Freeze Mountain, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x639
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x649
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0022 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x4E400, 0x7B00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3E;
    FIELDSTG_state.music = MUSIC(0x3E, 0);
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
    2,
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
AreaBattle area3Battle0 = { 219, 14, MUSIC(3, 0) };
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
    { 83, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x120, 0xD0, 0x20, 0x140, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x100, 0xB0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x128, 0xB0, 0x28, 0x160, 0x1FF },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x170, 0x1FF },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x24), 1, ITEM(1, 0x2B), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x1D, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor1Talk4Conditions[] = {
    FLAG(0xE, 0x13), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor1Talk4Actions[] = { FLAG(0xE, 0x13), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor1Talk5Conditions[] = {
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(0x1D, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor4Talk2Conditions[] = { PARTY_STAT(6), 1, FLAG(0, 0), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x1D, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor4Talk4Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x13), 0,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor4Talk4Actions[] = { FLAG(0xE, 0x13), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor4Talk5Conditions[] = {
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 0,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor4Talk6Conditions[] = {
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor4Talk6Actions[] = { CARD_BATTLE(0x1D, 1), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(2, 0x25), 1, ITEM(5, 0x116), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(0x1D, 0), 1, CODES_END };
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
    FLAG(0xE, 0x13), 0,
    CODES_END,
};
u16 actor6Talk4Actions[] = { FLAG(0xE, 0x13), 1, CODES_END };
u16 actor6Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor6Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x13), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor6Talk6Actions[] = { CARD_BATTLE(0x1D, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x170 },
    { actor1Talk1Conditions, NULL, 0x174 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x175 },
    { actor1Talk3Conditions, NULL, 0x176 },
    { actor1Talk4Conditions, actor1Talk4Actions, 0x177 },
    { actor1Talk5Conditions, NULL, 0x178 },
    { actor1Talk6Conditions, actor1Talk6Actions, 0x179 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x296 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x170 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x17A },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x17B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x171 },
    { actor4Talk1Conditions, NULL, 0x174 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x175 },
    { actor4Talk3Conditions, NULL, 0x176 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x177 },
    { actor4Talk5Conditions, NULL, 0x178 },
    { actor4Talk6Conditions, actor4Talk6Actions, 0x179 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x26A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x172 },
    { actor6Talk1Conditions, NULL, 0x172 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x172 },
    { actor6Talk3Conditions, NULL, 0x172 },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x172 },
    { actor6Talk5Conditions, NULL, 0x172 },
    { actor6Talk6Conditions, actor6Talk6Actions, 0x172 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x24), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, SPECIAL(4), 1, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor3Conditions[] = { FLAG(0, 0x11), 1, SPECIAL(9), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0, 0x11), 0, PROGRESS(0x26), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(2, 0x25), 0, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 992, 273, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2F, 5, 576, 448, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2F, 5, 576, 448, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2F, 5, 576, 448, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2F, 5, 576, 448, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x4D, 6, 896, 697, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 7, 576, 448, 1 };
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
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 4, 0, 837, 766, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 4, 0, 891, 248, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 4, 0, 1001, 145, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 4, 0, 1194, 110, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26E, 0xE0, 0x3A8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x262, 0x3A0, 0xA0, 1, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x15, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0xC, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 5, 3 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0xA, 2 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x420, 0x140, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x410, 0x1D8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x460, 0x2A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x450, 0x338, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x430, 0x288, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x420, 0x2F0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x36F, 0x1D8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x37F, 0x240, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x32F, 0x288, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x33F, 0x320, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x1F0, 0x338, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x1E0, 0x3A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0xD, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0xA, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
