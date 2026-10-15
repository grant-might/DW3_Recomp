/* FIGHTSTG's Digimon change: the effect of a digivolution, or of a blast's. */

#include "battle/battle_mode.h"

/* where FIGHTSTG_updateDigimonChange's effects are: the first (1000, 1001
   or 1007) when the old Digimon starts being wiped away, the second (1002)
   once it's gone, the third (1003 or 1008), behind (z -1), once the new one
   is shown whole; all are screen positions */
const SVECTOR FIGHTSTG_changeStartPos = { 0, 0, 0x7FFF, 0 };
const SVECTOR FIGHTSTG_changeMidPos = { 160, 120, 0x7FFF, 0 };
const SVECTOR FIGHTSTG_changeEndPos = { -160, -120, -1, 0 };

/* Applies the wipe: clips BATTLE_LAYER_MODELS to clip0 and
   BATTLE_LAYER_WIREFRAME to clip1 */
static inline void setChangeClips(DigimonChange *task) {
    Layer *layer = GFX.funcs.getLayer(BATTLE_LAYER_MODELS);

    layer->setClipPos(layer, task->clip0.x, task->clip0.y);
    layer->setClipSize(layer, task->clip0.w, task->clip0.h);
    layer = GFX.funcs.getLayer(BATTLE_LAYER_WIREFRAME);
    layer->setClipPos(layer, task->clip1.x, task->clip1.y);
    layer->setClipSize(layer, task->clip1.w, task->clip1.h);
}

/* Once the change's stage is set, plays the change sound and adds the new
   Digimon as model 1, then lets it show for 180 frames */
static inline void addNewDigimon(DigimonChange *task, Models *models, FightStage *stage) {
    switch (task->step) {
    case 0:
    default:
        if (stage->state == 2) {
            break;
        }
        SOUND.playSound(0x41140000);
        models->add(models, 1, task->key1, 0);
        task->nextStep(task);
        /* fallthrough */
    case 1:
        task->counter += GFX.funcs.getFrameTime();
        if (task->counter >= 180) {
            task->nextSubstate(task);
        }
        break;
    }
}

/* Starts the change: the clips at the old Digimon whole, the camera on the
   partner, its wireframe layer and change motion, and the first effects */
static inline void startChange(DigimonChange *task, DigimonChangeChildren *children, Models *models, BattleCamera *camera) {
    ModelControl *control;
    s32 z;

    task->clip0.x = 0;
    task->clip0.y = 0;
    task->clip0.w = 320;
    task->clip0.h = 240;
    task->clip1.x = 0;
    task->clip1.y = 240;
    task->clip1.w = 320;
    task->clip1.h = 0;
    control = models->get(models, 0);
    camera->getFighterView(camera, 0, 9);
    z = control->homePos.z;
    z = -z;
    FIGHTSTG_fighterView.vpz += z;
    FIGHTSTG_fighterView.vrz += z;
    camera->set(camera, &FIGHTSTG_fighterView);
    control->layers[1].enabled = 1;
    control->layers[1].wireframe = 1;
    control->layers[1].layerId = BATTLE_LAYER_WIREFRAME;
    control->pos.x = 0;
    control->pos.z = 0;
    control->rot.x = 0;
    control->rot.y = 0x800;
    control->rot.z = 0;
    control->pos.y = control->homePos.y;
    if (task->key2 != 0) {
        control->motion = 14;
    } else {
        control->motion = 13;
    }
    models->get(models, 0x10)->layers[0].enabled = 0;
    if (task->key2 == 0) {
        children->effects[0] = FIGHTSTG_startSpriteEffect(1000, &FIGHTSTG_changeStartPos);
        children->effects[1] = FIGHTSTG_startSpriteEffect(1001, &FIGHTSTG_changeStartPos);
    } else {
        children->effects[0] = FIGHTSTG_startSpriteEffect(1007, &FIGHTSTG_changeStartPos);
    }
}

/* Wipes the old Digimon away into its wireframe; once it's all wireframe,
   hides its model layer and starts the second effect */
