#ifndef STSTATUS_H
#define STSTATUS_H

/* STSTATUS.PRO: the field menu's screens. createFieldMenu (in main) lists
   the options; picking one switches to this mode, which opens the screen of
   FIELD_MENU_CHOICE (STSTATUS_screens) and goes back to the field menu when
   it closes. Its strings are in files 0xB1, 0x6B, 0x64, 0x4F, 0x48, 0xA3 and
   0x9C. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STSTATUS_##name

/* The menu's sprite sheet; the next file is its texture archive */
#if VERSION_US || VERSION_JP
#define FILE_STATUS_SPRITES 0x3F4
#define FILE_STATUS_BG 0x78A /* +2 in the second half of the game */
#elif VERSION_EU
#define FILE_STATUS_SPRITES 0x404
#define FILE_STATUS_BG 0x799
#endif

/* The mode's main task (STSTATUS_createMenu) */
typedef struct FieldMenuScreen {
    TASK_HEADER(FieldMenuScreen);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 bgScroll;
    /* 0x58 */ s32 bgTick;
    /* 0x5C */ s32 pad; /* never read or written */
    /* 0x60 */ s32 gameHalf; /* STSTATUS_getGameHalf */
    /* 0x64 */ s32 bgFile;
    /* 0x68 */ s32 bgFile2;
    /* 0x6C */ s32 bgArchive;
    /* 0x70 */ s32 bgArchive1;
    /* 0x74 */ s32 bgArchive2;
} FieldMenuScreen;

typedef struct FieldMenuScreenChildren {
    /* 0x0 */ FieldMenu *fieldMenu;
    /* 0x4 */ Task *screen;
} FieldMenuScreenChildren;

/* A screen of the field menu (STSTATUS_screens) */
typedef struct StatusScreen {
    TASK_HEADER(StatusScreen);
    /* 0x50 */ FieldMenuScreen *menu;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
} StatusScreen;

/* The screens with the party's pages (STSTATUS_createCardScreen, STSTATUS_createDemoScreen) */
typedef struct PartyScreen {
    TASK_HEADER(PartyScreen);
    /* 0x50 */ FieldMenuScreen *menu;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 count; /* party members */
    /* 0x60 */ s32 frames[3]; /* of their portraits' animations */
    /* 0x6C */ s32 frameTime;
    /* 0x70 */ s32 choice; /* 0, 1: the options */
    /* 0x74 */ PanelAnim pageFades[3];
    /* 0xA4 */ PanelAnim fades[2];
    /* 0xC4 */ PanelAnim fade;
} PartyScreen;

/* The field menu's first screen (STSTATUS_createItemScreen) */
typedef struct ItemScreen {
    TASK_HEADER(ItemScreen);
    /* 0x050 */ FieldMenuScreen *menu;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 count; /* party members */
    /* 0x060 */ s32 frames[3]; /* of the partners' portraits */
    /* 0x06C */ s32 frameTime;
    /* 0x070 */ s32 option;
    /* 0x074 */ u8 pad74[4]; /* never read or written */
    /* 0x078 */ s32 item; /* the chosen one */
    /* 0x07C */ s32 itemIndex;
    /* 0x080 */ s32 itemShown;
    /* 0x084 */ s16 items[0x194]; /* of the chosen option's list */
    /* 0x3AC */ s32 itemCount;
    /* 0x3B0 */ s32 member; /* the one an item is used on */
    /* 0x3B4 */ s32 cursorShown;
    /* 0x3B8 */ s32 cursorFrame;
    /* 0x3BC */ s32 cursorTime;
    /* 0x3C0 */ s32 blink; /* the help arrow */
    /* 0x3C4 */ s32 blinkFrame;
    /* 0x3C8 */ s32 blinkTime;
    /* 0x3CC */ PanelAnim pageFades[3];
    /* 0x3FC */ PanelAnim fades[2];
    /* 0x41C */ PanelAnim fades2[2];
    /* 0x43C */ PanelAnim fade;
    /* 0x44C */ u8 pad44C[0x45C - 0x44C]; /* never read or written */
} ItemScreen;

