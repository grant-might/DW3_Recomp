#ifndef FIGHTSTG_ACTION_H
#define FIGHTSTG_ACTION_H

/* FIGHTSTG's actions: techniques, enemy attacks, counterattacks, items, the
   first technique, action events and the turn at 1 HP (tech_action.c,
   enemy_attack.c, counterattack.c, item.c, first_tech.c, action_events.c
   and one_hp_turn.c). */

#include "battle/battle_mode/types.h"

/* FIGHTSTG_updateFirstTech's task (FIGHTSTG_startFirstTech): a side's first technique */
typedef struct FirstTech {
    TASK_HEADER(FirstTech);
    /* 0x50 */ u8 side; /* the side that uses it, 0 or 0x10 */
    /* 0x54 */ s32 tech;
    /* 0x58 */ s32 damage;
    /* 0x5C */ s32 asleep; /* whether the other side's fighter was asleep */
    /* 0x60 */ s32 lines[8]; /* FIGHTSTG_updateMessage's */
} FirstTech;

/* FIGHTSTG_updateItem's task: an item used in battle, FIGHTSTG_runItemScript its script */
typedef struct BattleItem {
    TASK_HEADER(BattleItem);
    /* 0x50 */ s32 lines[8]; /* FIGHTSTG_updateMessage's, 0 ends them */
    /* 0x70 */ s32 item; /* the item */
    /* 0x74 */ s32 element; /* item 0x54's element, 2-8 */
    /* 0x78 */ s32 damage; /* the damage */
    /* 0x7C */ s32 lowered; /* item 0x58's: 1 it lowered the enemy's first stat, 2 its second */
} BattleItem;

/* An item's script settings (FIGHTSTG_itemScripts, ended by -1) */
typedef struct ItemScript {
    /* 0x0 */ s16 item;
    /* 0x2 */ s16 effect; /* BattleScript's */
    /* 0x4 */ s16 sound;
} ItemScript;

/* A technique's boost (FIGHTSTG_techBoosts and FIGHTSTG_sideBoosts, each
   ended by -1) */
typedef struct TechBoost {
    /* 0x0 */ s16 tech;
    /* 0x2 */ s16 amount; /* times the technique's effectPower, below 0 for the other side */
    /* 0x4 */ s16 stat;
    /* 0x6 */ s16 line; /* the message */
} TechBoost;

extern TechBoost FIGHTSTG_techBoosts[];
extern TechBoost FIGHTSTG_sideBoosts[];

/* What an item cures (FIGHTSTG_itemCures, items 0x42-0x45) */
typedef struct StatusCure {
    /* 0x0 */ s16 message;
    /* 0x2 */ s16 flag; /* in BattleFighter.flags */
    /* 0x4 */ s32 item; /* FIGHTSTG_events.funcs.cureStatus's */
} StatusCure;

extern StatusCure FIGHTSTG_itemCures[4];
extern ItemScript FIGHTSTG_itemScripts[];
extern u8 FIGHTSTG_statusTechFlags[];
extern s32 FIGHTSTG_statusTechArgs[];
extern s32 FIGHTSTG_statusTechLines[];
#if VERSION_EU || VERSION_JP
extern s32 FIGHTSTG_clearIds[]; /* the six event types FIGHTSTG_reviveFighter clears */
void FIGHTSTG_reviveFighter(s32 tech, s32 fighter);
#endif

typedef struct TechAction {
    TASK_HEADER(TechAction);
    /* 0x50 */ u8 side; /* the side that uses it, 0 or 0x10 */
    /* 0x54 */ s32 tech; /* the technique or item */
    /* 0x58 */ s32 damage;
    /* 0x5C */ s32 heal; /* the HP an item heals */
    /* 0x60 */ s32 asleep; /* whether the other side's fighter was asleep */
    /* 0x64 */ s32 lines[8]; /* FIGHTSTG_updateMessage's */
} TechAction;

/* FIGHTSTG_updateEnemyAttack's task (FIGHTSTG_startEnemyAttack): an attack
   on the player's active fighter, as state kind + 1 */
