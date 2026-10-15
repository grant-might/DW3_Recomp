#include "cardgame/album.h"

/* The cursor's CLUT row on each frame of its blink */
s32 STCRDABM_cursorBlink[6] = {0, 1, 2, 3, 2, 1};

CardAlbumFuncs STCRDABM_funcs = {
    STCRDABM_loadFiles, STCRDABM_filesLoading, STCRDABM_startFade,
    STCRDABM_updateFade, STCRDABM_startLerp, STCRDABM_updateLerp,
};

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 0
#include "shared/create_fader.inc.c"

/* Loads the images of the grid's page of cards, from grid->first, into VRAM's 6x2 grid
   of card images */
void STCRDABM_loadIcons(CardAlbumGrid *grid) {
    CardDrawer icon;
    s32 card;
    s32 col;
    s32 row;

    initCardDrawer(&icon);
    icon.setImagePos(0x140, 0x100);
    icon.setClutPos(0x300, 0x100);
    card = grid->first;
    for (row = 0; row < 2; row++) {
        for (col = 0; col < 6 && card < CARD_COUNT; col++) {
            icon.setCard(card++);
            icon.setCell(col, row);
            icon.loadImage();
        }
    }
}

/* Turns the grid to the page whose first card is `first` (grid->setPage) */
void STCRDABM_setPage(CardAlbumGrid *grid, s32 first) {
    grid->prevFirst = grid->first;
    grid->first = first;
    grid->turned = 0;
    grid->frame = 0;
    grid->setState(grid, TASK_DONE);
}

#include "shared/hide_cards.inc.c"

/* Draws the page's cards, or the page being turned when previous != 0: a seen card's
   image, the sprite of its color and its AP and HP (sprite 0x1D instead for the other
   kinds), else an empty slot */
void STCRDABM_drawCards(CardAlbumGrid *grid, s32 previous) {
    SpriteDrawer sprite;
    CardDrawer icon;
    s32 digits[5];
    s32 i;
    s32 j;
    s32 card;
    s32 col;
    s32 row;
    s32 x;
    s32 y;
    s32 value;
    s32 dx;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(grid->layer, grid->depth);
    sprite.setTexture(0x280, 0);
    initCardDrawer(&icon);
    icon.setImagePos(0x140, 0x100);
    icon.setClutPos(0x300, 0x100);
    icon.setLayer(grid->layer, grid->depth);
    for (i = 0; i < grid->shown; i++) {
        if (previous != 0) {
            card = grid->prevFirst + i;
        } else {
            card = grid->first + i;
        }
        if (card >= CARD_COUNT) {
            break;
        }
        col = i % 6;
        row = i / 6;
        x = col * 42;
        y = row * 54;
        if (GAME.cardsSeen[card] != 0) {
            icon.setCard(card);
            icon.setCell(col, row);
            icon.draw(x + 0x27, y + 0x34);
            if (icon.getKind() != 0) {
                sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0x1D, x + 0x27, y + 0x53);
            } else {
                value = icon.card->ap;
                j = value / 10;
                if (j != 0) {
                    digits[0] = j + 0x1E;
                } else {
                    digits[0] = 0;
                }
                j = value % 10;
                digits[1] = j + 0x1E;
                digits[2] = 0x1C;
                for (j = 0, dx = 0x27; j < 3; j++, dx += 7) {
                    if (digits[j] != 0) {
                        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), digits[j], x + dx, y + 0x53);
                    }
                }
                value = icon.card->hp;
                j = value / 10;
                if (j != 0) {
                    digits[0] = j + 0x1E;
                } else {
                    digits[0] = 0;
                }
                j = value % 10;
                digits[1] = j + 0x1E;
                for (j = 0, dx = 0x3A; j < 2; j++, dx += 7) {
                    if (digits[j] != 0) {
                        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), digits[j], x + dx, y + 0x53);
                    }
                }
            }
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), icon.card->color - 1, x + 0x23, y + 0x32);
        } else {
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 6, x + 0x23, y + 0x32);
        }
    }
}

/* Draws the slots turned so far over the cards, flashing the ones with a seen card */
void STCRDABM_drawTurningSlots(CardAlbumGrid *grid) {
    SpriteDrawer sprite;
    s32 i;
    s32 card;
    s32 col;
    s32 row;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(grid->layer, grid->depth - 1);
    sprite.setTexture(0x280, 0);
    for (i = 0; i < grid->turned; i++) {
        card = grid->first + i;
        col = i % 6;
        row = i / 6;
        if (GAME.cardsSeen[card] != 0 || card >= CARD_COUNT) {
            sprite.setClutRow(grid->frame);
        } else {
            sprite.setClutRow(0);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 6, col * 42 + 0x23, row * 54 + 0x32);
    }
}

