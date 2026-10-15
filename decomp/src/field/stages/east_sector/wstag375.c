/*
 * WSTAG375: Protocol Ruins, East Sector.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the tables that use them */
extern Vec2 farSpriteSpots[];

/* The file of the sprites, which the versions number differently */
#if VERSION_US || VERSION_JP
#define SPRITES 0x1B1
#elif VERSION_EU
#define SPRITES 0x1BF
#endif

/* Draws 36 sprites of file SPRITES at the positions of farSpriteSpots, scrolled at 1/8 */
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
            drawer.draw(FILE_CACHE.getEntry(SPRITES << 16), 0, farSpriteSpots[i].x + pos[0], farSpriteSpots[i].y + pos[1]);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#include "common/create_far_sprites.inc.c"

/* Creates the stage helper task, and the event object when flag 0x4053 is set and 0x4054 is not */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createFarSprites();
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x53), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x54), 0)) {
            children[1] = FIELDSTG_startEvent(0x4ED);
        }
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

/* After event 1260, with Pharaohmon: lets event 1261 start and starts the stage's event battle 0 */
void endEvent1260(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x53), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1261, with Pharaohmon: gives the player the Old Wand and keeps
 * it from playing again
 */
void endEvent1261(void) {
    FLAGS_00.applyAction(ITEM(3, 0x8D), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0x54), 1);
}

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x1B1
#define STAGE_ARCHIVE 0x2C2
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x1BF
#define STAGE_ARCHIVE 0x2D1
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing PYRA_BGM;
 * battle list 0 before story step 0xE, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1AF00, 0x3B400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xB;
    FIELDSTG_state.music = MUSIC(0xB, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0xE) {
        FIELDSTG_state.battles = stageBattles0;
    } else {
        FIELDSTG_state.battles = stageBattles1;
    }
}

