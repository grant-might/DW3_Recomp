/* Plays the sequences of the map objects with animations 1 to 5 */
void updateTileCursors(StageTileCursors *task) {
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->cursors[0].seq = 0;
        task->cursors[0].index = 0;
        task->cursors[0].timer = updateTileCursorsFrames[0][0][0].duration;
        task->cursors[1].seq = 0;
        task->cursors[1].index = 0;
        task->cursors[1].timer = updateTileCursorsFrames[1][0][0].duration;
        task->cursors[2].seq = 0;
        task->cursors[2].index = 0;
        task->cursors[2].timer = updateTileCursorsFrames[2][0][0].duration;
        task->cursors[3].seq = 0;
        task->cursors[3].index = 0;
        task->cursors[3].timer = updateTileCursorsFrames[3][0][0].duration;
        task->cursors[4].seq = 0;
        task->cursors[4].index = 0;
        task->cursors[4].timer = updateTileCursorsFrames[3][0][0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
            switch (tile->anim) {
            case 1:
                stepTileSequences(tile, updateTileCursorsFrames[0], &task->cursors[0], 0);
                break;
            case 2:
                stepTileSequences(tile, updateTileCursorsFrames[1], &task->cursors[1], 0);
                break;
            case 3:
                stepTileSequences(tile, updateTileCursorsFrames[2], &task->cursors[2], 0);
                break;
            case 4:
                stepTileSequences(tile, updateTileCursorsFrames[3], &task->cursors[3], 0);
                break;
            case 5:
                stepTileSequences(tile, updateTileCursorsFrames[4], &task->cursors[4], 0);
                break;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
