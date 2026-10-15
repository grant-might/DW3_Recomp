/* A partner's panel: its windows, its drawing, its exp roll and the steps
   that give its Digimon their exp, levels, skills and new Digimon. */

#include "battle/report.h"

/* Creates the text windows of a partner's panel */
void STFGTREP_createPartnerWindows(ReportPartner *partner, ReportPartnerWindows *windows) {
    TextWindow *window;
    s32 y = partner->index * 50;
    s32 i;

    windows->name = createTextWindow(partner->layer, 1, 0x3D, y + 0x28);
    windows->levelLabel = createTextWindow(partner->layer, 3, 0x3E, y + 0x38);
    windows->level = createTextWindow(partner->layer, 3, 0x5B, y + 0x38);
    windows->expLabel = createTextWindow(partner->layer, 3, 0x6A, y + 0x45);
    windows->exp = createTextWindow(partner->layer, 3, 0x67, y + 0x45);
    for (i = 0; i < 3; i++) {
        y = partner->index * 50 + i * 15;
        windows->slots[i].name = createTextWindow(partner->layer, 1, 0x8D, y + 0x28);
        windows->slots[i].level = createTextWindow(partner->layer, 3, 0x114, y + 0x2C);
    }
    window = createTextWindow(partner->layer, 1, 0x14, 0xC2);
    windows->message = window;
    window->setLines(window, 2);
}

/* Fills a partner's panel with its name, level, exp and Digimon, or hides it */
void STFGTREP_fillPartnerWindows(ReportPartner *partner, ReportPartnerWindows *windows, s32 show) {
    PartnerStats *stats;
    s32 member;
    s32 i;

    if (show != 0) {
        member = GAME.funcs.getPartyMember(partner->index);
        stats = GAME.funcs.getPartnerStats(member);
        windows->name->setString(windows->name, stats, -1);
        windows->level->setNumber(windows->level, 0, stats->stats[STAT_LEVEL]);
        windows->level->setRightAlign(windows->level, 1);
        windows->levelLabel->setString(windows->levelLabel, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 3);
        windows->exp->setNumber(windows->exp, 0, partner->shownExp);
        windows->exp->setRightAlign(windows->exp, 1);
        windows->exp->setSpacing(windows->exp, 7, 0);
        windows->expLabel->setString(windows->expLabel, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 1);
        GAME.funcs.getPartnerSlots(member, partner->slots);
        for (i = 0; i < 3; i++) {
            if (partner->slots[i] >= 4) {
                GAME.funcs.getPartnerEntry(member, partner->slots[i], &partner->entry);
                windows->slots[i].name->setString(windows->slots[i].name, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), GET_DIGIMON(partner->slots[i])->nameId);
                windows->slots[i].level->setNumber(windows->slots[i].level, 0, partner->entry.level);
                windows->slots[i].level->setRightAlign(windows->slots[i].level, 1);
            } else {
                windows->slots[i].name->setVisible(windows->slots[i].name, 0);
                windows->slots[i].level->setVisible(windows->slots[i].level, 0);
            }
        }
    } else {
        windows->name->setVisible(windows->name, 0);
        windows->level->setVisible(windows->level, 0);
        windows->levelLabel->setVisible(windows->levelLabel, 0);
        windows->exp->setVisible(windows->exp, 0);
        windows->expLabel->setVisible(windows->expLabel, 0);
        for (i = 0; i < 3; i++) {
            windows->slots[i].name->setVisible(windows->slots[i].name, 0);
            windows->slots[i].level->setVisible(windows->slots[i].level, 0);
        }
    }
}

/* Draws a partner's panel: the level and slot blinks, the cursor, the
   partner's animation and its frames */
