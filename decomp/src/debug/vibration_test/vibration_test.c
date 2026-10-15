#include "debug/vibration_test.h"

ShockTestRow SHOCKTST_menuRows[4] = {
    {{1, 0}, {1, 0, 0, 0}},
    {{1, 1}, {2, 3, 6, 7}},
    {{1, 1}, {4, 5, 8, 9}},
    {{1, 0}, {10, 0, 11, 0}},
};

/* The strings of the pattern, time and power windows (setString), in font
   codes: the label, icon 7 (01 07), then control code 5 (02 05 01), which
   inserts work buffer 1, where setNumber writes the value */
char SHOCKTST_numberFormats[3][0x40] = {
    "\xC3\xB1\xE8\xE3\x01\x07\x02\x05\x01",
    "\xC7\xDE\xE8\xD2\x01\x07\x02\x05\x01",
    "\x65\x89\x56\x01\x07\x02\x05\x01",
};

#define SCENE_TASK Task
#define SCENE_CHILD Task
#define SCENE_CREATE OVL_NAME(createLoader)
#define SCENE_PRIM_BUFFERS 0x5000
#define SCENE_OT_SHIFT 1
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

/* Colors the editor's windows: palette 3 for the one under the cursor
   (highlight 1-5 and 10), 1 for the one being edited (0 and 6-9, 11) */
void SHOCKTST_highlight(ShockTest *task, ShockTestWindows *win, s32 highlight) {
    s32 i;

    win->pattern->setPalette(win->pattern, PALETTE_WHITE);
    for (i = 0; i < 2; i++) {
        win->times[i]->setPalette(win->times[i], PALETTE_WHITE);
        win->powers[i]->setPalette(win->powers[i], PALETTE_WHITE);
    }
    win->play->setPalette(win->play, PALETTE_WHITE);
    switch (highlight) {
    default:
        win->pattern->setPalette(win->pattern, PALETTE_YELLOW);
        break;
    case 2:
        win->times[0]->setPalette(win->times[0], PALETTE_YELLOW);
        break;
    case 3:
        win->times[1]->setPalette(win->times[1], PALETTE_YELLOW);
        break;
    case 4:
        win->powers[0]->setPalette(win->powers[0], PALETTE_YELLOW);
        break;
    case 5:
        win->powers[1]->setPalette(win->powers[1], PALETTE_YELLOW);
        break;
    case 10:
        win->play->setPalette(win->play, PALETTE_YELLOW);
        break;
    case 0:
        win->pattern->setPalette(win->pattern, PALETTE_BLUE);
        break;
    case 6:
        win->times[0]->setPalette(win->times[0], PALETTE_BLUE);
        break;
    case 7:
        win->times[1]->setPalette(win->times[1], PALETTE_BLUE);
        break;
    case 8:
        win->powers[0]->setPalette(win->powers[0], PALETTE_BLUE);
        break;
    case 9:
        win->powers[1]->setPalette(win->powers[1], PALETTE_BLUE);
        break;
    case 11:
        win->play->setPalette(win->play, PALETTE_BLUE);
        break;
    }
}

/* Picks the pattern with up and down: 1 on cross, -1 on triangle */
s32 SHOCKTST_selectPattern(ShockTest *task, ShockTestWindows *win) {
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        if (--task->pattern < 0) {
            task->pattern = 0;
        }
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        if (++task->pattern > task->count - 1) {
            task->pattern = task->count - 1;
        }
    }
    win->pattern->setNumber(win->pattern, 1, task->pattern);
    SHOCKTST_showPattern(task, win, task->pattern);
    if (PAD_PRESSED(PAD_CROSS)) {
        return 1;
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        return -1;
    }
    return 0;
}

/* Shows a pattern's number and each motor's time and power */
void SHOCKTST_showPattern(ShockTest *task, ShockTestWindows *win, s32 pattern) {
    s32 i;

    win->pattern->setNumber(win->pattern, 1, pattern);
    for (i = 0; i < 2; i++) {
        win->powers[i]->setNumber(win->powers[i], 1, task->steps[i][pattern].power);
        win->times[i]->setNumber(win->times[i], 1, task->steps[i][pattern].time);
    }
}

