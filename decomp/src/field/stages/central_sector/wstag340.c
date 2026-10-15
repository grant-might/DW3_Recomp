/*
 * WSTAG340: Shell Beach, Central Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x392
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x3A2
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV_0014; BGM_0029 instead at story steps 0x27 to 0x28; battle
 * list 0 before story step 0xB, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x30700, 0x9A00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x36;
    FIELDSTG_state.music = MUSIC(0x36, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress < 0xB) {
        FIELDSTG_state.battles = &stageBattles[0];
    } else {
        FIELDSTG_state.battles = &stageBattles[1];
    }
}

s16 script1310[] = {
    SCRIPT_WALK(2, 0x1C0, 0x270, 1),
    SCRIPT_POSE(0x11A, 7, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x11A, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x11A, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x11A, 7, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x11A, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AreaBattle place0Area0Battle0 = { 39, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle1 = { 39, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle2 = { 39, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle3 = { 39, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle4 = { 40, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle5 = { 40, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle6 = { 40, 1, MUSIC(2, 0) };
AreaBattle place0Area0Battle7 = { 40, 1, MUSIC(2, 0) };
BattleList place0Area0Battles = {
    3,
    { &place0Area0Battle0, &place0Area0Battle1, &place0Area0Battle2, &place0Area0Battle3,
      &place0Area0Battle4, &place0Area0Battle5, &place0Area0Battle6, &place0Area0Battle7 },
};
AreaBattle place0Area1Battle0 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle1 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle2 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle3 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle4 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle5 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle6 = { 37, 2, MUSIC(2, 0) };
AreaBattle place0Area1Battle7 = { 37, 2, MUSIC(2, 0) };
BattleList place0Area1Battles = {
    3,
    { &place0Area1Battle0, &place0Area1Battle1, &place0Area1Battle2, &place0Area1Battle3,
      &place0Area1Battle4, &place0Area1Battle5, &place0Area1Battle6, &place0Area1Battle7 },
};
AreaBattle place0Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place0Area2Battles = {
    0,
    { &place0Area2Battle0, &place0Area2Battle1, &place0Area2Battle2, &place0Area2Battle3,
      &place0Area2Battle4, &place0Area2Battle5, &place0Area2Battle6, &place0Area2Battle7 },
};
AreaBattle place0Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle place0Area3Battle4 = { 328, 2, MUSIC(2, 0) };
AreaBattle place0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle place0Area3Battle7 = { 54, 2, MUSIC(2, 0) };
BattleList place0Area3Battles = {
    0,
    { &place0Area3Battle0, &place0Area3Battle1, &place0Area3Battle2, &place0Area3Battle3,
      &place0Area3Battle4, &place0Area3Battle5, &place0Area3Battle6, &place0Area3Battle7 },
};
AreaBattle place1Area0Battle0 = { 39, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 39, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 39, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 39, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 40, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 40, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 40, 1, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 40, 1, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle1 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle2 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle3 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle4 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle5 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle6 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area1Battle7 = { 59, 2, MUSIC(2, 0) };
BattleList place1Area1Battles = {
    3,
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
AreaBattle place1Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle place1Area3Battle4 = { 328, 2, MUSIC(2, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle place1Area3Battle7 = { 54, 2, MUSIC(2, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 3, 0, 0, { &place0Area0Battles, &place0Area1Battles, &place0Area2Battles, &place0Area3Battles } },
    { 26, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x172, 0x181, 0xC8, 0x81, 0x150, 0x1FF },
    { 0x140, 0x100, 0x172, 0x1A1, 0xC8, 0xA1, 0x160, 0x1FF },
};
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x36), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1A, 0x36), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0x36), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor2Talk2Conditions[] = {
    FLAG(0x1A, 0x36), 1,
    SPECIAL(0x3A), 0,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor2Talk2Actions[] = { FLAG(0x1A, 0x21), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0x1A, 0x36), 1,
    SPECIAL(0x3A), 1,
    ITEM(0, 5), 0,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor2Talk3Actions[] = { ITEM(0, 5), 1, START_EVENT(0x47), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0x1A, 0x36), 1,
    SPECIAL(0x3A), 1,
    ITEM(0, 5), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { FLAG(0x1A, 0x36), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0x1A, 0x36), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(0, 0x192), 0, FLAG(0x1A, 0x36), 1, CODES_END };
u16 actor3Talk2Conditions[] = {
    FLAG(0x1A, 0x36), 1,
    ITEM(0, 0x192), 1,
    SPECIAL(0x3A), 0,
    CODES_END,
};
u16 actor3Talk2Actions[] = { FLAG(0x1A, 0x21), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x3A), 1,
    ITEM(0, 5), 0,
    FLAG(0x1A, 0x36), 1,
    CODES_END,
};
u16 actor3Talk3Actions[] = { ITEM(0, 5), 1, START_EVENT(0x47), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    FLAG(0x1A, 0x36), 1,
    ITEM(0, 0x192), 1,
    SPECIAL(0x3A), 1,
    ITEM(0, 5), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x293 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x292 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x28E },
    { actor2Talk1Conditions, NULL, 0x28F },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x290 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x291 },
    { actor2Talk4Conditions, NULL, 0x292 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x28E },
    { actor3Talk1Conditions, NULL, 0x28F },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x290 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x291 },
    { actor3Talk4Conditions, NULL, 0x292 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x292 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 5), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 5), 0, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 5), 0, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 5), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x9D, 4, 412, 642, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x11A, 5, 412, 642, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x11A, 5, 412, 642, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x11A, 5, 412, 642, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x11A, 5, 412, 642, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 5, 1, 5, 0xA, 8, 0, 395, 85, 0, 0 },
    { 1, 0, 0x40, 2, 5, 1, 5, 0xA, 8, 0, 698, 285, 0, 0 },
    { 1, 0, 0x50, 2, 0xB, 0, 0, 0, 0, 0, 688, 97, 0, 0 },
    { 1, 0, 0x72, 2, 0xC, 0, 0, 0, 0, 0, 784, 47, 0, 0 },
    { 1, 0, 0x80, 2, 0xD, 0, 0, 0, 0, 0, 640, 128, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 72, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 271, 529, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 677, 747, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 224, 557, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 309, 704, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 290, 716, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 698, 747, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 83, 463, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 47, 385, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 146, 531, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 37, 375, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 90, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 218, 638, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 75, 550, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 96, 549, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 182, 545, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 288, 706, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 310, 714, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 321, 691, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 494, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 514, 149, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 671, 111, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 799, 48, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 811, 36, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 817, 43, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 496, 558, 575, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 509, 517, 543, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 514, 471, 489, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 676, 390, 407, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 276, 499, 511, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 224, 271, 271, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 239, 239, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 320, 255, 255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 390, 243, 243, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 247, 247, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 231, 231, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 343, 343, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 656, 375, 375, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21D, 0x102, 0x3AC, 5, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 4, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x3C, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x40, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1310, script1310, EVENT_TEXT(0x26), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
