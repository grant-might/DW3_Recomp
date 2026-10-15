/* FIGHTSTG's battle state (FIGHTSTG_action, FIGHTSTG_battle and
   FIGHTSTG_battleFuncs) and the events of the partner's special techniques. */

#include "battle/battle_mode.h"

/* the event types of FIGHTSTG_startRestriction, by whether the technique's effect is TECH_EFFECT_NO_DIGIVOLVE */
s32 FIGHTSTG_techEvents[] = {
    EVENT_RESTRICTION_END, EVENT_RESTRICTION_END + 1,
};

/* the command being carried out */
BattleAction FIGHTSTG_action = {
    { 0 }, 0, 0, 0, 0, { 0 }, 0, 0, { 0 }, { 0 }, FIGHTSTG_startAction,
};
/* the battle: its fighters, speed and drawing functions */
Battle FIGHTSTG_battle = {
    0, 0, { { 0 }, { { { 0 } } }, { 0, 0 }, 0, 0, 0, 0, 0 }, { 0, 0 },
    FIGHTSTG_countFrames, FIGHTSTG_setSpeed, FIGHTSTG_projectPoint, FIGHTSTG_drawQuad, FIGHTSTG_drawBlendedQuad,
};
/* the fighters' data, loaded one at a time */
FighterCache FIGHTSTG_fighterCache = {
    0, 0, 0, 0, NULL, NULL, { FIGHTSTG_getFighterInfo, FIGHTSTG_cacheFighter, FIGHTSTG_getFighterRange }, FIGHTSTG_getFighterFace,
};
/* the battle's stats and the functions that compute with them */
BattleFuncs FIGHTSTG_battleFuncs = {
    { { 0 }, { 0 } },
    FIGHTSTG_computeStats, FIGHTSTG_computeDamage, FIGHTSTG_computeMagicDamage, FIGHTSTG_getDamage,
    FIGHTSTG_computeCounterDamage, FIGHTSTG_computeHeal, FIGHTSTG_getHeal, FIGHTSTG_rollHit,
    FIGHTSTG_rollMagicHit, FIGHTSTG_rollPoison, FIGHTSTG_rollParalysis, FIGHTSTG_rollConfusion,
    FIGHTSTG_rollSleep, FIGHTSTG_rollKnockOut, FIGHTSTG_rollSteal, FIGHTSTG_rollDrain,
    FIGHTSTG_rollDedigivolve, FIGHTSTG_rollStatusRaise, FIGHTSTG_rollNoSwitch, FIGHTSTG_rollNoDigivolve,
#if VERSION_EU || VERSION_JP
    FIGHTSTG_rollCounter,
#endif
    FIGHTSTG_testRunAway, FIGHTSTG_testWakeUp, FIGHTSTG_testConfusion, FIGHTSTG_testParalysis,
    FIGHTSTG_changeBoost, FIGHTSTG_getGaugeGain, FIGHTSTG_getTechCost,
};

/* Restricts the partner with technique tech: sets FIGHTER_NO_SWITCH or
   FIGHTER_NO_DIGIVOLVE by the technique's effect, and queues the
   restriction's end (by its effectPower) or pushes back the one already
   queued */
