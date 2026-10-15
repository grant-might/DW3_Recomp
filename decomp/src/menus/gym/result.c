/* The training's run and its result (STGTRAIN_runTraining), and an idle task
   that nothing creates: STGTRAIN.PRO's second object (see sprite.c), whose
   rodata starts at 0x8008251C (USA) */

#include "menus/gym.h"

/* Raises a battle stat (1-5) by the training's gain, up to 999 */
s32 STGTRAIN_raiseStat(TrainResult *result, s32 stat) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(result->partner);
    s32 column = 6;
    s16 *value;
    s32 gained;

    if ((u32)(stat - 1) >= 5) {
        return 0;
    }
    value = &stats->stats[stat + 5];
    if (result->training < 0xD) {
        column = 0;
    }
    column += result->bonusWorked * 3 + result->screen->intensity;
    if (STGTRAIN_statGains[column].range != 0) {
        gained = STGTRAIN_statGains[column].base + RANDOM.next() % STGTRAIN_statGains[column].range;
    } else {
        gained = STGTRAIN_statGains[column].base;
    }
    *value += gained;
    if (*value >= 1000) {
        *value = 999;
    }
    return gained;
}

/* Lowers a battle stat (1-5) by the training's loss, half of the time, down to 0 */
s32 STGTRAIN_lowerStat(TrainResult *result, s32 stat) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(result->partner);
    s16 *value;
    s32 lost;
    s32 column;

    if ((u32)(stat - 1) >= 5 || (RANDOM.next() & 1)) {
        return 0;
    }
    value = &stats->stats[stat + 5];
    column = result->screen->intensity;
    if (STGTRAIN_statLosses[column].range != 0) {
        lost = STGTRAIN_statLosses[column].base + RANDOM.next() % STGTRAIN_statLosses[column].range;
    } else {
        lost = STGTRAIN_statLosses[column].base;
    }
    *value -= lost;
    if (*value < 0) {
        *value = 0;
    }
    return lost;
}

/*
 * Raises a resistance (stat 8-14) by the training's gain, up to 999: the
 * table goes by the Digimon's growth of it and how high it already is.
 */
s32 STGTRAIN_raiseResistance(TrainResult *result, s32 stat) {
    PartnerStats *stats;
    s16 *value;
    DigimonData *digimon;
    TrainGain *gains;
    s32 column;
    s32 gained;
    s32 resist = stat - 8;

    if ((u32)resist >= 7) {
        return 0;
    }
    stats = GAME.funcs.getPartnerStats(result->partner);
    digimon = &DIGIMON_DATA[result->partner];
    value = &stats->stats[stat + 4];
    if (*value < 100) {
        gains = STGTRAIN_resistGainTables[(digimon->resistGrowth[resist] - 1) * 3];
    } else if (*value < 300) {
        gains = STGTRAIN_resistGainTables[(digimon->resistGrowth[resist] - 1) * 3 + 1];
    } else {
        gains = STGTRAIN_resistGainTables[(digimon->resistGrowth[resist] - 1) * 3 + 2];
    }
    /* the match depends on column being set in each branch: set before the
       test, its delay slot copy keeps a0 live where the table's lui wants it */
    if (result->bonusWorked == 0) {
        column = 0;
    } else if (result->training < 0xD) {
        column = 1;
    } else {
        column = 2;
    }
    column += result->screen->intensity * 3;
    if (gains[column].range != 0) {
        gained = gains[column].base + RANDOM.next() % gains[column].range;
    } else {
        gained = gains[column].base;
    }
    *value += gained;
    if (*value >= 1000) {
        *value = 999;
    }
    return gained;
}

/*
 * Raises the maximum HP (stat 15) or MP (16) by the training's gain, up to
 * 9999; the US version also raises the current value, up to the maximum.
 */
