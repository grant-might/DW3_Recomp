#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Small variables, addressed through $gp (see the Makefile) */
static s32 GFX_STARTED = 0;
static s32 FLIP_PENDING;

/*
 * Every vsync: advances the time counters and the play time, runs GFX.vsyncFunc, shows the
 * finished buffer and ticks the music
 */
void vsyncCallback(void) {
#if PAL_VIDEO
    if (NTSC_MODE) {
        GFX.timeCounter += VSYNC_STEP_NTSC;
        GFX.frameTimeCounter += VSYNC_STEP_NTSC;
        GAME.playTime.frames += VSYNC_STEP_NTSC;
    } else {
        GFX.timeCounter += VSYNC_STEP_PAL;
        GFX.frameTimeCounter += VSYNC_STEP_PAL;
        GAME.playTime.frames += VSYNC_STEP_PAL;
    }
#else
    GFX.timeCounter += VSYNC_STEP_NTSC;
    GFX.frameTimeCounter += VSYNC_STEP_NTSC;
    GAME.playTime.frames += VSYNC_STEP_NTSC;
#endif
    if (GFX.vsyncFunc != NULL) {
        GFX.vsyncFunc(GFX.vsyncArg);
    }
    if (FLIP_PENDING != 0) {
        GFX.dispBuffer = !GFX.dispBuffer;
        PutDispEnv(&GFX.disp[GFX.dispBuffer]);
    }
    SsSeqCalledTbyT();
    FLIP_PENDING = 0;
}

/* Installs vsyncCallback */
void startVSyncCallback(void) {
    VSyncCallback(vsyncCallback);
}

/*
 * Ends a frame: lets the layers run their draw callbacks, waits for the GPU
 * and for the vsync that shows the finished buffer, then sends every layer
 * of that buffer and clears the ones of the next.
 */
void drawFrame(s32 draw) {
    s32 i;
    Layer *layer;

    if (draw) {
        s32 j;

        for (j = 0; j < LAYER_COUNT; j++) {
            layer = GFX.layers[j];
            if (layer != NULL) {
                if (layer->keepView != 0) {
                    layer->loadView(layer);
                }
                if (layer->keepLightMatrix != 0) {
                    layer->loadLightMatrix(layer);
                }
                layer->runCallbacks(layer);
            }
        }
    }
    DrawSync(0);
    FLIP_PENDING = 1;
    while (*(volatile s32 *)&FLIP_PENDING != 0) {
    }
    if (GFX.prim != NULL) {
        for (i = 0; i < LAYER_COUNT; i++) {
            if (GFX.layers[i] != NULL) {
                GFX.layers[i]->draw(GFX.layers[i]);
            }
        }
    }
    GFX.buffer = GFX.buffer == 0;
#if PAL_VIDEO
    if (NTSC_MODE) {
        GFX.frameCounter += VSYNC_STEP_NTSC;
    } else {
        GFX.frameCounter += VSYNC_STEP_PAL;
    }
#else
    GFX.frameCounter += VSYNC_STEP_NTSC;
#endif
    GFX.frameCount = GFX.frameCounter >> 8;
    GFX.time = GFX.timeCounter >> 8;
    GFX.frameTime = GFX.frameTimeCounter >> 8;
    GFX.frameTimeCounter &= 0xFF;
    GAME.funcs.updatePlayTime();
    GFX.prim = GFX.primBufs[GFX.buffer];
    for (i = 0; i < LAYER_COUNT; i++) {
        if (GFX.layers[i] != NULL) {
            GFX.layers[i]->clearOt(GFX.layers[i]);
        }
    }
}

/* The frames drawn since boot */
s32 getFrameCount(void) {
    return GFX.frameCount;
}

/* The vsyncs since boot (in 60 Hz frames) */
s32 getTime(void) {
    return GFX.time;
}

/* The vsyncs the last frame took (in 60 Hz frames) */
s32 getFrameTime(void) {
    return GFX.frameTime;
}

/*
 * Destroys every layer and clears the packet buffers' state; the first call just picks the buffers
 */
void resetGraphics(void) {
    s32 i;

    if (GFX_STARTED != 0) {
        for (i = 0; i < LAYER_COUNT; i++) {
            if (GFX.layers[i] != NULL) {
                GFX.funcs.destroyLayer(GFX.layerIds[i]);
                i--;
            }
        }
        HEAP.zero(&GFX.prim, 0x18);
    } else {
        GFX.buffer = 1;
        GFX.dispBuffer = 0;
        GFX_STARTED = 1;
    }
}

/* Allocates the two GPU packet buffers, `size` bytes each, at the end of the heap */
void allocPrimBuffers(s32 size) {
    GFX.primBufSize = size;
    GFX.primBufs[0] = HEAP.allocHigh(size, MEM_MODE);
    GFX.primBufs[1] = HEAP.allocHigh(size, MEM_MODE);
    GFX.prim = GFX.primBufs[GFX.buffer];
}

/* The next free byte of the packet buffer being drawn */
void *getPrim(void) {
    return GFX.prim;
}

/* Moves the packet buffer's free pointer past the primitives just written */
void setPrim(void *next) {
    GFX.prim = next;
}

/* Frees the two packet buffers */
void freePrimBuffers(void) {
    if (GFX.primBufs[0] != NULL) {
        HEAP.free(GFX.primBufs[0]);
    }
    if (GFX.primBufs[1] != NULL) {
        HEAP.free(GFX.primBufs[1]);
    }
    GFX.primBufs[0] = NULL;
    GFX.primBufs[1] = NULL;
}

/*
 * Sets up the two display buffers: side by side in hi-res (interlaced 320x480 24-bit), else one
 * above the other
 */
void setDisplayMode(s32 w, s32 h, s32 hires, s32 interlace) {
    if (hires != 0) {
        if (interlace != 0) {
            SetDefDispEnv(&GFX.disp[0], 0, 0, 320, 480);
            GFX.disp[0].isinter = 1;
            GFX.disp[0].isrgb24 = 1;
            SetDefDispEnv(&GFX.disp[1], 480, 0, 320, 480);
            GFX.disp[1].isinter = 1;
            GFX.disp[1].isrgb24 = 1;
        } else {
            SetDefDispEnv(&GFX.disp[0], 0, 0, w, h);
            SetDefDispEnv(&GFX.disp[1], w, 0, w, h);
        }
    } else {
        SetDefDispEnv(&GFX.disp[0], 0, 0, w, h);
        SetDefDispEnv(&GFX.disp[1], 0, 256, w, h);
    }
#if PAL_VIDEO
    if (SHIFT_PAL_SCREEN) {
        GFX.disp[0].screen.y = 0x18;
        GFX.disp[1].screen.y = 0x18;
    }
#endif
    GsInit3D();
    SetGeomOffset(0, 0);
}

/* Shows the same VRAM area from both display buffers */
void setDisplayArea(s32 x, s32 y, s32 w, s32 h) {
    SetDefDispEnv(&GFX.disp[0], x, y, w, h);
    SetDefDispEnv(&GFX.disp[1], x, y, w, h);
}

/* GFX: the graphics state and its methods */
GfxState GFX = {
    .funcs = {
        resetGraphics, allocPrimBuffers, getPrim, setPrim, freePrimBuffers,
        startVSyncCallback, drawFrame, createLayer, destroyLayer, setDisplayMode, setDisplayArea, getLayer,
        moveLayer, getFrameCount, getTime, getFrameTime,
    },
};
