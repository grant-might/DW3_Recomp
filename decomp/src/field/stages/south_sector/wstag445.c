/*
 * WSTAG445: Bulk Swamp, South Sector.
 */

#include "common.h"
#include "field/stage.h"

/*
 * Back from the battle after event 1265, starts event 1266, with
 * MetalGreymon, unless it has played
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x23), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x24), 0)) {
            children[0] = FIELDSTG_startEvent(0x4F2);
        }
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
 * After event 1265, with MetalGreymon: lets event 1266 start and starts the
 * stage's event battle 1
 */
void endEvent1265(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x23), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 1266, with MetalGreymon: gives the player the Agumon DDNA and
 * keeps it from playing again
 */
void endEvent1266(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x24), 1);
    FLAGS_00.applyAction(ITEM(0, 6), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x501
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x511
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV_0012; battle list 0 before story step 0x18, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x14C00, 0x14400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x34;
    FIELDSTG_state.music = MUSIC(0x34, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0x18) {
        FIELDSTG_state.battles = &stageBattles[0];
    } else {
        FIELDSTG_state.battles = &stageBattles[1];
    }
}

s16 script1265[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x104, 0x14A, 3),
    SCRIPT_PLACE(0x7B, 0xE8, 0x13C),
    SCRIPT_POSE(0x7B, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x7B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x8FBF,
#endif
};
s16 script1266[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x104, 0x14A),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x7B, 0xE8, 0x13C),
    SCRIPT_POSE(0x7B, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x7B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
AreaBattle place0Area0Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle1 = { 53, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle2 = { 53, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle3 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle4 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle5 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle6 = { 54, 8, MUSIC(2, 0) };
AreaBattle place0Area0Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList place0Area0Battles = {
    3,
    { &place0Area0Battle0, &place0Area0Battle1, &place0Area0Battle2, &place0Area0Battle3,
      &place0Area0Battle4, &place0Area0Battle5, &place0Area0Battle6, &place0Area0Battle7 },
};
AreaBattle place0Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place0Area1Battles = {
    0,
    { &place0Area1Battle0, &place0Area1Battle1, &place0Area1Battle2, &place0Area1Battle3,
      &place0Area1Battle4, &place0Area1Battle5, &place0Area1Battle6, &place0Area1Battle7 },
};
AreaBattle place0Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place0Area2Battles = {
    0,
    { &place0Area2Battle0, &place0Area2Battle1, &place0Area2Battle2, &place0Area2Battle3,
      &place0Area2Battle4, &place0Area2Battle5, &place0Area2Battle6, &place0Area2Battle7 },
};
AreaBattle place0Area3Battle0 = { 211, 8, MUSIC(3, 0) };
AreaBattle place0Area3Battle1 = { 264, 8, MUSIC(0x22, 0) };
AreaBattle place0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle3 = { 329, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle6 = { 147, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place0Area3Battles = {
    0,
    { &place0Area3Battle0, &place0Area3Battle1, &place0Area3Battle2, &place0Area3Battle3,
      &place0Area3Battle4, &place0Area3Battle5, &place0Area3Battle6, &place0Area3Battle7 },
};
AreaBattle place1Area0Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 60, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 60, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 60, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 60, 8, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    3,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area1Battles = {
    0,
    { &place1Area1Battle0, &place1Area1Battle1, &place1Area1Battle2, &place1Area1Battle3,
      &place1Area1Battle4, &place1Area1Battle5, &place1Area1Battle6, &place1Area1Battle7 },
};
AreaBattle place1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area2Battles = {
    0,
    { &place1Area2Battle0, &place1Area2Battle1, &place1Area2Battle2, &place1Area2Battle3,
      &place1Area2Battle4, &place1Area2Battle5, &place1Area2Battle6, &place1Area2Battle7 },
};
AreaBattle place1Area3Battle0 = { 211, 8, MUSIC(3, 0) };
AreaBattle place1Area3Battle1 = { 264, 8, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle3 = { 329, 8, MUSIC(2, 0) };
AreaBattle place1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 13, 0, 0, { &place0Area0Battles, &place0Area1Battles, &place0Area2Battles, &place0Area3Battles } },
    { 54, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x1A5, 0xD8, 0xA5, 0x160, 0x1FF },
    { 0x140, 0x100, 0x140, 0x1B5, 0, 0xB5, 0x170, 0x1FF },
    { 0x140, 0x100, 0x166, 0x100, 0x98, 0, 0x140, 0x1FE },
    { 0x140, 0x100, 0x148, 0x1B5, 0x20, 0xB5, 0x150, 0x1FE },
    { 0x140, 0x100, 0x150, 0x1B5, 0x40, 0xB5, 0x160, 0x1FE },
    { 0x140, 0x100, 0x158, 0x1BD, 0x60, 0xBD, 0x170, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor0Talk2Actions[] = { CARD_BATTLE(0x15, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xB), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor0Talk6Actions[] = { CARD_BATTLE(0x15, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x15, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xB), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor2Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(0x15, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x15, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 0,
    CODES_END,
};
u16 actor3Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xB), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor3Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(6), 0,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    FLAG(0xE, 0xB), 1,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(0x15, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0x1C, 0x12), 0, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0x1C, 0x12), 1, START_EVENT(0x25), 1, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0x1C, 0x12), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor12Talk2Actions[] = { CARD_BATTLE(0x15, 0), 1, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xB), 0,
    CODES_END,
};
u16 actor12Talk3Actions[] = { FLAG(0xE, 0xB), 1, CODES_END };
u16 actor12Talk4Conditions[] = {
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xB), 1,
    CODES_END,
};
u16 actor12Talk5Conditions[] = {
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xB), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor12Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xB), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor12Talk6Actions[] = { CARD_BATTLE(0x15, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x97 },
    { actor0Talk1Conditions, NULL, 0x9C },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x9D },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x9E },
    { actor0Talk4Conditions, NULL, 0x9F },
    { actor0Talk5Conditions, NULL, 0xA0 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x27F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x98 },
    { actor2Talk1Conditions, NULL, 0x9C },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x9D },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x9E },
    { actor2Talk4Conditions, NULL, 0x9F },
    { actor2Talk5Conditions, NULL, 0xA0 },
    { actor2Talk6Conditions, actor2Talk6Actions, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x99 },
    { actor3Talk1Conditions, NULL, 0x9C },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x9D },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x9E },
    { actor3Talk4Conditions, NULL, 0x9F },
    { actor3Talk5Conditions, NULL, 0xA0 },
    { actor3Talk6Conditions, actor3Talk6Actions, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x97 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0xA2 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0xA3 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x11C },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x11D },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x12B },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x2BD },
    { actor10Talk1Conditions, NULL, 0x2BE },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x12A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x9A },
    { actor12Talk1Conditions, NULL, 0x9A },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x9A },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x9A },
    { actor12Talk4Conditions, NULL, 0x9A },
    { actor12Talk5Conditions, NULL, 0x9A },
    { actor12Talk6Conditions, actor12Talk6Actions, 0x9A },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x331 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x331 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x331 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor2Conditions[] = { SPECIAL(4), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(6, 0), 1, ITEM(0, 6), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0xA), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x31, 4, 595, 475, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x31, 4, 595, 475, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x31, 4, 595, 475, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x31, 4, 595, 475, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x31, 4, 595, 475, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x39, 5, 611, 171, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x39, 5, 611, 171, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x39, 5, 611, 171, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x39, 5, 611, 171, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x39, 5, 207, 311, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x7B, 6, 232, 316, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 7, 611, 171, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 8, 595, 475, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xB2, 9, 288, 496, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xB2, 9, 288, 496, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xB2, 9, 288, 496, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 2, 1, 2, 7, 8, 0, 95, 22, 0, 0 },
    { 1, 0, 0x40, 2, 2, 1, 2, 7, 8, 0, 700, 48, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 235, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 273, 238, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 363, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 696, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 94, 336, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 662, 456, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 367, 482, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 476, 345, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 669, 498, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 417, 446, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 421, 278, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 596, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 166, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 229, 375, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 436, 275, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 483, 489, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 172, 351, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 283, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 451, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 550, 507, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 127, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 133, 280, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 185, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 219, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 390, 474, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 417, 508, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 669, 570, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 672, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 683, 497, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 690, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 709, 426, 0, 0 },
    { 1, 0x64, 0x40, 6, 1, 0, 0, 0, 0, 0, 490, 108, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, -4, 274, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, -2, 366, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 65, 485, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 85, 170, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 129, 227, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 159, 389, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 176, 334, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 273, 212, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 355, 284, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 358, 517, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 378, 421, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 389, 573, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 471, 540, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 498, 475, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 500, 291, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 556, 625, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 650, 485, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 687, 407, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 750, 536, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 104, 604, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 133, 404, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 136, 302, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 142, 447, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 212, 399, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 290, 266, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 300, 550, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 341, 568, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 377, 471, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 420, 254, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 480, 327, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 576, 564, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 623, 521, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0xA, 0, 750, 426, 0, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 464, 77, 161, 0 },
    { 1, 0, 0x80, 4, 0xD, 0, 0, 0, 0, 0, 358, 119, 155, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 223, 167, 167, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 248, 140, 140, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 295, 115, 115, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 151, 151, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 599, 123, 123, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 656, 313, 313, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 690, 375, 375, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x234, 0x668, 0xB4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x232, 0x190, 0x218, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1265, script1265, EVENT_TEXT(0x13), NULL, endEvent1265 },
    { 1266, script1266, EVENT_TEXT(0x14), NULL, endEvent1266 },
    { -1, NULL, 0, NULL, NULL },
};
