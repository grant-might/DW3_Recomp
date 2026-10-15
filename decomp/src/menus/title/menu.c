/* "PRESS START", then the two options (new game, continue) */

#include "menus/title.h"

/* The menu's sprites: the European version has them for each language */
#if VERSION_US || VERSION_JP
#define MENU_SPRITE(i) (8 + (i))
#elif VERSION_EU
#define MENU_SPRITE(i) (STDWTITL_menuSprites[LANGUAGE][i])
#endif

/* Draws the menu: the cursor while it is shown, then "PRESS START" or the two
   options, the one not picked dimmed */
void STDWTITL_drawMenu(TitleMenuTask *task) {
    SpriteDrawer sprite;

    if (task->showCursor) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->blink);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), 7,
                    STDWTITL_menuCursorPositions[task->selection].x,
                    STDWTITL_menuCursorPositions[task->selection].y - 1);
    }
    if (task->showOptions == 0) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->selection != 2);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), MENU_SPRITE(0), 79, 166);
    } else {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->selection != 0);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), MENU_SPRITE(1), task->options[0].x, task->options[0].y);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(task->layerId, 0);
        sprite.setTexture(0x280, STDWTITL_TEXTURE_Y);
        sprite.setClutRow(task->selection != 1);
        sprite.draw(FILE_CACHE.getEntry(STDWTITL_spriteBank), MENU_SPRITE(2), task->options[1].x, task->options[1].y);
    }
}

/* "PRESS START": waits a moment, then for Start (choice 3 after ten seconds),
   blinks and spreads the two options out from it */
static inline void runPressStart(TitleMenuTask *task) {
    switch (task->substate) {
    case 1:
    default:
        if (task->timer++ >= 6) {
            task->selection = 2;
            task->timer = 0;
            task->showCursor = 1;
            task->nextSubstate(task);
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_START)) {
            task->nextSubstate(task);
            task->timer = 0;
            SOUND.playSound(SOUND_SELECT);
            task->blink = 0;
        }
        if (++task->timer >= 600) {
            task->choice = 3;
        }
        break;
    case 3:
        if ((++task->timer & 3) == 0) {
            task->timer = 0;
            if (++task->blink >= 3) {
                task->nextSubstate(task);
                task->showOptions = 1;
                task->options[0].x = task->options[1].x = 79;
                task->options[0].y = task->options[1].y = 166;
                task->blink = 0;
                task->showCursor = 0;
                SOUND.playSound(SOUND_SWITCH03);
            }
        }
        break;
    case 4:
        task->options[0].x = (STDWTITL_menuCursorPositions[0].x - 79) * task->timer / 15 + 79;
        task->options[0].y = (STDWTITL_menuCursorPositions[0].y - 166) * task->timer / 15 + 166;
        task->options[1].x = (STDWTITL_menuCursorPositions[1].x - 79) * task->timer / 15 + 79;
        task->options[1].y = (STDWTITL_menuCursorPositions[1].y - 166) * task->timer / 15 + 166;
        if (++task->timer >= 15) {
            task->timer = 0;
            task->nextSubstate(task);
        }
        break;
    case 5:
        if (++task->timer >= 6) {
            task->setState(task, TASK_DONE);
        }
        break;
    }
}

/* The two options: up and down move between new game and continue, and the
   confirm button blinks the option and sets choice */
static inline void runOptions(TitleMenuTask *task) {
    switch (task->substate) {
    case 0:
    default:
        task->showOptions = 1;
        task->selection = 1;
        task->showCursor = 1;
        task->options[0].x = STDWTITL_menuCursorPositions[0].x;
        task->options[0].y = STDWTITL_menuCursorPositions[0].y;
        task->options[1].x = STDWTITL_menuCursorPositions[1].x;
        task->options[1].y = STDWTITL_menuCursorPositions[1].y;
        task->nextSubstate(task);
        break;
    case 1:
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            if (task->selection != 0) {
                SOUND.playSound(SOUND_CURSOR);
            }
            task->selection = 0;
        }
        if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            if (task->selection != 1) {
                SOUND.playSound(SOUND_CURSOR);
            }
            task->selection = 1;
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            task->timer = 0;
            SOUND.playSound(SOUND_SELECT);
            task->nextSubstate(task);
        }
        break;
    case 2:
        if ((++task->timer & 3) == 0) {
            task->timer = 0;
            if (++task->blink >= 3) {
                if (task->selection != 0) {
                    task->choice = 2;
                } else {
                    task->choice = 1;
                }
                task->nextSubstate(task);
                task->blink = 0;
            }
        }
        break;
    case 3:
        break;
    }
}

/* The menu's task: once shown, waits for Start on "PRESS START" (choice 3 after ten
   seconds), blinks and spreads out the two options; then moves between new game
   and continue, and the confirm button blinks the option and sets choice */
void STDWTITL_tickMenu(TitleMenuTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->skip == 0) {
            task->nextState(task);
        } else {
            task->setState(task, TASK_DONE);
        }
        task->choice = 0;
        return;
    case TASK_RUN:
        if (task->substate == 0) {
            return;
        }
        runPressStart(task);
        break;
    case TASK_DONE:
        runOptions(task);
        break;
    case TASK_KILL:
        return;
    }
    STDWTITL_drawMenu(task);
}

/* Shows "PRESS START" (the task's show), only while it waits */
void STDWTITL_showMenu(TitleMenuTask *task) {
    if (task->state == TASK_RUN) {
        task->setSubstate(task, 1);
    }
}

/* Back to waiting for Start on "PRESS START" (the task's reset), only while running;
   nothing in the overlay calls it */
void STDWTITL_resetMenu(TitleMenuTask *task) {
    if (task->state == TASK_RUN) {
        task->selection = 2;
        task->showCursor = 1;
        task->timer = 0;
        task->setSubstate(task, 2);
    }
}

/* The menu's choice (the task's getChoice): 0 none yet, 1 new game, 2 continue,
   3 timed out */
s32 STDWTITL_getMenuChoice(TitleMenuTask *task) {
    return task->choice;
}

/* Creates the title screen's menu (task); skip: the two options shown from the start */
TitleMenuTask *STDWTITL_startMenuTask(s16 skip) {
    TitleMenuTask *task = createTask(STDWTITL_tickMenu, sizeof(TitleMenuTask), 0);

    task->show = STDWTITL_showMenu;
    task->reset = STDWTITL_resetMenu;
    task->getChoice = STDWTITL_getMenuChoice;
    task->layerId = STDWTITL_TITLE_LAYER;
    task->depth = 2;
    task->skip = skip;
    return task;
}
