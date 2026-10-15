/* The map's triggers: exits, climbs, warps, slides and the other things the
   player sets off on a cell. Their rodata, at 0x800825E0 (USA), starts
   FIELDSTG.PRO's fifth object (see data/fieldstg.c). */

#include "field/field_mode.h"

/* Finds the slot under the player (FIELD_MAP_TRIGGERS); those up to SLOT_GAUGE
   count only while the player faces them */
s32 FIELDSTG_findTrigger(Triggers *task) {
    Actor *actor = task->actor;
    Vec2 tile;
    u32 cell;
    s32 type;

    tile = actor->tile;
    cell = (u8)FIELDSTG_map.getCell(FIELD_MAP_TRIGGERS, &tile);
    if (cell == 0) {
        return 0;
    }
    task->dir = cell >> 5;
    task->index = cell & 0x1F;
    task->entry = &task->entries[task->index];
    type = task->entry->type;
    switch (type) {
    case SLOT_DEPTH:
    case SLOT_MAP:
    case SLOT_EVENT:
    case SLOT_WARP1:
    case SLOT_WARP0:
    case SLOT_SLIDE:
    case SLOT_STOP_SLIDE:
    case SLOT_LAUNCH:
    case SLOT_LAUNCH_OUT:
        return 1;
    }
    return FIELDSTG_nearDirs[task->dir][actor->dir] != 0;
}

/* Acts at once on a slot that does (SLOT_DEPTH...), or shows the balloon of
   one that waits for a button and returns 1; returns 0 when its conditions
   don't hold */
s32 FIELDSTG_offerTrigger(Triggers *task, TriggerChildren *children) {
#if VERSION_EU || VERSION_JP
    s32 arg;
#endif

    if (task->entry->conditions[0][0] != 0xFFFF
        && FLAGS_00.checkCondition(task->entry->conditions[0][0], task->entry->conditions[0][1]) == 0) {
        return 0;
    }
    if (task->entry->conditions[1][0] != 0xFFFF
        && FLAGS_00.checkCondition(task->entry->conditions[1][0], task->entry->conditions[1][1]) == 0) {
        return 0;
    }
    switch (task->entry->type) {
        case SLOT_DEPTH:
            task->actor->depth = task->entry->arg;
            GAME.playerDepth = task->entry->arg;
            return 0;
        case SLOT_MAP:
            FIELDSTG_map.setMap(task->entry->arg);
            return 0;
        case SLOT_EVENT:
            if (children->script == NULL) {
                children->script = FIELDSTG_startEvent(task->entry->arg);
            }
            return 0;
        case SLOT_SLIDE:
            task->actor->startSlide(task->actor, task->dir);
            return 0;
        case SLOT_STOP_SLIDE:
            task->actor->stopSlide(task->actor);
            return 0;
        case SLOT_LAUNCH:
            task->actor->launch(task->actor, SLOT_DEST(task->entry));
            return 0;
    }
#if VERSION_US
    if (children->balloon != NULL) {
        children->balloon->setState(children->balloon, TASK_RUN);
        return 1;
    }
    switch (task->entry->type) {
        default:
            children->balloon = FIELDSTG_createBalloon(0, 0, 6);
            break;
        case SLOT_CLIMB_UP:
        case SLOT_CLIMB_DOWN:
            children->balloon = FIELDSTG_createBalloon(0, 2, 6);
            break;
        case SLOT_DROP:
            children->balloon = FIELDSTG_createBalloon(0, 4, 6);
            break;
        case SLOT_GAUGE:
            children->balloon = FIELDSTG_createBalloon(0, 5, 6);
            break;
        case SLOT_EXIT:
            if (task->dir == 4) {
                children->balloon = FIELDSTG_createBalloon(0, 10, 6);
            } else {
                children->balloon = FIELDSTG_createBalloon(0, (task->dir >> 1) + 6, 6);
            }
            break;
    }
#elif VERSION_EU || VERSION_JP
    /* the European version restarts a running animation on the new row */
    switch (task->entry->type) {
        default:
            arg = 0;
            break;
        case SLOT_CLIMB_UP:
        case SLOT_CLIMB_DOWN:
            arg = 2;
            break;
        case SLOT_DROP:
            arg = 4;
            break;
        case SLOT_GAUGE:
            arg = 5;
            break;
        case SLOT_EXIT:
            if (task->dir == 4) {
                arg = 10;
            } else {
                arg = (task->dir >> 1) + 6;
            }
            break;
    }
    if (children->balloon != NULL) {
        children->balloon->setState(children->balloon, TASK_RUN);
        children->balloon->key2 = arg;
        children->balloon->frame = 0;
        children->balloon->time = 0;
    } else {
        children->balloon = FIELDSTG_createBalloon(0, arg, 6);
    }
#endif
    return 1;
}

