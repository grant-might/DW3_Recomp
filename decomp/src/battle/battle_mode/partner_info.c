/* FIGHTSTG's partner info, the pages of a partner's stats and techniques. */

#include "battle/battle_mode.h"

/* Draws the PartnerInfo's blinking page arrows */
void FIGHTSTG_drawPageArrows(PartnerInfo *task) {
    SpriteDrawer drawer;
    void *sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x200, 0);
    if (GFX.funcs.getTime() & 0x10) {
        drawer.draw(sheet, 0x1F, 0x18, 0xAB);
        drawer.draw(sheet, 0x20, 0x48, 0xAB);
    }
}

/* Shows the PartnerInfo's page buttons (texts 0x11 and 0x12) */
void FIGHTSTG_showPageButtons(PartnerInfo *task, TextWindow **windows) {
    void *text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));

    windows[1] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x22, 0xAB);
    windows[1]->setString(windows[1], text, 0x11);
    windows[1]->setPalette(windows[1], PALETTE_DARK_BLUE);
    windows[2] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x38, 0xAB);
    windows[2]->setString(windows[2], text, 0x12);
    windows[2]->setPalette(windows[2], PALETTE_DARK_BLUE);
}

/* Draws the frame of the stats page */
void FIGHTSTG_drawStatsFrame(PartnerInfo *task) {
    SpriteDrawer drawer;
    void *sheet;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    sheet = FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16);
    drawer.setTexture(0x140, 0);
    drawer.draw(sheet, 0x23, 0x18, 0x51);
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0x25, 0x10, 0x4A);
}

/* the stat numbers of FIGHTSTG_showStats */
StatLine FIGHTSTG_statLines[13] = {
    { 60, 82, 6 },
    { 60, 96, 7 },
    { 60, 110, 8 },
    { 60, 124, 9 },
    { 60, 138, 10 },
    { 60, 152, 11 },
    { 126, 82, 12 },
    { 126, 96, 13 },
    { 126, 110, 14 },
    { 126, 124, 15 },
    { 126, 138, 16 },
    { 126, 152, 17 },
    { 126, 166, 18 },
};

/* Creates the thirteen stat numbers of FIGHTSTG_statLines (up to 999), right
 * aligned, in palette 6 for windows 5, 6 and 9 when stats 19, 20 and 21 are
 * set. The match depends on reading the stat through a pointer sum. */
void FIGHTSTG_showStats(PartnerInfo *task, TextWindow **windows) {
    s32 i;
    s32 value;

    for (i = 0; i < 13; i++) {
        windows[i + 5] = createTextWindow(BATTLE_LAYER_MENUS, 1, FIGHTSTG_statLines[i].x, FIGHTSTG_statLines[i].y);
        value = *(task->stats + FIGHTSTG_statLines[i].stat);
        if (value >= 1000) {
            value = 999;
        }
        windows[i + 5]->setNumber(windows[i + 5], 0, value);
        windows[i + 5]->setRightAlign(windows[i + 5], 1);
    }
    if (task->stats[19]) {
        windows[5]->setPalette(windows[5], PALETTE_PURPLE);
    }
    if (task->stats[20]) {
        windows[6]->setPalette(windows[6], PALETTE_PURPLE);
    }
    if (task->stats[21]) {
        windows[9]->setPalette(windows[9], PALETTE_PURPLE);
    }
}

/* Draws the techniques page's icons and frame */
void FIGHTSTG_drawTechIcons(PartnerInfo *task) {
    SpriteDrawer drawer;
    void *sheet;
    s32 i;
    s32 tech;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    sheet = FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16);
    drawer.setTexture(0x140, 0);
    for (i = 0; i < 6; i++) {
        tech = task->techs[i] & SKILL_ID;
        if (tech != 0) {
            drawer.draw(sheet, TECHS[tech - 1].icon + 0x37, 0x18, 0x51 + i * 0xE);
        }
    }
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0x26, 0x10, 0x4A);
}

/* Shows the techniques page: the level (text 0x1A for none) and the six
   techniques' names, the partner's own in yellow and those it can pass on
   in green */
void FIGHTSTG_showTechs(PartnerInfo *task, TextWindow **windows) {
    void *text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
    s32 i;
    s32 tech;

    windows[3] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x5A, 0xA9);
    windows[3]->setString(windows[3], text, 0x19);
    windows[4] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x93, 0xA9);
    if (task->level >= 0) {
        windows[4]->setNumber(windows[4], 0, task->level);
    } else {
        windows[4]->setString(windows[4], text, 0x1A);
    }
    windows[4]->setRightAlign(windows[4], 1);
    for (i = 0; i < 6; i++) {
        windows[i + 18] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x26, 0x51 + i * 0xE);
        tech = task->techs[i];
        if (tech != 0) {
            windows[i + 18]->setString(windows[i + 18], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), tech & SKILL_ID);
            if (tech & 0x8000) {
                windows[i + 18]->setPalette(windows[i + 18], PALETTE_YELLOW);
            } else if (tech & SKILL_MARKED) {
                windows[i + 18]->setPalette(windows[i + 18], PALETTE_GREEN);
            }
        }
    }
}

