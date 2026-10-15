/* Creates the updateTileSix task */
void *createTileSix(void) {
    return createTask(updateTileSix, sizeof(StageTileSix), 0);
}
