/* The training screen: the party, the partner's stats and the training menu;
   the screen fade; and the first object's data */

#include "menus/gym.h"

/* Creates the screen's text windows */
void STGTRAIN_createScreenWindows(TrainScreen *screen, TrainScreenWindows *win) {
    s32 i;

    win->name = createTextWindow(screen->layerId, 1, 0x33, 0x13);
    win->level[0] = createTextWindow(screen->layerId, 3, 0x33, 0x22);
    win->level[1] = createTextWindow(screen->layerId, 3, 0x50, 0x22);
    win->hp[0] = createTextWindow(screen->layerId, 3, 0x33, 0x2C);
    win->hp[1] = createTextWindow(screen->layerId, 3, 0x5D, 0x2C);
    win->hp[2] = createTextWindow(screen->layerId, 3, 0x80, 0x2C);
    win->mp[0] = createTextWindow(screen->layerId, 3, 0x33, 0x35);
    win->mp[1] = createTextWindow(screen->layerId, 3, 0x5D, 0x35);
    win->mp[2] = createTextWindow(screen->layerId, 3, 0x80, 0x35);
    win->slashes[0] = createTextWindow(screen->layerId, 3, 0x5F, 0x2C);
    win->slashes[1] = createTextWindow(screen->layerId, 3, 0x5F, 0x35);
    for (i = 0; i < 6; i++) {
        win->stats[i] = createTextWindow(screen->layerId, 1, 0x33, i * 0xE + 0x4F);
    }
    for (i = 0; i < 7; i++) {
        win->resistances[i] = createTextWindow(screen->layerId, 1, 0x5D, i * 0xE + 0x4F);
    }
    win->tp[0] = createTextWindow(screen->layerId, 1, 0x10, 0xBB);
    win->tp[1] = createTextWindow(screen->layerId, 1, 0x2C, 0xBB);
    win->help = createTextWindow(screen->layerId, 1, 0xA1, 0x17);
    win->prompt = createTextWindow(screen->layerId, 1, 0xAE, 0x49);
}

/* Shows or hides the selected partner's name, level, HP and MP */
void STGTRAIN_showVitals(TrainScreen *screen, TrainScreenWindows *win, s32 show) {
    PartnerTotals totals;
    PartnerStats *vitals;
    s32 partner;
    s32 i;

    if (show) {
        partner = GAME.funcs.getPartyMember(screen->partner);
        vitals = GAME.funcs.getPartnerStats(partner);
        GAME.funcs.computeStats(partner, &totals);
        win->name->setString(win->name, vitals, -1);
        win->level[0]->setString(win->level[0], FILE_CACHE.load(STGTRAIN_TEXT), 1);
        win->level[1]->setNumber(win->level[1], 0, totals.fields.level);
        win->level[1]->setRightAlign(win->level[1], 1);
        win->hp[0]->setString(win->hp[0], FILE_CACHE.load(STGTRAIN_TEXT), 2);
        win->hp[1]->setNumber(win->hp[1], 0, totals.fields.hp);
        win->hp[2]->setNumber(win->hp[2], 0, totals.fields.maxHp);
        for (i = 1; i < 3; i++) {
            win->hp[i]->setPalette(win->hp[i], PALETTE_WHITE);
            win->hp[i]->setRightAlign(win->hp[i], 1);
        }
        win->mp[0]->setString(win->mp[0], FILE_CACHE.load(STGTRAIN_TEXT), 3);
        win->mp[1]->setNumber(win->mp[1], 0, totals.fields.mp);
        win->mp[2]->setNumber(win->mp[2], 0, totals.fields.maxMp);
        for (i = 1; i < 3; i++) {
            win->mp[i]->setPalette(win->mp[i], PALETTE_WHITE);
            win->mp[i]->setRightAlign(win->mp[i], 1);
        }
        for (i = 0; i < 2; i++) {
            win->slashes[i]->setString(win->slashes[i], FILE_CACHE.load(STGTRAIN_TEXT), 0x43);
        }
    } else {
        win->name->setVisible(win->name, 0);
        for (i = 0; i < 2; i++) {
            win->level[i]->setVisible(win->level[i], 0);
            win->slashes[i]->setVisible(win->slashes[i], 0);
        }
        for (i = 0; i < 3; i++) {
            win->hp[i]->setVisible(win->hp[i], 0);
        }
        for (i = 0; i < 3; i++) {
            win->mp[i]->setVisible(win->mp[i], 0);
        }
    }
}

