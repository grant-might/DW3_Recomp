/* Creates the updateTileAnims task, 0x54 bytes */
void *createTileAnims(void) {
    return createTask(updateTileAnims, 0x54, 0);
}
