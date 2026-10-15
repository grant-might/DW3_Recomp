#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Small variables, addressed through $gp (see the Makefile) */
static SpriteDrawer *SPRITE_DRAWER;

/* Makes `obj` the sprite drawer the methods work on */
void bindSpriteDrawer(SpriteDrawer *obj) {
    SPRITE_DRAWER = obj;
}

/* Sprite drawer method: the VRAM position of the sheet's texture and CLUTs */
void spriteDrawerSetTexture(s32 x, s32 y) {
    SPRITE_DRAWER->tpageX = x;
    SPRITE_DRAWER->tpageY = y;
    SPRITE_DRAWER->clutX = x;
    SPRITE_DRAWER->clutY = y;
}

/* Sprite drawer method: the other CLUT area, for parts that ask for it */
void spriteDrawerSetAltClut(s32 x, s32 y) {
    SPRITE_DRAWER->altClutX = x;
    SPRITE_DRAWER->altClutY = y - 0x100;
}

/* Sprite drawer method: the CLUT row added to every part's */
void spriteDrawerSetClutRow(s32 row) {
    SPRITE_DRAWER->clutRow = row;
}

/* Sprite drawer method: the layer and depth to draw to */
void spriteDrawerSetLayer(Layer *layer, s32 depth) {
    SPRITE_DRAWER->layer = layer;
    SPRITE_DRAWER->ot = layer->getOtEntry(layer, depth);
}

/* Sprite drawer method: the layer (by id) and depth to draw to */
void spriteDrawerSetLayerId(s32 id, s32 depth) {
    spriteDrawerSetLayer(GFX.funcs.getLayer(id), depth);
}

/*
 * Draws frame `frame` of a sprite sheet (see SpritePart) at (x, y): as
 * sprites, changing the texture page between parts when needed, or as
 * textured quads when the drawer is scaled or rotated.
 */
void spriteDrawerDraw(s32 *sheet, s32 frame, s32 x, s32 y) {
    Vec2 scroll;
    SVECTOR out;
    SVECTOR in[4];
    SpritePart *parts;
    SpritePart *part;
    u8 *ids;
    s16 *p;
    PrimPtr prim;
    s32 transform;
    s32 count;
    s32 frameRow;
    s32 semi;
    s32 abr;
    s32 i;
    s32 k;
    u8 u;
    u8 v;

    transform = 0;
    /* the sheet starts with byte offsets: to its parts, its frame ids, then
       each frame's list of parts */
    ids = (u8 *)sheet + sheet[1];
    parts = (SpritePart *)((u8 *)sheet + sheet[0]);
    for (i = 0; ids[i] != frame; i++) {
    }
    p = (s16 *)((u8 *)sheet + sheet[i + 2]);
    /* rot.vx and rot.vy are tested as one word */
    if (SPRITE_DRAWER->scale.vx != ONE || SPRITE_DRAWER->scale.vy != ONE || SPRITE_DRAWER->scale.vz != ONE ||
        *(s32 *)&SPRITE_DRAWER->rot.vx != 0 || SPRITE_DRAWER->rot.vz != 0) {
        transform = 1;
        if (SPRITE_DRAWER->transformDirty) {
            RotMatrixYXZ_gte(&SPRITE_DRAWER->rot, &SPRITE_DRAWER->matrix);
            ScaleMatrix(&SPRITE_DRAWER->matrix, &SPRITE_DRAWER->scale);
        }
    }
    count = *p++;
    frameRow = *p++;
    abr = *p++;
    if (abr == -1) {
        semi = 0;
        abr = 0;
    } else {
        semi = 1;
    }
    if (SPRITE_DRAWER->followScroll) {
        SPRITE_DRAWER->layer->getScroll(SPRITE_DRAWER->layer, &scroll);
    } else {
        scroll.x = 0;
        scroll.y = 0;
    }
    p += (count - 1) * 3;
    prim.any = GFX.funcs.getPrim();
    if (!transform) {
        u16 clut;
        u16 tpage;
        u16 prevTpage;

        tpage = 0;
        prevTpage = 0;
        for (i = 0; i < count; i++) {
            part = &parts[p[0]];
            clut = getClut((part->mode ? SPRITE_DRAWER->altClutX : SPRITE_DRAWER->clutX) + part->clutX,
                           (part->mode ? SPRITE_DRAWER->altClutY : SPRITE_DRAWER->clutY) + part->clutY + frameRow +
                               SPRITE_DRAWER->clutRow);
            tpage = getTPage(part->mode, abr, SPRITE_DRAWER->tpageX + (part->mode ? part->u / 2 : part->u / 4),
                             SPRITE_DRAWER->tpageY);
            if (i == 0) {
                prevTpage = tpage;
            }
            if (prevTpage != tpage) {
                SetDrawTPage(prim.tpage, 0, 1, prevTpage);
                addPrim(SPRITE_DRAWER->ot, prim.any);
                prim.tpage++;
                prevTpage = tpage;
            }
            /* the colour and the code byte as one word: setSprt sets the code after */
            *(CVECTOR *)&prim.sprt->r0 = SPRITE_DRAWER->color;
            setSprt(prim.sprt);
            if (semi) {
                setSemiTrans(prim.sprt, 1);
            }
            prim.sprt->x0 = p[1] + x - scroll.x;
            prim.sprt->y0 = p[2] + y - scroll.y;
            if (part->mode) {
                prim.sprt->u0 = part->u & 0x7F;
            } else {
                prim.sprt->u0 = part->u;
            }
            prim.sprt->v0 = part->v;
            prim.sprt->w = part->w;
            prim.sprt->h = part->h;
            prim.sprt->clut = clut;
            addPrim(SPRITE_DRAWER->ot, prim.any);
            prim.sprt++;
            p -= 3;
        }
        SetDrawTPage(prim.tpage, 0, 1, tpage);
        addPrim(SPRITE_DRAWER->ot, prim.any);
        prim.tpage++;
    } else {
        u16 clut;
        u16 tpage;

        for (i = 0; i < count; i++) {
            part = &parts[p[0]];
            clut = getClut((part->mode ? SPRITE_DRAWER->altClutX : SPRITE_DRAWER->clutX) + part->clutX,
                           (part->mode ? SPRITE_DRAWER->altClutY : SPRITE_DRAWER->clutY) + part->clutY + frameRow +
                               SPRITE_DRAWER->clutRow);
            tpage = getTPage(part->mode, abr, SPRITE_DRAWER->tpageX + (part->mode ? part->u / 2 : part->u / 4),
                             SPRITE_DRAWER->tpageY);
            /* as one word, as above */
            *(CVECTOR *)&prim.ft4->r0 = SPRITE_DRAWER->color;
            setPolyFT4(prim.ft4);
            if (semi) {
                setSemiTrans(prim.ft4, 1);
            }
            in[0].vx = in[2].vx = p[1] + x - SPRITE_DRAWER->pivotX;
            in[1].vx = in[3].vx = in[0].vx + part->w;
            in[0].vy = in[1].vy = p[2] + y - SPRITE_DRAWER->pivotY;
            in[2].vy = in[3].vy = in[0].vy + part->h;
            in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
            for (k = 0; k < 4; k++) {
                ApplyMatrixSV(&SPRITE_DRAWER->matrix, &in[k], &out);
                (&prim.ft4->x0)[k * 4] = out.vx - scroll.x + SPRITE_DRAWER->pivotX;
                (&prim.ft4->y0)[k * 4] = out.vy - scroll.y + SPRITE_DRAWER->pivotY;
            }
            if (part->mode) {
                u = part->u & 0x7F;
            } else {
                u = part->u;
            }
            prim.ft4->u0 = prim.ft4->u2 = u;
            prim.ft4->u1 = prim.ft4->u3 = u + part->w - 1;
            v = part->v;
            prim.ft4->v0 = prim.ft4->v1 = v;
            prim.ft4->v2 = prim.ft4->v3 = v + part->h - 1;
            prim.ft4->tpage = tpage;
            prim.ft4->clut = clut;
            addPrim(SPRITE_DRAWER->ot, prim.any);
            prim.ft4++;
            p -= 3;
        }
    }
    GFX.funcs.setPrim(prim.any);
}