/* Shows or hides the selected partner's stats and resistances */
void STGTRAIN_showBattleStats(TrainScreen *screen, TrainScreenWindows *win, s32 show) {
    PartnerTotals totals;
    s32 i;

    if (show) {
        GAME.funcs.computeStats(GAME.funcs.getPartyMember(screen->partner), &totals);
        for (i = 0; i < 6; i++) {
            win->stats[i]->setNumber(win->stats[i], 0, totals.fields.battle[i]);
            win->stats[i]->setRightAlign(win->stats[i], 1);
            win->stats[i]->setPalette(win->stats[i], PALETTE_WHITE);
        }
        if (totals.fields.lowered[0] != 0) {
            win->stats[0]->setPalette(win->stats[0], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[1] != 0) {
            win->stats[1]->setPalette(win->stats[1], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[2] != 0) {
            win->stats[4]->setPalette(win->stats[4], PALETTE_PURPLE);
        }
        for (i = 0; i < 7; i++) {
            win->resistances[i]->setNumber(win->resistances[i], 0, totals.fields.resist[i]);
            win->resistances[i]->setRightAlign(win->resistances[i], 1);
            win->resistances[i]->setPalette(win->resistances[i], PALETTE_WHITE);
        }
    } else {
        for (i = 0; i < 6; i++) {
            win->stats[i]->setVisible(win->stats[i], 0);
        }
        for (i = 0; i < 7; i++) {
            win->resistances[i]->setVisible(win->resistances[i], 0);
        }
    }
}

/* Shows or hides the selected partner's TP */
void STGTRAIN_showTp(TrainScreen *screen, TrainScreenWindows *win, s32 show) {
    PartnerTotals totals;
    s32 i;

    if (show) {
        GAME.funcs.computeStats(GAME.funcs.getPartyMember(screen->partner), &totals);
        win->tp[0]->setString(win->tp[0], FILE_CACHE.load(STGTRAIN_TEXT), 4);
        win->tp[1]->setNumber(win->tp[1], 0, totals.fields.tp);
        win->tp[1]->setRightAlign(win->tp[1], 1);
    } else {
        for (i = 0; i < 2; i++) {
            win->tp[i]->setVisible(win->tp[i], 0);
        }
    }
}

/*
 * Shows the selected partner's stats, in colour where they differ from
 * before (NULL: just shows them). The MP label is given win->mp[3] (that
 * is, slashes[0]) as its window, as in the original. The match depends on
 * the stats and resistances being read as *(totals.fields.battle + i).
 */
void STGTRAIN_showStatChanges(TrainScreen *screen, PartnerTotals *before) {
    TrainScreenWindows *win = screen->children;
    PartnerTotals totals;
    s32 partner;
    s32 i;

    if (before == NULL) {
        STGTRAIN_showVitals(screen, win, 1);
        STGTRAIN_showBattleStats(screen, win, 1);
        return;
    }
    partner = GAME.funcs.getPartyMember(screen->partner);
    GAME.funcs.getPartnerStats(partner);
    GAME.funcs.computeStats(partner, &totals);
    win->hp[0]->setString(win->hp[0], FILE_CACHE.load(STGTRAIN_TEXT), 2);
    win->hp[1]->setNumber(win->hp[1], 0, totals.fields.hp);
    win->hp[2]->setNumber(win->hp[2], 0, totals.fields.maxHp);
    for (i = 1; i < 3; i++) {
        win->hp[i]->setRightAlign(win->hp[i], 1);
    }
    if (before->fields.maxHp < totals.fields.maxHp) {
        win->hp[2]->setPalette(win->hp[2], PALETTE_BLUE);
    } else if (totals.fields.maxHp < before->fields.maxHp) {
        win->hp[2]->setPalette(win->hp[2], PALETTE_RED);
    } else {
        win->hp[2]->setPalette(win->hp[2], PALETTE_WHITE);
    }
    win->mp[0]->setString(win->mp[i], FILE_CACHE.load(STGTRAIN_TEXT), 3);
    win->mp[1]->setNumber(win->mp[1], 0, totals.fields.mp);
    win->mp[2]->setNumber(win->mp[2], 0, totals.fields.maxMp);
    for (i = 1; i < 3; i++) {
        win->mp[i]->setRightAlign(win->mp[i], 1);
    }
    if (before->fields.maxMp < totals.fields.maxMp) {
        win->mp[2]->setPalette(win->mp[2], PALETTE_BLUE);
    } else if (totals.fields.maxMp < before->fields.maxMp) {
        win->mp[2]->setPalette(win->mp[2], PALETTE_RED);
    } else {
        win->mp[2]->setPalette(win->mp[2], PALETTE_WHITE);
    }
    for (i = 0; i < 2; i++) {
        win->slashes[i]->setString(win->slashes[i], FILE_CACHE.load(STGTRAIN_TEXT), 0x43);
    }
    if (totals.fields.lowered[0] != 0) {
        win->stats[0]->setPalette(win->stats[0], PALETTE_PURPLE);
    }
    if (totals.fields.lowered[1] != 0) {
        win->stats[1]->setPalette(win->stats[1], PALETTE_PURPLE);
    }
    if (totals.fields.lowered[2] != 0) {
        win->stats[4]->setPalette(win->stats[4], PALETTE_PURPLE);
    }
    for (i = 0; i < 6; i++) {
        win->stats[i]->setNumber(win->stats[i], 0, *(totals.fields.battle + i));
        win->stats[i]->setRightAlign(win->stats[i], 1);
        if (before->fields.battle[i] < *(totals.fields.battle + i)) {
            win->stats[i]->setPalette(win->stats[i], PALETTE_BLUE);
        } else if (*(totals.fields.battle + i) < before->fields.battle[i]) {
            win->stats[i]->setPalette(win->stats[i], PALETTE_RED);
        }
    }
    for (i = 0; i < 7; i++) {
        win->resistances[i]->setNumber(win->resistances[i], 0, *(totals.fields.resist + i));
        win->resistances[i]->setRightAlign(win->resistances[i], 1);
        if (before->fields.resist[i] < *(totals.fields.resist + i)) {
            win->resistances[i]->setPalette(win->resistances[i], PALETTE_BLUE);
        } else if (*(totals.fields.resist + i) < before->fields.resist[i]) {
            win->resistances[i]->setPalette(win->resistances[i], PALETTE_RED);
        }
    }
}

/* Advances the selected partner's waiting animation, a frame every 13 ticks */
static inline void STGTRAIN_animatePartner(TrainScreen *screen, s32 partner) {
    if (GFX.funcs.getTime() - screen->time >= 0xD) {
        screen->time = GFX.funcs.getTime();
        screen->frame++;
        if (screen->frame >= 7 || STGTRAIN_waitAnims[partner][screen->frame] == -1) {
            screen->frame = 0;
        }
    }
}

/* Draws the screen: the panels, the party's sprites and the sign */
void STGTRAIN_drawScreen(TrainScreen *screen) {
    SpriteDrawer sprite;
    s32 i;
    s32 partner;

    for (i = 0; i < screen->partyCount; i++) {
        if (GFX.funcs.getTime() - screen->anims[i].time >= 0xD) {
            screen->anims[i].time = GFX.funcs.getTime();
            screen->anims[i].frame++;
            partner = GAME.funcs.getPartyMember(i);
            if (screen->anims[i].frame >= 7 || STGTRAIN_waitAnims[partner][screen->anims[i].frame] == -1) {
                screen->anims[i].frame = 0;
            }
        }
    }
    partner = GAME.funcs.getPartyMember(screen->partner);
    STGTRAIN_animatePartner(screen, partner);
    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layerId, screen->depth);
    if (screen->panels[0].level != 0) {
        if (screen->panels[0].level != ONE) {
            sprite.setScale(screen->panels[0].level, ONE, ONE);
            sprite.setPivot(0, 0x2B);
        } else {
            sprite.setTexture(0x240, 0x100);
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), STGTRAIN_waitAnims[partner][screen->frame], 0x10, 0x16);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xE, 0, 0xF);
        sprite.setTexture(0x240, 0x100);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x1F, 0, 0xF);
    }
    if (screen->panels[1].level != 0) {
        if (screen->panels[1].level != ONE) {
            sprite.setScale(screen->panels[1].level, ONE, ONE);
            sprite.setPivot(0, 0x7F);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xF, 0, 0x4B);
        sprite.setTexture(0x240, 0x100);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x20, 0, 0x4B);
    }
    if (screen->panels[2].level != 0) {
        if (screen->panels[2].level != ONE) {
            sprite.setScale(screen->panels[2].level, ONE, ONE);
            sprite.setPivot(0, 0xC1);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x21, 0, 0xB6);
    }
    if (screen->panels[5].level != 0) {
        for (i = 0; i < screen->partyCount; i++) {
            if (screen->panels[5].level != ONE) {
                sprite.setScale(screen->panels[5].level, ONE, ONE);
                sprite.setPivot(i * 0x30 + 0xB4, 0x8A);
            }
            partner = GAME.funcs.getPartyMember(i);
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), STGTRAIN_waitAnims[partner][screen->anims[i].frame], i * 0x30 + 0xA4, 0x81);
        }
    }
    if (screen->cursorShown != 0) {
        if (GFX.funcs.getTime() - screen->cursorTime >= 0xB) {
            screen->cursorTime = GFX.funcs.getTime();
            screen->cursorClut++;
            if (screen->cursorClut >= 4) {
                screen->cursorClut = 0;
            }
        }
        sprite.setLayerId(screen->layerId, screen->depth - 2);
        sprite.setTexture(0x140, 0);
        sprite.setClutRow(screen->cursorClut);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xD, screen->partner * 0x30 + 0xA4, 0x65);
        sprite.setClutRow(0);
    }
    if (screen->panels[6].level != 0) {
        if (screen->panels[6].level != ONE) {
            sprite.setScale(screen->panels[6].level, ONE, ONE);
            sprite.setPivot(0x140, 0x8A);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setLayerId(screen->layerId, screen->depth - 1);
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x10, 0x8F, 0x5F);
        sprite.setLayerId(screen->layerId, screen->depth);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x11, 0x8F, 0x5F);
        sprite.setTexture(0x240, 0x100);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x24, 0x8F, 0x5F);
    }
    if (screen->panels[3].level != 0) {
        if (screen->panels[3].level != ONE) {
            sprite.setScale(screen->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x1D);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x27, 0x8F, 0xF);
    }
    if (screen->panels[4].level != 0) {
        if (screen->panels[4].level != ONE) {
            sprite.setScale(screen->panels[4].level, ONE, ONE);
            sprite.setPivot(0x140, 0x4E);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x22, 0x92, 0x43);
    }
    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layerId, 7);
    sprite.setTexture(0x240, 0x100);
    if (screen->bgTick != 0) {
        screen->bgScroll++;
        screen->bgScroll = screen->bgScroll < 0x60 ? screen->bgScroll : 0;
        screen->bgTick = 0;
    } else {
        screen->bgTick = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), screen->bgSprite, screen->bgScroll, screen->bgScroll);
}

