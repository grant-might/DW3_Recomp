/*
 * Starts the menu stage's tween (stageFuncs.start) with its sound: up, from
 * 0 to ONE over its duration, or down, twice as fast
 */
void startTween(PanelAnim *tween, s32 up) {
    tween->active = 1;
    if (up) {
        SOUND.playSound(SOUND_MENU_OPEN);
        tween->step = ONE / tween->duration;
        tween->level = 0;
    } else {
        SOUND.playSound(SOUND_MENU_CLOSE);
        tween->level = ONE;
        tween->step = -(ONE / tween->duration * 2);
    }
}
