/* Starts fading the screen to black (fadeIn 0) or back, over `duration` frames */
void OVL_NAME(startFader)(ScreenFade *task, s32 fadeIn, s32 duration) {
    task->setState(task, TASK_RUN);
    task->substate = 1;
    task->fadeIn = fadeIn;
    if (fadeIn == 0) {
        task->level = 0;
        task->levelStep = FADE_LEVEL_MAX / duration;
    } else {
        task->level = FADE_LEVEL_MAX;
        task->levelStep = -(FADE_LEVEL_MAX / duration);
    }
}
