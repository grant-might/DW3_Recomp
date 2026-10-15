#include "engine/game.h"
#include <libpad.h>

/*
 * The libpad state of a port while it can be read (0 otherwise); a stable pad gets its actuators
 * aligned
 */
s32 pollPadState(u32 port) {
    u32 p = port;
    u32 mask;
    s32 state = PadGetState(p & 0xFF);

    switch (state) {
    case PadStateDiscon:
    case PadStateFindPad:
        /* meant to drop the slot's mode lock, but masks with the slot's
           index rather than its bit */
        mask = ~(((p >> 2) & 0x3C) | (p & 3)); PAD.flags = PAD.flags & mask & ~(PAD_FLAG_ALIGNING | PAD_FLAG_VIBRATION);
        return 0;
    case PadStateStable:
        if (!(PAD.flags & PAD_FLAG_VIBRATION)) {
            if (PAD.flags & PAD_FLAG_ALIGNING) {
                PAD.flags |= PAD_FLAG_VIBRATION;
            } else if (alignActuators(p & 0xFF)) {
                PAD.flags |= PAD_FLAG_ALIGNING;
            }
        }
        return state;
    case PadStateFindCTP1:
        return state;
    case PadStateFindCTP2:
    case PadStateReqInfo:
    case PadStateExecCmd:
    default:
        return 0;
    }
}

/* Builds held/pressed/repeated from the raw data (the demo record replaces all but Start) */
void readPadButtons(s32 port, PadData *data, PadData *record) {
    u32 id = port & 0xFF;
    s32 mode = PadInfoMode(id, InfoModeCurExID, 0);
    u32 pad = (id >> 4) & 1;
    PadSlot *slot = &PAD.slots[(u8)pad][port & 3];
    s16 buttons;
    s16 i;
    u16 b;
    s16 circle, cross, triangle;

    if ((PAD.flags & PAD_FLAG_DEMO_PLAYBACK) && PAD.demoPad == ((port & 3) | pad)) {
        buttons = (~data->buttons & 1 << PAD_START) | (~record->buttons & ~(1 << PAD_START));
        if (mode == PAD_ID_ANALOG) {
            /* not record->analog[i], which adds the index the other way round */
            for (i = 0; i < 4; i++) {
                slot->analog[i] = *(record->analog + i);
            }
        }
    } else {
#if VERSION_US
        b = ~data->buttons;
        circle = (b >> RAW_CIRCLE) & 1;
        cross = (b >> RAW_CROSS) & 1;
        triangle = (b >> RAW_TRIANGLE) & 1;
        buttons = ~data->buttons & ~FACE_BUTTONS;
        if (cross) {
            buttons |= 1 << PAD_CROSS;
        }
        if (triangle) {
            buttons |= 1 << PAD_TRIANGLE;
        }
        if (circle) {
            buttons |= 1 << PAD_CIRCLE;
        }
#elif VERSION_EU || VERSION_JP
        /* the Japanese language keeps the buttons as they are (the Japanese
           version's only one, LANGUAGE 0) */
        buttons = ~data->buttons;
        if (LANGUAGE != 0) {
            b = buttons;
            circle = (b >> RAW_CIRCLE) & 1;
            cross = (b >> RAW_CROSS) & 1;
            triangle = (b >> RAW_TRIANGLE) & 1;
            buttons &= ~FACE_BUTTONS;
            if (cross) {
                buttons |= 1 << PAD_CROSS;
            }
            if (triangle) {
                buttons |= 1 << PAD_TRIANGLE;
            }
            if (circle) {
                buttons |= 1 << PAD_CIRCLE;
            }
        }
#endif
        if (mode == PAD_ID_ANALOG) {
            /* as above, not data->analog[i] */
            for (i = 0; i < 4; i++) {
                slot->analog[i] = *(data->analog + i);
            }
        }
    }
    if (mode == PAD_ID_ANALOG) {
        if (slot->analog[2] <= STICK_LOW) {
            buttons |= 1 << PAD_LEFT;
        } else if (slot->analog[2] >= STICK_HIGH) {
            buttons |= 1 << PAD_RIGHT;
        }
        if (slot->analog[3] <= STICK_LOW) {
            buttons |= 1 << PAD_UP;
        } else if (slot->analog[3] >= STICK_HIGH) {
            buttons |= 1 << PAD_DOWN;
        }
    }
    i = 0;
    slot->repeated = 0;
    for (; i < 16; i++) {
        u8 bit = slot->buttonMap[i];
        s32 *time = &slot->repeatTime[bit];
        u8 *count = &slot->repeatCount[bit];

        if ((buttons >> bit) & 1) {
            if (PAD.repeatRate > 0) {
                if ((GFX.funcs.getTime() - *time + *count) / PAD.repeatRate != 0) {
                    *count += 10;
                    if (*count >= 12) {
                        *count = 12;
                    }
                    *time = GFX.funcs.getTime();
                    slot->repeated |= 1 << slot->buttonMap[i];
                }
            }
        } else {
            *time = GFX.funcs.getTime();
            *count = 0;
        }
    }
    b = slot->held;
    slot->held = buttons;
    slot->prevHeld = b;
    slot->pressed = buttons & (b ^ buttons);
}

