/* The battle menu: its task, the states that run the turn's events (the
   player's commands, the battle's end, recoveries, damage and statuses, the
   blast, knock-outs and the last enemy), its main state and WFIGHTMN's entry
   point. The states stay in one module: a cut between them moves a jump
   table off its 8-byte place. */

#include "battle/normal_battle.h"

/* Sets the battle's kind from the mode it came from and the battle: the
   escape, UNK2, no-damage and final battles, or a normal one */
static inline void setBattleKind(void) {
    s32 mode;

    mode = GAME.funcs.getPrevMode();
    if (mode == 0x22D && BATTLE_SETUP.battle == 0x143) {
        FIGHTSTG_battle.state.kind = BATTLE_KIND_ESCAPE;
    } else if (mode == 0x23A && BATTLE_SETUP.battle == 0xB) {
        FIGHTSTG_battle.state.kind = BATTLE_KIND_UNK2;
    } else if (mode == 0x272 && BATTLE_SETUP.battle == 0x1E) {
        FIGHTSTG_battle.state.kind = BATTLE_KIND_NO_DAMAGE;
    } else if (BATTLE_SETUP.battle == 0x144) {
        FIGHTSTG_battle.state.kind = BATTLE_KIND_FINAL;
        FIGHTSTG_battle.state.weakened = 0;
        FIGHTSTG_battle.state.hitCount = 0;
        FIGHTSTG_battle.state.tech = 0;
    } else {
        FIGHTSTG_battle.state.kind = BATTLE_KIND_NORMAL;
    }
}

/* Adds the partner's Digimon (its battle digivolution, but not after an
   ambush) and the enemy, the partner weak when its HP is low */
static inline void addFighters(BattleMenu *task, BattleMenuChildren *children) {
    s16 slots[4];
    s32 digimon;
    s32 partner;
    BattleFighter *unit;

    partner = GAME.funcs.getPartyMember(0);
    if (WFIGHTMN_rollAmbush() != 0) {
        digimon = DIGIMON_DATA[partner].id;
        task->counter = 1;
    } else if (GAME.funcs.getPartnerSlots(partner, slots) > 0 && GAME.partners[partner].battleDigivolve != 0) {
        digimon = GAME.partners[partner].battleDigivolve;
    } else {
        digimon = DIGIMON_DATA[partner].id;
    }
    children->models->add(children->models, 0, digimon, 1);
    children->models->face(children->models, 0);
    children->models->add(children->models, 0x10, BATTLE_SETUP.enemies[0].fighter, 1);
    children->models->face(children->models, 0x10);
    WFIGHTMN_initFighters(digimon);
    unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    if (unit->maxHp / 4 >= unit->hp) {
        children->models->setIdleMotion(children->models, 0, 1);
    }
}

/* Queues the first turns: the enemy's first after an ambush, with its
   message, or by a roll on the fighters' speeds */
static inline void queueFirstTurns(BattleMenu *task, BattleMenuChildren *children) {
    BattleStats *stats;
    s32 chance;

    if (task->counter != 0) {
        FIGHTSTG_queueEnemyTurn(0);
        FIGHTSTG_queuePlayerTurn(FIGHTSTG_events.funcs.getDelay(0, 0) / 2);
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = 7;
        children->task.message->show(children->task.message, 1, task->args);
        task->step++;
        FIGHTSTG_setPlayerTurnStep(1);
    } else {
        stats = FIGHTSTG_battleFuncs.computeStats(SIDE_PLAYER, 1, FIGHTSTG_battle.state.active[0]);
        chance = FIGHTSTG_battleFuncs.computeStats(SIDE_ENEMY, 0, FIGHTSTG_battle.state.active[1])->stats[BATTLE_STAT_SPEED] * 8 / stats->stats[BATTLE_STAT_SPEED];
        if (RANDOM.next() % 128 < chance) {
            FIGHTSTG_queueEnemyTurn(0);
            FIGHTSTG_queuePlayerTurn(FIGHTSTG_events.funcs.getDelay(0, 0) / 2);
            FIGHTSTG_setPlayerTurnStep(1);
        } else {
            FIGHTSTG_queuePlayerTurn(0);
            FIGHTSTG_queueEnemyTurn(FIGHTSTG_events.funcs.getDelay(0, 0) / 2);
        }
        task->nextState(task);
    }
}

/* Plays sound 0x60040000 once a partner is confused, and the battle's music
   again once none is */
static inline void switchConfusionMusic(BattleMenu *task) {
    BattleFighter *units;
    s32 found;
    s32 i;

    units = FIGHTSTG_battle.state.fighters[0];
    if (task->confusionSound == 0) {
        for (i = 0; i < 3; i++) {
            if (units[i].flags & FIGHTER_CONFUSED) {
                SOUND.playSound(0x60040000);
                task->confusionSound = 1;
                break;
            }
        }
    } else {
        found = 0;
        for (i = 0; i < 3; i++) {
            if (units[i].flags & FIGHTER_CONFUSED) {
                found = 1;
                break;
            }
        }
        if (!found) {
            SOUND.playSound(BATTLE_SETUP.music);
            task->confusionSound = 0;
        }
    }
}

/* The battle menu's task: sets up the battle (its music, FIGHTSTG's tasks,
   setBattleKind, the fighters' models in addFighters and queueFirstTurns),
   then runs the menu (WFIGHTMN_runTurn), playing sound 0x60040000 once a
   partner is confused and the battle's music again after
   (switchConfusionMusic) */
