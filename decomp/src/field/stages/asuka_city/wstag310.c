/*
 * WSTAG310: A.o.A Headquarters, Asuka City.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateTileAnimsFrames0[];
extern AnimFrame updateTileAnimsFrames1[];
extern AnimFrame updateTileAnimsFrames2[];
extern AnimFrame updateTileAnimsFrames3[];
extern AnimFrame updateEffectFrames1[];
extern AnimFrame updateEffectFrames0[];
extern StageEffectSpot updateStageSpots[];
StageEffect *createEffect(s32 x, s32 y, s32 frame);
extern AnimFrame updateTileSixFrames3[];
extern AnimFrame updateTileSixFrames4[];
extern AnimFrame updateTileSixWFrames3[];
extern AnimFrame updateTileSixWFrames4[];
extern AnimFrame updateTileSixFrames0[];
extern AnimFrame updateTileSixFrames1[];
extern AnimFrame updateTileSixFrames2[];
extern AnimFrame updateTileSixWFrames0[];
extern AnimFrame updateTileSixWFrames1[];
extern AnimFrame updateTileSixWFrames2[];

#include "common/step_looping_animation.inc.c"

#include "common/update_tile_anims4.inc.c"

#include "common/create_tile_anims.inc.c"

#include "common/step_tile_animation.inc.c"

#include "common/tile_six.inc.c"

/* Animates the map objects with animations 5 to 10 by mode (0: hidden, 1 and 3: the first two once, then mode 2 or 0; 2, the first: the other four) */
void updateTileSix(StageTileSix *task) {
    StageTile *tile;
    s32 done;
    s32 i;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        findTileSix(task);
        task->mode = 2;
        task->nextState(task);
        break;
    case TASK_RUN:
        done = 0;
        for (i = 0; i < 6; i++) {
            tile = task->tiles[i].tile;
            switch (task->mode) {
            case 0:
                tile->visible = 0;
                break;
            case 1:
                switch (i) {
                case 0:
                    frame = stepTileAnimation(&task->tiles[0], updateTileSixFrames0, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 1:
                    frame = stepTileAnimation(&task->tiles[1], updateTileSixFrames1, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    tile->visible = 0;
                    break;
                }
                break;
            case 2:
                showLastFourTiles(task, tile, i);
                break;
            case 3:
                switch (i) {
                case 0:
                    frame = stepTileAnimation(&task->tiles[0], updateTileSixFrames4, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 1:
                    frame = stepTileAnimation(&task->tiles[1], updateTileSixFrames4, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    tile->visible = 0;
                    break;
                }
                break;
            }
        }
        if (done >= 2) {
            switch (task->mode) {
            case 1:
                task->mode = 2;
                break;
            case 3:
                task->mode = 0;
                break;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Plays the first two map objects backwards (mode 3) for map 0x359 */
void handleCommand831(StageTileSix *task, s32 id) {
    if (task != NULL && id == 0x359) {
        task->tiles[0].anim.index = 0;
        task->tiles[0].anim.timer = updateTileSixFrames3[15].duration;
        task->tiles[1].anim.index = 0;
        task->tiles[1].anim.timer = updateTileSixFrames4[0].duration;
        task->mode = 3;
    }
}

/* Creates the task of the six map objects with id ID, in mode 2 */
void *createCommand831(s32 id) {
    StageTileSix *task = createTaskWithId(updateTileSix, sizeof(StageTileSix), 0, id);

    task->mode = 2;
    return task;
}

#include "common/create_tile_six.inc.c"

/*
 * stepTileAnimation (common/step_tile_animation.inc.c) for a
 * StageTileAnimFlag: steps OBJ's animation through FRAMES and returns its
 * frame, looping at the end or returning 0xFF if once
 */
s32 stepTileAnimation2(StageTileAnimFlag *obj, AnimFrame *frames, s32 once, s32 depth) {
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
        if (once) {
            if (frame->frame == 0xFF) {
                return 0xFF;
            }
        } else if (frame->frame == 0xFF) {
            frame = frames;
            obj->anim.index = 0;
            obj->anim.timer += frame->duration;
        }
        stepTileAnimation2(obj, frames, once, depth + 1);
    }
    return frame->frame;
}

/*
 * Finds the map objects with animations 11 to 16 and starts the
 * animations of the first four
 */
static inline void findTileSixW(StageTileSixW *task) {
    StageTile *object;

    for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
        if (object->anim >= 11 && object->anim <= 16) {
            switch (object->anim) {
            case 11:
                task->tiles[0].tile = object;
                task->tiles[0].anim.index = 0;
                task->tiles[0].anim.timer = updateTileSixWFrames0[0].duration;
                break;
            case 12:
                task->tiles[1].tile = object;
                task->tiles[1].anim.index = 0;
                task->tiles[1].anim.timer = updateTileSixWFrames1[0].duration;
                break;
            case 13:
                task->tiles[2].tile = object;
                task->tiles[2].anim.index = 0;
                task->tiles[2].anim.timer = updateTileSixWFrames2[0].duration;
                break;
            case 14:
                task->tiles[3].tile = object;
                task->tiles[3].anim.index = 0;
                task->tiles[3].anim.timer = updateTileSixWFrames3[0].duration;
                break;
            case 15:
                task->tiles[4].tile = object;
                task->tiles[4].anim.index = 0;
                task->tiles[4].anim.timer = 0;
                break;
            case 16:
                task->tiles[5].tile = object;
                task->tiles[5].anim.index = 0;
                task->tiles[5].anim.timer = 0;
                break;
            }
        }
    }
}

/*
 * Mode 2: hides the first two map objects and shows the other four, the
 * third and fourth with their looping palettes and the last two still
 */
static inline void showLastFourTilesW(StageTileSixW *task, StageTile *tile, s32 i) {
    switch (i) {
    case 0:
    case 1:
        tile->visible = 0;
        break;
    case 2:
        tile->frame = 0x29;
        tile->clutRow = stepTileAnimation2(&task->tiles[i], updateTileSixWFrames2, 0, 0);
        tile->visible = 1;
        break;
    case 3:
        tile->frame = 0x11;
        tile->clutRow = stepTileAnimation2(&task->tiles[i], updateTileSixWFrames3, 0, 0);
        tile->visible = 1;
        break;
    case 4:
        tile->visible = 1;
        tile->frame = 0x12;
        tile->clutRow = 0;
        break;
    case 5:
        tile->visible = 1;
        tile->frame = 0x13;
        tile->clutRow = 0;
        break;
    }
}

/* Animates the map objects with animations 11 to 16 by mode (0: hidden, 1 and 3: the first two once, then mode 2 or 0; 2, the first: the other four) */
void updateTileSixW(StageTileSixW *task) {
    StageTile *tile;
    s32 done;
    s32 i;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        findTileSixW(task);
        task->mode = 2;
        task->nextState(task);
        break;
    case TASK_RUN:
        done = 0;
        for (i = 0; i < 6; i++) {
            tile = task->tiles[i].tile;
            switch (task->mode) {
            case 0:
                tile->visible = 0;
                break;
            case 1:
                switch (i) {
                case 0:
                    frame = stepTileAnimation2(&task->tiles[0], updateTileSixWFrames0, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 1:
                    frame = stepTileAnimation2(&task->tiles[1], updateTileSixWFrames1, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    tile->visible = 0;
                    break;
                }
                break;
            case 2:
                showLastFourTilesW(task, tile, i);
                break;
            case 3:
                switch (i) {
                case 0:
                    frame = stepTileAnimation2(&task->tiles[0], updateTileSixWFrames4, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 1:
                    frame = stepTileAnimation2(&task->tiles[1], updateTileSixWFrames4, 1, 0);
                    if (frame != 0xFF) {
                        tile->frame = frame;
                        tile->visible = 1;
                    } else {
                        done++;
                        tile->frame = 0;
                        tile->visible = 0;
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    tile->visible = 0;
                    break;
                }
                break;
            }
        }
        if (done >= 2) {
            switch (task->mode) {
            case 1:
                task->mode = 2;
                break;
            case 3:
                task->mode = 0;
                break;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Plays the first two map objects backwards (mode 3) for map 0x359 */
void handleCommand832(StageTileSixW *task, s32 id) {
    if (task != NULL && id == 0x359) {
        task->tiles[0].anim.index = 0;
        task->tiles[0].anim.timer = updateTileSixWFrames3[12].duration;
        task->tiles[1].anim.index = 0;
        task->tiles[1].anim.timer = updateTileSixWFrames4[0].duration;
        task->mode = 3;
    }
}

/* Creates the task of the six map objects with id ID, in mode 2 */
void *createCommand832(s32 id) {
    StageTileSixW *task = createTaskWithId(updateTileSixW, sizeof(StageTileSixW), 0, id);

    task->mode = 2;
    return task;
}

/*
 * Creates the second set of six map objects (updateTileSixW) without an id;
 * nothing in the game calls it
 */
void *createTileSixW(void) {
    return createTask(updateTileSixW, sizeof(StageTileSixW), 0);
}

/* Creates the stage tasks, the sprite effects and the event object of the story so far */
void updateStage(StageTask *task, void **children) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createTileAnims();
        for (i = 0; i < 2; i++) {
            if (updateStageSpots[i].kind == 0) {
                children[3 + i] = createEffect(updateStageSpots[i].x, updateStageSpots[i].y, updateStageSpots[i].frame);
            }
        }
        do {
            if (GAME.progress == 0x17 && FLAGS_00.checkCondition(FLAG(0x40, 0x4E), 0)) {
                children[5] = FIELDSTG_startEvent(0x2A8);
                break;
            }
            if (GAME.progress == 0x17 && FLAGS_00.checkCondition(FLAG(0x40, 0x4E), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x4F), 0)) {
                children[5] = FIELDSTG_startEvent(0x2A9);
                children[1] = createCommand831(0x33F);
                children[2] = createCommand832(0x340);
                break;
            }
            if (GAME.progress == 0x17 && FLAGS_00.checkCondition(FLAG(0x40, 0x4F), 1)) {
                children[5] = FIELDSTG_startEvent(0x2AA);
                break;
            }
            if (GAME.progress == 0x26) {
                children[5] = FIELDSTG_startEvent(0x3C1);
                break;
            }
        } while (0);
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x18
#include "common/start_stage.inc.c"
#include "common/step_animation.inc.c"

/* Draws sprite IDX of the effect while it plays (a draw callback) */
void drawEffect(StageEffect *task, void *arg, s32 idx) {
    SpriteDrawer drawer;
    Layer *layer = arg;
    StageSprite *sprite;
    s32 y;
    s32 x;
    s32 depth;

    if (task->state == TASK_RUN) {
        sprite = &task->sprites[idx];
        y = task->y;
        x = task->x;
        if (idx == 1) {
            y -= 0x20;
            depth = 4;
        } else {
            depth = 6;
        }
        initSpriteDrawer(&drawer);
        drawer.setTexture(0x140, 0x100);
        drawer.setLayer(layer, depth);
        drawer.setClutRow(sprite->clutRow);
        drawer.draw(FILE_CACHE.getEntry(FIELDSTG_state.sheetEntry), sprite->frame, x, y);
    }
}

#include "common/is_on_screen.inc.c"

/* An effect that plays its animation once and ends */
void updateEffect(StageEffect *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    s32 done;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->key1 = task->x;
        task->key2 = task->y;
        task->anim.index = 0;
        task->anim.timer = updateEffectFrames0[0].duration;
        task->clutAnim.index = 0;
        task->clutAnim.timer = updateEffectFrames1[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        if (task->substate == 0) {
            task->anim.index = 0;
            task->anim.timer = updateEffectFrames0[0].duration;
            task->clutAnim.index = 0;
            task->clutAnim.timer = updateEffectFrames1[0].duration;
            task->setSubstate(task, 1);
        }
        task->sprites[0].frame = task->frame;
        task->sprites[0].clutRow = stepAnimation(&task->clutAnim, updateEffectFrames1, 0, 0);
        done = 0;
        frame = stepAnimation(&task->anim, updateEffectFrames0, 1, 0);
        switch (frame) {
        case 0xFF:
            task->sprites[1].frame = 0;
            done = 1;
            break;
        case 0x12C:
            task->sprites[1].frame = 0;
            break;
        default:
            task->sprites[1].frame = task->frame + frame;
            break;
        }
        if (done) {
            task->setState(task, TASK_KILL);
        }
        if (task->sprites[0].frame != 0 && isOnScreen(task->x, task->y, 0x20, 0x20)) {
            layer->addSortedCallback(layer, drawEffect, task, task->y, 0);
        }
        if (task->sprites[1].frame != 0 && isOnScreen(task->x, task->y - 0x20, 0x20, 0x40)) {
            layer->addSortedCallback(layer, drawEffect, task, task->y + 0x12, 1);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/*
 * Command 852 of the event scripts: the teleport effect at (0x26C, 0xFC) in
 * event 681, with its sound
 */
StageEffect *createCommand852(s32 id) {
    StageEffect *task = createTaskWithId(updateEffect, sizeof(StageEffect), 0, id);

    task->x = 0x26C;
    task->y = 0xFC;
    SOUND.playSound(SOUND_TELEPORT);
    task->frame = 0x14;
    return task;
}

/*
 * stepAnimation (common/step_animation.inc.c) under another name: steps ANIM
 * through FRAMES and returns its frame, looping at the end or returning 0xFF
 * if once
 */
s32 stepAnimation2(AnimState *anim, AnimFrame *frames, s32 once, s32 depth) {
    AnimFrame *frame = &frames[anim->index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        anim->timer -= dt;
    }
    if (anim->timer <= 0) {
        frame++;
        anim->index++;
        anim->timer += frame->duration;
        if (once) {
            if (frame->frame == 0xFF) {
                return 0xFF;
            }
        } else if (frame->frame == 0xFF) {
            frame = frames;
            anim->index = 0;
            anim->timer += frame->duration;
        }
        stepAnimation2(anim, frames, once, depth + 1);
    }
    return frame->frame;
}

#include "common/draw_stage_effect.inc.c"

/*
 * isOnScreen (common/is_on_screen.inc.c) under another name: whether the
 * rectangle (x, y, w, h) is in the view of the map's layer
 */
s32 isOnScreen2(s32 x, s32 y, s32 w, s32 h) {
    RECT rect;
    struct Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);

    layer->getViewRect(layer, &rect);
    if (x + w < rect.x) {
        return 0;
    }
    if (rect.x + rect.w < x) {
        return 0;
    }
    if (y + h < rect.y) {
        return 0;
    }
    return rect.y + rect.h >= y;
}

/* An effect that plays its animation each time it is set to TASK_DONE */
void updateStageEffect(StageEffect *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    s32 done;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->key1 = task->x;
        task->key2 = task->y;
        task->anim.index = 0;
        task->anim.timer = effectFrames[0].duration;
        task->clutAnim.index = 0;
        task->clutAnim.timer = effectClutFrames[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        task->sprites[0].frame = task->frame;
        task->sprites[0].clutRow = stepAnimation2(&task->clutAnim, effectClutFrames, 0, 0);
        if (task->sprites[0].frame != 0 && isOnScreen2(task->x, task->y, 0x20, 0x20)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y, 0);
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->anim.index = 0;
            task->anim.timer = effectFrames[0].duration;
            task->clutAnim.index = 0;
            task->clutAnim.timer = effectClutFrames[0].duration;
            task->setSubstate(task, 1);
        }
        task->sprites[0].frame = task->frame;
        task->sprites[0].clutRow = stepAnimation2(&task->clutAnim, effectClutFrames, 0, 0);
        done = 0;
        frame = stepAnimation2(&task->anim, effectFrames, 1, 0);
        switch (frame) {
        case 0xFF:
            task->sprites[1].frame = 0;
            done = 1;
            break;
        case 0x12C:
            task->sprites[1].frame = 0;
            break;
        default:
            task->sprites[1].frame = task->frame + frame;
            break;
        }
        if (done) {
            task->setState(task, TASK_RUN);
        }
        if (task->sprites[0].frame != 0 && isOnScreen2(task->x, task->y, 0x20, 0x20)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y, 0);
        }
        if (task->sprites[1].frame != 0 && isOnScreen2(task->x, task->y - 0x20, 0x20, 0x40)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y + 0x12, 1);
        }
        break;
    case TASK_KILL:
        break;
    }
}

/*
 * Creates one of the stage's effects (updateStageEffect) at (x, y) with its
 * sprites' frames from FRAME
 */
StageEffect *createEffect(s32 x, s32 y, s32 frame) {
    StageEffect *task = createTask(updateStageEffect, sizeof(StageEffect), 0);

    task->x = x;
    task->y = y;
    task->frame = frame;
    return task;
}

/*
 * After event 680, with Guard A, Guard B and Guard C: starts the stage's
 * event battle 1, sets flag 0x0C13 and keeps it from playing again
 */
void endEvent680(void) {
    FLAGS_00.applyAction(FLAG(0xC, 0x13), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0x4E), 1);
}

/*
 * After event 681, with Game Master: starts the stage's event battle 0 and
 * keeps it from playing again
 */
void endEvent681(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x4F), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 682, with Game Master, Lisa, Teddy and Nick: lets events 683
 * of WSTAG305 and 685 of WSTAG210 start
 */
void endEvent682(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x50), 1);
}

/* After event 961, with the player: moves the story on to its step 0x27 */
void endEvent961(void) {
    GAME.progress = 0x27;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x334
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x343
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0002 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x13E00, 0x19800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2A;
    FIELDSTG_state.music = MUSIC(0x2A, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script680[] = {
    SCRIPT_PLACE(2, 0x58, 0x20C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x6C, 0x23B, 0xEB),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_PLACE(0x6D, 0xA1, 0x1B9),
    SCRIPT_POSE(0x6D, 1, 1),
    SCRIPT_PLACE(0x6E, 0xFF, 0x1E9),
    SCRIPT_POSE(0x6E, 1, 1),
    SCRIPT_PLACE(0x77, 0xEB, 0x1C4),
    SCRIPT_POSE(0x77, 1, 1),
    SCRIPT_PLACE(0x110, 0x280, 0x109),
    SCRIPT_POSE(0x110, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x78, 0x1FC, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x6C),
    SCRIPT_WAIT(0x78),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 3, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xB0, 0x1E0, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x6D),
    SCRIPT_COMMAND(0x324, 0x325, 0x6E),
    SCRIPT_COMMAND(0x325, 0x325, 0x77),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6D),
    SCRIPT_COMMAND(0x324, 0x326, 0x6E),
    SCRIPT_COMMAND(0x325, 0x326, 0x77),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6D, 0xB8, 0x1C4, 0),
    SCRIPT_WALK(0x6E, 0xE7, 0x1DC, 2),
    SCRIPT_WALK(0x77, 0xD0, 0x1D0, 1),
    SCRIPT_WAIT_WALK(0x77),
    SCRIPT_TALK(0, 4, 0x6D, 2),
    SCRIPT_POSE(0x6D, 1, 0),
    SCRIPT_POSE(0x6E, 1, 2),
    SCRIPT_POSE(0x77, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x6E, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x77, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script681[] = {
    SCRIPT_PLACE(2, 0xB0, 0x1E0),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x6C, 0x23B, 0xEB),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_PLACE(0x110, 0x280, 0x109),
    SCRIPT_POSE(0x110, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 0xA, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x170, 0x180, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x1B0, 0x130, 5),
    SCRIPT_POSE(0x6C, 1, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_DEMO_BGM, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x6C),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x354, 0x335, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0x110, 0, 0),
    SCRIPT_POSE(0x110, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x20C, 0x102, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0x6C),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x33F, 0x359, 2),
    SCRIPT_WAIT(0x12),
    SCRIPT_COMMAND(0x340, 0x359, 2),
    SCRIPT_WAIT(0x48),
    SCRIPT_TALK(0, 7, 0x6C, 0),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script682[] = {
    SCRIPT_LOOK_AT(1, 0x20C, 0x108),
    SCRIPT_PLACE(2, 0x20C, 0x102),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x65, 0x150, 0x190),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_PLACE(0x6C, 0x23B, 0xEB),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0xC, 0x150, 0x190),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WALK(0x65, 0x170, 0x180, 5),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0xC, 0x170, 0x180, 5),
    SCRIPT_WALK(0x65, 0x182, 0x16A, 5),
    SCRIPT_PLACE(0x67, 0x150, 0x190),
    SCRIPT_POSE(0x67, 1, 5),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0xC, 0x19E, 0x146, 5),
    SCRIPT_WALK(0x65, 0x1B0, 0x130, 5),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0xC, 0x1B0, 0x130, 5),
    SCRIPT_WALK(0x65, 0x1D0, 0x120, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_WALK(0xC, 0x1D0, 0x120, 5),
    SCRIPT_WALK(0x65, 0x1F0, 0x110, 5),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0xC, 0x1F0, 0x110, 5),
    SCRIPT_WALK(0x65, 0x200, 0x118, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_WALK(0xC, 0x1E0, 0x108, 5),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 2, 0x65, 1),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_POSE(0x65, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xC, 2),
    SCRIPT_POSE(0xC, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x67, 0x170, 0x180, 5),
    SCRIPT_WAIT_WALK(0x67),
    SCRIPT_WALK(0x67, 0x1B0, 0x130, 5),
    SCRIPT_WAIT_WALK(0x67),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WALK(0x67, 0x1D0, 0x120, 5),
    SCRIPT_WAIT_WALK(0x67),
    SCRIPT_TALK(0, 4, 0x67, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x67, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 3),
    SCRIPT_POSE(0x65, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x65, 3),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x218, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script961[] = {
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x128, 0x1A5, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x110, 0x40, 0x218),
    SCRIPT_POSE(0x110, 1, 5),
    SCRIPT_PLACE(0x111, 0x28, 0x224),
    SCRIPT_POSE(0x111, 1, 5),
    SCRIPT_PLACE(0x112, 1, 0x238),
    SCRIPT_POSE(0x112, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(1, 0x60, 0x208),
    SCRIPT_WAIT(0x5A),
    SCRIPT_WALK(0x112, 0x10, 0x230, 5),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT_WALK(0x112),
    SCRIPT_POSE(0x110, 1, 4),
    SCRIPT_POSE(0x111, 1, 6),
    SCRIPT_POSE(0x112, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x110, 1, 5),
    SCRIPT_POSE(0x111, 1, 5),
    SCRIPT_POSE(0x112, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x110, 1, 6),
    SCRIPT_POSE(0x111, 1, 4),
    SCRIPT_POSE(0x112, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x110, 1, 5),
    SCRIPT_POSE(0x111, 1, 5),
    SCRIPT_POSE(0x112, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x110),
    SCRIPT_COMMAND(0x324, 0x325, 0x111),
    SCRIPT_COMMAND(0x325, 0x325, 0x112),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x110),
    SCRIPT_COMMAND(0x324, 0x326, 0x111),
    SCRIPT_COMMAND(0x325, 0x326, 0x112),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x110, 1, 1),
    SCRIPT_POSE(0x111, 1, 1),
    SCRIPT_POSE(0x112, 1, 1),
    SCRIPT_WAIT(0x12),
    SCRIPT_WALK(0x112, 1, 0x238, 1),
    SCRIPT_WAIT_WALK(0x112),
    SCRIPT_WALK(0x111, 1, 0x238, 1),
    SCRIPT_PLACE(0x112, 0, 0),
    SCRIPT_POSE(0x112, 1, 1),
    SCRIPT_WAIT_WALK(0x111),
    SCRIPT_WALK(0x110, 1, 0x238, 1),
    SCRIPT_PLACE(0x111, 0, 0),
    SCRIPT_POSE(0x111, 1, 1),
    SCRIPT_WAIT_WALK(0x110),
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(0x110, 0, 0),
    SCRIPT_POSE(0x110, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 3, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x70, 0x200, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x218, 0x178, 0xFC, 1),
    SCRIPT_END,
};
AnimFrame updateTileAnimsFrames0[] = {
    { 51, 8 }, { 52, 8 }, { 53, 8 }, { 54, 8 },
    { 55, 8 }, { 56, 8 }, { 57, 8 }, { 75, 40 },
    { 255, 0 },
};
AnimFrame updateTileAnimsFrames1[] = {
    { 58, 8 }, { 59, 8 }, { 60, 8 }, { 61, 8 },
    { 62, 8 }, { 63, 8 }, { 75, 40 }, { 255, 0 },
};
AnimFrame updateTileAnimsFrames2[] = {
    { 64, 8 }, { 65, 8 }, { 66, 8 }, { 67, 8 },
    { 68, 8 }, { 75, 40 }, { 255, 0 },
};
AnimFrame updateTileAnimsFrames3[] = {
    { 69, 8 }, { 70, 8 }, { 71, 8 }, { 72, 8 },
    { 73, 8 }, { 74, 8 }, { 75, 40 }, { 255, 0 },
};
AnimFrame updateTileSixFrames0[] = {
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 8, 4 },
    { 9, 4 }, { 10, 4 }, { 11, 4 }, { 12, 4 },
    { 13, 4 }, { 14, 4 }, { 15, 4 }, { 16, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixFrames1[] = {
    { 28, 4 }, { 29, 4 }, { 30, 4 }, { 31, 4 },
    { 32, 4 }, { 33, 4 }, { 34, 4 }, { 35, 4 },
    { 36, 4 }, { 37, 4 }, { 38, 4 }, { 39, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixFrames2[] = {
    { 0, 4 }, { 1, 4 }, { 255, 0 },
};
AnimFrame updateTileSixFrames3[] = {
    { 0, 12 }, { 1, 12 }, { 2, 12 }, { 3, 12 },
    { 4, 12 }, { 5, 12 }, { 6, 12 }, { 7, 12 },
    { 8, 12 }, { 9, 12 }, { 10, 12 }, { 11, 12 },
    { 12, 12 }, { 13, 12 }, { 255, 0 }, { 16, 4 },
    { 15, 4 }, { 14, 4 }, { 13, 4 }, { 12, 4 },
    { 11, 4 }, { 10, 4 }, { 9, 4 }, { 8, 4 },
    { 7, 4 }, { 6, 4 }, { 5, 4 }, { 255, 0x3E7 },
};
AnimFrame updateTileSixFrames4[] = {
    { 39, 4 }, { 38, 4 }, { 37, 4 }, { 36, 4 },
    { 35, 4 }, { 34, 4 }, { 33, 4 }, { 32, 4 },
    { 31, 4 }, { 30, 4 }, { 29, 4 }, { 28, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixWFrames0[] = {
    { 76, 4 }, { 77, 4 }, { 78, 4 }, { 79, 4 },
    { 80, 4 }, { 81, 4 }, { 82, 4 }, { 83, 4 },
    { 84, 4 }, { 85, 4 }, { 86, 4 }, { 87, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixWFrames1[] = {
    { 88, 4 }, { 89, 4 }, { 90, 4 }, { 91, 4 },
    { 92, 4 }, { 93, 4 }, { 94, 4 }, { 95, 4 },
    { 96, 4 }, { 97, 4 }, { 98, 4 }, { 99, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixWFrames2[] = {
    { 0, 4 }, { 1, 4 }, { 255, 0 },
};
AnimFrame updateTileSixWFrames3[] = {
    { 0, 12 }, { 1, 12 }, { 2, 12 }, { 3, 12 },
    { 4, 12 }, { 5, 12 }, { 6, 12 }, { 7, 12 },
    { 8, 12 }, { 9, 12 }, { 10, 12 }, { 255, 0 },
    { 87, 4 }, { 86, 4 }, { 85, 4 }, { 84, 4 },
    { 83, 4 }, { 82, 4 }, { 81, 4 }, { 80, 4 },
    { 79, 4 }, { 78, 4 }, { 77, 4 }, { 76, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileSixWFrames4[] = {
    { 99, 4 }, { 98, 4 }, { 97, 4 }, { 96, 4 },
    { 95, 4 }, { 94, 4 }, { 93, 4 }, { 92, 4 },
    { 91, 4 }, { 90, 4 }, { 89, 4 }, { 88, 4 },
    { 255, 0x3E7 },
};
StageEffectSpot updateStageSpots[] = {
    { 20, 0, 492, 188 },
    { 20, 0, 620, 252 },
};
AnimFrame updateEffectFrames1[] = {
    { 0, 6 }, { 1, 6 }, { 2, 68 }, { 1, 4 },
    { 0, 4 }, { 255, 0 },
};
AnimFrame updateEffectFrames0[] = {
    { 0x12C, 18 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 5, 4 },
    { 6, 4 }, { 7, 4 }, { 5, 4 }, { 6, 4 },
    { 7, 4 }, { 4, 4 }, { 3, 4 }, { 2, 4 },
    { 1, 4 }, { 255, 0x3E7 },
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
AreaBattle area3Battle0 = { 10, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 191, 18, MUSIC(2, 0) };
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
    { 137, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F6, 0x100, 0x2D8, 0, 0x160, 0x1F0 },
    { 0x1C0, 0x100, 0x1F6, 0x120, 0x2D8, 0x20, 0x140, 0x1EF },
    { 0x140, 0x100, 0x176, 0x100, 0xD8, 0, 0x150, 0x1EF },
    { 0x140, 0x100, 0x176, 0x128, 0xD8, 0x28, 0x160, 0x1EF },
    { 0x1C0, 0x100, 0x1F6, 0x140, 0x2D8, 0x40, 0x170, 0x1EF },
    { 0x140, 0x100, 0x176, 0x150, 0xD8, 0x50, 0x140, 0x1EE },
    { 0x140, 0x100, 0x176, 0x178, 0xD8, 0x78, 0x150, 0x1EE },
    { 0x140, 0x100, 0x176, 0x1A0, 0xD8, 0xA0, 0x170, 0x1EE },
    { 0x1C0, 0x100, 0x1E2, 0x178, 0x288, 0x78, 0x140, 0x1ED },
    { 0x1C0, 0x100, 0x1EA, 0x178, 0x2A8, 0x78, 0x150, 0x1ED },
    { 0x1C0, 0x100, 0x1CA, 0x180, 0x228, 0x80, 0x160, 0x1ED },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x40, 0x4E), 0, PROGRESS(0x17), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(0x40, 0x4E), 0, PROGRESS(0x17), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0x40, 0x4E), 0, PROGRESS(0x17), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x40, 0x4F), 0, PROGRESS(0x17), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x26), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0xC, 5, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0x65, 6, 0, 0, 1 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0x67, 7, 0, 0, 1 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0x6C, 8, 571, 235, 1 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x6D, 9, 161, 441, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x6E, 0xA, 255, 489, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x77, 0xB, 235, 452, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x110, 0xC, 640, 265, 1 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x110, 0xC, 0, 0, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x111, 0xD, 0, 0, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x112, 0xE, 0, 0, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 1, 0x40, 2, 0x33, 0, 0, 0, 0, 0, 584, 125, 0, 0 },
    { 1, 2, 0x40, 2, 0x3A, 0, 0, 0, 0, 0, 592, 144, 0, 0 },
    { 1, 3, 0x40, 2, 0x40, 0, 0, 0, 0, 0, 609, 140, 0, 0 },
    { 1, 4, 0x40, 2, 0x45, 0, 0, 0, 0, 0, 610, 167, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 4, 0, 160, 509, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 4, 0, 392, 457, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 4, 0, 448, 339, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 4, 0, 545, 307, 0, 0 },
    { 0, 6, 0x80, 2, 0x1C, 0, 0, 0, 0, 0, 550, 116, 0, 0 },
    { 0, 0xC, 0xFF, 2, 0x58, 0, 0, 0, 0, 0, 430, 103, 0, 0 },
    { 0, 8, 0x80, 2, 2, 0, 0, 0, 0, 0, 550, 116, 0, 0 },
    { 0, 9, 0x80, 2, 3, 0, 0, 0, 0, 0, 550, 116, 0, 0 },
    { 0, 0xA, 0x80, 2, 4, 0, 0, 0, 0, 0, 550, 116, 0, 0 },
    { 0, 5, 0x80, 2, 5, 0, 0, 0, 0, 0, 550, 116, 0, 0 },
    { 0, 7, 0x80, 2, 0x28, 0, 0, 0, 0, 0, 546, 112, 0, 0 },
    { 0, 0xE, 0xFF, 2, 0x11, 0, 0, 0, 0, 0, 430, 103, 0, 0 },
    { 0, 0xF, 0xFF, 2, 0x12, 0, 0, 0, 0, 0, 430, 103, 0, 0 },
    { 0, 0x10, 0xFF, 2, 0x13, 0, 0, 0, 0, 0, 430, 103, 0, 0 },
    { 0, 0xB, 0xFF, 2, 0x4C, 0, 0, 0, 0, 0, 430, 103, 0, 0 },
    { 0, 0xD, 0xFF, 2, 0x29, 0, 0, 0, 0, 0, 426, 99, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 4, 0, 72, 464, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 4, 0, 178, 350, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 4, 0, 303, 267, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 4, 0, 369, 219, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 392, 328, 386, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 545, 243, 266, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x218, 0x178, 0xFC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 680, script680, EVENT_TEXT(0xA), NULL, endEvent680 },
    { 681, script681, EVENT_TEXT(0xB), NULL, endEvent681 },
    { 682, script682, EVENT_TEXT(0xC), NULL, endEvent682 },
    { 961, script961, EVENT_TEXT(0x14), NULL, endEvent961 },
    { -1, NULL, 0, NULL, NULL },
};
