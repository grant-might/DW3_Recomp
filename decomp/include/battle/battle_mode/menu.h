#ifndef FIGHTSTG_MENU_H
#define FIGHTSTG_MENU_H

/* FIGHTSTG's player turn and its menus, the cursor, the HUD, the message
   box and the partner's info and view (player_turn.c to partner_view.c). */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/camera.h"

/* A number that eases from one value to another (FIGHTSTG_stepHpTween) */
typedef struct HpTween {
    /* 0x00 */ s32 from;
    /* 0x04 */ s32 to;
    /* 0x08 */ s32 value; /* the one shown */
    /* 0x0C */ s16 active;
    /* 0x0E */ s16 fighter; /* whose hp it is */
    /* 0x10 */ s16 time;
    /* 0x12 */ s16 duration;
} HpTween;

/* FIGHTSTG_createHud's task: the names and the hp of the fighters out */
typedef struct HpDisplay {
    TASK_HEADER(HpDisplay);
    /* 0x50 */ s32 shown[2]; /* the fighters the names are of */
    /* 0x58 */ HpTween hp[2]; /* each side's */
    /* 0x80 */ s32 timer;
    /* 0x84 */ s32 interval; /* how often the hp is checked */
} HpDisplay;

/* The player's turn (FIGHTSTG_updatePlayerTurn, BATTLE_TASK_PLAYER_TURN): the battle menu, then the
   menus of its commands */
typedef struct PlayerTurn {
    TASK_HEADER(PlayerTurn);
    /* 0x50 */ s32 command; /* the command menu's choice, a BATTLE_COMMAND_* */
    /* 0x54 */ s32 switchLine; /* FIGHTSTG_createSwitchMenu's line, kept while the turn goes back to it */
    /* 0x58 */ s32 result; /* the open menu's, -1 until it is done and -2 to go back */
    /* 0x5C */ s32 action; /* what the turn does */
    /* 0x60 */ s32 arg; /* the action's: the item, the technique, or the party slot of the partner that digivolves or switches out */
    /* 0x64 */ s32 digimon; /* the Digimon a partner changes into, or the one it switches to */
    /* 0x68 */ s32 pairTech; /* FIGHTSTG_createPairSwitchMenu's technique, which the outgoing and incoming partners do together (action 6) */
} PlayerTurn;

/* The menu of the Digimon the active partner can digivolve into
   (FIGHTSTG_createDigivolveMenu), with the PartnerInfo of the one under the
   cursor */
typedef struct DigivolveMenu {
    TASK_HEADER(DigivolveMenu);
    /* 0x50 */ s32 *result; /* -1 until it is done, then the Digimon or -2 */
    /* 0x54 */ s32 partner; /* *result when it was made */
    /* 0x58 */ s32 page; /* the PartnerInfo page, 0-2 */
    /* 0x5C */ s32 sel; /* the cursor's line last frame */
    /* 0x60 */ s16 ids[4]; /* the partner, then its digivolutions (slots of 3 or more) */
    /* 0x68 */ s32 count;
    /* 0x6C */ s16 slots[4]; /* GAME.funcs.getPartnerSlots's */
} DigivolveMenu;

/* FIGHTSTG_updateDigivolveMenu's children */
typedef struct DigivolveMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ TextWindow *names[4];
    /* 0x14 */ struct PartnerInfo *info[2]; /* shown and hiding, in turns */
} DigivolveMenuWindows;

/* The lines of a page of the technique menu */
#define TECH_MENU_LINES 6

/* FIGHTSTG_createTechMenu's task: a menu of techniques, six a page */
typedef struct TechMenu {
    TASK_HEADER(TechMenu);
    /* 0x50 */ s32 *result; /* -1 until it is done */
    /* 0x54 */ s32 sel; /* the cursor's line last frame */
    /* 0x58 */ s16 slots[4]; /* GAME.funcs.getPartnerSlots's */
    /* 0x60 */ PartnerEntry entries[3]; /* skills with SKILL_MARKED can be passed on */
    /* 0x9C */ s32 techs[12]; /* ids in the low 13 bits, from 1 */
    /* 0xCC */ s32 count;
    /* 0xD0 */ s32 page;
    /* 0xD4 */ s32 pageCount;
} TechMenu;

/* The lines of a page of the item menu */
#define ITEM_MENU_LINES 7

