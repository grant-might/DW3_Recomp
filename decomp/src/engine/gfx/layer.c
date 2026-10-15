#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* The layer with id `id`, or NULL */
Layer *getLayer(s32 id) {
    s32 i;

    for (i = 0; i < LAYER_COUNT; i++) {
        if (GFX.layers[i] != NULL && GFX.layerIds[i] == id) {
            return GFX.layers[i];
        }
    }
    return NULL;
}

/* The slot of the layer with this id; id 0 finds a free slot */
s32 findLayerSlot(s32 id) {
    s32 i;

    for (i = 0; i < LAYER_COUNT; i++) {
        if (id != 0) {
            if (GFX.layers[i] != NULL && GFX.layerIds[i] == id) {
                return i;
            }
        } else if (GFX.layers[i] == NULL) {
            return i;
        }
    }
    return -1;
}

/* Closes the gap of a removed layer in the draw order */
void removeLayerSlot(s32 index) {
    for (; index < LAYER_COUNT - 1; index++) {
        GFX.layers[index] = GFX.layers[index + 1];
        GFX.layerIds[index] = GFX.layerIds[index + 1];
    }
}

/* Puts a layer at position `index` of the draw order, moving the later ones back */
void insertLayerSlot(s32 index, Layer *layer, s32 id) {
    s32 i;

    for (i = LAYER_COUNT - 1; i != index; i--) {
        GFX.layers[i] = GFX.layers[i - 1];
        GFX.layerIds[i] = GFX.layerIds[i - 1];
    }
    GFX.layers[index] = layer;
    GFX.layerIds[index] = id;
}

/*
 * A new layer drawing to `rect`, with an OT of OT_LENGTHS[otShift - 1] entries; NULL when all are
 * in use
 */
Layer *createLayer(RECT *rect, s32 otShift, s32 id) {
    DRAWENV env;
    s32 index = findLayerSlot(0);

    if (index != -1) {
        SetDefDrawEnv(&env, rect->x, rect->y, rect->w, rect->h);
        GFX.layerIds[index] = id;
        return GFX.layers[index] = newLayer(&env, otShift);
    }
    return NULL;
}

/* Frees the layer with id `id`; 0 if there is none */
s32 destroyLayer(s32 id) {
    s32 index = findLayerSlot(id);
    Layer *layer;

    if (index != -1) {
        layer = GFX.layers[index];
        layer->free(layer);
        HEAP.free(GFX.layers[index]);
        removeLayerSlot(index);
        return 1;
    }
    return 0;
}

/* Moves a layer to the position of another one (plus delta) in the draw order */
void moveLayer(s32 id, s32 targetId, s32 delta) {
    s32 from = findLayerSlot(id);
    s32 to = findLayerSlot(targetId);
    s32 pos;
    Layer *layer;
    s32 layerId;

    if (from != -1 && to != -1) {
        pos = to + delta;
        layer = GFX.layers[from];
        layerId = GFX.layerIds[from];
        if (pos <= 0) {
            pos = 0;
        }
        removeLayerSlot(from);
        insertLayerSlot(pos, layer, layerId);
    }
}

/* Layer method: clears the OT of the buffer being drawn */
void layerClearOt(Layer *layer) {
    ClearOTagR(layer->ot[GFX.buffer], layer->otLen);
}

/* Links every run of empty OT entries past itself, so the GPU skips it */
void layerSkipEmptyOt(Layer *layer) {
    u_long mask = 0xFFFFFF;
    u_long *ot = layer->ot[GFX.buffer];
    u_long *p = ot + layer->otLen - 1;
    u_long *q;

    while (p != ot) {
        q = p - 1;
        if ((*p & mask) == ((u_long)q & mask)) {
            while ((*q & mask) == ((u_long)(q - 1) & mask)) {
                q--;
            }
            *p = (u_long)q & mask;
        }
        p = q;
    }
}

/* Layer method: sends the OT of the finished buffer, after its DRAWENV with the layer's offset */
void layerDraw(Layer *layer) {
    DRAWENV env = layer->env;
    DR_ENV *dr;
    u_long *ot;
    u_long *tag;

    env.ofs[0] = layer->offsetX;
    env.ofs[1] = layer->offsetY;
    layerSkipEmptyOt(layer);
    dr = GFX.prim;
    ot = layer->ot[GFX.buffer] + layer->otLen;
    tag = ot - 1;
    if (GFX.buffer != 0) {
        env.clip.y += 256;
        env.ofs[1] += 256;
    }
    SetDrawEnv(dr, &env);
    addPrim(ot - 1, dr);
    dr++;
    GFX.prim = dr;
    DrawOTag(tag);
}

