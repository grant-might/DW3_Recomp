/* The fifth object of STSTATUS.PRO (see card_screen.c), the fifth screen
   (STSTATUS_createStatusScreen): its rodata starts at 0x800828B0 (USA). */

#include "menus/field_menu.h"

/* Creates the windows of the fifth screen */
void STSTATUS_createStatusWindows(StatsScreen *screen, StatsScreenWindows *windows) {
    WindowPos *pos;
    s32 i;
    s32 j;

    pos = &STSTATUS_data.layout[11];
    windows->title = createTextWindow(screen->layer, 1, pos->x, pos->y);
    for (j = 0; j < 3; j++) {
        pos = &STSTATUS_data.layout[0];
        windows->pages[j].name = createTextWindow(screen->layer, 1, pos->x, pos->y + j * 46);
        pos = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].labels[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
        pos = &STSTATUS_data.layout[6];
        for (i = 0; i < 5; i++, pos++) {
            windows->pages[j].values[i] = createTextWindow(screen->layer, 3, pos->x, pos->y + j * 46);
        }
    }
    pos = &STSTATUS_data.layout[12];
    windows->help = createTextWindow(screen->layer, 1, pos->x, pos->y);
    windows->help->setLines(windows->help, 2);
    windows->help2 = createTextWindow(screen->layer, 1, pos->x, pos->y + 14);
    for (i = 0; i < 2; i++) {
        windows->options[i] = createTextWindow(screen->layer, 1, 0xAC, 0x13 + i * 14);
    }
    windows->cursor = createCursor(screen->layer, screen->depth - 1, 0x98, screen->option * 14 + 0x13);
    windows->cursor->setVisible(windows->cursor, 0);
    windows->exp = createTextWindow(screen->layer, 3, 0x42, 0x45);
    windows->expLabel = createTextWindow(screen->layer, 3, 0x46, 0x45);
    windows->slotTitle = createTextWindow(screen->layer, 1, 0xA7, 0x37);
    for (i = 0; i < 3; i++) {
        windows->slots[i] = createTextWindow(screen->layer, 1, 0xB2, 0x47 + i * 14);
    }
    windows->equipTitle = createTextWindow(screen->layer, 1, 0xA7, 0x79);
    for (i = 0; i < 6; i++) {
        windows->equip[i] = createTextWindow(screen->layer, 1, 0xC0, 0x89 + i * 14);
    }
    for (i = 0; i < 6; i++) {
        windows->values[i] = createTextWindow(screen->layer, 1, 0x32, 0x5B + i * 14);
    }
    for (i = 0; i < 7; i++) {
        windows->values[6 + i] = createTextWindow(screen->layer, 1, 0x5B, 0x5B + i * 14);
    }
    windows->tpLabel = createTextWindow(screen->layer, 1, 0x10, 0xCB);
    windows->tpLabel->setDepth(windows->tpLabel, screen->depth - 1);
    windows->tp = createTextWindow(screen->layer, 1, 0x2D, 0xCB);
    windows->tp->setDepth(windows->tp, screen->depth - 1);
}

/* As STSTATUS_showCardPage */
void STSTATUS_showStatusPage(StatsScreen *screen, StatsScreenWindows *windows, s32 member, s32 show) {
    PartnerTotals stats;
    WindowPos *layout;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(member);
        GAME.funcs.computeStats(id, &stats);
        windows->pages[member].name->setString(windows->pages[member].name, GAME.funcs.getPartnerStats(id), -1);
        layout = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, layout++) {
            windows->pages[member].labels[i]->setString(windows->pages[member].labels[i],
                                                        FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), layout->string);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats4[i]]);
            windows->pages[member].values[i]->setRightAlign(windows->pages[member].values[i], 1);
        }
    } else {
        windows->pages[member].name->setVisible(windows->pages[member].name, 0);
        for (i = 0; i < 5; i++) {
            windows->pages[member].labels[i]->setVisible(windows->pages[member].labels[i], 0);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[member].values[i]->setVisible(windows->pages[member].values[i], 0);
        }
    }
}

