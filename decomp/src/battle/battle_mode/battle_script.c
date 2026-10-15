/* FIGHTSTG's battle scripts: the commands of a fighter's script (models,
   effects, camera, waits, stage, sounds and fades) and its task. The
   European version has the camera turn here too (camera_turn.h), which the
   USA one has in camera_task.c. */

#include "battle/battle_mode.h"

/* The model a script command names by TYPE: 0 the script's own side's, 1-3
   the partner's slots and 4-6 the enemy's */
s32 FIGHTSTG_getScriptModel(BattleScript *script, s32 type) {
    switch (type) {
    case 0:
    default:
        return script->enemy != 0 ? 0x10 : 0;
    case 1:
        return 0;
    case 2:
        return 1;
    case 3:
        return 2;
    case 4:
        return 0x10;
    case 5:
        return 0x11;
    case 6:
        return 0x12;
    }
}

/* The battle script's hit command: 0 starts the other side's script for the
   last hit's result, 5 the one for the next hit's */
void FIGHTSTG_runScriptHits(BattleScript *script, BattleScriptChildren *children) {
    s32 hit;

    switch (*script->pc++) {
    case 0:
        if (children->script != NULL) {
            children->script->destroy(children->script);
        }
        children->script = FIGHTSTG_createBattleScript();
        children->script->enemy = script->enemy == 0;
        children->script->index = script->hits[3] + 1;
        break;
    /* the match depends on these cases, which do nothing */
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (children->script != NULL) {
            children->script->destroy(children->script);
        }
        children->script = FIGHTSTG_createBattleScript();
        children->script->enemy = script->enemy == 0;
        switch (script->hitsPlayed) {
        case 0:
        default:
            hit = script->hits[0];
            break;
        case 1:
            hit = script->hits[1];
            break;
        case 2:
            hit = script->hits[2];
            break;
        }
        children->script->index = hit + 1;
        script->hitsPlayed++;
        break;
    }
}

/* Model command 0 with 1: puts the model back to its idle motion. */
static inline void playIdleMotion(ModelControl *control) {
    control->motionDone = 0;
    control->motion = control->idleMotion + 1;
}

/* Model command 0 with any other MOTION: plays the model's motion MOTION
   from its start. */
static inline void playMotion(ModelControl *control, s32 motion) {
    control->motion = motion + 1;
    control->restart = 1;
    control->motionDone = 0;
}

/* Model command 4: sets the model's rotation from the script. */
static inline void turnModel(BattleScript *script, ModelControl *control) {
    control->rot.x = *script->pc++;
    control->rot.y = -*script->pc++;
    control->rot.z = -*script->pc++;
    script->pc++;
}

/* Model command 5: replaces the model's jump with a new one of kind KIND
   (DISTANCE only for kind 4). */
static inline void startModelJump(BattleScriptChildren *children, ModelControl *control, s32 kind, s32 distance) {
    if (children->jump != NULL) {
        children->jump->destroy(children->jump);
    }
    children->jump = FIGHTSTG_startJump(control, kind, distance);
}

/* Model command 6: adds fighter FIGHTER's model to the battle as model ID. */
static inline void addModel(s32 id, s32 fighter) {
    Models *models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);

    models->add(models, id, fighter, 0);
}

/* Model command 7: moves the model back home over the script's time, or at
   once when it is 0. */
static inline void moveModelHome(BattleScript *script, BattleScriptChildren *children, ModelControl *control) {
    s32 time = *script->pc++;

    if (time != 0) {
        children->move = FIGHTSTG_startMove(control, &control->homePos, time);
    } else {
        control->pos.x = control->homePos.x;
        control->pos.y = control->homePos.y;
        control->pos.z = control->homePos.z;
    }
}

/* Model command 9: removes model ID from the battle. */
static inline void removeModel(s32 id) {
    Models *models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);

    models->remove(models, id);
}

/* The battle script's model command, on the model given by FIGHTSTG_getScriptModel:
   0 plays a motion (0 waits for the one playing, 1 goes back to idle), 1
   and 2 turn layers[0] on and off, 3 moves it to a position in the script
   (over a time, or at once when it is 0), 4 turns it, 5 makes it jump (0
   waits for the jump), 6 adds a model (waiting while its file loads in
   script 12), 7 moves it home, 8 0x2800 from home and 9 removes it.
   Returning 0 runs the command again. The match depends on each case's
   time being its own and on case 3's being read before the position. */
