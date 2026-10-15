#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Small variables, addressed through $gp (see the Makefile) */
static CardDrawer *CARD_DRAWER;

/* Makes `obj` the card drawer the methods work on */
void bindCardDrawer(CardDrawer *obj) {
    CARD_DRAWER = obj;
}

/*
 * Card drawer method: picks the image of card `id` (0 or less: the first image of the first file)
 */
void cardDrawerSetCard(s32 id) {
    CardImage *images;
    s32 n;
    s32 i;
    if (id > 0) {
        n = id - 1;
        i = n >> 6;
        images = FILE_CACHE.load(CARD_IMAGE_FILES[i]);
        CARD_DRAWER->card = &images[n & 0x3F];
    } else {
        CARD_DRAWER->card = FILE_CACHE.load(CARD_IMAGE_FILES[0]);
    }
}

/* Card drawer method: loads the card's image and CLUT into its cell of VRAM */
void cardDrawerLoadImage(void) {
    TimLoader obj;

    initTimLoader(&obj);
    obj.setImagePos(CARD_DRAWER->imageX + CARD_DRAWER->cellX * 16, CARD_DRAWER->imageY + (CARD_DRAWER->cellY << 5));
    obj.setClutPos(CARD_DRAWER->clutX, CARD_DRAWER->clutY + CARD_DRAWER->cellX * CARD_DRAWER->clutStride + CARD_DRAWER->cellY);
    obj.load(CARD_DRAWER->card->tim);
}

/* Card drawer method: the layer and depth to draw to */
void cardDrawerSetLayer(s32 id, s32 depth) {
    Layer *layer = GFX.funcs.getLayer(id);

    CARD_DRAWER->layer = layer;
    CARD_DRAWER->ot = layer->getOtEntry(layer, depth);
}

/* Card drawer method: the VRAM area of the image cells */
void cardDrawerSetImagePos(s32 x, s32 y) {
    CARD_DRAWER->imageX = x;
    CARD_DRAWER->imageY = y;
}

/* Card drawer method: the VRAM area of the CLUTs */
void cardDrawerSetClutPos(s32 x, s32 y) {
    CARD_DRAWER->clutX = x;
    CARD_DRAWER->clutY = y;
}

/* Card drawer method: the cell a card is loaded to and drawn from */
void cardDrawerSetCell(s32 x, s32 y) {
    CARD_DRAWER->cellX = x;
    CARD_DRAWER->cellY = y;
}

/* Card drawer method: the CLUT rows between two columns of cells */
void cardDrawerSetClutStride(s32 stride) {
    CARD_DRAWER->clutStride = stride;
}

/* Card drawer method: draws semi-transparent, or not */
void cardDrawerSetSemiTrans(s32 on) {
    CARD_DRAWER->semiTrans = on;
}

/* Card drawer method: draws the card of the current cell at (x, y), a 32x32 sprite */
void cardDrawerDraw(s32 x, s32 y) {
    SPRT *sprt = GFX.funcs.getPrim();
    SPRT *base = sprt;
    u8 *end;
    u16 tpage;
    u16 clut;

    clut = getClut(CARD_DRAWER->clutX, CARD_DRAWER->clutY + CARD_DRAWER->cellX * CARD_DRAWER->clutStride + CARD_DRAWER->cellY);
    tpage = (1 << 7) | (1 << 5) | ((CARD_DRAWER->imageY & 0x100) >> 4) | (((CARD_DRAWER->imageX + CARD_DRAWER->cellX * 16) & 0x3C0) >> 6) | ((CARD_DRAWER->imageY & 0x200) << 2);
    setSprt(sprt);
    if (CARD_DRAWER->semiTrans != 0) {
        setSemiTrans(sprt, 1);
    }
    setRGB0(sprt, 0x80, 0x80, 0x80);
    setXY0(sprt, x, y);
    setUV0(sprt, (CARD_DRAWER->cellX & 3) * 32, CARD_DRAWER->cellY * 32);
    setWH(sprt, 32, 32);
    sprt->clut = clut;
    addPrim(CARD_DRAWER->ot, sprt);
    /* the texture page goes right after the sprite */
    sprt++;
    SetDrawTPage((DR_TPAGE *)sprt, 0, 1, tpage);
    end = (u8 *)base + sizeof(SPRT) + sizeof(DR_TPAGE); /* past both primitives */
    /* addPrim, with the tag written through the start of the block */
    setaddr(base + 1, getaddr(CARD_DRAWER->ot));
    setaddr(CARD_DRAWER->ot, sprt);
    GFX.funcs.setPrim(end);
}

/* Card drawer method: the kind of the current card (CARD_KINDS) */
s32 cardDrawerGetKind(void) {
    return CARD_KINDS[CARD_DRAWER->card->kind];
}

/* Clears a card drawer, gives it its methods and binds it */
void initCardDrawer(CardDrawer *obj) {
    HEAP.zero(obj, sizeof(CardDrawer));
    obj->setCard = cardDrawerSetCard;
    obj->loadImage = cardDrawerLoadImage;
    obj->setLayer = cardDrawerSetLayer;
    obj->setImagePos = cardDrawerSetImagePos;
    obj->setClutPos = cardDrawerSetClutPos;
    obj->setCell = cardDrawerSetCell;
    obj->setClutStride = cardDrawerSetClutStride;
    obj->setSemiTrans = cardDrawerSetSemiTrans;
    obj->draw = cardDrawerDraw;
    obj->getKind = cardDrawerGetKind;
    bindCardDrawer(obj);
    obj->clutStride = 8;
}

/* The card images, 64 cards to a file (cardDrawerSetCard), numbered in the
   Japanese disc as in the USA one */
#if VERSION_US || VERSION_JP
s32 CARD_IMAGE_FILES[] = { 2023, 2024, 2025, 2026, 2027 };
#elif VERSION_EU
s32 CARD_IMAGE_FILES[] = { 2038, 2039, 2040, 2041, 2042 };
#endif

/* cardDrawerGetKind's kinds, by a card image's byte 3 */
s32 CARD_KINDS[] = { 0, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 1, 2, 0, 0 };
