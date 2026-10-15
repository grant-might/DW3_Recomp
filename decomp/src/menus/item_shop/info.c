/* The details panel of the item under the cursor: its description, and what
   equipping it would change in each partner's stats */

#include "menus/item_shop.h"

/* A partner's stats with its equipment, as main's computeStats gives them: the
   weapons' attack (STAT_STRENGTH), the armor's defense (STAT_DEFENSE) and the
   items' bonuses added, up to 999, less the penalties on STAT_STRENGTH,
   STAT_DEFENSE and STAT_SPEED */
void STITSHOP_computeStats(s32 partner, StatBlock *out) {
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

    /* PartnerStats' stats[] and status[] (the penalties), as one block */
    *out = *(StatBlock *)save->partners[partner].info.stats;
    d = &GAME.partners[partner].info;
    equip = d->equip;
    for (i = 0; i < 6; i++) {
        if (equip[i] > 0) {
            info = ITEM_FUNCS.get(equip[i]);
            type = info->type;
            data = info->data.record;
            if (IS_WEAPON_TYPE(type)) {
                out->stats[STAT_STRENGTH] += data->weapon.atk;
                if (out->stats[STAT_STRENGTH] >= 1000) {
                    out->stats[STAT_STRENGTH] = 999;
                }
                for (j = 0; j < 2; j++) {
                    stat = *(j + data->weapon.stats);
                    amount = data->weapon.amounts[j];
                    if (stat != 0) {
                        STITSHOP_addStat(out->stats, stat, (s16)amount);
                    }
                }
            } else if (IS_ARMOR_TYPE(type)) {
                out->stats[STAT_DEFENSE] += data->armor.def;
                if (out->stats[STAT_DEFENSE] >= 1000) {
                    out->stats[STAT_DEFENSE] = 999;
                }
                for (j = 0; j < 2; j++) {
                    stat = *(j + data->armor.stats);
                    amount = data->armor.amounts[j];
                    if (stat != 0) {
                        STITSHOP_addStat(out->stats, stat, (s16)amount);
                    }
                }
            } else if (IS_ACCESSORY_TYPE(type)) {
                stat = data->acc.stat;
                amount = data->acc.amount;
                if (stat != 0) {
                    STITSHOP_addStat(out->stats, stat, (s16)amount);
                }
            } else {
                continue;
            }
            out->stats[STAT_CHARISMA] += data->weapon.charisma;
            if (out->stats[STAT_CHARISMA] >= 1000) {
                out->stats[STAT_CHARISMA] = 999;
            }
        }
    }
    out->stats[STAT_STRENGTH] -= out->penalties[0];
    if (out->stats[STAT_STRENGTH] < 0) {
        out->stats[STAT_STRENGTH] = 0;
    }
    out->stats[STAT_DEFENSE] -= out->penalties[1];
    if (out->stats[STAT_DEFENSE] < 0) {
        out->stats[STAT_DEFENSE] = 0;
    }
    out->stats[STAT_SPEED] -= out->penalties[2];
    if (out->stats[STAT_SPEED] < 0) {
        out->stats[STAT_SPEED] = 0;
    }
}

/* Adds an item's bonus to a stat, up to 999: 1 to 6 one of the six battle stats,
   7 all of them, 8 to 14 a resistance */
void STITSHOP_addStat(s16 *p, s32 stat, s32 delta) {
    s32 i;
    s16 value;

    if (stat == 7) {
        for (i = 0; i < 6; i++) {
            value = p[STAT_STRENGTH + i] + delta;
            p[STAT_STRENGTH + i] = value;
            if (value >= 1000) {
                p[STAT_STRENGTH + i] = 999;
            }
        }
    } else if (stat - 1 < 6U) {
        value = p[STAT_STRENGTH - 1 + stat] + delta;
        p[STAT_STRENGTH - 1 + stat] = value;
        if (value >= 1000) {
            p[STAT_STRENGTH - 1 + stat] = 999;
        }
    } else if (stat - 8 < 7U) {
        value = p[STAT_RESISTS - 8 + stat] + delta;
        p[STAT_RESISTS - 8 + stat] = value;
        if (value >= 1000) {
            p[STAT_RESISTS - 8 + stat] = 999;
        }
    }
}

/* Shows a row's stat of a partner in the details panel: as it is, or with the
   item equipped when the row compares */
