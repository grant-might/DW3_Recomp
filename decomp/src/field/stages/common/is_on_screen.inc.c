/* Whether the rectangle (x, y, w, h) is in the view of the map's layer */
s32 isOnScreen(s32 x, s32 y, s32 w, s32 h) {
    RECT rect;
    struct Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);

    layer->getViewRect(layer, &rect);
    if (x + w < rect.x) {
        return 0;
    }
    if (rect.x + rect.w < x) {
        return 0;
    }
    if (y + h < rect.y) {
        return 0;
    }
    return rect.y + rect.h >= y;
}