s32 STGTRAIN_raiseMaxHpMp(TrainResult *result, s32 stat) {
    PartnerStats *stats;
    s16 *value;
#if VERSION_US
    s16 *current;
#endif
    s32 column;
    s32 gained;

    if ((u32)(stat - 15) >= 2) {
        return 0;
    }
    stats = GAME.funcs.getPartnerStats(result->partner);
    if (stat == 15) {
        value = &stats->stats[3];
#if VERSION_US
        current = &stats->stats[2];
#endif
    } else {
        value = &stats->stats[5];
#if VERSION_US
        current = &stats->stats[4];
#endif
    }
    column = 0;
    if (result->bonusWorked != 0) {
        if (result->training < 0xD) {
            column = 1;
        } else {
            column = 2;
        }
    }
    column += result->screen->intensity * 3;
    if (STGTRAIN_maxHpMpGains[column].range != 0) {
        gained = STGTRAIN_maxHpMpGains[column].base + RANDOM.next() % STGTRAIN_maxHpMpGains[column].range;
    } else {
        gained = STGTRAIN_maxHpMpGains[column].base;
    }
    *value += gained;
    if (*value >= 10000) {
        *value = 9999;
    }
#if VERSION_US
    *current += gained;
    if (*current > *value) {
        *current = *value;
    }
#endif
    return gained;
}

/*
 * Applies the i-th try of a training (if it worked) and shows what it
 * changed: the stat it raises, and the one it lowers or also raises.
 */
void STGTRAIN_applyTry(TrainResult *result, s32 i) {
    TrainResultWindows *win = result->children;
    TrainEntry *entry = STGTRAIN_state.findTableEntry(result->modeArg, result->training);

    if (result->trained[i] != 0) {
        /* The match depends on stat (and other below) being s16 locals. */
        s16 stat = entry->stat;

        if (stat != 0) {
            if ((u16)stat - 1 < 5u) {
                result->gains[i] = STGTRAIN_raiseStat(result, stat);
            } else {
                result->gains[i] = STGTRAIN_raiseResistance(result, stat);
                stat = entry->other;
                if (stat != 0) {
                    if ((u16)stat - 1 < 5u) {
                        result->losses[i] = STGTRAIN_lowerStat(result, stat);
                    } else {
                        result->losses[i] = STGTRAIN_raiseMaxHpMp(result, stat);
                    }
                }
            }
        }
    }
    if (result->trained[i] != 0) {
        if ((u16)entry->stat - 1 < 5u) {
            win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), 0x5C);
            win->message[0]->setSubString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), entry->stat + 0x46, 1);
            win->message[0]->setNumber(win->message[0], 2, result->gains[i]);
            win->message[0]->setPalette(win->message[0], PALETTE_BLUE);
            win->message[1]->setVisible(win->message[1], 0);
        } else {
            s16 other;

            win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), 0x5C);
            win->message[0]->setSubString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), entry->stat + 0x4D, 1);
            win->message[0]->setNumber(win->message[0], 2, result->gains[i]);
            win->message[0]->setPalette(win->message[0], PALETTE_BLUE);
            other = entry->other;
            if (other != 0) {
                if ((u16)other - 1 < 5u) {
                    if (result->losses[i] != 0) {
                        win->message[1]->setString(win->message[1], FILE_CACHE.load(STGTRAIN_TEXT), 0x5D);
                        win->message[1]->setSubString(win->message[1], FILE_CACHE.load(STGTRAIN_TEXT), entry->other + 0x46, 1);
                        win->message[1]->setNumber(win->message[1], 2, result->losses[i]);
                        win->message[1]->setPalette(win->message[1], PALETTE_RED);
                    } else {
                        win->message[1]->setVisible(win->message[1], 0);
                    }
                } else {
                    win->message[1]->setString(win->message[1], FILE_CACHE.load(STGTRAIN_TEXT), 0x5C);
                    win->message[1]->setSubString(win->message[1], FILE_CACHE.load(STGTRAIN_TEXT), entry->other + 0x44, 1);
                    win->message[1]->setNumber(win->message[1], 2, result->losses[i]);
                    win->message[1]->setPalette(win->message[1], PALETTE_BLUE);
                }
            }
        }
    } else {
        win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), 0x46);
        win->message[0]->setPalette(win->message[0], PALETTE_WHITE);
        win->message[1]->setVisible(win->message[1], 0);
    }
}

/* The bonus of the accessories 0x151 (3) and 0x152 (6) */
s32 STGTRAIN_getAccessoryBonus(TrainResult *result) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(result->partner);

    if (stats->equip[4] == 0x151 || stats->equip[5] == 0x151) {
        return 3;
    }
    if (stats->equip[4] == 0x152 || stats->equip[5] == 0x152) {
        return 6;
    }
    return 0;
}