void WFIGHTMN_updateMenu(BattleMenu *task, BattleMenuChildren *children) {
    switch (task->state) {
    case 0:
    default:
        switch (task->substate) {
        case 0:
        default:
            WFIGHTMN_createLayers();
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            switch (task->step) {
            case 0:
            default:
                SOUND.loadBank((BATTLE_SETUP.music >> 18) & 0x7F);
                task->nextStep(task);
                /* fallthrough */
            case 1:
                if (SOUND.isLoading() == 0) {
                    SOUND.playSound(BATTLE_SETUP.music);
                    task->nextSubstate(task);
                }
                break;
            }
            break;
        case 2:
            switch (task->step) {
            case 0:
            default:
                children->commands = FIGHTSTG_startPlayerTurn();
                children->camera = FIGHTSTG_createBattleCamera(BATTLE_LAYER_CAMERA);
                children->stage = FIGHTSTG_createStage(BATTLE_SETUP.stage, 60);
                children->models = FIGHTSTG_createModels();
                children->lights = FIGHTSTG_createLights(BATTLE_LAYER_CAMERA);
                task->nextStep(task);
                break;
            case 1:
                setBattleKind();
                addFighters(task, children);
#if VERSION_EU || VERSION_JP
                GFX.frameTime = 1;
                children->cameraMove.cameraShots = FIGHTSTG_startCameraShots();
#else
                children->camera->set(children->camera, children->camera->getEnemyView(children->camera));
#endif
                task->step++;
                break;
            case 2:
#if VERSION_EU || VERSION_JP
                GFX.frameTime = 1;
#endif
                children->loader = WFIGHTMN_createLoader();
                task->step++;
                break;
            case 3:
                if (children->loader == NULL) {
                    task->step++;
                }
                break;
            case 4:
                WFIGHTMN_markFought();
                task->step++;
                break;
            case 5:
                queueFirstTurns(task, children);
                WFIGHTMN_checkParty();
                break;
            case 6:
                if (children->task.task == NULL) {
                    task->nextState(task);
                }
                break;
            }
            break;
        }
        break;
    case 1:
        WFIGHTMN_runTurn(task);
        switchConfusionMusic(task);
        break;
    case 2:
    case 3:
        break;
    }
}

/* Ends the current partner's temporary Digimon and the actions that go with it */
void WFIGHTMN_cancelBlast(void) {
    s32 index = FIGHTSTG_events.funcs.find(EVENT_BLAST_END, 0, FIGHTSTG_battle.state.active[0]);
    BattleFighter *unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
    s32 member = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);

    if (unit->temporary != 0) {
        if (index >= 0) {
            FIGHTSTG_events.events[index].type = 0;
        }
        BATTLE_SETUP.gauges[member] = 0;
        unit->temporary = 0;
        index = FIGHTSTG_events.funcs.find(EVENT_PARTNER_TECH, 0, FIGHTSTG_battle.state.active[0]);
        if (index >= 0) {
            FIGHTSTG_events.events[index].type = 0;
        }
    }
}

/* The player's turn (EVENT_PLAYER_TURN): drops the partner's queued technique
   and running away, then loses the turn to FIGHTER_PARALYZED, goes on to
   WFIGHTMN_runConfusedCommand when FIGHTER_CONFUSED, or lets the player pick
   a command and starts the state or the task that runs it */
void WFIGHTMN_runCommand(BattleMenu *task, BattleMenuChildren *children) {
    BattleFighter *unit;
    s32 index;

    switch (task->step) {
    case 0:
    default:
        index = FIGHTSTG_events.funcs.find(EVENT_PARTNER_TECH, 0, FIGHTSTG_battle.state.active[0]);
        if (index >= 0) {
            FIGHTSTG_events.events[index].type = 0;
        }
        index = FIGHTSTG_events.funcs.find(EVENT_RUN_AWAY, 0, FIGHTSTG_battle.state.active[0]);
        if (index >= 0) {
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = 0x42;
            task->args[1] = 0;
            children->task.message->show(children->task.message, 2, task->args);
            FIGHTSTG_events.events[index].type = 0;
        }
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
            if ((unit->flags & FIGHTER_PARALYZED) && FIGHTSTG_battleFuncs.testParalysis(0) != 0) {
                children->task.message = FIGHTSTG_createMessage();
                task->args[0] = 0x56;
                task->args[1] = 0;
                children->task.message->show(children->task.message, 2, task->args);
                FIGHTSTG_queuePlayerTurn(FIGHTSTG_events.funcs.getDelay(0, 0));
                task->setSubstate(task, 2);
            } else if (unit->flags & FIGHTER_CONFUSED) {
                task->setSubstate(task, 0x11);
            } else {
                FIGHTSTG_setPlayerTurnStep(2);
                task->step = 2;
            }
        }
        break;
    case 2:
        if (FIGHTSTG_isPlayerChoosing() == 0) {
            switch (children->commands->action) {
            case 1:
                FIGHTSTG_battle.state.runAttempts++;
                FIGHTSTG_queueRunAway(0);
                children->task.message = FIGHTSTG_createMessage();
                task->args[0] = 0x41;
                task->args[1] = 0;
                children->task.message->show(children->task.message, 2, task->args);
                task->setSubstate(task, 2);
                WFIGHTMN_endWeakness(0);
                break;
            case 0:
                children->task.attack = FIGHTSTG_startFirstTech(0);
                task->setSubstate(task, 2);
                break;
            case 4:
                children->task.tech = FIGHTSTG_startTechAction(0, children->commands->arg);
                task->setSubstate(task, 2);
                break;
            case 2:
                task->setSubstate(task, 4);
                WFIGHTMN_endWeakness(0);
                break;
            case 5:
                task->setSubstate(task, 5);
                WFIGHTMN_endWeakness(0);
                break;
            case 6:
                task->setSubstate(task, 6);
                break;
            case 3:
                children->task.item = FIGHTSTG_startItem(children->commands->arg);
                task->setSubstate(task, 2);
                break;
            default:
                task->substate = -1;
                break;
            }
            FIGHTSTG_queuePlayerTurn(FIGHTSTG_events.funcs.getDelay(0, 0));
        }
        break;
    }
}

/* The state that changes the current partner's Digimon to the one picked
   (commands->digimon), then says so */
void WFIGHTMN_digivolve(BattleMenu *task, BattleMenuChildren *children) {
    BattleFighter *unit;
    DigimonData *digimon;

    switch (task->step) {
    case 0:
    default:
        unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        unit->id = children->commands->digimon;
        WFIGHTMN_markFought();
        WFIGHTMN_cancelBlast();
        children->task.change = FIGHTSTG_startDigimonChange(unit->id, 0);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            /* a pointer sum, not units[0][...]: the match depends on it,
               which adds the base first */
            digimon = GET_DIGIMON((FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0])->id);
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = 0;
            task->args[1] = digimon->nameId;
            children->task.message->show(children->task.message, 0xC, task->args);
            task->setSubstate(task, 2);
        }
        break;
    }
}

/* Swaps the partner on the field for the one picked (children->commands):
   message 0x38, then the swap effect with current[0] set to the new partner's
   slot for it, then the switch itself and message 0x39 */
