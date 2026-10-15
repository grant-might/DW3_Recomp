/* FIGHTSTG's items used in battle: an item's script and its task. */

#include "battle/battle_mode.h"

/* Item 0x58's setup: lowers the higher of the enemy's attack and defense
   (when that is the defense, the USA version raises it instead), unless the
   battle blocks it, and picks the script's effect */
static inline void lowerEnemyStat(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleStats *stats;
    s32 atk;
    s32 def;
    s32 min;

    fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
    stats = FIGHTSTG_battleFuncs.computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
    if (stats->stats[BATTLE_STAT_ATTACK] > stats->stats[BATTLE_STAT_DEFENSE]) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_LOWER_ATTACK] == 0) {
            task->lowered = 1;
            atk = stats->stats[BATTLE_STAT_ATTACK];
            def = stats->stats[BATTLE_STAT_DEFENSE];
            if (fighter->boosts[0] != 0) {
                stats->stats[BATTLE_STAT_ATTACK] -= fighter->boosts[0];
            }
            min = -(stats->stats[BATTLE_STAT_ATTACK] / 2);
            fighter->boosts[0] -= atk - def;
            if (fighter->boosts[0] < min) {
                fighter->boosts[0] = min;
            }
        }
    } else if (stats->stats[BATTLE_STAT_ATTACK] < stats->stats[BATTLE_STAT_DEFENSE]) {
        if (BATTLE_SETUP.blocks[BATTLE_BLOCK_LOWER_DEFENSE] == 0) {
            task->lowered = 2;
            atk = stats->stats[BATTLE_STAT_ATTACK];
            def = stats->stats[BATTLE_STAT_DEFENSE];
            if (fighter->boosts[1] != 0) {
                stats->stats[BATTLE_STAT_DEFENSE] -= fighter->boosts[1];
            }
            min = -(stats->stats[BATTLE_STAT_DEFENSE] / 2);
#if VERSION_EU || VERSION_JP
            fighter->boosts[1] -= def - atk;
#else
            fighter->boosts[1] -= atk - def; /* raises it */
#endif
            if (fighter->boosts[1] < min) {
                fighter->boosts[1] = min;
            }
        }
    }
    children[0].script->index = 0x11;
    children[0].script->stage = -1;
    if (task->lowered == 1) {
        children[0].script->effect = 0x25;
        children[0].script->sound = 0x31;
    } else if (task->lowered == 2) {
        children[0].script->effect = 0x27;
        children[0].script->sound = 0x31;
    } else {
        children[0].script->effect = 0x2E;
        children[0].script->sound = 0x1E;
    }
}

/* Item 0x55's setup: its drain of 20 percent of the enemy's max HP, unless
   the battle blocks draining */
static inline void setUpDrain(BattleItem *task, BattleChild *children) {
    BattleFighter *enemy;

    children[0].script->index = 0xE;
    children[0].script->stage = -1;
    children[0].script->effect = 0x1C;
    children[0].script->sound = 0x27;
    if (BATTLE_SETUP.blocks[BATTLE_BLOCK_DRAIN] == 0) {
        enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        task->damage = enemy->maxHp * 20 / 100;
        if (enemy->hp - task->damage <= 0) {
            children[0].script->hits[3] = 2;
            task->counter = 1;
        } else {
            children[0].script->hits[3] = 1;
            WFIGHTMN_setIdleMotion(SIDE_ENEMY, task->damage);
            WFIGHTMN_setIdleMotion(0, -task->damage);
        }
    } else {
        children[0].script->hits[3] = 3;
    }
}

/* Item 0x5A's setup: the script of technique 0x89 and its magic damage to
   the enemy */
static inline void setUpItemTech(BattleItem *task, BattleChild *children) {
    BattleFighter *row;
    TechData *tech;

    children[0].script->index = 0xE;
    tech = &TECHS[0x88];
    children[0].script->stage = tech->scriptStage;
    children[0].script->effect = tech->scriptEffect;
    children[0].script->sound = tech->scriptSound;
    task->damage = WFIGHTMN_limitDamage(0, FIGHTSTG_battleFuncs.computeMagicDamage(0, 0x89), 0);
    row = FIGHTSTG_battle.state.fighters[1];
    if (task->damage > 0) {
        if (row[FIGHTSTG_battle.state.active[1]].hp - task->damage <= 0) {
            children[0].script->hits[3] = 2;
        } else {
            children[0].script->hits[3] = 1;
            WFIGHTMN_setIdleMotion(SIDE_ENEMY, task->damage);
        }
    } else {
        children[0].script->hits[3] = 3;
    }
}

