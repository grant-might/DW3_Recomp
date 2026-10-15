/* The mode's root task: sets up the 320x240 display with SCENE_PRIM_BUFFERS
   bytes of primitives and a black layer (SCENE_OT_SHIFT), then creates the
   mode's main task (SCENE_CREATE), its only child */
void OVL_NAME(updateScene)(SCENE_TASK *task, SCENE_CHILD **child) {
    RECT rect;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        GFX.funcs.reset();
        GFX.funcs.allocPrimBuffers(SCENE_PRIM_BUFFERS);
        GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
        rect.x = 0;
        rect.y = 0;
        rect.w = SCREEN_WIDTH;
        rect.h = SCREEN_HEIGHT;
        layer = GFX.funcs.createLayer(&rect, SCENE_OT_SHIFT, SCREEN_LAYER);
        layer->setBgColor(layer, 0, 0, 0);
        *child = SCENE_CREATE();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