/* Creates the training result's text windows and cursor */
void STGTRAIN_createResultWindows(TrainResult *result, TrainResultWindows *win) {
    win->message[0] = createTextWindow(result->layerId, 1, 0x74, 0xC0);
    win->message[0]->setLines(win->message[0], 2);
    win->message[1] = createTextWindow(result->layerId, 1, 0x74, 0xCE);
    win->question[0] = createTextWindow(0x1002, 1, 0xA2, 0x75);
    win->question[1] = createTextWindow(0x1002, 1, 0xA2, 0x91);
    win->question[2] = createTextWindow(0x1002, 1, 0xA2, 0xA1);
    win->cursor = createCursor(0x1002, result->depth - 1, 0x94, 0x91);
    win->cursor->setVisible(win->cursor, 0);
}

/*
 * Draws the training result: the blinking arrow, a mark for each try (0x45
 * worked, 0x46 failed) and the four panels.
 */
void STGTRAIN_drawResult(TrainResult *result) {
    SpriteDrawer sprite;
    s32 i;

    if (result->prompting != 0) {
        initSpriteDrawer(&sprite);
        sprite.setLayerId(0x1002, 3);
        sprite.setTexture(0x140, 0);
        if (GFX.funcs.getTime() - result->arrowTime >= 4) {
            result->arrowTime = GFX.funcs.getTime();
            result->arrowClut++;
            if (result->arrowClut >= 5) {
                result->arrowClut = 0;
            }
        }
        sprite.setClutRow(result->arrowClut);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xA, 0x124, 0xCD);
    }
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x240, 0x100);
    sprite.setLayerId(result->layerId, result->depth);
    if (result->bonusTrying != 0) {
        if (GFX.funcs.getTime() - result->bonusBlinkTime >= 3) {
            result->bonusBlinkTime = GFX.funcs.getTime();
            result->bonusBlink = 1 - result->bonusBlink;
        }
        sprite.setClutRow(result->bonusBlink + 1);
    }
    for (i = 0; i < 5; i++) {
        if (result->bonusWorked != 0 && i == 3) {
            sprite.setClutRow(3);
        }
        if (result->trained[i] == 1) {
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x45, i * 0x11 + 0x80, 0x58);
        } else if (result->trained[i] == 0) {
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x46, i * 0x11 + 0x80, 0x58);
        }
    }
    sprite.setClutRow(0);
    if (result->panels[0].level != 0) {
        if (result->panels[0].level != ONE) {
            sprite.setScale(result->panels[0].level, result->panels[0].level, ONE);
            sprite.setPivot(0xCF, 0x7F);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x28, 0x72, 0x4B);
    }
    if (result->panels[1].level != 0) {
        if (result->panels[1].level != ONE) {
            sprite.setScale(result->panels[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0xCD);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x23, 0x46, 0xBA);
    }
    sprite.setLayerId(0x1002, result->depth);
    if (result->panels[2].level != 0) {
        if (result->panels[2].level != ONE) {
            sprite.setScale(result->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x7B);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x26, 0x85, 0x70);
    }
    if (result->panels[3].level != 0) {
        if (result->panels[3].level != ONE) {
            sprite.setScale(result->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x9F);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x1D, 0x82, 0x8B);
    }
}

#if VERSION_EU || VERSION_JP
/* A battle stat (1-5) or a resistance (8-14) of TOTALS: the match depends on
   reading them through these (indexed in place, the stat's address is
   summed with the index first) */
static inline s16 getStat(PartnerTotals *totals, s32 stat) {
    return totals->fields.battle[stat - 1];
}

static inline s16 getResistance(PartnerTotals *totals, s32 stat) {
    return totals->fields.resist[stat - 8];
}
#endif

/* Takes the result of a try: plays its sound and applies it, then plays the
   next try, or, after the last, asks for the bonus try (three good tries) or
   ends the training (substate 3) */
