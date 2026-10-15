/* Creates a StageEffect at (x, y) with its sprites' frames from FRAME */
StageEffect *createStageEffect(s32 x, s32 y, s32 frame) {
    StageEffect *task = createTaskWithId(updateStageEffect, sizeof(StageEffect), 0, FIELD_TASK_LAUNCHER);

    task->x = x;
    task->y = y;
    task->frame = frame;
    return task;
}
