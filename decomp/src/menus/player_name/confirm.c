/* The confirmation page: the name and the partners chosen, yes or no, and
   the progress */

#include "menus/player_name.h"

/* Creates the confirmation's windows: the title, the name, the partners'
   name, the message, yes and no with their cursor, and the progress */
void STPLNMET_createConfirmWindows(NameConfirm *task, NameConfirmWindows *windows) {
    windows->title = createTextWindow(task->layer, 1, 0x20, 0x1A);
    windows->title->setPalette(windows->title, PALETTE_GREEN);
    windows->name = createTextWindow(task->layer, 1, 0x61, 0x42);
    windows->name->setPalette(windows->name, PALETTE_BLUE);
    windows->pack = createTextWindow(task->layer, 1, 0x40, 0x60);
    windows->pack->setPalette(windows->pack, PALETTE_BLUE);
    windows->message = createTextWindow(task->layer, 1, 0x5A, 0xA9);
    windows->message->setPalette(windows->message, PALETTE_BLUE);
    windows->yes = createTextWindow(task->layer, 1, 0x6B, 0xB9);
    windows->yes->setPalette(windows->yes, PALETTE_BLUE);
    windows->no = createTextWindow(task->layer, 1, 0x6B, 0xC7);
    windows->no->setPalette(windows->no, PALETTE_BLUE);
    windows->cursor = createCursor(task->layer, task->depth - 2, 0x5A, 0xB9);
    windows->cursor->setPalette(windows->cursor, PALETTE_BLUE);
    windows->cursor->setVisible(windows->cursor, 0);
    windows->progress = createTextWindow(task->layer, 1, 0xE3, 0xA9);
    windows->progress->setPalette(windows->progress, PALETTE_BLUE);
}

/* Shows the confirmation's windows with the name typed (without its leading
   spaces) and the partners chosen, the cursor on yes or no; or hides them */
void STPLNMET_showConfirmWindows(NameConfirm *task, NameConfirmWindows *windows, s32 show) {
    u16 *name;
    s32 i;

    if (show != 0) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x15);
        name = task->name;
        for (i = 0; name[i] == SJIS_SPACE; i++) {
        }
        windows->name->setText(windows->name, &name[i]);
        windows->pack->setString(windows->pack, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), task->choice + 10);
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x16);
        windows->yes->setString(windows->yes, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x17);
        windows->no->setString(windows->no, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x18);
        windows->cursor->setPos(windows->cursor, 0x5A, task->cursor * 14 + 0xB9);
        windows->cursor->setVisible(windows->cursor, 1);
    } else {
        windows->title->setVisible(windows->title, 0);
        windows->name->setVisible(windows->name, 0);
        windows->pack->setVisible(windows->pack, 0);
        windows->message->setVisible(windows->message, 0);
        windows->yes->setVisible(windows->yes, 0);
        windows->no->setVisible(windows->no, 0);
        windows->cursor->setVisible(windows->cursor, 0);
        windows->progress->setVisible(windows->progress, 0);
    }
}

/* Draws the confirmation: the title panel with its glow, the three partners
   chosen (animated) and the panels, as they open, and the progress bar
   while the progress counts up */
