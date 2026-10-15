/* A partner's growth: its stats at a new level, its exp and levels, the
   Digimon it learns, its Digimon's exp, levels and skills. */

#include "battle/report.h"

/* Raises a partner's max HP, max MP, battle stats and, up to level 40, its
   resistances for a new level, by its growth (DIGIMON_DATA) and at random */
void STFGTREP_raiseStats(s32 partner, s32 level) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(partner);
    DigimonData *digimon = &DIGIMON_DATA[partner];
    s32 tier;
    s32 i;
    s32 growth;
    s32 gain;
    s16 *values;

    if (level < 5) {
        tier = 0;
    } else if (level < 20) {
        tier = 1;
    } else if (level < 40) {
        tier = 2;
    } else {
        tier = 3;
    }
    /* read into a variable of its own: the match depends on it, since the
       original reads the growth before it calls RANDOM.next and adds the
       gain as a whole */
    growth = digimon->hpGrowth;
    gain = growth - STFGTREP_vitalCuts[tier] + STFGTREP_vitalRandom[RANDOM.next() % 9];
    stats->stats[STAT_MAX_HP] += gain;
    if (stats->stats[STAT_MAX_HP] >= 10000) {
        stats->stats[STAT_MAX_HP] = 9999;
    }
    growth = digimon->mpGrowth;
    gain = growth - STFGTREP_vitalCuts[tier] + STFGTREP_vitalRandom[RANDOM.next() % 9];
    stats->stats[STAT_MAX_MP] += gain;
    if (stats->stats[STAT_MAX_MP] >= 10000) {
        stats->stats[STAT_MAX_MP] = 9999;
    }
    values = stats->stats;
    if (level < 5) {
        tier = 0;
    } else if (level < 20) {
        tier = 1;
    } else if (level < 40) {
        tier = 2;
    } else if (level < 60) {
        tier = 3;
    } else if (level < 80) {
        tier = 4;
    } else {
        tier = 5;
    }
    for (i = 0; i < 6; i++) {
        growth = digimon->statGrowth[i];
        gain = STFGTREP_statGains[tier][growth + RANDOM.next() % 5];
        values[6 + i] += gain;
        if (values[6 + i] >= 1000) {
            values[6 + i] = 999;
        }
    }
    if (level < 41) {
        for (i = 0; i < 7; i++) {
            growth = digimon->resistGrowth[i];
            gain = STFGTREP_resistGains[growth + RANDOM.next() % 4];
            values[12 + i] += gain;
            if (values[12 + i] >= 1000) {
                values[12 + i] = 999;
            }
        }
    }
}

/* Adds exp to a partner and raises its level, and its stats with it, as far
   as the exp reaches; whether it went up */
s32 STFGTREP_addExp(s32 partner, s32 exp) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(partner);
    DigimonData *digimon = &DIGIMON_DATA[partner];
    s32 leveled;
    s32 level;
    s32 tier;
    s32 up;

    stats->exp += exp;
    if (stats->exp > 999999) {
        stats->exp = 999999;
    }
    leveled = 0;
    /* two statements: the match depends on it, since the level is loaded
       into the register that counts the levels */
    level = stats->stats[STAT_LEVEL];
    level++;
    do {
        if (level < 5) {
            tier = 0;
        } else if (level < 20) {
            tier = 1;
        } else if (level < 40) {
            tier = 2;
        } else if (level < 100) {
            tier = 3;
        } else {
            break;
        }
        if ((level * level * level + level * 5 - 6) * digimon->expRate / 10 + STFGTREP_levelExp[tier] < stats->exp) {
            if (++stats->stats[STAT_LEVEL] < 100) {
                STFGTREP_raiseStats(partner, stats->stats[STAT_LEVEL]);
            } else {
                stats->stats[STAT_LEVEL] = 99;
            }
            stats->stats[STAT_TP] += 5;
            if (stats->stats[STAT_TP] >= 100) {
                stats->stats[STAT_TP] = 99;
            }
            leveled = 1;
            up = 1;
        } else {
            up = 0;
        }
        level++;
    } while (up);
    return leveled;
}

/* Gives a partner the first Digimon of its evolution list (STFGTREP_evolutions)
   it hasn't yet and whose needs it meets: the Digimon it must have at a
   level, and a stat (1-6 battle stats, 7 the level, 8-14 resistances) of at
   least a value. The Digimon also takes the first free slot (below 3); the
   Digimon, or 0 */
