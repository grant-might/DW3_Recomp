/* FIGHTSTG's events: the queue of the battle's events in time
   (FIGHTSTG_events), and the functions that queue turns, statuses, recovery
   and the end of boosts: one module, as the original's jump tables are
   8-aligned from its start and a cut between these would move the second
   one's. */

#include "battle/battle_mode.h"

/* FIGHTSTG_events.funcs.push: queues an event in the first free one of the
   99 slots, due after its delay */
void FIGHTSTG_pushEvent(BattleEvent *event) {
    s32 i = 0;
    s32 free = -1;

    for (; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type == 0) {
            free = i;
            break;
        }
    }
    if (free != -1) {
        FIGHTSTG_events.events[free].type = event->type;
        FIGHTSTG_events.events[free].time = event->delay;
        for (i = 0; i < 6; i++) {
            FIGHTSTG_events.events[free].args[i] = event->args[i];
        }
    }
}

/* per event type, whether FIGHTSTG_popEvent takes (1), peeks at (-1) or
   drops (0) it */
s32 FIGHTSTG_eventPopModes[] = {
    0, 1, 1, 1,
    -1, 1, -1, 1,
    1, -1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 0,
};

/* the battle's events and their functions */
EventQueue FIGHTSTG_events = {
    { { 0 } }, { 0 }, 0, 0, 0, 0,
    {
        0, 0, FIGHTSTG_pushEvent, FIGHTSTG_pushEventFirst, FIGHTSTG_popEvent,
        FIGHTSTG_findFirstEvent, FIGHTSTG_findNextEvent, FIGHTSTG_findEvent, FIGHTSTG_removeEvents,
        FIGHTSTG_getEventDelay, FIGHTSTG_cureStatus,
    },
};

/* FIGHTSTG_events.funcs.pushFirst: queues an event ahead of all the others,
   which wait its delay longer (at least 1, 2 in the European version) */
void FIGHTSTG_pushEventFirst(BattleEvent *event) {
    s32 i;

#if VERSION_US
    if (event->delay <= 0) {
        event->delay = 1;
    }
#elif VERSION_EU || VERSION_JP
    if (event->delay < 2) {
        event->delay = 2;
    }
#endif
    for (i = 0; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type != 0) {
            FIGHTSTG_events.events[i].time += event->delay;
        }
    }
#if VERSION_US
    event->delay = 0;
#elif VERSION_EU || VERSION_JP
    event->delay = 1;
#endif
    FIGHTSTG_pushEvent(event);
}

/* Pops the next event: the one due soonest (in EU, events of types 2 and 3
   give way to any due no later), moving all the others' times on by its.
   Returns its type, or 0 when there's none or FIGHTSTG_eventPopModes says to
   drop it. */
s32 FIGHTSTG_popEvent(void) {
    s32 min = 0x7FFF;
    s32 best = -1;
    s32 i;
    s32 time;
#if VERSION_EU || VERSION_JP
    QueuedEvent *event;
#endif
    s32 type;

    for (i = 0; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type != 0) {
#if VERSION_EU || VERSION_JP
            if (best != -1 && FIGHTSTG_events.events[i].time <= FIGHTSTG_events.events[best].time &&
                (FIGHTSTG_events.events[best].type == EVENT_PLAYER_TURN || FIGHTSTG_events.events[best].type == EVENT_ENEMY_TURN)) {
                best = i;
                min = FIGHTSTG_events.events[i].time;
            }
#endif
            if (FIGHTSTG_events.events[i].time < min) {
                best = i;
                min = FIGHTSTG_events.events[i].time;
            }
        }
    }
    if (best == -1) {
        return 0;
    }
#if VERSION_US
    time = FIGHTSTG_events.events[best].time;
    for (i = 0; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type != 0) {
            FIGHTSTG_events.events[i].time -= time;
        }
    }
    FIGHTSTG_events.curType = FIGHTSTG_events.events[best].type;
    switch (FIGHTSTG_eventPopModes[FIGHTSTG_events.curType]) {
    case 0:
        return 0;
    case -1:
        type = FIGHTSTG_events.events[best].type;
        FIGHTSTG_events.curIndex = best;
        return type;
    default:
        type = FIGHTSTG_events.events[best].type;
        FIGHTSTG_events.curIndex = best;
        FIGHTSTG_events.events[best].type = 0;
        return type;
    }
