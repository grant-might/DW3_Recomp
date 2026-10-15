/* The main menu's third screen: each table's recipes, checked against the ids
   a partner has */

#include "menus/digimon_lab.h"

/* Puts in found[row][slot] the owned ids of the recipe (row, col) and clears
   complete[row] when they're fewer than the recipe needs; gives the ids
   checked, 0 for a column past the recipes */
s32 STGDGLAB_findRecipe(LabRecipeScreen *screen, s32 row, u32 col, s32 slot) {
    s32 i;
    s32 j;
    s32 table;
    s32 *found;
    s16 *ids;
    s16 id;

    if (col >= 5) {
        return 0;
    }
    table = screen->table;
    ids = STGDGLAB_data.recipes[table][row * 4 + col].ids;
    found = screen->found[row][slot];
    screen->foundCount[row] = 0;
    for (i = 0; i < 5; i++) {
        found[i] = 0;
        id = ids[i];
        if (id > 0) {
            for (j = 0; j < screen->ownedCount; j++) {
                if (screen->owned[j] == id) {
                    found[i] = screen->owned[j];
                    screen->foundCount[row]++;
                    break;
                }
            }
        }
    }
    if (screen->foundCount[row] < STGDGLAB_data.recipes[table][row * 4 + col].needed) {
        screen->complete[row] = 0;
    }
    return 5;
}

/* Whether the partner has the first id of the recipe (row, col); 0 for a column
   past the recipes */
s32 STGDGLAB_hasRecipeId(LabRecipeScreen *screen, s32 row, u32 col) {
    s16 id;
    s32 i;
    s32 j;
    s16 *ids;

    if (col >= 5) {
        return 0;
    }
    id = 0;
    ids = STGDGLAB_data.recipes[screen->table][row * 4 + col].ids;
    for (i = 0; i < 5; i++) {
        if (*ids > 0) {
            id = *ids;
            break;
        }
        ids++;
    }
    if (id != 0) {
        for (j = 0; j < screen->ownedCount; j++) {
            if (screen->owned[j] == id) {
                return 1;
            }
        }
    }
    return 0;
}

/* How many of the row's four recipes the third screen shows: those whose first
   id the partner has (STGDGLAB_hasRecipeId) */
s32 STGDGLAB_countRowIds(LabRecipeScreen *screen, u32 row) {
    s32 i;
    s32 count;

    if (row >= 4) {
        return 0;
    }
    i = 0;
    count = 0;
    for (; i < 4; i++) {
        count += STGDGLAB_hasRecipeId(screen, row, i);
    }
    return count;
}

/* Puts the third screen's cursor on the row's first recipe, at its first id
   found (the last one when none is) */
void STGDGLAB_resetRecipeCursor(LabRecipeScreen *screen) {
    screen->slot = 0;
    screen->col = 0;
    while (screen->found[screen->row][screen->slot][screen->col] == 0) {
        if (++screen->col >= 5) {
            screen->col = 4;
            break;
        }
    }
}

/* Draws the third screen: the picked id, the frames, the recipes of the row
   (the ids found, the empty slots after them) and the arrows */
