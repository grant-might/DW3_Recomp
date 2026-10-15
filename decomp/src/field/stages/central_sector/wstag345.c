/*
 * WSTAG345: Plug Cape, Central Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* After event 1264, with Soccer Kid Hide: sets flag 0x1C07 and keeps it from playing again */
void endEvent1264(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 7), 1);
    FLAGS_00.applyAction(FLAG(0x1A, 0x22), 1);
}

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x396
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x3A6
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
    FIELDSTG_state.start = (Vec2){0x6E00, 0xB600};
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

s16 script1264[] = {
    SCRIPT_WALK(2, 0x3A7, 0x394, 5),
    SCRIPT_PLACE(0x11B, 0x3C7, 0x385),
    SCRIPT_POSE(0x11B, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WALK(0x11B, 0x3E0, 0x378, 5),
    SCRIPT_WAIT_WALK(0x11B),
    SCRIPT_POSE(0x11B, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x11B, 8, 5),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SEARCH_EVENT_SPOT, 2),
    SCRIPT_WAIT_ANIM(0x11B),
    SCRIPT_POSE(0x11B, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x11B, 8, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SEARCH_EVENT_SPOT, 2),
    SCRIPT_WAIT_ANIM(0x11B),
    SCRIPT_POSE(0x11B, 1, 5),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x11B, 8, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SEARCH_EVENT_SPOT, 2),
    SCRIPT_WAIT_ANIM(0x11B),
    SCRIPT_POSE(0x11B, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x11B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x11B),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x11B),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x11B, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x11B, 0x3C7, 0x385, 1),
    SCRIPT_WAIT_WALK(0x11B),
    SCRIPT_POSE(0x11B, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x11B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x11B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x11B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x37F, 0x3A7, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AreaBattle place0Area0Battle0 = { 39, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle1 = { 39, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle2 = { 39, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle3 = { 39, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle4 = { 40, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle5 = { 40, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle6 = { 40, 13, MUSIC(2, 0) };
AreaBattle place0Area0Battle7 = { 40, 13, MUSIC(2, 0) };
BattleList place0Area0Battles = {
    3,
    { &place0Area0Battle0, &place0Area0Battle1, &place0Area0Battle2, &place0Area0Battle3,
      &place0Area0Battle4, &place0Area0Battle5, &place0Area0Battle6, &place0Area0Battle7 },
};
AreaBattle place0Area1Battle0 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle1 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle2 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle3 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle4 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle5 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle6 = { 51, 4, MUSIC(2, 0) };
AreaBattle place0Area1Battle7 = { 51, 4, MUSIC(2, 0) };
BattleList place0Area1Battles = {
    3,
    { &place0Area1Battle0, &place0Area1Battle1, &place0Area1Battle2, &place0Area1Battle3,
      &place0Area1Battle4, &place0Area1Battle5, &place0Area1Battle6, &place0Area1Battle7 },
};
AreaBattle place0Area2Battle0 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle1 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle2 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle3 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle4 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle5 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle6 = { 54, 2, MUSIC(2, 0) };
AreaBattle place0Area2Battle7 = { 54, 2, MUSIC(2, 0) };
BattleList place0Area2Battles = {
    3,
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
AreaBattle place1Area0Battle0 = { 39, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 39, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 39, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 39, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 40, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 40, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 40, 13, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 40, 13, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle1 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle2 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle3 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle4 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle5 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle6 = { 70, 4, MUSIC(2, 0) };
AreaBattle place1Area1Battle7 = { 70, 4, MUSIC(2, 0) };
BattleList place1Area1Battles = {
    3,
    { &place1Area1Battle0, &place1Area1Battle1, &place1Area1Battle2, &place1Area1Battle3,
      &place1Area1Battle4, &place1Area1Battle5, &place1Area1Battle6, &place1Area1Battle7 },
};
AreaBattle place1Area2Battle0 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle1 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle2 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle3 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle4 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle5 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle6 = { 59, 2, MUSIC(2, 0) };
AreaBattle place1Area2Battle7 = { 59, 2, MUSIC(2, 0) };
BattleList place1Area2Battles = {
    3,
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
    { 4, 0, 0, { &place0Area0Battles, &place0Area1Battles, &place0Area2Battles, &place0Area3Battles } },
    { 27, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x137, 0xD8, 0x37, 0x150, 0x1FF },
    { 0x140, 0x100, 0x176, 0x157, 0xD8, 0x57, 0x160, 0x1FF },
    { 0x140, 0x100, 0x176, 0x177, 0xD8, 0x77, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x197, 0xD8, 0x97, 0x140, 0x1FE },
};
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x22), 0, CODES_END };
u16 actor2Talk1Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 0,
    FLAG(0, 0), 0,
    CODES_END,
};
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 0,
    FLAG(0, 0), 1,
    CODES_END,
};
u16 actor2Talk3Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { FLAG(0x1A, 0x24), 1, FLAG(0x1C, 8), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 1,
    ITEM(0, 4), 0,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 1,
    ITEM(0, 4), 1,
    CODES_END,
};
u16 actor4Talk0Conditions[] = { FLAG(0x1A, 0x22), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0x1A, 0x22), 1, FLAG(0x1A, 0x23), 0, CODES_END };
u16 actor4Talk2Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 0,
    CODES_END,
};
u16 actor4Talk2Actions[] = { FLAG(0x1A, 0x24), 1, FLAG(0x1C, 8), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 1,
    ITEM(0, 4), 0,
    CODES_END,
};
u16 actor4Talk4Conditions[] = {
    FLAG(0x1A, 0x22), 1,
    FLAG(0x1A, 0x23), 1,
    FLAG(0x1A, 0x24), 1,
    ITEM(0, 4), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x1C },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x2A0 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x29B },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x29C },
    { actor2Talk2Conditions, NULL, 0x29E },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x29D },
    { actor2Talk4Conditions, NULL, 0x35D },
    { actor2Talk5Conditions, NULL, 0x29F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x29F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x29B },
    { actor4Talk1Conditions, NULL, 0x29C },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x29D },
    { actor4Talk3Conditions, NULL, 0x29E },
    { actor4Talk4Conditions, NULL, 0x2A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x2A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x1D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 4), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 4), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 4), 0, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 4), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x9D, 4, 885, 951, 5 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x119, 5, 865, 941, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x11B, 6, 967, 901, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x11B, 6, 967, 901, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x11B, 6, 967, 901, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x11B, 6, 967, 901, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x13A, 7, 905, 961, 5 };
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
    { 1, 0, 0x40, 2, 1, 1, 1, 6, 8, 0, 330, 524, 0, 0 },
    { 1, 0, 0x40, 2, 1, 1, 1, 6, 8, 0, 922, 950, 0, 0 },
    { 1, 0, 0x80, 2, 7, 0, 0, 0, 0, 0, 256, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 176, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 212, 597, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 534, 857, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 646, 785, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 820, 685, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 874, 498, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 139, 93, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 148, 223, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 154, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 460, 421, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 651, 773, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 750, 724, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 842, 515, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 185, 199, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 291, 768, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 398, 820, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 480, 897, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 608, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 693, 1010, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 213, 246, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 422, 830, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 462, 898, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 569, 865, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 647, 985, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 680, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 165, 373, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 170, 169, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 258, 270, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 323, 462, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 434, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 595, 944, 0, 0 },
    { 1, 0, 0xFF, 6, 8, 0, 0, 0, 0, 0, 703, 232, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 593, 792, 804, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 623, 849, 864, 0 },
    { 1, 0, 0x80, 4, 9, 0, 0, 0, 0, 0, 631, 378, 433, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 333, 235, 235, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 381, 259, 259, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 429, 283, 283, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 481, 532, 532, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 486, 775, 775, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 557, 557, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 533, 751, 751, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 541, 699, 699, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 574, 580, 580, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1014, 879, 879, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21D, 0x1D2, 0x92, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 0xC, 0x231, 0xB0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 0xC, 0x221, 0x178, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 3, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 2, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0xF, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x50, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFA2, 0x10, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x28, 0x20, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x38, 0x29, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 3, 1 },
    { { { FLAG(0x1A, 0x22), 0 }, { ITEM(0, 0x192), 1 } }, SLOT_EVENT, 0x4F0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1264, script1264, EVENT_TEXT(0x1D), NULL, endEvent1264 },
    { -1, NULL, 0, NULL, NULL },
};
