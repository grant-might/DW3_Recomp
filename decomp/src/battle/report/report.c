/* The report: its windows, its drawing, its run through the partners, the
   money and the item won, and its files. */

#include "battle/report.h"

/* Creates the report's text windows: the exp, its label and the two-line
   message */
void STFGTREP_createWindows(FightReport *report, FightReportChildren *children) {
    TextWindow *window;

    children->exp = createTextWindow(report->layer, 3, 0x42, 0x17);
    children->expLabel = createTextWindow(report->layer, 3, 0x45, 0x17);
    window = createTextWindow(report->layer, 1, 0x14, 0xC2);
    children->message = window;
    window->setLines(window, 2);
}

/* Draws the report's message arrow, background, header and footer */
void STFGTREP_drawReport(FightReport *report) {
    SpriteDrawer sprite;

    if (report->arrowOn != 0) {
        if (GFX.funcs.getTime() - report->arrowTime >= 5) {
            report->arrowTime = GFX.funcs.getTime();
            if (++report->arrowFrame >= 5) {
                report->arrowFrame = 0;
            }
        }
        initSpriteDrawer(&sprite);
        sprite.setTexture(0x140, 0);
        sprite.setLayerId(report->layer, report->depth - 2);
        sprite.setClutRow(report->arrowFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 10, 0x123, 0xD0);
    }
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(report->layer, report->depth);
    if (report->bgSkip != 0) {
        report->bgScroll++;
        report->bgScroll = report->bgScroll < 0x60 ? report->bgScroll : 0;
        report->bgSkip = 0;
    } else {
        report->bgSkip = 1;
    }
    sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x1E, report->bgScroll, report->bgScroll);
    sprite.setLayerId(report->layer, report->depth - 1);
    if (report->header.level != 0) {
        if (report->header.level != 0x1000) {
            sprite.setScale(report->header.level, 0x1000, 0x1000);
            sprite.setPivot(0, 0x18);
        }
        sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x21, 0, 0xF);
    }
    if (report->footer.level != 0) {
        sprite.setScale(0x1000, report->footer.level, 0x1000);
        if (report->footer.level != 0x1000) {
            sprite.setPivot(0xA0, 0xCE);
        }
        sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x1D, 0xB, 0xBC);
    }
}

/* Selects the next partner that got exp and fades the header in; after the
   last one goes on to the money */
static inline void selectNextPartner(FightReport *report, FightReportChildren *children) {
    if (report->step < report->count) {
        if (report->exp[report->step] == 0) {
            report->step++;
        } else {
            STFGTREP_funcs.startFade(&report->header, 1);
            children->partners[report->step]->select(children->partners[report->step]);
            report->substate++;
        }
    } else {
        report->setSubstate(report, 0x14);
    }
}

/* Shows the current partner's exp in the message and in the exp window */
static inline void showExp(FightReport *report, FightReportChildren *children) {
    children->message->setString(children->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 2);
    children->message->setNumber(children->message, 1, report->exp[report->step]);
    children->message->setTypeDelay(children->message, 6);
    children->exp->setNumber(children->exp, 0, report->exp[report->step]);
    children->exp->setRightAlign(children->exp, 1);
    children->expLabel->setString(children->expLabel, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 1);
}

/* While the message types, cross shows its whole page; at a page end the
   arrow shows until cross, which plays the confirm sound */
static inline void turnMessagePage(FightReport *report, FightReportChildren *children) {
    if (children->message->isWaitingForButton(children->message) != 0) {
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
        } else {
            report->arrowOn = 1;
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        children->message->showPage(children->message);
    }
}

/* Fades the header out and hides the exp once the partner's raise is shown,
   moving on to the next partner */
static inline void hideExp(FightReport *report, FightReportChildren *children) {
    STFGTREP_funcs.startFade(&report->header, 0);
    children->exp->setVisible(children->exp, 0);
    children->expLabel->setVisible(children->expLabel, 0);
    report->substate++;
    report->step++;
}

