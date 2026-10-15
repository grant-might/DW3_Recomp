/*
 * The map object of animation 1: frame 5 until substate 1 plays the stage's
 * updateSoundTileFrames once, then frame 10, keying off SOUND_COMCD115
 */
void updateSoundTile(StageSoundTile *task) {
    StageTile *object;
    StageTile *tile;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == 1) {
                task->obj.anim.index = 0;
                task->obj.anim.timer = updateSoundTileFrames[0].duration;
                task->obj.tile = object;
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        tile = task->obj.tile;
        switch (task->substate) {
        case 0:
        default:
            tile->frame = 5;
            tile->visible = 1;
            break;
        case 1:
            tile->visible = 1;
            frame = stepAnimationOnce(&task->obj, updateSoundTileFrames, 0);
            if (frame != 0xFF) {
                tile->frame = frame;
            } else {
                tile->frame = 10;
                task->nextSubstate(task);
                SOUND.keyOff(SOUND_COMCD115, task->voice);
            }
            break;
        case 2:
            tile->visible = 1;
            tile->frame = 10;
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