static inline void wipeToWireframe(DigimonChange *task, DigimonChangeChildren *children, Models *models) {
    s32 t;

    t = GFX.funcs.getFrameTime();
    task->clip0.h -= t * 2;
    task->clip1.h += t * 2;
    task->clip1.y -= t * 2;
    if (task->clip1.y <= 0) {
        task->clip0.h = 0;
        task->clip1.h = 240;
        task->clip1.y = 0;
        models->get(models, 0)->layers[0].enabled = 0;
        children->effects[2] = FIGHTSTG_startSpriteEffect(1002, &FIGHTSTG_changeMidPos);
        task->nextSubstate(task);
    }
    setChangeClips(task);
}

/* Puts the new Digimon in the partner's place (model 1 becomes model 0) with
   both layers on, the camera on it and the clips at the wireframe whole */
static inline void swapInNewDigimon(DigimonChange *task, Models *models, BattleCamera *camera) {
    ModelControl *control;
    s32 z;

    models->setId(models, 1, 0);
    control = models->get(models, 0);
    control->layers[0].enabled = 1;
    control->layers[0].layerId = BATTLE_LAYER_MODELS;
    control->layers[0].wireframe = 0;
    control->layers[1].enabled = 1;
    control->layers[1].wireframe = 1;
    control->layers[1].layerId = BATTLE_LAYER_WIREFRAME;
    models->face(models, 0);
    camera->getFighterView(camera, 0, 9);
    z = control->homePos.z;
    z = -z;
    FIGHTSTG_fighterView.vpz += z;
    FIGHTSTG_fighterView.vrz += z;
    camera->set(camera, &FIGHTSTG_fighterView);
    control->pos.x = 0;
    control->pos.z = 0;
    control->rot.x = 0;
    control->rot.y = 0x800;
    control->rot.z = 0;
    control->pos.y = control->homePos.y;
    task->clip0.x = 0;
    task->clip0.y = 0;
    task->clip0.w = 320;
    task->clip0.h = 0;
    task->clip1.x = 0;
    task->clip1.y = 0;
    task->clip1.w = 320;
    task->clip1.h = 240;
}

/* Wipes the new Digimon in over its wireframe; once it's whole, hides the
   wireframe layer and starts the third effect */
static inline void wipeFromWireframe(DigimonChange *task, DigimonChangeChildren *children, Models *models) {
    s32 t;

    t = GFX.funcs.getFrameTime();
    task->clip0.h += t * 2;
    task->clip1.y += t * 2;
    task->clip1.h -= t * 2;
    if (task->clip0.h >= 240) {
        task->clip0.h = 240;
        task->clip1.y = 240;
        task->clip1.h = 0;
        models->get(models, 0)->layers[1].enabled = 0;
        children->effects[3] = FIGHTSTG_startSpriteEffect(task->key2 == 0 ? 1003 : 1008, &FIGHTSTG_changeEndPos);
        task->nextSubstate(task);
    }
    setChangeClips(task);
}

/* Goes back to the battle: shows model 0x10 again, gives the new Digimon its
   idle motion, turns the camera to the enemy and plays the battle music, then
   ends the change */
static inline void returnToBattle(DigimonChange *task, Models *models, BattleCamera *camera) {
    switch (task->step) {
    case 0:
    default:
        models->get(models, 0x10)->layers[0].enabled = 1;
        models->get(models, 0)->motion = task->key2 != 0 || task->idleMotion == 0 ? 1 : 2;
        camera->fade(camera, NULL, camera->getEnemyView(camera), 60);
        SOUND.playSound(BATTLE_SETUP.music);
        task->nextStep(task);
        break;
    case 1:
        task->counter += GFX.funcs.getFrameTime();
        if (task->counter >= 60) {
            task->setState(task, 3);
        }
        break;
    }
}

/* A partner Digimon's change: a stage of its own (addNewDigimon), the new
   Digimon's model in front of a sprite effect, the old one wiped away by the
   clips of BATTLE_LAYER_MODELS and BATTLE_LAYER_WIREFRAME (startChange,
   wipeToWireframe, swapInNewDigimon, wipeFromWireframe), and a fade back to
   the battle (returnToBattle). The match depends on substate 2's control
   kept in startChange, on z read and then negated, on the if/else of the
   motion, on clip1.h written before clip1.y in wipeToWireframe and on the
   layers kept in setChangeClips */
