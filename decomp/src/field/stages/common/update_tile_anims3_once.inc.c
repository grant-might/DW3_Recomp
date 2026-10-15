/*
 * Animates the map objects of animations 1 to 3 with updateTileAnimsFrames0
 * to 2, each stepped once an update
 */
void updateTileAnims(StageTileAnims *task) {
    s32 frames[3];
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->anims[0].index = 0;
        task->anims[0].timer = updateTileAnimsFrames0[0].duration;
        task->anims[1].index = 0;
        task->anims[1].timer = updateTileAnimsFrames1[0].duration;
        task->anims[2].index = 0;
        task->anims[2].timer = updateTileAnimsFrames2[0].duration;
        break;
    case TASK_RUN:
        tile = FIELDSTG_state.objects;
        frames[0] = stepLoopingAnimation(&task->anims[0], updateTileAnimsFrames0, 0);
        frames[1] = stepLoopingAnimation(&task->anims[1], updateTileAnimsFrames1, 0);
        frames[2] = stepLoopingAnimation(&task->anims[2], updateTileAnimsFrames2, 0);
        for (; tile->margin != 0; tile++) {
            switch (tile->anim) {
            case 1:
                tile->frame = frames[0];
                break;
            case 2:
                tile->frame = frames[1];
                break;
            case 3:
                tile->frame = frames[2];
                break;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
