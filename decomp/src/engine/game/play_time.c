#include "engine/game.h"
#include "field/field_map.h"

/* Sets the play time to zero */
void resetPlayTime(void) {
    GAME.playTime.maxed = 0;
    GAME.playTime.seconds = 0;
    GAME.playTime.minutes = 0;
    GAME.playTime.hours = 0;
    GAME.playTime.frames = 0;
}

/* Turns the vsync-counted frames into seconds, minutes and hours, stopping at 999:59:59 */
void updatePlayTime(void) {
    if ((GAME.playTime.frames >> 8) >= 60) {
        GAME.playTime.frames &= 0xFF;
        if (++GAME.playTime.seconds >= 60) {
            GAME.playTime.seconds = 0;
            if (++GAME.playTime.minutes >= 60) {
                GAME.playTime.minutes = 0;
                if (++GAME.playTime.hours >= 1000) {
                    GAME.playTime.hours = 999;
                    GAME.playTime.minutes = 59;
                    GAME.playTime.seconds = 59;
                    GAME.playTime.maxed = 1;
                }
            }
        }
    }
}