s32 FIGHTSTG_runScriptModel(BattleScript *script, BattleScriptChildren *children) {
    ShortVec3 pos;
    s32 arg = 0;
    ModelControl *control = NULL;
    s32 cmd = *script->pc++;
    s32 id = FIGHTSTG_getScriptModel(script, *script->pc++);

    switch (cmd) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        break;
    default:
        arg = *script->pc++;
        break;
    }
    if (cmd != 6 && cmd != 9) {
        control = script->models->get(script->models, id);
    }
    switch (cmd) {
    case 0:
        switch (arg) {
        case 0:
            if (control->motionDone == 0) {
                script->pc -= 4;
                return 0;
            }
            return 1;
        case 1:
            playIdleMotion(control);
            break;
        default:
            playMotion(control, arg);
            break;
        }
        break;
    case 5:
        switch (arg) {
        case 0:
            if (children->jump == NULL) {
                return 1;
            }
            if (children->jump->state < 2) {
                script->pc -= 4;
                return 0;
            }
            return 1;
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
            startModelJump(children, control, arg, 0);
            break;
        case 4:
            startModelJump(children, control, arg, *script->pc++);
            break;
        }
        break;
    case 1:
        control->layers[0].enabled = 1;
        return 1;
    case 2:
        control->layers[0].enabled = 0;
        break;
    case 3: {
        s32 time = script->pc[3];

        pos.x = script->pc[0];
        pos.y = -script->pc[1];
        pos.z = -script->pc[2];
        script->pc += 4;
        if (time != 0) {
            children->move = FIGHTSTG_startMove(control, &pos, time);
        } else {
            control->pos.x = pos.x;
            control->pos.y = pos.y;
            control->pos.z = pos.z;
        }
        break;
    }
    case 4:
        turnModel(script, control);
        break;
    case 6:
        if (script->index == 12 && FILE_CACHE.isLoading(FIGHTSTG_fighterCache.funcs.getInfo(arg)->model >> 16)) {
            script->pc -= 4;
            return 0;
        }
        addModel(id, arg);
        return 1;
    case 7:
        moveModelHome(script, children, control);
        break;
    case 8: {
        s32 time = *script->pc++;

        pos.x = control->homePos.x;
        pos.y = control->homePos.y;
        if (id & 0xF0) {
            pos.z = control->homePos.z - 0x2800;
        } else {
            pos.z = control->homePos.z + 0x2800;
        }
        if (time != 0) {
            children->move = FIGHTSTG_startMove(control, &pos, time);
        } else {
            control->pos.x = pos.x;
            control->pos.y = pos.y;
            control->pos.z = pos.z;
        }
        break;
    }
    case 9:
        removeModel(id);
        break;
    }
    return 1;
}

/* The battle script's effect command: 0 starts sprite effect EFFECT (9999 the
   script's own) at a position in the script, 1 loads its images and sheet,
   running again until they are in. Returning 0 runs the command again */
s32 FIGHTSTG_runScriptEffect(BattleScript *script, BattleScriptChildren *children) {
    SVECTOR pos;
    TimLoader loader;
    s32 mode = *script->pc++;
    s32 effect = *script->pc++;
    s32 i;

    if (effect == 9999) {
        effect = script->effect;
    }
    switch (mode) {
    case 0:
    default:
        pos.vx = *script->pc++;
        pos.vy = *script->pc++;
        pos.vz = *script->pc++;
        for (i = 0; i < 8; i++) {
            if (children->spriteEffects[i] == NULL) {
                children->spriteEffects[i] = FIGHTSTG_startSpriteEffect(effect, &pos);
                break;
            }
        }
        break;
    case 1:
        switch (script->loadStep) {
        case 0:
        default:
            if (!FIGHTSTG_findEffectSheet(effect, &script->effectImages, &script->effectSheet, &script->effectTexPos)) {
                break;
            }
            script->loadStep++;
            /* fallthrough */
        case 1:
            if (script->effectImages == 0 || !FILE_CACHE.isLoading(script->effectImages >> 16)) {
                script->loadStep++;
            }
            script->pc -= 3;
            return 0;
        case 2:
            if (script->effectImages != 0) {
                initTimLoader(&loader);
                loader.setImagePos(script->effectTexPos.x, script->effectTexPos.y);
                loader.loadArchive(FILE_CACHE.getEntry(script->effectImages));
            }
            script->loadStep++;
            /* fallthrough */
        case 3:
            if (script->effectSheet != 0 && FILE_CACHE.isLoading(script->effectSheet >> 16)) {
                script->pc -= 3;
            } else {
                script->loadStep = 0;
            }
            return 0;
        }
    }
    return 1;
}

/* The battle script's effect model command: 0 starts effect model ID at a
   position and rotation in the script (their y and z negated), 1 waits while
   its file loads. Returning 0 runs the command again */
