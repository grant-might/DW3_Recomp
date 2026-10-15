#ifndef DW3_GFX_H
#define DW3_GFX_H

/* Display, drawing layers, the drawing helpers and the screen fade (gfx/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "engine/task.h"

typedef struct Layer Layer;
typedef struct DrawCallback DrawCallback;

/* The graphics methods (GFX.funcs) */
typedef struct GfxFuncs {
    /* 0x00 */ void (*reset)(void);
    /* 0x04 */ void (*allocPrimBuffers)(s32 size);
    /* 0x08 */ void *(*getPrim)(void);
    /* 0x0C */ void (*setPrim)(void *next);
    /* 0x10 */ void (*freePrimBuffers)(void);
    /* 0x14 */ void (*startVSync)(void);
    /* 0x18 */ void (*drawFrame)();
    /* 0x1C */ struct Layer *(*createLayer)(RECT *rect, s32 otShift, s32 id);
    /* 0x20 */ s32 (*destroyLayer)(s32 id);
    /* 0x24 */ void (*setDisplayMode)(s32 w, s32 h, s32 hires, s32 interlace);
    /* 0x28 */ void (*setDisplayArea)();
    /* 0x2C */ struct Layer *(*getLayer)(s32 id);
    /* 0x30 */ void (*moveLayer)();
    /* 0x34 */ s32 (*getFrameCount)(void);
    /* 0x38 */ s32 (*getTime)(void);
    /* 0x3C */ s32 (*getFrameTime)(void);
} GfxFuncs;

/* The screen the modes draw on (setDisplayMode, their layers' rects) */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

/* The drawing layers GFX holds at most */
#define LAYER_COUNT 30

/* The layer over the whole screen that most modes create first and draw
   their menus on (FIELDSTG and STDWTITL name theirs: FIELD_LAYER_BACK,
   STDWTITL_TITLE_LAYER); the ones they add next take 0x1001 and up */
#define SCREEN_LAYER 0x1000

/*
 * What a vsync adds to the 8.8 time counters: a frame of the 60 Hz game,
 * or 1.2 of them at 50 Hz so that the game keeps its pace
 */
#define VSYNC_STEP_NTSC 0x100
#define VSYNC_STEP_PAL 0x133

/*
 * The graphics state: double-buffered display, the GPU packet buffers that
 * primitives are written to (getPrim/setPrim), and up to LAYER_COUNT drawing layers,
 * each with its own ordering table, drawn in order by drawFrame.
 * Times are in vsyncs; the counters keep 8 fractional bits.
 */
typedef struct GfxState {
    /* 0x00 */ void (*vsyncFunc)(s32 arg);
    /* 0x04 */ s32 vsyncArg;
    /* 0x08 */ s32 frameCount;
    /* 0x0C */ s32 frameCounter;
    /* 0x10 */ s32 time; /* vsyncs since boot */
    /* 0x14 */ s32 timeCounter;
    /* 0x18 */ s32 frameTime; /* vsyncs that the last frame took */
    /* 0x1C */ s32 frameTimeCounter;
    /* 0x20 */ void *prim; /* next free byte of the packet buffer */
    /* 0x24 */ void *primBufs[2];
    /* 0x2C */ s32 primBufSize;
    /* 0x30 */ s32 dispBuffer;
    /* 0x34 */ s32 buffer; /* the one being drawn */
    /* 0x38 */ DISPENV disp[2];
    /* 0x60 */ struct Layer *layers[LAYER_COUNT];
    /* 0xD8 */ s32 layerIds[LAYER_COUNT];
    /* 0x150 */ GfxFuncs funcs;
} GfxState;

/* Layer as its clip/scroll methods see it: the clip rect as unsigned */
typedef struct LayerView {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 w;
    /* 0x06 */ u16 h;
    /* 0x08 */ s16 ofs[2]; /* DRAWENV's fields on (Layer.env) */
    /* 0x0C */ RECT tw;
    /* 0x14 */ u16 tpage;
    /* 0x16 */ u8 dtd;
    /* 0x17 */ u8 dfe;
    /* 0x18 */ u8 isbg;
    /* 0x19 */ u8 bgR;
    /* 0x1A */ u8 bgG;
    /* 0x1B */ u8 bgB;
    /* 0x1C */ DR_ENV drEnv;
    /* 0x5C */ u_long *ot[2];
    /* 0x64 */ s32 otLen;
    /* 0x68 */ s32 otShift;
    /* 0x6C */ s16 offsetX;
    /* 0x6E */ s16 offsetY;
    /* 0x70 */ s32 scrollX;
    /* 0x74 */ s32 scrollY;
    /* 0x78 */ s32 callbackCap;
    /* 0x7C */ s32 callbackCount;
    /* 0x80 */ DrawCallback *callbacks;
} LayerView;