/* Reads one pad (vibration included), or clears its buttons when it is missing */
s32 readPad(u16 port, PadData *data) {
    u8 id = port;
    s32 mode;

    if (data->status != 0 || pollPadState(id & 0xFF) == 0) {
        PAD.slots[(id >> 4) & 1][port & 3].pressed = 0;
        PAD.slots[(id >> 4) & 1][port & 3].repeated = 0;
        PAD.slots[(id >> 4) & 1][port & 3].held = 0;
        return 0;
    }
    mode = PadInfoMode(id & 0xFF, InfoModeCurExID, 0);
    if (mode == PAD_ID_DIGITAL || mode == PAD_ID_ANALOG) {
        updateVibration(id & 0xFF);
    }
    readPadButtons(id & 0xFF, data, NULL);
    return 1;
}

/* Locks or unlocks the analog button of a DualShock, keeping its current mode */
s32 lockPadMode(s32 port, s32 lock) {
    u32 id = port & 0xFF;
    s32 mode;
    s32 bit;

    if (pollPadState(id) != 0) {
        mode = PadInfoMode(id, InfoModeCurExID, 0);
        if (mode == PAD_ID_DIGITAL || mode == PAD_ID_ANALOG) {
            bit = ((id >> 4) << 2) | (port & 3);
            if (lock != 0) {
                PAD.flags |= 1 << bit;
                PadSetMainMode(id, PadInfoMode(id, InfoModeCurExOffs, 0), PAD_MODE_LOCKED);
            } else {
                PAD.flags &= ~(1 << bit);
                PadSetMainMode(id, PadInfoMode(id, InfoModeCurExOffs, 0), PAD_MODE_UNLOCKED);
            }
            PAD.flags &= ~(PAD_FLAG_ALIGNING | PAD_FLAG_VIBRATION);
            return 1;
        }
    }
    return 0;
}

/* Sends a DualShock its actuator alignment, so setVibration can drive the motors */
s32 alignActuators(u16 port) {
    u32 id = (u8)port;
    s32 count = PadInfoAct(id, -1, 0);
    s32 i;
    s32 act;

    for (i = 0; i < count; i++) {
        act = PadInfoAct(id, i, InfoActSub);
        if (act != 0) {
            PAD.act[id >> 4][i] = act & 1;
        }
    }
    return PadSetActAlign(port & 0xFF, PAD.act[(port & 0xFF) >> 4]);
}

/* Counts down a pad's vibration timers and stops each motor whose time is up */
void updateVibration(u16 port) {
    u8 id = port;
    s32 i;
    PadSlot *slot = &PAD.slots[id >> 4][port & 3];

    for (i = 0; i < 2; i++) {
        if (slot->vibrationTimers[i] != 0) {
            if ((slot->vibrationTimers[i] -= GFX.funcs.getFrameTime()) <= 0) {
                slot->vibrationTimers[i] = 0;
                PAD.act[id >> 4][i] = 0;
            }
            PadSetAct(port & 0xFF, PAD.act[id >> 4], 2);
        }
    }
}
