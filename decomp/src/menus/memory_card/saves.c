/* The save list: the saves on a memory card, loading and saving them, and the
   errors */

#include "menus/memory_card.h"

/* Shows the line with the memory card's port number, or hides it */
void STGMCARD_showPort(MemCardSaves *saves, MemCardSavesWindows *win, s32 show) {
    if (show) {
        win->windows[4]->setString(win->windows[4], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x28);
        win->windows[4]->setNumber(win->windows[4], 1, saves->port + 1);
    } else {
        win->windows[4]->setVisible(win->windows[4], 0);
    }
}

/* Shows the error of the last card operation (saves->result, made an index of
   STGMCARD_errorTexts), hiding the choices, the cursor and the panel */
void STGMCARD_showError(MemCardSaves *saves, MemCardSavesWindows *win) {
    saves->substate = 100;
    saves->choice = 0;
    saves->result--;
    if (win->windows[1] != NULL) {
        win->windows[1]->setVisible(win->windows[1], 0);
    }
    if (win->windows[2] != NULL) {
        win->windows[2]->setVisible(win->windows[2], 0);
    }
    if (win->windows[3] != NULL) {
        win->windows[3]->setVisible(win->windows[3], 0);
    }
    if (win->cursor != NULL) {
        win->cursor->setVisible(win->cursor, 0);
    }
    if (win->panel != NULL) {
        win->panel->reset(win->panel);
    }
}

/* Hides the message and panel and closes the slot picker, then shows the error in
   saves->result */
void STGMCARD_closeMenuForError(MemCardSaves *saves, MemCardSavesWindows *win) {
    win->windows[1]->setVisible(win->windows[1], 0);
    win->panel->reset(win->panel);
    saves->substate = 90;
    saves->step = 100;
    win->menu->substate = 0;
}

/* Fills the details window with the selected save again (saves->refresh) */
void STGMCARD_refreshSaves(MemCardSaves *saves) {
    MemCardSavesWindows *win = saves->children;

    win->info->refresh(win->info);
}

/* Hides the save list's windows and ends it (TASK_DONE), which makes the main task fade
   out and leave the screen (saves->hide) */
void STGMCARD_hideSaves(MemCardSaves *saves) {
    MemCardSavesWindows *win = saves->children;

    if (win->windows[0] != NULL) {
        win->windows[0]->setVisible(win->windows[0], 0);
    }
    if (win->windows[4] != NULL) {
        win->windows[4]->setVisible(win->windows[4], 0);
    }
    if (win->windows[1] != NULL) {
        win->windows[1]->setVisible(win->windows[1], 0);
    }
    if (win->windows[2] != NULL) {
        win->windows[2]->setVisible(win->windows[2], 0);
    }
    if (win->windows[3] != NULL) {
        win->windows[3]->setVisible(win->windows[3], 0);
    }
    if (win->cursor != NULL) {
        win->cursor->setVisible(win->cursor, 0);
    }
    if (win->panel != NULL) {
        win->panel->reset(win->panel);
    }
    saves->setState(saves, TASK_DONE);
}

/* Starts the save list: titles it for saving or loading and creates the details
   window */
static inline void STGMCARD_openSaves(MemCardSaves *saves, MemCardSavesWindows *win) {
    win->title->setDepth(win->title, 1);
    if (saves->screen->loading == 0) {
        win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 1);
    } else {
        win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0xE);
    }
    if (win->info == NULL) {
        win->info = STGMCARD_createInfo(saves);
    }
    saves->substate++;
}

/* Slides the title in, then asks which memory card port to use */
static inline void STGMCARD_slideInTitle(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (STGMCARD_funcs.updateLerp(&saves->slide[1]) != 0) {
        win->windows[0]->setVisible(win->windows[0], 0);
        if (saves->screen->loading == 0) {
            win->windows[4]->setString(win->windows[4], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 2);
        } else {
            win->windows[4]->setString(win->windows[4], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0xF);
        }
        win->windows[1]->setDepth(win->windows[1], 1);
        win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x1D);
        win->windows[2]->setString(win->windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 3);
        win->windows[2]->setNumber(win->windows[2], 1, 1);
        win->windows[3]->setString(win->windows[3], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 3);
        win->windows[3]->setNumber(win->windows[3], 1, 2);
        win->cursor->setVisible(win->cursor, 1);
        win->cursor->setPos(win->cursor, 0xC2, saves->port * 14 + 0xBD);
        saves->choosing = 1;
        saves->substate++;
    }
    win->title->setPos(win->title, saves->origin[0] + (s16)(saves->slide[0].value + 200), saves->origin[1] + (s16)(saves->slide[1].value + 9));
}

