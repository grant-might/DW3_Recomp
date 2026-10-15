/* FIGHTSTG's fighter cameras, which a fighter's model control moves. */

#include "battle/battle_mode.h"

/* Sets layer 0x1009's view to the fighter camera's, with its rotation and
   translation, and counts down one of the frames it is set for */
void FIGHTSTG_applyCamera(FighterCamera *task) {
    Layer *layer;

    RotMatrixYXZ_gte(&task->rot, &task->coord.coord);
    task->coord.flg = 0;
    task->view.super = &task->coord;
    task->coord.coord.t[0] = task->trans.vx;
    task->coord.coord.t[1] = task->trans.vy;
    task->coord.coord.t[2] = task->trans.vz;
    GsSetRefView2(&task->view);
    layer = GFX.funcs.getLayer(0x1009);
    layer->setKeepView(layer, 1, task->proj);
    task->frames--;
}

/* Loads a fighter camera's view from its fighter's camera 6, or 7 while the
   model has an idle motion, with no rotation or translation */
void FIGHTSTG_loadCamera(FighterCamera *task) {
    FighterInfo *info = FIGHTSTG_fighterCache.funcs.getInfo(task->fighter);
    s32 i = 6;

    if (task->control->idleMotion != 0) {
        i = 7;
    }
    task->view.vpx = info->camPos[i].x;
    task->view.vpy = -info->camPos[i].y;
    task->view.vpz = -info->camPos[i].z;
    task->view.vrx = info->camRef[i].x;
    task->view.vry = -info->camRef[i].y;
    task->view.vrz = -info->camRef[i].z;
    task->proj = info->camProj[i];
    task->rot.vx = 0;
    task->rot.vy = 0;
    task->rot.vz = 0;
    task->trans.vx = 0;
    task->trans.vy = 0;
    task->trans.vz = 0;
}

/* The fighter camera's task: once layer 0x1009 is there, loads the view and
   sets it for two frames */
void FIGHTSTG_updateCamera(FighterCamera *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->step) {
        case 0:
        default:
            if (GFX.funcs.getLayer(0x1009) != NULL) {
                FIGHTSTG_loadCamera(task);
                task->frames = 2;
                task->nextStep(task);
            }
            break;
        case 1:
            break;
        }
        if (task->frames != 0) {
            FIGHTSTG_applyCamera(task);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the camera of a fighter of the partner view, on control's model */
FighterCamera *FIGHTSTG_createCamera(s32 fighter, ModelControl *control) {
    FighterCamera *task = createTask(FIGHTSTG_updateCamera, sizeof(FighterCamera), 0);

    task->fighter = fighter;
    task->control = control;
    return task;
}
