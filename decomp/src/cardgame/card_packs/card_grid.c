/* The grid of six cards, which the pack screen and the buy screen turn over
   to the cards drawn */

#include "cardgame/card_packs.h"

/* Loads the images of the grid's cards into VRAM, one cell each, up to the first
   empty one */
void STCRDSHP_loadIcons(CardPackGrid *grid) {
    CardDrawer icon;
    s32 *cards;
    s32 i;
    s32 card;

    initCardDrawer(&icon);
    icon.setImagePos(0x140, 0x100);
    icon.setClutPos(0x300, 0x100);
    cards = grid->cards;
    for (i = 0; i < 6; i++) {
        card = *cards;
        if (card <= 0 || card >= CARD_PACK_IDS) {
            break;
        }
        cards++;
        icon.setCard(card);
        icon.setCell(0, i);
        icon.loadImage();
    }
}

/* Turns the grid over to six new cards (grid->setCards): keeps the old ones to
   draw while the slots turn */
void STCRDSHP_setCards(CardPackGrid *grid, s32 *cards) {
    s32 i;

    for (i = 0; i < 6; i++) {
        grid->prevCards[i] = grid->cards[i];
        grid->cards[i] = cards[i];
    }
    grid->turned = 0;
    grid->frame = 0;
    grid->setState(grid, TASK_DONE);
}

#include "shared/hide_cards.inc.c"

/* Draws the cards drawn (or the ones being turned over): each card's image,
   its numbers or its kind's mark, and its frame. The match depends on the
   digits' x being written dx + 0x27 + x and on cards++ being in the for. */
void STCRDSHP_drawCards(CardPackGrid *grid, s32 previous) {
    SpriteDrawer sprite;
    CardDrawer drawer;
    s32 digits[5];
    s32 *cards;
    s32 i;
    s32 j;
    s32 dx;
    s32 card;
    s32 x;
    s32 y;
    s32 n;
    s32 col;
    s32 row;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(grid->layer, grid->depth);
    sprite.setTexture(0x280, 0);
    initCardDrawer(&drawer);
    drawer.setImagePos(0x140, 0x100);
    drawer.setClutPos(0x300, 0x100);
    drawer.setLayer(grid->layer, grid->depth);
    if (previous) {
        cards = grid->prevCards;
    } else {
        cards = grid->cards;
    }
    for (i = 0; i < grid->shown; i++, cards++) {
        card = *cards;
        if (card <= 0 || card >= CARD_PACK_IDS) {
            break;
        }
        col = i % 6;
        row = i / 6;
        x = col * 0x2A;
        y = row * 0x36;
        drawer.setCard(card);
        drawer.setCell(0, i);
        drawer.draw(x + 0x27, y + 0x46);
        if (drawer.getKind() != 0) {
            sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 0x1D, x + 0x27, y + 0x65);
        } else {
            n = drawer.card->ap;
            j = n / 10;
            if (j != 0) {
                digits[0] = j + 0x1E;
            } else {
                digits[0] = 0;
            }
            j = n % 10;
            digits[1] = j + 0x1E;
            digits[2] = 0x1C;
            for (j = dx = 0; j < 3; j++, dx += 7) {
                if (digits[j] != 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), digits[j], dx + 0x27 + x, y + 0x65);
                }
            }
            n = drawer.card->hp;
            j = n / 10;
            if (j != 0) {
                digits[0] = j + 0x1E;
            } else {
                digits[0] = 0;
            }
            j = n % 10;
            digits[1] = j + 0x1E;
            for (j = dx = 0; j < 2; j++, dx += 7) {
                if (digits[j] != 0) {
                    sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), digits[j], dx + 0x3A + x, y + 0x65);
                }
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), drawer.card->color - 1, x + 0x23, y + 0x44);
    }
}

/* Draws the cover over each slot turned so far, in the palette of the turn's
   current frame */
void STCRDSHP_drawTurningSlots(CardPackGrid *grid) {
    SpriteDrawer sprite;
    s32 i;
    s32 col;
    s32 row;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(grid->layer, grid->depth - 1);
    sprite.setTexture(0x280, 0);
    for (i = 0; i < grid->turned; i++) {
        col = i % 6;
        row = i / 6;
        sprite.setClutRow(grid->frame);
        sprite.draw(FILE_CACHE.getEntry(FILE_CARDSHOP_SPRITES << 16), 6, col * 42 + 0x23, row * 54 + 0x44);
    }
}

#include "shared/update_hiding.inc.c"

/* The grid's task: covers its slots one by one (every 2 frames) above the old
   cards, loads the new ones' images and steps the covers' palette over 11
   frames above them, then shows them until they are hidden */
void STCRDSHP_updateGrid(CardPackGrid *grid) {
    switch (grid->state) {
    case TASK_INIT:
    default:
        grid->nextState(grid);
        STCRDSHP_setCards(grid, grid->cards);
        break;
    case TASK_RUN:
        STCRDSHP_updateHiding(grid);
        STCRDSHP_drawCards(grid, 0);
        break;
    case TASK_DONE:
        switch (grid->substate) {
        case 0:
        default:
            if (++grid->turned < 6) {
                grid->nextSubstate(grid);
                grid->counter = GFX.funcs.getTime();
            } else {
                grid->turned = 6;
                grid->substate = 2;
            }
            SOUND.playSound(SOUND_CARD_001);
            break;
        case 1:
            if (GFX.funcs.getTime() - grid->counter >= 2) {
                grid->substate = grid->step;
            }
            break;
        case 2:
            STCRDSHP_loadIcons(grid);
            grid->shown = 6;
            grid->time = GFX.funcs.getTime();
            grid->nextSubstate(grid);
            SOUND.playSound(SOUND_MENU_CONFIRM);
            break;
        case 3:
            if (GFX.funcs.getTime() - grid->time >= 2) {
                grid->time = GFX.funcs.getTime();
                if (++grid->frame >= 11) {
                    grid->state = TASK_RUN;
                }
            }
            break;
        }
        STCRDSHP_drawTurningSlots(grid);
        if (grid->substate < 3) {
            STCRDSHP_drawCards(grid, 1);
        } else {
            STCRDSHP_drawCards(grid, 0);
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the grid of six cards, copying the cards given. The match depends
   on cards++ being in the for. */
CardPackGrid *STCRDSHP_createGrid(CardShop *owner, s32 *cards) {
    CardPackGrid *grid = createTask(STCRDSHP_updateGrid, sizeof(CardPackGrid), 0);
    s32 i;

    grid->setCards = STCRDSHP_setCards;
    grid->hide = STCRDSHP_hideCards;
    grid->layer = SCREEN_LAYER;
    grid->depth = 6;
    grid->owner = owner;
    for (i = 0; i < 6; i++, cards++) {
        grid->cards[i] = *cards;
    }
    return grid;
}