typedef struct ItemScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ PartnerPage pages[3];
    /* 0x88 */ TextWindow *help;
    /* 0x8C */ TextWindow *kind;
    /* 0x90 */ TextWindow *answers[2];
    /* 0x98 */ Cursor *answerCursor;
    /* 0x9C */ TextWindow *money;
    /* 0xA0 */ TextWindow *moneyLabel;
    /* 0xA4 */ TextWindow *options[5];
    /* 0xB8 */ Cursor *optionCursor;
    /* 0xBC */ TextWindow *itemName;
    /* 0xC0 */ TextWindow *equippedLabel;
    /* 0xC4 */ TextWindow *equipped; /* how many of the item are equipped */
    /* 0xC8 */ TextWindow *ownedLabel;
    /* 0xCC */ TextWindow *owned;
    /* 0xD0 */ struct ItemList *panel; /* STSTATUS_createItemList's, while open */
} ItemScreenWindows;

/* The field menu's fifth screen (STSTATUS_createStatusScreen) */
typedef struct StatsScreen {
    TASK_HEADER(StatsScreen);
    /* 0x050 */ FieldMenuScreen *menu;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 count; /* party members */
    /* 0x060 */ s32 frames[3]; /* of the partners' portraits */
    /* 0x06C */ s32 frameTime;
    /* 0x070 */ s32 unk70; /* only cleared, when the equipment panel closes */
    /* 0x074 */ s32 option; /* the options' cursor */
    /* 0x078 */ s32 cursorShown;
    /* 0x07C */ s32 member; /* in the party */
    /* 0x080 */ s32 cursorFrame;
    /* 0x084 */ s32 cursorTime;
    /* 0x088 */ PanelAnim pageFades[3];
    /* 0x0B8 */ PanelAnim fades[2];
    /* 0x0D8 */ PanelAnim panelFades[6];
    /* 0x138 */ PanelAnim fade;
    /* 0x148 */ void (*previewStats)(struct StatsScreen *screen, s32 slot, s32 item);
} StatsScreen;

typedef struct StatsScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ PartnerPage pages[3];
    /* 0x88 */ TextWindow *help;
    /* 0x8C */ TextWindow *help2;
    /* 0x90 */ TextWindow *options[2];
    /* 0x98 */ Cursor *cursor;
    /* 0x9C */ TextWindow *exp;
    /* 0xA0 */ TextWindow *expLabel;
    /* 0xA4 */ TextWindow *slotTitle;
    /* 0xA8 */ TextWindow *slots[3]; /* the partner's getPartnerSlots entries */
    /* 0xB4 */ TextWindow *equipTitle;
    /* 0xB8 */ TextWindow *equip[6];
    /* 0xD0 */ TextWindow *values[13]; /* the stats of STSTATUS_equipStats */
    /* 0x104 */ TextWindow *tp;
    /* 0x108 */ TextWindow *tpLabel;
    /* 0x10C */ void *panel; /* STSTATUS_createDigivolvePanel's or STSTATUS_createEquipPanel's, while open */
} StatsScreenWindows;

/* A panel of the fifth screen (STSTATUS_createDigivolvePanel) */
typedef struct DigivolvePanel {
    TASK_HEADER(DigivolvePanel);
    /* 0x50 */ StatsScreen *screen;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 slot; /* the cursors' rows */
    /* 0x60 */ s32 option;
    /* 0x64 */ s32 tech; /* the list's row */
    /* 0x68 */ s32 techCount; /* the entry's techniques */
    /* 0x6C */ s32 fromEntry; /* the list shows the slot's techniques, not the partner's */
    /* 0x70 */ s32 member;
    /* 0x74 */ s16 slots[4]; /* getPartnerSlots */
    /* 0x7C */ s32 slotCount; /* the slots holding an entry (4 on) */
    /* 0x80 */ s32 listShown;
    /* 0x84 */ MenuLerp scroll; /* its value is added to the list's y */
    /* 0xA0 */ s32 blink; /* the help arrow */
    /* 0xA4 */ s32 blinkFrame;
    /* 0xA8 */ s32 time;
    /* 0xAC */ PanelAnim fades[5];
} DigivolvePanel;