/* Sprite drawer method: scales the sprites (ONE: as they are) */
void spriteDrawerSetScale(s32 x, s32 y, s32 z) {
    SPRITE_DRAWER->scale.vx = x;
    SPRITE_DRAWER->scale.vy = y;
    SPRITE_DRAWER->scale.vz = z;
    SPRITE_DRAWER->transformDirty = 1;
}

/*
 * Sprite drawer method: rotates the sprites. An old-style definition, as
 * SpriteDrawer.setRotation's unprototyped callers suit: they pass ints,
 * which it takes as shorts.
 */
void spriteDrawerSetRotation(x, y, z)
s16 x;
s16 y;
s16 z;
{
    SPRITE_DRAWER->rot.vx = x;
    SPRITE_DRAWER->rot.vy = y;
    SPRITE_DRAWER->rot.vz = z;
    SPRITE_DRAWER->transformDirty = 1;
}

/* Sprite drawer method: the point the sprites scale and rotate around */
void spriteDrawerSetPivot(s32 x, s32 y) {
    SPRITE_DRAWER->pivotX = x;
    SPRITE_DRAWER->pivotY = y;
}

/* Sprite drawer method: whether the sprites move with the layer's scroll */
void spriteDrawerSetFollowScroll(s32 on) {
    SPRITE_DRAWER->followScroll = on;
}

/* Sprite drawer method: the color the sprites are tinted with */
void spriteDrawerSetColor(CVECTOR *color) {
    SPRITE_DRAWER->color = *color;
}

/*
 * Clears a sprite drawer to neutral (no scale, gray tint, follows the scroll), gives it its
 * methods and binds it
 */
void initSpriteDrawer(SpriteDrawer *obj) {
    HEAP.zero(obj, sizeof(SpriteDrawer));
    obj->scale.vx = ONE;
    obj->scale.vy = ONE;
    obj->scale.vz = ONE;
    obj->followScroll = 1;
    obj->color.b = 0x80;
    obj->color.g = 0x80;
    obj->color.r = 0x80;
    obj->setTexture = spriteDrawerSetTexture;
    obj->draw = spriteDrawerDraw;
    obj->setAltClut = spriteDrawerSetAltClut;
    obj->setLayerId = spriteDrawerSetLayerId;
    obj->setLayer = spriteDrawerSetLayer;
    obj->bind = bindSpriteDrawer;
    obj->setClutRow = spriteDrawerSetClutRow;
    obj->setScale = spriteDrawerSetScale;
    obj->setRotation = spriteDrawerSetRotation;
    obj->setPivot = spriteDrawerSetPivot;
    obj->setFollowScroll = spriteDrawerSetFollowScroll;
    obj->setColor = spriteDrawerSetColor;
    bindSpriteDrawer(obj);
}
