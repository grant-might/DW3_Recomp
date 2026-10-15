/* The sixth object of STSTATUS.PRO (see card_screen.c), the first screen
   (STSTATUS_createItemScreen): its rodata starts at 0x800829D0 (USA). */

#include "menus/field_menu.h"

/* Creates the first screen's windows */
void STSTATUS_createItemWindows(ItemScreen *screen, ItemScreenWindows *windows) {
    WindowPos *pos;
    s32 i;
    s32 j;

    pos = &STSTATUS_data.layout[11];
    windows->title = createTextWindow(screen->layer, 1, pos->x, pos->y);
    pos = &STSTATUS_data.layout[12];
    windows->help = createTextWindow(screen->layer, 1, pos->x, pos->y);
    windows->kind = createTextWindow(screen->layer, 1, pos->x, pos->y + 14);
    windows->answers[0] = createTextWindow(screen->layer, 1, pos->x + 0x10, pos->y + 14);
    windows->answers[1] = createTextWindow(screen->layer, 1, pos->x + 0x42, pos->y + 14);
    windows->answerCursor = createCursor(screen->layer, screen->depth - 1, pos->x, pos->y + 14);
    windows->answerCursor->setVisible(windows->answerCursor, 0);
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
    windows->money = createTextWindow(screen->layer, 3, 0x42, 0xA6);
    windows->moneyLabel = createTextWindow(screen->layer, 3, 0x46, 0xA6);
    pos = &STSTATUS_data.layout[15];
    for (i = 0; i < 5; i++) {
        windows->options[i] = createTextWindow(screen->layer, 1, pos->x, pos->y + i * 14);
    }
    windows->optionCursor = createCursor(screen->layer, screen->depth - 1, 0xB0, screen->option * 14 + 0x31);
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    windows->itemName = createTextWindow(screen->layer, 1, 0x25, 0xAE);
    windows->equippedLabel = createTextWindow(screen->layer, 1, 0xA1, 0xAF);
    windows->equipped = createTextWindow(screen->layer, 1, 0xE0, 0xAF);
    windows->ownedLabel = createTextWindow(screen->layer, 1, 0xEA, 0xAF);
    windows->owned = createTextWindow(screen->layer, 1, 0x128, 0xAF);
}

/* As STSTATUS_showCardPage */
void STSTATUS_showItemPage(ItemScreen *screen, ItemScreenWindows *windows, s32 member, s32 show) {
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
            windows->pages[member].values[i]->setNumber(windows->pages[member].values[i], 0, stats.stats[STSTATUS_pageStats0[i]]);
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

/* Shows or hides the player's money */
void STSTATUS_showMoney(ItemScreen *screen, ItemScreenWindows *windows, s32 show) {
    if (show != 0) {
        windows->moneyLabel->setString(windows->moneyLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 5);
        windows->money->setNumber(windows->money, 0, GAME.money);
        windows->money->setRightAlign(windows->money, 1);
    } else {
        windows->moneyLabel->setVisible(windows->moneyLabel, 0);
        windows->money->setVisible(windows->money, 0);
    }
}

/* Shows or hides the first screen's five options, the item lists */
void STSTATUS_showItemLists(ItemScreen *screen, ItemScreenWindows *windows, s32 show) {
    WindowPos *layout;
    s32 i;

    if (show != 0) {
        layout = &STSTATUS_data.layout[15];
        for (i = 0; i < 5; i++) {
            windows->options[i]->setString(windows->options[i], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)),
                                           layout->string + i);
        }
    } else {
        for (i = 0; i < 5; i++) {
            windows->options[i]->setVisible(windows->options[i], 0);
        }
    }
}