static inline void STGTRAIN_takeTry(TrainResult *result, TrainResultWindows *win) {
    result->trained[result->step] = win->actor->getResult(win->actor);
    if (result->trained[result->step] != -1) {
        if (result->trained[result->step] != 0) {
            SOUND.playSound(0x840001);
        } else {
            SOUND.playSound(0x840000);
        }
        STGTRAIN_applyTry(result, result->step);
        result->step++;
        if (result->counter < result->step) {
            if (result->counter == 2) {
                if (result->trained[0] != 0 && result->trained[1] != 0 && result->trained[2] != 0) {
                    result->substate = 0xA;
                    return;
                }
                if (result->training >= 0xD) {
                    result->counter = 4;
                    win->actor->play(win->actor);
                    return;
                }
            }
            result->setSubstate(result, 3);
        } else {
            win->actor->play(win->actor);
        }
    }
}

/* Once cross is pressed, shows what the training gave: the stat's gain, and
   the other stat's loss or gain, in the message, and the stats before and
   after */
static inline void STGTRAIN_showGains(TrainResult *result, TrainResultWindows *win) {
    TrainEntry *entry;
    s32 sums[2];
#if VERSION_EU || VERSION_JP
    s32 shown[2]; /* the sums, up to the stats' limits */
    s16 other;
#endif
    s32 i;

    if (!PAD_PRESSED(PAD_CROSS)) {
        return;
    }
    SOUND.playSound(SOUND_MENU_CONFIRM);
    result->prompting = 0;
    entry = STGTRAIN_state.findTableEntry(result->modeArg, result->training);
    if (entry->stat != 0) {
        sums[0] = 0;
        for (i = 0; i < 5; i++) {
            sums[0] += result->gains[i];
        }
        if ((u16)entry->stat - 8 < 7u) {
            if (entry->other != 0) {
                sums[1] = 0;
                for (i = 0; i < 5; i++) {
                    sums[1] += result->losses[i];
                }
            }
        } else {
            sums[1] = 0;
        }
    }
    if (sums[0] == 0 && sums[1] == 0) {
        win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), 0x71);
    } else {
#if VERSION_EU || VERSION_JP
        for (i = 0; i < 2; i++) {
            shown[i] = sums[i];
        }
        if (sums[0] != 0) {
            if ((u16)entry->stat - 1 < 5u) {
                if (getStat(&result->before, entry->stat) + sums[0] >= 1000) {
                    shown[0] = 999 - getStat(&result->before, entry->stat);
                } else {
                    shown[0] = sums[0];
                }
            } else {
                if (getResistance(&result->before, entry->stat) + sums[0] >= 1000) {
                    shown[0] = 999 - getResistance(&result->before, entry->stat);
                } else {
                    shown[0] = sums[0];
                }
            }
        }
        if (sums[1] != 0) {
            other = entry->other;
            if ((u16)other - 1 < 5u) {
                if (getStat(&result->before, other) - sums[1] < 0) {
                    shown[1] = getStat(&result->before, other);
                } else {
                    shown[1] = sums[1];
                }
            } else if ((u16)(other - 8) < 7) {
                if (getResistance(&result->before, other) + sums[1] >= 1000) {
                    shown[1] = 999 - getResistance(&result->before, other);
                } else {
                    shown[1] = sums[1];
                }
            } else if (other == 15) {
                if (result->before.fields.maxHp + sums[1] >= 10000) {
                    shown[1] = 9999 - result->before.fields.maxHp;
                } else {
                    shown[1] = sums[1];
                }
            } else if (other == 16) {
                if (result->before.fields.maxMp + sums[1] >= 10000) {
                    shown[1] = 9999 - result->before.fields.maxMp;
                } else {
                    shown[1] = sums[1];
                }
            }
        }
#endif
        if ((u16)entry->stat - 1 < 5u) {
            win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), entry->stat + 0x5D);
#if VERSION_US
            win->message[0]->setNumber(win->message[0], 1, sums[0]);
#elif VERSION_EU || VERSION_JP
            win->message[0]->setNumber(win->message[0], 1, shown[0]);
#endif
        } else if (sums[1] != 0) {
            win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), entry->stat + 0x62);
#if VERSION_US
            win->message[0]->setNumber(win->message[0], 1, sums[0]);
