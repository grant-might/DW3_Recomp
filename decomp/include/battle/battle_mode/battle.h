#ifndef FIGHTSTG_BATTLE_H
#define FIGHTSTG_BATTLE_H

/* FIGHTSTG's battle state: the battle table, stats, elements and
   resistances, the fighters, the events queue, the action and its effects,
   the rolls and the damage (battle.c, battle_table.c, stats.c, damage.c,
   rolls.c, boosts.c, events.c, speed.c, tech_effects.c, enemy_turn.c and
   start.c). */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/model.h"

/* A table of 0x46-byte entries that start with an s16 id, 0 after the
   last (FIGHTSTG_findBattleTableIndex) */
#if VERSION_US || VERSION_JP
#define FILE_BATTLE_TABLE 0xBE
#elif VERSION_EU
#define FILE_BATTLE_TABLE 0x1CF
#endif

/* What an enemy does: its target (FIGHTSTG_getEnemyAction) when the condition holds */
typedef struct BattleTableAction {
    /* 0x0 */ u8 target;
    /* 0x1 */ u8 condition;
    /* 0x2 */ s16 conditionArg;
} BattleTableAction;

/* BattleStats.stats' indices: the partner's first five battle stats */
#define BATTLE_STAT_ATTACK 0 /* physical damage */
#define BATTLE_STAT_DEFENSE 1 /* against it */
#define BATTLE_STAT_SPIRIT 2 /* magic damage, and against it */
#define BATTLE_STAT_WISDOM 3 /* magic accuracy, statuses and heals */
#define BATTLE_STAT_SPEED 4 /* physical accuracy, evasion and running away */

/* The elements (TechData.element, BattleStats.element) and the families
   (TechData.family, BattleStats.family) start at 2: under it, none */
#define ELEMENT_FIRST 2
#define ELEMENT_COUNT 7
#define FAMILY_FIRST 2

/* TechData.icon's kinds of attack */
#define TECH_PHYSICAL 2 /* FIGHTSTG_rollHit and FIGHTSTG_computeDamage */
#define TECH_MAGIC 3 /* FIGHTSTG_rollMagicHit and FIGHTSTG_computeMagicDamage */

/* BattleStats.resist's indices: against the elements (element -
   ELEMENT_FIRST), then against the statuses */
#define RESIST_POISON 7
#define RESIST_PARALYSIS 8
#define RESIST_CONFUSION 9
#define RESIST_SLEEP 10
#define RESIST_KNOCK_OUT 11
#define RESIST_COUNT 12

/* BattleTableEntry.actions' last one, which the enemy takes when none of
   the others' conditions holds: its own is never tested */
#define BATTLE_TABLE_FALLBACK 3

typedef struct BattleTableEntry {
    /* 0x00 */ s16 id;
    /* 0x02 */ s16 item; /* what the enemy may leave */
    /* 0x04 */ s16 itemChance; /* in 1024ths, less one */
    /* 0x06 */ s16 nameId; /* string in file 0x4F */
    /* 0x08 */ s16 techs[3]; /* its first, then those of FIGHTSTG_getEnemyAction's kinds 2 and 3 */
    /* 0x0E */ s16 stats[5]; /* by BATTLE_STAT_*, scaled by the enemy's strength / 16 */
    /* 0x18 */ s16 resist[RESIST_COUNT];
    /* 0x30 */ u8 family; /* FAMILY_*, the enemy's stats' */
    /* 0x32 */ BattleTableAction actions[BATTLE_TABLE_FALLBACK + 1]; /* the first whose condition holds (FIGHTSTG_testEnemyCondition) */
    /* 0x42 */ BattleTableAction counter; /* its counterattack (FIGHTSTG_updateCounterattack) */
} BattleTableEntry;

/* FIGHTSTG_startEnemyTurn's task */
typedef struct EnemyTurn {
    TASK_HEADER(EnemyTurn);
    /* 0x50 */ s32 lines[10]; /* FIGHTSTG_updateMessage's, 0 ends them */
    /* 0x78 */ s32 target; /* -2 to -4 for an enemy, else any but the active one */
    /* 0x7C */ s32 switchTo; /* the enemy fighter it brings in (FIGHTSTG_pickEnemySwitch) */
} EnemyTurn;