/* Moves the port cursor: cross starts checking the card in that port, triangle
   leaves the screen */
static inline void STGMCARD_pickPort(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 prev;

    prev = saves->port;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        saves->port = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        saves->port = 1;
    }
    if (prev != saves->port) {
        win->cursor->setPos(win->cursor, 0xC2, saves->port * 14 + 0xBD);
        SOUND.playSound(SOUND_CURSOR);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        win->windows[0]->setString(win->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 4);
        STGMCARD_showPort(saves, win, 1);
        win->windows[1]->setVisible(win->windows[1], 0);
        win->windows[2]->setVisible(win->windows[2], 0);
        win->windows[3]->setVisible(win->windows[3], 0);
        win->cursor->setVisible(win->cursor, 0);
        if (win->panel == NULL) {
            win->panel = STGMCARD_createPanel(0xCD, 0xC1, 0x62, 0xA);
        }
        win->panel->setTopColor(win->panel, 0x7F, 0x32, 0xF2);
        win->panel->setBottomColor(win->panel, 0xD1, 0x2F, 0xDE);
        saves->substate = 10;
        SOUND.playSound(SOUND_SELECT);
        saves->choosing = 0;
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        saves->hide(saves);
        saves->choosing = 0;
        saves->screen->step = 1;
    }
}

/* Waits for the card in the port to answer: a ready card (1 or 4) has its files
   listed (11), any other answer ends the check (12) */
static inline void STGMCARD_acceptCard(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    if (win->panel->substate == 0) {
        win->panel->start(win->panel, 1, 0x4C);
    }
    status = saves->result = MEMCARD_SYSTEM.funcs.accept(saves->port);
    if (status != 0) {
        if (status == 1 || status - 1 == 3) {
            saves->substate++;
        } else {
            win->panel->start(win->panel, 2, 0x14);
            saves->substate += 2;
        }
    }
}

/* Lists the card's files, then ends the check (12) */
static inline void STGMCARD_listCard(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.list(saves->port);
    if (status != 0) {
        win->panel->start(win->panel, 2, 0x14);
        saves->substate++;
    }
}

/* Once the panel is done, goes on to read the info section (20) or shows the
   card's error */
static inline void STGMCARD_endCardCheck(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        if (saves->result == 1) {
            saves->substate = 20;
        } else {
            STGMCARD_showError(saves, win);
        }
    }
}

/* Once the panel is done, shows the reading message and restarts the panel */
static inline void STGMCARD_startInfoRead(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        win->windows[0]->setString(win->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 5);
        STGMCARD_showPort(saves, win, 1);
        win->panel->reset(win->panel);
        win->panel->start(win->panel, 1, 0x4C);
        saves->substate++;
    }
}

/* Reads the save file's info section (the slots' summaries) */
static inline void STGMCARD_readInfo(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.read(saves->port, STGMCARD_funcs.infoBuf, sizeof(MemCardFile), SAVE_SECTION_INFO);
    if (status != 0) {
        win->panel->start(win->panel, 2, 0x14);
        saves->substate++;
    }
}

/* Once the panel is done, checks the info section read: one without the magic
   starts empty, a wrong checksum is error 9, else it is kept with its last slot;
   then opens the slot picker (30) */
static inline void STGMCARD_checkInfo(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        if (saves->result == 1) {
            if (STGMCARD_funcs.infoBuf->magic != MEMCARD_FILE_MAGIC) {
                HEAP.zero(STGMCARD_funcs.infoBuf, 0x44);
                STGMCARD_funcs.infoBuf->magic = MEMCARD_FILE_MAGIC;
                STGMCARD_funcs.infoBuf->version = MEMCARD_SAVE_VERSION;
            } else if (MEMCARD_SYSTEM.funcs.computeChecksum(&STGMCARD_funcs.infoBuf->magic, sizeof(MemCardFile) - 4) & ~STGMCARD_funcs.infoBuf->checksum) {
                saves->result = 9;
                STGMCARD_showError(saves, win);
                return;
            } else {
                saves->file = *STGMCARD_funcs.infoBuf;
                STGMCARD_funcs.lastSlot = STGMCARD_funcs.infoBuf->last;
            }
            win->windows[0]->setVisible(win->windows[0], 0);
            win->windows[4]->setVisible(win->windows[4], 0);
            win->panel->reset(win->panel);
            saves->substate = 30;
        } else {
            STGMCARD_showError(saves, win);
        }
    }
}

