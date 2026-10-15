/* The screen that opens a card pack from the bag, and the packs' cards */

#include "cardgame/card_packs.h"

/* Creates the screen's windows and cursor */
void STCRDSHP_createPackOpenWindows(CardPackOpen *open, CardPackOpenWindows *win) {
    s32 i;

    for (i = 0; i < 8; i++) {
        win->packs[i] = createTextWindow(open->layer, 1, (i % 2) * 0x83 + 0x37, (i / 2) * 0xE + 0x39);
    }
    win->cursor = createCursor(open->layer, open->depth - 1, 0x1D, 0x39);
    win->cursor->setVisible(win->cursor, 0);
    win->page = createTextWindow(open->layer, 1, 0x92, 0x75);
    win->slash = createTextWindow(open->layer, 1, 0x93, 0x75);
    win->pages = createTextWindow(open->layer, 1, 0xA6, 0x75);
    win->prev = createTextWindow(open->layer, 1, 0x2D, 0x71);
    win->next = createTextWindow(open->layer, 1, 0x102, 0x71);
    win->name = createTextWindow(open->layer, 1, 0x8F, 0x8A);
    win->countLabel = createTextWindow(open->layer, 1, 0x10B, 0x8A);
    win->count = createTextWindow(open->layer, 1, 0x121, 0x8A);
    win->help[0] = createTextWindow(open->layer, 1, 0x14, 0xC2);
    win->help[1] = createTextWindow(open->layer, 1, 0x14, 0xD0);
    win->cardName = createTextWindow(open->layer, 1, 0x88, 0x80);
    win->pointsLabel = createTextWindow(open->layer, 1, 0x115, 0x80);
    win->points = createTextWindow(open->layer, 1, 0x126, 0x80);
    win->cardCountLabel = createTextWindow(open->layer, 1, 0x115, 0xA6);
    win->cardCount = createTextWindow(open->layer, 1, 0x12C, 0xA6);
    win->cardText = createTextWindow(open->layer, 1, 0x50, 0x97);
    win->apLabel = createTextWindow(open->layer, 1, 0xCE, 0x97);
    win->ap = createTextWindow(open->layer, 1, 0xF0, 0x97);
    win->hpLabel = createTextWindow(open->layer, 1, 0xCE, 0xA4);
    win->hp = createTextWindow(open->layer, 1, 0xF0, 0xA4);
}

/* Shows the page's packs, the page number and the arrows' labels, or hides
   them */
void STCRDSHP_showPackPage(CardPackOpen *open, CardPackOpenWindows *win, s32 show) {
    s32 i;
    s32 index;
    s32 pack;

    if (show) {
        for (i = 0; i < 8; i++) {
            index = open->page * 8 + i;
            pack = open->packs[index];
            if (index < open->packCount && pack != 0) {
                win->packs[i]->setString(win->packs[i], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), pack);
            } else {
                win->packs[i]->setVisible(win->packs[i], 0);
            }
        }
        win->page->setNumber(win->page, 0, open->page + 1);
        win->page->setRightAlign(win->page, 1);
        win->slash->setString(win->slash, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 9);
        win->pages->setNumber(win->pages, 0, open->pages);
        win->pages->setRightAlign(win->pages, 1);
        if (open->pages >= 2) {
            if (open->page > 0) {
                win->prev->setString(win->prev, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xA);
            } else {
                win->prev->setVisible(win->prev, 0);
            }
            if (open->page < open->pages - 1) {
                win->next->setString(win->next, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xB);
            } else {
                win->next->setVisible(win->next, 0);
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            win->packs[i]->setVisible(win->packs[i], 0);
        }
        win->page->setVisible(win->page, 0);
        win->slash->setVisible(win->slash, 0);
        win->pages->setVisible(win->pages, 0);
        win->prev->setVisible(win->prev, 0);
        win->next->setVisible(win->next, 0);
    }
}

/* Shows the name of the pack under the cursor, how many the bag holds and
   the help, or hides them */
void STCRDSHP_showPack(CardPackOpen *open, CardPackOpenWindows *win, s32 show) {
    s32 pack;

    if (show) {
        pack = open->packs[open->cursor];
        win->name->setString(win->name, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), pack);
        win->countLabel->setString(win->countLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
        win->count->setNumber(win->count, 0, GAME.items[pack]);
        win->count->setRightAlign(win->count, 1);
        win->help[0]->setString(win->help[0], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0xF);
        win->help[1]->setString(win->help[1], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 4);
    } else {
        win->name->setVisible(win->name, 0);
        win->countLabel->setVisible(win->countLabel, 0);
        win->count->setVisible(win->count, 0);
        win->help[0]->setVisible(win->help[0], 0);
        win->help[1]->setVisible(win->help[1], 0);
    }
}

/* Shows the card under the cursor: its name, how many the player has and
   its numbers or its text, or hides them (also while it hasn't been seen) */
void STCRDSHP_showPackCard(CardPackOpen *open, CardPackOpenWindows *win, s32 show) {
    CardDrawer drawer;
    s32 card;

    card = open->cards[open->card];
    if (GAME.cardsSeen[card] != 0 && show) {
        initCardDrawer(&drawer);
        drawer.setCard(card);
        win->cardName->setString(win->cardName, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), card);
        win->cardCountLabel->setString(win->cardCountLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
        win->cardCount->setNumber(win->cardCount, 0, GAME.cards[card]);
        win->cardCount->setRightAlign(win->cardCount, 1);
        if (drawer.getKind() != 0) {
            win->pointsLabel->setVisible(win->pointsLabel, 0);
            win->points->setVisible(win->points, 0);
            win->cardText->setString(win->cardText, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), card);
            win->apLabel->setVisible(win->apLabel, 0);
            win->ap->setVisible(win->ap, 0);
            win->hpLabel->setVisible(win->hpLabel, 0);
            win->hp->setVisible(win->hp, 0);
        } else {
            win->pointsLabel->setString(win->pointsLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
            win->points->setNumber(win->points, 0, drawer.card->points);
            win->points->setRightAlign(win->points, 1);
            if (card == 0x45 || card == 0x70 || card == 0x9B || card == 0xC6 || card == 0xF1) {
                win->cardText->setString(win->cardText, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), card);
                win->apLabel->setVisible(win->apLabel, 0);
                win->ap->setVisible(win->ap, 0);
                win->hpLabel->setVisible(win->hpLabel, 0);
                win->hp->setVisible(win->hp, 0);
            } else {
                win->cardText->setVisible(win->cardText, 0);
                win->apLabel->setString(win->apLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x11);
                win->ap->setNumber(win->ap, 0, drawer.card->ap);
                win->ap->setRightAlign(win->ap, 1);
                win->hpLabel->setString(win->hpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x12);
                win->hp->setNumber(win->hp, 0, drawer.card->hp);
                win->hp->setRightAlign(win->hp, 1);
            }
        }
    } else {
        win->cardName->setVisible(win->cardName, 0);
        win->cardCountLabel->setVisible(win->cardCountLabel, 0);
        win->cardCount->setVisible(win->cardCount, 0);
        win->pointsLabel->setVisible(win->pointsLabel, 0);
        win->points->setVisible(win->points, 0);
        win->cardText->setVisible(win->cardText, 0);
        win->apLabel->setVisible(win->apLabel, 0);
        win->ap->setVisible(win->ap, 0);
        win->hpLabel->setVisible(win->hpLabel, 0);
        win->hp->setVisible(win->hp, 0);
    }
}

