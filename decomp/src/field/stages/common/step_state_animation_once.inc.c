/*
 * Steps ANIM through FRAMES by the frame time (at most 4) and returns its
 * frame, 0xFF once it has ended. depth is 0 for the caller.
 */
s32 stepAnimationOnce(AnimState *anim, AnimFrame *frames, s32 depth) {
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
            return 0xFF;
        }
        stepAnimationOnce(anim, frames, depth + 1);
    }
    return frame->frame;
}