/* As STSTATUS_showStatusPage, on the first page */
void STSTATUS_showChosenPage(StatsScreen *screen, StatsScreenWindows *windows, s32 member, s32 show) {
    PartnerTotals stats;
    WindowPos *layout;
    PartnerStats *partner;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(member);
        partner = GAME.funcs.getPartnerStats(id);
        GAME.funcs.computeStats(id, &stats);
        windows->pages[0].name->setString(windows->pages[0].name, partner, -1);
        layout = &STSTATUS_data.layout[1];
        for (i = 0; i < 5; i++, layout++) {
            windows->pages[0].labels[i]->setString(windows->pages[0].labels[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)),
                                                   layout->string);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[0].values[i]->setNumber(windows->pages[0].values[i], 0, stats.stats[STSTATUS_pageStats4[i]]);
            windows->pages[0].values[i]->setRightAlign(windows->pages[0].values[i], 1);
        }
    } else {
        windows->pages[0].name->setVisible(windows->pages[0].name, 0);
        for (i = 0; i < 5; i++) {
            windows->pages[0].labels[i]->setVisible(windows->pages[0].labels[i], 0);
        }
        for (i = 0; i < 5; i++) {
            windows->pages[0].values[i]->setVisible(windows->pages[0].values[i], 0);
        }
    }
}

/* Shows or hides the fifth screen's two options: the digivolution panel and the
   equipment panel */
void STSTATUS_showStatusChoices(StatsScreen *screen, StatsScreenWindows *windows, s32 show) {
    s32 i;

    if (show != 0) {
        for (i = 0; i < 2; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), i + 0x2D);
        }
    } else {
        for (i = 0; i < 2; i++) {
            windows->options[i]->setVisible(windows->options[i], 0);
        }
    }
}

/* The partner's slots: the entries in it (getPartnerSlots), the one it starts
   battles as (battleDigivolve) in palette 1, or hides them */
void STSTATUS_showPartnerSlots(StatsScreen *screen, StatsScreenWindows *windows, s32 show) {
    s16 slots[4];
    Partner *partner;
    DigimonData *entry;
    s32 id;
    s32 i;

    if (show) {
        id = GAME.funcs.getPartyMember(screen->member);
        GAME.funcs.getPartnerSlots(id, slots);
        partner = &GAME.partners[id];
        windows->slotTitle->setString(windows->slotTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x34);
        for (i = 0; i < 3; i++) {
            if (slots[i] >= 4) {
                entry = GET_DIGIMON(slots[i]);
                windows->slots[i]->setString(windows->slots[i], FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), entry->nameId);
                if (partner->battleDigivolve == slots[i]) {
                    windows->slots[i]->setPalette(windows->slots[i], PALETTE_BLUE);
                } else {
                    windows->slots[i]->setPalette(windows->slots[i], PALETTE_WHITE);
                }
            } else {
                windows->slots[i]->setVisible(windows->slots[i], 0);
            }
        }
    } else {
        windows->slotTitle->setVisible(windows->slotTitle, 0);
        for (i = 0; i < 3; i++) {
            windows->slots[i]->setVisible(windows->slots[i], 0);
        }
    }
}

/* The party member's equipment, or the slots' names where it has none */
void STSTATUS_showPartnerEquipment(StatsScreen *screen, StatsScreenWindows *windows, s32 show) {
    PartnerStats *stats;
    s16 item;
    s32 i;

    if (show) {
        stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(screen->member));
        windows->equipTitle->setString(windows->equipTitle, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x20);
        for (i = 0; i < 6; i++) {
            item = stats->equip[i];
            if (item != 0) {
                windows->equip[i]->setString(windows->equip[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), item);
            } else {
                windows->equip[i]->setString(windows->equip[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_equipStrings[i]);
            }
        }
    } else {
        windows->equipTitle->setVisible(windows->equipTitle, 0);
        for (i = 0; i < 6; i++) {
            windows->equip[i]->setVisible(windows->equip[i], 0);
        }
    }
}