/* Draws the screen: the page's packs and the arrows, the pack under the
   cursor, the help's frame and the card under the cursor */
void STCRDSHP_drawPackOpen(CardPackOpen *open) {
    SpriteDrawer sprite;
    CardDrawer drawer;
    s32 i;
    s32 index;
    s32 pack;
    s32 card;
    s32 kind;
    s32 frame;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(open->layer, open->depth);
    if (open->fades[0].level != 0) {
        if (open->fades[0].level != ONE) {
            sprite.setScale(ONE, open->fades[0].level, ONE);
            sprite.setPivot(0xA0, 0x5C);
        } else {
            for (i = 0; i < 8; i++) {
                index = open->page * 8 + i;
                pack = open->packs[index];
                if (index >= open->packCount || pack <= 0) {
                    break;
                }
                sprite.setTexture(0x140, 0);
                sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(pack), (i % 2) * 0x83 + 0x28, (i / 2) * 0xE + 0x39);
                sprite.setTexture(0x280, 0);
                sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x31, (i % 2) * 0x83 + 0x28, (i / 2) * 0xE + 0x39);
            }
            sprite.setTexture(0x280, 0);
            if (open->pages >= 2) {
                if (GFX.funcs.getTime() - open->arrowTime >= 11) {
                    if (++open->arrowFrame >= 4) {
                        open->arrowFrame = 0;
                    }
                }
                sprite.setClutRow(open->arrowFrame);
                if (open->page > 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x48, 0x1E, 0x74);
                }
                if (open->page < open->pages - 1) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x47, 0xFD, 0x74);
                }
                sprite.setClutRow(0);
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2B, 0, 0x32);
    }
    if (open->fades[1].level != 0) {
        if (open->fades[1].level != ONE) {
            sprite.setScale(open->fades[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x8F);
        } else {
            sprite.setTexture(0x140, 0);
            pack = open->packs[open->cursor];
            sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), ITEM_FUNCS.getCategory(pack), 0x80, 0x8A);
        }
        sprite.setTexture(0x280, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2E, 0x79, 0x83);
    }
    if (open->fades[2].level != 0) {
        sprite.setScale(ONE, open->fades[2].level, ONE);
        if (open->fades[2].level != ONE) {
            sprite.setPivot(0xA0, 0xCF);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x2A, 0, 0xBD);
    }
    if (open->fades[3].level != 0) {
        card = open->cards[open->card];
        initCardDrawer(&drawer);
        drawer.setCard(card);
        sprite.setScale(open->fades[3].level, ONE, ONE);
        if (open->fades[3].level != ONE) {
            sprite.setPivot(0x140, 0x87);
        }
        kind = drawer.getKind();
        if (kind == 1) {
            frame = 0x12;
        } else if (kind == 2) {
            frame = 0x13;
        } else {
            frame = drawer.card->color + 0x13;
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), frame, 0x103, 0x7E);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xD, 0xFC, 0x7C);
        if (open->fades[3].level != ONE) {
            sprite.setPivot(0x140, 0x87);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xA, 0x82, 0x7C);
        if (open->fades[3].level != ONE) {
            sprite.setPivot(0x140, 0xAF);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x10, 0x103, 0xA4);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xD, 0xFC, 0xA2);
        if (open->fades[3].level != ONE) {
            sprite.setPivot(0x140, 0xA5);
        }
        if (kind != 0) {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xB, 0x4A, 0x92);
        } else if (card == 0x45 || card == 0x70 || card == 0x9B || card == 0xC6 || card == 0xF1) {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xB, 0x4A, 0x92);
        } else {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0xC, 0xC7, 0x92);
        }
        if (open->glowShown != 0) {
            if (GFX.funcs.getTime() - open->glowTime >= 5) {
                open->glowTime = GFX.funcs.getTime();
                if (++open->glowFrame >= 6) {
                    open->glowFrame = 0;
                }
            }
            sprite.setLayerId(open->layer, open->depth - 2);
            sprite.setClutRow(STCRDSHP_glowRows[open->glowFrame]);
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 7, open->card * 0x2A + 0x24, 0x44);
        }
    }
}

/* Lists the bag's card packs, and how many pages they fill */
void STCRDSHP_listPacks(CardPackOpen *open) {
    s32 count;
    s32 i;

    count = ITEM_FUNCS.list(1, open->items);
    open->packCount = 0;
    for (i = 0; i < count; i++) {
        if (ITEM_FUNCS.getCategory(open->items[i]) == CARD_PACK_CATEGORY) {
            open->packs[open->packCount++] = open->items[i];
        }
    }
    if (open->packCount != 0) {
        open->pages = open->packCount / 8 + (open->packCount % 8 != 0);
    }
}

/* The cursor on the page's packs (two columns of four): L1/R1 turn the page,
   the pad moves it; cross opens the pack, triangle leaves */
static inline void STCRDSHP_choosePack(CardPackOpen *open, CardPackOpenWindows *win) {
    s32 cursor;
    s32 first;
    s32 last;

    first = open->page;
    if (!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) {
        open->page--;
        if (open->page < 0) {
            open->page = 0;
        }
    } else if (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) {
        open->page++;
        if (open->page > open->pages - 1) {
            open->page = open->pages - 1;
        }
    }
    if (first != open->page) {
        SOUND.playSound(SOUND_CURSOR);
        open->cursor = open->page * 8;
        win->cursor->setPos(win->cursor, (open->cursor % 2) * 0x83 + 0x1D, (open->cursor % 8) / 2 * 0xE + 0x39);
        STCRDSHP_showPackPage(open, win, 1);
        STCRDSHP_showPack(open, win, 1);
        return;
    }
    last = (first + 1) * 8 - 1;
    cursor = open->cursor;
    first *= 8;
    if (last > open->packCount - 1) {
        last = open->packCount - 1;
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        open->cursor -= 2;
        if (open->cursor < first) {
            open->cursor = first;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        open->cursor += 2;
        if (open->cursor > last) {
            open->cursor = last;
        }
    }
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        open->cursor--;
        if (open->cursor < first) {
            open->cursor = first;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        open->cursor++;
        if (open->cursor > last) {
            open->cursor = last;
        }
    }
    if (cursor != open->cursor) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, (open->cursor % 2) * 0x83 + 0x1D, (open->cursor % 8) / 2 * 0xE + 0x39);
        STCRDSHP_showPack(open, win, 1);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        open->substate = 10;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        open->substate = 50;
        open->step = 0;
    }
}

/* Hides the packs' page and the pack under the cursor and closes their panels;
   the help says the pack is opened, triangle to go back */
static inline void STCRDSHP_closePackList(CardPackOpen *open, CardPackOpenWindows *win) {
    STCRDSHP_funcs.startFade(&open->fades[0], 0);
    STCRDSHP_showPackPage(open, win, 0);
    STCRDSHP_funcs.startFade(&open->fades[1], 0);
    STCRDSHP_showPack(open, win, 0);
    win->cursor->setVisible(win->cursor, 0);
    win->help[0]->setString(win->help[0], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x10);
    win->help[1]->setString(win->help[1], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 4);
    open->substate++;
}

/* Once the panels have closed (and the last pack's grid has gone): draws the
   pack's six cards, one of 16 at random for each slot, adds them to the
   player's and takes the pack from the bag, then lays them out */
