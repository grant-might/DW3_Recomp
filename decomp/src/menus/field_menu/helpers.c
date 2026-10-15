/* The helpers the screens call through STSTATUS_data and STSTATUS_areaFuncs:
   the towns, the item lists, equipping, the area; and the last object's data,
   which all the objects read */

#include "menus/field_menu.h"

/* The towns the map shows (a list up to 0), by STSTATUS_getGameHalf and
   GAME.progress (STSTATUS_townLists) */
s32 *STSTATUS_getTowns(s32 list, s32 index) {
    return STSTATUS_townLists[list][index];
}

/* Puts the owned items of a list in OUT and returns how many: below 5 the
   item lists of ITEM_FUNCS, 5 the equipment of STSTATUS_equipKinds, 6 and 7
   the items of kinds 4 and 5 (STSTATUS_slotLists) */
s32 STSTATUS_listItems(s32 list, s16 *out) {
    if (list < 5) {
        return ITEM_FUNCS.list(list, out);
    }
    switch (list) {
    case 5:
    default:
        return STSTATUS_listEquipItems(out);
    case 6:
        return STSTATUS_listItemsOfKind(4, out);
    case 7:
        return STSTATUS_listItemsOfKind(5, out);
    }
}

/* The items whose kind (WeaponData.kind) is one of STSTATUS_equipKinds's four */
s32 STSTATUS_listEquipItems(s16 *out) {
    s32 i;
    s32 j;
    s32 count;
    ItemData *data;

    STSTATUS_data.itemCount = ITEM_FUNCS.list(2, STSTATUS_data.items);
    STSTATUS_data.item2Count = ITEM_FUNCS.list(3, STSTATUS_data.items2);
    count = 0;
    for (i = 0; i < STSTATUS_data.itemCount; i++) {
        data = ITEM_FUNCS.get(STSTATUS_data.items[i])->data.record;
        for (j = 0; j < 4; j++) {
            if (data->weapon.kind == STSTATUS_equipKinds[j]) {
                out[count++] = STSTATUS_data.items[i];
            }
        }
    }
    for (i = 0; i < STSTATUS_data.item2Count; i++) {
        data = ITEM_FUNCS.get(STSTATUS_data.items2[i])->data.record;
        for (j = 0; j < 4; j++) {
            if (data->weapon.kind == STSTATUS_equipKinds[j]) {
                out[count++] = STSTATUS_data.items2[i];
            }
        }
    }
    return count;
}

/* The items of one kind (WeaponData.kind) */
s32 STSTATUS_listItemsOfKind(s32 kind, s16 *out) {
    s32 i;
    s32 count;

    STSTATUS_data.itemCount = ITEM_FUNCS.list(2, STSTATUS_data.items);
    STSTATUS_data.item2Count = ITEM_FUNCS.list(3, STSTATUS_data.items2);
    count = 0;
    for (i = 0; i < STSTATUS_data.itemCount; i++) {
        if (ITEM_FUNCS.get(STSTATUS_data.items[i])->data.record->weapon.kind == kind) {
            out[count++] = STSTATUS_data.items[i];
        }
    }
    for (i = 0; i < STSTATUS_data.item2Count; i++) {
        if (ITEM_FUNCS.get(STSTATUS_data.items2[i])->data.record->weapon.kind == kind) {
            out[count++] = STSTATUS_data.items2[i];
        }
    }
    return count;
}

/* Whether a partner can put an item in an equipment slot (-1: none): its
   partners has a bit per partner; kind 1 can't go in slot 3, nor 2 in slot 2 */
s32 STSTATUS_canEquip(s32 partner, s32 slot, s32 item) {
    ItemData *data;

    if (item != -1) {
        data = ITEM_FUNCS.get(item)->data.record;
        if (!((data->weapon.partners >> partner) & 1)) {
            return 0;
        }
        if (data->weapon.kind == EQUIP_KIND_RIGHT_HAND) {
            if (slot == 3) {
                return 0;
            }
        } else if (data->weapon.kind == EQUIP_KIND_LEFT_HAND) {
            if (slot == 2) {
                return 0;
            }
        }
    }
    return 1;
}

