/*
 * Restarts the three animations of a StageTileEffect, from the stage's
 * updateTileEffectFrames
 */
void resetTileEffect(StageTileEffect *task) {
    s32 i;

    for (i = 0; i < 3; i++) {
        task->anims[i].anim.index = 0;
        task->anims[i].anim.timer = updateTileEffectFrames[i][0].duration;
    }
}