typedef struct EnemyAttack {
    TASK_HEADER(EnemyAttack);
    /* 0x50 */ s32 lines[4]; /* FIGHTSTG_updateMessage's */
    /* 0x60 */ s32 resetIdleMotion; /* the weakness ended: WFIGHTMN_setIdleMotion once the technique starts */
    /* 0x64 */ u8 pad[0xC]; /* nothing uses it */
    /* 0x70 */ s32 tech; /* the technique */
    /* 0x74 */ s32 kind; /* 0 the enemy's technique, 1 another attack */
    /* 0x78 */ s32 noKnockout; /* don't knock the fighter out at 0 HP */
    /* 0x7C */ s32 asleep; /* the fighter was asleep */
} EnemyAttack;

/* FIGHTSTG_updateActionEvents's task (FIGHTSTG_startActionEvents) */
typedef struct ActionEvents {
    TASK_HEADER(ActionEvents);
    /* 0x50 */ u8 side; /* 0 or 0x10 */
    /* 0x54 */ s32 args[3];
    /* 0x60 */ u8 pad[0x14]; /* nothing uses it */
} ActionEvents;

/* A counterattack (FIGHTSTG_startCounterattack) */
typedef struct Counterattack {
    TASK_HEADER(Counterattack);
    /* 0x50 */ s32 lines[8]; /* FIGHTSTG_updateMessage's */
    /* 0x70 */ u8 side; /* the side that counters, 0 or 0x10 */
    /* 0x74 */ s32 received; /* the damage it answers, 0 for none */
    /* 0x78 */ s32 tech;
    /* 0x7C */ s32 hit; /* whether the technique hits (FIGHTSTG_battleFuncs.rollHit) */
    /* 0x80 */ s32 damage;
    /* 0x84 */ s32 noKnockOutEvent; /* a knockout doesn't call FIGHTSTG_queueKnockOut */
    /* 0x88 */ s32 pad; /* nothing uses it */
} Counterattack;

/* FIGHTSTG_startOneHpTurn's task, BATTLE_KIND_FINAL_SECOND's enemy turn:
   the enemy's second technique (WFIGHTMN_startTech), which leaves the
   player's fighter at 1 HP, each with its message, then the battle goes on
   (FIGHTSTG_queueEnemyTurn and FIGHTSTG_queueLastEnemy) */
typedef struct OneHpTurn {
    TASK_HEADER(OneHpTurn);
    /* 0x50 */ s32 lines[8]; /* FIGHTSTG_updateMessage's, 0 ends them */
} OneHpTurn;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
OneHpTurn *FIGHTSTG_startOneHpTurn(); /* (void): unprototyped, as WFIGHTMN passes it &FIGHTSTG_battle */
FirstTech *FIGHTSTG_startFirstTech(s32 side);
BattleItem *FIGHTSTG_startItem(s32 item);
TechAction *FIGHTSTG_startTechAction(s32 side, s32 tech);
EnemyAttack *FIGHTSTG_startEnemyAttack(s32 kind, s32 noKnockout);

/* Shared between the overlay's objects */
void FIGHTSTG_updateOneHpTurn(OneHpTurn *task, BattleChild *children);
Counterattack *FIGHTSTG_startCounterattack(s32 side, s32 received, s32 noKnockOutEvent);
ActionEvents *FIGHTSTG_startActionEvents(s32 side);
void FIGHTSTG_updateTechAction(TechAction *task, BattleChild *children);
void FIGHTSTG_updateEnemyAttack(EnemyAttack *task, BattleChild *children);
void FIGHTSTG_updateFirstTech(FirstTech *task, BattleChild *children);
void FIGHTSTG_updateItem(BattleItem *task, BattleChild *children);
s32 FIGHTSTG_runItemScript(BattleItem *task, BattleChild *children);
void FIGHTSTG_updateActionEvents(ActionEvents *task, BattleChild *children);
void FIGHTSTG_updateCounterattack(Counterattack *task, BattleChild *children);

#endif /* FIGHTSTG_ACTION_H */
