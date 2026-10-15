/* The details window of the save under the cursor */

#include "menus/memory_card.h"

/* Starts fading the details window in (info->show) */
void STGMCARD_showInfo(MemCardInfo *info) {
    info->setSubstate(info, 1);
}

/* Hides the details window's text and starts fading it out (info->hide) */
void STGMCARD_hideInfo(MemCardInfo *info) {
    TextWindow **windows;
    s32 i;

    info->setSubstate(info, 2);
    windows = info->children;
    for (i = 0; i < info->childCount; i++, windows++) {
        if (*windows != NULL) {
            (*windows)->setVisible(*windows, 0);
        }
    }
}

/* Fills the details window with the selected save, or hides it */
void STGMCARD_refreshInfo(MemCardInfo *info) {
    MemCardSave *save;
    TextWindow **windows;
    TextWindow **w;
    s32 *partners;
    s32 i;

    save = &info->saves->file.saves[STGMCARD_funcs.slot];
    windows = info->children;
    if (info->shown == 0) {
        for (i = 0, w = windows; i < info->childCount; i++, w++) {
            (*w)->setVisible(*w, 0);
        }
    } else if (save->name[0] == 0) {
        for (i = 0, w = windows; i < info->childCount; i++, w++) {
            (*w)->setString(*w, FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), STGMCARD_detailWindows[i].text);
            if (STGMCARD_detailWindows[i].isNumber != 0) {
                (*w)->setNumber(*w, 1, 0);
                (*w)->setRightAlign(*w, 1);
                (*w)->setPos(*w, STGMCARD_detailWindows[i].x, STGMCARD_detailWindows[i].y);
            }
        }
    } else {
        windows[0]->setString(windows[0], save, -1);
        windows[1]->setString(windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_AREA_NAMES)), save->area);
        windows[2]->setString(windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_SHOP_NAMES)), save->place);
        for (i = 0; i < 2; i++) {
            windows[i + 3]->setString(windows[i + 3], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x15);
        }
        windows[5]->setNumber(windows[5], 0, save->money);
        windows[5]->setRightAlign(windows[5], 1);
        windows[12]->setNumber(windows[12], 0, save->time.hours);
        windows[13]->setNumber(windows[13], 0, save->time.minutes);
        windows[14]->setNumber(windows[14], 0, save->time.seconds);
        for (i = 0, w = &windows[12]; i < 3; i++, w++) {
            (*w)->setRightAlign(*w, 1);
        }
        if (save->time.minutes < 10) {
            windows[17]->setNumber(windows[17], 0, 0);
            windows[17]->setRightAlign(windows[17], 1);
            windows[17]->setPos(windows[17], 0x111, 0xB2);
        } else {
            windows[17]->setVisible(windows[17], 0);
        }
        if (save->time.seconds < 10) {
            windows[18]->setNumber(windows[18], 0, 0);
            windows[18]->setRightAlign(windows[18], 1);
            windows[18]->setPos(windows[18], 0x124, 0xB2);
        } else {
            windows[18]->setVisible(windows[18], 0);
        }
        for (i = 0; i < 2; i++) {
            windows[i + 15]->setString(windows[i + 15], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x13);
        }
        partners = info->saves->file.saves[STGMCARD_funcs.slot].partners;
        for (i = 0; i < 3; i++) {
            windows[i + 6]->setString(windows[i + 6], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x14);
            if (partners[i] - 3 < 0) {
                windows[i + 9]->setString(windows[i + 9], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x23);
            } else {
                windows[i + 9]->setNumber(windows[i + 9], 0, save->levels[i]);
            }
            windows[i + 9]->setRightAlign(windows[i + 9], 1);
            info->frames[i] = 0;
        }
        info->time = 0;
    }
}

/* The details window's task: creates its text windows, fades it in and out,
   and draws the selected save's partners and frame */
void STGMCARD_updateInfo(MemCardInfo *info) {
    SpriteDrawer sprite;
    TextWindow **windows;
    s32 *partners;
    s32 i;
    s32 j;

    switch (info->state) {
    case TASK_INIT:
    default:
        info->nextState(info);
#if VERSION_US
        STGMCARD_detailWindows[0].text = 0x21;
#elif VERSION_EU || VERSION_JP
        STGMCARD_detailWindows[0].text = LANGUAGE != 0 ? 0x21 : 0x20;
#endif
        windows = info->children;
        for (i = 0; i < info->childCount; i++, windows++) {
            if (*windows == NULL) {
                *windows = createTextWindow(info->layer, STGMCARD_detailWindows[i].type, STGMCARD_detailWindows[i].x, STGMCARD_detailWindows[i].y);
            }
            /* the match depends on the unsigned compare, which GCC makes a sltiu */
            if (i < 3U) {
                (*windows)->setPalette(*windows, PALETTE_BLUE);
            }
        }
        info->fade.duration = 8;
        break;
    case TASK_RUN:
        switch (info->substate) {
        case 0:
        default:
            break;
        case 1:
            switch (info->step) {
            case 0:
            default:
                STGMCARD_funcs.startFade(&info->fade, 1);
                info->step++;
                break;
            case 1:
                if (STGMCARD_funcs.updateFade(&info->fade) != 0) {
                    info->setSubstate(info, 0);
                }
                break;
            }
            break;
        case 2:
            switch (info->step) {
            case 0:
            default:
                STGMCARD_funcs.startFade(&info->fade, 0);
                info->shown = 0;
                STGMCARD_refreshInfo(info);
                info->step++;
                break;
            case 1:
                if (STGMCARD_funcs.updateFade(&info->fade) != 0) {
                    info->drawing = 0;
                    info->setSubstate(info, 0);
                }
                break;
            }
            break;
        }
        if (info->drawing == 0) {
            break;
        }
        initSpriteDrawer(&sprite);
        sprite.setTexture(0x280, 0);
        sprite.setLayerId(info->layer, info->depth);
        if (info->fade.level != ONE) {
            sprite.setScale(ONE, info->fade.level, ONE);
            sprite.setPivot(160, 148);
        }
        partners = info->saves->file.saves[STGMCARD_funcs.slot].partners;
        if (GFX.funcs.getTime() - info->time >= 13) {
            info->time = GFX.funcs.getTime();
            for (j = 0; j < 3; j++) {
                if (partners[j] - 3 >= 0) {
                    info->frames[j]++;
                    if (info->frames[j] >= 8) {
                        info->frames[j] = 0;
                    }
                    if (STGMCARD_partnerAnims[partners[j] - 3][info->frames[j]] == -1) {
                        info->frames[j] = 0;
                    }
                }
            }
        }
        for (j = 0; j < 3; j++) {
            if (partners[j] - 3 >= 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), STGMCARD_partnerAnims[partners[j] - 3][info->frames[j]], 154 + j * 52, 133);
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 36, 16, 116);
        sprite.setLayerId(info->layer, info->depth - 1);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 37, 16, 116);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the details window (task) of the save list */
MemCardInfo *STGMCARD_createInfo(MemCardSaves *saves) {
    MemCardInfo *info = createTask(STGMCARD_updateInfo, sizeof(MemCardInfo), 0x4C);

    info->show = STGMCARD_showInfo;
    info->hide = STGMCARD_hideInfo;
    info->refresh = STGMCARD_refreshInfo;
    info->layer = SCREEN_LAYER;
    info->saves = saves;
    info->depth = 1;
    return info;
}
