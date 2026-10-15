/* Starts moving a value from `from` to `to` over `frames` frames, in 24.8
   fixed point; nothing when they are the same */
void OVL_NAME(startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames) {
    if (from != to) {
        lerp->duration = frames;
        lerp->fixed = from << 8;
        lerp->value = from;
        lerp->target = to;
        lerp->active = 1;
        lerp->step = ((to - from) << 8) / lerp->duration;
    }
}