void FIGHTSTG_startRestriction(s32 tech) {
    TechData *entry = &TECHS[tech - 1];
    s32 kind;
    s32 fighter;
    s32 i;
    s32 time;
    BattleFighter *target;

    kind = 0;
    if (entry->effect != TECH_EFFECT_NO_SWITCH) {
        kind = entry->effect == TECH_EFFECT_NO_DIGIVOLVE;
    }
    fighter = FIGHTSTG_battle.state.active[0];
    i = FIGHTSTG_findEvent(FIGHTSTG_techEvents[kind], 0, fighter);
    time = (RANDOM.next() % 101 + 100) * entry->effectPower;

    if (i >= 0) {
        QueuedEvent *queued = &FIGHTSTG_events.events[i];

        queued->time = time;
    } else {
        FIGHTSTG_newEvent.type = FIGHTSTG_techEvents[kind];
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = 0;
        FIGHTSTG_newEvent.args[1] = fighter;
        FIGHTSTG_newEvent.args[2] = kind;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
    target = &FIGHTSTG_battle.state.fighters[0][fighter];
    if (kind == 0) {
        target->flags |= FIGHTER_NO_SWITCH;
    } else {
        target->flags |= FIGHTER_NO_DIGIVOLVE;
    }
}

/* Queues the partner's technique tech, to run as its command */
void FIGHTSTG_queuePartnerTech(s32 tech) {
    FIGHTSTG_newEvent.type = EVENT_PARTNER_TECH;
    FIGHTSTG_newEvent.delay = 0x7FFF;
    FIGHTSTG_newEvent.args[0] = 0;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[0];
    FIGHTSTG_newEvent.args[2] = tech;
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Queues the partner's digivolution for the battle (event 18), now */
void FIGHTSTG_queueBlast(void) {
    FIGHTSTG_newEvent.type = EVENT_BLAST;
    FIGHTSTG_newEvent.delay = 0;
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Queues the end of the partner's digivolution for the battle (event 19),
   after delay kind + 3 */
void FIGHTSTG_queueBlastEnd(s32 kind) {
    FIGHTSTG_newEvent.type = EVENT_BLAST_END;
    FIGHTSTG_newEvent.delay = FIGHTSTG_getEventDelay(0, kind + 3);
    FIGHTSTG_newEvent.args[0] = 0;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[0];
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Queues the fall of side's active fighter (event 20), first */
void FIGHTSTG_queueKnockOut(u8 side) {
    FIGHTSTG_newEvent.type = EVENT_KNOCK_OUT;
    FIGHTSTG_newEvent.delay = 1;
    FIGHTSTG_newEvent.args[0] = side;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[side != 0];
    FIGHTSTG_pushEventFirst(&FIGHTSTG_newEvent);
}

/* Queues the end of the partner's special state (event 21): the USA version
   pushes back the one already queued */
void FIGHTSTG_queueSpecialEnd(void) {
#if VERSION_US
    s32 i = FIGHTSTG_findEvent(EVENT_SPECIAL_END, 0, FIGHTSTG_battle.state.active[0]);
    s32 time = FIGHTSTG_getEventDelay(0, 8);

    if (i >= 0) {
        QueuedEvent *queued = &FIGHTSTG_events.events[i];

        queued->time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_SPECIAL_END;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = 0;
        FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[0];
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
#elif VERSION_EU || VERSION_JP
    FIGHTSTG_newEvent.type = EVENT_SPECIAL_END;
    FIGHTSTG_newEvent.delay = 1;
    FIGHTSTG_newEvent.args[0] = 0;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[0];
    FIGHTSTG_pushEventFirst(&FIGHTSTG_newEvent);
#endif
}

/* Queues the partner's digidevolution, first */
void FIGHTSTG_queueDigidevolve(void) {
    FIGHTSTG_newEvent.type = EVENT_DIGIDEVOLVE;
    FIGHTSTG_newEvent.delay = 1;
    FIGHTSTG_newEvent.args[0] = 0;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[0];
    FIGHTSTG_pushEventFirst(&FIGHTSTG_newEvent);
}

/* Queues the coming in of the last battle's third enemy, first */
void FIGHTSTG_queueLastEnemy(void) {
    FIGHTSTG_newEvent.type = EVENT_LAST_ENEMY;
    FIGHTSTG_newEvent.delay = 1;
    FIGHTSTG_pushEventFirst(&FIGHTSTG_newEvent);
}

/* Weakens the enemy in BATTLE_KIND_FINAL_LAST: halves its boosts of stats 1 and 2
   and queues the end (event 24) in 3000 */
void FIGHTSTG_weakenEnemy(void) {
    BattleTableEntry *entry;

    FIGHTSTG_newEvent.type = EVENT_WEAKNESS_END;
    FIGHTSTG_newEvent.delay = 3000;
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    entry = FIGHTSTG_battleTableFunc(0x1D3);
    FIGHTSTG_battle.state.weakened = 1;
    FIGHTSTG_battle.state.fighters[1][0].boosts[1] = -entry->stats[1] >> 1;
    FIGHTSTG_battle.state.fighters[1][0].boosts[3] = -entry->stats[2] >> 1;
}

/* Ends the enemy's weakness now: its queued event 24 runs next */
void FIGHTSTG_endEnemyWeakness(void) {
    s32 i = FIGHTSTG_events.funcs.first(EVENT_WEAKNESS_END);

    if (i >= 0) {
        FIGHTSTG_events.events[i].time = 1;
    }
}
