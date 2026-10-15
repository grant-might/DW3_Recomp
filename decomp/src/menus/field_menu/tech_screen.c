/* The eighth object of STSTATUS.PRO (see card_screen.c), the screen of
   STSTATUS_createTechScreen: its rodata starts at 0x80082BA4 (USA). */

#include "menus/field_menu.h"

/* Fills the item list with the owned items of its option's list
   (STSTATUS_itemLists) and counts them */
void STSTATUS_fillItemList(ItemList *panel) {
    s32 i;

    if (panel->list == 0) {
        panel->count = ITEM_FUNCS.list(STSTATUS_itemLists[0], panel->bag);
        for (i = 0; i < panel->count; i++) {
            panel->items[i] = panel->bag[i];
        }
    } else {
        panel->count = ITEM_FUNCS.list(STSTATUS_itemLists[panel->list], panel->items);
    }
}

/* Lists the techniques of a party member that can be used here (0xB8-0xBC),
   each once, and returns how many */
s32 STSTATUS_listFieldTechs(TechScreen *screen, s32 member) {
    TechRow *row;
    PartnerEntry *entry;
    s16 *techs;
    s32 partner;
    s32 count;
    s32 found;
    s32 i;
    s32 j;
    s32 k;
    s32 tech;

    partner = GAME.funcs.getPartyMember(member);
    GAME.funcs.getPartnerSlots(partner, screen->rows[member].slots);
    /* count set with i, row in the loop, techs and found set with k: the
       match depends on them, which give the registers */
    for (i = 0, count = 0; i < 3; i++) {
        row = &screen->rows[member];
        if (screen->rows[member].slots[i] >= 3) {
            entry = &row->entries[i];
            GAME.funcs.getPartnerEntry(partner, screen->rows[member].slots[i], entry);
            for (j = 0; j < 6; j++) {
                tech = entry->skills[j] & SKILL_ID;
                if (tech >= 0xB8 && tech < 0xBD) {
                    techs = row->techs;
                    for (k = 0, found = 0; k < 5; k++) {
                        if (tech == techs[k]) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        screen->rows[member].techs[count++] = tech;
                        if (count >= 5) {
                            return count;
                        }
                    }
                }
            }
        }
    }
    return count;
}

/* The chosen technique, or 0 if its user hasn't the MP for it */
s32 STSTATUS_getChosenTech(TechScreen *screen) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(screen->member));
    s16 tech = screen->rows[screen->member].techs[screen->cursor];

    if (TECHS[tech - 1].mp <= stats->stats[STAT_MP]) {
        return tech;
    }
    return 0;
}

/* A technique's healing: 64 times its power, plus its power times the
   partner's wisdom over 8 */
s32 STSTATUS_getTechHealing(s32 partner, s32 tech) {
    PartnerTotals stats;
    TechData *info;
    s32 power;

    GAME.funcs.computeStats(partner, &stats);
    info = TECHS + tech - 1; /* the match depends on this pointer */
    power = info->effectPower;
    return (power << 6) + power * stats.fields.battle[3] / 8;
}

/* Uses the chosen healing technique on the target or on the whole party,
   then plays FAILSOUND and says so if no one needed it; returns whether it
   healed anyone */