/* Puts an item in a partner's equipment slot (0 or less: empties it), moving
   the counts between GAME.items and GAME.equippedItems. Kind 7 takes slots 2
   and 3; kind 8 replaces one in slots 4 and 5 with the same group */
void STSTATUS_equip(s32 partner, s32 slot, s32 item) {
    PartnerStats *stats = GAME.funcs.getPartnerStats(partner);
    s16 *equip;
    s16 *pair;
    ItemData *data;
    s32 group;
    s32 old;
    s32 i;
    s32 id = item; /* the match depends on this copy and on *(stats->equip + slot) */

    old = *(stats->equip + slot);
    if (old != 0) {
        GAME.equippedItems[old]--;
        GAME.items[old]++;
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
                GAME.equippedItems[*pair]--;
                GAME.items[*pair]++;
                *pair = 0;
            }
        } else if (data->weapon.kind == EQUIP_KIND_GROUP_ACCESSORY) {
            group = data->weapon.group;
            for (i = 0; i < 2; i++) {
                equip = &stats->equip[i + 4];
                if (*equip != 0) {
                    data = ITEM_FUNCS.get(*equip)->data.record;
                    if (data->weapon.group == group) {
                        GAME.equippedItems[*equip]--;
                        GAME.items[*equip]++;
                        *equip = 0;
                    }
                }
            }
        }
        GAME.equippedItems[id]++;
        GAME.items[id]--;
        data = ITEM_FUNCS.get(id)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            stats->equip[2] = id;
            stats->equip[3] = id;
        } else {
            *(stats->equip + slot) = id;
        }
    }
}

/* 1 on the field maps of the second half of the game (0x270 on), 0 on those of
   the first, -1 past them (0x2D7 on) */
s32 STSTATUS_getGameHalf(void) {
    if (GAME.fieldMode >= 0x2D7) {
        return -1;
    }
    return GAME.fieldMode >= 0x270;
}

/* The map's area of the current field map (STSTATUS_mapAreas) */
s32 STSTATUS_getArea(void) {
    return STSTATUS_mapAreas[(u8)GAME.fieldMode] & 0x7F;
}

/* Sets OUT[area] to 1 for the map's areas of this half of the game that have a
   field map whose flag (0x2000 + the map's low byte) is set */
void STSTATUS_getVisitedAreas(s32 *out) {
    s32 first;
    s32 last;
    s32 i;
    s32 area;
    s32 found;

    if (STSTATUS_getGameHalf() == 0) {
        first = 0x200;
        last = 0x26F;
    } else {
        first = 0x270;
        last = 0x2D6;
    }
    for (i = first; i <= last; i++) {
        area = STSTATUS_mapAreas[i & 0xFF] & 0x7F;
        found = FLAGS_00.checkCondition((i & 0xFF) | 0x2000, 1);
        if (found == 1) {
            out[area] = found;
        }
    }
}

/* The map cursor's frames */
s32 STSTATUS_cursorFrames[] = {
    0, 1, 2, 1,
};

