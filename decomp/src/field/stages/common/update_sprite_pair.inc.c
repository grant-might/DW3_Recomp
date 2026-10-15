/*
 * Plays the two sprites of a StageSpritePair once with the stage's
 * updateSpritePairFrames, the first 0xF0 above y and the second 0x14 below;
 * the task ends when both have ended
 */
void updateSpritePair(StageSpritePair *task) {
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
    s32 done;
    s32 i;
    s32 frame;
    s32 y;
    void (*draw)();

    switch (task->state) {
    case TASK_INIT:
    default:
        task->anims[0].index = 0;
        task->anims[0].timer = updateSpritePairFrames_0[0].duration;
        task->anims[1].index = 0;
        task->anims[1].timer = updateSpritePairFrames_1[0].duration;
        task->nextState(task);
        break;
    case TASK_RUN:
        done = 0;
        i = 0;
        draw = drawPairSprite;
        for (; i < 2; i++) {
            frame = stepAnimationOnce(&task->anims[i], updateSpritePairFrames[i], 0);
            switch (frame) {
            case 0xFF:
                task->sprites[i].frame = 0;
                done++;
                break;
            case 0x12C:
                task->sprites[i].frame = 0;
                break;
            default:
                task->sprites[i].frame = frame;
                break;
            }
            if (task->sprites[i].frame != 0) {
                y = task->y;
                if (i == 0) {
                    y -= 0xF0;
                } else {
                    y += 0x14;
                }
                layer->addSortedCallback(layer, draw, task, y, i);
            }
        }
        if (done == 2) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