/* The battle's item menu (FIGHTSTG_updateItemMenu): the usable items, seven a page */
typedef struct ItemMenu {
    TASK_HEADER(ItemMenu);
    /* 0x050 */ s32 *result; /* -1 until it is done, then the item or -2 */
    /* 0x054 */ s32 sel; /* the cursor's line last frame */
    /* 0x058 */ s16 items[0x194]; /* the bag's (ITEM_FUNCS.list) */
    /* 0x380 */ s16 *usable; /* the items with flag 2 */
    /* 0x384 */ s32 count;
    /* 0x388 */ s32 page;
    /* 0x38C */ s32 pageCount;
} ItemMenu;

/* FIGHTSTG_updateItemMenu's children */
typedef struct ItemMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ TextWindow *prevButton; /* the page buttons, texts 0x11 and 0x12 */
    /* 0x08 */ TextWindow *nextButton;
    /* 0x0C */ TextWindow *countLabel; /* text 0xC */
    /* 0x10 */ TextWindow *count; /* how many of the item */
    /* 0x14 */ TextWindow *message; /* its description */
    /* 0x18 */ TextWindow *names[ITEM_MENU_LINES];
} ItemMenuWindows;

/* The menu after the switch menu (FIGHTSTG_createSwitchInMenu): which of the
   incoming partner's Digimon comes in, then whether it just switches in or
   does the pair technique with the active fighter (only with
   FIGHTSTG_createPairSwitchMenu's techResult) */
typedef struct SwitchInMenu {
    TASK_HEADER(SwitchInMenu);
    /* 0x50 */ s32 *result; /* -1 until it is done, then the Digimon << 4 | 0 to switch or 1 for the pair technique, or -2 */
    /* 0x54 */ s32 page; /* the PartnerInfo page, 0-2 */
    /* 0x58 */ s32 shownPage;
    /* 0x5C */ s32 fighter; /* the incoming one, *result when it was made */
    /* 0x60 */ s32 partner; /* its party member */
    /* 0x64 */ s32 digimon; /* the line of ids picked */
    /* 0x68 */ s16 ids[4]; /* the partner, then its digivolutions (slots of 3 or more) */
    /* 0x70 */ s32 count;
    /* 0x74 */ s16 slots[4]; /* GAME.funcs.getPartnerSlots's */
    /* 0x7C */ s32 tech; /* the pair technique, from 1, 0 for none */
    /* 0x80 */ s32 *techResult; /* gets the technique picked */
} SwitchInMenu;

/* FIGHTSTG_updateSwitchInMenu's children */
typedef struct SwitchInMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ struct MenuCursor *techCursor;
    /* 0x08 */ struct PartnerInfo *info[2]; /* shown and hiding, in turns */
    /* 0x10 */ TextWindow *names[4];
    /* 0x20 */ TextWindow *choices[5]; /* switching (text 0x1B), the pair technique (0x1C), "MP", its MP and the Digimon's name */
    /* 0x34 */ TextWindow *mp[4]; /* "MP", the active fighter's, "/", its max */
} SwitchInMenuWindows;

/* The command menu's lines, PlayerTurn.command */
#define BATTLE_COMMAND_ATTACK 0
#define BATTLE_COMMAND_TECH 1
#define BATTLE_COMMAND_DIGIVOLVE 2
#define BATTLE_COMMAND_SWITCH 3
#define BATTLE_COMMAND_ITEM 4
#define BATTLE_COMMAND_RUN 5
#define BATTLE_COMMAND_COUNT 6

/* The battle's command menu (FIGHTSTG_createCommandMenu) */
typedef struct CommandMenu {
    TASK_HEADER(CommandMenu);
    /* 0x50 */ s32 start; /* the line the cursor starts on */
    /* 0x54 */ s32 *result; /* -1 until it is done */
} CommandMenu;

/* FIGHTSTG_updateCommandMenu's children: a cursor over six lines */
typedef struct CommandMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ TextWindow *lines[BATTLE_COMMAND_COUNT];
} CommandMenuWindows;

/* Shows the partner's model on layer 0x1009 (FIGHTSTG_createPartnerView), through
   the two FighterCamera children, a new one each time it is set up */
typedef struct PartnerView {
    TASK_HEADER(PartnerView);
    /* 0x50 */ Layer *layer;
    /* 0x54 */ s32 pad; /* nothing uses it */
} PartnerView;

/* A battle message box (FIGHTSTG_createMessage): shows its messages a line
   at a time, each until cross, then ends */
