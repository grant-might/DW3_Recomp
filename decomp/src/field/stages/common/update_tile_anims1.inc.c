/*
 * Animates the map objects of animation 1 with updateTileAnimsFrames,
 * stepped again for each map object that shows it (so it runs faster with
 * more of them)
 */
void updateTileAnims(StageTileAnims *task) {
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->anims[0].index = 0;
        task->anims[0].timer = updateTileAnimsFrames[0].duration;
        break;
    case TASK_RUN:
        for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
            if (tile->anim == 1) {
                tile->frame = stepLoopingAnimation(&task->anims[0], updateTileAnimsFrames, 0);
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
