/* The partners page: the choice between the sets of starting partners */

#include "menus/player_name.h"

/* Creates the choice's windows: the title, the three labels, the partners'
   name and text, and each partner's name and text */
void STPLNMET_createChoiceWindows(PartnerChoice *task, PartnerChoiceWindows *windows) {
    s32 i;

    windows->title = createTextWindow(task->layer, 1, 0x20, 0x1A);
    windows->title->setPalette(windows->title, PALETTE_GREEN);
    for (i = 0; i < 3; i++) {
        windows->labels[i] = createTextWindow(task->layer, 1, 0x1A, 0x34 + i * 0x18);
        windows->labels[i]->setPalette(windows->labels[i], PALETTE_BLUE);
    }
    windows->name = createTextWindow(task->layer, 1, 0x3A, 0x32);
    windows->name->setPalette(windows->name, PALETTE_GREEN);
    windows->text = createTextWindow(task->layer, 1, 0x3A, 0xC2);
    windows->text->setLines(windows->text, 3);
    windows->text->setPalette(windows->text, PALETTE_BLUE);
    for (i = 0; i < 3; i++) {
        windows->partners[i].name = createTextWindow(task->layer, 2, 0x72, 0x43 + i * 0x2A);
        windows->partners[i].name->setPalette(windows->partners[i].name, PALETTE_RED);
        windows->partners[i].text = createTextWindow(task->layer, 1, 0x72, 0x4F + i * 0x2A);
        windows->partners[i].text->setLines(windows->partners[i].text, 2);
        windows->partners[i].text->setPalette(windows->partners[i].text, PALETTE_BLUE);
    }
}

/* Shows the choice's windows with the texts of the partners under the arrow, or hides them */
void STPLNMET_showChoiceWindows(PartnerChoice *task, PartnerChoiceWindows *windows, s32 show) {
    s32 i;
    s32 *partners;

    if (show != 0) {
        partners = STPLNMET_funcs.choices[task->choice];
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 6);
        for (i = 0; i < 3; i++) {
            windows->labels[i]->setString(windows->labels[i], FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), i + 7);
        }
        windows->name->setString(windows->name, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), task->choice + 10);
        windows->text->setString(windows->text, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), task->choice + 26);
        for (i = 0; i < 3; i++) {
            windows->partners[i].name->setString(windows->partners[i].name, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), partners[i] + 1);
            windows->partners[i].text->setString(windows->partners[i].text, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), partners[i] + 13);
        }
        return;
    }
    windows->title->setVisible(windows->title, 0);
    for (i = 0; i < 3; i++) {
        windows->labels[i]->setVisible(windows->labels[i], 0);
    }
    windows->name->setVisible(windows->name, 0);
    windows->text->setVisible(windows->text, 0);
    for (i = 0; i < 3; i++) {
        windows->partners[i].name->setVisible(windows->partners[i].name, 0);
        windows->partners[i].text->setVisible(windows->partners[i].text, 0);
    }
}

/* Draws the choice: the three partners under the arrow (animated), the panel and
   the blinking arrow on the choice */
void STPLNMET_drawChoice(PartnerChoice *task) {
    SpriteDrawer sprite;
    s32 *partners = STPLNMET_funcs.choices[task->choice];
    PartnerAnim *anim;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layer, task->depth);
    if (GFX.funcs.getTime() - task->frameTime > 12) {
        task->frameTime = GFX.funcs.getTime();
        for (i = 0; i < 3; i++) {
            anim = &STPLNMET_funcs.anims[partners[i]];
            task->frames[i]++;
            if (task->frames[i] >= 8 || anim->frames[task->frames[i]] == -1) {
                task->frames[i] = 0;
            }
        }
    }
    sprite.setTexture(0x140, 0x100);
    for (i = 0; i < 3; i++) {
        anim = &STPLNMET_funcs.anims[partners[i]];
        sprite.draw(FILE_CACHE.getEntry(PLNMET_BANK), anim->frames[task->frames[i]], 0x41, 0x40 + i * 0x2A);
    }
    sprite.setTexture(0x280, 0x100);
    sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 7, 0x10, 0x2C);
    sprite.setLayerId(task->layer, task->depth - 1);
    sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 8, 0x10, 0x2C);
    if (GFX.funcs.getTime() - task->arrowTime > 8) {
        task->arrowTime = GFX.funcs.getTime();
        task->arrowFrame++;
        if (task->arrowFrame >= 6) {
            task->arrowFrame = 0;
        }
    }
    sprite.setClutRow(STPLNMET_choiceArrowCluts[task->arrowFrame]);
    sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 0x32, 0x10, 0x30 + task->choice * 0x18);
}

/* Picks one of the three choices of partners (up/down); cross confirms them
   (substate 100), triangle goes back to the name (substate 200) */
void STPLNMET_runChoice(PartnerChoice *task, PartnerChoiceWindows *windows) {
    s32 old = task->choice;
    s32 i;

    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        task->choice--;
        if (task->choice < 0) {
            task->choice = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        task->choice++;
        if (task->choice > 2) {
            task->choice = 2;
        }
    }
    if (old != task->choice) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STPLNMET_showChoiceWindows(task, windows, 1);
        for (i = 2; i >= 0; i--) {
            task->frames[i] = 0;
        }
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        task->substate = 100;
        return;
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate = 200;
        task->choice = 0;
    }
}

/* Hides the choice of the partners, or shows it again (partners->hide) */
void STPLNMET_hideChoice(PartnerChoice *task, s32 hide) {
    PartnerChoiceWindows *windows = task->children;

    if (hide != 0) {
        task->state = TASK_DONE;
        STPLNMET_showChoiceWindows(task, windows, 0);
    } else {
        task->state = TASK_RUN;
        task->substate = 0;
        STPLNMET_showChoiceWindows(task, windows, 1);
    }
}

/* The choice's task: creates its windows and waits hidden, then runs and draws it while shown */
void STPLNMET_updateChoice(PartnerChoice *task, PartnerChoiceWindows *windows) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        STPLNMET_createChoiceWindows(task, windows);
        STPLNMET_hideChoice(task, 1);
        break;
    case TASK_RUN:
        STPLNMET_runChoice(task, windows);
        STPLNMET_drawChoice(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the choice of the partners (task), hidden */
PartnerChoice *STPLNMET_createChoice(PlayerNameScreen *screen) {
    PartnerChoice *task = createTask(STPLNMET_updateChoice, sizeof(PartnerChoice), sizeof(PartnerChoiceWindows));

    task->hide = STPLNMET_hideChoice;
    task->layer = 0x1001;
    task->depth = 6;
    task->screen = screen;
    return task;
}
