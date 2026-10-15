/* The shop's helpers: a shop's stock, and who can equip an item, in which slot */

#include "menus/item_shop.h"

/* The items a shop sells (up to 0), noting how many in STITSHOP_funcs.count;
   NULL for none */
s16 *STITSHOP_getShopItems(s32 shop) {
    if (shop < 0 || STITSHOP_shops[shop].items == NULL) {
        return NULL;
    }
    STITSHOP_funcs.count = STITSHOP_shops[shop].count;
    return STITSHOP_shops[shop].items;
}

/* 1 when a partner can equip an item: the partner's bit in its partners */
s32 STITSHOP_canEquip(s32 partner, s32 item) {
    return (ITEM_FUNCS.get(item)->data.record->weapon.partners >> partner) & 1;
}

/* Which equipment slot a bought item would go in: 2 or 3 for the weapons (the
   one with the weaker item when both are taken), 4 or 5 for the accessories
   (the one with the same group, else the weaker one); -1 when it can't */
s32 STITSHOP_compareEquip(s32 partner, s32 item) {
    ItemData *data = ITEM_FUNCS.get(item)->data.record;
    PartnerStats *stats;
    ItemInfo *info;
    ItemData *first;
    ItemData *second;
    s32 equipped[2];
    ItemData *datas[2];
    s32 i;

    switch (data->weapon.kind) {
    case EQUIP_KIND_EITHER_HAND:
        stats = GAME.funcs.getPartnerStats(partner);
        if (stats->equip[2] > 0) {
            equipped[0] = stats->equip[2];
            if (stats->equip[3] > 0) {
                equipped[1] = stats->equip[3];
                first = ITEM_FUNCS.get(equipped[0])->data.record;
                info = ITEM_FUNCS.get(equipped[1]);
                second = info->data.record;
                if (first->weapon.kind == EQUIP_KIND_BOTH_HANDS || info->type == 0x14) {
                    return 2;
                }
                for (i = 0; i < 2; i++) {
                    info = ITEM_FUNCS.get(equipped[i]);
                    if (info->type < 2 || info->type > 14) {
                        return -1;
                    }
                }
                if (first->weapon.atk < second->weapon.atk) {
                    return 2;
                }
            }
            return 3;
        }
        return 2;
    case EQUIP_KIND_LEFT_HAND:
        return 3;
    case EQUIP_KIND_HEAD:
        return 0;
    case EQUIP_KIND_BODY:
        return 1;
    case EQUIP_KIND_ACCESSORY:
        stats = GAME.funcs.getPartnerStats(partner);
        if (stats->equip[4] > 0) {
            equipped[0] = stats->equip[4];
            if (stats->equip[5] > 0) {
                equipped[1] = stats->equip[5];
                datas[0] = ITEM_FUNCS.get(equipped[0])->data.record;
                datas[1] = ITEM_FUNCS.get(equipped[1])->data.record;
                /* and the accessories' bonuses signed */
                if ((s16)datas[0]->acc.amount < (s16)datas[1]->acc.amount) {
                    return 4;
                }
            }
            return 5;
        }
        return 4;
    case EQUIP_KIND_RIGHT_HAND:
    case EQUIP_KIND_BOTH_HANDS:
        return 2;
    case EQUIP_KIND_GROUP_ACCESSORY:
        stats = GAME.funcs.getPartnerStats(partner);
        if (stats->equip[4] > 0) {
            equipped[0] = stats->equip[4];
            if (stats->equip[5] <= 0) {
                return 5;
            }
            equipped[1] = stats->equip[5];
            datas[0] = ITEM_FUNCS.get(equipped[0])->data.record;
            if (datas[0]->acc.kind == EQUIP_KIND_GROUP_ACCESSORY && datas[0]->acc.group == data->acc.group) {
                return 4;
            }
            datas[1] = ITEM_FUNCS.get(equipped[1])->data.record;
            if (datas[1]->acc.kind == EQUIP_KIND_GROUP_ACCESSORY && datas[1]->acc.group == data->acc.group) {
                return 5;
            }
            break;
        }
        return 4;
    case 0:
    default:
        return -1;
    }
    if ((s16)datas[0]->acc.amount < (s16)datas[1]->acc.amount) {
        return 4;
    }
    return 5;
}

/* Puts an item in a partner's equipment slot like STSTATUS_equip, moving the
   counts between GAME.items and GAME.equippedItems only when fromBag is set */
void STITSHOP_equip(s32 partner, s32 slot, s32 item, s32 fromBag) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(partner);
    s16 *equip;
    s16 *pair;
    ItemData *data;
    s32 group;
    s32 old;
    s32 i;
    s32 id = item; /* the match depends on this copy, as in STSTATUS_equip */

    old = *(stats->equip + slot);
    if (old != 0) {
        if (fromBag) {
            GAME.equippedItems[old]--;
            GAME.items[old]++;
        }
        data = ITEM_FUNCS.get(old)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            stats->equip[2] = 0;
            stats->equip[3] = 0;
        } else {
            *(stats->equip + slot) = 0;
        }
    }
    if (id > 0) {
        data = ITEM_FUNCS.get(id)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            pair = &stats->equip[2];
            if (stats->equip[2] == 0) {
                pair = NULL;
                if (stats->equip[3] != 0) {
                    pair = &stats->equip[3];
                }
            }
            if (pair != NULL) {
                if (fromBag) {
                    GAME.equippedItems[*pair]--;
                    GAME.items[*pair]++;
                }
                *pair = 0;
            }
        } else if (data->acc.kind == EQUIP_KIND_GROUP_ACCESSORY) {
            group = data->acc.group;
            for (i = 0; i < 2; i++) {
                equip = &stats->equip[i + 4];
                if (*equip != 0) {
                    data = ITEM_FUNCS.get(*equip)->data.record;
                    if (data->acc.group == group) {
                        if (fromBag) {
                            GAME.equippedItems[*equip]--;
                            GAME.items[*equip]++;
                        }
                        *equip = 0;
                    }
                }
            }
        }
        if (fromBag) {
            GAME.equippedItems[id]++;
            GAME.items[id]--;
        }
        data = ITEM_FUNCS.get(id)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            stats->equip[2] = id;
            stats->equip[3] = id;
        } else {
            *(stats->equip + slot) = id;
        }
    }
}
