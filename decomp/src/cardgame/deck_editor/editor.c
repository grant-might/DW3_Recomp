/* The deck editor: the list of the cards owned, and the deck being built; and
   an idle task nothing creates */

#include "cardgame/deck_editor.h"

/* Creates the editor's windows: the title, the deck's name and counts of each
   kind, the hint, the card under the cursor's name, numbers and text, the
   list's lines and cursor, the same for the list's card, and the message */
void STCRDDEK_createEditorWindows(DeckEditor *task, DeckEditorChildren *children) {
    s32 i;

    children->title = createTextWindow(task->layer, 1, 0xC1, 0x17);
    children->title->setDepth(children->title, task->depth - 1);
    children->deckName = createTextWindow(task->layer, 1, 0x15, 0x18);
    children->deckName->setDepth(children->deckName, task->depth - 1);
    for (i = 0; i < 6; i++) {
        children->kinds[i] = createTextWindow(task->layer, 1, 0x34 + i * 0x23, 0x29);
        children->kinds[i]->setDepth(children->kinds[i], task->depth - 1);
    }
    children->hint = createTextWindow(task->layer, 1, 0x96, 0xCC);
    children->cardName = createTextWindow(task->layer, 1, 0x96, 0xBD);
    children->pointsLabel = createTextWindow(task->layer, 1, 0x115, 0x26);
    children->points = createTextWindow(task->layer, 1, 0x126, 0x26);
    children->effect = createTextWindow(task->layer, 1, 0x50, 0x17);
    children->apLabel = createTextWindow(task->layer, 1, 0xCE, 0x17);
    children->ap = createTextWindow(task->layer, 1, 0xF0, 0x17);
    children->hpLabel = createTextWindow(task->layer, 1, 0xCE, 0x24);
    children->hp = createTextWindow(task->layer, 1, 0xF0, 0x24);
    for (i = 0; i < 8; i++) {
        children->lines[i].name = createTextWindow(task->layer, 1, 0xA3, 0x27 + i * 0xE);
        children->lines[i].name->setDepth(children->lines[i].name, task->depth - 1);
        children->lines[i].label = createTextWindow(task->layer, 1, 0x111, 0x27 + i * 0xE);
        children->lines[i].label->setDepth(children->lines[i].label, task->depth - 1);
        children->lines[i].count = createTextWindow(task->layer, 1, 0x120, 0x27 + i * 0xE);
        children->lines[i].count->setDepth(children->lines[i].count, task->depth - 1);
    }
    children->cursor = createCursor(task->layer, task->depth - 1, 0x89, 0x27);
    children->cursor->setVisible(children->cursor, 0);
    children->listCardName = createTextWindow(task->layer, 1, 0x88, 0xA1);
    children->listPointsLabel = createTextWindow(task->layer, 1, 0x115, 0xA1);
    children->listPoints = createTextWindow(task->layer, 1, 0x126, 0xA1);
    children->listEffect = createTextWindow(task->layer, 1, 0x50, 0xB8);
    children->listApLabel = createTextWindow(task->layer, 1, 0xCE, 0xB8);
    children->listAp = createTextWindow(task->layer, 1, 0xF0, 0xB8);
    children->listHpLabel = createTextWindow(task->layer, 1, 0xCE, 0xC5);
    children->listHp = createTextWindow(task->layer, 1, 0xF0, 0xC5);
    children->message = createTextWindow(task->layer, 1, 0x3E, 0x6B);
    children->message->setLines(children->message, 2);
}

/* Shows the title, the deck's name and counts of each kind, the hint (taking
   turns with another one) and the card under the cursor's name, with its
   numbers or its text while the info panel is open; or hides them */