/* Shows the party member's stats, in another palette the ones that are raised */
void STSTATUS_showPartnerStats(StatsScreen *screen, StatsScreenWindows *windows, s32 show) {
    PartnerTotals totals;
    s32 value;
    s32 i;

    if (show) {
        GAME.funcs.computeStats(GAME.funcs.getPartyMember(screen->member), &totals);
        for (i = 0; i < 6; i++) {
            value = totals.stats[STSTATUS_equipStats[i]];
            if (value >= 1000) {
                value = 999;
            }
            windows->values[i]->setNumber(windows->values[i], 0, value);
        }
        for (i = 0; i < 7; i++) {
            value = totals.stats[STSTATUS_equipStats[i + 6]];
            if (value >= 1000) {
                value = 999;
            }
            windows->values[i + 6]->setNumber(windows->values[i + 6], 0, value);
        }
        for (i = 0; i < 13; i++) {
            windows->values[i]->setRightAlign(windows->values[i], 1);
            windows->values[i]->setPalette(windows->values[i], PALETTE_WHITE);
        }
        windows->tpLabel->setString(windows->tpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x4E);
        windows->tp->setNumber(windows->tp, 0, totals.fields.tp);
        windows->tp->setRightAlign(windows->tp, 1);
        if (totals.fields.lowered[0]) {
            windows->values[0]->setPalette(windows->values[0], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[1]) {
            windows->values[1]->setPalette(windows->values[1], PALETTE_PURPLE);
        }
        if (totals.fields.lowered[2]) {
            windows->values[4]->setPalette(windows->values[4], PALETTE_PURPLE);
        }
    } else {
        for (i = 0; i < 13; i++) {
            windows->values[i]->setVisible(windows->values[i], 0);
        }
        windows->tpLabel->setVisible(windows->tpLabel, 0);
        windows->tp->setVisible(windows->tp, 0);
    }
}

/* Shows the party member's stats as they would be with the item in the
   slot, in another palette the ones it changes (slot -1: as they are) */
void STSTATUS_previewStats(StatsScreen *screen, s32 slot, s32 item) {
    StatsScreenWindows *windows = screen->children;
    PartnerTotals now;
    PartnerTotals then;
    PartnerEquip saved;
    PartnerStats *stats;
    s16 *equip;
    s16 *hand;
    ItemData *data;
    s32 id;
    s32 group;
    s32 i;
    s32 j;
    s32 before;
    s32 after;

    if (slot == -1) {
        STSTATUS_showPartnerStats(screen, windows, 1);
        return;
    }
    id = GAME.funcs.getPartyMember(screen->member);
    stats = GAME.funcs.getPartnerStats(id);
    GAME.funcs.computeStats(id, &now);
    /* the six slots are saved and restored as one struct copy;
       PartnerStats.equip is a plain array */
    saved = *(PartnerEquip *)stats->equip;
    if (*(stats->equip + slot) != 0) {
        data = ITEM_FUNCS.get(*(stats->equip + slot))->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            stats->equip[2] = 0;
            stats->equip[3] = 0;
        } else {
            *(stats->equip + slot) = 0;
        }
    }
    if (item > 0) {
        data = ITEM_FUNCS.get(item)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            hand = &stats->equip[2];
            if (*hand == 0) {
                hand = NULL;
                if (stats->equip[3] != 0) {
                    hand = &stats->equip[3];
                }
            }
            if (hand != NULL) {
                *hand = 0;
            }
        } else if (data->weapon.kind == EQUIP_KIND_GROUP_ACCESSORY) {
            group = data->weapon.group;
            for (j = 0; j < 2; j++) {
                equip = &stats->equip[j + 4];
                if (*equip != 0) {
                    data = ITEM_FUNCS.get(*equip)->data.record;
                    if (data->weapon.group == group) {
                        *equip = 0;
                    }
                }
            }
        }
        data = ITEM_FUNCS.get(item)->data.record;
        if (data->weapon.kind == EQUIP_KIND_BOTH_HANDS) {
            stats->equip[2] = item;
            stats->equip[3] = item;
        } else {
            *(stats->equip + slot) = item;
        }
    }
    GAME.funcs.computeStats(id, &then);
    *(PartnerEquip *)stats->equip = saved;
    for (i = 0; i < 6; i++) {
        /* sums and not then.stats[...]: the match depends on them, which put
           the index first in the addu */
        after = *(then.stats + STSTATUS_equipStats[i]);
        before = *(now.stats + STSTATUS_equipStats[i]);
        if (after >= 1000) {
            windows->values[i]->setNumber(windows->values[i], 0, 999);
        } else {
            windows->values[i]->setNumber(windows->values[i], 0, after);
        }
        if (before < after) {
            windows->values[i]->setPalette(windows->values[i], PALETTE_BLUE);
        } else if (after < before) {
            windows->values[i]->setPalette(windows->values[i], PALETTE_RED);
        } else {
            windows->values[i]->setPalette(windows->values[i], PALETTE_WHITE);
            if (i == 0) {
                if (now.fields.lowered[0]) {
                    windows->values[0]->setPalette(windows->values[0], PALETTE_PURPLE);
                }
            } else if (i == 1) {
                if (now.fields.lowered[1]) {
                    windows->values[1]->setPalette(windows->values[1], PALETTE_PURPLE);
                }
            } else if (i == 4 && now.fields.lowered[2]) {
                windows->values[4]->setPalette(windows->values[4], PALETTE_PURPLE);
            }
        }
    }
    for (i = 0; i < 7; i++) {
        after = *(then.stats + STSTATUS_equipStats[i + 6]);
        before = *(now.stats + STSTATUS_equipStats[i + 6]);
        if (after >= 1000) {
            windows->values[i + 6]->setNumber(windows->values[i + 6], 0, 999);
        } else {
            windows->values[i + 6]->setNumber(windows->values[i + 6], 0, after);
        }
        if (before < after) {
            windows->values[i + 6]->setPalette(windows->values[i + 6], PALETTE_BLUE);
        } else if (after < before) {
            windows->values[i + 6]->setPalette(windows->values[i + 6], PALETTE_RED);
        } else {
            windows->values[i + 6]->setPalette(windows->values[i + 6], PALETTE_WHITE);
        }
    }
    for (i = 0; i < 13; i++) {
        windows->values[i]->setRightAlign(windows->values[i], 1);
    }
}

