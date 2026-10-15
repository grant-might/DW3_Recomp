/*
 * WSTAG949: Protocol Ruins, East Sector, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 661 starts it
 * instead of WSTAG376.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern Vec2 farSpriteSpots[];

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x919
#define STAGE_FILE_8 0x1BE
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x8EC
#define STAGE_FILE_8 0x1B0
#endif

/* Draws the 36 sprites of STAGE_FILE at their places, scrolling at 1/8 of the layer */
void updateFarSprites(StageTask *task) {
    SpriteDrawer drawer;
    s32 pos[2];
    s32 scroll[2];
    Layer *layer;
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        initSpriteDrawer(&drawer);
        drawer.setLayerId(FIELD_LAYER_MAP, 0xC);
        drawer.setTexture(0x140, 0x100);
        drawer.setAltClut(0, 0x1F0);
        layer = GFX.funcs.getLayer(FIELD_LAYER_MAP);
        layer->getScroll(layer, scroll);
        pos[0] = (scroll[0] - 0x2C0) >> 3;
        pos[1] = (scroll[1] - 0x280) >> 3;
        for (i = 0; i < 0x24; i++) {
            drawer.draw(FILE_CACHE.getEntry(STAGE_FILE << 16), 0, farSpriteSpots[i].x + pos[0], farSpriteSpots[i].y + pos[1]);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#include "common/create_far_sprites.inc.c"

/* The stage task: creates the far sprites (updateFarSprites) */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createFarSprites();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
/* Sets the stage up: its map, actors, battles and events, playing PYRA_BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x1AF00, 0x3B400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xB;
    FIELDSTG_state.music = MUSIC(0xB, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

Vec2 farSpriteSpots[] = {
    { 596, 399 }, { 809, 564 }, { 820, 832 }, { 1143, 1009 },
    { 738, 1035 }, { 416, 1055 }, { 36, 28 }, { 140, 0 },
    { 178, 159 }, { 218, 202 }, { 186, 242 }, { 299, 93 },
    { 418, 16 }, { 440, 329 }, { 476, 369 }, { 725, 82 },
    { 1137, 24 }, { 1213, 103 }, { 1281, 147 }, { 1349, 48 },
    { 669, 632 }, { 491, 796 }, { 661, 889 }, { 624, 926 },
    { 998, 634 }, { 1026, 648 }, { 890, 940 }, { 915, 959 },
    { 826, 1119 }, { 1028, 1089 }, { 1227, 1002 }, { 1306, 861 },
    { 498, 1190 }, { 146, 569 }, { 187, 590 }, { 394, 616 },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16A, 0x100, 0xA8, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x100, 0x68, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x150, 0x100, 0x40, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x162, 0x100, 0x88, 0, 0x170, 0x1FF },
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, CODES_END };
u16 actor1Talk3Actions[] = { CARD_BATTLE(0x42, 1), 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7C01, 1, CODES_END };
u16 actor3Talk0Conditions[] = { SPECIAL(0x96), 0, CODES_END };
u16 actor3Talk1Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xE), 0, CODES_END };
u16 actor3Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0x10, 0xE), 1, CODES_END };
u16 actor3Talk2Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xE), 1, CODES_END };
u16 actor4Talk0Conditions[] = { SPECIAL(0x96), 0, CODES_END };
u16 actor4Talk1Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xF), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0x10, 0xF), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor4Talk2Conditions[] = { SPECIAL(0x96), 1, FLAG(0x10, 0xF), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x3B },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x3C },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x39 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x3A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x87 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x81 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x82 },
    { actor3Talk2Conditions, NULL, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x84 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x85 },
    { actor4Talk2Conditions, NULL, 0x86 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x39, 4, 320, 344, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x39, 4, 320, 344, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x3D, 5, 97, 242, 1 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x90, 6, 1073, 145, 1 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x91, 7, 520, 220, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x294, 0x510, 0x88, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 8, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 4, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x7E, 0x1AF, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x6D, 0x117, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
AreaBattle area0Battle0 = { 46, 7, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 46, 7, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 150, 7, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 150, 7, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 97, 7, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 97, 7, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 306, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 307, 18, MUSIC(0x23, 0) };
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
    { 387, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
