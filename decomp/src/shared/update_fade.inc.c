#ifndef UPDATE_FADE
#define UPDATE_FADE OVL_NAME(updateFade)
#endif
/* Moves a panel's opening or closing one frame on: 1 once it is fully open or
   closed. The name entry has a copy of its own, which it names with
   UPDATE_FADE */
s32 UPDATE_FADE(PanelAnim *fade) {
    if (fade->active == 0) {
        return 1;
    }
    fade->level += fade->step;
    if (fade->step > 0) {
        if (fade->level > ONE) {
            fade->level = ONE;
            fade->active = 0;
            return 1;
        }
    } else if (fade->level < 0) {
        fade->level = 0;
        fade->active = 0;
        return 1;
    }
    return 0;
}
#undef UPDATE_FADE
