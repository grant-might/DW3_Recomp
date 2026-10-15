#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Starts opening (open != 0) or closing a panel */
void startPanel(PanelAnim *panel, s32 open) {
    panel->active = 1;
    if (open) {
        SOUND.playSound(SOUND_MENU_OPEN);
        panel->level = 0;
        panel->step = ONE / panel->duration;
    } else {
        SOUND.playSound(SOUND_MENU_CLOSE);
        panel->level = ONE;
        panel->step = -((ONE / panel->duration) * 2);
    }
}

/* Advances a panel animation; 1 once it has finished */
s32 updatePanel(PanelAnim *panel) {
    if (!panel->active) {
        return 1;
    }
    panel->level += panel->step;
    if (panel->step > 0) {
        if (panel->level > ONE) {
            panel->level = ONE;
            panel->active = 0;
            return 1;
        }
    } else if (panel->level < 0) {
        panel->level = 0;
        panel->active = 0;
        return 1;
    }
    return 0;
}

/* The party pages' windows: [0] the name, [1]-[5] the labels and [6]-[10] the values */
WindowPos FIELD_MENU_LAYOUT[11] = {
    {12, 55, 0, 19},
    {1, 16, 0, 28},
    {2, 16, 0, 37},
    {4, 61, 0, 37},
    {3, 16, 0, 46},
    {4, 61, 0, 46},
    {13, 44, 0, 28},
    {14, 59, 0, 37},
    {14, 94, 0, 37},
    {14, 59, 0, 46},
    {14, 94, 0, 46},
};

/* The stats the pages' values show, as indices into the stats halfwords */
s32 PAGE_STATS[5] = { STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP, STAT_MAX_MP };

/* Creates the field menu's windows: the title, the options, the cursor, the money and the pages */
void createFieldMenuWindows(FieldMenu *task, FieldMenuWindows *win) {
    s32 i;
    s32 j;
    WindowPos *pos;

    win->title = createTextWindow(task->layerId, 1, 0x98, 0x13);
    for (i = 0; i < task->count; i++) {
        win->options[i] = createTextWindow(task->layerId, 1, 0xBD, 0x31 + i * 14);
    }
    win->cursor = createCursor(task->layerId, 0, 0xB0, task->cursor * 14 + 0x31);
    win->cursor->setVisible(win->cursor, 0);
    win->moneyLabel = createTextWindow(task->layerId, 3, 0x46, 0xA6);
    win->money = createTextWindow(task->layerId, 3, 0x42, 0xA6);
    for (j = 0; j < 3; j++) {
        pos = &FIELD_MENU_LAYOUT[0];
        win->pages[j].name = createTextWindow(task->layerId, 1, pos->x, pos->y + j * 46);
        for (i = 0; i < 5; i++) {
            pos = &FIELD_MENU_LAYOUT[1 + i];
            win->pages[j].labels[i] = createTextWindow(task->layerId, 3, pos->x, pos->y + j * 46);
        }
        for (i = 0; i < 5; i++) {
            pos = &FIELD_MENU_LAYOUT[6 + i];
            win->pages[j].values[i] = createTextWindow(task->layerId, 3, pos->x, pos->y + j * 46);
        }
    }
}

/* Fills a party member's page of the field menu (name and five stats), or hides it */
void showPartnerPage(void *menu, FieldMenuWindows *win, s32 page, s32 show) {
    PartnerTotals stats;
    PartnerStats *info;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(page);
        if (id >= 0) {
            info = GAME.funcs.getPartnerStats(id);
            GAME.funcs.computeStats(id, &stats);
            win->pages[page].name->setString(win->pages[page].name, info, -1);
            for (i = 0; i < 5; i++) {
                /* the match depends on these two forms: (&FIELD_MENU_LAYOUT[1])[i] steps a pointer
                   from the labels, FIELD_MENU_LAYOUT[6 + i] below an offset from the array */
                win->pages[page].labels[i]->setString(win->pages[page].labels[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), (&FIELD_MENU_LAYOUT[1])[i].string);
                win->pages[page].values[i]->setNumber(win->pages[page].values[i], 0, stats.stats[PAGE_STATS[i]]);
                win->pages[page].values[i]->setRightAlign(win->pages[page].values[i], 1);
            }
        } else {
            win->pages[page].name->setString(win->pages[page].name, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0xC);
            for (i = 0; i < 5; i++) {
                win->pages[page].values[i]->setString(win->pages[page].values[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), FIELD_MENU_LAYOUT[6 + i].string);
                win->pages[page].values[i]->setRightAlign(win->pages[page].values[i], 1);
            }
        }
    } else {
        win->pages[page].name->setVisible(win->pages[page].name, 0);
        for (i = 0; i < 5; i++) {
            win->pages[page].labels[i]->setVisible(win->pages[page].labels[i], 0);
            win->pages[page].values[i]->setVisible(win->pages[page].values[i], 0);
        }
    }
}

/* The field's stages by mode: -1 from MODE_NEW_GAME (where the field menu
   greys out option 2), 1 from 0x270, else 0 */