void STCRDDEK_showEditorWindows(DeckEditor *task, DeckEditorChildren *children, s32 show) {
    CardDrawer card;
    s32 i;
    s32 id;

    if (show != 0) {
        children->title->setString(children->title, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 4);
        children->deckName->setString(children->deckName, GAME.decks[task->deck].name, -1);
        for (i = 0; i < 6; i++) {
            children->kinds[i]->setNumber(children->kinds[i], 0, task->screen->kinds[task->deck][i]);
            children->kinds[i]->setRightAlign(children->kinds[i], 1);
        }
        id = task->blinkTime < 60 ? task->infoShown + 0x1C : 0x1F;
        children->hint->setString(children->hint, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), id);
        id = GAME.decks[task->deck].cards[task->column + task->row * 9];
        children->cardName->setString(children->cardName, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), id);
        if (task->infoShown) {
            initCardDrawer(&card);
            card.setCard(id);
            if (card.getKind()) {
                children->pointsLabel->setVisible(children->pointsLabel, 0);
                children->points->setVisible(children->points, 0);
                children->effect->setString(children->effect, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), id);
                children->apLabel->setVisible(children->apLabel, 0);
                children->ap->setVisible(children->ap, 0);
                children->hpLabel->setVisible(children->hpLabel, 0);
                children->hp->setVisible(children->hp, 0);
            } else {
                children->pointsLabel->setString(children->pointsLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
                children->points->setNumber(children->points, 0, card.card->points);
                children->points->setRightAlign(children->points, 1);
                if (id == 0x45 || id == 0x70 || id == 0x9B || id == 0xC6 || id == 0xF1) {
                    children->effect->setString(children->effect, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), id);
                    children->apLabel->setVisible(children->apLabel, 0);
                    children->ap->setVisible(children->ap, 0);
                    children->hpLabel->setVisible(children->hpLabel, 0);
                    children->hp->setVisible(children->hp, 0);
                } else {
                    children->effect->setVisible(children->effect, 0);
                    children->apLabel->setString(children->apLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x11);
                    children->ap->setNumber(children->ap, 0, card.card->ap);
                    children->ap->setRightAlign(children->ap, 1);
                    children->hpLabel->setString(children->hpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x12);
                    children->hp->setNumber(children->hp, 0, card.card->hp);
                    children->hp->setRightAlign(children->hp, 1);
                }
            }
        } else {
            children->pointsLabel->setVisible(children->pointsLabel, 0);
            children->points->setVisible(children->points, 0);
            children->effect->setVisible(children->effect, 0);
            children->apLabel->setVisible(children->apLabel, 0);
            children->ap->setVisible(children->ap, 0);
            children->hpLabel->setVisible(children->hpLabel, 0);
            children->hp->setVisible(children->hp, 0);
        }
    } else {
        children->title->setVisible(children->title, 0);
        children->deckName->setVisible(children->deckName, 0);
        for (i = 0; i < 6; i++) {
            children->kinds[i]->setVisible(children->kinds[i], 0);
        }
        children->hint->setVisible(children->hint, 0);
        children->cardName->setVisible(children->cardName, 0);
        children->pointsLabel->setVisible(children->pointsLabel, 0);
        children->points->setVisible(children->points, 0);
        children->effect->setVisible(children->effect, 0);
        children->apLabel->setVisible(children->apLabel, 0);
        children->ap->setVisible(children->ap, 0);
        children->hpLabel->setVisible(children->hpLabel, 0);
        children->hp->setVisible(children->hp, 0);
    }
}

/* Shows the eight lines of the list of the cards owned from listTop (each card's
   name and how many are not in the deck), or hides them */
