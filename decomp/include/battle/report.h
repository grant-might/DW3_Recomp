#ifndef STFGTREP_H
#define STFGTREP_H

/* STFGTREP.PRO: mode 0x1400, the report after a battle. WFIGHTMN requests
   it when a battle ends; it goes through the party (getPartyMember) and
   shows the partners that went up a level, with their name and new level,
   from text file 0x56. Its one object is cut, in its link order, into
   src/battle/report's scene.c, partner.c, report.c, fade.c and growth.c, with
   its data in data/stfgtrep.c. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STFGTREP_##name

/* The sprite archive the overlay loads */
#if VERSION_US || VERSION_JP
#define FILE_FGTREP_SPRITES 0x791
#elif VERSION_EU
#define FILE_FGTREP_SPRITES 0x7A0
#endif

/* What a battle gives (STFGTREP_rewards) */
typedef struct BattleReward {
    /* 0x0 */ s32 digimonExp; /* shared by the Digimon used */
    /* 0x4 */ s32 exp; /* shared by the partners who fought */
    /* 0x8 */ s32 money;
} BattleReward;

/* A Digimon a partner can learn, once the conditions are met */
typedef struct Evolution {
    /* 0x0 */ s32 digimon; /* DIGIMON_DATA index + 1 */
    /* 0x4 */ struct {
        s16 digimon; /* 0 for none */
        s16 level;
    } needs[2];
    /* 0xC */ s16 stat; /* 1-6 battle stats, 7 level, 8-14 resistances */
    /* 0xE */ s16 value; /* the least the stat must be */
} Evolution;

struct FightReport;

/* A blinking image of a partner's panel */
typedef struct ReportBlink {
    /* 0x0 */ s32 on;
    /* 0x4 */ s32 frame;
    /* 0x8 */ s32 time;
} ReportBlink;

/* The panel of a partner of the party (STFGTREP_createPartner) */
typedef struct ReportPartner {
    TASK_HEADER(ReportPartner);
    /* 0x050 */ struct FightReport *report;
    /* 0x054 */ s32 layer;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 index; /* in the party */
    /* 0x060 */ s32 exp; /* what the partner gets */
    /* 0x064 */ s32 boosted; /* the exp accessory was applied */
    /* 0x068 */ s32 selected;
    /* 0x06C */ s32 frame; /* of the partner's animation */
    /* 0x070 */ s32 frameTime;
    /* 0x074 */ s16 slots[3];
    /* 0x07A */ s16 entries[44]; /* listPartnerEntries */
    /* 0x0D4 */ PartnerEntry entry;
    /* 0x0E8 */ s32 slot; /* whose Digimon gets its exp, -1 for none */
    /* 0x0EC */ s32 padEC; /* nothing uses it */
    /* 0x0F0 */ ReportBlink levelBlink;
    /* 0x0FC */ struct {
        s32 pad; /* nothing uses it */
        ReportBlink blink;
    } slotBlinks[3];
    /* 0x12C */ s32 learned;
    /* 0x130 */ s32 sound;
    /* 0x134 */ s16 voice; /* -1 for none */
    /* 0x138 */ s32 shownExp;
    /* 0x13C */ s32 targetExp;
    /* 0x140 */ s32 rollTime;
    /* 0x144 */ s32 pad144; /* nothing uses it */
    /* 0x148 */ PanelAnim fade;
    /* 0x158 */ s32 cursorFrame;
    /* 0x15C */ s32 cursorTime;
    /* 0x160 */ void (*show)(struct ReportPartner *partner);
    /* 0x164 */ void (*hide)(struct ReportPartner *partner);
    /* 0x168 */ void (*raise)(struct ReportPartner *partner);
    /* 0x16C */ s32 (*boostExp)(struct ReportPartner *partner);
    /* 0x170 */ void (*select)(struct ReportPartner *partner);
} ReportPartner;

/* The children of a partner's panel */
typedef struct ReportPartnerWindows {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *level;
    /* 0x08 */ TextWindow *levelLabel;
    /* 0x0C */ TextWindow *exp;
    /* 0x10 */ TextWindow *expLabel;
    /* 0x14 */ struct {
        TextWindow *name;
        TextWindow *level;
    } slots[3];
    /* 0x2C */ TextWindow *message;
} ReportPartnerWindows;

/* The main task of the screen (STFGTREP_createScreen) */
typedef struct FightReport {
    TASK_HEADER(FightReport);
    /* 0x50 */ s32 layer;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 bgScroll;
    /* 0x5C */ s32 bgSkip; /* the background moves every other frame */
    /* 0x60 */ s32 count; /* partners in the party */
    /* 0x64 */ s32 exp[3]; /* what each one gets */
    /* 0x70 */ s32 used; /* Digimon used in the battle */
    /* 0x74 */ s32 arrowOn; /* the message waits for a button */
    /* 0x78 */ s32 arrowTime;
    /* 0x7C */ s32 arrowFrame;
    /* 0x80 */ PanelAnim header;
    /* 0x90 */ PanelAnim footer;
} FightReport;

