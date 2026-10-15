/* The welcome message the screen opens with */

#include "menus/player_name.h"

/* The welcome's task: brings its panel in through the CLUT rows, types the
   welcome text (cross turns the pages, an arrow after it blinks while it
   waits), then takes the panel out the same way and ends */
void STPLNMET_updateWelcome(NameDialog *task, NameDialogWindows *windows) {
    SpriteDrawer sprite;

    switch (task->state) {
    case TASK_INIT:
    default:
        windows->text = createTextWindow(0x1001, 1, 0x42, 0x3B);
        windows->text->setLines(windows->text, 3);
        windows->arrow = createTextWindow(0x1001, 1, 0x42, 0x3B);
        task->nextState(task);
        task->setSubstate(task, 1);
        windows->arrow->setString(windows->arrow, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 2);
        windows->arrow->setPalette(windows->arrow, PALETTE_BLUE);
        windows->arrow->setVisible(windows->arrow, 0);
        windows->text->setTypeSound(windows->text, 0x800454C4);
        task->step = GFX.funcs.getTime();
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 1:
        default:
            if (GFX.funcs.getTime() - task->step > 5) {
                task->step = GFX.funcs.getTime();
                task->clutRow++;
                if (task->clutRow >= 4) {
                    task->clutRow = 3;
                    task->setSubstate(task, 2);
                    task->step = GFX.funcs.getTime();
                    windows->text->setString(windows->text, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 1);
                    windows->text->setPalette(windows->text, PALETTE_BLUE);
                    windows->text->setTypeDelay(windows->text, 6);
                    windows->arrow->setVisible(windows->arrow, 1);
                }
            }
            break;
        case 2:
            if (windows->text->isFinished(windows->text) != 0) {
                task->setSubstate(task, 3);
                task->step = GFX.funcs.getTime();
                task->clutRow = 4;
                windows->arrow->setVisible(windows->arrow, 0);
                windows->text->setVisible(windows->text, 0);
                break;
            }
            windows->arrow->setPos(windows->arrow, windows->text->cursorX + 0x42, windows->text->cursorY + 0x3B);
            if (windows->text->isWaitingForButton(windows->text) != 0) {
                if (windows->arrow->isVisible(windows->arrow) != 0) {
                    if (GFX.funcs.getTime() - task->step > 16) {
                        task->step = GFX.funcs.getTime();
                        windows->arrow->setVisible(windows->arrow, 0);
                    }
                } else if (GFX.funcs.getTime() - task->step > 8) {
                    task->step = GFX.funcs.getTime();
                    windows->arrow->setVisible(windows->arrow, 1);
                }
            } else {
                windows->arrow->setVisible(windows->arrow, 1);
                task->step = GFX.funcs.getTime();
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                windows->text->showPage(windows->text);
            }
            break;
        case 3:
            if (GFX.funcs.getTime() - task->step > 5) {
                task->clutRow++;
                if (task->clutRow >= 8) {
                    task->clutRow = 7;
                    task->setState(task, TASK_KILL);
                }
            }
            break;
        }
        initSpriteDrawer(&sprite);
        sprite.setLayerId(0x1001, 5);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 0x38, 0x4B, 0xB8);
        sprite.setClutRow(task->clutRow);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 0xF, 0x38, 0x2F);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the welcome (task), the first thing the screen shows */
NameDialog *STPLNMET_createWelcome(PlayerNameScreen *screen) {
    NameDialog *task = createTask(STPLNMET_updateWelcome, sizeof(NameDialog), 8);

    task->screen = screen;
    return task;
}
