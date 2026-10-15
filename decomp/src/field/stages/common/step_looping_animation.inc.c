/*
 * Steps ANIM through FRAMES by the frame time (at most 4), looping at the
 * end (frame 0xFF), and returns its frame. depth is 0 for the caller: a
 * frame that runs out goes on to the next one.
 */
s32 stepLoopingAnimation(AnimState *anim, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[anim->index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        anim->timer -= dt;
    }
    if (anim->timer <= 0) {
        frame++;
        anim->index++;
        anim->timer += frame->duration;
        if (frame->frame == 0xFF) {
            frame = frames;
            anim->index = 0;
            anim->timer += frame->duration;
        }
        stepLoopingAnimation(anim, frames, depth + 1);
    }
    return frame->frame;
}