void STITSHOP_showStat(ShopInfo *info, TextWindow *win, ShopStatRow *row) {
    ShopPartnerInfo *p = &info->partners[row->partner];
    s16 value;

    if (row->compare == 0) {
        value = *(STITSHOP_stats[row->stat] + p->current.stats);
    } else {
        value = *(STITSHOP_stats[row->stat] + p->withItem.stats);
    }
    win->setNumber(win, 0, value);
    win->setRightAlign(win, 1);
}

/* Colors a row's stat: palette 1 when the item would raise it, 5 when it would
   lower it, else 6 when a penalty lowers it (STAT_STRENGTH, STAT_DEFENSE and
   STAT_SPEED), 0 otherwise */
void STITSHOP_colorStat(ShopInfo *info, TextWindow *win, ShopStatRow *row) {
    ShopPartnerInfo *p = &info->partners[row->partner];
    s32 penalty;
    s16 v[2];

    if (row->stat == 0) {
        penalty = 0;
    } else if (row->stat == 1) {
        penalty = 1;
    } else if (row->stat == 4) {
        penalty = 2;
    } else {
        penalty = -1;
    }
    v[0] = *(STITSHOP_stats[row->stat] + p->current.stats);
    if (row->compare == 0) {
        if (penalty >= 0 && p->current.penalties[penalty] != 0) {
            win->setPalette(win, PALETTE_PURPLE);
        } else {
            win->setPalette(win, PALETTE_WHITE);
        }
    } else {
        v[1] = *(STITSHOP_stats[row->stat] + p->withItem.stats);
        if (v[0] == v[1]) {
            if (penalty >= 0 && p->current.penalties[penalty] != 0) {
                win->setPalette(win, PALETTE_PURPLE);
            } else {
                win->setPalette(win, PALETTE_WHITE);
            }
        } else if (v[0] < v[1]) {
            win->setPalette(win, PALETTE_BLUE);
        } else {
            win->setPalette(win, PALETTE_RED);
        }
    }
}

/* Shows the new values of the other stats the item would change (palette 1 up,
   5 down), noting them in the partner's rows; hides the windows left over */
void STITSHOP_showOtherChanges(ShopInfo *info, TextWindow **win, ShopStatRow *row) {
    ShopPartnerInfo *p = &info->partners[row->partner];
    s32 i;
    s32 n = 0;
    s16 v[2];

    for (i = 0; i < 13; i++) {
        if (i != row->skip && (row->skip2 < 0 || i != row->skip2)) {
            v[0] = *(STITSHOP_stats[i] + p->current.stats);
            v[1] = *(STITSHOP_stats[i] + p->withItem.stats);
            if (v[0] != v[1]) {
                p->rows[n + 2] = i + 1;
                win[n]->setNumber(win[n], 0, v[1]);
                win[n]->setRightAlign(win[n], 1);
                if (v[0] < v[1]) {
                    win[n]->setPalette(win[n], PALETTE_BLUE);
                } else {
                    win[n]->setPalette(win[n], PALETTE_RED);
                }
                n++;
            }
        }
    }
    p->changes += n;
    for (i = n; i < 4; i++) {
        win[i]->setVisible(win[i], 0);
    }
}

/* Fills a partner's rows of the details panel's second page: the stats of
   the slot the item goes in, then the others it would change. The item is
   equipped to compute them and the partner's equipment put back */