s32 STFGTREP_learnDigimon(s32 partner) {
    PartnerEntry entry;
    s16 slots[3];
    PartnerStats *stats;
    Evolution *list;
    s32 n;
    s32 id;
    s32 i;
    s32 j;

    list = STFGTREP_evolutions[partner];
    for (n = 0; n < 44; n++) {
        id = DIGIMON_DATA[list[n].digimon - 1].id;
        if (GAME.funcs.getPartnerEntry(partner, id, &entry) >= 0 || id <= 0) {
            continue;
        }
        for (i = 0; i < 2; i++) {
            if (list[n].needs[i].digimon != 0) {
                if (GAME.funcs.getPartnerEntry(partner, DIGIMON_DATA[list[n].needs[i].digimon - 1].id, &entry) == -1) {
                    id = -1;
                } else if (entry.level < list[n].needs[i].level) {
                    id = -1;
                }
            }
        }
        if (id <= 0) {
            continue;
        }
        if (list[n].stat != 0) {
            stats = GAME.funcs.getPartnerStats(partner);
            /* pointer sums, not stats[stat + 5]: the match depends on them,
               which add the 5 and the 4 to the load's offset */
            if (list[n].stat < 7) {
                if (*(stats->stats + list[n].stat + 5) < list[n].value) {
                    id = -1;
                }
            } else if (list[n].stat == 7) {
                if (stats->stats[STAT_LEVEL] < list[n].value) {
                    id = -1;
                }
            } else if (list[n].stat >= 8) {
                if (*(stats->stats + list[n].stat + 4) < list[n].value) {
                    id = -1;
                }
            }
        }
        if (id <= 0) {
            continue;
        }
        GAME.funcs.addPartnerEntry(partner, id);
        GAME.funcs.getPartnerSlots(partner, slots);
        for (j = 0; j < 3; j++) {
            if (slots[j] < 3) {
                slots[j] = id;
                GAME.funcs.setPartnerSlots(partner, slots);
                break;
            }
        }
        return id;
    }
    return 0;
}

/* Adds exp to one of a partner's Digimon and raises its level as far as the
   exp reaches, up to 99; whether it went up */
s32 STFGTREP_addDigimonExp(s32 partner, s32 id, s32 exp) {
    DigimonData *digimon = GET_DIGIMON(id);
    PartnerEntry entry;
    s32 leveled;
    s32 need;
    s32 up;
    s32 n;

    GAME.funcs.getPartnerEntry(partner, id, &entry);
    entry.exp += exp;
    if (entry.exp > 9999999) {
        entry.exp = 9999999;
    }
    if (entry.level >= 99) {
        return 0;
    }
    leveled = 0;
    do {
        /* 10 exp a level up to expLevel, 50 after it */
        if (digimon->expLevel >= entry.level + 1) {
            need = entry.level * 10;
        } else {
            n = digimon->expLevel - 1;
            need = n * 10 + (entry.level - n) * 50;
        }
        up = 0;
        if (entry.exp >= need) {
            leveled = 1;
            entry.level++;
            up = entry.level < 99;
        }
    } while (up);
    GAME.funcs.setPartnerEntry(partner, id, &entry);
    return leveled;
}

/* Gives one of a partner's Digimon the first skill whose level it has
   reached and that it hasn't yet (DigimonData.skills); the skill, or 0 */
s32 STFGTREP_addSkill(s32 partner, s32 id) {
    PartnerEntry entry;
    DigimonData *digimon;
    s32 i;

    GAME.funcs.getPartnerEntry(partner, id, &entry);
    for (i = 0; i < 6; i++) {
        if (entry.skills[i] == 0) {
            digimon = GET_DIGIMON(id);
            if (digimon->skills[i + 1] != 0 && entry.level >= digimon->skillLevels[i]) {
                entry.skills[i] = digimon->skills[i + 1];
                if (i == 5) {
                    entry.skills[i] |= SKILL_LAST;
                }
                GAME.funcs.setPartnerEntry(partner, id, &entry);
                return digimon->skills[i + 1];
            }
        }
    }
    return 0;
}

/* Marks known (SKILL_KNOWN) the first skill of one of a partner's Digimon whose
   level it has reached (DigimonData.knownLevels); the skill, or 0 */
s32 STFGTREP_learnSkill(s32 partner, s32 id) {
    PartnerEntry entry;
    DigimonData *digimon;
    s32 i;

    GAME.funcs.getPartnerEntry(partner, id, &entry);
    digimon = GET_DIGIMON(id);
    for (i = 0; i < 5; i++) {
        if (entry.skills[i] > 0 && !(entry.skills[i] & SKILL_KNOWN) && entry.level >= digimon->knownLevels[i]) {
            entry.skills[i] |= SKILL_KNOWN;
            GAME.funcs.setPartnerEntry(partner, id, &entry);
            return entry.skills[i] & SKILL_ID;
        }
    }
    return 0;
}

/* The exp one of a partner's Digimon gets of a battle's: less as the partner
   goes up in level, shared by the Digimon used, at least 1 and at most 10,
   or 50 once the Digimon is past its expLevel */
s32 STFGTREP_getDigimonExp(s32 partner, s32 id, s32 exp, s32 used) {
    PartnerEntry entry;
    DigimonData *digimon;
    s32 level = GAME.funcs.getPartnerStats(partner)->stats[STAT_LEVEL];
    s32 share;
    s32 n;
    s32 tenfold;

    /* computed apart: the match depends on it, since the original multiplies
       before it compares the level */
    tenfold = exp * 10;
    if (level < 51) {
        n = tenfold / level;
    } else {
        n = exp / 5;
    }
    switch (used) {
    case 0:
    case 1:
        share = n;
        break;
    case 2:
        share = n * 6 / 10;
        break;
    default:
        share = n / used;
        break;
    }
    digimon = GET_DIGIMON(id);
    GAME.funcs.getPartnerEntry(partner, id, &entry);
    if (share <= 0) {
        share = 1;
    } else if (entry.level < digimon->expLevel) {
        if (share > 10) {
            share = 10;
        }
    } else if (share > 50) {
        share = 50;
    }
    return share;
}