/* Moves the member cursor's palette on every 9 frames */
static inline void STSTATUS_stepStatusCursor(StatsScreen *screen) {
    if (GFX.funcs.getTime() - screen->cursorTime >= 9) {
        screen->cursorTime = GFX.funcs.getTime();
        screen->cursorFrame++;
        if (screen->cursorFrame >= 8) {
            screen->cursorFrame = 0;
        }
    }
}

/* Draws the partners' portraits and frames, the chosen one's, its equipment
   and the panels' frames */
void STSTATUS_drawStatusScreen(StatsScreen *screen) {
    SpriteDrawer sprite;
    PartnerStats *stats;
    s32 item;
    s32 id;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    if (GFX.funcs.getTime() - screen->frameTime >= 13) {
        screen->frameTime = GFX.funcs.getTime();
        for (i = 0; i < screen->count; i++) {
            id = GAME.funcs.getPartyMember(i);
            screen->frames[i]++;
            if (STSTATUS_data.partnerAnims[id].frames[screen->frames[i]] == -1 ||
                screen->frames[i] >= 7) {
                screen->frames[i] = 0;
            }
        }
    }
    sprite.setTexture(0x280, 0x100);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            id = GAME.funcs.getPartyMember(i);
            sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16),
                        STSTATUS_data.partnerAnims[id].frames[screen->frames[i]], 0x6B,
                        i * 0x2E + 0x13);
        }
    }
    sprite.setTexture(0x140, 0);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            /* both branches draw the frame's last part: the match depends on it */
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.setScale(screen->pageFades[i].level, ONE, ONE);
                sprite.setPivot(0, i * 0x2E + 0x25);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            } else {
                sprite.setScale(ONE, ONE, ONE);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, i * 0x2E + 0x11);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, i * 0x2E + 0x13);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, i * 0x2E + 0x11);
            }
        }
    }
    if (screen->panelFades[0].level != 0) {
        sprite.setTexture(0x280, 0x100);
        if (screen->panelFades[0].level != ONE) {
            sprite.setScale(screen->panelFades[0].level, screen->panelFades[0].level, ONE);
            sprite.setPivot(0x7C, 0x27);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        id = GAME.funcs.getPartyMember(screen->member);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16),
                    STSTATUS_data.partnerAnims[id].frames[screen->frames[screen->member]], 0x6B, 0x13);
        sprite.setTexture(0x140, 0);
        if (screen->panelFades[0].level != ONE) {
            sprite.setScale(screen->panelFades[0].level, ONE, ONE);
            sprite.setPivot(0, 0x25);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, 0x11);
            sprite.setScale(screen->panelFades[0].level, screen->panelFades[0].level, ONE);
            sprite.setPivot(0x7C, 0x27);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, 0x13);
            sprite.setScale(screen->panelFades[0].level, ONE, ONE);
            sprite.setPivot(0, 0x25);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, 0x11);
        } else {
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x15, 0, 0x11);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x16, 0x67, 0x13);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x17, 0, 0x11);
        }
    }
    if (screen->fades[0].level != 0) {
        if (screen->fades[0].level != ONE) {
            sprite.setScale(screen->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x19);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
    if (screen->panelFades[5].level != 0) {
        if (screen->panelFades[5].level != ONE) {
            sprite.setScale(screen->panelFades[5].level, ONE, ONE);
            sprite.setPivot(0, 0x47);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x1A, 0, 0x3D);
    }
    sprite.setTexture(0x280, 0x100);
    if (screen->fades[1].level != 0) {
        if (screen->fades[1].level != ONE) {
            sprite.setScale(screen->fades[1].level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    if (screen->panelFades[2].level != 0) {
        if (screen->panelFades[2].level != ONE) {
            sprite.setScale(screen->panelFades[2].level, ONE, ONE);
            sprite.setPivot(0, 0x88);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x140, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xF, 0, 0x57);
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x21, 0, 0x54);
        if (screen->panelFades[2].level != ONE) {
            sprite.setPivot(0, 0xD0);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x3C, 0, 0xC4);
    }
    if (screen->panelFades[3].level != 0) {
        if (screen->panelFades[3].level != ONE) {
            sprite.setScale(screen->panelFades[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x55);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x22, 0xA0, 0x35);
    }
    if (screen->panelFades[4].level != 0) {
        if (screen->panelFades[4].level != ONE) {
            sprite.setScale(screen->panelFades[4].level, ONE, ONE);
            sprite.setPivot(0x140, 0xAC);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(screen->member));
            for (i = 0; i < 6; i++) {
                item = stats->equip[i];
                if (item > 0) {
                    sprite.setTexture(0x140, 0);
                    sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(item), 0xB2,
                                i * 14 + 0x89);
                }
            }
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x23, 0xA0, 0x77);
    }
    if (screen->panelFades[1].level != 0) {
        if (screen->panelFades[1].level != ONE) {
            sprite.setScale(screen->panelFades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x20);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1F, 0x14, 0xD);
    }
    if (screen->cursorShown) {
        STSTATUS_stepStatusCursor(screen);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(screen->layer, screen->depth - 1);
        sprite.setTexture(0x280, 0x100);
        sprite.setClutRow(screen->cursorFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->member * 0x2E + 0x11);
    }
}

/* Runs the fifth screen's question: the panels come in, the cursor picks
   one of the two options and the panels go out */
void STSTATUS_runStatusChoice(StatsScreen *screen, StatsScreenWindows *windows) {
    s32 option;
    s16 unused[4]; /* unused, but it is in the original stack frame */

    switch (screen->step) {
    case 0:
    default:
        if (screen->counter == 0) {
            STSTATUS_data.funcs.startFade(&screen->panelFades[0], 1);
        }
        STSTATUS_data.funcs.startFade(&screen->panelFades[1], 1);
        screen->step++;
        break;
    case 1:
        if (screen->counter == 0) {
            STSTATUS_data.funcs.updateFade(&screen->panelFades[0]);
        }
        if (STSTATUS_data.funcs.updateFade(&screen->panelFades[1])) {
            if (screen->panelFades[5].level == 0) {
                STSTATUS_data.funcs.startFade(&screen->panelFades[5], 1);
            }
            STSTATUS_data.funcs.startFade(&screen->panelFades[3], 1);
            STSTATUS_showChosenPage(screen, windows, screen->member, 1);
            STSTATUS_showStatusChoices(screen, windows, 1);
            screen->step++;
        }
        break;
    case 2:
        STSTATUS_data.funcs.updateFade(&screen->panelFades[5]);
        if (STSTATUS_data.funcs.updateFade(&screen->panelFades[3])) {
            if (screen->panelFades[2].level == 0) {
                STSTATUS_data.funcs.startFade(&screen->panelFades[2], 1);
            }
            STSTATUS_data.funcs.startFade(&screen->panelFades[4], 1);
            STSTATUS_showPartnerSlots(screen, windows, 1);
            windows->exp->setNumber(windows->exp, 0,
                                      GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(screen->member))->exp);
            windows->exp->setRightAlign(windows->exp, 1);
            windows->expLabel->setString(windows->expLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x35);
            windows->cursor->setPos(windows->cursor, 0x98, screen->option * 14 + 0x13);
            windows->cursor->setVisible(windows->cursor, 1);
            screen->step++;
        }
        break;
    case 3:
        STSTATUS_data.funcs.updateFade(&screen->panelFades[2]);
        if (STSTATUS_data.funcs.updateFade(&screen->panelFades[4])) {
            STSTATUS_showPartnerEquipment(screen, windows, 1);
            STSTATUS_showPartnerStats(screen, windows, 1);
            screen->step = 5;
        }
        break;
    case 5:
        option = screen->option;
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            screen->option = 0;
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            screen->option = 1;
        }
        if (option != screen->option) {
            SOUND.playSound(SOUND_CURSOR);
            windows->cursor->setPos(windows->cursor, 0x98, screen->option * 14 + 0x13);
        } else if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            screen->step = 10;
            screen->counter = 0;
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            screen->step = 10;
            screen->option = 0;
            screen->counter = 1;
        }
        break;
    case 10:
        if (screen->counter) {
            STSTATUS_data.funcs.startFade(&screen->panelFades[0], 0);
            STSTATUS_showChosenPage(screen, windows, 0, 0);
        }
        STSTATUS_data.funcs.startFade(&screen->panelFades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->panelFades[3], 0);
        STSTATUS_data.funcs.startFade(&screen->panelFades[4], 0);
        STSTATUS_showStatusChoices(screen, windows, 0);
        STSTATUS_showPartnerSlots(screen, windows, 0);
        STSTATUS_showPartnerEquipment(screen, windows, 0);
        if (screen->option == 0) {
            STSTATUS_data.funcs.startFade(&screen->panelFades[5], 0);
            STSTATUS_data.funcs.startFade(&screen->panelFades[2], 0);
            STSTATUS_showPartnerStats(screen, windows, 0);
            windows->exp->setVisible(windows->exp, 0);
            windows->expLabel->setVisible(windows->expLabel, 0);
        }
        windows->cursor->setVisible(windows->cursor, 0);
        screen->step++;
        break;
    case 11:
        if (screen->counter) {
            STSTATUS_data.funcs.updateFade(&screen->panelFades[0]);
        }
        STSTATUS_data.funcs.updateFade(&screen->panelFades[1]);
        STSTATUS_data.funcs.updateFade(&screen->panelFades[3]);
        if (screen->option == 0) {
            STSTATUS_data.funcs.updateFade(&screen->panelFades[2]);
            STSTATUS_data.funcs.updateFade(&screen->panelFades[5]);
        }
        if (STSTATUS_data.funcs.updateFade(&screen->panelFades[4])) {
            screen->step++;
        }
        break;
    case 12:
        if (screen->counter) {
            screen->setSubstate(screen, 0);
        } else {
            screen->setSubstate(screen, 0x14);
        }
        break;
    }
}