typedef struct BattleMessageBox {
    TASK_HEADER(BattleMessageBox);
    /* 0x50 */ s32 queue[9]; /* message types for show, some with args after them; 0 ends them */
    /* 0x74 */ s32 started; /* set by show */
    /* 0x78 */ s32 shown; /* the window being shown */
    /* 0x7C */ s32 count; /* how many */
    /* 0x80 */ s32 interval; /* the time between them */
    /* 0x84 */ s32 shownTime;
    /* 0x88 */ s32 next; /* in queue */
    /* 0x8C */ s32 pad; /* nothing uses it */
    /* 0x90 */ s32 arrowPalette; /* the arrow's palette, 0-4 */
    /* 0x94 */ s32 arrowTime; /* when it last changed */
    /* 0x98 */ s32 showArrow; /* show the arrow */
    /* 0x9C */ s32 found[3]; /* the fighters FIGHTSTG_findFighters found */
    /* 0xA8 */ s32 foundCount;
    /* 0xAC */ void (*show)(); /* FIGHTSTG_showMessage: (task, type, data) */
    /* 0xB0 */ void (*finish)(struct BattleMessageBox *task); /* FIGHTSTG_finishMessage: closes after 0x14 ticks or Cross */
} BattleMessageBox;

/* FIGHTSTG_updateMessage's children: the lines it shows one by one */
typedef struct BattleMessageBoxWindows {
    /* 0x0 */ TextWindow *lines[2];
} BattleMessageBoxWindows;

/* What FIGHTSTG_updateMessage's messages say, for some of their types */
typedef struct BattleMessage {
    /* 0x0 */ u8 side;
    /* 0x4 */ s32 kind;
    /* 0x8 */ s32 value;
} BattleMessage;

/* A page about a partner in one of its Digimon (FIGHTSTG_createPartnerInfo):
   its stats, its techniques, or the techniques its other Digimon can pass on */
typedef struct PartnerInfo {
    TASK_HEADER(PartnerInfo);
    /* 0x50 */ s32 pad; /* nothing uses it */
    /* 0x54 */ s32 partner;
    /* 0x58 */ s32 page; /* what it shows: 0 the stats, 1 and 2 techniques */
    /* 0x5C */ s32 slot; /* from 1, or 0 for the partner itself */
    /* 0x60 */ s32 level; /* the Digimon's (PartnerEntry.level), or -1 for none: text 0x1A */
    /* 0x64 */ s16 stats[0x16]; /* the 22 of a PartnerTotals computeStats fills, shown up to 999 */
    /* 0x90 */ s16 slots[4]; /* GAME.funcs.getPartnerSlots's */
    /* 0x98 */ PartnerEntry entries[3]; /* skills with SKILL_MARKED can be passed on */
    /* 0xD4 */ s32 techs[6]; /* a technique in the low 13 bits */
} PartnerInfo;

/* A line of FIGHTSTG_showStats's stat list (FIGHTSTG_statLines): where its
   number goes and the stat it shows */
typedef struct StatLine {
    /* 0x0 */ u8 x;
    /* 0x1 */ u8 y;
    /* 0x2 */ u8 stat; /* of PartnerInfo.stats */
} StatLine;

/* The battle's switch menu (FIGHTSTG_createSwitchMenu): the player's other
   fighters, with their HP and MP, to bring in for the active one; a message
   when there are none */
typedef struct SwitchMenu {
    TASK_HEADER(SwitchMenu);
    /* 0x50 */ s32 *result; /* -1 until it is done, then the fighter or -2 */
    /* 0x54 */ s32 *line; /* the cursor's line, kept for the next time */
    /* 0x58 */ s32 canCancel; /* triangle goes back (not after a knockout) */
    /* 0x5C */ s32 others[2]; /* the other fighters of the player's side */
    /* 0x64 */ s32 count;
    /* 0x68 */ s32 arrowShown; /* the message's arrow, from its second frame */
    /* 0x6C */ s32 arrowTime;
    /* 0x70 */ s32 arrowPalette;
    /* 0x74 */ s32 pairs[2]; /* FIGHTSTG_getPairDigimon's, marked with an icon */
    /* 0x7C */ s16 slots[6]; /* GAME.funcs.getPartnerSlots's */
} SwitchMenu;

/* FIGHTSTG_updateSwitchMenu's children: two of each window, one per fighter */
typedef struct SwitchMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ TextWindow *hpLabel[2];
    /* 0x0C */ TextWindow *hp[2];
    /* 0x14 */ TextWindow *hpSlash[2];
    /* 0x1C */ TextWindow *maxHp[2];
    /* 0x24 */ TextWindow *mpLabel[2];
    /* 0x2C */ TextWindow *mp[2];
    /* 0x34 */ TextWindow *mpSlash[2];
    /* 0x3C */ TextWindow *maxMp[2];
    /* 0x44 */ TextWindow *name[2];
    /* 0x4C */ TextWindow *message;
} SwitchMenuWindows;