typedef struct Rect16 {
    s16 x;
    s16 y;
    u16 w;
    u16 h;
} Rect16;

/* A function that a layer calls when it is drawn (Layer.addDrawCallback) */
struct DrawCallback {
    /* 0x00 */ s32 priority;
    /* 0x04 */ s32 param;
    /* 0x08 */ void (*func)(s32 arg, void *layer, s32 param);
    /* 0x0C */ s32 arg;
    /* 0x10 */ struct DrawCallback *next;
};

/*
 * A drawing layer (GFX.funcs.createLayer): a DRAWENV with a pair of ordering
 * tables, one per buffer, a scroll position and a list of draw callbacks.
 */
struct Layer {
    /* 0x00 */ DRAWENV env;
    /* 0x5C */ u_long *ot[2];
    /* 0x64 */ s32 otLen;
    /* 0x68 */ s32 otShift;
    /* 0x6C */ s16 offsetX;
    /* 0x6E */ s16 offsetY;
    /* 0x70 */ s32 scrollX;
    /* 0x74 */ s32 scrollY;
    /* 0x78 */ s32 callbackCap;
    /* 0x7C */ s32 callbackCount;
    /* 0x80 */ DrawCallback *callbacks;
    /* 0x84 */ s32 keepView;
    /* 0x88 */ s32 projection;
    /* 0x8C */ MATRIX view[2];
    /* 0xCC */ s32 keepLightMatrix;
    /* 0xD0 */ MATRIX lightMatrix[2];
    /* 0x110 */ void (*setClipPos)(); /* no prototype: callers would truncate s16 args */
    /* 0x114 */ void (*setClipSize)(); /* no prototype: callers would truncate s16 args */
    /* 0x118 */ void (*setOffset)(); /* no prototype: callers would truncate s16 args */
    /* 0x11C */ void (*getScroll)();
    /* 0x120 */ void (*setScroll)();
    /* 0x124 */ void (*addScroll)();
    /* 0x128 */ void (*getViewRect)();
    /* 0x12C */ void (*setBgColor)(); /* no prototype: callers would truncate u8 args */
    /* 0x130 */ void (*draw)(struct Layer *);
    /* 0x134 */ void (*clearOt)(struct Layer *);
    /* 0x138 */ u_long *(*getOtEntry)(struct Layer *, s32 depth);
    /* 0x13C */ u_long *(*getOtEntryZ)(struct Layer *layer, s32 z);
    /* 0x140 */ u_long *(*getOt)(struct Layer *);
    /* 0x144 */ s32 (*getOtShift)();
    /* 0x148 */ void (*allocCallbacks)();
    /* 0x14C */ void (*addSortedCallback)();
    /* 0x150 */ void (*addCallback)();
    /* 0x154 */ void (*runCallbacks)(struct Layer *);
    /* 0x158 */ void (*loadView)(struct Layer *);
    /* 0x15C */ void (*setKeepView)();
    /* 0x160 */ void (*loadLightMatrix)(struct Layer *);
    /* 0x164 */ void (*setKeepLightMatrix)();
    /* 0x168 */ void (*free)(struct Layer *);
};

/* A card's image (CardDrawer.card): 64 to a file, a 12-byte header and a
   32x32 8-bit TIM */
typedef struct CardImage {
    /* 0x0 */ u8 color; /* from 1 */
    /* 0x1 */ u8 ap;
    /* 0x2 */ u8 hp;
    /* 0x3 */ u8 kind; /* an index in CARD_KINDS */
    /* 0x4 */ u8 unused4; /* 1-4 on the 60 option cards (a colour's first 4, its last two 1), 0 on the
                             Digimon; nothing reads it */
    /* 0x5 */ u8 points; /* what it adds to its colour */
    /* 0x6 */ u8 level; /* a Digimon's, 2-5 (CARDGAME shows its name); 0 on the other cards */
    /* 0x7 */ u8 pad7; /* 0 on every card; nothing reads it */
    /* 0x8 */ s16 rank; /* by strength, from 1 the strongest (0 on the two blank cards) */
    /* 0xA */ s16 comboCard; /* the card (from 1) that three or more of it make
                                (CARDGAME_findCardSet), 0 for none */
    /* 0xC */ u8 tim[0x620]; /* its header, a 256-colour CLUT block and the image block */
} CardImage;

