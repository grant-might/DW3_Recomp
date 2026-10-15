/* FIGHTSTG's message box and its messages. */

#include "battle/battle_mode.h"

/* Draws the message box and, once its lines are out, its blinking arrow */
void FIGHTSTG_drawMessageBox(BattleMessageBox *task) {
    SpriteDrawer drawer;
    void *sheet;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    if (task->showArrow != 0) {
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
    }
    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0x31, 0xB, 0xBC);
}

/* Shows the message's lines one by one, 3 frames apart, then on cross shows
   the queue's next message or ends; after finish (substate 4) it ends after
   0x14 frames or on cross */
void FIGHTSTG_stepMessage(BattleMessageBox *task, BattleMessageBoxWindows *windows) {
    switch (task->substate) {
    case 0:
    default:
        if (task->started != 0) {
            task->substate++;
        }
        break;
    case 1:
        task->shownTime = GFX.funcs.getTime();
        task->interval = 3;
        windows->lines[task->shown]->setVisible(windows->lines[task->shown], 1);
        task->substate++;
        break;
    case 2:
        if (task->interval < GFX.funcs.getTime() - task->shownTime) {
            if (++task->shown >= task->count) {
                task->showArrow = 1;
                task->substate++;
            } else {
                task->substate = 1;
            }
        }
        break;
    case 3:
        if (PAD.getPressed(0) & (1 << PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            if (task->queue[++task->next] == 0) {
                task->state = 3;
            } else {
                task->show(task, task->queue[task->next], 0);
                task->setSubstate(task, 0);
            }
            task->showArrow = 0;
        }
        break;
    case 4:
        if (GFX.funcs.getTime() - task->step > 0x14 || (PAD.getPressed(0) & (1 << PAD_CROSS))) {
            task->state = 3;
        }
        break;
    }
}

/* The battle message box's task: draws it and steps its message each frame */
void FIGHTSTG_updateMessage(BattleMessageBox *task, void *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        FIGHTSTG_drawMessageBox(task);
        FIGHTSTG_stepMessage(task, children);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Puts the name of side's fighter index (a partner's own, an enemy's from the
   battle table) into the message's first line */
void FIGHTSTG_setMessageName(BattleMessageBox *task, BattleMessageBoxWindows *w, s32 side, s32 index) {
    BattleFighter *fighter;
    BattleTableEntry *entry;

    if (side == 0) {
        w->lines[0]->setSubString(w->lines[0], GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(index)), -1, 1);
    } else {
        fighter = &FIGHTSTG_battle.state.fighters[1][index];
        entry = FIGHTSTG_battleTableFunc(fighter->id);
        w->lines[0]->setSubString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), entry->nameId, 1);
    }
}

/* Lists in task's found the fighters of filter's side that its type picks: 0
   the hurt, 1 the poisoned, 2 the paralyzed, 3 the confused, 4 those with
   any status, 5 the knocked out, 6 all, 7-9 those standing */
void FIGHTSTG_findFighters(BattleMessageBox *task, FighterFilter *filter) {
    s32 side = filter->side != 0;
    BattleFighter *fighters;
    s32 i;

    task->foundCount = 0;
    fighters = FIGHTSTG_battle.state.fighters[side];
    switch (filter->type) {
    case 0:
    default:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0 && fighters[i].hp < fighters[i].maxHp) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 1:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0 && (fighters[i].flags & FIGHTER_POISONED)) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 2:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0 && (fighters[i].flags & FIGHTER_PARALYZED)) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 3:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0 && (fighters[i].flags & FIGHTER_CONFUSED)) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 4:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0 && fighters[i].flags != 0) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 5:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp == 0) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 6:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    case 7:
    case 8:
    case 9:
        for (i = 0; i < 3; i++) {
            if (fighters[i].id != 0 && fighters[i].hp != 0) {
                task->found[task->foundCount++] = i;
            }
        }
        break;
    }
}

/* Sets up a message about the fighters FIGHTSTG_findFighters found: their
   names (text 0x15 plus their count) and what happened to them by msg's kind,
   or text 0x2F when there are none */
void FIGHTSTG_showFightersMessage(BattleMessageBox *task, BattleMessageBoxWindows *w, BattleMessage *msg) {
    BattleFighter *fighter;
    BattleTableEntry *entry;
    s32 member;
    u8 side;
    s32 kind;
    s32 i;

    if (task->foundCount != 0) {
        side = msg->side;
        kind = msg->kind;
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), task->foundCount + 0x15);
        if (side == 0) {
            for (i = 0; i < task->foundCount; i++) {
                member = GAME.funcs.getPartyMember(task->found[i]);
                if (member >= 0) {
                    w->lines[0]->setSubString(w->lines[0], GAME.funcs.getPartnerStats(member), -1, i + 1);
                }
            }
        } else {
            for (i = 0; i < task->foundCount; i++) {
                fighter = &FIGHTSTG_battle.state.fighters[1][task->found[i]];
                if (fighter->id != 0) {
                    entry = FIGHTSTG_battleTableFunc(fighter->id);
                    w->lines[0]->setSubString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), entry->nameId, i + 1);
                }
            }
        }
        switch (kind) {
        case 0:
        default:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x22);
            w->lines[1]->setNumber(w->lines[1], 1, msg->value);
            break;
        case 1:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x28);
            break;
        case 2:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x29);
            break;
        case 3:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x2A);
            break;
        case 4:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x2C);
            break;
        case 5:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x2D);
            break;
        case 6:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x2E);
            break;
        case 7:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x30);
            break;
        case 8:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x31);
            break;
        case 9:
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x32);
            break;
        }
        task->count = 2;
    } else {
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x2F);
        task->count = 1;
    }
}