#elif VERSION_EU || VERSION_JP
    event = &FIGHTSTG_events.events[best];
    time = event->time;
    for (i = 0; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type != 0) {
            FIGHTSTG_events.events[i].time -= time;
        }
    }
    FIGHTSTG_events.curType = event->type;
    switch (FIGHTSTG_eventPopModes[FIGHTSTG_events.curType]) {
    case 0:
        return 0;
    case -1:
        type = event->type;
        FIGHTSTG_events.curIndex = best;
        return type;
    default:
        type = event->type;
        event->type = 0;
        FIGHTSTG_events.curIndex = best;
        return type;
    }
#endif
}

/* The first queued event from slot start of the type being looked for
   (FIGHTSTG_events.findType, 1-24), or -1 */
s32 FIGHTSTG_findEventFrom(s32 start) {
    s32 type = FIGHTSTG_events.findType;
    s32 i;

    if ((u32)(type - 1) >= 24) {
        return -1;
    }
    for (i = start; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type == type) {
            return i;
        }
    }
    return -1;
}

/* FIGHTSTG_events.funcs.first: the first queued event of type, or -1 */
s32 FIGHTSTG_findFirstEvent(s32 type) {
    FIGHTSTG_events.findType = type;
    return FIGHTSTG_events.found = FIGHTSTG_findEventFrom(0);
}

/* FIGHTSTG_events.funcs.next: the next queued event of the same type, or -1 */
s32 FIGHTSTG_findNextEvent(void) {
    return FIGHTSTG_events.found = FIGHTSTG_findEventFrom(FIGHTSTG_events.found + 1);
}

/* FIGHTSTG_events.funcs.find: the first queued event of type for side's
   fighter, or -1 */
s32 FIGHTSTG_findEvent(s32 type, u8 side, s32 fighter) {
    FIGHTSTG_events.findType = type;
    FIGHTSTG_events.found = FIGHTSTG_findEventFrom(0);
    while (FIGHTSTG_events.found >= 0) {
        if (FIGHTSTG_events.events[FIGHTSTG_events.found].args[0] == side && FIGHTSTG_events.events[FIGHTSTG_events.found].args[1] == fighter) {
            break;
        }
        FIGHTSTG_events.found = FIGHTSTG_findEventFrom(FIGHTSTG_events.found + 1);
    }
    return FIGHTSTG_events.found;
}

/* FIGHTSTG_events.funcs.remove: drops every queued event of key's side and
   fighter */
void FIGHTSTG_removeEvents(EventKey *key) {
    s32 i;
    s32 fighter = key->fighter;
    s32 side = key->side;

    for (i = 0; i < 99; i++) {
        if (FIGHTSTG_events.events[i].type != 0 && FIGHTSTG_events.events[i].args[0] == side && FIGHTSTG_events.events[i].args[1] == fighter) {
            FIGHTSTG_events.events[i].type = 0;
        }
    }
}

/* the delay ranges of FIGHTSTG_getEventDelay's kinds */
EventDelay FIGHTSTG_eventDelays[] = {
    { 1000, 707, 1414 }, { 250, 176, 353 }, { 2001, 1001, 0 }, { 2000, 0, 0 },
    { 2500, 0, 0 }, { 3000, 0, 0 }, { 3500, 0, 0 }, { 4000, 0, 0 },
    { 0, 2000, 6000 }, { 500, 1000, 0 }, { 500, 1000, 0 }, { 500, 500, 0 },
    { 2001, 0, 0 },
};

/* FIGHTSTG_events.funcs.getDelay: the time until side's next event of kind
   (FIGHTSTG_eventDelays): random, or from the active fighters' stats and
   the other side's resistances, within the kind's range */
