#ifndef STGDGLAB_H
#define STGDGLAB_H

/* STGDGLAB.PRO: the Digimon Lab (MODE_DIGI_LAB; "デジモンラボ" in STAGSLCT's list
   of scenes), where the partners' digivolutions are set. Its main menu
   (STGDGLAB_createMenu) opens one of three screens (STGDGLAB_screens,
   TEXT_DIGI_LAB strings 2-4): "Switch Digimon" swaps a party member with
   another partner, "Digivolve" sets a partner's three slots
   (setPartnerSlots) and loads its techniques, and "Digivolve Chart" checks
   the recipes of STGDGLAB_data (how many of a few ids are needed) against
   the entries a partner has (listPartnerEntries). On the way in it packs the
   party (STGDGLAB_packParty) so that the members come first. Its strings are
   in TEXT_DIGI_LAB, TEXT_DIGIMON_NAMES, TEXT_DIGIMON_INFO, TEXT_SKILL_NAMES
   and TEXT_SKILL_INFO. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STGDGLAB_##name

/* The lab's sprite sheet; the next file is its texture archive */
#if VERSION_US || VERSION_JP
#define FILE_LAB_SPRITES 0x2B6
#elif VERSION_EU
#define FILE_LAB_SPRITES 0x2C5
#endif

/* The lab's main menu (STGDGLAB_createMenu) */
typedef struct LabMenu {
    TASK_HEADER(LabMenu);
    /* 0x050 */ struct Lab *lab;
    /* 0x054 */ s32 picked;
    /* 0x058 */ s32 layer;
    /* 0x05C */ s32 depth;
    /* 0x060 */ s32 choice; /* into STGDGLAB_screens */
    /* 0x064 */ PanelAnim panels[9];
    /* 0x0F4 */ s32 animTime;
    /* 0x0F8 */ s32 blinkTime;
    /* 0x0FC */ s32 frames[5]; /* the party's animations, the picked partner's,
                                  then the blinking frame's CLUT row */
    /* 0x110 */ s32 memberCount; /* the members left and right go through */
    /* 0x114 */ void (*open)(struct LabMenu *menu);
    /* 0x118 */ void (*close)(struct LabMenu *menu);
} LabMenu;

typedef struct LabMenuWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *options[3]; /* the three screens */
    /* 0x10 */ TextWindow *label;
    /* 0x14 */ TextWindow *statLabels[5];
    /* 0x28 */ TextWindow *statValues[5]; /* STGDGLAB_menuStats */
    /* 0x3C */ TextWindow *name;
    /* 0x40 */ TextWindow *entriesHint; /* circle shows the entries (grey
                                           without a partner) */
    /* 0x44 */ Cursor *cursor;
    /* 0x48 */ struct LabEntryList *panel;
} LabMenuWindows;

/* A recipe of STGDGLAB_data's tables */
typedef struct LabRecipe {
    /* 0x0 */ s16 needed; /* how many of the ids the partner must have */
    /* 0x2 */ s16 ids[5]; /* 0: none */
} LabRecipe;

/* Where a window of the menu or the first screen goes, and its string */
typedef struct LabWindowPos {
    /* 0x0 */ s32 string; /* of TEXT_DIGI_LAB, -1: none */
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 y;
} LabWindowPos;

/* The main menu's first screen (STGDGLAB_createPartyScreen) */
typedef struct LabPartyScreen {
    TASK_HEADER(LabPartyScreen);
    /* 0x050 */ struct Lab *lab;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 pageRow; /* 1: the page is 0x7A lower, 0 (at the top)
                                while the entry list is open */
    /* 0x060 */ s32 pick; /* into partners */
    /* 0x064 */ PanelAnim panels[5];
    /* 0x0B4 */ u8 pad[0xC4 - 0xB4]; /* never read or written */
    /* 0x0C4 */ s32 animTime;
    /* 0x0C8 */ s32 blinkTime;
    /* 0x0CC */ s32 frames[3]; /* the party's animation frames */
    /* 0x0D8 */ s32 frame; /* the picked partner's */
    /* 0x0DC */ s32 clut;
    /* 0x0E0 */ s32 clut2;
    /* 0x0E4 */ s32 partners[8]; /* the unlocked partners outside the party, -1 for none */
    /* 0x104 */ s32 count;
} LabPartyScreen;

