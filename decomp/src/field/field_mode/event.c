/* The events: their scripts of commands, or a task of their own. Their
   rodata, at 0x80082598 (USA), starts FIELDSTG.PRO's third object (see
   data/fieldstg.c). */

#include "field/field_mode.h"

/* The actor of an event's cast with the id, below the script commands' ids
   (0x320) */
Actor *FIELDSTG_findEventActor(EventTask *task, s32 id) {
    s32 i;

    if (id < 0x320) {
        for (i = 0; i < 30; i++) {
            if (task->entries[i].id == 0) {
                break;
            }
            if (task->entries[i].id == id) {
                return task->entries[i].actor;
            }
        }
    }
    return NULL;
}

/* An event's pose command (SCRIPT_POSE): puts its actor in a pose, or, for
   an id of the script commands (SCRIPT_COMMAND), creates the command's task
   if needed and hands it the other two words, a command and its argument */
s32 FIELDSTG_setEventPose(EventTask *task, s16 *op, EventChildren *children) {
    s32 id = op[1];
    s32 set = op[2]; /* or the command */
    s32 dir = op[3]; /* or its argument */
    Actor *actor;
    s32 target;
    s32 i;

    if (id < 0x320) {
        actor = FIELDSTG_findEventActor(task, id);
        if (actor != NULL) {
            actor->setPose(actor, set, dir);
        }
    } else {
        target = (s32)TASK_REGISTRY.funcs.find(id, -1, -1);
        if (target == 0) {
            for (i = 0; i < 10; i++) {
                if (children->scripts[i] == 0) {
                    children->scripts[i] = FIELDSTG_createScriptCommand(id);
                    if (children->scripts[i] != 0) {
                        target = children->scripts[i];
                    }
                    break;
                }
            }
        }
        if (target != 0) {
            FIELDSTG_handleScriptCommand(target, id, set, dir);
        }
    }
    return 4;
}

/* Gives the characters the event held their controls back */
static inline void releaseEventActors(EventTask *task) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (task->entries[i].id == 0) {
            break;
        }
        task->entries[i].actor->resetControl(task->entries[i].actor);
    }
}

/*
 * Runs an event: in TASK_INIT it waits for the event's text file and the
 * field (FIELD_TASK_FIELD) and holds the characters; in TASK_RUN it runs the script
 * until a command waits (kind 0 ends it, 1 moves or turns a character, 2
 * opens a message box, 3 waits, 6 runs FIELDSTG_followWithCamera or FIELDSTG_pointCamera), or
 * without a script runs start's task until it ends; TASK_KILL releases the
 * characters and calls end. The match depends on next and actor being two
 * variables, and sub an s16.
 */
