/*
 * WSTAG555: Noise Desert, West Sector.
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
#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

/* After event 380, with Numemon: keeps it from playing again */
void endEvent380(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x235
#define STAGE_ARCHIVE 0x312
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x244
#define STAGE_ARCHIVE 0x321
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0016 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x4D700, 0x4B700};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x38;
    FIELDSTG_state.music = MUSIC(0x38, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script380[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xF6, 0x163, 3),
    SCRIPT_PLACE(0x69, 0x140, 0xC8),
    SCRIPT_POSE(0x69, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(0x69, 0xDB, 0xFA, 1),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_FOLLOW(0, 0x69),
    SCRIPT_COMMAND(0x323, 0x325, 0x69),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x69),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x69, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(0x69, 0xDB, 0x151, 7),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_POSE(0x69, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x69, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x69, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x69, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x69, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 2),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0x69, 0xC7, 0x15C, 1),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WALK(0x69, 0x10E, 0x180, 7),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WALK(0x69, 0x1A7, 0x1CB, 7),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_TALK(0, 7, 2, 2),
    SCRIPT_PLACE(0x69, 0, 0),
    SCRIPT_POSE(0x69, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
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
AreaBattle area0Battle0 = { 148, 5, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 148, 5, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 148, 5, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 148, 5, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 154, 5, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 154, 5, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 154, 5, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 154, 5, MUSIC(2, 0) };
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
    { 32, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x180, 0x1C0, 0x80, 0x170, 0x1FF },
};
u16 actor0Conditions[] = { PROGRESS(0xF), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0x69, 4, 0, 0, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 6, 0, 0, 0, 0, 0, 874, 1273, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 50, 721, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 87, 297, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 150, 1359, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 185, 1233, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 211, 917, 0, 0 },
    { 1, 3, 0xC8, 6, 0x48, 0, 0, 0, 0, 0, 232, 991, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 299, 296, 0, 0 },
    { 1, 3, 0xC8, 6, 0x48, 0, 0, 0, 0, 0, 337, 292, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 401, 938, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 530, 1299, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 710, 1019, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 832, 395, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 964, 526, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 0, 0, 0, 0, 0, 967, 699, 0, 0 },
    { 1, 3, 0xC8, 6, 0x48, 0, 0, 0, 0, 0, 1223, 896, 0, 0 },
    { 1, 2, 0x40, 6, 0x3D, 0, 0, 0, 0, 0, 1411, 668, 0, 0 },
    { 1, 0, 0x80, 6, 7, 0, 0, 0, 0, 0, 856, 1221, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 506, 475, 544, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 317, 423, 488, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 225, 615, 666, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 476, 647, 713, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 592, 815, 880, 0 },
    { 1, 0, 0x64, 4, 5, 0, 0, 0, 0, 0, 289, 127, 230, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25D, 0xC8, 0x3BC, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x249, 0x7A, 0xEA, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x247, 0xB0, 0x90, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x257, 0x320, 0x88, 1, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0x1B, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 1, 2 },
    { { { PROGRESS(0xF), 1 }, { FLAG(0x40, 0xC), 0 } }, SLOT_EVENT, 0x17C, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 380, script380, EVENT_TEXT(4), NULL, endEvent380 },
    { -1, NULL, 0, NULL, NULL },
};