/* Shows or hides the chosen item's name and how many are equipped and owned */
void STSTATUS_showChosenItem(ItemScreen *screen, s32 show) {
    ItemScreenWindows *windows = screen->children;

    if (show) {
        windows->itemName->setString(windows->itemName, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), screen->item);
        windows->equippedLabel->setString(windows->equippedLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x20);
        windows->equipped->setNumber(windows->equipped, 0, GAME.equippedItems[screen->item]);
        windows->equipped->setRightAlign(windows->equipped, 1);
        windows->ownedLabel->setString(windows->ownedLabel, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x16);
        windows->owned->setNumber(windows->owned, 0, GAME.items[screen->item]);
        windows->owned->setRightAlign(windows->owned, 1);
        screen->itemShown = 1;
    } else {
        windows->itemName->setVisible(windows->itemName, 0);
        windows->equippedLabel->setVisible(windows->equippedLabel, 0);
        windows->equipped->setVisible(windows->equipped, 0);
        windows->ownedLabel->setVisible(windows->ownedLabel, 0);
        windows->owned->setVisible(windows->owned, 0);
        screen->itemShown = 0;
    }
}

/* The help line: 1 the chosen item and the kind of a weapon, 2 a question
   with two answers, -2 hides the answers, -1 the kind, others all */
void STSTATUS_showItemHelp(ItemScreen *screen, s32 mode) {
    ItemScreenWindows *windows = screen->children;
    ItemInfo *info;
    ItemData *data;

    if (mode == 1) {
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_INFO)), screen->item);
        info = ITEM_FUNCS.get(screen->item);
        if (info->type >= 2 && info->type <= 14) {
            data = info->data.record;
            windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), STSTATUS_kindStrings0[data->weapon.kind]);
        }
    } else if (mode == -1) {
        windows->kind->setVisible(windows->kind, 0);
    } else if (mode == 2) {
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x31);
        windows->answers[0]->setString(windows->answers[0], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x32);
        windows->answers[1]->setString(windows->answers[1], FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x33);
        windows->answerCursor->setVisible(windows->answerCursor, 1);
    } else if (mode == -2) {
        windows->answers[0]->setVisible(windows->answers[0], 0);
        windows->answers[1]->setVisible(windows->answers[1], 0);
        windows->answerCursor->setVisible(windows->answerCursor, 0);
    } else {
        windows->help->setVisible(windows->help, 0);
        windows->kind->setVisible(windows->kind, 0);
    }
}

/* Opens or closes the chosen item's panel (its icon and frame); closing it also
   hides the item's name, counts and help */
void STSTATUS_fadeItemInfo(ItemScreen *screen, s32 show) {
    if (show != 0) {
        STSTATUS_data.funcs.startFade(&screen->fade, 1);
        return;
    }
    STSTATUS_data.funcs.startFade(&screen->fade, 0);
    STSTATUS_showChosenItem(screen, 0);
    STSTATUS_showItemHelp(screen, 0);
}

/* Moves the chosen item's panel on; 1 once it is fully open or closed */
s32 STSTATUS_itemInfoFaded(ItemScreen *screen) {
    return STSTATUS_data.funcs.updateFade(&screen->fade) != 0;
}

/* Uses the chosen item on the chosen party member */
void STSTATUS_useItem(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 amount = 0;
    ItemEffect *effect = ITEM_FUNCS.get(screen->item)->data.effect;
    PartnerStats *stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(screen->member));
    StatusStatItem *entry;
    s16 *values;
    s32 i;
    s32 result = 0;

    switch (effect->kind) {
    case 0:
        break;
    case 1:
        if (stats->stats[STAT_HP] < stats->stats[STAT_MAX_HP]) {
            stats->stats[STAT_HP] += effect->amount;
            amount = effect->amount;
            if (stats->stats[STAT_HP] > stats->stats[STAT_MAX_HP]) {
                stats->stats[STAT_HP] = stats->stats[STAT_MAX_HP];
                windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x52);
                result = 2;
            } else {
                result = 1;
            }
        }
        break;
    case 17:
        if (stats->stats[STAT_TP] < 99) {
            stats->stats[STAT_TP] += effect->amount;
            amount = effect->amount;
            result = 1;
            if (stats->stats[STAT_TP] > 99) {
                stats->stats[STAT_TP] = 99;
            }
        }
        break;
    default:
        for (i = 0; STSTATUS_statItems[i].kind != -1; i++) {
            entry = &STSTATUS_statItems[i];
            if (effect->kind == entry->kind) {
                values = stats->stats;
                if (values[entry->stat] < entry->max) {
                    amount = RANDOM.next() % effect->amount + 1;
                    values[entry->stat] += amount;
                    if (values[entry->stat] > entry->max) {
                        values[entry->stat] = entry->max;
                    }
                    result = 1;
                }
                break;
            }
        }
        break;
    }
    if (result != 0) {
        if (result == 1) {
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), effect->kind + 0x52);
            windows->help->setNumber(windows->help, 1, amount);
        }
        GAME.items[screen->item]--;
        STSTATUS_showItemPage(screen, windows, screen->member, 1);
        SOUND.playSound(SOUND_RECOVERY);
    } else {
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x6A);
        SOUND.playSound(SOUND_MENU_CONFIRM);
    }
}

