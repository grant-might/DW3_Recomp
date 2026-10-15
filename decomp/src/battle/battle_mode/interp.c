/* FIGHTSTG's interpolation (FIGHTSTG_interp). */

#include "battle/battle_mode.h"
#include "engine/gte.h"

/* FIGHTSTG_interp.nop: empty, and nothing calls it */
void FIGHTSTG_interpNop(void) {
}

/* FIGHTSTG_interp.lerp: out is from moved t (0-0x1000) of the way to `to`,
   on the GTE */
void FIGHTSTG_lerpVector(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    SVECTOR diff;
    SVECTOR step;

    gte_lddp(t);
    diff.vx = to->vx - from->vx;
    diff.vy = to->vy - from->vy;
    diff.vz = to->vz - from->vz;
    gte_ldsv(&diff);
    gte_gpf12();
    *out = *from;
    gte_stsv(&step);
    out->vx += step.vx;
    out->vy += step.vy;
    out->vz += step.vz;
}

/* Returns value scaled by the sine of t (4096 is 1.0) on the given curve: 0 a
 * quarter of t, 1 the same as a cosine, 2 half of t. The match depends on a
 * return in each case; one shared return after the switch schedules the
 * epilogue differently. */
s32 FIGHTSTG_ease(s32 curve, s32 t, s32 value) {
    switch (curve) {
    case 0:
    default:
        return rsin(t >> 2) * value / 4096;
    case 1:
        return rsin((t >> 2) + 0x400) * value / 4096;
    case 2:
        return rsin(t >> 1) * value / 4096;
    }
}

/* the functions that go between values */
InterpFuncs FIGHTSTG_interp = { FIGHTSTG_interpNop, FIGHTSTG_lerpVector, FIGHTSTG_ease };