void STITSHOP_fillPartnerRows(ShopInfo *info, ShopInfoWindows *win, s32 member) {
    PartnerEquip equip;
    ShopStatRow row;
    s32 partner = GAME.funcs.getPartyMember(member);
    PartnerStats *stats = GAME.funcs.getPartnerStats(partner);
    ShopPartnerInfo *p = &info->partners[member];
    ItemData *data;

    /* the partner's equipment, kept as one block while the item is tried on */
    equip = *(PartnerEquip *)stats->equip;
    STITSHOP_computeStats(partner, &p->current);
    p->slot = STITSHOP_funcs.compareEquip(partner, info->item);
    STITSHOP_funcs.equip(partner, p->slot, info->item, 0);
    STITSHOP_computeStats(partner, &p->withItem);
    *(PartnerEquip *)stats->equip = equip;
    p->changes = 2;
    switch (p->slot) {
    case 0:
    case 2:
    case 3:
    default:
        if (IS_WEAPON_TYPE(ITEM_FUNCS.get(info->item)->type)) {
            p->rows[0] = 1;
            row.partner = member;
            row.stat = 0;
            row.compare = 0;
            STITSHOP_showStat(info, win->partners[member].changes[0], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[0], &row);
            row.compare = 1;
            STITSHOP_showStat(info, win->partners[member].changes[2], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[2], &row);
            p->rows[1] = 6;
            row.partner = member;
            row.stat = 5;
            row.compare = 0;
            STITSHOP_showStat(info, win->partners[member].changes[1], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[1], &row);
            row.compare = 1;
            STITSHOP_showStat(info, win->partners[member].changes[3], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[3], &row);
            row.skip = 0;
            row.skip2 = 5;
            STITSHOP_showOtherChanges(info, &win->partners[member].changes[4], &row);
        } else {
            p->rows[0] = 2;
            row.partner = member;
            row.stat = 1;
            row.compare = 0;
            STITSHOP_showStat(info, win->partners[member].changes[0], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[0], &row);
            row.compare = 1;
            STITSHOP_showStat(info, win->partners[member].changes[2], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[2], &row);
            p->rows[1] = 6;
            row.partner = member;
            row.stat = 5;
            row.compare = 0;
            STITSHOP_showStat(info, win->partners[member].changes[1], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[1], &row);
            row.compare = 1;
            STITSHOP_showStat(info, win->partners[member].changes[3], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[3], &row);
            row.skip = 1;
            row.skip2 = 5;
            STITSHOP_showOtherChanges(info, &win->partners[member].changes[4], &row);
        }
        win->partners[member].stats[0]->setString(win->partners[member].stats[0], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
        win->partners[member].stats[1]->setString(win->partners[member].stats[1], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
        break;
    case 1:
        p->rows[0] = 2;
        row.partner = member;
        row.stat = 1;
        row.compare = 0;
        STITSHOP_showStat(info, win->partners[member].changes[0], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[0], &row);
        row.compare = 1;
        STITSHOP_showStat(info, win->partners[member].changes[2], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[2], &row);
        p->rows[1] = 6;
        row.partner = member;
        row.stat = 5;
        row.compare = 0;
        STITSHOP_showStat(info, win->partners[member].changes[1], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[1], &row);
        row.compare = 1;
        STITSHOP_showStat(info, win->partners[member].changes[3], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[3], &row);
        row.skip = 1;
        row.skip2 = 5;
        STITSHOP_showOtherChanges(info, &win->partners[member].changes[4], &row);
        win->partners[member].stats[0]->setString(win->partners[member].stats[0], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
        win->partners[member].stats[1]->setString(win->partners[member].stats[1], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
        break;
    case 4:
    case 5:
        p->rows[0] = 6;
        row.partner = member;
        row.stat = 5;
        row.compare = 0;
        STITSHOP_showStat(info, win->partners[member].changes[0], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[0], &row);
        row.compare = 1;
        STITSHOP_showStat(info, win->partners[member].changes[2], &row);
        STITSHOP_colorStat(info, win->partners[member].changes[2], &row);
        row.skip = 5;
        data = ITEM_FUNCS.get(info->item)->data.record;
        /* the row of the stat the accessory raises (7 raises them all and has
           none). The match depends on the range tests being written out: the
           inner one isn't merged with the outer one before cse */
        if ((data->acc.stat >= 1 && data->acc.stat <= 5) || (data->acc.stat >= 8 && data->acc.stat <= 14)) {
            if (data->acc.stat >= 1 && data->acc.stat <= 5) {
                p->rows[1] = data->acc.stat;
            } else {
                p->rows[1] = data->acc.stat - 1;
            }
            row.partner = member;
            row.stat = p->rows[1] - 1;
            row.compare = 0;
            STITSHOP_showStat(info, win->partners[member].changes[1], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[1], &row);
            row.compare = 1;
            STITSHOP_showStat(info, win->partners[member].changes[3], &row);
            STITSHOP_colorStat(info, win->partners[member].changes[3], &row);
            win->partners[member].stats[0]->setString(win->partners[member].stats[0], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
            win->partners[member].stats[1]->setString(win->partners[member].stats[1], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
            row.skip2 = row.stat;
        } else {
            win->partners[member].changes[1]->setVisible(win->partners[member].changes[1], 0);
            win->partners[member].changes[3]->setVisible(win->partners[member].changes[3], 0);
            row.skip2 = -1;
            p->rows[1] = 0;
            win->partners[member].stats[0]->setString(win->partners[member].stats[0], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 0xC);
            win->partners[member].stats[1]->setVisible(win->partners[member].stats[1], 0);
        }
        STITSHOP_showOtherChanges(info, &win->partners[member].changes[4], &row);
        break;
    }
}

/* The y of a line of the details panel, two lower when selling. The match
   depends on this being a function: its sum isn't folded into the offset */
static inline s32 STITSHOP_lineY(ShopInfo *info, s32 y) {
    return info->selling * 2 + y;
}

/* Creates the details panel's windows: the item's rows, then, when buying,
   each partner's name, stats and stat changes */
void STITSHOP_createInfoWindows(ShopInfo *info, ShopInfoWindows *win) {
    s32 offset = info->selling * 0x28;
    s16 left;
    s16 right;
    s32 x;
    s32 i;
    s32 j;
    s32 column;

    win->name = createTextWindow(info->layer, 1, 0x25, offset + STITSHOP_lineY(info, 0x73));
    win->equippedLabel = createTextWindow(info->layer, 1, 0xA1, offset + STITSHOP_lineY(info, 0x75));
    win->equipped = createTextWindow(info->layer, 1, 0xE1, offset + STITSHOP_lineY(info, 0x75));
    win->ownedLabel = createTextWindow(info->layer, 1, 0xEA, offset + STITSHOP_lineY(info, 0x75));
    win->owned = createTextWindow(info->layer, 1, 0x12A, offset + STITSHOP_lineY(info, 0x75));
    win->priceLabel = createTextWindow(info->layer, 3, 0x11A, offset + STITSHOP_lineY(info, 0x8B));
    win->price = createTextWindow(info->layer, 3, 0x117, offset + STITSHOP_lineY(info, 0x8B));
    win->desc = createTextWindow(info->layer, 1, 0x14, offset + 0xA1);
    win->desc->setLines(win->desc, 2);
    win->kind = createTextWindow(info->layer, 1, 0x14, offset + 0xAF);
    if (info->selling == 0) {
        /* the match depends on the column counter of its own, apart from i, and
           on x set from it in the stats loop */
        column = 0;
        for (i = 0; i < info->partyCount; i++) {
            win->partners[i].name = createTextWindow(info->layer, 1, i * 0x63 + 0x17, 0x9E);
            for (j = 0; j < 2; j++) {
                x = column * 0x63;
                win->partners[i].stats[j] = createTextWindow(info->layer, 1, i * 0x63 + 0x3C, j * 0xE + 0xAC);
            }
            left = x + 0x39;
            win->partners[i].changes[0] = createTextWindow(info->layer, 1, left, 0xAC);
            win->partners[i].changes[1] = createTextWindow(info->layer, 1, left, 0xBA);
            right = x + 0x5A;
            win->partners[i].changes[2] = createTextWindow(info->layer, 1, right, 0xAC);
            win->partners[i].changes[3] = createTextWindow(info->layer, 1, right, 0xBA);
            win->partners[i].changes[4] = createTextWindow(info->layer, 1, left, 0xC8);
            win->partners[i].changes[5] = createTextWindow(info->layer, 1, x + 0x63, 0xC8);
            win->partners[i].changes[6] = createTextWindow(info->layer, 1, left, 0xD6);
            win->partners[i].changes[7] = createTextWindow(info->layer, 1, x + 0x63, 0xD6);
            column++;
        }
        win->pageHint = createTextWindow(info->layer, 1, 0x13, 0x87);
    }
}

/* Shows the item's name, how many are equipped and in the bag, and its
   price; or hides them */
void STITSHOP_showItemRows(ShopInfo *info, ShopInfoWindows *win, s32 show) {
    if (show) {
        win->name->setString(win->name, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), info->item);
        win->equippedLabel->setString(win->equippedLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 6);
        win->equipped->setNumber(win->equipped, 0, GAME.equippedItems[info->item]);
        win->equipped->setRightAlign(win->equipped, 1);
        win->ownedLabel->setString(win->ownedLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 7);
        win->owned->setNumber(win->owned, 0, GAME.items[info->item]);
        win->owned->setRightAlign(win->owned, 1);
        win->priceLabel->setString(win->priceLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 2);
        if (info->selling == 0) {
            win->price->setNumber(win->price, 0, ITEM_FUNCS.get(info->item)->price * info->quantity);
        } else {
            win->price->setNumber(win->price, 0, ITEM_FUNCS.get(info->item)->sellPrice * info->quantity);
        }
        win->price->setRightAlign(win->price, 1);
    } else {
        win->name->setVisible(win->name, 0);
        win->equippedLabel->setVisible(win->equippedLabel, 0);
        win->equipped->setVisible(win->equipped, 0);
        win->ownedLabel->setVisible(win->ownedLabel, 0);
        win->owned->setVisible(win->owned, 0);
        win->priceLabel->setVisible(win->priceLabel, 0);
        win->price->setVisible(win->price, 0);
    }
}

/* Shows the item's description and, for a weapon, its kind; or hides them */
void STITSHOP_showItemDesc(ShopInfo *info, ShopInfoWindows *win, s32 show) {
    ItemInfo *item;
    ItemData *data;

    if (show) {
        win->desc->setString(win->desc, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_INFO)), info->item);
        item = ITEM_FUNCS.get(info->item);
        if (item->type >= 2 && item->type < 14) {
            data = item->data.record;
            win->kind->setString(win->kind, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), STITSHOP_kindStrings[data->weapon.kind]);
            return;
        }
    } else {
        win->desc->setVisible(win->desc, 0);
    }
    win->kind->setVisible(win->kind, 0);
}

/* Shows the party's names and, for those who can equip the item, the stats
   it changes; or hides them */
void STITSHOP_showPartnerStats(ShopInfo *info, ShopInfoWindows *win, s32 show) {
    s32 partner;
    s32 i;
    s32 j;

    if (show) {
        for (i = 0; i < info->partyCount; i++) {
            partner = GAME.funcs.getPartyMember(i);
            win->partners[i].name->setString(win->partners[i].name, GAME.funcs.getPartnerStats(partner)->name, -1);
            if (STITSHOP_funcs.canEquip(partner, info->item)) {
                STITSHOP_fillPartnerRows(info, win, i);
                win->partners[i].name->setPalette(win->partners[i].name, PALETTE_WHITE);
            } else {
                win->partners[i].name->setPalette(win->partners[i].name, PALETTE_GREY);
                for (j = 0; j < 2; j++) {
                    win->partners[i].stats[j]->setVisible(win->partners[i].stats[j], 0);
                }
                for (j = 0; j < 8; j++) {
                    win->partners[i].changes[j]->setVisible(win->partners[i].changes[j], 0);
                }
            }
        }
    } else {
        for (i = 0; i < info->partyCount; i++) {
            win->partners[i].name->setVisible(win->partners[i].name, 0);
            for (j = 0; j < 2; j++) {
                win->partners[i].stats[j]->setVisible(win->partners[i].stats[j], 0);
            }
            for (j = 0; j < 8; j++) {
                win->partners[i].changes[j]->setVisible(win->partners[i].changes[j], 0);
            }
        }
    }
}

/* Draws the second page's frames: the panel, then for each partner its frame
   and either the icons of the stats the item changes or a cross */
void STITSHOP_drawStatsPage(ShopInfo *info, ShopInfoWindows *win) {
    SpriteDrawer sprite;
    s32 i;
    s32 j;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(info->layer, info->depth);
    sprite.setTexture(0x280, 0x100);
    if (info->panels[1].level != 0) {
        if (info->panels[1].level != ONE) {
            sprite.setScale(info->panels[1].level, ONE, ONE);
            sprite.setPivot(0, 0x8C);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 9, 0, 0x81);
    }
    if (info->panels[3].level != 0) {
        sprite.setScale(ONE, info->panels[3].level, ONE);
        for (i = 0; i < info->partyCount; i++) {
            if (info->panels[3].level != ONE) {
                sprite.setPivot(i * 0x64 + 0x3C, 0xC0);
            } else if (STITSHOP_funcs.canEquip(GAME.funcs.getPartyMember(i), info->item)) {
                sprite.setTexture(0x140, 0);
                for (j = 0; j < info->partners[i].changes; j++) {
                    if (info->partners[i].rows[j] > 0) {
                        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), info->partners[i].rows[j] + 0x24,
                                    STITSHOP_changePos[j].x + i * 0x63, STITSHOP_changePos[j].y);
                    }
                }
            } else {
                sprite.setTexture(0x280, 0x100);
                sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x16, i * 0x63 + 0x19, 0xAD);
            }
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 8, i * 0x63 + 0xB, 0x9C);
        }
    }
}

