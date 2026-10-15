/* Creates the updateTileAnims task, 0x5C bytes */
void *createTileAnims(void) {
    return createTask(updateTileAnims, 0x5C, 0);
}