/* Moves the cursor over the party (left, right), showing the partner's
   stats, and picks the one to train (cross) or leaves (triangle) */
static inline void STGTRAIN_pickPartner(TrainScreen *screen, TrainScreenWindows *win) {
    s32 partner;

    partner = screen->partner;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        screen->partner--;
        if (screen->partner < 0) {
            screen->partner = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        screen->partner++;
        if (screen->partner > screen->partyCount - 1) {
            screen->partner = screen->partyCount - 1;
        }
    }
    if (partner != screen->partner) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STGTRAIN_showVitals(screen, win, 1);
        STGTRAIN_showBattleStats(screen, win, 1);
        STGTRAIN_showTp(screen, win, 1);
        screen->frame = 0;
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->substate = 0x14;
        if (win->menu != NULL) {
            win->menu->state = TASK_KILL;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->substate = 0x32;
    }
}

/* Closes the partner pick: hides the cursor, the prompt and the help, and
   closes their panels */
static inline void STGTRAIN_closePartnerPick(TrainScreen *screen, TrainScreenWindows *win) {
    STGTRAIN_state.startFade(&screen->panels[6], 0);
    screen->cursorShown = 0;
    STGTRAIN_state.startFade(&screen->panels[4], 0);
    win->prompt->setVisible(win->prompt, 0);
    STGTRAIN_state.startFade(&screen->panels[3], 0);
    win->help->setVisible(win->help, 0);
}

