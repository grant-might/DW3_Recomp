/*
 * The parts of a list menu (StageListMenu), up to eight options that each
 * open a message: WSTAG210's guide (event 8), WSTAG924's on the starters'
 * digivolutions and on raising (events 1602 and 1604) and WSTAG935's on card
 * battles and cards (1616 and 1618). The stage defines MENU_TEXT, the menus'
 * text file, before it includes this.
 */

/* Creates the menu's eight option lines, its hidden cursor and its message box */
static inline void createListMenu(StageListMenu *task, StageListMenuChildren *children) {
    s32 i;

    task->nextState(task);
    task->tweens[0].duration = 10;
    task->tweens[1].duration = 10;
    for (i = 0; i < 8; i++) {
        children->options[i] = createTextWindow(FIELD_LAYER_MAP, 1, 0xBD, 0x21 + i * 14);
        children->options[i]->setDepth(children->options[i], 1);
    }
    children->cursor = createCursor(FIELD_LAYER_MAP, 1, 0xAF, 0x21);
    children->cursor->setVisible(children->cursor, 0);
    children->message = createTextWindow(FIELD_LAYER_MAP, 1, 0x12, 0xB0);
    children->message->setLines(children->message, 3);
    task->count = 8;
}

/*
 * Writes the options, lines 1 on of the menus' text entry n (each option's
 * message is a line from 9 on), and shows the cursor
 */
static inline void showListMenuOptions(StageListMenu *task, StageListMenuChildren *children, s32 n) {
    s32 i;

    for (i = 0; i < task->count; i++) {
        children->options[i]->setString(children->options[i], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, n)), i + 1);
    }
    children->cursor->setVisible(children->cursor, 1);
}

/* Moves the cursor up or down the list, held buttons repeating, without leaving it */
static inline void moveListMenuCursor(StageListMenu *task) {
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--task->cursor < 0) {
            task->cursor = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        task->cursor++;
        if (task->cursor > task->count - 1) {
            task->cursor = task->count - 1;
        }
    }
}

/* Starts typing the message of the option under the cursor, from the menus' text entry n */
static inline void openListMenuMessage(StageListMenu *task, StageListMenuChildren *children, s32 n) {
    children->message->setString(children->message, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, n)), task->cursor + 9);
    children->message->setTypeDelay(children->message, 6);
}

/*
 * Pages through the option's message with X, the arrow blinking while a page
 * waits for it, and moves on to the next substate once the message is done
 */
static inline void readListMenuMessage(StageListMenu *task, StageListMenuChildren *children) {
    if (children->message->isFinished(children->message)) {
        task->substate++;
    } else if (children->message->isWaitingForButton(children->message)) {
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            task->showArrow = 0;
        } else {
            task->showArrow = 1;
        }
    } else if (PAD_PRESSED(PAD_CROSS)) {
        children->message->showPage(children->message);
    }
}

/* Hides the options and the cursor and starts closing the list's window */
static inline void closeListMenu(StageListMenu *task, StageListMenuChildren *children) {
    s32 i;

    for (i = 0; i < task->count; i++) {
        children->options[i]->setVisible(children->options[i], 0);
    }
    children->cursor->setVisible(children->cursor, 0);
    stageFuncs.start(&task->tweens[0], 0);
    task->substate++;
}
