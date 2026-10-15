/* FIGHTSTG's confused menu, the commands of a confused partner. */

#include "battle/battle_mode.h"

/* Shows the confused menu's six lines once, in FIGHTSTG_confusedLines's order */
void FIGHTSTG_showConfusedCommands(ConfusedMenu *task) {
    ConfusedMenuWindows *w = task->children;
    char *text;
    s32 i;

    if (w->lines[0] == NULL) {
        text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
        for (i = 0; i < 6; i++) {
            w->lines[i] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x24, i * 0x13 + 0x6D);
            w->lines[i]->setString(w->lines[i], text, FIGHTSTG_confusedLines[i] + 0x6B);
        }
    }
}

/* Draws the confused menu's message box and, once its lines are out, its
   blinking arrow */
void FIGHTSTG_drawConfusedMessageBox(ConfusedMenu *task) {
    SpriteDrawer drawer;
    void *sheet;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    if (task->showArrow != 0) {
        if (GFX.funcs.getTime() - task->arrowTime >= 4) {
            task->arrowTime = GFX.funcs.getTime();
            task->arrowPalette++;
            if (task->arrowPalette >= 5) {
                task->arrowPalette = 0;
            }
        }
        drawer.setTexture(0x140, 0);
        drawer.setClutRow(task->arrowPalette);
        drawer.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 10, 0x123, 0xD0);
        drawer.setClutRow(0);
    }
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0x31, 0xB, 0xBC);
}

/* Shows the confused message's lines one by one, 3 frames apart, then ends on
   cross */
void FIGHTSTG_stepConfusedMessage(ConfusedMenu *task, ConfusedMenuWindows *w) {
    s32 pressed = PAD.getPressed(0);

    switch (task->substate) {
    case 0:
    default:
        if (task->started != 0) {
            task->substate++;
        }
        break;
    case 1:
        task->time = GFX.funcs.getTime();
        task->delay = 3;
        w->lines[task->line]->setVisible(w->lines[task->line], 1);
        task->substate++;
        break;
    case 2:
        if (task->delay < GFX.funcs.getTime() - task->time) {
            if (++task->line >= task->lineCount) {
                task->showArrow = 1;
                if (pressed & (1 << PAD_CROSS)) {
                    SOUND.playSound(SOUND_MENU_CONFIRM);
                    if (task->queue[++task->next] == 0) {
                        task->state = 3;
                    } else {
                        task->show(task, task->queue[task->next], 0);
                        task->setSubstate(task, 0);
                    }
                    task->showArrow = 0;
                }
            } else {
                task->substate = 1;
            }
        }
        break;
    }
}

/* Puts the active partner's name into the message's first line (side 0 only) */
void FIGHTSTG_setConfusedName(ConfusedMenu *task, ConfusedMenuWindows *windows, s32 side) {
    if (side == 0) {
        windows->lines[0]->setSubString(windows->lines[0], GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0])), -1, 1);
    }
}

/* the confused menu's (FIGHTSTG_updateConfusedMenu) results, two per line,
   each a message (task->queue[0]) and its line of text 0x80 */
ConfusedMessage FIGHTSTG_confusedMessages[16] = {
    { 1, 0x73 }, { 2, 0x74 }, { 3, 0x75 }, { 4, 0x76 },
    { 5, 0x77 }, { 6, 0x78 }, { 7, 0x79 }, { 8, 0x7A },
    { 9, 0x7B }, { 10, 0x7C }, { 11, 0x7D }, { 12, 0x7E },
    { 13, 0x7F }, { 14, 0x80 }, { 15, 0x81 }, { 16, 0x82 },
};
/* the confused menu's lines, which it shuffles */
s16 FIGHTSTG_confusedLines[8] = {
    7, 1, 0, 3, 2, 5, 4, 6,
};

/* Sets up confused message index (FIGHTSTG_confusedMessages) with the
   partner's name and starts showing it */
void FIGHTSTG_showConfusedMessage(ConfusedMenu *task, s32 index, s32 unused) {
    ConfusedMenuWindows *w = task->children;

    task->started = 1;
    if (w->lines[0] == NULL) {
        w->lines[0] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xC2);
    }
    if (w->lines[1] == NULL) {
        w->lines[1] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xD0);
    }
    task->queue[0] = FIGHTSTG_confusedMessages[index].result;
    w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
    w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), FIGHTSTG_confusedMessages[index].text);
    task->lineCount = 2;
    FIGHTSTG_setConfusedName(task, w, 0);
    w->lines[0]->setVisible(w->lines[0], 0);
    w->lines[1]->setVisible(w->lines[1], 0);
}

/* the confused menu's cursor */
CursorLayout FIGHTSTG_confusedCursor = {
    6, 17, 110, 19, 34, 16, 109, 19,
};

/* The confused menu's task: shuffles the lines, then on cross attacks
   (*result 0) when FIGHTSTG_battleFuncs.testConfusion fails; else closes the
   lines, the partner view and the camera and shows one of the picked line's
   two messages at random, giving 1 when it is done */
void FIGHTSTG_updateConfusedMenu(ConfusedMenu *task, ConfusedMenuWindows *w) {
    s32 i;
    s32 j;
    s16 line;

    switch (task->state) {
    case TASK_INIT:
    default:
        w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_confusedCursor);
        w->cursor->sel = task->firstLine;
        for (i = 0; i < 8; i++) {
            j = RANDOM.next() % 8;
            line = FIGHTSTG_confusedLines[i];
            FIGHTSTG_confusedLines[i] = FIGHTSTG_confusedLines[j];
            FIGHTSTG_confusedLines[j] = line;
        }
        FIGHTSTG_showConfusedCommands(task);
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            if (PAD.getPressed(0) & (1 << PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                if (FIGHTSTG_battleFuncs.testConfusion(0) == 0) {
                    *task->result = 0;
                    task->setState(task, 3);
                } else {
                    task->picked = w->cursor->sel;
                    for (i = 0; i < 6; i++) {
                        w->lines[i]->setState(w->lines[i], 3);
                    }
                    w->cursor->setState(w->cursor, 3);
                    task->partnerView->state = 3;
                    task->shotCamera->state = 3;
                    task->nextSubstate(task);
                }
                w->cursor->locked = 1;
            }
            break;
        case 1:
            if (w->lines[0] == NULL && w->lines[1] == NULL) {
                FIGHTSTG_showConfusedMessage(task, (FIGHTSTG_confusedLines[task->picked] << 1) | (RANDOM.next() & 1), 0);
                task->setState(task, 2);
            }
            break;
        }
        break;
    case TASK_DONE:
        FIGHTSTG_drawConfusedMessageBox(task);
        FIGHTSTG_stepConfusedMessage(task, w);
        if (task->state == 3) {
            *task->result = 1;
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Opens a confused partner's command menu; *result gets 0 for an attack
   or 1 for a lost turn */
ConfusedMenu *FIGHTSTG_createConfusedMenu(s32 *result, Task *partnerView, Task *shotCamera) {
    ConfusedMenu *task = createTask(FIGHTSTG_updateConfusedMenu, sizeof(ConfusedMenu), sizeof(ConfusedMenuWindows));

    task->result = result;
    *result = -1;
    task->firstLine = 0;
    task->partnerView = partnerView;
    task->shotCamera = shotCamera;
    return task;
}