s32 STSTATUS_useTech(TechScreen *screen, TechScreenWindows *windows, s32 failSound) {
    /* the user's and the target's ids: the match depends on them being in
       memory, as an array */
    s32 ids[2];
    PartnerStats *user;
    PartnerStats *target;
    s32 tech;
    s32 power;
    s32 healed;
    s32 full;
    s32 i;

    tech = screen->rows[screen->member].techs[screen->cursor];
    ids[0] = GAME.funcs.getPartyMember(screen->member);
    user = GAME.funcs.getPartnerStats(ids[0]);
    power = STSTATUS_getTechHealing(ids[0], tech);
    if (TECHS[tech - 1].kind == 3) {
        ids[1] = GAME.funcs.getPartyMember(screen->target);
        target = GAME.funcs.getPartnerStats(ids[1]);
        if (target->stats[STAT_HP] < target->stats[STAT_MAX_HP]) {
            user->stats[STAT_MP] -= TECHS[tech - 1].mp;
            target->stats[STAT_HP] += power;
            if (target->stats[STAT_HP] > target->stats[STAT_MAX_HP]) {
                target->stats[STAT_HP] = target->stats[STAT_MAX_HP];
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x52);
            } else {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x53);
                windows->help->setNumber(windows->help, 1, power);
            }
            for (i = 0; i < screen->count; i++) {
                STSTATUS_showTechPage(screen, windows, i, 1);
            }
            SOUND.playSound(SOUND_RECOVERY);
            return 1;
        }
    } else {
        i = 0;
        healed = 0;
        full = 0;
        for (; i < screen->count; i++) {
            ids[1] = GAME.funcs.getPartyMember(i);
            if (ids[1] >= 0) {
                target = GAME.funcs.getPartnerStats(ids[1]);
                if (target->stats[STAT_HP] < target->stats[STAT_MAX_HP]) {
                    target->stats[STAT_HP] += power;
                    if (target->stats[STAT_HP] > target->stats[STAT_MAX_HP]) {
                        target->stats[STAT_HP] = target->stats[STAT_MAX_HP];
                        full++;
                    }
                    /* the match depends on this coming after full++ */
                    healed = 1;
                } else {
                    full++;
                }
            }
        }
        if (healed) {
            user->stats[STAT_MP] -= TECHS[tech - 1].mp;
            for (i = 0; i < screen->count; i++) {
                STSTATUS_showTechPage(screen, windows, i, 1);
            }
            if (full == screen->count) {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x52);
            } else {
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x53);
                windows->help->setNumber(windows->help, 1, power);
            }
            SOUND.playSound(SOUND_RECOVERY);
            return 1;
        }
    }
    SOUND.playSound(failSound);
    windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x6A);
    return 0;
}

/* Creates the technique screen's windows: the title, the party's pages, the
   help, the MP cost and the list of techniques with its cursor */
void STSTATUS_createTechWindows(TechScreen *screen, TechScreenWindows *windows) {
    s32 i;
    s32 j;
    WindowPos *pos;
    WindowPos *layout;

    pos = &STSTATUS_data.layout[11];
    windows->title = createTextWindow(screen->layer, 1, pos->x, pos->y);
    for (j = 0; j < 3; j++) {
        pos = &STSTATUS_data.layout[0];
        windows->pages[j].name = createTextWindow(screen->layer, 1, pos->x, pos->y + j * 46);
        pos = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].labels[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
        pos = &STSTATUS_data.layout[6];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].values[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
    }
    layout = STSTATUS_data.layout;
    pos = &layout[12];
    windows->help = createTextWindow(screen->layer, 1, pos->x, pos->y);
    windows->help->setLines(windows->help, 2);
    windows->help2 = createTextWindow(screen->layer, 1, pos->x, pos->y + 14);
    pos = &layout[13];
    windows->mpLabel = createTextWindow(screen->layer, 1, pos->x, pos->y);
    pos = &layout[14];
    windows->mp = createTextWindow(screen->layer, 1, pos->x, pos->y);
    windows->listTitle = createTextWindow(screen->layer, 1, 0x9C, 0x31);
    for (i = 0; i < 5; i++) {
        windows->techs[i] = createTextWindow(screen->layer, 1, 0xB1, 0x41 + i * 14);
    }
    windows->cursor = createCursor(screen->layer, screen->depth - 1, 0xA6, screen->cursor * 14 + 0x41);
    windows->cursor->setVisible(windows->cursor, 0);
}

