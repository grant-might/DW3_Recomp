#ifndef WFIGHTTS_H
#define WFIGHTTS_H

/* WFIGHTTS.PRO: the battle test, a debug menu. FIGHTSTG loads it (file 0x1FB)
   in place of WFIGHTMN when the battle mode's argument is set. It picks
   the fighters, their motions (ウェイト, ダメージ, ガード...), the effects
   (ＭＥＦＴ00xx) and the fight stage (ＭＦＳＴＧ0xx) from lists. */

#include "battle/battle_mode.h"

/* The fighters' and the cameras' lists (WFIGHTTS_fighterList, WFIGHTTS_cameraList) */
typedef struct BattleTestList {
    TASK_HEADER(BattleTestList);
    /* 0x50 */ s32 *side; /* 0 the partner, 1 the enemy, -1 for none */
    /* 0x54 */ s32 *pick; /* set to 0 when it starts */
} BattleTestList;

/* The stage list's task */
typedef struct BattleTestStageList {
    TASK_HEADER(BattleTestStageList);
    /* 0x50 */ s32 *result; /* the stage picked, 1-55, or -1 */
} BattleTestStageList;

/* A fighter's motions, in the motion list (WFIGHTTS_motionList) */
typedef struct BattleTestMotionList {
    /* 0x00 */ s32 motions[0x3E]; /* the ones the fighter has */
    /* 0xF8 */ s32 count;
    /* 0xFC */ s32 shown; /* up to 14 */
} BattleTestMotionList;

/* A fighter's effects, in the effect list (WFIGHTTS_effectList): each one
   plus 1, or just a 0 for a fighter without any */
typedef struct BattleTestEffectList {
    /* 0x00 */ s32 effects[0x13];
    /* 0x4C */ s32 count;
    /* 0x50 */ s32 shown; /* up to 14 */
} BattleTestEffectList;

/* Where the motion and the effect lists are: the fighters they were made
   for, the list LEFT and RIGHT pick and the lists' cursors and scrolls */
typedef struct BattleTestCursors {
    /* 0x00 */ s32 fighter[2];
    /* 0x08 */ s32 side;
    /* 0x0C */ s32 cursor[2];
    /* 0x14 */ s32 scroll[2];
} BattleTestCursors;

/* The fighter, motion and effect lists' windows, by list */
typedef struct BattleTestWindows {
    /* 0x00 */ TextWindow *windows[2][14];
} BattleTestWindows;

/* The camera list's windows: the partners' cameras and the enemies' */
typedef struct CameraListWindows {
    /* 0x00 */ TextWindow *partner[12];
    /* 0x30 */ TextWindow *enemy[3];
} CameraListWindows;

/* The motion list's task */
typedef struct BattleTestMotions {
    TASK_HEADER(BattleTestMotions);
    /* 0x050 */ BattleTestMotionList lists[2]; /* the partner's, the enemy's */
    /* 0x250 */ s32 *side;
    /* 0x254 */ s32 *motion; /* the motion picked */
} BattleTestMotions;

/* The effect list's task */
typedef struct BattleTestEffects {
    TASK_HEADER(BattleTestEffects);
    /* 0x50 */ BattleTestEffectList lists[2]; /* the partner's, the enemy's */
    /* 0xF8 */ s32 *side;
    /* 0xFC */ s32 *effect; /* the effect picked */
} BattleTestEffects;

/* The battle test (WFIGHTTS_battleTest). Its pages (substate): 0 the battle,
   1 the fighters, 2 the camera, 3 the stage, 4 the motions, 5 the effects;
   L1 and R1 go from one list to the next */
typedef struct BattleTest {
    TASK_HEADER(BattleTest);
    /* 0x50 */ s32 page; /* the last list, which CROSS goes back to */
    /* 0x54 */ s32 side; /* 0 the partner, 1 the enemy, -1 for none */
    /* 0x58 */ s32 fighter;
    /* 0x5C */ s32 camera; /* by the fighter's motion */
    /* 0x60 */ s32 stage; /* 1-55, or -1 */
    /* 0x64 */ s32 motion;
    /* 0x68 */ s32 effect;
} BattleTest;

/* The battle test's children */
typedef struct BattleTestChildren {
    /* 0x00 */ BattleChild task; /* what plays, NULL when done */
    /* 0x04 */ PlayerTurn *commands; /* the player's turn (FIGHTSTG_startPlayerTurn) */
    /* 0x08 */ BattleCamera *camera;
    /* 0x0C */ Lights *lights;
    /* 0x10 */ FightStage *stage;
    /* 0x14 */ BattleTestList *fighters;
    /* 0x18 */ BattleTestList *cameras;
    /* 0x1C */ BattleTestStageList *stages;
    /* 0x20 */ BattleTestMotions *motions;
    /* 0x24 */ BattleTestEffects *effects;
    /* 0x28 */ Models *models;
} BattleTestChildren;

/* The battle test's data, defined after its code */
extern RECT WFIGHTTS_screen;
extern s32 WFIGHTTS_stageCursor;
extern s32 WFIGHTTS_stageScroll;
extern char *WFIGHTTS_stageNames[];
extern s16 WFIGHTTS_fighters[][2];
extern DVECTOR WFIGHTTS_backPoints[4];
extern CVECTOR WFIGHTTS_backColors[4];
extern s32 WFIGHTTS_speedMode;
extern s32 WFIGHTTS_fighterSide;
extern s32 WFIGHTTS_fighterCursors[2];
extern s32 WFIGHTTS_fighterScrolls[2];
extern s32 WFIGHTTS_cameraSide;
extern s32 WFIGHTTS_cameraPicks[2];
extern char *WFIGHTTS_partnerCameraNames[];
extern char *WFIGHTTS_enemyCameraNames[];
extern CameraView WFIGHTTS_partnerView;
extern CameraView WFIGHTTS_enemyView;
extern s32 WFIGHTTS_displayX;
extern s32 WFIGHTTS_displayY;
extern BattleTestCursors WFIGHTTS_motionCursors;
extern BattleTestCursors WFIGHTTS_effectCursors;
extern char *WFIGHTTS_motionNames[];
extern char *WFIGHTTS_effectNames[];

void WFIGHTTS_initLayers(void);
void WFIGHTTS_loadImages(void);
void WFIGHTTS_battleTest(BattleTest *task, BattleTestChildren *children);
BattleTestList *WFIGHTTS_createFighterList(s32 *side, s32 *pick);
BattleTestList *WFIGHTTS_createCameraList(s32 *side, s32 *pick);
BattleTestStageList *WFIGHTTS_createStageList(s32 *result);
BattleTestMotions *WFIGHTTS_createMotionList(s32 *side, s32 *motion);
BattleTestEffects *WFIGHTTS_createEffectList(s32 *side, s32 *effect);
void WFIGHTTS_fighterList(BattleTestList *task, BattleTestWindows *windows);
void WFIGHTTS_cameraList(BattleTestList *task, CameraListWindows *windows);
void WFIGHTTS_stageList(BattleTestStageList *task, TextWindow **windows);
void WFIGHTTS_motionList(BattleTestMotions *task, BattleTestWindows *windows);
void WFIGHTTS_effectList(BattleTestEffects *task, BattleTestWindows *windows);

#endif /* WFIGHTTS_H */
