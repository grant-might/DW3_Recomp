/* The main menu's first screen, where a partner joins the party */

#include "menus/digimon_lab.h"

/* Shows the first screen's page: the name and five stats (STGDGLAB_pageStats)
   of the picked partner, or empty strings and a grey entries hint without one;
   lower by pageRow */
void STGDGLAB_showPartyPage(LabPartyScreen *screen, LabPartyScreenWindows *windows) {
    PartnerTotals totals;
    LabWindowPos *pos;
    s32 member;
    s32 i;

    for (i = 0; i < 5; i++) {
        pos = &STGDGLAB_data.layout[i];
        if (windows->labels[i] == NULL) {
            windows->labels[i] = createTextWindow(screen->layer, 3, pos->x, pos->y);
            windows->labels[i]->setDepth(windows->labels[i], screen->depth - 1);
        }
        windows->labels[i]->setString(windows->labels[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), pos->string);
        windows->labels[i]->setPos(windows->labels[i], pos->x, pos->y + screen->pageRow * 0x7A);
    }
    member = screen->partners[screen->pick];
    if (member >= 0) {
        GAME.funcs.computeStats(member, &totals);
    }
    for (i = 0; i < 5; i++) {
        pos = &STGDGLAB_data.layout[i + 5];
        if (windows->values[i] == NULL) {
            windows->values[i] = createTextWindow(screen->layer, 3, pos->x, pos->y);
            windows->values[i]->setDepth(windows->values[i], screen->depth - 1);
        }
        windows->values[i]->setPos(windows->values[i], pos->x, pos->y + screen->pageRow * 0x7A);
        if (member < 0) {
            if (i == 0) {
                windows->values[0]->setString(windows->values[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1C);
            } else {
                windows->values[i]->setString(windows->values[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x12);
            }
        } else {
            windows->values[i]->setNumber(windows->values[i], 0, totals.stats[STGDGLAB_pageStats[i]]);
        }
        windows->values[i]->setRightAlign(windows->values[i], 1);
    }
    pos = &STGDGLAB_data.layout[10];
    if (windows->name == NULL) {
        windows->name = createTextWindow(screen->layer, 1, pos->x, pos->y);
        windows->name->setDepth(windows->name, screen->depth - 1);
    }
    if (member < 0) {
        windows->name->setString(windows->name, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1B);
        windows->entriesHint->setPalette(windows->entriesHint, PALETTE_GREY);
    } else {
        windows->name->setString(windows->name, GAME.funcs.getPartnerStats(member)->name, -1);
        windows->entriesHint->setPalette(windows->entriesHint, PALETTE_WHITE);
    }
    windows->name->setPos(windows->name, pos->x, pos->y + screen->pageRow * 0x7A);
    screen->frame = 0;
}

/* Hides the first screen's text windows: all its children but the last, the
   entry list */
void STGDGLAB_hideWindows(LabPartyScreen *screen) {
    s32 i;
    TextWindow **windows = screen->children;

    for (i = 0; i < screen->childCount - 1; i++, windows++) {
        TextWindow *w = *windows;
        if (w != NULL) {
            w->setVisible(w, 0);
        }
    }
}

/* Draws the first screen's frames and the party's animations */
void STGDGLAB_drawPartyScreen(LabPartyScreen *screen, void *children) {
    SpriteDrawer sprite;
    PartnerAnim *anim;
    s32 member;
    s32 id;
    s32 i;

    member = screen->partners[screen->pick];
    if (GFX.funcs.getTime() - screen->animTime >= 12) {
        screen->animTime = GFX.funcs.getTime();
        if (member >= 0) {
            anim = &STGDGLAB_data.anim[member];
            screen->frame++;
            if (anim->frames[screen->frame] == -1 || screen->frame >= 7) {
                screen->frame = 0;
            }
        }
        for (i = 0; i < screen->lab->partySize; i++) {
            id = GAME.funcs.getPartyPartner(i);
            if (id >= 0) {
                screen->frames[i]++;
                anim = &STGDGLAB_data.anim[id];
                if (anim->frames[screen->frames[i]] == -1) {
                    screen->frames[i] = 0;
                }
            }
        }
    }
    if (GFX.funcs.getTime() - screen->blinkTime >= 10) {
        screen->blinkTime = GFX.funcs.getTime();
        screen->clut++;
        if (screen->clut >= 4) {
            screen->clut = 0;
        }
        screen->clut2++;
        if (screen->clut2 >= 6) {
            screen->clut2 = 0;
        }
    }
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    sprite.setLayerId(screen->layer, screen->depth);
    if (screen->panels[0].level != 0) {
        if (screen->panels[0].level != ONE) {
            sprite.setScale(screen->panels[0].level, ONE, ONE);
            sprite.setPivot(0, screen->pageRow * 0x7A + 0x2F);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        if (member >= 0) {
            anim = &STGDGLAB_data.anim[member];
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), anim->frames[screen->frame], 0x10,
                        screen->pageRow * 0x7A + 0x16);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xE, 0, screen->pageRow * 0x7A + 0x13);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x1E, 0, screen->pageRow * 0x7A + 0x13);
    }
    if (screen->panels[1].level != 0) {
        if (screen->panels[1].level != ONE) {
            sprite.setScale(screen->panels[1].level, ONE, ONE);
            sprite.setPivot(0, 0xD1);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x26, 0, 0xC4);
    }
    if (screen->panels[2].level != 0) {
        if (screen->panels[2].level != ONE) {
            sprite.setScale(screen->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x15);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x20, 0x92, 0xF);
    }
    if (screen->panels[4].level != 0) {
        if (screen->panels[4].level != ONE) {
            sprite.setScale(screen->panels[4].level, ONE, ONE);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            if (screen->count >= 2) {
                sprite.setLayerId(screen->layer, screen->depth - 1);
                sprite.setClutRow(screen->clut);
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x31, 0xB8, 0xB6);
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x32, 0x100, 0xB6);
                sprite.setLayerId(screen->layer, screen->depth);
                sprite.setClutRow(0);
            }
        }
        for (i = 0; i < screen->lab->partySize; i++) {
            id = GAME.funcs.getPartyPartner(i);
            if (id >= 0) {
                if (screen->panels[4].level != ONE) {
                    sprite.setPivot(i * 0x30 + 0xB4, 0x4E);
                }
                anim = &STGDGLAB_data.anim[id];
                sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), anim->frames[screen->frames[i]], i * 0x30 + 0xA5, 0x47);
            }
        }
        if (member >= 0) {
            if (screen->panels[4].level != ONE) {
                sprite.setPivot(0xE4, 0xB5);
            }
            anim = &STGDGLAB_data.anim[member];
            sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), anim->frames[screen->frame], 0xD5, 0xAE);
        }
    }
    if (screen->panels[3].level != 0) {
        if (screen->panels[3].level != ONE) {
            sprite.setScale(screen->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x85);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.setLayerId(screen->layer, screen->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x33, 0xAD, 0x66);
        sprite.setClutRow(screen->clut);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), screen->lab->member + 0x38, 0xA5, 0x41);
        sprite.setClutRow(0);
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x10, 0x90, 0x25);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xB, 0xD5, 0x92);
        sprite.setLayerId(screen->layer, screen->depth);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x11, 0x90, 0x25);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xC, 0xD5, 0x92);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x2D, 0x90, 0x2A);
        if (screen->panels[3].level != ONE) {
            sprite.setPivot(0x48, 0x69);
        }
        sprite.setClutRow(screen->clut2);
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x3B, 0x33, 0x4A);
    }
}

