#ifndef DW3_PAD_H
#define DW3_PAD_H

/* Controllers and the demo recorder (pad/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * Logical buttons: bits of PadSlot.held/pressed/repeated, which readPadButtons
 * builds from the raw pad data (circle, cross and triangle are rotated, and
 * the left analog stick also sets the d-pad bits). A button map
 * (getButtonBit) translates them again; by default it is the identity.
 * The menus confirm with PAD_CROSS and cancel with PAD_TRIANGLE.
 */
#define PAD_SELECT 0
#define PAD_L3 1
#define PAD_R3 2
#define PAD_START 3
#define PAD_UP 4
#define PAD_RIGHT 5
#define PAD_DOWN 6
#define PAD_LEFT 7
#define PAD_L2 8
#define PAD_R2 9
#define PAD_L1 10
#define PAD_R1 11
#define PAD_CIRCLE 12
#define PAD_CROSS 13
#define PAD_TRIANGLE 14
#define PAD_SQUARE 15

/* PadInfoMode(InfoModeCurExID) of a DualShock in digital and analog mode */
#define PAD_ID_DIGITAL 4
#define PAD_ID_ANALOG 7

/* The second byte of the raw buffer when a multitap answers */
#define PAD_MULTITAP_ID 0x80

/* PadSetMainMode's lock: the analog button may change the mode, or not */
#define PAD_MODE_UNLOCKED 2
#define PAD_MODE_LOCKED 3

/* Raw bits of circle, cross and triangle, which readPadButtons rotates */
#define RAW_TRIANGLE 12
#define RAW_CIRCLE 13
#define RAW_CROSS 14
#define FACE_BUTTONS (1 << PAD_CIRCLE | 1 << PAD_CROSS | 1 << PAD_TRIANGLE)

/* A stick at or below LOW, or at or above HIGH, also presses the d-pad */
#define STICK_LOW 0x40
#define STICK_HIGH 0xC0

/* What a pad sends (libpad's receive buffer) */
typedef struct PadData {
    /* 0x0 */ u8 status; /* 0: it answered */
    /* 0x1 */ u8 id;
    /* 0x2 */ u16 buttons; /* raw: a bit is clear while its button is pressed */
    /* 0x4 */ u8 analog[4];
} PadData;

/* A port's receive buffer, and a frame of a demo: a pad, or a multitap and
   the four pads behind it */
typedef union PadBuffer {
    u8 bytes[0x22];
    PadData pad;
    struct {
        u8 status;
        u8 id; /* PAD_MULTITAP_ID */
        PadData pads[4];
    } tap;
} PadBuffer;

/* One controller: a port, or one of the four multitap slots behind it */
typedef struct PadSlot {
    /* 0x00 */ u16 pressed; /* this frame */
    /* 0x02 */ u16 repeated; /* pressed, or held long enough to repeat */
    /* 0x04 */ u16 prevHeld;
    /* 0x06 */ u16 held;
    /* 0x08 */ u8 analog[4];
    /* 0x0C */ s32 repeatTime[16];
    /* 0x4C */ u8 repeatCount[16];
    /* 0x5C */ u8 buttonMap[16];
    /* 0x6C */ s16 vibrationTimers[2];
} PadSlot;

/*
 * PadState.flags: bits 0-7 hold the mode lock of each port/slot (lockPadMode),
 * the rest are these.
 */
#define PAD_FLAG_DEMO_PLAYBACK 0x00400000
#define PAD_FLAG_DEMO_RECORDING 0x00800000
#define PAD_FLAG_ALIGNING 0x04000000 /* PadSetActAlign sent, not yet stable */
#define PAD_FLAG_VIBRATION 0x08000000 /* the actuators are aligned */
#define PAD_FLAG_STARTED 0x20000000 /* PadStartCom done */
#define PAD_FLAG_INITIALIZED 0x40000000
#define PAD_FLAG_MULTITAP 0x80000000

/* The bytes PadState keeps before its methods, which initPad clears */
#define PAD_DATA_SIZE 0x3E0

/*
 * A demo replays recorded pad data (one raw buffer a frame, up to
 * DEMO_FRAME_COUNT frames) instead of the pad, except for Start.
 */
#define DEMO_FRAME_COUNT 0x707

/* Controller input (PAD) */
typedef struct PadState {
    /* 0x000 */ s32 flags;
    /* 0x004 */ PadBuffer buf[2];
    /* 0x048 */ PadSlot slots[2][4];
    /* 0x3C8 */ u8 act[2][6];
    /* 0x3D4 */ s16 repeatRate; /* vsyncs */
    /* 0x3D6 */ s16 demoPad;
    /* 0x3D8 */ PadBuffer *demoData; /* a buffer a frame */
    /* 0x3DC */ s16 demoFrame;
    /* 0x3E0 */ void (*init)(); /* PAD_INIT */
    /* 0x3E4 */ void (*shutdown)();
    /* 0x3E8 */ void (*update)(); /* PAD_UPDATE */
    /* 0x3EC */ s32 (*setVibration)(u16 port, s32 motor, s16 time, u8 value);
    /* 0x3F0 */ s32 (*lockMode)(s32 port, s32 lock);
    /* 0x3F4 */ s32 (*getPressed)(s32 pad);
    /* 0x3F8 */ s32 (*getHeld)(s32 pad);
    /* 0x3FC */ s32 (*getRepeated)(s32 pad);
    /* 0x400 */ void (*resetButtonMap)(u16 port);
    /* 0x404 */ void (*swapButtons)(u16 port, s32 a, s32 b);
    /* 0x408 */ s32 (*getButtonBit)(s32 pad, s32 button);
    /* 0x40C */ s32 (*startDemoRecording)();
    /* 0x410 */ void (*stopDemoRecording)();
    /* 0x414 */ s32 (*isDemoRecording)();
    /* 0x418 */ s32 (*startDemoPlayback)(s16 pad, PadBuffer *data);
    /* 0x41C */ void (*stopDemoPlayback)();
    /* 0x420 */ s32 (*isDemoPlaying)();
} PadState;

/* One button of pad 1: newly pressed, auto-repeated while held, or held */
#define PAD_PRESSED(button) ((PAD.getPressed(0) >> PAD.getButtonBit(0, button)) & 1)
#define PAD_REPEATED(button) ((PAD.getRepeated(0) >> PAD.getButtonBit(0, button)) & 1)
#define PAD_HELD(button) ((PAD.getHeld(0) >> PAD.getButtonBit(0, button)) & 1)

void readPadButtons(s32 port, PadData *data, PadData *record);
void updateVibration(u16 port);
void resetButtonMap(u16 port);
void swapButtons(u16 port, s32 a, s32 b);
s32 getButtonBit(s32 pad, s32 index);
void startPad(void);
s32 pollPadState(u32 port);
void stopPad(void);
void updatePad(void);
void initPad(s32 multitap, s32 repeatRate);
void shutdownPad(void);
s32 setVibration(u16 port, s32 motor, s16 time, u8 value);
s32 getPadPressed(s32 pad);
s32 getPadHeld(s32 pad);
s32 getPadRepeated(s32 pad);
s32 readPad(u16 port, PadData *data);
s32 startDemoRecording(void);
void stopDemoRecording(void);
s32 isDemoRecording(s32 pad);
s32 alignActuators(u16 port);
s32 startDemoPlayback(s16 pad, PadBuffer *data);
void stopDemoPlayback(void);
s32 isDemoPlaying(s32 pad);
s32 lockPadMode(s32 port, s32 lock);

extern u8 DEFAULT_BUTTON_MAP[16];
extern PadState PAD;

#endif /* DW3_PAD_H */