void STCRDDEK_showCardList(DeckEditor *task, DeckEditorChildren *children, s32 show) {
    s32 i;
    s32 id;

    if (show != 0) {
        for (i = 0; i < 8; i++) {
            id = task->list[task->listTop + i];
            if (id != 0) {
                children->lines[i].name->setString(children->lines[i].name, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), id);
                children->lines[i].label->setString(children->lines[i].label, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
                children->lines[i].count->setNumber(children->lines[i].count, 0, task->owned[id]);
                children->lines[i].count->setRightAlign(children->lines[i].count, 1);
            } else {
                children->lines[i].name->setVisible(children->lines[i].name, 0);
                children->lines[i].label->setVisible(children->lines[i].label, 0);
                children->lines[i].count->setVisible(children->lines[i].count, 0);
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            children->lines[i].name->setVisible(children->lines[i].name, 0);
            children->lines[i].label->setVisible(children->lines[i].label, 0);
            children->lines[i].count->setVisible(children->lines[i].count, 0);
        }
    }
}

/* Shows the list's card under the cursor: its name and its numbers or its text, or hides them */
void STCRDDEK_showListCard(DeckEditor *task, DeckEditorChildren *children, s32 show) {
    CardDrawer card;
    s32 id;

    if (show != 0) {
        id = task->list[task->listTop + task->listCursor];
        children->listCardName->setString(children->listCardName, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), id);
        initCardDrawer(&card);
        card.setCard(id);
        if (card.getKind()) {
            children->listPointsLabel->setVisible(children->listPointsLabel, 0);
            children->listPoints->setVisible(children->listPoints, 0);
            children->listEffect->setString(children->listEffect, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), id);
            children->listApLabel->setVisible(children->listApLabel, 0);
            children->listAp->setVisible(children->listAp, 0);
            children->listHpLabel->setVisible(children->listHpLabel, 0);
            children->listHp->setVisible(children->listHp, 0);
        } else {
            children->listPointsLabel->setString(children->listPointsLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 8);
            children->listPoints->setNumber(children->listPoints, 0, card.card->points);
            children->listPoints->setRightAlign(children->listPoints, 1);
            if (id == 0x45 || id == 0x70 || id == 0x9B || id == 0xC6 || id == 0xF1) {
                children->listEffect->setString(children->listEffect, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), id);
                children->listApLabel->setVisible(children->listApLabel, 0);
                children->listAp->setVisible(children->listAp, 0);
                children->listHpLabel->setVisible(children->listHpLabel, 0);
                children->listHp->setVisible(children->listHp, 0);
            } else {
                children->listEffect->setVisible(children->listEffect, 0);
                children->listApLabel->setString(children->listApLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x11);
                children->listAp->setNumber(children->listAp, 0, card.card->ap);
                children->listAp->setRightAlign(children->listAp, 1);
                children->listHpLabel->setString(children->listHpLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x12);
                children->listHp->setNumber(children->listHp, 0, card.card->hp);
                children->listHp->setRightAlign(children->listHp, 1);
            }
        }
    } else {
        children->listCardName->setVisible(children->listCardName, 0);
        children->listPointsLabel->setVisible(children->listPointsLabel, 0);
        children->listPoints->setVisible(children->listPoints, 0);
        children->listEffect->setVisible(children->listEffect, 0);
        children->listApLabel->setVisible(children->listApLabel, 0);
        children->listAp->setVisible(children->listAp, 0);
        children->listHpLabel->setVisible(children->listHpLabel, 0);
        children->listHp->setVisible(children->listHp, 0);
    }
}

/* Lists the cards owned but not in the deck: owned holds each card's copies
   left over, list the cards that have some (listCount of them) */
void STCRDDEK_buildCardList(DeckEditor *task) {
    s32 i;
    s32 n;
    s16 *cards;

    for (i = 0; i < 315; i++) {
        task->list[i] = 0;
        task->owned[i] = GAME.cards[i];
    }
    cards = GAME.decks[task->deck].cards;
    for (n = 0; n < 40; n++, cards++) {
        task->owned[*cards]--;
    }
    task->listCount = 0;
    for (i = 0, n = 0; i < 315; i++) {
        if (task->owned[i] > 0) {
            task->list[n++] = i;
            task->listCount++;
        }
    }
}

/* Draws the editor: the cursor on the deck's grid, the info panel, the deck's
   panel, the list of the cards owned with their icons and scroll arrows,
   the list card's panel and the message's panel, as they open */