/* Once the first panels have faded in, fades in the page's and the party's
   panels and shows the title and the entries hint (grey without partners) */
static inline void showPartyTitle(LabPartyScreen *screen, LabPartyScreenWindows *windows) {
    STGDGLAB_data.funcs.updateFade(&screen->panels[1]);
    if (STGDGLAB_data.funcs.updateFade(&screen->panels[2])) {
        STGDGLAB_data.funcs.startFade(&screen->panels[0], 1);
        STGDGLAB_data.funcs.startFade(&screen->panels[3], 1);
        if (windows->title == NULL) {
            windows->title = createTextWindow(screen->layer, 1, 0xAE, 0x15);
        }
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x1D);
        if (windows->entriesHint == NULL) {
            windows->entriesHint = createTextWindow(screen->layer, 1, 0xF, 0xCC);
        }
        windows->entriesHint->setString(windows->entriesHint, FILE_CACHE.load(TEXT_FILE(TEXT_DIGI_LAB)), 0x14);
        if (screen->count == 0) {
            windows->entriesHint->setPalette(windows->entriesHint, PALETTE_GREY);
        } else {
            windows->entriesHint->setPalette(windows->entriesHint, PALETTE_WHITE);
        }
        screen->substate++;
    }
}

/* Left and right go through the partners outside the party; circle shows the
   picked one's entries, cross swaps it with the party member (refused when
   both are empty), triangle goes back */
static inline void pickPartner(LabPartyScreen *screen, LabPartyScreenWindows *windows) {
    s32 old;

    old = screen->pick;
    if (screen->count >= 2) {
        if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
            screen->pick--;
            if (screen->pick < 0) {
                screen->pick = screen->count - 1;
            }
        } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
            screen->pick++;
            if (screen->pick > screen->count - 1) {
                screen->pick = 0;
            }
        }
    }
    if (old != screen->pick) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STGDGLAB_showPartyPage(screen, windows);
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        if (screen->partners[screen->pick] >= 0) {
            screen->step = 2;
            screen->substate++;
            SOUND.playSound(SOUND_MENU_CONFIRM);
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        if (GAME.funcs.getPartyMember(screen->lab->member) >= 0 || screen->partners[screen->pick] >= 0) {
            screen->step = 1;
            screen->substate++;
            SOUND.playSound(SOUND_MENU_CONFIRM);
        } else {
            SOUND.playSound(SOUND_MENU_CANCEL);
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->step = 0;
        screen->substate++;
    }
}

/* The first screen's states: fades its panels in, picks a partner with left
   and right, then swaps it into the party (cross) or shows its entries
   (circle) */