static inline void STCRDSHP_openPack(CardPackOpen *open, CardPackOpenWindows *win) {
    s32 pack;
    s32 index;
    s32 i;
    s32 slot;

    STCRDSHP_funcs.updateFade(&open->fades[0]);
    if (STCRDSHP_funcs.updateFade(&open->fades[1])) {
        pack = open->packs[open->cursor];
        if (win->grid != NULL) {
            win->grid->state = TASK_KILL;
            return;
        }
        index = 0;
        for (i = 0; STCRDSHP_packs[i].item != 0; i++) {
            if (STCRDSHP_packs[i].item == pack) {
                index = i;
            }
        }
        for (i = 0; i < 6; i++) {
            slot = RANDOM.next() % 16;
            open->cards[i] = STCRDSHP_packs[index].slots[i][slot];
            GAME.funcs.addCards(open->cards[i], 1);
        }
        GAME.items[pack]--;
        win->grid = STCRDSHP_createGrid(open->shop, open->cards);
        open->card = 0;
        STCRDSHP_funcs.startFade(&open->fades[3], 1);
        open->substate++;
    }
}

/* The cursor on the cards drawn: left/right show each one; triangle closes
   them */
static inline void STCRDSHP_choosePackCard(CardPackOpen *open, CardPackOpenWindows *win) {
    s32 old;

    old = open->card;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        open->card--;
        if (open->card < 0) {
            open->card = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        open->card++;
        if (open->card >= 6) {
            open->card = 5;
        }
    }
    if (old != open->card) {
        SOUND.playSound(SOUND_MENU_MOVE);
        STCRDSHP_showPackCard(open, win, 1);
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        open->substate = 50;
        open->glowShown = 0;
        open->step = 1;
    }
}

/* Leaving, once the help's panel has closed: hides the packs' page and the
   pack under the cursor and closes their panels */
static inline void STCRDSHP_closePackOpen(CardPackOpen *open, CardPackOpenWindows *win) {
    if (STCRDSHP_funcs.updateFade(&open->fades[2])) {
        STCRDSHP_funcs.startFade(&open->fades[0], 0);
        STCRDSHP_showPackPage(open, win, 0);
        STCRDSHP_funcs.startFade(&open->fades[1], 0);
        STCRDSHP_showPack(open, win, 0);
        win->cursor->setVisible(win->cursor, 0);
        open->substate++;
    }
}

/* The states of the screen that opens a pack: its fades, the page and
   cursor, and the grid of cards a pack gives */
void STCRDSHP_runPackOpen(CardPackOpen *open, CardPackOpenWindows *win) {
    s32 i;
    s32 end;
    s32 top;

    switch (open->substate) {
    case 0:
    default:
        STCRDSHP_funcs.startFade(&open->fades[0], 1);
        open->substate++;
        break;
    case 1:
        if (STCRDSHP_funcs.updateFade(&open->fades[0])) {
            STCRDSHP_showPackPage(open, win, 1);
            STCRDSHP_funcs.startFade(&open->fades[1], 1);
            STCRDSHP_funcs.startFade(&open->fades[2], 1);
            open->substate++;
        }
        break;
    case 2:
        STCRDSHP_funcs.updateFade(&open->fades[2]);
        if (STCRDSHP_funcs.updateFade(&open->fades[1])) {
            STCRDSHP_showPack(open, win, 1);
            win->cursor->setPos(win->cursor, (open->cursor % 2) * 0x83 + 0x1D, (open->cursor % 8) / 2 * 0xE + 0x39);
            win->cursor->setVisible(win->cursor, 1);
            open->substate++;
        }
        break;
    case 3:
        open->substate++;
        break;
    case 4:
        STCRDSHP_choosePack(open, win);
        break;
    case 10:
        STCRDSHP_closePackList(open, win);
        break;
    case 11:
        STCRDSHP_openPack(open, win);
        break;
    case 12:
        if (STCRDSHP_funcs.updateFade(&open->fades[3])) {
            STCRDSHP_showPackCard(open, win, 1);
            if (win->grid->state == TASK_RUN) {
                open->glowShown = 1;
                open->substate++;
            }
        }
        break;
    case 13:
        STCRDSHP_choosePackCard(open, win);
        break;
    case 50:
        STCRDSHP_funcs.startFade(&open->fades[2], 0);
        win->help[0]->setVisible(win->help[0], 0);
        win->help[1]->setVisible(win->help[1], 0);
        if (open->step == 0) {
            open->substate = 55;
        } else {
            open->substate++;
        }
        break;
    case 51:
        if (STCRDSHP_funcs.updateFade(&open->fades[2])) {
            STCRDSHP_funcs.startFade(&open->fades[3], 0);
            STCRDSHP_showPackCard(open, win, 0);
            win->grid->hide(win->grid);
            open->substate++;
        }
        break;
    case 52:
        if (STCRDSHP_funcs.updateFade(&open->fades[3]) || win->grid == NULL) {
            /* The match depends on the page's rows: the last as
               (page + 1) * 8 - 1, as STCRDSHP_choosePack writes it, which
               shifts the page twice, and the first in a variable of its
               own. */
            open->substate = 0;
            end = (open->page + 1) * 8 - 1;
            i = open->packs[open->cursor];
            top = open->page * 8;
            STCRDSHP_listPacks(open);
            if (end > open->packCount - 1) {
                end = open->packCount - 1;
            }
            if (GAME.items[i] <= 0) {
                i = open->packs[open->cursor];
                if (GAME.items[i] <= 0) {
                    open->cursor--;
                    if (open->cursor < top) {
                        open->page--;
                        if (open->page >= 0) {
                            open->cursor = end;
                        } else {
                            open->state = TASK_KILL;
                        }
                    }
                }
            }
        }
        break;
    case 55:
        STCRDSHP_closePackOpen(open, win);
        break;
    case 56:
        STCRDSHP_funcs.updateFade(&open->fades[0]);
        if (STCRDSHP_funcs.updateFade(&open->fades[1])) {
            open->substate = 60;
        }
        break;
    case 60:
        open->state = TASK_KILL;
        break;
    }
}