void STCRDDEK_drawEditor(DeckEditor *task) {
    SpriteDrawer sprite;
    CardDrawer card;
    s32 i;
    s32 id;
    s32 kind;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(task->layer, task->depth - 2);
    if (task->cursorShown) {
        if (GFX.funcs.getTime() - task->cursorTime >= 5) {
            task->cursorTime = GFX.funcs.getTime();
            if (++task->cursorFrame >= 6) {
                task->cursorFrame = 0;
            }
        }
        sprite.setClutRow(STCRDDEK_cursorCluts[task->cursorFrame]);
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x43, (task->column << 5) | 0x10, (task->row << 5) + 0x3A);
        sprite.setClutRow(0);
    }
    if (task->panels[1].level != 0) {
        if (task->panels[1].level != ONE) {
            sprite.setScale(task->panels[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x25);
        }
        initCardDrawer(&card);
        id = GAME.decks[task->deck].cards[task->column + task->row * 9];
        card.setCard(id);
        kind = card.getKind();
        if (kind) {
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), kind + 0x11, 0x103, 0x24);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xD, 0xFC, 0x22);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xB, 0x4A, 0x12);
        } else {
            kind = card.card->color;
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), kind + 0x13, 0x103, 0x24);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xD, 0xFC, 0x22);
            if (id == 0x45 || id == 0x70 || id == 0x9B || id == 0xC6 || id == 0xF1) {
                sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xB, 0x4A, 0x12);
            } else {
                sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xC, 0xC7, 0x12);
            }
        }
    }
    if (task->panels[0].level != 0) {
        sprite.setScale(ONE, task->panels[0].level, ONE);
        if (task->panels[0].level != ONE) {
            sprite.setPivot(0xA0, 0x74);
        }
        sprite.setLayerId(task->layer, task->depth);
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x40, 0x10, 0x15);
    }
    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(task->layer, task->depth);
    if (task->panels[2].level != 0) {
        if (task->panels[2].level != ONE) {
            sprite.setScale(task->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x5C);
        } else {
            /* the match depends on i being set before initCardDrawer */
            i = 0;
            initCardDrawer(&card);
            for (; i < 8; i++) {
                id = task->list[task->listTop + i];
                if (id != 0) {
                    card.setCard(id);
                    if (card.getKind()) {
                        sprite.setClutRow(card.card->color - 1);
                        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x4B, 0x94, 0x27 + i * 14);
                    } else {
                        sprite.setClutRow(0);
                        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), card.card->color + 0x4B, 0x94, 0x27 + i * 14);
                    }
                    sprite.setClutRow(0);
                    sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x31, 0x94, 0x27 + i * 14);
                }
            }
            if (task->listCount >= 9) {
                if (GFX.funcs.getTime() - task->arrowTime >= 9) {
                    task->arrowTime = GFX.funcs.getTime();
                    task->arrowsShown = 1 - task->arrowsShown;
                }
                if (task->arrowsShown) {
                    if (task->listTop > 0) {
                        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x45, 0x126, 0x22);
                    }
                    if (task->listTop < task->listCount - 8) {
                        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x46, 0x126, 0x8F);
                    }
                }
            }
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x3F, 0x81, 0x1E);
    }
    if (task->panels[3].level != 0) {
        if (task->panels[3].level != ONE) {
            sprite.setScale(task->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0x25);
        }
        initCardDrawer(&card);
        id = task->list[task->listTop + task->listCursor];
        card.setCard(id);
        kind = card.getKind();
        if (kind) {
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), kind + 0x11, 0x103, 0x9F);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xD, 0xFC, 0x9D);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xB, 0x4A, 0xB3);
        } else {
            kind = card.card->color;
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), kind + 0x13, 0x103, 0x9F);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xD, 0xFC, 0x9D);
            if (id == 0x45 || id == 0x70 || id == 0x9B || id == 0xC6 || id == 0xF1) {
                sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xB, 0x4A, 0xB3);
            } else {
                sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xC, 0xC7, 0xB3);
            }
        }
        if (task->panels[3].level != ONE) {
            sprite.setPivot(0x140, 0xA8);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0xA, 0x82, 0x9D);
    }
    if (task->panels[4].level != 0) {
        if (task->panels[4].level != ONE) {
            sprite.setScale(ONE, task->panels[4].level, ONE);
            sprite.setPivot(0, 0x78);
        }
        sprite.setLayerId(task->layer, task->depth - 3);
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x36, 0, 0x64);
    }
}

