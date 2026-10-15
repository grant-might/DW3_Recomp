#include "engine/game.h"
#include "field/field_map.h"

/* Whether bit `index` of a bitset is set (set != 0) or clear (set 0) */
s32 testBit(u8 *bits, s32 index, s32 set) {
    s32 byte = index >> 3;
    s32 mask = 1 << (index & 7);

    if (set != 0) {
        return (bits[byte] & mask) != 0;
    }
    return (bits[byte] & mask) == 0;
}

/* Sets or clears bit `index` of a bitset */
void setBit(u8 *bits, s32 index, s32 set) {
    s32 byte = index >> 3;
    s32 mask = 1 << (index & 7);

    if (set) {
        bits[byte] |= mask;
    } else {
        bits[byte] &= ~mask;
    }
}

/* Special condition 0x00: the player has every item of a set (the European
   and Japanese versions have a second set) */
s32 checkItemSet(s32 op, s32 arg) {
    s32 ret = 0;

#if VERSION_US
    if ((GAME.items[7] != 0 || GAME.equippedItems[7] != 0) &&
        (GAME.items[0x59] != 0 || GAME.equippedItems[0x59] != 0) &&
        (GAME.items[0xA7] != 0 || GAME.equippedItems[0xA7] != 0)) {
        ret = 1;
    }
#elif VERSION_EU || VERSION_JP
    switch (op) {
    case 0:
        if ((GAME.items[7] != 0 || GAME.equippedItems[7] != 0) &&
            (GAME.items[0x59] != 0 || GAME.equippedItems[0x59] != 0) &&
            (GAME.items[0xA7] != 0 || GAME.equippedItems[0xA7] != 0)) {
            ret = 1;
        }
        break;
    case 1:
        if ((GAME.items[0x69] != 0 || GAME.equippedItems[0x69] != 0) &&
            (GAME.items[0x77] != 0 || GAME.equippedItems[0x77] != 0) &&
            (GAME.items[0x83] != 0 || GAME.equippedItems[0x83] != 0) &&
            (GAME.items[0x90] != 0 || GAME.equippedItems[0x90] != 0) &&
            (GAME.items[0x9C] != 0 || GAME.equippedItems[0x9C] != 0)) {
            ret = 1;
        }
        break;
    }
#endif
    return ret;
}

/*
 * Special conditions 0x10: tests partners (unlocked, in the party, levels) or unlocks one; the
 * European and Japanese versions add four
 */
s32 checkPartner(u32 op, s32 arg) {
    s32 result = 0;
    s32 i;
    s32 total;
    s32 id;
#if VERSION_EU || VERSION_JP
    s32 partner;
#endif

    switch (op) {
    case 0:
        if (GAME.partySet == arg) {
            result = 1;
        }
        break;
    case 1:
        if (GAME.partners[arg].unlocked != 0) {
            result = 1;
        }
        break;
    case 2:
        for (i = 0; i < PARTY_SIZE; i++) {
            if (GAME.funcs.getPartyMember(i) == arg) {
                result = 1;
                break;
            }
        }
        break;
    case 3:
        GAME.partners[arg].unlocked = arg + 3;
        result = 1;
        break;
    case 4:
        if (GAME.partners[arg].unlocked != 0 && GAME.partners[arg].info.stats[STAT_LEVEL] >= 0x2D) {
            result = 1;
        }
        break;
    case 5:
        total = 0;
        for (i = 0; i < PARTY_SIZE; i++) {
            id = GAME.funcs.getPartyMember(i);
            if (id >= 0) {
                total += GAME.funcs.getPartnerStats(id)->stats[STAT_LEVEL];
            }
        }
        if (total >= arg * 15 + 30) {
            result = 1;
        }
        break;
    case 6:
        if (GAME.partners[arg].unlocked == 0) {
            result = 1;
        }
        break;
#if VERSION_EU || VERSION_JP
    case 7:
        result = 1;
        for (id = 0; id < PARTNER_COUNT; id++) {
            if (GAME.partners[id].unlocked != 0 && GAME.partners[id].info.stats[STAT_LEVEL] < 0x2D) {
                result = 0;
                break;
            }
        }
        break;
    case 8:
        for (i = 0; i < PARTY_SIZE; i++) {
            partner = GAME.funcs.getPartyMember(i);
            if (partner >= 0) {
                GAME.partners[partner].info.stats[STAT_HP] = GAME.partners[partner].info.stats[STAT_MAX_HP];
                GAME.partners[partner].info.stats[STAT_MP] = GAME.partners[partner].info.stats[STAT_MAX_MP];
            }
        }
        result = 1;
        break;
    case 9:
        if (GAME.partners[0].unlocked != 0 && GAME.partners[1].unlocked != 0 && GAME.partners[2].unlocked != 0 &&
            GAME.partners[3].unlocked != 0 && GAME.partners[4].unlocked != 0 && GAME.partners[5].unlocked != 0 &&
            GAME.partners[6].unlocked != 0 && GAME.partners[7].unlocked != 0) {
            result = 1;
        }
        break;
#endif
    }
    return result;
}

