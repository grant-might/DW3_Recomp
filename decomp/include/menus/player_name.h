#ifndef STPLNMET_H
#define STPLNMET_H

/* STPLNMET.PRO: the player's name entry (MODE_PLAYER_NAME). Its modules are in
   src/menus/player_name/, and the functions below are by module, in the order they
   link. */

#include "engine/game.h"

/* The name of this overlay's copy of a function or type of src/shared/ */
#define OVL_NAME(name) STPLNMET_##name

/* The overlay's NameEntry and the name entry's files (name_entry.h) */
#define NAME_ENTRY_HAS_UNUSED98 0
#define NAME_ENTRY_HAS_HIDE 1
#define NAME_ENTRY_SPRITES PLNMET_BANK
#define NAME_ENTRY_FILE_KEYBOARD FILE_PLNMET_KEYBOARD
#include "shared/name_entry.h" /* the keyboard's types, as STDGNAME's and STCRDDEK's */

/* The files of the screen */
#if VERSION_US || VERSION_JP
#define FILE_PLNMET_SPRITES 0x27C
#define FILE_PLNMET_KEYBOARD 0x762
#define FILE_PLNMET_IMAGE 0x896
#define FILE_PLNMET_MENU 0x27B
#define FILE_PLNMET_BANK 0x761
#define FILE_PLNMET_SCROLL 0x2B5
#elif VERSION_EU
#define FILE_PLNMET_SPRITES 0x28B
#define FILE_PLNMET_KEYBOARD 0x771
#define FILE_PLNMET_IMAGE 0x8A7
#define FILE_PLNMET_MENU 0x28A
#define FILE_PLNMET_BANK 0x770
#define FILE_PLNMET_SCROLL 0x2C4
#endif
#define PLNMET_MENU (FILE_PLNMET_MENU << 16) /* sprite bank */
#define PLNMET_BANK (FILE_PLNMET_BANK << 16) /* the keyboard's and partners' sprites */
#define PLNMET_SCROLL (FILE_PLNMET_SCROLL << 16)

/* Two scrolling sprites (STPLNMET_createScroll), once their image is loaded */
typedef struct NameScroll {
    TASK_HEADER(NameScroll);
    /* 0x50 */ s32 vramX;
    /* 0x54 */ s32 vramY;
    /* 0x58 */ s32 loaded;
    /* 0x5C */ s32 layer;
    /* 0x60 */ s32 depth;
    /* 0x64 */ s32 times[2];
    /* 0x6C */ s32 pos[2][2];
    /* 0x7C */ void (*load)(struct NameScroll *task, s32 x, s32 y);
    /* 0x80 */ void (*setLayer)(struct NameScroll *task, s32 layer, s32 depth);
} NameScroll;

/* A frame of STPLNMET_updateShine's animation: 0 ends */
typedef struct SparkleFrame {
    /* 0x0 */ s32 delay;
    /* 0x4 */ s32 sprite;
} SparkleFrame;

/* An animated sprite (STPLNMET_updateSparkles, STPLNMET_updateShine) */
typedef struct NameSparkle {
    TASK_HEADER(NameSparkle);
    /* 0x50 */ s32 pad[4]; /* never read or written */
    /* 0x60 */ s32 frame;
    /* 0x64 */ s32 time;
} NameSparkle;

/* The welcome to Digimon Online and its registration (STPLNMET_createWelcome) */
typedef struct NameDialog {
    TASK_HEADER(NameDialog);
    /* 0x50 */ struct PlayerNameScreen *screen;
    /* 0x54 */ s32 clutRow;
    /* 0x58 */ s32 pad; /* never read or written */
} NameDialog;

typedef struct NameDialogWindows {
    /* 0x0 */ TextWindow *text;
    /* 0x4 */ TextWindow *arrow; /* follows the text, blinks while it waits */
} NameDialogWindows;

/* The main task of the screen (STPLNMET_createScreen) */
typedef struct PlayerNameScreen {
    TASK_HEADER(PlayerNameScreen);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 page; /* 0 the name, 1 the partners, 2 the confirmation */
    /* 0x5C */ PanelAnim fade;
} PlayerNameScreen;