/* Hides the messages and shows the slot picker and the details window */
static inline void STGMCARD_openSlots(MemCardSaves *saves, MemCardSavesWindows *win) {
    win->windows[0]->setVisible(win->windows[0], 0);
    win->windows[4]->setVisible(win->windows[4], 0);
    if (win->menu->slid != 5) {
        win->menu->reset(win->menu);
    }
    if (win->info != NULL) {
        win->info->show(win->info);
    }
    saves->substate++;
}

/* Slides the picker's header and slots in, then starts the pick at the last
   slot used */
static inline void STGMCARD_showSlots(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->menu->substate == 0) {
        if (win->menu->slid == 0) {
            win->menu->slideInHeader(win->menu);
        } else if (win->menu->slid == 1) {
            win->menu->slideInSlots(win->menu);
        } else if (win->menu->slid == 2) {
            win->menu->startPick(win->menu, STGMCARD_funcs.lastSlot);
            saves->substate++;
        }
    }
}

/* Once the picker waits, asks for the slot to save to or load from and shows its
   details */
static inline void STGMCARD_promptSlot(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->menu->substate == 6) {
        if (saves->screen->loading == 0) {
            win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 6);
        } else {
            win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x10);
        }
        win->info->drawing = 1;
        win->info->shown = 1;
        win->info->refresh(win->info);
        saves->nextSubstate(saves);
        win->menu->substate = 6;
    }
}

/* Waits for the slot: cross saves to it (asking first over a used one) or loads
   it, triangle closes the picker, and a card that fails its check shows its
   error */
static inline void STGMCARD_pickSlot(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    if (win->menu->substate == 6) {
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            saves->substate = 400;
            win->menu->substate = 0;
            if (saves->screen->loading == 0) {
                if (STGMCARD_funcs.infoBuf->saves[STGMCARD_funcs.slot].name[0] != 0) {
                    saves->step = 40;
                    saves->choice = 0;
                    win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 7);
                    win->windows[2]->setString(win->windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x16);
                    win->windows[3]->setString(win->windows[3], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x17);
                    win->cursor->setVisible(win->cursor, 1);
                    saves->choosing = 1;
                    win->cursor->setPos(win->cursor, 0xC2, saves->choice * 14 + 0xBD);
                } else {
                    saves->step = 50;
                }
            } else {
                saves->step = 70;
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            win->menu->substate = 0;
            saves->substate = 90;
            saves->step = 1;
        } else {
            status = MEMCARD_SYSTEM.funcs.check(saves->port);
            if (status != 0) {
                if (status != 1) {
                    saves->result = status - 1;
                    STGMCARD_closeMenuForError(saves, win);
                }
            }
        }
    }
}

/* Slides the slots and header out, then goes on to saves->step */
static inline void STGMCARD_closeSlots(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->menu->substate == 0) {
        switch (win->menu->slid) {
        case 3:
            win->menu->slideOutHeader(win->menu);
            saves->substate = saves->step;
            saves->step = saves->counter;
            saves->counter = 0;
            break;
        case 2:
            win->info->hide(win->info);
            win->menu->slideOutSlots(win->menu);
            break;
        }
    }
}

/* Slides the slots and header out before leaving the screen (601) */
static inline void STGMCARD_closeSlotsToLeave(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->menu->substate == 0) {
        if (win->menu->slid == 2) {
            win->info->hide(win->info);
            win->menu->slideOutSlots(win->menu);
        } else if (win->menu->slid == 3) {
            win->menu->slideOutHeader(win->menu);
            saves->substate++;
        }
    }
}

/* Loads from the selected slot: an empty one only tells so and goes back to the
   pick (501), else shows the loading message and starts the panel */
static inline void STGMCARD_startLoad(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (STGMCARD_funcs.infoBuf->saves[STGMCARD_funcs.slot].name[0] == 0) {
        win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x18);
        saves->prompting = 1;
        saves->substate = 501;
        saves->step = 32;
        saves->backToPick = 1;
    } else {
        win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x11);
        win->panel->start(win->panel, 1, MEMCARD_LOAD_FRAMES);
        saves->substate++;
    }
}

/* Reads the selected slot's save into the game: a wrong checksum, or another
   save version while loading, is error 8, and a failed read shows its error */