void WFIGHTMN_tag(BattleMenu *task, BattleMenuChildren *children) {
    s32 member;
    s32 slot;
    s32 newSlot;
    s32 i;
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = 0x38;
        task->args[1] = 0;
        children->task.message->show(children->task.message, 2, task->args);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            /* the match depends on setting i here, before the call */
            i = 0;
            slot = -1;
            member = GAME.funcs.getPartyMember(children->commands->arg);
            for (; i < 3; i++) {
                if (GAME.funcs.getPartyMember(i) == member) {
                    slot = i;
                    break;
                }
            }
            task->args[0] = slot;
            task->args[1] = FIGHTSTG_battle.state.active[0];
            FIGHTSTG_battle.state.active[0] = task->args[0];
            children->task.entrance = FIGHTSTG_startEntrance(children->commands->digimon, 0, WFIGHTMN_setIdleMotion(0, 0));
            FIGHTSTG_battle.state.active[0] = task->args[1];
            task->step++;
        }
        break;
    case 2:
        if (children->task.entrance->done != 0) {
            newSlot = task->args[0];
            unit = FIGHTSTG_battle.state.fighters[0] + FIGHTSTG_battle.state.active[0];
            if (unit->special != 0) {
                unit->special = 0;
#if VERSION_US
                /* FIGHTSTG_queueSpecialEnd's event, built in the arguments */
                task->args[0] = EVENT_SPECIAL_END;
                task->args[1] = 1;
                task->args[2] = 0;
                task->args[3] = FIGHTSTG_battle.state.active[0];
                FIGHTSTG_events.funcs.pushFirst((BattleEvent *)task->args);
#elif VERSION_EU || VERSION_JP
                FIGHTSTG_queueSpecialEnd();
#endif
            }
            WFIGHTMN_cancelBlast();
            FIGHTSTG_battle.state.active[0] = newSlot;
            unit = FIGHTSTG_battle.state.fighters[0] + newSlot;
            unit->id = children->commands->digimon;
            WFIGHTMN_markFought();
            task->step++;
        }
        break;
    case 3:
        if (children->task.task == NULL) {
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = 0x39;
            task->args[1] = 0;
            children->task.message->show(children->task.message, 2, task->args);
            task->setSubstate(task, 2);
        }
        break;
    }
}

/* State 6, the pair technique (the player turn's action 6): the outgoing
   and incoming partners do commands->pairTech, then the incoming one tags in
   (state 5, past its message) while an enemy is left; otherwise the turn ends
   with the partner marked as fought */