/* 1 when the grid's page holds a seen card (or runs past the last card) */
s32 STCRDABM_pageHasCards(CardAlbumGrid *grid) {
    s32 card;
    s32 i;

    for (i = 0; i < ALBUM_PAGE_CARDS; i++) {
        card = grid->first + i;
        if (GAME.cardsSeen[card] != 0 || card >= CARD_COUNT) {
            return 1;
        }
    }
    return 0;
}

#include "shared/update_hiding.inc.c"

/* The card grid's task: draws the page's cards; on a page turn, turns its 12 slots one
   every 2 frames over the old cards, then loads and shows the new ones, with a sound
   when the page has cards */
void STCRDABM_updateGrid(CardAlbumGrid *grid) {
    switch (grid->state) {
    case TASK_INIT:
    default:
        grid->nextState(grid);
        grid->first = 1;
        STCRDABM_setPage(grid, 1);
        break;
    case TASK_RUN:
        STCRDABM_updateHiding(grid);
        STCRDABM_drawCards(grid, 0);
        break;
    case TASK_DONE:
        switch (grid->substate) {
        case 0:
        default:
            if (++grid->turned < ALBUM_PAGE_CARDS) {
                grid->nextSubstate(grid);
                grid->counter = GFX.funcs.getTime();
            } else {
                grid->turned = ALBUM_PAGE_CARDS;
                grid->substate = 2;
            }
            break;
        case 1:
            if (GFX.funcs.getTime() - grid->counter >= 2) {
                grid->substate = grid->step;
            }
            break;
        case 2:
            STCRDABM_loadIcons(grid);
            grid->shown = ALBUM_PAGE_CARDS;
            grid->time = GFX.funcs.getTime();
            grid->nextSubstate(grid);
            if (STCRDABM_pageHasCards(grid) != 0) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
            }
            break;
        case 3:
            if (GFX.funcs.getTime() - grid->time >= 2) {
                grid->time = GFX.funcs.getTime();
                if (++grid->frame >= 11) {
                    grid->state = 1;
                }
            }
            break;
        }
        STCRDABM_drawTurningSlots(grid);
        if (grid->substate < 3) {
            STCRDABM_drawCards(grid, 1);
        } else {
            STCRDABM_drawCards(grid, 0);
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the album's card grid (task), on the top layer */
CardAlbumGrid *STCRDABM_createGrid(CardAlbum *album) {
    CardAlbumGrid *grid = createTask(STCRDABM_updateGrid, sizeof(CardAlbumGrid), 0);

    grid->setPage = STCRDABM_setPage;
    grid->hide = STCRDABM_hideCards;
    grid->layer = SCREEN_LAYER;
    grid->depth = 6;
    grid->album = album;
    return grid;
}

#define SCENE_TASK Task
#define SCENE_CHILD CardAlbum
#define SCENE_CREATE OVL_NAME(createAlbum)
#define SCENE_PRIM_BUFFERS 0xF000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

/* Creates the album's text windows (the page's and the selected card's), in front of
   the album */
void STCRDABM_createWindows(CardAlbum *album, CardAlbumWindows *win) {
    TextWindow **items;
    s32 i;

    win->title = createTextWindow(album->layer, 1, 0x10, 0x19);
    win->help = createTextWindow(album->layer, 1, 0xD0, 0x19);
    win->page = createTextWindow(album->layer, 1, 0x34, 0x9E);
    win->pageSlash = createTextWindow(album->layer, 1, 0x35, 0x9E);
    win->pageCount = createTextWindow(album->layer, 1, 0x4A, 0x9E);
    win->prev = createTextWindow(album->layer, 1, 0x12, 0x6C);
    win->next = createTextWindow(album->layer, 1, 0x121, 0x6C);
    win->name = createTextWindow(album->layer, 1, 0x88, 0xA1);
    win->levelLabel = createTextWindow(album->layer, 1, 0x115, 0xA1);
    win->level = createTextWindow(album->layer, 1, 0x126, 0xA1);
    win->countLabel = createTextWindow(album->layer, 1, 0x115, 0xC7);
    win->count = createTextWindow(album->layer, 1, 0x12C, 0xC7);
    win->text = createTextWindow(album->layer, 1, 0x50, 0xB8);
    win->stat1Label = createTextWindow(album->layer, 1, 0xCE, 0xB8);
    win->stat1 = createTextWindow(album->layer, 1, 0xF0, 0xB8);
    win->stat2Label = createTextWindow(album->layer, 1, 0xCE, 0xC5);
    win->stat2 = createTextWindow(album->layer, 1, 0xF0, 0xC5);
    for (i = 0, items = album->children; i < album->childCount - 2; i++, items++) {
        (*items)->setDepth(*items, album->depth - 3);
    }
}

/* Shows the title, help and page number and, while the album takes input, the previous
   and next page hints where there is such a page; hides them all when show is 0 */
void STCRDABM_showPageInfo(CardAlbum *album, CardAlbumWindows *win, s32 show) {
    if (show != 0) {
        win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 1);
        win->help->setString(win->help, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 2);
        win->page->setNumber(win->page, 0, album->page + 1);
        win->page->setRightAlign(win->page, 1);
        win->pageSlash->setString(win->pageSlash, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 5);
        win->pageCount->setNumber(win->pageCount, 0, album->pageCount);
        win->pageCount->setRightAlign(win->pageCount, 1);
        if (album->active != 0) {
            if (album->page > 0) {
                win->prev->setString(win->prev, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 3);
            } else {
                win->prev->setVisible(win->prev, 0);
            }
            if (album->page < album->pageCount - 1) {
                win->next->setString(win->next, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 4);
            } else {
                win->next->setVisible(win->next, 0);
            }
        }
    } else {
        win->title->setVisible(win->title, 0);
        win->help->setVisible(win->help, 0);
        win->page->setVisible(win->page, 0);
        win->pageSlash->setVisible(win->pageSlash, 0);
        win->pageCount->setVisible(win->pageCount, 0);
        win->prev->setVisible(win->prev, 0);
        win->next->setVisible(win->next, 0);
    }
}

/*
 * Shows the card under the cursor (album->card = page * 12 + slot + 1) once
 * the player has seen it: its name, the copies owned and, for a card of kind
 * 0, its level and its two stats, or its text for cards 0x45, 0x70, 0x9B,
 * 0xC6 and 0xF1; the other kinds show their text. Hides it all otherwise.
 */
void STCRDABM_showCardInfo(CardAlbum *album, CardAlbumWindows *win, s32 show) {
    CardDrawer icon;

    album->card = album->page * 12 + album->slot + 1;
    if (GAME.cardsSeen[album->card] != 0 && show != 0) {
        initCardDrawer(&icon);
        icon.setCard(album->card);
        win->name->setString(win->name, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_NAMES)), album->card);
        win->countLabel->setString(win->countLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 8);
        win->count->setNumber(win->count, 0, GAME.cards[album->card]);
        win->count->setRightAlign(win->count, 1);
        if (icon.getKind() != 0) {
            win->levelLabel->setVisible(win->levelLabel, 0);
            win->level->setVisible(win->level, 0);
            win->text->setString(win->text, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), album->card);
            win->stat1Label->setVisible(win->stat1Label, 0);
            win->stat1->setVisible(win->stat1, 0);
            win->stat2Label->setVisible(win->stat2Label, 0);
            win->stat2->setVisible(win->stat2, 0);
        } else {
            win->levelLabel->setString(win->levelLabel, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 8);
            win->level->setNumber(win->level, 0, icon.card->points);
            win->level->setRightAlign(win->level, 1);
            if (album->card == 0x45 || album->card == 0x70 || album->card == 0x9B ||
                album->card == 0xC6 || album->card == 0xF1) {
                win->text->setString(win->text, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_EFFECTS)), album->card);
                win->stat1Label->setVisible(win->stat1Label, 0);
                win->stat1->setVisible(win->stat1, 0);
                win->stat2Label->setVisible(win->stat2Label, 0);
                win->stat2->setVisible(win->stat2, 0);
            } else {
                win->text->setVisible(win->text, 0);
                win->stat1Label->setString(win->stat1Label, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 6);
                win->stat1->setNumber(win->stat1, 0, icon.card->ap);
                win->stat1->setRightAlign(win->stat1, 1);
                win->stat2Label->setString(win->stat2Label, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_ALBUM)), 7);
                win->stat2->setNumber(win->stat2, 0, icon.card->hp);
                win->stat2->setRightAlign(win->stat2, 1);
            }
        }
    } else {
        win->name->setVisible(win->name, 0);
        win->countLabel->setVisible(win->countLabel, 0);
        win->count->setVisible(win->count, 0);
        win->levelLabel->setVisible(win->levelLabel, 0);
        win->level->setVisible(win->level, 0);
        win->text->setVisible(win->text, 0);
        win->stat1Label->setVisible(win->stat1Label, 0);
        win->stat1->setVisible(win->stat1, 0);
        win->stat2Label->setVisible(win->stat2Label, 0);
        win->stat2->setVisible(win->stat2, 0);
    }
}

