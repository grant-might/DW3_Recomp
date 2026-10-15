/*
 * WSTAG815: Gunslinger 2F, Spy Satellite. Lord Megadeath waits beyond its Warp
 * Gate.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern StageEffectSpot updateStageSpots[];

/* The files of the background, which the versions number differently */
#if VERSION_US || VERSION_JP
#define BG_ARCHIVE 0x767
#define BG_FILE 0x766
#define BG_FILE2 0x887
#elif VERSION_EU
#define BG_ARCHIVE 0x776
#define BG_FILE 0x775
#define BG_FILE2 0x898
#endif

/* Draws the two background images (file BG_ARCHIVE) scrolled at 2/3 of the layer's scroll */
void drawBackground(StageTask *task) {
    SpriteDrawer drawer;
    Vec2 scroll;
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);

    layer->getScroll(layer, &scroll);
    scroll.x = (scroll.x << 8) / 384;
    scroll.y = (scroll.y << 8) / 384;
    initSpriteDrawer(&drawer);
    drawer.setLayerId(FIELD_LAYER_MAP, 7);
    drawer.setTexture(0x280, 0);
    drawer.setAltClut(0, 0xF0);
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 2), 0, scroll.x, scroll.y);
    drawer.setTexture(0x280, 0x100);
    drawer.setAltClut(0, 0x1F0);
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 3), 0, scroll.x, scroll.y + 0x100);
}

/* Loads the background images (files BG_FILE and BG_FILE2) to VRAM, then draws them every frame */
void updateBackground(StageTask *task) {
    TimLoader loader;

    switch (task->state) {
    case TASK_INIT:
    default:
        initTimLoader(&loader);
        loader.setImagePos(0x280, 0);
        loader.setClutPos(0, 0xF0);
        loader.loadArchive(FILE_CACHE.load(BG_FILE));
        loader.setImagePos(0x280, 0x100);
        loader.setClutPos(0, 0x1F0);
        loader.loadArchive(FILE_CACHE.load(BG_FILE2));
        task->nextState(task);
        break;
    case TASK_RUN:
        drawBackground(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the spy satellite's background (updateBackground) */
void *createBackground(void) {
    return createTask(updateBackground, 0x60, 0);
}

/* Creates the background, the stage's five effects and the event object of flags 0x4072/0x40A9 */
void updateStage(StageTask *task, void **children) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createBackground();
        for (i = 0; i < 5; i++) {
            if (updateStageSpots[i].kind == 0) {
                children[i + 1] = createStageEffect(updateStageSpots[i].x, updateStageSpots[i].y, updateStageSpots[i].frame);
            }
        }
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x72), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0xA9), 0)) {
            children[6] = FIELDSTG_startEvent(0x411);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the stage task (id 0x17) and calls the first function of its table */
StageTask *startStage(void *owner) {
    StageTask *task = createTaskWithId(updateStage, sizeof(StageTask), 0x1C, FIELD_TASK_LAUNCHER);

    task->owner = owner;
    stageFuncs[0]();
    return task;
}

#include "common/step_animation.inc.c"
#include "common/draw_stage_effect.inc.c"
#include "common/is_on_screen.inc.c"
#include "common/update_stage_effect.inc.c"
#include "common/create_stage_effect.inc.c"

/*
 * After event 1020, with Metal Soldier: starts the stage's event battle 0
 * and keeps it from playing again
 */
void endEvent1020(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x70), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1030, with Metal Soldier: starts the stage's event battle 0
 * and keeps it from playing again
 */
void endEvent1030(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x71), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 1040, with the player: keeps it from playing again */
void endEvent1040(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x72), 1);
}

/* After event 1041, with the player: keeps it from playing again */
void endEvent1041(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xA9), 1);
}

/*
 * After event 1050, with the player: starts the stage's event battle 1 and
 * keeps it from playing again
 */
