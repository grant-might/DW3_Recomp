/*
 * WSTAG810: Gunslinger 1F, Spy Satellite. Reached from Amaterasu's A.o.A
 * Headquarters (WSTAG311); its blocks open from panels.
 */

#include "common.h"
#include "field/stage.h"
#if VERSION_US || VERSION_JP
#define STAGE_FILE 0x6DD
#elif VERSION_EU
#define STAGE_FILE 0x6ED
#endif

/* Defined below, after the code that uses them */
extern AnimFrame updateTilePairNFrames0[];
extern AnimFrame updateTilePairNFrames2[];
extern AnimFrame updateTilePairNFrames1[];
extern StageEffectSpot updateStageSpots[];
extern StageSlot stageSlots0[];
extern StageSlot stageSlots1[];

#include "common/step_tile_animation_u8.inc.c"

/* Shows the first map object animated while running; when done, both */
void updateTilePairN(StageTilePairN *task) {
    StageTile *tile;
    StageTile *object;
    s32 i;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == task->anim) {
                task->anims[0].anim.index = 0;
                task->anims[0].anim.timer = (u8)updateTilePairNFrames0[0].duration;
                task->anims[0].tile = object;
            }
            if (object->anim == task->anim + 1) {
                task->anims[1].anim.index = 0;
                task->anims[1].anim.timer = (u8)updateTilePairNFrames1[0].duration;
                task->anims[1].tile = object;
            }
        }
        task->playing = 0;
        if (task->done == 1) {
            task->playing = 1;
            task->anims[0].anim.index = 0;
            task->anims[0].anim.timer = (u8)updateTilePairNFrames2[0].duration;
            task->setState(task, TASK_DONE);
        } else {
            task->nextState(task);
        }
        break;
    case TASK_RUN:
        for (i = 0; i < 2; i++) {
            tile = task->anims[i].tile;
            switch (i) {
            case 0:
                tile->visible = 1;
                tile->frame = 0x55;
                tile->clutRow = stepTileAnimation(&task->anims[0], updateTilePairNFrames0, 0, 0);
                break;
            case 1:
                tile->visible = 0;
                break;
            }
        }
        break;
    case TASK_DONE:
        for (i = 0; i < 2; i++) {
            tile = task->anims[i].tile;
            switch (i) {
            case 0:
                tile->visible = 1;
                if (task->playing) {
                    frame = stepTileAnimation(&task->anims[0], updateTilePairNFrames2, 1, 0);
                    if (frame == 0xFF) {
                        tile->frame = 0x54;
                        task->playing = 0;
                    } else {
                        tile->frame = frame;
                    }
                } else {
                    tile->frame = 0x54;
                }
                tile->clutRow = 0;
                break;
            case 1:
                tile->visible = 1;
                tile->frame = 0x46;
                tile->clutRow = stepTileAnimation(&task->anims[1], updateTilePairNFrames1, 0, 0);
                break;
            }
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Plays the first map object's one-shot animation when the event of map object 0x35B happens */
void handleCommands842To844(StageTilePairN *task, s32 id) {
    if (id == 0x35B) {
        task->playing = 1;
        task->anims[0].anim.index = 0;
        task->anims[0].anim.timer = (u8)updateTilePairNFrames2[0].duration;
        task->setState(task, TASK_DONE);
    }
}

/* Creates the tile pair object of animations ANIM and ANIM + 1 with the id ARG */
static inline void *createTilePairN(s32 arg, s32 anim) {
    StageTilePairN *task = createTaskWithId(updateTilePairN, 0x68, 0, arg);

    task->anim = anim;
    task->done = 0;
    return task;
}

/* Creates the tile pair object of animations 1 and 2 */
void *createTilePairN1(s32 arg) {
    return createTilePairN(arg, 1);
}

/* Creates the tile pair object of animations 3 and 4 */
void *createTilePairN3(s32 arg) {
    return createTilePairN(arg, 3);
}

/* Creates the tile pair object of animations 5 and 6 */
void *createTilePairN5(s32 arg) {
    return createTilePairN(arg, 5);
}

/* Creates the tile pair object of animations anim and anim + 1, started in TASK_DONE */
void *createTilePairNDone(s32 anim) {
    StageTilePairN *task = createTask(updateTilePairN, 0x68, 0);

    task->anim = anim;
    task->done = 1;
    return task;
}

/* Creates the stage's 62 effects and its three tile pairs, started by flags 0x406E, 0x406F and 0x406D */
void updateStage(StageTask *task, void **children) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        for (i = 0; i < 62; i++) {
            if (updateStageSpots[i].kind == 0) {
                children[i] = createStageEffect(updateStageSpots[i].x, updateStageSpots[i].y, updateStageSpots[i].frame);
            }
        }
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x6E), 0)) {
            children[62] = createTilePairN1(0x34C);
        } else {
            children[62] = createTilePairNDone(1);
        }
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x6F), 0)) {
            children[63] = createTilePairN3(0x34B);
        } else {
            children[63] = createTilePairNDone(3);
        }
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x6D), 0)) {
            children[64] = createTilePairN5(0x34A);
        } else {
            children[64] = createTilePairNDone(5);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x104
#include "common/start_stage.inc.c"
#include "common/step_animation.inc.c"
#include "common/draw_stage_effect.inc.c"
#include "common/is_on_screen.inc.c"
#include "common/update_stage_effect.inc.c"
#include "common/create_stage_effect.inc.c"

/* Puts back the first triggers map when GAME.altTriggers is set, and clears it */
void *startEvent9000(void) {
    if (GAME.altTriggers != 0) {
        FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
        FIELDSTG_state.slots = stageSlots0;
        GAME.altTriggers = 0;
    }
    return NULL;
}

/* Switches to the second triggers map when GAME.altTriggers is clear, and sets it */
void *startEvent9001(void) {
    if (GAME.altTriggers == 0) {
        FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 4);
        FIELDSTG_state.slots = stageSlots1;
        GAME.altTriggers = 0x20;
    }
    return NULL;
}