/* Gives the money the battle won, a fifth more with item 0x142 equipped,
   and shows it in the message */
static inline void giveMoney(FightReport *report, FightReportChildren *children) {
    PartnerStats *stats;
    s32 money;

    stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(BATTLE_RESULT.member));
    money = STFGTREP_rewards[BATTLE_RESULT.battle].money;
    if (stats->equip[4] == 0x142 || stats->equip[5] == 0x142) {
        money += STFGTREP_rewards[BATTLE_RESULT.battle].money / 5;
    }
    children->message->setString(children->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 10);
    children->message->setNumber(children->message, 1, money);
    children->message->setTypeDelay(children->message, 6);
    report->substate = 0x19;
    report->step = 0;
    GAME.money += money;
    if (GAME.money > 9999999) {
        GAME.money = 9999999;
    }
}

/* Gives the item the battle won, up to 99, and shows it in the message;
   with none the report ends */
static inline void giveItem(FightReport *report, FightReportChildren *children) {
    if (BATTLE_RESULT.item != 0) {
        children->message->setString(children->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 11);
        children->message->setSubString(children->message, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), BATTLE_RESULT.item, 1);
        children->message->setTypeDelay(children->message, 6);
        report->substate = 0x19;
        report->step = 1;
        GAME.items[BATTLE_RESULT.item]++;
        if (GAME.items[BATTLE_RESULT.item] >= 100) {
            GAME.items[BATTLE_RESULT.item] = 99;
        }
    } else {
        report->substate = 0x32;
    }
}

/* Starts fading the screen and the footer out and hides the message */
static inline void startFadeOut(FightReport *report, FightReportChildren *children) {
    children->fade = STFGTREP_createFader();
    children->fade->start(children->fade, 0, 30);
    STFGTREP_funcs.startFade(&report->footer, 0);
    children->message->setVisible(children->message, 0);
    report->substate++;
}

/* The report while it runs: shows the panels, then for each partner that
   got exp its message and the raise (selectNextPartner, showExp, hideExp),
   then the money and the item won (giveMoney, giveItem), and fades out
   (startFadeOut) */
void STFGTREP_runReport(FightReport *report, FightReportChildren *children) {
    s32 shown;
    s32 hidden;
    s32 i;
    s32 j;

    switch (report->substate) {
    case 1:
        for (i = 0; i < report->count; i++) {
            children->partners[i]->show(children->partners[i]);
        }
        /* fallthrough */
    case 0:
    default:
        report->substate++;
        break;
    case 4:
        shown = 1;
        for (j = 0; j < report->count; j++) {
            if (children->partners[j]->substate != 5) {
                shown = 0;
                break;
            }
        }
        if (shown) {
            STFGTREP_funcs.startFade(&report->footer, 1);
            report->substate++;
        }
        break;
    case 5:
        if (STFGTREP_funcs.updateFade(&report->footer) != 0) {
            report->substate++;
        }
        break;
    case 6:
        selectNextPartner(report, children);
        break;
    case 7:
        if (STFGTREP_funcs.updateFade(&report->header) != 0) {
            showExp(report, children);
            report->substate++;
        }
        break;
    case 8:
        if (children->message->isFinished(children->message) != 0) {
            report->arrowOn = 0;
            children->message->setVisible(children->message, 0);
            report->substate = 10;
        } else {
            turnMessagePage(report, children);
        }
        break;
    case 10:
        children->partners[report->step]->raise(children->partners[report->step]);
        report->substate++;
        break;
    case 11:
        if (children->partners[report->step]->substate == 5) {
            hideExp(report, children);
        }
        break;
    case 12:
        if (STFGTREP_funcs.updateFade(&report->header) != 0) {
            report->substate = 6;
        }
        break;
    case 0x14:
        giveMoney(report, children);
        break;
    case 0x15:
        giveItem(report, children);
        break;
    case 0x19:
        if (children->message->isFinished(children->message) != 0) {
            children->message->setVisible(children->message, 0);
            report->arrowOn = 0;
            switch (report->step) {
            case 0:
            default:
                report->substate = 0x15;
                break;
            case 1:
                report->substate = 0x32;
                break;
            }
        } else {
            turnMessagePage(report, children);
        }
        break;
    case 0x32:
        startFadeOut(report, children);
        break;
    case 0x33:
        if (STFGTREP_funcs.updateFade(&report->footer) != 0) {
            for (i = 0; i < report->count; i++) {
                children->partners[i]->hide(children->partners[i]);
            }
            report->substate++;
        }
        break;
    case 0x34:
        hidden = 1;
        for (j = 0; j < report->count; j++) {
            if (children->partners[j]->substate != 15) {
                hidden = 0;
                break;
            }
        }
        if (hidden) {
            report->substate++;
        }
        break;
    case 0x35:
        if (children->fade->state == TASK_DONE) {
            report->state = TASK_KILL;
        }
        break;
    }
}

