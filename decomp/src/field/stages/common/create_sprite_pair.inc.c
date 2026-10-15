/* Creates a StageSpritePair at (x, y) */
void *createSpritePair(s32 x, s32 y) {
    StageSpritePair *task = createTask(updateSpritePair, sizeof(StageSpritePair), 0);

    task->x = x;
    task->y = y;
    return task;
}