/* The types of the battle's events: WFIGHTMN runs a state for each
   (WFIGHTMN_runTurn) */
#define EVENT_END_BATTLE 1 /* FIGHTSTG_endBattle */
#define EVENT_PLAYER_TURN 2
#define EVENT_ENEMY_TURN 3
#define EVENT_RUN_AWAY 4
#define EVENT_AUTO_RECOVER_END 5
#define EVENT_RECOVERY 6
#define EVENT_CLEAR_FIELD 7
#define EVENT_PARTNER_TECH 8 /* a technique of the partner's, run as its command */
#define EVENT_STATUS_DAMAGE 9 /* the damage of a poisoned fighter (FIGHTER_POISONED) */
#define EVENT_STATUS_END 10 /* 10-12: the end of FIGHTER_PARALYZED, FIGHTER_CONFUSED or FIGHTER_ASLEEP */
#define EVENT_BOOST_END 13 /* 13-15: the end of a boost, by the stat */
#define EVENT_RESTRICTION_END 16 /* 16-17: the end of a restriction (FIGHTER_NO_SWITCH or FIGHTER_NO_DIGIVOLVE) */
#define EVENT_BLAST 18 /* the partner's digivolution for the battle */
#define EVENT_BLAST_END 19
#define EVENT_KNOCK_OUT 20
#define EVENT_SPECIAL_END 21 /* the end of the partner's special state */
#define EVENT_DIGIDEVOLVE 22
#define EVENT_LAST_ENEMY 23 /* the last battle's third enemy comes in */
#define EVENT_WEAKNESS_END 24 /* the end of the enemy's weakness (BATTLE_KIND_FINAL_LAST) */

/* An event of the battle, as it is queued (FIGHTSTG_pushEvent) */
typedef struct BattleEvent {
    /* 0x00 */ s32 type; /* EVENT_END_BATTLE... */
    /* 0x04 */ s32 delay;
    /* 0x08 */ s32 args[6];
} BattleEvent;

/* A queued event: its time counts down to when it runs */
typedef struct QueuedEvent {
    /* 0x00 */ s16 type; /* 0 for a free entry */
    /* 0x02 */ s16 time;
    /* 0x04 */ s32 args[6];
} QueuedEvent;

/* What FIGHTSTG_removeEvents removes the events of: an event's first two
   args, as FIGHTSTG_queueRecovery and the others push them */
typedef struct EventKey {
    /* 0x0 */ u8 side; /* SIDE_PLAYER or SIDE_ENEMY */
    /* 0x4 */ s32 fighter; /* its index in the side's fighters */
} EventKey;

/* The kinds of battle (Battle.kind), which WFIGHTMN picks by the battle and
   the mode it came from */
#define BATTLE_KIND_NORMAL 0
#define BATTLE_KIND_ESCAPE 1 /* the enemy runs away under a tenth of its HP */
#define BATTLE_KIND_UNK2 2 /* the enemy keeps an eleventh of its HP */
#define BATTLE_KIND_NO_DAMAGE 3 /* the partner's hits do no damage */
#define BATTLE_KIND_FINAL 4 /* battle 0x144, the last one: its first enemy */
#define BATTLE_KIND_FINAL_SECOND 5 /* its second enemy */
#define BATTLE_KIND_FINAL_LAST 6 /* its third enemy; winning plays the ending */

/* How a battle ends (FIGHTSTG_endBattle) */
#define BATTLE_FLED 0 /* a side ran away */
#define BATTLE_WON 1
#define BATTLE_LOST 2