/* The cursor on the deck's grid (9 columns, the last row 4): the pad moves it,
   R1 opens or closes the card's info panel, cross goes on to the list of the
   cards owned, triangle leaves and circle sorts the deck */
static inline void STCRDDEK_moveDeckCursor(DeckEditor *task, DeckEditorChildren *children) {
    s32 prev;

    prev = task->column + task->row * 9;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        if (--task->column < 0) {
            task->column = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        if (task->row == 4) {
            if (++task->column >= 4) {
                task->column = 3;
            }
        } else {
            if (++task->column >= 9) {
                task->column = 8;
            }
        }
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--task->row < 0) {
            task->row = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        task->row++;
        if (task->column >= 4) {
            if (task->row >= 4) {
                task->row = 3;
            }
        } else {
            if (task->row >= 5) {
                task->row = 4;
            }
        }
    }
    STCRDDEK_showEditorWindows(task, children, 1);
    if (prev != task->column + task->row * 9) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) {
        SOUND.playSound(SOUND_MENU_MOVE);
        task->substate = 10;
    } else if (PAD_PRESSED(PAD_CROSS)) {
        if (task->listCount != 0) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            task->substate = 20;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate = 50;
    } else if (PAD_PRESSED(PAD_CIRCLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate = 30;
    }
}

/* Opens or closes the info panel of the card under the cursor */
static inline void STCRDDEK_toggleCardInfo(DeckEditor *task, DeckEditorChildren *children) {
    task->cursorShown = 0;
    task->infoShown = 1 - task->infoShown;
    if (task->infoShown) {
        STCRDDEK_funcs.startFade(&task->panels[1], 1);
        task->substate = 11;
        task->step = 0;
    } else {
        STCRDDEK_showEditorWindows(task, children, 1);
        STCRDDEK_funcs.startFade(&task->panels[1], 0);
        task->substate = 11;
        task->step = 1;
    }
}

/* Leaves the deck's grid for the list of the cards owned: fills the list and
   keeps its cursor on a card, closes the grid's panels and opens the list's */
static inline void STCRDDEK_openCardList(DeckEditor *task, DeckEditorChildren *children) {
    children->cards->state = TASK_KILL;
    STCRDDEK_buildCardList(task);
    STCRDDEK_funcs.startFade(&task->panels[2], 1);
    while (task->list[task->listTop + task->listCursor] == 0) {
        if (--task->listTop < 0) {
            task->listTop = 0;
            if (--task->listCursor < 0) {
                task->listCursor = 0;
            }
        }
    }
    task->cursorShown = 0;
    STCRDDEK_funcs.startFade(&task->panels[0], 0);
    if (task->infoShown) {
        task->panels[1].level = 0;
        task->infoShown = 0;
    }
    STCRDDEK_showEditorWindows(task, children, 0);
    task->substate++;
}

/* Once the list card's panel has opened: adds a scroll bar to a list longer
   than a page and shows the cursor and the card under it */
