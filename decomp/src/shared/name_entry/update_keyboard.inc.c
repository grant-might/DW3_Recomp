/* The keyboard's input: moving the cursor, turning the pages and typing the name */
void OVL_NAME(updateKeyboard)(NameEntry *task, NameEntryWindows *windows) {
    s32 page;
    s32 newPage;
    s32 i;
    s32 key;
    s32 j;
    u16 c;
    u32 glyph; /* the match depends on this u32 copy of c, which orders the loads of the key's glyph */

    switch (task->substate) {
    case 0:
    default:
        OVL_NAME(startTween)(&task->keyboardScale, 1);
        task->substate++;
        break;
    case 1:
        if (OVL_NAME(updateTween)(&task->keyboardScale)) {
            OVL_NAME(showNameWindows)(task, windows, 1);
            task->active = 1;
            task->substate++;
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_START)) {
            SOUND.playSound(SOUND_MENU_MOVE);
            task->column = 13;
            task->row = 6;
            break;
        }
        if (OVL_NAME(keyboard).pageCount >= 2) {
            page = task->page;
            if (!PAD_HELD(PAD_R1) && PAD_PRESSED(PAD_L1)) {
                if (--task->page < 0) {
                    task->page = OVL_NAME(keyboard).pageCount - 1;
                }
            } else if (!PAD_HELD(PAD_L1) && PAD_PRESSED(PAD_R1)) {
                if (++task->page > OVL_NAME(keyboard).pageCount - 1) {
                    task->page = 0;
                }
            }
            if (page != task->page) {
                SOUND.playSound(SOUND_MENU_MOVE);
                OVL_NAME(showNameWindows)(task, windows, 1);
                if (NAME_ENTRY_JAPANESE) {
                    newPage = task->page;
                    if (newPage == 0 || newPage == 1) {
                        while (OVL_NAME(keyboard).pages[newPage].cells[task->row][task->column].kind != 1) {
                            if (--task->column < 0) {
                                task->column = 14;
                            }
                        }
                    } else {
                        while (OVL_NAME(keyboard).pages[newPage].cells[task->row][task->column].kind == 0) {
                            if (--task->row < 0) {
                                task->row = 6;
                            }
                        }
                    }
                }
            }
        }
        if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
            if (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind < 0) {
                task->column += OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind;
            }
            do {
                if (--task->column < 0) {
                    task->column = 14;
                }
            } while (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind != 1);
            SOUND.playSound(SOUND_MENU_MOVE);
        } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
            if (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind < 0) {
                task->column += OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind;
            }
            do {
                if (++task->column >= 15) {
                    task->column = 0;
                }
            } while (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind != 1);
            SOUND.playSound(SOUND_MENU_MOVE);
        }
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            do {
                if (--task->row < 0) {
                    task->row = 6;
                }
            } while (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind == 0);
            SOUND.playSound(SOUND_MENU_MOVE);
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            do {
                if (++task->row >= 7) {
                    task->row = 0;
                }
            } while (OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].kind == 0);
            SOUND.playSound(SOUND_MENU_MOVE);
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            for (i = 0; OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column + i].kind != 1; i--) {
            }
            key = task->row * 15 + task->column + i;
            SOUND.playSound(SOUND_MENU_CONFIRM);
            switch (key) {
            case NAME_KEY_LEFT:
                if (--task->cursor < 0) {
                    task->cursor = 0;
                }
                break;
            case NAME_KEY_RIGHT:
                if (++task->cursor > task->maxLength - 1) {
                    task->cursor = task->maxLength - 1;
                }
                break;
            case NAME_KEY_DELETE:
                if (task->cursor <= task->maxLength - 1 && task->name[task->cursor] == SJIS_SPACE) {
                    if (--task->cursor < 0) {
                        task->cursor = 0;
                    }
                }
                c = windows->name->style->iconMap[1].code;
                task->name[task->cursor] = (c >> 8) | ((c & 0xFF) << 8);
                windows->name->setText(windows->name, task->name);
                break;
            case NAME_KEY_SPACE:
                c = windows->name->style->iconMap[1].code;
                task->name[task->cursor] = (c >> 8) | ((c & 0xFF) << 8);
                if (++task->cursor > task->maxLength - 1) {
                    task->cursor = task->maxLength - 1;
                }
                windows->name->setText(windows->name, task->name);
                break;
            case NAME_KEY_END:
                for (j = 0; j < task->maxLength; j++) {
                    if (task->name[j] != SJIS_SPACE && task->name[j] != 0) {
                        for (j = 19; j >= 0; j--) {
                            if (task->name[j] != SJIS_SPACE) {
                                task->substate = 100;
                                return;
                            }
                            task->name[j] = 0;
                        }
                    }
                }
                task->substate = 20;
                break;
            default:
                glyph = windows->name->style->sjisMap[OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column].code].code;
                task->name[task->cursor] = (glyph >> 8) | ((glyph & 0xFF) << 8);
                windows->name->setText(windows->name, task->name);
                if (++task->cursor > task->maxLength - 1) {
                    task->cursor = task->maxLength - 1;
                    task->column = 13;
                    task->row = 6;
                }
                break;
            }
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            if (task->cursor <= task->maxLength - 1 && task->name[task->cursor] == SJIS_SPACE) {
                if (--task->cursor < 0) {
                    task->cursor = 0;
                }
            }
            c = windows->name->style->iconMap[1].code;
            task->name[task->cursor] = (c >> 8) | ((c & 0xFF) << 8);
            windows->name->setText(windows->name, task->name);
        }
        break;
    case 10:
        OVL_NAME(showNameWindows)(task, windows, 0);
        OVL_NAME(startTween)(&task->keyboardScale, 0);
        task->active = 0;
        task->substate++;
        break;
    case 11:
        if (OVL_NAME(updateTween)(&task->keyboardScale)) {
            task->state = TASK_DONE;
        }
        break;
    case 20:
        task->active = 0;
        OVL_NAME(startTween)(&task->messageScale, 1);
        task->substate++;
        break;
    case 21:
        if (OVL_NAME(updateTween)(&task->messageScale)) {
            windows->message->setString(windows->message, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0x12);
            task->substate++;
        }
        break;
    case 22:
        if (PAD_PRESSED(PAD_CROSS)) {
            windows->message->setVisible(windows->message, 0);
            OVL_NAME(startTween)(&task->messageScale, 0);
            task->substate++;
        }
        break;
    case 23:
        if (OVL_NAME(updateTween)(&task->messageScale)) {
            task->active = 1;
            task->substate = 2;
        }
        break;
    case 100:
        break;
    }
}
