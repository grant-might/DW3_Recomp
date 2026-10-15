/* FIGHTSTG's drawing helpers: a projected point and quads on a layer. */

#include "battle/battle_mode.h"
#include "engine/gte.h"

/* Projects pos to the screen: its x and y, and its depth in the layer's
   ordering table */
void FIGHTSTG_projectPoint(Layer *layer, SVECTOR *pos, ShortVec3 *out) {
    s32 shift = 16 - layer->getOtShift(layer);
    DVECTOR screen;
    MATRIX matrix;
    s32 z;

    gte_CompMatrix(&GsWSMATRIX, &IDENTITY_MATRIX, &matrix);
    gte_SetRotMatrix(&matrix);
    gte_SetTransMatrix(&matrix);
    gte_ldv0_unaligned(pos);
    gte_rtps();
    gte_stsxy(&screen);
    gte_stszotz(&z);
    out->x = screen.vx;
    out->y = screen.vy;
    out->z = z >> shift;
}

/* A Gouraud-shaded quad on a layer, blended when semi is set; the match
   depends on setSemiTrans inside the if and on poly moving on past it */
void FIGHTSTG_drawShadedQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors, s32 semi) {
    Layer *layer = GFX.funcs.getLayer(layerId);
    u_long *ot = layer->getOtEntry(layer, depth);
    POLY_G4 *poly = GFX.funcs.getPrim();
    DR_TPAGE *mode;

    /* each color is copied whole over r, g, b and the byte after them */
    *(CVECTOR *)&poly->r0 = colors[0];
    *(CVECTOR *)&poly->r1 = colors[1];
    *(CVECTOR *)&poly->r2 = colors[2];
    *(CVECTOR *)&poly->r3 = colors[3];
    setPolyG4(poly);
    if (semi) {
        setSemiTrans(poly, 1);
    }
    poly->x0 = xy[0].vx;
    poly->x1 = xy[1].vx;
    poly->x2 = xy[2].vx;
    poly->x3 = xy[3].vx;
    poly->y0 = xy[0].vy;
    poly->y1 = xy[1].vy;
    poly->y2 = xy[2].vy;
    poly->y3 = xy[3].vy;
    addPrim(ot, poly);
    poly++;
    if (semi) {
        mode = (DR_TPAGE *)poly;
        setlen(mode, 1);
        mode->code[0] = 0xE1000245;
        addPrim(ot, mode);
        poly = (POLY_G4 *)(mode + 1);
    }
    GFX.funcs.setPrim(poly);
}

/* A Gouraud-shaded quad on a layer */
void FIGHTSTG_drawQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors) {
    FIGHTSTG_drawShadedQuad(layerId, depth, xy, colors, 0);
}

/* A Gouraud-shaded quad on a layer, blended */
void FIGHTSTG_drawBlendedQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors) {
    FIGHTSTG_drawShadedQuad(layerId, depth, xy, colors, 1);
}