s32 FIGHTSTG_getEventDelay(u8 side, s32 kind) {
    s32 delay;

    /* the match depends on each case having its own row and stats */
    switch (kind) {
    case 2: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);

        delay = RANDOM.next() % FIGHTSTG_eventDelays[kind].div + own->stats[BATTLE_STAT_SPIRIT] * 10;
        break;
    }
    case 8:
        delay = RANDOM.next() % 8001;
        break;
    case 9: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);
        BattleStats *other = FIGHTSTG_battleFuncs.computeStats((side == 0) << 4, 0, FIGHTSTG_battle.state.active[1 - row]);

        delay = RANDOM.next() % FIGHTSTG_eventDelays[kind].div + 3000 + (own->stats[BATTLE_STAT_SPIRIT] + FIGHTSTG_events.funcs.statusStrength) * 8
              - (other->resist[RESIST_PARALYSIS] + other->resist[4]) * 8;
        break;
    }
    case 10: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);
        BattleStats *other = FIGHTSTG_battleFuncs.computeStats((side == 0) << 4, 0, FIGHTSTG_battle.state.active[1 - row]);

        delay = RANDOM.next() % FIGHTSTG_eventDelays[kind].div + 3000 + (own->stats[BATTLE_STAT_SPIRIT] + FIGHTSTG_events.funcs.statusStrength) * 8
              - (other->resist[RESIST_CONFUSION] + other->resist[3]) * 8;
        break;
    }
    case 11: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);
        BattleStats *other = FIGHTSTG_battleFuncs.computeStats((side == 0) << 4, 0, FIGHTSTG_battle.state.active[1 - row]);

        delay = RANDOM.next() % FIGHTSTG_eventDelays[kind].div + 1000 + (own->stats[BATTLE_STAT_SPIRIT] + FIGHTSTG_events.funcs.statusStrength) * 8
              - (other->resist[RESIST_SLEEP] + other->resist[2]) * 8;
        break;
    }
    case 12: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);

        delay = RANDOM.next() % FIGHTSTG_eventDelays[kind].div + 2000 + own->stats[BATTLE_STAT_SPIRIT] * 10;
        break;
    }
    default: {
        s32 row = side != 0;
        BattleStats *own = FIGHTSTG_battleFuncs.computeStats(side, 1, FIGHTSTG_battle.state.active[row]);
        BattleStats *other = FIGHTSTG_battleFuncs.computeStats(SIDE_ENEMY - side, 0, FIGHTSTG_battle.state.active[1 - row]);
        s32 square = own->stats[BATTLE_STAT_SPEED] * other->stats[BATTLE_STAT_SPEED];
        s32 root = 999;
        s32 i;

        /* Newton's square root */
        for (i = 0; i < 10; i++) {
            root = (root + square / root) / 2;
        }
        delay = FIGHTSTG_eventDelays[kind].div * other->stats[BATTLE_STAT_SPEED] / root;
        break;
    }
    }
    if (FIGHTSTG_eventDelays[kind].min != 0 && delay < FIGHTSTG_eventDelays[kind].min) {
        delay = FIGHTSTG_eventDelays[kind].min;
    }
    if (FIGHTSTG_eventDelays[kind].max != 0 && delay > FIGHTSTG_eventDelays[kind].max) {
        delay = FIGHTSTG_eventDelays[kind].max;
    }
    return delay;
}

/* the status events FIGHTSTG_cureStatus clears, by kind of status, */
u8 FIGHTSTG_statusEvents[] = {
    EVENT_STATUS_DAMAGE, EVENT_STATUS_END, EVENT_STATUS_END + 1, EVENT_STATUS_END + 2,
    EVENT_RESTRICTION_END, EVENT_RESTRICTION_END + 1, 0, 0,
};
/* and the flags it clears with them (all six for kind 3) */
u8 FIGHTSTG_statusFlags[] = {
    FIGHTER_POISONED, FIGHTER_PARALYZED, FIGHTER_CONFUSED, 0x3F, /* all six */
};

/* An item's cure (FIGHTSTG_events.funcs.cureStatus): items 0xBE-0xC5 clear a
   status of the fighter and remove its events, 0xC4 and 0xC5 all of them */
void FIGHTSTG_cureStatus(u8 side, s32 fighter, s32 item) {
    s32 kind;
    s32 index;
    BattleFighter *fighters;
    s32 i;
    s32 row;

    switch (item) {
    case 0xBE:
    case 0xBF:
    default:
        kind = 0;
        break;
    case 0xC0:
    case 0xC1:
        kind = 1;
        break;
    case 0xC2:
    case 0xC3:
        kind = 2;
        break;
    case 0xC4:
    case 0xC5:
        kind = 3;
        break;
    }
    /* the match depends on the row local, and on kind becoming the event type */
    row = side != 0;
    fighters = FIGHTSTG_battle.state.fighters[row];
    if (fighters[fighter].id == 0) {
        return;
    }
    fighters[fighter].flags &= ~FIGHTSTG_statusFlags[kind];
    if (kind != 3) {
        kind = FIGHTSTG_statusEvents[kind];
        index = FIGHTSTG_events.funcs.find(kind, side, fighter);
        if (index >= 0) {
            FIGHTSTG_events.events[index].type = 0;
        }
    } else {
        for (i = 0; i < 6; i++) {
            index = FIGHTSTG_events.funcs.find(FIGHTSTG_statusEvents[i], side, fighter);
            if (index >= 0) {
                FIGHTSTG_events.events[index].type = 0;
            }
        }
    }
}