static inline void STCRDDEK_showCardListCursor(DeckEditor *task, DeckEditorChildren *children) {
    if (STCRDDEK_funcs.updateFade(&task->panels[3])) {
        if (task->listCount >= 9 && children->scrollBar == NULL) {
            children->scrollBar = STCRDDEK_createScrollBar();
            children->scrollBar->setX(children->scrollBar, 0x125, 0xC);
            children->scrollBar->setRange(children->scrollBar, 0x2A, 0x8F);
            children->scrollBar->setCount(children->scrollBar, 8, task->listCount);
            children->scrollBar->setPos(children->scrollBar, task->listCursor);
        }
        children->cursor->setPos(children->cursor, 0x89, task->listCursor * 14 + 0x27);
        children->cursor->setVisible(children->cursor, 1);
        STCRDDEK_showListCard(task, children, 1);
        task->substate++;
    }
}

/* The cursor on the list of the cards owned: L1/R1 turn a page, up/down move
   it (scrolling the list); cross puts the card in the deck's slot unless the
   deck already has four of it (a message), triangle goes back to the grid */
static inline void STCRDDEK_chooseListCard(DeckEditor *task, DeckEditorChildren *children) {
    s32 prev;
#if VERSION_EU || VERSION_JP
    s32 prevTop;
#endif
    s32 id;
    s32 old;

    prev = task->listTop + task->listCursor;
#if VERSION_EU || VERSION_JP
    prevTop = task->listTop;
#endif
    if (task->listCount >= 9) {
        if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
            task->listTop -= 7;
            if (task->listTop < 0) {
                task->listTop = 0;
            }
        } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
            s32 k;

            for (k = 0; k < 7; k++) {
                if (++task->listTop > task->listCount - 8) {
                    task->listTop = task->listCount - 8;
                    break;
                }
            }
        }
    }
    if (prev == task->listTop + task->listCursor) {
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            if (--task->listCursor < 0) {
                task->listCursor = 0;
                if (--task->listTop < 0) {
                    task->listTop = 0;
                }
            }
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            if (task->listCount >= 9) {
                if (++task->listCursor >= 8) {
                    task->listCursor = 7;
                    if (++task->listTop > task->listCount - 8) {
                        task->listTop = task->listCount - 8;
                    }
                }
            } else {
                if (++task->listCursor > task->listCount - 1) {
                    task->listCursor = task->listCount - 1;
                }
            }
        }
    }
#if VERSION_EU || VERSION_JP
    if (children->scrollBar != NULL && prevTop != task->listTop) {
        children->scrollBar->setPos(children->scrollBar, task->listTop);
    }
#endif
    if (prev != task->listTop + task->listCursor) {
        SOUND.playSound(SOUND_CURSOR);
        children->cursor->setPos(children->cursor, 0x89, task->listCursor * 14 + 0x27);
        STCRDDEK_showCardList(task, children, 1);
        STCRDDEK_showListCard(task, children, 1);
#if VERSION_US
        if (children->scrollBar != NULL) {
            children->scrollBar->setPos(children->scrollBar, task->listTop + task->listCursor);
        }
#endif
    } else if (PAD_PRESSED(PAD_CROSS)) {
        id = task->list[task->listTop + task->listCursor];
        old = GAME.decks[task->deck].cards[task->column + task->row * 9];
        SOUND.playSound(SOUND_SELECT);
        if (id != old && GAME.cards[id] - task->owned[id] >= 4) {
            task->substate = 60;
        } else {
            GAME.decks[task->deck].cards[task->column + task->row * 9] = id;
            task->screen->countKinds(task->screen);
            task->substate++;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate++;
    }
}

/* Hides the list, its card and its cursor and closes their panels, and ends
   the scroll bar */
static inline void STCRDDEK_closeCardList(DeckEditor *task, DeckEditorChildren *children) {
    STCRDDEK_funcs.startFade(&task->panels[2], 0);
    STCRDDEK_showCardList(task, children, 0);
    STCRDDEK_funcs.startFade(&task->panels[3], 0);
    STCRDDEK_showListCard(task, children, 0);
    children->cursor->setVisible(children->cursor, 0);
    if (children->scrollBar != NULL) {
        children->scrollBar->state = TASK_KILL;
    }
    task->substate++;
}