void WFIGHTMN_pairTech(BattleMenu *task, BattleMenuChildren *children) {
    s32 i;
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        children->task.tech = FIGHTSTG_startTechAction(0, children->commands->pairTech);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            for (i = 0, unit = FIGHTSTG_battle.state.fighters[1]; i < 3; i++, unit++) {
                if (unit->id != 0 && unit->hp != 0) {
                    task->setSubstate(task, 5);
                    task->step = 1;
                    return;
                }
            }
            WFIGHTMN_markPicked(task, children);
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* After a won battle, picks the enemy whose item the player may get; saves the
   partners' HP and MP, fades out and requests the next mode */
void WFIGHTMN_endBattle(BattleMenu *task, BattleMenuChildren *children) {
    Battle *battle;
    BattleFighter *units;
    BattleFighter *unit;
    BattleTableEntry *info;
    ScreenFade *end;
    Layer *layer;
    s32 count;
    s32 pick;
    s32 i;
    s32 partner;
    s32 member;
    s32 mode;

    switch (task->step) {
    case 0:
    default:
#if VERSION_EU || VERSION_JP
        if (children->cameraMove.task != NULL && children->cameraMove.task->state != TASK_DONE) {
            break;
        }
#endif
        if (FIGHTSTG_events.funcs.result != BATTLE_FLED) {
            count = 0;
            battle = &FIGHTSTG_battle;
            units = battle->state.fighters[1];
            BATTLE_RESULT.battle = BATTLE_SETUP.battle;
            BATTLE_RESULT.member = battle->state.active[0];
            for (i = 0; i < 3; i++) {
                if (units[i].id != 0 && units[i].item != 0) {
                    count++;
                }
            }
            pick = RANDOM.next() % count;
            count = 0;
            for (i = 0; i < 3; i++) {
                if (units[i].id != 0 && units[i].item != 0) {
                    if (pick == count) {
                        break;
                    }
                    count++;
                }
            }
            /* an address sum with the offset first: the match depends on it, which
               puts the offset first in the addu */
            unit = (BattleFighter *)(count * sizeof(BattleFighter) + (s32)units);
            info = FIGHTSTG_battleTableFunc(unit->id);
            if (unit->item > 0 && info->itemChance + 1 > RANDOM.next() % 1024) {
                BATTLE_RESULT.item = unit->item;
            } else {
                BATTLE_RESULT.item = 0;
            }
            if (BATTLE_SETUP.hasPrize == 1) {
                BATTLE_RESULT.item = BATTLE_SETUP.prize;
            }
        }
        for (member = 0; member < 3; member++) {
            partner = GAME.funcs.getPartyMember(member);
            if (partner >= 0) {
                if ((FIGHTSTG_battle.state.fighters[0] + member)->hp <= 0) {
                    GAME.partners[partner].info.stats[STAT_HP] = 1;
                    BATTLE_RESULT.partners[member].fought = 0;
                    BATTLE_RESULT.partners[member].used[0] = 0;
                    BATTLE_RESULT.partners[member].used[1] = 0;
                    BATTLE_RESULT.partners[member].used[2] = 0;
                } else {
                    GAME.partners[partner].info.stats[STAT_HP] = (FIGHTSTG_battle.state.fighters[0] + member)->hp;
                }
                GAME.partners[partner].info.stats[STAT_MP] = (FIGHTSTG_battle.state.fighters[0] + member)->mp;
            }
        }
        end = FIGHTSTG_createFader();
        children->task.fade = end;
        end->start(end, 0, 10);
        task->step++;
        break;
    case 1:
        if (children->task.task->state == TASK_DONE) {
            layer = GFX.funcs.getLayer(SCREEN_LAYER);
            layer->setBgColor(layer, 0, 0, 0);
#if VERSION_EU || VERSION_JP
            if (children->cameraMove.task != NULL) {
                children->cameraMove.task->setState(children->cameraMove.task, TASK_KILL);
            }
#endif
            task->step++;
        }
        break;
    case 2:
        WFIGHTMN_cancelBlast();
        switch (FIGHTSTG_events.funcs.result) {
        case BATTLE_FLED:
        default:
            GAME.funcs.requestMode(GAME.fieldMode, 0);
            break;
        case BATTLE_WON:
            mode = MODE_ENDING;
            if (FIGHTSTG_battle.state.kind != BATTLE_KIND_FINAL_LAST) {
                mode = MODE_BATTLE_REPORT;
            }
            GAME.funcs.requestMode(mode, 0);
            break;
        case BATTLE_LOST:
            GAME.funcs.requestMode(MODE_TITLE, 0);
            break;
        }
        break;
    }
}

/* Asks FIGHTSTG (FIGHTSTG_battleFuncs.testRunAway) whether the current action's actor gets away; if
   so shows message 0x43 (a partner) or 0x5F (an enemy) and waits for the
   window to close, otherwise sets the event's time from getDelay */
void WFIGHTMN_runAway(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action;

    switch (task->counter) {
    case 0:
    default:
        action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
        if (FIGHTSTG_battleFuncs.testRunAway(action->args[0]) != 0) {
            task->counter = 1;
            action->type = 0;
            children->task.message = FIGHTSTG_createMessage();
            /* the match depends on args[1] being set in both branches */
            if (action->args[0] == 0) {
                task->args[0] = 0x43;
                task->args[1] = action->args[0];
            } else {
                task->args[0] = 0x5F;
                task->args[1] = action->args[0];
            }
            children->task.message->show(children->task.message, 2, task->args);
        } else {
            action->time = FIGHTSTG_events.funcs.getDelay(action->args[0], 1);
            task->setSubstate(task, 0);
        }
        break;
    case 1:
        if (children->task.task == NULL) {
            FIGHTSTG_endBattle(BATTLE_FLED);
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* Removes the current action's EVENT_RECOVERY event, unless the partner it is for
   has WFIGHTMN_RECOVERY_ITEM equipped */
void WFIGHTMN_endAutoRecover(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    s32 index = FIGHTSTG_events.funcs.find(EVENT_RECOVERY, action->args[0], action->args[1]);
    QueuedEvent *found;
    PartnerStats *stats;

    if (index >= 0) {
        found = &FIGHTSTG_events.events[index];
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = 0x5A;
        task->args[1] = action->args[0];
        task->args[2] = action->args[1];
        children->task.message->show(children->task.message, 7, task->args);
        if (found->args[0] == 0) {
            stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(found->args[1]));
            if (stats->equip[4] == WFIGHTMN_RECOVERY_ITEM || stats->equip[5] == WFIGHTMN_RECOVERY_ITEM) {
                found->type = EVENT_RECOVERY;
                found->args[2] = 0;
            } else {
                found->type = 0;
            }
        } else {
            found->type = 0;
        }
    }
    task->setSubstate(task, 2);
}

/* A fighter's recovery (EVENT_RECOVERY): unless its HP is full, plays
   technique 0xBD on it when it is the side's active fighter, shows message
   0xA with the amount and adds it, up to the maximum */
void WFIGHTMN_recover(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    s32 side = action->args[0] >> 4;
    BattleFighter *unit = &FIGHTSTG_battle.state.fighters[side][action->args[1]];

    switch (task->step) {
    case 0:
    default:
        action->time = 1000;
        task->counter = FIGHTSTG_battleFuncs.getHeal(action->args[0], action->args[1], action->args[2]);
        if (unit->hp < unit->maxHp) {
            if (action->args[1] == FIGHTSTG_battle.state.active[side]) {
                children->task.script = WFIGHTMN_startTech(action->args[0], 0xBD);
                WFIGHTMN_setIdleMotion(action->args[0], -task->counter);
            }
            task->step++;
        } else {
            task->setSubstate(task, 0);
        }
        break;
    case 1:
        if (children->task.task == NULL) {
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = action->args[0];
            task->args[1] = action->args[1];
            task->args[2] = task->counter;
            children->task.message->show(children->task.message, 0xA, task->args);
            task->step++;
        }
        break;
    case 2:
        if (children->task.task == NULL) {
            unit->hp += task->counter;
            if (unit->hp > unit->maxHp) {
                unit->hp = unit->maxHp;
            }
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* EVENT_CLEAR_FIELD: shows message 0x6A and ends the element boost
   (FIGHTSTG_battle.state.boost.element) */
void WFIGHTMN_clearField(BattleMenu *task, BattleMenuChildren *children) {
    children->task.message = FIGHTSTG_createMessage();
    task->args[0] = 0x6A;
    children->task.message->show(children->task.message, 1, task->args);
    FIGHTSTG_battle.state.boost.element = 0;
    FIGHTSTG_battle.state.boost.amount = 0;
    task->setSubstate(task, 2);
}

/* Carries out the current action's damage (FIGHTSTG_battleFuncs.getDamage) on
   its unit */
void WFIGHTMN_takeDamage(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action;
    BattleFighter *unit;
    BattleFighter *hit;
    s32 side;
    s32 damage;

    switch (task->step) {
    case 0:
    default:
        action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
        side = action->args[0] >> 4;
        task->args[0] = action->args[0];
        task->args[1] = action->args[1];
        damage = FIGHTSTG_battleFuncs.getDamage(&action->args[0]);
        task->args[2] = damage;
        if (action->args[1] == FIGHTSTG_battle.state.active[side]) {
            unit = &FIGHTSTG_battle.state.fighters[side][action->args[1]];
            if (action->args[0] == 0) {
                children->task.hitEffect = FIGHTSTG_startHitEffect(unit->hp - damage <= 0 ? 2 : 1, 1, damage);
            } else {
                children->task.script = FIGHTSTG_createBattleScript();
                children->task.script->enemy = 0;
                children->task.script->index = 0xE;
                children->task.script->effect = 0x13;
                children->task.script->sound = 0x1A;
                children->task.script->stage = -1;
                if (unit->hp - task->args[2] <= 0) {
                    children->task.script->hits[3] = 2;
                } else {
                    children->task.script->hits[3] = 1;
                }
            }
            WFIGHTMN_setIdleMotion(action->args[0], task->args[2]);
            task->step++;
        } else {
            task->setSubstate(task, 0);
        }
        action->time = 1000;
        break;
    case 1:
        if (children->task.task == NULL) {
            hit = &FIGHTSTG_battle.state.fighters[task->args[0] >> 4][task->args[1]];
            hit->hp -= task->args[2];
            if (hit->hp <= 0) {
                hit->hp = 0;
                FIGHTSTG_queueKnockOut(task->args[0]);
            }
            children->task.message = FIGHTSTG_createMessage();
            children->task.message->show(children->task.message, 0xF, task->args);
            task->step++;
        }
        break;
    case 2:
        if (children->task.task == NULL) {
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* States 13-15: end the current action's unit's paralysis, confusion or sleep */
void WFIGHTMN_cureStatus(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        task->args[0] = task->substate + 0x1C;
        task->args[1] = action->args[0];
        task->args[2] = action->args[1];
        children->task.message = FIGHTSTG_createMessage();
        children->task.message->show(children->task.message, 7, task->args);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            unit = &FIGHTSTG_battle.state.fighters[action->args[0] != 0][action->args[1]];
            switch (task->substate) {
            case 13:
            default:
                unit->flags &= ~FIGHTER_PARALYZED;
                break;
            case 14:
                unit->flags &= ~FIGHTER_CONFUSED;
                break;
            case 15:
                unit->flags &= ~FIGHTER_ASLEEP;
                break;
            }
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* Clears the boost (BattleFighter.boosts) that the current action ends */
void WFIGHTMN_endBoost(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = action->args[2] + 0x5B;
        task->args[1] = action->args[0];
        task->args[2] = action->args[1];
        children->task.message->show(children->task.message, 7, task->args);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            unit = &FIGHTSTG_battle.state.fighters[action->args[0] >> 4][action->args[1]];
            unit->boosts[action->args[2]] = 0;
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* A confused partner's turn (FIGHTER_CONFUSED): the player's turn at step 3,
   where only an attack does anything */
void WFIGHTMN_runConfusedCommand(BattleMenu *task, BattleMenuChildren *children) {
    switch (task->step) {
    case 0:
    default:
        FIGHTSTG_setPlayerTurnStep(3);
        task->step++;
        break;
    case 1:
        if (FIGHTSTG_isPlayerChoosing() == 0) {
            if (children->commands->action == 0) {
                children->task.attack = FIGHTSTG_startFirstTech(0);
                task->setSubstate(task, 2);
            } else {
                task->setSubstate(task, 0);
            }
            FIGHTSTG_queuePlayerTurn(FIGHTSTG_events.funcs.getDelay(0, 0));
        }
        break;
    }
}

/* Clears the restriction that the current action ends (FIGHTER_NO_SWITCH or
   FIGHTER_NO_DIGIVOLVE) from its unit */
void WFIGHTMN_endRestriction(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = action->args[2] + 0x57;
        task->args[1] = action->args[0];
        task->args[2] = action->args[1];
        children->task.message->show(children->task.message, 7, task->args);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            unit = &FIGHTSTG_battle.state.fighters[action->args[0] >> 4][action->args[1]];
            unit->flags &= ~(1 << (action->args[2] + 4));
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* Digivolves the current partner for the battle, to the Digimon its level
   reaches (DigimonData.blastForms), and heals it when the change ends */
void WFIGHTMN_blast(BattleMenu *task, BattleMenuChildren *children) {
    Models *models;
    BattleFighter *unit;
    DigimonData *digimon;
    s32 partner;
    s32 level;
    s32 tier;
#if VERSION_EU || VERSION_JP
    s32 index;
#endif

    switch (task->step) {
    case 0:
    default:
        children->task.message = FIGHTSTG_createMessage();
        task->args[0] = 0x61;
        task->args[1] = 0;
        children->task.message->show(children->task.message, 2, task->args);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            partner = GAME.funcs.getPartyMember(FIGHTSTG_battle.state.active[0]);
            level = GAME.funcs.getPartnerStats(partner)->stats[STAT_LEVEL];
            if (level < 4) {
                tier = 0;
            } else if (level < 19) {
                tier = 1;
            } else if (level < 39) {
                tier = 2;
            } else if (level < 70) {
                tier = 3;
            } else {
                tier = 4;
            }
            unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
            digimon = GET_DIGIMON(DIGIMON_DATA[partner].id);
            unit->prevId = unit->id;
            unit->id = DIGIMON_DATA[digimon->blastForms[tier] - 1].id;
            unit->temporary = 1;
            FIGHTSTG_queueBlastEnd(partner);
            children->task.change = FIGHTSTG_startDigimonChange(unit->id, 1);
            task->step++;
        }
        break;
    case 2:
        models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        if (models->get(models, 0)->motion == 13) {
            BattleFighter *current = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
            current->hp = current->maxHp;
#if VERSION_EU || VERSION_JP
            if (current->flags & FIGHTER_CONFUSED) {
                index = FIGHTSTG_events.funcs.find(EVENT_STATUS_END + 1, 0, FIGHTSTG_battle.state.active[0]);
                if (index >= 0) {
                    FIGHTSTG_events.events[index].type = 0;
                    FIGHTSTG_events.events[index].time = 0;
                }
                current->flags &= ~FIGHTER_CONFUSED;
                task->confusionSound = 0;
            }
#endif
            task->setSubstate(task, 2);
        }
        break;
    }
}

/* Turns the current partner back into the Digimon it was (prevId) */
void WFIGHTMN_endBlast(BattleMenu *task, BattleMenuChildren *children) {
    BattleFighter *unit;

    switch (task->step) {
    case 0:
    default:
        unit = &FIGHTSTG_battle.state.fighters[0][FIGHTSTG_battle.state.active[0]];
        unit->id = unit->prevId;
        WFIGHTMN_cancelBlast();
        children->task.entrance = FIGHTSTG_startEntrance(unit->id, 0, WFIGHTMN_setIdleMotion(0, 0));
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = 0x62;
            task->args[1] = 0;
            children->task.message->show(children->task.message, 2, task->args);
            task->setSubstate(task, 2);
        }
        break;
    }
}

/* Takes the fallen fighter's event off the queue, shows message 0x50 or
   0x51 and clears its status, ending the partner's special */
static inline void announceKnockOut(BattleMenu *task, BattleMenuChildren *children, QueuedEvent *action) {
    BattleFighter *fighter;
    s32 side;

    /* the key is the event's first two args: args[0]'s low byte and args[1] */
    FIGHTSTG_events.funcs.remove((EventKey *)&action->args[0]);
    side = action->args[0] >> 4;
    fighter = FIGHTSTG_battle.state.fighters[side] + FIGHTSTG_battle.state.active[side];
    if (side == 0) {
        if (fighter->special != 0) {
            fighter->special = 0;
#if VERSION_US
            /* FIGHTSTG_queueSpecialEnd's event, built in the arguments */
            task->args[0] = EVENT_SPECIAL_END;
            task->args[1] = 1;
            task->args[2] = 0;
            task->args[3] = FIGHTSTG_battle.state.active[0];
            FIGHTSTG_events.funcs.pushFirst((BattleEvent *)task->args);
#elif VERSION_EU || VERSION_JP
            FIGHTSTG_queueSpecialEnd();
#endif
        }
        task->args[0] = 0x50;
        task->args[1] = 0;
    } else {
        task->args[0] = 0x51;
        task->args[1] = 0x10;
    }
    children->task.message->show(children->task.message, 2, task->args);
    fighter->charge = 0;
    fighter->flags = 0;
    fighter->confusion = 0;
    fighter->sleep = 0;
    fighter->paralysis = 0;
}

/* With no fighter left on the side, ends the battle: lost (message 0x53)
   or won (message 0x52, the partner's motion 0xD and, but in
   BATTLE_KIND_FINAL_LAST, the win jingle) */
static inline void endBattleOnKnockOut(BattleMenu *task, BattleMenuChildren *children, QueuedEvent *event) {
    Models *models;

    children->task.message = FIGHTSTG_createMessage();
    if (event->args[0] == 0) {
        FIGHTSTG_endBattle(BATTLE_LOST);
        SOUND.playSound(0x60040008);
        task->args[0] = 0x53;
        task->setSubstate(task, 2);
        children->cameraMove.cameraTurn = FIGHTSTG_startCameraTurn();
    } else {
        FIGHTSTG_endBattle(BATTLE_WON);
        if (FIGHTSTG_battle.state.kind != BATTLE_KIND_FINAL_LAST) {
            SOUND.playSound(SOUND_WIN_JINGLE);
        }
        task->args[0] = 0x52;
        task->setSubstate(task, 0x18);
        task->args[1] = GFX.funcs.getTime();
        task->args[2] = 100;
        models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        models->get(models, 0)->motion = 0xD;
    }
    children->task.message->show(children->task.message, 1, task->args);
}

/* In BATTLE_KIND_FINAL, brings in the enemy's second fighter in place of
   the first, then lets the enemy's turn come at once */
static inline void bringSecondEnemy(BattleMenu *task, BattleMenuChildren *children) {
    s32 index;

    switch (task->counter) {
    case 0:
    default:
        if (children->task.task == NULL) {
            FIGHTSTG_battle.state.kind = BATTLE_KIND_FINAL_SECOND;
            FIGHTSTG_battle.state.active[1] = 1;
            children->task.entrance = FIGHTSTG_startEntrance(FIGHTSTG_battle.state.fighters[1][1].id, 1, 0);
            FIGHTSTG_battle.state.active[1] = 0;
            task->counter++;
        }
        break;
    case 1:
        if (children->task.entrance->done != 0) {
            FIGHTSTG_battle.state.fighters[1][0] = FIGHTSTG_battle.state.fighters[1][1];
            FIGHTSTG_battle.state.active[1] = 0;
            FIGHTSTG_battle.state.fighters[1][1].id = 0;
            task->counter++;
        }
        break;
    case 2:
        index = FIGHTSTG_events.funcs.first(EVENT_ENEMY_TURN);
        if (index >= 0) {
            FIGHTSTG_events.events[index].time = 0;
        }
        task->setSubstate(task, 2);
        break;
    }
}

/* A fighter falls (?): case 0 takes its event off the queue, shows message
   0x50 or 0x51 and clears its status (announceKnockOut); case 1 looks for the
   side's next fighter standing. The player's side picks one
   (FIGHTSTG_setPlayerTurnStep(4)), the enemy's brings in the first; with none
   left the battle ends (endBattleOnKnockOut: 0x53 lost, 0x52 won). Case 5
   brings in the enemy's second fighter in BATTLE_KIND_FINAL
   (bringSecondEnemy). */
void WFIGHTMN_knockOut(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action;
    QueuedEvent *event;
    BattleFighter *unit;
    BattleFighter *enemy;
    BattleTableEntry *entry;
    s32 slot;
    s32 i;

    switch (task->step) {
    case 0:
    default:
        action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
        children->task.message = FIGHTSTG_createMessage();
        if (action->args[0] != 0 && FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL) {
            task->args[0] = 0x8B;
            children->task.message->show(children->task.message, 1, task->args);
            task->setStep(task, 5);
            return;
        }
        announceKnockOut(task, children, action);
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            /* event and not action, and unit for the search too: the
               match depends on both, which give the registers */
            event = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
            slot = -1;
            unit = FIGHTSTG_battle.state.fighters[event->args[0] >> 4];
            for (i = 0; i < 3; i++) {
                if (unit[i].id != 0 && unit[i].hp != 0) {
                    slot = i;
                    break;
                }
            }
            if (slot != -1) {
                if (event->args[0] == 0) {
                    FIGHTSTG_setPlayerTurnStep(4);
                    task->step = 2;
                } else {
                    unit = FIGHTSTG_battle.state.fighters[1] + slot;
                    task->step = 3;
                    task->args[0] = slot;
                    task->args[1] = FIGHTSTG_battle.state.active[1];
                    FIGHTSTG_battle.state.active[1] = slot;
                    children->task.entrance = FIGHTSTG_startEntrance(unit->id, 1, WFIGHTMN_setIdleMotion(SIDE_ENEMY, 0));
                    FIGHTSTG_battle.state.active[1] = task->args[1];
                }
                break;
            }
            endBattleOnKnockOut(task, children, event);
        }
        break;
    case 2:
        if (FIGHTSTG_isPlayerChoosing() == 0) {
            task->setSubstate(task, 5);
            task->step = 1;
        }
        break;
    case 3:
        if (children->task.entrance->done != 0) {
            FIGHTSTG_battle.state.active[1] = task->args[0];
            task->step++;
        }
        break;
    case 4:
        if (children->task.task == NULL) {
            /* through a pointer: the match depends on it, which loads
               active[1] from the address of fighters[1] */
            enemy = FIGHTSTG_battle.state.fighters[1];
            entry = FIGHTSTG_battleTableFunc((enemy + FIGHTSTG_battle.state.active[1])->id);
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = entry->nameId;
            children->task.message->show(children->task.message, 0xD, task->args);
            task->setSubstate(task, 2);
        }
        break;
    case 5:
        bringSecondEnemy(task, children);
        break;
    }
}

/* The end of the partner's special state (EVENT_SPECIAL_END): clears it and
   shows message 0x59 */
void WFIGHTMN_endSpecial(BattleMenu *task, BattleMenuChildren *children) {
    QueuedEvent *action = &FIGHTSTG_events.events[FIGHTSTG_events.curIndex];
    BattleFighter *unit;

    unit = &FIGHTSTG_battle.state.fighters[0][action->args[1]];
    unit->special = 0;
    children->task.message = FIGHTSTG_createMessage();
    task->args[0] = 0x59;
    task->args[1] = action->args[0];
    task->args[2] = action->args[1];
    children->task.message->show(children->task.message, 7, task->args);
    task->setSubstate(task, 2);
}

/* EVENT_DIGIDEVOLVE: the partner goes back to its own form, its blast and its
   queued technique are dropped, and it comes in again with message 0x47 */
void WFIGHTMN_digidevolve(BattleMenu *task, BattleMenuChildren *children) {
    Battle *battle;
    EventQueue *actions;
    s32 index;
    BattleFighter *unit;
    BattleFighter *units;

    switch (task->step) {
    case 0:
    default:
        battle = &FIGHTSTG_battle;
        units = battle->state.fighters[0];
        index = GAME.funcs.getPartyMember(battle->state.active[0]);
        unit = units + battle->state.active[0];
        unit->id = DIGIMON_DATA[index].id;
        unit->charge = 0;
        WFIGHTMN_cancelBlast();
        actions = &FIGHTSTG_events;
        index = actions->funcs.find(EVENT_PARTNER_TECH, 0, battle->state.active[0]);
        if (index >= 0) {
            actions->events[index].type = 0;
        }
        children->task.entrance = FIGHTSTG_startEntrance(unit->id, 0, WFIGHTMN_setIdleMotion(0, 0));
        task->step++;
        break;
    case 1:
        if (children->task.task == NULL) {
            children->task.message = FIGHTSTG_createMessage();
            task->args[0] = 0x47;
            task->args[1] = 0;
            children->task.message->show(children->task.message, 2, task->args);
            task->setSubstate(task, 2);
        }
        break;
    }
}

/* Shows the won message (from WFIGHTMN_knockOut) for its time, then closes it */
void WFIGHTMN_showWon(BattleMenu *task, BattleMenuChildren *children) {
    switch (task->step) {
    case 0:
    default:
        if (children->task.task != NULL) {
            if (GFX.funcs.getTime() - task->args[1] > task->args[2]) {
                children->task.message->finish(children->task.message);
                task->step++;
            }
            break;
        }
        task->setSubstate(task, 0);
        break;
    case 1:
        if (children->task.task != NULL) {
            children->task.task->state = TASK_KILL;
        }
        task->setSubstate(task, 0);
        break;
    }
}

/* The enemy's third fighter comes in (BATTLE_KIND_FINAL_LAST), takes the first's
   place, and technique 440 is made from 443 with FIGHTSTG_battle.state.tech's
   effect (its look from WFIGHTMN_effectVisuals) and element (from
   WFIGHTMN_elementVisuals) before message 0x16 names it (?) */
void WFIGHTMN_bringLastEnemy(BattleMenu *task, BattleMenuChildren *children) {
    TechData *tech;
    TechData *dst;
    s32 id;
    s32 i;

    switch (task->step) {
    case 0:
    default:
        FIGHTSTG_battle.state.kind = BATTLE_KIND_FINAL_LAST;
        FIGHTSTG_battle.state.active[1] = 2;
        children->task.entrance = FIGHTSTG_startEntrance(FIGHTSTG_battle.state.fighters[1][2].id, 1, 0);
        FIGHTSTG_battle.state.active[1] = 0;
        task->step++;
        break;
    case 1:
        if (children->task.entrance->done != 0) {
            FIGHTSTG_battle.state.fighters[1][0] = FIGHTSTG_battle.state.fighters[1][2];
            FIGHTSTG_battle.state.active[1] = 0;
            FIGHTSTG_battle.state.fighters[1][2].id = 0;
            task->step++;
        }
        break;
    case 2:
        if (children->task.task == NULL) {
            /* dst[3] and not TECHS[442]: the match depends on it */
            dst = &TECHS[439];
            id = FIGHTSTG_battle.state.tech;
            *dst = dst[3];
            if (id != 0) {
                tech = &TECHS[id - 1];
                if (tech->script != 5 && tech->script != 12) {
                    if (tech->effect >= TECH_EFFECT_FIRST && !(tech->effect == TECH_EFFECT_MULTI_HIT || tech->effect == TECH_EFFECT_ENEMY_ONLY) && tech->effect != TECH_EFFECT_STEAL) {
                        dst->effect = tech->effect;
                        dst->effectPower = tech->effectPower;
                        dst->effectChance = tech->effectChance;
                        for (i = 0; WFIGHTMN_effectVisuals[i].effect != -1; i++) {
                            if (WFIGHTMN_effectVisuals[i].effect == tech->effect) {
                                dst->scriptEffect = WFIGHTMN_effectVisuals[i].scriptEffect;
                                dst->scriptSound = WFIGHTMN_effectVisuals[i].scriptSound;
                                dst->scriptStage = 0;
                                break;
                            }
                        }
                    }
                    if (tech->element >= ELEMENT_FIRST) {
                        dst->element = tech->element;
                        dst->elementPower = tech->elementPower;
                        if (dst->effect < TECH_EFFECT_FIRST) {
                            /* while (1), not for (;;): the match depends on it,
                               which leaves the loop's test at its top */
                            i = 0;
                            while (1) {
                                if (WFIGHTMN_elementVisuals[i].element == tech->element) {
                                    dst->scriptEffect = WFIGHTMN_elementVisuals[i].scriptEffect;
                                    dst->scriptSound = WFIGHTMN_elementVisuals[i].scriptSound;
                                    dst->scriptStage = WFIGHTMN_elementVisuals[i].scriptStage;
                                    break;
                                }
                                i++;
                            }
                        }
                    }
                }
                children->task.message = FIGHTSTG_createMessage();
                task->args[0] = FIGHTSTG_battle.state.tech;
                children->task.message->show(children->task.message, 0x16, task->args);
                task->step = 3;
            } else {
                task->step = 4;
            }
        }
        break;
    case 3:
        if (children->task.task == NULL) {
            task->step++;
        }
        break;
    case 4:
        task->setSubstate(task, 2);
        break;
    }
}

/* The end of the enemy's weakness (EVENT_WEAKNESS_END): its lowered stats
   come back and it takes its turn first */
void WFIGHTMN_restoreEnemy(BattleMenu *task, BattleMenuChildren *children) {
    BattleEvent request;
    s32 index = FIGHTSTG_events.funcs.first(EVENT_ENEMY_TURN);

    if (index >= 0) {
        FIGHTSTG_events.events[index].type = 0;
    }
    request.type = EVENT_ENEMY_TURN;
    request.delay = 1;
    request.args[0] = -1;
    FIGHTSTG_events.funcs.pushFirst(&request);
    FIGHTSTG_battle.state.fighters[1][0].boosts[3] = 0;
    FIGHTSTG_battle.state.fighters[1][0].boosts[1] = 0;
    task->setSubstate(task, 2);
}

/* The battle menu's main state: waits for FIGHTSTG's turn (FIGHTSTG_events.funcs.pop),
   then runs the state of the command it gets (WFIGHTMN_states) */
void WFIGHTMN_runTurn(BattleMenu *task) {
    BattleMenuChildren *children = task->children;

    switch (task->substate) {
    default:
        if (task->substate >= 3 && task->substate < 27) {
            WFIGHTMN_states[task->substate](task, children);
        } else {
#if VERSION_EU || VERSION_JP
            task->setSubstate(task, 1);
#endif
            task->step = FIGHTSTG_events.funcs.pop();
            if (task->step == 0) {
                FIGHTSTG_queuePlayerTurn(0);
                FIGHTSTG_queueEnemyTurn(FIGHTSTG_events.funcs.getDelay(0, 0) / 2);
                task->setSubstate(task, 0);
            }
#if VERSION_US
            else {
                task->setSubstate(task, 1);
            }
#endif
        }
        break;
    case 0:
#if VERSION_EU || VERSION_JP
        task->setSubstate(task, 1);
        task->step = FIGHTSTG_events.funcs.pop();
#else
        task->step = FIGHTSTG_events.funcs.pop();
        if (task->step != 0) {
            task->substate = 1;
        }
#endif
        break;
    case 1:
        switch (task->step) {
        default:
            task->setSubstate(task, 3);
            break;
        case EVENT_END_BATTLE:
            task->setSubstate(task, 7);
            break;
        case EVENT_ENEMY_TURN:
            if (FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_SECOND) {
                children->task.oneHpTurn = FIGHTSTG_startOneHpTurn(&FIGHTSTG_battle);
                task->setSubstate(task, 2);
            } else if (FIGHTSTG_battle.state.kind == BATTLE_KIND_FINAL_LAST) {
                children->task.enemyAttack = FIGHTSTG_startEnemyAttack(0, 0);
                task->setSubstate(task, 2);
            } else {
                (FIGHTSTG_battle.state.fighters[1] + FIGHTSTG_battle.state.active[1])->turns++;
                children->task.enemyTurn = FIGHTSTG_startEnemyTurn(&FIGHTSTG_battle);
                task->setSubstate(task, 2);
            }
            break;
        case EVENT_RUN_AWAY:
            task->setSubstate(task, 8);
            break;
        case EVENT_AUTO_RECOVER_END:
            task->setSubstate(task, 9);
            break;
        case EVENT_RECOVERY:
            task->setSubstate(task, 0xA);
            break;
        case EVENT_CLEAR_FIELD:
            task->setSubstate(task, 0xB);
            break;
        case EVENT_STATUS_DAMAGE:
            task->setSubstate(task, 0xC);
            break;
        case EVENT_STATUS_END:
            task->setSubstate(task, 0xD);
            break;
        case EVENT_STATUS_END + 1:
            task->setSubstate(task, 0xE);
            break;
        case EVENT_STATUS_END + 2:
            task->setSubstate(task, 0xF);
            break;
        case EVENT_BOOST_END:
        case EVENT_BOOST_END + 1:
        case EVENT_BOOST_END + 2:
            task->setSubstate(task, 0x10);
            break;
        case EVENT_RESTRICTION_END:
        case EVENT_RESTRICTION_END + 1:
            task->setSubstate(task, 0x12);
            break;
        case EVENT_BLAST:
            task->setSubstate(task, 0x13);
            break;
        case EVENT_BLAST_END:
            task->setSubstate(task, 0x14);
            break;
        case EVENT_KNOCK_OUT:
            task->setSubstate(task, 0x15);
            break;
        case EVENT_SPECIAL_END:
            task->setSubstate(task, 0x16);
            break;
        case EVENT_DIGIDEVOLVE:
            task->setSubstate(task, 0x17);
            break;
        case EVENT_LAST_ENEMY:
            task->setSubstate(task, 0x19);
            break;
        case EVENT_WEAKNESS_END:
            task->setSubstate(task, 0x1A);
            break;
        }
        break;
    case 2:
        if (children->task.task == NULL) {
            task->setSubstate(task, 0);
        }
        break;
    }
}

/* WFIGHTMN's entry point: creates the battle menu, and zeroes FIGHTSTG_battle's
   state and BATTLE_RESULT */
Task *WFIGHTMN_start(void) {
    Task *task = createTaskWithId(WFIGHTMN_updateMenu, sizeof(BattleMenu), sizeof(BattleMenuChildren), BATTLE_TASK_MENU);

    HEAP.zero(&FIGHTSTG_battle.state, sizeof(FIGHTSTG_battle.state));
    HEAP.zero(&BATTLE_RESULT, sizeof(BATTLE_RESULT));
    return task;
}