/*
 * Draws the cards' images (CardImage), from files 0x7E7-0x7EB. There are 320
 * of them (the save has counts for 317). The methods act on the drawer that
 * was set up last (CARD_DRAWER).
 */
typedef struct CardDrawer {
    /* 0x00 */ CardImage *card;
    /* 0x04 */ s32 imageX; /* VRAM position of the image grid */
    /* 0x08 */ s32 imageY;
    /* 0x0C */ s32 clutX;
    /* 0x10 */ s32 clutY;
    /* 0x14 */ s32 cellX; /* grid cell: 32x32 pixels, one CLUT row each */
    /* 0x18 */ s32 cellY;
    /* 0x1C */ s32 clutStride;
    /* 0x20 */ s32 semiTrans;
    /* 0x24 */ Layer *layer;
    /* 0x28 */ u_long *ot;
    /* 0x2C */ void (*setCard)(); /* (id) */
    /* 0x30 */ void (*loadImage)();
    /* 0x34 */ void (*draw)(); /* (x, y) */
    /* 0x38 */ void (*setLayer)(); /* (id, depth) */
    /* 0x3C */ void (*setImagePos)(); /* (x, y) */
    /* 0x40 */ void (*setClutPos)(); /* (x, y) */
    /* 0x44 */ void (*setCell)(); /* (x, y) */
    /* 0x48 */ void (*setClutStride)(); /* (stride) */
    /* 0x4C */ void (*setSemiTrans)(); /* (on) */
    /* 0x50 */ s32 (*getKind)(void);
} CardDrawer;

/*
 * Draws sprites from a sprite sheet (such as file 0x277, the menu graphics):
 * a sheet has a table of frames, each made of textured parts, and can be
 * scaled, rotated and tinted. Set up on the stack with initSpriteDrawer; its
 * methods act on the drawer that was set up last (SPRITE_DRAWER).
 */
typedef struct SpriteDrawer {
    /* 0x00 */ Layer *layer;
    /* 0x04 */ u_long *ot;
    /* 0x08 */ s32 tpageX; /* VRAM position of the sheet's texture */
    /* 0x0C */ s32 tpageY;
    /* 0x10 */ s32 clutX;
    /* 0x14 */ s32 clutY;
    /* 0x18 */ s32 altClutX; /* for the parts that are flagged */
    /* 0x1C */ s32 altClutY;
    /* 0x20 */ s32 clutRow; /* added to the CLUT y: palette animations */
    /* 0x24 */ s32 followScroll; /* offset by the layer's scroll */
    /* 0x28 */ CVECTOR color;
    /* 0x2C */ s32 transformDirty;
    /* 0x30 */ s32 pivotX;
    /* 0x34 */ s32 pivotY;
    /* 0x38 */ VECTOR scale; /* ScaleMatrix's */
    /* 0x48 */ SVECTOR rot;
    /* 0x50 */ MATRIX matrix;
    /* 0x70 */ void (*bind)(); /* (drawer) */
    /* 0x74 */ void (*setTexture)(); /* (x, y) */
    /* 0x78 */ void (*setAltClut)(); /* (x, y) */
    /* 0x7C */ void (*setLayerId)(); /* (id, depth) */
    /* 0x80 */ void (*setLayer)(); /* (layer, depth) */
    /* 0x84 */ void (*draw)(); /* (sheet, frame, x, y) */
    /* 0x88 */ void (*setClutRow)(); /* (row) */
    /* 0x8C */ void (*setScale)(); /* (x, y, z), ONE = 1.0 */
    /* 0x90 */ void (*setRotation)(); /* (x, y, z); no prototype: callers would truncate s16 args */
    /* 0x94 */ void (*setPivot)(); /* (x, y) */
    /* 0x98 */ void (*setFollowScroll)(); /* (on) */
    /* 0x9C */ void (*setColor)(); /* (CVECTOR *) */
} SpriteDrawer;

/*
 * One part of a sprite sheet, a rectangle of its texture. A sheet starts with
 * three offsets from its start: to its parts, to the list of its frame ids
 * (bytes) and to its first frame, followed by those of the other frames. A
 * frame is three s16, the part count, a CLUT row added to the parts' and the
 * semi-transparency rate (-1: opaque), then an s16 part index, x and y for
 * each part; they are drawn from the last to the first.
 */