/* Moves the partners' portraits to their next frames every 13 frames */
static inline void STSTATUS_animateItemPortraits(ItemScreen *screen) {
    s32 id;
    s32 i;

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
}

/* Moves the member cursor's palette on every 9 frames */
static inline void STSTATUS_stepItemCursor(ItemScreen *screen) {
    if (GFX.funcs.getTime() - screen->cursorTime >= 9) {
        screen->cursorTime = GFX.funcs.getTime();
        screen->cursorFrame++;
        if (screen->cursorFrame >= 8) {
            screen->cursorFrame = 0;
        }
    }
}

/* Draws the partners' portraits and frames, the item, the cursor and the help arrow */
void STSTATUS_drawItemScreen(ItemScreen *screen) {
    SpriteDrawer sprite;
    s32 id;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(screen->layer, screen->depth);
    STSTATUS_animateItemPortraits(screen);
    for (i = 0; i < screen->count; i++) {
        if (screen->pageFades[i].level != 0) {
            if (screen->pageFades[i].level != ONE) {
                sprite.setScale(screen->pageFades[i].level, screen->pageFades[i].level, ONE);
                sprite.setPivot(0x7C, i * 0x2E + 0x27);
            } else {
                sprite.setScale(ONE, ONE, ONE);
            }
            id = GAME.funcs.getPartyMember(i);
            sprite.setTexture(0x280, 0x100);
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
    if (screen->fades[0].level != 0) {
        if (screen->fades[0].level != ONE) {
            sprite.setScale(screen->fades[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x19);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x18, 0x22, 0xD);
    }
    if (screen->fades2[0].level != 0) {
        if (screen->fades2[0].level != ONE) {
            sprite.setScale(screen->fades2[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x52);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x1C, 0xA8, 0x28);
    }
    if (screen->fades2[1].level != 0) {
        if (screen->fades2[1].level != ONE) {
            sprite.setScale(screen->fades2[1].level, ONE, ONE);
            sprite.setPivot(0, 0xA8);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x1A, 0, 0x9E);
    }
    if (screen->fades[1].level != 0) {
        if (screen->blink) {
            if (GFX.funcs.getTime() - screen->blinkTime >= 4) {
                screen->blinkTime = GFX.funcs.getTime();
                screen->blinkFrame++;
                if (screen->blinkFrame >= 5) {
                    screen->blinkFrame = 0;
                }
            }
            sprite.setTexture(0x140, 0);
            sprite.setClutRow(screen->blinkFrame);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0xA, 0x123, 0xD6);
            sprite.setClutRow(0);
        }
        if (screen->fades[1].level != ONE) {
            sprite.setScale(screen->fades[1].level, ONE, ONE);
            sprite.setPivot(0, 0xD3);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x20, 0, 0xC2);
    }
    if (screen->fade.level != 0) {
        if (screen->fade.level != ONE) {
            sprite.setScale(screen->fade.level, ONE, ONE);
            sprite.setPivot(0x140, 0xB2);
        } else {
            sprite.setScale(ONE, ONE, ONE);
            sprite.setTexture(0x140, 0);
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(screen->item), 0x16, 0xAE);
        }
        sprite.setTexture(0x280, 0x100);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x2F, 0xF, 0xA5);
    }
    if (screen->cursorShown) {
        STSTATUS_stepItemCursor(screen);
        sprite.setScale(ONE, ONE, ONE);
        sprite.setLayerId(screen->layer, screen->depth - 1);
        sprite.setClutRow(screen->cursorFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1E, 0, screen->member * 0x2E + 0x11);
    }
}

/* Starts the fades of the first page and the title and, with one party
   member, of the other panels (the help's unless it stayed open); the next
   substate is the party's size */
static inline void STSTATUS_startItemScreen(ItemScreen *screen) {
    switch (screen->count) {
    case 1:
    default:
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
        if (screen->step == 0) {
            STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        }
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 1);
        break;
    case 2:
    case 3:
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
        break;
    }
    screen->substate = screen->count;
}