/* Once the item's panel (and the page hint's, for equipment) has opened: shows
   the item's rows and the hint to turn the page, and opens the description's
   panel */
static inline void STITSHOP_openBuyInfo(ShopInfo *info, ShopInfoWindows *win) {
    if (info->hasStats) {
        STITSHOP_funcs.updateFade(&info->panels[1]);
    }
    if (STITSHOP_funcs.updateFade(&info->panels[0])) {
        STITSHOP_showItemRows(info, win, 1);
        if (info->hasStats) {
            win->pageHint->setString(win->pageHint, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), info->page + 10);
        }
        STITSHOP_funcs.startFade(&info->panels[2], 1);
        info->substate++;
    }
}

/* While the buying dialog asks to equip the item (info->nextSubstate): hides
   the page hint and closes its panel, and the description when it is the page
   shown */
static inline void STITSHOP_hideBuyInfoPage(ShopInfo *info, ShopInfoWindows *win) {
    info->shown = 0;
    win->pageHint->setVisible(win->pageHint, 0);
    STITSHOP_funcs.startFade(&info->panels[1], 0);
    if (info->page == 0) {
        STITSHOP_funcs.startFade(&info->panels[2], 0);
        STITSHOP_showItemDesc(info, win, 0);
    }
    info->substate++;
}