void endEvent1050(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x73), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x767
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x151
#define STAGE_FILE 0x776
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0026 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x3D400, 0x33B00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1C;
    FIELDSTG_state.music = MUSIC(0x1C, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1020[] = {
    SCRIPT_WALK(2, 0x160, 0x2CB, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xD0),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0xD0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xD0, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x150, 0x2C4, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script1030[] = {
    SCRIPT_WALK(2, 0x3D0, 0x103, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0xD1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xD1, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x3C0, 0xFB, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script1040[] = {
    SCRIPT_WALK(2, 0x27D, 0x1F9, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHAKE_CAMERA, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0xB4),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_CAMERA_SHAKE, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHAKE_CAMERA, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_CAMERA_SHAKE, 2),
#if VERSION_US
    SCRIPT_LEAVE(0xE08, 0x27D, 0x1F9, 1),
#elif VERSION_EU || VERSION_JP
    SCRIPT_LEAVE(0xE09, 0x27D, 0x1F9, 1),
#endif
    SCRIPT_END,
};
s16 script1041[] = {
    SCRIPT_PLACE(2, 0x27D, 0x1F9),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1050[] = {
    SCRIPT_WALK(2, 0x17D, 0x119, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHAKE_CAMERA, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0xB4),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_CAMERA_SHAKE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
StageEffectSpot updateStageSpots[] = {
    { 60, 0, 0x318, 166 },
    { 60, 0, 0x3F8, 0x116 },
    { 60, 0, 168, 0x26E },
    { 60, 0, 0x188, 0x2DE },
    { 44, 0, 0x3F8, 0x2C6 },
};
AnimFrame effectClutFrames[] = {
    { 0, 6 }, { 1, 6 }, { 2, 68 }, { 1, 4 },
    { 0, 4 }, { 255, 0 },
};
AnimFrame effectFrames[] = {
    { 0x12C, 18 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 5, 4 },
    { 6, 4 }, { 7, 4 }, { 5, 4 }, { 6, 4 },
    { 7, 4 }, { 4, 4 }, { 3, 4 }, { 2, 4 },
    { 1, 4 }, { 255, 0x3E7 },
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
AreaBattle area3Battle0 = { 194, 17, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 144, 17, MUSIC(2, 0) };
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
    { 125, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1CD, 0x182, 0x234, 0x82, 0x160, 0x1FC },
    { 0x1C0, 0x100, 0x1CD, 0x15A, 0x234, 0x5A, 0x170, 0x1FC },
};
u16 actor0Conditions[] = { FLAG(0x40, 0x70), 0, PROGRESS(0x28), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x28), 1, FLAG(0x40, 0x71), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0xD0, 4, 302, 691, 7 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0xD1, 5, 927, 235, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0xFF, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0xF9, 6, 3, 0, 0, 0, 0, 0, 853, 643, 643, 0 },
    { 1, 0, 0xFF, 6, 4, 0, 0, 0, 0, 0, 124, 99, 0, 0 },
    { 1, 0, 0xD6, 6, 1, 0, 0, 0, 0, 0, 341, 195, 0, 0 },
    { 1, 0, 0xD6, 6, 1, 0, 0, 0, 0, 0, 469, 307, 0, 0 },
    { 1, 0, 0xD6, 6, 1, 0, 0, 0, 0, 0, 597, 419, 0, 0 },
    { 1, 0, 0xD6, 6, 1, 0, 0, 0, 0, 0, 725, 531, 0, 0 },
    { 1, 0, 0xFF, 6, 2, 0, 0, 0, 0, 0, 156, 617, 617, 0 },
    { 1, 0, 0xFF, 6, 2, 0, 0, 0, 0, 0, 780, 161, 161, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2DF, 0x310, 0x228, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DD, 0x504, 0x296, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DD, 0x344, 0xF6, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DD, 0x29C, 0x1AA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DD, 0x3E4, 0x146, 5, 0, 0, 0 },
    { { { FLAG(0x40, 0x70), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x3FC, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x71), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x406, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x72), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x410, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x73), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x41A, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1020, script1020, EVENT_TEXT(9), NULL, endEvent1020 },
    { 1030, script1030, EVENT_TEXT(0xA), NULL, endEvent1030 },
    { 1040, script1040, EVENT_TEXT(0xC), NULL, endEvent1040 },
    { 1041, script1041, EVENT_TEXT(0xD), NULL, endEvent1041 },
    { 1050, script1050, EVENT_TEXT(0xE), NULL, endEvent1050 },
    { -1, NULL, 0, NULL, NULL },
};
