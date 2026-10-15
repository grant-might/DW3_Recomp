/* The map's hidden spots, one of which holds a prize, and their effects */

#include "field/field_mode.h"

/* The hint after a search without the prize: a sprite that flashes faster
   the closer the prize is */
void FIELDSTG_showSpotHint(SpotHint *task) {
    SpriteDrawer sprite;
    s32 dx;
    s32 dy;
    void *sprites;

    switch (task->state) {
        default:
        case TASK_INIT:
            dx = task->from.x - task->to.x;
            if (dx < 0) {
                dx = -dx;
            }
            dy = task->from.y - task->to.y;
            if (dy < 0) {
                dy = -dy;
            }
            if (dx < 0x80 && dy < 0x80) {
                task->speed = 3;
                task->frame = 0x52;
            } else if (dx < 0x100 && dy < 0x100) {
                task->speed = 6;
                task->frame = 0x51;
            } else {
                task->speed = 0xC;
                task->frame = 0x50;
            }
            task->nextState(task);
            /* fallthrough */
        case TASK_RUN:
            sprites = FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16 | 1);
            initSpriteDrawer(&sprite);
            sprite.setLayerId(FIELD_LAYER_MAP, 0);
            sprite.setTexture(FIELD_SPRITES2_X, FIELD_SPRITES_Y);
            sprite.setFollowScroll(0);
            sprite.setClutRow(task->time / task->speed % 10);
            sprite.draw(sprites, task->frame, 0xF8, 0xA8);
            sprite.draw(sprites, 0x4F, 0xF8, 0xA8);
            task->time += GFX.funcs.getFrameTime();
            if (task->time >= 0x78) {
                task->setState(task, TASK_KILL);
            }
            break;
        case TASK_DONE:
        case TASK_KILL:
            break;
    }
}

/* Creates the hint for a search at to, from the prize at from */
SpotHint *FIELDSTG_createSpotHint(Vec2 from, Vec2 to) {
    SpotHint *task = createTask(FIELDSTG_showSpotHint, sizeof(SpotHint), 0);

    task->from = from;
    task->to = to;
    return task;
}

/* Hides the prize in one of the hidden spots at random (GAME.prizeSpot keeps
   it) */
void FIELDSTG_hidePrize(HiddenSpots *task) {
    s32 index = RANDOM.next() % task->count;

    GAME.prizeSpot = index;
    task->entries[index].hasPrize = 1;
    task->pos = task->entries[index].pos;
}

/* The hidden spots' update: a search plays its effect, then starts a battle
   where the prize is (and hides it again) or shows a hint, and animates the
   searched spot's object */
void FIELDSTG_updateHiddenSpots(HiddenSpots *task, HiddenSpotsChildren *children) {
    StageTile *objects;
    s32 step;
    s32 time;
    s32 i;

    switch (task->state) {
    default:
    case TASK_INIT:
        FILE_CACHE.request(FIELD_SEARCH_FILE);
        task->nextState(task);
        break;
    case TASK_RUN:
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            children->effect = FIELDSTG_createSpotEffect(task->forEvent);
            task->nextSubstate(task);
        } else if (task->substate != 0x80) {
            if (task->substate < 0x14) {
                task->substate = task->substate + GFX.funcs.getFrameTime() + 1;
            } else {
                if (task->forEvent == 0) {
                    if (task->entries[task->selected].hasPrize != 0) {
                        if ((RANDOM.next() & 0x7F) < 0x66) {
                            FIELDSTG_battleFuncs.startEventBattle(3);
                        } else {
                            FIELDSTG_battleFuncs.startEventBattle(6);
                        }
                        FIELDSTG_hidePrize(task);
                    } else {
                        if (children->hint != NULL) {
                            children->hint->destroy(children->hint);
                        }
                        children->hint = FIELDSTG_createSpotHint(task->pos, task->entries[task->selected].pos);
                    }
                }
                task->setSubstate(task, 0x80);
            }
        }
        step = task->step;
        time = task->counter;
        time += GFX.funcs.getFrameTime();
        if (FIELDSTG_spotAnim[step][1] < time) {
            time -= FIELDSTG_spotAnim[step][1];
            step++;
            if (FIELDSTG_spotAnim[step][0] == 0xFF) {
                task->setState(task, TASK_RUN);
                return;
            }
            task->entries[task->selected].frame = FIELDSTG_spotAnim[step][0];
            task->step = step;
        }
        task->counter = time;
        objects = FIELDSTG_state.objects;
        for (i = 0; i < task->count; i++) {
            if (task->entries[i].frame != 0) {
                if (task->selected == i) {
                    objects[task->entries[i].object].frame = task->entries[i].frame;
                } else {
                    objects[task->entries[i].object].frame = 0x38;
                }
            }
        }
        break;
    case TASK_KILL:
        if (task->entries != NULL) {
            HEAP.free(task->entries);
        }
        break;
    }
}

