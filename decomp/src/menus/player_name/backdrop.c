/* The screen's backdrop: the scrolling sprites, the sparkles and the shine */

#include "menus/player_name.h"

/* The scrolling sprites' task: once their image is loaded, moves the two
   sprites diagonally at different speeds, each starting over at its end
   (STPLNMET_scrollEnds), and draws them with a still one */
void STPLNMET_updateScroll(NameScroll *task) {
    SpriteDrawer sprite;
    s32 now;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->loaded != 0) {
            task->times[0] = task->times[1] = GFX.funcs.getTime();
            task->nextState(task);
        }
        break;
    case TASK_RUN:
        initSpriteDrawer(&sprite);
        sprite.setTexture(task->vramX, task->vramY);
        sprite.setLayerId(task->layer, task->depth);
        now = GFX.funcs.getTime();
        for (i = 0; i < 2; i++) {
            if (now - task->times[i] > i + 1) {
                task->pos[i][0] -= 2;
                task->pos[i][1]++;
                if (task->pos[i][0] <= STPLNMET_scrollEnds[i]) {
                    task->pos[i][0] = task->pos[i][1] = 0;
                }
                task->times[i] = now;
            }
            sprite.draw(FILE_CACHE.getEntry(PLNMET_SCROLL), i, task->pos[i][0], task->pos[i][1]);
        }
        sprite.draw(FILE_CACHE.getEntry(PLNMET_SCROLL), 2, 0, 0);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Loads the scrolling sprites' image into VRAM at (x, y) (scroll->load) */
void STPLNMET_loadScroll(NameScroll *task, s32 x, s32 y) {
    TimLoader loader;

    task->vramX = x;
    task->vramY = y;
    initTimLoader(&loader);
    loader.setImagePos(x, y);
    loader.loadArchive(FILE_CACHE.getEntry(FILE_PLNMET_IMAGE << 16));
    task->loaded = 1;
}

/* Sets the layer and depth the scrolling sprites are drawn in (scroll->setLayer) */
void STPLNMET_setScrollLayer(NameScroll *task, s32 layer, s32 depth) {
    task->layer = layer;
    task->depth = depth;
}

/* Creates the scrolling sprites (task), drawn once their image is loaded */
NameScroll *STPLNMET_createScroll(void) {
    NameScroll *task = createTask(STPLNMET_updateScroll, sizeof(NameScroll), 0);

    task->load = STPLNMET_loadScroll;
    task->setLayer = STPLNMET_setScrollLayer;
    return task;
}

/* The sparkles' task: draws five sparkles in place, all on the same looping frame */
void STPLNMET_updateSparkles(NameSparkle *task) {
    SpriteDrawer sprite;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->frame = 24;
        task->nextState(task);
        break;
    case TASK_RUN:
        initSpriteDrawer(&sprite);
        sprite.setTexture(0x280, 0x100);
        sprite.setLayerId(0x1001, 6);
        if (GFX.funcs.getTime() - task->time > 4) {
            task->time = GFX.funcs.getTime();
            task->frame++;
            if (task->frame >= 36) {
                task->frame = 24;
            }
        }
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), task->frame, 0x33, 0);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), task->frame, 0x78, -0x14);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), task->frame, 0xA7, -0x58);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), task->frame, 0xF8, 0xD);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), task->frame, 0x120, 0);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the sparkles (task) */
NameSparkle *STPLNMET_createSparkles(void) {
    return createTask(STPLNMET_updateSparkles, sizeof(NameSparkle), 0);
}

/* The shine's task: draws a sprite animated by STPLNMET_sparkleFrames, looping */
void STPLNMET_updateShine(NameSparkle *task) {
    SpriteDrawer sprite;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        initSpriteDrawer(&sprite);
        sprite.setTexture(0x280, 0x100);
        sprite.setLayerId(0x1001, 6);
        if (GFX.funcs.getTime() - task->time > STPLNMET_sparkleFrames[task->frame].delay) {
            task->time = GFX.funcs.getTime();
            task->frame++;
            if (STPLNMET_sparkleFrames[task->frame].sprite == 0) {
                task->frame = 0;
            }
        }
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), STPLNMET_sparkleFrames[task->frame].sprite, 0, 0);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the shine (task) */
NameSparkle *STPLNMET_createShine(void) {
    return createTask(STPLNMET_updateShine, sizeof(NameSparkle), 0);
}