typedef struct SpritePart {
    /* 0x0 */ s16 u;
    /* 0x2 */ s16 v;
    /* 0x4 */ s16 w;
    /* 0x6 */ s16 h;
    /* 0x8 */ s16 clutX; /* added to the drawer's CLUT position */
    /* 0xA */ s16 clutY;
    /* 0xC */ s16 mode; /* 0: 4-bit texture, 1: 8-bit, with the drawer's other CLUT */
} SpritePart;

/* One frame of a sprite animation; frame 0xFF ends it (or loops it) */
typedef struct AnimFrame {
    /* 0x0 */ s16 frame;
    /* 0x2 */ s16 duration;
} AnimFrame;

typedef struct AnimState {
    /* 0x0 */ s16 index;
    /* 0x2 */ s16 timer;
} AnimState;

/* A TIM's CLUT or image block */
typedef struct TimBlock {
    /* 0x0 */ u32 size; /* in bytes, this header included */
    /* 0x4 */ u16 x;
    /* 0x6 */ u16 y;
    /* 0x8 */ u16 w; /* in halfwords */
    /* 0xA */ u16 h;
    /* 0xC */ u_long pixels[FLEXIBLE];
} TimBlock;

/* Where timLoaderLoad is in a TIM: its words, then its blocks, which their
   sizes in bytes take it past */
typedef union TimCursor {
    u_long *word;
    u8 *byte;
    TimBlock *block;
} TimCursor;

/* Uploads TIM images to VRAM (initTimLoader); acts on TIM_LOADER */
typedef struct TimLoader {
    /* 0x00 */ u16 w; /* size of the last image */
    /* 0x02 */ u16 h;
    /* 0x04 */ u8 pad4[4]; /* nothing reads or writes them */
    /* 0x08 */ s32 imageX;
    /* 0x0C */ s32 imageY;
    /* 0x10 */ s32 clutX;
    /* 0x14 */ s32 clutY;
    /* 0x18 */ s32 bufferSize; /* for the RLE-compressed images */
    /* 0x1C */ void (*bind)(); /* (loader) */
    /* 0x20 */ void (*load)(); /* (TIM) */
    /* 0x24 */ void (*setImagePos)(); /* (x, y) */
    /* 0x28 */ void (*setClutPos)(); /* (x, y) */
    /* 0x2C */ void (*loadArchive)(); /* (archive): each image 64 halfwords to the right */
    /* 0x30 */ void (*setBufferSize)(); /* (size) */
} TimLoader;

/* The next free primitive of the packet buffer (getPrim), written as the
   kind that is drawn next */
typedef union PrimPtr {
    void *any;
    SPRT *sprt;
    POLY_FT4 *ft4;
    DR_TPAGE *tpage;
} PrimPtr;

typedef struct Vec2 {
    s32 x;
    s32 y;
} Vec2;

/* ScreenFade.level when the screen is black: 0xFF in 8.8 fixed point */
#define FADE_LEVEL_MAX 0xFF00

/* Fades the whole screen to black and back with a subtractive rectangle */
typedef struct ScreenFade {
    TASK_HEADER(ScreenFade);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 fadeIn; /* 0: to black */
    /* 0x5C */ s32 level; /* 0-FADE_LEVEL_MAX */
    /* 0x60 */ s32 levelStep;
    /* 0x64 */ void (*start)(struct ScreenFade *fade, s32 fadeIn, s32 frames); /* TASK_DONE when done */
} ScreenFade;

