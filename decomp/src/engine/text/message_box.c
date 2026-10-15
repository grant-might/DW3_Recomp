#include "engine/game.h"

/* Draws the two halves of a message box's panel, growing, still or fading */
void drawMessageBoxFrame(MessageBoxFrame *task) {
    SpriteDrawer obj;
    s32 i;
    s32 x;
    FileCache *cache;

    i = 0;
    cache = &FILE_CACHE;
    x = 0;
    for (; i < 2; i++) {
        initSpriteDrawer(&obj);
        obj.setLayerId(task->layerId, 0);
        obj.setTexture(0x140, 0);
        if (task->substate == 0) {
            obj.setScale(task->scale, ONE, ONE);
            obj.setPivot(0x140 - x, 0xC4);
        } else if (task->substate == 2) {
            obj.setClutRow(task->fadeRow);
        }
        obj.draw(cache->getEntry(FILE_MENU_SPRITES << 16), i + 8, 0, 0xA6);
        x += 0x140;
    }
}

/* Draws a message box's blinking "next" arrow */
void drawMessageBoxArrow(MessageBoxFrame *task) {
    SpriteDrawer obj;

    initSpriteDrawer(&obj);
    obj.setLayerId(task->layerId, 0);
    obj.setTexture(0x140, 0);
    if (GFX.funcs.getTime() - task->arrowTime >= 4) {
        task->arrowTime = GFX.funcs.getTime();
        if (++task->arrowFrame >= 5) {
            task->arrowFrame = 0;
        }
    }
    obj.setClutRow(task->arrowFrame);
    obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 10, 0x124, 0xCD);
}

/* The message box frame task: grows, waits with the arrow when asked, then fades out and ends */
void updateMessageBoxFrame(MessageBoxFrame *task) {
    switch (task->state) {
    case 0:
    default:
        task->nextState(task);
        task->scaleStep = 0x199;
        break;
    case 1:
        switch (task->substate) {
        case 0:
        default:
            task->scale += task->scaleStep;
            if (task->scale > ONE) {
                task->scale = ONE;
                task->nextSubstate(task);
            }
            break;
        case 1:
            if (task->showArrow != 0) {
                drawMessageBoxArrow(task);
            }
            break;
        case 2:
            if (GFX.funcs.getTime() - task->time >= 3) {
                task->time = GFX.funcs.getTime();
                if (++task->fadeRow >= 5) {
                    task->setState(task, TASK_KILL);
                    return;
                }
            }
            break;
        }
        drawMessageBoxFrame(task);
        break;
    case 2:
    case 3:
        break;
    }
}

/* A message box frame on a layer, with the menu sound */
MessageBoxFrame *createMessageBoxFrame(s32 layerId) {
    MessageBoxFrame *task = createTask(updateMessageBoxFrame, sizeof(MessageBoxFrame), 0);

    task->layerId = layerId;
    SOUND.playSound(SOUND_MENU_OPEN);
    return task;
}

/* The message box task: shows the text once the frame is open, closes when the text ends */
void updateMessageBox(Task *task, MessageBox *data) {
    switch (task->state) {
    case 0:
    default:
        task->nextState(task);
        break;
    case 1:
        switch (task->substate) {
        case 0:
        default:
            if (data->window->isVisible(data->window) == 0 && data->frame->substate == 1) {
                data->window->setVisible(data->window, 1);
                task->substate++;
            }
            break;
        case 1:
            if (data->window->isFinished(data->window) != 0) {
                data->frame->substate = 2;
                data->window->setVisible(data->window, 0);
                task->substate++;
                SOUND.playSound(SOUND_MENU_CLOSE);
                break;
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                data->window->showPage(data->window);
            }
            if (data->window->isWaitingForButton(data->window) != 0) {
                data->frame->showArrow = 1;
            } else {
                data->frame->showArrow = 0;
            }
            break;
        case 2:
            if (data->frame == NULL) {
                task->setState(task, TASK_KILL);
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* A message box with string `index` of a table, typed out in three-line pages */
Task *createMessageBox(s32 layerId, void *strings, s32 index) {
    Task *task = createTask(updateMessageBox, sizeof(Task), sizeof(MessageBox));
    MessageBox *data = task->children;

    data->window = createTextWindow(layerId, 1, 0x12, 0xB0);
    data->window->setLines(data->window, 3);
    data->window->setString(data->window, strings, index);
    data->window->setVisible(data->window, 0);
    data->window->setTypeDelay(data->window, 6);
    data->frame = createMessageBoxFrame(layerId);
    return task;
}
