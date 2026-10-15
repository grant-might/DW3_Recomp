/*
 * WSTAG956: Qing Long City, East Sector, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 668 starts it
 * instead of WSTAG421.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x927
#define STAGE_FILE_8 0x783
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x901
#define STAGE_FILE_8 0x774
#endif
/* Sets the stage up: its map, actors and battles, playing SEIR_BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x1D100, 0x1F600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xC;
    FIELDSTG_state.music = MUSIC(0xC, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x179, 0x1CE, 0xE4, 0xCE, 0x140, 0x1FF },
    { 0x140, 0x100, 0x14E, 0x1DC, 0x38, 0xDC, 0x150, 0x1FF },
    { 0x1C0, 0x100, 0x1D0, 0x100, 0x240, 0, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1B0, 0x1AE, 0x1C0, 0xAE, 0x170, 0x1FF },
    { 0x1C0, 0x100, 0x1D8, 0x100, 0x260, 0, 0x140, 0x1FE },
    { 0x180, 0x100, 0x1A4, 0x140, 0x190, 0x40, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1F4, 0x100, 0x2D0, 0, 0x160, 0x1FE },
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0, 1), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor1Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor1Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 1, CODES_END };
u16 actor1Talk3Actions[] = { CARD_BATTLE(0x45, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { SPECIAL(0x96), 0, CODES_END };
u16 actor5Talk1Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xD), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0x10, 0xD), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor5Talk2Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xD), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0, 0), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, CODES_END };
u16 actor7Talk3Actions[] = { CARD_BATTLE(0x4B, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x5E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x60 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x61 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x5E },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x5F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x76 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x75 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x72 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x73 },
    { actor5Talk2Conditions, NULL, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x5A },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x5C },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x5D },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x5A },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x5B },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x78 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
u16 actor6Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor7Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 623, 408, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 623, 408, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x2E, 5, 592, 424, 5 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x31, 6, 449, 481, 1 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x86, 7, 1088, 400, 7 };
FieldActorEntry actor5 = { NULL, actor5Talks, 0x8F, 8, 352, 234, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xB3, 9, 914, 344, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xB3, 9, 914, 344, 7 };
FieldActorEntry actor8 = { NULL, actor8Talks, 0x175, 0xA, 833, 257, 5 };
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
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 441, 290, 0, 0 },
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 817, 114, 0, 0 },
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 949, 528, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 366, 274, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 446, 241, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 1129, 333, 0, 0 },
    { 1, 0, 0x80, 2, 0x24, 0, 0, 0, 0, 0, 469, 191, 0, 0 },
    { 1, 0, 0x80, 2, 0x25, 0, 0, 0, 0, 0, 256, 161, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 411, 424, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 538, 379, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 568, 341, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 607, 347, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 976, 215, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 544, 363, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 579, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 884, 243, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 913, 244, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 1020, 218, 0, 0 },
    { 1, 0, 0x40, 6, 0x10, 1, 0x10, 0x12, 4, 0, 906, 526, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x1B, 0, 0, 0, 0, 0, 721, 200, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x1C, 0, 0, 0, 0, 0, 451, 367, 0, 0 },
    { 1, 0, 0x40, 4, 8, 1, 8, 0xB, 4, 0, 448, 405, 445, 0 },
    { 1, 0, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 465, 438, 464, 0 },
    { 1, 0, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 737, 256, 281, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 898, 552, 576, 0 },
    { 1, 0, 0x40, 4, 0x19, 0, 0, 0, 0, 0, 727, 260, 280, 0 },
    { 1, 0, 0x40, 4, 0x1A, 0, 0, 0, 0, 0, 447, 417, 444, 0 },
    { 1, 0, 0x64, 4, 0x1D, 0, 0, 0, 0, 0, 784, 496, 560, 0 },
    { 1, 0, 0x64, 4, 0x1E, 0, 0, 0, 0, 0, 768, 488, 552, 0 },
    { 1, 0, 0x64, 4, 0x1F, 0, 0, 0, 0, 0, 752, 480, 544, 0 },
    { 1, 0, 0x64, 4, 0x20, 0, 0, 0, 0, 0, 736, 472, 536, 0 },
    { 1, 0, 0x64, 4, 0x21, 0, 0, 0, 0, 0, 720, 464, 528, 0 },
    { 1, 0, 0x64, 4, 0x22, 0, 0, 0, 0, 0, 704, 456, 520, 0 },
    { 1, 0, 0x64, 4, 0x23, 0, 0, 0, 0, 0, 672, 456, 496, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x298, 0x448, 0xF8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29E, 0xC8, 0x1A4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29E, 0x1B8, 0x204, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29D, 0x208, 0x1AC, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29F, 0x1A7, 0x254, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x400, 0x120, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x410, 0x188, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
AreaBattle area0Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle7 = { 0, 0, MUSIC(1, 0) };
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
AreaBattle area3Battle0 = { 305, 18, MUSIC(0x23, 0) };
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
    { 393, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
