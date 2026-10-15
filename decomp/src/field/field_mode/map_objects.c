/* The map's objects: animated, drawn and found by their animation */

#include "field/field_mode.h"

/* The layer's sorted callback that draws a map object */
void FIELDSTG_drawMapObject(MapObjects *task, Layer *layer, s32 index) {
    StageTile *object = &task->objects[index];
    SpriteDrawer sprite;

    if (task->state == TASK_RUN) {
        initSpriteDrawer(&sprite);
        sprite.setAltClut(0, FIELD_OBJECTS_CLUT_Y);
        sprite.setLayer(layer, object->depth);
        if (object->anim != 0xFF) {
            sprite.setTexture(FIELD_OBJECTS_X, FIELD_OBJECTS_Y);
            sprite.setClutRow(object->clutRow);
            if (FIELDSTG_state.spriteColor.cd != 0) {
                sprite.setColor(&FIELDSTG_state.spriteColor);
            }
            sprite.draw(FILE_CACHE.getEntry(task->sprites), object->frame, object->x, object->y);
        } else {
            sprite.setTexture(FIELD_SPRITES_X, FIELD_SPRITES_Y);
            sprite.setClutRow(object->clutRow);
            sprite.draw(FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16), object->frame, object->x, object->y);
        }
    }
}

/* Animates and draws the map's objects that are in view */
void FIELDSTG_updateMapObjects(MapObjects *task, HiddenSpots **children) {
    StageTile *object;
    StageTile *entry;
    Layer *layer;
    SpriteDrawer sprite;
    RECT view;
    s32 count;
    s32 i;
    s32 x;
    s32 y;
    u8 margin;
    s32 back;

    switch (task->state) {
    case TASK_INIT:
    default:
        count = 0;
        for (entry = task->objects; entry->y != 0; entry++) {
            if (entry->anim == 0xFF) {
                count++;
            }
        }
        if (count != 0) {
            *children = FIELDSTG_createHiddenSpots(count);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        object = task->objects;
        layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
        layer->getViewRect(layer, &view);
        initSpriteDrawer(&sprite);
        sprite.setTexture(FIELD_OBJECTS_X, FIELD_OBJECTS_Y);
        sprite.setAltClut(0, FIELD_OBJECTS_CLUT_Y);
        for (i = 0; object->margin != 0; object++, i++) {
            if (object->visible == 0) {
                continue;
            }
            x = object->anim == 0xFF ? object->x - 50 : object->x;
            y = object->anim == 0xFF ? object->y - 100 : object->y;
            margin = object->margin;
            if (x >= view.x - margin && view.x + view.w >= x && y >= view.y - margin && view.y + view.h >= y) {
                switch (object->cycle) {
                case 1:
                    if (object->cycleTime < 0x100) {
                        object->frame++;
                        if (object->frame > object->cycleLast) {
                            object->frame = object->cycleFirst;
                        }
                        object->cycleTime = object->cycleDelay << 8;
                    } else {
                        object->cycleTime -= 0x100;
                    }
                    break;
                case 2:
                    if (object->cycleTime < 0x100) {
                        object->clutRow++;
                        if (object->clutRow > object->cycleLast) {
                            object->clutRow = object->cycleFirst;
                        }
                        object->cycleTime = object->cycleDelay << 8;
                    } else {
                        object->cycleTime -= 0x100;
                    }
                    break;
                case 3:
                    if (object->cycleTime & 0x8000) {
                        back = 0x8000;
                        object->cycleTime &= 0x7FFF;
                        if (object->cycleTime < 0x100) {
                            object->clutRow--;
                            object->cycleTime = object->cycleDelay << 8;
                            if (object->clutRow == (u8)(object->cycleFirst - 1)) {
                                object->clutRow = object->cycleFirst + 1;
                                back = 0;
                            }
                        } else {
                            object->cycleTime -= 0x100;
                        }
                        object->cycleTime |= back;
                    } else if (object->cycleTime < 0x100) {
                        object->clutRow++;
                        object->cycleTime = object->cycleDelay << 8;
                        if (object->clutRow == object->cycleLast + 1) {
                            object->clutRow = object->cycleLast - 1;
                            object->cycleTime |= 0x8000;
                        }
                    } else {
                        object->cycleTime -= 0x100;
                    }
                    break;
                }
                if (object->sortY != 0) {
                    layer->addSortedCallback(layer, FIELDSTG_drawMapObject, task, object->sortY, i);
                } else {
                    sprite.setLayerId(FIELD_LAYER_MAP, object->depth);
                    sprite.setClutRow(object->clutRow);
                    if (FIELDSTG_state.spriteColor.cd != 0) {
                        sprite.setColor(&FIELDSTG_state.spriteColor);
                    }
                    sprite.draw(FILE_CACHE.getEntry(task->sprites), object->frame, object->x, object->y);
                }
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the map's objects, with their sprites */
MapObjects *FIELDSTG_createMapObjects(s32 sprites, StageTile *objects) {
    MapObjects *task = createTask(FIELDSTG_updateMapObjects, sizeof(MapObjects), 4);

    task->objects = objects;
    task->sprites = sprites;
    return task;
}

/* The next map object of the animation FIELDSTG_findObject looks for, or
   NULL */
StageTile *FIELDSTG_findNextObject(void) {
    StageTile *entry = FIELDSTG_objectCursor;
    s32 found = 0;

    for (; entry->margin != 0; entry++) {
        if (entry->anim == FIELDSTG_objectId) {
            found = 1;
            break;
        }
    }
    FIELDSTG_objectCursor = entry + 1;
    if (found) {
        return entry;
    }
    return NULL;
}

/* The first map object of an animation, or NULL (FIELDSTG_findNextObject
   goes on) */
StageTile *FIELDSTG_findObject(s32 anim) {
    FIELDSTG_objectId = anim;
    FIELDSTG_objectCursor = FIELDSTG_state.objects;
    return FIELDSTG_findNextObject();
}