/*
 * After event 990, with the player: sets flag 0x1C3A (which decides whether
 * actor 0 appears) and keeps it from playing again
 */
void endEvent990(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x6D), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 0x3A), 1);
}

/*
 * After event 1000, with the player: sets flag 0x1C3B (which decides whether
 * actor 1 appears) and keeps it from playing again
 */
void endEvent1000(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x6E), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 0x3B), 1);
}

/*
 * After event 1010, with the player: sets flag 0x1C3C (which decides whether
 * actor 2 appears) and keeps it from playing again
 */
void endEvent1010(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x6F), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 0x3C), 1);
}

/* After event 1035, with the player: moves the story on to its step 0x29 */
void endEvent1035(void) {
    GAME.progress = 0x29;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x14A
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x151
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0026,
 * with the first triggers map when the player comes in, and the second one
 * kept when back from a battle or a menu that left it in use
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x31000, 0x1F800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1C;
    FIELDSTG_state.music = MUSIC(0x1C, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.clearTempFlags != 0 || GAME.altTriggers == 0) {
        GAME.altTriggers = 1;
        startEvent9000();
    } else {
        GAME.altTriggers = 0;
        startEvent9001();
    }
}

s16 script990[] = {
    SCRIPT_WALK(2, 0x530, 0x140, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH03, 2),
    SCRIPT_COMMAND(0x34A, 0x35B, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x180, 0, 0),
    SCRIPT_POSE(0x180, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_TRAP_OFF, 2),
    SCRIPT_WAIT(0x5A),
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
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x510, 0x151, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script1000[] = {
    SCRIPT_WALK(2, 0x390, 0x90, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH03, 2),
    SCRIPT_COMMAND(0x34C, 0x35B, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x181, 0, 0),
    SCRIPT_POSE(0x181, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_TRAP_OFF, 2),
    SCRIPT_WAIT(0x5A),
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
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x370, 0xA0, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script1010[] = {
    SCRIPT_WALK(2, 0x510, 0x90, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH03, 2),
    SCRIPT_COMMAND(0x34B, 0x35B, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x182, 0, 0),
    SCRIPT_POSE(0x182, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_TRAP_OFF, 2),
    SCRIPT_WAIT(0x5A),
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
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x50F, 0x91, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script1035[] = {
    SCRIPT_WALK(2, 0xA0, 0x2D9, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AnimFrame updateTilePairNFrames0[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 255, 0 },
};
AnimFrame updateTilePairNFrames2[] = {
    { 71, 4 }, { 72, 4 }, { 73, 4 }, { 74, 4 },
    { 75, 4 }, { 76, 4 }, { 77, 4 }, { 78, 4 },
    { 79, 4 }, { 80, 4 }, { 81, 4 }, { 82, 4 },
    { 83, 4 }, { 84, 4 }, { 255, 0 },
};
AnimFrame updateTilePairNFrames1[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 6 }, { 255, 0 },
};
StageEffectSpot updateStageSpots[] = {
    { 60, 0, 92, 181 },
    { 60, 0, 92, 245 },
    { 60, 0, 92, 0x195 },
    { 52, 0, 92, 0x1D5 },
    { 60, 0, 156, 213 },
    { 52, 0, 220, 245 },
    { 60, 0, 220, 0x135 },
    { 44, 0, 236, 0x2EC },
    { 44, 0, 252, 0x395 },
    { 60, 0, 0x11C, 149 },
    { 60, 0, 0x11C, 0x1D5 },
    { 44, 0, 0x11C, 0x324 },
    { 60, 0, 0x12C, 0x2CC },
    { 60, 0, 0x13C, 0x3B5 },
    { 60, 0, 0x15C, 181 },
    { 60, 0, 0x15C, 0x344 },
    { 60, 0, 0x17C, 133 },
    { 52, 0, 0x19C, 0x1D5 },
    { 60, 0, 0x1BC, 0x1A5 },
    { 60, 0, 0x1DC, 0x115 },
    { 60, 0, 0x1FC, 0x225 },
    { 52, 0, 0x21C, 0x1D5 },
    { 60, 0, 0x23C, 133 },
    { 60, 0, 0x23C, 0x245 },
    { 60, 0, 0x27C, 0x265 },
    { 60, 0, 0x29C, 0x195 },
    { 60, 0, 0x2BC, 133 },
    { 60, 0, 0x2BC, 0x285 },
    { 60, 0, 0x2DC, 0x175 },
    { 60, 0, 0x2EC, 0x41D },
    { 60, 0, 0x2FC, 0x2A5 },
    { 60, 0, 0x31C, 245 },
    { 60, 0, 0x33C, 0x225 },
    { 52, 0, 0x35C, 0x1B5 },
    { 60, 0, 0x36C, 0x2BD },
    { 52, 0, 0x36C, 0x2FD },
    { 60, 0, 0x36C, 0x33D },
    { 60, 0, 0x3AC, 0x35D },
    { 60, 0, 0x3BC, 0x145 },
    { 60, 0, 0x3BC, 0x1A5 },
    { 60, 0, 0x3CC, 0x38D },
    { 52, 0, 0x3EC, 0x33D },
    { 60, 0, 0x3EC, 0x3BD },
    { 60, 0, 0x3FC, 0x2A5 },
    { 52, 0, 0x42C, 0x39D },
    { 52, 0, 0x43C, 133 },
    { 52, 0, 0x43C, 0x245 },
    { 60, 0, 0x44C, 0x40D },
    { 60, 0, 0x45C, 0x1F5 },
    { 60, 0, 0x48C, 0x34D },
    { 60, 0, 0x49C, 0x1B5 },
    { 52, 0, 0x4DC, 245 },
    { 52, 0, 0x4DC, 0x1F5 },
    { 60, 0, 0x4DC, 0x295 },
    { 60, 0, 0x4EC, 0x35D },
    { 60, 0, 0x4EC, 0x41D },
    { 52, 0, 0x51C, 0x1B5 },
    { 60, 0, 0x51C, 0x235 },
    { 60, 0, 0x51C, 0x2B5 },
    { 60, 0, 0x51C, 0x2F5 },
    { 52, 0, 108, 0x2DC },
    { 44, 0, 204, 0x40D },
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
AreaBattle area0Battle0 = { 185, 17, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 185, 17, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 185, 17, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 135, 17, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 135, 17, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 135, 17, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 140, 17, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 140, 17, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
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
    { 124, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x152, 0x1B1, 0x48, 0xB1, 0x170, 0x1F8 },
    { 0x140, 0x100, 0x162, 0x1B1, 0x88, 0xB1, 0x140, 0x1F7 },
    { 0x180, 0x100, 0x180, 0x100, 0x100, 0, 0x150, 0x1F7 },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x348 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x348 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x348 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x1C, 0x3A), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(0x1C, 0x3B), 0, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1C, 0x3C), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x180, 4, 369, 912, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x181, 5, 400, 800, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x182, 6, 208, 704, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 2, 0x64, 6, 0x46, 0, 0, 0, 0, 0, 910, 80, 0, 0 },
    { 1, 4, 0x64, 6, 0x46, 0, 0, 0, 0, 0, 1295, 80, 0, 0 },
    { 1, 6, 0x64, 6, 0x46, 0, 0, 0, 0, 0, 1327, 256, 0, 0 },
    { 1, 1, 0x64, 6, 0x47, 0, 0, 0, 0, 0, 910, 80, 0, 0 },
    { 1, 3, 0x64, 6, 0x47, 0, 0, 0, 0, 0, 1295, 80, 0, 0 },
    { 1, 5, 0x64, 6, 0x47, 0, 0, 0, 0, 0, 1327, 256, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 26, 975, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 47, 138, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 123, 576, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 125, 877, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 195, 656, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 231, 32, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 307, 580, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 431, 874, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 486, 102, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 532, 732, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 549, 1020, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 595, 820, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 615, 349, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 668, 33, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 713, 235, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 738, 837, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 890, 262, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 1037, 46, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 1054, 329, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 1108, 403, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0, 0, 0, 0, 0, 0, 1354, 1060, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots0[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x314, 0x41E, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x13C, 0x3B6, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x84, 0x196, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x44C, 0x40E, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x4C4, 0x1B6, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x84, 0x10A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x3CC, 0x3A2, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x15C, 0x346, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x33C, 0x23A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x3EC, 0x3D2, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x224, 0x226, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x3AC, 0x372, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x2E4, 0x286, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x264, 0x246, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x11C, 0x1D6, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x324, 0x2A6, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x51C, 0x24A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x2A4, 0x266, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DE, 0x189, 0x2DF, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DE, 0xCE, 0x283, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x394, 0x33E, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x12C, 0x2E2, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DE, 0x3F8, 0x117, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DE, 0x33E, 0xBA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x110, 0x3A0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x130, 0x330, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x2DE, 0x40C, 0x2D2, 1, 0, 0, 0 },
    { { { FLAG(0x40, 0x6D), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x3DE, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x6E), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x3E8, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x6F), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x3F2, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0x28), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x40B, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2329, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots1[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x11C, 0xAA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x394, 0x2D2, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x48C, 0x34E, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x45C, 0x20A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x3FC, 0x2A6, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x184, 0xCA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x204, 0x12A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x104, 0x14A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x51C, 0x2CA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x1BC, 0x1A6, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x51C, 0x30A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x84, 0xB6, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x23C, 0x9A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0xC4, 0xD6, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x3E4, 0x1BA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x2BC, 0x9A, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x4EC, 0x372, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x1A4, 0x9A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x4EC, 0x41E, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x304, 0x18A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH, 0x2DD, 0x100, 0x2F8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 990, script990, EVENT_TEXT(6), NULL, endEvent990 },
    { 1000, script1000, EVENT_TEXT(7), NULL, endEvent1000 },
    { 1010, script1010, EVENT_TEXT(8), NULL, endEvent1010 },
    { 1035, script1035, EVENT_TEXT(0xB), NULL, endEvent1035 },
    { 9000, NULL, 0, startEvent9000, NULL },
    { 9001, NULL, 0, startEvent9001, NULL },
    { -1, NULL, 0, NULL, NULL },
};
