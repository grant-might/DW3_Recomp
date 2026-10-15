/* The title screen's glint */

#include "menus/title.h"

/* Draws the glint (sprite 4) for the European version's language 0, and sprite 2 once lit */
void STDWTITL_drawGlintAlt(GlintTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    if (task->lit) {
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setAltClut(0, 0x1F0);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 2, 49, 121);
        initSpriteDrawer(&sprite);
    }
    sprite.setLayerId(task->layerId, 0);
    sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
    sprite.setClutRow(task->frame);
    sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 4, 37, 114);
}

/* The glint's task for the European version's language 0: waits until shown, plays the
   glint's 12 frames once, then keeps sprite 2 lit; with skip it starts lit */
void STDWTITL_tickGlintAlt(GlintTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->lit = 0;
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
            task->index = 0;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->frame = STDWTITL_glintAltFrames[task->index];
            task->index++;
            if (task->index >= 12) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawGlintAlt(task);
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->frame = 11;
            task->lit = 1;
            task->nextSubstate(task);
        }
        STDWTITL_drawGlintAlt(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Starts the glint (the task's show), only while it waits */
void STDWTITL_showGlint(GlintTask *task) {
    if (task->state == TASK_RUN) {
        task->setSubstate(task, 1);
    }
}

/* Creates the title screen's glint (task) for the European version's language 0 */
GlintTask *STDWTITL_startGlintAltTask(s32 skip) {
    GlintTask *task = createTask(STDWTITL_tickGlintAlt, sizeof(GlintTask), 0);

    task->show = STDWTITL_showGlint;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}

/* Draws the glint (sprite 4) while it plays, and sprite 2 once lit */
void STDWTITL_drawGlint(GlintTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    if (task->lit) {
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setAltClut(0, 0x1F0);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 2, 253, 100);
        initSpriteDrawer(&sprite);
    }
    if (task->frame != -1) {
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->frame);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 4, 243, 90);
    }
}

/* The glint's task: waits until shown, plays the glint once (up to its last frame
   and back), then keeps sprite 2 lit; with skip it starts lit */
void STDWTITL_tickGlint(GlintTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->lit = 0;
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
            task->index = 0;
            task->frame = 0;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            task->frame = STDWTITL_glintFrames[task->index];
            if (task->frame == -1) {
                task->setState(task, TASK_DONE);
            }
            STDWTITL_drawGlint(task);
            task->index++;
            break;
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->frame = -1;
            task->lit = 1;
            task->nextSubstate(task);
        }
        STDWTITL_drawGlint(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the title screen's glint (task); skip: lit from the start */
GlintTask *STDWTITL_startGlintTask(s32 skip) {
    GlintTask *task = createTask(STDWTITL_tickGlint, sizeof(GlintTask), 0);

    task->show = STDWTITL_showGlint;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}
