/* The deck screen: the three decks and the options to edit or rename one; the
   files it loads; and the panels' openings and moves */

#include "cardgame/deck_editor.h"

/* The title, each deck's name and six counts, the two options and the
   cursor, then every window from the title on takes the cursor's depth */
void STCRDDEK_createScreenWindows(DeckScreen *task, DeckScreenChildren *children) {
    s32 i;
    s32 j;
    s32 y;
    TextWindow **windows;

    children->title = createTextWindow(task->layer, 1, 0x97, 0x22);
    for (i = 0; i < 3; i++) {
        /* the match depends on y: with i * 0x2D written in both calls, the
           options loop's counter takes s2 instead of the other loops' s3 */
        y = i * 0x2D;
        children->rows[i].name = createTextWindow(task->layer, 1, 0x1C, 0x53 + y);
        for (j = 0; j < 6; j++) {
            children->rows[i].counts[j] = createTextWindow(task->layer, 1, 0x3B + j * 0x23, 0x64 + y);
        }
    }
    for (i = 0; i < 2; i++) {
        children->options[i] = createTextWindow(task->layer, 1, 0xA7, 0x23 + i * 0xE);
    }
    children->cursor = createCursor(task->layer, task->depth - 2, 0x9A, 0x23);
    children->cursor->setVisible(children->cursor, 0);
    windows = &children->title;
    for (i = 0; i < task->childCount - 3; i++, windows++) {
        (*windows)->setDepth(*windows, task->depth - 2);
    }
}

/* Shows a deck's row: its name and how many cards of each kind it has, or hides it */
void STCRDDEK_showDeckRow(DeckScreen *task, DeckScreenChildren *children, s32 deck, s32 show) {
    s32 i;

    if (show != 0) {
        children->rows[deck].name->setString(children->rows[deck].name, GAME.decks[deck].name, -1);
        for (i = 0; i < 6; i++) {
            children->rows[deck].counts[i]->setNumber(children->rows[deck].counts[i], 0, task->kinds[deck][i]);
            children->rows[deck].counts[i]->setRightAlign(children->rows[deck].counts[i], 1);
        }
    } else {
        children->rows[deck].name->setVisible(children->rows[deck].name, 0);
        for (i = 0; i < 6; i++) {
            children->rows[deck].counts[i]->setVisible(children->rows[deck].counts[i], 0);
        }
    }
}

/* Draws the screen: the background scrolling diagonally, the title panel, the
   decks' rows, the options' panel and the frame on the deck under the cursor
   (glowing until it is chosen), as they open */
void STCRDDEK_drawScreen(DeckScreen *task) {
    SpriteDrawer sprite;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(task->layer, task->depth);
    sprite.setTexture(0x280, 0);
    if (task->tick) {
        task->scroll = ++task->scroll < 96 ? task->scroll : 0;
        task->tick = 0;
    } else {
        task->tick = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 8, task->scroll, task->scroll);
    sprite.setLayerId(task->layer, task->depth - 1);
    if (task->panels[0].level != 0) {
        if (task->panels[0].level != ONE) {
            sprite.setScale(task->panels[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x22);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x30, 0x7B, 0x1C);
    }
    for (i = 0; i < 3; i++) {
        if (task->rowPanels[i].level != 0) {
            sprite.setScale(task->rowPanels[i].level, ONE, ONE);
            if (task->rowPanels[i].level != ONE) {
                sprite.setPivot(0x17, 0x63 + i * 0x2D);
            }
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x41, 0x17, 0x50 + i * 0x2D);
        }
    }
    if (task->panels[1].level != 0) {
        sprite.setScale(task->panels[1].level, ONE, ONE);
        if (task->panels[1].level != ONE) {
            sprite.setPivot(0x140, 0x37);
        }
        sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x2D, 0x92, 0x1C);
    }
    if (task->panels[2].level != 0) {
        sprite.setLayerId(task->layer, task->depth - 2);
        sprite.setScale(ONE, task->panels[2].level, ONE);
        if (task->panels[2].level != ONE) {
            sprite.setPivot(0x17, task->deck * 0x2D + 0x63);
        }
        if (task->chosen == 0) {
            if (GFX.funcs.getTime() - task->cursorTime >= 5) {
                task->cursorTime = GFX.funcs.getTime();
                if (++task->cursorFrame >= 16) {
                    task->cursorFrame = 0;
                }
            }
            sprite.setClutRow(task->cursorFrame);
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x42, 0x17, task->deck * 0x2D + 0x50);
        } else {
            sprite.draw(FILE_CACHE.getEntry(STCRDDEK_SPRITES), 0x44, 0x17, task->deck * 0x2D + 0x50);
        }
    }
}

