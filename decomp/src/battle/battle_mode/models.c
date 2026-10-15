/* FIGHTSTG's models task (FIGHTSTG_createModels): the fighters' models and
   their controls. */

#include "battle/battle_mode.h"

/* The fighters' models' task: nothing to do but start */
void FIGHTSTG_updateModels(Models *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The slot of the fighter model registered as ID, or -1 */
s32 FIGHTSTG_findModelSlot(Models *task, s32 id) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (task->controls[i].active && task->controls[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* A free fighter model slot, or -1 */
s32 FIGHTSTG_findFreeModelSlot(Models *task) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (!task->controls[i].active) {
            return i;
        }
    }
    return -1;
}

/* Models.remove: kills the model registered as ID and frees its slot */
void FIGHTSTG_removeFighterModel(Models *task, s32 id) {
    ModelsChildren *children = task->children;
    s32 i = FIGHTSTG_findModelSlot(task, id);

    if (i != -1) {
        children->dying[i] = children->models[i];
        children->models[i] = NULL;
        children->dying[i]->setState(children->dying[i], TASK_KILL);
        task->controls[i].active = 0;
    }
}

/* the fighters' models' texture places */
Vec2 FIGHTSTG_fighterTexPos[] = {
    { 832, 0 }, { 896, 0 }, { 960, 0 }, { 960, 256 },
};

/* Models.add: replaces the model registered as ID with FIGHTER's, in a free
   slot with that slot's textures, drawn on layer BATTLE_LAYER_MODELS when VISIBLE */
void FIGHTSTG_addFighterModel(Models *task, s32 id, s32 fighter, s32 visible) {
    ModelsChildren *children = task->children;
    FighterInfo *info;
    ModelControl *control;
    s32 i;

    FIGHTSTG_removeFighterModel(task, id);
    i = FIGHTSTG_findFreeModelSlot(task);
    if (i != -1) {
        info = FIGHTSTG_fighterCache.funcs.getInfo(fighter);
        control = &task->controls[i];
        children->models[i] = FIGHTSTG_createIdlingModel(info->model, info->motions, FIGHTSTG_fighterTexPos[i], control);
        HEAP.zero(control, sizeof(ModelControl));
        task->controls[i].active = 1;
        task->controls[i].motion = 1;
        task->controls[i].fighter = fighter;
        task->controls[i].id = id;
        task->controls[i].layers[0].enabled = visible;
        task->controls[i].layers[0].wireframe = 0;
        task->controls[i].layers[0].layerId = BATTLE_LAYER_MODELS;
    }
}

/* Models.get: the control of the model registered as ID, or NULL */
ModelControl *FIGHTSTG_getModelControl(Models *task, s32 id) {
    s32 i = FIGHTSTG_findModelSlot(task, id);

    if (i != -1) {
        return &task->controls[i];
    }
    return NULL;
}

/* Models.setId: registers the model ID as NEWID instead, removing the one
   that had it */
void FIGHTSTG_setModelId(Models *task, s32 id, s32 newId) {
    FIGHTSTG_removeFighterModel(task, newId);
    task->controls[FIGHTSTG_findModelSlot(task, id)].id = newId;
}

/* Models.getFighter: the fighter of the model registered as ID, or 0 */
s32 FIGHTSTG_getModelFighter(Models *task, s32 id) {
    s32 i = FIGHTSTG_findModelSlot(task, id);

    if (i != -1) {
        return task->controls[i].fighter;
    }
    return 0;
}

/* Models.face: puts the model registered as ID at its side's place, facing
   the other side, as its home */
void FIGHTSTG_faceModel(Models *task, s32 id) {
    ModelControl *control = FIGHTSTG_getModelControl(task, id);
    FighterInfo *info;
    s16 z;

    if (control != NULL) {
        info = FIGHTSTG_fighterCache.funcs.getInfo(control->fighter);
        if (id < 0x10) {
            control->pos.x = 0;
            control->pos.y = -info->height;
            z = -0x1400 - info->distance;
            control->rot.y = 0x800;
            control->rot.x = 0;
            control->rot.z = 0;
        } else {
            control->pos.x = 0;
            control->pos.y = -info->height;
            z = info->distance + 0x1400;
            control->rot.x = 0;
            control->rot.y = 0;
            control->rot.z = 0;
        }
        control->pos.z = z;
        control->homePos = control->pos;
        control->homeRot = control->rot;
    }
}

/* Models.setIdleMotion */
void FIGHTSTG_setModelIdleMotion(Models *task, s32 id, s32 motion) {
    ModelControl *control = FIGHTSTG_getModelControl(task, id);

    if (control != NULL) {
        control->idleMotion = motion;
    }
}

/* Creates the fighters' models (Models), with no fighters yet */
Models *FIGHTSTG_createModels(void) {
    Models *task = createTaskWithId(FIGHTSTG_updateModels, sizeof(Models), sizeof(ModelsChildren), BATTLE_TASK_MODELS);
    s32 i;

    for (i = 3; i >= 0; i--) {
        task->controls[i].active = 0;
    }
    task->add = FIGHTSTG_addFighterModel;
    task->getFighter = FIGHTSTG_getModelFighter;
    task->remove = FIGHTSTG_removeFighterModel;
    task->get = FIGHTSTG_getModelControl;
    task->setId = FIGHTSTG_setModelId;
    task->face = FIGHTSTG_faceModel;
    task->setIdleMotion = FIGHTSTG_setModelIdleMotion;
    return task;
}
