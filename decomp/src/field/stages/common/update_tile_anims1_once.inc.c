/*
 * Animates the map objects of animation 1 with updateTileAnimsFrames,
 * stepped once an update
 */
void updateTileAnims(StageTileAnims *task) {
    StageTile *tile;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->anims[0].index = 0;
        task->anims[0].timer = updateTileAnimsFrames[0].duration;
        break;
    case TASK_RUN:
        tile = FIELDSTG_state.objects;
        frame = stepLoopingAnimation(&task->anims[0], updateTileAnimsFrames, 0);
        for (; tile->margin != 0; tile++) {
            if (tile->anim == 1) {
                tile->frame = frame;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