#elif VERSION_EU || VERSION_JP
            win->message[0]->setNumber(win->message[0], 1, shown[0]);
#endif
#if VERSION_US
            win->message[0]->setNumber(win->message[0], 2, sums[1]);
#elif VERSION_EU || VERSION_JP
            win->message[0]->setNumber(win->message[0], 2, shown[1]);
#endif
        } else {
            win->message[0]->setString(win->message[0], FILE_CACHE.load(STGTRAIN_TEXT), entry->stat + 0x5B);
#if VERSION_US
            win->message[0]->setNumber(win->message[0], 1, sums[0]);
#elif VERSION_EU || VERSION_JP
            win->message[0]->setNumber(win->message[0], 1, shown[0]);
#endif
        }
    }
    win->message[0]->setPalette(win->message[0], PALETTE_WHITE);
    win->message[0]->setTypeDelay(win->message[0], 6);
    win->message[1]->setVisible(win->message[1], 0);
    result->substate++;
    result->screen->showStats(result->screen, &result->before);
}

/* Moves the cursor between yes and no of the bonus try's question, and
   takes the answer (triangle: no) */
static inline void STGTRAIN_askBonusTry(TrainResult *result, TrainResultWindows *win) {
    s32 last;

    last = result->before.fields.spare;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        result->before.fields.spare = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        result->before.fields.spare = 1;
    }
    if (last != result->before.fields.spare) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0x94, result->before.fields.spare * 16 + 0x91);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (result->before.fields.spare == 0) {
            result->counter = 0;
            result->substate++;
        } else {
            result->counter = 4;
            result->substate++;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        result->counter = 4;
        result->substate++;
    }
}

/* Once the question's panels close: yes makes the bonus try with its sound
   (a sure fail when the training is the partner's last bonus one, else a
   50% chance), no plays the other tries */
static inline void STGTRAIN_startBonusTry(TrainResult *result, TrainResultWindows *win) {
    PartnerStats *stats;

    STGTRAIN_state.updateFade(&result->panels[2]);
    if (STGTRAIN_state.updateFade(&result->panels[3])) {
        if (result->counter == 0) {
            result->substate = 0x14;
            result->bonusTrying = 1;
            win->actor->play(win->actor);
            result->bonusSound = SOUND.playSound(0xA084603C);
            stats = GAME.funcs.getPartnerStats(result->partner);
            if (stats->lastBonus != 0 && stats->lastBonus == result->training) {
                win->actor->setChance(win->actor, 0);
            } else {
                stats->lastBonus = 0;
                win->actor->setChance(win->actor, 0x32);
            }
        } else {
            win->actor->play(win->actor);
            result->substate = 2;
        }
    }
}

/* Takes the result of the bonus try: a good one makes the training the
   partner's last bonus one and the gains take their better columns */
static inline void STGTRAIN_takeBonusTry(TrainResult *result, TrainResultWindows *win) {
    PartnerStats *stats;

    result->trained[3] = win->actor->getResult(win->actor);
    if (result->trained[3] != -1) {
        stats = GAME.funcs.getPartnerStats(result->partner);
        if (result->trained[3] != 0) {
            stats->lastBonus = result->training;
            result->bonusWorked = 1;
        } else {
            stats->lastBonus = 0;
        }
        if (result->trained[3] != 0) {
            SOUND.playSound(0x840001);
        } else {
            SOUND.playSound(0x840000);
        }
        SOUND.keyOff(0xA084603C, result->bonusSound);
        STGTRAIN_applyTry(result, 3);
        result->prompting = 1;
        result->bonusTrying = 0;
        result->setSubstate(result, 3);
    }
}

/*
 * Runs a training: the partner tries it three times (five at the gyms'
 * second half), then shows what it gained. Three good tries in a row give
 * a bonus try, asked for (with the training as the partner's last) after
 * the gyms' first half.
 */