/* Creates the hidden spots, the map objects of animation 0xFF, and hides the
   prize in one (or where it was, in the same mode) */
HiddenSpots *FIELDSTG_createHiddenSpots(s32 count) {
    HiddenSpots *task = createTaskWithId(FIELDSTG_updateHiddenSpots, sizeof(HiddenSpots), 8, FIELD_TASK_HIDDEN_SPOTS);
    StageTile *object;
    s32 i;
    s32 n;
    s32 index;

    task->count = count;
    object = FIELDSTG_state.objects;
    task->entries = HEAP.alloc(count * sizeof(HiddenSpot), 2);
    i = 0;
    n = 0;
    for (; object->y != 0; object++, i++) {
        if (object->anim == 0xFF) {
            task->entries[n].pos.x = object->x;
            task->entries[n].pos.y = object->y;
            task->entries[n].object = i;
            task->entries[n].frame = 0x38;
            task->entries[n].hasPrize = 0;
            n++;
        }
    }
    if (n != 0) {
        if (GAME.clearTempFlags != 0) {
            FIELDSTG_hidePrize(task);
        } else {
            index = GAME.prizeSpot;
            task->entries[index].hasPrize = 1;
            task->pos = task->entries[index].pos;
        }
    }
    return task;
}

/* The hidden spots, when one is within 10 pixels of pos (selecting it if
   select), or NULL */
HiddenSpots *FIELDSTG_findHiddenSpot(Vec2 *pos, s32 select) {
    HiddenSpots *task = TASK_REGISTRY.funcs.find(FIELD_TASK_HIDDEN_SPOTS, -1, -1);
    s32 i;

    if (task != NULL) {
        for (i = 0; i < task->count; i++) {
            if (pos->x >= task->entries[i].pos.x - 10 && task->entries[i].pos.x + 10 >= pos->x
                && pos->y >= task->entries[i].pos.y - 10 && task->entries[i].pos.y + 10 >= pos->y) {
                if (select) {
                    task->selected = i;
                    task->forEvent = 0;
                }
                return task;
            }
        }
    }
    return NULL;
}

/* Searches the hidden spots off the map (x 1000 and up), for an event */
void FIELDSTG_searchEventSpot(void) {
    HiddenSpots *task = TASK_REGISTRY.funcs.find(FIELD_TASK_HIDDEN_SPOTS, -1, -1);
    s32 i;

    if (task != NULL) {
        for (i = 0; i < task->count; i++) {
            if (task->entries[i].pos.x >= 1000) {
                task->selected = i;
                task->forEvent = 1;
                task->setState(task, TASK_DONE);
            }
        }
    }
}

/* The layer's sorted callback that draws a search's effect */
void FIELDSTG_drawSpotEffect(SpotEffect *task, Layer *layer) {
    SpriteDrawer sprite;

    if (task->state == TASK_RUN) {
        initSpriteDrawer(&sprite);
        sprite.setTexture(FIELD_SPRITES_X, FIELD_SPRITES_Y);
        sprite.setLayer(layer, 4);
        sprite.draw(FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16), task->frame, task->x, task->y);
    }
}

/* A search's effect, over the player or, for an event, its actor 0x11B */
void FIELDSTG_updateSpotEffect(SpotEffect *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    Actor *actor;
    s32 step;
    s32 time;
    u8 *anim;

    switch (task->state) {
        default:
        case TASK_INIT:
            if (task->key1 != 0) {
                actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, 0x11B, -1);
            } else {
                actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
            }
            if (actor == NULL) {
                break;
            }
            task->x = actor->tile.x;
            task->y = actor->tile.y;
            task->dir = actor->dir;
            task->anim = FIELDSTG_dirAnims[task->dir];
            task->nextState(task);
            SOUND.playSound(SOUND_PLAYER08);
            /* fallthrough */
        case TASK_RUN:
            step = task->step;
            time = task->counter;
            time += GFX.funcs.getFrameTime();
            anim = task->anim;
            if (anim[step * 2 + 1] < time) {
                time -= anim[step * 2 + 1];
                step++;
                if (anim[step * 2] == 0xFF) {
                    task->setState(task, TASK_KILL);
                    break;
                }
                task->frame = anim[step * 2];
                task->step = step;
            }
            task->counter = time;
            if (task->frame != 0) {
                layer->addSortedCallback(layer, FIELDSTG_drawSpotEffect, task, task->y + FIELDSTG_dirDepths[task->dir], 0);
            }
            break;
        case TASK_DONE:
        case TASK_KILL:
            break;
    }
}

/* Creates a search's effect, over the event's actor 0x11B if forEvent, else
   over the player */
SpotEffect *FIELDSTG_createSpotEffect(s32 forEvent) {
    SpotEffect *task = createTask(FIELDSTG_updateSpotEffect, sizeof(SpotEffect), 0);

    task->key1 = forEvent;
    return task;
}