void STFGTREP_drawPartner(ReportPartner *partner) {
    SpriteDrawer sprite;
    s32 digimon;
    s32 y;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(partner->layer, partner->depth);
    sprite.setTexture(0x280, 0);
    if (partner->substate >= 20) {
        if (partner->levelBlink.on != 0) {
            if (GFX.funcs.getTime() - partner->levelBlink.time >= 3) {
                partner->levelBlink.time = GFX.funcs.getTime();
                if (++partner->levelBlink.frame >= 11) {
                    partner->levelBlink.frame = 10;
                }
            }
            sprite.setClutRow(partner->levelBlink.frame);
            y = partner->index * 50;
            sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x22, 0x63, y + 0x2B);
            sprite.setClutRow(0);
        }
        for (i = 0; i < 3; i++) {
            if (partner->slotBlinks[i].blink.on != 0) {
                if (GFX.funcs.getTime() - partner->slotBlinks[i].blink.time >= 3) {
                    partner->slotBlinks[i].blink.time = GFX.funcs.getTime();
                    if (++partner->slotBlinks[i].blink.frame >= 11) {
                        partner->slotBlinks[i].blink.frame = 10;
                    }
                }
                sprite.setClutRow(partner->slotBlinks[i].blink.frame);
                y = partner->index * 50 + i * 15;
                sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x23, 0xEF, y + 0x23);
            }
        }
        sprite.setClutRow(0);
    }
    if (partner->selected != 0) {
        if (GFX.funcs.getTime() - partner->cursorTime >= 5) {
            partner->cursorTime = GFX.funcs.getTime();
            if (++partner->cursorFrame >= 4) {
                partner->cursorFrame = 0;
            }
        }
        sprite.setClutRow(partner->cursorFrame);
        y = partner->index * 50;
        sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x24, 0x11, y + 0x26);
        if (partner->substate >= 20) {
            if (partner->slot >= 0 && partner->slots[partner->slot] >= 4) {
                y = partner->index * 50 + partner->slot * 15;
                sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x25, 0x87, y + 0x26);
            }
        }
        sprite.setClutRow(0);
    }
    if (partner->fade.level != 0) {
        if (partner->fade.level != 0x1000) {
            sprite.setScale(partner->fade.level, 0x1000, 0x1000);
        }
        digimon = GAME.funcs.getPartyMember(partner->index);
        y = partner->index * 50;
        if (partner->fade.level != 0x1000) {
            sprite.setPivot(0x11, y + 0x3A);
        }
        if (GFX.funcs.getTime() - partner->frameTime >= 13) {
            partner->frameTime = GFX.funcs.getTime();
            if (++partner->frame >= 8 || STFGTREP_animations[digimon][partner->frame] == -1) {
                partner->frame = 0;
            }
        }
        sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), STFGTREP_animations[digimon][partner->frame], 0x14, y + 0x28);
        if (partner->levelBlink.on != 0) {
            sprite.setClutRow(partner->levelBlink.frame);
        }
        sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x1F, 0x11, y + 0x26);
        if (partner->levelBlink.on != 0) {
            sprite.setClutRow(0);
        }
        for (i = 0; i < 3; i++) {
            y = partner->index * 50 + i * 15;
            if (partner->fade.level != 0x1000) {
                sprite.setPivot(0x87, y + 0x2D);
            }
            if (partner->slotBlinks[i].blink.on != 0) {
                sprite.setClutRow(partner->slotBlinks[i].blink.frame);
            } else {
                sprite.setClutRow(0);
            }
            sprite.draw(FILE_CACHE.getEntry((FILE_FGTREP_SPRITES - 1) << 16), 0x20, 0x87, y + 0x26);
        }
    }
}

