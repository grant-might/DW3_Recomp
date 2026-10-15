/* The lab's main menu, which opens one of its three screens */

#include "menus/digimon_lab.h"

/* The main menu's open: fades its page back in, which then shows the party
   member's stats again (STGDGLAB_updateMenu) */
void STGDGLAB_openMenu(LabMenu *menu) {
    menu->state = TASK_DONE;
    menu->counter = 0;
    STGDGLAB_data.funcs.startFade(&menu->panels[3], 1);
}

/* The main menu's close: fades its page out and hides its stats and name, while
   a screen is open over it */
void STGDGLAB_closeMenu(LabMenu *menu) {
    LabMenuWindows *windows;
    s32 i;

    menu->state = TASK_DONE;
    menu->counter = 1;
    windows = menu->children;
    STGDGLAB_data.funcs.startFade(&menu->panels[3], 0);
    for (i = 0; i < 5; i++) {
        if (windows->statLabels[i] != NULL) {
            windows->statLabels[i]->setVisible(windows->statLabels[i], 0);
        }
        if (windows->statValues[i] != NULL) {
            windows->statValues[i]->setVisible(windows->statValues[i], 0);
        }
    }
    if (windows->name != NULL) {
        windows->name->setVisible(windows->name, 0);
    }
    for (i = 4; i >= 0; i--) {
        menu->frames[i] = 0;
    }
}

/* Shows the main menu's page: the name and five stats (STGDGLAB_menuStats) of
   the lab's party member, or empty strings and a grey entries hint without one */
void STGDGLAB_showMenuPage(LabMenu *menu, LabMenuWindows *windows) {
    PartnerTotals totals;
    LabWindowPos *pos;
    s32 member;
    s32 i;

    for (i = 0; i < 5; i++) {
        pos = &STGDGLAB_data.layout[i];
        if (windows->statLabels[i] == NULL) {
            windows->statLabels[i] = createTextWindow(menu->layer, 3, pos->x, pos->y);
            windows->statLabels[i]->setDepth(windows->statLabels[i], menu->depth - 1);
        }
        windows->statLabels[i]->setString(windows->statLabels[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), pos->string);
    }
    member = GAME.funcs.getPartyMember(menu->lab->member);
    if (member >= 0) {
        GAME.funcs.computeStats(member, &totals);
    }
    for (i = 0; i < 5; i++) {
        pos = &STGDGLAB_data.layout[i + 5];
        if (windows->statValues[i] == NULL) {
            windows->statValues[i] = createTextWindow(menu->layer, 3, pos->x, pos->y);
            windows->statValues[i]->setDepth(windows->statValues[i], menu->depth - 1);
        }
        if (member < 0) {
            if (i == 0) {
                windows->statValues[0]->setString(windows->statValues[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1C);
            } else {
                windows->statValues[i]->setString(windows->statValues[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x12);
            }
        } else {
            windows->statValues[i]->setNumber(windows->statValues[i], 0, totals.stats[STGDGLAB_menuStats[i]]);
        }
        windows->statValues[i]->setRightAlign(windows->statValues[i], 1);
    }
    pos = &STGDGLAB_data.layout[10];
    if (windows->name == NULL) {
        windows->name = createTextWindow(menu->layer, 1, pos->x, pos->y);
        windows->name->setDepth(windows->name, menu->depth - 1);
    }
    if (member < 0) {
        windows->name->setString(windows->name, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1B);
        if (windows->entriesHint != NULL) {
            windows->entriesHint->setPalette(windows->entriesHint, PALETTE_GREY);
        }
    } else {
        windows->name->setString(windows->name, GAME.funcs.getPartnerStats(member)->name, -1);
        if (windows->entriesHint != NULL) {
            windows->entriesHint->setPalette(windows->entriesHint, PALETTE_WHITE);
        }
    }
    menu->frames[3] = 0;
}

/* The menu's title and button hint, each once its panel has faded in; then
   fades the options' panel in */
static inline void showMenuTitle(LabMenu *menu, LabMenuWindows *windows) {
    s32 done;

    done = 0;
    if (STGDGLAB_data.funcs.updateFade(&menu->panels[0])) {
        if (windows->title == NULL) {
            windows->title = createTextWindow(menu->layer, 1, 0xAE, 0x15);
            windows->title->setDepth(windows->title, menu->depth - 1);
        }
        windows->title->setPos(windows->title, 0xAE, 0x15);
        done = 1;
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 1);
    }
    if (STGDGLAB_data.funcs.updateFade(&menu->panels[2])) {
        if (windows->label == NULL) {
            windows->label = createTextWindow(menu->layer, 1, 0xD3, 0xCC);
            windows->label->setDepth(windows->label, menu->depth - 1);
        }
        windows->label->setPos(windows->label, 0xD3, 0xCC);
        done++;
        windows->label->setString(windows->label, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 5);
    }
    if (done == 2) {
        STGDGLAB_data.funcs.startFade(&menu->panels[1], 1);
        menu->substate++;
    }
}

/* Once their panel has faded in, the three screens' names and the cursor */
static inline void showMenuOptions(LabMenu *menu, LabMenuWindows *windows) {
    s32 i;

    if (STGDGLAB_data.funcs.updateFade(&menu->panels[1])) {
        for (i = 0; i < 3; i++) {
            if (windows->options[i] == NULL) {
                windows->options[i] = createTextWindow(menu->layer, 1, 0xA7, i * 0xE + 0x31);
                windows->options[i]->setDepth(windows->options[i], menu->depth - 1);
            }
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), i + 2);
        }
        if (windows->cursor == NULL) {
            windows->cursor = createCursor(menu->layer, 1, 0x9A, menu->choice * 0xE + 0x31);
        }
        windows->cursor->setVisible(windows->cursor, 1);
        menu->substate++;
    }
}

