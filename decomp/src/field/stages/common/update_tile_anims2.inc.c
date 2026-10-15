/*
 * Animates the map objects of animations 1 and 2 with updateTileAnimsFrames0
 * and 1, each stepped again for each map object that shows it (so it runs
 * faster with more of them)
 */
void updateTileAnims(StageTileAnims *task) {
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->anims[0].index = 0;
        task->anims[0].timer = updateTileAnimsFrames0[0].duration;
        task->anims[1].index = 0;
        task->anims[1].timer = updateTileAnimsFrames1[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
            if (tile->anim == 1) {
                tile->frame = stepLoopingAnimation(&task->anims[0], updateTileAnimsFrames0, 0);
            }
            if (tile->anim == 2) {
                tile->frame = stepLoopingAnimation(&task->anims[1], updateTileAnimsFrames1, 0);
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