typedef struct DigivolvePanelWindows {
    /* 0x00 */ TextWindow *slots[3];
    /* 0x0C */ Cursor *slotCursor;
    /* 0x10 */ TextWindow *partnerName;
    /* 0x14 */ TextWindow *choiceTitle;
    /* 0x18 */ TextWindow *options[2];
    /* 0x20 */ Cursor *optionCursor;
    /* 0x24 */ Cursor *listCursor;
    /* 0x28 */ TextWindow *entryName;
    /* 0x2C */ TextWindow *skillLevel;
    /* 0x30 */ TextWindow *skillLabel;
    /* 0x34 */ TextWindow *list[13]; /* two columns: 6 and 7 */
    /* 0x68 */ TextWindow *values[6];
    /* 0x80 */ TextWindow *title;
    /* 0x84 */ TextWindow *help;
    /* 0x88 */ TextWindow *mpLabel;
    /* 0x8C */ TextWindow *mp;
} DigivolvePanelWindows;

/* A list of the fifth screen (STSTATUS_createEquipPanel) */
typedef struct EquipPanel {
    TASK_HEADER(EquipPanel);
    /* 0x050 */ StatsScreen *screen;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 partner;
    /* 0x060 */ s32 slot; /* the equipment slot being changed */
    /* 0x064 */ s32 cursor; /* the list's row */
    /* 0x068 */ s32 count; /* items */
    /* 0x06C */ s32 scroll; /* the first one shown */
    /* 0x070 */ s32 arrowShown; /* the scroll arrows blink */
    /* 0x074 */ s32 arrowTime;
    /* 0x078 */ s16 items[0x194]; /* those that fit the slot, -1: remove */
    /* 0x3A0 */ s16 owned[0x194]; /* listItems' result, sorted into items */
    /* 0x6C8 */ s32 showSlot; /* draw the slot's item icon */
    /* 0x6CC */ PanelAnim panels[4];
} EquipPanel;

typedef struct EquipPanelWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *slots[6]; /* the equipment */
    /* 0x1C */ Cursor *cursor;
    /* 0x20 */ TextWindow *listTitle;
    /* 0x24 */ Cursor *listCursor;
    /* 0x28 */ struct {
        TextWindow *name;
        TextWindow *times; /* the "x" before count */
        TextWindow *count; /* how many are owned */
    } rows[8];
    /* 0x88 */ TextWindow *help;
    /* 0x8C */ TextWindow *kind;
    /* 0x90 */ TextWindow *slotTitle;
    /* 0x94 */ TextWindow *slotItem;
    /* 0x98 */ ScrollBar *scrollBar; /* for more than 8 items */
    /* 0x9C */ u8 pad[0xA4 - 0x9C]; /* never read or written */
} EquipPanelWindows;

/* A list of the first screen (STSTATUS_createItemList) */
typedef struct ItemList {
    TASK_HEADER(ItemList);
    /* 0x050 */ ItemScreen *screen;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 list; /* into STSTATUS_itemLists */
    /* 0x060 */ s32 startItem; /* an item to put the cursor on */
    /* 0x064 */ s32 active; /* the player can move the cursor */
    /* 0x068 */ s32 count;
    /* 0x06C */ s16 items[0x194];
    /* 0x394 */ s16 bag[0x194];
    /* 0x6BC */ s32 cursor;
    /* 0x6C0 */ s32 arrowFrame; /* the page arrows' palette */
    /* 0x6C4 */ s32 time;
    /* 0x6C8 */ s32 page; /* of 16 items */
    /* 0x6CC */ s32 pageCount;
    /* 0x6D0 */ s32 hasPrev; /* pages before and after this one */
    /* 0x6D4 */ s32 hasNext;
    /* 0x6D8 */ u8 pad[4]; /* never read or written */
    /* 0x6DC */ PanelAnim fades[3];
} ItemList;

typedef struct ItemListWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *items[8][2];
    /* 0x44 */ TextWindow *page;
    /* 0x48 */ TextWindow *pageSeparator;
    /* 0x4C */ TextWindow *pageCount;
    /* 0x50 */ TextWindow *prev; /* arrows */
    /* 0x54 */ TextWindow *next;
    /* 0x58 */ Cursor *cursor;
    /* 0x5C */ u8 pad[0x6C - 0x5C]; /* never read or written */
} ItemListWindows;