s32 getFieldZone(void) {
    s32 value = GAME.funcs.getMode();

    if (value == MODE_STATUS) {
        value = GAME.fieldMode;
    }
    if (value >= MODE_NEW_GAME) {
        return -1;
    }
    return value >= 0x270;
}

/* The options' strings, without and with the extra option */
s32 FIELD_MENU_OPTIONS[2][6] = {
    { 6, 7, 8, 9, 10, 0 },
    { 6, 7, 8, 9, 10, 11 },
};

s32 FIELD_MENU_SPRITES[6] = { 29, 30, 31, 32, 33, 34 };

/* Starts the field menu: opens the panels and the windows, with the extra option
   when the player has item 0x192 and option 2 only in a field zone */
static inline void openFieldMenu(FieldMenu *task, FieldMenuWindows *win) {
    task->nextState(task);
    task->panels[0].duration = task->panels[1].duration = task->panels[2].duration = 10;
    startPanel(&task->panels[0], 1);
    startPanel(&task->panels[1], 1);
    startPanel(&task->panels[2], 1);
    if (GAME.items[0x192] != 0) {
        FIELD_MENU_CHOICE.extra = 1;
        task->extraOption = 1;
    } else {
        FIELD_MENU_CHOICE.extra = 0;
    }
    task->count = task->extraOption + 5;
    if (getFieldZone() >= 0) {
        task->option2Enabled = 1;
    } else {
        task->option2Enabled = 0;
    }
    createFieldMenuWindows(task, win);
}

/* Once the second panel is open: the options (option 2 greyed when disabled) */
static inline void showFieldMenuOptions(FieldMenu *task, FieldMenuWindows *win) {
    showPartnerPage(task, win, 1, 1);
    for (task->counter = 0; task->counter < task->count; task->counter++) {
        win->options[task->counter]->setString(win->options[task->counter], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)),
                                  FIELD_MENU_OPTIONS[task->extraOption][task->counter]);
    }
    SOUND.playSound(SOUND_MENU_OPEN);
    if (task->option2Enabled == 0) {
        win->options[2]->setPalette(win->options[2], PALETTE_GREY);
    }
}

/* Once the third panel is open: the money and the cursor */
static inline void showFieldMenuMoney(FieldMenu *task, FieldMenuWindows *win) {
    showPartnerPage(task, win, 2, 1);
    win->moneyLabel->setString(win->moneyLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 5);
    win->money->setNumber(win->money, 0, GAME.money);
    win->money->setRightAlign(win->money, 1);
    win->cursor->setVisible(win->cursor, 1);
}

/*
 * Moves the cursor, or on a choice (option 2 only when enabled) or a cancel
 * closes the panels, with the step that picks the next mode
 */
static inline void chooseFieldMenuOption(FieldMenu *task, FieldMenuWindows *win) {
    s32 prev;
    s32 done;

    prev = task->cursor;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--task->cursor < 0) {
            task->cursor = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        task->cursor++;
        if (task->cursor > task->count - 1) {
            task->cursor = task->count - 1;
        }
    }
    if (prev != task->cursor) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0xB0, task->cursor * 14 + 0x31);
        return;
    }
    done = 0;
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (task->option2Enabled == 0 && task->cursor == 2) {
            return;
        }
        done = 1;
        if (GAME.funcs.getMode() == MODE_STATUS) {
            task->step = 1;
        } else {
            task->step = 0;
        }
        FIELD_MENU_CHOICE.option = task->cursor;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        done = 1;
        if (GAME.funcs.getMode() == MODE_STATUS) {
            task->step = 0;
        } else {
            task->step = 1;
        }
    }
    if (done) {
        startPanel(&task->panels[0], 0);
        startPanel(&task->panels[1], 0);
        startPanel(&task->panels[2], 0);
        showPartnerPage(task, win, 2, 0);
        win->moneyLabel->setVisible(win->moneyLabel, 0);
        win->money->setVisible(win->money, 0);
        win->cursor->setVisible(win->cursor, 0);
        task->substate++;
    }
}

/*
 * Steps the fade to the next mode, a CLUT row each 2 frames: its first three
 * sprites through rows 0-7, the next three through rows 8-15; then it asks for the
 * mode (the field's, from the status screen; else the status screen, with the option)
 */
static inline void stepFieldMenuFade(FieldMenu *task) {
    switch (task->substate) {
    default:
        task->setState(task, TASK_DONE);
        /* fallthrough */
    case 0:
    case 1:
    case 2:
        if (GFX.funcs.getTime() - task->time >= 2) {
            task->time = GFX.funcs.getTime();
            if (++task->fadeRow >= 8) {
                if (++task->substate != 3) {
                    task->fadeRow = 0;
                } else {
                    task->fadeRow = 8;
                }
            }
        }
        break;
    case 3:
    case 4:
    case 5:
        if (GFX.funcs.getTime() - task->time >= 2) {
            task->time = GFX.funcs.getTime();
            if (++task->fadeRow >= 0x10) {
                if (++task->substate == 6) {
                    task->fadeRow = 0xF;
                } else {
                    task->fadeRow = 8;
                }
            }
        }
        break;
    case 6:
        if (GAME.funcs.getMode() == MODE_STATUS) {
            GAME.funcs.requestMode(GAME.fieldMode, 0);
        } else {
            GAME.funcs.requestMode(MODE_STATUS, 0);
            FIELD_MENU_CHOICE.option = task->cursor;
        }
        task->substate++;
        break;
    case 7:
        break;
    }
}