/* Shows each motor's power and the time it has left */
void SHOCKTST_showTimers(ShockTest *task, ShockTestWindows *win, s32 pattern) {
    s32 i;

    for (i = 0; i < 2; i++) {
        win->powers[i]->setNumber(win->powers[i], 1, task->steps[i][pattern].power);
        win->times[i]->setNumber(win->times[i], 1, task->timers[i]);
    }
}

/* Plays a pattern on both motors, one frame on: 1 once both have stopped
   or triangle stops them */
s32 SHOCKTST_playPattern(ShockTest *task, ShockTestWindows *win, s32 pattern) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (task->motors[i] == 0) {
            if (task->steps[i][pattern].power != 0 && task->steps[i][pattern].time != 0) {
                PAD.setVibration(0, i, task->steps[i][pattern].time, task->steps[i][pattern].power);
                task->timers[i] = task->steps[i][pattern].time;
                task->motors[i] = 1;
            } else {
                task->motors[i] = -1;
            }
            win->powers[i]->setNumber(win->powers[i], 1, task->steps[i][pattern].power);
            win->times[i]->setNumber(win->times[i], 1, task->steps[i][pattern].time);
        } else if (task->motors[i] == 1) {
            if (--task->timers[i] < 0) {
                task->timers[i] = 0;
                task->motors[i] = -1;
            }
        }
    }
    if ((task->motors[0] == -1 && task->motors[1] == -1) ||
        PAD_PRESSED(PAD_TRIANGLE)) {
        for (i = 0; i < 2; i++) {
            PAD.setVibration(0, i, 0, 0);
        }
        SHOCKTST_showPattern(task, win, task->pattern);
        return 1;
    }
    SHOCKTST_showTimers(task, win, task->pattern);
    return 0;
}

/*
 * Plays the patterns from task->playing to task->count in turn, the next one
 * once a pattern ends or both motors stop, and returns 1 after the last. The
 * match depends on the reset being written in both branches.
 */
s32 SHOCKTST_playAllPatterns(ShockTest *task, ShockTestWindows *win) {
    do {
        SHOCKTST_showPattern(task, win, task->playing);
        if (SHOCKTST_playPattern(task, win, task->playing) == 0) {
            if (task->motors[0] != -1 || task->motors[1] != -1) {
                SHOCKTST_showTimers(task, win, task->playing);
                return 0;
            }
            task->timers[0] = task->timers[1] = 0;
            task->motors[0] = task->motors[1] = 0;
            task->playing++;
        } else {
            task->timers[0] = task->timers[1] = 0;
            task->motors[0] = task->motors[1] = 0;
            task->playing++;
        }
    } while (task->playing < task->count);
    SHOCKTST_showPattern(task, win, task->pattern);
    return 1;
}

/* Moves the editor's cursor over its rows and columns, skipping the empty
   ones: 1 when cross picks a value, -1 when it picks the play row, 2 on circle */
s32 SHOCKTST_moveCursor(ShockTest *task, ShockTestWindows *win) {
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        do {
            if (--task->column < 0) {
                task->column = 1;
            }
        } while (SHOCKTST_menuRows[task->row].enabled[task->column] == 0);
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        do {
            if (++task->column >= 2) {
                task->column = 0;
            }
        } while (SHOCKTST_menuRows[task->row].enabled[task->column] == 0);
    }
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        do {
            if (--task->row < 0) {
                task->row = 3;
            }
        } while (SHOCKTST_menuRows[task->row].enabled[task->column] == 0);
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        do {
            if (++task->row >= 4) {
                task->row = 0;
            }
        } while (SHOCKTST_menuRows[task->row].enabled[task->column] == 0);
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SHOCKTST_highlight(task, win, SHOCKTST_menuRows[task->row].highlight[task->column + 2]);
        if (task->row == 3) {
            return -1;
        }
        return 1;
    }
    if (PAD_PRESSED(PAD_CIRCLE)) {
        return 2;
    }
    SHOCKTST_highlight(task, win, SHOCKTST_menuRows[task->row].highlight[task->column]);
    return 0;
}

