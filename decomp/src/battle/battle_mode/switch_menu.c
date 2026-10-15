/* FIGHTSTG's switch menu, the partners that can come in. */

#include "battle/battle_mode.h"

/* Returns the Digimon fighter index has a pair technique with (its pairPartner, a
   1-based DIGIMON_DATA entry) when party member member has it among its
   digivolutions, else 0 */
s32 FIGHTSTG_getPairDigimon(SwitchMenu *task, s32 index, s32 member) {
    BattleFighter *fighter;
    s32 partner;
    s32 next;
    s32 count;
    s32 i;

    GAME.funcs.getPartyMember(index);
    partner = GAME.funcs.getPartyMember(member);
    fighter = &FIGHTSTG_battle.state.fighters[0][index];
    next = GET_DIGIMON(fighter->id)->pairPartner;
    count = GAME.funcs.getPartnerSlots(partner, task->slots);
    if (count <= 0 || next == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        if (task->slots[i] == DIGIMON_DATA[next - 1].id) {
            return next;
        }
    }
    return 0;
}

/* Draws the pair technique icon by the fighters that have one with the active
   fighter */
void FIGHTSTG_drawSwitchMenu(SwitchMenu *task) {
    SpriteDrawer drawer;
    void *sheet;
    s32 i;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    for (i = 0; i < task->count; i++) {
        if (task->pairs[i] != 0) {
            drawer.draw(sheet, 0x30, 0x5A, 0x45 + i * 0x1F);
        }
    }
}

/* Makes the switch menu's windows, two lines a fighter: the name, HP over max
   HP and MP over max MP */
void FIGHTSTG_createSwitchWindows(SwitchMenu *task, SwitchMenuWindows *w) {
    char *text;
    s32 i;

    text = FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU));
    for (i = 0; i < task->count; i++) {
        w->hpLabel[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x6C, 0x4E + i * 0x20);
        w->hpLabel[i]->setString(w->hpLabel[i], text, 14);
        w->hpSlash[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x99, 0x4E + i * 0x20);
        w->hpSlash[i]->setString(w->hpSlash[i], text, 16);
        w->mpLabel[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x6C, 0x5C + i * 0x20);
        w->mpLabel[i]->setString(w->mpLabel[i], text, 13);
        w->mpSlash[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x99, 0x5C + i * 0x20);
        w->mpSlash[i]->setString(w->mpSlash[i], text, 16);
        w->hp[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x98, 0x4E + i * 0x20);
        w->maxHp[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xBB, 0x4E + i * 0x20);
        w->mp[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x98, 0x5C + i * 0x20);
        w->maxMp[i] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0xBB, 0x5C + i * 0x20);
        w->name[i] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x24, 0x4C + i * 0x20);
    }
}

/* Shows the switch menu's fighters' names, HP and MP, the knocked out ones'
   names in palette 7 */
void FIGHTSTG_showSwitchFighters(SwitchMenu *task, SwitchMenuWindows *w) {
    BattleFighter *fighter;
    s32 i;

    for (i = 0; i < task->count; i++) {
        fighter = &FIGHTSTG_battle.state.fighters[0][task->others[i]];
        w->hp[i]->setNumber(w->hp[i], 0, fighter->hp);
        w->hp[i]->setRightAlign(w->hp[i], 1);
        w->maxHp[i]->setNumber(w->maxHp[i], 0, fighter->maxHp);
        w->maxHp[i]->setRightAlign(w->maxHp[i], 1);
        w->mp[i]->setNumber(w->mp[i], 0, fighter->mp);
        w->mp[i]->setRightAlign(w->mp[i], 1);
        w->maxMp[i]->setNumber(w->maxMp[i], 0, fighter->maxMp);
        w->maxMp[i]->setRightAlign(w->maxMp[i], 1);
        w->name[i]->setString(w->name[i], GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(task->others[i])), -1);
        if (fighter->hp == 0) {
            w->name[i]->setPalette(w->name[i], PALETTE_GREY);
        } else {
            w->name[i]->setPalette(w->name[i], PALETTE_WHITE);
        }
    }
}