typedef struct PlayerNameScreenChildren {
    /* 0x00 */ struct NameDialog *dialog;
    /* 0x04 */ NameEntry *name;
    /* 0x08 */ struct PartnerChoice *partners;
    /* 0x0C */ struct NameConfirm *confirm;
    /* 0x10 */ NameScroll *scroll;
    /* 0x14 */ NameSparkle *sparkles;
    /* 0x18 */ NameSparkle *shine;
    /* 0x1C */ TextWindow *tabs[3];
} PlayerNameScreenChildren;

/* The confirmation of the name and the partners (STPLNMET_updateConfirm) */
typedef struct NameConfirm {
    TASK_HEADER(NameConfirm);
    /* 0x50 */ struct PlayerNameScreen *screen;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 choice; /* STPLNMET_funcs.choices */
    /* 0x60 */ s32 cursor; /* 0 yes, 1 no */
    /* 0x64 */ s32 clutRow;
    /* 0x68 */ s32 clutTime;
    /* 0x6C */ u16 *name;
    /* 0x70 */ s32 frames[3];
    /* 0x7C */ s32 frameTime;
    /* 0x80 */ s32 progress; /* 0-100 */
    /* 0x84 */ VECTOR scale;
    /* 0x94 */ SVECTOR rot;
    /* 0x9C */ MATRIX matrix;
    /* 0xBC */ PanelAnim tweenA;
    /* 0xCC */ PanelAnim tweenB;
    /* 0xDC */ void (*hide)(struct NameConfirm *task, s32 hide);
    /* 0xE0 */ void (*close)(struct NameConfirm *task);
} NameConfirm;

typedef struct NameConfirmWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *name;
    /* 0x08 */ TextWindow *pack;
    /* 0x0C */ TextWindow *message;
    /* 0x10 */ TextWindow *progress;
    /* 0x14 */ TextWindow *yes;
    /* 0x18 */ TextWindow *no;
    /* 0x1C */ Cursor *cursor;
} NameConfirmWindows;

/* The choice of the partners (STPLNMET_updateChoice) */
typedef struct PartnerChoice {
    TASK_HEADER(PartnerChoice);
    /* 0x50 */ struct PlayerNameScreen *screen;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 frames[3];
    /* 0x68 */ s32 frameTime;
    /* 0x6C */ s32 choice; /* STPLNMET_funcs.choices */
    /* 0x70 */ s32 arrowFrame;
    /* 0x74 */ s32 arrowTime;
    /* 0x78 */ u8 pad[0x88 - 0x78]; /* never read or written */
    /* 0x88 */ void (*hide)(struct PartnerChoice *task, s32 hide);
} PartnerChoice;

typedef struct PartnerChoiceWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *labels[3];
    /* 0x10 */ TextWindow *name;
    /* 0x14 */ TextWindow *text;
    /* 0x18 */ struct {
        TextWindow *name;
        TextWindow *text;
    } partners[3];
} PartnerChoiceWindows;

/* The screen's helpers (STPLNMET_funcs) */
typedef struct PlayerNameFuncs {
    /* 0x00 */ PartnerAnim *anims;
    /* 0x04 */ s32 (*choices)[3]; /* three partners each */
    /* 0x08 */ void (*loadFiles)(void);
    /* 0x0C */ s32 (*filesLoading)(void);
    /* 0x10 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x14 */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x18 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x1C */ s32 (*updateLerp)(MenuLerp *lerp);
} PlayerNameFuncs;

/* scene.c */
void STPLNMET_centerLayer(Task *task, PlayerNameScreen **screen, Layer *layer, RECT *rect);
void STPLNMET_updateScene(Task *task, PlayerNameScreen **screen);

/* backdrop.c */
void STPLNMET_updateScroll(NameScroll *task);
void STPLNMET_loadScroll(NameScroll *task, s32 x, s32 y);
void STPLNMET_setScrollLayer(NameScroll *task, s32 layer, s32 depth);
NameScroll *STPLNMET_createScroll(void);
void STPLNMET_updateSparkles(NameSparkle *task);
NameSparkle *STPLNMET_createSparkles(void);
void STPLNMET_updateShine(NameSparkle *task);
NameSparkle *STPLNMET_createShine(void);

/* welcome.c */
void STPLNMET_updateWelcome(NameDialog *task, NameDialogWindows *windows);
NameDialog *STPLNMET_createWelcome(PlayerNameScreen *screen);