/* The screens' helpers (STSTATUS_data.funcs) */
typedef struct StatusFuncs {
    /* 0x00 */ void (*loadFiles)(void);
    /* 0x04 */ s32 (*filesLoading)(void);
    /* 0x08 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x0C */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x10 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x14 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x18 */ s32 *(*getList)(s32 list, s32 index);
    /* 0x1C */ s32 (*listItems)(s32 list, s16 *out); /* 6, 7: special lists; returns the count */
    /* 0x20 */ s32 (*canEquip)(s32 partner, s32 slot, s32 item);
    /* 0x24 */ void (*equip)(s32 partner, s32 slot, s32 item);
} StatusFuncs;

/* Where the game is (STSTATUS_areaFuncs) */
typedef struct StatusAreaFuncs {
    /* 0x0 */ s32 (*getGameHalf)(void); /* 0, 1, or -1 outside of the field */
    /* 0x4 */ s32 (*getArea)(void);
    /* 0x8 */ void (*getVisitedAreas)(s32 *out);
} StatusAreaFuncs;

/* The windows of the screens STSTATUS_updateCardScreen and STSTATUS_updateDemoScreen */
typedef struct PartyScreenWindows {
    /* 0x00 */ PartnerPage pages[3];
    /* 0x84 */ TextWindow *help; /* two lines */
    /* 0x88 */ TextWindow *hint;
    /* 0x8C */ TextWindow *choiceTitle;
    /* 0x90 */ TextWindow *options[2];
    /* 0x98 */ Cursor *cursor;
    /* 0x9C */ ScreenFade *fader;
} PartyScreenWindows;

/* The map screen (STSTATUS_createMapScreen); the offsets are the European version's,
   the USA version's are 4 more from 0x64 to 0x188 */
typedef struct StatusMapScreen {
    TASK_HEADER(StatusMapScreen);
    /* 0x050 */ FieldMenuScreen *menu;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 height;
    /* 0x060 */ s32 top; /* where the map starts */
#if VERSION_US
    /* 0x064 */ s32 hoverY;
#endif
    /* 0x064 */ u8 pad64[4]; /* never read or written */
    /* 0x068 */ s32 file; /* FILE_STATUS_BG or the next one but one */
    /* 0x06C */ u8 pad6C[0x78 - 0x6C]; /* never read or written */
    /* 0x078 */ s32 gameHalf;
    /* 0x07C */ s32 progress; /* which towns are drawn */
    /* 0x080 */ s32 archive; /* entries of file: the map */
    /* 0x084 */ s32 archive1; /* the towns */
    /* 0x088 */ s32 archive2; /* the marks */
    /* 0x08C */ s32 textFile;
    /* 0x090 */ s32 scrollX;
    /* 0x094 */ s32 scrollY;
    /* 0x098 */ s32 homeX; /* the current area's mark */
    /* 0x09C */ s32 homeY;
    /* 0x0A0 */ s32 homeFrame;
    /* 0x0A4 */ u8 padA4[4]; /* never read or written */
    /* 0x0A8 */ s32 visited[47]; /* the areas, spot i at i - 1 */
    /* 0x164 */ s32 cursorX;
    /* 0x168 */ s32 cursorY;
    /* 0x16C */ s32 ready;
    /* 0x170 */ s32 cursorFrame;
    /* 0x174 */ s32 time;
    /* 0x178 */ s32 speedX;
    /* 0x17C */ s32 speedY;
    /* 0x180 */ s32 hovering;
    /* 0x184 */ s32 spot; /* the area under the cursor */
#if VERSION_EU || VERSION_JP
    /* 0x188 */ s32 hoverX; /* where the cursor was when it got there */
    /* 0x18C */ s32 hoverY;
#endif
    /* 0x190 */ PanelAnim fade;
} StatusMapScreen;

/* The screen of STSTATUS_createSortScreen */
typedef struct SortScreen {
    TASK_HEADER(SortScreen);
    /* 0x50 */ FieldMenuScreen *menu;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 count; /* party members */
    /* 0x60 */ s32 frames[3]; /* of the partners' portraits */
    /* 0x6C */ s32 frameTime;
    /* 0x70 */ s32 arrowShown; /* the swap's arrow */
    /* 0x74 */ s32 arrowFrame;
    /* 0x78 */ s32 arrowTime;
    /* 0x7C */ s32 cursorShown;
    /* 0x80 */ s32 cursor;
    /* 0x84 */ s32 first; /* the members to swap */
    /* 0x88 */ s32 second;
    /* 0x8C */ s32 cursorFrame;
    /* 0x90 */ s32 cursorTime;
    /* 0x94 */ PanelAnim pageFades[3];
    /* 0xC4 */ PanelAnim fades[2];
    /* 0xE4 */ PanelAnim fade;
} SortScreen;

