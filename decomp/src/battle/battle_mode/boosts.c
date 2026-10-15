/* FIGHTSTG's stat boosts, the technique gauge's gain and the techniques' MP
   cost. */

#include "battle/battle_mode.h"

/* the stat of BattleStats.stats that each of BattleFighter.boosts adds to */
s16 FIGHTSTG_boostStats[] = {
    0x0000, 0x0001, 0x0004, 0x0000,
};

/* Changes a fighter's stat boost by percent 128ths of the stat without it,
   between minus half the stat and the stat; nothing for an empty or
   knocked-out fighter */
void FIGHTSTG_changeBoost(u8 side, s32 index, s32 stat, s32 percent) {
    BattleStats *stats;
    BattleFighter *fighter;
    s32 value;
    s32 min;

    if (side == 0) {
        stats = FIGHTSTG_computeStats(SIDE_PLAYER, 0, index);
        fighter = &FIGHTSTG_battle.state.fighters[0][index];
    } else {
        stats = FIGHTSTG_computeStats(SIDE_ENEMY, 0, index);
        fighter = &FIGHTSTG_battle.state.fighters[1][index];
    }
    if (fighter->id == 0 || fighter->hp == 0) {
        return;
    }
    if (fighter->boosts[stat] != 0) {
        stats->stats[FIGHTSTG_boostStats[stat]] -= fighter->boosts[stat];
    }
    value = stats->stats[FIGHTSTG_boostStats[stat]];
    min = -(value / 2);
    fighter->boosts[stat] += value * percent / 128;
    if (fighter->boosts[stat] < min) {
        fighter->boosts[stat] = min;
    }
    if (fighter->boosts[stat] > value) {
        fighter->boosts[stat] = value;
    }
}

/* What the partner's damage adds to the gauge: the square of its percent of
   the max HP over 20, more with accessories 0x149 and 0x14A, at most 1000 */
s32 FIGHTSTG_getGaugeGain(s32 damage) {
    PartnerStats *partner;
    s32 ratio;
    BattleFighter *fighter;
    s32 value;
    s32 i;
    s16 *acc;

    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    ratio = damage * 100 / fighter->maxHp;
    value = ratio * ratio / 20;
    partner = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]));
    acc = &partner->equip[4];
    for (i = 0; i < 2; i++) {
        if (acc[i] == 0x149) {
            value += value / 5;
        } else if (acc[i] == 0x14A) {
            value += value * 4 / 10;
        }
    }
    if (value > 1000) {
        value = 1000;
    }
    return value;
}

/* A technique's MP cost; accessories 0x143 and 0x144 cut the player's, to no
   less than 1 */
s32 FIGHTSTG_getTechCost(u8 side, s32 id) {
    PartnerStats *partner;
    s32 cost;
    s32 extra;
    s16 item;

    cost = TECHS[(id & 0x1FFF) - 1].mp;
    if (side != 0) {
        return cost;
    }
    partner = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]));
    item = 0;
    if (partner->equip[4] == 0x143 || partner->equip[4] == 0x144) {
        item = partner->equip[4];
    }
    if (partner->equip[5] == 0x143 || partner->equip[5] == 0x144) {
        item = partner->equip[5];
    }
    if (item != 0) {
        /* the game reads this amount signed */
        cost -= (s16)ITEM_FUNCS.get(item)->data.acc->amount;
        if (cost <= 0) {
            cost = 1;
        }
    }
    if (id & 0x4000) {
        extra = cost / 5;
        if (extra != 0) {
            cost += extra;
        } else {
            cost++;
        }
    }
    return cost;
}