/* name_entry.c */
void STPLNMET_startTween(PanelAnim *fade, s32 fadeIn);
s32 STPLNMET_updateTween(PanelAnim *fade);
void STPLNMET_createNameWindows(NameEntry *task, NameEntryWindows *windows);
void STPLNMET_showNameWindows(NameEntry *task, NameEntryWindows *windows, s32 show);
void STPLNMET_drawKeyboard(NameEntry *task);
void STPLNMET_updateKeyboard(NameEntry *task, NameEntryWindows *windows);
void STPLNMET_updateNameEntry(NameEntry *task, NameEntryWindows *windows);
void STPLNMET_setNameVram(NameEntry *task, s32 x, s32 y);
void STPLNMET_setName(NameEntry *task, char *name);
void STPLNMET_getName(NameEntry *task, char *out);
void STPLNMET_closeNameEntry(NameEntry *task);
void STPLNMET_hideNameEntry(NameEntry *task, s32 hide);
NameEntry *STPLNMET_createNameEntry(char *name);

/* confirm.c */
void STPLNMET_createConfirmWindows(NameConfirm *task, NameConfirmWindows *windows);
void STPLNMET_showConfirmWindows(NameConfirm *task, NameConfirmWindows *windows, s32 show);
void STPLNMET_drawConfirm(NameConfirm *task);
void STPLNMET_runConfirm(NameConfirm *task, NameConfirmWindows *windows);
void STPLNMET_hideConfirm(NameConfirm *task, s32 hide);
void STPLNMET_closeConfirm(NameConfirm *task);
void STPLNMET_updateConfirm(NameConfirm *task, NameConfirmWindows *windows);
NameConfirm *STPLNMET_createConfirm(PlayerNameScreen *screen);

/* choice.c */
void STPLNMET_createChoiceWindows(PartnerChoice *task, PartnerChoiceWindows *windows);
void STPLNMET_showChoiceWindows(PartnerChoice *task, PartnerChoiceWindows *windows, s32 show);
void STPLNMET_drawChoice(PartnerChoice *task);
void STPLNMET_runChoice(PartnerChoice *task, PartnerChoiceWindows *windows);
void STPLNMET_hideChoice(PartnerChoice *task, s32 hide);
void STPLNMET_updateChoice(PartnerChoice *task, PartnerChoiceWindows *windows);
PartnerChoice *STPLNMET_createChoice(PlayerNameScreen *screen);

/* screen.c */
void STPLNMET_createTabs(PlayerNameScreen *screen, PlayerNameScreenChildren *children);
void STPLNMET_showTabs(PlayerNameScreen *screen, PlayerNameScreenChildren *children, s32 show);
void STPLNMET_stepScreen(PlayerNameScreen *screen, PlayerNameScreenChildren *children);
void STPLNMET_drawTitle(PlayerNameScreen *screen);
void STPLNMET_updateScreen(PlayerNameScreen *screen, PlayerNameScreenChildren *children);
PlayerNameScreen *STPLNMET_createScreen(void);
void STPLNMET_loadFiles(void);
s32 STPLNMET_filesLoading(void);
void STPLNMET_startFade(PanelAnim *fade, s32 fadeIn);
s32 STPLNMET_updateFade(PanelAnim *fade);
void STPLNMET_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STPLNMET_updateLerp(MenuLerp *lerp);

/* STPLNMET's data (data/stplnmet.c), in its order */
extern PlayerNameFuncs STPLNMET_funcs;
extern TextStyle STPLNMET_nameStyle;
extern s32 STPLNMET_scrollEnds[];
extern SparkleFrame STPLNMET_sparkleFrames[];
extern s32 STPLNMET_nameAnims[][7];
extern BigKey STPLNMET_bigKeys[];
extern s32 STPLNMET_keyArrowCluts[];
extern KeyTabs STPLNMET_keyPagesJp[];
extern KeyPage STPLNMET_keyCharsJp[];
extern KeyTabs STPLNMET_keyPages[];
extern KeyPage STPLNMET_keyChars[];
extern NameKeyboard STPLNMET_keyboard;
extern s32 STPLNMET_choiceArrowCluts[];
extern s32 STPLNMET_tabX[];

#endif /* STPLNMET_H */
