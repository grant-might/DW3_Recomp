/*
 * The update of the stage's task (startStage) where it has nothing to do:
 * the stages that have something (the events they create as its children)
 * have their own.
 */
void updateStage(StageTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