void STGDGLAB_runPartyScreen(LabPartyScreen *screen, LabPartyScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STGDGLAB_data.funcs.startFade(&screen->panels[1], 1);
        STGDGLAB_data.funcs.startFade(&screen->panels[2], 1);
        screen->substate++;
        break;
    case 1:
        showPartyTitle(screen, windows);
        break;
    case 2:
        STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[3])) {
            STGDGLAB_data.funcs.startFade(&screen->panels[4], 1);
            STGDGLAB_showPartyPage(screen, windows);
            screen->substate++;
        }
        break;
    case 3:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4])) {
            screen->substate++;
        }
        break;
    case 4:
        pickPartner(screen, windows);
        break;
    case 5:
        STGDGLAB_data.funcs.startFade(&screen->panels[4], 0);
        screen->substate++;
        break;
    case 6:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[4])) {
            STGDGLAB_hideWindows(screen);
            STGDGLAB_data.funcs.startFade(&screen->panels[0], 0);
            STGDGLAB_data.funcs.startFade(&screen->panels[1], 0);
            STGDGLAB_data.funcs.startFade(&screen->panels[2], 0);
            STGDGLAB_data.funcs.startFade(&screen->panels[3], 0);
            if (screen->step != 0) {
                screen->lab->closeMenu(screen->lab);
            }
            screen->substate++;
        }
        break;
    case 7:
        STGDGLAB_data.funcs.updateFade(&screen->panels[0]);
        STGDGLAB_data.funcs.updateFade(&screen->panels[1]);
        STGDGLAB_data.funcs.updateFade(&screen->panels[2]);
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[3]) && screen->lab->menuOpen(screen->lab)) {
            screen->substate++;
        }
        break;
    case 8:
        if (screen->step == 1) {
            GAME.party[screen->lab->member] = screen->partners[screen->pick];
            screen->lab->packParty(screen->lab);
        }
        if (screen->step == 2) {
            screen->pageRow = 0;
            STGDGLAB_data.funcs.startFade(&screen->panels[0], 1);
            if (windows->panel == NULL) {
                windows->panel = STGDGLAB_createEntryList(screen->partners[screen->pick], 1, 1);
            }
            screen->substate++;
        } else {
            screen->setState(screen, TASK_KILL);
        }
        break;
    case 9:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[0])) {
            STGDGLAB_showPartyPage(screen, windows);
            screen->substate++;
        }
        break;
    case 10:
        if (windows->panel->substate >= 2 && PAD_PRESSED(PAD_TRIANGLE)) {
            STGDGLAB_data.funcs.startFade(&screen->panels[0], 0);
            STGDGLAB_hideWindows(screen);
            screen->substate++;
        }
        break;
    case 11:
        if (STGDGLAB_data.funcs.updateFade(&screen->panels[0]) && windows->panel == NULL) {
            if (screen->lab->menuOpen(screen->lab)) {
                screen->lab->openMenu(screen->lab);
                screen->substate++;
            }
            screen->pageRow = 1;
            screen->setSubstate(screen, 0);
        }
        break;
    }
}

/* The first screen's task: lists the unlocked partners outside the party, plus
   an empty pick to take the member out when the party has two or more, then
   runs and draws the screen */
void STGDGLAB_updatePartyScreen(LabPartyScreen *screen, void *children) {
    s32 party[3];
    s32 i;
    s32 j;
    s32 id;
    s32 free;

    switch (screen->state) {
    case TASK_INIT:
    default:
        screen->nextState(screen);
        screen->panels[0].duration = 10;
        screen->panels[1].duration = 8;
        screen->panels[2].duration = 10;
        screen->panels[3].duration = 10;
        screen->panels[4].duration = 8;
        screen->pageRow = 1;
        for (i = 0; i < 3; i++) {
            party[i] = GAME.funcs.getPartyPartner(i);
        }
        for (i = 0; i < 8; i++) {
            id = GAME.partners[i].unlocked - 3;
            screen->partners[i] = -1;
            if (id >= 0) {
                j = 0;
                free = 1;
                for (; j < 3; j++) {
                    if (party[j] >= 0 && id == party[j]) {
                        free = 0;
                    }
                }
            } else {
                free = 0;
            }
            if (free) {
                screen->partners[screen->count++] = id;
            }
        }
        id = party[screen->lab->member];
        if (screen->lab->partyCount >= 2 && id >= 0) {
            screen->count++;
        }
        break;
    case TASK_RUN:
        STGDGLAB_runPartyScreen(screen, children);
        STGDGLAB_drawPartyScreen(screen, children);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the main menu's first screen (task), where a partner joins the party
   in the member's place */
Task *STGDGLAB_createPartyScreen(Lab *lab) {
    LabPartyScreen *screen = createTask(STGDGLAB_updatePartyScreen, sizeof(LabPartyScreen), 0x38);

    screen->layer = SCREEN_LAYER;
    screen->depth = 2;
    screen->lab = lab;
    return (Task *)screen;
}

/* The stats the first screen's page shows, in PartnerTotals.stats */
s32 STGDGLAB_pageStats[] = {
    0, 2, 3, 4,
    5,
};
