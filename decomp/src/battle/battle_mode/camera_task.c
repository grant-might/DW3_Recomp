/* The last object of FIGHTSTG.PRO (see model_mesh.c). The USA version has the
   task of FIGHTSTG_updateCameraTurn here, which the European one has in
   battle_script.c; the European version has a task of its own here
   instead, whose jump table starts its rodata at 0x800832B8. */

#include "battle/battle_mode.h"

#if VERSION_US
#include "camera_turn.h"
#elif VERSION_EU || VERSION_JP
/* the lists of shots of the European version's own camera (FIGHTSTG_updateCameraShots) */
CameraShot FIGHTSTG_euCameraShots[3][3] = {
    { { 64, 6 }, { 2, 3 }, { -1, 0 } },
    { { 32, 1 }, { 32, 4 }, { -1, 0 } },
    { { 32, 2 }, { 32, 4 }, { -1, 0 } },
};

/* A camera of the European version's own: one of the three lists of shots of
   FIGHTSTG_euCameraShots, picked at random, from the views of the fighters and
   the enemy, fades to them and a turn, then the enemy's view. The match
   depends on the turn's time * 32, which written as a shift reads only the
   time's low half */
void FIGHTSTG_updateCameraShots(CameraShots *task) {
    switch (task->state) {
    case 0:
    default:
        task->nextState(task);
        task->list = RANDOM.next() & 3;
        if (task->list == 3) {
            task->list = 0;
        }
        task->shot = 0;
        task->time = FIGHTSTG_euCameraShots[task->list][task->shot].time;
        task->setSubstate(task, FIGHTSTG_euCameraShots[task->list][task->shot].substate);
        task->camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
        task->models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        task->view = task->camera->getEnemyView(task->camera);
        /* fallthrough */
    case 1:
        if (task->time <= 0) {
            if (FIGHTSTG_euCameraShots[task->list][++task->shot].time == -1) {
                task->setState(task, 3);
                break;
            }
            task->time = FIGHTSTG_euCameraShots[task->list][task->shot].time;
            task->setSubstate(task, FIGHTSTG_euCameraShots[task->list][task->shot].substate);
        }
        switch (task->substate) {
        case 1:
            task->view = task->camera->getFighterView(task->camera, 0x10, 0);
            task->camera->set(task->camera, task->view);
            task->substate = 0;
            break;
        case 2:
            task->view = task->camera->getFighterView(task->camera, 0, 8);
            task->camera->set(task->camera, task->view);
            task->substate = 0;
            break;
        case 3:
            task->view = task->camera->getEnemyView(task->camera);
            task->camera->set(task->camera, task->view);
            task->substate = 0;
            break;
        case 4:
            task->to = *task->camera->getEnemyView(task->camera);
            task->camera->fade(task->camera, NULL, &task->to, task->time);
            task->substate = 0;
            break;
        case 5:
            task->to = *task->camera->getFighterView(task->camera, 0, 10);
            task->camera->fade(task->camera, NULL, &task->to, task->time);
            task->substate = 0;
            break;
        case 6:
            if (task->step == 0) {
                task->view = task->camera->getEnemyView(task->camera);
                task->rx = task->view->rot.vx;
                task->ry = task->view->rot.vy;
                task->view->rot.vx = task->rx - 0x155;
                task->view->rot.vy = task->ry + 0x800;
                task->step++;
            }
            task->view->rot.vx = task->rx - task->time * 0x155 / 64;
            task->view->rot.vy = task->ry + task->time * 32;
            task->camera->set(task->camera, task->view);
            break;
        }
        task->time -= GFX.funcs.getFrameTime();
        break;
    case 3:
        task->view = task->camera->getEnemyView(task->camera);
        task->camera->set(task->camera, task->view);
        break;
    case 2:
        break;
    }
}

/* Starts the European version's own camera */
CameraShots *FIGHTSTG_startCameraShots(void) {
    return createTask(FIGHTSTG_updateCameraShots, sizeof(CameraShots), 0);
}
#endif
