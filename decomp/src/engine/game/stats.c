#include "engine/game.h"
#include "field/field_map.h"

/* The partner id of party member `index` (its unlocked value - 3), or -1 */
s32 getPartyPartner(u32 index) {
    if (index >= PARTY_SIZE) {
        return -1;
    }
    return GAME.partners[GAME.party[index]].unlocked - 3;
}

/* Sets a partner's stat, kept within 0-99 (level, TP), 0-9999 (HP, MP) or 0-999 */
void setStat(s32 partner, u32 stat, s16 value) {
    PartnerStats *d = &GAME.partners[partner].info;
    s16 *p = d->stats;

    if (stat < STAT_COUNT) {
        p += stat;
        *p = value;
        if (value < 0) {
            *p = 0;
            return;
        }
        if (stat < STAT_HP) {
            if (value >= 100) {
                *p = 99;
            }
        } else if (stat - STAT_HP < 4) {
            if (value >= 10000) {
                *p = 9999;
            }
        } else if (value >= 1000) {
            *p = 999;
        }
    }
}

/* Adds to a partner's stat, kept within the same limits as setStat */
void addStat(s32 partner, u32 stat, s32 delta) {
    PartnerStats *d = &GAME.partners[partner].info;
    s16 *stats = d->stats;
    s16 value;

    if (stat < STAT_COUNT) {
        stats += stat;
        value = *stats + delta;
        *stats = value;
        if (value < 0) {
            *stats = 0;
        } else if (stat < STAT_HP) {
            if (value >= 100) {
                *stats = 99;
            }
        } else if (stat - STAT_HP < 4) {
            if (value >= 10000) {
                *stats = 9999;
            }
        } else if (value >= 1000) {
            *stats = 999;
        }
    }
}

/* A partner's stats with its equipment (and its equipment set bonus) added */
void computeStats(s32 partner, PartnerTotals *out) {
    s16 *equip;
    s32 i;
    s32 j;
    ItemInfo *info;
    ItemData *data;
    u8 type;
    u8 stat;
    s32 amount;

    GameState *save = &GAME;
    PartnerStats *d;

    *(StatBlock *)out = *(StatBlock *)save->partners[partner].info.stats;
    d = &GAME.partners[partner].info;
    equip = d->equip;
    for (i = 0; i < 6; i++) {
        if (equip[i] > 0) {
            info = ITEM_FUNCS.get(equip[i]);
            type = info->type;
            data = info->data.record;
            if (IS_WEAPON_TYPE(type)) {
                out->fields.battle[0] += data->weapon.atk;
                if (out->fields.battle[0] >= 1000) {
                    out->fields.battle[0] = 999;
                }
                for (j = 0; j < 2; j++) {
                    stat = *(j + data->weapon.stats);
                    amount = data->weapon.amounts[j];
                    if (stat != 0) {
                        addStatBonus(out->stats, stat, (s16)amount);
                    }
                }
            } else if (IS_ARMOR_TYPE(type)) {
                out->fields.battle[1] += data->armor.def;
                if (out->fields.battle[1] >= 1000) {
                    out->fields.battle[1] = 999;
                }
                for (j = 0; j < 2; j++) {
                    stat = *(j + data->armor.stats);
                    amount = data->armor.amounts[j];
                    if (stat != 0) {
                        addStatBonus(out->stats, stat, (s16)amount);
                    }
                }
            } else if (IS_ACCESSORY_TYPE(type)) {
                stat = data->acc.stat;
                amount = data->acc.amount;
                if (stat != 0) {
                    addStatBonus(out->stats, stat, (s16)amount);
                }
            } else {
                continue;
            }
            out->fields.battle[5] += data->weapon.charisma;
            if (out->fields.battle[5] >= 1000) {
                out->fields.battle[5] = 999;
            }
        }
    }
    out->fields.battle[0] -= out->fields.lowered[0];
    if (out->fields.battle[0] < 0) {
        out->fields.battle[0] = 0;
    }
    out->fields.battle[1] -= out->fields.lowered[1];
    if (out->fields.battle[1] < 0) {
        out->fields.battle[1] = 0;
    }
    out->fields.battle[4] -= out->fields.lowered[2];
    if (out->fields.battle[4] < 0) {
        out->fields.battle[4] = 0;
    }
    if (equip[0] == EQUIP_SETS[partner].items[0] && equip[1] == EQUIP_SETS[partner].items[1] &&
        equip[2] == EQUIP_SETS[partner].items[2] && equip[3] == EQUIP_SETS[partner].items[3]) {
        for (i = 0; i < 6; i++) {
            out->fields.battle[i] += EQUIP_SET_BONUSES[partner][i];
        }
    }
}

/* Adds an equipment bonus to a stat of computeStats' result (7 raises every battle stat) */
void addStatBonus(s16 *p, s32 stat, s32 delta) {
    s32 i;
    s16 value;

    if (stat == 7) {
        for (i = 0; i < 6; i++) {
            value = p[i + 6] + delta;
            p[i + 6] = value;
            if (value >= 1000) {
                p[i + 6] = 999;
            }
        }
    } else if (stat - 1 < 6U) {
        value = p[stat + 5] + delta;
        p[stat + 5] = value;
        if (value >= 1000) {
            p[stat + 5] = 999;
        }
    } else if (stat - 8 < 7U) {
        value = p[stat + 4] + delta;
        p[stat + 4] = value;
        if (value >= 1000) {
            p[stat + 4] = 999;
        }
    }
}

/* Each partner's equipment set and what wearing it whole adds (computeStats) */
EquipSet EQUIP_SETS[] = {
    { { 0x00F3, 0x010D, 0x0062, 0x011C } }, { { 0x00F2, 0x0102, 0x0070, 0x011D } },
    { { 0x00DE, 0x0100, 0x007C, 0x011A } }, { { 0x00F4, 0x010F, 0x00A3, 0x011E } },
    { { 0x00D3, 0x010C, 0x00CB, 0x0120 } }, { { 0x00F5, 0x010E, 0x0095, 0x011F } },
    { { 0x00DD, 0x0101, 0x0089, 0x011B } }, { { 0x00E8, 0x00FF, 0x0088, 0x0119 } },
};
s16 EQUIP_SET_BONUSES[][6] = {
    { 4, 4, 4, 4, 4, 20 }, { 10, 10, 0, 0, 0, 20 }, { 10, 0, 0, 0, 10, 20 }, { 0, 10, 10, 0, 0, 20 },
    { 10, 0, 0, 10, 0, 20 }, { 0, 0, 0, 10, 10, 20 }, { 10, 0, 10, 0, 0, 20 }, { 0, 10, 0, 10, 0, 20 },
};
