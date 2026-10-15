/*
 * WSTAG971: Circuit Board, Underground: reached only from the other tunnels,
 * in the extra chapter. The European version's: in its extra chapter
 * (FIELD_PROGRESS_EXTRA), mode 747 starts it instead of WSTAG880.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x945
#define STAGE_FILE_8 0x68F
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x92B
#define STAGE_FILE_8 0x67F
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0028 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 4;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xEB00, 0x1F500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1E;
    FIELDSTG_state.music = MUSIC(0x1E, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 6);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 7);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 5);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints1_1Point0 = { 0x2EE, 1, 1, 224, 0x240, 5, NULL };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints1_2Point0 = { 0x2ED, 1, 1, 224, 192, 5, NULL };
StagePoints placePoints1_2 = { 1, 2, &placePoints1_2Point0 };
StagePoint placePoints1_3Point0 = { 0x2ED, 1, 2, 224, 192, 5, NULL };
StagePoints placePoints1_3 = { 1, 3, &placePoints1_3Point0 };
StagePoint placePoints1_4Point0 = { 0x2EE, 1, 2, 224, 0x240, 5, NULL };
StagePoints placePoints1_4 = { 1, 4, &placePoints1_4Point0 };
StagePoint placePoints1_5Point0 = { 0x2ED, 1, 5, 224, 192, 5, NULL };
StagePoints placePoints1_5 = { 1, 5, &placePoints1_5Point0 };
StagePoint placePoints1_6Point0 = { 0x2EE, 1, 4, 224, 0x240, 5, NULL };
StagePoints placePoints1_6 = { 1, 6, &placePoints1_6Point0 };
StagePoint placePoints5_1Point0 = { 0x2ED, 5, 4, 224, 192, 5, NULL };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints5_2Point0 = { 0x2ED, 5, 5, 224, 192, 5, NULL };
StagePoints placePoints5_2 = { 5, 2, &placePoints5_2Point0 };
StagePoint placePoints5_3Point0 = { 0x2ED, 5, 6, 0x350, 0x1F8, 5, NULL };
StagePoints placePoints5_3 = { 5, 3, &placePoints5_3Point0 };
StagePoint placePoints6_1Point0 = { 0x2ED, 6, 4, 224, 192, 5, NULL };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints1_2, &placePoints1_3, &placePoints1_4,
    &placePoints1_5, &placePoints1_6, &placePoints5_1, &placePoints5_2,
    &placePoints5_3, &placePoints6_1, NULL,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x15E, 0x140, 0x78, 0x40, 0x140, 0x1FF },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x152, 0x140, 0x48, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x140, 0x140, 0, 0x40, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16A, 0x140, 0xA8, 0x40, 0x140, 0x1FE },
    { 0x140, 0x100, 0x154, 0x100, 0x50, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x160, 0x1FE },
};
u16 actor0Talk0Conditions[] = { ITEM(0, 0x168), 1, CODES_END };
u16 actor0Talk0Actions[] = { SPECIAL(0x38), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(0, 0x168), 0, FLAG(0, 0), 0, FLAG(0xA, 0x1A), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(0, 0x168), 0, FLAG(0, 0), 1, FLAG(0xA, 0x1A), 0, CODES_END };
u16 actor0Talk2Actions[] = { EVENT_BATTLE(3), 1, FLAG(0xA, 0x1A), 1, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(0, 0x168), 0, FLAG(0xA, 0x1A), 1, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk3Actions[] = { ITEM(0, 0x168), 1, SPECIAL(0x38), 1, CODES_END };
u16 actor0Talk4Conditions[] = { ITEM(0, 0x168), 0, FLAG(0, 0), 1, FLAG(0xA, 0x1A), 1, CODES_END };
u16 actor0Talk4Actions[] = { ITEM(0, 0x168), 1, SPECIAL(0x38), 1, CODES_END };
u16 actor1Talk0Conditions[] = { ITEM(0, 6), 1, CODES_END };
u16 actor1Talk0Actions[] = { SPECIAL(0x31), 1, CODES_END };
u16 actor1Talk1Conditions[] = { ITEM(0, 6), 0, FLAG(0, 0), 0, FLAG(0xA, 0x14), 0, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk2Conditions[] = { ITEM(0, 6), 0, FLAG(0, 0), 1, FLAG(0xA, 0x14), 0, CODES_END };
u16 actor1Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xA, 0x14), 1, CODES_END };
u16 actor1Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x14), 1, ITEM(0, 6), 0, CODES_END };
u16 actor1Talk3Actions[] = { SPECIAL(0x31), 1, ITEM(0, 6), 1, CODES_END };
u16 actor1Talk4Conditions[] = { ITEM(0, 6), 0, FLAG(0, 0), 1, FLAG(0xA, 0x14), 1, CODES_END };
u16 actor1Talk4Actions[] = { ITEM(0, 6), 1, SPECIAL(0x31), 1, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(0, 0x26), 1, CODES_END };
u16 actor2Talk0Actions[] = { SPECIAL(0x35), 1, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(0, 0x26), 0, FLAG(0, 0), 0, FLAG(0xA, 0x17), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(0, 0x26), 0, FLAG(0, 0), 1, FLAG(0xA, 0x17), 0, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(2), 1, FLAG(0xA, 0x17), 1, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x17), 1, ITEM(0, 0x26), 0, CODES_END };
u16 actor2Talk3Actions[] = { SPECIAL(0x35), 1, ITEM(0, 0x26), 1, CODES_END };
u16 actor2Talk4Conditions[] = { ITEM(0, 0x26), 0, FLAG(0, 0), 1, FLAG(0xA, 0x17), 1, CODES_END };
u16 actor2Talk4Actions[] = { ITEM(0, 0x26), 1, SPECIAL(0x35), 1, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(0, 0x22), 1, CODES_END };
u16 actor3Talk0Actions[] = { SPECIAL(0x33), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(0, 0x22), 0, FLAG(0, 0), 0, FLAG(0xA, 0x15), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(0, 0x22), 0, FLAG(0, 0), 1, FLAG(0xA, 0x15), 0, CODES_END };
u16 actor3Talk2Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xA, 0x15), 1, CODES_END };
u16 actor3Talk3Conditions[] = { FLAG(0, 0), 0, FLAG(0xA, 0x15), 1, ITEM(0, 0x22), 0, CODES_END };
u16 actor3Talk3Actions[] = { SPECIAL(0x33), 1, ITEM(0, 0x22), 1, CODES_END };
u16 actor3Talk4Conditions[] = { ITEM(0, 0x22), 0, FLAG(0, 0), 1, FLAG(0xA, 0x15), 1, CODES_END };
u16 actor3Talk4Actions[] = { ITEM(0, 0x22), 1, SPECIAL(0x33), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0xE9 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0xEA },
    { actor0Talk2Conditions, actor0Talk2Actions, 0xEB },
    { actor0Talk3Conditions, actor0Talk3Actions, 0xEC },
    { actor0Talk4Conditions, actor0Talk4Actions, 0xEC },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0xD1 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0xD2 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0xD3 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0xD4 },
    { actor1Talk4Conditions, actor1Talk4Actions, 0xD4 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0xDD },
    { actor2Talk1Conditions, actor2Talk1Actions, 0xDE },
    { actor2Talk2Conditions, actor2Talk2Actions, 0xDF },
    { actor2Talk3Conditions, actor2Talk3Actions, 0xE0 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0xE0 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0xD5 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0xD6 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0xD7 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0xD8 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0xD8 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    WARP_ARG(5), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0x11), 0,
    CODES_END,
};
u16 actor1Conditions[] = {
    SPECIAL(0x95), 1,
    SPECIAL(0xB), 0,
    WARP_ARG(4), 1,
    WARP_ARG(0x1F), 1,
    CODES_END,
};
u16 actor2Conditions[] = {
    WARP_ARG(4), 1,
    WARP_ARG(0x20), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0xF), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    WARP_ARG(4), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x95), 1,
    SPECIAL(0xC), 0,
    CODES_END,
};
u16 actor5Conditions[] = { WARP_ARG(0), 1, FLAG(0, 8), 0, CODES_END };
u16 actor6Conditions[] = { WARP_ARG(4), 1, FLAG(0, 8), 0, CODES_END };
u16 actor7Conditions[] = { WARP_ARG(0x1E), 1, FLAG(0, 9), 0, CODES_END };
u16 actor8Conditions[] = { WARP_ARG(0x1F), 1, FLAG(0, 9), 0, CODES_END };
u16 actor9Conditions[] = { FLAG(0, 9), 0, WARP_ARG(0x20), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x64, 4, 240, 600, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x7B, 5, 240, 600, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x7D, 6, 240, 600, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x7E, 7, 240, 600, 1 };
FieldActorEntry actor4 = { NULL, NULL, 0x146, 8, 0, 0, 0 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x148, 9, 480, 208, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x148, 9, 400, 344, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x15F, 0xA, 288, 400, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x15F, 0xA, 240, 488, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x15F, 0xA, 352, 272, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2EB, 0x240, 0xA0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
AreaBattle place1Area0Battle0 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 140, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 141, 10, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 141, 10, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area1Battles = {
    0,
    { &place1Area1Battle0, &place1Area1Battle1, &place1Area1Battle2, &place1Area1Battle3,
      &place1Area1Battle4, &place1Area1Battle5, &place1Area1Battle6, &place1Area1Battle7 },
};
AreaBattle place1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area2Battles = {
    0,
    { &place1Area2Battle0, &place1Area2Battle1, &place1Area2Battle2, &place1Area2Battle3,
      &place1Area2Battle4, &place1Area2Battle5, &place1Area2Battle6, &place1Area2Battle7 },
};
AreaBattle place1Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
AreaBattle place5Area0Battle0 = { 116, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 116, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 164, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 164, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 139, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 139, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 163, 10, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 163, 10, MUSIC(2, 0) };
BattleList place5Area0Battles = {
    3,
    { &place5Area0Battle0, &place5Area0Battle1, &place5Area0Battle2, &place5Area0Battle3,
      &place5Area0Battle4, &place5Area0Battle5, &place5Area0Battle6, &place5Area0Battle7 },
};
AreaBattle place5Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area1Battles = {
    0,
    { &place5Area1Battle0, &place5Area1Battle1, &place5Area1Battle2, &place5Area1Battle3,
      &place5Area1Battle4, &place5Area1Battle5, &place5Area1Battle6, &place5Area1Battle7 },
};
AreaBattle place5Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area2Battles = {
    0,
    { &place5Area2Battle0, &place5Area2Battle1, &place5Area2Battle2, &place5Area2Battle3,
      &place5Area2Battle4, &place5Area2Battle5, &place5Area2Battle6, &place5Area2Battle7 },
};
AreaBattle place5Area3Battle0 = { 301, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle1 = { 302, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle2 = { 304, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle3 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place5Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area3Battles = {
    0,
    { &place5Area3Battle0, &place5Area3Battle1, &place5Area3Battle2, &place5Area3Battle3,
      &place5Area3Battle4, &place5Area3Battle5, &place5Area3Battle6, &place5Area3Battle7 },
};
AreaBattle place6Area0Battle0 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 162, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 135, 10, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 135, 10, MUSIC(2, 0) };
BattleList place6Area0Battles = {
    3,
    { &place6Area0Battle0, &place6Area0Battle1, &place6Area0Battle2, &place6Area0Battle3,
      &place6Area0Battle4, &place6Area0Battle5, &place6Area0Battle6, &place6Area0Battle7 },
};
AreaBattle place6Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area1Battles = {
    0,
    { &place6Area1Battle0, &place6Area1Battle1, &place6Area1Battle2, &place6Area1Battle3,
      &place6Area1Battle4, &place6Area1Battle5, &place6Area1Battle6, &place6Area1Battle7 },
};
AreaBattle place6Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area2Battles = {
    0,
    { &place6Area2Battle0, &place6Area2Battle1, &place6Area2Battle2, &place6Area2Battle3,
      &place6Area2Battle4, &place6Area2Battle5, &place6Area2Battle6, &place6Area2Battle7 },
};
AreaBattle place6Area3Battle0 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle1 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle2 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle3 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle4 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle5 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle6 = { 311, 10, MUSIC(0x22, 0) };
AreaBattle place6Area3Battle7 = { 311, 10, MUSIC(0x22, 0) };
BattleList place6Area3Battles = {
    0,
    { &place6Area3Battle0, &place6Area3Battle1, &place6Area3Battle2, &place6Area3Battle3,
      &place6Area3Battle4, &place6Area3Battle5, &place6Area3Battle6, &place6Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 416, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 439, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 445, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
};