/* Turning the page (info->turnPage): updates the page hint and closes the page
   that was shown */
static inline void STITSHOP_startInfoPageTurn(ShopInfo *info, ShopInfoWindows *win) {
    win->pageHint->setString(win->pageHint, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), info->page + 10);
    if (info->page == 0) {
        STITSHOP_funcs.startFade(&info->panels[3], 0);
        STITSHOP_showPartnerStats(info, win, 0);
        info->substate = 15;
    } else {
        STITSHOP_funcs.startFade(&info->panels[2], 0);
        STITSHOP_showItemDesc(info, win, 0);
        info->substate = 11;
    }
}

/* Closing (info->close): hides the item's rows, the page hint and the page
   shown, and closes their panels */
static inline void STITSHOP_closeBuyInfo(ShopInfo *info, ShopInfoWindows *win) {
    STITSHOP_showItemRows(info, win, 0);
    win->pageHint->setVisible(win->pageHint, 0);
    STITSHOP_funcs.startFade(&info->panels[0], 0);
    if (info->hasStats) {
        STITSHOP_funcs.startFade(&info->panels[1], 0);
    }
    if (info->page == 0) {
        STITSHOP_showItemDesc(info, win, 0);
        STITSHOP_funcs.startFade(&info->panels[2], 0);
    } else {
        STITSHOP_showPartnerStats(info, win, 0);
        STITSHOP_funcs.startFade(&info->panels[3], 0);
    }
    info->substate++;
}