/* Up and down pick a screen; cross goes on to the party, triangle leaves the lab */
static inline void pickMenuOption(LabMenu *menu, LabMenuWindows *windows) {
    s32 old;

    old = menu->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--menu->choice < 0) {
            menu->choice = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++menu->choice >= 3) {
            menu->choice = 2;
        }
    }
    if (old != menu->choice) {
        SOUND.playSound(SOUND_CURSOR);
        windows->cursor->setPos(windows->cursor, 0x9A, menu->choice * 0xE + 0x31);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        menu->step = 0;
        menu->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        menu->step = 1;
        menu->substate++;
        menu->lab->fadeOut(menu->lab);
    }
}

/* Left and right go through the party's members; cross opens the screen for
   the member, triangle goes back to the screens, circle (on "Switch
   Digimon", with a member) opens the member's entries */
static inline void pickMember(LabMenu *menu, LabMenuWindows *windows) {
    s32 old;

    old = menu->lab->member;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        if (--menu->lab->member < 0) {
            menu->lab->member = menu->memberCount - 1;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        if (++menu->lab->member > menu->memberCount - 1) {
            menu->lab->member = 0;
        }
    }
    if (old != menu->lab->member) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STGDGLAB_showMenuPage(menu, windows);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        menu->step = 1;
        menu->substate++;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        menu->step = 0;
        menu->substate++;
    } else if (PAD_PRESSED(PAD_CIRCLE) && menu->choice == 0 &&
               GAME.funcs.getPartyMember(menu->lab->member) >= 0) {
        menu->step = 2;
        menu->substate++;
        SOUND.playSound(SOUND_MENU_CONFIRM);
    }
}

/* Once the member panel has faded out, fades the page out and hides its
   windows (the stats and name only when going back to the screens) */
static inline void closeMenuPage(LabMenu *menu, LabMenuWindows *windows) {
    s32 i;

    if (STGDGLAB_data.funcs.updateFade(&menu->panels[7])) {
        if (menu->step == 0) {
            STGDGLAB_data.funcs.startFade(&menu->panels[3], 0);
            for (i = 0; i < 5; i++) {
                if (windows->statLabels[i] != NULL) {
                    windows->statLabels[i]->setVisible(windows->statLabels[i], 0);
                }
                if (windows->statValues[i] != NULL) {
                    windows->statValues[i]->setVisible(windows->statValues[i], 0);
                }
            }
            if (windows->name != NULL) {
                windows->name->setVisible(windows->name, 0);
            }
        }
        STGDGLAB_data.funcs.startFade(&menu->panels[4], 0);
        STGDGLAB_data.funcs.startFade(&menu->panels[5], 0);
        STGDGLAB_data.funcs.startFade(&menu->panels[6], 0);
        if (menu->choice == 0) {
            STGDGLAB_data.funcs.startFade(&menu->panels[8], 0);
        }
        if (windows->title != NULL) {
            windows->title->setVisible(windows->title, 0);
        }
        if (windows->label != NULL) {
            windows->label->setVisible(windows->label, 0);
        }
        if (windows->entriesHint != NULL) {
            windows->entriesHint->setVisible(windows->entriesHint, 0);
        }
        menu->substate++;
    }
}

