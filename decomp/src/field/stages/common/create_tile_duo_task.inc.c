/* Creates the updateTileDuo task */
void *createTileDuoTask(void) {
    return createTask(updateTileDuo, sizeof(StageTileDuo), 0);
}