/* Layer method: the OT entry at `depth` in the buffer being drawn */
u_long *layerGetOtEntry(Layer *layer, s32 depth) {
    return layer->ot[GFX.buffer] + depth;
}

/* Layer method: the OT entry for a 16-bit Z, scaled to the OT's length */
u_long *layerGetOtEntryZ(Layer *layer, s32 z) {
    s32 i = z >> (16 - layer->otShift);

    return layer->ot[GFX.buffer] + i;
}

/* Layer method: the OT of the buffer being drawn */
u_long *layerGetOt(Layer *layer) {
    return layer->ot[GFX.buffer];
}

/* Layer method: the OT size it was created with */
s32 layerGetOtShift(LayerView *layer) {
    return layer->otShift;
}

/*
 * Layer method: the color the layer's area is cleared to (black: not
 * cleared). An old-style definition, as Layer.setBgColor's unprototyped
 * callers suit: they pass ints, which it takes as bytes.
 */
void layerSetBgColor(layer, r, g, b)
LayerView *layer;
u8 r;
u8 g;
u8 b;
{
    layer->bgR = r;
    layer->bgB = b;
    layer->bgG = g;
    if (r | g | b) {
        layer->isbg = 1;
    } else {
        layer->isbg = 0;
    }
}

/* Layer method: the drawing offset. Old-style, as layerSetBgColor */
void layerSetOffset(layer, x, y)
LayerView *layer;
s16 x;
s16 y;
{
    layer->offsetX = x;
    layer->offsetY = y;
}

/* Layer method: the scroll position, in pixels */
void layerGetScroll(LayerView *layer, Vec2 *out) {
    out->x = layer->scrollX >> 8;
    out->y = layer->scrollY >> 8;
}

/* Layer method: sets the scroll position (8.8 fixed point) */
void layerSetScroll(LayerView *layer, s32 x, s32 y) {
    layer->scrollX = x;
    layer->scrollY = y;
}

/* Layer method: scrolls by (dx, dy) (8.8 fixed point) */
void layerAddScroll(LayerView *layer, s32 dx, s32 dy) {
    layer->scrollX += dx;
    layer->scrollY += dy;
}

/* Layer method: moves the clip rectangle. Old-style, as layerSetBgColor */
void layerSetClipPos(layer, x, y)
LayerView *layer;
s16 x;
s16 y;
{
    layer->x = x;
    layer->y = y;
}

/* Layer method: resizes the clip rectangle. Old-style, as layerSetBgColor */
void layerSetClipSize(layer, w, h)
LayerView *layer;
s16 w;
s16 h;
{
    layer->w = w;
    layer->h = h;
}

/* Layer method: the part of the scrolled world the clip rectangle shows */
void layerGetViewRect(LayerView *layer, Rect16 *rect) {
    rect->x = layer->x - layer->offsetX + (layer->scrollX >> 8);
    rect->y = layer->y - layer->offsetY + (layer->scrollY >> 8);
    rect->w = layer->w;
    rect->h = layer->h;
}

/* Layer method: frees its OTs and draw callbacks, once the GPU is done with them */
void layerFree(Layer *layer) {
    DrawSync(0);
    HEAP.free(layer->ot[0]);
    HEAP.free(layer->ot[1]);
    if (layer->callbackCap != 0) {
        HEAP.free(layer->callbacks);
    }
}

/* Layer method: empties the draw callbacks, but for the head entry, which sorts first */
void layerResetCallbacks(Layer *layer) {
    layer->callbacks->priority = 0x7FFFFFFF;
    layer->callbacks->param = 0;
    layer->callbacks->func = NULL;
    layer->callbacks->arg = 0;
    layer->callbacks->next = NULL;
    layer->callbackCount = 1;
}

/* Layer method: room for `count` draw callbacks */
void layerAllocCallbacks(Layer *layer, s32 count) {
    layer->callbacks = HEAP.alloc(count * sizeof(DrawCallback), MEM_MODE);
    layer->callbackCap = count;
    layerResetCallbacks(layer);
}

