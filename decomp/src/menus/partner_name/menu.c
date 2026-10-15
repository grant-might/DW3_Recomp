/* The partner menu, where the partner to rename is chosen */

#include "menus/partner_name.h"

/* Shows (creating it the first time) or hides a text window of the partner menu:
   2 to 4 are the party's names, the others strings of TEXT_DIGIMON_NAMING */
void STDGNAME_showMenuWindow(PartnerMenuTask *task, TextWindow **window, s32 index, s32 show) {
    s32 layer = 0;
    char *name;

    if (index < 5) {
        layer = task->layer;
    }
    if (show) {
        if (*window == NULL) {
            *window = createTextWindow(layer, 1, STDGNAME_menuWindows[index].x, STDGNAME_menuWindows[index].y);
            (*window)->setDepth(*window, 1);
        }
        if (index >= 2 && index <= 4) {
            name = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(index - 2))->name;
            (*window)->style = STDGNAME_funcs.style;
            (*window)->setString(*window, name, -1);
        } else {
            (*window)->setString(*window, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMING)), STDGNAME_menuWindows[index].text);
        }
        (*window)->setPalette(*window, PALETTE_WHITE);
    } else if (*window != NULL) {
        (*window)->setVisible(*window, 0);
    }
}

/* Draws the partner menu, its parts opening by scaling: its three sprites (the
   one at the chosen partner in a cycling palette), each partner's animation in
   its slot and the slots' frames over a glow in a cycling palette */
void STDGNAME_drawMenu(PartnerMenuTask *task, TextWindow **windows) {
    SpriteDrawer sprite;
    s32 i;
    s32 value;
    s32 partner;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layer, 2);
    sprite.setTexture(0x280, 0);
    for (i = 0; i < 3; i++) {
        if (STDGNAME_menuSprites[i].sprite == -1) {
            break;
        }
        value = task->tweens[i].level;
        if (value != 0x1000) {
            if (STDGNAME_menuSprites[i].vertical) {
                sprite.setScale(ONE, value, ONE);
            } else {
                sprite.setScale(value, ONE, ONE);
            }
            if (STDGNAME_menuSprites[i].sprite == 0x20) {
                sprite.setPivot(STDGNAME_menuSprites[i].pivotX, STDGNAME_menuSprites[i].pivotY + STDGNAME_funcs.partner * 43);
            } else {
                sprite.setPivot(STDGNAME_menuSprites[i].pivotX, STDGNAME_menuSprites[i].pivotY);
            }
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        if (STDGNAME_menuSprites[i].sprite == 0x20) {
            if ((GFX.funcs.getTime() & 3) == 3) {
                if (++task->titleClut >= 16) {
                    task->titleClut = 0;
                }
            }
            sprite.setClutRow(task->titleClut);
            sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), STDGNAME_menuSprites[i].sprite, STDGNAME_menuSprites[i].x, STDGNAME_menuSprites[i].y + STDGNAME_funcs.partner * 43);
            sprite.setClutRow(0);
        } else {
            sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), STDGNAME_menuSprites[i].sprite, STDGNAME_menuSprites[i].x, STDGNAME_menuSprites[i].y);
        }
    }
    for (i = 0; i < task->partyCount; i++) {
        partner = GAME.funcs.getPartyPartner(i);
        if (partner >= 0 && task->tweens[i + 3].level != 0) {
            if (GFX.funcs.getTime() - task->anims[i].time >= 13) {
                task->anims[i].time = GFX.funcs.getTime();
                if (STDGNAME_menuAnims[partner][++task->anims[i].frame] == -1) {
                    task->anims[i].frame = 0;
                }
            }
            if (task->tweens[i + 3].level != ONE) {
                sprite.setScale(task->tweens[i + 3].level, ONE, ONE);
                sprite.setPivot(STDGNAME_menuSlots[0].pivotX + 2, STDGNAME_menuSlots[0].pivotY + 2);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), STDGNAME_menuAnims[partner][task->anims[i].frame],
                        STDGNAME_menuSlots[0].x + 2, STDGNAME_menuSlots[0].y + 2 + i * 43);
        }
    }
    if ((GFX.funcs.getTime() & 3) == 3) {
        if (++task->cursorClut >= 14) {
            task->cursorClut = 0;
        }
    }
    for (i = 0; i < task->partyCount; i++) {
        if (GAME.funcs.getPartyPartner(i) >= 0) {
            if (task->tweens[i + 3].level != 0) {
                if (task->tweens[i + 3].level != ONE) {
                    sprite.setScale(task->tweens[i + 3].level, ONE, ONE);
                    sprite.setPivot(STDGNAME_menuSlots[0].pivotX, STDGNAME_menuSlots[0].pivotY + i * 43);
                } else {
                    sprite.setScale(ONE, ONE, ONE);
                }
                sprite.setLayerId(task->layer, 1);
                sprite.setClutRow(task->cursorClut);
                sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), 0x1F, STDGNAME_menuSlots[0].x, STDGNAME_menuSlots[0].y + i * 43);
                sprite.setLayerId(task->layer, 2);
                sprite.setClutRow(0);
                sprite.draw(FILE_CACHE.getEntry(STDGNAME_SPRITES), 0x1E, STDGNAME_menuSlots[0].x, STDGNAME_menuSlots[0].y + i * 43);
            }
        }
    }
}