/*
 * Special conditions 0x20: whether the player has MONEY_REQUIRED[item], or gives or takes an
 * amount
 */
s32 checkMoney(s32 op, s32 item) {
    s32 ret = 0;

    switch (op) {
    case 0:
        if (MONEY_REQUIRED[item] <= GAME.money) {
            ret = 1;
        }
        break;
    case 1:
        GAME.money += MONEY_GAINS[item];
        if (GAME.money > MONEY_MAX) {
            GAME.money = MONEY_MAX;
        }
        break;
    case 2:
        GAME.money -= MONEY_LOSSES[item];
        if (GAME.money < 0) {
            GAME.money = 0;
        }
        break;
    }
    return ret;
}

/* Special conditions 0x30: whether GAME.progress is within PROGRESS_RANGES[index] */
s32 checkProgressRange(s32 unused, s32 index) {
    s32 value = GAME.progress;
    s32 min = PROGRESS_RANGES[index][0];
    s32 max = PROGRESS_RANGES[index][1];
    s32 ret = 0;

    if (value >= min) {
        ret = max >= value;
    }
    return ret;
}

#if VERSION_US
/* Special conditions 0x40: counts flags 0x27-0x2D of GAME.flags1C; mode 0 is whether any is set,
   1 whether two or more are clear, 2 whether exactly one is clear */
s32 checkFlagCount(s32 unused, s32 mode) {
    s32 ret = 0;
    s32 on = 0;
    s32 off = 0;
    s32 i;

    for (i = 0x27; i < 0x2E; i++) {
        if (testBit(GAME.flags1C, i, 1) != 0) {
            on++;
        } else {
            off++;
        }
    }
    switch (mode) {
    case 0:
        if (on != 0) {
            ret = 1;
        }
        break;
    case 1:
        if (off >= 2) {
            ret = 1;
        }
        break;
    case 2:
        if (off == 1) {
            ret = 1;
        }
        break;
    }
    return ret;
}
#elif VERSION_EU || VERSION_JP
/* Special conditions 0x40: group 0 as in the USA version; group 1 whether four flags of group 0x10 are set */
s32 checkFlagCount(s32 group, s32 mode) {
    s32 ret = 0;
    s32 on = 0;
    s32 off = 0;
    s32 i;

    switch (group) {
    case 0:
        for (i = 0x27; i < 0x2E; i++) {
            if (testBit(GAME.flags1C, i, 1) != 0) {
                on++;
            } else {
                off++;
            }
        }
        switch (mode) {
        case 0:
            if (on != 0) {
                ret = 1;
            }
            break;
        case 1:
            if (off >= 2) {
                ret = 1;
            }
            break;
        case 2:
            if (off == 1) {
                ret = 1;
            }
            break;
        }
        break;
    case 1:
        if (testBit(GAME.flags10, 0xD, 1) && testBit(GAME.flags10, 0xE, 1) && testBit(GAME.flags10, 0xF, 1) &&
            testBit(GAME.flags10, 0x10, 1)) {
            ret = 1;
        }
        break;
    }
    return ret;
}
#endif

/* Special condition 0x50: the field actor's icon (FIELDSTG's task 0x16, FIELD_TASK_ICON)
   plays its animation 3, as event command 0x34A does */
s32 showActorIcon(s32 op, s32 arg) {
    Task *obj = TASK_REGISTRY.funcs.find(0x16, -1, -1);

    obj->setSubstate(obj, 3);
    return 1;
}

