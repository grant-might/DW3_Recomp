/* Creates the updateTileAnims task, 0x58 bytes */
void *createTileAnims(void) {
    return createTask(updateTileAnims, 0x58, 0);
}
