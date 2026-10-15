#ifndef DW3_OVERLAY_H
#define DW3_OVERLAY_H

/* The game modes' overlays and the task that runs them (system/overlay.c) */

#include "common.h"
#include "engine/task.h"

/* Loads the overlay of the current mode (OVERLAY_LOADER) */
typedef struct OverlayLoader {
    /* 0x0 */ s32 mode; /* whose overlay is loaded */
    /* 0x4 */ s32 subOverlay;
    /* 0x8 */ void (*loadModeOverlay)(void);
    /* 0xC */ void (*loadSubOverlay)(s32 file);
} OverlayLoader;

/* The mode overlays' files (MODE_OVERLAY_FILES), by their disc names: each
   one's sector count in FILE_SECTOR_COUNTS is its .PRO file's. The Japanese
   disc numbers them as the USA one does */
#if VERSION_US || VERSION_JP
#define FILE_CARDGAME 0x153
#define FILE_STDGNAME 0x154
#define FILE_CNTY_SEL 0x157
#define FILE_FIELDSTG 0x158
#define FILE_FIGHTSTG 0x159
#define FILE_SHOCKTST 0x15A
#define FILE_SOUNDTST 0x15B
#define FILE_STAGSLCT 0x176
#define FILE_STCRDABM 0x177
#define FILE_STCRDDEK 0x1BC
#define FILE_STCRDSHP 0x1C0
#define FILE_STDWTITL 0x1C1
#define FILE_STFGTREP 0x1C2
#define FILE_STGDGLAB 0x1DF
#define FILE_STGMCARD 0x1E3
#define FILE_STGTRAIN 0x1F6
#define FILE_STITSHOP 0x1F7
#define FILE_STPLNMET 0x1F8
#define FILE_STSTATUS 0x1F9
#elif VERSION_EU
#define FILE_CARDGAME 0x161
#define FILE_STDGNAME 0x162
#define FILE_CNTY_SEL 0x165
#define FILE_FIELDSTG 0x166
#define FILE_FIGHTSTG 0x167
#define FILE_SHOCKTST 0x168
#define FILE_SOUNDTST 0x169
#define FILE_STAGSLCT 0x184
#define FILE_STCRDABM 0x185
#define FILE_STCRDDEK 0x1CA
#define FILE_STCRDSHP 0x1CE
#define FILE_STDWTITL 0x1D0
#define FILE_STFGTREP 0x1D1
#define FILE_STGDGLAB 0x1EC
#define FILE_STGMCARD 0x1EE
#define FILE_STGTRAIN 0x1F2
#define FILE_STITSHOP 0x205
#define FILE_STPLNMET 0x206
#define FILE_STSTATUS 0x207
#endif

void updateModeTask(Task *task, Task **children);
void *createModeTask(void);
void loadModeOverlay(void);
void loadSubOverlay(s32 id);

extern OverlayLoader OVERLAY_LOADER;
extern Task *(*MODE_ENTRY_POINTS[])(void);
extern s32 MODE_OVERLAY_FILES[];

/* The mode overlays' entry points, which MODE_ENTRY_POINTS holds: declared
   here, where the executable and every overlay see them, as the overlays'
   headers can't be included together */
Task *FIELDSTG_start(void);
Task *STCRDDEK_start(void);
Task *STPLNMET_start(void);
Task *FIGHTSTG_start(void);
Task *CARDGAME_start(void);
Task *SOUNDTST_start(void);
Task *SHOCKTST_start(void);
Task *STGTRAIN_start(void);
Task *STDGNAME_start(void);
Task *STGMCARD_start(void);
Task *STGDGLAB_createScene(void);
Task *STDWTITL_start(void);
Task *STITSHOP_start(void);
Task *STSTATUS_start(void);
Task *STCRDABM_start(void);
Task *STCRDSHP_start(void);
Task *STFGTREP_start(void);
Task *STAGSLCT_start(void);
Task *CNTY_SEL_start(void);

/* the memory map of mk/version/<version>.mk, which the Makefile gives the
   linker: where the overlays and the stages load */
extern u8 OVERLAY_VRAM[];
extern u8 STAGE_VRAM[];
/* the two areas the loader copies to, at those addresses (system/main.c) */
extern void *const OVERLAY_ADDRESS;
extern void *const SUB_OVERLAY_ADDRESS;

#endif /* DW3_OVERLAY_H */