s32 FIGHTSTG_runScriptEffectModel(BattleScript *script, BattleScriptChildren *children) {
    SVECTOR pos;
    SVECTOR rot;
    s32 mode = *script->pc++;
    s32 id = *script->pc++;
    s32 file;
    s32 i;

    switch (mode) {
    case 0:
    default:
        pos.vx = *script->pc++;
        pos.vy = -*script->pc++;
        pos.vz = -*script->pc++;
        rot.vx = *script->pc++;
        rot.vy = -*script->pc++;
        rot.vz = -*script->pc++;
        for (i = 0; i < 3; i++) {
            if (children->effects[i] == NULL) {
                children->effects[i] = FIGHTSTG_startEffectModel(id, &pos, &rot);
                break;
            }
        }
        break;
    case 1:
        file = FIGHTSTG_getEffectModelFile(id);
        if (file != 0 && FILE_CACHE.isLoading(file)) {
            script->pc -= 3;
            return 0;
        }
        break;
    }
    return 1;
}

/* The battle script's camera command: the time of the fade (0 to cut),
   then the enemy's view, a fighter's or one given in the script */
void FIGHTSTG_runScriptCamera(BattleScript *script, BattleScriptChildren *children) {
    BattleCamera *camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
    s32 time = *script->pc++;
    /* the match depends on pc, which points at the mode, and on view */
    s16 *pc = script->pc;
    CameraView *view = &FIGHTSTG_fighterView;
    s32 id;

    switch (*script->pc++) {
    case 0:
    default:
        id = *script->pc++;
        switch (id) {
        case 0:
        default:
            camera->getEnemyView(camera);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            camera->getFighterView(camera, 0, id + 7);
            break;
        case 5:
        case 6:
        case 7:
            camera->getFighterView(camera, 0x10, id - 5);
            break;
        }
        break;
    case 1:
        view->vpx = pc[1];
        view->vpy = -pc[2];
        view->vpz = -pc[3];
        view->vrx = pc[4];
        view->vry = -pc[5];
        view->vrz = -pc[6];
        view->tx = pc[7];
        view->ty = -pc[8];
        view->tz = -pc[9];
        view->rot.vx = pc[10];
        view->rot.vy = -pc[11];
        view->rot.vz = -pc[12];
        view->rz = pc[13];
        view->proj = pc[14];
        script->pc = pc + 15;
        break;
    }
    if (time != 0) {
        camera->fade(camera, NULL, view, time);
    } else {
        camera->set(camera, view);
    }
}

/* The battle script's wait command: waits the time in the script, returning 0
   until it is over */
s32 FIGHTSTG_runScriptWait(BattleScript *script, BattleScriptChildren *children) {
    switch (script->waiting) {
    case 0:
    default:
        script->wait = *script->pc;
        script->waiting = 1;
        script->pc--;
        return 0;
    case 1:
        script->wait -= GFX.funcs.getFrameTime();
        script->pc--;
        if (script->wait > 0) {
            return 0;
        }
        script->waiting = 0;
        script->wait = 0;
        script->pc += 2;
        return 1;
    }
}

/* The battle script's stage command (0x38 the script's own stage, -1 none): 0
   changes the fight stage (0 the battle's) with the fade times in the script,
   keeping the partner's idle motion aside on stage 0x1D and bringing it back
   on 0x1E; 1 waits while the stage's motions load. Returning 0 runs the
   command again */
s32 FIGHTSTG_runScriptStage(BattleScript *script, BattleScriptChildren *children) {
    FightStage *stage = TASK_REGISTRY.funcs.find(BATTLE_TASK_STAGE, -1, -1);
    s32 mode = *script->pc++;
    s32 id = *script->pc++;
    s32 fadeOut;
    s32 fadeIn;
    s32 motions;
    ModelControl *control;

    if (id == 0x38) {
        id = script->stage;
    }
    if (id == -1) {
        return 1;
    }
    switch (mode) {
    case 0:
    default:
        fadeOut = *script->pc++;
        fadeIn = *script->pc++;
        if (id == 0) {
            id = BATTLE_SETUP.stage;
        }
        stage->setStage(stage, id, fadeOut, fadeIn);
        switch (id) {
        case 0x1D:
            control = script->models->get(script->models, 0);
            FIGHTSTG_partnerIdleMotion = control->idleMotion;
            control->idleMotion = 0;
            break;
        case 0x1E:
            if (FIGHTSTG_partnerIdleMotion != 0) {
                script->models->get(script->models, 0)->motion = 2;
            }
            break;
        }
        break;
    case 1:
        if (id == 0) {
            return 1;
        }
        motions = FIGHTSTG_getStageMotionsFile(id);
        if (motions != 0 && FILE_CACHE.isLoading(motions)) {
            script->pc -= 3;
            return 0;
        }
        break;
    }
    return 1;
}

