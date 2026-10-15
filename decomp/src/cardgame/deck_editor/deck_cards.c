/* The editor's grid of the deck's cards, shown one by one as their images load */

#include "cardgame/deck_editor.h"

/* Draws the deck's cards shown so far on the grid, 9 a row: each one's frame in
   its color and its image */
void STCRDDEK_drawDeckCards(DeckCards *task) {
    SpriteDrawer sprite;
    CardDrawer card;
    s32 i;
    s32 col;
    s32 row;
    s32 x;
    s32 y;
    void *sheet;

    initSpriteDrawer(&sprite);
    sprite.setTexture(0x280, 0);
    sprite.setLayerId(task->layer, task->depth);
    initCardDrawer(&card);
    card.setImagePos(0x140, 0x100);
    card.setClutPos(0x300, 0x100);
    card.setLayer(task->layer, task->depth);
    for (i = 0; i < task->count; i++) {
        card.setCard(GAME.decks[task->editor->deck].cards[i]);
        /* the match depends on col holding the row first */
        col = i / 9;
        row = col;
        col = i - row * 9;
        sheet = FILE_CACHE.getEntry(STCRDDEK_SPRITES);
        x = col * 32 + 0x10;
        y = row * 32 + 0x3A;
        sprite.draw(sheet, card.card->color + 0x52, x, y);
        card.setCell(col, row);
        card.draw(x, y);
    }
}

/* Shows one more of the deck's cards, loading its image into its cell of VRAM;
   TASK_DONE once all 40 are shown */
void STCRDDEK_loadNextCard(DeckCards *task) {
    CardDrawer card;
    s32 i;

    if (++task->count > 40) {
        task->state = TASK_DONE;
        task->count = 40;
        return;
    }
    initCardDrawer(&card);
    card.setCard(GAME.decks[task->editor->deck].cards[task->count - 1]);
    card.setImagePos(0x140, 0x100);
    card.setClutPos(0x300, 0x100);
    i = task->count - 1;
    card.setCell(i % 9, i / 9);
    card.loadImage();
}

/* The deck cards' task: shows one more card each frame, then keeps drawing all
   40 until the editor kills it */
void STCRDDEK_updateDeckCards(DeckCards *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->count = 0;
        break;
    case TASK_RUN:
        STCRDDEK_loadNextCard(task);
        /* fallthrough */
    case TASK_DONE:
        STCRDDEK_drawDeckCards(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the deck's cards (task) on the editor's grid */
DeckCards *STCRDDEK_createDeckCards(DeckEditor *editor) {
    DeckCards *task = createTask(STCRDDEK_updateDeckCards, sizeof(DeckCards), 0);

    task->layer = SCREEN_LAYER;
    task->depth = 5;
    task->editor = editor;
    return task;
}