/* The report's main task: loads the files, shares the battle's exp among the
   partners who fought and creates their panels */
void STFGTREP_updateReport(FightReport *report, FightReportChildren *children) {
    s32 fought;
    s32 exp;
    s32 i;
    s32 j;

    switch (report->state) {
    case TASK_INIT:
    default:
        switch (report->substate) {
        case 0:
        default:
            STFGTREP_funcs.loadFiles();
            report->substate++;
            break;
        case 1:
            if (STFGTREP_funcs.filesLoading() != 0) {
                break;
            }
            report->nextState(report);
            STFGTREP_createWindows(report, children);
            for (i = 0; i < 3; i++) {
                if (GAME.funcs.getPartyMember(i) >= 0) {
                    report->count++;
                }
            }
            fought = 0;
            for (i = 0; i < 3; i++) {
                if (BATTLE_RESULT.partners[i].fought != 0) {
                    fought++;
                    for (j = 0; j < 3; j++) {
                        if (BATTLE_RESULT.partners[i].used[j] != 0) {
                            report->used++;
                        }
                    }
                }
            }
            switch (fought) {
            case 1:
            default:
                exp = STFGTREP_rewards[BATTLE_RESULT.battle].exp;
                break;
            case 2:
                exp = STFGTREP_rewards[BATTLE_RESULT.battle].exp * 6 / 10;
                break;
            case 3:
                exp = STFGTREP_rewards[BATTLE_RESULT.battle].exp / 3;
                break;
            }
            for (i = 0; i < report->count; i++) {
                if (BATTLE_RESULT.partners[i].fought != 0) {
                    children->partners[i] = STFGTREP_createPartner(report, i, exp);
                    report->exp[i] = children->partners[i]->boostExp(children->partners[i]);
                } else {
                    children->partners[i] = STFGTREP_createPartner(report, i, 0);
                }
            }
            report->header.duration = 10;
            report->footer.duration = 10;
            break;
        }
        break;
    case TASK_RUN:
        STFGTREP_runReport(report, children);
        STFGTREP_drawReport(report);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.fieldMode, 0);
        break;
    }
}

/* Creates the report (STFGTREP_updateReport) on the screen layer */
FightReport *STFGTREP_createScreen(void) {
    FightReport *report = createTask(STFGTREP_updateReport, sizeof(FightReport), 0x1C);

    report->layer = SCREEN_LAYER;
    report->depth = 7;
    return report;
}

/* Loads the report's sprites and requests its texts, the Digimon names and
   the item names */
void STFGTREP_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(FILE_FGTREP_SPRITES << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_FIGHT_REPORT));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
}

/* Whether STFGTREP_loadFiles's texts are still loading */
s32 STFGTREP_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_FIGHT_REPORT)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_NAMES)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_NAMES)) != 0;
}