/* The event queue's functions (FIGHTSTG_events.funcs) */
typedef struct EventQueueFuncs {
    /* 0x00 */ u8 result; /* the battle's, for WFIGHTMN (BATTLE_FLED...) */
    /* 0x01 */ u8 statusStrength; /* getDelay's, for the end of a status (kinds 9 to 11) */
    /* 0x04 */ void (*push)(BattleEvent *event);
    /* 0x08 */ void (*pushFirst)(BattleEvent *event); /* before all the others */
    /* 0x0C */ s32 (*pop)(void); /* the next event due */
    /* 0x10 */ s32 (*first)(s32 type);
    /* 0x14 */ s32 (*next)(void);
    /* 0x18 */ s32 (*find)(s32 type, u8 side, s32 fighter);
    /* 0x1C */ void (*remove)(EventKey *key); /* the events whose first two args are its */
    /* 0x20 */ s32 (*getDelay)(u8 side, s32 kind); /* FIGHTSTG_getEventDelay: when an event of side's runs */
    /* 0x24 */ void (*cureStatus)(u8 side, s32 fighter, s32 item); /* FIGHTSTG_cureStatus: an item's cure */
} EventQueueFuncs;

/* An event kind's delay range (FIGHTSTG_eventDelays, by kind): FIGHTSTG_getEventDelay
   adds a random part below div, and clamps to min and max where they are
   not 0 */
typedef struct EventDelay {
    /* 0x0 */ s16 div;
    /* 0x2 */ s16 min;
    /* 0x4 */ s16 max;
} EventDelay;

extern EventDelay FIGHTSTG_eventDelays[];

/* The battle's events (FIGHTSTG_events) */
typedef struct EventQueue {
    /* 0x000 */ QueuedEvent events[99];
    /* 0xAD4 */ u8 pad[0x1C]; /* nothing uses it */
    /* 0xAF0 */ s8 curType; /* the type of the event popped last */
    /* 0xAF1 */ s8 curIndex; /* and where it is */
    /* 0xAF2 */ s8 findType; /* what first and next look for, 1-24 */
    /* 0xAF3 */ s8 found; /* the event they found, or -1 */
    /* 0xAF4 */ EventQueueFuncs funcs;
} EventQueue;

/* The battle's sides, as the functions that take a side want them: side >> 4
   is its row of FIGHTSTG_battle.state.fighters, and SIDE_ENEMY - side the other
   side */
#define SIDE_PLAYER 0
#define SIDE_ENEMY 0x10

/* BattleFighter.flags: the statuses, which the techniques give
   (FIGHTSTG_inflict*) and events of type EVENT_STATUS_END + n end */
#define FIGHTER_POISONED 1 /* takes damage (EVENT_STATUS_DAMAGE) */
#define FIGHTER_PARALYZED 2 /* loses turns at random (FIGHTSTG_battleFuncs.testParalysis) */
#define FIGHTER_CONFUSED 4
#define FIGHTER_ASLEEP 8 /* can't act or run away; a hit may wake it up */
#define FIGHTER_NO_SWITCH 0x10 /* can't switch out or run away (TECH_EFFECT_NO_SWITCH) */
#define FIGHTER_NO_DIGIVOLVE 0x20 /* can't digivolve or use a switch-in technique (TECH_EFFECT_NO_DIGIVOLVE) */

/* One of the battle's fighters, three on each side (WFIGHTMN's BattleUnit):
   a partner's HP and MP go back to the party when the battle ends */
typedef struct BattleFighter {
    /* 0x00 */ s16 id; /* the Digimon (DIGIMON_DATA) */
    /* 0x02 */ s16 prevId; /* its own id while id is a temporary one */
    /* 0x04 */ s16 turns; /* an enemy's, for its battle table's conditions */
    /* 0x06 */ s16 maxHp;
    /* 0x08 */ s16 hp;
    /* 0x0A */ s16 maxMp;
    /* 0x0C */ s16 mp;
    /* 0x0E */ s16 charge; /* added in 64ths to its next technique's damage, which uses it up */
    /* 0x10 */ s16 boosts[4]; /* added to the stats FIGHTSTG_boostStats picks */
    /* 0x18 */ s16 item; /* an enemy's */
    /* 0x1A */ u8 temporary; /* id is a temporary Digimon */
    /* 0x1B */ u8 special; /* the partner's special state (EVENT_SPECIAL_END): doubles its effects */
    /* 0x1C */ u8 flags; /* FIGHTER_* */
    /* 0x1D */ u8 paralysis; /* FIGHTER_PARALYZED's strength */
    /* 0x1E */ u8 sleep; /* FIGHTER_ASLEEP's */
    /* 0x1F */ u8 confusion; /* FIGHTER_CONFUSED's */
} BattleFighter;

