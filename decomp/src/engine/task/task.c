#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Task method: goes to state `state`, from substate 0 */
void taskSetState(Task *task, s32 state) {
    task->state = state;
    task->substate = 0;
    task->step = 0;
    task->counter = 0;
}

/* Task method: goes to substate `substate`, from step 0 */
void taskSetSubstate(Task *task, s32 substate) {
    task->substate = substate;
    task->step = 0;
    task->counter = 0;
}

/* Task method: goes to step `step`, with its counter at 0 */
void taskSetStep(Task *task, s32 step) {
    task->step = step;
    task->counter = 0;
}

/* Task method: sets the counter */
void taskSetCounter(Task *task, s32 counter) {
    task->counter = counter;
}

/* Task method: goes to the next state, from substate 0 */
void taskNextState(Task *task) {
    task->substate = 0;
    task->step = 0;
    task->counter = 0;
    task->state++;
}

/* Task method: goes to the next substate, from step 0 */
void taskNextSubstate(Task *task) {
    task->step = 0;
    task->counter = 0;
    task->substate++;
}

/* Task method: goes to the next step, with its counter at 0 */
void taskNextStep(Task *task) {
    task->counter = 0;
    task->step++;
}

/* Task method: counts one more */
void taskTickCounter(Task *task) {
    task->counter++;
}

/* Default destructor: kills the children, then frees the task */
void destroyTask(Task *task) {
    s32 i;
    Task **children;

    if (task->childCount != 0) {
        children = task->children;
        for (i = 0; i < task->childCount; i++) {
            if (children[i] != NULL) {
                TASK_REGISTRY.funcs.kill(children[i]);
            }
        }
        HEAP.free(task->children);
    }
    TASK_REGISTRY.funcs.remove(task);
    HEAP.free(task);
}

/*
 * Allocates a zeroed task of `size` bytes (header included) with room for
 * childrenSize / 4 child tasks. A nonzero id also registers it (findTask).
 */
void *createTaskWithId(void (*update)(), s32 size, s32 childrenSize, s32 id) {
    Task *task = HEAP.allocZeroed(size, MEM_MODE);

    if (childrenSize != 0) {
        task->children = HEAP.allocZeroed(childrenSize, MEM_MODE);
        task->childCount = childrenSize / 4;
    }
    task->setState = taskSetState;
    task->setSubstate = taskSetSubstate;
    task->setStep = taskSetStep;
    task->setCounter = taskSetCounter;
    task->nextState = taskNextState;
    task->nextSubstate = taskNextSubstate;
    task->nextStep = taskNextStep;
    task->tickCounter = taskTickCounter;
    task->update = update;
    task->destroy = destroyTask;
    if (id != 0) {
        task->id = id;
        TASK_REGISTRY.funcs.add(task);
    }
    return task;
}

/* createTaskWithId for a task nothing needs to find */
void *createTask(void (*update)(), s32 size, s32 childrenSize) {
    return createTaskWithId(update, size, childrenSize, 0);
}
