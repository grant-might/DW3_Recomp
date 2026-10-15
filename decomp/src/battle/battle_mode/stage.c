/* The second object of FIGHTSTG.PRO (see model_mesh.c), the fight stage: its
   rodata starts at 0x80082464 (USA), 4 bytes past a multiple of 8. */

#include "battle/battle_mode.h"

/* Sets the stage model's color and SCREEN_LAYER's background to their fades'
   current values (a black background is drawn as 1, 1, 1) */
void FIGHTSTG_fadeStage(FightStage *task, Model **children) {
    u8 color[3];
    Model *model;
    Layer *layer;

    FIGHTSTG_interp.lerp(&task->colorFrom, &task->colorTo, task->fade, &task->color);
    color[0] = task->color.vx;
    color[1] = task->color.vy;
    color[2] = task->color.vz;
    model = children[0];
    model->setColor(model, 1, color);
    FIGHTSTG_interp.lerp(&task->bgFrom, &task->bgTo, task->fade, &task->bg);
    layer = GFX.funcs.getLayer(SCREEN_LAYER);
    if (task->bg.vx != 0 || task->bg.vy != 0 || task->bg.vz != 0) {
        layer->setBgColor(layer, (u8)task->bg.vx, (u8)task->bg.vy, (u8)task->bg.vz);
    } else {
        layer->setBgColor(layer, 1, 1, 1);
    }
}

const Vec2 FIGHTSTG_stageTexPos = { 0x280, 0 };

/* the fight stages' music (FightStageInfo.music), as sound ids */
s32 FIGHTSTG_stageMusic[8] = {
    0xA004E03C, 0xA004E0BD, 0xA004E13E, 0xA004E1BF,
    0xA004E240, 0xA004E2C1, 0xA004E342, 0xA004E3C3,
};

/* Fades the stage's lights in. */
static inline void fadeInStageLights(FightStage *task) {
    Lights *lights = TASK_REGISTRY.funcs.find(BATTLE_TASK_LIGHTS, -1, -1);

    if (lights != NULL) {
        lights->fade(lights, NULL, lights->getStageLights(lights, task->stage), task->fadeInTime);
    }
}

/* Once the stage's model is in: turns off the bounds check of the stage's
   listed bones and starts the model and the background in black. */
static inline void prepareStageModel(FightStage *task, Model **children, FightStageInfo *stages) {
    Model *model;
    Layer *layer;
    u8 color[3];
    s32 i;

    for (i = 0; i < 8; i++) {
        if (stages[task->stage].noBoundsBones[i] == 0) {
            break;
        }
        model = children[0];
        model->setBoneNoBoundsCheck(model, stages[task->stage].noBoundsBones[i], 1);
    }
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    model = children[0];
    model->setColor(model, 1, color);
    layer = GFX.funcs.getLayer(SCREEN_LAYER);
    layer->setBgColor(layer, 1, 1, 1);
}

/* Sets the fade of the model's color and the background in from black. */
static inline void startStageFadeIn(FightStage *task, FightStageInfo *stages) {
    task->colorTo.vz = 0x80;
    task->colorTo.vy = 0x80;
    task->colorTo.vx = 0x80;
    task->colorFrom.vz = 0;
    task->colorFrom.vy = 0;
    task->colorFrom.vx = 0;
    task->bgTo.vx = stages[task->stage].bgColor[0];
    task->bgTo.vy = stages[task->stage].bgColor[1];
    task->bgTo.vz = stages[task->stage].bgColor[2];
    task->bgFrom.vz = 0;
    task->bgFrom.vy = 0;
    task->bgFrom.vx = 0;
    task->fade = 0;
    task->fadeStep = 0x1000 / task->fadeInTime;
}

/* Moves the fade on by the frame's time, up to its end (0x1000). */
static inline void advanceStageFade(FightStage *task) {
    task->fade += task->fadeStep * GFX.funcs.getFrameTime();
    if (task->fade > 0x1000) {
        task->fade = 0x1000;
    }
}

/* Sets the fade of the model's color and the old stage's background out to
   black. */