/* The update of the screen to open a pack */
void STCRDSHP_updatePackOpen(CardPackOpen *open, CardPackOpenWindows *win) {
    switch (open->state) {
    case TASK_INIT:
    default:
        open->nextState(open);
        STCRDSHP_createPackOpenWindows(open, win);
        open->fades[3].duration = 10;
        open->fades[2].duration = 10;
        open->fades[0].duration = 10;
        open->fades[1].duration = 10;
        STCRDSHP_listPacks(open);
        break;
    case TASK_RUN:
        STCRDSHP_runPackOpen(open, win);
        STCRDSHP_drawPackOpen(open);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the screen to open a pack */
CardPackOpen *STCRDSHP_createPackOpen(CardShop *shop) {
    CardPackOpen *open = createTask(STCRDSHP_updatePackOpen, sizeof(CardPackOpen), sizeof(CardPackOpenWindows));

    open->layer = SCREEN_LAYER;
    open->depth = 6;
    open->shop = shop;
    return open;
}

/* The cards each slot of a pack draws one of, 16 each (STCRDSHP_packs) */
s32 STCRDSHP_slotCards[123][16] = {
    { 101, 101, 101, 138, 139, 139, 139, 139,
      184, 184, 184, 184, 225, 225, 225, 225 },
    { 57, 57, 57, 58, 58, 58, 186, 186,
      230, 230, 311, 311, 311, 312, 312, 312 },
    { 99, 99, 103, 103, 144, 144, 186, 186,
      230, 230, 232, 232, 271, 271, 272, 272 },
    { 99, 103, 144, 144, 146, 146, 189, 189,
      232, 232, 271, 271, 272, 272, 275, 275 },
    { 144, 144, 144, 146, 146, 189, 189, 232,
      232, 271, 271, 272, 272, 272, 275, 275 },
    { 146, 146, 146, 146, 189, 189, 189, 189,
      232, 232, 232, 232, 275, 275, 275, 275 },
    { 100, 100, 100, 141, 141, 141, 182, 182,
      182, 227, 227, 227, 228, 228, 228, 266 },
    { 59, 59, 100, 142, 142, 142, 187, 187,
      187, 187, 268, 270, 313, 313, 313, 313 },
    { 59, 59, 142, 142, 187, 187, 228, 268,
      268, 268, 270, 270, 270, 313, 313, 313 },
    { 53, 53, 53, 53, 231, 231, 231, 231,
      273, 273, 273, 273, 314, 314, 314, 314 },
    { 102, 102, 143, 143, 145, 145, 188, 188,
      231, 231, 273, 273, 274, 274, 314, 314 },
    { 102, 102, 102, 143, 143, 143, 145, 145,
      145, 145, 188, 188, 188, 274, 274, 274 },
    { 96, 96, 96, 140, 140, 140, 185, 185,
      226, 226, 226, 229, 229, 229, 254, 295 },
    { 60, 60, 60, 136, 136, 136, 178, 178,
      178, 185, 264, 264, 264, 265, 265, 265 },
    { 97, 97, 97, 97, 97, 136, 136, 136,
      178, 178, 178, 229, 264, 264, 265, 265 },
    { 55, 55, 93, 93, 93, 95, 95, 95,
      95, 181, 181, 181, 223, 223, 223, 223 },
    { 55, 55, 98, 98, 98, 98, 138, 138,
      138, 224, 224, 224, 269, 269, 269, 269 },
    { 93, 93, 95, 95, 98, 98, 138, 138,
      181, 181, 223, 223, 224, 224, 269, 269 },
    { 47, 47, 47, 83, 83, 83, 91, 91,
      91, 129, 129, 129, 220, 220, 220, 220 },
    { 24, 24, 24, 24, 24, 56, 56, 56,
      130, 130, 130, 130, 220, 299, 299, 299 },
    { 56, 87, 87, 87, 87, 130, 130, 130,
      173, 173, 173, 259, 259, 259, 259, 299 },
    { 24, 24, 51, 51, 51, 52, 52, 52,
      87, 137, 137, 137, 173, 259, 300, 300 },
    { 94, 94, 94, 137, 137, 179, 179, 179,
      180, 180, 180, 222, 222, 222, 300, 300 },
    { 51, 51, 52, 52, 94, 94, 137, 137,
      179, 179, 180, 180, 222, 222, 300, 300 },
    { 44, 44, 81, 81, 81, 126, 126, 126,
      175, 175, 175, 175, 218, 218, 218, 218 },
    { 30, 30, 81, 85, 85, 85, 174, 174,
      174, 175, 216, 216, 216, 218, 257, 257 },
    { 30, 85, 85, 85, 85, 174, 174, 174,
      174, 216, 216, 216, 216, 257, 257, 257 },
    { 12, 12, 12, 12, 18, 18, 18, 18,
      54, 54, 54, 54, 267, 267, 267, 267 },
    { 12, 18, 54, 54, 134, 134, 134, 134,
      221, 221, 221, 221, 266, 266, 266, 266 },
    { 12, 18, 267, 267, 308, 308, 308, 308,
      309, 309, 309, 309, 310, 310, 310, 310 },
    { 10, 10, 10, 80, 80, 80, 80, 125,
      125, 125, 168, 168, 168, 210, 210, 210 },
    { 43, 43, 43, 43, 43, 50, 50, 50,
      50, 50, 125, 168, 212, 212, 212, 212 },
    { 84, 84, 84, 84, 84, 212, 215, 215,
      215, 215, 215, 258, 258, 258, 258, 258 },
    { 6, 6, 86, 86, 219, 219, 219, 263,
      263, 263, 306, 306, 306, 307, 307, 307 },
    { 6, 6, 6, 11, 11, 17, 17, 86,
      86, 86, 92, 92, 92, 219, 219, 219 },
    { 11, 11, 11, 17, 17, 17, 92, 92,
      92, 263, 263, 306, 306, 307, 307, 307 },
    { 48, 48, 49, 49, 49, 167, 167, 167,
      167, 167, 209, 209, 252, 252, 252, 252 },
    { 15, 15, 15, 42, 42, 128, 128, 128,
      128, 214, 214, 214, 214, 298, 298, 298 },
    { 82, 82, 128, 128, 128, 128, 214, 214,
      214, 214, 253, 253, 253, 298, 298, 298 },
    { 135, 135, 135, 170, 170, 170, 177, 177,
      177, 262, 262, 262, 303, 303, 305, 305 },
    { 23, 23, 23, 29, 29, 29, 170, 170,
      170, 170, 262, 262, 305, 305, 305, 305 },
    { 23, 23, 23, 29, 29, 29, 135, 135,
      177, 177, 177, 177, 303, 303, 303, 303 },
    { 16, 16, 16, 22, 22, 38, 38, 169,
      169, 169, 208, 208, 208, 208, 211, 211 },
    { 3, 3, 3, 3, 3, 16, 122, 122,
      122, 124, 124, 124, 169, 213, 213, 213 },
    { 46, 46, 46, 46, 46, 122, 122, 122,
      124, 124, 124, 208, 208, 213, 213, 213 },
    { 5, 5, 5, 5, 88, 88, 88, 254,
      254, 254, 295, 295, 295, 304, 304, 304 },
    { 5, 5, 5, 5, 90, 90, 90, 132,
      132, 132, 254, 254, 254, 304, 304, 304 },
    { 5, 5, 5, 5, 90, 90, 90, 132,
      132, 132, 261, 261, 261, 302, 302, 302 },
    { 21, 28, 41, 41, 41, 45, 45, 45,
      75, 75, 75, 75, 202, 202, 202, 202 },
    { 21, 27, 27, 28, 79, 79, 79, 79,
      164, 164, 164, 164, 294, 294, 294, 294 },
    { 27, 27, 121, 121, 121, 121, 171, 171,
      172, 172, 293, 293, 293, 293, 294, 294 },
    { 79, 164, 171, 171, 171, 171, 172, 172,
      176, 176, 176, 176, 217, 217, 217, 217 },
    { 89, 89, 89, 89, 121, 133, 133, 133,
      260, 260, 260, 293, 301, 301, 301, 301 },
    { 89, 89, 89, 133, 133, 133, 133, 176,
      176, 217, 217, 260, 260, 301, 301, 301 },
    { 9, 9, 9, 14, 14, 113, 113, 113,
      113, 158, 158, 158, 160, 160, 160, 160 },
    { 4, 4, 4, 4, 113, 160, 249, 249,
      249, 249, 249, 287, 287, 287, 287, 287 },
    { 74, 74, 74, 74, 78, 78, 165, 165,
      203, 203, 203, 203, 206, 250, 250, 250 },
    { 78, 78, 78, 78, 127, 127, 127, 255,
      255, 255, 296, 296, 296, 297, 297, 297 },
    { 131, 131, 131, 206, 206, 206, 206, 255,
      255, 255, 256, 256, 256, 296, 296, 296 },
    { 127, 127, 127, 131, 131, 131, 165, 165,
      165, 165, 256, 256, 256, 297, 297, 297 },
    { 26, 26, 67, 67, 110, 110, 110, 110,
      115, 115, 115, 115, 157, 157, 157, 157 },
    { 2, 2, 2, 36, 36, 36, 36, 73,
      73, 73, 152, 152, 204, 204, 204, 204 },
    { 154, 154, 154, 154, 161, 161, 161, 161,
      161, 161, 205, 205, 205, 205, 205, 205 },
    { 2, 77, 77, 77, 120, 120, 120, 207,
      207, 207, 251, 251, 251, 290, 290, 290 },
    { 120, 120, 120, 123, 123, 123, 154, 166,
      166, 166, 207, 207, 207, 290, 290, 290 },
    { 77, 77, 77, 77, 123, 123, 123, 123,
      166, 166, 166, 166, 251, 251, 251, 251 },
    { 8, 72, 72, 72, 72, 116, 116, 116,
      116, 116, 199, 199, 201, 201, 201, 201 },
    { 20, 20, 20, 71, 71, 111, 111, 111,
      111, 111, 153, 153, 284, 284, 284, 284 },
    { 37, 37, 37, 111, 111, 111, 111, 111,
      153, 153, 153, 153, 153, 284, 284, 284 },
    { 76, 76, 117, 162, 162, 162, 162, 247,
      247, 247, 247, 286, 286, 286, 286, 289 },
    { 118, 118, 118, 118, 162, 162, 163, 163,
      163, 163, 247, 286, 291, 291, 291, 291 },
    { 76, 76, 76, 76, 117, 117, 117, 117,
      118, 118, 163, 289, 289, 289, 289, 291 },
    { 40, 40, 40, 148, 148, 148, 148, 191,
      191, 191, 191, 193, 193, 281, 281, 281 },
    { 70, 70, 70, 148, 148, 156, 156, 156,
      191, 191, 200, 200, 200, 285, 285, 285 },
    { 109, 109, 109, 156, 156, 156, 156, 280,
      280, 282, 282, 282, 285, 285, 285, 285 },
    { 70, 114, 114, 114, 114, 114, 242, 242,
      242, 242, 248, 248, 248, 248, 248, 248 },
    { 159, 159, 159, 159, 159, 200, 243, 243,
      243, 243, 246, 246, 246, 246, 246, 246 },
    { 114, 114, 119, 119, 119, 119, 119, 159,
      159, 246, 248, 288, 288, 288, 288, 288 },
    { 63, 63, 63, 64, 64, 105, 105, 105,
      105, 105, 149, 149, 149, 149, 149, 192 },
    { 39, 39, 106, 106, 106, 106, 150, 150,
      150, 194, 194, 194, 196, 196, 196, 196 },
    { 66, 66, 66, 106, 106, 106, 196, 196,
      196, 279, 279, 279, 283, 283, 283, 283 },
    { 239, 239, 239, 240, 240, 240, 244, 244,
      244, 244, 245, 245, 245, 292, 292, 292 },
    { 68, 68, 68, 108, 108, 108, 244, 244,
      244, 245, 245, 245, 245, 292, 292, 292 },
    { 244, 244, 244, 244, 245, 245, 245, 245,
      283, 283, 292, 292, 292, 292, 292, 292 },
    { 61, 62, 62, 62, 62, 62, 104, 104,
      104, 104, 104, 147, 147, 190, 190, 233 },
    { 65, 65, 65, 65, 276, 276, 276, 277,
      277, 277, 278, 278, 278, 278, 278, 278 },
    { 234, 234, 234, 234, 235, 235, 235, 236,
      236, 236, 236, 236, 237, 237, 238, 238 },
    { 107, 107, 107, 107, 151, 151, 151, 151,
      195, 195, 195, 195, 197, 197, 197, 197 },
    { 151, 151, 151, 195, 195, 195, 237, 237,
      237, 237, 237, 237, 237, 278, 278, 278 },
    { 107, 107, 107, 197, 197, 197, 238, 238,
      238, 238, 238, 238, 238, 278, 278, 278 },
    { 100, 100, 100, 136, 139, 139, 140, 140,
      140, 225, 225, 225, 228, 228, 228, 264 },
    { 100, 100, 101, 101, 141, 141, 146, 146,
      189, 189, 227, 227, 228, 228, 275, 275 },
    { 57, 57, 59, 97, 97, 103, 103, 142,
      142, 186, 186, 268, 268, 311, 311, 312 },
    { 59, 59, 103, 103, 145, 145, 145, 146,
      146, 189, 189, 268, 268, 268, 312, 312 },
    { 102, 102, 138, 143, 143, 145, 145, 146,
      146, 188, 188, 189, 189, 274, 274, 275 },
    { 102, 102, 143, 143, 145, 145, 146, 146,
      188, 188, 189, 189, 274, 274, 275, 275 },
    { 43, 43, 44, 56, 56, 84, 84, 125,
      125, 126, 259, 259, 270, 270, 270, 270 },
    { 30, 30, 30, 56, 56, 83, 91, 91,
      129, 129, 130, 130, 130, 258, 258, 258 },
    { 12, 12, 43, 84, 137, 137, 137, 137,
      144, 144, 259, 263, 263, 263, 263, 270 },
    { 11, 12, 12, 53, 53, 134, 137, 137,
      144, 144, 263, 271, 271, 307, 314, 314 },
    { 11, 11, 12, 12, 53, 134, 134, 137,
      137, 144, 263, 263, 271, 307, 307, 314 },
    { 11, 11, 12, 53, 53, 134, 137, 144,
      144, 263, 263, 271, 271, 307, 307, 314 },
    { 96, 96, 182, 182, 183, 183, 183, 184,
      184, 184, 185, 185, 226, 226, 229, 229 },
    { 24, 24, 93, 93, 93, 95, 95, 95,
      181, 181, 181, 183, 183, 184, 185, 229 },
    { 58, 60, 99, 99, 99, 178, 178, 178,
      187, 230, 230, 230, 265, 265, 265, 313 },
    { 24, 55, 58, 58, 60, 60, 187, 187,
      232, 232, 269, 269, 272, 272, 313, 313 },
    { 24, 55, 93, 93, 95, 95, 181, 181,
      223, 223, 232, 232, 269, 269, 272, 272 },
    { 24, 55, 93, 93, 95, 95, 181, 181,
      223, 223, 232, 232, 269, 269, 272, 272 },
    { 47, 47, 48, 168, 168, 175, 175, 183,
      183, 183, 210, 210, 218, 218, 220, 220 },
    { 18, 18, 47, 47, 48, 92, 92, 92,
      183, 183, 220, 220, 222, 222, 267, 267 },
    { 50, 50, 87, 87, 87, 173, 173, 173,
      212, 214, 214, 257, 298, 298, 299, 299 },
    { 50, 50, 87, 87, 173, 173, 179, 179,
      179, 222, 222, 224, 257, 257, 257, 267 },
    { 17, 17, 98, 98, 179, 179, 222, 222,
      224, 224, 231, 231, 267, 267, 273, 273 },
    { 17, 17, 18, 18, 23, 23, 29, 29,
      92, 92, 179, 179, 222, 222, 267, 267 },
    { 10, 10, 15, 15, 42, 42, 49, 49,
      128, 128, 209, 209, 252, 252, 253, 253 },
    { 80, 80, 81, 81, 82, 82, 85, 85,
      174, 174, 184, 184, 216, 216, 229, 229 },
    { 15, 42, 42, 42, 82, 82, 85, 85,
      128, 128, 174, 174, 216, 216, 253, 253 },
    { 6, 51, 52, 54, 135, 135, 135, 300,
      300, 300, 303, 303, 303, 310, 310, 310 },
    { 6, 6, 6, 52, 52, 52, 135, 135,
      300, 303, 306, 306, 309, 309, 310, 310 },
    { 51, 51, 52, 52, 54, 54, 300, 300,
      303, 303, 306, 306, 309, 309, 310, 310 },
    { 16, 16, 22, 22, 96, 96, 96, 160,
      169, 169, 182, 182, 182, 211, 226, 226 },
    { 3, 3, 96, 164, 164, 169, 169, 169,
      182, 208, 208, 226, 226, 226, 293, 294 },
    { 3, 3, 164, 164, 164, 208, 208, 208,
      213, 215, 293, 293, 293, 294, 294, 294 },
};
#if VERSION_US
/* 19 cards in the USA version, of which the slot only draws from the first 16 */
s32 STCRDSHP_slotCards123[19] = {
    86, 86, 86, 170, 170, 170, 213, 213,
    215, 215, 226, 226, 226, 295, 295, 295,
    304, 304, 304,
};
#elif VERSION_EU || VERSION_JP
s32 STCRDSHP_slotCards123[16] = {
    86, 86, 170, 170, 170, 213, 213, 215,
    215, 226, 226, 226, 295, 295, 304, 304,
};
#endif
s32 STCRDSHP_slotCards124[86][16] = {
    { 86, 86, 88, 88, 94, 94, 170, 170,
      177, 177, 180, 180, 219, 219, 221, 221 },
    { 88, 88, 94, 94, 177, 177, 180, 180,
      219, 219, 221, 221, 295, 295, 304, 304 },
    { 9, 9, 27, 27, 75, 75, 79, 79,
      79, 113, 113, 122, 122, 122, 249, 249 },
    { 38, 38, 45, 45, 46, 46, 46, 121,
      121, 124, 124, 124, 167, 167, 185, 185 },
    { 27, 27, 27, 46, 46, 46, 79, 79,
      121, 121, 122, 122, 124, 124, 249, 249 },
    { 5, 132, 254, 261, 262, 262, 262, 266,
      266, 266, 305, 305, 305, 308, 308, 308 },
    { 5, 5, 5, 132, 132, 132, 254, 254,
      254, 261, 261, 261, 262, 266, 305, 308 },
    { 5, 5, 132, 132, 133, 133, 254, 254,
      260, 260, 261, 261, 262, 262, 305, 305 },
    { 8, 71, 71, 71, 72, 72, 72, 110,
      115, 115, 115, 115, 116, 116, 116, 116 },
    { 2, 2, 20, 20, 20, 36, 36, 36,
      37, 37, 111, 111, 115, 116, 205, 205 },
    { 2, 2, 20, 20, 36, 36, 123, 123,
      127, 127, 131, 131, 205, 205, 255, 255 },
    { 77, 77, 77, 78, 120, 120, 120, 123,
      123, 123, 127, 131, 255, 290, 290, 290 },
    { 77, 77, 77, 78, 78, 78, 120, 120,
      120, 123, 127, 131, 255, 290, 290, 290 },
    { 77, 78, 78, 78, 120, 123, 127, 127,
      127, 131, 131, 131, 255, 255, 255, 290 },
    { 14, 21, 21, 21, 28, 28, 28, 41,
      41, 41, 158, 158, 158, 202, 202, 202 },
    { 21, 89, 89, 90, 90, 158, 176, 176,
      176, 202, 217, 217, 217, 301, 301, 301 },
    { 4, 4, 4, 74, 74, 74, 203, 203,
      203, 250, 250, 250, 287, 287, 301, 302 },
    { 4, 74, 89, 89, 89, 90, 90, 90,
      171, 171, 176, 176, 176, 217, 217, 250 },
    { 89, 89, 89, 90, 171, 172, 176, 176,
      176, 217, 217, 217, 301, 301, 301, 302 },
    { 89, 90, 90, 90, 171, 171, 171, 172,
      172, 172, 176, 217, 301, 302, 302, 302 },
    { 26, 26, 73, 73, 73, 73, 152, 152,
      157, 157, 157, 157, 201, 201, 201, 201 },
    { 73, 73, 166, 166, 166, 199, 256, 256,
      256, 256, 296, 296, 296, 297, 297, 297 },
    { 153, 153, 154, 154, 157, 161, 161, 161,
      204, 204, 204, 207, 207, 207, 284, 284 },
    { 165, 165, 166, 166, 166, 201, 206, 206,
      251, 251, 256, 256, 296, 296, 297, 297 },
    { 165, 165, 165, 166, 206, 206, 206, 207,
      207, 207, 251, 251, 251, 256, 296, 297 },
    { 165, 166, 166, 166, 206, 207, 251, 256,
      256, 256, 296, 296, 296, 297, 297, 297 },
    { 21, 21, 21, 21, 148, 148, 148, 158,
      158, 158, 158, 193, 202, 202, 202, 202 },
    { 21, 158, 159, 159, 159, 162, 162, 162,
      202, 202, 242, 242, 242, 248, 248, 248 },
    { 76, 76, 80, 80, 156, 156, 163, 163,
      200, 200, 209, 209, 252, 252, 280, 280 },
    { 76, 76, 80, 80, 159, 159, 162, 162,
      163, 163, 209, 209, 242, 248, 252, 252 },
    { 159, 159, 159, 159, 242, 242, 242, 242,
      248, 248, 248, 248, 286, 288, 288, 289 },
    { 76, 162, 163, 163, 286, 286, 286, 286,
      288, 288, 288, 288, 289, 289, 289, 289 },
    { 67, 70, 70, 70, 73, 73, 73, 109,
      109, 201, 201, 281, 281, 285, 285, 285 },
    { 40, 40, 41, 41, 81, 81, 81, 115,
      115, 115, 178, 178, 191, 282, 282, 282 },
    { 70, 70, 70, 73, 73, 73, 115, 115,
      115, 117, 118, 119, 285, 285, 285, 291 },
    { 114, 114, 117, 117, 118, 118, 119, 119,
      243, 243, 246, 246, 247, 247, 291, 291 },
    { 58, 58, 99, 99, 114, 114, 178, 178,
      243, 243, 246, 246, 246, 247, 247, 247 },
    { 58, 58, 99, 117, 117, 117, 118, 118,
      118, 119, 119, 119, 178, 291, 291, 291 },
    { 14, 28, 28, 28, 28, 28, 63, 63,
      63, 63, 64, 105, 105, 105, 105, 199 },
    { 28, 28, 63, 63, 66, 66, 66, 105,
      105, 108, 108, 194, 194, 194, 239, 239 },
    { 66, 66, 72, 72, 72, 106, 106, 110,
      110, 116, 116, 116, 194, 194, 279, 279 },
    { 2, 2, 4, 4, 4, 36, 36, 72,
      72, 72, 106, 110, 116, 116, 116, 279 },
    { 2, 4, 36, 74, 74, 74, 205, 244,
      244, 244, 250, 250, 250, 292, 292, 292 },
    { 2, 2, 2, 4, 4, 4, 36, 36,
      36, 74, 205, 205, 205, 244, 250, 292 },
    { 39, 147, 149, 149, 149, 149, 149, 149,
      157, 157, 157, 157, 157, 157, 192, 233 },
    { 10, 10, 10, 149, 150, 150, 152, 152,
      157, 157, 196, 196, 236, 236, 277, 277 },
    { 10, 10, 150, 152, 152, 187, 187, 187,
      196, 196, 230, 230, 230, 236, 277, 277 },
    { 68, 68, 151, 151, 197, 197, 237, 237,
      240, 240, 245, 245, 245, 283, 283, 283 },
    { 60, 68, 68, 68, 187, 230, 240, 240,
      240, 245, 245, 245, 283, 283, 283, 313 },
    { 60, 151, 151, 151, 187, 197, 197, 197,
      230, 237, 237, 237, 240, 240, 240, 313 },
    { 8, 26, 61, 62, 62, 62, 62, 62,
      62, 104, 104, 104, 104, 104, 104, 190 },
    { 9, 9, 49, 49, 49, 62, 65, 65,
      65, 104, 234, 234, 235, 235, 276, 276 },
    { 9, 9, 49, 49, 65, 65, 65, 71,
      71, 203, 234, 234, 234, 276, 276, 287 },
    { 107, 107, 111, 111, 111, 111, 195, 195,
      238, 238, 265, 265, 265, 265, 278, 278 },
    { 20, 20, 20, 20, 37, 37, 37, 37,
      107, 107, 195, 195, 238, 238, 278, 278 },
    { 20, 20, 37, 37, 111, 111, 111, 111,
      203, 203, 265, 265, 265, 265, 287, 287 },
    { 96, 96, 96, 96, 183, 183, 183, 185,
      185, 185, 227, 227, 227, 228, 228, 228 },
    { 101, 101, 101, 139, 139, 139, 139, 184,
      184, 184, 226, 226, 226, 229, 229, 229 },
    { 100, 100, 100, 140, 140, 140, 141, 141,
      141, 182, 182, 182, 182, 225, 225, 225 },
    { 100, 100, 100, 141, 141, 141, 182, 182,
      182, 183, 183, 183, 225, 225, 225, 225 },
    { 101, 101, 101, 139, 139, 139, 184, 184,
      184, 226, 226, 226, 226, 229, 229, 229 },
    { 96, 96, 96, 140, 140, 140, 185, 185,
      185, 227, 227, 227, 227, 228, 228, 228 },
    { 44, 44, 91, 91, 91, 91, 125, 125,
      125, 126, 126, 126, 126, 129, 129, 129 },
    { 10, 10, 10, 10, 80, 80, 80, 81,
      81, 81, 175, 175, 175, 218, 218, 218 },
    { 47, 47, 47, 47, 91, 91, 91, 126,
      126, 126, 168, 168, 168, 220, 220, 220 },
    { 10, 10, 10, 81, 81, 81, 83, 83,
      83, 210, 210, 210, 210, 218, 218, 218 },
    { 47, 47, 47, 125, 125, 125, 125, 129,
      129, 129, 168, 168, 168, 220, 220, 220 },
    { 80, 80, 80, 80, 83, 83, 83, 83,
      175, 175, 175, 175, 210, 210, 210, 210 },
    { 28, 28, 28, 28, 38, 38, 38, 38,
      45, 45, 45, 45, 49, 49, 49, 49 },
    { 16, 16, 16, 16, 16, 16, 41, 41,
      41, 41, 41, 41, 41, 41, 41, 41 },
    { 21, 21, 21, 21, 22, 22, 22, 22,
      22, 22, 48, 48, 48, 48, 48, 48 },
    { 75, 75, 75, 75, 75, 75, 75, 75,
      202, 202, 202, 202, 202, 202, 202, 202 },
    { 167, 167, 169, 169, 209, 209, 209, 209,
      209, 211, 211, 211, 211, 211, 252, 252 },
    { 167, 167, 167, 167, 169, 169, 169, 169,
      209, 209, 211, 211, 252, 252, 252, 252 },
    { 8, 8, 8, 8, 14, 14, 14, 14,
      14, 14, 26, 26, 26, 26, 26, 26 },
    { 67, 67, 67, 67, 72, 72, 113, 113,
      113, 113, 115, 115, 157, 157, 201, 201 },
    { 73, 73, 73, 110, 110, 116, 116, 116,
      152, 152, 158, 158, 158, 160, 160, 160 },
    { 71, 71, 72, 72, 72, 113, 113, 113,
      115, 115, 115, 199, 199, 201, 201, 201 },
    { 9, 9, 72, 72, 72, 110, 110, 113,
      113, 113, 115, 115, 115, 201, 201, 201 },
    { 9, 9, 73, 73, 73, 110, 110, 116,
      116, 116, 157, 157, 157, 158, 158, 158 },
    { 39, 39, 39, 40, 40, 40, 105, 105,
      105, 191, 191, 191, 281, 281, 281, 281 },
    { 61, 61, 61, 105, 105, 105, 149, 149,
      149, 192, 192, 192, 281, 281, 281, 281 },
    { 62, 62, 62, 104, 104, 104, 148, 148,
      148, 233, 233, 233, 281, 281, 281, 281 },
    { 63, 63, 63, 147, 147, 147, 149, 149,
      149, 190, 190, 190, 281, 281, 281, 281 },
    { 63, 63, 63, 64, 64, 64, 191, 191,
      191, 193, 193, 193, 281, 281, 281, 281 },
    { 40, 40, 40, 62, 62, 62, 104, 104,
      104, 148, 148, 148, 281, 281, 281, 281 },
};
CardPack STCRDSHP_packs[] = {
    { 91, { STCRDSHP_slotCards[0], STCRDSHP_slotCards[1], STCRDSHP_slotCards[2],
            STCRDSHP_slotCards[3], STCRDSHP_slotCards[4], STCRDSHP_slotCards[5] } },
    { 361, { STCRDSHP_slotCards[6], STCRDSHP_slotCards[7], STCRDSHP_slotCards[8],
            STCRDSHP_slotCards[9], STCRDSHP_slotCards[10], STCRDSHP_slotCards[11] } },
    { 362, { STCRDSHP_slotCards[12], STCRDSHP_slotCards[13], STCRDSHP_slotCards[14],
            STCRDSHP_slotCards[15], STCRDSHP_slotCards[16], STCRDSHP_slotCards[17] } },
    { 363, { STCRDSHP_slotCards[18], STCRDSHP_slotCards[19], STCRDSHP_slotCards[20],
            STCRDSHP_slotCards[21], STCRDSHP_slotCards[22], STCRDSHP_slotCards[23] } },
    { 364, { STCRDSHP_slotCards[24], STCRDSHP_slotCards[25], STCRDSHP_slotCards[26],
            STCRDSHP_slotCards[27], STCRDSHP_slotCards[28], STCRDSHP_slotCards[29] } },
    { 365, { STCRDSHP_slotCards[30], STCRDSHP_slotCards[31], STCRDSHP_slotCards[32],
            STCRDSHP_slotCards[33], STCRDSHP_slotCards[34], STCRDSHP_slotCards[35] } },
    { 366, { STCRDSHP_slotCards[36], STCRDSHP_slotCards[37], STCRDSHP_slotCards[38],
            STCRDSHP_slotCards[39], STCRDSHP_slotCards[40], STCRDSHP_slotCards[41] } },
    { 367, { STCRDSHP_slotCards[42], STCRDSHP_slotCards[43], STCRDSHP_slotCards[44],
            STCRDSHP_slotCards[45], STCRDSHP_slotCards[46], STCRDSHP_slotCards[47] } },
    { 368, { STCRDSHP_slotCards[48], STCRDSHP_slotCards[49], STCRDSHP_slotCards[50],
            STCRDSHP_slotCards[51], STCRDSHP_slotCards[52], STCRDSHP_slotCards[53] } },
    { 369, { STCRDSHP_slotCards[54], STCRDSHP_slotCards[55], STCRDSHP_slotCards[56],
            STCRDSHP_slotCards[57], STCRDSHP_slotCards[58], STCRDSHP_slotCards[59] } },
    { 370, { STCRDSHP_slotCards[60], STCRDSHP_slotCards[61], STCRDSHP_slotCards[62],
            STCRDSHP_slotCards[63], STCRDSHP_slotCards[64], STCRDSHP_slotCards[65] } },
    { 371, { STCRDSHP_slotCards[66], STCRDSHP_slotCards[67], STCRDSHP_slotCards[68],
            STCRDSHP_slotCards[69], STCRDSHP_slotCards[70], STCRDSHP_slotCards[71] } },
    { 372, { STCRDSHP_slotCards[72], STCRDSHP_slotCards[73], STCRDSHP_slotCards[74],
            STCRDSHP_slotCards[75], STCRDSHP_slotCards[76], STCRDSHP_slotCards[77] } },
    { 373, { STCRDSHP_slotCards[78], STCRDSHP_slotCards[79], STCRDSHP_slotCards[80],
            STCRDSHP_slotCards[81], STCRDSHP_slotCards[82], STCRDSHP_slotCards[83] } },
    { 374, { STCRDSHP_slotCards[84], STCRDSHP_slotCards[85], STCRDSHP_slotCards[86],
            STCRDSHP_slotCards[87], STCRDSHP_slotCards[88], STCRDSHP_slotCards[89] } },
    { 375, { STCRDSHP_slotCards[90], STCRDSHP_slotCards[91], STCRDSHP_slotCards[92],
            STCRDSHP_slotCards[93], STCRDSHP_slotCards[94], STCRDSHP_slotCards[95] } },
    { 376, { STCRDSHP_slotCards[96], STCRDSHP_slotCards[97], STCRDSHP_slotCards[98],
            STCRDSHP_slotCards[99], STCRDSHP_slotCards[100], STCRDSHP_slotCards[101] } },
    { 377, { STCRDSHP_slotCards[102], STCRDSHP_slotCards[103], STCRDSHP_slotCards[104],
            STCRDSHP_slotCards[105], STCRDSHP_slotCards[106], STCRDSHP_slotCards[107] } },
    { 378, { STCRDSHP_slotCards[108], STCRDSHP_slotCards[109], STCRDSHP_slotCards[110],
            STCRDSHP_slotCards[111], STCRDSHP_slotCards[112], STCRDSHP_slotCards[113] } },
    { 379, { STCRDSHP_slotCards[114], STCRDSHP_slotCards[115], STCRDSHP_slotCards[116],
            STCRDSHP_slotCards[117], STCRDSHP_slotCards[118], STCRDSHP_slotCards[119] } },
    { 380, { STCRDSHP_slotCards[120], STCRDSHP_slotCards[121], STCRDSHP_slotCards[122],
            STCRDSHP_slotCards123, STCRDSHP_slotCards124[0], STCRDSHP_slotCards124[1] } },
    { 381, { STCRDSHP_slotCards124[2], STCRDSHP_slotCards124[3], STCRDSHP_slotCards124[4],
            STCRDSHP_slotCards124[5], STCRDSHP_slotCards124[6], STCRDSHP_slotCards124[7] } },
    { 382, { STCRDSHP_slotCards124[8], STCRDSHP_slotCards124[9], STCRDSHP_slotCards124[10],
            STCRDSHP_slotCards124[11], STCRDSHP_slotCards124[12], STCRDSHP_slotCards124[13] } },
    { 383, { STCRDSHP_slotCards124[14], STCRDSHP_slotCards124[15], STCRDSHP_slotCards124[16],
            STCRDSHP_slotCards124[17], STCRDSHP_slotCards124[18], STCRDSHP_slotCards124[19] } },
    { 384, { STCRDSHP_slotCards124[20], STCRDSHP_slotCards124[21], STCRDSHP_slotCards124[22],
            STCRDSHP_slotCards124[23], STCRDSHP_slotCards124[24], STCRDSHP_slotCards124[25] } },
    { 385, { STCRDSHP_slotCards124[26], STCRDSHP_slotCards124[27], STCRDSHP_slotCards124[28],
            STCRDSHP_slotCards124[29], STCRDSHP_slotCards124[30], STCRDSHP_slotCards124[31] } },
    { 386, { STCRDSHP_slotCards124[32], STCRDSHP_slotCards124[33], STCRDSHP_slotCards124[34],
            STCRDSHP_slotCards124[35], STCRDSHP_slotCards124[36], STCRDSHP_slotCards124[37] } },
    { 387, { STCRDSHP_slotCards124[38], STCRDSHP_slotCards124[39], STCRDSHP_slotCards124[40],
            STCRDSHP_slotCards124[41], STCRDSHP_slotCards124[42], STCRDSHP_slotCards124[43] } },
    { 388, { STCRDSHP_slotCards124[44], STCRDSHP_slotCards124[45], STCRDSHP_slotCards124[46],
            STCRDSHP_slotCards124[47], STCRDSHP_slotCards124[48], STCRDSHP_slotCards124[49] } },
    { 389, { STCRDSHP_slotCards124[50], STCRDSHP_slotCards124[51], STCRDSHP_slotCards124[52],
            STCRDSHP_slotCards124[53], STCRDSHP_slotCards124[54], STCRDSHP_slotCards124[55] } },
    { 390, { STCRDSHP_slotCards124[56], STCRDSHP_slotCards124[57], STCRDSHP_slotCards124[58],
            STCRDSHP_slotCards124[59], STCRDSHP_slotCards124[60], STCRDSHP_slotCards124[61] } },
    { 391, { STCRDSHP_slotCards124[62], STCRDSHP_slotCards124[63], STCRDSHP_slotCards124[64],
            STCRDSHP_slotCards124[65], STCRDSHP_slotCards124[66], STCRDSHP_slotCards124[67] } },
    { 392, { STCRDSHP_slotCards124[68], STCRDSHP_slotCards124[69], STCRDSHP_slotCards124[70],
            STCRDSHP_slotCards124[71], STCRDSHP_slotCards124[72], STCRDSHP_slotCards124[73] } },
    { 393, { STCRDSHP_slotCards124[74], STCRDSHP_slotCards124[75], STCRDSHP_slotCards124[76],
            STCRDSHP_slotCards124[77], STCRDSHP_slotCards124[78], STCRDSHP_slotCards124[79] } },
    { 394, { STCRDSHP_slotCards124[80], STCRDSHP_slotCards124[81], STCRDSHP_slotCards124[82],
            STCRDSHP_slotCards124[83], STCRDSHP_slotCards124[84], STCRDSHP_slotCards124[85] } },
    { 0, { NULL } },
};
/* The CLUT rows of the glow around the card drawn last */
s32 STCRDSHP_glowRows[] = {
    0, 1, 2, 3,
    2, 1,
};