/* How the battle's frames go by (FIGHTSTG_setSpeed sets it) */
typedef struct BattleSpeed {
    /* 0x0 */ s32 mode; /* 1: stopped, 2: slowed (a frame per 4 of time), 3: double */
    /* 0x4 */ s32 rest; /* the time mode 2 hasn't counted yet */
} BattleSpeed;

/* The battle's element boost: one element's techniques do more damage, and
   the ones of the element it weakens less (FIGHTSTG_getElementBoost) */
typedef struct ElementBoost {
    /* 0x0 */ s16 element; /* under ELEMENT_FIRST: none */
    /* 0x2 */ s16 amount; /* in 128ths */
} ElementBoost;

/* What a battle starts from: WFIGHTMN_start zeroes it as one block, and
   FIGHTSTG_testRunAway reads runAttempts through a pointer to it */
typedef struct BattleState {
    /* 0x00 */ s32 active[2]; /* each side's fighter */
    /* 0x08 */ BattleFighter fighters[2][3];
    /* 0xC8 */ ElementBoost boost;
    /* 0xCC */ s16 runAttempts; /* the player's, each of which makes running away likelier (FIGHTSTG_testRunAway) */
    /* 0xCE */ s16 kind; /* the kind of battle */
    /* 0xD0 */ s16 tech; /* a technique id, set by WFIGHTMN */
    /* 0xD2 */ s8 hitCount; /* BATTLE_KIND_FINAL_LAST: the partner's hits that did damage */
    /* 0xD3 */ u8 weakened; /* BATTLE_KIND_FINAL_LAST: the enemy is weakened (FIGHTSTG_weakenEnemy) */
} BattleState;

/* FIGHTSTG_battle: the battle */
typedef struct Battle {
    /* 0x00 */ s32 pad; /* nothing uses it */
    /* 0x04 */ s32 frames; /* since the last update */
    /* 0x08 */ BattleState state;
    /* 0xDC */ BattleSpeed speed;
    /* 0xE4 */ void (*countFrames)(void);
    /* 0xE8 */ void (*setSpeed)(s32 mode);
    /* 0xEC */ void (*project)(Layer *layer, SVECTOR *pos, ShortVec3 *out); /* FIGHTSTG_projectPoint */
    /* 0xF0 */ void (*drawQuad)(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors);
    /* 0xF4 */ void (*drawBlendedQuad)(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors);
} Battle;

/* A technique's effect (TechData.effect), which FIGHTSTG_applyTechEffect
   carries out when its hit lands, and BattleAction.effects' index */
#define TECH_EFFECT_FIRST 2 /* under it, the technique has none */
#define TECH_EFFECT_POISON 2
#define TECH_EFFECT_PARALYSIS 3
#define TECH_EFFECT_CONFUSION 4
#define TECH_EFFECT_SLEEP 5
#define TECH_EFFECT_KNOCK_OUT 6
#define TECH_EFFECT_DRAIN 8 /* HP, in 128ths of the damage */
#define TECH_EFFECT_MULTI_HIT 9
#define TECH_EFFECT_ENEMY_ONLY 10 /* does nothing but for the enemy whose battle table entry lists it first */
#define TECH_EFFECT_CRITICAL 11 /* raises the critical hit chance */
#define TECH_EFFECT_STEAL 12 /* the enemy's item */
#define TECH_EFFECT_NO_SWITCH 13 /* FIGHTER_NO_SWITCH for a while */
#define TECH_EFFECT_NO_DIGIVOLVE 14 /* FIGHTER_NO_DIGIVOLVE for a while */
#define TECH_EFFECT_LOWER_ATTACK 26
#define TECH_EFFECT_LOWER_DEFENSE 27
#define TECH_EFFECT_DRAIN_MP 29 /* the partner's, in 128ths of its max MP */
#define TECH_EFFECT_DOUBLE_MAGIC 31 /* two magic hits, as techniques 0x1B9 and 0x1BA */
#define TECH_EFFECT_RAISE_ONE_STATUS 32 /* PartnerStats.status: one at random */
#define TECH_EFFECT_RAISE_EACH_STATUS 33 /* each one that a roll lets */
#define TECH_EFFECT_RAISE_ALL_STATUS 34
#define TECH_EFFECT_END_BATTLE 35 /* in BATTLE_KIND_UNK2: takes 70 percent of the partner's HP, ends as BATTLE_FLED */