void STPLNMET_drawConfirm(NameConfirm *task) {
    SpriteDrawer sprite;
    SVECTOR out[4];
    SVECTOR in[4];
    s32 *partners;
    PartnerAnim *anim;
    POLY_G4 *poly;
    u_long *ot;
    Layer *layer;
    s32 i;
    s32 j;

    partners = STPLNMET_funcs.choices[task->choice];
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0x100);
    if (GFX.funcs.getTime() - task->clutTime > 4) {
        task->clutTime = GFX.funcs.getTime();
        task->clutRow++;
        if (task->clutRow >= 14) {
            task->clutRow = 0;
        }
    }
    if (task->tweenA.level != 0) {
        if (task->tweenA.level != ONE) {
            sprite.setScale(task->tweenA.level, ONE, ONE);
            sprite.setPivot(0x37, 0x42);
        }
        sprite.setLayerId(task->layer, task->depth);
        sprite.setClutRow(task->clutRow);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 6, 0x37, 0x2F);
        sprite.setClutRow(0);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 2, 0x37, 0x2F);
    }
    if (GFX.funcs.getTime() - task->frameTime > 12) {
        task->frameTime = GFX.funcs.getTime();
        for (i = 0; i < 3; i++) {
            anim = &STPLNMET_funcs.anims[partners[i]];
            task->frames[i]++;
            if (task->frames[i] >= 8 || anim->frames[task->frames[i]] == -1) {
                task->frames[i] = 0;
            }
        }
    }
    if (task->tweenB.level != 0) {
        sprite.setLayerId(task->layer, task->depth);
        if (task->tweenB.level == ONE) {
            sprite.setTexture(0x140, 0x100);
            for (i = 0; i < 3; i++) {
                anim = &STPLNMET_funcs.anims[partners[i]];
                sprite.draw(FILE_CACHE.getEntry(PLNMET_BANK), anim->frames[task->frames[i]], 0x57 + i * 0x38, 0x6C);
            }
        }
        sprite.setTexture(0x280, 0x100);
        if (task->tweenB.level != ONE) {
            sprite.setScale(ONE, task->tweenB.level, ONE);
            sprite.setPivot(0xA0, 0x78);
        }
        sprite.setLayerId(task->layer, task->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 10, 0x37, 0x59);
        sprite.setLayerId(task->layer, task->depth);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 9, 0x37, 0x59);
        if (task->tweenB.level != ONE) {
            sprite.setPivot(0xA0, 0xBE);
        }
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 1, 0, 0xA2);
    }
    if (task->substate == 1 || task->substate == 2) {
        sprite.setLayerId(task->layer, task->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(PLNMET_MENU), 0x37, 0x51, 0xBF);
        task->scale.vy = task->scale.vz = ONE;
        if (task->progress == 100) {
            task->scale.vx = ONE;
        } else {
            task->scale.vx = task->progress * 41;
        }
        RotMatrixYXZ_gte(&task->rot, &task->matrix);
        ScaleMatrix(&task->matrix, &task->scale);
        layer = GFX.funcs.getLayer(task->layer);
        ot = (u_long *)layer->getOtEntry(layer, task->depth - 1);
        poly = GFX.funcs.getPrim();
        setPolyG4(poly);
        setRGB0(poly, 0x7F, 0x32, 0xF2);
        setRGB1(poly, 0xD1, 0x2F, 0xDE);
        setRGB2(poly, 0x7F, 0x32, 0xF2);
        setRGB3(poly, 0xD1, 0x2F, 0xDE);
        in[1].vx = in[3].vx = 0x98;
        in[0].vy = in[1].vy = 0xBC;
        in[0].vx = in[2].vx = 0;
        in[2].vy = in[3].vy = 0xC8;
        in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
        for (j = 0; j < 4; j++) {
            ApplyMatrixSV(&task->matrix, &in[j], &out[j]);
            out[j].vx += 0x54;
            out[j].vy += 6;
        }
        setXY4(poly, out[0].vx, out[0].vy, out[1].vx, out[1].vy, out[2].vx, out[2].vy, out[3].vx, out[3].vy);
        addPrim(ot, poly);
        GFX.funcs.setPrim(poly + 1);
    }
}

/* The confirmation's states: yes or no (substate 200 for no or triangle); yes
   counts the progress up to 100, then waits 120 frames before substate 100.
   Once closing, closes the panels and ends */
