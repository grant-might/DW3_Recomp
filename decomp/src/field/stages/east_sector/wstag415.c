/*
 * WSTAG415: Deeper Crevice, East Sector. The gondola ride to South Sector.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern StageFallParams updateFallerParams[];
extern AnimFrame updateMoverFrames0[];
extern AnimFrame updateMoverFrames1[];
extern AnimFrame updateMoverFrames2[];

/* The files of the background, which the versions number differently */
#if VERSION_US || VERSION_JP
#define BG_ARCHIVE 0x74E
#define BG_FILE 0x759
#elif VERSION_EU
#define BG_ARCHIVE 0x75E
#define BG_FILE 0x769
#endif

/* Steps the looping animation of a faller's StageBody, returning its frame */
s32 stepFallBody(StageBody *body, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[body->anim.index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        body->anim.timer -= dt;
    }
    if (body->anim.timer <= 0) {
        frame++;
        body->anim.index++;
        body->anim.timer += frame->duration;
        if (frame->frame == 0xFF) {
            frame = frames;
            body->anim.index = 0;
            body->anim.timer += frame->duration;
        }
        stepFallBody(body, frames, depth + 1);
    }
    return frame->frame;
}

/* Draws a StageFaller (a layer callback) */
void drawFaller(StageFaller *task, Layer *layer) {
    SpriteDrawer drawer;

    if (task->state == TASK_RUN) {
        initSpriteDrawer(&drawer);
        drawer.setTexture(0x140, 0x100);
        drawer.setLayer(layer, 4);
        drawer.setClutRow(0);
        drawer.setAltClut(0, 0x1F0);
        drawer.draw(FILE_CACHE.getEntry(FIELDSTG_state.sheetEntry), task->frame, task->x, task->y);
    }
}

/*
 * Updates a StageFaller of kind key1: it falls from the start of its
 * updateFallerParams with their speed and gravity, bounces once off their
 * floorY (if they bounce), and ends below the screen
 */
