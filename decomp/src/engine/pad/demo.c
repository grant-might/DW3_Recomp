#include "engine/game.h"
#include <libpad.h>

/* Demo recording was left out of the release: it never starts */
s32 startDemoRecording(void) {
    return 0;
}

/* Does nothing: demo recording was left out */
void stopDemoRecording(void) {
}

/* 1 while pad `pad` records a demo, -1 while another pad does, 0 otherwise (never set) */
s32 isDemoRecording(s32 pad) {
    if (PAD.flags & PAD_FLAG_DEMO_RECORDING) {
        if (PAD.demoPad == pad) {
            return 1;
        }
        return -1;
    }
    return 0;
}

/* Replays `data` (one raw buffer a frame) as the input of pad */
s32 startDemoPlayback(s16 pad, PadBuffer *data) {
    if (!(PAD.flags & (PAD_FLAG_DEMO_PLAYBACK | PAD_FLAG_DEMO_RECORDING))) {
        PAD.flags |= PAD_FLAG_DEMO_PLAYBACK;
        if (PAD.demoData == NULL) {
            PAD.demoPad = pad;
            PAD.demoData = data;
            PAD.demoFrame = 0;
            setVibration((pad * 16) & 0xF0, 0, 0, 0);
            return 1;
        }
    }
    return 0;
}

/* Gives the pads back to the players */
void stopDemoPlayback(void) {
    if (PAD.flags & PAD_FLAG_DEMO_PLAYBACK) {
        PAD.flags &= ~PAD_FLAG_DEMO_PLAYBACK;
        PAD.demoData = NULL;
        PAD.demoPad = 0;
        PAD.demoFrame = 0;
    }
}

/* 1 while pad `pad` replays a demo, -1 while another pad does, 0 otherwise */
s32 isDemoPlaying(s32 pad) {
    if (PAD.flags & PAD_FLAG_DEMO_PLAYBACK) {
        if (PAD.demoPad == pad) {
            return 1;
        }
        return -1;
    }
    return 0;
}
