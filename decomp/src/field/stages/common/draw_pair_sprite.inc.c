/* Draws sprite idx at (x, y) */
void drawPairSprite(StageSpritePair *task, void *arg, s32 idx) {
    SpriteDrawer drawer;
    Layer *layer = arg;
    StageSprite *sprite = &task->sprites[idx];

    initSpriteDrawer(&drawer);
    drawer.setTexture(0x140, 0x100);
    drawer.setLayer(layer, 4);
    drawer.setClutRow(sprite->clutRow);
    drawer.draw(FILE_CACHE.getEntry(FIELDSTG_state.sheetEntry), sprite->frame, task->x, task->y);
}
