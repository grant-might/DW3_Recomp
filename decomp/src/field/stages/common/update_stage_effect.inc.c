/*
 * A StageEffect's update: its sprite with its palette animated
 * (effectClutFrames) while it runs, and the one-shot animation
 * (effectFrames) above it once done, until it ends
 */
void updateStageEffect(StageEffect *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    s32 done;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->key1 = task->x;
        task->key2 = task->y;
        task->anim.index = 0;
        task->anim.timer = effectFrames[0].duration;
        task->clutAnim.index = 0;
        task->clutAnim.timer = effectClutFrames[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        task->sprites[0].frame = task->frame;
        task->sprites[0].clutRow = stepAnimation(&task->clutAnim, effectClutFrames, 0, 0);
        if (task->sprites[0].frame != 0 && isOnScreen(task->x, task->y, 0x20, 0x20)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y, 0);
        }
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->anim.index = 0;
            task->anim.timer = effectFrames[0].duration;
            task->clutAnim.index = 0;
            task->clutAnim.timer = effectClutFrames[0].duration;
            task->setSubstate(task, 1);
        }
        task->sprites[0].frame = task->frame;
        task->sprites[0].clutRow = stepAnimation(&task->clutAnim, effectClutFrames, 0, 0);
        done = 0;
        frame = stepAnimation(&task->anim, effectFrames, 1, 0);
        switch (frame) {
        case 0xFF:
            task->sprites[1].frame = 0;
            done = 1;
            break;
        case 0x12C:
            task->sprites[1].frame = 0;
            break;
        default:
            task->sprites[1].frame = task->frame + frame;
            break;
        }
        if (done) {
            task->setState(task, TASK_RUN);
        }
        if (task->sprites[0].frame != 0 && isOnScreen(task->x, task->y, 0x20, 0x20)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y, 0);
        }
        if (task->sprites[1].frame != 0 && isOnScreen(task->x, task->y - 0x20, 0x20, 0x40)) {
            layer->addSortedCallback(layer, drawStageEffect, task, task->y + 0x12, 1);
        }
        break;
    case TASK_KILL:
        break;
    }
}
