/* The panel that fills up while a card operation runs */

#include "menus/memory_card.h"

/* Empties the panel and stops it (panel->reset) */
void STGMCARD_resetPanel(MemCardPanel *panel) {
    panel->substate = 0;
    panel->duration = 0;
    panel->rate = 0;
    panel->done = 0;
    panel->scale.vx = 0;
    panel->scale.vz = ONE;
    panel->scale.vy = ONE;
    panel->pivotX = panel->x;
    panel->pivotY = panel->y;
}

/* Starts the panel (panel->start): substate 1 opens it to 95% over `duration` frames
   while a card operation runs, 2 opens it fully and sets done */
void STGMCARD_startPanel(MemCardPanel *panel, s32 substate, s32 duration) {
    panel->substate = substate;
    panel->duration = duration;
    panel->rate = 0;
}

/* Sets the color of the panel's gradient at its top (panel->setTopColor) */
void STGMCARD_setPanelTopColor(MemCardPanel *panel, u8 r, u8 g, u8 b) {
    panel->top.r = r;
    panel->top.g = g;
    panel->top.b = b;
    panel->top.cd = 0;
}

/* Sets the color of the panel's gradient at its bottom (panel->setBottomColor) */
void STGMCARD_setPanelBottomColor(MemCardPanel *panel, u8 r, u8 g, u8 b) {
    panel->bottom.r = r;
    panel->bottom.g = g;
    panel->bottom.b = b;
    panel->bottom.cd = 0;
}

/* Moves the panel (panel->setPos) */
void STGMCARD_setPanelPos(MemCardPanel *panel, s32 x, s32 y) {
    panel->x = x;
    panel->y = y;
}

/* The panel's task: opens it by scaling it horizontally, then draws it as a
   gradient with a sprite */
void STGMCARD_updatePanel(MemCardPanel *panel) {
    SVECTOR out[4];
    SVECTOR in[4];
    SpriteDrawer sprite;
    Layer *layer;
    u_long *ot;
    POLY_G4 *poly;
    s32 i;

    switch (panel->state) {
    case TASK_INIT:
    default:
        panel->nextState(panel);
        STGMCARD_resetPanel(panel);
        break;
    case TASK_RUN:
        if (panel->duration == 0) {
            break;
        }
        switch (panel->substate) {
        case 0:
        default:
            return;
        case 1:
            if (panel->rate == 0) {
                panel->rate = ONE / panel->duration;
            }
            panel->scale.vx += panel->rate;
            if (panel->scale.vx > 0xF33) {
                panel->scale.vx = 0xF33;
            }
            break;
        case 2:
            if (panel->rate == 0) {
                panel->rate = ONE / panel->duration;
            }
            panel->scale.vx += panel->rate;
            if (panel->scale.vx > ONE) {
                panel->scale.vx = ONE;
                panel->done = 1;
            }
            break;
        }
        if (panel->scale.vx == 0) {
            break;
        }
        layer = GFX.funcs.getLayer(panel->layer);
        ot = layer->getOtEntry(layer, panel->depth);
        RotMatrixYXZ_gte(&panel->rot, &panel->matrix);
        ScaleMatrix(&panel->matrix, &panel->scale);
        poly = GFX.funcs.getPrim();
        setlen(poly, 8);
        poly->code = 0x38;
        poly->r0 = panel->top.r;
        poly->g0 = panel->top.g;
        poly->b0 = panel->top.b;
        poly->r1 = panel->bottom.r;
        poly->g1 = panel->bottom.g;
        poly->b1 = panel->bottom.b;
        poly->r2 = panel->top.r;
        poly->g2 = panel->top.g;
        poly->b2 = panel->top.b;
        poly->r3 = panel->bottom.r;
        poly->g3 = panel->bottom.g;
        poly->b3 = panel->bottom.b;
        in[0].vx = in[2].vx = panel->x - panel->pivotX;
        in[1].vx = in[3].vx = in[0].vx + panel->w;
        in[0].vy = in[1].vy = panel->y - panel->pivotY;
        in[2].vy = in[3].vy = in[0].vy + panel->h;
        in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
        for (i = 0; i < 4; i++) {
            ApplyMatrixSV(&panel->matrix, &in[i], &out[i]);
            out[i].vx += panel->pivotX;
            out[i].vy += panel->pivotY;
        }
        poly->x0 = out[0].vx;
        poly->y0 = out[0].vy;
        poly->x1 = out[1].vx;
        poly->y1 = out[1].vy;
        poly->x2 = out[2].vx;
        poly->y2 = out[2].vy;
        poly->x3 = out[3].vx;
        poly->y3 = out[2].vy; /* the same as out[3].vy unless the panel is rotated */
        addPrim(ot, poly);
        GFX.funcs.setPrim(poly + 1);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(panel->layer, panel->depth);
        sprite.setTexture(0x280, 0);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 38, 204, 192);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates a gradient panel (task) at x, y, w by h, empty until started */
MemCardPanel *STGMCARD_createPanel(s32 x, s32 y, s32 w, s32 h) {
    MemCardPanel *panel = createTask(STGMCARD_updatePanel, sizeof(MemCardPanel), 0);

    panel->reset = STGMCARD_resetPanel;
    panel->start = STGMCARD_startPanel;
    panel->setTopColor = STGMCARD_setPanelTopColor;
    panel->setBottomColor = STGMCARD_setPanelBottomColor;
    panel->setPos = STGMCARD_setPanelPos;
    panel->layer = SCREEN_LAYER;
    panel->depth = 1;
    panel->x = x;
    panel->y = y;
    panel->w = w;
    panel->h = h;
    return panel;
}
