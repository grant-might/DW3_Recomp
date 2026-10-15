/*
 * Animates the map objects of animations 1 to 4 with updateTileAnimsFrames0
 * to 3, each stepped again for each map object that shows it (so it runs
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
        task->anims[2].index = 0;
        task->anims[2].timer = updateTileAnimsFrames2[0].duration;
        task->anims[3].index = 0;
        task->anims[3].timer = updateTileAnimsFrames3[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
            switch (tile->anim) {
            case 1:
                tile->frame = stepLoopingAnimation(&task->anims[0], updateTileAnimsFrames0, 0);
                break;
            case 2:
                tile->frame = stepLoopingAnimation(&task->anims[1], updateTileAnimsFrames1, 0);
                break;
            case 3:
                tile->frame = stepLoopingAnimation(&task->anims[2], updateTileAnimsFrames2, 0);
                break;
            case 4:
                tile->frame = stepLoopingAnimation(&task->anims[3], updateTileAnimsFrames3, 0);
                break;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