/* One member: once the panels are in, shows the title, the page, the help,
   the money and the options */
static inline void STSTATUS_openItemScreenAlone(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (screen->step == 0) {
        STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    }
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x14);
        STSTATUS_showItemPage(screen, windows, 0, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showMoney(screen, windows, 1);
        STSTATUS_showItemLists(screen, windows, 1);
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        screen->substate = 10;
    }
}

/* Two members: once the first page and the title are in, shows them and
   starts the second page and the other panels */
static inline void STSTATUS_openFirstOfTwoItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        if (screen->step == 0) {
            STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        }
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x14);
        STSTATUS_showItemPage(screen, windows, 0, 1);
        screen->substate = 4;
    }
}

/* Two members: once the second page and the panels are in, shows the page,
   the options, the help and the money */
static inline void STSTATUS_openSecondOfTwoItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (screen->step == 0) {
        STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    }
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        STSTATUS_showItemPage(screen, windows, 1, 1);
        STSTATUS_showItemLists(screen, windows, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        STSTATUS_showMoney(screen, windows, 1);
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        screen->substate = 10;
    }
}

/* Three members: once the first page and the title are in, shows them and
   starts the second page and the options' panel */
static inline void STSTATUS_openFirstOfThreeItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 1);
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x14);
        STSTATUS_showItemPage(screen, windows, 0, 1);
        screen->substate = 5;
    }
}

/* Three members: once the second page and the options' panel are in, shows
   them and starts the third page and the help's and the money's panels */
static inline void STSTATUS_openSecondOfThreeItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
        if (screen->step == 0) {
            STSTATUS_data.funcs.startFade(&screen->fades[1], 1);
        }
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 1);
        STSTATUS_showItemPage(screen, windows, 1, 1);
        STSTATUS_showItemLists(screen, windows, 1);
        screen->substate++;
    }
}

/* Three members: once the third page and the panels are in, shows the page,
   the help and the money */
static inline void STSTATUS_openThirdItemPage(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    if (screen->step == 0) {
        STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    }
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[1])) {
        STSTATUS_showItemPage(screen, windows, 2, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        STSTATUS_showMoney(screen, windows, 1);
        screen->substate = 10;
    }
}

/* The options, the item lists: up and down move the cursor, cross opens the
   chosen list (or says it is empty), triangle closes the screen */
static inline void STSTATUS_chooseItemList(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 old;

    old = screen->option;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        screen->option--;
        if (screen->option < 0) {
            screen->option = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        screen->option++;
        if (screen->option >= 5) {
            screen->option = 4;
        }
    }
    if (old != screen->option) {
        SOUND.playSound(SOUND_CURSOR);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        windows->optionCursor->setPos(windows->optionCursor, 0xB0, screen->option * 0xE + 0x31);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        screen->itemCount = ITEM_FUNCS.list(STSTATUS_itemLists[screen->option], screen->items);
        if (screen->itemCount <= 0) {
            windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x64);
        } else {
            screen->substate = 11;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->substate = 0x32;
    }
}

/* Starts closing the pages and the panels but the help's, for the item list */
static inline void STSTATUS_closeItemScreenForList(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 i;

    for (i = 0; i < screen->count; i++) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[i], 0);
        STSTATUS_showItemPage(screen, windows, i, 0);
    }
    STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
    windows->title->setVisible(windows->title, 0);
    windows->help->setVisible(windows->help, 0);
    windows->kind->setVisible(windows->kind, 0);
    STSTATUS_data.funcs.startFade(&screen->fades2[1], 0);
    windows->moneyLabel->setVisible(windows->moneyLabel, 0);
    windows->money->setVisible(windows->money, 0);
    STSTATUS_data.funcs.startFade(&screen->fades2[0], 0);
    for (i = 0; i < 5; i++) {
        windows->options[i]->setVisible(windows->options[i], 0);
    }
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    screen->substate++;
}