/* FIGHTSTG_action: the technique being carried out */
typedef struct BattleAction {
    /* 0x00 */ u8 pad[0x20]; /* nothing uses it */
    /* 0x20 */ u8 side; /* the acting side, SIDE_PLAYER or SIDE_ENEMY */
    /* 0x24 */ s32 tech; /* the technique */
    /* 0x28 */ s32 damage; /* per hit */
    /* 0x2C */ s32 drain; /* the HP or MP that TECH_EFFECT_DRAIN or TECH_EFFECT_DRAIN_MP takes */
    /* 0x30 */ u8 hits[4]; /* whether each hit lands */
    /* 0x34 */ s16 hitsLanded;
    /* 0x36 */ s16 hitCount; /* rolled in hits */
    /* 0x38 */ u8 effects[0x28]; /* by TECH_EFFECT_*: what each one does, 0 for none */
    /* 0x60 */ s32 hitDamage[2]; /* TECH_EFFECT_DOUBLE_MAGIC's */
    /* 0x68 */ void (*start)(u8 side, s32 tech); /* FIGHTSTG_startAction */
} BattleAction;

/* The statuses a weapon's hits can inflict (BattleStats.statuses) */
#define HIT_POISON 0
#define HIT_PARALYSIS 1
#define HIT_CONFUSION 2
#define HIT_KNOCK_OUT 3 /* its power is read by nothing */
#define HIT_DRAIN 4 /* its power is in 128ths of the damage */
#define HIT_STATUS_COUNT 5

/* A status a weapon's hits can inflict */
typedef struct HitStatus {
    /* 0x0 */ u8 chance; /* in 128ths */
    /* 0x1 */ u8 power;
} HitStatus;

/* A side's stats as FIGHTSTG_computeStats works them out. A family is the
   kind of Digimon that a technique (TechData.family) or a weapon does half again
   the damage to, and to which it lands critical hits more often. */
typedef struct BattleStats {
    /* 0x00 */ s16 level;
    /* 0x02 */ s16 stats[5]; /* with the fighter's boosts */
    /* 0x0C */ s16 resist[RESIST_COUNT];
    /* 0x24 */ u8 flags; /* the fighter's */
    /* 0x25 */ u8 family;
    /* 0x26 */ u8 damageBonus; /* in 64ths: the fighter's charge */
    /* 0x27 */ u8 damageCut; /* taken off each hit */
    /* 0x28 */ u8 weaponFamilies[3]; /* from the equipment, as are the ones after */
    /* 0x2B */ u8 element; /* the accessory's, for techniques without one */
    /* 0x2C */ u8 elementPower;
    /* 0x2D */ HitStatus statuses[HIT_STATUS_COUNT]; /* by HIT_*, from the weapon */
    /* 0x37 */ u8 tripleHit; /* plain physical techniques hit 3 times for 0.4 of the damage */
    /* 0x38 */ u8 criticalBonus; /* to plain techniques' critical chance, off their accuracy */
    /* 0x39 */ u8 counter; /* answers hits with the partner's first skill */
    /* 0x3A */ u8 accuracy;
    /* 0x3B */ u8 evasion;
    /* 0x3C */ u8 runAwayBonus; /* to the player's chance to run away */
    /* 0x3D */ u8 runAwayGuard; /* halves the enemy's chance to run away */
    /* 0x3E */ u8 stealBonus;
    /* 0x3F */ u8 pad;
} BattleStats;