/* The battle script's sound command: plays a battle sound for a time (0x62
   and 0x63 the hit's sound, or 0x38 for a hit of result 3) */
void FIGHTSTG_runScriptSound(BattleScript *script, BattleScriptChildren *children) {
    s32 sound = *script->pc++;
    s32 time = *script->pc++;

    if (sound == 0x62) {
        sound = script->hits[script->soundsPlayed] == 3 ? 0x38 : script->sound;
        script->soundsPlayed++;
    } else if (sound == 0x63) {
        sound = script->hits[3] == 3 ? 0x38 : script->sound;
        script->soundsPlayed++;
    }
    if (children->sound == NULL) {
        children->sound = FIGHTSTG_playBattleSound(sound, time);
    }
}

/* The battle script's fade command: 0 fades the screen out over the frames in
   the script, 1 back in */
void FIGHTSTG_runScriptFade(BattleScript *script, WhiteFlash **fade) {
    s32 op = *script->pc++;
    s32 frames = *script->pc++;

    switch (op) {
    case 0:
        *fade = FIGHTSTG_startWhiteFlash(frames);
        break;
    case 1:
        if (*fade != NULL) {
            FIGHTSTG_endWhiteFlash(*fade, frames);
        }
        break;
    }
}

/* The battle script's task: finds the fighter's effect archive and its script
   (loading sound bank 0x46 for script 12), then runs its commands until one
   waits, and ends once its children are done */
void FIGHTSTG_updateBattleScript(BattleScript *script, BattleScriptChildren *children) {
    s32 more;
    s32 busy;
    s32 i;

    switch (script->state) {
    case 0:
    default:
        switch (script->substate) {
        case 0:
        default:
            script->model = script->enemy != 0 ? 0x10 : 0;
            script->models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            script->fighter = script->models->get(script->models, script->model)->fighter;
            FIGHTSTG_fighterCache.funcs.getInfo(script->fighter);
            script->archive = FIGHTSTG_fighterCache.partnerInfo->effects;
            /* getArchiveEntry gives the entry as a u8 * */
            script->pc = FILE_CACHE.getArchiveEntry(script->index, FILE_CACHE.getEntry(script->archive));
            if (script->index != 12) {
                script->nextState(script);
                break;
            }
            script->nextSubstate(script);
            /* fallthrough */
        case 1:
            switch (script->step) {
            case 0:
            default:
                SOUND.loadBank(0x46);
                script->nextStep(script);
                /* fallthrough */
            case 1:
                if (SOUND.isLoading()) {
                    return;
                }
            }
            script->nextState(script);
            break;
        }
        break;
    case 1:
        do {
            more = 1;
            switch (*script->pc++) {
            case 1:
                FIGHTSTG_runScriptHits(script, children);
                break;
            case 2:
                more = FIGHTSTG_runScriptModel(script, children);
                break;
            case 3:
                more = FIGHTSTG_runScriptEffect(script, children);
                break;
            case 4:
                more = FIGHTSTG_runScriptStage(script, children);
                break;
            case 5:
                FIGHTSTG_runScriptCamera(script, children);
                break;
            case 6:
                more = FIGHTSTG_runScriptEffectModel(script, children);
                break;
            case 7:
                FIGHTSTG_runScriptFade(script, &children->fade);
                break;
            /* the match depends on these cases, which do nothing, and on 2 and 3 */
            case 8:
            case 9:
                break;
            case 10:
                FIGHTSTG_runScriptSound(script, children);
                break;
            case 11:
                more = FIGHTSTG_runScriptWait(script, children);
                break;
            case 0:
            case 0xFF:
                busy = 0;
                if (children->jump != NULL) {
                    busy = children->jump->state < 2;
                }
                if (children->script != NULL) {
                    busy = 1;
                }
                for (i = 0; i < 8; i++) {
                    if (children->spriteEffects[i] != NULL) {
                        busy = 1;
                        break;
                    }
                }
                if (busy) {
                    script->pc--;
                } else {
                    script->setState(script, 3);
                }
                more = 0;
                break;
            }
        } while (more);
        break;
    case 2:
    case 3:
        break;
    }
}

/* Creates a battle script, which plays a technique (FIGHTSTG_updateBattleScript) */
BattleScript *FIGHTSTG_createBattleScript(void) {
    return createTask(FIGHTSTG_updateBattleScript, sizeof(BattleScript), 16 * sizeof(Task *));
}

#if VERSION_EU || VERSION_JP
/* the European version has the task of FIGHTSTG_updateCameraTurn here */
#include "camera_turn.h"
#endif