/* Once they are closed, opens the chosen option's item list and the item's
   panel */
static inline void STSTATUS_openItemList(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 i;

    for (i = 0; i < screen->count; i++) {
        STSTATUS_data.funcs.updateFade(&screen->pageFades[i]);
    }
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        windows->panel = STSTATUS_createItemList(screen, screen->option, 0);
        STSTATUS_fadeItemInfo(screen, 1);
        screen->substate = 15;
    }
}

/* Once the item list closes: back to the options when it was cancelled, to
   the end (0x28) at -2, else to choosing a member to use the item on */
static inline void STSTATUS_leaveItemList(ItemScreen *screen, ItemScreenWindows *windows) {
    if (windows->panel == NULL) {
        if (screen->itemIndex == -1) {
            screen->substate = 0;
            screen->step = 1;
        } else if (screen->itemIndex == -2) {
            screen->substate = 0x28;
        } else {
            screen->setSubstate(screen, 0x14);
        }
    }
}

/* Once the first page and the title are in, shows them and starts the second
   page, or shows the cursor with one member */
static inline void STSTATUS_openFirstMemberPage(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x27);
        STSTATUS_showItemPage(screen, windows, 0, 1);
        if (screen->count != 1) {
            STSTATUS_data.funcs.startFade(&screen->pageFades[1], 1);
            screen->substate++;
        } else {
            screen->substate = 0x18;
        }
    }
}

/* Once the second page is in, shows it and starts the third, or shows the
   cursor with two members */
static inline void STSTATUS_openSecondMemberPage(ItemScreen *screen, ItemScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_showItemPage(screen, windows, 1, 1);
        if (screen->count == 2) {
            screen->substate = 0x18;
        } else {
            STSTATUS_data.funcs.startFade(&screen->pageFades[2], 1);
            screen->substate++;
        }
    }
}

/* Once the third page is in, shows it */
static inline void STSTATUS_openThirdMemberPage(ItemScreen *screen, ItemScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[2])) {
        STSTATUS_showItemPage(screen, windows, 2, 1);
        screen->substate++;
    }
}

/* Choosing the member to use the item on: up and down move the cursor, cross
   uses the item, triangle goes back to the list */
static inline void STSTATUS_chooseItemMember(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 old;

    old = screen->member;
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
    if (old != screen->member) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        STSTATUS_fadeItemInfo(screen, 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_useItem(screen, windows);
        screen->substate = 0x64;
        screen->step = 0;
        screen->blink = 1;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        screen->step = 1;
        screen->substate++;
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
    }
}

/* Hides the cursor; once the title's and the item's panels are closed, goes
   on to closing the pages */
static inline void STSTATUS_closeItemTitle(ItemScreen *screen) {
    s32 done;

    screen->cursorShown = 0;
    done = STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_itemInfoFaded(screen) && done) {
        screen->substate++;
    }
}

/* Starts closing the last page (and the title with one member) */
static inline void STSTATUS_closeMemberPages(ItemScreen *screen, ItemScreenWindows *windows) {
    switch (screen->count) {
    case 1:
    default:
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showItemPage(screen, windows, 0, 0);
        windows->title->setVisible(windows->title, 0);
        screen->substate = 0x1E;
        break;
    case 2:
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showItemPage(screen, windows, 1, 0);
        screen->substate = 0x1D;
        break;
    case 3:
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 0);
        STSTATUS_showItemPage(screen, windows, 2, 0);
        screen->substate = 0x1C;
        break;
    }
}

/* Once the third page is closed, starts closing the second */
static inline void STSTATUS_closeThirdMemberPage(ItemScreen *screen, ItemScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[2])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_showItemPage(screen, windows, 1, 0);
        screen->substate++;
    }
}

/* Once the second page is closed, starts closing the first and the title */
static inline void STSTATUS_closeSecondMemberPage(ItemScreen *screen, ItemScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_showItemPage(screen, windows, 0, 0);
        windows->title->setVisible(windows->title, 0);
        screen->substate++;
    }
}

