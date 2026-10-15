/* Creates the updateTileAnims task */
void *createTileAnims(void) {
    return createTask(updateTileAnims, sizeof(StageTileAnims), 0);
}
