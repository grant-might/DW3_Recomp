/* FIGHTSTG's shot camera, which goes through lists of shots while the player
   picks a command. */

#include "battle/battle_mode.h"

/* FIGHTSTG_updateShotCamera's shots: how long each lasts and the substate that makes it,
   in four lists that end with a time of -1 */
CameraShot FIGHTSTG_cameraShots[4][6] = {
    { { 1800, 1 }, { 480, 9 }, { 360, 3 }, { 300, 4 }, { 480, 9 }, { -1, 0 } },
    { { 360, 6 }, { 900, 1 }, { 300, 5 }, { 480, 9 }, { -1, 0 }, { 0, 0 } },
    { { 1920, 1 }, { 360, 6 }, { 180, 7 }, { 480, 9 }, { -1, 0 }, { 0, 0 } },
    { { 90, 10 }, { 90, 5 }, { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
};
/* the lists that can follow each one */
u8 FIGHTSTG_nextShotLists[8][3] = {
    { 1, 2, 1 }, { 0, 2, 2 }, { 0, 1, 0 }, { 0, 1, 2 },
};

/* Shows both models again and starts the list's next shot, or the first
   shot of a list picked at random among those that can follow. */
static inline void startNextShot(ShotCamera *task) {
    task->models->get(task->models, 0)->layers[0].enabled = 1;
    task->models->get(task->models, 0x10)->layers[0].enabled = 1;
    if (FIGHTSTG_cameraShots[task->list][++task->shot].time == -1) {
        task->list = FIGHTSTG_nextShotLists[task->list][RANDOM.next() % 3];
        task->shot = 0;
    }
    task->time = FIGHTSTG_cameraShots[task->list][task->shot].time;
    task->setSubstate(task, FIGHTSTG_cameraShots[task->list][task->shot].substate);
}

/* Shot 1: turns around the fighters from the enemy's view, going on from
   where the last turn stopped (in stage 4, a half turn ends the shot). */
static inline void turnAroundFighters(ShotCamera *task) {
    if (task->step == 0) {
        task->view = task->camera->getEnemyView(task->camera);
        task->turned = 0;
        task->step++;
        if ((BATTLE_SETUP.stage & 0xF) != 4) {
            task->view->rot.vy = task->ry;
        }
    }
    task->view->rot.vy += GFX.funcs.getFrameTime() * 2;
    task->turned += GFX.funcs.getFrameTime() * 2;
    if (task->view->rot.vy >= 0x1000) {
        task->view->rot.vy -= 0x1000;
    }
    task->ry = task->view->rot.vy;
    if ((BATTLE_SETUP.stage & 0xF) == 4 && task->turned > 0x800) {
        task->time = 0;
    }
}

/* Shot 2: a fixed view, set off to the side of the enemy's view. */
static inline void viewFromSide(ShotCamera *task) {
    task->view = task->camera->getEnemyView(task->camera);
    task->view->vpx = -0x1E80;
    task->view->vpy = -0x500;
    task->view->vpz = 0;
    task->view->vrx = 0;
    task->view->vry = 0x500;
    task->view->vrz = 0;
    task->view->proj = 0x98;
}

/* Shot 9: a turning view, set high above the enemy's view. */
static inline void viewFromAbove(ShotCamera *task) {
    task->view = task->camera->getEnemyView(task->camera);
    task->view->vpx = -0x1400;
    task->view->vpy = -0x2800;
    task->view->vpz = 0;
    task->view->vrx = 0;
    task->view->vry = 0;
    task->view->vrz = 0;
    task->view->proj = 0xC8;
    task->speed = 1;
    task->view->rot.vy -= 0x155;
}

/* Views the partner's side, with the enemy's model hidden. */
static inline void viewPartnerSide(ShotCamera *task) {
    task->models->get(task->models, 0x10)->layers[0].enabled = 0;
    task->view = task->camera->getFighterView(task->camera, 0, 8);
    task->view->tz -= 0x1400;
    task->view->vpz += 0x1400;
    task->view->vrz += 0x1400;
    task->speed = 1;
}

/* Views the enemy's side, with the partner's model hidden. */
static inline void viewEnemySide(ShotCamera *task) {
    task->models->get(task->models, 0)->layers[0].enabled = 0;
    task->view = task->camera->getFighterView(task->camera, 0x10, 0);
    task->view->tz += 0x1400;
    task->view->vpz -= 0x1400;
    task->view->vrz -= 0x1400;
    task->speed = 1;
}

/* Turns the view around at the shot's speed. */
static inline void spinView(ShotCamera *task) {
    task->view->rot.vy += GFX.funcs.getFrameTime() * task->speed;
    if (task->view->rot.vy >= 0x1000) {
        task->view->rot.vy -= 0x1000;
    }
}

/* Turns the view around the other way at the shot's speed. */
static inline void spinViewBack(ShotCamera *task) {
    task->view->rot.vy -= GFX.funcs.getFrameTime() * task->speed;
    if (task->view->rot.vy < 0) {
        task->view->rot.vy += 0x1000;
    }
}

/* A camera that plays lists of shots (FIGHTSTG_cameraShots): turns around the
   fighters, fixed views and views of one side with the other side's model
   hidden */
void FIGHTSTG_updateShotCamera(ShotCamera *task) {
    switch (task->state) {
    case 0:
    default:
        task->nextState(task);
        task->list = 3;
        task->shot = 0;
        task->time = FIGHTSTG_cameraShots[task->list][task->shot].time;
        task->setSubstate(task, FIGHTSTG_cameraShots[task->list][task->shot].substate);
        task->camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
        task->models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        task->view = task->camera->getEnemyView(task->camera);
        task->ry = task->view->rot.vy;
        break;
    case 1:
        if (task->time <= 0) {
            startNextShot(task);
        }
        switch (task->substate) {
        case 1:
        default:
            turnAroundFighters(task);
            break;
        case 2:
            if (task->step == 0) {
                viewFromSide(task);
                task->step++;
            }
            break;
        case 3:
            if (task->step == 0) {
                viewPartnerSide(task);
                task->step++;
            }
            spinView(task);
            break;
        case 4:
            if (task->step == 0) {
                viewEnemySide(task);
                task->step++;
            }
            spinViewBack(task);
            break;
        case 5:
            if (task->step == 0) {
                viewPartnerSide(task);
                task->step++;
                task->view->rot.vy += 0xE3;
            }
            spinViewBack(task);
            break;
        case 6:
            if (task->step == 0) {
                viewEnemySide(task);
                task->step++;
                task->view->rot.vy -= 0xE3;
            }
            spinView(task);
            break;
        case 7:
            if (task->step == 0) {
                task->models->get(task->models, 0x10)->layers[0].enabled = 0;
                task->view = task->camera->getFighterView(task->camera, 0, 10);
                task->step++;
            }
            break;
        case 8:
            if (task->step == 0) {
                task->models->get(task->models, 0x10)->layers[0].enabled = 0;
                task->view = task->camera->getFighterView(task->camera, 0, 8);
                task->step++;
            }
            break;
        case 9:
            if (task->step == 0) {
                viewFromAbove(task);
                task->step++;
            }
            spinView(task);
            break;
        case 10:
            if (task->step == 0) {
                task->view = task->camera->getEnemyView(task->camera);
                task->step++;
            }
            break;
        }
        if (task->view != NULL) {
            task->camera->set(task->camera, task->view);
        }
        task->time -= GFX.funcs.getFrameTime();
        break;
    case 3:
        task->models->get(task->models, 0)->layers[0].enabled = 1;
        task->models->get(task->models, 0x10)->layers[0].enabled = 1;
        task->view = task->camera->getEnemyView(task->camera);
        task->camera->set(task->camera, task->view);
        break;
    case 2:
        break;
    }
}

/* Starts the camera that plays lists of shots (FIGHTSTG_updateShotCamera) */
ShotCamera *FIGHTSTG_createShotCamera(void) {
    return createTask(FIGHTSTG_updateShotCamera, sizeof(ShotCamera), 0);
}