Layer *getLayer(s32 id);
Layer *createLayer(RECT *rect, s32 otShift, s32 id);
s32 destroyLayer(s32 id);
void moveLayer(s32 id, s32 targetId, s32 delta);
void layerClearOt(Layer *layer);
u_long *layerGetOtEntryZ(Layer *layer, s32 z);
u_long *layerGetOt(Layer *layer);
s32 layerGetOtShift(LayerView *layer);
void layerSetBgColor(LayerView *layer, s32 r, s32 g, s32 b); /* old-style: u8 r, g, b */
void layerSetOffset(LayerView *layer, s32 x, s32 y); /* old-style: s16 x, y */
void layerGetScroll(LayerView *layer, Vec2 *out);
void layerSetScroll(LayerView *layer, s32 x, s32 y);
void layerAddScroll(LayerView *layer, s32 dx, s32 dy);
void layerSetClipPos(LayerView *layer, s32 x, s32 y); /* old-style: s16 x, y */
void layerSetClipSize(LayerView *layer, s32 w, s32 h); /* old-style: s16 w, h */
void layerGetViewRect(LayerView *layer, Rect16 *rect);
void layerFree(Layer *layer);
void layerResetCallbacks(Layer *layer);
void layerAllocCallbacks(Layer *layer, s32 count);
void layerAddSortedCallback(Layer *layer, void (*func)(s32, void *, s32), s32 arg, s32 priority, s32 param);
void layerAddCallback(Layer *layer, void (*func)(s32, void *, s32), s32 arg);
void layerRunCallbacks(Layer *layer);
void layerSetKeepView(Layer *layer, s32 enable, s32 projection);
void layerLoadView(Layer *layer);
void layerSetKeepLightMatrix(Layer *layer, s32 enable);
void layerLoadLightMatrix(Layer *layer);
void layerSkipEmptyOt(Layer *layer);
void layerDraw(Layer *layer);
u_long *layerGetOtEntry(Layer *layer, s32 depth);
Layer *newLayer(DRAWENV *env, s32 otShift);
s32 findLayerSlot(s32 id);
void removeLayerSlot(s32 index);
void insertLayerSlot(s32 index, Layer *layer, s32 id);
void initTimLoader(TimLoader *obj);
void vsyncCallback(void);
void startVSyncCallback(void);
void drawFrame(s32 draw);
s32 getFrameCount(void);
s32 getTime(void);
s32 getFrameTime(void);
void resetGraphics(void);
void allocPrimBuffers(s32 size);
void *getPrim(void);
void setPrim(void *next);
void freePrimBuffers(void);
void setDisplayMode(s32 w, s32 h, s32 hires, s32 interlace);
void setDisplayArea(s32 x, s32 y, s32 w, s32 h);
void bindCardDrawer(CardDrawer *obj);
void cardDrawerSetCard(s32 id);
void cardDrawerLoadImage(void);
void cardDrawerSetLayer(s32 id, s32 depth);
void cardDrawerSetImagePos(s32 x, s32 y);
void cardDrawerSetClutPos(s32 x, s32 y);
void cardDrawerSetCell(s32 x, s32 y);
void cardDrawerSetClutStride(s32 stride);
void cardDrawerSetSemiTrans(s32 on);
void cardDrawerDraw(s32 x, s32 y);
s32 cardDrawerGetKind(void);
void bindSpriteDrawer(SpriteDrawer *obj);
void spriteDrawerSetTexture(s32 x, s32 y);
void spriteDrawerSetAltClut(s32 x, s32 y);
void spriteDrawerSetClutRow(s32 row);
void spriteDrawerSetLayer(Layer *layer, s32 arg1);
void spriteDrawerSetLayerId(s32 id, s32 depth);
void spriteDrawerDraw(s32 *sheet, s32 frame, s32 x, s32 y);
void spriteDrawerSetScale(s32 x, s32 y, s32 z);
void spriteDrawerSetRotation(s32 x, s32 y, s32 z); /* old-style: s16 x, y, z */
void spriteDrawerSetPivot(s32 x, s32 y);
void spriteDrawerSetFollowScroll(s32 on);
void spriteDrawerSetColor(CVECTOR *color);
void bindTimLoader(TimLoader *obj);
void timLoaderSetImagePos(s32 x, s32 y);
void timLoaderSetClutPos(s32 x, s32 y);
void timLoaderLoad(void *tim);
void timLoaderLoadArchive(void *archive);
void timLoaderSetBufferSize(s32 size);
void initSpriteDrawer(struct SpriteDrawer *obj);
void initCardDrawer(CardDrawer *obj);
void screenFadeStart(ScreenFade *task, s32 fadeIn, s32 duration);
ScreenFade *createScreenFade(s32 layerId);
void drawScreenFade(struct ScreenFade *task);
void updateScreenFade(struct ScreenFade *task);

extern s16 OT_LENGTHS[];
extern s32 CARD_IMAGE_FILES[];
extern GfxState GFX;
#if PAL_VIDEO
/* The European version runs at 50 Hz (PAL) unless NTSC_MODE is set; its
   clocks then advance 0x133, 60/50 of 0x100, per frame */
extern s32 NTSC_MODE;
extern s32 SHIFT_PAL_SCREEN; /* moves both display areas 0x18 lines down */
#endif
extern s32 CARD_KINDS[];

#endif /* DW3_GFX_H */