void updateFaller(StageFaller *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    s32 dv;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->body.x = updateFallerParams[task->key1].x << 8;
        task->body.y = updateFallerParams[task->key1].y << 8;
        task->body.mode = 0;
        task->body.vy = 0;
        task->body.vx = updateFallerParams[task->key1].vx;
        task->nextState(task);
        break;
    case TASK_RUN:
        task->frame = stepFallBody(&task->body, updateFallerParams[task->key1].frames, 0);
        dv = updateFallerParams[task->key1].gravity * GFX.funcs.getFrameTime();
        task->body.x += task->body.vx;
        task->body.vy += dv;
        task->x = task->body.x >> 8;
        task->body.y += task->body.vy;
        task->y = task->body.y >> 8;
        if (updateFallerParams[task->key1].bounce != 0 && task->body.mode == 0 && task->y >= updateFallerParams[task->key1].floorY) {
            task->body.y = updateFallerParams[task->key1].floorY << 8;
            task->y = updateFallerParams[task->key1].floorY;
            task->body.vy = -(task->body.vy / updateFallerParams[task->key1].bounce);
            task->body.vx = updateFallerParams[task->key1].vx * 3;
            task->body.mode = 1;
            SOUND.playSound(SOUND_MTL_DOWN);
        }
        if (task->frame != 0) {
            layer->addSortedCallback(layer, drawFaller, task, task->y, 0);
        }
        if (task->y > 0x244) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the task of updateFaller with the given id, of kind KIND (its updateFallerParams) */
static inline Task *createFaller(s32 id, s32 kind) {
    Task *task = createTaskWithId(updateFaller, sizeof(StageFaller), 0, id);

    task->key1 = kind;
    return task;
}

/* The fallers of kinds 0 to 9 */
Task *createCommand816(s32 id) {
    return createFaller(id, 0);
}

/* Command 817 of the event scripts: a thing that falls in event 150's earthquake (kind 1) */
Task *createCommand817(s32 id) {
    return createFaller(id, 1);
}

/* Command 818 of the event scripts: a thing that falls in event 150's earthquake (kind 2) */
Task *createCommand818(s32 id) {
    return createFaller(id, 2);
}

/* Command 819 of the event scripts: a thing that falls in event 150's earthquake (kind 3) */
Task *createCommand819(s32 id) {
    return createFaller(id, 3);
}

/* Command 820 of the event scripts: a thing that falls in event 150's earthquake (kind 4) */
Task *createCommand820(s32 id) {
    return createFaller(id, 4);
}

/* Command 821 of the event scripts: a thing that falls in event 150's earthquake (kind 5) */
Task *createCommand821(s32 id) {
    return createFaller(id, 5);
}

/* Command 822 of the event scripts: a thing that falls in event 150's earthquake (kind 6) */
Task *createCommand822(s32 id) {
    return createFaller(id, 6);
}

/* Command 823 of the event scripts: a thing that falls in event 150's earthquake (kind 7) */
Task *createCommand823(s32 id) {
    return createFaller(id, 7);
}

/* Command 824 of the event scripts: a thing that falls in event 150's earthquake (kind 8) */
Task *createCommand824(s32 id) {
    return createFaller(id, 8);
}

/*
 * Command 825 of the event scripts: the last of the ten things that fall in
 * event 150's earthquake (kind 9)
 */
Task *createCommand825(s32 id) {
    return createFaller(id, 9);
}

/* Steps the animation of the mover's StageBody, holding its last frame, and returns the frame */
s32 stepMoverBody(StageBody *body, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[body->anim.index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        body->anim.timer -= dt;
    }
    if (body->anim.timer <= 0) {
        frame++;
        body->anim.index++;
        body->anim.timer += frame->duration;
        if (frame->frame == 0xFF) {
            frame--;
            body->anim.index--;
            body->anim.timer += frame->duration;
        }
        stepMoverBody(body, frames, depth + 1);
    }
    return frame->frame;
}

/* Updates a StageMover: falls in mode 1 and plays an animation in modes 2 to 4 */
void updateMover(StageMover *task) {
    StageTile *tile;
    StageTile *object;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->key1 == 0) {
            task->body.x = 0x9600;
            task->body.y = 0xE600;
            task->body.mode = 0;
            task->body.vy = 0;
            task->body.vx = 0;
            task->body.timer = 0;
        } else {
            task->body.x = 0x9600;
            task->body.y = 0x1AE00;
            task->body.timer = 0;
            task->body.mode = 0;
            task->x = task->body.x >> 8;
            task->y = task->body.y >> 8;
        }
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == 1) {
                task->tile = object;
                object->x = task->x;
                object->x = task->y; /* x again, not y */
                break;
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->body.mode) {
        case 0:
            task->frame = 0x32;
            break;
        case 1:
            task->frame = 0x32;
            task->body.vy += GFX.funcs.getFrameTime() * 0x30;
            task->body.x += task->body.vx;
            task->body.y += task->body.vy;
            task->x = task->body.x >> 8;
            task->y = task->body.y >> 8;
            if (task->y >= 0x1AE) {
                task->body.y = 0x1AE00;
                task->y = 0x1AE;
                if (task->body.vy > 0x200) {
                    SOUND.playSound(SOUND_COMEX114);
                }
                task->body.vy = -(task->body.vy / 6);
            }
            task->body.timer++;
            break;
        case 2:
            task->frame = stepMoverBody(&task->body, updateMoverFrames0, 0);
            if (task->frame == 0x35 && task->substate == 0) {
                SOUND.playSound(SOUND_COMEX112);
                task->substate++;
            }
            if (task->frame == 0x3B && task->substate == 1) {
                SOUND.playSound(SOUND_BULB_000);
                task->substate++;
            }
            break;
        case 3:
            task->frame = stepMoverBody(&task->body, updateMoverFrames1, 0);
            break;
        case 4:
            task->frame = stepMoverBody(&task->body, updateMoverFrames2, 0);
            if (task->frame == 2 && task->substate == 0) {
                SOUND.playSound(SOUND_BULB_001);
                task->substate++;
            }
            if (task->frame == 0x20 && task->substate == 1) {
                SOUND.playSound(SOUND_MTL_DOWN);
                task->substate++;
            }
            if (task->frame == 0x2A && task->substate == 2) {
                SOUND.playSound(SOUND_MTL_DOWN);
                task->step = 0;
                task->substate++;
            }
            if (task->substate == 3 || task->substate == 4) {
                if ((++task->step & 0x1F) == 0) {
                    SOUND.playSound(SOUND_MTL_DOWN);
                    task->step = 0;
                    task->substate++;
                }
            }
            break;
        }
        tile = task->tile;
        tile->frame = task->frame;
        tile->x = task->x;
        tile->y = task->y;
        tile->visible = 1;
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Handles the events 0x331 to 0x334 sent to a StageMover */
void handleCommand811(StageMover *task, s32 id) {
    if (task != NULL) {
        switch (id) {
        case 0x331:
            task->body.x = 0x9600;
            task->body.y = 0xE600;
            task->body.mode = 1;
            task->body.vy = 0;
            task->body.vx = 0;
            task->body.timer = 0;
            break;
        case 0x332:
            SOUND.playSound(SOUND_COMEX105);
            task->body.anim.index = 0;
            task->body.anim.timer = updateMoverFrames0[0].duration;
            task->body.mode = 2;
            task->substate = 0;
            break;
        case 0x333:
            SOUND.playSound(SOUND_COMEX112);
            task->body.anim.index = 0;
            task->body.anim.timer = updateMoverFrames1[0].duration;
            task->body.mode = 3;
            break;
        case 0x334:
            task->body.mode = 4;
            task->body.anim.index = 0;
            task->body.anim.timer = updateMoverFrames2[0].duration;
            task->substate = 0;
            break;
        }
    }
}