/* Counts how many cards of each kind (their color) each of the three decks has */
void STCRDDEK_countCardKinds(DeckScreen *task) {
    CardDrawer card;
    s32 d;
    s32 i;

    initCardDrawer(&card);
    for (d = 0; d < 3; d++) {
        for (i = 0; i < 6; i++) {
            task->kinds[d][i] = 0;
        }
        for (i = 0; i < 40; i++) {
            card.setCard(GAME.decks[d].cards[i]);
            task->kinds[d][card.card->color - 1]++;
        }
    }
}

/* Up/down choose one of the three decks; cross offers what to do with it,
   triangle leaves the screen */
static inline void STCRDDEK_chooseDeck(DeckScreen *task, DeckScreenChildren *children) {
    s32 prevDeck;

    prevDeck = task->deck;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--task->deck < 0) {
            task->deck = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++task->deck >= 3) {
            task->deck = 2;
        }
    }
    if (prevDeck != task->deck) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        task->substate = 10;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->setSubstate(task, 100);
    }
}

/* Once the options' panel has opened: shows edit folder and change name, the
   cursor on edit */
static inline void STCRDDEK_openDeckOptions(DeckScreen *task, DeckScreenChildren *children) {
    if (STCRDDEK_funcs.updateFade(&task->panels[1])) {
        task->renaming = 0;
        children->cursor->setPos(children->cursor, 0x9A, 0x23);
        children->cursor->setVisible(children->cursor, 1);
        children->options[0]->setString(children->options[0], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x1A);
        children->options[1]->setString(children->options[1], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x1B);
        task->substate++;
    }
}

/* Up/down choose to edit the deck or to rename it; cross closes the decks'
   rows to open it, triangle goes back to the decks */
static inline void STCRDDEK_chooseDeckOption(DeckScreen *task, DeckScreenChildren *children) {
    s32 prevChoice;

    prevChoice = task->renaming;
    if (PAD_PRESSED(PAD_UP)) {
        task->renaming = 0;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        task->renaming = 1;
    }
    if (prevChoice != task->renaming) {
        SOUND.playSound(SOUND_CURSOR);
        children->cursor->setPos(children->cursor, 0x9A, task->renaming * 14 + 0x23);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        task->setSubstate(task, 50);
        task->step = 1;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate = 15;
    }
}

/* Closes the decks' rows, from the last, to open the editor or the name entry
   (step set): the cursor stays frozen on the chosen option */
static inline void STCRDDEK_startClosingRows(DeckScreen *task, DeckScreenChildren *children) {
    if (task->step) {
        children->cursor->setStill(children->cursor, 1);
        children->cursor->setPalette(children->cursor, PALETTE_GREY);
    }
    STCRDDEK_funcs.startFade(&task->panels[2], 0);
    STCRDDEK_funcs.startFade(&task->rowPanels[2], 0);
    STCRDDEK_showDeckRow(task, children, 2, 0);
    task->substate++;
}

/* Once the second deck's row has closed: hides the options (the title, step
   unset) and closes the first deck's row */
static inline void STCRDDEK_closeFirstRow(DeckScreen *task, DeckScreenChildren *children) {
    if (STCRDDEK_funcs.updateFade(&task->rowPanels[1])) {
        if (task->step) {
            children->cursor->setStill(children->cursor, 0);
            children->cursor->setPalette(children->cursor, PALETTE_WHITE);
            children->cursor->setVisible(children->cursor, 0);
            children->options[0]->setVisible(children->options[0], 0);
            children->options[1]->setVisible(children->options[1], 0);
            STCRDDEK_funcs.startFade(&task->panels[1], 0);
        } else {
            children->title->setVisible(children->title, 0);
            STCRDDEK_funcs.startFade(&task->panels[0], 0);
        }
        STCRDDEK_funcs.startFade(&task->rowPanels[0], 0);
        STCRDDEK_showDeckRow(task, children, 0, 0);
        task->substate++;
    }
}