void STGTRAIN_runTraining(TrainResult *result, TrainResultWindows *win) {
    switch (result->substate) {
    case 0:
    default:
        STGTRAIN_state.startFade(&result->panels[0], 1);
        STGTRAIN_state.startFade(&result->panels[1], 1);
        win->actor->grow(win->actor);
        result->substate++;
        break;
    case 1:
        STGTRAIN_state.updateFade(&result->panels[1]);
        if (STGTRAIN_state.updateFade(&result->panels[0]) && (win->actor->mode & 1)) {
            result->counter = 2;
            result->substate++;
            win->actor->play(win->actor);
            win->actor->setChance(win->actor, STGTRAIN_getAccessoryBonus(result) + 0x4B);
        }
        break;
    case 2:
        STGTRAIN_takeTry(result, win);
        break;
    case 3:
        win->actor->end(win->actor);
        result->substate++;
        break;
    case 4:
        if (win->actor->mode & 1) {
            result->prompting = 1;
            result->substate++;
        }
        break;
    case 5:
        STGTRAIN_showGains(result, win);
        break;
    case 6:
        if (win->message[0]->isFinished(win->message[0])) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            result->substate = 0x32;
            result->prompting = 0;
            result->screen->showStats(result->screen, NULL);
        } else if (win->message[0]->isWaitingForButton(win->message[0])) {
            result->prompting = 1;
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                result->prompting = 0;
            }
        } else {
            result->prompting = 0;
            if (PAD_PRESSED(PAD_CROSS)) {
                win->message[0]->showPage(win->message[0]);
            }
        }
        break;
    case 0xA:
        if (result->training < 0xD) {
            result->substate = 0x14;
            result->bonusTrying = 1;
            win->actor->play(win->actor);
            result->bonusSound = SOUND.playSound(0xA084603C);
        } else {
            result->substate++;
        }
        break;
    case 0xB:
        STGTRAIN_state.startFade(&result->panels[2], 1);
        result->substate++;
        break;
    case 0xC:
        if (STGTRAIN_state.updateFade(&result->panels[2])) {
            win->question[0]->setString(win->question[0], FILE_CACHE.load(STGTRAIN_TEXT), 0x11);
            STGTRAIN_state.startFade(&result->panels[3], 1);
            result->substate++;
        }
        break;
    case 0xD:
        if (STGTRAIN_state.updateFade(&result->panels[3])) {
            win->question[1]->setString(win->question[1], FILE_CACHE.load(STGTRAIN_TEXT), 0xC);
            win->question[2]->setString(win->question[2], FILE_CACHE.load(STGTRAIN_TEXT), 0xD);
            win->cursor->setPos(win->cursor, 0x94, result->before.fields.spare * 16 + 0x91);
            win->cursor->setVisible(win->cursor, 1);
            result->substate++;
        }
        break;
    case 0xE:
        STGTRAIN_askBonusTry(result, win);
        break;
    case 0xF:
        STGTRAIN_state.startFade(&result->panels[2], 0);
        STGTRAIN_state.startFade(&result->panels[3], 0);
        win->question[0]->setVisible(win->question[0], 0);
        win->question[1]->setVisible(win->question[1], 0);
        win->question[2]->setVisible(win->question[2], 0);
        win->cursor->setVisible(win->cursor, 0);
        result->substate++;
        break;
    case 0x10:
        STGTRAIN_startBonusTry(result, win);
        break;
    case 0x14:
        STGTRAIN_takeBonusTry(result, win);
        break;
    case 0x32:
        result->state = TASK_KILL;
        break;
    }
}

