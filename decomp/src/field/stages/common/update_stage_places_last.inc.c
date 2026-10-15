/*
 * As update_stage_places.inc.c, with the points copied after the task moves
 * on (seven stages)
 */
void updateStage(StageTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        copyPlacePoints(FIELDSTG_state.slots, placePoints, GAME.place, GAME.placeArg);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