/* Up/down choose a partner; cross picks it to rename, triangle leaves the screen
   (choice -1, fading out). 1 once either is pressed */
s32 STDGNAME_runMenu(PartnerMenuTask *task, TextWindow **windows) {
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--STDGNAME_funcs.partner < 0) {
            STDGNAME_funcs.partner = task->partyCount - 1;
        }
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++STDGNAME_funcs.partner > task->partyCount - 1) {
            STDGNAME_funcs.partner = 0;
        }
        SOUND.playSound(SOUND_MENU_MOVE);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        task->screen->choice = STDGNAME_funcs.partner;
        return 1;
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->screen->choice = -1;
        task->screen->fadeOut(task->screen);
        return 1;
    }
    return 0;
}

/* The partner menu's task: opens its panels, then each partner's slot and name
   one after the other, runs the menu and closes everything once a choice is
   made */
void STDGNAME_updateMenu(PartnerMenuTask *task, TextWindow **windows) {
    s32 i;
    s32 j;
    s32 done;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        for (i = 5; i >= 0; i--) {
            task->tweens[i].duration = 10;
        }
        for (i = 0; i < 3; i++) {
            if (GAME.funcs.getPartyPartner(i) >= 0) {
                task->partyCount++;
            }
        }
        break;
    case TASK_RUN:
        done = 0;
        switch (task->substate) {
        case 0:
        default:
            STDGNAME_funcs.startFade(&task->tweens[0], 1);
            STDGNAME_funcs.startFade(&task->tweens[1], 1);
            task->substate++;
            break;
        case 1:
            done += STDGNAME_funcs.updateFade(&task->tweens[0]);
            done += STDGNAME_funcs.updateFade(&task->tweens[1]);
            if (done == 2) {
                STDGNAME_showMenuWindow(task, &windows[0], 0, 1);
                STDGNAME_showMenuWindow(task, &windows[1], 1, 1);
                STDGNAME_funcs.startFade(&task->tweens[3], 1);
                task->nextSubstate(task);
            }
            break;
        case 2:
            if (STDGNAME_funcs.updateFade(&task->tweens[task->step + 3])) {
                STDGNAME_showMenuWindow(task, &windows[task->step + 2], task->step + 2, 1);
                if (task->step < task->partyCount - 1) {
                    task->step++;
                    STDGNAME_funcs.startFade(&task->tweens[task->step + 3], 1);
                } else {
                    STDGNAME_funcs.startFade(&task->tweens[2], 1);
                    task->nextSubstate(task);
                }
            }
            break;
        case 4:
            if (STDGNAME_runMenu(task, windows)) {
                task->substate++;
                for (i = 0; i < 6; i++) {
                    STDGNAME_funcs.startFade(&task->tweens[i], 0);
                    STDGNAME_showMenuWindow(task, &windows[i], i, 0);
                }
            }
            break;
        case 3:
        case 5:
            if (STDGNAME_funcs.updateFade(&task->tweens[2])) {
                task->substate++;
            }
            break;
        case 6:
            for (j = 0; j < 6; j++) {
                done += STDGNAME_funcs.updateFade(&task->tweens[j]);
            }
            if (done == 6) {
                task->setState(task, TASK_KILL);
            }
            break;
        }
        STDGNAME_drawMenu(task, windows);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the partner menu (task) to pick the partner to rename */
PartnerMenuTask *STDGNAME_createMenu(ScreenTask *screen) {
    PartnerMenuTask *task = createTask(STDGNAME_updateMenu, sizeof(PartnerMenuTask), 10 * sizeof(TextWindow *));

    task->screen = screen;
    task->layer = SCREEN_LAYER;
    return task;
}