/* Rolls the shown exp towards the target, digit by digit; 0 once it is there */
s32 STFGTREP_rollExp(ReportPartner *partner) {
    ReportPartnerWindows *windows = partner->children;
    s32 digits = 0;
    s32 n;
    s32 step;
    s32 i;

    if (++partner->rollTime >= 10) {
        partner->shownExp = partner->targetExp;
        STFGTREP_fillPartnerWindows(partner, windows, 1);
        return 0;
    }
    n = partner->targetExp;
    for (i = 10; n != 0; i *= 10) {
        digits++;
        n -= n % i;
    }
    step = 1;
    for (i = digits - 1; i != 0; i--) {
        step *= 10;
        step++;
    }
    partner->shownExp += step;
    if (partner->shownExp > step * 9) {
        partner->shownExp -= step * 9 + 1;
    }
    STFGTREP_fillPartnerWindows(partner, windows, 1);
    return 1;
}

/* Gives the slot's Digimon its share of the battle's exp, if it was used
   in the battle, and blinks the slot when its level goes up */
static inline void giveDigimonExp(ReportPartner *partner) {
    s32 member;

    member = GAME.funcs.getPartyMember(partner->index);
    GAME.funcs.getPartnerSlots(member, partner->slots);
    if (BATTLE_RESULT.partners[partner->index].used[partner->slot] != 0 && partner->slots[partner->slot] >= 4) {
        partner->slotBlinks[partner->slot].blink.on = STFGTREP_funcs.addDigimonExp(
            member, partner->slots[partner->slot],
            STFGTREP_funcs.getDigimonExp(member, partner->slots[partner->slot],
                                         STFGTREP_rewards[BATTLE_RESULT.battle].digimonExp, partner->report->used));
    }
    partner->substate++;
}

/* When the slot's Digimon went up a level, plays its sound and shows it in
   the message */
static inline void showDigimonLevelUp(ReportPartner *partner, ReportPartnerWindows *windows) {
    if (partner->slotBlinks[partner->slot].blink.on != 0) {
        if (partner->voice != -1) {
            SOUND.keyOff(partner->sound, partner->voice);
            partner->voice = -1;
        }
        partner->voice = SOUND.playSound(0x4000C);
        partner->sound = 0x4000C;
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 4);
        windows->message->setTypeDelay(windows->message, 6);
        STFGTREP_fillPartnerWindows(partner, windows, 1);
        partner->substate = 25;
        partner->step = 0;
    } else {
        partner->substate = 24;
    }
}

/* Gives the slot's Digimon its next skill and shows it in the message */
static inline void giveNextSkill(ReportPartner *partner, ReportPartnerWindows *windows) {
    s32 member;
    s32 id;

    member = GAME.funcs.getPartyMember(partner->index);
    GAME.funcs.getPartnerSlots(member, partner->slots);
    id = STFGTREP_funcs.addSkill(member, partner->slots[partner->slot]);
    if (id != 0) {
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 5);
        windows->message->setSubString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), id, 1);
        windows->message->setTypeDelay(windows->message, 6);
        partner->substate = 25;
        partner->step = 1;
    } else {
        partner->substate = 23;
    }
}

/* Marks known the next skill of the slot's Digimon and shows it in the
   message */
static inline void markNextSkillKnown(ReportPartner *partner, ReportPartnerWindows *windows) {
    s32 member;
    s32 id;

    member = GAME.funcs.getPartyMember(partner->index);
    GAME.funcs.getPartnerSlots(member, partner->slots);
    id = STFGTREP_funcs.learnSkill(member, partner->slots[partner->slot]);
    if (id != 0) {
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 6);
        windows->message->setSubString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_SKILL_NAMES)), id, 1);
        windows->message->setTypeDelay(windows->message, 6);
        partner->substate = 25;
        partner->step = 2;
    } else {
        partner->substate = 24;
    }
}

/* Gives the partner its exp; when its level goes up, blinks the level and
   shows the new level in the message */