/* Draws the album: the scrolling background, its panels as they open, the blinking
   previous and next page sprites and the cursor while it takes input, and the panels of
   the card's details */
void STCRDABM_drawAlbum(CardAlbum *album) {
    SpriteDrawer sprite;
    CardDrawer icon;
    s32 kind;
    s32 frame;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(album->layer, album->depth);
    if (album->frameToggle != 0) {
        album->frame = ++album->frame < 0x60 ? album->frame : 0;
        album->frameToggle = 0;
    } else {
        album->frameToggle = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 8, album->frame, album->frame);
    sprite.setLayerId(album->layer, album->depth - 2);
    if (album->fade.level != 0) {
        if (album->fade.level != ONE) {
            sprite.setScale(album->fade.level, ONE, ONE);
            sprite.setPivot(0, 0x20);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 9, 0, 0x15);
        if (album->fade.level != ONE) {
            sprite.setPivot(0x140, 0x20);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xF, 0xC8, 0x15);
        if (album->fade.level != ONE) {
            sprite.setScale(album->fade.level, album->fade.level, ONE);
            sprite.setPivot(0x3A, 0xA5);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xE, 0x22, 0x9A);
    }
    if (album->active != 0) {
        if (album->pageCount >= 2) {
            if (GFX.funcs.getTime() - album->blinkTime > 0x10) {
                album->blinkTime = GFX.funcs.getTime();
                album->blink = 1 - album->blink;
            }
            if (album->blink != 0) {
                if (album->page > 0) {
                    sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0x1A, 0xE, 0x5A);
                }
                if (album->page < album->pageCount - 1) {
                    sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0x1B, 0x121, 0x5A);
                }
            }
        }
        if (album->pageHasCards != 0) {
            if (GFX.funcs.getTime() - album->cursorTime > 0x10) {
                album->cursorTime = GFX.funcs.getTime();
                if (++album->cursorFrame >= 6) {
                    album->cursorFrame = 0;
                }
            }
            sprite.setClutRow(STCRDABM_cursorBlink[album->cursorFrame]);
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 7, album->slot % 6 * 42 + 0x24, album->slot / 6 * 54 + 0x32);
            sprite.setClutRow(0);
        }
    }
    if (album->infoFade.level != 0) {
        initCardDrawer(&icon);
        icon.setCard(album->card);
        if (album->infoFade.level != ONE) {
            sprite.setScale(album->infoFade.level, ONE, ONE);
            sprite.setPivot(0x140, 0xA8);
        }
        kind = icon.getKind();
        if (kind == 1) {
            frame = 0x12;
        } else if (kind == 2) {
            frame = 0x13;
        } else {
            frame = icon.card->color + 0x13;
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), frame, 0x103, 0x9F);
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xD, 0xFC, 0x9D);
        if (album->infoFade.level != ONE) {
            sprite.setPivot(0x140, 0xA8);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xA, 0x82, 0x9D);
        if (album->infoFade.level != ONE) {
            sprite.setPivot(0x140, 0xD0);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0x10, 0x103, 0xC5);
        sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xD, 0xFC, 0xC3);
        if (album->infoFade.level != ONE) {
            sprite.setPivot(0x140, 0xC6);
        }
        if (kind != 0) {
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xB, 0x4A, 0xB3);
        } else if (album->card == 0x45 || album->card == 0x70 || album->card == 0x9B || album->card == 0xC6 ||
                   album->card == 0xF1) {
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xB, 0x4A, 0xB3);
        } else {
            sprite.draw(FILE_CACHE.getEntry(STCRDABM_SPRITES), 0xC, 0xC7, 0xB3);
        }
    }
}