/* Once the rows have closed: opens the editor or the name entry for the chosen
   deck and waits for it; after the fade out of triangle (step unset), ends
   the screen */
static inline void STCRDDEK_openChosen(DeckScreen *task, DeckScreenChildren *children) {
    if (task->step) {
        if (task->renaming == 0) {
            children->child.editor = STCRDDEK_createEditor(task, task->deck);
        } else {
            children->child.name = STCRDDEK_createNameEntry(GAME.decks[task->deck].name);
        }
        task->setState(task, TASK_DONE);
    } else {
        task->state = TASK_KILL;
    }
}

/* The screen's states: opens the title and the decks' rows; cross on a deck
   offers to edit or rename it, closing the rows to open the editor or the
   name entry; triangle fades the screen out to leave */
void STCRDDEK_stepScreen(DeckScreen *task, DeckScreenChildren *children) {
    ScreenFade *fader;

    switch (task->substate) {
    case 0:
    default:
        STCRDDEK_countCardKinds(task);
        STCRDDEK_funcs.startFade(&task->panels[0], 1);
        STCRDDEK_funcs.startFade(&task->rowPanels[0], 1);
        task->chosen = 0;
        task->substate++;
        break;
    case 1:
        STCRDDEK_funcs.updateFade(&task->panels[0]);
        if (STCRDDEK_funcs.updateFade(&task->rowPanels[0])) {
            children->title->setString(children->title, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x19);
            STCRDDEK_showDeckRow(task, children, 0, 1);
            STCRDDEK_funcs.startFade(&task->rowPanels[1], 1);
            task->substate++;
        }
        break;
    case 2:
        if (STCRDDEK_funcs.updateFade(&task->rowPanels[1])) {
            STCRDDEK_showDeckRow(task, children, 1, 1);
            STCRDDEK_funcs.startFade(&task->rowPanels[2], 1);
            STCRDDEK_funcs.startFade(&task->panels[2], 1);
            task->substate++;
        }
        break;
    case 3:
        STCRDDEK_funcs.updateFade(&task->panels[2]);
        if (STCRDDEK_funcs.updateFade(&task->rowPanels[2])) {
            STCRDDEK_showDeckRow(task, children, 2, 1);
            task->substate++;
        }
        break;
    case 4:
        STCRDDEK_chooseDeck(task, children);
        break;
    case 10:
        task->chosen = 1;
        children->title->setVisible(children->title, 0);
        STCRDDEK_funcs.startFade(&task->panels[0], 0);
        task->substate++;
        break;
    case 11:
        if (STCRDDEK_funcs.updateFade(&task->panels[0])) {
            children->title->setVisible(children->title, 0);
            STCRDDEK_funcs.startFade(&task->panels[1], 1);
            task->substate++;
        }
        break;
    case 12:
        STCRDDEK_openDeckOptions(task, children);
        break;
    case 13:
        STCRDDEK_chooseDeckOption(task, children);
        break;
    case 15:
        children->cursor->setVisible(children->cursor, 0);
        children->options[0]->setVisible(children->options[0], 0);
        children->options[1]->setVisible(children->options[1], 0);
        STCRDDEK_funcs.startFade(&task->panels[1], 0);
        task->substate++;
        break;
    case 16:
        if (STCRDDEK_funcs.updateFade(&task->panels[1])) {
            STCRDDEK_funcs.startFade(&task->panels[0], 1);
            task->substate++;
        }
        break;
    case 17:
        if (STCRDDEK_funcs.updateFade(&task->panels[0])) {
            children->title->setString(children->title, FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), 0x19);
            task->chosen = 0;
            task->setSubstate(task, 4);
        }
        break;
    case 50:
        STCRDDEK_startClosingRows(task, children);
        break;
    case 51:
        STCRDDEK_funcs.updateFade(&task->panels[2]);
        if (STCRDDEK_funcs.updateFade(&task->rowPanels[2])) {
            STCRDDEK_funcs.startFade(&task->rowPanels[1], 0);
            STCRDDEK_showDeckRow(task, children, 1, 0);
            task->substate++;
        }
        break;
    case 52:
        STCRDDEK_closeFirstRow(task, children);
        break;
    case 53:
        if (task->step) {
            STCRDDEK_funcs.updateFade(&task->panels[1]);
        } else {
            STCRDDEK_funcs.updateFade(&task->panels[0]);
        }
        if (STCRDDEK_funcs.updateFade(&task->rowPanels[0])) {
            task->substate++;
        }
        break;
    case 54:
        STCRDDEK_openChosen(task, children);
        break;
    case 100:
        children->fader = fader = STCRDDEK_createFader();
        fader->start(fader, 0, 10);
        task->substate++;
        break;
    case 101:
        if (children->fader->state == TASK_DONE) {
            task->substate = 54;
        }
        break;
    }
}