/* Creates the task of updateMover with the given id, kind 0 */
Task *createCommand811(s32 id) {
    Task *task = createTaskWithId(updateMover, sizeof(StageMover), 0, id);

    task->key1 = 0;
    return task;
}

/*
 * Creates the mover of event 151, where the Digimon gets away (updateMover),
 * already falling (mode 1)
 */
Task *createMover(s32 id) {
    Task *task = createTaskWithId(updateMover, sizeof(StageMover), 0, id);

    task->key1 = 1;
    return task;
}

/* Draws a StageScroller: each image twice, the second 0x200 left and 0x100 down */
void drawScroller(StageScroller *task) {
    SpriteDrawer drawer;
    s32 pos[2];

    initSpriteDrawer(&drawer);
    drawer.setLayerId(FIELD_LAYER_MAP, 7);
    drawer.setTexture(0x280, 0);
    drawer.setAltClut(0, 0xF0);
    pos[0] = task->x >> 8;
    pos[1] = (task->y >> 8) + 0x100;
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 2), 0, pos[0], pos[1]);
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 2), 0, pos[0] - 0x200, pos[1] + 0x100);
    drawer.setTexture(0x280, 0x100);
    drawer.setAltClut(0, 0xF8);
    pos[0] = (task->x >> 8) + 0x100;
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 4), 0, pos[0], pos[1]);
    drawer.draw(FILE_CACHE.getEntry(BG_ARCHIVE << 16 | 4), 0, pos[0] - 0x200, pos[1] + 0x100);
}