static inline void startStageFadeOut(FightStage *task, FightStageInfo *stages) {
    task->colorFrom.vz = 0x80;
    task->colorFrom.vy = 0x80;
    task->colorFrom.vx = 0x80;
    task->colorTo.vz = 0;
    task->colorTo.vy = 0;
    task->colorTo.vx = 0;
    task->bgTo.vz = 0;
    task->bgTo.vy = 0;
    task->bgTo.vx = 0;
    task->bgFrom.vx = stages[task->prevStage].bgColor[0];
    task->bgFrom.vy = stages[task->prevStage].bgColor[1];
    task->bgFrom.vz = stages[task->prevStage].bgColor[2];
    task->fade = 0;
    task->fadeStep = 0x1000 / task->fadeOutTime;
}

/* The fight stage's task: loads the stage's model, starts its music and fades
   in its lights, then fades the model's color and the background in from
   black; setStage fades them out, frees the old model and stops its music,
   and loads the new stage */
void FIGHTSTG_updateStage(FightStage *task, Model **children) {
    FightStageInfo *stages = FILE_CACHE.load(FILE_FIGHT_STAGES);

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            task->control.layers[0].enabled = 1;
            task->control.layers[0].layerId = BATTLE_LAYER_STAGE;
            task->control.fighter = 0;
            task->control.layers[0].wireframe = 0;
            children[0] = FIGHTSTG_createPlainModel(stages[task->stage].model, stages[task->stage].motions, FIGHTSTG_stageTexPos, &task->control);
            if (stages[task->stage].music != -1) {
                task->voice = SOUND.playSound(FIGHTSTG_stageMusic[stages[task->stage].music]);
            }
            fadeInStageLights(task);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            if (children[0]->state == TASK_RUN) {
                prepareStageModel(task, children, stages);
                task->nextSubstate(task);
            }
            break;
        case 2:
            startStageFadeIn(task, stages);
            task->nextSubstate(task);
            /* fallthrough */
        case 3:
            advanceStageFade(task);
            FIGHTSTG_fadeStage(task, children);
            if (task->fade == 0x1000) {
                task->nextSubstate(task);
            }
            break;
        case 4:
            break;
        }
        break;
    case TASK_DONE:
        switch (task->substate) {
        case 0:
        default:
            FILE_CACHE.request((s16)(stages[task->stage].model >> 16));
            startStageFadeOut(task, stages);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            advanceStageFade(task);
            FIGHTSTG_fadeStage(task, children);
            if (task->fade == 0x1000) {
                children[0]->setState(children[0], TASK_KILL);
                task->setState(task, TASK_RUN);
                if (stages[task->prevStage].music != -1) {
                    SOUND.keyOff(FIGHTSTG_stageMusic[stages[task->prevStage].music], task->voice);
                }
            }
            break;
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* FightStage.setStage: changes to stage ID, fading the old one out over
   FADEOUTTIME and the new one in over FADEINTIME */
void FIGHTSTG_setStage(FightStage *task, s32 id, s32 fadeOutTime, s32 fadeInTime) {
    if (task->stage != id) {
        task->prevStage = task->stage;
        task->stage = id;
        task->fadeOutTime = fadeOutTime;
        task->fadeInTime = fadeInTime;
        task->setState(task, TASK_DONE);
    }
}

/* A random fight stage, 1-27 */
s32 FIGHTSTG_randomStage(void) {
    return RANDOM.next() % 27 + 1;
}

/* The file of fight stage ID's motions */
s32 FIGHTSTG_getStageMotionsFile(s32 id) {
    FightStageInfo *stages = FILE_CACHE.load(FILE_FIGHT_STAGES);

    return (s16)(stages[id].motions >> 16);
}

/* Creates the fight stage (id 0x15), which loads stage id and fades it in
   over fadeInTime */
FightStage *FIGHTSTG_createStage(s32 id, s32 fadeInTime) {
    FightStage *task = createTaskWithId(FIGHTSTG_updateStage, sizeof(FightStage), 4, BATTLE_TASK_STAGE);

    task->setStage = FIGHTSTG_setStage;
    task->stage = id;
    task->fadeInTime = fadeInTime;
    return task;
}
