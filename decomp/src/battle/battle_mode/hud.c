/* FIGHTSTG's HUD: the fighters' names, HP and MP bars and the technique gauge. */

#include "battle/battle_mode.h"

/* Every frame: the hp tween of a side whose active fighter changed jumps to
 * that fighter's hp; every interval, a side's tween starts easing (30 frames)
 * to its fighter's hp, clamped to 0 and the max hp, when that changed. The
 * match depends on a counter for each loop and the side's row of fighters. */
void FIGHTSTG_updateHpTweens(HpDisplay *task, TextWindow **windows) {
    s32 hp;
    s32 i;
    s32 j;
    BattleFighter *row;

    task->timer += GFX.funcs.getFrameTime();
    for (i = 0; i < 2; i++) {
        row = FIGHTSTG_battle.state.fighters[i];
        if (task->hp[i].fighter != FIGHTSTG_battle.state.active[i]) {
            task->hp[i].from = row[FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].to = row[FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].value = row[FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].active = 0;
            task->hp[i].fighter = FIGHTSTG_battle.state.active[i];
        }
    }
    if (task->timer > task->interval) {
        task->timer -= task->interval;
        for (j = 0; j < 2; j++) {
            row = FIGHTSTG_battle.state.fighters[j];
            hp = row[FIGHTSTG_battle.state.active[j]].hp;
            if (hp <= 0) {
                hp = 0;
            }
            if (hp > row[FIGHTSTG_battle.state.active[j]].maxHp) {
                hp = row[FIGHTSTG_battle.state.active[j]].maxHp;
            }
            if (hp != task->hp[j].to) {
                task->hp[j].from = task->hp[j].value;
                task->hp[j].to = hp;
                task->hp[j].active = 1;
                task->hp[j].time = 0;
                task->hp[j].duration = 30;
            }
        }
    }
}

/* Eases an HP number towards its target over its duration, along a sine */
void FIGHTSTG_stepHpTween(HpTween *tween) {
    s32 t;

    if (tween->active != 0) {
        tween->time += GFX.funcs.getFrameTime();
        if (tween->time >= tween->duration) {
            tween->active = 0;
            tween->from = tween->value = tween->to;
        } else {
            t = rsin((tween->time << 10) / tween->duration) * tween->duration / 4096;
            tween->value = tween->from + (tween->to - tween->from) * t / tween->duration;
        }
    }
}

/* Shows the names of both sides' active fighters, when they changed */
void FIGHTSTG_showFighterNames(HpDisplay *task, TextWindow **windows) {
    BattleTableEntry *enemy;
    s32 i;

    if (FIGHTSTG_battle.state.active[0] != task->shown[0]) {
        if (windows[0] == NULL) {
            windows[0] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0xAE, 0x15);
        }
        windows[0]->setString(windows[0], GAME.partners[GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0])].info.name, -1);
    }
    if (FIGHTSTG_battle.state.active[1] != task->shown[1]) {
        enemy = FIGHTSTG_battleTableFunc(BATTLE_SETUP.enemies[FIGHTSTG_battle.state.active[1]].fighter);
        if (windows[1] == NULL) {
            windows[1] = createTextWindow(BATTLE_LAYER_MENUS, 1, 0x11, 0x15);
        }
        if (enemy != NULL) {
            windows[1]->setString(windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), enemy->nameId);
        }
    }
    for (i = 0; i < 2; i++) {
        task->shown[i] = FIGHTSTG_battle.state.active[i];
    }
}

/* FIGHTSTG_drawHud's HP bars, one row per side; the European version swaps
   side 1's ends, so that its bar shrinks toward its right end */
