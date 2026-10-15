#ifndef START_FADE
#define START_FADE OVL_NAME(startFade)
#endif
/* Starts opening (fadeIn) or closing a menu panel, with its sound: its level
   goes 0 -> ONE in `duration` frames, and back at twice the speed. The name
   entry has a copy of its own, which it names with START_FADE */
void START_FADE(PanelAnim *fade, s32 fadeIn) {
    fade->active = 1;
    if (fadeIn != 0) {
        SOUND.playSound(SOUND_MENU_OPEN);
        fade->level = 0;
        fade->step = ONE / fade->duration;
    } else {
        SOUND.playSound(SOUND_MENU_CLOSE);
        fade->level = ONE;
        fade->step = -((ONE / fade->duration) * 2);
    }
}
#undef START_FADE