void STGDGLAB_drawRecipeScreen(LabRecipeScreen *screen, LabRecipeScreenWindows *win) {
    SpriteDrawer sprite;
    s32 complete;
    s32 frame;
    s32 id;
    s32 shown;
    /* The match depends on the slots' loop and the second pair of loops
       having their own counters, and on the first inner loop setting shown
       after j */
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 n;

    complete = 0;
    if (screen->complete[0] != 0 && screen->complete[1] != 0 && screen->complete[2] != 0) {
        complete = screen->complete[3] != 0;
    }
    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth - 1);
    if (screen->panels[1].level != 0) {
        id = screen->found[screen->row][screen->slot][screen->col];
        sprite.setTexture(0x140, 0x100);
        sprite.setAltClut(0x280, 0);
        sprite.setScale(screen->panels[1].level, ONE, ONE);
        if (screen->slot < 2) {
            sprite.setPivot(0, 0xC2);
            if (id != 0) {
                frame = STGDGLAB_data.funcs.getSprite(id);
                sprite.draw(FILE_CACHE.getEntry((FILE_LAB_SPRITES << 16) | 1), frame, 0x14, 0xAF);
            }
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x42, 0, 0xA8);
        } else {
            sprite.setPivot(0, 0x4C);
            if (id != 0) {
                frame = STGDGLAB_data.funcs.getSprite(id);
                sprite.draw(FILE_CACHE.getEntry((FILE_LAB_SPRITES << 16) | 1), frame, 0x14, 0x39);
            }
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x42, 0, 0x32);
        }
    }
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(screen->layer, screen->depth);
    if (GFX.funcs.getTime() - screen->time >= 8) {
        screen->time = GFX.funcs.getTime();
        if (++screen->clutRow >= 4) {
            screen->clutRow = 0;
        }
    }
    sprite.setClutRow(screen->clutRow);
    if (screen->panels[4].level == ONE) {
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x47, screen->col * 0x28 + 0x4A, screen->slot * 0x2E + 0x32);
    }
    if (screen->arrowLeft != 0) {
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x45, 0x17, 0xC9);
    }
    if (screen->arrowRight != 0) {
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x46, 0x110, 0xC9);
    }
    sprite.setClutRow(0);
    if (screen->panels[0].level != ONE) {
        sprite.setScale(screen->panels[0].level, ONE, ONE);
        sprite.setPivot(0, 0x1F);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x44, 0, 0x11);
    if (screen->panels[0].level != ONE) {
        sprite.setPivot(0x140, 0x1F);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x43, 0xF0, 0x11);
    if (screen->panels[4].level != ONE) {
        sprite.setScale(screen->panels[4].level, ONE, ONE);
        sprite.setPivot(0x44, 0x42);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    if (screen->found[screen->row][0][0] != -1) {
        if (complete) {
            sprite.setClutRow(2);
        } else if (screen->complete[screen->row] != 0) {
            sprite.setClutRow(1);
        }
        for (i = 0; i < 4; i++) {
            for (j = 4, shown = 0; j >= 0; j--) {
                id = screen->found[screen->row][i][j];
                if (id != 0) {
                    if (STGDGLAB_data.funcs.getSprite(id) != -1) {
                        shown = 1;
                        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3F, j * 0x28 + 0x4A, i * 0x2E + 0x32);
                    }
                } else if (shown) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x40, j * 0x28 + 0x4A, i * 0x2E + 0x32);
                }
            }
        }
    } else {
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3F, 0x4A, 0x32);
    }
    if (complete) {
        sprite.setClutRow(2);
    } else {
        sprite.setClutRow(0);
    }
    if (screen->panels[3].level != ONE) {
        sprite.setScale(ONE, screen->panels[3].level, ONE);
        sprite.setPivot(0x40, 0x3F);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    for (k = screen->slots - 2; k >= 0; k--) {
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3C, 0x3D, k * 0x2E + 0x3F);
    }
    if (screen->panels[2].level != ONE) {
        sprite.setScale(ONE, screen->panels[2].level, ONE);
        sprite.setPivot(0x3D, 0x42);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3D, 0x14, 0x32);
    sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x40, 0x24, 0x32);
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x140, 0x100);
    sprite.setAltClut(0x280, 0);
    sprite.setLayerId(screen->layer, screen->depth - 1);
    if (screen->panels[2].level != ONE) {
        sprite.setScale(ONE, screen->panels[2].level, ONE);
        sprite.setPivot(0x3D, 0x42);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    id = STGDGLAB_tableItems[screen->table];
    sprite.draw(FILE_CACHE.getEntry((FILE_LAB_SPRITES << 16) | 1), STGDGLAB_data.funcs.getSprite(id), 0x14, 0x32);
    if (screen->panels[4].level != ONE) {
        sprite.setScale(screen->panels[4].level, ONE, ONE);
        sprite.setPivot(0x44, 0x42);
    } else {
        sprite.setScale(ONE, ONE, ONE);
    }
    if (screen->found[screen->row][0][0] != -1) {
        for (m = 0; m < 4; m++) {
            for (n = 4; n >= 0; n--) {
                id = screen->found[screen->row][m][n];
                if (id != 0) {
                    frame = STGDGLAB_data.funcs.getSprite(id);
                    if (frame != -1) {
                        sprite.draw(FILE_CACHE.getEntry((FILE_LAB_SPRITES << 16) | 1), frame, n * 0x28 + 0x4A, m * 0x2E + 0x32);
                    }
                }
            }
        }
    } else {
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x49, 0x4A, 0x32);
    }
}

#if VERSION_EU || VERSION_JP
/* Moves the third screen's cursor by step slots, to the first column with
   an id; whether it moved */
