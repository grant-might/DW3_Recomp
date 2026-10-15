/*
 * The update of the stage's task (startStage) in the stages that animate
 * their map objects (their tile animation task is its child) and copy the
 * points of the place the player comes from to their triggers
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createTileAnims();
        copyPlacePoints(FIELDSTG_state.slots, placePoints, GAME.place, GAME.placeArg);
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