void STPLNMET_runConfirm(NameConfirm *task, NameConfirmWindows *windows) {
    s32 old;

    switch (task->substate) {
    case 0:
    default:
        old = task->cursor;
        if (PAD_PRESSED(PAD_UP)) {
            task->cursor = 0;
        } else if (PAD_PRESSED(PAD_DOWN)) {
            task->cursor = 1;
        }
        if (old != task->cursor) {
            SOUND.playSound(SOUND_CURSOR);
            windows->cursor->setPos(windows->cursor, 0x5A, task->cursor * 14 + 0xB9);
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            if (task->cursor == 0) {
                task->substate = 1;
                task->progress = 0;
                windows->yes->setVisible(windows->yes, 0);
                windows->no->setVisible(windows->no, 0);
                windows->cursor->setVisible(windows->cursor, 0);
                windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x1D);
                windows->progress->setString(windows->progress, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x19);
                windows->progress->setNumber(windows->progress, 1, task->progress);
                windows->progress->setRightAlign(windows->progress, 1);
            } else {
                task->substate = 200;
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            task->substate = 200;
        }
        break;
    case 1:
        task->progress++;
        if (task->progress > 100) {
            task->progress = 100;
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_ONLINE)), 0x1E);
            SOUND.playSound(SOUND_SYSTEM05);
            task->nextSubstate(task);
        } else {
            SOUND.playSound(SOUND_COUNT);
        }
        windows->progress->setNumber(windows->progress, 1, task->progress);
        windows->progress->setRightAlign(windows->progress, 1);
        break;
    case 2:
        task->step++;
        if (task->step > 120) {
            task->substate = 100;
        }
        break;
    case 50:
        STPLNMET_funcs.startFade(&task->tweenA, 0);
        STPLNMET_funcs.startFade(&task->tweenB, 0);
        STPLNMET_showConfirmWindows(task, windows, 0);
        task->substate++;
        break;
    case 51:
        STPLNMET_funcs.updateFade(&task->tweenA);
        if (STPLNMET_funcs.updateFade(&task->tweenB) != 0) {
            task->state = TASK_KILL;
        }
        break;
    case 100:
    case 200:
        break;
    }
}

/* Hides the confirmation, or shows it again (confirm->hide) with its cursor on
   yes and the partners' animations from their first frame */
void STPLNMET_hideConfirm(NameConfirm *task, s32 hide) {
    NameConfirmWindows *windows = task->children;
    s32 i;

    if (hide != 0) {
        task->state = TASK_DONE;
        STPLNMET_showConfirmWindows(task, windows, 0);
    } else {
        task->state = TASK_RUN;
        task->substate = 0;
        for (i = 2; i >= 0; i--) {
            task->frames[i] = 0;
        }
        task->cursor = 0;
        STPLNMET_showConfirmWindows(task, windows, 1);
    }
}

/* Starts closing the confirmation (confirm->close): its panels close, then it ends */
void STPLNMET_closeConfirm(NameConfirm *task) {
    task->substate = 50;
}

/* The confirmation's task: creates its windows and waits hidden with its
   panels open, then runs and draws it while shown */
void STPLNMET_updateConfirm(NameConfirm *task, NameConfirmWindows *windows) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        STPLNMET_createConfirmWindows(task, windows);
        task->tweenA.duration = 10;
        task->tweenA.level = ONE;
        task->tweenB.duration = 10;
        task->tweenB.level = ONE;
        STPLNMET_hideConfirm(task, 1);
        break;
    case TASK_RUN:
        STPLNMET_runConfirm(task, windows);
        STPLNMET_drawConfirm(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the confirmation of the name and the partners (task), hidden */
NameConfirm *STPLNMET_createConfirm(PlayerNameScreen *screen) {
    NameConfirm *task = createTask(STPLNMET_updateConfirm, sizeof(NameConfirm), sizeof(NameConfirmWindows));

    task->hide = STPLNMET_hideConfirm;
    task->close = STPLNMET_closeConfirm;
    task->layer = 0x1001;
    task->depth = 6;
    task->screen = screen;
    return task;
}
