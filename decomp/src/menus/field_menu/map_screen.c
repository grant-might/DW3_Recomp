/* The map screen, with the areas visited and the towns. It begins STSTATUS's
   last object, which has no rodata: where its code starts is a guess */

#include "menus/field_menu.h"

/* Finds the area under the cursor and shows its name */
void STSTATUS_showMapArea(StatusMapScreen *screen, TextWindow **windows) {
    s32 wasHovering = screen->hovering;
    StatusMapSpot *spots;
    s32 spotX;
    s32 spotY;
    s32 x;
    s32 y;
    s32 i;

    screen->hovering = 0;
    x = screen->cursorX - screen->scrollX;
    y = screen->cursorY - screen->scrollY;
    spots = STSTATUS_data.spots;
    for (i = 1; i < 47; i++) {
        if (screen->visited[i - 1]) {
            spotX = spots[i].x;
            spotY = spots[i].y;
            if (spotX + 6 < x && x <= spotX + 0x12 && spotY + 6 < y && y <= spotY + 0x12) {
                screen->hovering = 1;
                screen->spot = i;
                break;
            }
        }
    }
    if (wasHovering != screen->hovering) {
        if (screen->hovering) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
#if VERSION_EU || VERSION_JP
            screen->hoverX = screen->cursorX;
#endif
            screen->hoverY = screen->cursorY;
        } else {
            windows[0]->setVisible(windows[0], 0);
        }
    }
    if (screen->hovering) {
        if (screen->hoverY < screen->height / 2) {
            windows[0]->setPos(windows[0], 0x10, 0xB9);
        } else {
            windows[0]->setPos(windows[0], 0x10, 0x19);
        }
        windows[0]->setString(windows[0], FILE_CACHE.load(screen->textFile), screen->spot);
    }
}

/* Moves the cursor, or the map when the cursor is in the middle */
void STSTATUS_moveMapCursor(StatusMapScreen *screen, s32 dx, s32 dy) {
    s32 middle;

    if (dx < 0) {
        if (screen->cursorX > 0xA0) {
            screen->cursorX -= screen->speedX;
            if (screen->cursorX < 0xA0) {
                screen->cursorX = 0xA0;
            }
        } else if (screen->cursorX < 0xA0) {
            screen->cursorX -= screen->speedX;
            if (screen->cursorX < 0x16) {
                screen->cursorX = 0x16;
            }
        } else {
            screen->scrollX += screen->speedX;
            if (screen->scrollX > 0) {
                screen->scrollX = 0;
                screen->cursorX = 0x9F;
            }
        }
    } else if (dx > 0) {
        if (screen->cursorX < 0xA0) {
            screen->cursorX += screen->speedX;
            if (screen->cursorX > 0xA0) {
                screen->cursorX = 0xA0;
            }
        } else if (screen->cursorX > 0xA0) {
            screen->cursorX += screen->speedX;
            if (screen->cursorX > 0x12A) {
                screen->cursorX = 0x12A;
            }
        } else {
            screen->scrollX -= screen->speedX;
            if (screen->scrollX < -0x48) {
                screen->scrollX = -0x48;
                screen->cursorX = 0xA1;
            }
        }
    }
    middle = screen->height / 2;
    if (dy < 0) {
        if (screen->cursorY > middle) {
            screen->cursorY -= screen->speedY;
            if (screen->cursorY < middle) {
                screen->cursorY = middle;
            }
        } else if (screen->cursorY < middle) {
            screen->cursorY -= screen->speedY;
            if (screen->cursorY < 0xF) {
                screen->cursorY = 0xF;
            }
        } else {
            screen->scrollY += screen->speedY;
            if (screen->scrollY > 0) {
                screen->scrollY = 0;
                screen->cursorY = middle - 1;
            }
        }
    } else if (dy > 0) {
        if (screen->cursorY < middle) {
            screen->cursorY += screen->speedY;
            if (screen->cursorY > middle) {
                screen->cursorY = middle;
            }
        } else if (screen->cursorY > middle) {
            screen->cursorY += screen->speedY;
            if (screen->cursorY > screen->height - screen->top - 0x1E) {
                screen->cursorY = screen->height - screen->top - 0x1E;
            }
        } else {
            screen->scrollY -= screen->speedY;
            if (screen->scrollY < -0x60) {
                screen->scrollY = -0x60;
                screen->cursorY = middle + 1;
            }
        }
    }
}