/* Queues the player's turn (event 2) in delay */
void FIGHTSTG_queuePlayerTurn(s32 delay) {
    FIGHTSTG_newEvent.type = EVENT_PLAYER_TURN;
    FIGHTSTG_newEvent.delay = delay;
    FIGHTSTG_newEvent.args[0] = -1;
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Queues the enemy's turn (event 3) in delay */
void FIGHTSTG_queueEnemyTurn(s32 delay) {
    FIGHTSTG_newEvent.type = EVENT_ENEMY_TURN;
    FIGHTSTG_newEvent.delay = delay;
    FIGHTSTG_newEvent.args[0] = -1;
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Ends the battle: queues event 1 ahead of the others, with how it ended
   (BATTLE_FLED, BATTLE_WON or BATTLE_LOST) for WFIGHTMN */
void FIGHTSTG_endBattle(s32 result) {
    FIGHTSTG_newEvent.type = EVENT_END_BATTLE;
    FIGHTSTG_newEvent.delay = 1;
    FIGHTSTG_newEvent.args[0] = -1;
    FIGHTSTG_events.funcs.result = result;
    FIGHTSTG_pushEventFirst(&FIGHTSTG_newEvent);
}

/* Queues side's attempt to run away (event 4) */
void FIGHTSTG_queueRunAway(u8 side) {
    FIGHTSTG_newEvent.type = EVENT_RUN_AWAY;
    FIGHTSTG_newEvent.delay = FIGHTSTG_getEventDelay(side, 1);
    FIGHTSTG_newEvent.args[0] = side;
    FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[side != 0];
    FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
}

/* Queues the end of side's automatic recovery (item 0xBD), or pushes back the
   one already queued */
void FIGHTSTG_queueAutoRecoverEnd(u8 side) {
    s32 other = side != 0;
    s32 i = FIGHTSTG_findEvent(EVENT_AUTO_RECOVER_END, side, other);
    s32 time = FIGHTSTG_getEventDelay(side, 2);

    if (i >= 0) {
        QueuedEvent *queued = &FIGHTSTG_events.events[i];

        queued->time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_AUTO_RECOVER_END;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[other];
        FIGHTSTG_newEvent.args[2] = 0xBD;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
}

/* Queues a fighter's recovery of HP (event 6, with what getHeal takes as
   big), or sets the one already queued to 0xBD */
void FIGHTSTG_queueRecovery(u8 side, s32 fighter, s32 big) {
    s32 i = FIGHTSTG_findEvent(EVENT_RECOVERY, side, fighter);

    if (i >= 0) {
        QueuedEvent *queued = &FIGHTSTG_events.events[i];

        queued->args[2] = 0xBD;
    } else {
        FIGHTSTG_newEvent.type = EVENT_RECOVERY;
        FIGHTSTG_newEvent.delay = 1000;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = fighter;
        FIGHTSTG_newEvent.args[2] = big;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
}

/* Queues the field's clearing in time (at most 0x7FFF), or moves the one
   already queued to it */
void FIGHTSTG_queueClearField(s32 time) {
    s32 i;

    FIGHTSTG_events.findType = EVENT_CLEAR_FIELD;
    i = FIGHTSTG_findEventFrom(0);
    if (time > 0x7FFF) {
        time = 0x7FFF;
    }
    if (i >= 0) {
        FIGHTSTG_events.events[i].time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_CLEAR_FIELD;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = -1;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
}

/* Poisons a fighter (FIGHTER_POISONED), and queues its damage or updates the
   one already queued */
void FIGHTSTG_inflictPoison(u8 side, s32 fighter, s32 damage) {
    s32 i = FIGHTSTG_findEvent(EVENT_STATUS_DAMAGE, side, fighter);
    s32 other;
    BattleFighter *entry;

    if (i >= 0) {
        QueuedEvent *queued = &FIGHTSTG_events.events[i];

        queued->args[2] = damage;
    } else {
        FIGHTSTG_newEvent.type = EVENT_STATUS_DAMAGE;
        FIGHTSTG_newEvent.delay = 1000;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = fighter;
        FIGHTSTG_newEvent.args[2] = damage;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
    other = side != 0;
    entry = &FIGHTSTG_battle.state.fighters[other][fighter];
    entry->flags |= FIGHTER_POISONED;
}

/* Paralyzes side's active fighter (FIGHTER_PARALYZED) with a strength, and
   queues its end, which the strength puts off, or pushes back the one
   already queued */
void FIGHTSTG_inflictParalysis(u8 side, s32 unused, u8 strength) {
    s32 other = side != 0;
    s32 i = FIGHTSTG_findEvent(EVENT_STATUS_END, side, FIGHTSTG_battle.state.active[other]);
    s32 time;
    BattleFighter *entry;

    FIGHTSTG_events.funcs.statusStrength = strength;
    time = FIGHTSTG_getEventDelay(side, 9);
    if (i >= 0) {
        FIGHTSTG_events.events[i].time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_STATUS_END;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[other];
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
    entry = &FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]];
    entry->paralysis = strength;
    entry->flags |= FIGHTER_PARALYZED;
}

/* Confuses side's active fighter (FIGHTER_CONFUSED) with a strength, and
   queues its end (put off by the strength when a technique, fromTech,
   causes it) or pushes back the one already queued */
void FIGHTSTG_inflictConfusion(u8 side, s32 fromTech, u8 strength) {
    s32 other = side != 0;
    s32 i = FIGHTSTG_findEvent(EVENT_STATUS_END + 1, side, FIGHTSTG_battle.state.active[other]);
    s32 time;
    BattleFighter *entry;

    if (fromTech == 0) {
        time = FIGHTSTG_getEventDelay(side, 8);
    } else {
        FIGHTSTG_events.funcs.statusStrength = strength;
        time = FIGHTSTG_getEventDelay(side, 10);
    }
    if (i >= 0) {
        FIGHTSTG_events.events[i].time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_STATUS_END + 1;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[other];
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
    entry = &FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]];
    entry->confusion = strength;
    entry->flags |= FIGHTER_CONFUSED;
}

/* Puts side's active fighter to sleep (FIGHTER_ASLEEP) with a strength, and
   queues its end, which the strength puts off, or pushes back the one
   already queued */
void FIGHTSTG_inflictSleep(u8 side, s32 unused, u8 strength) {
    s32 other = side != 0;
    s32 i = FIGHTSTG_findEvent(EVENT_STATUS_END + 2, side, FIGHTSTG_battle.state.active[other]);
    s32 time;
    BattleFighter *entry;

    FIGHTSTG_events.funcs.statusStrength = strength;
    time = FIGHTSTG_getEventDelay(side, 11);
    if (i >= 0) {
        FIGHTSTG_events.events[i].time = time;
    } else {
        FIGHTSTG_newEvent.type = EVENT_STATUS_END + 2;
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = FIGHTSTG_battle.state.active[other];
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
    entry = &FIGHTSTG_battle.state.fighters[other][FIGHTSTG_battle.state.active[other]];
    entry->sleep = strength;
    entry->flags |= FIGHTER_ASLEEP;
}

/* the event types of FIGHTSTG_queueBoostEnd, by the stat boosted */
s32 FIGHTSTG_boostEvents[] = {
    EVENT_BOOST_END, EVENT_BOOST_END + 1, EVENT_BOOST_END + 2,
};

/* Queues the end of a fighter's boost of stat kind, 12 frames on when a
   technique (tech) gave it and 8 when an item did (tech 0), or pushes back
   the one already queued */
void FIGHTSTG_queueBoostEnd(u8 side, s32 fighter, s32 kind, s32 tech) {
    s32 i = FIGHTSTG_findEvent(FIGHTSTG_boostEvents[kind], side, fighter);
    s32 time;

    if (tech != 0) {
        time = FIGHTSTG_getEventDelay(side, 12);
    } else {
        time = FIGHTSTG_getEventDelay(side, 8);
    }
    if (i >= 0) {
        FIGHTSTG_events.events[i].time = time;
    } else {
        FIGHTSTG_newEvent.type = FIGHTSTG_boostEvents[kind];
        FIGHTSTG_newEvent.delay = time;
        FIGHTSTG_newEvent.args[0] = side;
        FIGHTSTG_newEvent.args[1] = fighter;
        FIGHTSTG_newEvent.args[2] = kind;
        FIGHTSTG_pushEvent(&FIGHTSTG_newEvent);
    }
}