/* Once the first page is closed, opens the item list again on the item (and
   the item's panel, after an item was used) */
static inline void STSTATUS_reopenItemList(ItemScreen *screen, ItemScreenWindows *windows) {
    if (STSTATUS_data.funcs.updateFade(&screen->pageFades[0])) {
        if (screen->step == 0) {
            STSTATUS_fadeItemInfo(screen, 1);
        }
        windows->panel = STSTATUS_createItemList(screen, screen->option, screen->item);
        screen->substate = 15;
    }
}

/* Hides the cursor; once the item's panel is closed, waits for cross */
static inline void STSTATUS_closeUsedItem(ItemScreen *screen) {
    screen->cursorShown = 0;
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    if (STSTATUS_itemInfoFaded(screen)) {
        screen->substate++;
    }
}

/* Cross after an item was used: back to the item list, or to the options
   when the list is empty now */
static inline void STSTATUS_confirmItemUsed(ItemScreen *screen) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        screen->itemCount = ITEM_FUNCS.list(STSTATUS_itemLists[screen->option], screen->items);
        if (screen->itemCount <= 0) {
            screen->nextSubstate(screen);
            STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
            STSTATUS_data.funcs.startFade(&screen->fades2[1], 1);
            STSTATUS_data.funcs.startFade(&screen->fades2[0], 1);
        } else {
            screen->substate = 0x1B;
        }
        screen->blink = 0;
    }
}

/* Once the panels are in again, shows the title, the options, the money and
   the help */
static inline void STSTATUS_reopenItemOptions(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x14);
        STSTATUS_showItemLists(screen, windows, 1);
        STSTATUS_showMoney(screen, windows, 1);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        windows->kind->setString(windows->kind, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), 0x15);
        windows->optionCursor->setVisible(windows->optionCursor, 1);
        screen->substate = 10;
    }
}

/* Greys the options' cursor and says the chosen list is empty */
static inline void STSTATUS_lockItemOptions(ItemScreen *screen, ItemScreenWindows *windows) {
    windows->optionCursor->setStill(windows->optionCursor, 1);
    windows->optionCursor->setPalette(windows->optionCursor, PALETTE_GREY);
    windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x64);
    screen->substate++;
}

/* Triangle gives the options' cursor back */
static inline void STSTATUS_unlockItemOptions(ItemScreen *screen, ItemScreenWindows *windows) {
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        windows->optionCursor->setStill(windows->optionCursor, 0);
        windows->optionCursor->setPalette(windows->optionCursor, PALETTE_WHITE);
        windows->help->setString(windows->help, FILE_CACHE.load(TEXT_FILE(TEXT_STATUS)), screen->option + 0x1B);
        screen->substate = 10;
    }
}

/* Starts closing the screen: the last page and, with it, the panels the
   other pages don't wait for */
static inline void STSTATUS_closeItemScreen(ItemScreen *screen, ItemScreenWindows *windows) {
    switch (screen->count) {
    case 1:
    default:
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 0);
        STSTATUS_showItemPage(screen, windows, 0, 0);
        windows->title->setVisible(windows->title, 0);
        windows->help->setVisible(windows->help, 0);
        windows->kind->setVisible(windows->kind, 0);
        windows->moneyLabel->setVisible(windows->moneyLabel, 0);
        windows->money->setVisible(windows->money, 0);
        STSTATUS_showItemLists(screen, windows, 0);
        break;
    case 2:
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 0);
        STSTATUS_showItemPage(screen, windows, 1, 0);
        windows->help->setVisible(windows->help, 0);
        windows->kind->setVisible(windows->kind, 0);
        windows->moneyLabel->setVisible(windows->moneyLabel, 0);
        windows->money->setVisible(windows->money, 0);
        STSTATUS_showItemLists(screen, windows, 0);
        break;
    case 3:
        STSTATUS_data.funcs.startFade(&screen->pageFades[2], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[1], 0);
        STSTATUS_showItemPage(screen, windows, 2, 0);
        windows->help->setVisible(windows->help, 0);
        windows->kind->setVisible(windows->kind, 0);
        windows->moneyLabel->setVisible(windows->moneyLabel, 0);
        windows->money->setVisible(windows->money, 0);
        break;
    }
    windows->optionCursor->setVisible(windows->optionCursor, 0);
    screen->substate = screen->count + 0x32;
}

