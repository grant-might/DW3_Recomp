/* FIGHTSTG's technique menu. */

#include "battle/battle_mode.h"

/* Draws the page's technique icons, the page arrows (blinking) and the frame */
void FIGHTSTG_drawTechMenu(TechMenu *task) {
    SpriteDrawer drawer;
    void *sheet;
    s32 index;
    s32 tech;
    s32 i;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x140, 0);
    sheet = FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16);
    for (i = 0; i < TECH_MENU_LINES; i++) {
        index = task->page * TECH_MENU_LINES + i;
        if (index < task->count) {
            tech = task->techs[index] & SKILL_ID;
            if (tech == 0) {
                break;
            }
            drawer.draw(sheet, TECHS[tech - 1].icon + 0x37, 0x1D, 0x45 + i * 0xE);
        }
    }
    drawer.setTexture(0x200, 0);
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    if (GFX.funcs.getTime() & 0x10) {
        if (task->page > 0) {
            drawer.draw(sheet, 0x1F, 0x10, 0x9F);
        }
        if (task->page < task->pageCount - 1) {
            drawer.draw(sheet, 0x20, 0x90, 0x9F);
        }
    }
    drawer.draw(sheet, 0x2A, 8, 0x3E);
    drawer.draw(sheet, 0x29, 0xA3, 0x21);
    drawer.draw(sheet, 0x31, 0xB, 0xBC);
}

/* Makes the technique menu's windows: the page buttons, the active fighter's
   MP over its max (or a text by its MP for a temporary Digimon), the six
   names, the description and the technique's MP cost */
void FIGHTSTG_createTechWindows(TechMenu *task, TechMenuChild *children) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    void *text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
    s32 i;

    children[1].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x1A, 0x9F);
    children[1].window->setString(children[1].window, text, 0x11);
    children[1].window->setPalette(children[1].window, PALETTE_DARK_BLUE);
    children[2].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x80, 0x9F);
    children[2].window->setString(children[2].window, text, 0x12);
    children[2].window->setPalette(children[2].window, PALETTE_DARK_BLUE);
    children[3].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xAC, 0x3A);
    children[3].window->setString(children[3].window, text, 0xD);
    children[6].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xD8, 0x3A);
    if (fighter->temporary) {
        if (fighter->mp < 100) {
            children[6].window->setString(children[6].window, text, 0x1A);
        } else if (fighter->mp < 1000) {
            children[6].window->setString(children[6].window, text, 0xF);
        } else {
            children[6].window->setString(children[6].window, text, 0x24);
        }
    } else {
        children[6].window->setNumber(children[6].window, 0, fighter->mp);
    }
    children[6].window->setRightAlign(children[6].window, 1);
    children[5].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xD9, 0x3A);
    children[5].window->setString(children[5].window, text, 0x10);
    children[4].window = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xFB, 0x3A);
    children[4].window->setNumber(children[4].window, 0, fighter->maxMp);
    children[4].window->setRightAlign(children[4].window, 1);
    for (i = 0; i < TECH_MENU_LINES; i++) {
        children[7 + i].window = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x2A, 0x45 + i * 0xE);
    }
    children[13].window = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xC2);
    children[14].window = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x100, 0xD0);
    children[15].window = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x12B, 0xD0);
}

/* Shows the page's techniques (palette 7 for those the fighter lacks the MP
   for, 3 its signature one, 4 those passed on) and the description and MP
   cost of the one under the cursor, or text 0x13 when there are none */