#if VERSION_US
DVECTOR FIGHTSTG_hpBars[2][4] = {
    { { 0xAF, 0x25 }, { 0x12F, 0x25 }, { 0xAF, 0x2B }, { 0x12F, 0x2B } },
    { { 0x0F, 0x25 }, { 0x8F, 0x25 }, { 0x0F, 0x2B }, { 0x8F, 0x2B } },
};
#elif VERSION_EU || VERSION_JP
DVECTOR FIGHTSTG_hpBars[2][4] = {
    { { 0xAF, 0x25 }, { 0x12F, 0x25 }, { 0xAF, 0x2B }, { 0x12F, 0x2B } },
    { { 0x8F, 0x25 }, { 0x0F, 0x25 }, { 0x8F, 0x2B }, { 0x0F, 0x2B } },
};
#endif
/* the bars' colours: above and at a quarter of the HP */
CVECTOR FIGHTSTG_hpBarColors[4] = {
    { 0x00, 0x71, 0x28, 0x00 },
    { 0x00, 0xC8, 0x3E, 0x00 },
    { 0xB3, 0x37, 0x16, 0x00 },
    { 0xFF, 0x37, 0x0D, 0x00 },
};
/* the technique gauge, and its colours */
DVECTOR FIGHTSTG_techGauge[4] = {
    { 0x109, 0x3E }, { 0x131, 0x3E }, { 0x109, 0x46 }, { 0x131, 0x46 },
};
CVECTOR FIGHTSTG_techGaugeColors[4] = {
    { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
};
/* the fighter icons' x, for each side and slot */
s16 FIGHTSTG_iconX[2][3] = {
    { 0x010F, 0x011A, 0x0125 },
    { 0x0025, 0x001A, 0x000F },
};

/* Draws the battle HUD: both HP bars, the technique gauge (full at 1000) and
   the fighters' icons */
void FIGHTSTG_drawHud(HpDisplay *task, TextWindow **windows) {
    SpriteDrawer drawer;
    CVECTOR colors[4];
    BattleFighter *fighter;
    void *sheet;
    s32 width;
    s32 blink;
    s32 member;
    s32 frame;
    s32 i;
    s32 side;
    s32 slot;

    sheet = FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16);
    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 1);
    drawer.setTexture(0x200, 0);
    drawer.draw(sheet, 0, 8, 0xF);
    drawer.draw(sheet, 1, 0xA1, 0xF);
    FIGHTSTG_showFighterNames(task, windows);
    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    if (windows[4] == NULL) {
        windows[4] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x10B, 0x1A);
    }
    windows[4]->setNumber(windows[4], 0, task->hp[0].value);
    windows[4]->setRightAlign(windows[4], 1);
    if (windows[3] == NULL) {
        windows[3] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x12E, 0x1A);
    }
    windows[3]->setNumber(windows[3], 0, fighter->maxHp);
    windows[3]->setRightAlign(windows[3], 1);
    for (i = 0; i < 2; i++) {
        fighter = &FIGHTSTG_battle.state.fighters[i][FIGHTSTG_battle.state.active[i]];
        width = (task->hp[i].value << 7) / fighter->maxHp;
        if (task->hp[i].value > 0 && width < 4) {
            width = 3;
        }
#if VERSION_EU || VERSION_JP
        /* the match depends on testing i == 0 first (i != 0 gives 13 diffs) */
        if (i == 0) {
            FIGHTSTG_hpBars[i][1].vx = FIGHTSTG_hpBars[i][0].vx + width;
            FIGHTSTG_hpBars[i][3].vx = FIGHTSTG_hpBars[i][2].vx + width;
        } else {
            FIGHTSTG_hpBars[i][1].vx = FIGHTSTG_hpBars[i][0].vx - width;
            FIGHTSTG_hpBars[i][3].vx = FIGHTSTG_hpBars[i][2].vx - width;
        }
#else
        FIGHTSTG_hpBars[i][1].vx = FIGHTSTG_hpBars[i][0].vx + width;
        FIGHTSTG_hpBars[i][3].vx = FIGHTSTG_hpBars[i][2].vx + width;
#endif
        if ((fighter->maxHp >> 2) < task->hp[i].value) {
            colors[0] = colors[2] = FIGHTSTG_hpBarColors[0];
            colors[1] = colors[3] = FIGHTSTG_hpBarColors[1];
        } else {
            colors[0] = colors[2] = FIGHTSTG_hpBarColors[2];
            colors[1] = colors[3] = FIGHTSTG_hpBarColors[3];
        }
        FIGHTSTG_battle.drawQuad(BATTLE_LAYER_MENUS, 1, FIGHTSTG_hpBars[i], colors);
    }
    blink = (GFX.funcs.getTime() >> 2) & 3;
    member = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
    drawer.draw(sheet, 0x1E, 0x104, 0x3D);
    if (BATTLE_SETUP.gauges[member] < 1000) {
        /* the match depends on storing [2] before [0] (the other order gives 2
           diffs) */
        FIGHTSTG_techGauge[2].vx = FIGHTSTG_techGauge[0].vx = BATTLE_SETUP.gauges[member] / 25 + 0x109;
        FIGHTSTG_battle.drawQuad(BATTLE_LAYER_MENUS, 1, FIGHTSTG_techGauge, FIGHTSTG_techGaugeColors);
    }
    if (BATTLE_SETUP.gauges[member] < 1000) {
        drawer.draw(sheet, blink + 0x33, 0x109, 0x3E);
    } else {
        drawer.draw(sheet, blink + 0x37, 0x109, 0x3E);
    }
    drawer.draw(sheet, 0xB, 0x104, 0x3D);
    for (side = 0; side < 2; side++) {
        for (slot = 0; slot < 3; slot++) {
            if (FIGHTSTG_battle.state.kind >= BATTLE_KIND_FINAL && slot > 0 && side > 0) {
                return;
            }
            fighter = &FIGHTSTG_battle.state.fighters[side][slot];
            if (fighter->id != 0) {
                if (fighter->flags != 0) {
                    frame = 3;
                } else if (fighter->hp == 0) {
                    frame = 2;
                } else {
                    frame = fighter->hp != fighter->maxHp;
                }
                if (FIGHTSTG_battle.state.active[side] == slot) {
                    frame += 0xC;
                } else {
                    frame += 0x3C;
                }
                drawer.draw(sheet, frame, FIGHTSTG_iconX[side][slot], 0x30);
            }
        }
    }
}

