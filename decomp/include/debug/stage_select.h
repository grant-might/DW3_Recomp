#ifndef STAGSLCT_H
#define STAGSLCT_H

/* STAGSLCT.PRO: the debug stage select (MODE_STAGE_SELECT), a menu of every
   scene of the game with some debug settings on the second controller */

#include "engine/game.h"

/* One line of the menu; the list ends with scene -1 and scene 0 lines are
   separators the cursor skips */
typedef struct StageSelectEntry {
    /* 0x0 */ char *name;
    /* 0x4 */ char *title;
    /* 0x8 */ s32 scene;
    /* 0xC */ s32 arg; /* bit 15: go through GAME.funcs.requestMode */
} StageSelectEntry;

typedef struct StageSelect {
    TASK_HEADER(StageSelect);
    /* 0x50 */ s32 cursor; /* line on screen */
    /* 0x54 */ s32 top;    /* first entry shown */
    /* 0x58 */ s32 lines;  /* lines on screen */
    /* 0x5C */ s32 count;  /* entries */
    /* 0x60 */ s32 biosShown;
    /* 0x64 */ s32 fading;
    /* 0x68 */ s32 fade;
    /* 0x6C */ s32 fadeStep;
} StageSelect;

typedef struct StageSelectWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *cursor;
    /* 0x08 */ TextWindow *names[14];
    /* 0x40 */ TextWindow *titles[14];
    /* 0x78 */ TextWindow *bios;
    /* 0x7C */ TextWindow *pad; /* never read or written */
    /* 0x80 */ TextWindow *region;
    /* 0x84 */ TextWindow *progress; /* GAME.progress */
    /* 0x88 */ TextWindow *randomBattles; /* BATTLE_SETUP.randomBattles */
    /* 0x8C */ TextWindow *charisma; /* party member 0's (STAT_CHARISMA) */
    /* 0x90 */ TextWindow *level; /* party member 0's */
    /* 0x94 */ TextWindow *debugUpDown; /* BATTLE_SETUP.debugUpDown */
    /* 0x98 */ TextWindow *debugLeftRight; /* BATTLE_SETUP.debugLeftRight */
} StageSelectWindows;

Task *STAGSLCT_createStageSelect(void);
void STAGSLCT_updateScene(Task *task, Task **items);
void STAGSLCT_moveCursor(StageSelect *sel, s32 delta);
void STAGSLCT_scrollPage(StageSelect *sel, s32 delta);
void STAGSLCT_showBiosVersion(StageSelect *sel, StageSelectWindows *win);
void STAGSLCT_zoomTitle(StageSelect *sel, StageSelectWindows *win);
void STAGSLCT_updateStageSelect(Task *task, StageSelectWindows *win);

#if VERSION_EU || VERSION_JP
extern const char STAGSLCT_STR_CURSOR[]; /* "＞" with its padding */
#endif

#endif /* STAGSLCT_H */
