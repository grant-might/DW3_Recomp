/*
 * WSTAG460: Reliability Spot, South Sector. Where the Qing Long Chief welcomes
 * the player.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateWandererFrames0[];
extern AnimFrame updateWandererFrames1[];
extern u16 wandererSpeeds[];
extern u16 angleTangents[];
extern AnimFrame *updateTileDuoFrames0[];
extern AnimFrame *updateTileDuoFrames1[];
extern u8 tileDuoObjectFrames[2][2];
void *createTileSoloTask(void);
void *createTileDuoTask(void);
void fadeOutWanderer();
extern AnimFrame updateTileSoloFrames0[];
extern AnimFrame updateTileSoloFrames1[];
void fadeOutTileSolo(StageTileSolo *task, s32 command, s32 arg);
void fadeOutTileDuo(StageTileDuo *task, s32 command, s32 arg);
StageWanderer *createWandererTask(s32 tileAnim, s32 speedIndex, s32 start);

/* Creates the tiles and the seven wanderers; in TASK_DONE makes them all hide and goes back to TASK_RUN */
void updateCommand830(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createTileSoloTask();
        children[1] = createTileDuoTask();
        children[2] = createWandererTask(1, 0, 0);
        children[3] = createWandererTask(2, 0, 0);
        children[4] = createWandererTask(3, 0, 0);
        children[5] = createWandererTask(4, 1, 0);
        children[6] = createWandererTask(5, 1, 0);
        children[7] = createWandererTask(6, 1, 0);
        children[8] = createWandererTask(7, 1, 0);
        task->nextState(task);
        break;
    case TASK_RUN:
        break;
    case TASK_DONE:
        fadeOutTileSolo(children[0], 0, 0);
        fadeOutTileDuo(children[1], 0, 0);
        fadeOutWanderer(children[2], 0, 0);
        fadeOutWanderer(children[3], 0, 0);
        fadeOutWanderer(children[4], 0, 0);
        fadeOutWanderer(children[5], 0, 0);
        fadeOutWanderer(children[6], 0, 0);
        fadeOutWanderer(children[7], 0, 0);
        fadeOutWanderer(children[8], 0, 0);
        task->setState(task, TASK_RUN);
        break;
    case TASK_KILL:
        break;
    }
}

/* Sets the task to TASK_DONE when the id is 0x34C */
void handleCommand830(Task *task, s32 id) {
    if (task != NULL && id == 0x34C) {
        task->setState(task, TASK_DONE);
    }
}

/* Creates the task of updateCommand830 with the given id */
void *createCommand830(s32 id) {
    return createTaskWithId(updateCommand830, 0x50, 0x24, id);
}

#include "common/step_tile_animation.inc.c"