/* As STSTATUS_showCardPage */
void STSTATUS_showTechPage(TechScreen *screen, TechScreenWindows *windows, s32 member, s32 show) {
    PartnerTotals stats;
    WindowPos *layout;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(member);
        GAME.funcs.computeStats(id, &stats);
        windows->pages[member].name->setString(windows->pages[member].name, GAME.funcs.getPartnerStats(id), -1);
        layout = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, layout++) {
            windows->pages[member].labels[i]->setString(windows->pages[member].labels[i],
                                                        FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), layout->string);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats8[i]]);
            windows->pages[member].values[i]->setRightAlign(windows->pages[member].values[i], 1);
        }
    } else {
        windows->pages[member].name->setVisible(windows->pages[member].name, 0);
        for (i = 0; i < 5; i++) {
            windows->pages[member].labels[i]->setVisible(windows->pages[member].labels[i], 0);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setVisible(windows->pages[member].values[i], 0);
        }
    }
}

/* Shows or hides the list of the party member's techniques */
void STSTATUS_showTechList(TechScreen *screen, TechScreenWindows *windows, s32 show) {
    s32 i;

    if (show) {
        windows->listTitle->setString(windows->listTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 9);
        for (i = 0; i < screen->rows[screen->member].techCount; i++) {
            windows->techs[i]->setString(windows->techs[i], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)),
                                         screen->rows[screen->member].techs[i] & SKILL_ID);
        }
    } else {
        windows->listTitle->setVisible(windows->listTitle, 0);
        for (i = 0; i < screen->rows[screen->member].techCount; i++) {
            windows->techs[i]->setVisible(windows->techs[i], 0);
        }
    }
}

/* Shows or hides the chosen technique's name and MP cost */
void STSTATUS_showChosenTech(TechScreen *screen, TechScreenWindows *windows, s32 show) {
    s32 tech;

    if (show) {
        tech = screen->rows[screen->member].techs[screen->cursor] & SKILL_ID;
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), tech);
        windows->mpLabel->setString(windows->mpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 3);
        windows->mp->setNumber(windows->mp, 0, TECHS[tech - 1].mp);
        windows->mp->setRightAlign(windows->mp, 1);
    } else {
        windows->help->setVisible(windows->help, 0);
        windows->mpLabel->setVisible(windows->mpLabel, 0);
        windows->mp->setVisible(windows->mp, 0);
    }
}

/* Moves the partners' portraits to their next frames every 13 frames */
static inline void STSTATUS_animateTechPortraits(TechScreen *screen) {
    s32 id;
    s32 i;

    if (GFX.funcs.getTime() - screen->frameTime >= 13) {
        screen->frameTime = GFX.funcs.getTime();
        for (i = 0; i < screen->count; i++) {
            id = GAME.funcs.getPartyMember(i);
            screen->frames[i]++;
            if (STSTATUS_data.partnerAnims[id].frames[screen->frames[i]] == -1 ||
                screen->frames[i] >= 7) {
                screen->frames[i] = 0;
            }
        }
    }
}

/* Moves the member cursor's palette on every 9 frames */
static inline void STSTATUS_stepTechCursor(TechScreen *screen) {
    if (GFX.funcs.getTime() - screen->cursorTime >= 9) {
        screen->cursorTime = GFX.funcs.getTime();
        screen->cursorFrame++;
        if (screen->cursorFrame >= 8) {
            screen->cursorFrame = 0;
        }
    }
}