/* The other items' setup: their effect and sound from FIGHTSTG_itemScripts,
   and the partner's idle motion after what items 0x2B-0x2E and 0x47 heal */
static inline void setItemScript(BattleItem *task, BattleChild *children) {
    s32 i;

    children[0].script->index = 0xA;
    children[0].script->stage = -1;
    for (i = 0; FIGHTSTG_itemScripts[i].item != -1; i++) {
        if (FIGHTSTG_itemScripts[i].item == task->item) {
            children[0].script->effect = FIGHTSTG_itemScripts[i].effect;
            children[0].script->sound = FIGHTSTG_itemScripts[i].sound;
            break;
        }
    }
    if (task->item >= 0x2B && task->item < 0x2F) {
        WFIGHTMN_setIdleMotion(0, -ITEM_FUNCS.get(task->item)->data.effect->amount);
    } else if (task->item == 0x47) {
        WFIGHTMN_setIdleMotion(0, -((FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0])->maxHp >> 1));
    }
}

/* An item's script (FIGHTSTG_updateItem): step 0 sets it up and deals the
   damage (lowerEnemyStat, setUpDrain, setUpItemTech, setItemScript), 1 plays
   item 0x55's second part when the enemy is still standing, 2 waits.
   The match depends on item 0x55's damage being 20 percent, on the row
   pointer of item 0x5A, on the stage being written out in each case and on
   the table being walked by index. */
s32 FIGHTSTG_runItemScript(BattleItem *task, BattleChild *children) {
    switch (task->step) {
    case 0:
    default:
        children[0].script = FIGHTSTG_createBattleScript();
        children[0].script->enemy = 0;
        switch (task->item) {
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
            children[0].script->index = 0xF;
            children[0].script->stage = (task->item - 0x4D) * 3 + 0x22;
            children[0].script->effect = 2;
            children[0].script->sound = 0x39;
            break;
        case 0x54:
            task->element = RANDOM.next() % ELEMENT_COUNT + ELEMENT_FIRST;
            children[0].script->index = 0xF;
            children[0].script->stage = (task->element - ELEMENT_FIRST) * 3 + 0x22;
            children[0].script->effect = 2;
            children[0].script->sound = 0x39;
            break;
        case 0x56:
            children[0].script->index = 0x11;
            children[0].script->stage = -1;
            children[0].script->effect = 0x15;
            children[0].script->sound = 0x1B;
            break;
        case 0x57:
            children[0].script->index = 0x11;
            children[0].script->stage = -1;
            children[0].script->effect = 0x27;
            children[0].script->sound = 0x31;
            break;
        case 0x59:
            children[0].script->index = 0x11;
            children[0].script->stage = -1;
            children[0].script->effect = 0x29;
            children[0].script->sound = 0x31;
            break;
        case 0x58:
            lowerEnemyStat(task, children);
            break;
        case 0x55:
            setUpDrain(task, children);
            break;
        case 0x5A:
            setUpItemTech(task, children);
            break;
        default:
            setItemScript(task, children);
            break;
        }
        task->step++;
        if (FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
            if (task->item == 0x5A) {
                WFIGHTMN_countHit(0, task->damage);
            } else {
                WFIGHTMN_endWeakness(0);
            }
        }
        break;
    case 1:
        if (children[0].script == NULL) {
            if (task->item != 0x55 || task->counter != 0 || BATTLE_SETUP.blocks[BATTLE_BLOCK_DRAIN] != 0) {
                return 1;
            }
            children[0].script = FIGHTSTG_createBattleScript();
            children[0].script->enemy = 0;
            children[0].script->index = 0xA;
            children[0].script->effect = 0x21;
            children[0].script->sound = 0x1F;
            task->step++;
        }
        break;
    case 2:
        if (children[0].script == NULL) {
            return 1;
        }
        break;
    }
    return 0;
}