/* The switch menu's task: puts a cursor on the other fighters, starting on
   *line; cross picks one with HP into *result, triangle gives -2 when it
   can cancel. With none to pick it shows message 0x14 and its blinking
   arrow until cross gives -2 */
void FIGHTSTG_updateSwitchMenu(SwitchMenu *task, SwitchMenuWindows *w) {
    SpriteDrawer drawer;
    BattleFighter *fighters;
    BattleFighter *fighter;
    s32 pressed;
    s32 sel;
    s32 active;
    s32 temporary;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        active = FIGHTSTG_battle.state.active[0];
        fighters = FIGHTSTG_battle.state.fighters[0];
        temporary = fighters[active].temporary;
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && i != FIGHTSTG_battle.state.active[0]) {
                task->others[task->count] = i;
                if (task->canCancel != 0 && temporary == 0) {
                    task->pairs[task->count] = FIGHTSTG_getPairDigimon(task, FIGHTSTG_battle.state.active[0], i);
                }
                task->count++;
            }
        }
        if (task->count > 0) {
            FIGHTSTG_switchCursor.count = task->count;
            w->cursor = FIGHTSTG_createCursor(&FIGHTSTG_switchCursor);
            w->cursor->sel = *task->line;
            FIGHTSTG_createSwitchWindows(task, w);
            FIGHTSTG_showSwitchFighters(task, w);
            task->nextState(task);
        } else {
            w->message = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xC2);
            w->message->setVisible(w->message, 0);
            task->setState(task, 2);
        }
        break;
    case TASK_RUN:
        FIGHTSTG_drawSwitchMenu(task);
        /* the match depends on the do-while and its break, the stages' early exit */
        do {
            pressed = PAD.getPressed(0);
            if (task->canCancel != 0 && (pressed & (1 << PAD_TRIANGLE))) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                *task->result = -2;
                task->setState(task, 3);
                break;
            }
            if (pressed & (1 << PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                sel = w->cursor->sel;
                fighter = &FIGHTSTG_battle.state.fighters[0][task->others[sel]];
                if (fighter->hp != 0) {
                    *task->line = sel;
                    *task->result = task->others[w->cursor->sel];
                    task->setState(task, 3);
                    w->cursor->locked = 1;
                }
            }
        } while (0);
        break;
    case 2:
        if (PAD.getPressed(0) & (1 << PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            *task->result = -2;
            task->setState(task, 3);
            w->message->setVisible(w->message, 0);
            break;
        }
        initSpriteDrawer(&drawer);
        drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
        if (task->arrowShown != 0) {
            if (GFX.funcs.getTime() - task->arrowTime >= 4) {
                task->arrowTime = GFX.funcs.getTime();
                task->arrowPalette++;
                if (task->arrowPalette >= 5) {
                    task->arrowPalette = 0;
                }
            }
            drawer.setTexture(0x140, 0);
            drawer.setClutRow(task->arrowPalette);
            drawer.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 10, 0x123, 0xD0);
            drawer.setClutRow(0);
        } else {
            task->arrowShown = 1;
        }
        drawer.setTexture(0x200, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16), 0x31, 0xB, 0xBC);
        if (!w->message->isVisible(w->message)) {
            w->message->setString(w->message, FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x14);
        }
        break;
    case 3:
        break;
    }
}

/* Opens the switch menu; *result gets the fighter to bring in, or -2 */
SwitchMenu *FIGHTSTG_createSwitchMenu(s32 *result, s32 *line, s32 canCancel) {
    SwitchMenu *task = createTask(FIGHTSTG_updateSwitchMenu, sizeof(SwitchMenu), sizeof(SwitchMenuWindows));

    task->result = result;
    *result = -1;
    task->line = line;
    task->canCancel = canCancel;
    return task;
}
