/*
 * WSTAG480: Jungle Grave, South Sector.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern StageTileFrame **updateTileCursorsFrames[];

#include "common/step_tile_sequences.inc.c"

#include "common/update_tile_cursors.inc.c"

#include "common/create_tile_cursors.inc.c"

/* Creates the stage's second object, and the event object of story progress 7 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[1] = createTileCursors();
        if (GAME.progress == 7 && FLAGS_00.checkCondition(FLAG(0x40, 0), 1)) {
            children[0] = FIELDSTG_startEvent(0xAB);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

/* After event 170, with Zanbamon: lets event 171 start and starts the stage's event battle 0 */
void endEvent170(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 171, with Zanbamon: moves the story on to its step 8 */
void endEvent171(void) {
    GAME.progress = 8;
}

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0 };

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x357
#define STAGE_ARCHIVE 0x628
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x366
#define STAGE_ARCHIVE 0x638
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0015;
 * battle list 0 before story step 0xA, 1 before story step 0x18, 2 from then
 * on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1F000, 0x34400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x11;
    FIELDSTG_state.music = MUSIC(0x11, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0xA) {
        FIELDSTG_state.battles = &stageBattles[0];
    } else if (GAME.progress < 0x18) {
        FIELDSTG_state.battles = &stageBattles[1];
    } else {
        FIELDSTG_state.battles = &stageBattles[2];
    }
}