/* what items 0x42-0x45 cure (FIGHTSTG_updateItem, below) */
StatusCure FIGHTSTG_itemCures[4] = {
    { 0x28, 0x01, 190 },
    { 0x29, 0x02, 192 },
    { 0x2A, 0x04, 194 },
    { 0x2C, 0x3F, 196 },
};
/* the items' script settings (FIGHTSTG_runItemScript, above) */
ItemScript FIGHTSTG_itemScripts[] = {
    { 0x2B, 0x21, 0x1F },
    { 0x2C, 0x21, 0x1F },
    { 0x2D, 0x21, 0x1F },
    { 0x2E, 0x21, 0x1F },
    { 0x42, 0x22, 0x1F },
    { 0x43, 0x22, 0x1F },
    { 0x44, 0x22, 0x1F },
    { 0x45, 0x22, 0x1F },
    { 0x46, 0x23, 0x1F },
    { 0x47, 0x21, 0x1F },
    { 0x48, 0x28, 0x1F },
    { 0x49, 0x24, 0x1F },
    { 0x4A, 0x26, 0x1F },
    { 0x4B, 0x2E, 0x1E },
    { 0x4C, 0x2E, 0x1E },
    { -1, 0, 0 },
};

/* Items 0x2B-0x41: heal the partner by the item's amount, up to its max HP */
static inline void healPartner(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    ItemEffect *effect;
    s32 heal;

    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    effect = ITEM_FUNCS.get(task->item)->data.effect;
    if (fighter->maxHp == fighter->hp) {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
        task->nextSubstate(task);
        return;
    }
    if (fighter->hp + effect->amount <= fighter->maxHp) {
        heal = effect->amount;
        fighter->hp += heal;
    } else {
        heal = fighter->maxHp - fighter->hp;
        fighter->hp = fighter->maxHp;
    }
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0;
    task->lines[1] = heal;
    children[0].message->show(children[0].message, 8, task->lines);
    task->nextSubstate(task);
}

/* Items 0x42-0x45: cure the partner of what FIGHTSTG_itemCures says, or say
   they do nothing */
static inline void cureStatus(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    StatusCure *cure;

    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    switch (task->item) {
    case 0x42:
    default:
        cure = &FIGHTSTG_itemCures[0];
        break;
    case 0x43:
        cure = &FIGHTSTG_itemCures[1];
        break;
    case 0x44:
        cure = &FIGHTSTG_itemCures[2];
        break;
    case 0x45:
        cure = &FIGHTSTG_itemCures[3];
        break;
    }
    children[0].message = FIGHTSTG_createMessage();
    if (fighter->flags & cure->flag) {
        fighter->flags &= ~cure->flag;
        FIGHTSTG_events.funcs.cureStatus(0, FIGHTSTG_battle.state.active[0], cure->item);
        task->lines[0] = cure->message;
        task->lines[1] = 0;
        children[0].message->show(children[0].message, 2, task->lines);
    } else {
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
    }
    task->nextSubstate(task);
}

/* Item 0x46: revives the partners knocked out to their max HP, then is taken
   from the bag and ends the task */
static inline void reviveParty(BattleItem *task, BattleChild *children) {
    s32 i;

    switch (task->step) {
    case 0:
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0;
        task->lines[1] = 5;
        children[0].message->show(children[0].message, 9, task->lines);
        task->nextStep(task);
        break;
    case 1:
        if (children[0].task == NULL) {
            for (i = 0; i < 3; i++) {
                if (FIGHTSTG_battle.state.fighters[0][i].hp == 0) {
                    FIGHTSTG_battle.state.fighters[0][i].hp = FIGHTSTG_battle.state.fighters[0][i].maxHp;
                    WFIGHTMN_checkEquip(i);
                }
            }
            task->nextStep(task);
        }
        break;
    case 2:
        if (children[0].task == NULL) {
            GAME.items[task->item]--;
            task->state = TASK_KILL;
        }
        break;
    }
}

/* Item 0x47: restores half the partner's max HP and MP, or says it does
   nothing when both are full */
static inline void restoreHalfHpMp(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;

    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    children[0].message = FIGHTSTG_createMessage();
    if (fighter->hp != fighter->maxHp || fighter->mp != fighter->maxMp) {
        s32 half;
        s32 halfMp;

        half = fighter->maxHp / 2;
        halfMp = fighter->maxMp / 2;
        if (fighter->hp + half >= fighter->maxHp) {
            fighter->hp = fighter->maxHp;
        } else {
            fighter->hp += half;
        }
        if (fighter->maxMp <= fighter->mp + halfMp) {
            fighter->mp = fighter->maxMp;
        } else {
            fighter->mp += halfMp;
        }
        task->lines[0] = 0x3B;
        task->lines[1] = 0;
        children[0].message->show(children[0].message, 2, task->lines);
    } else {
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
    }
    task->nextSubstate(task);
}

