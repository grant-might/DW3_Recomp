/* The training menu */

#include "menus/gym.h"

/* Creates the training menu's text windows */
void STGTRAIN_createMenuWindows(TrainMenu *menu, TextWindow **win) {
    win[0] = createTextWindow(menu->layerId, 1, 0xAE, 0x49);
    win[1] = createTextWindow(menu->layerId, 1, 0xA3, 0xA0);
    win[2] = createTextWindow(menu->layerId, 1, 0x74, 0xC0);
    win[3] = createTextWindow(menu->layerId, 1, 0x74, 0xCE);
}

/* Shows the name and description of the selected training (show) or hides them */
void STGTRAIN_showTrainingInfo(TrainMenu *menu, TextWindow **win, s32 show) {
    s32 entry;

    if (show != 0) {
        entry = menu->trainings[menu->page][menu->col + menu->row * 4];
        if (entry > 0) {
            win[2]->setString(win[2], FILE_CACHE.load(STGTRAIN_TEXT), STGTRAIN_state.trainings[entry].name);
            win[3]->setString(win[3], FILE_CACHE.load(STGTRAIN_TEXT), STGTRAIN_state.trainings[entry].desc);
            return;
        }
    }
    win[2]->setVisible(win[2], 0);
    win[3]->setVisible(win[3], 0);
}

/* Draws the training menu: its panels, the trainings of the page and the cursor */
void STGTRAIN_drawMenu(TrainMenu *menu) {
    SpriteDrawer sprite;
    s32 i;
    s32 entry;
    s32 x;
    s32 y;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(menu->layerId, menu->depth);
    sprite.setTexture(0x240, 0x100);
    if (menu->panels[0].level != 0) {
        if (menu->panels[0].level != ONE) {
            sprite.setScale(menu->panels[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x4E);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x22, 0x92, 0x43);
    }
    if (menu->cursorShown != 0) {
        if (GFX.funcs.getTime() - menu->cursorTime >= 0xB) {
            menu->cursorTime = GFX.funcs.getTime();
            menu->cursorClut++;
            if (menu->cursorClut >= 4) {
                menu->cursorClut = 0;
            }
        }
        sprite.setClutRow(menu->cursorClut);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x1E, menu->col * 40 + 0x94, menu->row * 40 + 0x64);
        sprite.setClutRow(0);
    }
    if (menu->panels[2].level != 0) {
        if (GFX.funcs.getTime() - menu->iconTime >= 0x10) {
            menu->iconTime = GFX.funcs.getTime();
            menu->iconFrame++;
            if (menu->iconFrame >= 4) {
                menu->iconFrame = 0;
            }
        }
        if (menu->panels[2].level != ONE) {
            sprite.setScale(menu->panels[2].level, menu->panels[2].level, ONE);
        }
        for (i = 0; i < 8; i++) {
            entry = menu->trainings[menu->page][i];
            if (entry != 0) {
                x = (i % 4) * 40;
                y = (i / 4) * 40;
                if (menu->panels[2].level != ONE) {
                    sprite.setPivot(x + 0xA8, y + 0x76);
                }
                if (entry == -1) {
                    sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x5F, x + 0x94, y + 0x64);
                } else if (i == menu->col + menu->row * 4) {
                    sprite.setClutRow(0);
                    sprite.draw(FILE_CACHE.getEntry(STGTRAIN_FILE_SPRITES << 16),
                                STGTRAIN_state.trainings[entry].icons[menu->iconFrame], x + 0x94, y + 0x64);
                } else {
                    sprite.setClutRow(1);
                    sprite.draw(FILE_CACHE.getEntry(STGTRAIN_FILE_SPRITES << 16), STGTRAIN_state.trainings[entry].icons[0],
                                x + 0x94, y + 0x64);
                }
            }
        }
    }
    if (menu->arrowShown != 0) {
        if (GFX.funcs.getTime() - menu->arrowTime >= 0xB) {
            menu->arrowTime = GFX.funcs.getTime();
            menu->arrowClut++;
            if (menu->arrowClut >= 4) {
                menu->arrowClut = 0;
            }
        }
        sprite.setClutRow(menu->arrowClut);
        if (menu->page == 0) {
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x2C, 0xE4, 0xA0);
        } else {
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x2B, 0x94, 0xA0);
        }
    }
    sprite.setClutRow(0);
    if (menu->panels[1].level != 0) {
        sprite.setScale(menu->panels[1].level, ONE, ONE);
        if (menu->panels[1].level != ONE) {
            sprite.setPivot(0x140, 0x8A);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x24, 0x8F, 0x5F);
    }
    if (menu->panels[3].level != 0) {
        if (menu->panels[3].level != ONE) {
            sprite.setScale(menu->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0xCD);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x23, 0x46, 0xBA);
    }
}

