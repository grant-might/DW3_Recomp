/* Creates the updateTileDuo task with the id ARG */
void *createTileDuo(s32 arg) {
    return createTaskWithId(updateTileDuo, sizeof(StageTileDuo), 0, arg);
}
