/* FIGHTSTG's entrances: a fighter's model coming into the battle. */

#include "battle/battle_mode.h"

/* Loads what fighter 0x1D2 (its stage file) or 0x1D3 (its music, fading the
   current one out) brings, then ends the loading state. */
static inline void loadEntranceExtras(Entrance *task, s32 is1D2) {
    switch (task->step) {
    case 0:
    default:
        if (is1D2) {
            FILE_CACHE.request(FILE_ENTRANCE_1D2);
        } else {
            SOUND.fadeOut(MUSIC(0x24, 0));
            SOUND.loadBank(0x26);
        }
        task->nextStep(task);
        break;
    case 1:
        if (is1D2) {
            if (FILE_CACHE.isLoading(FILE_ENTRANCE_1D2) == 0) {
                task->nextState(task);
            }
        } else if (SOUND.isLoading() == 0) {
            task->nextState(task);
        }
        break;
    }
}

/* Gives the loaded model the fighter's id and shows it in its entrance motion,
   with the camera on it. */
static inline void showEnteringModel(Entrance *task, Models *models, BattleCamera *camera) {
    ModelControl *control;

    models->setId(models, task->key2 + 1, task->key2);
    models->face(models, task->key2);
    control = models->get(models, task->key2);
    control->layers[0].layerId = BATTLE_LAYER_MODELS;
    control->layers[0].enabled = 1;
    control->layers[0].wireframe = 0;
    control->motion = 13;
    camera->set(camera, camera->getFighterView(camera, task->key2, task->key2 != 0 ? 2 : 10));
}

/* Fighter 0x1D2 changes the fight stage and 0x1D3 the music. */
static inline void applyEntranceExtras(FightStage *stage, s32 is1D2, s32 is1D3) {
    if (is1D2) {
        BATTLE_SETUP.stage = 0x16;
        stage->setStage(stage, 0x16, 1, 1);
    }
    if (is1D3) {
        BATTLE_SETUP.music = MUSIC(0x26, 0);
        SOUND.playSound(MUSIC(0x26, 0));
    }
}

/* A fighter's entrance: loads its model and adds it, then shows it behind a fade
   (FIGHTSTG_startWhiteFlash) and turns the camera to it; fighter 0x1D2 changes the fight
   stage and 0x1D3 the music. The match depends on substate 7's own ModelControl
   pointer. */
void FIGHTSTG_updateEntrance(Entrance *task, WhiteFlash **children) {
    Models *models = task->models;
    BattleCamera *camera = task->camera;
    FightStage *stage = task->stage;
    s32 is1D2 = task->key1 == 0x1D2;
    s32 is1D3 = task->key1 == 0x1D3;
    ModelControl *entering;

    switch (task->state) {
    case 0:
    default:
        switch (task->substate) {
        case 0:
        default:
            models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            task->models = models;
            task->camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
            task->stage = TASK_REGISTRY.funcs.find(BATTLE_TASK_STAGE, -1, -1);
            task->file = FIGHTSTG_fighterCache.funcs.getInfo(task->key1)->model >> 16;
            FILE_CACHE.request(task->file);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            if (FILE_CACHE.isLoading(task->file) != 0) {
                break;
            }
            models->add(models, task->key2 + 1, task->key1, 0);
            if (!is1D2 && !is1D3) {
                task->nextState(task);
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            loadEntranceExtras(task, is1D2);
            break;
        }
        break;
    case 1:
        switch (task->substate) {
        case 0:
            camera->fade(camera, NULL, camera->getFighterView(camera, task->key2, task->key2 != 0 ? 2 : 10), 60);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            task->counter += GFX.funcs.getFrameTime();
            switch (task->step) {
            case 0:
            default:
                if (task->counter < 30) {
                    break;
                }
                models->get(models, task->key2 == 0 ? 0x10 : 0)->layers[0].enabled = 0;
                task->step++;
                /* fallthrough */
            case 1:
                if (task->counter >= 60) {
                    task->nextSubstate(task);
                }
                break;
            }
            break;
        case 2:
            SOUND.playSound(SOUND_PLAYER11);
            *children = FIGHTSTG_startWhiteFlash(60);
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            if ((*children)->substate == 0) {
                break;
            }
            task->nextSubstate(task);
            task->done = 1;
            showEnteringModel(task, models, camera);
            applyEntranceExtras(stage, is1D2, is1D3);
            break;
        case 4:
            FIGHTSTG_endWhiteFlash(*children, 60);
            task->nextSubstate(task);
            /* fallthrough */
        case 5:
            if (*children != NULL) {
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 6:
            task->step += GFX.funcs.getFrameTime();
            if (is1D2 ? task->step < 10 : task->step < 120) {
                break;
            }
            task->nextSubstate(task);
            /* fallthrough */
        case 7:
            camera->fade(camera, NULL, camera->getEnemyView(camera), task->key2 != 0 ? 1 : 60);
            entering = models->get(models, task->key2);
            if (task->weak != 0) {
                entering->motion = 2;
            } else {
                entering->motion = 1;
            }
            models->get(models, task->key2 == 0 ? 0x10 : 0)->layers[0].enabled = 1;
            task->setState(task, 3);
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts a fighter's entrance: Digimon id on the player's side (side 0) or
   the enemy's */
Entrance *FIGHTSTG_startEntrance(s32 id, s32 side, s32 weak) {
    Entrance *task = createTask(FIGHTSTG_updateEntrance, sizeof(Entrance), 0xC);

    task->key1 = id;
    if (side) {
        task->key2 = 0x10;
    } else {
        task->key2 = 0;
    }
    task->weak = weak;
    return task;
}