/* A confused partner's command menu (FIGHTSTG_createConfusedMenu): a cursor
   over six of the shuffled lines; unless the fighter shakes off the
   confusion, the message of the one picked, shown like a
   BattleMessageBox's */
typedef struct ConfusedMenu {
    TASK_HEADER(ConfusedMenu);
    /* 0x50 */ s32 firstLine; /* the cursor's first line */
    /* 0x54 */ s32 picked;
    /* 0x58 */ s32 *result; /* -1 until it is done */
    /* 0x5C */ Task *partnerView; /* the PartnerView, stopped with shotCamera on a pick */
    /* 0x60 */ Task *shotCamera;
    /* 0x64 */ s32 queue[9]; /* like BattleMessageBox's: the message picked, then 0 */
    /* 0x88 */ s32 started; /* set by FIGHTSTG_showConfusedMessage */
    /* 0x8C */ s32 line; /* the window being shown */
    /* 0x90 */ s32 lineCount;
    /* 0x94 */ s32 delay; /* the time between them */
    /* 0x98 */ s32 time;
    /* 0x9C */ s32 next; /* in queue */
    /* 0xA0 */ s32 padA0; /* nothing uses it */
    /* 0xA4 */ s32 arrowPalette; /* 0-4 */
    /* 0xA8 */ s32 arrowTime; /* when it last changed */
    /* 0xAC */ s32 showArrow;
    /* 0xB0 */ s32 padB0[4]; /* nothing uses it */
    /* 0xC0 */ void (*show)(); /* like BattleMessageBox's, but never set: the queue ends first */
} ConfusedMenu;

/* Its children: the cursor, then its six lines; the first two are the
   message's after a pick */
typedef struct ConfusedMenuWindows {
    /* 0x00 */ struct MenuCursor *cursor;
    /* 0x04 */ TextWindow *lines[6];
} ConfusedMenuWindows;

/* A menu cursor's layout, which FIGHTSTG_createCursor copies into its task: a
   highlight bar over count lines, and a sprite per line (sprite -1: none) */
typedef struct CursorLayout {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 step;
    /* 0x10 */ s32 sprite;
    /* 0x14 */ s32 spriteX;
    /* 0x18 */ s32 spriteY;
    /* 0x1C */ s32 spriteStep;
} CursorLayout;

/* A menu cursor (FIGHTSTG_createCursor): up and down move it, its bar is
   drawn by FIGHTSTG_drawCursorBar at vsync and each line's sprite blinks
   while it is picked */
typedef struct MenuCursor {
    TASK_HEADER(MenuCursor);
    /* 0x50 */ s32 sel;
    /* 0x54 */ s32 locked;
    /* 0x58 */ CursorLayout params;
    /* 0x78 */ s32 prevSel; /* where the bar was drawn last */
    /* 0x7C */ s32 frame; /* of the bar, 0-11 */
    /* 0x80 */ s32 blink[6]; /* the time of each line's sprite */
    /* 0x98 */ u8 pad[0x10]; /* nothing uses it */
} MenuCursor;

/* One of FIGHTSTG_updateTechMenu's children: the cursor first, then the text windows */
typedef union TechMenuChild {
    MenuCursor *cursor;
    TextWindow *window;
} TechMenuChild;

/* One of FIGHTSTG_updatePlayerTurn's seven children: 0 the HUD, 1 the
   command or the confused menu, 2 the partner's view, 3 the open command's
   menu, 4 the pair switch or switch-in menu and 6 the shot camera, which
   substate 0 closes alike as tasks */
typedef union PlayerTurnChild {
    Task *task;
    HpDisplay *hud;
    CommandMenu *commandMenu;
    ConfusedMenu *confusedMenu;
    PartnerView *partnerView;
    DigivolveMenu *digivolveMenu;
    ItemMenu *itemMenu;
    TechMenu *techMenu;
    SwitchMenu *switchMenu;
    SwitchInMenu *switchInMenu;
    ShotCamera *shotCamera;
} PlayerTurnChild;

/* Which fighters FIGHTSTG_findFighters looks for */
typedef struct FighterFilter {
    /* 0x0 */ s32 side;
    /* 0x4 */ s32 type;
} FighterFilter;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
PlayerTurn *FIGHTSTG_startPlayerTurn(void);
BattleMessageBox *FIGHTSTG_createMessage(void);