typedef struct SortScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ PartnerPage pages[3];
    /* 0x88 */ TextWindow *help;
} SortScreenWindows;

/* A party member's techniques on the tech screen */
typedef struct TechRow {
    /* 0x00 */ s16 slots[4]; /* getPartnerSlots */
    /* 0x08 */ PartnerEntry entries[3];
    /* 0x44 */ s16 techs[5]; /* the ones it can use here */
    /* 0x50 */ s32 techCount;
} TechRow;

/* The tech screen's windows */
typedef struct TechScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ PartnerPage pages[3];
    /* 0x88 */ TextWindow *help;
    /* 0x8C */ TextWindow *help2;
    /* 0x90 */ TextWindow *mpLabel;
    /* 0x94 */ TextWindow *mp; /* the technique's cost */
    /* 0x98 */ TextWindow *listTitle;
    /* 0x9C */ TextWindow *techs[5];
    /* 0xB0 */ Cursor *cursor;
} TechScreenWindows;

/* The tech screen (STSTATUS_createTechScreen) */
typedef struct TechScreen {
    TASK_HEADER(TechScreen);
    /* 0x050 */ FieldMenuScreen *menu;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 count; /* party members */
    /* 0x060 */ s32 frames[3]; /* of the partners' portraits */
    /* 0x06C */ s32 frameTime;
    /* 0x070 */ s32 cursorFrame;
    /* 0x074 */ s32 cursorTime;
    /* 0x078 */ s32 memberShown; /* the user's cursor */
    /* 0x07C */ s32 member; /* who uses the technique */
    /* 0x080 */ s32 targetShown; /* the target's cursor */
    /* 0x084 */ s32 target;
    /* 0x088 */ s32 cursor;
    /* 0x08C */ TechRow rows[3];
    /* 0x188 */ s32 blink; /* the help arrow */
    /* 0x18C */ s32 blinkFrame;
    /* 0x190 */ s32 blinkTime;
    /* 0x194 */ PanelAnim pageFades[3];
    /* 0x1C4 */ PanelAnim fades[2];
    /* 0x1E4 */ PanelAnim fade;
} TechScreen;

/* An area on the map screen */
typedef struct StatusMapSpot {
    /* 0x0 */ s32 sprite;
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 y;
} StatusMapSpot;

typedef struct StatusMapPoint {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} StatusMapPoint;

/* The screens' tables, item lists and helpers, in one object: the functions
   that use two of them keep its address in a register */
typedef struct StatusData {
    /* 0x000 */ PartnerAnim *partnerAnims; /* the partners' portraits */
    /* 0x004 */ WindowPos *layout; /* where the windows go, and their strings */
    /* 0x008 */ StatusMapSpot *spots; /* the map's areas, from 1 */
    /* 0x00C */ StatusMapPoint *towns; /* from 1 */
    /* 0x010 */ s16 items[404]; /* ITEM_FUNCS.list(2) */
    /* 0x338 */ s32 itemCount;
    /* 0x33C */ s16 items2[404]; /* ITEM_FUNCS.list(3) */
    /* 0x664 */ s32 item2Count;
    /* 0x668 */ StatusFuncs funcs;
} StatusData;

