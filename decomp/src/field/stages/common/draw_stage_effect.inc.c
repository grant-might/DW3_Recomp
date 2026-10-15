/*
 * Draws sprite idx of a StageEffect (the second, the one-shot one, 0x20
 * higher) on LAYER, the map's: a callback of updateStageEffect
 */
void drawStageEffect(StageEffect *task, void *arg, s32 idx) {
    SpriteDrawer drawer;
    Layer *layer = arg;
    StageSprite *sprite = &task->sprites[idx];
    s32 y = task->y;
    s32 x = task->x;
    s32 depth;

    if (idx == 1) {
        y -= 0x20;
        depth = 4;
    } else {
        depth = 6;
    }
    initSpriteDrawer(&drawer);
    drawer.setTexture(0x140, 0x100);
    drawer.setLayer(layer, depth);
    drawer.setClutRow(sprite->clutRow);
    drawer.draw(FILE_CACHE.getEntry(FIELDSTG_state.sheetEntry), sprite->frame, x, y);
}