/* The events they queue for WFIGHTMN, and its turn's substate */
void FIGHTSTG_setPlayerTurnStep(s32 substate);
s32 FIGHTSTG_isPlayerChoosing(void);

/* Shared between the overlay's objects */
extern RECT FIGHTSTG_fighterCameraRect; /* FIGHTSTG_updatePartnerView's layer */
void FIGHTSTG_updateHpTweens(HpDisplay *task, TextWindow **windows);
void FIGHTSTG_stepHpTween(HpTween *tween);
void FIGHTSTG_showFighterNames(HpDisplay *task, TextWindow **windows);
void FIGHTSTG_drawHud(HpDisplay *task, TextWindow **windows);
void FIGHTSTG_updateHud(HpDisplay *task, TextWindow **windows);
void FIGHTSTG_drawPartnerViewFrame(PartnerView *task, FighterCamera **cameras);
void FIGHTSTG_updatePartnerView(PartnerView *task, FighterCamera **cameras);
void FIGHTSTG_updatePlayerTurn(PlayerTurn *task, PlayerTurnChild *children);
void FIGHTSTG_drawSwitchInMenu(SwitchInMenu *task);
void FIGHTSTG_createSwitchInWindows(SwitchInMenu *task, SwitchInMenuWindows *w);
void FIGHTSTG_showSwitchInNames(SwitchInMenu *task, SwitchInMenuWindows *w, s32 visible);
void FIGHTSTG_showSwitchInChoices(SwitchInMenu *task, SwitchInMenuWindows *w, s32 visible);
void FIGHTSTG_updateSwitchInMenu(SwitchInMenu *task, SwitchInMenuWindows *w);
void FIGHTSTG_showDigivolveNames(DigivolveMenu *task);
void FIGHTSTG_updateDigivolveMenu(DigivolveMenu *task, void *children);
SwitchInMenu *FIGHTSTG_createSwitchInMenu(s32 *result);
void FIGHTSTG_drawItemMenu(ItemMenu *task);
void FIGHTSTG_createItemWindows(ItemMenu *task, ItemMenuWindows *w);
void FIGHTSTG_showItemPage(ItemMenu *task, ItemMenuWindows *w);
void FIGHTSTG_updateItemMenu(ItemMenu *task, ItemMenuWindows *w);
void FIGHTSTG_drawTechMenu(TechMenu *task);
void FIGHTSTG_createTechWindows(TechMenu *task, TechMenuChild *children);
void FIGHTSTG_showTechPage(TechMenu *task, TechMenuChild *children);
void FIGHTSTG_updateTechMenu(TechMenu *task, TechMenuChild *children);
void FIGHTSTG_drawMessageBox(BattleMessageBox *task);
void FIGHTSTG_stepMessage(BattleMessageBox *task, BattleMessageBoxWindows *windows);
void FIGHTSTG_updateMessage(BattleMessageBox *task, void *children);
void FIGHTSTG_showMessage(BattleMessageBox *task, s32 type, s32 *data);
void FIGHTSTG_finishMessage(BattleMessageBox *task);
void FIGHTSTG_showCommands(CommandMenu *task);
void FIGHTSTG_updateCommandMenu(CommandMenu *task, CommandMenuWindows *w);
void FIGHTSTG_drawPageArrows(PartnerInfo *task);
void FIGHTSTG_showPageButtons(PartnerInfo *task, TextWindow **windows);
void FIGHTSTG_drawStatsFrame(PartnerInfo *task);
void FIGHTSTG_showStats(PartnerInfo *task, TextWindow **windows);
void FIGHTSTG_drawTechIcons(PartnerInfo *task);
void FIGHTSTG_showTechs(PartnerInfo *task, TextWindow **windows);
void FIGHTSTG_updatePartnerInfo(PartnerInfo *task, TextWindow **windows);
void FIGHTSTG_updateSwitchMenu(SwitchMenu *task, SwitchMenuWindows *w);
void FIGHTSTG_updateConfusedMenu(ConfusedMenu *task, ConfusedMenuWindows *w);
void FIGHTSTG_updateCursor(MenuCursor *task);
MenuCursor *FIGHTSTG_createCursor(CursorLayout *layout);
extern DVECTOR FIGHTSTG_hpBars[2][4]; /* FIGHTSTG_drawHud's HP bars */
extern CVECTOR FIGHTSTG_hpBarColors[4];
extern DVECTOR FIGHTSTG_techGauge[4]; /* and its technique gauge */
extern CVECTOR FIGHTSTG_techGaugeColors[4];
extern s16 FIGHTSTG_iconX[2][3];
extern CursorLayout FIGHTSTG_commandCursor; /* FIGHTSTG_updateCommandMenu's cursor */
extern CursorLayout FIGHTSTG_changeCursor; /* FIGHTSTG_updateDigivolveMenu's cursor */
extern CursorLayout FIGHTSTG_itemCursor; /* FIGHTSTG_updateItemMenu's cursor */
extern CursorLayout FIGHTSTG_techCursor; /* FIGHTSTG_updateTechMenu's cursor */
extern CursorLayout FIGHTSTG_switchCursor; /* FIGHTSTG_updateSwitchMenu's cursor */
extern CursorLayout FIGHTSTG_pairCursors[2]; /* FIGHTSTG_updateSwitchInMenu's cursors */
extern CursorLayout FIGHTSTG_confusedCursor; /* FIGHTSTG_updateConfusedMenu's */
extern StatLine FIGHTSTG_statLines[13];
/* A result of FIGHTSTG_updateConfusedMenu */
typedef struct ConfusedMessage {
    /* 0x0 */ s16 result; /* for ConfusedMenu.queue[0] */
    /* 0x2 */ s16 text; /* its line of TEXT_BATTLE_MENU */
} ConfusedMessage;
extern ConfusedMessage FIGHTSTG_confusedMessages[16];
extern s16 FIGHTSTG_confusedLines[8]; /* FIGHTSTG_updateConfusedMenu's lines, shuffled */
extern RECT FIGHTSTG_cursorBarRect; /* where FIGHTSTG_drawCursorBar's bar is in VRAM */
extern DR_MOVE FIGHTSTG_cursorBarMoves[4]; /* FIGHTSTG_drawCursorBar's bar */
extern u_long FIGHTSTG_cursorBarOt[2]; /* their OT */
PartnerInfo *FIGHTSTG_createPartnerInfo(s32 partner, s32 page, s32 slot);
HpDisplay *FIGHTSTG_createHud(void);
CommandMenu *FIGHTSTG_createCommandMenu(s32 start, s32 *result);
PartnerView *FIGHTSTG_createPartnerView(void);
DigivolveMenu *FIGHTSTG_createDigivolveMenu(s32 *result);
ItemMenu *FIGHTSTG_createItemMenu(s32 *result);
TechMenu *FIGHTSTG_createTechMenu(s32 *result);
SwitchMenu *FIGHTSTG_createSwitchMenu(s32 *result, s32 *line, s32 canCancel);
SwitchInMenu *FIGHTSTG_createPairSwitchMenu(s32 *result, s32 *techResult);
ConfusedMenu *FIGHTSTG_createConfusedMenu(s32 *result, Task *partnerView, Task *shotCamera);
s32 FIGHTSTG_getPairDigimon(SwitchMenu *task, s32 index, s32 member);
void FIGHTSTG_drawSwitchMenu(SwitchMenu *task);
void FIGHTSTG_createSwitchWindows(SwitchMenu *task, SwitchMenuWindows *w);
void FIGHTSTG_showSwitchFighters(SwitchMenu *task, SwitchMenuWindows *w);
extern s32 FIGHTSTG_mpWindowX[4]; /* FIGHTSTG_createSwitchInWindows's MP windows' x */
void FIGHTSTG_setMessageName(BattleMessageBox *task, BattleMessageBoxWindows *w, s32 side, s32 index);
void FIGHTSTG_findFighters(BattleMessageBox *task, FighterFilter *filter);
void FIGHTSTG_showFightersMessage(BattleMessageBox *task, BattleMessageBoxWindows *w, BattleMessage *msg);
void FIGHTSTG_showConfusedCommands(ConfusedMenu *task);
void FIGHTSTG_drawConfusedMessageBox(ConfusedMenu *task);
void FIGHTSTG_stepConfusedMessage(ConfusedMenu *task, ConfusedMenuWindows *w);
void FIGHTSTG_setConfusedName(ConfusedMenu *task, ConfusedMenuWindows *windows, s32 side);
void FIGHTSTG_showConfusedMessage(ConfusedMenu *task, s32 index, s32 unused);
void FIGHTSTG_drawCursorSprites(MenuCursor *task);
void FIGHTSTG_drawCursorBar(s32 arg);

#endif /* FIGHTSTG_MENU_H */