/* Once the page's panel has faded in, the member's page and its label */
static inline void showMenuPageLabel(LabMenu *menu, LabMenuWindows *windows) {
    STGDGLAB_data.funcs.updateFade(&menu->panels[3]);
    if (STGDGLAB_data.funcs.updateFade(&menu->panels[4])) {
        STGDGLAB_showMenuPage(menu, windows);
        if (windows->label == NULL) {
            windows->label = createTextWindow(menu->layer, 1, 0xA1, 0x17);
            windows->label->setDepth(windows->label, menu->depth - 1);
        }
        windows->label->setPos(windows->label, 0xA1, 0x17);
        windows->label->setString(windows->label, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 6);
        menu->substate++;
    }
}

/* Once their panels have faded in, the entries hint (on "Switch Digimon")
   and the title that asks for a member for the picked screen */
static inline void showMenuPageTitle(LabMenu *menu, LabMenuWindows *windows) {
    if (menu->choice == 0 && STGDGLAB_data.funcs.updateFade(&menu->panels[8])) {
        if (windows->entriesHint == NULL) {
            windows->entriesHint = createTextWindow(menu->layer, 1, 0xF, 0x55);
            windows->entriesHint->setDepth(windows->entriesHint, menu->depth - 1);
        }
        windows->entriesHint->setString(windows->entriesHint, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x14);
    }
    STGDGLAB_data.funcs.updateFade(&menu->panels[5]);
    if (STGDGLAB_data.funcs.updateFade(&menu->panels[6])) {
        if (windows->title == NULL) {
            windows->title = createTextWindow(menu->layer, 1, 0xAE, 0x49);
        }
        windows->title->setPos(windows->title, 0xAE, 0x49);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), menu->choice + 0x18);
        menu->substate++;
    }
}

/* Once the page's panels have faded out (all but the page itself when a
   screen or the entries open): opens the screen, the member's entries or the
   screens' menu again */
static inline void waitMenuPageClosed(LabMenu *menu) {
    s32 closed;
    s32 m;

    closed = 0;
    for (m = menu->step != 0; m < 6; m++) {
        closed += STGDGLAB_data.funcs.updateFade(&menu->panels[m + 3]);
    }
    if (menu->step == 1) {
        if (closed == 5) {
            menu->nextSubstate(menu);
            menu->picked = 1;
        }
    } else if (menu->step == 2) {
        if (closed == 5) {
            menu->setSubstate(menu, 30);
        }
    } else if (closed == 6) {
        menu->setSubstate(menu, 0);
    }
}

/* The main menu's states: fades its panels in, picks one of the three
   screens with up and down, then shows the party's stats while left and
   right go through them; circle opens the partner's entries */