/* Starts the fades of the first page and the title (and, with one party
   member, of the help's panel); the next substate is the party's size */
static inline void STSTATUS_startStatusScreen(StatsScreen *screen) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
    STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
    if (screen->count == 1) {
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
    }
    screen->substate = screen->count;
}

/* One member: once the page and the panels are in, shows the page, the title
   and the help */
static inline void STSTATUS_openStatusScreenAlone(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showStatusPage(screen, windows, 0, 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x30);
        windows->help2->setString(windows->help2, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        screen->substate = 10;
    }
}

/* Two members: once the first page and the title are in, shows them and
   starts the second page and the help's panel */
static inline void STSTATUS_openFirstOfTwoStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showStatusPage(screen, windows, 0, 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        screen->substate = 4;
    }
}

/* Two members: once the second page and the help's panel are in, shows the
   page and the help */
static inline void STSTATUS_openSecondOfTwoStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showStatusPage(screen, windows, 1, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x30);
        windows->help2->setString(windows->help2, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        screen->substate = 10;
    }
}

/* Three members: once the first page and the title are in, shows them and
   starts the second page */
static inline void STSTATUS_openFirstOfThreeStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_showStatusPage(screen, windows, 0, 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        screen->substate = 5;
    }
}

/* Three members: once the second page is in, shows it and starts the third
   page and the help's panel */
