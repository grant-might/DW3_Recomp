/* WFIGHTMN's display layers and the battle's fighters: their stats, the
   ambush roll, the WFIGHTMN_RECOVERY_ITEM checks and the record of who fought. */

#include "battle/normal_battle.h"

/* Sets up the display and the battle's layers */
void WFIGHTMN_createLayers(void) {
    FIGHTSTG_CREATE_LAYERS(WFIGHTMN_screen, 10);
}

/* Sets a stat and its maximum. The match depends on the pointers: stores
   through them aren't struct accesses to GCC, so the load of
   FIGHTSTG_battleTableFunc stays after them */
static inline void WFIGHTMN_setStat(s16 *cur, s16 *max, s16 value) {
    *cur = *max = value;
}

/* Fills the battle's fighters: the party's partners (the first one with the
   id DIGIMON) and the encounter's enemies, with their battle table items */
void WFIGHTMN_initFighters(s32 digimon) {
    PartnerStats *stats;
    BattleTableEntry *entry;
    BattleFighter *fighter;
    BattleFighter *units;
    s32 partner;
    s32 i;
    units = FIGHTSTG_battle.state.fighters[0];
    for (i = 0; i < 3; i++) {
        partner = GAME.funcs.getPartyMember(i);
        if (partner >= 0) {
            stats = GAME.funcs.getPartnerStats(partner);
            if (i == 0) {
                units[0].id = digimon;
            } else {
                units[i].id = DIGIMON_DATA[partner].id;
            }
            units[i].hp = stats->stats[STAT_HP];
            units[i].maxHp = stats->stats[STAT_MAX_HP];
            units[i].mp = stats->stats[STAT_MP];
            units[i].maxMp = stats->stats[STAT_MAX_MP];
        }
    }
    units = FIGHTSTG_battle.state.fighters[1];
    for (i = 0; i < 3; i++) {
        fighter = &units[i];
        fighter->id = BATTLE_SETUP.enemies[i].fighter;
        if (BATTLE_SETUP.enemies[i].fighter != 0) {
            WFIGHTMN_setStat(&fighter->hp, &fighter->maxHp, BATTLE_SETUP.enemies[i].hp);
            WFIGHTMN_setStat(&fighter->mp, &fighter->maxMp, BATTLE_SETUP.enemies[i].mp);
            entry = FIGHTSTG_battleTableFunc(fighter->id);
            if (entry->item != 0) {
                fighter->item = entry->item;
            }
        }
    }
}

/* Checks the chance in BATTLE_SETUP.ambushChance, scaled by how far the first
   partner's level and the first enemy's level are under 32 */
s32 WFIGHTMN_rollAmbush(void) {
    s32 chance;
    s32 gap;
    s32 r;

    if (BATTLE_SETUP.ambushChance == 0) {
        return 0;
    }
    /* gap on its own line: the match depends on it, which loads the level
       before the enemy's */
    gap = 32 - GAME.partners[GAME.funcs.getPartyMember(0)].info.stats[STAT_LEVEL];
    chance = BATTLE_SETUP.ambushChance * (gap - BATTLE_SETUP.enemies[0].level) / 32;
    r = RANDOM.next() % 128;
    if (r == 0) {
        return 1;
    }
    return r < chance;
}

/* Queues a recovery (FIGHTSTG_queueRecovery) for party member MEMBER's
   partner when it has WFIGHTMN_RECOVERY_ITEM in one of its last two equipment slots */
void WFIGHTMN_checkEquip(s32 member) {
    s32 partner = GAME.funcs.getPartyMember(member);
    s16 *equip;
    s32 i;

    if (partner >= 0) {
        equip = &GAME.funcs.getPartnerStats(partner)->equip[4];
        for (i = 0; i < 2; i++) {
            if (equip[i] == WFIGHTMN_RECOVERY_ITEM) {
                FIGHTSTG_queueRecovery(0, member, 0);
                return;
            }
        }
    }
}

/* Checks each of the party's three partners for WFIGHTMN_RECOVERY_ITEM
   (WFIGHTMN_checkEquip) */
void WFIGHTMN_checkParty(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        WFIGHTMN_checkEquip(i);
    }
}

/* Marks in BATTLE_RESULT that the current partner fought, and with which of
   its slots' Digimon if it isn't in its own form */
void WFIGHTMN_markFought(void) {
    s16 slots[4];
    DigimonData *digimon;
    s32 partner;
    BattleFighter *unit;
    s32 i;

    BATTLE_RESULT.partners[FIGHTSTG_battle.state.active[0]].fought = 1;
    partner = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
    unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    digimon = &DIGIMON_DATA[partner];
    if (digimon->id != unit->id && GAME.funcs.getPartnerSlots(partner, slots) > 0) {
        for (i = 0; i < 3; i++) {
            if (slots[i] == unit->id) {
                BATTLE_RESULT.partners[FIGHTSTG_battle.state.active[0]].used[i] = 1;
                return;
            }
        }
    }
}

/* Records in BATTLE_RESULT that the partner of the player's turn fought, and
   which of its digivolution slots it fought as */
void WFIGHTMN_markPicked(BattleMenu *task, BattleMenuChildren *children) {
    s16 slots[4];
    s32 i;
    s32 member;
    s32 partner;

    i = 0;
    member = -1;
    partner = GAME.funcs.getPartyMember(children->commands->arg);
    for (; i < 3; i++) {
        if (GAME.funcs.getPartyMember(i) == partner) {
            member = i;
            break;
        }
    }
    BATTLE_RESULT.partners[member].fought = 1;
    if (GAME.funcs.getPartnerSlots(GAME.funcs.getPartyMember(member), slots) > 0) {
        for (i = 0; i < 3; i++) {
            if (slots[i] == children->commands->digimon) {
                BATTLE_RESULT.partners[member].used[i] = 1;
                return;
            }
        }
    }
}