static inline void STGMCARD_loadSave(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.read(saves->port, STGMCARD_funcs.dataBuf, sizeof(GameSave), STGMCARD_funcs.slot + SAVE_SECTION_DATA);
    if (status != 0) {
        if (status == 1) {
            if (MEMCARD_SYSTEM.funcs.computeChecksum(&STGMCARD_funcs.dataBuf->bytes[SAVE_CHECKED], sizeof(GameSave) - SAVE_CHECKED) &
                ~STGMCARD_funcs.dataBuf->game.checksum) {
                saves->result = 8;
                STGMCARD_closeMenuForError(saves, win);
            } else if (STGMCARD_funcs.dataBuf->game.version != MEMCARD_SAVE_VERSION && saves->screen->loading != 0) {
                saves->result = 8;
                STGMCARD_closeMenuForError(saves, win);
            } else {
                GAME_SAVE = STGMCARD_funcs.dataBuf->save;
                win->panel->start(win->panel, 2, 0x14);
                saves->substate = 500;
                win->menu->substate = 0;
                saves->backToPick = 0;
            }
        } else {
            saves->result = status - 1;
            STGMCARD_closeMenuForError(saves, win);
        }
    }
}

/* Hides the two choices and their cursor */
static inline void STGMCARD_hideChoices(MemCardSavesWindows *win) {
    win->windows[2]->setVisible(win->windows[2], 0);
    win->windows[3]->setVisible(win->windows[3], 0);
    win->cursor->setVisible(win->cursor, 0);
}

/* Moves the cursor between the two choices (saves->choice) */
static inline void STGMCARD_moveChoiceCursor(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 prev;

    prev = saves->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        saves->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        saves->choice = 1;
    }
    if (prev != saves->choice) {
        win->cursor->setPos(win->cursor, 0xC2, saves->choice * 14 + 0xBD);
        SOUND.playSound(SOUND_CURSOR);
    }
}

/* Yes to saving over the slot saves the game there (50), no goes back to the pick */
static inline void STGMCARD_answerOverwrite(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (saves->choice == 0) {
        saves->step = 50;
    } else {
        saves->step = 32;
        win->menu->substate = 6;
    }
}

/* Without cross on the question to save over the slot: triangle goes back to the
   pick, and a card that fails its check shows its error */
static inline void STGMCARD_cancelOverwrite(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 result;

    if (PAD_PRESSED(PAD_TRIANGLE)) {
        STGMCARD_hideChoices(win);
        SOUND.playSound(SOUND_MENU_CANCEL);
        saves->substate = 400;
        saves->choosing = 0;
        saves->step = 32;
        win->menu->substate = 6;
    } else {
        result = MEMCARD_SYSTEM.funcs.check(saves->port);
        if (result != 0) {
            if (result != 1) {
                STGMCARD_hideChoices(win);
                saves->choosing = 0;
                saves->result = result - 1;
                STGMCARD_closeMenuForError(saves, win);
            }
        }
    }
}

/* Shows the saving message and starts the panel */
static inline void STGMCARD_startSave(MemCardSaves *saves, MemCardSavesWindows *win) {
    win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 8);
    win->panel->start(win->panel, 1, MEMCARD_SAVE_FRAMES);
    saves->substate++;
}

/* Copies the game to the save buffer with its checksum, fills the slot's summary
   (name, place, money, play time and the party's levels and partners), and
   makes it the last slot */
static inline void STGMCARD_fillSave(MemCardSaves *saves) {
    MemCardSave *save;
    s32 i;
    s32 member;

    save = &STGMCARD_funcs.infoBuf->saves[STGMCARD_funcs.slot];
    STGMCARD_funcs.dataBuf->save = GAME_SAVE;
    STGMCARD_funcs.dataBuf->game.checksum =
        MEMCARD_SYSTEM.funcs.computeChecksum(&STGMCARD_funcs.dataBuf->bytes[SAVE_CHECKED], sizeof(GameSave) - SAVE_CHECKED);
    STGMCARD_funcs.dataBuf->game.version = MEMCARD_SAVE_VERSION;
    strcpy(save->name, STGMCARD_funcs.dataBuf->game.name);
    save->area = saves->screen->area;
    save->place = saves->screen->place;
    save->money = STGMCARD_funcs.dataBuf->game.money;
    save->time = STGMCARD_funcs.dataBuf->game.playTime;
    for (i = 0; i < 3; i++) {
        member = GAME.funcs.getPartyMember(i);
        save->levels[i] = STGMCARD_funcs.dataBuf->game.partners[member].info.stats[STAT_LEVEL];
        save->partners[i] = STGMCARD_funcs.dataBuf->game.partners[member].unlocked;
    }
    STGMCARD_funcs.infoBuf->last = STGMCARD_funcs.slot;
    STGMCARD_funcs.infoBuf->checksum = MEMCARD_SYSTEM.funcs.computeChecksum(&STGMCARD_funcs.infoBuf->magic, sizeof(MemCardFile) - 4);
    saves->substate++;
}