static inline void STSTATUS_openSecondOfThreeStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        STSTATUS_showStatusPage(screen, windows, 1, 1);
        screen->substate++;
    }
}

/* Three members: once the third page and the help's panel are in, shows the
   page and the help */
static inline void STSTATUS_openThirdStatusPage(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_showStatusPage(screen, windows, 2, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x30);
        windows->help2->setString(windows->help2, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        screen->substate = 10;
    }
}

/* Choosing a party member: up and down move the cursor, cross closes the
   pages for the member's question, triangle closes the screen */
static inline void STSTATUS_chooseStatusMember(StatsScreen *screen) {
    s32 member;

    member = screen->member;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->member--;
        if (screen->member < 0) {
            screen->member = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->member++;
        if (screen->member > screen->count - 1) {
            screen->member = screen->count - 1;
        }
    }
    if (member != screen->member) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->cursorShown = 0;
        screen->option = 0;
        screen->setSubstate(screen, 0x32);
        screen->step = 1;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->cursorShown = 0;
        screen->setSubstate(screen, 0x32);
    }
}

/* Creates the chosen option's panel: the digivolution panel or the
   equipment panel */
static inline void STSTATUS_openStatusPanel(StatsScreen *screen, StatsScreenWindows *windows) {
    if (screen->option == 0) {
        if (windows->panel == NULL) {
            windows->panel = STSTATUS_createDigivolvePanel(screen);
        }
    } else if (windows->panel == NULL) {
        windows->panel = STSTATUS_createEquipPanel(screen);
    }
    screen->substate++;
}

