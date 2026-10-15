/* The training's sprites (TrainSprite), which begin STGTRAIN.PRO's first
   object. STGTRAIN.PRO was at least three objects: each one's jump tables are
   aligned to 8 from the start of its own rodata, and the tables at 0x8008251C
   and 0x800825E8 (USA) each start right where the one before ends, 4 bytes past
   a multiple of 8, so each one starts a new object. Where each object's code
   starts is only known to be between the function with the last jump table of
   the object before and the one with its first: here at the training result
   (result.c) and the session (session.c). Each object's data is in its last
   module */

#include "menus/gym.h"

/* Sets the sprite bank and where the sprites start in it */
void STGTRAIN_setSpriteBank(TrainSprite *sprite, TrainSpriteBank *bank, s32 offset) {
    sprite->bank = bank;
    sprite->bankOffset = offset;
    sprite->unk84 = bank->unk2;
    sprite->spriteCount = bank->spriteCount;
}

/* Starts an animation from its first frame */
void STGTRAIN_setSpriteAnim(TrainSprite *sprite, TrainAnim *anim) {
    if (anim != NULL) {
        sprite->anim = anim;
        sprite->frame = 0;
        sprite->frameTime = 0;
        sprite->frameCount = anim->frameCount;
        sprite->flags = 0;
    }
}

/* Sets the layer and depth the sprite is drawn at */
void STGTRAIN_setSpriteLayer(TrainSprite *sprite, s32 layerId, s32 depth) {
    sprite->layerId = layerId;
    sprite->depth = depth;
}

/* Sets where the sprite is drawn */
void STGTRAIN_setSpritePos(TrainSprite *sprite, s32 x, s32 y) {
    sprite->x = x;
    sprite->y = y;
}

/* Sets where the sprite's image is in VRAM */
void STGTRAIN_setSpriteImagePos(TrainSprite *sprite, s32 x, s32 y) {
    sprite->imageX = x;
    sprite->imageY = y;
}

/* Sets where the sprite's CLUT is in VRAM */
void STGTRAIN_setSpriteClutPos(TrainSprite *sprite, s32 x, s32 y) {
    sprite->clutX = x;
    sprite->clutY = y;
}

/* Scales the sprite (0x1000 is its size), drawing it as quads */
void STGTRAIN_setSpriteScale(TrainSprite *sprite, s32 x, s32 y, s32 z) {
    sprite->scale.vx = x;
    sprite->scale.vy = y;
    sprite->scale.vz = z;
    sprite->transformed = 1;
}

/* Sets the point the sprite scales and turns about */
void STGTRAIN_setSpritePivot(TrainSprite *sprite, s32 x, s32 y) {
    sprite->pivotX = x;
    sprite->pivotY = y;
}

/* Turns the sprite, drawing it as quads */
void STGTRAIN_setSpriteRotation(TrainSprite *sprite, s16 x, s16 y, s16 z) {
    sprite->rotation.vx = x;
    sprite->rotation.vy = y;
    sprite->rotation.vz = z;
    sprite->transformed = 1;
}

/* Whether the animation has ended (the flags but for the pause) */
s32 STGTRAIN_getSpriteFlags(TrainSprite *sprite) {
    return sprite->flags & 0x7FFFFFFF;
}

/* Pauses the animation (paused) or plays it on */
void STGTRAIN_setSpritePaused(TrainSprite *sprite, s32 paused) {
    if (paused) {
        sprite->flags |= 0x80000000;
    } else {
        sprite->flags &= 0x7FFFFFFF;
    }
}

/* Fills in a part of the sprite as a sprite, and adds it to the sprite's
   ordering table */
static inline void STGTRAIN_addSpriteSprt(TrainSprite *sprite, SPRT *sprt, TrainSpritePart *part,
                                          TrainAnimFrame *frame, s32 x, s32 y, u8 u, u8 v, u16 w, u16 h,
                                          u16 clut) {
    setSprt(sprt);
    if ((s16)part->clut & 0x8000) {
        setSemiTrans(sprt, 1);
    }
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->x0 = x + (frame->x + sprite->x);
    sprt->y0 = y + (frame->y + sprite->y);
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->w = w;
    sprt->h = h;
    sprt->clut = clut;
    addPrim(sprite->ot, sprt);
}

/* Fills in a part of the sprite as a quad, scaled and rotated about the
   pivot, and adds it to the sprite's ordering table */
static inline void STGTRAIN_addSpriteQuad(TrainSprite *sprite, POLY_FT4 *ft4, TrainSpritePart *part,
                                          TrainAnimFrame *frame, s32 x, s32 y, u8 u, u8 v, u16 w, u16 h,
                                          u16 tpage, u16 clut) {
    SVECTOR out;
    SVECTOR in[4];
    s32 k;

    setPolyFT4(ft4);
    if ((s16)part->clut & 0x8000) {
        setSemiTrans(ft4, 1);
    }
    setRGB0(ft4, 0x80, 0x80, 0x80);
    in[0].vx = in[2].vx = x + (frame->x + sprite->x) - sprite->pivotX;
    in[1].vx = in[3].vx = in[0].vx + w;
    in[0].vy = in[1].vy = y + (frame->y + sprite->y) - sprite->pivotY;
    in[2].vy = in[3].vy = in[0].vy + h;
    in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
    for (k = 0; k < 4; k++) {
        ApplyMatrixSV(&sprite->matrix, &in[k], &out);
        (&ft4->x0)[k * 4] = out.vx + sprite->pivotX;
        (&ft4->y0)[k * 4] = out.vy + sprite->pivotY;
    }
    ft4->u0 = ft4->u2 = u;
    ft4->u1 = ft4->u3 = w + u - 1;
    ft4->v0 = ft4->v1 = v;
    ft4->v2 = ft4->v3 = h + v - 1;
    ft4->tpage = tpage;
    ft4->clut = clut;
    addPrim(sprite->ot, ft4);
}