static inline void raisePartnerLevel(ReportPartner *partner, ReportPartnerWindows *windows) {
    s32 member = GAME.funcs.getPartyMember(partner->index);
    PartnerStats *stats;

    if (STFGTREP_funcs.addExp(member, partner->exp) != 0) {
        stats = GAME.funcs.getPartnerStats(member);
        partner->levelBlink.on = 1;
        if (partner->voice != -1) {
            SOUND.keyOff(partner->sound, partner->voice);
            partner->voice = -1;
        }
        partner->voice = SOUND.playSound(0x4000B);
        partner->sound = 0x4000B;
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 7);
        windows->message->setSubString(windows->message, stats->name, -1, 1);
        windows->message->setNumber(windows->message, 2, stats->stats[STAT_LEVEL]);
        windows->message->setTypeDelay(windows->message, 6);
        partner->substate = 45;
        partner->step = 3;
    } else {
        partner->substate = 41;
    }
    STFGTREP_fillPartnerWindows(partner, windows, 1);
}

/* Gives the partner the next Digimon of its evolution list and shows it in
   the message */
static inline void learnNextDigimon(ReportPartner *partner, ReportPartnerWindows *windows) {
    s32 learnt;
    DigimonData *digimon;

    learnt = STFGTREP_funcs.learnDigimon(GAME.funcs.getPartyMember(partner->index));
    if (learnt != 0) {
        digimon = GET_DIGIMON(learnt);
        windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 8);
        windows->message->setSubString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), digimon->nameId, 1);
        windows->message->setTypeDelay(windows->message, 6);
        STFGTREP_fillPartnerWindows(partner, windows, 1);
        partner->substate = 45;
        partner->step = 4;
        partner->learned = 1;
    } else if (partner->learned != 0) {
        partner->substate = 42;
    } else {
        partner->substate = 50;
    }
}

/* A panel's steps: it fades in and out, gives each slot's Digimon its exp,
   with the levels and skills that brings (giveDigimonExp, showDigimonLevelUp,
   giveNextSkill, markNextSkillKnown), then the partner's exp and the Digimon it
   learns (raisePartnerLevel, learnNextDigimon), each with its message */
void STFGTREP_runPartner(ReportPartner *partner, ReportPartnerWindows *windows) {
    s32 done;
    s32 i;

    switch (partner->substate) {
    case 0: /* the match depends on it, which starts the jump table at 0 */
        break;
    case 1:
        STFGTREP_funcs.startFade(&partner->fade, 1);
        partner->substate++;
        break;
    case 2:
        if (STFGTREP_funcs.updateFade(&partner->fade) != 0) {
            STFGTREP_fillPartnerWindows(partner, windows, 1);
            partner->substate = 5;
        }
        break;
    case 11:
        STFGTREP_funcs.startFade(&partner->fade, 0);
        STFGTREP_fillPartnerWindows(partner, windows, 0);
        partner->substate++;
        break;
    case 12:
        if (STFGTREP_funcs.updateFade(&partner->fade) != 0) {
            partner->substate = 15;
        }
        break;
    case 20:
        giveDigimonExp(partner);
        break;
    case 21:
        showDigimonLevelUp(partner, windows);
        break;
    case 22:
        giveNextSkill(partner, windows);
        break;
    case 23:
        markNextSkillKnown(partner, windows);
        break;
    case 24:
        if (++partner->slot >= 3) {
            partner->slot = -1;
            partner->substate = 40;
        } else {
            partner->substate = 20;
        }
        break;
    case 25:
    case 45:
        if (windows->message->isFinished(windows->message) != 0) {
            windows->message->setVisible(windows->message, 0);
            partner->report->arrowOn = 0;
            switch (partner->step) {
            case 0:
            case 1:
            default:
                partner->substate = 22;
                break;
            case 2:
                partner->substate = 23;
                break;
            case 3:
            case 4:
                partner->substate = 41;
                break;
            case 5:
                partner->substate = 50;
                break;
            }
        } else if (windows->message->isWaitingForButton(windows->message) != 0) {
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
            } else {
                partner->report->arrowOn = 1;
            }
        } else if (PAD_PRESSED(PAD_CROSS)) {
            windows->message->showPage(windows->message);
        }
        break;
    case 40:
        raisePartnerLevel(partner, windows);
        break;
    case 41:
        learnNextDigimon(partner, windows);
        break;
    case 42:
        if (GAME.funcs.listPartnerEntries(GAME.funcs.getPartyMember(partner->index), partner->entries) >= 4) {
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_FIGHT_REPORT)), 9);
            windows->message->setTypeDelay(windows->message, 6);
            partner->substate = 45;
            partner->step = 5;
        } else {
            partner->substate = 50;
        }
        break;
    case 50:
        done = 1;
        if (partner->levelBlink.on != 0) {
            done = partner->levelBlink.frame >= 10;
        }
        if (done) {
            for (i = 0; i < 3; i++) {
                if (partner->slotBlinks[i].blink.on != 0 && partner->slotBlinks[i].blink.frame < 10) {
                    done = 0;
                }
            }
            if (done) {
                partner->substate = 5;
                partner->selected = 0;
            }
        }
        break;
    }
    if (partner->substate >= 20 && STFGTREP_rollExp(partner) != 0) {
        SOUND.playSound(SOUND_COUNT);
    }
}