/* Item 0x48: raises the partner's speed by the item's amount in 128ths, up to
   double */
static inline void boostSpeed(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleStats *stats;
    ItemEffect *effect;
    s32 max;

    effect = ITEM_FUNCS.get(task->item)->data.effect;
    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    stats = FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
    if (fighter->boosts[2] != 0) {
        stats->stats[BATTLE_STAT_SPEED] -= fighter->boosts[2];
    }
    max = stats->stats[BATTLE_STAT_SPEED];
    fighter->boosts[2] += max * effect->amount / 128;
    if (fighter->boosts[2] > max) {
        fighter->boosts[2] = max;
    }
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, FIGHTSTG_battle.state.active[0], 2, 0);
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x32;
    task->lines[1] = 0;
    task->lines[2] = FIGHTSTG_battle.state.active[0];
    children[0].message->show(children[0].message, 7, task->lines);
    task->nextSubstate(task);
}

/* Item 0x49: raises the partner's attack and lowers its defense */
static inline void boostAttack(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleStats *stats;
    ItemEffect *effect;
    s32 max;
    s32 min;

    effect = ITEM_FUNCS.get(task->item)->data.effect;
    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    stats = FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
    if (fighter->boosts[0] != 0) {
        stats->stats[BATTLE_STAT_ATTACK] -= fighter->boosts[0];
    }
    if (fighter->boosts[1] != 0) {
        stats->stats[BATTLE_STAT_DEFENSE] -= fighter->boosts[1];
    }
    max = stats->stats[BATTLE_STAT_ATTACK];
    fighter->boosts[0] += max * effect->amount / 128;
    if (fighter->boosts[0] > max) {
        fighter->boosts[0] = max;
    }
    min = -(stats->stats[BATTLE_STAT_DEFENSE] / 2);
    fighter->boosts[1] -= stats->stats[BATTLE_STAT_DEFENSE] * effect->amount / 512;
    if (fighter->boosts[1] < min) {
        fighter->boosts[1] = min;
    }
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, FIGHTSTG_battle.state.active[0], 0, 0);
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, FIGHTSTG_battle.state.active[0], 1, 0);
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x3C;
    task->lines[1] = 0;
    children[0].message->show(children[0].message, 2, task->lines);
    task->nextSubstate(task);
}

/* Item 0x4A: raises the partner's defense and lowers its attack */
static inline void boostDefense(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleStats *stats;
    ItemEffect *effect;
    s32 max;
    s32 min;

    effect = ITEM_FUNCS.get(task->item)->data.effect;
    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    stats = FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
    if (fighter->boosts[0] != 0) {
        stats->stats[BATTLE_STAT_ATTACK] -= fighter->boosts[0];
    }
    if (fighter->boosts[1] != 0) {
        stats->stats[BATTLE_STAT_DEFENSE] -= fighter->boosts[1];
    }
    max = stats->stats[BATTLE_STAT_DEFENSE];
    fighter->boosts[1] += max * effect->amount / 128;
    if (fighter->boosts[1] > max) {
        fighter->boosts[1] = max;
    }
    min = -(stats->stats[BATTLE_STAT_ATTACK] / 2);
    fighter->boosts[0] -= stats->stats[BATTLE_STAT_ATTACK] * effect->amount / 512;
    if (fighter->boosts[0] < min) {
        fighter->boosts[0] = min;
    }
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, FIGHTSTG_battle.state.active[0], 0, 0);
    FIGHTSTG_queueBoostEnd(SIDE_PLAYER, FIGHTSTG_battle.state.active[0], 1, 0);
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x3D;
    task->lines[1] = 0;
    children[0].message->show(children[0].message, 2, task->lines);
    task->nextSubstate(task);
}

/* Item 0x4B: puts the partner in its special state, which doubles its
   effects */
static inline void startSpecial(BattleItem *task, BattleChild *children) {
    (FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0])->special = 1;
#if VERSION_US
    FIGHTSTG_queueSpecialEnd();
#endif
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x3E;
    task->lines[1] = 0;
    children[0].message->show(children[0].message, 2, task->lines);
    task->nextSubstate(task);
}