/* The children of the main task */
typedef struct FightReportChildren {
    /* 0x00 */ TextWindow *exp;
    /* 0x04 */ TextWindow *expLabel;
    /* 0x08 */ TextWindow *message;
    /* 0x0C */ ReportPartner *partners[3];
    /* 0x18 */ ScreenFade *fade;
} FightReportChildren;

/* The overlay's helpers (STFGTREP_funcs) */
typedef struct FightReportFuncs {
    /* 0x00 */ void (*loadFiles)(void);
    /* 0x04 */ s32 (*filesLoading)(void);
    /* 0x08 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x0C */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x10 */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x14 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x18 */ s32 (*addExp)(s32 partner, s32 exp);
    /* 0x1C */ s32 (*learnDigimon)(s32 partner);
    /* 0x20 */ s32 (*addDigimonExp)(s32 partner, s32 id, s32 exp);
    /* 0x24 */ s32 (*addSkill)(s32 partner, s32 id);
    /* 0x28 */ s32 (*learnSkill)(s32 partner, s32 id);
    /* 0x2C */ s32 (*getDigimonExp)(s32 partner, s32 id, s32 exp, s32 used);
} FightReportFuncs;


/* What a level adds to the exp the next one needs, by tier */
extern s32 STFGTREP_levelExp[4];
/* What is taken from the HP and MP growth, by tier, and added at random */
extern s32 STFGTREP_vitalCuts[4];
extern s32 STFGTREP_vitalRandom[9];
/* A battle stat's gain by tier, from its growth plus 0-4 at random */
extern s32 STFGTREP_statGains[6][9];
/* A resistance's gain, from its growth plus 0-3 at random */
extern s32 STFGTREP_resistGains[8];

extern BattleReward STFGTREP_rewards[];
extern FightReportFuncs STFGTREP_funcs;
extern Evolution *STFGTREP_evolutions[8]; /* each partner's (STFGTREP_learnDigimon) */

extern s32 STFGTREP_animations[][7]; /* each partner Digimon's sprite frames, -1 ending them early */

/* scene.c */
void STFGTREP_updateScene(Task *task, FightReport **children);
void STFGTREP_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STFGTREP_drawFader(ScreenFade *task);
void STFGTREP_updateFader(ScreenFade *task);
ScreenFade *STFGTREP_createFader(void);

/* partner.c */
void STFGTREP_createPartnerWindows(ReportPartner *partner, ReportPartnerWindows *windows);
void STFGTREP_fillPartnerWindows(ReportPartner *partner, ReportPartnerWindows *windows, s32 show);
void STFGTREP_drawPartner(ReportPartner *partner);
s32 STFGTREP_rollExp(ReportPartner *partner);
void STFGTREP_runPartner(ReportPartner *partner, ReportPartnerWindows *windows);
void STFGTREP_updatePartner(ReportPartner *partner, ReportPartnerWindows *windows);
void STFGTREP_showPartner(ReportPartner *partner);
void STFGTREP_hidePartner(ReportPartner *partner);
void STFGTREP_raisePartner(ReportPartner *partner);
s32 STFGTREP_boostExp(ReportPartner *partner);
void STFGTREP_selectPartner(ReportPartner *partner);
ReportPartner *STFGTREP_createPartner(FightReport *report, s32 index, s32 exp);

/* report.c */
void STFGTREP_createWindows(FightReport *report, FightReportChildren *children);
void STFGTREP_drawReport(FightReport *report);
void STFGTREP_runReport(FightReport *report, FightReportChildren *children);
void STFGTREP_updateReport(FightReport *report, FightReportChildren *children);
FightReport *STFGTREP_createScreen(void);
void STFGTREP_loadFiles(void);
s32 STFGTREP_filesLoading(void);

/* fade.c */
void STFGTREP_startFade(PanelAnim *fade, s32 fadeIn);
s32 STFGTREP_updateFade(PanelAnim *fade);
void STFGTREP_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STFGTREP_updateLerp(MenuLerp *lerp);

/* growth.c */
void STFGTREP_raiseStats(s32 partner, s32 level);
s32 STFGTREP_addExp(s32 partner, s32 exp);
s32 STFGTREP_learnDigimon(s32 partner);
s32 STFGTREP_addDigimonExp(s32 partner, s32 id, s32 exp);
s32 STFGTREP_addSkill(s32 partner, s32 id);
s32 STFGTREP_learnSkill(s32 partner, s32 id);
s32 STFGTREP_getDigimonExp(s32 partner, s32 id, s32 exp, s32 used);

#endif /* STFGTREP_H */
