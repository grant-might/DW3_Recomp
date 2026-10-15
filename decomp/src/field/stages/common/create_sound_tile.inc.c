/* Creates the updateSoundTile task with the id ARG */
void *createSoundTile(s32 arg) {
    return createTaskWithId(updateSoundTile, sizeof(StageSoundTile), 0, arg);
}