const Vec2 FIELDSTG_noOffset = {0, 0};

/* Sets off the slot the player pressed cross on */
void FIELDSTG_setOffTrigger(Triggers *task) {
    StageTile *object;
    s32 id;

    switch (task->entry->type) {
        case SLOT_EXIT:
            task->actor->walkInDir(task->actor, task->dir);
            FIELDSTG_leaveField(task->entry->arg, -1, task->entry->x << 8, task->entry->y << 8, task->entry->dir);
            if (task->entry->hideAnim != 0) {
                object = FIELDSTG_state.objects;
                id = task->entry->hideAnim;
                for (; object->margin != 0; object++) {
                    if (object->anim == id) {
                        object->visible = 0;
                    }
                }
            }
            GAME.place = task->entry->place;
            GAME.placeArg = task->entry->placeArg;
            break;
        case SLOT_LAUNCH_OUT:
            task->actor->launch(task->actor, SLOT_DEST(task->entry));
            FIELDSTG_leaveFieldAfter(task->entry->arg, -1, task->entry->x << 8, task->entry->y << 8, task->entry->dir,
                          0x3C);
            break;
        case SLOT_CLIMB_UP:
            task->actor->climbUp(task->actor, task->dir, task->entry->x, task->entry->y,
                                (task->entry->arg - 1) * 16);
            break;
        case SLOT_CLIMB_DOWN:
            task->actor->climbDown(task->actor, task->dir == 1 ? 5 : 3, task->entry->x, task->entry->y,
                                (task->entry->arg - 1) * 16);
            break;
        case SLOT_DROP:
            task->actor->dropDown(task->actor, task->dir, FIELDSTG_noOffset, task->entry->arg * 16);
            break;
        case SLOT_GAUGE:
            task->actor->playGauge(task->actor, task->dir, (Vec2){task->entry->arg, task->entry->x});
            break;
        case SLOT_WARP0:
            task->actor->warp(task->actor, SLOT_DEST(task->entry), 0);
            break;
        case SLOT_WARP1:
            task->actor->warp(task->actor, SLOT_DEST(task->entry), 1);
            break;
    }
}

/* The triggers' update: offers the slot under the player while the field
   isn't busy, and sets it off on cross */
void FIELDSTG_updateTriggers(Triggers *task, TriggerChildren *children) {
    task->entries = FIELDSTG_state.slots;
    switch (task->state) {
        default:
        case TASK_INIT:
            task->actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
            if (task->actor != NULL) {
                task->nextState(task);
            }
            break;
        case TASK_RUN:
            if (FIELDSTG_state.busy != 0 || FIELDSTG_state.frozen != 0) {
                break;
            }
            switch (task->substate) {
                default:
                case 0:
                    if (FIELDSTG_findTrigger(task) != 0 && FIELDSTG_offerTrigger(task, children) != 0) {
                        task->nextSubstate(task);
                    }
                    break;
                case 1:
                    if (FIELDSTG_findTrigger(task) == 0) {
                        task->setSubstate(task, 0);
                        children->balloon->setState(children->balloon, TASK_DONE);
                    } else if ((PAD.getPressed(0) & (1 << PAD_CROSS)) && FIELDSTG_state.frozen == 0) {
                        children->balloon->setState(children->balloon, TASK_KILL);
                        FIELDSTG_setOffTrigger(task);
                        task->nextSubstate(task);
                    }
                    break;
                case 2:
                    if (task->actor->substate < ACTOR_WALK_OUT) {
                        task->setSubstate(task, 0);
                    }
                    break;
            }
            break;
        case TASK_DONE:
        case TASK_KILL:
            break;
    }
}

/* Creates the triggers */
Triggers *FIELDSTG_createTriggers(s32 sheetEntry, StageSlot *slots) {
    Triggers *task = createTask(FIELDSTG_updateTriggers, sizeof(Triggers), 8);

    task->sheetEntry = sheetEntry;
    return task;
}
