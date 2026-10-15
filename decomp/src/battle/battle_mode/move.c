/* FIGHTSTG's jumps and moves of a fighter's model. */

#include "battle/battle_mode.h"

/* The jump's task: lifts its model and drops it along ease curves at its
   speed; kind 4 also moves it forward from home, 5 back home and 6 drops it
   to its home height, mirrored for the enemy */
void FIGHTSTG_updateJump(Jump *task) {
    s32 y;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->zTravel = 0;
        switch (task->kind) {
        case 4:
            task->zTravel = task->distance + 0x2800;
            break;
        case 5:
            task->zTravel = task->control->pos.z - task->control->homePos.z;
            if (task->zTravel < 0) {
                task->zTravel = -task->zTravel;
            }
            break;
        }
        if (task->control->id == 0x10) {
            task->zTravel = -task->zTravel;
        }
        task->t = FIGHTSTG_battle.frames * task->speed;
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->kind) {
        default:
            y = FIGHTSTG_interp.ease(2, task->t, task->height);
            task->control->pos.y = FIGHTSTG_interp.ease(1, task->t, task->y) - y;
            break;
        case 4:
        case 5:
            task->control->pos.y = task->control->homePos.y - FIGHTSTG_interp.ease(2, task->t, task->height);
            break;
        case 6:
            task->control->pos.y = FIGHTSTG_interp.ease(0, task->t, task->control->homePos.y);
            break;
        }
        if (task->kind == 4) {
            task->control->pos.z = task->control->homePos.z + FIGHTSTG_interp.ease(0, task->t, task->zTravel);
        }
        if (task->kind == 5) {
            task->control->pos.z = task->control->homePos.z + task->zTravel - FIGHTSTG_interp.ease(0, task->t, task->zTravel);
        }
        task->t += FIGHTSTG_battle.frames * task->speed;
        if (task->t < 0x1000) {
            break;
        }
        task->nextState(task);
        break;
    case TASK_DONE:
        switch (task->kind) {
        default:
            task->control->pos.y = 0;
            break;
        case 4:
        case 5:
        case 6:
            task->control->pos.y = task->control->homePos.y;
            break;
        }
        if (task->kind == 4) {
            task->control->pos.z = task->control->homePos.z + task->zTravel;
        }
        if (task->kind == 5) {
            task->control->pos.z = task->control->homePos.z;
        }
        task->nextState(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* the jumps of FIGHTSTG_startJump, by kind from 1 */
JumpParams FIGHTSTG_jumps[] = {
    { 640, 136 }, { 1280, 102 }, { 1920, 81 }, { 2560, 64 }, { 1280, 64 }, { 0, 42 },
};

/* Starts a jump of control's model of kind 1-6 (FIGHTSTG_jumps), forward by
   distance for kind 4 */
Jump *FIGHTSTG_startJump(ModelControl *control, s32 kind, s32 distance) {
    Jump *task = createTask(FIGHTSTG_updateJump, sizeof(Jump), 0);

    task->kind = kind;
    task->control = control;
    task->distance = distance;
    task->y = control->pos.y;
    task->height = FIGHTSTG_jumps[kind - 1].height;
    task->speed = FIGHTSTG_jumps[kind - 1].speed;
    return task;
}

/* The move's task: takes the model from from to to along tStep, a step each
   frame, and dies there */
void FIGHTSTG_updateMove(MoveTask *task) {
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    s32 t;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->t = FIGHTSTG_battle.frames * task->tStep;
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        t = task->t;
        from.vx = task->from.x;
        from.vy = task->from.y;
        from.vz = task->from.z;
        to.vx = task->to.x;
        to.vy = task->to.y;
        to.vz = task->to.z;
        FIGHTSTG_interp.lerp(&from, &to, t, &out);
        task->control->pos.x = out.vx;
        task->control->pos.y = out.vy;
        task->control->pos.z = out.vz;
        task->t += FIGHTSTG_battle.frames * task->tStep;
        if (task->t >= 0x1000) {
            task->control->pos.x = task->to.x;
            task->control->pos.y = task->to.y;
            task->control->pos.z = task->to.z;
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Moves control's model from where it is to to over time frames */
MoveTask *FIGHTSTG_startMove(ModelControl *control, ShortVec3 *to, s32 time) {
    MoveTask *task = createTask(FIGHTSTG_updateMove, sizeof(MoveTask), 0);

    task->control = control;
    task->to = *to;
    task->from = control->pos;
    task->t = 0;
    task->tStep = 0x1000 / time;
    return task;
}