/* Shows the map object with animation 10 (animated) while mode isn't 0, and hides it by a one-shot animation wait frames after mode 2 */
void updateTileSolo(StageTileSolo *task) {
    StageTile *object;
    StageTile *tile;
    StageTile *fading;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->tile.anim.index = 0;
        task->tile.anim.timer = updateTileSoloFrames0[0].duration;
        task->tile.anim.index = 0;
        task->tile.anim.timer = updateTileSoloFrames0[0].duration;
        task->mode = 1;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == 10) {
                task->tile.tile = object;
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        tile = task->tile.tile;
        if (task->mode != 0) {
            tile->visible = 1;
            tile->frame = 0x3C;
            tile->clutRow = stepTileAnimation(&task->tile, updateTileSoloFrames0, 0, 0);
        } else {
            tile->visible = 0;
        }
        if (task->mode == 2) {
            if (--task->wait <= 0) {
                task->setState(task, TASK_DONE);
            }
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->tile.anim.index = 0;
            task->tile.anim.timer = updateTileSoloFrames1[0].duration;
            task->setSubstate(task, 1);
        }
        fading = task->tile.tile;
        frame = stepTileAnimation(&task->tile, updateTileSoloFrames1, 1, 0);
        switch (frame) {
        case 0x12C:
            fading->visible = 0;
            break;
        case 0xFF:
            fading->visible = 0;
            task->mode = 0;
            task->setState(task, TASK_RUN);
            break;
        default:
            fading->visible = 1;
            fading->frame = 0x3D;
            fading->clutRow = frame;
            break;
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Makes the StageTileSolo hide in 20 frames; command and arg, a script
   command handler's (ScriptCommand.handle), are 0 from the stage and unread */
void fadeOutTileSolo(StageTileSolo *task, s32 command, s32 arg) {
    if (task != NULL) {
        task->mode = 2;
        task->wait = 0x14;
    }
}

/*
 * Creates the color-cycling map object by the Digi-Egg of Sincerity
 * (updateTileSolo) with an id; nothing in the game calls it
 */
void *createTileSolo(s32 arg) {
    return createTaskWithId(updateTileSolo, sizeof(StageTileSolo), 0, arg);
}

/*
 * Creates the color-cycling map object by the Digi-Egg of Sincerity
 * (updateTileSolo), the one with animation 10
 */
void *createTileSoloTask(void) {
    return createTask(updateTileSolo, sizeof(StageTileSolo), 0);
}

#include "common/step_tile_animation2.inc.c"

/*
 * Cycles the colors of the map objects with animations 8 and 9 by the Digi-
 * Egg of Sincerity while mode isn't 0, and ends wait frames after mode 2
 */
void updateTileDuo(StageTileDuo *task) {
    StageTile *tile;
    StageTile *object;
    StageTile *fading;
    s32 i;
    s32 j;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->tiles[0].anim.index = 0;
        task->tiles[0].anim.timer = updateTileDuoFrames0[0]->duration;
        task->tiles[1].anim.index = 0;
        task->tiles[1].anim.timer = updateTileDuoFrames0[1]->duration;
        task->mode = 1;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            switch (object->anim) {
            case 8:
                task->tiles[0].tile = object;
                break;
            case 9:
                task->tiles[1].tile = object;
                break;
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        for (i = 0; i < 2; i++) {
            tile = task->tiles[i].tile;
            if (task->mode != 0) {
                tile->visible = 1;
                tile->clutRow = stepTileAnimation2(&task->tiles[i], updateTileDuoFrames0[i], 0, 0);
                tile->frame = tileDuoObjectFrames[0][i];
            } else {
                tile->visible = 0;
            }
        }
        if (task->mode == 2) {
            if (--task->wait <= 0) {
                task->setState(task, TASK_DONE);
            }
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->tiles[0].anim.index = 0;
            task->tiles[0].anim.timer = updateTileDuoFrames1[0]->duration;
            task->tiles[1].anim.index = 0;
            task->tiles[1].anim.timer = updateTileDuoFrames1[1]->duration;
            task->setSubstate(task, 1);
        }
        for (j = 0; j < 2; j++) {
            fading = task->tiles[j].tile;
            frame = stepTileAnimation2(&task->tiles[j], updateTileDuoFrames1[j], 1, 0);
            switch (frame) {
            case 0x12C:
                fading->visible = 0;
                break;
            case 0xFF:
                fading->visible = 0;
                task->mode = 0;
                task->setState(task, TASK_RUN);
                break;
            default:
                fading->visible = 1;
                fading->frame = tileDuoObjectFrames[1][j];
                fading->clutRow = frame;
                break;
            }
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Makes the StageTileDuo hide in 150 frames (command and arg as fadeOutTileSolo's) */
void fadeOutTileDuo(StageTileDuo *task, s32 command, s32 arg) {
    if (task != NULL) {
        task->mode = 2;
        task->wait = 0x96;
    }
}

#include "common/create_tile_duo.inc.c"

#include "common/create_tile_duo_task.inc.c"

#include "common/step_tile_animation3.inc.c"

/* Whether the wanderer is more than dist from home (in x + y) */
s32 isFarFromHome(StageWanderer *task, s32 dist) {
    s32 dx = task->posX - task->homeX;
    s32 dy = task->posY - task->homeY;

    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    return dist < dx + dy;
}

#include "common/get_angle.inc.c"

/* Moves the wanderer, turning it every period frames */
void moveWanderer(StageWanderer *task) {
    s32 dx;
    s32 dy;

    task->timer += GFX.funcs.getFrameTime();
    if (task->period < task->timer) {
        if (isFarFromHome(task, 0x1E)) {
            task->angle = ((getAngle(task->homeX - task->posX, task->homeY - task->posY) - 0x40) << 4) & 0xFFF;
        } else {
            task->angle = (RANDOM.next() & 0xFF) << 4;
        }
        task->timer -= task->period;
        task->timer += (RANDOM.next() & 0xF) - 7;
    }
    dx = rsin(task->angle) * task->speed / 4096;
    dy = rcos(task->angle) * task->speed / 4096;
    task->x += dx;
    task->y += dy;
    task->posX = task->x >> 8;
    task->posY = task->y >> 8;
}

/* Wanders; when done, plays the animation of updateWandererFrames1 once and hides */
void updateWanderer(StageWanderer *task) {
    StageTile *tile;
    StageTile *object;
    StageTile *fading;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->tile.anim.index = task->start;
        task->tile.anim.timer = updateWandererFrames0[0].duration;
        task->mode = 1;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == task->tileAnim) {
                task->tile.tile = object;
                task->homeX = object->x;
                task->homeY = object->y;
                task->x = object->x << 8;
                task->y = object->y << 8;
            }
        }
        task->period = 0x28;
        task->timer = task->start;
        task->speed = wandererSpeeds[task->speedIndex];
        task->angle = (RANDOM.next() & 0xFF) << 4;
        task->nextState(task);
        break;
    case TASK_RUN:
        if (task->mode != 0) {
            moveWanderer(task);
        }
        tile = task->tile.tile;
        if (task->mode != 0) {
            tile->visible = 1;
            tile->clutRow = stepTileAnimation3(&task->tile, updateWandererFrames0, 0, 0);
            tile->x = task->posX;
            tile->y = task->posY;
        } else {
            tile->visible = 0;
        }
        if (task->mode == 3) {
            task->speed -= 4;
            if (task->speed <= 0x10) {
                task->setState(task, TASK_DONE);
            }
        }
        if (task->mode == 2) {
            if (--task->wait <= 0) {
                task->mode = 3;
            }
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->tile.anim.index = 0;
            task->tile.anim.timer = updateWandererFrames1[0].duration;
            task->setSubstate(task, 1);
        }
        moveWanderer(task);
        fading = task->tile.tile;
        frame = stepTileAnimation3(&task->tile, updateWandererFrames1, 1, 0);
        switch (frame) {
        case 0x12C:
            fading->visible = 0;
            break;
        case 0xFF:
            fading->visible = 0;
            task->mode = 0;
            task->setState(task, TASK_RUN);
            break;
        default:
            fading->visible = 1;
            fading->clutRow = frame;
            fading->x = task->posX;
            fading->y = task->posY;
            break;
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Makes the wanderer wait 60 frames, then slow down and finish */
void fadeOutWanderer(StageWanderer *task) {
    if (task != NULL) {
        task->mode = 2;
        task->wait = 0x3C;
    }
}

/*
 * Creates one of the map objects that wander by the Digi-Egg of Sincerity
 * (updateWanderer) with an id; nothing in the game calls it
 */
void *createWanderer(s32 arg) {
    return createTaskWithId(updateWanderer, sizeof(StageWanderer), 0, arg);
}

/* Creates a wanderer of the map object with the given animation */
StageWanderer *createWandererTask(s32 tileAnim, s32 speedIndex, s32 start) {
    StageWanderer *task = createTask(updateWanderer, sizeof(StageWanderer), 0);

    task->start = start;
    task->speedIndex = speedIndex;
    task->tileAnim = tileAnim;
    return task;
}

/* Creates the task of updateCommand830 (id 0x33E) before progress 0xF */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (GAME.progress < 0xF) {
            children[0] = createCommand830(0x33E);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/*
 * After event 370, with the player: moves the story on to its step 0xF and
 * gives the player the DE Sincerity
 */
void endEvent370(void) {
    GAME.progress = 0xF;
    FLAGS_00.applyAction(ITEM(0, 0x10), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x354
#define STAGE_ARCHIVE 0x44F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x363
#define STAGE_ARCHIVE 0x45F
#endif
/* Sets the stage up: its map, actors and events, playing BGM_0014 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0xC200, 0x1A500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x10;
    FIELDSTG_state.music = MUSIC(0x10, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script370[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x70, 0x188, 3),
    SCRIPT_PLACE(0x82, 0x60, 0x16F),
    SCRIPT_POSE(0x82, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x82, 0, 0),
    SCRIPT_POSE(0x82, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x33E, 0x34C, 2),
    SCRIPT_WAIT(0xD2),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xB0, 0x1A8, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x235, 0x470, 0xE0, 7),
    SCRIPT_END,
};
AnimFrame updateTileSoloFrames0[] = {
    { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 },
    { 4, 8 }, { 5, 8 }, { 6, 8 }, { 7, 8 },
    { 8, 8 }, { 9, 8 }, { 10, 8 }, { 11, 8 },
    { 12, 8 }, { 13, 8 }, { 14, 8 }, { 255, 0 },
};
AnimFrame updateTileSoloFrames1[] = {
    { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 },
    { 4, 8 }, { 5, 8 }, { 6, 8 }, { 7, 8 },
    { 8, 8 }, { 9, 8 }, { 10, 8 }, { 11, 8 },
    { 255, 0x3E7 },
};
AnimFrame updateTileDuoFrames0_0[] = {
    { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 },
    { 4, 8 }, { 5, 8 }, { 6, 8 }, { 7, 8 },
    { 8, 8 }, { 9, 8 }, { 10, 8 }, { 11, 8 },
    { 12, 8 }, { 13, 8 }, { 14, 8 }, { 255, 0 },
};
AnimFrame updateTileDuoFrames1_0[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 },
    { 8, 6 }, { 9, 6 }, { 10, 6 }, { 11, 6 },
    { 255, 0x3E7 },
};
AnimFrame updateTileDuoFrames0_1[] = {
    { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 },
    { 4, 8 }, { 5, 8 }, { 6, 8 }, { 7, 8 },
    { 8, 8 }, { 9, 8 }, { 10, 8 }, { 11, 8 },
    { 12, 8 }, { 13, 8 }, { 14, 8 }, { 255, 0 },
};
AnimFrame updateTileDuoFrames1_1[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 },
    { 8, 6 }, { 9, 6 }, { 10, 6 }, { 11, 6 },
    { 255, 0x3E7 },
};
AnimFrame *updateTileDuoFrames0[] = {
    updateTileDuoFrames0_0, updateTileDuoFrames0_1,
};
AnimFrame *updateTileDuoFrames1[] = {
    updateTileDuoFrames1_0, updateTileDuoFrames1_1,
};
u8 tileDuoObjectFrames[2][2] = {
    { 62, 63 },
    { 64, 65 },
};
AnimFrame updateWandererFrames0[] = {
    { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 },
    { 4, 4 }, { 5, 4 }, { 4, 4 }, { 3, 4 },
    { 2, 4 }, { 1, 4 }, { 255, 0 },
};
AnimFrame updateWandererFrames1[] = {
    { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 },
    { 4, 4 }, { 5, 4 }, { 6, 8 }, { 7, 8 },
    { 8, 8 }, { 9, 8 }, { 10, 8 }, { 255, 0x3E7 },
};
u16 wandererSpeeds[] = {
    96, 80, 72, 64,
};
u16 angleTangents[] = {
    0, 0x648, 0xC93, 0x12E2, 0x1936, 0x1F92, 0x259F, 0x2C6B,
    0x32EB, 0x39C7, 0x401F, 0x46D7, 0x4DA7, 0x5491, 0x5B98, 0x62BF,
    0x6A09, 0x7179, 0x7913, 0x80DB, 0x88B5, 0x9105, 0x9970, 0xA21B,
    0xAB0D, 0xB44A, 0xBDDC, 0xC7C8, 0xD217, 0xDCD2, 0xE805, 0xF3BA,
    0xFFFE, 0xFFFF,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x17A, 0x1D8, 0xE8, 0xD8, 0x170, 0x1F1 },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x33A },
    { NULL, NULL, 0 },
};
u16 actor1Conditions[] = { PROGRESS(0xE), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x3F, 4, 250, 109, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0x82, 5, 96, 367, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 8, 0xFF, 2, 0x3E, 0, 0, 0, 0, 0, 46, 288, 0, 0 },
    { 0, 3, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 52, 288, 0, 0 },
    { 0, 2, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 154, 262, 0, 0 },
    { 0, 1, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 174, 359, 0, 0 },
    { 0, 7, 0x40, 2, 0xE, 0, 0, 0, 0, 0, 103, 297, 0, 0 },
    { 0, 5, 0x40, 2, 0xE, 0, 0, 0, 0, 0, 117, 236, 0, 0 },
    { 0, 6, 0x40, 2, 0xE, 0, 0, 0, 0, 0, 217, 300, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 1, 0xC, 0, 48, 288, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 1, 0xC, 0, 160, 280, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 0xC, 0, 104, 248, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 0xC, 0, 233, 263, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 0xC, 0, 233, 335, 0, 0 },
    { 0, 0xA, 0xFF, 6, 0x3C, 0, 0, 0, 0, 0, 50, 239, 0, 0 },
    { 0, 9, 0xFF, 6, 0x3F, 0, 0, 0, 0, 0, 46, 288, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 240, 72, 135, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { PROGRESS(0xE), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x172, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 0xE, 0xEF, 0xA8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 0xE, 0xDF, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 370, script370, EVENT_TEXT(0x28), NULL, endEvent370 },
    { -1, NULL, 0, NULL, NULL },
};