s32 STGDGLAB_moveRecipeCursor(LabRecipeScreen *screen, s32 step) {
    s32 old = screen->slot;
    s32 col;

    do {
        screen->slot += step;
        if (screen->slot < 0) {
            screen->slot = 0;
        } else if (screen->slots - 1 < screen->slot) {
            screen->slot = screen->slots - 1;
        }
        col = 0;
        while (screen->found[screen->row][screen->slot][col] == 0) {
            if (++col >= 5) {
                break;
            }
        }
    } while (screen->found[screen->row][screen->slot][col] == 0);
    if (old != screen->slot) {
        screen->col = col;
        return 1;
    }
    return 0;
}
#endif

#define FOUND(screen) ((screen)->found[(screen)->row][(screen)->slot][(screen)->col])

/* Finds the owned ids of each recipe of the table's four rows: a row is
   complete when none of its recipes lacks ids, and has found[row][0][0] -1
   when it has none */
static inline void checkRecipes(LabRecipeScreen *screen) {
    s32 i;
    s32 j;
    s32 n;
    s32 count;
    s32 found;

    for (i = 0; i < 4; i++) {
        screen->complete[i] = 1;
        for (j = 0, n = 0, count = 0; j < 4; j++) {
            found = STGDGLAB_hasRecipeId(screen, i, j);
            count += found;
            if (found) {
                STGDGLAB_findRecipe(screen, i, j, n);
                n++;
            } else if (STGDGLAB_data.recipes[screen->table][i * 4 + j].needed != 0) {
                screen->complete[i] = 0;
            }
        }
        if (count == 0) {
            screen->found[i][0][0] = -1;
        }
    }
}

/* Once the title's panel has faded in, the title with the partner's name, the
   row's number and the arrows' hints; then fades the row's panel in */
static inline void showRecipeTitle(LabRecipeScreen *screen, LabRecipeScreenWindows *win) {
    PartnerStats *stats;

    STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
    if (STGDGLAB_data.funcs.updateFade(&screen->panels[2]) != 0) {
        stats = GAME.funcs.getPartnerStats(screen->table);
        win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x27);
        win->title->setSubString(win->title, stats, -1, 1);
        win->rowNumber->setNumber(win->rowNumber, 0, screen->row + 1);
        win->rowNumber->setRightAlign(win->rowNumber, 1);
        win->rowLabel->setString(win->rowLabel, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x28);
        win->prev->setString(win->prev, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x29);
        win->prev->setVisible(win->prev, 0);
        win->next->setString(win->next, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x2A);
        STGDGLAB_data.funcs.startFade(&screen->panels[3], 1);
        screen->substate++;
    }
}

/* Moves the cursor over the row's owned ids, skipping the empty places; cross
   opens the picked id's name and description */
static inline void pickRecipeId(LabRecipeScreen *screen) {
    s32 oldCol;
    s32 oldSlot;

    oldCol = screen->col;
    oldSlot = screen->slot;
#if VERSION_US
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        do {
            if (--screen->slot < 0) {
                screen->slot = 0;
                break;
            }
        } while (FOUND(screen) == 0);
        if (FOUND(screen) == 0) {
            screen->slot = oldSlot;
            if (screen->slots != 0) {
                if (screen->col == 0) {
                    screen->col = 1;
                } else {
                    screen->col--;
                }
                do {
                    if (--screen->slot < 0) {
                        screen->slot = 0;
                        break;
                    }
                } while (FOUND(screen) == 0);
                if (FOUND(screen) == 0) {
                    screen->slot = oldSlot;
                    screen->col = oldCol;
                }
            }
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        do {
            if (++screen->slot > screen->slots - 1) {
                screen->slot = screen->slots - 1;
                break;
            }
        } while (FOUND(screen) == 0);
        if (FOUND(screen) == 0) {
            screen->slot = oldSlot;
            if (screen->slots != 0) {
                if (screen->col == 0) {
                    screen->col = 1;
                } else {
                    screen->col--;
                }
                do {
                    if (++screen->slot > screen->slots - 1) {
                        screen->slot = screen->slots - 1;
                        break;
                    }
                } while (FOUND(screen) == 0);
                if (FOUND(screen) == 0) {
                    screen->slot = oldSlot;
                    screen->col = oldCol;
                }
            }
        }
    }
#elif VERSION_EU || VERSION_JP
    if (screen->slots >= 2) {
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            STGDGLAB_moveRecipeCursor(screen, -1);
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            STGDGLAB_moveRecipeCursor(screen, 1);
        }
    }
    if (oldSlot == screen->slot)
