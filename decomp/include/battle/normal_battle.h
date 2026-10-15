#ifndef WFIGHTMN_H
#define WFIGHTMN_H

/* WFIGHTMN.PRO: the battle's sub-overlay. FIGHTSTG loads it (file 0x1FA)
   at STAGE_VRAM for a normal battle, and WFIGHTTS in its place for the
   battle test. */

#include "battle/battle_mode.h"

/* The item a partner can equip that makes FIGHTSTG's FIGHTSTG_queueRecovery act on
   it at the start of the battle */
#define WFIGHTMN_RECOVERY_ITEM 0x140

/* The battle menu (WFIGHTMN_start), registered as BATTLE_TASK_MENU */
typedef struct BattleMenu {
    TASK_HEADER(BattleMenu);
    /* 0x50 */ s32 args[8]; /* the message's arguments */
    /* 0x70 */ s32 confusionSound;
} BattleMenu;

/* The task that loads the battle menu's files (WFIGHTMN_createLoader) */
typedef struct BattleLoader {
    TASK_HEADER(BattleLoader);
    /* 0x50 */ s32 pad; /* nothing uses it */
} BattleLoader;

/* The battle menu's children */
typedef struct BattleMenuChildren {
    /* 0x00 */ BattleLoader *loader;
    /* 0x04 */ PlayerTurn *commands; /* the player's turn (FIGHTSTG_startPlayerTurn) */
    /* 0x08 */ BattleCamera *camera; /* FIGHTSTG_createBattleCamera */
    /* 0x0C */ Lights *lights;
    /* 0x10 */ FightStage *stage;
    /* 0x14 */ Models *models;
    /* 0x18 */ BattleChild cameraMove; /* the European version's shots, or the turn when the player loses */
    /* 0x1C */ BattleChild task; /* what the state waits for, NULL when done */
} BattleMenuChildren;

/* The screen its layers cover */
extern RECT WFIGHTMN_screen;
/* A technique's look (TechData's script fields) by its effect, for
   WFIGHTMN_bringLastEnemy */
typedef struct EffectLook {
    /* 0x0 */ s32 effect; /* TechData.effect, -1 ends the list */
    /* 0x4 */ s32 scriptEffect;
    /* 0x8 */ s32 scriptSound;
} EffectLook;
/* A technique's look by its element, for WFIGHTMN_bringLastEnemy */
typedef struct ElementLook {
    /* 0x0 */ s32 element; /* TechData.element */
    /* 0x4 */ s32 scriptEffect;
    /* 0x8 */ s32 scriptSound;
    /* 0xC */ s32 scriptStage;
} ElementLook;
/* A battle script's look (BattleScript.effect and sound) */
typedef struct ScriptLook {
    /* 0x0 */ s32 effect;
    /* 0x4 */ s32 sound;
} ScriptLook;
extern EffectLook WFIGHTMN_effectVisuals[];
extern ElementLook WFIGHTMN_elementVisuals[];
/* WFIGHTMN_startTech's effects */
extern ScriptLook WFIGHTMN_actionEffects[];
/* The battle menu's states, by its substate */
extern void (*WFIGHTMN_states[])(BattleMenu *task, BattleMenuChildren *children);

void WFIGHTMN_loadFiles(BattleLoader *task);
BattleLoader *WFIGHTMN_createLoader(void);

/* setup.c */
void WFIGHTMN_createLayers(void);
void WFIGHTMN_initFighters(s32 digimon);
s32 WFIGHTMN_rollAmbush(void);
void WFIGHTMN_checkParty(void);
void WFIGHTMN_markFought(void);
void WFIGHTMN_markPicked(BattleMenu *task, BattleMenuChildren *children);

/* menu.c */
void WFIGHTMN_updateMenu(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_cancelBlast(void);
void WFIGHTMN_runTurn(BattleMenu *task);
/* The states in WFIGHTMN_states */
void WFIGHTMN_runCommand(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_digivolve(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_tag(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_pairTech(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endBattle(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_runAway(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endAutoRecover(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_recover(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_clearField(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_takeDamage(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_cureStatus(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endBoost(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_runConfusedCommand(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endRestriction(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_blast(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endBlast(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_knockOut(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_endSpecial(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_digidevolve(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_showWon(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_bringLastEnemy(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_restoreEnemy(BattleMenu *task, BattleMenuChildren *children);
void WFIGHTMN_recordTech(u8 side, s32 id);

#endif /* WFIGHTMN_H */
