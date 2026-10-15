/* Creates the updateFarSprites task */
void *createFarSprites(void) {
    return createTask(updateFarSprites, 0x50, 0);
}