/* Updates the fades of the partner pick's panels; whether they are done */
static inline s32 STGTRAIN_partnerPickClosed(TrainScreen *screen) {
    STGTRAIN_state.updateFade(&screen->panels[6]);
    STGTRAIN_state.updateFade(&screen->panels[4]);
    return STGTRAIN_state.updateFade(&screen->panels[3]);
}

/* Waits for the session: once it is training (its substate 0x23), ends the
   last result first, then makes the next one with the TP shown; once it is
   done, goes back to the menu */
static inline void STGTRAIN_waitSession(TrainScreen *screen, TrainScreenWindows *win) {
    if (win->session->substate == 0x23) {
        if (win->result != NULL) {
            win->result->state = TASK_KILL;
        } else {
            screen->substate = 0x23;
            STGTRAIN_showTp(screen, win, 1);
        }
    } else if (win->session->state == TASK_DONE) {
        win->session->state = TASK_KILL;
        screen->substate = 0xA;
    }
}

/* Starts leaving the screen: fades it out and closes every panel */
static inline void STGTRAIN_startLeaving(TrainScreen *screen, TrainScreenWindows *win) {
    s32 i;

    win->fade = STGTRAIN_createFader();
    win->fade->start(win->fade, 0, 30);
    screen->panels[5].level = 0;
    for (i = 0; i < 3; i++) {
        STGTRAIN_state.startFade(&screen->panels[i], 0);
    }
    STGTRAIN_showVitals(screen, win, 0);
    STGTRAIN_showBattleStats(screen, win, 0);
    STGTRAIN_showTp(screen, win, 0);
    STGTRAIN_closePartnerPick(screen, win);
}