typedef struct LabPartyScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *labels[5];
    /* 0x18 */ TextWindow *values[5];
    /* 0x2C */ TextWindow *name;
    /* 0x30 */ TextWindow *entriesHint; /* as LabMenuWindows' */
    /* 0x34 */ struct LabEntryList *panel;
} LabPartyScreenWindows;

/* The main menu's second screen (STGDGLAB_createSlotScreen) */
typedef struct LabSlotScreen {
    TASK_HEADER(LabSlotScreen);
    /* 0x050 */ struct Lab *lab;
    /* 0x054 */ PanelAnim panels[6];
    /* 0x0B4 */ s32 layer;
    /* 0x0B8 */ s32 depth;
    /* 0x0BC */ s32 choice;
    /* 0x0C0 */ u8 pad[0xC8 - 0xC0]; /* never read or written */
    /* 0x0C8 */ s32 picked; /* the list's cursor when an entry was picked */
    /* 0x0CC */ s16 slots[3]; /* getPartnerSlots */
    /* 0x0D2 */ s16 entries[45]; /* listPartnerEntries */
    /* 0x12C */ s32 entryCount;
} LabSlotScreen;

typedef struct LabSlotScreenWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *list[13]; /* two columns: 6, then 7 */
    /* 0x38 */ TextWindow *slotNames[3]; /* the slots' Digimon */
    /* 0x44 */ TextWindow *options[2];
    /* 0x4C */ TextWindow *message;
    /* 0x50 */ Cursor *cursor;
    /* 0x54 */ struct LabEntryList *entryList;
    /* 0x58 */ struct LabEntryPanel *entryPanel;
    /* 0x5C */ struct LabSkillPanel *skillsPanel;
} LabSlotScreenWindows;

/* The main menu's third screen (STGDGLAB_createRecipeScreen) */
typedef struct LabRecipeScreen {
    TASK_HEADER(LabRecipeScreen);
    /* 0x050 */ struct Lab *lab;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 pad5C; /* never read or written */
    /* 0x060 */ s16 owned[44];
    /* 0x0B8 */ s32 ownedCount;
    /* 0x0BC */ s32 table; /* into STGDGLAB_data.recipes */
    /* 0x0C0 */ s32 row;
    /* 0x0C4 */ s32 rowCount;
    /* 0x0C8 */ s32 nextRow; /* L1 and R1 move to it */
    /* 0x0CC */ s32 slots; /* how many of found[row] are in use */
    /* 0x0D0 */ s32 found[4][4][5]; /* the owned ids of each recipe */
    /* 0x210 */ s32 complete[4]; /* 0: a recipe of the row lacks ids */
    /* 0x220 */ s32 foundCount[4];
    /* 0x230 */ PanelAnim panels[5];
    /* 0x280 */ s32 arrowLeft;
    /* 0x284 */ s32 arrowRight;
    /* 0x288 */ s32 time;
    /* 0x28C */ s32 clutRow;
    /* 0x290 */ s32 col;
    /* 0x294 */ s32 slot;
    /* 0x298 */ s32 pad298; /* never read or written */
} LabRecipeScreen;

typedef struct LabRecipeScreenWindows {
    /* 0x00 */ TextWindow *title; /* with the partner's name */
    /* 0x04 */ TextWindow *rowNumber;
    /* 0x08 */ TextWindow *rowLabel;
    /* 0x0C */ TextWindow *prev; /* by the arrows */
    /* 0x10 */ TextWindow *next;
    /* 0x14 */ TextWindow *name; /* the picked id's */
    /* 0x18 */ TextWindow *desc;
} LabRecipeScreenWindows;

/* A panel of the second screen (STGDGLAB_createEntryPanel): a partner's entries and
   its three slots */
typedef struct LabEntryPanel {
    TASK_HEADER(LabEntryPanel);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 partner;
    /* 0x5C */ s32 slot; /* the one the picked entry goes in */
    /* 0x60 */ s32 col;
    /* 0x64 */ s32 row;
    /* 0x68 */ s32 scroll;
    /* 0x6C */ s32 blinkTime;
    /* 0x70 */ s32 blink;
    /* 0x74 */ s16 slots[3]; /* getPartnerSlots */
    /* 0x7A */ s16 entries[47]; /* listPartnerEntries */
    /* 0xD8 */ s32 entryCount;
    /* 0xDC */ PanelAnim fades[2];
    /* 0xFC */ s32 option; /* the slot to digivolve to in battle, 3 for none */
} LabEntryPanel;

