/*
 * WSTAG316: Admin Center B1F, Amaterasu City. On the Amaterasu server; its
 * Asuka server twin is WSTAG315.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x55D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x56D
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0003 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12000, 0x2B300};
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
AreaBattle area3Battle0 = { 195, 12, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 199, 12, MUSIC(2, 0) };
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
    { 149, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x175, 0xE0, 0x75, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B8, 0x100, 0x1E0, 0, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1A8, 0x118, 0x1A0, 0x18, 0x170, 0x1FD },
    { 0x140, 0x100, 0x170, 0x155, 0xC0, 0x55, 0x170, 0x1FC },
    { 0x140, 0x100, 0x170, 0x17D, 0xC0, 0x7D, 0x150, 0x1FB },
    { 0x140, 0x100, 0x158, 0x18E, 0x60, 0x8E, 0x160, 0x1FB },
    { 0x140, 0x100, 0x172, 0x1A5, 0xC8, 0xA5, 0x170, 0x1FB },
    { 0x140, 0x100, 0x172, 0x1CD, 0xC8, 0xCD, 0x150, 0x1FA },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x160, 0x1FA },
};
u16 actor0Talk0Actions[] = { SPECIAL(0x92), 1, FLAG(2, 0x61), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(2, 0x62), 1, ITEM(1, 0x2D), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 0x63), 1, CODES_END };
u16 actor3Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x34), 1, CODES_END };
u16 actor4Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0x38), 1, CODES_END };
u16 actor5Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0x39), 1, CODES_END };
u16 actor6Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x35), 1, CODES_END };
u16 actor7Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x36), 1, CODES_END };
u16 actor8Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x35), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x1F8 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x16F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x1C9 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x1CD },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x1CE },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x1CA },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x1CB },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0x1CC },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x61), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(2, 0x62), 0, CODES_END };
u16 actor2Conditions[] = { FLAG(2, 0x63), 0, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x34), 0, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x38), 0, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x39), 0, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x35), 0, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x36), 0, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x35), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 144, 647, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x4D, 5, 209, 201, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x4E, 6, 768, 490, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x12B, 7, 236, 720, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x12D, 8, 377, 310, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x12E, 9, 593, 608, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x12F, 0xA, 419, 666, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x130, 0xB, 736, 233, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x131, 0xC, 446, 679, 1 };
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
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 191, 124, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 383, 219, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 735, 142, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 807, 178, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 855, 202, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 4, 0, 927, 238, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 4, 0, 492, 156, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 70, 458, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 110, 478, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 150, 498, 0, 0 },
    { 1, 0, 0x40, 2, 0x35, 2, 0, 3, 4, 0, 380, 474, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 3, 4, 0, 584, 338, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 2, 0, 0xB, 4, 0, 569, 365, 0, 0 },
    { 1, 0, 0x4E, 2, 5, 0, 0, 0, 0, 0, 640, 434, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 704, 448, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 336, 602, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 0xB, 4, 0, 702, 441, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 0xB, 4, 0, 838, 509, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 737, 176, 214, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 392, 265, 292, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 419, 258, 304, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 544, 550, 583, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 185, 681, 707, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 704, 89, 176, 0 },
    { 1, 0, 0x60, 4, 8, 0, 0, 0, 0, 0, 512, 89, 174, 0 },
    { 1, 0, 0x48, 4, 9, 0, 0, 0, 0, 0, 233, 359, 422, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x284, 0x78, 0x124, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 8, 0x19E, 0x1C0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 8, 0x1AE, 0x248, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