/* Once the vitals' panel is open (with the help's), shows the vitals and
   the help, and opens the battle stats' and the prompt's panels */
static inline void STGTRAIN_openBattleStats(TrainScreen *screen, TrainScreenWindows *win) {
    STGTRAIN_state.updateFade(&screen->panels[3]);
    if (STGTRAIN_state.updateFade(&screen->panels[0])) {
        STGTRAIN_showVitals(screen, win, 1);
        win->help->setString(win->help, FILE_CACHE.load(STGTRAIN_TEXT), 5);
        STGTRAIN_state.startFade(&screen->panels[1], 1);
        STGTRAIN_state.startFade(&screen->panels[4], 1);
        screen->substate++;
    }
}

/* Once the battle stats' panel is open, shows them and the prompt, and
   opens the TP's panel and the party's window */
static inline void STGTRAIN_openTp(TrainScreen *screen, TrainScreenWindows *win) {
    STGTRAIN_state.updateFade(&screen->panels[4]);
    if (STGTRAIN_state.updateFade(&screen->panels[1])) {
        STGTRAIN_showBattleStats(screen, win, 1);
        win->prompt->setString(win->prompt, FILE_CACHE.load(STGTRAIN_TEXT), 6);
        STGTRAIN_state.startFade(&screen->panels[2], 1);
        STGTRAIN_state.startFade(&screen->panels[6], 1);
        screen->substate++;
    }
}

/* Once the TP's panel is open, shows the TP and opens the party's panel */
static inline void STGTRAIN_openParty(TrainScreen *screen, TrainScreenWindows *win) {
    STGTRAIN_state.updateFade(&screen->panels[6]);
    if (STGTRAIN_state.updateFade(&screen->panels[2])) {
        STGTRAIN_showTp(screen, win, 1);
        STGTRAIN_state.startFade(&screen->panels[5], 1);
        screen->substate++;
    }
}