/* Before sorting the deck: closes the grid's panels and hides its windows */
static inline void STCRDDEK_startSort(DeckEditor *task, DeckEditorChildren *children) {
    children->cards->state = TASK_KILL;
    STCRDDEK_buildCardList(task);
    task->counter = 0;
    task->substate++;
    STCRDDEK_funcs.startFade(&task->panels[0], 0);
    task->cursorShown = 0;
    STCRDDEK_funcs.startFade(&task->panels[0], 0);
    task->panels[1].level = 0;
    task->infoShown = 0;
    STCRDDEK_showEditorWindows(task, children, 0);
}

/* Sorts the deck's 40 cards by id, then shows the grid again */
static inline void STCRDDEK_sortDeck(DeckEditor *task, DeckEditorChildren *children) {
    s32 i;
    s32 j;
    s32 card;

    for (i = 0; i < 39; i++) {
        for (j = i + 1; j < 40; j++) {
            if (GAME.decks[task->deck].cards[i] > GAME.decks[task->deck].cards[j]) {
                card = GAME.decks[task->deck].cards[i];
                GAME.decks[task->deck].cards[i] = GAME.decks[task->deck].cards[j];
                GAME.decks[task->deck].cards[j] = card;
            }
        }
    }
    task->substate = 0;
}

/* Leaving: ends the deck's cards and closes the grid's panels and windows */
static inline void STCRDDEK_closeEditor(DeckEditor *task, DeckEditorChildren *children) {
    children->cards->state = TASK_KILL;
    task->cursorShown = 0;
    STCRDDEK_funcs.startFade(&task->panels[0], 0);
    if (task->infoShown) {
        task->panels[1].level = 0;
        task->infoShown = 0;
    }
    STCRDDEK_showEditorWindows(task, children, 0);
    task->substate++;
}

/* Once the grid's panel has opened: shows the deck's windows, fills the list
   of the cards owned and starts laying out the deck's cards */
static inline void STCRDDEK_openEditor(DeckEditor *task, DeckEditorChildren *children) {
    if (STCRDDEK_funcs.updateFade(&task->panels[0])) {
        task->blinkTime = 0;
        STCRDDEK_showEditorWindows(task, children, 1);
        STCRDDEK_buildCardList(task);
        children->cards = STCRDDEK_createDeckCards(task);
        task->substate++;
    }
}

/* The editor's states: picks a card of the deck (R1 its info panel, circle sorts
   the deck, triangle leaves), then a card of the list to take its place (L1/R1
   a page, triangle back), refusing with a message one the deck has 4 of */