/* Ends the details panel once its panels have closed */
static inline void STITSHOP_endBuyInfo(ShopInfo *info, ShopInfoWindows *win) {
    if (info->page == 0) {
        STITSHOP_funcs.updateFade(&info->panels[2]);
    } else {
        STITSHOP_funcs.updateFade(&info->panels[3]);
    }
    if (info->hasStats) {
        STITSHOP_funcs.updateFade(&info->panels[1]);
    }
    if (STITSHOP_funcs.updateFade(&info->panels[0])) {
        info->state = TASK_KILL;
    }
}

/* The details panel's states when buying: fades in its first page, turns
   between the pages (10-16), and fades out when closed (50) */
void STITSHOP_runBuyInfo(ShopInfo *info, ShopInfoWindows *win) {
    switch (info->substate) {
    case 0:
    default:
        STITSHOP_funcs.startFade(&info->panels[0], 1);
        if (info->hasStats) {
            STITSHOP_funcs.startFade(&info->panels[1], 1);
        }
        info->substate++;
        break;
    case 1:
        STITSHOP_openBuyInfo(info, win);
        break;
    case 2:
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            STITSHOP_showItemDesc(info, win, 1);
            info->substate++;
        }
        break;
    case 3:
        break;
    case 4:
        STITSHOP_hideBuyInfoPage(info, win);
        break;
    case 5:
        STITSHOP_funcs.updateFade(&info->panels[2]);
        if (STITSHOP_funcs.updateFade(&info->panels[1])) {
            if (info->page == 0) {
                STITSHOP_funcs.startFade(&info->panels[3], 1);
            }
            info->substate++;
        }
        break;
    case 6:
        if (STITSHOP_funcs.updateFade(&info->panels[3])) {
            STITSHOP_showPartnerStats(info, win, 1);
            info->substate++;
        }
        break;
    case 7:
        break;
    case 8:
        if (info->page == 0) {
            STITSHOP_showPartnerStats(info, win, 0);
            STITSHOP_funcs.startFade(&info->panels[3], 0);
        }
        STITSHOP_funcs.startFade(&info->panels[1], 1);
        info->substate++;
        break;
    case 9:
        STITSHOP_funcs.updateFade(&info->panels[3]);
        if (STITSHOP_funcs.updateFade(&info->panels[1])) {
            win->pageHint->setVisible(win->pageHint, 1);
            if (info->page == 0) {
                STITSHOP_funcs.startFade(&info->panels[2], 1);
            }
            info->substate = 1000;
        }
        break;
    case 1000:
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            if (info->page == 0) {
                STITSHOP_showItemDesc(info, win, 1);
            }
            info->substate = 3;
            info->shown = 1;
        }
        break;
    case 10:
        STITSHOP_startInfoPageTurn(info, win);
        break;
    case 11:
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            STITSHOP_funcs.startFade(&info->panels[3], 1);
            info->substate++;
        }
        break;
    case 12:
        if (STITSHOP_funcs.updateFade(&info->panels[3])) {
            STITSHOP_showPartnerStats(info, win, 1);
            info->substate = 3;
            info->shown = 1;
        }
        break;
    case 15:
        if (STITSHOP_funcs.updateFade(&info->panels[3])) {
            STITSHOP_funcs.startFade(&info->panels[2], 1);
            info->substate++;
        }
        break;
    case 16:
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            STITSHOP_showItemDesc(info, win, 1);
            info->substate = 3;
            info->shown = 1;
        }
        break;
    case 50:
        STITSHOP_closeBuyInfo(info, win);
        break;
    case 51:
        STITSHOP_endBuyInfo(info, win);
        break;
    }
}