#endif
    {
        if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
            do {
                if (--screen->col < 0) {
                    screen->col = 0;
                    break;
                }
            } while (FOUND(screen) == 0);
            if (FOUND(screen) == 0) {
                screen->col = oldCol;
            }
        } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
            do {
                if (++screen->col >= 5) {
                    screen->col = 4;
                    break;
                }
            } while (FOUND(screen) == 0);
            if (FOUND(screen) == 0) {
                screen->col = oldCol;
            }
        }
    }
    if (oldCol != screen->col || oldSlot != screen->slot) {
        SOUND.playSound(SOUND_MENU_MOVE);
    }
#if VERSION_EU || VERSION_JP
    else
#endif
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        STGDGLAB_data.funcs.startFade(&screen->panels[1], 1);
        screen->substate++;
    }
}

/* Once its panel has faded in, the picked id's name and description, by
   the panel of its place */
static inline void showRecipeName(LabRecipeScreen *screen, LabRecipeScreenWindows *win) {
    s32 id;
    s32 nameId;

    if (STGDGLAB_data.funcs.updateFade(&screen->panels[1]) != 0) {
        id = FOUND(screen);
        nameId = GET_DIGIMON(id)->nameId;
        if (id != 0) {
            if (screen->slot < 2) {
                win->name->setPos(win->name, 0x3A, 0xAD);
                win->desc->setPos(win->desc, 0x3A, 0xBC);
            } else {
                win->name->setPos(win->name, 0x3A, 0x37);
                win->desc->setPos(win->desc, 0x3A, 0x46);
            }
            win->name->setString(win->name, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), nameId);
            win->desc->setString(win->desc, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_INFO)), nameId);
        }
        screen->substate++;
    }
}

/* Fades the row out and, unless the screen is closing (counter), the next
   row in */
static inline void changeRecipeRow(LabRecipeScreen *screen) {
    switch (screen->step) {
    case 0:
    default:
        STGDGLAB_data.funcs.startFade(&screen->panels[4], 0);
        screen->step++;
        break;
    case 1:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4]) != 0) {
            STGDGLAB_data.funcs.startFade(&screen->panels[3], 0);
            screen->row = screen->nextRow;
            screen->step++;
        }
        break;
    case 2:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[3]) != 0) {
            if (screen->counter == 0) {
                STGDGLAB_resetRecipeCursor(screen);
                STGDGLAB_data.funcs.startFade(&screen->panels[3], 1);
                STGDGLAB_data.funcs.startFade(&screen->panels[4], 1);
                screen->slots = STGDGLAB_countRowIds(screen, screen->row);
                screen->step++;
            } else {
                screen->state = TASK_RUN;
            }
        }
        break;
    case 3:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[3]) != 0) {
            screen->step++;
        }
        break;
    case 4:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4]) != 0) {
            screen->state = TASK_RUN;
        }
        break;
    }
}

/* L1 and R1 go to the previous or next row (the arrows show where there is
   one), triangle closes the screen; otherwise the cursor moves over the row */
static inline void runRecipeRow(LabRecipeScreen *screen, LabRecipeScreenWindows *win) {
    screen->nextRow = screen->row;
    if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
        if (--screen->nextRow < 0) {
            screen->nextRow = 0;
        }
    } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
        if (++screen->nextRow > screen->rowCount - 1) {
            screen->nextRow = screen->rowCount - 1;
        }
    }
    if (screen->nextRow != screen->row) {
        SOUND.playSound(SOUND_MENU_MOVE);
        win->rowNumber->setNumber(win->rowNumber, 0, screen->nextRow + 1);
        win->rowNumber->setRightAlign(win->rowNumber, 1);
        if (screen->nextRow == 0) {
            win->prev->setVisible(win->prev, 0);
            screen->arrowLeft = 0;
        } else if (screen->nextRow == screen->rowCount - 1) {
            win->next->setVisible(win->next, 0);
            screen->arrowRight = 0;
        } else {
            win->prev->setVisible(win->prev, 1);
            win->next->setVisible(win->next, 1);
            screen->arrowLeft = 1;
            screen->arrowRight = 1;
        }
        screen->state = TASK_DONE;
        screen->step = 0;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->state = TASK_DONE;
        screen->substate = 7;
        screen->step = 0;
        screen->counter = 1;
        screen->arrowLeft = 0;
        screen->arrowRight = 0;
        win->prev->setVisible(win->prev, 0);
        win->next->setVisible(win->next, 0);
    } else if (screen->found[screen->row][0][0] != -1) {
        pickRecipeId(screen);
    }
}

/* The third screen: a row of recipes at a time, the picked id's name and
   description on cross */