void STGDGLAB_runMenu(LabMenu *menu, LabMenuWindows *windows) {
    TextWindow **items;
    /* The match depends on cases 4 and 5 having their own counters, and on
       case 5 clearing its count after k */
    s32 faded;
    s32 j;
    s32 k;

    switch (menu->substate) {
    case 0:
    default:
        STGDGLAB_data.funcs.startFade(&menu->panels[0], 1);
        STGDGLAB_data.funcs.startFade(&menu->panels[2], 1);
        menu->substate++;
        break;
    case 1:
        showMenuTitle(menu, windows);
        break;
    case 2:
        showMenuOptions(menu, windows);
        break;
    case 3:
        pickMenuOption(menu, windows);
        break;
    case 4:
        for (j = 0, items = menu->children; j < menu->childCount - 2; j++, items++) {
            if (*items != NULL) {
                (*items)->setVisible(*items, 0);
            }
        }
        windows->cursor->setVisible(windows->cursor, 0);
        STGDGLAB_data.funcs.startFade(&menu->panels[0], 0);
        STGDGLAB_data.funcs.startFade(&menu->panels[1], 0);
        STGDGLAB_data.funcs.startFade(&menu->panels[2], 0);
        menu->substate++;
        break;
    case 5:
        for (k = 0, faded = 0; k < 3; k++) {
            faded += STGDGLAB_data.funcs.updateFade(&menu->panels[k]);
            if (faded == 3) {
                if (menu->step == 0) {
                    menu->setSubstate(menu, 10);
                    STGDGLAB_data.funcs.startFade(&menu->panels[3], 1);
                    menu->lab->member = 0;
                    if (menu->choice == 0) {
                        menu->memberCount = menu->lab->partySize;
                    } else {
                        menu->memberCount = menu->lab->partyCount;
                    }
                } else {
                    menu->setState(menu, TASK_KILL);
                }
            }
        }
        break;
    case 10:
        STGDGLAB_data.funcs.startFade(&menu->panels[4], 1);
        STGDGLAB_data.funcs.startFade(&menu->panels[5], 1);
        STGDGLAB_data.funcs.startFade(&menu->panels[6], 1);
        STGDGLAB_data.funcs.startFade(&menu->panels[7], 1);
        if (menu->choice == 0) {
            STGDGLAB_data.funcs.startFade(&menu->panels[8], 1);
        }
        menu->substate++;
        break;
    case 11:
        showMenuPageLabel(menu, windows);
        break;
    case 12:
        showMenuPageTitle(menu, windows);
        break;
    case 14:
        if (STGDGLAB_data.funcs.updateFade(&menu->panels[7])) {
            menu->substate++;
        }
        break;
    case 15:
        pickMember(menu, windows);
        break;
    case 16:
        STGDGLAB_data.funcs.startFade(&menu->panels[7], 0);
        menu->substate++;
        break;
    case 17:
        closeMenuPage(menu, windows);
        break;
    case 18:
        waitMenuPageClosed(menu);
        break;
    case 19:
        if (menu->picked == 0) {
            menu->setSubstate(menu, 10);
            if (menu->panels[3].level == 0) {
                STGDGLAB_data.funcs.startFade(&menu->panels[3], 1);
            }
        }
        break;
    case 30:
        if (windows->panel == NULL) {
            windows->panel = STGDGLAB_createEntryList(GAME.funcs.getPartyMember(menu->lab->member), 1, 1);
        }
        menu->substate++;
        break;
    case 31:
        if (windows->panel == NULL) {
            menu->setSubstate(menu, 10);
        }
        break;
    }
}