s16 script170[] = {
    SCRIPT_WALK(2, 0x138, 0x335, 3),
    SCRIPT_PLACE(0x85, 0x118, 0x325),
    SCRIPT_POSE(0x85, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x85),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x85),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x85, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x85, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script171[] = {
    SCRIPT_PLACE(2, 0x138, 0x335),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x85, 0x118, 0x325),
    SCRIPT_POSE(0x85, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x85, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x85, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x138, 0x354, 0),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x158, 0x364, 7),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x234, 0x130, 0x360, 5),
    SCRIPT_END,
};
s16 script240[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x138, 0x335, 3),
    SCRIPT_PLACE(0x85, 0x118, 0x325),
    SCRIPT_POSE(0x85, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x85),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x85),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 6, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x85, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x85, 0x80, 0x2D9, 3),
    SCRIPT_WAIT_WALK(0x85),
    SCRIPT_TALK(0, 5, 2, 3),
    SCRIPT_PLACE(0x85, 0, 0),
    SCRIPT_POSE(0x85, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
StageTileFrame updateTileCursorsFrames_0_0[] = {
    { 64, 10, 0, 0 }, { 64, 10, 1, 0 }, { 64, 10, 2, 0 }, { 64, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_0_12[] = {
    { 65, 4, 2, 0 }, { 65, 4, 4, 0 }, { 65, 4, 6, 0 }, { 65, 4, 4, 0 },
    { 65, 4, 2, 0 }, { 65, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_1_0[] = {
    { 66, 10, 0, 0 }, { 66, 10, 1, 0 }, { 66, 10, 2, 0 }, { 66, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_1_12[] = {
    { 67, 4, 2, 0 }, { 67, 4, 4, 0 }, { 67, 4, 6, 0 }, { 67, 4, 4, 0 },
    { 67, 4, 2, 0 }, { 67, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_2_0[] = {
    { 68, 10, 0, 0 }, { 68, 10, 1, 0 }, { 68, 10, 2, 0 }, { 68, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_2_12[] = {
    { 69, 4, 2, 0 }, { 69, 4, 4, 0 }, { 69, 4, 6, 0 }, { 69, 4, 4, 0 },
    { 69, 4, 2, 0 }, { 69, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_3_0[] = {
    { 70, 10, 0, 0 }, { 70, 10, 1, 0 }, { 70, 10, 2, 0 }, { 70, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_3_12[] = {
    { 71, 4, 2, 0 }, { 71, 4, 4, 0 }, { 71, 4, 6, 0 }, { 71, 4, 4, 0 },
    { 71, 4, 2, 0 }, { 71, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_4_0[] = {
    { 72, 10, 0, 0 }, { 72, 10, 1, 0 }, { 72, 10, 2, 0 }, { 72, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_4_12[] = {
    { 73, 4, 2, 0 }, { 73, 4, 4, 0 }, { 73, 4, 6, 0 }, { 73, 4, 4, 0 },
    { 73, 4, 2, 0 }, { 73, 4, 0, 1 },
};
StageTileFrame *updateTileCursorsFrames_0[] = {
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_1[] = {
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_2[] = {
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_3[] = {
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_4[] = {
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_12, NULL,
};
StageTileFrame **updateTileCursorsFrames[] = {
    updateTileCursorsFrames_0, updateTileCursorsFrames_1, updateTileCursorsFrames_2, updateTileCursorsFrames_3,
    updateTileCursorsFrames_4,
};
AreaBattle place0Area0Battle0 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle1 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle2 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle3 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle4 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle5 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle6 = { 91, 9, MUSIC(2, 0) };
AreaBattle place0Area0Battle7 = { 91, 9, MUSIC(2, 0) };
BattleList place0Area0Battles = {
    4,
    { &place0Area0Battle0, &place0Area0Battle1, &place0Area0Battle2, &place0Area0Battle3,
      &place0Area0Battle4, &place0Area0Battle5, &place0Area0Battle6, &place0Area0Battle7 },
};
AreaBattle place0Area1Battle0 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle1 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle2 = { 54, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle3 = { 54, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle4 = { 91, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle5 = { 91, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle6 = { 91, 8, MUSIC(2, 0) };
AreaBattle place0Area1Battle7 = { 91, 8, MUSIC(2, 0) };
BattleList place0Area1Battles = {
    1,
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
AreaBattle place0Area3Battle0 = { 11, 19, MUSIC(0x22, 0) };
AreaBattle place0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle3 = { 329, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle6 = { 48, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList place0Area3Battles = {
    0,
    { &place0Area3Battle0, &place0Area3Battle1, &place0Area3Battle2, &place0Area3Battle3,
      &place0Area3Battle4, &place0Area3Battle5, &place0Area3Battle6, &place0Area3Battle7 },
};
AreaBattle place1Area0Battle0 = { 38, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 38, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 38, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 38, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 55, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 55, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 55, 9, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 55, 9, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle1 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle2 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle3 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle4 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle5 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle6 = { 54, 8, MUSIC(2, 0) };
AreaBattle place1Area1Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList place1Area1Battles = {
    2,
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
AreaBattle place1Area3Battle3 = { 329, 9, MUSIC(2, 0) };
AreaBattle place1Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 48, 9, MUSIC(2, 0) };
AreaBattle place1Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
AreaBattle place2Area0Battle0 = { 38, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle1 = { 55, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle2 = { 56, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle3 = { 56, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle4 = { 56, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle5 = { 56, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle6 = { 56, 9, MUSIC(2, 0) };
AreaBattle place2Area0Battle7 = { 56, 9, MUSIC(2, 0) };
BattleList place2Area0Battles = {
    3,
    { &place2Area0Battle0, &place2Area0Battle1, &place2Area0Battle2, &place2Area0Battle3,
      &place2Area0Battle4, &place2Area0Battle5, &place2Area0Battle6, &place2Area0Battle7 },
};
AreaBattle place2Area1Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle1 = { 147, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle2 = { 60, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle3 = { 60, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle4 = { 60, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle5 = { 60, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle6 = { 60, 8, MUSIC(2, 0) };
AreaBattle place2Area1Battle7 = { 60, 8, MUSIC(2, 0) };
BattleList place2Area1Battles = {
    2,
    { &place2Area1Battle0, &place2Area1Battle1, &place2Area1Battle2, &place2Area1Battle3,
      &place2Area1Battle4, &place2Area1Battle5, &place2Area1Battle6, &place2Area1Battle7 },
};
AreaBattle place2Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place2Area2Battles = {
    0,
    { &place2Area2Battle0, &place2Area2Battle1, &place2Area2Battle2, &place2Area2Battle3,
      &place2Area2Battle4, &place2Area2Battle5, &place2Area2Battle6, &place2Area2Battle7 },
};
AreaBattle place2Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle3 = { 329, 9, MUSIC(2, 0) };
AreaBattle place2Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle place2Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place2Area3Battle6 = { 48, 9, MUSIC(2, 0) };
AreaBattle place2Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList place2Area3Battles = {
    0,
    { &place2Area3Battle0, &place2Area3Battle1, &place2Area3Battle2, &place2Area3Battle3,
      &place2Area3Battle4, &place2Area3Battle5, &place2Area3Battle6, &place2Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 17, 0, 0, { &place0Area0Battles, &place0Area1Battles, &place0Area2Battles, &place0Area3Battles } },
    { 18, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 56, 2, 0, { &place2Area0Battles, &place2Area1Battles, &place2Area2Battles, &place2Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x160, 0x1FA },
};
u16 actor0Talk0Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor0Talk1Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor0Talk2Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor0Talk3Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor0Talk3Actions[] = { FLAG(0x1C, 9), 1, START_EVENT(0x1F), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x245 },
    { actor0Talk1Conditions, NULL, 0x245 },
    { actor0Talk2Conditions, NULL, 0x246 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x247 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x1C, 9), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x85, 4, 280, 805, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 1, 0x40, 2, 0x40, 0, 0, 0, 0, 0, 310, 197, 0, 0 },
    { 1, 2, 0x40, 2, 0x42, 0, 0, 0, 0, 0, 381, 208, 0, 0 },
    { 1, 3, 0x40, 2, 0x44, 0, 0, 0, 0, 0, 416, 204, 0, 0 },
    { 1, 5, 0x40, 2, 0x48, 0, 0, 0, 0, 0, 464, 258, 0, 0 },
    { 1, 4, 0x40, 2, 0x46, 0, 0, 0, 0, 0, 468, 206, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 191, 408, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 564, 488, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 594, 498, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 724, 120, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 772, 419, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 804, 410, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 901, 541, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 977, 774, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 986, 772, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 995, 783, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1010, 771, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1017, 771, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1024, 784, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1047, 799, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1057, 791, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1066, 796, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1081, 795, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1088, 789, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1096, 796, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 79, 617, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 262, 882, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 431, 638, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 702, 664, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 931, 679, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 1151, 642, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 1293, 701, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 707, 1103, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 732, 964, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1051, 956, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1076, 1132, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1290, 929, 0, 0 },
    { 1, 0, 0x40, 2, 0x4B, 1, 0x4B, 0x56, 0x10, 0, 871, 187, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 1, 0x57, 0x62, 0x10, 0, 256, 462, 0, 0 },
    { 1, 0, 0xFF, 2, 0x1A, 0, 0, 0, 0, 0, 1280, 250, 0, 0 },
    { 1, 0, 0x80, 2, 0x2D, 0, 0, 0, 0, 0, 640, 640, 0, 0 },
    { 1, 0, 0xFF, 2, 0x4A, 0, 0, 0, 0, 0, 1152, 640, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 1127, 1030, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 1, 0x1B, 0x1D, 0xA, 0, 548, 1332, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 1, 0x1B, 0x1D, 0xA, 0, 718, 1206, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 1, 0x1B, 0x1D, 0xA, 0, 1060, 992, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 1, 0x1B, 0x1D, 0xA, 0, 1185, 379, 0, 0 },
    { 1, 0, 0x40, 6, 0x1B, 1, 0x1B, 0x1D, 0xA, 0, 1201, 1022, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 308, 945, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 399, 792, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 716, 561, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 733, 801, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 774, 561, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 778, 921, 0, 0 },
    { 1, 0, 0x40, 6, 0x1E, 1, 0x1E, 0x20, 0xA, 0, 1397, 1163, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 807, 1209, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 849, 1138, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1038, 926, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1111, 887, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1208, 1014, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1294, 541, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1343, 315, 0, 0 },
    { 1, 0, 0x40, 6, 0x21, 1, 0x21, 0x23, 0xA, 0, 1377, 734, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 190, 620, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 256, 1260, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 271, 861, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 354, 1089, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 467, 1200, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 544, 925, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 550, 1203, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 582, 1143, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 884, 148, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 923, 1263, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 1068, 1156, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 1221, 1220, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 189, 608, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 272, 404, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 383, 545, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 507, 1100, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 647, 505, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 743, 998, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 806, 518, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 858, 817, 0, 0 },
    { 1, 0, 0x40, 6, 0x24, 1, 0x24, 0x26, 0xA, 0, 961, 743, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 265, 1270, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 337, 461, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 388, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 599, 726, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 658, 358, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 660, 285, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 714, 1139, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 765, 97, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 809, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 874, 1029, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 893, 157, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 928, 871, 0, 0 },
    { 1, 0, 0x40, 6, 0x27, 1, 0x27, 0x29, 0xA, 0, 1225, 216, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 429, 1292, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 508, 1256, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 601, 1307, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 609, 1298, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 740, 1203, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 827, 1044, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 839, 1046, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 904, 1256, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 957, 1156, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 959, 1149, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1020, 810, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1028, 1041, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1044, 1105, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1064, 919, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1065, 831, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1110, 1001, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1155, 1152, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1168, 747, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1172, 739, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1183, 385, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1184, 749, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1224, 1022, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1273, 1198, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1357, 1139, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1378, 723, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1399, 723, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1428, 1151, 0, 0 },
    { 1, 0, 0x40, 6, 0x2A, 1, 0x2A, 0x2C, 0xA, 0, 1508, 1083, 0, 0 },
    { 1, 0, 0xFF, 6, 0x63, 0, 0, 0, 0, 0, 280, 280, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 31, 277, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 59, 658, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 215, 936, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 258, 235, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 277, 1117, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 293, 769, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 321, 1004, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 329, 1232, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 336, 615, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 349, 501, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 426, 720, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 444, 1104, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 478, 1313, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 500, 1150, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 516, 860, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 522, 188, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 546, 1010, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 592, 274, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 627, 1062, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 628, 1190, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 647, 1284, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 658, 794, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 675, 223, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 675, 500, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 689, 925, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 695, 359, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 752, 518, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 765, 187, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 768, 1184, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 787, 97, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 819, 1001, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 831, 586, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 863, 772, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 909, 1064, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 928, 561, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 951, 1243, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 996, 869, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 996, 869, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1010, 95, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1012, 1165, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1077, 1061, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1121, 928, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1126, 178, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1136, 400, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1155, 305, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1210, 1101, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1247, 712, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1319, 451, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1354, 1028, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1362, 347, 0, 0 },
    { 1, 0, 0x40, 0xA, 6, 1, 6, 9, 0xA, 0, 1396, 620, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 64, 753, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 144, 413, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 146, 568, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 193, 175, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 218, 836, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 257, 1190, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 261, 651, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 274, 1049, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 410, 1279, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 424, 842, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 425, 1235, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 449, 1160, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 460, 932, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 475, 1012, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 493, 701, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 660, 735, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 725, 327, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 728, 1128, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 732, 984, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 976, 249, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1007, 556, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1083, 1286, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1087, 762, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1190, 127, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1232, 1060, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1352, 221, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1453, 531, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 1478, 397, 0, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 1041, 566, 603, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 1133, 585, 596, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 862, 659, 666, 0 },
    { 1, 0, 0x80, 4, 0xD, 0, 0, 0, 0, 0, 1022, 313, 367, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 693, 475, 479, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 661, 583, 589, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 1029, 891, 899, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 1011, 936, 945, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 321, 1025, 1031, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 821, 1047, 1057, 0 },
    { 1, 0, 0x40, 4, 0x14, 0, 0, 0, 0, 0, 261, 1055, 1060, 0 },
    { 1, 0, 0x40, 4, 0x15, 0, 0, 0, 0, 0, 612, 1088, 1097, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 993, 1122, 1127, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 804, 1152, 1160, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 401, 1225, 1231, 0 },
    { 1, 0, 0x40, 4, 0x19, 0, 0, 0, 0, 0, 608, 1249, 1255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 496, 807, 807, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 951, 951, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 560, 743, 743, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 577, 359, 359, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 607, 768, 768, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 704, 847, 847, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 800, 847, 847, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1072, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 447, 447, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1136, 487, 487, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1152, 527, 527, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1175, 441, 441, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1184, 559, 559, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1200, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1248, 495, 495, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1303, 605, 605, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1312, 431, 431, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1344, 623, 623, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23B, 0x6C8, 0x1D4, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x234, 0x140, 0x350, 5, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 3, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0xE, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x28, 0, 0, 0, 0, 0 },
    { { { PROGRESS(7), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0xAA, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 170, script170, EVENT_TEXT(8), NULL, endEvent170 },
    { 171, script171, EVENT_TEXT(9), NULL, endEvent171 },
    { 240, script240, EVENT_TEXT(0xF), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