/* Draws the partners' portraits and frames, the cursors and the help arrow */
void STSTATUS_drawTechScreen(TechScreen *screen) {
    SpriteDrawer sprite;
    s32 id;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    STSTATUS_animateTechPortraits(screen);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            id = GAME.funcs.getPartyMember(i);
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16),
                        STSTATUS_data.partnerAnims[id].frames[screen->frames[i]], 0x6B,
                        i * 0x2E + 0x13);
        }
    }
    sprite.setTexture(0x140, 0);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            /* both branches draw the frame's last part: the match depends on it */
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            } else {
                sprite.setScale(ONE, ONE, ONE);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            }
        }
    }
    if (screen->fades[0].level != 0) {
        if (screen->fades[0].level != ONE) {
            sprite.setScale(screen->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x19);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
    if (screen->fades[1].level != 0) {
        if (screen->blink) {
            if (GFX.funcs.getTime() - screen->blinkTime >= 4) {
                screen->blinkTime = GFX.funcs.getTime();
                screen->blinkFrame++;
                if (screen->blinkFrame >= 5) {
                    screen->blinkFrame = 0;
                }
            }
            sprite.setTexture(0x140, 0);
            sprite.setClutRow(screen->blinkFrame);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xA, 0x123, 0xD6);
            sprite.setClutRow(0);
        }
        if (screen->fades[1].level != ONE) {
            sprite.setScale(screen->fades[1].level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    STSTATUS_stepTechCursor(screen);
    if (screen->targetShown) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth - 1);
        sprite.setTexture(0x280, 0x100);
        sprite.setClutRow(screen->cursorFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->target * 0x2E + 0x11);
    }
    if (screen->memberShown) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth - 1);
        sprite.setTexture(0x280, 0x100);
        /* the match depends on two calls, not a conditional argument */
        if (screen->substate >= 0x1E) {
            sprite.setClutRow(8);
        } else {
            sprite.setClutRow(screen->cursorFrame);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->member * 0x2E + 0x11);
    }
    if (screen->fade.level != 0) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth);
        sprite.setTexture(0x280, 0x100);
        if (screen->fade.level != ONE) {
            sprite.setScale(screen->fade.level, ONE, ONE);
            sprite.setPivot(0x140, 0x63);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), STSTATUS_techSprites[screen->rows[screen->member].techCount - 1],
                    0x94, 0x2F);
    }
}

/* Starts the fades of the first page and the title (and, with one party
   member, of the help's panel); the next substate is the party's size */
static inline void STSTATUS_startTechScreen(TechScreen *screen) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
    STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
    if (screen->count == 1) {
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
    }
    screen->substate = screen->count;
}

/* One member: once the page and the panels are in, shows the title and the
   page */
static inline void STSTATUS_openTechScreenAlone(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        STSTATUS_showTechPage(screen, windows, 0, 1);
        screen->substate = 10;
    }
}

/* Two members: once the first page and the title are in, shows them and
   starts the second page and the help's panel */
static inline void STSTATUS_openFirstOfTwoTechPages(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        STSTATUS_showTechPage(screen, windows, 0, 1);
        screen->substate = 4;
    }
}

/* Two members: once the second page and the help's panel are in, shows the
   page */
static inline void STSTATUS_openSecondOfTwoTechPages(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showTechPage(screen, windows, 1, 1);
        screen->substate = 10;
    }
}

/* Three members: once the first page and the title are in, shows them and
   starts the second page */
static inline void STSTATUS_openFirstOfThreeTechPages(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        STSTATUS_showTechPage(screen, windows, 0, 1);
        screen->substate = 5;
    }
}

/* Three members: once the second page is in, shows it and starts the third
   page and the help's panel */
static inline void STSTATUS_openSecondOfThreeTechPages(TechScreen *screen, TechScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showTechPage(screen, windows, 1, 1);
        screen->substate++;
    }
}

/* Three members: once the third page and the help's panel are in, shows the
   page */
static inline void STSTATUS_openThirdTechPage(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showTechPage(screen, windows, 2, 1);
        screen->substate = 10;
    }
}

/* Shows the member cursor and the help for choosing who uses a technique */
static inline void STSTATUS_showTechHelp(TechScreen *screen, TechScreenWindows *windows) {
    screen->memberShown = 1;
    windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x28);
    windows->help2->setString(windows->help2, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
    screen->substate++;
}

/* Choosing who uses a technique: up and down move the cursor, cross opens
   the member's techniques, triangle closes the screen */
static inline void STSTATUS_chooseTechMember(TechScreen *screen) {
    s32 member;

    member = screen->member;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->member--;
        if (screen->member < 0) {
            screen->member = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->member++;
        if (screen->member > screen->count - 1) {
            screen->member = screen->count - 1;
        }
    }
    if (member != screen->member) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->substate = 15;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->substate = 0x32;
    }
}

