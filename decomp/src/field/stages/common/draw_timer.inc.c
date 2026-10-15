/*
 * Draws the timer: its frame and the three digits of GAME.countdown, from
 * the sprites of the stage's TIMER_SHEET
 */
void drawTimer(StageTask *task) {
    SpriteDrawer drawer;
    Vec2 scroll;
    s32 i;
    Layer *layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);

    layer->getScroll(layer, &scroll);
    initSpriteDrawer(&drawer);
    drawer.setLayer(layer, 0);
    drawer.setTexture(0x140, 0x100);
    drawer.setAltClut(0, 0x1F0);
    drawer.draw(FILE_CACHE.getEntry(TIMER_SHEET << 16), 1, scroll.x + 0xE0, scroll.y + 0x16);
    scroll.y += 0x19;
    for (i = 0; i < 3; i++) {
        drawer.draw(FILE_CACHE.getEntry(TIMER_SHEET << 16), GAME.countdown[i] + 2, scroll.x + timerDigitX[i], scroll.y);
    }
}