/* Once the panel is gone, goes back to the question */
static inline void STSTATUS_waitStatusPanel(StatsScreen *screen, StatsScreenWindows *windows) {
    if (windows->panel == NULL) {
        screen->setSubstate(screen, 15);
        screen->counter = 1;
    }
}

/* Starts closing the pages: the last page, the help's panel and, with one
   member, the title */
static inline void STSTATUS_closeStatusScreen(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.startFade(&screen->pageFades[screen->count - 1], 0);
    STSTATUS_showStatusPage(screen, windows, screen->count - 1, 0);
    STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
    windows->help->setVisible(windows->help, 0);
    windows->help2->setVisible(windows->help2, 0);
    if (screen->count == 1) {
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
    }
    screen->substate = screen->count + 0x32;
}

/* One member: once the page and the panels are closed, goes on to the end */
static inline void STSTATUS_closeStatusScreenAlone(StatsScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        screen->substate = 0x39;
    }
}

/* Two members: once the second page and the help's panel are closed, starts
   closing the first and the title */
static inline void STSTATUS_closeSecondOfTwoStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        STSTATUS_showStatusPage(screen, windows, 0, 0);
        windows->title->setVisible(windows->title, 0);
        screen->substate = 0x36;
    }
}

/* Three members: once the third page and the help's panel are closed, starts
   closing the second */
static inline void STSTATUS_closeThirdStatusPage(StatsScreen *screen, StatsScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showStatusPage(screen, windows, 1, 0);
        screen->substate = 0x37;
    }
}

/* Three members: once the second page is closed, starts closing the first
   and the title */
static inline void STSTATUS_closeSecondOfThreeStatusPages(StatsScreen *screen, StatsScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        STSTATUS_showStatusPage(screen, windows, 0, 0);
        windows->title->setVisible(windows->title, 0);
        screen->substate++;
    }
}

/* Two or three members: once the first page and the title are closed, goes
   on to the end */
static inline void STSTATUS_closeFirstStatusPage(StatsScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        screen->substate = 0x39;
    }
}

/* With the pages closed: the chosen member's question, or the screen's end */
static inline void STSTATUS_endStatusPages(StatsScreen *screen) {
    if (screen->step) {
        screen->setSubstate(screen, 15);
    } else {
        screen->state = 3;
    }
}