/* Starts opening the member's technique list, or says they have none */
static inline void STSTATUS_openTechList(TechScreen *screen, TechScreenWindows *windows) {
    if (screen->rows[screen->member].techCount != 0) {
        screen->fade.duration = 8;
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2A);
        windows->help->setVisible(windows->help, 0);
        windows->help2->setVisible(windows->help2, 0);
        screen->substate = 0x1E;
    } else {
        screen->substate = 0x10;
    }
}

/* Hides the member cursor and says the member has no technique to use here */
static inline void STSTATUS_sayNoFieldTechs(TechScreen *screen, TechScreenWindows *windows) {
    screen->memberShown = 0;
    windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x29);
    windows->help2->setVisible(windows->help2, 0);
    screen->blink = 1;
    screen->substate++;
}

/* Cross takes the message away and goes back to choosing a member */
static inline void STSTATUS_closeNoFieldTechs(TechScreen *screen, TechScreenWindows *windows) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->memberShown = 1;
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x28);
        windows->help2->setVisible(windows->help2, 1);
        screen->blink = 0;
        screen->substate = 11;
    }
}

/* Once the list's panel is in, shows the techniques and the cursor on the
   first one */
static inline void STSTATUS_enterTechList(TechScreen *screen, TechScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->fade)) {
        screen->cursor = 0;
        STSTATUS_showTechList(screen, windows, 1);
        STSTATUS_showChosenTech(screen, windows, 1);
        windows->cursor->setPos(windows->cursor, 0xA6, screen->cursor * 0xE + 0x41);
        windows->cursor->setVisible(windows->cursor, 1);
        screen->substate++;
    }
}

/* Choosing a technique: up and down move the cursor, cross uses it (after
   choosing whom, for one that heals one member) or says the MP is short,
   triangle closes the list */
static inline void STSTATUS_chooseTech(TechScreen *screen, TechScreenWindows *windows) {
    s32 old;
    s32 tech;

    old = screen->cursor;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->cursor--;
        if (screen->cursor < 0) {
            screen->cursor = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->cursor++;
        if (screen->cursor > screen->rows[screen->member].techCount - 1) {
            screen->cursor = screen->rows[screen->member].techCount - 1;
        }
    }
    if (old != screen->cursor) {
        windows->cursor->setPos(windows->cursor, 0xA6, screen->cursor * 0xE + 0x41);
        STSTATUS_showChosenTech(screen, windows, 1);
        SOUND.playSound(SOUND_CURSOR);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        tech = STSTATUS_getChosenTech(screen);
        if (tech != 0) {
            if (TECHS[tech - 1].kind == 3) {
                SOUND.playSound(SOUND_SELECT);
                screen->targetShown = 1;
                windows->cursor->setPalette(windows->cursor, PALETTE_GREY);
                windows->cursor->setStill(windows->cursor, 1);
                windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2B);
                screen->substate++;
            } else {
                windows->cursor->setPalette(windows->cursor, PALETTE_GREY);
                windows->cursor->setStill(windows->cursor, 1);
                STSTATUS_showChosenTech(screen, windows, 0);
                screen->step = screen->substate;
                STSTATUS_useTech(screen, windows, SOUND_SELECT);
                screen->substate = 0x24;
                screen->blink = 1;
            }
        } else {
            SOUND.playSound(SOUND_SELECT);
            windows->cursor->setPalette(windows->cursor, PALETTE_GREY);
            windows->cursor->setStill(windows->cursor, 1);
            STSTATUS_showChosenTech(screen, windows, 0);
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x69);
            screen->substate = 0x23;
            screen->blink = 1;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        STSTATUS_data.funcs.startFade(&screen->fade, 0);
        STSTATUS_showTechList(screen, windows, 0);
        STSTATUS_showChosenTech(screen, windows, 0);
        windows->cursor->setVisible(windows->cursor, 0);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        screen->substate = 0x28;
    }
}