/* Loads the images of a StageScroller and scrolls it, slowing down to a stop in substate 1 */
void updateScroller(StageScroller *task) {
    TimLoader loader;

    switch (task->state) {
    case TASK_INIT:
    default:
        initTimLoader(&loader);
        loader.setImagePos(0x280, 0);
        loader.setClutPos(0, 0xF0);
        loader.loadArchive(FILE_CACHE.load(BG_FILE));
        loader.setImagePos(0x280, 0x100);
        loader.setClutPos(0, 0xF8);
        loader.loadArchive(FILE_CACHE.load(BG_FILE + 1));
        task->nextState(task);
        if (task->key1 != 0) {
            task->nextSubstate(task);
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            task->vx = 0x800;
            task->vy = 0x400;
            break;
        case 1:
            if (task->vx == 0x800) {
                SOUND.playSound(SOUND_GONDRA_B);
            }
            if (task->vx != 0) {
                task->vx -= 0x10;
                task->vy = task->vx / 2;
                if (task->vx < 0) {
                    task->vx = 0;
                    task->vy = 0;
                }
            }
            break;
        }
        task->x += task->vx;
        task->y -= task->vy;
        if (task->x > 0x1FFFF) {
            task->x -= 0x20000;
            task->y += 0x10000;
        }
        drawScroller(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Sets the substate of the task to 1 when the id is 0x336 */
void handleCommand812(Task *task, s32 id) {
    if (task != NULL && id == 0x336) {
        task->setSubstate(task, 1);
    }
}

/* Creates the task of updateScroller (id 0x32C) of the given kind */
Task *createScroller(s32 kind) {
    Task *task = createTaskWithId(updateScroller, sizeof(StageScroller), 0, 0x32C);

    task->key1 = kind;
    return task;
}

/* Creates the event objects of progress 6, which depend on flag 0x4018 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 6) {
            if (FLAGS_00.checkCondition(FLAG(0x40, 0x18), 0)) {
                children[0] = createScroller(0);
                children[1] = FIELDSTG_startEvent(0x96);
            } else {
                children[0] = createScroller(1);
                children[2] = createMover(0x32B);
                children[1] = FIELDSTG_startEvent(0x97);
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0xC
#include "common/start_stage.inc.c"

/* After event 150, with the player: starts the stage's event battle 0 and sets flag 0x4018 */
void endEvent150(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x18), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x74E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x75E
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0011 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.imageFile = STAGE_FILE + 1;
    FIELDSTG_state.start = (Vec2){0x11500, 0x15500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xD;
    FIELDSTG_state.music = MUSIC(0xD, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_map.setFirstMap(0);
}

s16 script150[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x114, 0x157),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_COMMAND(0x32B, 0x330, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_COMMAND(0x32C, 0x336, 0x32C),
    SCRIPT_WAIT_ANIM(0x32C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 4),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_GONDRA_S, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHAKE_CAMERA, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 3, 1, 0),
    SCRIPT_POSE(1, 0x33, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0xD8, 0x175),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x330, 0x335, 0x330),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x331, 0x335, 0x331),
    SCRIPT_COMMAND(0x337, 0x335, 0x337),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x332, 0x335, 0x332),
    SCRIPT_COMMAND(0x334, 0x335, 0x334),
    SCRIPT_COMMAND(0x336, 0x335, 0x336),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x333, 0x335, 0x333),
    SCRIPT_COMMAND(0x339, 0x335, 0x339),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x335, 0x335, 0x335),
    SCRIPT_COMMAND(0x338, 0x335, 0x333),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x32B, 0x331, 0x32B),
    SCRIPT_WAIT(0x78),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_CAMERA_SHAKE, 1),
    SCRIPT_WAIT(0xC),
    SCRIPT_TALK(0, 4, 1, 0),
    SCRIPT_POSE(1, 0x33, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x32B, 0x332, 0x32B),
    SCRIPT_WAIT(0x78),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x32B, 0x333, 0x32B),
    SCRIPT_WAIT(0xC),
    SCRIPT_END,
};
s16 script151[] = {
    SCRIPT_LOOK_AT(1, 0xD8, 0x175),
    SCRIPT_PLACE(1, 0x114, 0x157),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_COMMAND(0x32C, 0x336, 1),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x54),
    SCRIPT_COMMAND(0x32B, 0x334, 1),
    SCRIPT_WAIT(0x9C),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 1, 4),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_GONDRA_S, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x232, 0x1EF, 0xB9, 1),
    SCRIPT_END,
};
AnimFrame updateFallerParams_0[] = {
    { 70, 8 }, { 71, 8 }, { 72, 8 }, { 73, 8 },
    { 255, 0 },
};
AnimFrame updateFallerParams_1[] = {
    { 74, 8 }, { 75, 8 }, { 76, 8 }, { 77, 8 },
    { 255, 0 },
};
StageFallParams updateFallerParams[] = {
    { updateFallerParams_0, -0x20, 230, 0, 0x180, 88, 0 },
    { updateFallerParams_1, 0, 230, 0x1C2, 0x200, 72, 4 },
    { updateFallerParams_0, 32, 230, 0x19A, 0x180, 88, 8 },
    { updateFallerParams_1, 64, 230, 0, 0x200, 72, 0 },
    { updateFallerParams_1, 80, 230, 0, 0x200, 72, 0 },
    { updateFallerParams_0, 16, 230, 0, 96, 88, 0 },
    { updateFallerParams_1, 48, 230, 0x1CC, 128, 72, 4 },
    { updateFallerParams_0, 80, 230, 0x1A4, 96, 88, 8 },
    { updateFallerParams_1, 112, 230, 0, 128, 72, 0 },
    { updateFallerParams_1, 140, 230, 0, 128, 72, 0 },
};
AnimFrame updateMoverFrames0[] = {
    { 50, 5 }, { 52, 5 }, { 53, 5 }, { 54, 40 },
    { 55, 8 }, { 56, 8 }, { 57, 10 }, { 58, 12 },
    { 59, 180 }, { 255, 0 },
};
AnimFrame updateMoverFrames1[] = {
    { 60, 4 }, { 61, 4 }, { 62, 0x3E7 }, { 255, 0 },
};
AnimFrame updateMoverFrames2[] = {
    { 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 },
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 8, 4 },
    { 9, 4 }, { 10, 4 }, { 11, 4 }, { 12, 4 },
    { 13, 4 }, { 14, 4 }, { 15, 4 }, { 16, 4 },
    { 17, 4 }, { 18, 4 }, { 19, 4 }, { 20, 4 },
    { 21, 4 }, { 22, 4 }, { 23, 4 }, { 24, 4 },
    { 25, 4 }, { 26, 4 }, { 27, 4 }, { 28, 4 },
    { 29, 4 }, { 30, 4 }, { 31, 4 }, { 32, 4 },
    { 33, 4 }, { 34, 4 }, { 35, 4 }, { 36, 4 },
    { 37, 4 }, { 38, 4 }, { 39, 4 }, { 40, 4 },
    { 41, 4 }, { 42, 4 }, { 43, 4 }, { 44, 4 },
    { 45, 4 }, { 46, 4 }, { 47, 4 }, { 255, 0x3E7 },
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
AreaBattle area3Battle0 = { 323, 10, MUSIC(0x22, 0) };
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
    { 128, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1CC, 0x150, 0x230, 0x50, 0x160, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 1, 4, 0, 0, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x6E, 6, 0, 0, 0, 0, 0, 0, 225, 318, 0, 0 },
    { 1, 1, 0x94, 4, 0x32, 0, 0, 0, 0, 0, 0, -50, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 150, script150, EVENT_TEXT(4), NULL, endEvent150 },
    { 151, script151, EVENT_TEXT(5), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