/* The fifth screen's steps: the pages fade in, a party member is chosen and
   its panel opens, then the pages fade out */
void STSTATUS_runStatusScreen(StatsScreen *screen, StatsScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_startStatusScreen(screen);
        break;
    case 1:
        STSTATUS_openStatusScreenAlone(screen, windows);
        break;
    case 2:
        STSTATUS_openFirstOfTwoStatusPages(screen, windows);
        break;
    case 4:
        STSTATUS_openSecondOfTwoStatusPages(screen, windows);
        break;
    case 3:
        STSTATUS_openFirstOfThreeStatusPages(screen, windows);
        break;
    case 5:
        STSTATUS_openSecondOfThreeStatusPages(screen, windows);
        break;
    case 6:
        STSTATUS_openThirdStatusPage(screen, windows);
        break;
    case 10:
        screen->cursorShown = 1;
        screen->substate++;
        break;
    case 11:
        STSTATUS_chooseStatusMember(screen);
        break;
    case 15:
        STSTATUS_runStatusChoice(screen, windows);
        break;
    case 0x14:
        STSTATUS_openStatusPanel(screen, windows);
        break;
    case 0x15:
        STSTATUS_waitStatusPanel(screen, windows);
        break;
    case 0x32:
        STSTATUS_closeStatusScreen(screen, windows);
        break;
    case 0x33:
        STSTATUS_closeStatusScreenAlone(screen);
        break;
    case 0x34:
        STSTATUS_closeSecondOfTwoStatusPages(screen, windows);
        break;
    case 0x35:
        STSTATUS_closeThirdStatusPage(screen, windows);
        break;
    case 0x37:
        STSTATUS_closeSecondOfThreeStatusPages(screen, windows);
        break;
    case 0x36:
    case 0x38:
        STSTATUS_closeFirstStatusPage(screen);
        break;
    case 0x39:
        STSTATUS_endStatusPages(screen);
        break;
    }
}

/* The fifth screen's task: counts the party, starts the pages' fades and creates
   the windows, then runs and draws the screen */
void STSTATUS_updateStatusScreen(StatsScreen *screen, StatsScreenWindows *windows) {
    s32 i;

    switch (screen->state) {
    case TASK_INIT:
    default:
        screen->nextState(screen);
        for (i = 0; i < 3; i++) {
            if (GAME.funcs.getPartyMember(i) >= 0) {
                screen->count++;
            }
        }
        for (i = 0; i < screen->count; i++) {
            screen->pageFades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->pageFades[i], 1);
        }
        for (i = 0; i < 2; i++) {
            screen->fades[i].duration = 10;
            STSTATUS_data.funcs.startFade(&screen->fades[i], 1);
        }
        screen->panelFades[0].duration = 10;
        screen->panelFades[1].duration = 10;
        screen->panelFades[2].duration = 10;
        screen->panelFades[4].duration = 10;
        screen->panelFades[3].duration = 10;
        screen->panelFades[5].duration = 10;
        screen->fade.duration = 8;
        STSTATUS_createStatusWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runStatusScreen(screen, windows);
        STSTATUS_drawStatusScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the field menu's fifth screen (task), the party's stats, slots and
   equipment */
Task *STSTATUS_createStatusScreen(FieldMenuScreen *menu, s32 extra) {
    StatsScreen *screen = createTask(STSTATUS_updateStatusScreen, sizeof(StatsScreen), sizeof(StatsScreenWindows));

    screen->previewStats = STSTATUS_previewStats;
    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->menu = menu;
    return (Task *)screen;
}

s32 STSTATUS_pageStats4[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
/* The stats screen 4 shows, as STSTATUS_panelStats */
s32 STSTATUS_equipStats[] = {
    STAT_STRENGTH, STAT_DEFENSE, STAT_SPIRIT, STAT_WISDOM,
    STAT_SPEED, STAT_CHARISMA, STAT_RESISTS, STAT_RESISTS + 1,
    STAT_RESISTS + 2, STAT_RESISTS + 3, STAT_RESISTS + 4, STAT_RESISTS + 5,
    STAT_RESISTS + 6,
};
/* The strings of the empty equipment slots, as STSTATUS_slotStrings */
s32 STSTATUS_equipStrings[] = {
    65, 66, 67, 77,
    68, 68,
};