/* Runs SPECIAL_CONDITIONS entry `id`; whether its result equals `expected` */
s32 checkSpecialCondition(s32 id, s32 expected) {
    SpecialCondition *cond;
    s32 result = 0;
    s32 op;
    s32 arg;

    for (cond = SPECIAL_CONDITIONS; cond->id != 0xFF; cond++) {
        if (cond->id == id) {
            op = cond->check & 0xF;
            arg = cond->arg;
            switch (cond->check & 0xF0) {
            case 0x00:
                result = checkItemSet(op, arg);
                break;
            case 0x10:
                result = checkPartner(op, arg);
                break;
            case 0x20:
                result = checkMoney(op, arg);
                break;
            case 0x30:
                result = checkProgressRange(op, arg);
                break;
            case 0x40:
                result = checkFlagCount(op, arg);
                break;
            case 0x50:
                result = showActorIcon(op, arg);
                break;
            }
            break;
        }
    }
    return expected == result;
}

/* Conditions 0x60: whether GAME.progress is `value` (mode != 0) or not */
s32 checkProgress(s32 value, s32 mode) {
    if (mode != 0) {
        if (GAME.progress == value) {
            return 1;
        }
    } else {
        if (GAME.progress != value) {
            return 1;
        }
    }
    return 0;
}

/*
 * Conditions 0x80-0x8E: whether the player has item `index`, in the bag or equipped (mode != 0),
 * or not
 */
s32 checkItem(s32 index, s32 mode) {
    if (mode != 0) {
        if (GAME.items[index] != 0 || GAME.equippedItems[index] != 0) {
            return 1;
        }
    } else {
        if (GAME.items[index] == 0 && GAME.equippedItems[index] == 0) {
            return 1;
        }
    }
    return 0;
}