/* Writes the info section with the slot's new summary */
static inline void STGMCARD_writeInfo(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.write(saves->port, STGMCARD_funcs.infoBuf, sizeof(MemCardFile), SAVE_SECTION_INFO);
    if (status != 0) {
        if (status == 1) {
            saves->substate++;
        } else {
            saves->result = status - 1;
            STGMCARD_closeMenuForError(saves, win);
        }
    }
}

/* Saves the game to the selected slot's section, then ends the panel (500) */
static inline void STGMCARD_writeSave(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.write(saves->port, STGMCARD_funcs.dataBuf, sizeof(GameSave), STGMCARD_funcs.slot + SAVE_SECTION_DATA);
    if (status != 0) {
        if (status == 1) {
            win->panel->start(win->panel, 2, 0x14);
            saves->substate = 500;
        } else {
            saves->result = status - 1;
            STGMCARD_closeMenuForError(saves, win);
        }
    }
}

/* Once the panel is done, tells that the game was saved (501) or loaded (502) */
static inline void STGMCARD_showDone(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        if (saves->screen->loading == 0) {
            saves->file = *STGMCARD_funcs.infoBuf;
            saves->refresh(saves);
            win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 9);
            saves->step = 32;
        } else {
            win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x12);
            saves->step = 400;
            saves->substate++;
        }
        saves->substate++;
        win->panel->reset(win->panel);
        saves->prompting = 1;
    }
}

/* Waits for cross on the message (saved, or an empty slot to load): goes back
   to the pick, or after loading on to leaving (600); a card that fails its
   check shows error 1 */
static inline void STGMCARD_confirmMessage(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 check;

    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        saves->prompting = 0;
        saves->substate = saves->step;
        if (saves->screen->loading == 0) {
            saves->setStep(saves, 0);
            win->menu->substate = 6;
        } else {
            win->windows[1]->setVisible(win->windows[1], 0);
            saves->step = 600;
            if (saves->backToPick != 0) {
                win->menu->substate = 6;
            } else {
                win->menu->substate = 0;
            }
        }
    } else {
        check = MEMCARD_SYSTEM.funcs.check(saves->port);
        if (check != 0) {
            if (check != 1) {
                saves->prompting = 0;
                saves->result = 1;
                STGMCARD_closeMenuForError(saves, win);
            }
        }
    }
}

/* Waits for cross on the loaded message, then waits for the card (400) and leaves
   the screen (600) */
static inline void STGMCARD_waitLoadedMessage(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        saves->prompting = 0;
        saves->substate = saves->step;
        win->windows[1]->setVisible(win->windows[1], 0);
        saves->step = 600;
        win->menu->substate = 0;
    }
}

/* Shows the error STGMCARD_errorTexts[result] with the port: when saving, an
   unformatted card (4) asks to format it and a card without the save file (5)
   asks to create it, unless the card lacks the room for it (7); other errors
   wait for cross */
static inline void STGMCARD_askError(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 blocks;
    s32 j;
    s32 ask;

    win->windows[0]->setString(win->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), STGMCARD_errorTexts[saves->result]);
    STGMCARD_showPort(saves, win, 1);
    if (saves->screen->loading == 0) {
        if (saves->result == CARD_ERR_UNFORMATTED) {
            saves->choice = 1;
            ask = 1;
        } else if (saves->result == CARD_ERR_NO_FILE) {
            if (MEMCARD.fileCount != 0) {
                for (j = 0, blocks = 0; j < MEMCARD.fileCount; j++) {
                    blocks += MEMCARD.files[j].size / 0x2000;
                }
                if (blocks + 4 >= 16) {
                    win->windows[0]->setVisible(win->windows[0], 0);
                    saves->result = CARD_ERR_FULL;
                    saves->substate = 100;
                    return;
                }
            }
            ask = 1;
        } else {
            saves->prompting = 1;
            ask = 0;
            if (saves->result == CARD_ERR_FULL) {
                win->windows[0]->setNumber(win->windows[0], 1, 4);
            }
        }
        if (ask) {
            if (saves->result == CARD_ERR_UNFORMATTED) {
                win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x19);
            } else {
                win->windows[1]->setString(win->windows[1], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x1A);
            }
            win->windows[2]->setString(win->windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x16);
            win->windows[3]->setString(win->windows[3], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x17);
            win->cursor->setVisible(win->cursor, 1);
            saves->choosing = 1;
            win->cursor->setPos(win->cursor, 0xC2, saves->choice * 14 + 0xBD);
        }
    } else {
        saves->prompting = 1;
    }
    saves->substate++;
}

