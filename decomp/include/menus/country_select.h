#ifndef CNTY_SEL_H
#define CNTY_SEL_H

/*
 * CNTY_SEL.PRO: the country select screen.
 *
 * A background scrolls diagonally, three panels open one after the other and
 * a highlighted option blinks until Start is pressed. Then the option flashes,
 * the panels close, the screen fades to black and the game moves on to
 * MODE_OPENING. The USA version doesn't store the choice; the European one
 * sets the language from it (LANGUAGE), and moves on to MODE_OPENING_2 for
 * any language but 0.
 */

#include "engine/game.h"

/*
 * The screen's tasks use the engine states (task.h); the owner sets TASK_DONE
 * to make a task fade out, flash, or open/close its panel.
 */

/* The screen's file, read with FILE_CACHE.getEntry: the discs number
   their files differently */
#if VERSION_US || VERSION_JP
#define CNTY_SEL_FILE 0x892
#elif VERSION_EU
#define CNTY_SEL_FILE 0x8A3
#endif
#define CNTY_SEL_SPRITES (CNTY_SEL_FILE << 16)      /* sprite bank */
#define CNTY_SEL_IMAGES (CNTY_SEL_FILE << 16 | 1)   /* TIM archive for VRAM */

/* Sprites in the sprite bank */
#define SPRITE_BACKGROUND 0
#define SPRITE_RIGHT_PANEL 1
#define SPRITE_TOP_PANEL 2
#define SPRITE_LEFT_PANEL 3
#define SPRITE_OPTIONS 4 /* one per option */

/* Draw layer of the screen */
#define CNTY_SEL_LAYER 0x100

/* Sounds (SOUND.playSound) */
#define CNTY_SEL_SOUND_BANK 0x21
#define CNTY_SEL_MUSIC MUSIC(0x21, 2)

/* A linear tween of a panel's scale */
typedef struct PanelTween {
    /* 0x0 */ s32 duration;
    /* 0x4 */ s16 to;
    /* 0x6 */ s16 from;
} PanelTween;

typedef struct LeftPanelTween {
    /* 0x0 */ s32 duration;
    /* 0x4 */ s16 to;
    /* 0x6 */ s16 from;
    /* 0x8 */ s32 pad; /* never read; 0 in both tweens */
} LeftPanelTween;

/* The scrolling background, which also fades the screen out */
typedef struct CountryBackgroundTask {
    TASK_HEADER(CountryBackgroundTask);
    /* 0x50 */ s16 scroll;
    /* 0x52 */ s16 fade;
} CountryBackgroundTask;

/* The highlighted option */
typedef struct CursorTask {
    TASK_HEADER(CursorTask);
    /* 0x50 */ s32 blink;
    /* 0x54 */ s16 selection;
    /* 0x56 */ s16 frame;
    /* 0x58 */ AnimState anim;
    /* 0x5C */ void (*setSelection)(struct CursorTask *task, s16 selection);
} CursorTask;

/* A panel that opens and closes by scaling */
typedef struct PanelTask {
    TASK_HEADER(PanelTask);
    /* 0x50 */ s16 phase; /* 0: opening, 1: closing */
    /* 0x52 */ s16 time;
    /* 0x54 */ s16 scaleX;
    /* 0x56 */ s16 scaleY;
} PanelTask;

typedef struct MenuChildren {
    /* 0x00 */ CursorTask *cursor;
    /* 0x04 */ PanelTask *rightPanel;
    /* 0x08 */ PanelTask *topPanel;
    /* 0x0C */ PanelTask *leftPanel;
    /* 0x10 */ CountryBackgroundTask *background;
} MenuChildren;

/* The screen's controller */
typedef struct CountryMenuTask {
    TASK_HEADER(CountryMenuTask);
    /* 0x50 */ s16 selection;
    /* 0x52 */ s16 timer;
} CountryMenuTask;

/* CountryMenuTask substates (Task.substate) in TASK_RUN */
enum MenuStep {
    MENU_OPEN_RIGHT_PANEL,
    MENU_WAIT_RIGHT_PANEL,
    MENU_OPEN_LEFT_PANEL,
    MENU_WAIT_LEFT_PANEL,
    MENU_OPEN_TOP_PANEL,
    MENU_WAIT_TOP_PANEL,
    MENU_SHOW_CURSOR,
    MENU_SELECT,
    MENU_WAIT_FLASH,
    MENU_CLOSE_PANELS,
    MENU_WAIT_PANELS,
    MENU_FADE_OUT,
    MENU_WAIT_FADE,
    MENU_EXIT,
};

extern RECT CNTY_SEL_screenRect;
extern RECT CNTY_SEL_vramRect;
extern RECT CNTY_SEL_fadeRect;
extern AnimFrame CNTY_SEL_cursorFlash[];
extern PanelTween CNTY_SEL_topPanelTweens[];
extern PanelTween CNTY_SEL_rightPanelTweens[];
extern LeftPanelTween CNTY_SEL_leftPanelTweens[];
#if VERSION_EU
extern u8 CNTY_SEL_languages[];
#endif

void CNTY_SEL_tickScreen(Task *task, CountryMenuTask **menu);
void CNTY_SEL_drawBackground(CountryBackgroundTask *task);
s32 CNTY_SEL_getFadeLevel(s32 time);
void CNTY_SEL_drawFade(s32 level);
void CNTY_SEL_tickBackground(CountryBackgroundTask *task);
CountryBackgroundTask *CNTY_SEL_startBackgroundTask(void);
s16 CNTY_SEL_stepAnimation(AnimState *anim, AnimFrame *frames, s32 depth);
void CNTY_SEL_drawCursor(CursorTask *task);
void CNTY_SEL_setCursorSelection(CursorTask *task, s16 selection);
void CNTY_SEL_tickCursor(CursorTask *task);
CursorTask *CNTY_SEL_startCursorTask(void);
s32 CNTY_SEL_getTopPanelScale(PanelTask *task, s32 phase);
void CNTY_SEL_drawTopPanel(PanelTask *task);
void CNTY_SEL_tickTopPanel(PanelTask *task);
PanelTask *CNTY_SEL_startTopPanelTask(void);
s32 CNTY_SEL_getRightPanelScale(PanelTask *task, s32 phase);
void CNTY_SEL_drawRightPanel(PanelTask *task);
void CNTY_SEL_tickRightPanel(PanelTask *task);
PanelTask *CNTY_SEL_startRightPanelTask(void);
s32 CNTY_SEL_getLeftPanelScale(PanelTask *task, s32 phase);
void CNTY_SEL_drawLeftPanel(PanelTask *task);
void CNTY_SEL_tickLeftPanel(PanelTask *task);
PanelTask *CNTY_SEL_startLeftPanelTask(void);
void CNTY_SEL_tickMenu(CountryMenuTask *task, MenuChildren *children);
CountryMenuTask *CNTY_SEL_startMenuTask(void);

#endif /* CNTY_SEL_H */