void STCRDDEK_stepEditor(DeckEditor *task, DeckEditorChildren *children) {
    switch (task->substate) {
    case 0:
    default:
        STCRDDEK_funcs.startFade(&task->panels[0], 1);
        task->substate++;
        break;
    case 1:
        STCRDDEK_openEditor(task, children);
        break;
    case 2:
        if (children->cards->state == TASK_DONE) {
            task->cursorShown = 1;
            task->substate = 5;
        }
        break;
    case 5:
        STCRDDEK_moveDeckCursor(task, children);
        break;
    case 10:
        STCRDDEK_toggleCardInfo(task, children);
        break;
    case 11:
        if (STCRDDEK_funcs.updateFade(&task->panels[1])) {
            if (task->step == 0) {
                STCRDDEK_showEditorWindows(task, children, 1);
            }
            task->cursorShown = 1;
            task->setSubstate(task, 5);
        }
        break;
    case 20:
        STCRDDEK_openCardList(task, children);
        break;
    case 21:
        if (STCRDDEK_funcs.updateFade(&task->panels[0])) {
            task->substate++;
        }
        break;
    case 22:
        if (STCRDDEK_funcs.updateFade(&task->panels[2])) {
            STCRDDEK_showCardList(task, children, 1);
            STCRDDEK_funcs.startFade(&task->panels[3], 1);
            task->substate++;
        }
        break;
    case 23:
        STCRDDEK_showCardListCursor(task, children);
        break;
    case 24:
        STCRDDEK_chooseListCard(task, children);
        break;
    case 25:
        STCRDDEK_closeCardList(task, children);
        break;
    case 26:
        STCRDDEK_funcs.updateFade(&task->panels[2]);
        if (STCRDDEK_funcs.updateFade(&task->panels[3])) {
            task->listCursor = 0;
            task->listTop = 0;
            task->substate = 0;
        }
        break;
    case 30:
        STCRDDEK_startSort(task, children);
        break;
    case 31:
        if (STCRDDEK_funcs.updateFade(&task->panels[3])) {
            task->substate++;
        }
        break;
    case 32:
        STCRDDEK_sortDeck(task, children);
        break;
    case 50:
        STCRDDEK_closeEditor(task, children);
        break;
    case 51:
        if (STCRDDEK_funcs.updateFade(&task->panels[0])) {
            task->state = TASK_KILL;
        }
        break;
    case 60:
        children->cursor->setPalette(children->cursor, PALETTE_GREY);
        children->cursor->setStill(children->cursor, 1);
        STCRDDEK_funcs.startFade(&task->panels[4], 1);
        task->substate++;
        break;
    case 61:
        if (STCRDDEK_funcs.updateFade(&task->panels[4])) {
            children->message->setString(children->message, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x1E);
            task->substate++;
        }
        break;
    case 62:
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            children->message->setVisible(children->message, 0);
            STCRDDEK_funcs.startFade(&task->panels[4], 0);
            task->substate++;
        }
        break;
    case 63:
        if (STCRDDEK_funcs.updateFade(&task->panels[4])) {
            children->cursor->setPalette(children->cursor, PALETTE_WHITE);
            children->cursor->setStill(children->cursor, 0);
            task->substate = 23;
        }
        break;
    }
    task->blinkTime += GFX.funcs.getFrameTime();
    if (task->blinkTime > 120) {
        task->blinkTime -= 120;
    }
}

/* The editor's task: creates its windows, then runs and draws it */
void STCRDDEK_updateEditor(DeckEditor *task, DeckEditorChildren *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        STCRDDEK_createEditorWindows(task, children);
        task->panels[0].duration = 10;
        task->panels[1].duration = 10;
        task->panels[2].duration = 10;
        task->panels[3].duration = 10;
        task->panels[4].duration = 10;
        break;
    case TASK_RUN:
        STCRDDEK_stepEditor(task, children);
        STCRDDEK_drawEditor(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the editor (task) of one of GAME.decks */
DeckEditor *STCRDDEK_createEditor(DeckScreen *screen, s32 deck) {
    DeckEditor *task = createTask(STCRDDEK_updateEditor, sizeof(DeckEditor), sizeof(DeckEditorChildren));

    task->layer = SCREEN_LAYER;
    task->depth = 6;
    task->screen = screen;
    task->deck = deck;
    return task;
}

/* Left empty */
void STCRDDEK_initIdle(DeckIdle *task, void *children) {
}

/* Only sets up a sprite drawer */
void STCRDDEK_drawIdle(DeckIdle *task) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(task->layer, task->depth);
}

/* Ends the idle task on triangle */
void STCRDDEK_stepIdle(DeckIdle *task, void *children) {
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        task->state = TASK_KILL;
    }
}

/* A task that waits for triangle, then ends; nothing creates it */
void STCRDDEK_updateIdle(DeckIdle *task, void *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        STCRDDEK_initIdle(task, children);
        break;
    case TASK_RUN:
        STCRDDEK_stepIdle(task, children);
        STCRDDEK_drawIdle(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the idle task; nothing calls it */
DeckIdle *STCRDDEK_createIdle(void *parent) {
    DeckIdle *task = createTask(STCRDDEK_updateIdle, sizeof(DeckIdle), 0);

    task->layer = SCREEN_LAYER;
    task->depth = 6;
    task->parent = parent;
    return task;
}