/* Waits for the answer to the error: yes formats the card (110) or creates the
   save file (120), no or triangle goes back to the port pick, and a card that
   fails its check shows its error; other errors wait for cross */
static inline void STGMCARD_answerError(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 prev;
    s32 status;

    if (saves->screen->loading == 0 && (u32)(saves->result - CARD_ERR_UNFORMATTED) < 2) {
        prev = saves->choice;
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            saves->choice = 0;
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            saves->choice = 1;
        }
        if (prev != saves->choice) {
            SOUND.playSound(SOUND_CURSOR);
            win->cursor->setPos(win->cursor, 0xC2, saves->choice * 14 + 0xBD);
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            win->windows[0]->setVisible(win->windows[0], 0);
            win->windows[4]->setVisible(win->windows[4], 0);
            win->windows[1]->setVisible(win->windows[1], 0);
            win->windows[2]->setVisible(win->windows[2], 0);
            win->windows[3]->setVisible(win->windows[3], 0);
            win->cursor->setVisible(win->cursor, 0);
            saves->substate = 400;
            saves->choosing = 0;
            if (saves->result == CARD_ERR_UNFORMATTED) {
                if (saves->choice == 0) {
                    saves->step = 110;
                    win->windows[0]->setString(win->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x1E);
                    STGMCARD_showPort(saves, win, 1);
                    win->panel->start(win->panel, 1, 0x90);
                } else {
                    saves->step = 1;
                    win->panel->reset(win->panel);
                }
            } else if (saves->choice == 0) {
                saves->step = 120;
                win->windows[0]->setString(win->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 0x1F);
                STGMCARD_showPort(saves, win, 1);
                win->panel->start(win->panel, 1, 0x90);
            } else {
                saves->step = 1;
                win->panel->reset(win->panel);
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            win->windows[0]->setVisible(win->windows[0], 0);
            win->windows[4]->setVisible(win->windows[4], 0);
            win->windows[1]->setVisible(win->windows[1], 0);
            win->windows[2]->setVisible(win->windows[2], 0);
            win->windows[3]->setVisible(win->windows[3], 0);
            win->cursor->setVisible(win->cursor, 0);
            saves->substate = 400;
            saves->choosing = 0;
            saves->step = 1;
            win->panel->reset(win->panel);
        }
        status = MEMCARD_SYSTEM.funcs.check(saves->port);
        if (status != 0) {
            if (status != 1) {
                saves->result = status;
                saves->choosing = 0;
                STGMCARD_showError(saves, win);
            }
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        saves->substate = 400;
        saves->prompting = 0;
        saves->step = 1;
    }
}

/* Shows the error of a card that failed its check, then goes on to saves->step */
static inline void STGMCARD_endWaitCard(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (saves->result != 1) {
        STGMCARD_showError(saves, win);
    }
    saves->substate = saves->step;
    saves->step = saves->counter;
    saves->counter = 0;
}

/* Formats the card, then ends the panel (111) */
static inline void STGMCARD_formatCard(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.format(saves->port);
    if (status != 0) {
        if (status == 1) {
            win->panel->start(win->panel, 2, 0x14);
            saves->substate++;
        } else {
            STGMCARD_showError(saves, win);
        }
    }
}

/* Once the panel is done, asks to create the save file (error 5) */
static inline void STGMCARD_endFormat(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        STGMCARD_showError(saves, win);
        saves->result = 5;
    }
}

/* Creates the save file on the card */
static inline void STGMCARD_createFile(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.create(saves->port);
    if (status != 0) {
        if (status == 1) {
            saves->substate++;
        } else {
            STGMCARD_showError(saves, win);
        }
    }
}

/* Writes the new save file's header; a wrong icon count is error 3 */
static inline void STGMCARD_writeHeader(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.write(saves->port, &MEMCARD.header, sizeof(CardHeader), SAVE_SECTION_HEADER);
    if (status != 0) {
        if (status == 1) {
            if ((u32)(MEMCARD.iconCount - 1) >= SAVE_MAX_ICONS) {
                saves->result = 3;
                STGMCARD_showError(saves, win);
            } else {
                MEMCARD.iconIndex = 0;
                saves->substate++;
            }
        } else {
            saves->result = 10;
            STGMCARD_showError(saves, win);
        }
    }
}

/* Writes the new save file's icon frames, one per call, then makes its empty
   info section */
