/* FIGHTSTG's menu cursor. */

#include "battle/battle_mode.h"

/* where FIGHTSTG_drawCursorBar puts the cursor's bar in VRAM */
RECT FIGHTSTG_cursorBarRect = { 0, 0xF4, 12, 12 };

/* The vsync callback while the list is open: draws the picked line's bar,
   moving through its frames, and the bar off the line picked before when the
   pick has changed, over the frame being drawn. The match depends on the task
   coming as the callback's s32 argument, cast into a variable. */
void FIGHTSTG_drawCursorBar(s32 arg) {
    MenuCursor *task = (MenuCursor *)arg;
    u_long *saved;
    s32 i;
    s32 y;

    saved = (u_long *)BreakDraw();
    if (saved != (u_long *)-1) {
        if (++task->frame == 12) {
            task->frame = 0;
        }
        ClearOTag(FIGHTSTG_cursorBarOt, 2);
        for (i = 0; i < 4; i += 2) {
            if (i == 2 && task->prevSel == task->sel) {
                break;
            }
            if (i < 2) {
                FIGHTSTG_cursorBarRect.x = task->frame * 12;
                y = task->params.y + task->sel * task->params.step;
            } else {
                FIGHTSTG_cursorBarRect.x = 0x90;
                y = task->params.y + task->prevSel * task->params.step;
            }
            SetDrawMove(&FIGHTSTG_cursorBarMoves[i], &FIGHTSTG_cursorBarRect, task->params.x, y);
            addPrim(FIGHTSTG_cursorBarOt, &FIGHTSTG_cursorBarMoves[i]);
            SetDrawMove(&FIGHTSTG_cursorBarMoves[i + 1], &FIGHTSTG_cursorBarRect, task->params.x, y + 0x100);
            addPrim(FIGHTSTG_cursorBarOt, &FIGHTSTG_cursorBarMoves[i + 1]);
        }
        while (IsIdleGPU(0)) {
        }
        ContinueDraw(FIGHTSTG_cursorBarOt, saved);
    }
    task->prevSel = task->sel;
}

/* Draws each line's sprite of the cursor, the picked one blinking through the
   palettes and the one left fading out */
void FIGHTSTG_drawCursorSprites(MenuCursor *task) {
    SpriteDrawer drawer;
    void *sheet;
    s32 i;
    s32 y;
    s32 row;

    initSpriteDrawer(&drawer);
    i = 0;
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x200, i);
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    y = task->params.spriteY;
    for (; i < task->params.count; i++) {
        if (task->blink[i] != 0 || task->sel == i) {
            task->blink[i] += GFX.funcs.getFrameTime();
            if (task->blink[i] >= 0x14) {
                if (task->sel != i) {
                    task->blink[i] = 0;
                } else {
                    task->blink[i] -= 0x14;
                }
            }
        }
        row = task->blink[i] >> 2;
        if ((u32)row < 5) {
            drawer.setClutRow(row);
        } else {
            drawer.setClutRow(0);
        }
        drawer.draw(sheet, task->params.sprite, task->params.spriteX, y);
        y += task->params.spriteStep;
    }
}

/* The menu cursor's task: sets FIGHTSTG_drawCursorBar as the vsync callback,
   draws the lines' sprites and moves with up and down until locked; when
   killed, takes the callback off */
void FIGHTSTG_updateCursor(MenuCursor *task) {
    s32 pressed;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 2:
        default:
            GFX.vsyncFunc = FIGHTSTG_drawCursorBar;
            GFX.vsyncArg = (s32)task;
            /* fallthrough */
        case 0:
        case 1:
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            if (task->params.sprite != -1) {
                FIGHTSTG_drawCursorSprites(task);
            }
            if (task->locked == 0) {
                pressed = PAD.getRepeated(0) | PAD.getPressed(0);
                if (pressed & (1 << PAD_UP)) {
                    if (task->sel != 0) {
                        task->sel--;
                        SOUND.playSound(SOUND_MENU_MOVE);
                    }
                } else if (pressed & (1 << PAD_DOWN)) {
                    if (task->sel != task->params.count - 1) {
                        task->sel++;
                        SOUND.playSound(SOUND_MENU_MOVE);
                    }
                }
            }
            break;
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        GFX.vsyncFunc = NULL;
        break;
    }
}

/* Creates a menu cursor laid out by layout */
MenuCursor *FIGHTSTG_createCursor(CursorLayout *layout) {
    MenuCursor *task = createTask(FIGHTSTG_updateCursor, sizeof(MenuCursor), 0);

    task->params = *layout;
    return task;
}