/* The battle HUD's task: makes its windows and starts the HP numbers at the
   fighters' HP, then each frame eases them and draws the HUD
   (FIGHTSTG_drawHud) */
void FIGHTSTG_updateHud(HpDisplay *task, TextWindow **windows) {
    s32 i;
    BattleFighter (*fighters)[3];

    switch (task->state) {
    case TASK_INIT:
    default:
        task->shown[1] = -1;
        task->shown[0] = -1;
        FIGHTSTG_showFighterNames(task, windows);
        windows[2] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x10C, 0x1A);
        windows[2]->setString(windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_BATTLE_MENU)), 0x10);
        fighters = FIGHTSTG_battle.state.fighters;
        windows[4] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x10B, 0x1A);
        windows[4]->setNumber(windows[4], 0, fighters[0][FIGHTSTG_battle.state.active[0]].hp);
        windows[4]->setRightAlign(windows[4], 1);
        windows[3] = createTextWindow(BATTLE_LAYER_MENUS, 3, 0x12E, 0x1A);
        windows[3]->setNumber(windows[3], 0, fighters[0][FIGHTSTG_battle.state.active[0]].maxHp);
        windows[3]->setRightAlign(windows[3], 1);
        for (i = 0; i < 2; i++) {
            task->hp[i].from = fighters[i][FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].to = fighters[i][FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].value = fighters[i][FIGHTSTG_battle.state.active[i]].hp;
            task->hp[i].active = 0;
            task->hp[i].fighter = FIGHTSTG_battle.state.active[i];
            task->hp[i].time = 0;
            task->hp[i].duration = 0;
        }
        task->timer = 0;
        task->interval = 8;
        task->nextState(task);
        break;
    case TASK_RUN:
        FIGHTSTG_updateHpTweens(task, windows);
        FIGHTSTG_stepHpTween(&task->hp[0]);
        FIGHTSTG_stepHpTween(&task->hp[1]);
        FIGHTSTG_drawHud(task, windows);
        break;
    case TASK_DONE:
        task->nextState(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the battle HUD (FIGHTSTG_updateHud) */
HpDisplay *FIGHTSTG_createHud(void) {
    return createTask(FIGHTSTG_updateHud, sizeof(HpDisplay), 5 * sizeof(TextWindow *));
}