static inline void STGMCARD_writeIcons(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    /* each icon frame is the sector after the header's or the frame before's,
       at the offset in bits 8 and up */
    status = saves->result = MEMCARD_SYSTEM.funcs.write(saves->port, MEMCARD.icons[MEMCARD.iconIndex], CARD_SECTOR_SIZE, (MEMCARD.iconIndex * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE) << 8 | SAVE_SECTION_HEADER);
    if (status != 0) {
        if (status == 1) {
            MEMCARD.iconIndex++;
            if (MEMCARD.iconIndex > MEMCARD.iconCount - 1) {
                HEAP.zero(STGMCARD_funcs.infoBuf, sizeof(MemCardFile));
                STGMCARD_funcs.infoBuf->magic = MEMCARD_FILE_MAGIC;
                STGMCARD_funcs.infoBuf->version = MEMCARD_SAVE_VERSION;
                STGMCARD_funcs.infoBuf->checksum = MEMCARD_SYSTEM.funcs.computeChecksum(&STGMCARD_funcs.infoBuf->magic, sizeof(MemCardFile) - 4);
                saves->file = *STGMCARD_funcs.infoBuf;
                STGMCARD_funcs.lastSlot = STGMCARD_funcs.infoBuf->last;
                saves->substate++;
            }
        } else {
            saves->result = 11;
            STGMCARD_showError(saves, win);
        }
    }
}

/* Writes the new save file's info section, then ends the panel (125) */
static inline void STGMCARD_writeNewInfo(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    status = saves->result = MEMCARD_SYSTEM.funcs.write(saves->port, STGMCARD_funcs.infoBuf, sizeof(MemCardFile), SAVE_SECTION_INFO);
    if (status != 0) {
        win->panel->start(win->panel, 2, 0x14);
        saves->substate++;
    }
}

/* Once the panel is done, opens the slot picker (30) or shows the error */
static inline void STGMCARD_endCreate(MemCardSaves *saves, MemCardSavesWindows *win) {
    if (win->panel->done != 0) {
        win->panel->reset(win->panel);
        if (saves->result == 1) {
            saves->substate = 30;
        } else {
            STGMCARD_showError(saves, win);
        }
    }
}

/*
 * The save list's states (saves->substate): picks the port, reads the card's
 * info section, lets the menu pick a slot, then loads or saves it. A failed
 * operation leaves its error in saves->result and goes to 100, which shows
 * STGMCARD_errorTexts[result] and can format the card (110) or create the save
 * file (120). 400 waits for the card and goes on to step.
 * Match depends on status, which 40 and 400 share.
 */
void STGMCARD_runSaves(MemCardSaves *saves, MemCardSavesWindows *win) {
    s32 status;

    switch (saves->substate) {
    case 0:
    default:
        STGMCARD_openSaves(saves, win);
        /* fallthrough */
    case 1:
        STGMCARD_slideInTitle(saves, win);
        break;
    case 2:
        STGMCARD_pickPort(saves, win);
        break;
    case 10:
        STGMCARD_acceptCard(saves, win);
        break;
    case 11:
        STGMCARD_listCard(saves, win);
        break;
    case 12:
        STGMCARD_endCardCheck(saves, win);
        break;
    case 20:
        STGMCARD_startInfoRead(saves, win);
        /* fallthrough */
    case 21:
        STGMCARD_readInfo(saves, win);
        break;
    case 22:
        STGMCARD_checkInfo(saves, win);
        break;
    case 30:
        STGMCARD_openSlots(saves, win);
        break;
    case 31:
        STGMCARD_showSlots(saves, win);
        break;
    case 32:
        STGMCARD_promptSlot(saves, win);
        break;
    case 33:
        STGMCARD_pickSlot(saves, win);
        break;
    case 90:
        STGMCARD_closeSlots(saves, win);
        break;
    case 600:
        STGMCARD_closeSlotsToLeave(saves, win);
        break;
    case 601:
        saves->hide(saves);
        break;
    case 70:
        STGMCARD_startLoad(saves, win);
        break;
    case 71:
        STGMCARD_loadSave(saves, win);
        break;
    case 40:
        STGMCARD_moveChoiceCursor(saves, win);
        if (PAD_PRESSED(PAD_CROSS)) {
            STGMCARD_hideChoices(win);
            SOUND.playSound(SOUND_SELECT);
            /* match depends on the 400 going through status */
            status = 400;
            saves->choosing = 0;
            saves->substate = status;
            STGMCARD_answerOverwrite(saves, win);
        } else {
            STGMCARD_cancelOverwrite(saves, win);
        }
        break;
    case 50:
        STGMCARD_startSave(saves, win);
        break;
    case 51:
        STGMCARD_fillSave(saves);
        break;
    case 52:
        STGMCARD_writeInfo(saves, win);
        break;
    case 53:
        STGMCARD_writeSave(saves, win);
        break;
    case 500:
        STGMCARD_showDone(saves, win);
        break;
    case 501:
        STGMCARD_confirmMessage(saves, win);
        break;
    case 502:
        STGMCARD_waitLoadedMessage(saves, win);
        break;
    case 100:
        STGMCARD_askError(saves, win);
        break;
    case 101:
        STGMCARD_answerError(saves, win);
        break;
    case 400:
        status = saves->result = MEMCARD_SYSTEM.funcs.check(saves->port);
        if (status != 0) {
            saves->substate++;
        }
        break;
    case 401:
        STGMCARD_endWaitCard(saves, win);
        break;
    case 110:
        STGMCARD_formatCard(saves, win);
        break;
    case 111:
        STGMCARD_endFormat(saves, win);
        break;
    case 120:
        saves->substate = 121;
        break;
    case 121:
        STGMCARD_createFile(saves, win);
        break;
    case 122:
        STGMCARD_writeHeader(saves, win);
        break;
    case 123:
        STGMCARD_writeIcons(saves, win);
        break;
    case 124:
        STGMCARD_writeNewInfo(saves, win);
        break;
    case 125:
        STGMCARD_endCreate(saves, win);
        break;
    }
}

