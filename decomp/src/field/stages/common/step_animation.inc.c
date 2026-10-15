/*
 * Steps ANIM through FRAMES by the frame time (at most 4) and returns its
 * frame: at the end (frame 0xFF) it loops, or returns 0xFF if once. depth
 * is 0 for the caller.
 */
s32 stepAnimation(AnimState *anim, AnimFrame *frames, s32 once, s32 depth) {
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
        if (once) {
            if (frame->frame == 0xFF) {
                return 0xFF;
            }
        } else if (frame->frame == 0xFF) {
            frame = frames;
            anim->index = 0;
            anim->timer += frame->duration;
        }
        stepAnimation(anim, frames, once, depth + 1);
    }
    return frame->frame;
}