/* Draws the fade to the next mode: the rows already dark, and the current one at its CLUT row */
static inline void drawFieldMenuFade(FieldMenu *task) {
    SpriteDrawer obj2;
    s32 j;

    initSpriteDrawer(&obj2);
    obj2.setTexture(0x140, 0);
    obj2.setLayerId(task->layerId, task->depth);
    obj2.setFollowScroll(0);
    for (j = 0; j <= task->substate; j++) {
        if (j == 6) {
            break;
        }
        if (j == task->substate) {
            obj2.setClutRow(task->fadeRow);
        } else if (j < 3) {
            obj2.setClutRow(7);
        } else {
            obj2.setClutRow(0xF);
        }
        obj2.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), FIELD_MENU_SPRITES[j], 0, 0);
    }
}

/* Runs the field menu: opens its panels, moves the cursor and switches mode on a choice */
void updateFieldMenu(FieldMenu *task, FieldMenuWindows *win) {
    SpriteDrawer obj;
    s32 i;
    s32 y;
    s32 y2;

    switch (task->state) {
    case 0:
    default:
        openFieldMenu(task, win);
        break;
    case 1:
        switch (task->substate) {
        default:
        case 0:
            if (updatePanel(&task->panels[0])) {
                showPartnerPage(task, win, 0, 1);
                win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x13);
                SOUND.playSound(SOUND_MENU_OPEN);
                task->substate++;
            }
            break;
        case 1:
            if (updatePanel(&task->panels[1])) {
                showFieldMenuOptions(task, win);
                task->substate++;
            }
            break;
        case 2:
            if (updatePanel(&task->panels[2])) {
                showFieldMenuMoney(task, win);
                task->substate++;
            }
            break;
        case 3:
            chooseFieldMenuOption(task, win);
            break;
        case 4:
            if (updatePanel(&task->panels[2])) {
                showPartnerPage(task, win, 1, 0);
                for (task->counter = 0; task->counter < task->count; task->counter++) {
                    win->options[task->counter]->setVisible(win->options[task->counter], 0);
                }
                SOUND.playSound(SOUND_MENU_CLOSE);
                task->substate++;
            }
            break;
        case 5:
            if (updatePanel(&task->panels[1])) {
                showPartnerPage(task, win, 0, 0);
                win->title->setVisible(win->title, 0);
                SOUND.playSound(SOUND_MENU_CLOSE);
                task->substate++;
            }
            break;
        case 6:
            if (updatePanel(&task->panels[0])) {
                if (task->step == 0) {
                    task->setState(task, TASK_DONE);
                    task->time = GFX.funcs.getTime();
                } else {
                    task->setState(task, TASK_KILL);
                }
            }
            break;
        }
        initSpriteDrawer(&obj);
        obj.setTexture(0x140, 0);
        obj.setLayerId(task->layerId, task->depth);
        obj.setFollowScroll(0);
        for (i = 0, y = 0x11, y2 = 0x25; i < 3; i++) {
            if (task->panels[i].level != 0) {
                if (task->panels[i].level != ONE) {
                    obj.setScale(task->panels[i].level, ONE, ONE);
                    obj.setPivot(0, y2);
                } else {
                    obj.setScale(ONE, ONE, ONE);
                }
                obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, y);
                obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, y);
            }
            y += 0x2E;
            y2 += 0x2E;
        }
        if (task->panels[0].level != 0) {
            if (task->panels[0].level != ONE) {
                obj.setScale(task->panels[0].level, ONE, ONE);
                obj.setPivot(0x140, 0x19);
            } else {
                obj.setScale(ONE, ONE, ONE);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
        }
        if (task->panels[1].level != 0) {
            if (task->panels[1].level != ONE) {
                obj.setScale(task->panels[1].level, ONE, ONE);
                obj.setPivot(0x140, 0x52);
            } else {
                obj.setScale(ONE, ONE, ONE);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x1C - task->extraOption, 0xA8, 0x28);
        }
        if (task->panels[2].level != 0) {
            if (task->panels[2].level != ONE) {
                obj.setScale(task->panels[2].level, ONE, ONE);
                obj.setPivot(0, 0xA8);
            } else {
                obj.setScale(ONE, ONE, ONE);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x1A, 0, 0x9E);
        }
        break;
    case 2:
        stepFieldMenuFade(task);
        drawFieldMenuFade(task);
        break;
    case 3:
        break;
    }
}

/* Opens the field menu on a layer with the cursor on an option */
FieldMenu *createFieldMenu(s32 layerId, s32 cursor) {
    FieldMenu *task = createTask(updateFieldMenu, sizeof(FieldMenu), sizeof(FieldMenuWindows));

    task->layerId = layerId;
    task->depth = 1;
    task->cursor = cursor;
    return task;
}