/* One member: ends the screen once the page and the panels are closed */
static inline void STSTATUS_closeItemScreenAlone(ItemScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[0]);
    STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        screen->state = 3;
    }
}

/* Two members: once the second page and its panels are closed, starts
   closing the first and the title */
static inline void STSTATUS_closeSecondOfTwoItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    STSTATUS_data.funcs.updateFade(&screen->fades2[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_showItemPage(screen, windows, 0, 0);
        screen->substate = 0x36;
    }
}

/* Two members: ends the screen once the first page and the help are closed */
static inline void STSTATUS_closeFirstOfTwoItemPages(ItemScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[1])) {
        screen->state = 3;
    }
}

/* Three members: once the third page and its panels are closed, starts
   closing the second and the options */
static inline void STSTATUS_closeThirdItemPage(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[2]);
    STSTATUS_data.funcs.updateFade(&screen->fades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[1])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[1], 0);
        STSTATUS_data.funcs.startFade(&screen->fades2[0], 0);
        STSTATUS_showItemPage(screen, windows, 1, 0);
        STSTATUS_showItemLists(screen, windows, 0);
        screen->substate = 0x37;
    }
}

/* Three members: once the second page and the options are closed, starts
   closing the first and the title */
static inline void STSTATUS_closeSecondOfThreeItemPages(ItemScreen *screen, ItemScreenWindows *windows) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[1]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades2[0])) {
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 0);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 0);
        windows->title->setVisible(windows->title, 0);
        STSTATUS_showItemPage(screen, windows, 0, 0);
        screen->substate++;
    }
}

/* Three members: ends the screen once the first page and the title are
   closed */
static inline void STSTATUS_closeFirstOfThreeItemPages(ItemScreen *screen) {
    STSTATUS_data.funcs.updateFade(&screen->pageFades[0]);
    if (STSTATUS_data.funcs.updateFade(&screen->fades[0])) {
        screen->state = 3;
    }
}

/* The first screen's update: opens the pages, picks an item list, then
   the party member to use the chosen item on, and closes them */
void STSTATUS_runItemScreen(ItemScreen *screen, ItemScreenWindows *windows) {
    switch (screen->substate) {
    case 0:
    default:
        STSTATUS_startItemScreen(screen);
        break;
    case 1:
        STSTATUS_openItemScreenAlone(screen, windows);
        break;
    case 2:
        STSTATUS_openFirstOfTwoItemPages(screen, windows);
        break;
    case 4:
        STSTATUS_openSecondOfTwoItemPages(screen, windows);
        break;
    case 3:
        STSTATUS_openFirstOfThreeItemPages(screen, windows);
        break;
    case 5:
        STSTATUS_openSecondOfThreeItemPages(screen, windows);
        break;
    case 6:
        STSTATUS_openThirdItemPage(screen, windows);
        break;
    case 10:
        STSTATUS_chooseItemList(screen, windows);
        break;
    case 11:
        STSTATUS_closeItemScreenForList(screen, windows);
        break;
    case 12:
        STSTATUS_openItemList(screen, windows);
        break;
    case 15:
        if (STSTATUS_itemInfoFaded(screen)) {
            screen->substate++;
        }
        break;
    case 0x10:
        STSTATUS_leaveItemList(screen, windows);
        break;
    case 0x14:
        STSTATUS_data.funcs.startFade(&screen->pageFades[0], 1);
        STSTATUS_data.funcs.startFade(&screen->fades[0], 1);
        screen->substate++;
        break;
    case 0x15:
        STSTATUS_openFirstMemberPage(screen, windows);
        break;
    case 0x16:
        STSTATUS_openSecondMemberPage(screen, windows);
        break;
    case 0x17:
        STSTATUS_openThirdMemberPage(screen, windows);
        break;
    case 0x18:
        screen->cursorShown = 1;
        screen->substate++;
        break;
    case 0x19:
        STSTATUS_chooseItemMember(screen, windows);
        break;
    case 0x1A:
        STSTATUS_closeItemTitle(screen);
        break;
    case 0x1B:
        STSTATUS_closeMemberPages(screen, windows);
        break;
    case 0x1C:
        STSTATUS_closeThirdMemberPage(screen, windows);
        break;
    case 0x1D:
        STSTATUS_closeSecondMemberPage(screen, windows);
        break;
    case 0x1E:
        STSTATUS_reopenItemList(screen, windows);
        break;
    case 0x28:
        screen->state = 3;
        break;
    case 0x64:
        STSTATUS_closeUsedItem(screen);
        break;
    case 0x65:
        STSTATUS_confirmItemUsed(screen);
        break;
    case 0x66:
        STSTATUS_reopenItemOptions(screen, windows);
        break;
    case 0x6E:
        STSTATUS_lockItemOptions(screen, windows);
        break;
    case 0x6F:
        STSTATUS_unlockItemOptions(screen, windows);
        break;
    case 0x32:
        STSTATUS_closeItemScreen(screen, windows);
        break;
    case 0x33:
        STSTATUS_closeItemScreenAlone(screen);
        break;
    case 0x34:
        STSTATUS_closeSecondOfTwoItemPages(screen, windows);
        break;
    case 0x36:
        STSTATUS_closeFirstOfTwoItemPages(screen);
        break;
    case 0x35:
        STSTATUS_closeThirdItemPage(screen, windows);
        break;
    case 0x37:
        STSTATUS_closeSecondOfThreeItemPages(screen, windows);
        break;
    case 0x38:
        STSTATUS_closeFirstOfThreeItemPages(screen);
        break;
    }
}

