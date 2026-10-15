/* The parts of updateTileSix, the same in WSTAG310 and WSTAG311 */

/*
 * Finds the map objects with animations 5 to 10 and starts the
 * animations of the first four
 */
static inline void findTileSix(StageTileSix *task) {
    StageTile *object;

    for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
        if (object->anim >= 5 && object->anim <= 10) {
            switch (object->anim) {
            case 5:
                task->tiles[0].tile = object;
                task->tiles[0].anim.index = 0;
                task->tiles[0].anim.timer = updateTileSixFrames0[0].duration;
                break;
            case 6:
                task->tiles[1].tile = object;
                task->tiles[1].anim.index = 0;
                task->tiles[1].anim.timer = updateTileSixFrames1[0].duration;
                break;
            case 7:
                task->tiles[2].tile = object;
                task->tiles[2].anim.index = 0;
                task->tiles[2].anim.timer = updateTileSixFrames2[0].duration;
                break;
            case 8:
                task->tiles[3].tile = object;
                task->tiles[3].anim.index = 0;
                task->tiles[3].anim.timer = updateTileSixFrames3[0].duration;
                break;
            case 9:
                task->tiles[4].tile = object;
                task->tiles[4].anim.index = 0;
                task->tiles[4].anim.timer = 0;
                break;
            case 10:
                task->tiles[5].tile = object;
                task->tiles[5].anim.index = 0;
                task->tiles[5].anim.timer = 0;
                break;
            }
        }
    }
}

/*
 * Mode 2: hides the first two map objects and shows the other four, the
 * third and fourth with their looping palettes and the last two still
 */
static inline void showLastFourTiles(StageTileSix *task, StageTile *tile, s32 i) {
    switch (i) {
    case 0:
    case 1:
        tile->visible = 0;
        break;
    case 2:
        tile->frame = 0x28;
        tile->clutRow = stepTileAnimation(&task->tiles[i], updateTileSixFrames2, 0, 0);
        tile->visible = 1;
        break;
    case 3:
        tile->frame = 2;
        tile->clutRow = stepTileAnimation(&task->tiles[i], updateTileSixFrames3, 0, 0);
        tile->visible = 1;
        break;
    case 4:
        tile->visible = 1;
        tile->frame = 3;
        tile->clutRow = 0;
        break;
    case 5:
        tile->visible = 1;
        tile->frame = 4;
        tile->clutRow = 0;
        break;
    }
}
