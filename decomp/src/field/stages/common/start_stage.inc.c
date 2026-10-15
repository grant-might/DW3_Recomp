/*
 * FIELDSTG's start of the stage (StageEntry.init in FIELDSTG_stages): the
 * stage's task, updated by updateStage, with STAGE_CHILDREN_SIZE bytes of
 * children (the stage defines it if they are any), then the stage's setup.
 * FIELDSTG links it as WSTAGnnn_startStage (include/field/stages.h) and keeps the
 * task as a plain Task, the same for every stage.
 */

#ifndef STAGE_CHILDREN_SIZE
#define STAGE_CHILDREN_SIZE 0
#endif
StageTask *startStage(void *owner) {
    StageTask *task = createTask(updateStage, sizeof(StageTask), STAGE_CHILDREN_SIZE);

    task->owner = owner;
#ifdef STAGE_TWEEN
    stageFuncs.setup();
#else
    stageFuncs[0]();
#endif
    return task;
}