void FIGHTSTG_showTechPage(TechMenu *task, TechMenuChild *children) {
    BattleFighter *fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    s32 index;
    s32 tech;
    s32 mp;
    s32 i;

    if (task->count != 0) {
        for (i = 0; i < TECH_MENU_LINES; i++) {
            index = task->page * TECH_MENU_LINES + i;
            if (index > task->count - 1) {
                children[7 + i].window->setVisible(children[7 + i].window, 0);
            } else {
                tech = task->techs[index];
                children[7 + i].window->setString(children[7 + i].window, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), tech & SKILL_ID);
                mp = FIGHTSTG_battleFuncs.getTechCost(0, tech);
                if (fighter->temporary) {
                    if (tech & 0x8000) {
                        children[7 + i].window->setPalette(children[7 + i].window, PALETTE_YELLOW);
                    } else if (tech & SKILL_MARKED) {
                        children[7 + i].window->setPalette(children[7 + i].window, PALETTE_GREEN);
                    } else {
                        children[7 + i].window->setPalette(children[7 + i].window, PALETTE_WHITE);
                    }
                } else if (fighter->mp < mp) {
                    children[7 + i].window->setPalette(children[7 + i].window, PALETTE_GREY);
                } else if (tech & 0x8000) {
                    children[7 + i].window->setPalette(children[7 + i].window, PALETTE_YELLOW);
                } else if (tech & SKILL_MARKED) {
                    children[7 + i].window->setPalette(children[7 + i].window, PALETTE_GREEN);
                } else {
                    children[7 + i].window->setPalette(children[7 + i].window, PALETTE_WHITE);
                }
            }
        }
        mp = FIGHTSTG_battleFuncs.getTechCost(0, task->techs[task->page * TECH_MENU_LINES + children[0].cursor->sel]);
        tech = task->techs[task->page * TECH_MENU_LINES + children[0].cursor->sel];
        if (fighter->temporary == 0 && fighter->mp < mp) {
            children[13].window->setString(children[13].window, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x54);
            children[14].window->setString(children[14].window, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0xD);
            children[14].window->setPalette(children[14].window, PALETTE_GREY);
            children[15].window->setNumber(children[15].window, 0, mp);
            children[15].window->setRightAlign(children[15].window, 1);
            children[15].window->setPalette(children[15].window, PALETTE_GREY);
        } else {
            children[13].window->setString(children[13].window, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_INFO)), tech & SKILL_ID);
            children[14].window->setString(children[14].window, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0xD);
            children[15].window->setNumber(children[15].window, 0, mp);
            children[15].window->setRightAlign(children[15].window, 1);
            if (tech & SKILL_MARKED) {
                children[14].window->setPalette(children[14].window, PALETTE_GREEN);
                children[15].window->setPalette(children[15].window, PALETTE_GREEN);
            } else {
                children[14].window->setPalette(children[14].window, PALETTE_WHITE);
                children[15].window->setPalette(children[15].window, PALETTE_WHITE);
            }
        }
    } else {
        children[13].window->setString(children[13].window, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x13);
    }
}

/* The techniques of a Digimon of the partner's slots: its entry's, its
   signature one when it is temporary, then the ones the other entries can
   pass on. */
static inline void listSlotTechs(TechMenu *task, BattleFighter *fighter, s32 member, s32 id) {
    s32 extra[10];
    DigimonData *data;
    s32 slots;
    s32 found;
    s32 count;
    s32 tech;
    s32 i;
    s32 j;

    slots = GAME.funcs.getPartnerSlots(member, task->slots);
    task->count = 0;
    for (i = 0; i < slots; i++) {
        GAME.funcs.getPartnerEntry(member, task->slots[i], &task->entries[i]);
        if (id == task->entries[i].id) {
            for (j = 0; j < 6; j++) {
                if (task->entries[i].skills[j] != 0) {
                    task->techs[task->count++] = (s16)(task->entries[i].skills[j] & ~SKILL_MARKED);
                }
            }
        }
    }
    if (fighter->temporary != 0) {
        data = GET_DIGIMON(fighter->id);
        found = 0;
        for (j = 0; j < task->count; j++) {
            if ((task->techs[j] & SKILL_ID) == data->skills[6]) {
                task->techs[j] = (task->techs[j] & SKILL_ID) | 0x8000;
                found = -1;
                break;
            }
        }
        if (found != -1) {
            task->techs[task->count++] = data->skills[6] | 0x8000;
        }
    }
    /* the techniques the other entries can pass on */
    for (j = 9; j >= 0; j--) {
        extra[j] = 0;
    }
    count = 0;
    for (i = 0; i < slots; i++) {
        if (id != task->entries[i].id) {
            for (j = 0; j < 6; j++) {
                if (task->entries[i].skills[j] & SKILL_MARKED) {
                    extra[count++] = task->entries[i].skills[j];
                }
            }
        }
    }
    for (j = 0; j < count; j++) {
        tech = extra[j] & SKILL_ID;
        for (i = 0; i < task->count; i++) {
            if (tech == (task->techs[i] & SKILL_ID)) {
                tech = 0;
                break;
            }
        }
        if (tech != 0) {
            task->techs[task->count++] = extra[j];
        }
    }
}