/* A partner's panel task */
void STFGTREP_updatePartner(ReportPartner *partner, ReportPartnerWindows *windows) {
    PartnerStats *stats;

    switch (partner->state) {
    case TASK_INIT:
    default:
        partner->nextState(partner);
        STFGTREP_createPartnerWindows(partner, windows);
        partner->fade.duration = 10;
        stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(partner->index));
        partner->shownExp = stats->exp;
        if (partner->shownExp > 999998) {
            partner->rollTime = 10;
        }
        if (partner->boosted == 0) {
            partner->boostExp(partner);
        }
        partner->targetExp = stats->exp + partner->exp;
        if (partner->targetExp > 999999) {
            partner->targetExp = 999999;
        }
        break;
    case TASK_RUN:
        STFGTREP_runPartner(partner, windows);
        STFGTREP_drawPartner(partner);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* ReportPartner.show: fades the panel in (substate 1) */
void STFGTREP_showPartner(ReportPartner *partner) {
    partner->substate = 1;
}

/* ReportPartner.hide: fades the panel out (substate 11) */
void STFGTREP_hidePartner(ReportPartner *partner) {
    partner->substate = 11;
}

/* ReportPartner.raise: gives the exp to the partner's Digimon, with its new
   levels and skills (substate 20) */
void STFGTREP_raisePartner(ReportPartner *partner) {
    partner->substate = 20;
}

/* Adds a fifth to the exp once, if the partner wears item 0x141 */
s32 STFGTREP_boostExp(ReportPartner *partner) {
    PartnerStats *stats;

    if (partner->boosted == 0) {
        stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(partner->index));
        if (stats->equip[4] == 0x141 || stats->equip[5] == 0x141) {
            partner->exp += partner->exp / 5;
        }
    }
    partner->boosted = 1;
    return partner->exp;
}

/* ReportPartner.select: shows the cursor on the panel until its exp is given */
void STFGTREP_selectPartner(ReportPartner *partner) {
    partner->selected = 1;
}

/* Creates the panel of party member index, who gets exp */
ReportPartner *STFGTREP_createPartner(FightReport *report, s32 index, s32 exp) {
    ReportPartner *partner = createTask(STFGTREP_updatePartner, sizeof(ReportPartner), 0x30);

    partner->show = STFGTREP_showPartner;
    partner->hide = STFGTREP_hidePartner;
    partner->raise = STFGTREP_raisePartner;
    partner->boostExp = STFGTREP_boostExp;
    partner->select = STFGTREP_selectPartner;
    partner->layer = SCREEN_LAYER;
    partner->depth = 6;
    partner->report = report;
    partner->voice = -1;
    partner->index = index;
    partner->exp = exp;
    return partner;
}
