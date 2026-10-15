#include "engine/game.h"

/* Cursor method: shows it (from its first frame) or hides it */
void cursorSetVisible(Cursor *task, s32 visible) {
    task->visible = visible;
    if (visible == 0) {
        task->substate = 0;
        task->time = 0;
    }
    task->dirty = 1;
}

/* Cursor method: moves it */
void cursorSetPos(Cursor *task, s32 x, s32 y) {
    task->x = x;
    task->y = y;
    task->dirty = 1;
}

/* Cursor method: the CLUT row of its glyph */
void cursorSetPalette(Cursor *task, s32 palette) {
    task->palette = palette;
    task->dirty = 1;
}

/* Cursor method: the vsyncs it rests between two swings */
void cursorSetIdleDelay(Cursor *task, s32 delay) {
    task->idleDelay = delay;
}

/* Cursor method: the vsyncs of each frame of a swing */
void cursorSetFrameDelay(Cursor *task, s32 delay) {
    task->frameDelay = delay;
}

/* Cursor method: keeps it on its first frame */
void cursorSetStill(Cursor *task, s32 still) {
    task->still = still;
}

/* The cursor task: creates its text window, then swings it every idleDelay vsyncs */
void updateCursor(Cursor *task, TextWindow **win) {
    switch (task->state) {
    case 0:
    default:
        if (*win == NULL) {
            *win = createTextWindow(task->layerId, 1, task->x, task->y);
        }
        (*win)->setText(*win, CURSOR_FRAMES[task->frame]);
        (*win)->setVisible(*win, task->visible);
        (*win)->setDepth(*win, task->depth);
        task->time = GFX.funcs.getTime();
        task->nextState(task);
        break;
    case 1:
        if (task->dirty != 0) {
            (*win)->setVisible(*win, task->visible);
            (*win)->setPos(*win, task->x, task->y);
            (*win)->setPalette(*win, task->palette);
            task->dirty = 0;
        }
        if (task->visible != 0) {
            if (task->still != 0) {
                if (task->frame != 0) {
                    task->frame = 0;
                    (*win)->setText(*win, CURSOR_FRAMES[0]);
                }
            } else if (task->substate == 0) {
                if ((GFX.funcs.getTime() - task->time) / task->idleDelay != 0) {
                    task->time = GFX.funcs.getTime();
                    task->frame = 1;
                    (*win)->setText(*win, CURSOR_FRAMES[1]);
                    task->substate = 1;
                }
            } else if ((GFX.funcs.getTime() - task->time) / task->frameDelay != 0) {
                task->time = GFX.funcs.getTime();
                if (++task->frame >= 5) {
                    task->frame = 0;
                    task->substate = 0;
                }
                (*win)->setText(*win, CURSOR_FRAMES[task->frame]);
            }
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* A menu cursor on a layer at (x, y) */
Cursor *createCursor(s16 layerId, s32 depth, s16 x, s16 y) {
    Cursor *task = createTask(updateCursor, sizeof(Cursor), 4);

    task->layerId = layerId;
    task->depth = depth;
    task->x = x;
    task->y = y;
    task->visible = 1;
    task->idleDelay = 0x20;
    task->frameDelay = 6;
    task->setVisible = cursorSetVisible;
    task->setPos = cursorSetPos;
    task->setIdleDelay = cursorSetIdleDelay;
    task->setFrameDelay = cursorSetFrameDelay;
    task->setPalette = cursorSetPalette;
    task->setStill = cursorSetStill;
    return task;
}

/* The cursor's frames as text, each the code of one of its glyphs in the font. -G8
   unit: they are small, so they go to the small data (.sdata) */
static char CURSOR_TEXT_3[4] = "\x81\x88";
static char CURSOR_TEXT_2[4] = "\x81\x87";
static char CURSOR_TEXT_1[4] = "\x81\x86";
static char CURSOR_TEXT_0[4] = "\x81\x85";

/* The cursor's frames, the last one repeated as it swings back */
char *CURSOR_FRAMES[5] = { CURSOR_TEXT_0, CURSOR_TEXT_1, CURSOR_TEXT_2, CURSOR_TEXT_3, CURSOR_TEXT_2 };