void STGDGLAB_updateRecipeScreen(LabRecipeScreen *screen, LabRecipeScreenWindows *win) {
    switch (screen->state) {
    case TASK_INIT:
    default:
        if (screen->lab->menuOpen(screen->lab)) {
            screen->nextState(screen);
            screen->table = GAME.funcs.getPartyMember(screen->lab->member);
            screen->ownedCount = GAME.funcs.listPartnerEntries(screen->table, screen->owned);
            win->title = createTextWindow(screen->layer, 1, 0x14, 0x19);
            win->rowNumber = createTextWindow(screen->layer, 1, 0xFF, 0x19);
            win->rowLabel = createTextWindow(screen->layer, 1, 0x101, 0x19);
            win->prev = createTextWindow(screen->layer, 1, 0x27, 0xC4);
            win->prev->setDepth(win->prev, 2);
            win->next = createTextWindow(screen->layer, 1, 0x115, 0xC4);
            win->next->setDepth(win->next, 2);
            win->name = createTextWindow(screen->layer, 1, 0x14, 0x19);
            win->name->setDepth(win->name, 0);
            win->desc = createTextWindow(screen->layer, 1, 0x14, 0x19);
            win->desc->setDepth(win->desc, 0);
            checkRecipes(screen);
            screen->rowCount = 4;
            screen->slots = STGDGLAB_countRowIds(screen, screen->row);
            STGDGLAB_resetRecipeCursor(screen);
            screen->panels[0].duration = 8;
            screen->panels[1].duration = 8;
            screen->panels[2].duration = 8;
            screen->panels[4].duration = 8;
            screen->panels[3].duration = 8;
            screen->arrowLeft = 0;
            screen->arrowRight = 1;
            STGDGLAB_data.funcs.startFade(&screen->panels[0], 1);
            STGDGLAB_data.funcs.startFade(&screen->panels[2], 1);
            STGDGLAB_data.funcs.startFade(&screen->panels[4], 1);
            STGDGLAB_data.funcs.startFade(&screen->panels[3], 1);
        }
        break;
    case TASK_RUN:
        switch (screen->substate) {
        case 0:
        default:
            showRecipeTitle(screen, win);
            break;
        case 1:
            if (STGDGLAB_data.funcs.updateFade(&screen->panels[3]) != 0) {
                STGDGLAB_data.funcs.startFade(&screen->panels[4], 1);
                screen->substate++;
            }
            break;
        case 2:
            if (STGDGLAB_data.funcs.updateFade(&screen->panels[4]) != 0) {
                screen->substate++;
            }
            break;
        case 3:
            runRecipeRow(screen, win);
            break;
        case 4:
            showRecipeName(screen, win);
            break;
        case 5:
            if (PAD_PRESSED(PAD_CROSS)) {
                STGDGLAB_data.funcs.startFade(&screen->panels[1], 0);
                win->name->setVisible(win->name, 0);
                win->desc->setVisible(win->desc, 0);
                screen->substate++;
            }
            break;
        case 6:
            if (STGDGLAB_data.funcs.updateFade(&screen->panels[1]) != 0) {
                screen->substate = 3;
            }
            break;
        case 7:
            STGDGLAB_data.funcs.startFade(&screen->panels[0], 0);
            STGDGLAB_data.funcs.startFade(&screen->panels[2], 0);
            win->title->setVisible(win->title, 0);
            win->rowNumber->setVisible(win->rowNumber, 0);
            win->rowLabel->setVisible(win->rowLabel, 0);
            screen->substate++;
            break;
        case 8:
            STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
            if (STGDGLAB_data.funcs.updateFade(&screen->panels[2]) != 0) {
                screen->setState(screen, TASK_KILL);
            }
            break;
        }
        STGDGLAB_drawRecipeScreen(screen, win);
        break;
    case TASK_DONE:
        changeRecipeRow(screen);
        STGDGLAB_drawRecipeScreen(screen, win);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the main menu's third screen (task), the recipes, and fades the
   menu's page out */
Task *STGDGLAB_createRecipeScreen(Lab *lab) {
    LabRecipeScreen *screen = createTask(STGDGLAB_updateRecipeScreen, sizeof(LabRecipeScreen), 0x1C);

    screen->layer = SCREEN_LAYER;
    screen->depth = 2;
    screen->lab = lab;
    lab->closeMenu(lab);
    return (Task *)screen;
}

/* The item each table's screen shows (STGDGLAB_entries) */
s32 STGDGLAB_tableItems[] = {
    383, 385, 384, 3,
    145, 366, 373, 31,
};