/* Moves the cursor faster while the pad is held; triangle closes */
void STSTATUS_runMapScreen(StatusMapScreen *screen, TextWindow **windows) {
    if (PAD_HELD(PAD_LEFT) || PAD_HELD(PAD_RIGHT)) {
        if (PAD_PRESSED(PAD_LEFT) || PAD_PRESSED(PAD_RIGHT)) {
            screen->speedX = 2;
        } else if (PAD_REPEATED(PAD_LEFT) || PAD_REPEATED(PAD_RIGHT)) {
            screen->speedX++;
            if (screen->speedX > 4) {
                screen->speedX = 4;
            }
        }
    } else {
        screen->speedX = 0;
    }
    if (PAD_HELD(PAD_UP) || PAD_HELD(PAD_DOWN)) {
        if (PAD_PRESSED(PAD_UP) || PAD_PRESSED(PAD_DOWN)) {
            screen->speedY = 2;
        } else if (PAD_REPEATED(PAD_UP) || PAD_REPEATED(PAD_DOWN)) {
            screen->speedY++;
            if (screen->speedY > 4) {
                screen->speedY = 4;
            }
        }
    } else {
        screen->speedY = 0;
    }
    if (PAD_HELD(PAD_LEFT)) {
        STSTATUS_moveMapCursor(screen, -1, 0);
    } else if (PAD_HELD(PAD_RIGHT)) {
        STSTATUS_moveMapCursor(screen, 1, 0);
    }
    if (PAD_HELD(PAD_UP)) {
        STSTATUS_moveMapCursor(screen, 0, -1);
    } else if (PAD_HELD(PAD_DOWN)) {
        STSTATUS_moveMapCursor(screen, 0, 1);
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->state = TASK_KILL;
    }
}

/* Draws the map, the visited areas, the towns and the cursor */
void STSTATUS_drawMapScreen(StatusMapScreen *screen) {
    SpriteDrawer sprite;
    s32 *towns;
#if VERSION_EU || VERSION_JP
    s32 x;
#endif
    s32 frame;
    s32 i;

    if (screen->hovering) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth - 2);
        sprite.setTexture(0x280, 0x100);
        if (screen->hoverY < screen->height / 2) {
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x3B, 0, 0xB4);
        } else {
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x3B, 0, 0x14);
        }
    }
    initSpriteDrawer(&sprite);
    if (screen->ready) {
        if (GFX.funcs.getTime() - screen->time >= 4) {
            screen->time = GFX.funcs.getTime();
            screen->cursorFrame++;
            if (screen->cursorFrame >= 4) {
                screen->cursorFrame = 0;
            }
            screen->homeFrame++;
            if (screen->homeFrame >= 3) {
                screen->homeFrame = 0;
            }
        }
        sprite.setTexture(0x300, 0x100);
        if (screen->hovering) {
            sprite.setLayerId(screen->layer, screen->depth);
            sprite.draw(FILE_CACHE.getEntry(screen->archive2), STSTATUS_cursorFrames[screen->cursorFrame] + 0x38,
                        STSTATUS_data.spots[screen->spot].x + 0xC + screen->scrollX,
                        STSTATUS_data.spots[screen->spot].y + 0xC + screen->scrollY + screen->top);
        } else {
            sprite.setLayerId(screen->layer, screen->depth - 1);
            sprite.draw(FILE_CACHE.getEntry(screen->archive2), STSTATUS_cursorFrames[screen->cursorFrame] + 0x32,
                        screen->cursorX, screen->cursorY + screen->top);
        }
        sprite.setLayerId(screen->layer, screen->depth - 2);
#if VERSION_EU || VERSION_JP
        if (screen->hovering) {
            x = screen->hoverX;
        } else {
            x = screen->cursorX;
        }
        /* the home mark faces the cursor */
        frame = 0x3B;
        if (x < screen->homeX + screen->scrollX) {
            frame = 0x35;
        }
#else
        frame = 0x35;
#endif
        sprite.draw(FILE_CACHE.getEntry(screen->archive2), frame + screen->homeFrame, screen->homeX + screen->scrollX,
                    screen->homeY + screen->scrollY + screen->top);
    }
    sprite.setLayerId(screen->layer, screen->depth - 1);
    if (screen->ready) {
        sprite.setTexture(0x300, 0x100);
        for (i = 1; i < 47; i++) {
            if (screen->visited[i - 1]) {
                sprite.draw(FILE_CACHE.getEntry(screen->archive2), STSTATUS_data.spots[i].sprite,
                            STSTATUS_data.spots[i].x + screen->scrollX,
                            STSTATUS_data.spots[i].y + screen->scrollY + screen->top);
            }
        }
    }
    sprite.setLayerId(screen->layer, screen->depth + 1);
    towns = STSTATUS_data.funcs.getList(screen->gameHalf, screen->progress);
    if (GAME.progress < 7) {
        screen->progress = 0;
    } else if (GAME.progress < 0xF) {
        screen->progress = 1;
    } else if (GAME.progress < 0x1B) {
        screen->progress = 2;
    } else if (GAME.progress < 0x1E) {
        screen->progress = 3;
    } else {
        screen->progress = 4;
    }
    sprite.setTexture(0x380, 0x100);
    /* the match depends on indexing the list rather than walking a pointer */
    for (i = 0; towns[i] != 0; i++) {
        sprite.draw(FILE_CACHE.getEntry(screen->archive1), towns[i] - 1, STSTATUS_data.towns[towns[i]].x + screen->scrollX,
                    STSTATUS_data.towns[towns[i]].y + screen->scrollY + screen->top);
    }
    sprite.setTexture(0x280, 0);
    sprite.setAltClut(0x140, 0x100);
    sprite.draw(FILE_CACHE.getEntry(screen->archive), 0, screen->scrollX, screen->scrollY + screen->top);
}

