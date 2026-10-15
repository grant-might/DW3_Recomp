/* Creates the updateTileCursors task */
void *createTileCursors(void) {
    return createTask(updateTileCursors, sizeof(StageTileCursors), 0);
}