/* Item 0x4C: fills the partner's gauge by the item's amount, up to 999 */
static inline void fillGauge(BattleItem *task, BattleChild *children) {
    ItemEffect *effect;
    s32 i;

    effect = ITEM_FUNCS.get(task->item)->data.effect;
    i = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
    BATTLE_SETUP.gauges[i] += effect->amount;
    if (BATTLE_SETUP.gauges[i] >= 999) {
        BATTLE_SETUP.gauges[i] = 999;
    }
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x3F;
    task->lines[1] = 0;
    children[0].message->show(children[0].message, 2, task->lines);
    task->nextSubstate(task);
}

/* Items 0x4D-0x53: boost one element on the field */
static inline void boostElement(BattleItem *task, BattleChild *children) {
    FIGHTSTG_queueClearField(FIGHTSTG_events.funcs.getDelay(0, 8));
    FIGHTSTG_battle.state.boost.element = task->item - 0x4B;
    FIGHTSTG_battle.state.boost.amount = 0x40;
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = task->item + 0x16;
    children[0].message->show(children[0].message, 1, task->lines);
    task->nextSubstate(task);
}

/* Item 0x54: boosts the element its script picked at random on the field */
static inline void boostRandomElement(BattleItem *task, BattleChild *children) {
    FIGHTSTG_queueClearField(FIGHTSTG_events.funcs.getDelay(0, 8));
    FIGHTSTG_battle.state.boost.element = task->element;
    FIGHTSTG_battle.state.boost.amount = 0x7F;
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = task->element + 0x61;
    children[0].message->show(children[0].message, 1, task->lines);
    task->nextSubstate(task);
}

/* Item 0x55: drains the damage from the enemy to the partner, or says it
   does nothing when the battle blocks draining */
static inline void drainEnemy(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;

    if (BATTLE_SETUP.blocks[BATTLE_BLOCK_DRAIN] == 0) {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x10;
        task->lines[1] = task->damage;
        fighter = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        if (fighter->hp - task->damage <= 0) {
            children[0].message->show(children[0].message, 4, task->lines);
            fighter->hp = 0;
            FIGHTSTG_queueKnockOut(SIDE_ENEMY);
        } else {
            children[0].message->show(children[0].message, 0x14, task->lines);
            fighter->hp -= task->damage;
            fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
            fighter->hp += task->damage;
            if (fighter->hp > fighter->maxHp) {
                fighter->hp = fighter->maxHp;
            }
        }
    } else {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
    }
    task->nextSubstate(task);
}

/* Item 0x56: confuses the enemy half of the time, unless the battle blocks
   it, and then the partner a quarter of those times */
static inline void confuseFighters(BattleItem *task, BattleChild *children) {
    BattleFighter *fighter;
    BattleFighter *enemy;
    ItemEffect *effect;

    effect = ITEM_FUNCS.get(task->item)->data.effect;
    fighter = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
    switch (task->step) {
    case 0:
    default:
        if ((RANDOM.next() & 1) && BATTLE_SETUP.blocks[BATTLE_BLOCK_CONFUSION] == 0) {
            FIGHTSTG_inflictConfusion(0x10, 0, effect->amount);
            children[0].message = FIGHTSTG_createMessage();
            task->lines[0] = 0x20;
            task->lines[1] = 0x10;
            children[0].message->show(children[0].message, 2, task->lines);
            if ((RANDOM.next() & 3) == 0) {
                task->nextStep(task);
            } else {
                task->nextSubstate(task);
            }
            enemy->flags |= FIGHTER_CONFUSED;
        } else {
            children[0].message = FIGHTSTG_createMessage();
            task->lines[0] = 0x2F;
            children[0].message->show(children[0].message, 1, task->lines);
            task->nextSubstate(task);
        }
        break;
    case 1:
        if (children[0].task == NULL) {
            fighter->flags |= FIGHTER_CONFUSED;
            FIGHTSTG_inflictConfusion(0, 0, effect->amount);
            children[0].message = FIGHTSTG_createMessage();
            task->lines[0] = 0x20;
            task->lines[1] = 0;
            children[0].message->show(children[0].message, 2, task->lines);
            task->nextSubstate(task);
        }
        break;
    }
}

/* Item 0x57: raises the enemy's attack and lowers its defense, or says it
   does nothing when the battle blocks it */