/* Notes which slots of the current page hold a seen card, and whether any does */
void STCRDABM_findPageCards(CardAlbum *album) {
    s32 i;
    s32 card;

    album->pageHasCards = 0;
    card = album->page * ALBUM_PAGE_CARDS;
    for (i = 0; i < ALBUM_PAGE_CARDS; i++) {
        card++;
        if (card < CARD_COUNT && GAME.cardsSeen[card] != 0) {
            album->slotHasCard[i] = 1;
            album->pageHasCards = 1;
        } else {
            album->slotHasCard[i] = 0;
        }
    }
}

/* The page turned: shows the new page's cards, fading the card's details in
   or out when the page gains or loses cards seen */
static inline void STCRDABM_turnAlbumPage(CardAlbum *album, CardAlbumWindows *win) {
    SOUND.playSound(SOUND_MENU_MOVE);
    album->active = 0;
    album->slot = 0;
    STCRDABM_findPageCards(album);
    STCRDABM_showPageInfo(album, win, 1);
    if (album->infoFade.level == 0) {
        if (album->pageHasCards != 0) {
            album->substate = 10;
            album->step = 1;
            win->grid->setPage(win->grid, album->page * ALBUM_PAGE_CARDS | 1);
        } else {
            album->substate = 15;
        }
    } else {
        win->grid->setPage(win->grid, album->page * ALBUM_PAGE_CARDS | 1);
        if (album->pageHasCards == 0) {
            album->substate = 10;
            album->step = 0;
        } else {
            album->substate = 15;
        }
    }
}