/* The details panel's first page: fades in, shows the item, then fades out
   when closed */
void STITSHOP_runSellInfo(ShopInfo *info, ShopInfoWindows *win) {
    switch (info->substate) {
    case 0:
    default:
        STITSHOP_funcs.startFade(&info->panels[0], 1);
        info->substate++;
        break;
    case 1:
        if (STITSHOP_funcs.updateFade(&info->panels[0])) {
            STITSHOP_showItemRows(info, win, 1);
            STITSHOP_funcs.startFade(&info->panels[2], 1);
            info->substate++;
        }
        break;
    case 2:
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            STITSHOP_showItemDesc(info, win, 1);
            info->substate++;
        }
        break;
    case 3:
        break;
    case 50:
        STITSHOP_showItemRows(info, win, 0);
        STITSHOP_showItemDesc(info, win, 0);
        STITSHOP_funcs.startFade(&info->panels[0], 0);
        STITSHOP_funcs.startFade(&info->panels[2], 0);
        info->substate++;
        break;
    case 51:
        STITSHOP_funcs.updateFade(&info->panels[0]);
        if (STITSHOP_funcs.updateFade(&info->panels[2])) {
            info->state = TASK_KILL;
        }
        break;
    }
}

/* The details panel: counts the party, then runs the pages and draws the
   item's icon and frames */