void FIGHTSTG_updateDigimonChange(DigimonChange *task, DigimonChangeChildren *children) {
    Models *models = task->models;
    BattleCamera *camera = task->camera;
    FightStage *stage = task->stage;
    TimLoader loader;

    switch (task->state) {
    case 0:
    default:
        switch (task->substate) {
        case 0:
        default:
            switch (task->step) {
            case 0:
            default:
                SOUND.loadBank(0x45);
                task->nextStep(task);
                /* fallthrough */
            case 1:
                if (SOUND.isLoading() == 0) {
                    task->nextSubstate(task);
                }
                break;
            }
            break;
        case 1:
            models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            task->models = models;
            task->camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
            task->stage = TASK_REGISTRY.funcs.find(BATTLE_TASK_STAGE, -1, -1);
            task->file = FIGHTSTG_fighterCache.funcs.getInfo(task->key1)->model >> 16;
            FILE_CACHE.request(task->file);
            task->idleMotion = models->get(models, 0)->idleMotion;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            if (FILE_CACHE.isLoading(task->file) != 0) {
                break;
            }
            FILE_CACHE.request(FILE_CHANGE);
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            if (FILE_CACHE.isLoading(FILE_CHANGE) != 0) {
                break;
            }
            initTimLoader(&loader);
            loader.setImagePos(0x300, 0x100);
            loader.loadArchive(FILE_CACHE.getEntry((FILE_CHANGE << 16) | 5));
            task->nextState(task);
            break;
        }
        break;
    case 1:
        switch (task->substate) {
        case 0:
        default:
            stage->setStage(stage, task->key2 != 0 ? 0x1F : 0x1C, 0x20, 0x20);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            addNewDigimon(task, models, stage);
            break;
        case 2:
            startChange(task, children, models, camera);
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            wipeToWireframe(task, children, models);
            break;
        case 4:
            task->counter += GFX.funcs.getFrameTime();
            if (task->counter >= 120) {
                task->nextSubstate(task);
            }
            break;
        case 5:
            switch (task->step) {
            case 0:
            default:
                children->fade = FIGHTSTG_startWhiteFlash(32);
                task->nextStep(task);
                break;
            case 1:
                if (children->fade->substate != 0) {
                    task->nextSubstate(task);
                }
                break;
            }
            break;
        case 6:
            swapInNewDigimon(task, models, camera);
            task->nextSubstate(task);
            /* fallthrough */
        case 7:
            switch (task->step) {
            case 0:
            default:
                children->fade->setState(children->fade, 2);
                task->nextStep(task);
                break;
            case 1:
                if (children->fade == NULL) {
                    task->nextSubstate(task);
                }
                break;
            }
            break;
        case 8:
            wipeFromWireframe(task, children, models);
            break;
        case 9:
            models->get(models, 0)->motion = 13;
            task->nextSubstate(task);
            /* fallthrough */
        case 10:
            task->counter += GFX.funcs.getFrameTime();
            if (task->counter >= 180) {
                task->nextSubstate(task);
            }
            break;
        case 11:
            stage->setStage(stage, BATTLE_SETUP.stage, 0x20, 0x20);
            task->nextSubstate(task);
            /* fallthrough */
        case 12:
            if (stage->state != 2) {
                models->face(models, 0);
                camera->set(camera, camera->getFighterView(camera, 0, 9));
                task->nextSubstate(task);
            }
            break;
        case 13:
            task->nextSubstate(task);
            break;
        case 14:
            returnToBattle(task, models, camera);
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts a partner's Digimon change (FIGHTSTG_updateDigimonChange) */
DigimonChange *FIGHTSTG_startDigimonChange(s32 key1, s32 key2) {
    DigimonChange *task = createTask(FIGHTSTG_updateDigimonChange, sizeof(DigimonChange), 6 * sizeof(Task *));

    task->key1 = key1;
    task->key2 = key2;
    return task;
}