typedef struct LabEntryPanelWindows {
    /* 0x00 */ TextWindow *entries[10];
    /* 0x28 */ TextWindow *digimonName;
    /* 0x2C */ TextWindow *levelLabel; /* "Skill LV", left of values[13] */
    /* 0x30 */ TextWindow *skills[6];
    /* 0x48 */ TextWindow *values[14]; /* the battle stats, the resistances, the level */
    /* 0x80 */ TextWindow *help;
    /* 0x84 */ TextWindow *options[4];
    /* 0x94 */ Cursor *optionCursor;
    /* 0x98 */ Cursor *cursor;
    /* 0x9C */ ScrollBar *scrollBar; /* USA only */
} LabEntryPanelWindows;

/* A panel of the second screen (STGDGLAB_createSkillPanel) */
typedef struct LabSkillPanel {
    TASK_HEADER(LabSkillPanel);
    /* 0x50 */ PanelAnim fade;
    /* 0x60 */ PanelAnim confirm;
    /* 0x70 */ s32 layer;
    /* 0x74 */ s32 depth;
    /* 0x78 */ s32 member;
    /* 0x7C */ s32 slot;
    /* 0x80 */ s16 slots[4];
    /* 0x88 */ PartnerEntry entry;
    /* 0x9C */ s32 learned;
    /* 0xA0 */ s32 choice;
    /* 0xA4 */ s32 cursor;
    /* 0xA8 */ s32 skillCount;
} LabSkillPanel;

typedef struct LabSkillPanelWindows {
    /* 0x00 */ TextWindow *digimonName;
    /* 0x04 */ TextWindow *learnedLabel;
    /* 0x08 */ TextWindow *learned; /* how many skills are marked */
    /* 0x0C */ TextWindow *left[6];
    /* 0x24 */ TextWindow *right[6];
    /* 0x3C */ TextWindow *message;
    /* 0x40 */ TextWindow *yes;
    /* 0x44 */ TextWindow *no;
    /* 0x48 */ TextWindow *help;
    /* 0x4C */ TextWindow *mpLabel;
    /* 0x50 */ TextWindow *mp; /* the picked skill's */
    /* 0x54 */ Cursor *optionCursor;
    /* 0x58 */ Cursor *cursor;
} LabSkillPanelWindows;

/* A panel of the main menu (STGDGLAB_createEntryList) */
typedef struct LabEntryList {
    TASK_HEADER(LabEntryList);
    /* 0x050 */ s32 allEntries; /* list the other entries after the slots */
    /* 0x054 */ s32 partner;
    /* 0x058 */ s32 layer;
    /* 0x05C */ s32 depth;
    /* 0x060 */ PanelAnim titlePanel; /* their levels follow fade's */
    /* 0x070 */ PanelAnim listPanel;
    /* 0x080 */ PanelAnim fade;
    /* 0x090 */ s32 ids[50]; /* the partner's slots, then (allEntries) its other entries */
    /* 0x158 */ s32 scroll;
    /* 0x15C */ s32 count;
    /* 0x160 */ s32 cursor;
    /* 0x164 */ s32 blinkTime;
    /* 0x168 */ s32 blink; /* the scroll arrows */
    /* 0x16C */ s32 closable; /* triangle closes it */
    /* 0x170 */ void (*close)(struct LabEntryList *panel);
} LabEntryList;

typedef struct LabEntryListWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *options[3];
    /* 0x10 */ TextWindow *digimonName;
    /* 0x14 */ TextWindow *levelLabel; /* "Skill LV", left of values[13] */
    /* 0x18 */ TextWindow *skills[6];
    /* 0x30 */ TextWindow *values[14]; /* the battle stats, the resistances, the level */
    /* 0x68 */ Cursor *cursor;
    /* 0x6C */ ScrollBar *scrollBar;
} LabEntryListWindows;

/* The mode's main task (STGDGLAB_createLab) */
typedef struct Lab {
    TASK_HEADER(Lab);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 bgScroll;
    /* 0x58 */ s32 bgTick;
    /* 0x5C */ s32 partySize; /* the party's slots: PARTY_SIZE */
    /* 0x60 */ s32 partyCount;
    /* 0x64 */ s32 member; /* the party member the screens show */
    /* 0x68 */ s32 (*openMenu)(struct Lab *lab);
    /* 0x6C */ s32 (*closeMenu)(struct Lab *lab);
    /* 0x70 */ s32 (*menuOpen)(struct Lab *lab);
    /* 0x74 */ void (*packParty)(struct Lab *lab);
    /* 0x78 */ void (*fadeOut)(struct Lab *lab);
} Lab;