void STITSHOP_updateInfo(ShopInfo *info, ShopInfoWindows *win) {
    SpriteDrawer sprite;
    s32 offset;
    s32 i;

    switch (info->state) {
    case TASK_INIT:
    default:
        info->nextState(info);
        for (i = 0; i < 3; i++) {
            if (GAME.funcs.getPartyMember(i) >= 0) {
                info->partyCount++;
            }
        }
        STITSHOP_createInfoWindows(info, win);
        info->panels[0].duration = 10;
        info->panels[1].duration = 10;
        info->panels[2].duration = 10;
        info->panels[3].duration = 10;
        if (info->selling == 0 &&
            (ITEM_FUNCS.isKind(info->item, ITEM_KIND_WEAPON) || ITEM_FUNCS.isKind(info->item, ITEM_KIND_ARMOR) ||
             ITEM_FUNCS.isKind(info->item, ITEM_KIND_ACCESSORY))) {
            info->hasStats = 1;
        }
        break;
    case TASK_RUN:
        if (info->selling == 0) {
            STITSHOP_runBuyInfo(info, win);
            STITSHOP_drawStatsPage(info, win);
        } else {
            STITSHOP_runSellInfo(info, win);
        }
        offset = info->selling * 0x28;
        initSpriteDrawer(&sprite);
        sprite.setLayerId(info->layer, info->depth - 1);
        sprite.setTexture(0x280, 0x100);
        if (info->panels[0].level != 0) {
            if (info->panels[0].level != ONE) {
                sprite.setScale(info->panels[0].level, ONE, ONE);
                sprite.setPivot(0x140, offset + 0x81);
            } else {
                sprite.setTexture(0x140, 0);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(info->item), 0x16,
                            offset + STITSHOP_lineY(info, 0x73));
            }
            sprite.setTexture(0x280, 0x100);
            sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0xE, 0xF, offset + STITSHOP_lineY(info, 0x6C));
        }
        if (info->panels[2].level != 0) {
            if (info->panels[2].level != ONE) {
                sprite.setScale(ONE, info->panels[2].level, ONE);
                sprite.setPivot(0xA0, offset + 0xAE);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 3, 0, offset + 0x9C);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Shows an item and the quantity (for the price) in the details panel
   (info->showItem); the windows are only refreshed while the page is shown */
void STITSHOP_setInfoItem(ShopInfo *info, s32 item, s32 quantity) {
    ShopInfoWindows *win = info->children;

    info->item = item;
    info->quantity = quantity;
    if (info->shown != 0) {
        STITSHOP_showItemRows(info, win, 1);
        if (info->page == 0) {
            STITSHOP_showItemDesc(info, win, 1);
        } else {
            STITSHOP_showItemDesc(info, win, 0);
            STITSHOP_showPartnerStats(info, win, 1);
        }
    }
}

/* Closes the details panel (info->close), when it is open and idle */
void STITSHOP_closeInfo(ShopInfo *info) {
    if (info->substate == 3) {
        info->substate = 50;
    }
}

/* Shows or hides the weapon's kind in the details panel (info->setKindVisible) */
void STITSHOP_setKindVisible(ShopInfo *info, s32 visible) {
    ShopInfoWindows *win = info->children;

    win->kind->setVisible(win->kind, visible);
}

/* Turns the details panel between the description and the partners' stats
   (info->turnPage); only for the items of kinds 3 to 5, which have stats */
void STITSHOP_turnInfoPage(ShopInfo *info) {
    if (ITEM_FUNCS.isKind(info->item, ITEM_KIND_WEAPON) != 0 || ITEM_FUNCS.isKind(info->item, ITEM_KIND_ARMOR) != 0 ||
        ITEM_FUNCS.isKind(info->item, ITEM_KIND_ACCESSORY) != 0) {
        SOUND.playSound(SOUND_MENU_MOVE);
        info->substate = 10;
        info->shown = 0;
        info->page = 1 - info->page;
    }
}

/* Refreshes the details panel once the item is equipped on a party member
   (info->refreshPartner): the item's rows and that partner's */
void STITSHOP_refreshPartner(ShopInfo *info, s32 member) {
    ShopInfoWindows *win = info->children;

    STITSHOP_showItemRows(info, win, 1);
    STITSHOP_fillPartnerRows(info, win, member);
}

/* Creates the details panel (task) of the item selected, when buying or
   selling */
ShopInfo *STITSHOP_createInfo(s32 selling, s32 item) {
    ShopInfo *info = createTask(STITSHOP_updateInfo, sizeof(ShopInfo), 0xAC);

    info->showItem = STITSHOP_setInfoItem;
    info->close = STITSHOP_closeInfo;
    info->setKindVisible = STITSHOP_setKindVisible;
    info->turnPage = STITSHOP_turnInfoPage;
    info->refreshPartner = STITSHOP_refreshPartner;
    info->layer = SCREEN_LAYER;
    info->depth = 6;
    info->selling = selling;
    info->item = item;
    info->quantity = 1;
    info->shown = 1;
    return info;
}
