/* FIGHTSTG's battle speed. */

#include "battle/battle_mode.h"

/* How many frames the battle moves on this frame by its speed mode: as many
   as went by, none, a quarter of them or twice as many */
void FIGHTSTG_countFrames(void) {
    BattleSpeed *speed = &FIGHTSTG_battle.speed;
    s32 time;

    switch (speed->mode) {
    case 0:
    default:
        FIGHTSTG_battle.frames = GFX.funcs.getFrameTime();
        break;
    case 1:
        FIGHTSTG_battle.frames = 0;
        break;
    case 2:
        time = GFX.funcs.getFrameTime();
        FIGHTSTG_battle.frames = 0;
        speed->rest += time;
        while (speed->rest > 4) {
            FIGHTSTG_battle.frames++;
            speed->rest -= 4;
        }
        break;
    case 3:
        FIGHTSTG_battle.frames = GFX.funcs.getFrameTime() * 2;
        break;
    }
}

/* Sets the battle's speed mode and counts this frame again */
void FIGHTSTG_setSpeed(s32 mode) {
    FIGHTSTG_battle.speed.mode = mode;
    FIGHTSTG_battle.speed.rest = 0;
    FIGHTSTG_countFrames();
}