/* Changes a time or power with up and down (by 10 when repeating), or flips it
   between 0 and 1 when toggle is set: 1 on cross, -1 on triangle */
s32 SHOCKTST_editValue(ShockTest *task, ShockTestWindows *win, TextWindow **windows, u8 *value, u8 toggle) {
    if (toggle) {
        if (PAD_PRESSED(PAD_UP) || PAD_PRESSED(PAD_DOWN)) {
            *value = 1 - *value;
        }
    } else if (PAD_PRESSED(PAD_UP)) {
        *value += 1;
    } else if (PAD_REPEATED(PAD_UP)) {
        *value += 10;
    } else if (PAD_PRESSED(PAD_DOWN)) {
        *value -= 1;
    } else if (PAD_REPEATED(PAD_DOWN)) {
        *value -= 10;
    }
    SHOCKTST_showPattern(task, win, task->pattern);
    if (PAD_PRESSED(PAD_CROSS)) {
        return 1;
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        return -1;
    }
    return 0;
}

/* Edits what the cursor picked: the pattern, or a motor's time or power (the
   small motor's is only on or off); 1 once done */
s32 SHOCKTST_editRow(ShockTest *task, ShockTestWindows *win) {
    switch (task->row) {
    case 0:
    default:
        if (SHOCKTST_selectPattern(task, win) != 0) {
            return 1;
        }
        break;
    case 1:
        task->step = task->column;
        if (SHOCKTST_editValue(task, win, win->times, &task->steps[task->step][task->pattern].time, 0) != 0) {
            return 1;
        }
        break;
    case 2:
        task->step = task->column;
        if (task->step != 0) {
            if (SHOCKTST_editValue(task, win, win->times, &task->steps[task->step][task->pattern].power, 0) != 0) {
                return 1;
            }
        } else if (SHOCKTST_editValue(task, win, win->times, &task->steps[0][task->pattern].power, 1) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}

/* The names of the motors: "こうそく" (fast) and "ていそく" (slow) */
char *SHOCKTST_motorNames[2] = {
    "\x82\xB1\x82\xA4\x82\xBB\x82\xAD",
    "\x82\xC4\x82\xA2\x82\xBB\x82\xAD",
};

/* The editor's task: creates its windows, then edits and plays the patterns
   (play with L1 held plays them all, circle checks the memory card); when
   killed, frees them and stops the motors */
void SHOCKTST_updateEditor(ShockTest *task, ShockTestWindows *win) {
    s32 i;
    s32 motor;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->windowId = 0x1000;
        win->pattern = createTextWindow(task->windowId, 1, 0x28, 0x3C);
        task->file = FILE_CACHE.load(SHOCKTST_FILE);
        win->pattern->setString(win->pattern, SHOCKTST_numberFormats[0], -1);
        win->pattern->setNumber(win->pattern, 1, task->pattern);
        for (i = 0; i < 2; i++) {
            win->motors[i] = createTextWindow(task->windowId, 1, i * 100 + 0x3C, 0x50);
            win->motors[i]->setText(win->motors[i], SHOCKTST_motorNames[i]);
            win->times[i] = createTextWindow(task->windowId, 1, i * 100 + 0x3C, 0x64);
            win->times[i]->setString(win->times[i], SHOCKTST_numberFormats[1], -1);
            win->times[i]->setNumber(win->times[i], 1, task->steps[i][0].time);
            win->powers[i] = createTextWindow(task->windowId, 1, i * 100 + 0x3C, 0x78);
            win->powers[i]->setString(win->powers[i], SHOCKTST_numberFormats[2], -1);
            win->powers[i]->setNumber(win->powers[i], 1, task->steps[i][0].power);
        }
        win->play = createTextWindow(task->windowId, 1, 0x28, 0x8C);
        win->play->setText(win->play, "\x83\x70\x83\x5E\x81\x5B\x83\x93\x82\xB6\x82\xC1\x82\xB1\x82\xA4"); /* "パターンじっこう" */
        break;
    case TASK_RUN:
        switch (task->substate) {
        default:
            task->setSubstate(task, 0);
            /* fallthrough */
        case 0:
            switch (SHOCKTST_moveCursor(task, win)) {
            case 2:
                task->setSubstate(task, 4);
                break;
            case 1:
                task->nextSubstate(task);
                break;
            case -1:
                if (PAD_HELD(PAD_L1)) {
                    task->setSubstate(task, 2);
                } else {
                    task->setSubstate(task, 3);
                }
                task->motors[0] = task->motors[1] = task->timers[0] = task->timers[1] = task->playing = 0;
                break;
            }
            break;
        case 1:
            if (SHOCKTST_editRow(task, win) != 0) {
                task->setSubstate(task, 0);
            }
            break;
        case 2:
            if (SHOCKTST_playAllPatterns(task, win) != 0) {
                task->setSubstate(task, 0);
            }
            break;
        case 3:
            if (SHOCKTST_playPattern(task, win, task->pattern) != 0) {
                task->setSubstate(task, 0);
            }
            break;
        case 4:
            if (MEMCARD_FUNCS.check(0) != 0) {
                task->setSubstate(task, 0);
            }
            break;
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        HEAP.free(task->steps[0]);
        HEAP.free(task->steps[1]);
        for (motor = 0; motor < 2; motor++) {
            PAD.setVibration(0, motor, 0, 0);
        }
        break;
    }
}

/* Copies the pattern file's times and powers into the editor */
void SHOCKTST_loadPatterns(ShockTest *task, ShockFile *file) {
    s32 i;
    /* the offsets count bytes from the file's start */
    u8 *times = (u8 *)file + file->timesOffset;
    u8 *powers = (u8 *)file + file->powersOffset;

    for (i = 0; i < task->count; i++) {
        task->steps[0][i].time = times[0];
        task->steps[1][i].time = times[1];
        times += 2;
        task->steps[0][i].power = powers[0];
        task->steps[1][i].power = powers[1];
        powers += 2;
    }
}

/* Creates the editor, with room for count patterns */
ShockTest *SHOCKTST_createEditor(s32 count) {
    ShockTest *task = createTask(SHOCKTST_updateEditor, sizeof(ShockTest), sizeof(ShockTestWindows));

    task->count = count;
    task->steps[0] = HEAP.allocZeroed(count * sizeof(ShockStep), 2);
    task->steps[1] = HEAP.allocZeroed(task->count * sizeof(ShockStep), 2);
    return task;
}

char *SHOCKTST_textPath = "sim:C:\\DEVELOP\\DLSKDATA.TXT";

/* Turns DLSKDATA.TXT (the count, then a tab-separated row per pattern, of
   which type 1 holds the motors' times and powers) into the pattern file, and
   writes it to the PC as DLSKDATA.BIN */
void SHOCKTST_convertText(ShockLoader *task) {
    u8 *s = task->text;
    /* the header is written a word at a time, through this pointer */
    s32 *header = (s32 *)task->file;
    s32 count;
    s32 *types;
    u8 *times;
    u8 *powers;
    s32 type;
    u8 values[4];
    s32 i;
    s32 n;
    s32 fd;
    s32 k;
    u8 *t;
    u8 *p;
    u8 *nt;
    u8 *np;

    count = atoi(s);
    while (*s != '\n') {
        s++;
    }
    *header++ = count;
    *header++ = sizeof(ShockFile);
    *header = header[-1] + count * 4;
    header++;
    *header = header[-1] + count * 2;
    s++;
    /* the offsets count bytes from the file's start. The match depends on
       times holding the file's start until t and p are copied from it, and
       on the branch stepping p before t */
    types = (s32 *)task->file;
    times = (u8 *)types;
    types = (s32 *)((u8 *)types + ((ShockFile *)types)->typesOffset);
    p = t = times;
    times = t += ((ShockFile *)t)->timesOffset;
    powers = p += ((ShockFile *)p)->powersOffset;
    k = 1;
    while (*s != '/') {
        s += strcspn(s, "\t");
        while (*s == '\t') {
            s++;
        }
        s += strcspn(s, "\t");
        while (*s == '\t') {
            s++;
        }
        n = strcspn(s, "\t");
        type = atoi(s);
        s += n;
        while (*s == '\t') {
            s++;
        }
        if (type == 1) {
            for (i = 0; i < 4; i++) {
                n = strcspn(s, "\t\n");
                values[i] = atoi(s);
                s += n;
                while (*s == '\t' || *s == '\n') {
                    s++;
                }
            }
            nt = &times[k * 2];
            np = &powers[k * 2];
            *types++ = type;
            t[0] = values[0];
            nt[-1] = values[2];
            p[0] = values[1];
            np[-1] = values[3];
            k++;
            p = np;
            t = nt;
        } else {
            n = strcspn(s, "\n");
            s += n;
            while (*s == '\n') {
                s++;
            }
        }
    }
    fd = open("sim:C:\\DEVELOP\\DLSKDATA.BIN", 0x200);
    if (fd != -1) {
        write(fd, task->file, count * 8 + sizeof(ShockFile));
        close(fd);
    }
}

/* The loader's task: creates the title and help windows, reads DLSKDATA.TXT
   from the PC and starts the editor on it; START goes back to the stage
   select (MODE_STAGE_SELECT) */
void SHOCKTST_updateLoader(ShockLoader *task, ShockLoaderWindows *win) {
    s32 fd;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        win->title = createTextWindow(SCREEN_LAYER, 0, 0x14, 0x1E);
        win->title->setText(win->title, "\x82\xB5\x82\xF1\x82\xC7\x82\xA4\x83\x65\x83\x58\x83\x67"); /* "しんどうテスト" */
        win->help[0] = createTextWindow(SCREEN_LAYER, 1, 0xDC, 0xB4);
        win->help[0]->setText(win->help[0], "\x81\x7E\x81\x46\x82\xB6\x82\xC1\x82\xB1\x82\xA4\x82\xC4\x82\xA2\x82\xB5"); /* "×：じっこうていし" */
        win->help[1] = createTextWindow(SCREEN_LAYER, 1, 0xDC, 0xC8);
        win->help[1]->setText(win->help[1], SHOCKTST_STR_START_BACK);
        task->text = HEAP.allocZeroed(0x4000, 2);
        task->file = HEAP.allocZeroed(0x4000, 2);
        if (task->text == NULL || task->file == NULL) {
            task->setState(task, TASK_KILL);
            break;
        }
        fd = open(SHOCKTST_textPath, 1);
        if (fd == -1) {
            task->setState(task, TASK_KILL);
            break;
        }
        read(fd, task->text, 0x4000);
        close(fd);
        SHOCKTST_convertText(task);
        if (task->file != NULL) {
            win->test = SHOCKTST_createEditor(task->file->count);
            SHOCKTST_loadPatterns(win->test, task->file);
        } else {
            win->test = SHOCKTST_createEditor(10);
        }
        break;
    case TASK_RUN:
        if (PAD_PRESSED(PAD_START)) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        if (task->file != NULL) {
            HEAP.free(task->file);
        }
        if (task->text != NULL) {
            HEAP.free(task->text);
        }
        GAME.funcs.requestMode(MODE_STAGE_SELECT, 0);
        break;
    }
}

/* Creates the loader */
Task *SHOCKTST_createLoader(void) {
    return createTask(SHOCKTST_updateLoader, sizeof(ShockLoader), sizeof(ShockLoaderWindows));
}

/* "ＳＴＡＲＴ：もどる" (START: back), padded to a word: the USA file's
   padding isn't zeros but what the assembler left there; the size leaves
   out the closing NUL */
const char SHOCKTST_STR_START_BACK[20] =
#if VERSION_US
    "\x82\x72\x82\x73\x82\x60\x82\x71\x82\x73\x81\x46\x82\xE0\x82\xC7\x82\xE9\0\x03";
#elif VERSION_EU || VERSION_JP
    "\x82\x72\x82\x73\x82\x60\x82\x71\x82\x73\x81\x46\x82\xE0\x82\xC7\x82\xE9\0\0";
#endif