/* Layer method: adds a draw callback in order of priority, highest first */
void layerAddSortedCallback(Layer *layer, void (*func)(s32, void *, s32), s32 arg, s32 priority, s32 param) {
    DrawCallback *cur;
    DrawCallback *prev;
    DrawCallback *e;
    s32 i;
    s32 cap;

    if (layer->callbackCount < layer->callbackCap) {
        cur = layer->callbacks;
        e = &cur[layer->callbackCount];
        e->func = func;
        e->arg = arg;
        e->priority = priority;
        e->param = param;
        prev = NULL;
        if (layer->callbackCount != 0) {
            cap = layer->callbackCap;
            for (i = 0; i < cap; i++) {
                if (cur->priority < priority) {
                    cur = prev;
                    break;
                }
                prev = cur;
                if (cur->next == NULL) {
                    break;
                }
                cur = cur->next;
            }
            if (cur->next == NULL) {
                cur->next = e;
                e->next = NULL;
            } else {
                e->next = cur->next;
                cur->next = e;
            }
        } else {
            e->next = NULL;
        }
        layer->callbackCount++;
    }
}

/* Layer method: adds a draw callback at the end of the list */
void layerAddCallback(Layer *layer, void (*func)(s32, void *, s32), s32 arg) {
    DrawCallback *cb;

    if (layer->callbackCount < layer->callbackCap) {
        cb = &layer->callbacks[layer->callbackCount];
        cb->func = func;
        cb->arg = arg;
        cb->priority = 0;
        cb->param = 0;
        cb->next = NULL;
        if (layer->callbackCount != 0) {
            cb[-1].next = cb;
        }
        layer->callbackCount++;
    }
}

/* Layer method: runs the draw callbacks in order, then empties the list */
void layerRunCallbacks(Layer *layer) {
    DrawCallback *cb;

    if (layer->callbackCount) {
        cb = layer->callbacks;
        do {
            if (cb->func != NULL) {
                cb->func(cb->arg, layer, cb->param);
            }
            cb = cb->next;
        } while (cb != NULL);
        layerResetCallbacks(layer);
    }
}

/* Layer method: keeps the current world-screen matrix and projection for this buffer's drawing */
void layerSetKeepView(Layer *layer, s32 enable, s32 projection) {
    layer->keepView = enable;
    if (enable) {
        layer->projection = projection;
        layer->view[GFX.buffer] = GsWSMATRIX;
    }
}

/* Layer method: restores the view that setKeepView kept */
void layerLoadView(Layer *layer) {
    GsSetProjection(layer->projection);
    GsWSMATRIX = layer->view[GFX.buffer];
}

/* Layer method: keeps the current light matrix for this buffer's drawing */
void layerSetKeepLightMatrix(Layer *layer, s32 enable) {
    layer->keepLightMatrix = enable;
    if (enable) {
        layer->lightMatrix[GFX.buffer] = GsLIGHTWSMATRIX;
    }
}

/* Layer method: restores the light matrix that setKeepLightMatrix kept */
void layerLoadLightMatrix(Layer *layer) {
    GsLIGHTWSMATRIX = layer->lightMatrix[GFX.buffer];
}

/* Allocates a layer with its two OTs and methods */
Layer *newLayer(DRAWENV *env, s32 otShift) {
    Layer *layer = HEAP.allocZeroed(sizeof(Layer), MEM_MODE);

    layer->env = *env;
    layer->otShift = otShift;
    layer->otLen = OT_LENGTHS[otShift - 1];
    layer->ot[0] = HEAP.alloc(layer->otLen * sizeof(u_long), MEM_MODE);
    layer->ot[1] = HEAP.alloc(layer->otLen * sizeof(u_long), MEM_MODE);
    ClearOTagR(layer->ot[0], layer->otLen);
    ClearOTagR(layer->ot[1], layer->otLen);
    layer->setBgColor = layerSetBgColor;
    layer->draw = layerDraw;
    layer->clearOt = layerClearOt;
    layer->getOtEntry = layerGetOtEntry;
    layer->getOtEntryZ = layerGetOtEntryZ;
    layer->free = layerFree;
    layer->setClipPos = layerSetClipPos;
    layer->setClipSize = layerSetClipSize;
    layer->setOffset = layerSetOffset;
    layer->setScroll = layerSetScroll;
    layer->addScroll = layerAddScroll;
    layer->getScroll = layerGetScroll;
    layer->getViewRect = layerGetViewRect;
    layer->getOt = layerGetOt;
    layer->getOtShift = layerGetOtShift;
    layer->allocCallbacks = layerAllocCallbacks;
    layer->addSortedCallback = layerAddSortedCallback;
    layer->addCallback = layerAddCallback;
    layer->runCallbacks = layerRunCallbacks;
    layer->setKeepView = layerSetKeepView;
    layer->loadView = layerLoadView;
    layer->setKeepLightMatrix = layerSetKeepLightMatrix;
    layer->loadLightMatrix = layerLoadLightMatrix;
    return layer;
}

/* A layer's ordering table length, by its otShift - 1 */
s16 OT_LENGTHS[] = {
    0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000,
};
