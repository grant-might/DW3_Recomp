/* The title screen's loader task: loads its sound bank and images, then runs it */

#include "menus/title.h"

/* Creates the title screen, then kills the title loader once the title screen has ended */
void STDWTITL_runTitleLoader(TitleLoaderTask *task, TitleTask **title) {
    switch (task->substate) {
    case 0:
    default:
        *title = STDWTITL_startTitleTask(task);
        task->substate++;
        break;
    case 1:
        if (*title == NULL) {
            task->setState(task, TASK_KILL);
        }
        break;
    }
}

/* The title loader's task: waits for the title's sound bank, loads the title's
   images, then runs the title screen (STDWTITL_runTitleLoader) */
void STDWTITL_tickTitleLoader(TitleLoaderTask *task, TitleTask **title) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (SOUND.isLoading() == 0) {
            task->nextState(task);
            STDWTITL_titleFuncs.loadImages();
        }
        break;
    case TASK_RUN:
        STDWTITL_runTitleLoader(task, title);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the title loader (task) and starts loading the title's sound bank */
TitleLoaderTask *STDWTITL_startTitleLoaderTask(void) {
    TitleLoaderTask *task = createTask(STDWTITL_tickTitleLoader, sizeof(TitleLoaderTask), sizeof(TitleTask *));

    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    SOUND.loadBank(STDWTITL_TITLE_SOUND_BANK);
    return task;
}