/* The map screen's update */
void STSTATUS_updateMapScreen(StatusMapScreen *screen, TextWindow **windows) {
    s32 area;

    switch (screen->state) {
    case TASK_INIT:
    default:
        switch (screen->substate) {
        case 0:
        default:
            if (FILE_CACHE.isLoading(screen->textFile) == 0) {
                screen->substate++;
            }
            break;
        case 1:
            windows[0] = createTextWindow(screen->layer, 1, 0x14, 0x16);
            windows[0]->setLines(windows[0], 2);
            screen->fade.duration = 10;
            STSTATUS_areaFuncs.getVisitedAreas(screen->visited);
            area = STSTATUS_areaFuncs.getArea() + 1;
            screen->homeX = STSTATUS_data.spots[area].x + 0xC;
            screen->homeY = STSTATUS_data.spots[area].y + 0xC;
            screen->speedX = 1;
            screen->speedY = 1;
            while (screen->homeX + screen->scrollX != screen->cursorX) {
                STSTATUS_moveMapCursor(screen, 1, 0);
            }
            while (screen->homeY + screen->scrollY != screen->cursorY) {
                STSTATUS_moveMapCursor(screen, 0, 1);
            }
            screen->speedX = 0;
            screen->speedY = 0;
            screen->ready = 1;
            screen->nextState(screen);
            break;
        }
        break;
    case TASK_RUN:
        STSTATUS_runMapScreen(screen, windows);
        STSTATUS_showMapArea(screen, windows);
        STSTATUS_drawMapScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the map screen (task): the map of the first half of the game or, in the
   late game, of the second (FILE_STATUS_BG + 2), and requests its area names */
Task *STSTATUS_createMapScreen(FieldMenuScreen *menu, s32 extra) {
    StatusMapScreen *screen = createTask(STSTATUS_updateMapScreen, sizeof(StatusMapScreen), 4);

    screen->layer = SCREEN_LAYER;
    screen->depth = 5;
    screen->menu = menu;
    screen->gameHalf = STSTATUS_areaFuncs.getGameHalf();
    if (screen->gameHalf == 0) {
        screen->archive = FILE_STATUS_BG << 16;
        screen->archive1 = (FILE_STATUS_BG << 16) + 1;
        screen->archive2 = (FILE_STATUS_BG << 16) + 2;
        screen->textFile = TEXT_FILE(TEXT_ASUKA_MAP);
        screen->file = FILE_STATUS_BG;
    } else {
        screen->archive = (FILE_STATUS_BG + 2) << 16;
        screen->archive1 = ((FILE_STATUS_BG + 2) << 16) + 1;
        screen->archive2 = ((FILE_STATUS_BG + 2) << 16) + 2;
        screen->textFile = TEXT_FILE(TEXT_AMATERASU_MAP);
        screen->file = FILE_STATUS_BG + 2;
    }
    FILE_CACHE.request(screen->textFile);
    screen->height = 0xF0;
    screen->top = 0x10;
    return (Task *)screen;
}
