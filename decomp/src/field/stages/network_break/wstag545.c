/*
 * WSTAG545: Bug Maze Pit, Network Break.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateTileAtFrames[];
extern AnimFrame updateTileTaskFrames[];
extern AnimFrame *updateTileSwitchFrames[3];
extern StageTileAtSpot updateTileSwitchSpots[];

/* Steps an animation, holding its last frame, and returns the frame */
s32 stepHeldAnimation(StageTileAnim *obj, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[obj->anim.index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        obj->anim.timer -= dt;
    }
    if (obj->anim.timer <= 0) {
        frame++;
        obj->anim.index++;
        obj->anim.timer += frame->duration;
        if (frame->frame == 0xFF) {
            obj->anim.index--;
            frame = &frames[obj->anim.index];
            obj->anim.timer += frame->duration;
        }
        stepHeldAnimation(obj, frames, depth + 1);
    }
    return frame->frame;
}

/* Moves the map object with the given animation to (x, y); hidden while running, shown and animated when done */
void updateTileAt(StageTileAt *task) {
    StageTile *object;
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->obj.anim.index = 0;
        task->obj.anim.timer = updateTileAtFrames[0].duration;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == task->anim) {
                task->obj.tile = object;
                object->x = task->x;
                object->y = task->y;
            }
        }
        break;
    case TASK_RUN:
        task->obj.tile->visible = 0;
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->obj.anim.index = 0;
            task->obj.anim.timer = updateTileAtFrames[0].duration;
            task->setSubstate(task, 1);
        }
        tile = task->obj.tile;
        tile->visible = 1;
        tile->frame = stepHeldAnimation(&task->obj, updateTileAtFrames, 0);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates a StageTileAt for the map object with the given animation */
StageTileAt *createTileAt(s32 anim, s32 x, s32 y) {
    StageTileAt *task = createTask(updateTileAt, sizeof(StageTileAt), 0);

    task->anim = anim;
    task->x = x;
    task->y = y;
    return task;
}

#include "common/step_animation_once.inc.c"

/* Shows the map object with animation 1 (frame 0x18); when done, animates it once with a sound and goes back to TASK_RUN */
void updateTileTask(StageTileTask *task) {
    StageTile *object;
    StageTile *shown;
    StageTile *tile;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->obj.anim.index = 0;
        task->obj.anim.timer = updateTileTaskFrames[0].duration;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == 1) {
                task->obj.tile = object;
            }
        }
        break;
    case TASK_RUN:
        shown = task->obj.tile;
        shown->visible = 1;
        shown->frame = 0x18;
        shown->clutRow = 9;
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->obj.anim.index = 0;
            task->obj.anim.timer = updateTileTaskFrames[0].duration;
            task->setSubstate(task, 1);
            SOUND.playSound(SOUND_FLOOR_LT);
        }
        tile = task->obj.tile;
        frame = stepAnimationOnce(&task->obj, updateTileTaskFrames, 0);
        if (frame == 0xFF) {
            task->setState(task, TASK_RUN);
            tile->clutRow = 9;
        } else {
            tile->clutRow = frame;
        }
        tile->visible = 1;
        tile->frame = 0x18;
        break;
    case TASK_KILL:
        break;
    }
}

/*
 * Command 847 of the event scripts: told 0x35A in event 710 (Bulbmon's
 * earthquake), ends its task
 */
void handleCommand847(StageTask *task, s32 id) {
    if (task != NULL && id == 0x35A) {
        task->setState(task, TASK_DONE);
    }
}

/*
 * Creates the map object that event 710, Bulbmon's earthquake, sets off
 * (updateTileTask), with an id
 */
void *createTileTask(s32 arg) {
    return createTaskWithId(updateTileTask, sizeof(StageTileTask), 0, arg);
}

#include "common/step_tile_animation.inc.c"

/*
 * Plays the switch's animation N once (from its start, on step 0), then
 * holds frame DONE and goes on to substate NEXT
 */
static inline void playSwitchOnce(StageTileSwitch *task, StageTile *tile, s32 n, s32 done, s32 next) {
    s32 frame;

    if (task->step == 0) {
        task->obj.anim.index = 0;
        task->obj.anim.timer = updateTileSwitchFrames[n][0].duration;
        task->setStep(task, 1);
    }
    frame = stepTileAnimation(&task->obj, updateTileSwitchFrames[n], 1, 0);
    tile->visible = 1;
    if (frame == 0xFF) {
        tile->frame = done;
        task->setSubstate(task, next);
    } else {
        tile->frame = frame;
    }
}

/*
 * The switch, the map object with animation 2, played by substate (events
 * 0x35D to 0x35F); also sets the child that events 0x360 to 0x364 pick to
 * TASK_DONE
 */