/* Waits for the menu to close: with a training picked it is gone, and the
   training's session starts; without one (triangle) it is done, and the
   partner pick reopens */
static inline void STGTRAIN_waitMenu(TrainScreen *screen, TrainScreenWindows *win) {
    if (win->menu == NULL) {
        screen->substate = 0x1E;
    } else if (win->menu->state == TASK_DONE) {
        screen->training = 0;
        win->menu->state = TASK_KILL;
        screen->substate = 0x19;
    }
}

/* Updates the fades of every panel as the screen closes; whether they are
   done */
static inline s32 STGTRAIN_screenClosed(TrainScreen *screen) {
    s32 i;

    for (i = 0; i < 3; i++) {
        STGTRAIN_state.updateFade(&screen->panels[i]);
    }
    return STGTRAIN_partnerPickClosed(screen);
}

/* The training screen: opens the panels, picks the partner, runs the menu
   and the trainings, and closes everything when leaving */
void STGTRAIN_runScreen(TrainScreen *screen, TrainScreenWindows *win) {
    switch (screen->substate) {
    case 0:
    default:
        STGTRAIN_state.startFade(&screen->panels[0], 1);
        STGTRAIN_state.startFade(&screen->panels[3], 1);
        screen->substate++;
        break;
    case 1:
        STGTRAIN_openBattleStats(screen, win);
        break;
    case 2:
        STGTRAIN_openTp(screen, win);
        break;
    case 3:
        STGTRAIN_openParty(screen, win);
        break;
    case 4:
        if (STGTRAIN_state.updateFade(&screen->panels[5])) {
            screen->cursorShown = 1;
            screen->substate++;
        }
        break;
    case 5:
        STGTRAIN_pickPartner(screen, win);
        break;
    case 0xA:
        if (win->menu == NULL) {
            win->menu = STGTRAIN_createMenu(screen);
            screen->substate++;
        }
        break;
    case 0xB:
        STGTRAIN_waitMenu(screen, win);
        break;
    case 0x14:
        screen->panels[5].level = 0;
        STGTRAIN_closePartnerPick(screen, win);
        screen->substate++;
        break;
    case 0x15:
        if (STGTRAIN_partnerPickClosed(screen)) {
            screen->substate = 0xA;
        }
        break;
    case 0x19:
        STGTRAIN_state.startFade(&screen->panels[3], 1);
        screen->substate++;
        break;
    case 0x1A:
        if (STGTRAIN_state.updateFade(&screen->panels[3])) {
            win->help->setString(win->help, FILE_CACHE.load(STGTRAIN_TEXT), 5);
            STGTRAIN_state.startFade(&screen->panels[4], 1);
            screen->substate++;
        }
        break;
    case 0x1B:
        if (STGTRAIN_state.updateFade(&screen->panels[4])) {
            win->prompt->setString(win->prompt, FILE_CACHE.load(STGTRAIN_TEXT), 6);
            STGTRAIN_state.startFade(&screen->panels[6], 1);
            screen->substate++;
        }
        break;
    case 0x1C:
        if (STGTRAIN_state.updateFade(&screen->panels[6])) {
            STGTRAIN_state.startFade(&screen->panels[5], 1);
            screen->substate = 4;
        }
        break;
    case 0x1E:
        if (win->session == NULL) {
            win->session = STGTRAIN_createSession(screen);
            screen->substate++;
            STGTRAIN_state.requestFile(screen->training);
        }
        break;
    case 0x1F:
        STGTRAIN_waitSession(screen, win);
        break;
    case 0x23:
        if (STGTRAIN_state.getFile() != NULL && win->result == NULL) {
            win->result = STGTRAIN_createResult(screen, GAME.funcs.getPartyMember(screen->partner), screen->training);
            screen->substate++;
        }
        break;
    case 0x24:
        if (win->result == NULL) {
            win->session->finish(win->session);
            screen->substate++;
        }
        break;
    case 0x25:
        if (win->session == NULL) {
            STGTRAIN_showBattleStats(screen, win, 1);
            screen->substate = 0x19;
        }
        break;
    case 0x32:
        STGTRAIN_startLeaving(screen, win);
        screen->substate++;
        break;
    case 0x33:
        if (STGTRAIN_screenClosed(screen)) {
            screen->substate++;
        }
        break;
    case 0x34:
        if (win->fade->state == TASK_DONE) {
            screen->state = TASK_KILL;
        }
        break;
    }
}