/* Counts the menu's pages and sets the cursor's lines for the first one. */
static inline void setTechPages(TechMenu *task) {
    if (task->count != 0) {
        if (task->count % TECH_MENU_LINES != 0) {
            task->pageCount = task->count / TECH_MENU_LINES + 1;
        } else {
            task->pageCount = task->count / TECH_MENU_LINES;
        }
    }
    if (task->count != 0) {
        if (task->count > TECH_MENU_LINES) {
            FIGHTSTG_techCursor.count = TECH_MENU_LINES;
        } else {
            FIGHTSTG_techCursor.count = task->count;
        }
    }
#if VERSION_EU || VERSION_JP
    else {
        FIGHTSTG_techCursor.count = 1;
    }
#endif
}

/* Shows the page turned to, with the cursor on its first line. */
static inline void showNewTechPage(TechMenu *task, TechMenuChild *children) {
    s32 lines;

    children[0].cursor->sel = 0;
    lines = task->count - task->page * TECH_MENU_LINES;
    if (lines > TECH_MENU_LINES) {
        lines = TECH_MENU_LINES;
    }
    children[0].cursor->params.count = lines;
    FIGHTSTG_showTechPage(task, children);
    SOUND.playSound(SOUND_MENU_MOVE);
}

/* The battle's technique menu: the active fighter's techniques (a Digimon of
   a partner's slots has its entry's, its signature one and the ones the other
   entries can pass on), six a page. L1 and R1 turn the pages, cross picks
   the technique under the cursor into *result, spending its MP (a temporary
   Digimon spends none), and triangle gives -2 */
void FIGHTSTG_updateTechMenu(TechMenu *task, TechMenuChild *children) {
    BattleFighter *fighter;
    BattleFighter *active;
    s32 member;
    s32 id;
    s32 pressed;
    s32 page;
    s32 mp;

    switch (task->state) {
    case TASK_INIT:
    default:
        member = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
        fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        id = fighter->id;
        if (id == DIGIMON_DATA[member].id) {
            task->techs[0] = DIGIMON_DATA[member].skills[6] | 0x8000;
            task->count = 1;
        } else {
            listSlotTechs(task, fighter, member, id);
        }
        setTechPages(task);
        children[0].cursor = FIGHTSTG_createCursor(&FIGHTSTG_techCursor);
        FIGHTSTG_createTechWindows(task, children);
        FIGHTSTG_showTechPage(task, children);
        task->nextState(task);
        break;
    case TASK_RUN:
        FIGHTSTG_drawTechMenu(task);
        /* the match depends on the do-while and its breaks, which skip the
           update of sel, the stages' early exit */
        do {
            pressed = PAD.getPressed(0);
            page = task->page;
            if (task->pageCount != 0) {
                if (pressed & (1 << PAD_L1)) {
                    if (--task->page < 0) {
                        task->page = 0;
                    }
                } else if (pressed & (1 << PAD_R1)) {
                    if (++task->page > task->pageCount - 1) {
                        task->page = task->pageCount - 1;
                    }
                }
            }
            if (page != task->page) {
                showNewTechPage(task, children);
            } else if (task->sel != children[0].cursor->sel) {
                FIGHTSTG_showTechPage(task, children);
            } else if (pressed & (1 << PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                if (task->count != 0) {
                    active = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
                    /* the match depends on the choice written in both branches */
                    if (active->temporary != 0) {
                        *task->result = task->techs[task->page * TECH_MENU_LINES + children[0].cursor->sel] & SKILL_ID;
                        task->setState(task, 3);
                        children[0].cursor->locked = 1;
                        break;
                    }
                    mp = FIGHTSTG_battleFuncs.getTechCost(0, task->techs[task->page * TECH_MENU_LINES + children[0].cursor->sel]);
                    if (active->mp >= mp) {
                        active->mp -= mp;
                        *task->result = task->techs[task->page * TECH_MENU_LINES + children[0].cursor->sel] & SKILL_ID;
                        task->setState(task, 3);
                        children[0].cursor->locked = 1;
                        break;
                    }
                }
            } else if (pressed & (1 << PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                *task->result = -2;
                task->setState(task, 3);
                break;
            }
            task->sel = children[0].cursor->sel;
        } while (0);
        break;
    case 2:
    case 3:
        break;
    }
}

/* Opens the battle's technique menu; *result gets the technique picked, or
   -2 to go back */
TechMenu *FIGHTSTG_createTechMenu(s32 *result) {
    TechMenu *task = createTask(FIGHTSTG_updateTechMenu, sizeof(TechMenu), 16 * sizeof(TechMenuChild));

    task->result = result;
    *result = -1;
    return task;
}