/* The pad moves the cursor between the page's slots (two rows of six) to the
   next card seen that way, and shows its details; it stays put when there is none */
static inline void STCRDABM_moveAlbumCursor(CardAlbum *album, CardAlbumWindows *win) {
    s32 slot;
    s32 i;

    slot = album->slot;
    if (PAD_PRESSED(PAD_UP)) {
        if (album->slot >= 6) {
            album->slot -= 6;
        }
    } else if (PAD_PRESSED(PAD_DOWN)) {
        if (album->slot < 6) {
            album->slot += 6;
        }
    }
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        if (--album->slot < 0) {
            album->slot = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        if (++album->slot >= ALBUM_PAGE_CARDS) {
            album->slot = ALBUM_PAGE_CARDS - 1;
        }
    }
    if (album->page * ALBUM_PAGE_CARDS + album->slot >= CARD_COUNT - 1) {
        album->slot = CARD_COUNT - 2 - album->page * ALBUM_PAGE_CARDS;
    }
    if (slot != album->slot) {
        i = album->slot;
        album->slot = -1;
        if (i < slot) {
            for (; i >= 0; i--) {
                if (album->slotHasCard[i] != 0) {
                    album->slot = i;
                    break;
                }
            }
        } else if (slot < i) {
            for (; i < ALBUM_PAGE_CARDS; i++) {
                if (album->slotHasCard[i] != 0) {
                    album->slot = i;
                    break;
                }
            }
        }
        if (album->slot == -1) {
            album->slot = slot;
        } else {
            STCRDABM_showCardInfo(album, win, 1);
            SOUND.playSound(SOUND_MENU_MOVE);
        }
    }
}

/* The album open: L1/R1 turn the page, the pad moves the cursor between the
   page's cards seen; triangle fades the screen out to leave */
static inline void STCRDABM_browseAlbum(CardAlbum *album, CardAlbumWindows *win) {
    s32 prev;

    prev = album->page;
    if ((!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) || (!PAD_HELD(PAD_R1) && PAD_REPEATED(PAD_L1))) {
        if (--album->page < 0) {
            album->page = 0;
        }
    } else if ((!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) || (!PAD_HELD(PAD_L1) && PAD_REPEATED(PAD_R1))) {
        if (++album->page > album->pageCount - 1) {
            album->page = album->pageCount - 1;
        }
    }
    if (prev != album->page) {
        STCRDABM_turnAlbumPage(album, win);
    } else {
        STCRDABM_moveAlbumCursor(album, win);
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        album->active = 0;
        album->substate = 50;
        win->fader = STCRDABM_createFader();
        win->fader->start(win->fader, 0, 10);
    }
}

/* The album's steps: opens it, then turns pages with L1 and R1, moves the cursor
   between the seen cards and shows the selected one's details; Triangle fades the
   screen out to leave */
