/*
 * WSTAG725: Kulon Pit, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x694
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x6A4
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0023 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x37900, 0x10500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3F;
    FIELDSTG_state.music = MUSIC(0x3F, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 161, 7, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 161, 7, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 161, 7, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 161, 7, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 110, 7, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 110, 7, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 110, 7, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 110, 7, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 223, 7, MUSIC(3, 0) };
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
    { 86, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x142, 0x88, 0x42, 0x160, 0x1FF },
    { 0x140, 0x100, 0x170, 0x120, 0xC0, 0x20, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16A, 0x148, 0xA8, 0x48, 0x160, 0x1FE },
    { 0x140, 0x100, 0x172, 0x148, 0xC8, 0x48, 0x170, 0x1FE },
};
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x21, 0), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x10), 1, CARD(0x19), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk2Actions[] = {
    CARD(0x19), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x13), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor7Talk3Conditions[] = { FLAG(0, 0x10), 1, CARD(0x19), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, CODES_END };
u16 actor8Talk2Actions[] = { CARD_BATTLE(0x21, 0), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x17), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0xE, 0x17), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x17), 1, PARTY_STAT(0xE), 0, CODES_END };
u16 actor9Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x17), 1, PARTY_STAT(0xE), 1, CODES_END };
u16 actor9Talk3Actions[] = { CARD_BATTLE(0x21, 1), 1, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 0, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xA), 1, CODES_END };
u16 actor10Talk2Actions[] = { CARD_BATTLE(0x21, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x2CD },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x2D0 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x2CE },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x1A0 },
    { actor4Talk1Conditions, NULL, 0x1A3 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x29A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x1A0 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x1A9 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x1AA },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x1A0 },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x1AB },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x1AC },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x1AD },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x1A1 },
    { actor8Talk1Conditions, NULL, 0x1A3 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x1A5 },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x1A6 },
    { actor9Talk2Conditions, NULL, 0x1A7 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x1A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0x1A2 },
    { actor10Talk1Conditions, NULL, 0x1A3 },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x2D1 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x2D4 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x2D2 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x2D3 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(4), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor6Conditions[] = {
    FLAG(0, 0x11), 1,
    SPECIAL(0xA), 1,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor7Conditions[] = {
    FLAG(0, 0x11), 1,
    ITEM(0, 0x12), 1,
    ITEM(0, 0x192), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor8Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    PROGRESS(0x26), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    ITEM(0, 0x12), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x22, 4, 161, 321, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x22, 4, 161, 321, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x22, 4, 161, 321, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x22, 4, 161, 321, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x33, 5, 816, 209, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 6, 816, 209, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xB5, 7, 692, 265, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xB5, 7, 495, 75, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xB5, 7, 692, 265, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xB5, 7, 692, 265, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x33, 2, 0, 3, 6, 0, 807, 124, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 647, 322, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 714, 356, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 755, 312, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 773, 350, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 830, 322, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 6, 0, 831, 297, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 289, 211, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 317, 214, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 4, 0, 762, 104, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 287, 282, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 482, 16, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 566, 355, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 630, 94, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 6, 0, 243, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 6, 0, 435, 24, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 6, 0, 467, 302, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 6, 0, 909, 158, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 6, 0, 330, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 6, 0, 339, 462, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 6, 0, 658, 77, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 3, 0, 9, 8, 0, 105, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 3, 0, 9, 8, 0, 143, 271, 0, 0 },
    { 1, 0, 0x40, 4, 0x38, 3, 0, 9, 8, 0, 77, 305, 328, 0 },
    { 1, 0, 0x44, 4, 0, 0, 0, 0, 0, 0, 635, 206, 246, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 464, 183, 220, 0 },
    { 1, 0, 0x42, 4, 2, 0, 0, 0, 0, 0, 61, 264, 328, 0 },
    { 1, 0, 0x50, 4, 3, 0, 0, 0, 0, 0, 800, 104, 175, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26A, 0x508, 0x54, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x269, 0xA8, 0xF4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x265, 0x220, 0x2C0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x200, 0x60, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x210, 0xC8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x220, 0x100, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x210, 0x198, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
