/* The screen fade's task: once started, moves the level one frame on and
   draws it; TASK_DONE at the end, still drawn */
void OVL_NAME(updateFader)(ScreenFade *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        if (task->substate == 0) {
            break;
        }
        task->level += task->levelStep;
        if (task->fadeIn == 0) {
            if (task->level > FADE_LEVEL_MAX) {
                task->level = FADE_LEVEL_MAX;
                task->state = TASK_DONE;
            }
        } else if (task->level < 0) {
            task->level = 0;
            task->state = TASK_DONE;
        }
        /* fallthrough */
    case TASK_DONE:
        OVL_NAME(drawFader)(task);
        break;
    case TASK_KILL:
        break;
    }
}