/* Draws the main menu's frames and the party's animations */
void STGDGLAB_drawMenu(LabMenu *menu, void *children) {
    SpriteDrawer sprite;
    PartnerAnim *anim;
    s32 member;
    s32 id;
    s32 i;

    initSpriteDrawer(&sprite);
    if (menu->substate < 10) {
        sprite.setTexture(0x280, 0x100);
        sprite.setLayerId(menu->layer, menu->depth);
        if (menu->panels[0].level != 0) {
            if (menu->panels[0].level != ONE) {
                sprite.setScale(menu->panels[0].level, ONE, ONE);
                sprite.setPivot(0x140, 0x15);
            }
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x20, 0x92, 0xF);
        }
        if (menu->panels[1].level != 0) {
            if (menu->panels[1].level != ONE) {
                sprite.setScale(menu->panels[1].level, ONE, ONE);
                sprite.setPivot(0x140, 0x45);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x21, 0x92, 0x2A);
        }
        if (menu->panels[2].level != 0) {
            if (menu->panels[2].level != ONE) {
                sprite.setScale(menu->panels[2].level, ONE, ONE);
                sprite.setPivot(0x140, 0xD2);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x24, 0xC6, 0xC4);
        }
        return;
    }
    member = GAME.funcs.getPartyPartner(menu->lab->member);
    if (GFX.funcs.getTime() - menu->animTime >= 12) {
        menu->animTime = GFX.funcs.getTime();
        if (member >= 0) {
            anim = &STGDGLAB_data.anim[member];
            menu->frames[3]++;
            if (anim->frames[menu->frames[3]] == -1 || menu->frames[3] >= 7) {
                menu->frames[3] = 0;
            }
        }
        for (i = 0; i < menu->lab->partySize; i++) {
            id = GAME.funcs.getPartyPartner(i);
            if (id >= 0) {
                anim = &STGDGLAB_data.anim[id];
                menu->frames[i]++;
                if (anim->frames[menu->frames[i]] == -1) {
                    menu->frames[i] = 0;
                }
            }
        }
    }
    if (GFX.funcs.getTime() - menu->blinkTime >= 10) {
        menu->blinkTime = GFX.funcs.getTime();
        menu->frames[4]++;
        if (menu->frames[4] >= 4) {
            menu->frames[4] = 0;
        }
    }
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(menu->layer, menu->depth);
    if (menu->panels[7].level != 0) {
        if (menu->panels[7].level != ONE) {
            sprite.setScale(menu->panels[7].level, ONE, ONE);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            sprite.setTexture(0x140, 0);
            sprite.setLayerId(menu->layer, menu->depth - 1);
            sprite.setClutRow(menu->frames[4]);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xD, menu->lab->member * 0x30 + 0xA5, 0x65);
            sprite.setLayerId(menu->layer, menu->depth);
            sprite.setClutRow(0);
        }
        sprite.setTexture(0x280, 0x100);
        for (i = 0; i < menu->lab->partySize; i++) {
            id = GAME.funcs.getPartyPartner(i);
            if (id >= 0) {
                if (menu->panels[7].level != ONE) {
                    sprite.setPivot(i * 0x30 + 0xB4, 0x8A);
                }
                anim = &STGDGLAB_data.anim[id];
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), anim->frames[menu->frames[i]], i * 0x30 + 0xA5, 0x81);
            }
        }
    }
    if (menu->panels[6].level != 0) {
        if (menu->panels[6].level != ONE) {
            sprite.setScale(menu->panels[6].level, ONE, ONE);
            sprite.setPivot(0x140, 0x8A);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x140, 0);
        sprite.setLayerId(menu->layer, menu->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x10, 0x90, 0x5F);
        sprite.setTexture(0x140, 0);
        sprite.setLayerId(menu->layer, menu->depth);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x11, 0x90, 0x5F);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x27, 0x90, 0x5F);
    }
    if (menu->panels[3].level != 0) {
        if (menu->panels[3].level != ONE) {
            sprite.setScale(menu->panels[3].level, ONE, ONE);
            sprite.setPivot(0, 0x2F);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        if (member >= 0) {
            anim = &STGDGLAB_data.anim[member];
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), anim->frames[menu->frames[3]], 0x10, 0x16);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xE, 0, 0x13);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x1E, 0, 0x13);
    }
    sprite.setTexture(0x280, 0x100);
    if (menu->panels[4].level != 0) {
        if (menu->panels[4].level != ONE) {
            sprite.setScale(menu->panels[4].level, ONE, ONE);
            sprite.setPivot(0x140, 0x1D);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x25, 0x8F, 0xF);
    }
    if (menu->panels[5].level != 0) {
        if (menu->panels[5].level != ONE) {
            sprite.setScale(menu->panels[5].level, ONE, ONE);
            sprite.setPivot(0x140, 0x4E);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x20, 0x92, 0x43);
    }
    if (menu->panels[8].level != 0) {
        if (menu->panels[8].level != ONE) {
            sprite.setScale(menu->panels[8].level, ONE, ONE);
            sprite.setPivot(0, 0x5A);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x26, 0, 0x4D);
    }
}

/* The main menu's task: runs and draws the menu; while its page fades
   (TASK_DONE, from STGDGLAB_openMenu and STGDGLAB_closeMenu) it only draws */
void STGDGLAB_updateMenu(LabMenu *menu, void *children) {
    switch (menu->state) {
    case TASK_INIT:
    default:
        menu->nextState(menu);
        menu->panels[0].duration = 10;
        menu->panels[1].duration = 10;
        menu->panels[2].duration = 8;
        menu->panels[3].duration = 10;
        menu->panels[4].duration = 10;
        menu->panels[5].duration = 10;
        menu->panels[6].duration = 10;
        menu->panels[7].duration = 8;
        menu->panels[8].duration = 8;
        break;
    case TASK_RUN:
        STGDGLAB_runMenu(menu, children);
        STGDGLAB_drawMenu(menu, children);
        break;
    case TASK_DONE:
        if (STGDGLAB_data.funcs.updateFade(&menu->panels[3]) != 0) {
            menu->state = TASK_RUN;
            if (menu->counter == 0) {
                STGDGLAB_showMenuPage(menu, children);
            }
        }
        STGDGLAB_drawMenu(menu, children);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the lab's main menu (task) */
LabMenu *STGDGLAB_createMenu(Lab *lab) {
    LabMenu *menu = createTask(STGDGLAB_updateMenu, sizeof(LabMenu), sizeof(LabMenuWindows));

    menu->open = STGDGLAB_openMenu;
    menu->close = STGDGLAB_closeMenu;
    menu->layer = SCREEN_LAYER;
    menu->depth = 2;
    menu->lab = lab;
    return menu;
}

/* The stats the menu's page shows, in PartnerTotals.stats */
s32 STGDGLAB_menuStats[] = {
    0, 2, 3, 4,
    5,
};
