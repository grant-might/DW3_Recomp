/* Steps the tween (stageFuncs.update): whether it has ended */
s32 updateTween(PanelAnim *tween) {
    if (tween->active == 0) {
        return 1;
    }
    tween->level += tween->step;
    if (tween->step > 0) {
        if (tween->level > ONE) {
            tween->level = ONE;
            tween->active = 0;
            return 1;
        }
    } else if (tween->level < 0) {
        tween->level = 0;
        tween->active = 0;
        return 1;
    }
    return 0;
}
