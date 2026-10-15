/* FIGHTSTG's battle camera and its views of the fighters. */

#include "battle/battle_mode.h"

/* The battle camera's task: fades its view from `from` to `to` (position,
   reference, translation, rotation, roll and projection) by tStep a frame,
   sets it on its layer every frame and goes to the next substate once there;
   set and fade restart it through TASK_DONE */
void FIGHTSTG_updateBattleCamera(BattleCamera *task) {
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    GsCOORDINATE2 coord;
    GsRVIEW2 view;
    Layer *layer;
    s32 t;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->t = 0x1000;
        task->nextState(task);
        break;
    case TASK_DONE:
        task->setState(task, TASK_RUN);
        task->setSubstate(task, 1);
        /* fallthrough */
    case TASK_RUN:
        switch (task->substate) {
        case 1:
        case 2:
            if (task->t != 0x1000) {
                task->t += (task->tStep >> 8) * GFX.funcs.getFrameTime();
                t = task->t;
                if (t < 0x1000) {
                    from.vx = task->from.vpx;
                    from.vy = task->from.vpy;
                    from.vz = task->from.vpz;
                    to.vx = task->to.vpx;
                    to.vy = task->to.vpy;
                    to.vz = task->to.vpz;
                    FIGHTSTG_interp.lerp(&from, &to, t, &out);
                    task->current.vpx = out.vx;
                    task->current.vpy = out.vy;
                    task->current.vpz = out.vz;
                    from.vx = task->from.vrx;
                    from.vy = task->from.vry;
                    from.vz = task->from.vrz;
                    to.vx = task->to.vrx;
                    to.vy = task->to.vry;
                    to.vz = task->to.vrz;
                    FIGHTSTG_interp.lerp(&from, &to, t, &out);
                    task->current.vrx = out.vx;
                    task->current.vry = out.vy;
                    task->current.vrz = out.vz;
                    from.vx = task->from.tx;
                    from.vy = task->from.ty;
                    from.vz = task->from.tz;
                    to.vx = task->to.tx;
                    to.vy = task->to.ty;
                    to.vz = task->to.tz;
                    FIGHTSTG_interp.lerp(&from, &to, t, &out);
                    task->current.tx = out.vx;
                    task->current.ty = out.vy;
                    task->current.tz = out.vz;
                    FIGHTSTG_interp.lerp(&task->from.rot, &task->to.rot, t, &out);
                    task->current.rot.vx = out.vx;
                    task->current.rot.vy = out.vy;
                    task->current.rot.vz = out.vz;
                    from.vx = task->from.rz;
                    from.vy = task->from.proj;
                    from.vz = 0;
                    to.vx = task->to.rz;
                    to.vy = task->to.proj;
                    to.vz = 0;
                    FIGHTSTG_interp.lerp(&from, &to, t, &out);
                    task->current.rz = out.vx;
                    task->current.proj = out.vy;
                } else {
                    task->t = 0x1000;
                    task->current = task->to;
                }
            }
            RotMatrixYXZ_gte(&task->current.rot, &coord.coord);
            coord.coord.t[0] = task->current.tx;
            coord.coord.t[1] = task->current.ty;
            coord.coord.t[2] = task->current.tz;
            coord.flg = 0;
            coord.param = NULL;
            coord.super = NULL;
            coord.sub = NULL;
            view.vpx = task->current.vpx;
            view.vpy = task->current.vpy;
            view.vpz = task->current.vpz;
            view.vrx = task->current.vrx;
            view.vry = task->current.vry;
            view.vrz = task->current.vrz;
            view.rz = task->current.rz << 12;
            view.super = &coord;
            GsSetRefView2(&view);
            layer = GFX.funcs.getLayer(task->layerId);
            layer->setKeepView(layer, 1, task->current.proj);
            if (task->t == 0x1000) {
                task->nextSubstate(task);
            }
            break;
        }
        break;
    case TASK_KILL:
        layer = GFX.funcs.getLayer(task->layerId);
        layer->setKeepView(layer, 0, 0);
        break;
    }
}

/* BattleCamera.set: puts the battle camera at view */
void FIGHTSTG_setBattleCameraView(BattleCamera *task, CameraView *view) {
    task->current = *view;
    task->t = 0x1000;
    task->setState(task, TASK_DONE);
}