/* The PartnerInfo's task: works out its page (the stats with the slot's
   Digimon's added, that Digimon's techniques, or those the partner's other
   Digimon can pass on) and makes its windows, then draws the page each frame */
void FIGHTSTG_updatePartnerInfo(PartnerInfo *task, TextWindow **windows) {
    s32 list[10];
    s32 partner;
    DigimonData *data;
    s32 count;
    s32 tech;
    s32 n;
    s32 k;
    s32 m;
    s32 i;
    s32 j;

    switch (task->state) {
    case 0:
    default:
        partner = task->partner;
        switch (task->page) {
        case 0:
            GAME.funcs.computeStats(partner, (PartnerTotals *)task->stats);
            if (task->slot != 0) {
                GAME.funcs.getPartnerSlots(partner, task->slots);
                data = GET_DIGIMON(task->slots[task->slot - 1]);
                /* the match depends on indexing from &task->stats[STAT_STRENGTH] and [STAT_RESISTS] */
                for (j = 0; j < 6; j++) {
                    (&task->stats[STAT_STRENGTH])[j] += data->battleStats[j];
                }
                for (j = 0; j < 7; j++) {
                    (&task->stats[STAT_RESISTS])[j] += data->resistances[j];
                }
            }
            break;
        case 1:
            if (task->slot == 0) {
                data = &DIGIMON_DATA[partner];
                task->techs[0] = data->skills[6] | 0x8000;
                task->level = -1;
            } else {
                GAME.funcs.getPartnerSlots(partner, task->slots);
                GAME.funcs.getPartnerEntry(partner, task->slots[task->slot - 1], &task->entries[0]);
                task->level = task->entries[0].level;
                for (j = 0, n = 0; j < 6; j++) {
                    if (task->entries[0].skills[j] != 0) {
                        task->techs[n++] = task->entries[0].skills[j];
                    }
                }
            }
            break;
        case 2:
            if (task->slot == 0) {
                task->level = -1;
            } else {
                for (k = 0; k < 10; k++) {
                    list[k] = 0;
                }
                k = 0;
                count = GAME.funcs.getPartnerSlots(partner, task->slots);
                for (i = 0; i < count; i++) {
                    if (GAME.funcs.getPartnerEntry(partner, task->slots[i], &task->entries[i]) >= 0 &&
                        i != task->slot - 1) {
                        for (j = 0; j < 6; j++) {
                            tech = task->entries[i].skills[j];
                            if (tech != 0 && (tech & SKILL_MARKED)) {
                                list[k++] = tech & SKILL_ID;
                            }
                        }
                    }
                }
                n = 0;
                for (m = 0; m < 10; m++) {
                    tech = list[m];
                    for (k = 0; k < 6; k++) {
                        if (task->techs[k] == list[m]) {
                            tech = 0;
                            break;
                        }
                    }
                    if (tech != 0) {
                        task->techs[n++] = tech;
                    }
                }
                task->level = task->entries[task->slot - 1].level;
            }
            break;
        }
        FIGHTSTG_showPageButtons(task, windows);
        switch (task->page) {
        case 0:
            FIGHTSTG_showStats(task, windows);
            break;
        case 1:
        case 2:
            FIGHTSTG_showTechs(task, windows);
            break;
        }
        task->nextState(task);
        break;
    case 1:
        FIGHTSTG_drawPageArrows(task);
        switch (task->page) {
        case 0:
            FIGHTSTG_drawStatsFrame(task);
            break;
        case 1:
        case 2:
            FIGHTSTG_drawTechIcons(task);
            break;
        }
        break;
    case 2:
        FIGHTSTG_drawPageArrows(task);
        switch (task->page) {
        case 0:
            FIGHTSTG_drawStatsFrame(task);
            break;
        case 1:
        case 2:
            FIGHTSTG_drawTechIcons(task);
            break;
        }
        task->nextState(task);
        break;
    case 3:
        break;
    }
}

/* Shows page page about the partner in slot (0 for itself) */
PartnerInfo *FIGHTSTG_createPartnerInfo(s32 partner, s32 page, s32 slot) {
    PartnerInfo *task = createTask(FIGHTSTG_updatePartnerInfo, sizeof(PartnerInfo), 24 * sizeof(TextWindow *));

    task->partner = partner;
    task->page = page;
    task->slot = slot;
    return task;
}