typedef struct LabChildren {
    /* 0x0 */ LabMenu *menu;
    /* 0x4 */ Task *screen;
    /* 0x8 */ ScreenFade *fade;
} LabChildren;

/* An entry of STGDGLAB_entries: a Digimon of the recipes (GET_DIGIMON) */
typedef struct LabEntry {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 sprite;
    /* 0x4 */ s16 b; /* 0x100-0x135, one per Digimon; only STGDGLAB_getItemB
                        reads it, and nothing in src/ calls that (LabFuncs.getB) */
} LabEntry;

/* The lab's helpers (STGDGLAB_data.funcs) */
typedef struct LabFuncs {
    /* 0x00 */ void (*loadFiles)(void);
    /* 0x04 */ s32 (*filesLoading)(void);
    /* 0x08 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x0C */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x10 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x14 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x18 */ s32 (*getSprite)(s32 id);
    /* 0x1C */ s32 (*getB)(s32 id);
} LabFuncs;

/* The overlay's tables and helpers */
typedef struct LabData {
    /* 0x00 */ PartnerAnim *anim; /* STGDGLAB_partnerAnims, by partner */
    /* 0x04 */ LabWindowPos *layout; /* STGDGLAB_layout */
    /* 0x08 */ LabRecipe *recipes[8]; /* [row * 4 + col] */
    /* 0x28 */ LabFuncs funcs;
} LabData;

/* scene.c */
void STGDGLAB_updateScene(Task *task, Lab **child);
void STGDGLAB_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STGDGLAB_drawFader(ScreenFade *task);
void STGDGLAB_updateFader(ScreenFade *task);
ScreenFade *STGDGLAB_createFader(void);

/* recipe_screen.c */
s32 STGDGLAB_findRecipe(LabRecipeScreen *screen, s32 row, u32 col, s32 slot);
s32 STGDGLAB_hasRecipeId(LabRecipeScreen *screen, s32 row, u32 col);
s32 STGDGLAB_countRowIds(LabRecipeScreen *screen, u32 row);
void STGDGLAB_resetRecipeCursor(LabRecipeScreen *screen);
void STGDGLAB_drawRecipeScreen(LabRecipeScreen *screen, LabRecipeScreenWindows *win);
s32 STGDGLAB_moveRecipeCursor(LabRecipeScreen *screen, s32 step);
void STGDGLAB_updateRecipeScreen(LabRecipeScreen *screen, LabRecipeScreenWindows *win);
Task *STGDGLAB_createRecipeScreen(Lab *lab);

/* entry_panel.c */
void STGDGLAB_createEntryPanelWindows(LabEntryPanel *panel, LabEntryPanelWindows *windows);
void STGDGLAB_showEntryPanel(LabEntryPanel *panel, LabEntryPanelWindows *windows, s32 show);
void STGDGLAB_showEntryPanelOptions(LabEntryPanel *panel, LabEntryPanelWindows *windows, s32 show);
void STGDGLAB_updateEntryPanel(LabEntryPanel *panel, LabEntryPanelWindows *windows);
LabEntryPanel *STGDGLAB_createEntryPanel(s32 partner, s32 slot);

/* slot_screen.c */
void STGDGLAB_createSlotScreenWindows(LabSlotScreen *screen, LabSlotScreenWindows *windows);
void STGDGLAB_showSlotScreen(LabSlotScreen *screen, LabSlotScreenWindows *windows, s32 show);
void STGDGLAB_runSlotScreen(LabSlotScreen *screen, LabSlotScreenWindows *windows);
void STGDGLAB_drawSlotScreen(LabSlotScreen *screen, void *children);
void STGDGLAB_updateSlotScreen(LabSlotScreen *screen, void *children);
Task *STGDGLAB_createSlotScreen(Lab *lab);

/* menu.c */
void STGDGLAB_openMenu(LabMenu *menu);
void STGDGLAB_closeMenu(LabMenu *menu);
void STGDGLAB_showMenuPage(LabMenu *menu, LabMenuWindows *windows);
void STGDGLAB_runMenu(LabMenu *menu, LabMenuWindows *windows);
void STGDGLAB_drawMenu(LabMenu *menu, void *children);
void STGDGLAB_updateMenu(LabMenu *menu, void *children);
LabMenu *STGDGLAB_createMenu(Lab *lab);

