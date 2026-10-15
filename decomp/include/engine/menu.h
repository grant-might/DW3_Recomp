#ifndef DW3_MENU_H
#define DW3_MENU_H

/* The inn and the field menu (menu/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "engine/task.h"
#include "engine/gfx.h"
#include "engine/text.h"

/*
 * Opens or closes a menu panel: level goes 0 -> ONE in `duration` frames
 * (or back at twice the speed), and the panel is drawn scaled by it.
 */
typedef struct PanelAnim {
    s32 duration;
    s32 step;
    s32 level;
    s32 active;
} PanelAnim;

/* Moves a value towards a target in fixed point, as the menu overlays'
   startLerp and updateLerp do */
typedef struct MenuLerp {
    /* 0x00 */ s32 duration;
    /* 0x04 */ s32 pad4; /* nothing reads or writes it */
    /* 0x08 */ s32 value;
    /* 0x0C */ s32 fixed; /* value << 8 */
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 step;
    /* 0x18 */ s32 active;
} MenuLerp;

/* A vertical scroll bar, as the menu overlays (STSTATUS, STGDGLAB) draw one */
typedef struct ScrollBar {
    TASK_HEADER(ScrollBar);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 x;
    /* 0x5C */ s32 y;
    /* 0x60 */ s32 width;
    /* 0x64 */ s32 size; /* of the thumb, in fixed point */
    /* 0x68 */ s32 hasCount;
    /* 0x6C */ s32 pageSize;
    /* 0x70 */ s32 count;
    /* 0x74 */ s32 pos;
    /* 0x78 */ s32 hasRange;
    /* 0x7C */ s32 top;
    /* 0x80 */ s32 bottom;
    /* 0x84 */ s32 pad84; /* nothing reads or writes it */
    /* 0x88 */ s32 posStep; /* fixed point */
    /* 0x8C */ void (*setX)(struct ScrollBar *bar, s32 x, s32 width);
    /* 0x90 */ void (*setRange)(struct ScrollBar *bar, s32 top, s32 bottom);
    /* 0x94 */ void (*setCount)(struct ScrollBar *bar, s32 pageSize, s32 count);
    /* 0x98 */ void (*setPos)(struct ScrollBar *bar, s32 pos);
} ScrollBar;

/*
 * The inn: pay INNS[inn].price per party member to restore HP, MP and
 * status, with a fade to black and a jingle. `step` is set when the money
 * is not enough.
 */
typedef struct Inn {
    TASK_HEADER(Inn);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 inn;
    /* 0x5C */ s32 choice; /* 0 yes, 1 no */
    /* 0x60 */ s32 count; /* party members */
    /* 0x64 */ s32 music;
    /* 0x68 */ s32 time;
    /* 0x6C */ PanelAnim panels[3];
} Inn;

typedef struct InnChildren {
    /* 0x00 */ struct ScreenFade *fade;
    /* 0x04 */ TextWindow *windows[6];
    /* 0x1C */ Cursor *cursor;
} InnChildren;

/* One party member in the field menu: name and five stats */
typedef struct PartnerPage {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *labels[5];
    /* 0x18 */ TextWindow *values[5];
} PartnerPage;

/* The children of the field menu */
typedef struct FieldMenuWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *moneyLabel;
    /* 0x08 */ TextWindow *money;
    /* 0x0C */ TextWindow *options[6];
    /* 0x24 */ struct Cursor *cursor;
    /* 0x28 */ PartnerPage pages[3];
} FieldMenuWindows;

/* Where a window goes, and the string it shows */
typedef struct WindowPos {
    /* 0x0 */ s32 string;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 pad6; /* 0 in every layout; nothing reads it */
    /* 0x8 */ s16 y;
} WindowPos;

/*
 * The menu opened on the field: the three party members' pages, a list of
 * options (FIELD_MENU_OPTIONS, one more when the player has item 0x192)
 * and the money. Picking an option switches to MODE_STATUS with the choice
 * in FIELD_MENU_CHOICE; from MODE_STATUS, cancelling goes back to
 * GAME.fieldMode. `step` tells whether a mode change follows (0) or not.
 */
typedef struct FieldMenu {
    TASK_HEADER(FieldMenu);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 cursor;
    /* 0x5C */ s32 count;
    /* 0x60 */ s32 extraOption;
    /* 0x64 */ s32 option2Enabled;
    /* 0x68 */ s32 fadeRow;
    /* 0x6C */ s32 time;
    /* 0x70 */ PanelAnim panels[3];
} FieldMenu;

/* What the field menu picked, for STSTATUS (MODE_STATUS) */
typedef struct FieldMenuChoice {
    /* 0x0 */ s32 option;
    /* 0x4 */ s32 extra; /* the list had the extra option: STSTATUS_screens' row */
} FieldMenuChoice;

extern FieldMenuChoice FIELD_MENU_CHOICE;

void innStartPanel(PanelAnim *panel, s32 open);
s32 innUpdatePanel(PanelAnim *panel);
void healParty(void);
void updateInnMenu(Inn *task, InnChildren *data);
void updateInn(struct Inn *task, struct InnChildren *data);
Inn *createInn(s32 layerId);
void startPanel(PanelAnim *panel, s32 open);
s32 updatePanel(PanelAnim *panel);
void createFieldMenuWindows(FieldMenu *task, FieldMenuWindows *win);
void showPartnerPage(void *menu, FieldMenuWindows *win, s32 page, s32 show);
s32 getFieldZone(void);
void updateFieldMenu(FieldMenu *task, FieldMenuWindows *win);
FieldMenu *createFieldMenu(s32 layerId, s32 cursor);

#endif /* DW3_MENU_H */