/* The first screen's task: counts the party, sets the fades and creates the
   windows, then runs and draws the screen */
void STSTATUS_updateItemScreen(ItemScreen *screen, ItemScreenWindows *windows) {
    s32 duration;
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
        }
        duration = 10; /* the match depends on this temporary */
        for (i = 1; i >= 0; i--) {
            screen->fades[i].duration = duration;
        }
        screen->fades2[0].duration = 10;
        screen->fades2[1].duration = 10;
        screen->fade.duration = 10;
        STSTATUS_createItemWindows(screen, windows);
        break;
    case TASK_RUN:
        STSTATUS_runItemScreen(screen, windows);
        STSTATUS_drawItemScreen(screen);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the field menu's first screen (task), the items */
Task *STSTATUS_createItemScreen(FieldMenuScreen *menu, s32 extra) {
    ItemScreen *screen = createTask(STSTATUS_updateItemScreen, sizeof(ItemScreen), sizeof(ItemScreenWindows));

    screen->layer = SCREEN_LAYER;
    screen->depth = 6;
    screen->menu = menu;
    return (Task *)screen;
}

/* The item lists of the first screen's options */
s32 STSTATUS_itemLists[] = {
    1, 0x80000002, 0x80000003, 0x80000004,
    0,
};
s32 STSTATUS_pageStats0[] = {
    STAT_LEVEL, STAT_HP, STAT_MAX_HP, STAT_MP,
    STAT_MAX_MP,
};
/* The strings of the item kinds (WeaponData.kind), from 0 */
s32 STSTATUS_kindStrings0[] = {
    0, 67, 77, 79,
    65, 66, 68, 80,
    68,
};
/* What the items that raise a stat raise */
StatusStatItem STSTATUS_statItems[] = {
    { 2, STAT_MAX_HP, 9999 },
    { 3, STAT_MAX_MP, 9999 },
    { 4, STAT_STRENGTH, 999 },
    { 5, STAT_DEFENSE, 999 },
    { 6, STAT_SPIRIT, 999 },
    { 7, STAT_WISDOM, 999 },
    { 8, STAT_SPEED, 999 },
    { 9, STAT_CHARISMA, 999 },
    { 10, STAT_RESISTS, 999 },
    { 11, STAT_RESISTS + 1, 999 },
    { 12, STAT_RESISTS + 2, 999 },
    { 13, STAT_RESISTS + 3, 999 },
    { 14, STAT_RESISTS + 4, 999 },
    { 15, STAT_RESISTS + 5, 999 },
    { 16, STAT_RESISTS + 6, 999 },
    { -1, 0, 0 },
};