/* Choosing whom the technique heals: up and down move the cursor, cross uses
   it or says the MP is short, triangle goes back to the list */
static inline void STSTATUS_chooseTechTarget(TechScreen *screen, TechScreenWindows *windows) {
    s32 old;

    old = screen->target;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->target--;
        if (screen->target < 0) {
            screen->target = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->target++;
        if (screen->target > screen->count - 1) {
            screen->target = screen->count - 1;
        }
    }
    if (old != screen->target) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        STSTATUS_showChosenTech(screen, windows, 0);
        if (STSTATUS_getChosenTech(screen) != 0) {
            screen->targetShown = 0;
            screen->step = screen->substate;
            STSTATUS_useTech(screen, windows, SOUND_MENU_CONFIRM);
            screen->substate = 0x24;
        } else {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            screen->memberShown = 0;
            screen->targetShown = 0;
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x69);
            screen->substate = 0x23;
        }
        screen->blink = 1;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->substate = 0x1F;
        screen->targetShown = 0;
        windows->cursor->setPalette(windows->cursor, PALETTE_WHITE);
        windows->cursor->setStill(windows->cursor, 0);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x2A);
    }
}

/* Cross takes the short MP message away and goes back to the list */
static inline void STSTATUS_closeShortMp(TechScreen *screen, TechScreenWindows *windows) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        STSTATUS_showChosenTech(screen, windows, 1);
        windows->cursor->setPalette(windows->cursor, PALETTE_WHITE);
        windows->cursor->setStill(windows->cursor, 0);
        screen->substate = 0x1F;
        screen->blink = 0;
    }
}

/* Cross takes the technique's result away and goes back to where it was
   chosen */
static inline void STSTATUS_closeTechUsed(TechScreen *screen, TechScreenWindows *windows) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        STSTATUS_showChosenTech(screen, windows, 1);
        screen->substate = screen->step;
        /* the match depends on testing step, not substate */
        if (screen->step == 0x1F) {
            windows->cursor->setPalette(windows->cursor, PALETTE_WHITE);
            windows->cursor->setStill(windows->cursor, 0);
        } else {
            screen->targetShown = 1;
        }
        screen->blink = 0;
    }
}

/* Once the list's panel is closed, goes back to choosing a member */
static inline void STSTATUS_closeTechList(TechScreen *screen) {
    if (STSTATUS_data.funcs.updateFade(&screen->fade)) {
        screen->setSubstate(screen, 10);
    }
}

/* Starts closing the screen: the last page, the help's panel and, with one
   member, the title */
static inline void STSTATUS_closeTechScreen(TechScreen *screen, TechScreenWindows *windows) {
    screen->memberShown = 0;
    windows->help2->setVisible(windows->help2, 0);
    STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
    STSTATUS_showChosenTech(screen, windows, 0);
    STSTATUS_data.funcs.startFade(&screen->pageFades[screen->count - 1], 0);
    STSTATUS_showTechPage(screen, windows, screen->count - 1, 0);
    if (screen->count == 1) {
        windows->title->setVisible(windows->title, 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
    }
    screen->substate = screen->count + 0x32;
}

/* One member: ends the screen once the page and the panels are closed */
static inline void STSTATUS_closeTechScreenAlone(TechScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        screen->state = 3;
    }
}

/* Two members: once the second page and the help's panel are closed, starts
   closing the first and the title */
static inline void STSTATUS_closeSecondOfTwoTechPages(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showTechPage(screen, windows, 0, 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        screen->substate = 0x36;
    }
}

/* Three members: once the third page and the help's panel are closed, starts
   closing the second */
static inline void STSTATUS_closeThirdTechPage(TechScreen *screen, TechScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showTechPage(screen, windows, 1, 0);
        screen->substate = 0x37;
    }
}

/* Three members: once the second page is closed, starts closing the first
   and the title */
static inline void STSTATUS_closeSecondOfThreeTechPages(TechScreen *screen, TechScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_showTechPage(screen, windows, 0, 0);
        screen->substate++;
    }
}