/* Draws the save list's sprites: the blinking cross button while a message
   waits for it, the list's frame and the box of the cursor's two choices */
void STGMCARD_drawSaves(MemCardSaves *saves) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(saves->layer, 2);
    sprite.setTexture(0x140, 0);
    if (saves->prompting != 0) {
        if ((GFX.funcs.getTime() - saves->blinkTime) / 3 != 0) {
            saves->blinkTime = GFX.funcs.getTime();
            saves->blinkFrame++;
            if (saves->blinkFrame >= 5) {
                saves->blinkFrame = 0;
            }
        }
        sprite.setClutRow(saves->blinkFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 10, 294, 208);
    }
    sprite.setTexture(0x280, 0);
    sprite.setClutRow(0);
    sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 33, saves->origin[0] + saves->slide[0].value, saves->origin[1] + saves->slide[1].value);
    if (saves->choosing != 0) {
        sprite.setLayerId(saves->layer, 1);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 29, 188, 185);
    }
}

/* The save list's task: creates its windows, then runs it */
void STGMCARD_updateSaves(MemCardSaves *saves, MemCardSavesWindows *win) {
    switch (saves->state) {
    case TASK_INIT:
    default:
        saves->nextState(saves);
        saves->origin[0] = 5;
        saves->origin[1] = 89;
        STGMCARD_funcs.startLerp(&saves->slide[1], 151, 0, 10);
        win->title = createTextWindow(saves->layer, 1, saves->origin[0] + 200, saves->origin[1] + (s16)(saves->slide[1].value + 9));
        win->windows[4] = createTextWindow(saves->layer, 1, saves->origin[0] + 15, saves->origin[1] + 24);
        win->windows[0] = createTextWindow(saves->layer, 1, saves->origin[0] + 15, saves->origin[1] + 40);
        win->windows[0]->setLines(win->windows[0], 5);
        win->windows[0]->setDepth(win->windows[0], 1);
        win->windows[1] = createTextWindow(saves->layer, 1, saves->origin[0] + 15, saves->origin[1] + 103);
        win->windows[1]->setLines(win->windows[1], 2);
        win->windows[2] = createTextWindow(saves->layer, 1, 207, 189);
        win->windows[3] = createTextWindow(saves->layer, 1, 207, 203);
        win->cursor = createCursor(saves->layer, 0, 207, 189);
        win->cursor->setVisible(win->cursor, 0);
        win->menu = STGMCARD_createMenu(saves);
        break;
    case TASK_RUN:
        STGMCARD_runSaves(saves, win);
        STGMCARD_drawSaves(saves);
        break;
    case TASK_DONE:
        switch (saves->substate) {
        case 0:
        default:
            win->title->setVisible(win->title, 0);
            saves->substate++;
            break;
        case 1:
            break;
        }
        STGMCARD_drawSaves(saves);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the save list (task) of the main task */
MemCardSaves *STGMCARD_createSaves(MemCardScreen *screen) {
    MemCardSaves *saves = createTask(STGMCARD_updateSaves, sizeof(MemCardSaves), sizeof(MemCardSavesWindows));

    saves->refresh = STGMCARD_refreshSaves;
    saves->hide = STGMCARD_hideSaves;
    saves->screen = screen;
    saves->layer = SCREEN_LAYER;
    return saves;
}