/* Conditions 0x92: whether the player has card `item` (have != 0) or not */
s32 checkCard(s32 card, s32 have) {
    if (have != 0) {
        if (GAME.cards[card] != 0) {
            return 1;
        }
    } else {
        if (GAME.cards[card] == 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Conditions 0x72: whether the party's total of battle stat 5 reaches PARTY_STAT_THRESHOLDS[index]
 * (mode != 0) or not
 */
s32 checkPartyStat(s32 index, s32 mode) {
    PartnerTotals buf;
    s32 total = 0;
    s32 i;
    s32 id;

    for (i = 0; i < PARTY_SIZE; i++) {
        id = GAME.funcs.getPartyMember(i);
        if (id >= 0) {
            GAME.funcs.computeStats(id, &buf);
            total += buf.fields.battle[5];
        }
    }
    if (mode != 0) {
        if (total >= PARTY_STAT_THRESHOLDS[index]) {
            return 1;
        }
    } else {
        if (total < PARTY_STAT_THRESHOLDS[index]) {
            return 1;
        }
    }
    return 0;
}

/* Conditions 0x7E: the values a warp leaves for its stage (GAME.place for
   ids under 30, GAME.placeArg above) */
s32 checkWarpArg(s32 id, s32 mode) {
#if VERSION_US
    if (id < 30) {
        if (GAME.place == id + 1) {
            return 1;
        }
    } else {
        if (GAME.placeArg == id - 29) {
            return 1;
        }
    }
#elif VERSION_EU || VERSION_JP
    /* mode zero: the opposite test, as checkProgress's */
    if (id < 30) {
        if (mode != 0) {
            if (GAME.place == id + 1) {
                return 1;
            }
        } else {
            if (GAME.place != id + 1) {
                return 1;
            }
        }
    } else {
        if (mode != 0) {
            if (GAME.placeArg == id - 29) {
                return 1;
            }
        } else {
            if (GAME.placeArg != id - 29) {
                return 1;
            }
        }
    }
#endif
    return 0;
}

/* Takes item `item` off a partner (one of type 7 empties slots 2 and 3 both); 1 if it had it */
s32 unequipItem(s32 partner, s32 item) {
    PartnerStats *d = &GAME.partners[partner].info;
    u8 *info = ITEM_FUNCS.get(item)->data.bytes;
    s16 *equip = d->equip;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (equip[i] == item) {
            if (info[2] == 7) {
                d->equip[2] = 0;
                d->equip[3] = 0;
            } else {
                equip[i] = 0;
            }
            return 1;
        }
    }
    return 0;
}

/* Actions 0x80-0x8E: gives an item (up to 99) or takes one, from the bag or else off a partner */
void changeItem(s32 item, s32 add) {
    s32 i;

    if (add != 0) {
        if (++GAME.items[item] >= 100) {
            GAME.items[item] = 99;
        }
    } else if (GAME.items[item] != 0) {
        if (--GAME.items[item] < 0) {
            GAME.items[item] = 0;
        }
    } else if (GAME.equippedItems[item] != 0) {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (unequipItem(GAME.funcs.getPartyMember(i), item) != 0) {
                goto found;
            }
        }
        for (i = 0; i < PARTNER_COUNT; i++) {
            if (GAME.partners[i].unlocked >= 3 && unequipItem(i, item) != 0) {
                break;
            }
        }
    found:
        GAME.equippedItems[item]--;
    }
}

/* Actions 0x92: gives or takes a copy of a card */
void changeCard(s32 card, s32 add) {
    if (add != 0) {
        GAME.funcs.addCards(card, 1);
        return;
    }
    GAME.cards[card]--;
    if (GAME.cards[card] < 0) {
        GAME.cards[card] = 0;
    }
}

/* Actions 0x76 and 0x78: a card battle (MODE_CARD_GAME) against an opponent */
void startCardBattle(s32 opponent, s32 kind) {
    FIELDSTG_leaveField(MODE_CARD_GAME, opponent * 2 + kind + 1, 0, 0, 0);
}

/*
 * Event scripts test conditions and run actions given as (code, value)
 * pairs. code >> 9 picks the kind, code & 0x1FF the flag or item:
 *   groups 0x00-0x40 (code >> 8 & ~1): the flag bitsets FLAGS_00 and
 *   GAME.flags02-flags40
 *   0x60 GAME.progress, 0x70 SPECIAL_CONDITIONS, 0x72 party stat totals,
 *   0x80-0x8E items, 0x92 cards. A condition holds when it equals value.
 */
s32 checkCondition(u16 code, u16 value) {
    u16 group = (code >> 8) & 0xFE;
    s32 id = code & 0x1FF;
    u16 arg = value;

    if (group == 0x00) {
        return testBit(FLAGS_00.bits, id, arg);
    } else if (group == 0x02) {
        return testBit(GAME.flags02, id, arg);
    } else if (group == 0x04) {
        return testBit(GAME.flags04, id, arg);
    } else if (group == 0x06) {
        return testBit(GAME.flags06, id, arg);
    } else if (group == 0x08) {
        return testBit(GAME.flags08, id, arg);
    } else if (group == 0x0A) {
        return testBit(GAME.flags0A, id, arg);
    } else if (group == 0x0C) {
        return testBit(GAME.flags0C, id, arg);
    } else if (group == 0x0E) {
        return testBit(GAME.flags0E, id, arg);
    } else if (group == 0x10) {
        return testBit(GAME.flags10, id, arg);
    } else if (group == 0x18) {
        return testBit(GAME.flags18, id, arg);
    } else if (group == 0x1A) {
        return testBit(GAME.flags1A, id, arg);
    } else if (group == 0x1C) {
        return testBit(GAME.flags1C, id, arg);
    } else if (group == 0x20) {
        return testBit(GAME.flags20, id, arg);
    } else if (group == 0x40) {
        return testBit(GAME.flags40, id, arg);
    } else if (group == 0x60) {
        return checkProgress(id, arg);
    } else if (group == 0x70) {
        return checkSpecialCondition(id, arg);
    } else if (group == 0x72) {
        return checkPartyStat(id, arg);
    } else if (group == 0x7E) {
        return checkWarpArg(id, arg);
    } else if (group >= 0x80 && group < 0x8F) {
        return checkItem(id, arg);
    } else if (group == 0x92) {
        return checkCard(id, arg);
    }
    return 1;
}

/*
 * Actions: the same flag groups (set to value), 0x70 special actions, 0x74
 * an overlay hook, 0x76/0x78, 0x7A, 0x7C, 0x94 mode changes through the
 * overlay (0x8008AEB4), 0x80-0x8E give/take an item, 0x92 a card.
 */
void applyAction(s32 code, s32 value) {
    u16 group = (code >> 8) & ~1;
    s32 id = code & 0x1FF;

    if (group == 0x00) {
        setBit(FLAGS_00.bits, id, value);
    }
    if (group == 0x02) {
        setBit(GAME.flags02, id, value);
    }
    if (group == 0x04) {
        setBit(GAME.flags04, id, value);
    }
    if (group == 0x06) {
        setBit(GAME.flags06, id, value);
    }
    if (group == 0x08) {
        setBit(GAME.flags08, id, value);
    }
    if (group == 0x0A) {
        setBit(GAME.flags0A, id, value);
    }
    if (group == 0x0C) {
        setBit(GAME.flags0C, id, value);
    }
    if (group == 0x0E) {
        setBit(GAME.flags0E, id, value);
    }
    if (group == 0x10) {
        setBit(GAME.flags10, id, value);
    }
    if (group == 0x18) {
        setBit(GAME.flags18, id, value);
    }
    if (group == 0x1A) {
        setBit(GAME.flags1A, id, value);
    }
    if (group == 0x1C) {
        setBit(GAME.flags1C, id, value);
    }
    if (group == 0x20) {
        setBit(GAME.flags20, id, value);
    }
    if (group == 0x40) {
        setBit(GAME.flags40, id, value);
    }
    if (group == 0x70) {
        checkSpecialCondition(id, 1);
    }
    if (group == 0x74) {
        FIELDSTG_battleFuncs.startEventBattle(id);
    }
    if (group == 0x76) {
        startCardBattle(id, 0);
    }
    if (group == 0x78) {
        startCardBattle(id, 1);
    }
    if (group >= 0x80 && group < 0x8F) {
        changeItem(id, value);
    }
    if (group == 0x90) {
        FIELDSTG_startListedEvent(id);
    }
    if (group == 0x92) {
        changeCard(id, value);
    }
    if (group == 0x94) {
        FIELDSTG_leaveField(MODE_TRAINING, id, 0, 0, 0);
    }
    if (group == 0x7A) {
        if ((u16)id < 30) {
            FIELDSTG_leaveField(MODE_ITEM_SHOP, (u16)id, 0, 0, 0);
        } else if ((u32)(id - 0x31) < 0x13 || (u32)(id - 0x46) < 5) {
            FIELDSTG_leaveField(MODE_CARD_SHOP, (u16)id, 0, 0, 0);
        } else {
            FIELDSTG_openInn();
        }
    }
    if (group == 0x7C) {
        if ((u16)id == 0) {
            FIELDSTG_leaveField(MODE_DIGI_LAB, 0, 0, 0, 0);
        } else if ((u16)id == 1) {
            FIELDSTG_leaveField(MODE_NAMING, 0, 0, 0, 0);
        }
    }
}

/* All the (code, value) pairs until 0xFFFF must hold */
s32 checkConditions(u16 *list) {
    u16 a;

    for (a = *list; a != 0xFFFF; a = *list) {
        list++;
        if (!checkCondition(a, *list++)) {
            return 0;
        }
    }
    return 1;
}

/* Runs every (code, value) action until 0xFFFF */
void applyActions(u16 *list) {
    u16 a;

    for (a = *list; a != 0xFFFF; a = *list) {
        list++;
        applyAction(a, *list++);
    }
}

/*
 * On a mode change: clears the temporary flags if asked, and sets flags 0x10-0x12 after a card
 * battle
 */
void updateModeFlags(void) {
    s32 i;
    u8 *p;

    if (GAME.clearTempFlags != 0) {
        for (i = 2, p = &FLAGS_00.bits[i]; i >= 0; i--) {
            *p-- = 0;
        }
        applyAction(0x12, 0);
    }
    if (GAME.funcs.getPrevMode() == MODE_CARD_GAME) {
        applyAction(0x11, 1);
        applyAction(0x12, 1);
        if (FLAGS_00.pendingFlag10 != 0) {
            applyAction(0x10, 1);
        } else {
            applyAction(0x10, 0);
        }
        FLAGS_00.pendingFlag10 = 0;
    }
}

/* The flag bitset FLAGS_00 and the event functions */
GameFlags FLAGS_00 = {
    { 0 }, 0, checkConditions, applyAction, checkCondition, applyActions, updateModeFlags,
};

/* checkSpecialCondition's conditions, up to an id of 0xFF */
SpecialCondition SPECIAL_CONDITIONS[] = {
#if VERSION_US
    { 0x3A, 0x00, 0x00 }, { 0x00, 0x10, 0x00 }, { 0x01, 0x10, 0x01 }, { 0x02, 0x10, 0x02 }, { 0x10, 0x11, 0x00 },
    { 0x12, 0x11, 0x01 }, { 0x11, 0x11, 0x02 }, { 0x0B, 0x11, 0x03 }, { 0x0D, 0x11, 0x04 }, { 0x0C, 0x11, 0x05 },
    { 0x0F, 0x11, 0x06 }, { 0x0E, 0x11, 0x07 }, { 0x42, 0x12, 0x00 }, { 0x44, 0x12, 0x01 }, { 0x43, 0x12, 0x02 },
    { 0x47, 0x12, 0x03 }, { 0x46, 0x12, 0x04 }, { 0x45, 0x12, 0x05 }, { 0x49, 0x12, 0x06 }, { 0x48, 0x12, 0x07 },
    { 0x37, 0x13, 0x00 }, { 0x39, 0x13, 0x01 }, { 0x38, 0x13, 0x02 }, { 0x31, 0x13, 0x03 }, { 0x32, 0x13, 0x04 },
    { 0x33, 0x13, 0x05 }, { 0x35, 0x13, 0x06 }, { 0x34, 0x13, 0x07 }, { 0x4A, 0x14, 0x00 }, { 0x4C, 0x14, 0x01 },
    { 0x4B, 0x14, 0x02 }, { 0x4F, 0x14, 0x03 }, { 0x4E, 0x14, 0x04 }, { 0x4D, 0x14, 0x05 }, { 0x51, 0x14, 0x06 },
    { 0x50, 0x14, 0x07 }, { 0x25, 0x15, 0x00 }, { 0x26, 0x15, 0x01 }, { 0x27, 0x15, 0x02 }, { 0x28, 0x15, 0x03 },
    { 0x29, 0x15, 0x04 }, { 0x2E, 0x16, 0x00 }, { 0x30, 0x16, 0x01 }, { 0x2F, 0x16, 0x02 }, { 0x2A, 0x16, 0x03 },
    { 0x2B, 0x16, 0x04 }, { 0x2C, 0x16, 0x05 }, { 0x36, 0x16, 0x06 }, { 0x2D, 0x16, 0x07 }, { 0x74, 0x20, 0x00 },
    { 0x75, 0x20, 0x01 }, { 0x76, 0x20, 0x02 }, { 0x77, 0x20, 0x03 }, { 0x78, 0x20, 0x04 }, { 0x79, 0x20, 0x05 },
    { 0x7A, 0x20, 0x06 }, { 0x7B, 0x20, 0x07 }, { 0x7C, 0x20, 0x08 }, { 0x7D, 0x20, 0x09 }, { 0x8B, 0x21, 0x00 },
    { 0x8C, 0x21, 0x01 }, { 0x8D, 0x21, 0x02 }, { 0x8E, 0x21, 0x03 }, { 0x8F, 0x21, 0x04 }, { 0x90, 0x21, 0x05 },
    { 0x91, 0x21, 0x06 }, { 0x92, 0x21, 0x07 }, { 0x7E, 0x22, 0x00 }, { 0x7F, 0x22, 0x01 }, { 0x80, 0x22, 0x02 },
    { 0x81, 0x22, 0x03 }, { 0x82, 0x22, 0x04 }, { 0x83, 0x22, 0x05 }, { 0x84, 0x22, 0x06 }, { 0x85, 0x22, 0x07 },
    { 0x86, 0x22, 0x08 }, { 0x87, 0x22, 0x09 }, { 0x06, 0x30, 0x00 }, { 0x03, 0x30, 0x01 }, { 0x0A, 0x30, 0x02 },
    { 0x07, 0x30, 0x03 }, { 0x04, 0x30, 0x04 }, { 0x05, 0x30, 0x05 }, { 0x09, 0x30, 0x06 }, { 0x14, 0x30, 0x07 },
    { 0x08, 0x30, 0x08 }, { 0x3F, 0x30, 0x09 }, { 0x40, 0x30, 0x0A }, { 0x41, 0x30, 0x0B }, { 0x93, 0x30, 0x0C },
    { 0x94, 0x30, 0x0D }, { 0x15, 0x30, 0x0E }, { 0x16, 0x30, 0x0F }, { 0x17, 0x30, 0x10 }, { 0x18, 0x30, 0x11 },
    { 0x19, 0x30, 0x12 }, { 0x1A, 0x30, 0x13 }, { 0x1C, 0x30, 0x14 }, { 0x1D, 0x30, 0x15 }, { 0x21, 0x30, 0x16 },
    { 0x1E, 0x30, 0x17 }, { 0x1F, 0x30, 0x18 }, { 0x20, 0x30, 0x19 }, { 0x22, 0x30, 0x1A }, { 0x23, 0x30, 0x1B },
    { 0x24, 0x30, 0x1C }, { 0x3B, 0x30, 0x1D }, { 0x3C, 0x30, 0x1E }, { 0x3D, 0x30, 0x1F }, { 0x3E, 0x30, 0x20 },
    { 0x71, 0x40, 0x00 }, { 0x70, 0x40, 0x01 }, { 0x73, 0x40, 0x02 }, { 0x72, 0x40, 0x03 }, { 0x13, 0x50, 0x00 },
    { 0xFF },
#elif VERSION_EU || VERSION_JP
    { 0x3A, 0x00, 0x00 }, { 0x96, 0x01, 0x00 }, { 0x00, 0x10, 0x00 }, { 0x01, 0x10, 0x01 }, { 0x02, 0x10, 0x02 },
    { 0x10, 0x11, 0x00 }, { 0x12, 0x11, 0x01 }, { 0x11, 0x11, 0x02 }, { 0x0B, 0x11, 0x03 }, { 0x0D, 0x11, 0x04 },
    { 0x0C, 0x11, 0x05 }, { 0x0F, 0x11, 0x06 }, { 0x0E, 0x11, 0x07 }, { 0x42, 0x12, 0x00 }, { 0x44, 0x12, 0x01 },
    { 0x43, 0x12, 0x02 }, { 0x47, 0x12, 0x03 }, { 0x46, 0x12, 0x04 }, { 0x45, 0x12, 0x05 }, { 0x49, 0x12, 0x06 },
    { 0x48, 0x12, 0x07 }, { 0x37, 0x13, 0x00 }, { 0x39, 0x13, 0x01 }, { 0x38, 0x13, 0x02 }, { 0x31, 0x13, 0x03 },
    { 0x32, 0x13, 0x04 }, { 0x33, 0x13, 0x05 }, { 0x35, 0x13, 0x06 }, { 0x34, 0x13, 0x07 }, { 0x4A, 0x14, 0x00 },
    { 0x4C, 0x14, 0x01 }, { 0x4B, 0x14, 0x02 }, { 0x4F, 0x14, 0x03 }, { 0x4E, 0x14, 0x04 }, { 0x4D, 0x14, 0x05 },
    { 0x51, 0x14, 0x06 }, { 0x50, 0x14, 0x07 }, { 0x25, 0x15, 0x00 }, { 0x26, 0x15, 0x01 }, { 0x27, 0x15, 0x02 },
    { 0x28, 0x15, 0x03 }, { 0x29, 0x15, 0x04 }, { 0x2E, 0x16, 0x00 }, { 0x30, 0x16, 0x01 }, { 0x2F, 0x16, 0x02 },
    { 0x2A, 0x16, 0x03 }, { 0x2B, 0x16, 0x04 }, { 0x2C, 0x16, 0x05 }, { 0x36, 0x16, 0x06 }, { 0x2D, 0x16, 0x07 },
    { 0x95, 0x17, 0x00 }, { 0x53, 0x18, 0x00 }, { 0x55, 0x19, 0x00 }, { 0x74, 0x20, 0x00 }, { 0x75, 0x20, 0x01 },
    { 0x76, 0x20, 0x02 }, { 0x77, 0x20, 0x03 }, { 0x78, 0x20, 0x04 }, { 0x79, 0x20, 0x05 }, { 0x7A, 0x20, 0x06 },
    { 0x7B, 0x20, 0x07 }, { 0x7C, 0x20, 0x08 }, { 0x7D, 0x20, 0x09 }, { 0x8B, 0x21, 0x00 }, { 0x8C, 0x21, 0x01 },
    { 0x8D, 0x21, 0x02 }, { 0x8E, 0x21, 0x03 }, { 0x8F, 0x21, 0x04 }, { 0x90, 0x21, 0x05 }, { 0x91, 0x21, 0x06 },
    { 0x92, 0x21, 0x07 }, { 0x7E, 0x22, 0x00 }, { 0x7F, 0x22, 0x01 }, { 0x80, 0x22, 0x02 }, { 0x81, 0x22, 0x03 },
    { 0x82, 0x22, 0x04 }, { 0x83, 0x22, 0x05 }, { 0x84, 0x22, 0x06 }, { 0x85, 0x22, 0x07 }, { 0x86, 0x22, 0x08 },
    { 0x87, 0x22, 0x09 }, { 0x06, 0x30, 0x00 }, { 0x03, 0x30, 0x01 }, { 0x0A, 0x30, 0x02 }, { 0x07, 0x30, 0x03 },
    { 0x04, 0x30, 0x04 }, { 0x05, 0x30, 0x05 }, { 0x09, 0x30, 0x06 }, { 0x14, 0x30, 0x07 }, { 0x08, 0x30, 0x08 },
    { 0x3F, 0x30, 0x09 }, { 0x40, 0x30, 0x0A }, { 0x41, 0x30, 0x0B }, { 0x93, 0x30, 0x0C }, { 0x94, 0x30, 0x0D },
    { 0x15, 0x30, 0x0E }, { 0x16, 0x30, 0x0F }, { 0x17, 0x30, 0x10 }, { 0x18, 0x30, 0x11 }, { 0x19, 0x30, 0x12 },
    { 0x1A, 0x30, 0x13 }, { 0x1C, 0x30, 0x14 }, { 0x1D, 0x30, 0x15 }, { 0x21, 0x30, 0x16 }, { 0x1E, 0x30, 0x17 },
    { 0x1F, 0x30, 0x18 }, { 0x20, 0x30, 0x19 }, { 0x22, 0x30, 0x1A }, { 0x23, 0x30, 0x1B }, { 0x24, 0x30, 0x1C },
    { 0x3B, 0x30, 0x1D }, { 0x3C, 0x30, 0x1E }, { 0x3D, 0x30, 0x1F }, { 0x3E, 0x30, 0x20 }, { 0x71, 0x40, 0x00 },
    { 0x70, 0x40, 0x01 }, { 0x73, 0x40, 0x02 }, { 0x72, 0x40, 0x03 }, { 0x54, 0x41, 0x00 }, { 0x13, 0x50, 0x00 },
    { 0xFF },
#endif
};

/* checkMoney's amounts, by its argument */
s32 MONEY_REQUIRED[] = {
    800, 1600, 2700, 4000, 6000, 8700, 11500, 17500, 24000, 32000,
};
s32 MONEY_GAINS[] = {
    100, 300, 600, 1000, 1600, 3000, 5000, 8500,
};
s32 MONEY_LOSSES[] = {
    800, 1600, 2700, 4000, 6000, 8700, 11500, 17500, 24000, 32000,
};

/* checkProgressRange's ranges of GAME.progress: { min, max } */
u8 PROGRESS_RANGES[][2] = {
    { 0x02, 0x12 }, { 0x04, 0x17 }, { 0x04, 0x2C }, { 0x14, 0x17 }, { 0x18, 0x25 }, { 0x27, 0x2A },
    { 0x04, 0x63 }, { 0x07, 0x63 }, { 0x18, 0x63 }, { 0x16, 0x2A }, { 0x12, 0x63 }, { 0x14, 0x63 },
    { 0x0F, 0x63 }, { 0x1E, 0x63 }, { 0x05, 0x0A }, { 0x0F, 0x12 }, { 0x14, 0x16 }, { 0x18, 0x1A },
    { 0x1B, 0x25 }, { 0x27, 0x28 }, { 0x25, 0x26 }, { 0x1B, 0x24 }, { 0x22, 0x26 }, { 0x1B, 0x26 },
    { 0x1C, 0x25 }, { 0x1B, 0x21 }, { 0x04, 0x26 }, { 0x04, 0x15 }, { 0x0E, 0x26 }, { 0x11, 0x26 },
    { 0x0A, 0x0D }, { 0x1D, 0x26 }, { 0x0C, 0x16 }, { 0x00, 0x00 },
};

/* checkPartyStat's thresholds */
s32 PARTY_STAT_THRESHOLDS[] = {
    60, 150, 210, 285, 378, 492, 630, 795, 990, 1218, 1482, 1785, 2049, 2277, 2472,
};