/* BattleCamera.fade: fades the battle camera from `from` (or where it is) to
   `to` over time frames */
void FIGHTSTG_fadeBattleCamera(BattleCamera *task, CameraView *from, CameraView *to, s32 time) {
    if (from != NULL) {
        task->from = *from;
    } else {
        task->from = task->current;
    }
    task->to = *to;
    task->tStep = 0x100000 / time;
    task->t = 0;
    task->setState(task, TASK_DONE);
}

/* The view of a fighter's camera (id as Models.getFighter takes it), which ends
   the battle camera's fade */
CameraView *FIGHTSTG_getFighterView(BattleCamera *task, s32 id, s32 camera) {
    Models *models;
    s32 fighter;
    FighterInfo *info;
    FighterInfoEnemy *enemy;

    /* the match depends on the do-while and its breaks, the early exit that
       the stages' event code uses too */
    do {
        models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        if (models == NULL) {
            break;
        }
        fighter = models->getFighter(models, id);
        if (fighter == 0) {
            break;
        }
        if (!(id & 0xF0)) {
            info = FIGHTSTG_fighterCache.funcs.getInfo(fighter);
            FIGHTSTG_fighterView.vpx = info->camPos[camera].x;
            FIGHTSTG_fighterView.vpy = -info->camPos[camera].y;
            FIGHTSTG_fighterView.vpz = -info->camPos[camera].z;
            FIGHTSTG_fighterView.vrx = info->camRef[camera].x;
            FIGHTSTG_fighterView.vry = -info->camRef[camera].y;
            FIGHTSTG_fighterView.vrz = -info->camRef[camera].z;
            FIGHTSTG_fighterView.proj = info->camProj[camera];
        } else {
            enemy = (FighterInfoEnemy *)FIGHTSTG_fighterCache.funcs.getInfo(fighter);
            FIGHTSTG_fighterView.vpx = enemy->camPos[camera].x;
            FIGHTSTG_fighterView.vpy = -enemy->camPos[camera].y;
            FIGHTSTG_fighterView.vpz = -enemy->camPos[camera].z;
            FIGHTSTG_fighterView.vrx = enemy->camRef[camera].x;
            FIGHTSTG_fighterView.vry = -enemy->camRef[camera].y;
            FIGHTSTG_fighterView.vrz = -enemy->camRef[camera].z;
            FIGHTSTG_fighterView.proj = enemy->camProj[camera];
        }
        FIGHTSTG_fighterView.rot.vx = 0;
        FIGHTSTG_fighterView.rot.vy = 0;
        FIGHTSTG_fighterView.rot.vz = 0;
        FIGHTSTG_fighterView.tx = 0;
        FIGHTSTG_fighterView.ty = 0;
        FIGHTSTG_fighterView.tz = 0;
        FIGHTSTG_fighterView.rz = 0;
        task->setState(task, TASK_DONE);
    } while (0);
    return &FIGHTSTG_fighterView;
}

/* The view of the enemy's last camera */
CameraView *FIGHTSTG_getEnemyView(BattleCamera *task) {
    Models *models;
    s32 fighter;

    /* the match depends on the do-while and its breaks, as FIGHTSTG_getFighterView's */
    do {
        models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        if (models == NULL) {
            break;
        }
        fighter = models->getFighter(models, 0x10);
        if (fighter == 0) {
            break;
        }
        FIGHTSTG_fighterCache.funcs.getInfo(fighter);
        FIGHTSTG_getFighterView(task, 0, ((FighterInfoEnemy *)FIGHTSTG_fighterCache.enemyInfo)->cameraCount - 1);
    } while (0);
    return &FIGHTSTG_fighterView;
}

/* Creates the battle camera (BATTLE_TASK_CAMERA) on a layer */
BattleCamera *FIGHTSTG_createBattleCamera(s32 layerId) {
    BattleCamera *task = createTaskWithId(FIGHTSTG_updateBattleCamera, sizeof(BattleCamera), 0, BATTLE_TASK_CAMERA);

    task->set = FIGHTSTG_setBattleCameraView;
    task->fade = FIGHTSTG_fadeBattleCamera;
    task->getEnemyView = FIGHTSTG_getEnemyView;
    task->layerId = layerId;
    task->getFighterView = FIGHTSTG_getFighterView;
    return task;
}