static inline void weakenEnemyDefense(BattleItem *task, BattleChild *children) {
    BattleFighter *enemy;
    BattleStats *stats;
    s32 max;
    s32 atk;
    s32 def;

    if (BATTLE_SETUP.blocks[BATTLE_BLOCK_LOWER_DEFENSE] == 0) {
        enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        stats = FIGHTSTG_battleFuncs.computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
        atk = stats->stats[BATTLE_STAT_ATTACK];
        def = stats->stats[BATTLE_STAT_DEFENSE];
        if (enemy->boosts[0] != 0) {
            stats->stats[BATTLE_STAT_ATTACK] -= enemy->boosts[0];
        }
        if (enemy->boosts[1] != 0) {
            stats->stats[BATTLE_STAT_DEFENSE] -= enemy->boosts[1];
        }
        enemy->boosts[0] += atk * 3 / 10;
        enemy->boosts[1] -= def / 2;
        max = stats->stats[BATTLE_STAT_ATTACK];
        if (enemy->boosts[0] > max) {
            enemy->boosts[0] = max;
        }
        if (enemy->boosts[1] < -stats->stats[BATTLE_STAT_DEFENSE] / 2) {
            enemy->boosts[1] = -stats->stats[BATTLE_STAT_DEFENSE] / 2;
        }
        FIGHTSTG_queueBoostEnd(SIDE_ENEMY, FIGHTSTG_battle.state.active[1], 0, 0);
        FIGHTSTG_queueBoostEnd(SIDE_ENEMY, FIGHTSTG_battle.state.active[1], 1, 0);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x40;
        task->lines[1] = 0x10;
        children[0].message->show(children[0].message, 2, task->lines);
        task->nextSubstate(task);
        return;
    }
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x2F;
    children[0].message->show(children[0].message, 1, task->lines);
    task->nextSubstate(task);
}

/* Item 0x58: says which of the enemy's stats its script lowered */
static inline void showLoweredStat(BattleItem *task, BattleChild *children) {
    switch (task->lowered) {
    case 1:
        FIGHTSTG_queueBoostEnd(SIDE_ENEMY, FIGHTSTG_battle.state.active[1], 0, 0);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x33;
        task->lines[1] = 0x10;
        task->lines[2] = FIGHTSTG_battle.state.active[1];
        children[0].message->show(children[0].message, 2, task->lines);
        break;
    case 2:
        FIGHTSTG_queueBoostEnd(SIDE_ENEMY, FIGHTSTG_battle.state.active[1], 1, 0);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x34;
        task->lines[1] = 0x10;
        task->lines[2] = FIGHTSTG_battle.state.active[1];
        children[0].message->show(children[0].message, 2, task->lines);
        break;
    default:
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x2F;
        children[0].message->show(children[0].message, 1, task->lines);
        break;
    }
    task->nextSubstate(task);
}

/* Item 0x59: lowers the enemy's speed, or says it does nothing when the
   battle blocks it */
static inline void lowerEnemySpeed(BattleItem *task, BattleChild *children) {
    BattleFighter *enemy;
    BattleStats *stats;
    ItemEffect *effect;
    s32 min;

    if (BATTLE_SETUP.blocks[BATTLE_BLOCK_LOWER_SPEED] == 0) {
        effect = ITEM_FUNCS.get(task->item)->data.effect;
        enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        stats = FIGHTSTG_battleFuncs.computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1]);
        if (enemy->boosts[2] != 0) {
            stats->stats[BATTLE_STAT_SPEED] -= enemy->boosts[2];
        }
        min = -(stats->stats[BATTLE_STAT_SPEED] / 2);
        enemy->boosts[2] -= stats->stats[BATTLE_STAT_SPEED] * effect->amount / 128;
        if (enemy->boosts[2] < min) {
            enemy->boosts[2] = min;
        }
        FIGHTSTG_queueBoostEnd(SIDE_ENEMY, FIGHTSTG_battle.state.active[1], 2, 0);
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x35;
        task->lines[1] = 0x10;
        task->lines[2] = FIGHTSTG_battle.state.active[1];
        children[0].message->show(children[0].message, 2, task->lines);
        task->nextSubstate(task);
        return;
    }
    children[0].message = FIGHTSTG_createMessage();
    task->lines[0] = 0x2F;
    children[0].message->show(children[0].message, 1, task->lines);
    task->nextSubstate(task);
}