/* FIGHTSTG_battleFuncs */
typedef struct BattleFuncs {
    /* 0x00 */ BattleStats stats[2]; /* the player's, then the enemy's */
    /* 0x80 */ BattleStats *(*computeStats)(u8 side, s32 which, s32 index);
    /* 0x84 */ s32 (*computeDamage)(u8 side, s32 tech); /* of each hit */
    /* 0x88 */ s32 (*computeMagicDamage)(u8 side, s32 id);
    /* 0x8C */ s32 (*getDamage)(s32 *args); /* of the event whose args these are */
    /* 0x90 */ s32 (*computeCounterDamage)(u8 side, s32 id, s32 value);
    /* 0x94 */ s32 (*computeHeal)(u8 side, s32 id);
    /* 0x98 */ s32 (*getHeal)(u8 side, s32 index, s32 big); /* a part of its max HP */
    /* 0x9C */ s32 (*rollHit)(u8 side, s32 tech); /* whether a hit lands */
    /* 0xA0 */ s32 (*rollMagicHit)(u8 side, s32 id);
    /* 0xA4 */ s32 (*rollPoison)(u8 side, s32 id);
    /* 0xA8 */ s32 (*rollParalysis)(u8 side, s32 id);
    /* 0xAC */ s32 (*rollConfusion)(u8 side, s32 id);
    /* 0xB0 */ s32 (*rollSleep)(u8 side, s32 id);
    /* 0xB4 */ s32 (*rollKnockOut)(u8 side, s32 id);
    /* 0xB8 */ s32 (*rollSteal)(u8 side, s32 id);
    /* 0xBC */ s32 (*rollDrain)(u8 side, s32 id);
    /* 0xC0 */ s32 (*rollDedigivolve)(s32 actor, s32 id);
    /* 0xC4 */ s32 (*rollStatusRaise)(s32 actor, s32 id);
    /* 0xC8 */ s32 (*rollNoSwitch)(s32 actor, s32 id);
    /* 0xCC */ s32 (*rollNoDigivolve)(s32 actor, s32 id);
#if VERSION_EU || VERSION_JP
    /* 0xD0 */ s32 (*rollCounter)(s32 arg0, s32 arg1);
#endif
    /* the European version's offsets are 4 more from here */
    /* 0xD0 */ s32 (*testRunAway)(u8 side);
    /* 0xD4 */ s32 (*testWakeUp)(u8 side, s32 value);
    /* 0xD8 */ s32 (*testConfusion)(u8 side);
    /* 0xDC */ s32 (*testParalysis)(u8 side); /* whether side's paralysis costs it the turn */
    /* 0xE0 */ void (*changeBoost)(u8 side, s32 index, s32 stat, s32 percent); /* changes a fighter's stat boost */
    /* 0xE4 */ s32 (*getGaugeGain)(s32 damage); /* from the partner's damage, up to 1000 */
    /* 0xE8 */ s32 (*getTechCost)(u8 side, s32 id);
} BattleFuncs;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
/* WFIGHTMN passes them the battle, which they don't read; its code loads
   it, so no parameter list */
EnemyTurn *FIGHTSTG_startEnemyTurn(); /* (void): unprototyped, as WFIGHTMN passes it &FIGHTSTG_battle */

/* The events they queue for WFIGHTMN, and its turn's substate */
void FIGHTSTG_queuePlayerTurn(s32 delay);
void FIGHTSTG_queueEnemyTurn(s32 delay);
void FIGHTSTG_endBattle(s32 result);
void FIGHTSTG_queueRunAway(u8 side);
void FIGHTSTG_queueRecovery(u8 side, s32 fighter, s32 big);
void FIGHTSTG_queueKnockOut(u8 side);
void FIGHTSTG_queueBlast(void);
void FIGHTSTG_queueBlastEnd(s32 kind);
void FIGHTSTG_queueSpecialEnd(void);
void FIGHTSTG_weakenEnemy(void);
void FIGHTSTG_endEnemyWeakness(void);