/* scroll_bar.c */
void STGDGLAB_setScrollBarX(ScrollBar *bar, s32 x, s32 width);
void STGDGLAB_setScrollBarRange(ScrollBar *bar, s32 top, s32 bottom);
void STGDGLAB_setScrollBarCount(ScrollBar *bar, s32 pageSize, s32 count);
void STGDGLAB_setScrollBarPos(ScrollBar *bar, s32 pos);
void STGDGLAB_updateScrollBar(ScrollBar *bar);
ScrollBar *STGDGLAB_createScrollBar(void);

/* party_screen.c */
void STGDGLAB_showPartyPage(LabPartyScreen *screen, LabPartyScreenWindows *windows);
void STGDGLAB_hideWindows(LabPartyScreen *screen);
void STGDGLAB_drawPartyScreen(LabPartyScreen *screen, void *children);
void STGDGLAB_runPartyScreen(LabPartyScreen *screen, LabPartyScreenWindows *windows);
void STGDGLAB_updatePartyScreen(LabPartyScreen *screen, void *children);
Task *STGDGLAB_createPartyScreen(Lab *lab);

/* skill_panel.c */
void STGDGLAB_createSkillPanelWindows(LabSkillPanel *panel, LabSkillPanelWindows *windows);
void STGDGLAB_showSkillPanel(LabSkillPanel *panel, LabSkillPanelWindows *windows, s32 show);
void STGDGLAB_updateSkillPanel(LabSkillPanel *panel, LabSkillPanelWindows *windows);
LabSkillPanel *STGDGLAB_createSkillPanel(s32 member, s32 slot);

/* entry_list.c */
void STGDGLAB_createEntryListWindows(LabEntryList *panel, LabEntryListWindows *windows);
void STGDGLAB_showEntryList(LabEntryList *panel, LabEntryListWindows *windows, s32 show);
void STGDGLAB_closeEntryList(LabEntryList *panel);
void STGDGLAB_runEntryList(LabEntryList *panel, LabEntryListWindows *windows);
void STGDGLAB_drawEntryList(LabEntryList *panel, LabEntryListWindows *windows);
void STGDGLAB_updateEntryList(LabEntryList *panel, LabEntryListWindows *windows);
LabEntryList *STGDGLAB_createEntryList(s32 partner, s32 allEntries, s32 closable);

/* lab.c */
void STGDGLAB_runLab(Lab *lab, LabChildren *children);
void STGDGLAB_packParty(Lab *lab);
void STGDGLAB_updateLab(Lab *lab, LabChildren *children);
s32 STGDGLAB_openLabMenu(Lab *lab);
s32 STGDGLAB_closeLabMenu(Lab *lab);
s32 STGDGLAB_labMenuRunning(Lab *lab);
void STGDGLAB_fadeOutLab(Lab *lab);
Lab *STGDGLAB_createLab(void);
void STGDGLAB_loadFiles(void);
s32 STGDGLAB_filesLoading(void);
void STGDGLAB_startFade(PanelAnim *fade, s32 fadeIn);
s32 STGDGLAB_updateFade(PanelAnim *fade);
void STGDGLAB_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STGDGLAB_updateLerp(MenuLerp *lerp);
s32 STGDGLAB_getItemSprite(s32 id);
s32 STGDGLAB_getItemB(s32 id);

/* STGDGLAB's data, in its order: the recipe screen's, the menu's, the party
   screen's and the lab's */
extern s32 STGDGLAB_tableItems[]; /* the item each table's screen shows */
extern s32 STGDGLAB_menuStats[]; /* the stats the menu's page shows */
extern s32 STGDGLAB_pageStats[]; /* the stats the first screen's page shows */
extern Task *(*STGDGLAB_screens[])(Lab *lab);
extern PartnerAnim STGDGLAB_partnerAnims[];
extern LabWindowPos STGDGLAB_layout[];
extern LabEntry STGDGLAB_entries[]; /* the items of the recipes */
extern LabRecipe STGDGLAB_recipes0[]; /* to STGDGLAB_recipes7, by table */
extern LabRecipe STGDGLAB_recipes1[];
extern LabRecipe STGDGLAB_recipes2[];
extern LabRecipe STGDGLAB_recipes3[];
extern LabRecipe STGDGLAB_recipes4[];
extern LabRecipe STGDGLAB_recipes5[];
extern LabRecipe STGDGLAB_recipes6[];
extern LabRecipe STGDGLAB_recipes7[];
extern LabData STGDGLAB_data;

#endif /* STGDGLAB_H */
