/*
 * WSTAG630: S Noise Desert, West Sector.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateTileAnimsFrames0[];
extern AnimFrame updateTileAnimsFrames1[];
extern AnimFrame updateTileAnimsFrames2[];

#include "common/step_looping_animation.inc.c"

#include "common/update_tile_anims3_once.inc.c"

#include "common/create_tile_anims_5c.inc.c"

#include "common/update_stage_tile_anims.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x484
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x494
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1AF00, 0x11800};
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

AnimFrame updateTileAnimsFrames0[] = {
    { 50, 8 }, { 51, 8 }, { 52, 8 }, { 53, 8 },
    { 54, 8 }, { 55, 8 }, { 56, 8 }, { 57, 8 },
    { 58, 8 }, { 59, 8 }, { 60, 8 }, { 82, 160 },
    { 255, 0 },
};
AnimFrame updateTileAnimsFrames1[] = {
    { 61, 8 }, { 62, 8 }, { 63, 8 }, { 64, 8 },
    { 65, 8 }, { 66, 8 }, { 67, 8 }, { 68, 8 },
    { 69, 8 }, { 70, 8 }, { 71, 8 }, { 82, 160 },
    { 255, 0 },
};
AnimFrame updateTileAnimsFrames2[] = {
    { 72, 8 }, { 73, 8 }, { 74, 8 }, { 75, 8 },
    { 76, 8 }, { 77, 8 }, { 78, 8 }, { 79, 8 },
    { 80, 8 }, { 81, 8 }, { 82, 160 }, { 255, 0 },
};
AreaBattle area0Battle0 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 156, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 156, 5, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
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
    { 36, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x188, 0x100, 0x120, 0, 0x170, 0x1FF },
    { 0x180, 0x100, 0x190, 0x100, 0x140, 0, 0x140, 0x1FE },
};
u16 actor1Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor11Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor12Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor13Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x72 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x1B9 },
    { actor1Talk1Conditions, NULL, 0x4A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x1BD },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x1BC },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x1BB },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1BA },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x1B9 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x73 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x1AE },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x1BF },
    { actor9Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, actor11Talk0Actions, 0x1C6 },
    { actor11Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0x1C5 },
    { actor12Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0x1C4 },
    { actor13Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x1C3 },
    { actor14Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0x1C2 },
    { actor15Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0x1C1 },
    { actor16Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0x1C0 },
    { actor17Talk1Conditions, NULL, 4 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x16), 1, PROGRESS(0x10), 0, PROGRESS(0xF), 0, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0xF), 0, PROGRESS(0x10), 0, SPECIAL(0x16), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x10), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x22, 4, 433, 280, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x40, 5, 433, 537, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x40, 5, 433, 537, 1 };
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
    { 1, 3, 0x40, 2, 0x48, 0, 0, 0, 0, 0, 302, 396, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 120, 716, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 277, 632, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 342, 321, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 604, 413, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 726, 314, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 752, 174, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 772, 512, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 949, 77, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 541, 275, 326, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 364, 459, 509, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x248, 0x90, 0x540, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x258, 0x340, 0xF0, 1, 0, 1, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
