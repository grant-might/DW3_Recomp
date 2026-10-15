/*
 * WSTAG205: Asuka Bridge, Central Sector.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern u8 *spriteFieldAnims[];
extern StageSpriteSpot updateSpriteFieldSpots[];

/* The sprites, which the versions number differently */
#if VERSION_US || VERSION_JP
#define SPRITES 0x18B
#elif VERSION_EU
#define SPRITES 0x199
#endif

/* Plays the nine animations and draws the sprites of the table that are on screen */
void updateSpriteField(StageSpriteField *task) {
    SpriteDrawer drawer;
    RECT rect;
    Layer *layer;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        initSpriteDrawer(&drawer);
        drawer.setLayerId(FIELD_LAYER_MAP, 0xA);
        drawer.setTexture(0x140, 0x100);
        drawer.setAltClut(0, 0x1F0);
        for (i = 0; i < 9; i++) {
            task->anims[i].timer += GFX.funcs.getFrameTime();
            for (;;) {
                if (spriteFieldAnims[i][task->anims[i].index * 2] == 0) {
                    task->anims[i].index = 0;
                }
                if (spriteFieldAnims[i][task->anims[i].index * 2 + 1] >= task->anims[i].timer) {
                    break;
                }
                task->anims[i].timer -= spriteFieldAnims[i][task->anims[i].index * 2 + 1];
                task->anims[i].index++;
            }
            task->anims[i].frame = spriteFieldAnims[i][task->anims[i].index * 2];
        }
        layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
        layer->getViewRect(layer, &rect);
        rect.w += rect.x;
        rect.h += rect.y;
        for (i = 0;; i++) {
            s32 x, y;

            if (updateSpriteFieldSpots[i].x == 0) {
                break;
            }
            x = updateSpriteFieldSpots[i].x;
            y = updateSpriteFieldSpots[i].y;
            if (x < rect.x - 0x40 || rect.w + 0x40 < x || y < rect.y - 0x40 || rect.h + 0x40 < y) {
                continue;
            }
            if (!updateSpriteFieldSpots[i].flip) {
                drawer.setScale(0x1000, 0x1000, 0x1000);
            } else {
                drawer.setPivot(x, y);
                drawer.setScale(-0x1000, 0x1000, 0x1000);
            }
            drawer.draw(FILE_CACHE.getEntry(SPRITES << 16), task->anims[updateSpriteFieldSpots[i].anim].frame, x, y);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the field of animated sprites (updateSpriteField); nothing in the game calls it */
void *createSpriteField(void) {
    return createTask(updateSpriteField, sizeof(StageSpriteField), 0);
}

/* Creates the event object of progress 6 when flag 0x4006 is set and 0x4016 is not */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 6 && FLAGS_00.checkCondition(FLAG(0x40, 6), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x16), 0)) {
            children[0] = FIELDSTG_startEvent(0x65);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/*
 * After event 100, with Mystery Player and Keith: starts the stage's event
 * battle 0 and keeps it from playing again
 */
void endEvent100(void) {
    FLAGS_00.applyAction(FLAG(0x40, 6), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 101, with Keith: keeps it from playing again */
void endEvent101(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x16), 1);
}

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x18B
#define STAGE_ARCHIVE 0x2B4
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x199
#define STAGE_ARCHIVE 0x2C3
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing ASKA_BGM;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x2DC00, 0xF200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 4;
    FIELDSTG_state.music = MUSIC(4, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x14 && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script100[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x28F, 0x11F, 5),
    SCRIPT_PLACE(0x66, 0x317, 0xDC),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x2CF, 0x101, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_COMMAND(0x323, 0x325, 0x66),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x66),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x66),
    SCRIPT_TALK(0, 1, 0x66, 2),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0x66, 0x2EF, 0xF0, 1),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script101[] = {
    SCRIPT_PLACE(2, 0x2CF, 0x101),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x66, 0x2EF, 0xF0),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x66, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x66, 0x2CF, 0xF0, 2),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WALK(0x66, 0x2BF, 0xF8, 1),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0x66, 0x22E, 0x13F, 1),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_PLACE(0x66, 0, 0),
    SCRIPT_POSE(0x66, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
u8 spriteFieldAnims_0[] = {
    0x32, 0x0A, 0x33, 0x0A, 0x34, 0x0A, 0x00, 0x00,
};
u8 spriteFieldAnims_1[] = {
    0x35, 0x0A, 0x36, 0x0A, 0x37, 0x0A, 0x00, 0x00,
};
u8 spriteFieldAnims_2[] = {
    0x38, 0x0A, 0x39, 0x0A, 0x3A, 0x0A, 0x00, 0x00,
};
u8 spriteFieldAnims_3[] = {
    72, 6, 73, 6, 74, 6, 75, 6,
    0, 0, 0, 0,
};
u8 spriteFieldAnims_4[] = {
    62, 6, 63, 6, 64, 6, 0, 0,
};
u8 spriteFieldAnims_5[] = {
    80, 4, 81, 4, 82, 4, 83, 4,
    0, 0, 0, 0,
};
u8 spriteFieldAnims_6[] = {
    76, 6, 77, 6, 78, 6, 79, 6,
    0, 0, 0, 0,
};
u8 spriteFieldAnims_7[] = {
    65, 6, 66, 6, 67, 6, 0, 0,
};
u8 spriteFieldAnims_8[] = {
    84, 4, 85, 4, 86, 4, 87, 4,
    0, 0, 0, 0,
};
u8 *spriteFieldAnims[] = {
    spriteFieldAnims_0,
    spriteFieldAnims_1,
    spriteFieldAnims_2,
    spriteFieldAnims_3,
    spriteFieldAnims_4,
    spriteFieldAnims_5,
    spriteFieldAnims_6,
    spriteFieldAnims_7,
    spriteFieldAnims_8,
};
StageSpriteSpot updateSpriteFieldSpots[] = {
    { 40, 0x1A5, 0, 0 },
    { 56, 0x173, 0, 0 },
    { 143, 0x106, 0, 0 },
    { 0x126, 0x157, 0, 0 },
    { 0x17D, 98, 0, 0 },
    { 0x27C, 0x1AA, 0, 0 },
    { 0x29F, 0x19B, 0, 0 },
    { 0x2E7, 0x18A, 0, 0 },
    { 0x36E, 0x1A8, 0, 0 },
    { 0x4C2, 0x123, 0, 0 },
    { 0x50D, 0x110, 0, 0 },
    { 0x4AA, 0x12F, 0, 0 },
    { 0x4C0, 0x166, 0, 0 },
    { 0x46C, 0x177, 0, 0 },
    { 0x4FE, 0x15E, 0, 0 },
    { 0x50C, 0x163, 0, 0 },
    { 0x4B3, 0x1E9, 0, 0 },
    { 0x455, 0x1F5, 0, 0 },
    { 0x462, 0x1CA, 0, 0 },
    { 0x3D1, 0x1C7, 0, 0 },
    { 0x347, 0x18F, 0, 0 },
    { 0x31F, 0x191, 0, 0 },
    { 0x22B, 229, 0, 0 },
    { 0x1E8, 0x105, 0, 0 },
    { 0x255, 0x1AB, 0, 0 },
    { 0x20F, 0x1CF, 0, 0 },
    { 0x1AE, 0x12A, 0, 0 },
    { 0x177, 0x145, 0, 0 },
    { 0x533, 0x18E, 0, 0 },
    { 0x4E5, 0x1A5, 0, 0 },
    { 8, 0x17D, 1, 0 },
    { 31, 0x17B, 1, 0 },
    { 64, 0x19E, 1, 0 },
    { 87, 0x19C, 1, 0 },
    { 125, 0x107, 1, 0 },
    { 0x114, 0x158, 1, 0 },
    { 0x120, 0x161, 1, 0 },
    { 0x1A9, 62, 1, 0 },
    { 0x1C9, 0x124, 1, 0 },
    { 0x269, 0x1A9, 1, 0 },
    { 0x277, 141, 1, 0 },
    { 0x2B7, 0x194, 1, 0 },
    { 0x2CE, 0x192, 1, 0 },
    { 0x35C, 0x1A9, 1, 0 },
    { 0x364, 0x1D2, 1, 0 },
    { 0x3E3, 192, 1, 0 },
    { 0x4D7, 0x11A, 1, 0 },
    { 0x48E, 0x13D, 1, 0 },
    { 0x4F0, 0x116, 1, 0 },
    { 0x475, 0x14B, 1, 0 },
    { 0x4A5, 0x171, 1, 0 },
    { 0x48E, 0x170, 1, 0 },
    { 0x52F, 0x156, 1, 0 },
    { 0x516, 0x155, 1, 0 },
    { 0x47F, 0x1E7, 1, 0 },
    { 0x470, 0x1E4, 1, 0 },
    { 0x3EB, 0x1BC, 1, 0 },
    { 0x3CC, 0x1D5, 1, 0 },
    { 0x315, 0x186, 1, 0 },
    { 0x32F, 0x192, 1, 0 },
    { 0x20A, 252, 1, 0 },
    { 0x1F9, 0x105, 1, 0 },
    { 0x222, 0x1D0, 1, 0 },
    { 0x227, 0x1C5, 1, 0 },
    { 0x196, 0x12F, 1, 0 },
    { 0x180, 0x13E, 1, 0 },
    { 0x51A, 0x19E, 1, 0 },
    { 0x503, 0x198, 1, 0 },
    { 34, 0x16F, 2, 0 },
    { 50, 0x19D, 2, 0 },
    { 90, 0x190, 2, 0 },
    { 137, 0x110, 2, 0 },
    { 0x179, 107, 2, 0 },
    { 0x19A, 59, 2, 0 },
    { 0x1BC, 0x122, 2, 0 },
    { 0x230, 0x1C4, 2, 0 },
    { 0x26A, 139, 2, 0 },
    { 0x2A9, 0x193, 2, 0 },
    { 0x2D1, 0x186, 2, 0 },
    { 0x357, 0x1D0, 2, 0 },
    { 0x368, 0x1B2, 2, 0 },
    { 0x3D6, 190, 2, 0 },
    { 0x4F7, 0x11D, 2, 0 },
    { 0x45B, 0x151, 2, 0 },
    { 0x4AF, 0x162, 2, 0 },
    { 0x478, 0x169, 2, 0 },
    { 0x528, 0x145, 2, 0 },
    { 0x521, 0x164, 2, 0 },
    { 0x494, 0x1E7, 2, 0 },
    { 0x466, 0x1F5, 2, 0 },
    { 0x3F3, 0x1C3, 2, 0 },
    { 0x3E1, 0x1D0, 2, 0 },
    { 0x330, 0x186, 2, 0 },
    { 0x304, 0x191, 2, 0 },
    { 0x21A, 252, 2, 0 },
    { 0x21F, 243, 2, 0 },
    { 0x246, 0x1B5, 2, 0 },
    { 0x240, 0x1C4, 2, 0 },
    { 0x182, 0x135, 2, 0 },
    { 0x17A, 0x14C, 2, 0 },
    { 0x517, 0x194, 2, 0 },
    { 0x4F5, 0x195, 2, 0 },
    { 37, 0x109, 3, 0 },
    { 77, 0x11D, 3, 0 },
    { 117, 0x132, 3, 0 },
    { 157, 0x146, 3, 0 },
    { 197, 0x159, 3, 0 },
    { 237, 0x16C, 3, 0 },
    { 0x115, 0x181, 3, 0 },
    { 0x1DF, 0x1E6, 3, 0 },
    { 0x206, 0x1F9, 3, 0 },
    { 36, 0x124, 4, 0 },
    { 76, 0x138, 4, 0 },
    { 116, 0x14D, 4, 0 },
    { 156, 0x161, 4, 0 },
    { 196, 0x174, 4, 0 },
    { 236, 0x187, 4, 0 },
    { 0x1DE, 0x203, 4, 0 },
    { 0x205, 0x214, 4, 0 },
    { 0x266, 92, 4, 1 },
    { 28, 0x153, 5, 0 },
    { 68, 0x167, 5, 0 },
    { 108, 0x17C, 5, 0 },
    { 148, 0x190, 5, 0 },
    { 188, 0x1A3, 5, 0 },
    { 228, 0x1B6, 5, 0 },
    { 0x1D5, 0x232, 5, 0 },
    { 0x1FD, 0x244, 5, 0 },
    { 0x26D, 124, 5, 1 },
    { 0x3AE, 206, 6, 0 },
    { 0x1D5, 110, 6, 1 },
    { 0x3AF, 255, 7, 0 },
    { 0x1D5, 137, 7, 1 },
    { 0x3EC, 164, 8, 0 },
    { 0x196, 48, 8, 1 },
    { 0x1DC, 160, 8, 1 },
    { 0, 0, 0, 0 },
};
AreaBattle area0Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area1Battles = {
    0,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area2Battles = {
    0,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 263, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 168, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x168, 0x184, 0xA0, 0x84, 0x150, 0x1FF },
    { 0x180, 0x100, 0x180, 0x144, 0x100, 0x44, 0x160, 0x1FF },
    { 0x180, 0x100, 0x188, 0x144, 0x120, 0x44, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B2, 0x1CA, 0x1C8, 0xCA, 0x140, 0x1FE },
    { 0x180, 0x100, 0x190, 0x148, 0x140, 0x48, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1EA, 0x118, 0x2A8, 0x18, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1F2, 0x118, 0x2C8, 0x18, 0x170, 0x1FE },
};
u16 actor0Talk0Actions[] = { FLAG(2, 6), 1, ITEM(2, 0x6C), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor20Talk0Actions[] = { FLAG(0x1A, 0x19), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x44C },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x2DB },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x2DC },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x2DD },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x2DE },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x2E4 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x2E0 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x2E1 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x2E2 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x1A },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x2E5 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x2E7 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x2E6 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x2E8 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x2EE },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x2EA },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x2EB },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x2EC },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, actor20Talk0Actions, 0x50 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x2E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x2ED },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 6), 0, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(6), 1, FLAG(0x40, 0x16), 0, CODES_END };
u16 actor20Conditions[] = { PROGRESS(8), 1, FLAG(0x1A, 0x18), 1, FLAG(0x1A, 0x19), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(5), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 304, 160, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x25, 5, 736, 184, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x26, 6, 848, 240, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x66, 7, 792, 221, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x72, 8, 1144, 405, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9D, 9, 736, 184, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x9E, 0xA, 848, 240, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    &actor8,
    &actor9,
    &actor10,
    &actor11,
    &actor12,
    &actor13,
    &actor14,
    &actor15,
    &actor16,
    &actor17,
    &actor18,
    &actor19,
    &actor20,
    &actor21,
    &actor22,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 0, 0, 0, 0, 0, 0, 384, 384, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 128, 475, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 240, 499, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 252, 384, 0, 0 },
    { 1, 0, 0x80, 2, 4, 0, 0, 0, 0, 0, 512, 307, 0, 0 },
    { 1, 0, 0x40, 2, 5, 0, 0, 0, 0, 0, 496, 371, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 248, 352, 0, 0 },
    { 1, 0, 0x40, 2, 0x10, 0, 0, 0, 0, 0, 104, 512, 0, 0 },
    { 1, 0, 0x80, 2, 7, 0, 0, 0, 0, 0, 896, 128, 0, 0 },
    { 1, 0, 0x80, 2, 8, 0, 0, 0, 0, 0, 256, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 8, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 11, 459, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 22, 447, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 142, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 344, 345, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 410, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 417, 596, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 507, 261, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 606, 427, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 680, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 712, 393, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 985, 188, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 37, 451, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 357, 335, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 444, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 457, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 540, 244, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 554, 453, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 782, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 817, 391, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 942, 475, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 990, 449, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1109, 334, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1122, 488, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1250, 276, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 102, 206, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 113, 248, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 416, 303, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 422, 57, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 574, 418, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 633, 141, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 654, 397, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 887, 427, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 928, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1034, 516, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1181, 488, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 168, 451, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 392, 601, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 440, 70, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 440, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 510, 455, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1085, 507, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 44, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 381, 116, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 397, 206, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 415, 603, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 479, 284, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 541, 258, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 545, 250, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 555, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 711, 403, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 781, 404, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 843, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 977, 194, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 985, 457, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1114, 365, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1144, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1218, 456, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 30, 340, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 70, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 110, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 150, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 190, 420, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 470, 560, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 1, 0x50, 0x53, 4, 0, 510, 580, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x5B, 4, 0, 585, 125, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 4, 0, 363, 49, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 4, 0, 435, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 1, 0x54, 0x57, 4, 0, 1009, 162, 0, 0 },
    { 1, 0x64, 0x80, 6, 9, 0, 0, 0, 0, 0, 758, 96, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 6, 0, 943, 296, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 6, 0, 1019, 140, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x42, 1, 0x42, 0x44, 6, 0, 582, 78, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x45, 1, 0x45, 0x47, 6, 0, 358, 18, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 37, 267, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 77, 287, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 117, 307, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 157, 327, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 196, 347, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 236, 367, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 276, 387, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 477, 487, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x48, 1, 0x48, 0x4B, 6, 0, 517, 507, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x50, 1, 0x50, 0x53, 4, 0, 230, 440, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x60, 1, 0x60, 0x63, 6, 0, 422, 110, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4C, 1, 0x4C, 0x4F, 6, 0, 942, 205, 0, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 224, 177, 200, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 600, 214, 273, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 1046, 371, 407, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 1062, 315, 337, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 902, 275, 295, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 846, 177, 226, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { SPECIAL(7), 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x60, 0x31C, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21D, 0x5E2, 0xE0, 1, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E1, 0x1D0, 0x154, 7, 0, 2, 1 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E1, 0xE0, 0x110, 7, 0, 2, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0x110, 0x100, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0x120, 0xC8, 0, 0, 0, 0 },
    { { { PROGRESS(6), 1 }, { FLAG(0x40, 6), 0 } }, SLOT_EVENT, 0x64, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 100, script100, EVENT_TEXT(0x15), NULL, endEvent100 },
    { 101, script101, EVENT_TEXT(0x16), NULL, endEvent101 },
    { -1, NULL, 0, NULL, NULL },
};