s16 script1260[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x414, 0x9F, 5),
    SCRIPT_PLACE(0x3B, 0x431, 0x91),
    SCRIPT_POSE(0x3B, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x3B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x3B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU || VERSION_JP
    0x640,
#endif
};
s16 script1261[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x414, 0x9F),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x3B, 0x431, 0x91),
    SCRIPT_POSE(0x3B, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x3B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x3B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x3B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
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
AreaBattle battles0Area0Battle0 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle1 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle2 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle3 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle4 = { 45, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 45, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 45, 7, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 45, 7, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle1 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle2 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle3 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle4 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle5 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle6 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles0Area1Battle7 = { 0, 7, MUSIC(2, 0) };
BattleList battles0Area1Battles = {
    0,
    { &battles0Area1Battle0, &battles0Area1Battle1, &battles0Area1Battle2, &battles0Area1Battle3,
      &battles0Area1Battle4, &battles0Area1Battle5, &battles0Area1Battle6, &battles0Area1Battle7 },
};
AreaBattle battles0Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area2Battles = {
    0,
    { &battles0Area2Battle0, &battles0Area2Battle1, &battles0Area2Battle2, &battles0Area2Battle3,
      &battles0Area2Battle4, &battles0Area2Battle5, &battles0Area2Battle6, &battles0Area2Battle7 },
};
AreaBattle battles0Area3Battle0 = { 1, 19, MUSIC(0x22, 0) };
AreaBattle battles0Area3Battle1 = { 309, 19, MUSIC(0x22, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle5 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles0Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 45, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 46, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 46, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 46, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 46, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 46, 7, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 46, 7, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle1 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle2 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle3 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle4 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle5 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle6 = { 0, 7, MUSIC(2, 0) };
AreaBattle battles1Area1Battle7 = { 0, 7, MUSIC(2, 0) };
BattleList battles1Area1Battles = {
    0,
    { &battles1Area1Battle0, &battles1Area1Battle1, &battles1Area1Battle2, &battles1Area1Battle3,
      &battles1Area1Battle4, &battles1Area1Battle5, &battles1Area1Battle6, &battles1Area1Battle7 },
};
AreaBattle battles1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area2Battles = {
    0,
    { &battles1Area2Battle0, &battles1Area2Battle1, &battles1Area2Battle2, &battles1Area2Battle3,
      &battles1Area2Battle4, &battles1Area2Battle5, &battles1Area2Battle6, &battles1Area2Battle7 },
};
AreaBattle battles1Area3Battle0 = { 1, 19, MUSIC(0x22, 0) };
AreaBattle battles1Area3Battle1 = { 309, 19, MUSIC(0x22, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle5 = { 47, 7, MUSIC(2, 0) };
AreaBattle battles1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 11, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 30, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x162, 0x128, 0x88, 0x28, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16A, 0x128, 0xA8, 0x28, 0x140, 0x1FE },
    { 0x140, 0x100, 0x15A, 0x100, 0x68, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x150, 0x100, 0x40, 0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x162, 0x100, 0x88, 0, 0x170, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x100, 0xA8, 0, 0x140, 0x1FD },
    { 0x140, 0x100, 0x150, 0x130, 0x40, 0x30, 0x150, 0x1FD },
    { 0x140, 0x100, 0x172, 0x140, 0xC8, 0x40, 0x160, 0x1FD },
};
u16 actor2Talk0Conditions[] = { SPECIAL(0xD), 0, CODES_END };
u16 actor2Talk0Actions[] = { SPECIAL(0x13), 1, SPECIAL(0x32), 1, CODES_END };
u16 actor2Talk1Conditions[] = { SPECIAL(0xD), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1A, 0x29), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0x1A, 0x29), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1A, 0x29), 1, SPECIAL(0x2B), 0, CODES_END };
u16 actor3Talk2Conditions[] = {
    FLAG(0x1A, 0x29), 1,
    SPECIAL(0x2B), 1,
    SPECIAL(0x25), 0,
    CODES_END,
};
u16 actor3Talk2Actions[] = { FLAG(6, 7), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0x1A, 0x29), 1,
    SPECIAL(0x2B), 1,
    SPECIAL(0x25), 1,
    CODES_END,
};
u16 actor4Talk0Conditions[] = { SPECIAL(0xD), 0, CODES_END };
u16 actor4Talk0Actions[] = { SPECIAL(0x13), 1, SPECIAL(0x32), 1, CODES_END };
u16 actor4Talk1Conditions[] = { SPECIAL(0xD), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0x1A, 0x29), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0x1A, 0x29), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x1A, 0x29), 1, SPECIAL(0x2B), 0, CODES_END };
u16 actor5Talk2Conditions[] = {
    FLAG(0x1A, 0x29), 1,
    SPECIAL(0x2B), 1,
    SPECIAL(0x25), 0,
    CODES_END,
};
u16 actor5Talk2Actions[] = { FLAG(6, 7), 1, CODES_END };
u16 actor5Talk3Conditions[] = {
    FLAG(0x1A, 0x29), 1,
    SPECIAL(0x2B), 1,
    SPECIAL(0x25), 1,
    CODES_END,
};
u16 actor6Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor11Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor12Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor13Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0xA, 0), 1, START_EVENT(0x35), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0xA, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { START_EVENT(0x35), 1, FLAG(0xA, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0xA, 0), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0x1C, 0xF), 0, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0x1C, 0xF), 1, FLAG(0, 5), 0, CODES_END };
u16 actor17Talk1Actions[] = { FLAG(0x1C, 0x10), 1, FLAG(0, 5), 1, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0x1C, 0xF), 1, FLAG(0, 5), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0x1A, 0x17), 0, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0x1A, 0x17), 1, FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor18Talk1Actions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor18Talk2Conditions[] = { FLAG(0x1A, 0x17), 1, FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor21Talk0Conditions[] = { ITEM(0, 0xE), 0, FLAG(0x1A, 0x2B), 0, CODES_END };
u16 actor21Talk1Conditions[] = {
    ITEM(0, 0xE), 0,
    FLAG(0x1A, 0x2B), 1,
    FLAG(0x1C, 0x4F), 0,
    CODES_END,
};
u16 actor21Talk1Actions[] = { FLAG(0x1C, 0x4F), 1, CODES_END };
u16 actor21Talk2Conditions[] = {
    ITEM(1, 0x5A), 0,
    ITEM(0, 0xE), 0,
    FLAG(0x1A, 0x2B), 1,
    FLAG(0x1C, 0x4F), 1,
    CODES_END,
};
u16 actor21Talk3Conditions[] = {
    ITEM(0, 0xE), 0,
    FLAG(0x1A, 0x2B), 1,
    ITEM(1, 0x5A), 1,
    FLAG(0x1C, 0x4F), 1,
    CODES_END,
};
u16 actor21Talk3Actions[] = { SPECIAL(0x13), 1, ITEM(0, 0xE), 1, CODES_END };
u16 actor21Talk4Conditions[] = { ITEM(0, 0xE), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0xED },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xEF },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x2A5 },
    { actor2Talk1Conditions, NULL, 0x2A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x2A3 },
    { actor3Talk1Conditions, NULL, 0x2A8 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x2A4 },
    { actor3Talk3Conditions, NULL, 0x2A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x2A5 },
    { actor4Talk1Conditions, NULL, 0x2A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x2A3 },
    { actor5Talk1Conditions, NULL, 0x2A8 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x2A4 },
    { actor5Talk3Conditions, NULL, 0x2A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0xEB },
    { actor6Talk1Conditions, NULL, 0x1B8 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0xEB },
    { actor7Talk1Conditions, NULL, 0x1B0 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0xEB },
    { actor8Talk1Conditions, NULL, 0x1B1 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0xEB },
    { actor9Talk1Conditions, NULL, 0x1B2 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0xEB },
    { actor10Talk1Conditions, NULL, 0x1B3 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, actor11Talk0Actions, 0xEB },
    { actor11Talk1Conditions, NULL, 0x1B4 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0xEB },
    { actor12Talk1Conditions, NULL, 0x1B5 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0xEB },
    { actor13Talk1Conditions, NULL, 0x1B6 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0xEB },
    { actor14Talk1Conditions, NULL, 0x1B7 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0xEB },
    { actor15Talk1Conditions, NULL, 0x1AF },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xEB },
    { actor16Talk1Conditions, NULL, 0x2E8 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, NULL, 0xEC },
    { actor17Talk1Conditions, actor17Talk1Actions, 0x2DA },
    { actor17Talk2Conditions, NULL, 0x2DB },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0x2AA },
    { actor18Talk1Conditions, actor18Talk1Actions, 0x2AB },
    { actor18Talk2Conditions, NULL, 0x2AC },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x2AC },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x2AD },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0x2AE },
    { actor21Talk1Conditions, actor21Talk1Actions, 0x2AF },
    { actor21Talk2Conditions, NULL, 4 },
    { actor21Talk3Conditions, actor21Talk3Actions, 0x2B0 },
    { actor21Talk4Conditions, NULL, 0x2B1 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xEE },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x2A6 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x1C, 0x11), 0, PROGRESS(4), 1, CODES_END };
u16 actor1Conditions[] = { FLAG(0x1C, 0x11), 0, PROGRESS(4), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x3B), 1, ITEM(0, 0x23), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x23), 0, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x23), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x23), 0, SPECIAL(0x3B), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor17Conditions[] = { FLAG(0x1C, 0x11), 0, PROGRESS(4), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x3C), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor22Conditions[] = { FLAG(0x1C, 0x11), 0, PROGRESS(4), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x22, 4, 505, 213, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x23, 5, 480, 225, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2B, 6, 304, 328, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2B, 6, 304, 328, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2B, 6, 304, 328, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2B, 6, 304, 328, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x3B, 7, 1073, 145, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x3C, 8, 560, 240, 3 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x3D, 9, 97, 242, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x3D, 9, 97, 242, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x3D, 9, 97, 242, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x3D, 9, 97, 242, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x40, 0xA, 529, 201, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x9D, 0xB, 304, 328, 1 };
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
    &actor23,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x225, 0x510, 0x88, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 8, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DEPTH, 4, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x7E, 0x1AF, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x6D, 0x117, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1260, script1260, EVENT_TEXT(0x1B), NULL, endEvent1260 },
    { 1261, script1261, EVENT_TEXT(0x1C), NULL, endEvent1261 },
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
