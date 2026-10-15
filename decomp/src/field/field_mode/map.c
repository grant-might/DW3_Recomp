/* FieldMap's methods: the cells of the map's levels, the free tiles and the
   steps of a walk or a flight */

#include "field/field_mode.h"

/* Part i of a map file, at the offset from its start that its word i holds */
#define MAP_PART(entry, i) ((void *)((entry)[i] + (s32)(entry)))

/*
 * Points the map at the file of map index: the grid, whose first two bytes
 * are its width and height, the levels of cells and the pixels, at the
 * offsets the file's entry starts with. Returns 0 when no file is set. The
 * match depends on the file's check being an early exit, a do-while (0)
 * that breaks once the file is set, with file declared in it: the block's
 * note stops stmt.c from rolling the exit test to the loop's end, and the
 * loop notes keep the prologue's stores of ra and s0 ahead of the load in
 * the second scheduler.
 */
s32 FIELDSTG_selectMap(s32 index) {
    s32 *entry;
    u8 *grid;

    do {
        s32 file = FIELDSTG_map.files[index];

        if (file != 0) {
            break;
        }
        return 0;
    } while (0);
    entry = FILE_CACHE.getEntry(FIELDSTG_map.files[index]);
    FIELDSTG_map.grid = MAP_PART(entry, 0);
    FIELDSTG_map.cells64 = MAP_PART(entry, 1);
    FIELDSTG_map.cells32 = MAP_PART(entry, 2);
    FIELDSTG_map.cells16 = MAP_PART(entry, 3);
    FIELDSTG_map.cells8 = MAP_PART(entry, 4);
    FIELDSTG_map.pixels = MAP_PART(entry, 5);
    grid = FIELDSTG_map.grid;
    FIELDSTG_map.width = grid[0];
    FIELDSTG_map.height = grid[1];
    FIELDSTG_map.grid = grid + 2;
    return 1;
}

/* Sets the file of map index (FieldMap.setFile) */
void FIELDSTG_setMapFile(s32 index, s32 value) {
    FIELDSTG_map.files[index] = value;
}

/* Puts the player on map index when the mode is new (FieldMap.setFirstMap) */
void FIELDSTG_setFirstMap(s32 index) {
    if (GAME.clearTempFlags != 0) {
        GAME.mapIndex = index;
    }
}

/* Puts the player on map index (FieldMap.setMap) */
void FIELDSTG_setMap(s32 index) {
    GAME.mapIndex = index;
}

/* The cell of map index at pos, down the tree from the grid to the pixel
   (FieldMap.getCell); 1 without a file */
s32 FIELDSTG_getMapCell(s32 index, Vec2 *pos) {
    s32 x;
    s32 y;
    s32 i;

    if (FIELDSTG_selectMap(index) == 0) {
        return 1;
    }
    y = pos->y;
    x = pos->x;
    /* the match depends on the * 4 being a statement of its own */
    i = FIELDSTG_map.grid[y / 128 * FIELDSTG_map.width + x / 128];
    i *= 4;
    if (y & 0x40) {
        i += 2;
    }
    i = FIELDSTG_map.cells64[(x & 0x40) ? i + 1 : i];
    i *= 4;
    if (y & 0x20) {
        i += 2;
    }
    i = FIELDSTG_map.cells32[(x & 0x20) ? i + 1 : i];
    i *= 4;
    if (y & 0x10) {
        i += 2;
    }
    i = FIELDSTG_map.cells16[(x & 0x10) ? i + 1 : i];
    i *= 4;
    if (y & 8) {
        i += 2;
    }
    i = FIELDSTG_map.cells8[(x & 8) ? i + 1 : i];
    return FIELDSTG_map.pixels[i * 64 + (y & 7) * 8 + (x & 7)];
}

/* Whether nothing stands at pos: no character's box (gathered once a frame)
   and no object. The match depends on the boxes' variables being local to
   their blocks. */
s32 FIELDSTG_isTileFree(Vec2 *pos) {
    s32 frame = GFX.funcs.getFrameCount();
    Actor *actor;
    s32 i;

    if (frame != FIELDSTG_boxFrame) {
        actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 1);
        for (i = 0; actor != NULL; i++) {
            s32 width = actor->halfWidth;

            FIELDSTG_boxes[i].left = actor->tile.x - width;
            FIELDSTG_boxes[i].right = actor->tile.x + width;
            width /= 2;
            FIELDSTG_boxes[i].top = actor->tile.y - width;
            FIELDSTG_boxes[i].bottom = actor->tile.y + width;
            actor = TASK_REGISTRY.funcs.findNext();
        }
        FIELDSTG_boxCount = i;
        FIELDSTG_boxFrame = frame;
    }
    for (i = 0; i < FIELDSTG_boxCount; i++) {
        if (pos->x >= FIELDSTG_boxes[i].left && FIELDSTG_boxes[i].right >= pos->x && pos->y >= FIELDSTG_boxes[i].top
            && FIELDSTG_boxes[i].bottom >= pos->y) {
            s32 dx = pos->x - FIELDSTG_boxes[i].left;
            s32 dy = pos->y - FIELDSTG_boxes[i].top;
            s32 width = FIELDSTG_boxes[i].right - FIELDSTG_boxes[i].left;
            s32 height = FIELDSTG_boxes[i].bottom - FIELDSTG_boxes[i].top;
            s32 halfWidth = width / 2;
            s32 halfHeight = height / 2;
            s32 ratio = width / height;

            if (halfWidth < dx) {
                dx = halfWidth - (dx - halfWidth);
            }
            if (halfHeight < dy) {
                dy = halfHeight - (dy - halfHeight);
            }
            if (dx >= halfWidth - dy * ratio) {
                return 0;
            }
        }
    }
    return FIELDSTG_findHiddenSpot(pos, 0) == NULL;
}

/* A step of scale in a direction on the map the player is on, along the
   slope of its cell (FieldMap.getWalkStep) */
void FIELDSTG_getWalkStep(Vec2 *pos, s32 scale, s32 dir, Vec2 *out) {
    s32 cell = (u8)FIELDSTG_getMapCell(GAME.mapIndex, pos);
    s32 row = cell & 0xF;
    s32 slopeDir;
    s32 sign;

    row -= row != 0;
    slopeDir = (cell & 0x10) ? FIELDSTG_mirrorDirs[dir] : dir;
    sign = (cell & 0x10) ? -1 : 1;
    out->x = FIELDSTG_dirVectors[row][slopeDir].x * scale * sign / 4096;
    out->y = FIELDSTG_dirVectors[row][slopeDir].y * scale / 4096;
}

/* A step of scale in a direction, on the flat (FieldMap.getFlyStep) */
void FIELDSTG_getFlyStep(Vec2 *pos, s32 scale, s32 dir, Vec2 *out) {
    out->x = FIELDSTG_dirVectors[0][dir].x * scale / 4096;
    out->y = FIELDSTG_dirVectors[0][dir].y * scale / 4096;
}