/* The training screen's update: loads its files, then runs the menu */
void STGTRAIN_updateScreen(TrainScreen *screen, TrainScreenWindows *win) {
    s32 i;

    switch (screen->state) {
    case TASK_INIT:
    default:
        switch (screen->substate) {
        case 0:
        default:
            STGTRAIN_state.loadImages();
            FILE_CACHE.request(STGTRAIN_TEXT);
            SOUND.loadBank(0x21);
            screen->substate++;
            break;
        case 1:
            if (FILE_CACHE.isLoading(STGTRAIN_TEXT) == 0) {
                screen->nextState(screen);
                STGTRAIN_createScreenWindows(screen, win);
                for (i = 0; i < 3; i++) {
                    if (GAME.funcs.getPartyMember(i) != -1) {
                        screen->partyCount++;
                    }
                }
                for (i = 2; i >= 0; i--) {
                    screen->panels[i].duration = 10;
                }
                screen->panels[3].duration = 10;
                screen->panels[4].duration = 10;
                screen->panels[6].duration = 10;
                screen->panels[5].duration = 8;
                screen->setState(screen, TASK_DONE);
            }
            break;
        }
        break;
    case TASK_RUN:
        STGTRAIN_runScreen(screen, win);
        STGTRAIN_drawScreen(screen);
        break;
    case TASK_DONE:
        if (SOUND.isLoading() == 0) {
            SOUND.playSound(MUSIC(0x21, 2));
            screen->setState(screen, TASK_RUN);
        }
        break;
    case TASK_KILL:
        SOUND.stopSound(MUSIC(0x21, 2));
        GAME.funcs.requestMode(GAME.fieldMode, 0);
        break;
    }
}

/* Creates the training screen */
TrainScreen *STGTRAIN_createScreen(void) {
    TrainScreen *screen = createTask(STGTRAIN_updateScreen, sizeof(TrainScreen), 0x80);
    s32 sign;

    screen->showStats = STGTRAIN_showStatChanges;
    screen->layerId = SCREEN_LAYER;
    screen->depth = 6;
    /* the gym's sign, by the stage the player came from */
    switch (GAME.funcs.getPrevMode()) {
    case 0x23D: /* South Cape */
        sign = 0x2E;
        break;
    case 0x24B: /* North Wind Wasteland East */
        sign = 0x2F;
        break;
    case 0x267: /* the Legendary Gym */
        sign = 0x30;
        break;
    case 0x28C: /* Central Park, in Amaterasu */
        sign = 0x31;
        break;
    case 0x2AA: /* South Cape, in Amaterasu */
        sign = 0x32;
        break;
    case 0x2B5: /* North Wind Wasteland East, in Amaterasu */
        sign = 0x33;
        break;
    case 0x2CF: /* the Legendary Gym, in Amaterasu */
        sign = 0x34;
        break;
    case 0x21D: /* Central Park */
    default:
        sign = 0x2D;
        break;
    }
    screen->bgSprite = sign;
    return screen;
}

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 6
#include "shared/create_fader.inc.c"

/* The partners' sprites while they wait, by partner: -1 ends a loop */
s32 STGTRAIN_waitAnims[8][7] = {
    {7, 8, 9, 10, 9, 8, -1},
    {14, 15, 16, 15, -1, -1, -1},
    {11, 12, 13, 12, -1, -1, -1},
    {3, 4, 5, 6, 5, 4, -1},
    {25, 26, 27, 28, 27, 26, -1},
    {0, 1, 2, 1, -1, -1, -1},
    {17, 18, 19, 20, 19, 18, -1},
    {21, 22, 23, 24, 23, 22, -1},
};