/* The message box's show: sets up the lines of message type from data, mostly
   a fighter's name with a text, a technique or a number, and starts showing
   them */
void FIGHTSTG_showMessage(BattleMessageBox *task, s32 type, s32 *data) {
    BattleMessageBoxWindows *w = task->children;
    s32 total;
    s32 side;

    task->started = 1;
    if (w->lines[0] == NULL) {
        w->lines[0] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xC2);
    }
    if (w->lines[1] == NULL) {
        w->lines[1] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x14, 0xD0);
    }
    task->shown = 0;
    switch (type) {
    case 1:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), data[0]);
        task->count = 1;
        break;
    case 2:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), data[0]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[1], FIGHTSTG_battle.state.active[data[1] != 0]);
        break;
    case 3:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 9);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), data[1], 1);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 4:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0xB);
        w->lines[1]->setNumber(w->lines[1], 1, data[1]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 5:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0xA);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), data[1], 1);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 6:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x25);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 7:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), data[0]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[1], data[2]);
        break;
    case 8:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x22);
        w->lines[1]->setNumber(w->lines[1], 1, data[1]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 9:
        /* its args are read both as which fighters to find and as what to
           say of them */
        FIGHTSTG_findFighters(task, (FighterFilter *)data);
        FIGHTSTG_showFightersMessage(task, w, (BattleMessage *)data);
        break;
    case 10:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x27);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], data[1]);
        task->queue[1] = 11;
        task->queue[2] = data[0];
        task->queue[3] = data[1];
        task->queue[4] = data[2];
        break;
    case 11:
        data = &task->queue[task->next + 1];
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x22);
        w->lines[1]->setNumber(w->lines[1], 1, data[2]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], data[1]);
        task->next += 3;
        break;
    case 12:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x37);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data[1], 1);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 13:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x4E);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x4F);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), data[0], 1);
        task->count = 2;
        break;
    case 14:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 9);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), data[1], 1);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 15:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x55);
        w->lines[1]->setNumber(w->lines[1], 1, data[2]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], data[1]);
        break;
    case 16:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x23);
        total = data[1] * data[2];
        w->lines[1]->setNumber(w->lines[1], 1, data[1]);
        w->lines[1]->setNumber(w->lines[1], 2, data[2]);
        w->lines[1]->setNumber(w->lines[1], 3, total);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 17:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x26);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 9);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), data[0], 1);
        task->count = 2;
        break;
    case 18:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        if (data[2] == 0) {
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x22);
        } else {
            w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x8F);
        }
        w->lines[1]->setNumber(w->lines[1], 1, data[1]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 19:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), data[1] + 0x49);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, data[0], FIGHTSTG_battle.state.active[data[0] != 0]);
        break;
    case 20:
        FIGHTSTG_showMessage(task, 4, data);
        task->queue[1] = 21;
        task->queue[2] = (data[0] == 0) << 4;
        task->queue[3] = data[1];
        break;
    case 21:
        side = task->queue[task->next + 1];
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x16);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x22);
        w->lines[1]->setNumber(w->lines[1], 1, task->queue[task->next + 2]);
        task->count = 2;
        FIGHTSTG_setMessageName(task, w, side, FIGHTSTG_battle.state.active[side != 0]);
        task->next += 2;
        break;
    case 22:
        w->lines[0]->setString(w->lines[0], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x8C);
        w->lines[1]->setString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 9);
        w->lines[1]->setSubString(w->lines[1], FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), data[0], 1);
        task->count = 2;
        break;
    }
    w->lines[0]->setVisible(w->lines[0], 0);
    w->lines[1]->setVisible(w->lines[1], 0);
}

/* The message box's finish: closes it after 0x14 frames or on cross */
void FIGHTSTG_finishMessage(BattleMessageBox *task) {
    task->substate = 4;
    task->showArrow = 0;
    task->step = GFX.funcs.getTime();
}

/* Creates a battle message box (FIGHTSTG_updateMessage) */
BattleMessageBox *FIGHTSTG_createMessage(void) {
    BattleMessageBox *task = createTask(FIGHTSTG_updateMessage, sizeof(BattleMessageBox), 8);

    task->show = FIGHTSTG_showMessage;
    task->finish = FIGHTSTG_finishMessage;
    return task;
}