/* card_screen.c */
void STSTATUS_updateScene(Task *task, FieldMenuScreen **child);
void STSTATUS_createCardWindows(PartyScreen *screen, PartyScreenWindows *windows);
void STSTATUS_showCardPage(PartyScreen *screen, PartyScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showCardChoices(PartyScreen *screen, PartyScreenWindows *windows, s32 show);
void STSTATUS_drawCardScreen(PartyScreen *screen);
void STSTATUS_runCardScreen(PartyScreen *screen, PartyScreenWindows *windows);
void STSTATUS_updateCardScreen(PartyScreen *screen, PartyScreenWindows *windows);
Task *STSTATUS_createCardScreen(FieldMenuScreen *menu, s32 extra);
void STSTATUS_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STSTATUS_drawFader(ScreenFade *task);
void STSTATUS_updateFader(ScreenFade *task);
ScreenFade *STSTATUS_createFader(void);

/* demo_screen.c */
void STSTATUS_createDemoWindows(PartyScreen *screen, PartyScreenWindows *windows);
void STSTATUS_showDemoPage(PartyScreen *screen, PartyScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showDemoChoices(PartyScreen *screen, PartyScreenWindows *windows, s32 show);
void STSTATUS_drawDemoScreen(PartyScreen *screen);
void STSTATUS_runDemoScreen(PartyScreen *screen, PartyScreenWindows *windows);
void STSTATUS_updateDemoScreen(PartyScreen *screen, PartyScreenWindows *windows);
Task *STSTATUS_createDemoScreen(FieldMenuScreen *menu, s32 extra);

/* equip_panel.c */
void STSTATUS_createEquipWindows(EquipPanel *panel, EquipPanelWindows *windows);
void STSTATUS_showEquipment(EquipPanel *panel, EquipPanelWindows *windows, s32 show);
void STSTATUS_showEquipItem(EquipPanel *panel, EquipPanelWindows *windows, s32 item);
void STSTATUS_showEquipList(EquipPanel *panel, EquipPanelWindows *windows, s32 show);
void STSTATUS_showSlotItem(EquipPanel *panel, EquipPanelWindows *windows, s32 show);
void STSTATUS_drawEquipPanel(EquipPanel *panel);
void STSTATUS_runEquipPanel(EquipPanel *panel, EquipPanelWindows *windows);
void STSTATUS_updateEquipPanel(EquipPanel *panel, EquipPanelWindows *windows);
EquipPanel *STSTATUS_createEquipPanel(StatsScreen *screen);

/* digivolve_panel.c */
void STSTATUS_createDigivolveWindows(DigivolvePanel *panel, DigivolvePanelWindows *windows);
void STSTATUS_showDigivolveSlots(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show);
void STSTATUS_showDigivolveChoices(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show);
void STSTATUS_showDigivolveStats(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show);
void STSTATUS_scrollTechList(DigivolvePanel *panel, DigivolvePanelWindows *windows);
void STSTATUS_showTechCost(DigivolvePanel *panel, DigivolvePanelWindows *windows, s32 show);
void STSTATUS_drawDigivolvePanel(DigivolvePanel *panel);
void STSTATUS_runDigivolvePanel(DigivolvePanel *panel, DigivolvePanelWindows *windows);
void STSTATUS_updateDigivolvePanel(DigivolvePanel *panel, DigivolvePanelWindows *windows);
DigivolvePanel *STSTATUS_createDigivolvePanel(StatsScreen *screen);

/* status_screen.c */
void STSTATUS_createStatusWindows(StatsScreen *screen, StatsScreenWindows *windows);
void STSTATUS_showStatusPage(StatsScreen *screen, StatsScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showChosenPage(StatsScreen *screen, StatsScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showStatusChoices(StatsScreen *screen, StatsScreenWindows *windows, s32 show);
void STSTATUS_showPartnerSlots(StatsScreen *screen, StatsScreenWindows *windows, s32 show);
void STSTATUS_showPartnerEquipment(StatsScreen *screen, StatsScreenWindows *windows, s32 show);
void STSTATUS_showPartnerStats(StatsScreen *screen, StatsScreenWindows *windows, s32 show);
void STSTATUS_previewStats(StatsScreen *screen, s32 slot, s32 item);
void STSTATUS_drawStatusScreen(StatsScreen *screen);
void STSTATUS_runStatusChoice(StatsScreen *screen, StatsScreenWindows *windows);
void STSTATUS_runStatusScreen(StatsScreen *screen, StatsScreenWindows *windows);
void STSTATUS_updateStatusScreen(StatsScreen *screen, StatsScreenWindows *windows);
Task *STSTATUS_createStatusScreen(FieldMenuScreen *menu, s32 extra);

/* item_screen.c */
void STSTATUS_createItemWindows(ItemScreen *screen, ItemScreenWindows *windows);
void STSTATUS_showItemPage(ItemScreen *screen, ItemScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showMoney(ItemScreen *screen, ItemScreenWindows *windows, s32 show);
void STSTATUS_showItemLists(ItemScreen *screen, ItemScreenWindows *windows, s32 show);
void STSTATUS_showChosenItem(ItemScreen *screen, s32 show);
void STSTATUS_showItemHelp(ItemScreen *screen, s32 mode);
void STSTATUS_fadeItemInfo(ItemScreen *screen, s32 show);
s32 STSTATUS_itemInfoFaded(ItemScreen *screen);
void STSTATUS_useItem(ItemScreen *screen, ItemScreenWindows *windows);
void STSTATUS_drawItemScreen(ItemScreen *screen);
void STSTATUS_runItemScreen(ItemScreen *screen, ItemScreenWindows *windows);
void STSTATUS_updateItemScreen(ItemScreen *screen, ItemScreenWindows *windows);
Task *STSTATUS_createItemScreen(FieldMenuScreen *menu, s32 extra);

/* item_list.c */
void STSTATUS_createItemListWindows(ItemList *panel, ItemListWindows *windows);
void STSTATUS_showItemListPage(ItemList *panel, ItemListWindows *windows, s32 show);
void STSTATUS_refreshItemList(ItemList *panel, ItemListWindows *windows);
s32 STSTATUS_moveItemListCursor(ItemList *panel, ItemListWindows *windows);
void STSTATUS_runItemList(ItemList *panel, ItemListWindows *windows);
void STSTATUS_drawItemList(ItemList *panel);
void STSTATUS_updateItemList(ItemList *panel, ItemListWindows *windows);
ItemList *STSTATUS_createItemList(ItemScreen *screen, s32 list, s32 item);

/* tech_screen.c */
void STSTATUS_fillItemList(ItemList *panel);
s32 STSTATUS_listFieldTechs(TechScreen *screen, s32 member);
s32 STSTATUS_getChosenTech(TechScreen *screen);
s32 STSTATUS_getTechHealing(s32 partner, s32 tech);
s32 STSTATUS_useTech(TechScreen *screen, TechScreenWindows *windows, s32 failSound);
void STSTATUS_createTechWindows(TechScreen *screen, TechScreenWindows *windows);
void STSTATUS_showTechPage(TechScreen *screen, TechScreenWindows *windows, s32 member, s32 show);
void STSTATUS_showTechList(TechScreen *screen, TechScreenWindows *windows, s32 show);
void STSTATUS_showChosenTech(TechScreen *screen, TechScreenWindows *windows, s32 show);
void STSTATUS_drawTechScreen(TechScreen *screen);
void STSTATUS_runTechScreen(TechScreen *screen, TechScreenWindows *windows);
void STSTATUS_updateTechScreen(TechScreen *screen, TechScreenWindows *windows);
Task *STSTATUS_createTechScreen(FieldMenuScreen *menu, s32 extra);

/* sort_screen.c */
void STSTATUS_createSortWindows(SortScreen *screen, SortScreenWindows *windows);
void STSTATUS_showSortPage(SortScreen *screen, SortScreenWindows *windows, s32 member, s32 show);
void STSTATUS_drawSortScreen(SortScreen *screen);
void STSTATUS_runSortScreen(SortScreen *screen, SortScreenWindows *windows);
void STSTATUS_updateSortScreen(SortScreen *screen, SortScreenWindows *windows);
Task *STSTATUS_createSortScreen(FieldMenuScreen *menu, s32 extra);

/* map_screen.c */
void STSTATUS_showMapArea(StatusMapScreen *screen, TextWindow **windows);
void STSTATUS_moveMapCursor(StatusMapScreen *screen, s32 dx, s32 dy);
void STSTATUS_runMapScreen(StatusMapScreen *screen, TextWindow **windows);
void STSTATUS_drawMapScreen(StatusMapScreen *screen);
void STSTATUS_updateMapScreen(StatusMapScreen *screen, TextWindow **windows);
Task *STSTATUS_createMapScreen(FieldMenuScreen *menu, s32 extra);

/* menu.c */
void STSTATUS_setScrollBarX(ScrollBar *bar, s32 x, s32 width);
void STSTATUS_setScrollBarRange(ScrollBar *bar, s32 top, s32 bottom);
void STSTATUS_setScrollBarCount(ScrollBar *bar, s32 pageSize, s32 count);
void STSTATUS_setScrollBarPos(ScrollBar *bar, s32 pos);
void STSTATUS_updateScrollBar(ScrollBar *bar);
ScrollBar *STSTATUS_createScrollBar(void);
void STSTATUS_runScreens(FieldMenuScreen *menu, FieldMenuScreenChildren *children);
void STSTATUS_drawBackground(FieldMenuScreen *menu);
void STSTATUS_updateMenu(FieldMenuScreen *menu, FieldMenuScreenChildren *children);
FieldMenuScreen *STSTATUS_createMenu(void);
void STSTATUS_loadFiles(void);
s32 STSTATUS_filesLoading(void);
void STSTATUS_startFade(PanelAnim *fade, s32 fadeIn);
s32 STSTATUS_updateFade(PanelAnim *fade);
void STSTATUS_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STSTATUS_updateLerp(MenuLerp *lerp);

/* helpers.c */
s32 *STSTATUS_getTowns(s32 list, s32 index);
s32 STSTATUS_listItems(s32 list, s16 *out);
s32 STSTATUS_listEquipItems(s16 *out);
s32 STSTATUS_listItemsOfKind(s32 kind, s16 *out);
s32 STSTATUS_canEquip(s32 partner, s32 slot, s32 item);
void STSTATUS_equip(s32 partner, s32 slot, s32 item);
s32 STSTATUS_getGameHalf(void);
s32 STSTATUS_getArea(void);
void STSTATUS_getVisitedAreas(s32 *out);

/* What the items that raise a stat raise (STSTATUS_statItems), up to a kind of -1 */
typedef struct StatusStatItem {
    /* 0x0 */ s16 kind; /* the item's second byte */
    /* 0x2 */ s16 stat; /* in PartnerStats.stats */
    /* 0x4 */ s16 max;
} StatusStatItem;

/* STSTATUS's data, in its order: each object's tables, then the last one's
   (helpers.c), which all the objects read */
extern s32 STSTATUS_pageStats[]; /* the stats a page shows (one table a screen) */
extern s32 STSTATUS_pageStats2[];
extern s32 STSTATUS_slotStrings[]; /* the strings of the empty equipment slots */
extern s32 STSTATUS_kindStrings[]; /* the strings of the item kinds, from 1 */
extern s32 STSTATUS_slotLists[]; /* the item list of each equipment slot */
extern s32 STSTATUS_panelStats[]; /* the stats the fifth screen's panel shows */
extern s32 STSTATUS_pageStats4[];
extern s32 STSTATUS_equipStats[]; /* the stats screen 4 shows */
extern s32 STSTATUS_equipStrings[];
extern s32 STSTATUS_itemLists[]; /* the item lists of the first screen's options */
extern s32 STSTATUS_pageStats0[];
extern s32 STSTATUS_kindStrings0[]; /* the strings of the item kinds, from 0 */
extern StatusStatItem STSTATUS_statItems[];
extern s32 STSTATUS_pageStats8[];
extern s32 STSTATUS_techSprites[]; /* the sprites of the technique counts */
extern s32 STSTATUS_pageStats9[];
extern s32 STSTATUS_cursorFrames[]; /* the map cursor's frames */
extern Task *(*STSTATUS_screens[2][7])(FieldMenuScreen *menu, s32 extra);
extern PartnerAnim STSTATUS_partnerAnims[];
extern WindowPos STSTATUS_layout[];
extern StatusMapSpot STSTATUS_spots[];
extern StatusMapPoint STSTATUS_towns[];
extern s32 STSTATUS_townList0[]; /* the towns the map shows, up to 0 */
extern s32 STSTATUS_townList1[];
extern s32 STSTATUS_townList2[];
extern s32 STSTATUS_townList3[];
extern s32 STSTATUS_noTowns;
extern s32 *STSTATUS_townLists[][5];
extern StatusData STSTATUS_data;
extern u8 STSTATUS_equipKinds[]; /* the item kinds of item list 5 */
extern u8 STSTATUS_mapAreas[]; /* the map's area of each field map */
extern StatusAreaFuncs STSTATUS_areaFuncs;

#endif /* STSTATUS_H */
