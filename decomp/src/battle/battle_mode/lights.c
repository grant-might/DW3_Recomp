/* FIGHTSTG's stage lights. */

#include "battle/battle_mode.h"

/* The stage lights' task: steps their fade, then sets the three flat lights
   and the ambient color for the layer each frame */
void FIGHTSTG_updateLights(Lights *task) {
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    Layer *layer;
    s32 t;
    s32 i;
    s32 j;

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
                task->t += task->tStep * GFX.funcs.getFrameTime();
                if (task->t < 0x1000) {
                    t = task->t;
                    for (i = 0; i < 3; i++) {
                        from.vx = task->from.lights[i].vx;
                        from.vy = task->from.lights[i].vy;
                        from.vz = task->from.lights[i].vz;
                        to.vx = task->to.lights[i].vx;
                        to.vy = task->to.lights[i].vy;
                        to.vz = task->to.lights[i].vz;
                        FIGHTSTG_interp.lerp(&from, &to, t, &out);
                        task->current.lights[i].vx = out.vx;
                        task->current.lights[i].vy = out.vy;
                        task->current.lights[i].vz = out.vz;
                        from.vx = task->from.lights[i].r;
                        from.vy = task->from.lights[i].g;
                        from.vz = task->from.lights[i].b;
                        to.vx = task->to.lights[i].r;
                        to.vy = task->to.lights[i].g;
                        to.vz = task->to.lights[i].b;
                        FIGHTSTG_interp.lerp(&from, &to, t, &out);
                        task->current.lights[i].r = out.vx;
                        task->current.lights[i].g = out.vy;
                        task->current.lights[i].b = out.vz;
                    }
                    from.vx = task->from.ambient[0];
                    from.vy = task->from.ambient[1];
                    from.vz = task->from.ambient[2];
                    to.vx = task->to.ambient[0];
                    to.vy = task->to.ambient[1];
                    to.vz = task->to.ambient[2];
                    FIGHTSTG_interp.lerp(&from, &to, t, &out);
                    task->current.ambient[0] = out.vx;
                    task->current.ambient[1] = out.vy;
                    task->current.ambient[2] = out.vz;
                } else {
                    task->t = 0x1000;
                    task->current = task->to;
                }
            }
            GsSetLightMode(0);
            for (j = 0; j < 3; j++) {
                GsSetFlatLight(j, &task->current.lights[j]);
            }
            SetBackColor(task->current.ambient[0], task->current.ambient[1], task->current.ambient[2]);
            layer = GFX.funcs.getLayer(task->layerId);
            layer->setKeepLightMatrix(layer, 1);
            if (task->t == 0x1000) {
                task->nextSubstate(task);
            }
            break;
        }
        break;
    case TASK_KILL:
        layer = GFX.funcs.getLayer(task->layerId);
            layer->setKeepLightMatrix(layer, 0);
        break;
    }
}

/* Lights.set: switches to SET at once */
void FIGHTSTG_setLights(Lights *task, LightSet *set) {
    task->current = *set;
    task->setState(task, TASK_DONE);
}

/* Lights.fade: fades from FROM (NULL for the current lights) to TO over TIME */
void FIGHTSTG_fadeLights(Lights *task, LightSet *from, LightSet *to, s32 time) {
    if (from != NULL) {
        task->from = *from;
    } else {
        task->from = task->current;
    }
    task->to = *to;
    task->tStep = 0x1000 / time;
    task->t = task->tStep * GFX.funcs.getFrameTime();
    task->setState(task, TASK_DONE);
}

/* Lights.getStageLights: fight stage STAGE's lights */
LightSet *FIGHTSTG_getStageLights(Lights *task, s32 stage) {
    FightStageInfo *stages = FILE_CACHE.load(FILE_FIGHT_STAGES);

    return &stages[stage].lights;
}

/* Creates the stage lights (id 0x13) for layer layerId */
Lights *FIGHTSTG_createLights(s32 layerId) {
    Lights *task = createTaskWithId(FIGHTSTG_updateLights, sizeof(Lights), 0, BATTLE_TASK_LIGHTS);
    task->set = FIGHTSTG_setLights;
    task->fade = FIGHTSTG_fadeLights;
    task->layerId = layerId;
    task->getStageLights = FIGHTSTG_getStageLights;
    return task;
}