/* Shows the hint to the other page: R1 on the first page, L1 on the second */
static inline void STGTRAIN_showPageHint(TrainMenu *menu, TextWindow **win) {
    if (menu->page == 0) {
        win[1]->setString(win[1], FILE_CACHE.load(STGTRAIN_TEXT), 0x45);
        win[1]->setPos(win[1], 0xE4, 0xA0);
    } else {
        win[1]->setString(win[1], FILE_CACHE.load(STGTRAIN_TEXT), 0x44);
        win[1]->setPos(win[1], 0xA3, 0xA0);
    }
}

/* Turns the page with L1 or R1 when the gym has more than five trainings,
   putting the cursor on the new page's first training; or moves the cursor
   to the next training in the direction pressed, and closes the menu with
   the one under it picked (cross) or none (triangle) */
static inline void STGTRAIN_pickTraining(TrainMenu *menu, TextWindow **win) {
    s32 col;
    s32 row;

    col = menu->page;
    if (STGTRAIN_state.tableCount >= 6) {
        if (!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) {
            menu->page = 0;
        } else if (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) {
            menu->page = 1;
        }
    }
    if (col != menu->page) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STGTRAIN_showPageHint(menu, win);
        for (col = 0; col < 8; col++) {
            if (menu->trainings[menu->page][col] > 0) {
                menu->col = col % 4;
                menu->row = col / 4;
                break;
            }
        }
        STGTRAIN_showTrainingInfo(menu, win, 1);
        menu->iconFrame = 0;
        return;
    }
    col = menu->col;
    row = menu->row;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        for (;;) {
            if (--menu->col < 0) {
                menu->col = 0;
                break;
            }
            if (menu->trainings[menu->page][menu->col + menu->row * 4] > 0) {
                break;
            }
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        for (;;) {
            if (++menu->col >= 4) {
                menu->col = 3;
                break;
            }
            if (menu->trainings[menu->page][menu->col + menu->row * 4] > 0) {
                break;
            }
        }
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        for (;;) {
            if (--menu->row < 0) {
                menu->row = 0;
                break;
            }
            if (menu->trainings[menu->page][menu->col + menu->row * 4] > 0) {
                break;
            }
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        for (;;) {
            if (++menu->row >= 2) {
                menu->row = 1;
                break;
            }
            if (menu->trainings[menu->page][menu->col + menu->row * 4] > 0) {
                break;
            }
        }
    }
    if (col != menu->col || row != menu->row) {
        if (menu->trainings[menu->page][menu->col + menu->row * 4] > 0) {
            SOUND.playSound(SOUND_MENU_MOVE);
            STGTRAIN_showTrainingInfo(menu, win, 1);
        } else {
            menu->col = col;
            menu->row = row;
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_MOVE);
        menu->screen->training = menu->trainings[menu->page][menu->col + menu->row * 4];
        if (menu->screen->training > 0) {
            menu->substate = 0x32;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        menu->substate = 0x32;
        menu->step = 1;
    }
}

/*
 * Runs the training menu: opens its panels, moves the cursor over the
 * trainings of a page (L1 and R1 turn the pages when the gym has more than
 * five), and closes with a training picked (cross) or none (triangle).
 */
void STGTRAIN_runMenu(TrainMenu *menu, TextWindow **win) {
    switch (menu->substate) {
    case 0:
    default:
        STGTRAIN_state.startFade(&menu->panels[0], 1);
        menu->substate++;
        break;
    case 1:
        if (STGTRAIN_state.updateFade(&menu->panels[0])) {
            win[0]->setString(win[0], FILE_CACHE.load(STGTRAIN_TEXT), 7);
            STGTRAIN_state.startFade(&menu->panels[1], 1);
            menu->substate++;
        }
        break;
    case 2:
        if (STGTRAIN_state.updateFade(&menu->panels[1])) {
            STGTRAIN_state.startFade(&menu->panels[2], 1);
            menu->substate++;
        }
        break;
    case 3:
        if (STGTRAIN_state.updateFade(&menu->panels[2])) {
            if (STGTRAIN_state.tableCount >= 6) {
                menu->arrowShown = 1;
                STGTRAIN_showPageHint(menu, win);
            }
            STGTRAIN_state.startFade(&menu->panels[3], 1);
            menu->substate++;
        }
        break;
    case 4:
        if (STGTRAIN_state.updateFade(&menu->panels[3])) {
            STGTRAIN_showTrainingInfo(menu, win, 1);
            menu->cursorShown = 1;
            menu->substate = 10;
        }
        break;
    case 10:
        STGTRAIN_pickTraining(menu, win);
        break;
    case 0x32:
        menu->cursorShown = 0;
        menu->arrowShown = 0;
        win[1]->setVisible(win[1], 0);
        STGTRAIN_state.startFade(&menu->panels[2], 0);
        menu->substate++;
        break;
    case 0x33:
        if (STGTRAIN_state.updateFade(&menu->panels[2])) {
            STGTRAIN_showTrainingInfo(menu, win, 0);
            win[0]->setVisible(win[0], 0);
            STGTRAIN_state.startFade(&menu->panels[0], 0);
            STGTRAIN_state.startFade(&menu->panels[1], 0);
            STGTRAIN_state.startFade(&menu->panels[3], 0);
            menu->substate++;
        }
        break;
    case 0x34:
        STGTRAIN_state.updateFade(&menu->panels[0]);
        STGTRAIN_state.updateFade(&menu->panels[1]);
        if (STGTRAIN_state.updateFade(&menu->panels[3])) {
            if (menu->step != 0) {
                menu->setState(menu, TASK_DONE);
            } else {
                menu->state = TASK_KILL;
            }
        }
        break;
    }
}

/*
 * The training menu's task: a grid of trainings on two pages, filled from
 * the gym's table (the trainings it has), with the cursor on the last one.
 */
void STGTRAIN_updateMenu(TrainMenu *menu, TextWindow **win) {
    TrainEntry *table;
    s32 i;
    s32 page;
    s32 row;
    s32 col;

    switch (menu->state) {
    case TASK_INIT:
    default:
        menu->nextState(menu);
        STGTRAIN_createMenuWindows(menu, win);
        menu->panels[0].duration = 10;
        menu->panels[1].duration = 10;
        menu->panels[2].duration = 10;
        menu->panels[3].duration = 10;
        table = STGTRAIN_state.getTable(GAME.funcs.getModeArg());
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 3; col++) {
                if (row == 1 && col == 2) {
                    break;
                }
                menu->trainings[0][col + row * 4] = -1;
            }
        }
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 4; col++) {
                if (row != 1 || col != 0) {
                    menu->trainings[1][col + row * 4] = -1;
                }
            }
        }
        for (i = 0; i < 16; i++) {
            switch (table[i].id) {
            case 1:
            case 13:
                menu->trainings[0][0] = table[i].id;
                break;
            case 2:
            case 14:
                menu->trainings[0][1] = table[i].id;
                break;
            case 3:
            case 15:
                menu->trainings[0][2] = table[i].id;
                break;
            case 4:
            case 16:
                menu->trainings[0][4] = table[i].id;
                break;
            case 5:
            case 17:
                menu->trainings[0][5] = table[i].id;
                break;
            case 6:
            case 18:
                menu->trainings[1][0] = table[i].id;
                break;
            case 7:
            case 19:
                menu->trainings[1][1] = table[i].id;
                break;
            case 8:
            case 20:
                menu->trainings[1][2] = table[i].id;
                break;
            case 9:
            case 21:
                menu->trainings[1][3] = table[i].id;
                break;
            case 10:
            case 22:
                menu->trainings[1][5] = table[i].id;
                break;
            case 11:
            case 23:
                menu->trainings[1][6] = table[i].id;
                break;
            case 12:
            case 24:
                menu->trainings[1][7] = table[i].id;
                break;
            }
        }
        if (menu->screen->training > 0) {
            for (page = 0; page < 2; page++) {
                for (row = 0; row < 2; row++) {
                    for (col = 0; col < 4; col++) {
                        if (menu->trainings[page][col + row * 4] == menu->screen->training) {
                            menu->page = page;
                            menu->col = col;
                            menu->row = row;
                            break;
                        }
                    }
                }
            }
        }
        break;
    case TASK_RUN:
        STGTRAIN_runMenu(menu, win);
        STGTRAIN_drawMenu(menu);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Opens the training menu (its substate 0) */
void STGTRAIN_openMenu(TrainMenu *menu) {
    menu->state = TASK_RUN;
    menu->substate = 0;
}

/* Closes the training menu (its substate 0x32) */
void STGTRAIN_closeMenu(TrainMenu *menu) {
    menu->state = TASK_RUN;
    menu->substate = 0x32;
}

/* Shows the selected training's name and description */
void STGTRAIN_showMenuInfo(TrainMenu *menu) {
    STGTRAIN_showTrainingInfo(menu, menu->children, 1);
}

/* Creates the training menu */
TrainMenu *STGTRAIN_createMenu(TrainScreen *screen) {
    TrainMenu *menu = createTask(STGTRAIN_updateMenu, sizeof(TrainMenu), 0x10);

    menu->open = STGTRAIN_openMenu;
    menu->close = STGTRAIN_closeMenu;
    menu->showInfo = STGTRAIN_showMenuInfo;
    menu->layerId = SCREEN_LAYER;
    menu->depth = 6;
    menu->screen = screen;
    return menu;
}
