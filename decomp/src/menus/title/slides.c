/* The two parts of the title, which slide in from the sides of the screen */

#include "menus/title.h"

/* Draws the second part of the title (sprite 1) for the European version's language 0 */
void STDWTITL_drawTitle1Alt(SlideTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setAltClut(0, 0x1F0);
    sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 1, task->x, task->y);
}

/* The second part of the title for the European version's language 0: waits until
   shown, then slides in from the right in 10 steps; with skip it starts in place */
void STDWTITL_tickTitle1Alt(SlideTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->x = 527;
            task->y = 34;
            task->nextState(task);
        } else {
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->steps = 10;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->x = task->steps * 32 + 207;
            if (task->steps-- <= 0) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawTitle1Alt(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->x = 207;
            task->y = 34;
            task->nextSubstate(task);
        }
        STDWTITL_drawTitle1Alt(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Starts sliding in the second part of the title (the task's show), only while it waits */
void STDWTITL_showTitle1(SlideTask *task) {
    if (task->state == TASK_RUN) {
        task->setSubstate(task, 1);
    }
}

/* Creates the second part of the title (task) for the European version's language 0 */
SlideTask *STDWTITL_startTitle1AltTask(s32 skip) {
    SlideTask *task = createTask(STDWTITL_tickTitle1Alt, sizeof(SlideTask), 0);

    task->show = STDWTITL_showTitle1;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}

/* Draws the second part of the title (sprite 1) */
void STDWTITL_drawTitle1(SlideTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setAltClut(0, 0x1F0);
    sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 1, task->x, task->y);
}

/* The second part of the title: waits until shown, then slides in from the right
   in 10 steps (skip is not used) */
void STDWTITL_tickTitle1(SlideTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->x = 339;
        task->y = 111;
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->steps = 10;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->x = task->steps * 32 + 19;
            if (task->steps-- <= 0) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawTitle1(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->x = 19;
            task->y = 111;
            task->nextSubstate(task);
        }
        STDWTITL_drawTitle1(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the second part of the title (task) */
SlideTask *STDWTITL_startTitle1Task(s32 skip) {
    SlideTask *task = createTask(STDWTITL_tickTitle1, sizeof(SlideTask), 0);

    task->show = STDWTITL_showTitle1;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}

/* Draws the first part of the title (sprite 0) for the European version's language 0 */
void STDWTITL_drawTitle0Alt(SlideTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setAltClut(0, 0x1F0);
    sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 0, task->x, task->y);
}

/* The first part of the title for the European version's language 0: waits until
   shown, then slides in from the left in 10 steps; with skip it starts in place */
void STDWTITL_tickTitle0Alt(SlideTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->x = -303;
            task->y = 61;
            task->nextState(task);
        } else {
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->steps = 10;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->x = -(task->steps * 320) / 10 + 17;
            if (task->steps-- <= 0) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawTitle0Alt(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->x = 17;
            task->y = 61;
            task->nextSubstate(task);
        }
        STDWTITL_drawTitle0Alt(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Starts sliding in the first part of the title (the task's show), only while it waits */
void STDWTITL_showTitle0(SlideTask *task) {
    if (task->state == TASK_RUN) {
        task->setSubstate(task, 1);
    }
}

/* Creates the first part of the title (task) for the European version's language 0 */
SlideTask *STDWTITL_startTitle0AltTask(s32 skip) {
    SlideTask *task = createTask(STDWTITL_tickTitle0Alt, sizeof(SlideTask), 0);

    task->show = STDWTITL_showTitle0;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}

/* Draws the first part of the title (sprite 0) */
void STDWTITL_drawTitle0(SlideTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layerId, 0);
    sprite.setAltClut(0, 0x1F0);
    sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 0, task->x, task->y);
}

/* The first part of the title: waits until shown, then slides in from the left in
   10 steps (skip is not used) */
void STDWTITL_tickTitle0(SlideTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->x = -301;
        task->y = 24;
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            task->steps = 10;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->x = -(task->steps * 320) / 10 + 19;
            if (task->steps-- <= 0) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawTitle0(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->x = 19;
            task->y = 24;
            task->nextSubstate(task);
        }
        STDWTITL_drawTitle0(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the first part of the title (task) */
SlideTask *STDWTITL_startTitle0Task(s32 skip) {
    SlideTask *task = createTask(STDWTITL_tickTitle0, sizeof(SlideTask), 0);

    task->show = STDWTITL_showTitle0;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}