/* Shared between the overlay's objects */
extern Battle FIGHTSTG_battle;
extern BattleAction FIGHTSTG_action;
/* FIGHTSTG_action's bytes before start, which a new action clears */
#define BATTLE_ACTION_STATE_SIZE ((u8 *)&FIGHTSTG_action.start - (u8 *)&FIGHTSTG_action)
extern BattleFuncs FIGHTSTG_battleFuncs;
extern s16 FIGHTSTG_boostStats[];
extern EventQueue FIGHTSTG_events;
extern u8 FIGHTSTG_statusEvents[]; /* FIGHTSTG_cureStatus's event types */
extern u8 FIGHTSTG_statusFlags[]; /* and the status flags they clear */
extern BattleEvent FIGHTSTG_newEvent;
extern BattleTableEntry *(*FIGHTSTG_battleTableFunc)(s32 id);
void FIGHTSTG_pushEvent(BattleEvent *event);
void FIGHTSTG_pushEventFirst(BattleEvent *event);
s32 FIGHTSTG_popEvent(void);
s32 FIGHTSTG_findEventFrom(s32 start);
s32 FIGHTSTG_findFirstEvent(s32 type);
s32 FIGHTSTG_findNextEvent(void);
s32 FIGHTSTG_findEvent(s32 type, u8 side, s32 fighter);
void FIGHTSTG_removeEvents(EventKey *key);
void FIGHTSTG_updateRoot(Task *task, Task **children);
s32 FIGHTSTG_pickEnemySwitch(EnemyTurn *task);
void FIGHTSTG_updateEnemyTurn(EnemyTurn *task, BattleChild *children);
void FIGHTSTG_queueLastEnemy(void);
s32 FIGHTSTG_testEnemyCondition(u8 condition, s16 arg);
s32 FIGHTSTG_getEnemyAction(u8 kind);
s32 FIGHTSTG_getEventDelay(u8 side, s32 kind);
void FIGHTSTG_cureStatus(u8 side, s32 fighter, s32 item);
void FIGHTSTG_tryPoison(void);
void FIGHTSTG_tryParalysis(void);
void FIGHTSTG_tryConfusion(void);
void FIGHTSTG_trySleep(void);
void FIGHTSTG_tryKnockOut(void);
void FIGHTSTG_rollMultiHit(void);
void FIGHTSTG_tryDrain(void);
void FIGHTSTG_markEnemyOnly(void);
void FIGHTSTG_markCritical(void);
void FIGHTSTG_lowerAttack(void);
void FIGHTSTG_lowerDefense(void);
void FIGHTSTG_drainMp(void);
void FIGHTSTG_raiseOneStatus(void);
void FIGHTSTG_raiseEachStatus(void);
void FIGHTSTG_raiseAllStatus(void);
void FIGHTSTG_tryNoSwitch(void);
void FIGHTSTG_startDoubleMagic(void);
void FIGHTSTG_startEndBattle(void);
void FIGHTSTG_applyTechEffect(void);
void FIGHTSTG_trySteal(void);
void FIGHTSTG_queueBoostEnd(u8 side, s32 fighter, s32 kind, s32 tech);
s32 FIGHTSTG_findBattleTableIndex(s32 id);
BattleTableEntry *FIGHTSTG_getBattleTableEntry(s32 id);
BattleStats *FIGHTSTG_computeStats(u8 side, s32 which, s32 index);
extern s32 FIGHTSTG_opposedElements[];
extern s32 FIGHTSTG_eventPopModes[]; /* per event type, FIGHTSTG_popEvent takes (1), peeks at (-1) or skips (0) it */
s32 FIGHTSTG_getElementBoost(s32 value, s32 element);
s32 FIGHTSTG_adjustDamage(u8 side, s32 id, s32 value);
s32 FIGHTSTG_rollCritical(u8 side, s32 id);
s32 FIGHTSTG_rollMagicCritical(u8 side, s32 id);
extern s32 FIGHTSTG_boostEvents[]; /* FIGHTSTG_queueBoostEnd's event types */
extern s32 FIGHTSTG_techEvents[]; /* and of FIGHTSTG_startRestriction */