/*
 * The animated sprite's update: state 1 advances the animation, unless it
 * is paused, and draws the frame's sprite, its parts from the last to the
 * first, as sprites or, scaled and rotated about the pivot, as quads.
 * The match depends on cur holding the animation before its frames.
 */
void STGTRAIN_updateSprite(TrainSprite *sprite) {
    union {
        TrainAnim *anim;
        TrainAnimFrame *frame;
    } cur; /* the animation, then its frame */
    union {
        TrainSpriteBank *bank;
        u8 *bytes;
        s32 *count;
        TrainSpritePart *part;
    } p; /* through the bank: each sprite is a count and its parts */
    TrainSpritePart *part;
    s32 n;
    s32 count;
    s32 i;
    s32 j;
    s32 state;
    u16 info;
    u16 w;
    u16 h;
    u16 tpage;
    u16 prevTpage;
    u16 clut;
    s32 transform;
    u8 u;
    u8 v;
    s32 x;
    s32 y;
    union {
        void *ptr;
        SPRT *sprt;
        POLY_FT4 *ft4;
        DR_TPAGE *tpage;
    } prim;

    state = sprite->state;
    switch (state) {
    case 0:
    default:
        sprite->nextState(sprite);
        sprite->frameTime = GFX.funcs.getTime();
        break;
    case 1:
        if (sprite->bank == NULL) {
            break;
        }
        if (sprite->anim == NULL) {
            break;
        }
        cur.anim = sprite->anim;
        cur.frame = cur.anim->frames;
        cur.frame += sprite->frame;
        if (sprite->flags >= 0 && GFX.funcs.getTime() - sprite->frameTime > cur.frame->duration) {
            sprite->frameTime = GFX.funcs.getTime();
            if (++sprite->frame >= sprite->frameCount - 1) {
                sprite->frame = sprite->frameCount - 1;
                sprite->flags = 1;
            }
            cur.anim = sprite->anim;
            cur.frame = cur.anim->frames;
            cur.frame += sprite->frame;
        }
        p.bank = sprite->bank;
        p.bytes += sprite->bankOffset;
        count = cur.frame->sprite;
        for (i = 0; i < count; i++) {
            n = *p.count++;
            for (j = 0; j < n; j++) {
                p.part++;
            }
        }
        tpage = 0;
        prevTpage = 0;
        transform = 0;
        if (sprite->transformed) {
            if (sprite->scale.vx == 0 && sprite->scale.vy == 0) {
                break;
            }
            if (sprite->scale.vx == ONE && sprite->scale.vy == ONE) {
                sprite->transformed = 0;
            } else {
                transform = 1;
                RotMatrixYXZ_gte(&sprite->rotation, &sprite->matrix);
                ScaleMatrix(&sprite->matrix, &sprite->scale);
            }
        }
        sprite->layer = GFX.funcs.getLayer(sprite->layerId);
        sprite->ot = sprite->layer->getOtEntry(sprite->layer, sprite->depth);
        prim.ptr = GFX.funcs.getPrim();
        n = *p.count++;
        for (i = 0; i < n; i++) {
            p.part++;
        }
        part = p.part;
        for (i = 0; i < n; i++) {
            part--;
            info = part->tpage;
            clut = getClut(sprite->clutX + (info & 0x1F) * 16, sprite->clutY + ((part->clut & 0x7FC0) >> 6));
            tpage = getTPage(info >> 7, info >> 5, sprite->imageX + (info & 0x1F) * 64, sprite->imageY);
            u = part->u;
            x = part->x;
            y = part->y;
            v = part->v;
            w = part->w;
            h = part->h;
            if (!transform) {
                if (i == 0) {
                    prevTpage = tpage;
                }
                if (prevTpage != tpage) {
                    SetDrawTPage(prim.tpage, 0, 1, prevTpage);
                    addPrim(sprite->ot, prim.ptr);
                    prim.tpage++;
                    prevTpage = tpage;
                }
                STGTRAIN_addSpriteSprt(sprite, prim.sprt, part, cur.frame, x, y, u, v, w, h, clut);
                prim.sprt++;
            } else {
                STGTRAIN_addSpriteQuad(sprite, prim.ft4, part, cur.frame, x, y, u, v, w, h, tpage, clut);
                prim.ft4++;
            }
        }
        if (!transform) {
            SetDrawTPage(prim.tpage, 0, 1, tpage);
            addPrim(sprite->ot, prim.ptr);
            prim.tpage++;
        }
        GFX.funcs.setPrim(prim.ptr);
        break;
    case 2:
    case 3:
        break;
    }
}

/* Creates an animated sprite */
TrainSprite *STGTRAIN_createSprite(void) {
    TrainSprite *sprite = createTask(STGTRAIN_updateSprite, sizeof(TrainSprite), 0);
    sprite->setBank = STGTRAIN_setSpriteBank;
    sprite->setAnim = STGTRAIN_setSpriteAnim;
    sprite->setPos = STGTRAIN_setSpritePos;
    sprite->setImagePos = STGTRAIN_setSpriteImagePos;
    sprite->setLayer = STGTRAIN_setSpriteLayer;
    sprite->setClutPos = STGTRAIN_setSpriteClutPos;
    sprite->setScale = STGTRAIN_setSpriteScale;
    sprite->setPivot = STGTRAIN_setSpritePivot;
    sprite->setRotation = STGTRAIN_setSpriteRotation;
    sprite->getFlags = STGTRAIN_getSpriteFlags;
    sprite->setPaused = STGTRAIN_setSpritePaused;
    return sprite;
}