/* Two or three members: ends the screen once the first page and the title
   are closed */
static inline void STSTATUS_closeFirstTechPage(TechScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        screen->state = 3;
    }
}

/* The technique screen's update: opens the pages, picks who uses a
   technique, which one and on whom, then closes them */
void STSTATUS_runTechScreen(TechScreen *screen, TechScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_startTechScreen(screen);
        break;
    case 1:
        STSTATUS_openTechScreenAlone(screen, windows);
        break;
    case 2:
        STSTATUS_openFirstOfTwoTechPages(screen, windows);
        break;
    case 4:
        STSTATUS_openSecondOfTwoTechPages(screen, windows);
        break;
    case 3:
        STSTATUS_openFirstOfThreeTechPages(screen, windows);
        break;
    case 5:
        STSTATUS_openSecondOfThreeTechPages(screen, windows);
        break;
    case 6:
        STSTATUS_openThirdTechPage(screen, windows);
        break;
    case 10:
        STSTATUS_showTechHelp(screen, windows);
        break;
    case 11:
        STSTATUS_chooseTechMember(screen);
        break;
    case 15:
        STSTATUS_openTechList(screen, windows);
        break;
    case 0x10:
        STSTATUS_sayNoFieldTechs(screen, windows);
        break;
    case 0x11:
        STSTATUS_closeNoFieldTechs(screen, windows);
        break;
    case 0x1E:
        STSTATUS_enterTechList(screen, windows);
        break;
    case 0x1F:
        STSTATUS_chooseTech(screen, windows);
        break;
    case 0x20:
        STSTATUS_chooseTechTarget(screen, windows);
        break;
    case 0x23:
        STSTATUS_closeShortMp(screen, windows);
        break;
    case 0x24:
        STSTATUS_closeTechUsed(screen, windows);
        break;
    case 0x28:
        STSTATUS_closeTechList(screen);
        break;
    case 0x32:
        STSTATUS_closeTechScreen(screen, windows);
        break;
    case 0x33:
        STSTATUS_closeTechScreenAlone(screen);
        break;
    case 0x34:
        STSTATUS_closeSecondOfTwoTechPages(screen, windows);
        break;
    case 0x35:
        STSTATUS_closeThirdTechPage(screen, windows);
        break;
    case 0x37:
        STSTATUS_closeSecondOfThreeTechPages(screen, windows);
        break;
    case 0x36:
    case 0x38:
        STSTATUS_closeFirstTechPage(screen);
        break;
    }
}

/* The technique screen's task: counts the party, lists each member's techniques
   that can be used here and creates the windows, then runs and draws the screen */
void STSTATUS_updateTechScreen(TechScreen *screen, TechScreenWindows *windows) {
    s32 i;

    switch (screen->state) {
    case TASK_INIT:
    default:
        screen->nextState(screen);
        for (i = 0; i < 3; i++) {
            if (GAME.funcs.getPartyMember(i) >= 0) {
                screen->count++;
            }
        }
        for (i = 0; i < screen->count; i++) {
            screen->pageFades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->pageFades[i], 1);
        }
        for (i = 0; i < 2; i++) {
            screen->fades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->fades[i], 1);
        }
        for (i = 0; i < screen->count; i++) {
            screen->rows[i].techCount = STSTATUS_listFieldTechs(screen, i);
        }
        STSTATUS_createTechWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runTechScreen(screen, windows);
        STSTATUS_drawTechScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the field menu's technique screen (task) */
Task *STSTATUS_createTechScreen(FieldMenuScreen *menu, s32 extra) {
    TechScreen *screen = createTask(STSTATUS_updateTechScreen, sizeof(TechScreen), sizeof(TechScreenWindows));

    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->menu = menu;
    return (Task *)screen;
}

s32 STSTATUS_pageStats8[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
/* The sprites of the technique counts, from 1 */
s32 STSTATUS_techSprites[] = {
    57, 56, 55, 54,
    45,
};