/* events.c's statuses and events */
void FIGHTSTG_queueClearField(s32 time);
void FIGHTSTG_inflictConfusion(u8 side, s32 fromTech, u8 strength);
void FIGHTSTG_inflictPoison(u8 side, s32 fighter, s32 damage);
void FIGHTSTG_inflictParalysis(u8 side, s32 unused, u8 strength);
void FIGHTSTG_inflictSleep(u8 side, s32 unused, u8 strength);
void FIGHTSTG_queuePartnerTech(s32 tech);
void FIGHTSTG_queueAutoRecoverEnd(u8 side);
void FIGHTSTG_startRestriction(s32 tech);
void FIGHTSTG_queueDigidevolve(void);

/* the functions of FIGHTSTG_battle, FIGHTSTG_action and FIGHTSTG_battleFuncs */
void FIGHTSTG_startAction(u8 side, s32 tech);
void FIGHTSTG_countFrames(void);
void FIGHTSTG_setSpeed(s32 mode);
s32 FIGHTSTG_computeDamage(u8 side, s32 id);
s32 FIGHTSTG_computeMagicDamage(u8 side, s32 id);
s32 FIGHTSTG_getDamage(s32 *args);
s32 FIGHTSTG_computeCounterDamage(u8 side, s32 id, s32 value);
s32 FIGHTSTG_computeHeal(u8 side, s32 id);
s32 FIGHTSTG_getHeal(u8 side, s32 index, s32 big);
s32 FIGHTSTG_rollHit(u8 side, s32 id);
s32 FIGHTSTG_rollMagicHit(u8 side, s32 id);
s32 FIGHTSTG_rollPoison(u8 side, s32 id);
s32 FIGHTSTG_rollParalysis(u8 side, s32 id);
s32 FIGHTSTG_rollConfusion(u8 side, s32 id);
s32 FIGHTSTG_rollSleep(u8 side, s32 id);
s32 FIGHTSTG_rollKnockOut(u8 side, s32 id);
s32 FIGHTSTG_rollSteal(u8 side, s32 id);
s32 FIGHTSTG_rollDrain(u8 side, s32 id);
s32 FIGHTSTG_rollDedigivolve(s32 actor, s32 id);
s32 FIGHTSTG_rollStatusRaise(s32 actor, s32 id);
s32 FIGHTSTG_rollNoSwitch(s32 actor, s32 id);
s32 FIGHTSTG_rollNoDigivolve(s32 actor, s32 id);
s32 FIGHTSTG_testRunAway(u8 side);
#if VERSION_EU || VERSION_JP
s32 FIGHTSTG_rollCounter(s32 side, s32 damage);
#endif
s32 FIGHTSTG_testWakeUp(u8 side, s32 value);
s32 FIGHTSTG_testConfusion(u8 side);
s32 FIGHTSTG_testParalysis(u8 side);
void FIGHTSTG_changeBoost(u8 side, s32 index, s32 stat, s32 percent);
s32 FIGHTSTG_getGaugeGain(s32 damage);
s32 FIGHTSTG_getTechCost(u8 side, s32 id);

/* Row's active fighter (FIGHTSTG_battle.state.fighters[row][active[row]]), as the
   row's offset in bytes added to the slot's fighter in row 0 */
static inline BattleFighter *FIGHTSTG_getActiveFighter(s32 row) {
    s32 offset = row * sizeof(FIGHTSTG_battle.state.fighters[0]);
    BattleFighter *slot = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[row]];

    return (BattleFighter *)(offset + (s32)slot);
}

#endif /* FIGHTSTG_BATTLE_H */