void updateTileSwitch(StageTileSwitch *task, StageTileAts *children) {
    StageTile *object;
    StageTile *tile;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == 2) {
                task->obj.anim.index = 0;
                task->obj.anim.timer = updateTileSwitchFrames[0][0].duration;
                task->obj.tile = object;
            }
        }
        for (i = 0; i < 5; i++) {
            children->tiles[i] = createTileAt(updateTileSwitchSpots[i].anim, updateTileSwitchSpots[i].x, updateTileSwitchSpots[i].y);
        }
        task->silent = 0;
        task->spawn = 0;
        task->nextState(task);
        break;
    case TASK_RUN:
        tile = task->obj.tile;
        switch (task->substate) {
        case 0:
            tile->visible = 0;
            break;
        case 1:
            playSwitchOnce(task, tile, 0, 8, 2);
            break;
        case 3:
            playSwitchOnce(task, tile, 1, 9, 4);
            break;
        case 5:
            if (task->step == 0) {
                task->obj.anim.index = 0;
                task->obj.anim.timer = updateTileSwitchFrames[2][0].duration;
                task->setStep(task, 1);
                SOUND.playSound(SOUND_BULB_002);
            }
            {
                s32 frame = stepTileAnimation(&task->obj, updateTileSwitchFrames[2], 0, 0);

                tile->visible = 1;
                tile->frame = frame;
            }
            break;
        case 2:
        case 4:
            break;
        }
        if (task->spawn != 0) {
            children->tiles[task->spawn - 1]->setState(children->tiles[task->spawn - 1], TASK_DONE);
            task->spawn = 0;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Events 0x35D to 0x364: animate the map object (substates 1, 3, 5) or pick a child to set to TASK_DONE */
void handleCommand848(void *arg, s32 id) {
    StageTileSwitch *task = arg;

    if (task != NULL) {
        if (task->silent == 0) {
            SOUND.playSound(SOUND_COMEX114);
        }
        switch (id) {
        case 0x35D:
            task->setSubstate(task, 1);
            break;
        case 0x35E:
            task->setSubstate(task, 3);
            break;
        case 0x35F:
            task->setSubstate(task, 5);
            break;
        case 0x360:
            task->spawn = 1;
            break;
        case 0x361:
            task->spawn = 2;
            break;
        case 0x362:
            task->spawn = 3;
            break;
        case 0x363:
            task->spawn = 4;
            break;
        case 0x364:
            task->spawn = 5;
            break;
        }
        task->setStep(task, 0);
    }
}

/* Creates the task of updateTileSwitch with the given id */
void *createCommand848(s32 id) {
    return createTaskWithId(updateTileSwitch, sizeof(StageTileSwitch), sizeof(StageTileAts), id);
}

/* Creates the stage object of flag 0x4051, and the event object of story progress 0x1A */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x51), 0)) {
            children[0] = createTileTask(0x34F);
        }
        if (GAME.progress == 0x1A && FLAGS_00.checkCondition(FLAG(0x40, 0x51), 1)) {
            children[2] = FIELDSTG_startEvent(0x2C7);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0xC
#include "common/start_stage.inc.c"

/*
 * After event 710, with the player: starts the stage's event battle 0 and
 * keeps it from playing again
 */
void endEvent710(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x51), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 711, with the player: moves the story on to its step 0x1B */
void endEvent711(void) {
    GAME.progress = 0x1B;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x267
#define STAGE_ARCHIVE 0x3CF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x276
#define STAGE_ARCHIVE 0x3DF
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0017 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x18800, 0x24600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x13;
    FIELDSTG_state.music = MUSIC(0x13, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script710[] = {
    SCRIPT_WALK(2, 0x218, 0x1C4, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_COMMAND(0x350, 0x35C, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x268, 0x19C, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LOOK_AT(0, 0x296, 0x184),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHAKE_CAMERA, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_DEMO_BGM, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x34F, 0x35A, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x34F, 0x35A, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x350, 0x360, 2),
    SCRIPT_WAIT(0x48),
    SCRIPT_COMMAND(0x350, 0x361, 2),
    SCRIPT_WAIT(0x24),
    SCRIPT_COMMAND(0x350, 0x362, 2),
    SCRIPT_WAIT(0x18),
    SCRIPT_COMMAND(0x350, 0x363, 2),
    SCRIPT_WAIT(0x30),
    SCRIPT_COMMAND(0x350, 0x364, 2),
    SCRIPT_WAIT(0x60),
    SCRIPT_COMMAND(0x350, 0x35D, 2),
    SCRIPT_WAIT(0x96),
    SCRIPT_COMMAND(0x350, 0x35E, 2),
    SCRIPT_WAIT(0x12),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_CAMERA_SHAKE, 2),
    SCRIPT_WAIT(0x2A),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x350, 0x35F, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script711[] = {
    SCRIPT_LOOK_AT(1, 0x29A, 0x184),
    SCRIPT_PLACE(2, 0x268, 0x19C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x268, 0x168, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x2A8, 0x148, 5),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x245, 0x708, 0x20C, 3),
    SCRIPT_END,
};
AnimFrame updateTileAtFrames[] = {
    { 11, 4 }, { 12, 4 }, { 13, 7 }, { 14, 9 },
    { 15, 10 }, { 16, 8 }, { 17, 4 }, { 18, 4 },
    { 19, 4 }, { 20, 4 }, { 21, 4 }, { 22, 4 },
    { 23, 100 }, { 255, 0 },
};
AnimFrame updateTileTaskFrames[] = {
    { 9, 25 }, { 8, 4 }, { 7, 4 }, { 6, 4 },
    { 5, 4 }, { 4, 4 }, { 3, 6 }, { 2, 7 },
    { 1, 8 }, { 0, 8 }, { 1, 10 }, { 2, 10 },
    { 3, 10 }, { 4, 10 }, { 5, 10 }, { 6, 10 },
    { 7, 10 }, { 8, 10 }, { 255, 0x3E7 },
};
AnimFrame updateTileSwitchFrames_0[] = {
    { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 },
    { 4, 4 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
    { 8, 4 }, { 255, 0x3E7 },
};
AnimFrame updateTileSwitchFrames_1[] = {
    { 10, 4 }, { 9, 10 }, { 255, 0x3E7 },
};
AnimFrame updateTileSwitchFrames_2[] = {
    { 10, 4 }, { 9, 4 }, { 255, 0 },
};
AnimFrame *updateTileSwitchFrames[3] = { updateTileSwitchFrames_0, updateTileSwitchFrames_1, updateTileSwitchFrames_2 };
StageTileAtSpot updateTileSwitchSpots[] = {
    { 0x288, 0x1D7, 3 },
    { 0x32F, 0x170, 4 },
    { 0x250, 0x11F, 5 },
    { 0x307, 0x1C9, 6 },
    { 0x234, 0x171, 7 },
};
AreaBattle area0Battle0 = { 165, 10, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 165, 10, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 173, 10, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 173, 10, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 150, 10, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 92, 10, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 12, 10, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 315, 10, MUSIC(0x22, 0) };
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
    { 61, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1A8, 0x1C8, 0x1A0, 0xC8, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1A0, 0x1C8, 0x180, 0xC8, 0x160, 0x1FC },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x1B), 1, ITEM(5, 0x109), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x264 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x25 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x12C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x26 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x110 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x1B), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 785, 697, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 559, 1145, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 559, 1145, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 559, 1145, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 559, 1145, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x64, 2, 0x63, 0, 0, 0, 0, 0, 529, 465, 0, 0 },
    { 1, 0, 0x64, 2, 0x61, 0, 0, 0, 0, 0, 427, 1040, 0, 0 },
    { 1, 0, 0x64, 2, 0x61, 0, 0, 0, 0, 0, 546, 1044, 0, 0 },
    { 1, 0, 0x64, 2, 0x62, 0, 0, 0, 0, 0, 502, 1051, 0, 0 },
    { 0, 2, 0x80, 4, 0, 0, 0, 0, 0, 0, 701, 371, 371, 0 },
    { 0, 7, 0xF0, 4, 0xB, 0, 0, 0, 0, 0, 564, 369, 369, 0 },
    { 0, 5, 0xF0, 4, 0xB, 0, 0, 0, 0, 0, 592, 287, 287, 0 },
    { 0, 3, 0xF0, 4, 0xB, 0, 0, 0, 0, 0, 648, 471, 471, 0 },
    { 0, 6, 0xF0, 4, 0xB, 0, 0, 0, 0, 0, 775, 457, 457, 0 },
    { 0, 4, 0xF0, 4, 0xB, 0, 0, 0, 0, 0, 815, 368, 368, 0 },
    { 0, 1, 0x50, 4, 0x18, 0, 0, 0, 0, 0, 663, 340, 370, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x245, 0x708, 0x20C, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x245, 0x708, 0x2DC, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x245, 0x708, 0x3BC, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x245, 0x708, 0x50E, 3, 0, 0, 0 },
    { { { PROGRESS(0x1A), 1 }, { FLAG(0x40, 0x51), 0 } }, SLOT_EVENT, 0x2C6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 710, script710, EVENT_TEXT(0x21), NULL, endEvent710 },
    { 711, script711, EVENT_TEXT(0x1F), NULL, endEvent711 },
    { -1, NULL, 0, NULL, NULL },
};