/* The field menu's screens, by FIELD_MENU_CHOICE */
Task *(*STSTATUS_screens[2][7])(FieldMenuScreen *menu, s32 extra) = {
    { STSTATUS_createItemScreen, STSTATUS_createSortScreen, STSTATUS_createMapScreen, STSTATUS_createTechScreen, STSTATUS_createStatusScreen, STSTATUS_createDemoScreen, NULL },
    { STSTATUS_createItemScreen, STSTATUS_createSortScreen, STSTATUS_createMapScreen, STSTATUS_createTechScreen, STSTATUS_createStatusScreen, STSTATUS_createCardScreen, STSTATUS_createDemoScreen },
};
/* The partners' portrait animations */
PartnerAnim STSTATUS_partnerAnims[] = {
    { { 7, 8, 9, 10, 9, 8, -1 } },
    { { 14, 15, 16, 15, -1, -1, -1 } },
    { { 11, 12, 13, 12, -1, -1, -1 } },
    { { 3, 4, 5, 6, 5, 4, -1 } },
    { { 25, 26, 27, 28, 27, 26, -1 } },
    { { 0, 1, 2, 1, -1, -1, -1 } },
    { { 17, 18, 19, 20, 19, 18, -1 } },
    { { 21, 22, 23, 24, 23, 22, -1 } },
};
/* Where the screens' windows go, and their strings */
WindowPos STSTATUS_layout[] = {
    { 12, 55, 0, 19 },
    { 1, 16, 0, 28 },
    { 2, 16, 0, 37 },
    { 4, 61, 0, 37 },
    { 3, 16, 0, 46 },
    { 4, 61, 0, 46 },
    { 13, 44, 0, 28 },
    { 14, 59, 0, 37 },
    { 14, 94, 0, 37 },
    { 14, 59, 0, 46 },
    { 14, 94, 0, 46 },
    { 14, 152, 0, 19 },
    { 14, 20, 0, 198 },
    { 3, 265, 0, 212 },
    { 14, 300, 0, 212 },
    { 22, 189, 0, 49 },
};
/* The map's areas (STSTATUS_mapAreas), from 1 */
StatusMapSpot STSTATUS_spots[] = {
    { 0, 0, 0 },
    { 1, 60, 53 },
    { 2, 100, 59 },
    { 3, 126, 74 },
    { 4, 174, 79 },
    { 4, 208, 61 },
    { 5, 209, 36 },
    { 6, 288, 52 },
    { 7, 59, 98 },
    { 8, 116, 102 },
    { 9, 149, 104 },
    { 10, 204, 85 },
    { 10, 251, 152 },
    { 10, 253, 220 },
    { 11, 259, 99 },
    { 12, 303, 115 },
    { 13, 340, 105 },
    { 14, 57, 123 },
    { 15, 162, 138 },
    { 15, 158, 223 },
    { 16, 186, 119 },
    { 17, 251, 126 },
    { 17, 224, 127 },
    { 18, 279, 130 },
    { 19, 304, 141 },
    { 20, 330, 130 },
    { 21, 60, 169 },
    { 22, 122, 162 },
    { 22, 109, 205 },
    { 22, 89, 163 },
    { 23, 185, 143 },
    { 24, 332, 153 },
    { 24, 294, 190 },
    { 25, 199, 167 },
    { 26, 47, 199 },
    { 27, 72, 213 },
    { 28, 185, 221 },
    { 29, 265, 194 },
    { 30, 319, 234 },
    { 31, 35, 254 },
    { 32, 69, 248 },
    { 33, 191, 247 },
    { 34, 224, 227 },
    { 35, 297, 215 },
    { 35, 276, 233 },
    { 36, 296, 253 },
    { 37, 225, 253 },
};
/* Where the map's towns are, from 1 */
StatusMapPoint STSTATUS_towns[] = {
    { 0, 0 },
    { 171, 65 },
    { 154, 123 },
    { 230, 104 },
    { 182, 174 },
    { 286, 129 },
    { 285, 56 },
    { 316, 133 },
    { 262, 161 },
    { 249, 193 },
    { 253, 251 },
    { 172, 221 },
    { 160, 221 },
    { 21, 242 },
    { 25, 160 },
    { 67, 151 },
    { 30, 106 },
    { 32, 45 },
    { 114, 78 },
    { 55, 34 },
    { 128, 88 },
    { 165, 43 },
    { 198, 36 },
};
/* The towns the map shows at each stage of the game, up to 0 */
s32 STSTATUS_townList0[] = {
    1, 2, 3, 4,
    5, 7, 0,
};
s32 STSTATUS_townList1[] = {
    1, 2, 3, 4,
    5, 7, 8, 9,
    10, 11, 12, 0,
};
s32 STSTATUS_townList2[] = {
    1, 2, 3, 4,
    5, 7, 8, 9,
    10, 11, 12, 6,
    13, 14, 15, 16,
    17, 0,
};
s32 STSTATUS_townList3[] = {
    1, 2, 3, 4,
    5, 7, 8, 9,
    10, 11, 12, 6,
    13, 14, 15, 16,
    17, 18, 19, 20,
    21, 22, 0,
};
s32 STSTATUS_noTowns = 0;
/* The towns the map shows, by STSTATUS_getGameHalf and GAME.progress */
s32 *STSTATUS_townLists[][5] = {
    { STSTATUS_townList0, STSTATUS_townList1, STSTATUS_townList2, STSTATUS_townList2, STSTATUS_townList3 },
    { &STSTATUS_noTowns, &STSTATUS_noTowns, &STSTATUS_noTowns, STSTATUS_townList2, STSTATUS_townList3 },
};
StatusData STSTATUS_data = {
    STSTATUS_partnerAnims,
    STSTATUS_layout,
    STSTATUS_spots,
    STSTATUS_towns,
    { 0 },
    0,
    { 0 },
    0,
    {
        STSTATUS_loadFiles, STSTATUS_filesLoading, STSTATUS_startFade, STSTATUS_updateFade, STSTATUS_startLerp,
        STSTATUS_updateLerp, STSTATUS_getTowns, STSTATUS_listItems, STSTATUS_canEquip, STSTATUS_equip,
    },
};
/* The item kinds (WeaponData.kind) of item list 5 */
u8 STSTATUS_equipKinds[] = {
    EQUIP_KIND_RIGHT_HAND, EQUIP_KIND_LEFT_HAND,
    EQUIP_KIND_EITHER_HAND, EQUIP_KIND_BOTH_HANDS,
};
/* The map's area of each field map, by GAME.fieldMode's low byte (bit 7
   marks the late game's maps, 0x70 on; 0xFF: none) */
