/* Creates the updateTimer task */
void *createTimer(void) {
    return createTask(updateTimer, sizeof(StageTask), 0x4);
}
