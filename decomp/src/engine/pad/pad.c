#include "engine/game.h"
#include <libpad.h>

/* The vsyncs a held button takes to repeat, unless initPad is told otherwise */
#define DEFAULT_REPEAT_RATE 0x10

/* Opens the pads (PadInitMtap or PadInitDirect); repeatRate 0 means DEFAULT_REPEAT_RATE */
void initPad(s32 multitap, s32 repeatRate) {
    s32 i;
    s32 j;
    s16 count;

    HEAP.zero(&PAD, PAD_DATA_SIZE);
    HEAP.fill(PAD.act, 0xFF, sizeof(PAD.act));
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            resetButtonMap((i * 16 + j) & 0xFF);
        }
    }
    if (multitap != 0) {
        PadInitMtap(PAD.buf[0].bytes, PAD.buf[1].bytes);
        PAD.flags |= PAD_FLAG_MULTITAP;
    } else {
        PadInitDirect(PAD.buf[0].bytes, PAD.buf[1].bytes);
    }
    repeatRate &= 0x7F;
    PAD.repeatRate = (u8)repeatRate;
    count = (u8)repeatRate;
    PAD.flags |= PAD_FLAG_INITIALIZED;
    if (count == 0) {
        PAD.repeatRate = DEFAULT_REPEAT_RATE;
    }
    startPad();
}

/* Stops reading the pads and forgets every flag, so the next startPad opens them again */
void shutdownPad(void) {
    stopPad();
    PAD.flags = 0;
}

/* Opens the pads with the defaults if needed, then starts reading them every vsync */
void startPad(void) {
    if (!(PAD.flags & PAD_FLAG_INITIALIZED)) {
        initPad(0, DEFAULT_REPEAT_RATE);
    }
    if (!(PAD.flags & PAD_FLAG_STARTED)) {
        PadStartCom();
        PAD.flags |= PAD_FLAG_STARTED;
    }
}

/* Stops reading the pads every vsync */
void stopPad(void) {
    if (PAD.flags & PAD_FLAG_STARTED) {
        PadStopCom();
    }
    PAD.flags &= ~PAD_FLAG_STARTED;
}

/* Reads every pad, or the demo data while a demo plays */
void updatePad(void) {
    PadBuffer *record = PAD.demoData + PAD.demoFrame;
    s32 ret;
    s32 i;
    s32 j;
    u16 port;

    ret = PadChkVsync();
    if (ret != 1) {
        return;
    }
    if (++PAD.demoFrame >= DEMO_FRAME_COUNT && isDemoRecording(PAD.demoPad) == ret) {
        stopDemoRecording();
        return;
    }
    for (i = 0; i < 2; i++) {
        port = i * 16;
        if (PAD.buf[i].tap.id == PAD_MULTITAP_ID) {
            for (j = 0; j < 4; j++) {
                if (PAD.flags & PAD_FLAG_DEMO_PLAYBACK) {
                    readPadButtons((u8)port, &PAD.buf[i].tap.pads[j], &record->tap.pads[j]);
                } else {
                    readPad((u8)(port + j), &PAD.buf[i].tap.pads[j]);
                }
            }
        } else {
            if (PAD.flags & PAD_FLAG_DEMO_PLAYBACK) {
                readPadButtons((u8)port, &PAD.buf[i].pad, &record->pad);
            } else {
                readPad((u8)port, &PAD.buf[i].pad);
            }
        }
    }
}

/* Starts a motor for `time` vsyncs (0 stops it) */
s32 setVibration(u16 port, s32 motor, s16 time, u8 value) {
    u8 id = port;
    s32 mode;
    PadSlot *slot;
    s16 t;
    s32 pad;

    t = time;
    if (!(PAD.flags & PAD_FLAG_VIBRATION)) {
        return 0;
    }
    if (pollPadState(id) == 0) {
        return 0;
    }
    mode = PadInfoMode(id, InfoModeCurExID, 0);
    pad = id >> 4;
    slot = &PAD.slots[pad][port & 3];
    if (mode == PAD_ID_DIGITAL || mode == PAD_ID_ANALOG) {
        PAD.act[pad][motor & 1] = value;
    } else {
        PAD.act[pad][0] = 0x40;
        PAD.act[pad][1] = 1;
    }
    if (slot->vibrationTimers[motor] <= 0) {
        slot->vibrationTimers[motor] = t;
    } else if (t == 0) {
        slot->vibrationTimers[motor] = 0;
    }
    PadSetAct(id, PAD.act[pad], 2);
    return 1;
}

/* The logical buttons newly pressed on pad 1 or 2 this frame */
s32 getPadPressed(s32 pad) {
    return PAD.slots[pad][0].pressed;
}

/* The logical buttons held on pad 1 or 2 */
s32 getPadHeld(s32 pad) {
    return PAD.slots[pad][0].held;
}

/* The logical buttons pressed on pad 1 or 2, repeating while held */
s32 getPadRepeated(s32 pad) {
    return PAD.slots[pad][0].repeated;
}

/* Gives a pad slot the default button map, where button i is bit i */
void resetButtonMap(u16 port) {
    s32 i;

    for (i = 0; i < 16; i++) {
        PAD.slots[(u8)port >> 4][port & 3].buttonMap[i] = DEFAULT_BUTTON_MAP[i];
    }
}

/* Swaps two logical buttons in a pad slot's button map */
void swapButtons(u16 port, s32 a, s32 b) {
    u8 bit = PAD.slots[(u8)port >> 4][port & 3].buttonMap[a];

    PAD.slots[(u8)port >> 4][port & 3].buttonMap[a] = PAD.slots[(u8)port >> 4][port & 3].buttonMap[b];
    PAD.slots[(u8)port >> 4][port & 3].buttonMap[b] = bit;
}

/* The bit that logical button `index` sets on pad 1 or 2 */
s32 getButtonBit(s32 pad, s32 index) {
    return PAD.slots[pad][0].buttonMap[index];
}

/* Controller input: cleared by initPad, then the methods */
PadState PAD = {
    0, { { { 0 } } }, { { { 0 } } }, { { 0 } }, 0, 0, NULL, 0,
    initPad, shutdownPad, updatePad, setVibration, lockPadMode,
    getPadPressed, getPadHeld, getPadRepeated,
    resetButtonMap, swapButtons, getButtonBit,
    startDemoRecording, stopDemoRecording, isDemoRecording,
    startDemoPlayback, stopDemoPlayback, isDemoPlaying,
};

/* The identity: logical button i is bit i */
u8 DEFAULT_BUTTON_MAP[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
};