/* The screen's task: loads its files and creates its windows, then runs it;
   waits for the editor or the name entry (saving the deck's new name), then
   opens again; returns to the previous mode when it ends */
void STCRDDEK_updateScreen(DeckScreen *task, DeckScreenChildren *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        switch (task->substate) {
        case 0:
        default:
            STCRDDEK_funcs.loadFiles();
            task->substate++;
            break;
        case 1:
            if (STCRDDEK_funcs.filesLoading() == 0) {
                STCRDDEK_createScreenWindows(task, children);
                task->panels[0].duration = 10;
                task->rowPanels[0].duration = 10;
                task->rowPanels[1].duration = 10;
                task->rowPanels[2].duration = 10;
                task->panels[1].duration = 10;
                task->panels[2].duration = 10;
                task->nextState(task);
            }
            break;
        }
        break;
    case TASK_RUN:
        STCRDDEK_stepScreen(task, children);
        STCRDDEK_drawScreen(task);
        break;
    case TASK_DONE:
        if (task->renaming != 0) {
            switch (task->substate) {
            case 0:
            default:
                if (children->child.name->substate == 100) {
                    children->child.name->getName(children->child.name, GAME.decks[task->deck].name);
                    children->child.name->close(children->child.name);
                    task->substate++;
                }
                break;
            case 1:
                if (children->child.name->state == TASK_DONE) {
                    children->child.name->state = TASK_KILL;
                    task->setState(task, TASK_RUN);
                }
                break;
            }
        } else if (children->child.editor == NULL) {
            task->setState(task, TASK_RUN);
        }
        STCRDDEK_drawScreen(task);
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.funcs.getPrevMode(), GAME.funcs.getModeArg());
        break;
    }
}

/* Creates the deck screen (task) */
DeckScreen *STCRDDEK_createScreen(void) {
    DeckScreen *task = createTask(STCRDDEK_updateScreen, sizeof(DeckScreen), sizeof(DeckScreenChildren));

    task->countKinds = STCRDDEK_countCardKinds;
    task->layer = SCREEN_LAYER;
    task->depth = 7;
    return task;
}

/* Loads the screen's textures and requests the card images, the card texts and
   the keyboard */
void STCRDDEK_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0);
    loader.loadArchive(FILE_CACHE.getEntry(STCRDDEK_IMAGES));
    FILE_CACHE.request(STCRDDEK_FILE_CARDS);
    FILE_CACHE.request(STCRDDEK_FILE_CARDS + 1);
    FILE_CACHE.request(STCRDDEK_FILE_CARDS + 2);
    FILE_CACHE.request(STCRDDEK_FILE_CARDS + 3);
    FILE_CACHE.request(STCRDDEK_FILE_CARDS + 4);
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_EFFECTS));
    FILE_CACHE.request(TEXT_FILE(TEXT_CARD_SHOP));
    FILE_CACHE.request(TEXT_FILE(TEXT_NAME_ENTRY));
    FILE_CACHE.request(STCRDDEK_FILE_KEYBOARD);
}

/* Whether the texts or the keyboard are still loading (the card images aren't waited for) */
s32 STCRDDEK_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_EFFECTS)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_CARD_SHOP)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_NAME_ENTRY)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(STCRDDEK_FILE_KEYBOARD) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