void FIELDSTG_runEvent(EventTask *task, EventChildren *children) {
    Actor *actor;
    Actor *next;
    s16 *pc;
    s32 running;
    s16 sub;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        if ((FIELDSTG_state.eventText == 0 || !FILE_CACHE.isLoading(FIELDSTG_state.eventText >> 16)) &&
            ((Task *)TASK_REGISTRY.funcs.find(FIELD_TASK_FIELD, -1, -1))->state == TASK_RUN) {
            next = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, -1);
            for (i = 0; next != NULL; next = TASK_REGISTRY.funcs.findNext()) {
                actor = next;
                task->entries[i].actor = actor;
                task->entries[i].id = actor->key1;
                i++;
                actor->startWalk(actor);
                if (i == 30) {
                    break;
                }
            }
            task->nextState(task);
        }
        break;
    case TASK_RUN:
        if (task->pc != NULL) {
            pc = task->pc;
            running = 1;
            do {
                sub = *pc & 0xFF;
                switch (*pc >> 8) {
                case 0:
                default:
                    task->setState(task, TASK_KILL);
                    running = 0;
                    break;
                case 1:
                    switch (sub) {
                    case 0:
                    default:
                        actor = FIELDSTG_findEventActor(task, pc[1]);
                        if (actor != NULL) {
                            actor->pos.x = pc[2] << 8;
                            actor->pos.y = pc[3] << 8;
                        }
                        pc += 4;
                        break;
                    case 1:
                        pc += FIELDSTG_setEventPose(task, pc, children);
                        break;
                    case 2:
                        actor = FIELDSTG_findEventActor(task, pc[1]);
                        if (actor != NULL) {
                            actor->setGoal(actor, pc[2], pc[3], pc[4]);
                        }
                        pc += 5;
                        break;
                    }
                    break;
                case 2: {
                    s32 box = pc[1];
                    s32 text = pc[2];
                    s32 kind = pc[4];
                    s32 id = pc[3];

                    actor = NULL;
                    if (kind != 4) {
                        actor = FIELDSTG_findEventActor(task, id);
                        if (actor != NULL) {
                            children->boxes[box] = FIELDSTG_createSpeech(actor, text, kind, 0);
                        }
                    } else {
                        children->boxes[box] = FIELDSTG_createSpeech(actor, text, 4, 1);
                    }
                    pc += 5;
                    break;
                }
                case 3:
                    switch (sub) {
                    case 0:
                    default:
                        if (task->wait == 0) {
                            task->wait = pc[1];
                        }
                        if (--task->wait != 0) {
                            running = 0;
                        } else {
                            pc += 2;
                        }
                        break;
                    case 1:
                        if (children->boxes[0] != NULL) {
                            running = 0;
                        } else {
                            pc += 1;
                        }
                        break;
                    case 2:
                        actor = FIELDSTG_findEventActor(task, pc[1]);
                        if (actor != NULL && actor->isWalking(actor)) {
                            running = 0;
                        } else {
                            pc += 2;
                        }
                        break;
                    case 3:
                        actor = FIELDSTG_findEventActor(task, pc[1]);
                        if (actor == NULL || actor->isAnimDone(actor)) {
                            pc += 2;
                        } else {
                            running = 0;
                        }
                        break;
                    case 4:
                        FIELDSTG_leaveField(pc[1], -1, pc[2] << 8, pc[3] << 8, pc[4]);
                        running = 0;
                        task->setState(task, TASK_DONE);
                        break;
                    }
                    break;
                case 6:
                    switch (sub) {
                    case 0:
                    default:
                        FIELDSTG_followWithCamera(pc[1], pc[2]);
                        pc += 3;
                        break;
                    case 1:
                        FIELDSTG_pointCamera(pc[1], pc[2], pc[3]);
                        pc += 4;
                        break;
                    }
                    break;
                }
            } while (running);
            task->pc = pc;
            break;
        }
        switch (task->substate) {
        case 0:
        default:
            children->task = task->start();
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            if (children->task == NULL) {
                task->setState(task, TASK_KILL);
            }
            break;
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        releaseEventActors(task);
        FIELDSTG_state.busy = 0;
        if (task->end != NULL) {
            task->end();
        }
        break;
    }
}

/* Starts the event id of FieldState.events: its script, start and end, and
   its text; the player stands still, except for the events 8000 to 8999 */
EventTask *FIELDSTG_startEvent(s32 id) {
    EventTask *task = createTask(FIELDSTG_runEvent, sizeof(EventTask), 0x38);
    FieldEvent *entry;
    Task *other;
    s32 text;

    for (entry = FIELDSTG_state.events; entry->id != -1; entry++) {
        if (entry->id == id) {
            task->event = id;
            task->pc = entry->script;
            task->start = entry->start;
            task->end = entry->end;
            FIELDSTG_state.eventText = text = entry->text;
            if (text != 0) {
                FIELDSTG_state.eventText = text + (TEXT_FILE(1) << 16);
                FILE_CACHE.request(entry->text >> 16);
            }
            FIELDSTG_state.busy = 1;
            if (id < 8000 || id >= 9000) {
                other = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
                if (other != NULL && other->state == TASK_RUN) {
                    other->setSubstate(other, ACTOR_STAND);
                }
            }
            break;
        }
    }
    if (FIELDSTG_state.busy == 0) {
        task->setState(task, TASK_KILL);
    }
    return task;
}