/* Item 0x5A: deals its technique's damage to the enemy */
static inline void damageEnemy(BattleItem *task, BattleChild *children) {
    BattleFighter *enemy;

    if (task->damage > 0) {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x10;
        task->lines[1] = task->damage;
        children[0].message->show(children[0].message, 4, task->lines);
        enemy = &FIGHTSTG_battle.state.fighters[1][FIGHTSTG_battle.state.active[1]];
        if (enemy->hp - task->damage <= 0) {
            enemy->hp = 0;
            FIGHTSTG_queueKnockOut(SIDE_ENEMY);
        } else {
            enemy->hp -= task->damage;
        }
    } else {
        children[0].message = FIGHTSTG_createMessage();
        task->lines[0] = 0x1D;
        task->lines[1] = 0x10;
        children[0].message->show(children[0].message, 2, task->lines);
    }
    task->nextSubstate(task);
}

/* An item used in battle (FIGHTSTG_startItem): its message,
   FIGHTSTG_runItemScript's script, then substate 1 does what the item does
   (healPartner to damageEnemy, one per item) and 2 takes it from the bag
   (item 0x55 attacks again in BATTLE_KIND_FINAL_LAST). The match depends on
   each case's variables being its own, on item 0x47's halves being declared in
   its `if`, on its MP test being written `maxMp <= mp + halfMp`, on item
   0x57's cap being read after the boosts change, on the pointer sum of item
   0x4B and on the cases that say the item does nothing ending on their own
   (item 0x2B's, 0x57's and 0x59's). */
void FIGHTSTG_updateItem(BattleItem *task, BattleChild *children) {
    s8 unused[0x90]; /* unused, but it is in the original stack frame */

    switch (task->state) {
    case TASK_INIT:
    default:
        switch (task->substate) {
        case 0:
        default:
            children[0].message = FIGHTSTG_createMessage();
            task->lines[0] = 0;
            task->lines[1] = task->item;
            children[0].message->show(children[0].message, 0xE, task->lines);
            task->substate++;
            break;
        case 1:
            if (children[0].task == NULL && FIGHTSTG_runItemScript(task, children)) {
                task->nextState(task);
            }
            break;
        }
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            if (children[0].task == NULL) {
                task->substate++;
            }
            break;
        case 1:
            switch (task->item) {
            case 0x2B ... 0x41:
            default:
                healPartner(task, children);
                break;
            case 0x42 ... 0x45:
                cureStatus(task, children);
                break;
            case 0x46:
                reviveParty(task, children);
                break;
            case 0x47:
                restoreHalfHpMp(task, children);
                break;
            case 0x48:
                boostSpeed(task, children);
                break;
            case 0x49:
                boostAttack(task, children);
                break;
            case 0x4A:
                boostDefense(task, children);
                break;
            case 0x4B:
                startSpecial(task, children);
                break;
            case 0x4C:
                fillGauge(task, children);
                break;
            case 0x4D ... 0x53:
                boostElement(task, children);
                break;
            case 0x54:
                boostRandomElement(task, children);
                break;
            case 0x55:
                drainEnemy(task, children);
                break;
            case 0x56:
                confuseFighters(task, children);
                break;
            case 0x57:
                weakenEnemyDefense(task, children);
                break;
            case 0x58:
                showLoweredStat(task, children);
                break;
            case 0x59:
                lowerEnemySpeed(task, children);
                break;
            case 0x5A:
                damageEnemy(task, children);
                break;
            }
            break;
        case 2:
            if (children[0].task == NULL) {
                GAME.items[task->item]--;
                if (task->item == 0x55 && FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
                    children[0].enemyAttack = FIGHTSTG_startEnemyAttack(1, 0);
                    task->nextSubstate(task);
                } else {
                    task->state = TASK_KILL;
                }
            }
            break;
        case 3:
            if (children[0].task == NULL) {
                task->setState(task, TASK_KILL);
            }
            break;
        }
        break;
    case 2:
    case TASK_KILL:
        break;
    }
}

/* Starts an item's script (FIGHTSTG_updateItem) */
BattleItem *FIGHTSTG_startItem(s32 item) {
    BattleItem *task = createTask(FIGHTSTG_updateItem, sizeof(BattleItem), sizeof(Task *));

    task->item = item;
    return task;
}