/* The training result's task. The match depends on the -1 being in a variable. */
void STGTRAIN_updateResult(TrainResult *result, TrainResultWindows *win) {
    s32 i;

    switch (result->state) {
    case TASK_INIT:
    default:
        result->nextState(result);
        STGTRAIN_createResultWindows(result, win);
        result->panels[0].duration = 10;
        result->panels[1].duration = 10;
        result->panels[2].duration = 10;
        result->panels[3].duration = 10;
        {
            s32 none = -1;
            for (i = 4; i >= 0; i--) {
                result->trained[i] = none;
            }
        }
        win->actor = STGTRAIN_createActor(result->partner, result->training, result->layerId, result->depth - 3);
        win->actor->setPos(win->actor, 0x300, 0);
        win->actor->setClutPos(win->actor, 0x2C0, 0);
        win->actor->pause(win->actor);
        win->actor->setScale(win->actor, 0);
        GAME.funcs.computeStats(result->partner, &result->before);
        break;
    case TASK_RUN:
        STGTRAIN_runTraining(result, win);
        STGTRAIN_drawResult(result);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the results of a training of a partner */
TrainResult *STGTRAIN_createResult(TrainScreen *screen, s32 partner, s32 training) {
    TrainResult *result = createTask(STGTRAIN_updateResult, sizeof(TrainResult), sizeof(TrainResultWindows));

    result->layerId = SCREEN_LAYER;
    result->depth = 6;
    result->screen = screen;
    result->partner = partner;
    result->training = training;
    result->modeArg = GAME.funcs.getModeArg();
    return result;
}

/* Left empty, as are the idle task's other steps */
void STGTRAIN_initIdle(TrainIdle *task, void *children) {
}

/* Left empty */
void STGTRAIN_showIdle(TrainIdle *task, void *children, s32 show) {
}

/* Only sets up a sprite drawer */
void STGTRAIN_drawIdle(TrainIdle *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
}

/* Left empty */
void STGTRAIN_runIdle(TrainIdle *task, void *children) {
}

/* A task that does nothing: its steps are empty, and nothing creates it */
void STGTRAIN_updateIdle(TrainIdle *task, void *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        STGTRAIN_initIdle(task, children);
        STGTRAIN_showIdle(task, children, 1);
        break;
    case TASK_RUN:
        STGTRAIN_runIdle(task, children);
        STGTRAIN_drawIdle(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the idle task for the screen */
TrainIdle *STGTRAIN_createIdle(TrainScreen *screen) {
    TrainIdle *task = createTask(STGTRAIN_updateIdle, sizeof(TrainIdle), 0);

    task->layerId = SCREEN_LAYER;
    task->depth = 6;
    task->screen = screen;
    return task;
}

/* The data: this object's tables */
/* The battle stats a training raises, by [training >= 13][bonusWorked][level] */
TrainGain STGTRAIN_statGains[] = {
    {1, 2}, {7, 2}, {14, 3},
    {2, 0}, {10, 0}, {20, 0},
    {1, 2}, {6, 3}, {11, 5},
    {4, 0}, {22, 0}, {48, 0},
};
/* What a training may lower another battle stat by, by level */
TrainGain STGTRAIN_statLosses[] = {
    {1, 0}, {2, 3}, {4, 3},
};
/* The resistance gains, by [bonusWorked ? (training < 13 ? 1 : 2) : 0][level] */
TrainGain STGTRAIN_resistGainsLow[] = {
    {1, 0}, {1, 0}, {2, 0},
    {4, 2}, {8, 0}, {12, 0},
    {8, 2}, {20, 0}, {30, 0},
};
TrainGain STGTRAIN_resistGainsMid[] = {
    {1, 2}, {2, 0}, {3, 0},
    {6, 3}, {12, 0}, {18, 0},
    {12, 4}, {30, 0}, {40, 0},
};
TrainGain STGTRAIN_resistGainsHigh[] = {
    {2, 0}, {2, 0}, {4, 0},
    {8, 4}, {12, 0}, {25, 0},
    {16, 5}, {30, 0}, {50, 0},
};
/* The resistance gain tables, by the Digimon's affinity and the
   resistance's value (under 100, under 300, more) */
TrainGain *STGTRAIN_resistGainTables[] = {
    STGTRAIN_resistGainsLow, STGTRAIN_resistGainsMid, STGTRAIN_resistGainsMid, STGTRAIN_resistGainsLow,
    STGTRAIN_resistGainsMid, STGTRAIN_resistGainsMid, STGTRAIN_resistGainsMid, STGTRAIN_resistGainsMid,
    STGTRAIN_resistGainsMid, STGTRAIN_resistGainsHigh, STGTRAIN_resistGainsHigh, STGTRAIN_resistGainsMid,
    STGTRAIN_resistGainsHigh, STGTRAIN_resistGainsHigh, STGTRAIN_resistGainsMid,
};
/* The max HP and MP gains, indexed like the resistances */
TrainGain STGTRAIN_maxHpMpGains[] = {
    {1, 2}, {2, 0}, {4, 0},
    {6, 4}, {10, 0}, {20, 0},
    {13, 5}, {20, 0}, {40, 0},
};
