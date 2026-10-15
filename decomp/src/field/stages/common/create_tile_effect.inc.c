/* Creates the updateTileEffect task with the id ARG */
void *createTileEffect(s32 arg) {
    return createTaskWithId(updateTileEffect, sizeof(StageTileEffect), 0, arg);
}