u8 STSTATUS_mapAreas[] = {
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x1D, 0x15, 0x20,
    0x11, 0x14, 0x14, 0x0B, 0x0B, 0x0D, 0x0D, 0x16,
    0x06, 0x17, 0x18, 0x0F, 0x1E, 0x1E, 0x0E, 0x0E,
    0x0E, 0x0E, 0x1F, 0x2A, 0x2A, 0x25, 0x25, 0x2B,
    0x0C, 0x24, 0x2C, 0x2D, 0x28, 0x12, 0x29, 0x29,
    0x29, 0x29, 0x23, 0x23, 0x23, 0x23, 0x23, 0x1B,
    0x22, 0x19, 0x1C, 0x1A, 0x10, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x22,
    0x27, 0x27, 0x26, 0x26, 0x26, 0x21, 0x21, 0x21,
    0x21, 0x09, 0x03, 0x0A, 0x04, 0x02, 0x01, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x05, 0x05,
    0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93,
    0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93,
    0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93,
    0x93, 0x93, 0x93, 0x93, 0x9D, 0x95, 0xA0, 0x91,
    0x94, 0x94, 0x8B, 0x8B, 0x8D, 0x8D, 0x96, 0x86,
    0x97, 0x98, 0x8F, 0x9E, 0x8E, 0x8E, 0x8E, 0x8E,
    0x9F, 0xAA, 0xAA, 0xA5, 0xAB, 0x8C, 0xA4, 0xAC,
    0xAD, 0xA8, 0x92, 0xA9, 0xA9, 0xA9, 0xA9, 0xA3,
    0xA3, 0x9B, 0xA2, 0x99, 0x9C, 0x9A, 0x90, 0x87,
    0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87,
    0xA2, 0xA7, 0xA7, 0xA6, 0xA6, 0xA6, 0xA1, 0xA1,
    0xA1, 0x89, 0x83, 0x8A, 0x84, 0x82, 0x81, 0x80,
    0x88, 0x88, 0x88, 0x88, 0x88, 0x85, 0x85, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
};
/* Where the game is */
StatusAreaFuncs STSTATUS_areaFuncs = {
    STSTATUS_getGameHalf,
    STSTATUS_getArea,
    STSTATUS_getVisitedAreas,
};