void STCRDABM_runAlbum(CardAlbum *album, CardAlbumWindows *win) {
    switch (album->substate) {
    case 0:
    default:
        STCRDABM_funcs.startFade(&album->fade, 1);
        STCRDABM_findPageCards(album);
        win->grid = STCRDABM_createGrid(album);
        album->substate++;
        break;
    case 1:
        if (STCRDABM_funcs.updateFade(&album->fade) != 0) {
            STCRDABM_showPageInfo(album, win, 1);
            if (album->pageHasCards != 0) {
                STCRDABM_funcs.startFade(&album->infoFade, 1);
            }
            album->substate = 11;
        }
        break;
    case 3:
        album->active = 1;
        album->substate++;
        break;
    case 4:
        STCRDABM_browseAlbum(album, win);
        break;
    case 10:
        if (album->step == 0) {
            STCRDABM_showCardInfo(album, win, 0);
        }
        STCRDABM_funcs.startFade(&album->infoFade, album->step);
        album->nextSubstate(album);
        break;
    case 11:
        if (STCRDABM_funcs.updateFade(&album->infoFade) != 0) {
            album->substate = 15;
        }
        break;
    case 15:
        if (win->grid->state == TASK_RUN) {
            album->active = win->grid->state;
            STCRDABM_showPageInfo(album, win, 1);
            if (album->infoFade.level != 0) {
                while (1) {
                    if (album->slotHasCard[album->slot] != 0) {
                        break;
                    }
                    album->slot++;
                }
                STCRDABM_showCardInfo(album, win, 1);
            }
            album->substate = 3;
        }
        break;
    case 50:
        if (win->fader->state == TASK_DONE) {
            album->state = TASK_KILL;
        }
        break;
    case 51:
        if (STCRDABM_funcs.updateFade(&album->infoFade) != 0) {
            STCRDABM_showPageInfo(album, win, 0);
            STCRDABM_funcs.startFade(&album->fade, 0);
            album->substate++;
        }
        break;
    case 52:
        if (STCRDABM_funcs.updateFade(&album->fade) != 0) {
            album->substate++;
        }
        break;
    case 53:
        if (win->grid == NULL) {
            album->setState(album, TASK_KILL);
        }
        break;
    }
}

/* The album's task: loads the files and creates its windows, then runs and draws it;
   once the screen has faded out, goes back to the previous mode */
void STCRDABM_updateAlbum(CardAlbum *album, CardAlbumWindows *win) {
    switch (album->state) {
    case TASK_INIT:
    default:
        switch (album->substate) {
        case 0:
        default:
            STCRDABM_funcs.loadFiles();
            album->substate++;
            break;
        case 1:
            if (STCRDABM_funcs.filesLoading() == 0) {
                STCRDABM_createWindows(album, win);
                album->fade.duration = 8;
                album->infoFade.duration = 8;
                album->pageCount = 27;
                album->card = 1;
                album->nextState(album);
            }
            break;
        }
        break;
    case TASK_RUN:
        STCRDABM_runAlbum(album, win);
        STCRDABM_drawAlbum(album);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.funcs.getPrevMode(), 0);
        break;
    }
}

/* Creates the album (task), on the top layer */
CardAlbum *STCRDABM_createAlbum(void) {
    CardAlbum *album = createTask(STCRDABM_updateAlbum, sizeof(CardAlbum), sizeof(CardAlbumWindows));

    album->layer = SCREEN_LAYER;
    album->depth = 7;
    return album;
}

/* Loads the album's images and requests the card data and the card names, effects and
   album strings */
void STCRDABM_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STCRDABM_IMAGES));
    FILE_CACHE.request(STCRDABM_FILE_DATA);
    FILE_CACHE.request(STCRDABM_FILE_DATA + 1);
    FILE_CACHE.request(STCRDABM_FILE_DATA + 2);
    FILE_CACHE.request(STCRDABM_FILE_DATA + 3);
    FILE_CACHE.request(STCRDABM_FILE_DATA + 4);
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_EFFECTS));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_ALBUM));
}

/* Whether the card data or the strings are still loading */
s32 STCRDABM_filesLoading(void) {
    if (FILE_CACHE.isLoading(STCRDABM_FILE_DATA) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(STCRDABM_FILE_DATA + 1) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(STCRDABM_FILE_DATA + 2) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(STCRDABM_FILE_DATA + 3) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(STCRDABM_FILE_DATA + 4) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_EFFECTS)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_ALBUM)) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
