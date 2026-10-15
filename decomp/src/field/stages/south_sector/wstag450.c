/*
 * WSTAG450: Bulk Bridge, South Sector.
 */

#include "common.h"
#include "field/stage.h"

/*
 * Back from the battle after event 1279, starts event 1280, with Paildramon,
 * unless it has played
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x31), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x32), 0)) {
            children[0] = FIELDSTG_startEvent(0x500);
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

/* After event 1279, with Paildramon: lets event 1280 start and starts the stage's event battle 1 */
void endEvent1279(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x31), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 1280, with Paildramon: gives the player the Veemon DDNA and
 * keeps it from playing again
 */
void endEvent1280(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x32), 1);
    FLAGS_00.applyAction(ITEM(0, 0x23), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x389
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x399
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
    FIELDSTG_state.start = (Vec2){0x1E800, 0x2FB00};
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

s16 script1279[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x14A, 0x125, 3),
    SCRIPT_PLACE(0x7F, 0x131, 0x119),
    SCRIPT_POSE(0x7F, 1, 7),
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
    SCRIPT_TALK(0, 2, 0x7F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x7F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1280[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x14A, 0x125),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x7F, 0x131, 0x119),
    SCRIPT_POSE(0x7F, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x7F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x7F, 2),
    SCRIPT_WAIT_BOX,
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
AreaBattle place0Area3Battle0 = { 212, 8, MUSIC(3, 0) };
AreaBattle place0Area3Battle1 = { 271, 8, MUSIC(0x22, 0) };
AreaBattle place0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle place0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place0Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList place0Area3Battles = {
    0,
    { &place0Area3Battle0, &place0Area3Battle1, &place0Area3Battle2, &place0Area3Battle3,
      &place0Area3Battle4, &place0Area3Battle5, &place0Area3Battle6, &place0Area3Battle7 },
};
AreaBattle place1Area0Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 147, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 60, 8, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 60, 8, MUSIC(2, 0) };
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
AreaBattle place1Area3Battle0 = { 212, 8, MUSIC(3, 0) };
AreaBattle place1Area3Battle1 = { 271, 8, MUSIC(0x22, 0) };
AreaBattle place1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 14, 0, 0, { &place0Area0Battles, &place0Area1Battles, &place0Area2Battles, &place0Area3Battles } },
    { 55, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x158, 0x197, 0x60, 0x97, 0x160, 0x1FF },
    { 0x140, 0x100, 0x176, 0x167, 0xD8, 0x67, 0x140, 0x1FE },
    { 0x140, 0x100, 0x15A, 0x100, 0x68, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x14A, 0x184, 0x28, 0x84, 0x160, 0x1FE },
};
u16 actor0Talk0Actions[] = { FLAG(2, 4), 1, ITEM(5, 0x107), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x16, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 0,
    FLAG(0, 0), 1,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xC), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor1Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(0x16, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PARTY_STAT(2), 0, FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x16, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xC), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor2Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(0x16, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x16, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 0,
    CODES_END,
};
u16 actor4Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xC), 1, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xC), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor4Talk5Conditions[] = {
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xC), 1,
    PARTY_STAT(6), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor4Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor4Talk6Actions[] = { CARD_BATTLE(0x16, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0x1C, 0x19), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0x1C, 0x19), 1, START_EVENT(0x2C), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0x1C, 0x19), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk1Conditions[] = { PARTY_STAT(2), 0, FLAG(0, 0), 1, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(0x16, 0), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xC), 0,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor7Talk3Actions[] = { FLAG(0xE, 0xC), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor7Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor7Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xC), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor7Talk6Actions[] = { CARD_BATTLE(0x16, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x24D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0xA5 },
    { actor1Talk1Conditions, NULL, 0xA9 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0xAA },
    { actor1Talk3Conditions, actor1Talk3Actions, 0xAB },
    { actor1Talk4Conditions, NULL, 0xAC },
    { actor1Talk5Conditions, NULL, 0xAD },
    { actor1Talk6Conditions, actor1Talk6Actions, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0xA6 },
    { actor2Talk1Conditions, NULL, 0xA9 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0xAA },
    { actor2Talk3Conditions, actor2Talk3Actions, 0xAB },
    { actor2Talk4Conditions, NULL, 0xAC },
    { actor2Talk5Conditions, NULL, 0xAD },
    { actor2Talk6Conditions, actor2Talk6Actions, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0xA4 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0xAF },
    { actor3Talk2Conditions, actor3Talk2Actions, 0xB0 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0xA4 },
    { actor4Talk1Conditions, NULL, 0xA9 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0xAA },
    { actor4Talk3Conditions, actor4Talk3Actions, 0xAB },
    { actor4Talk4Conditions, NULL, 0xAC },
    { actor4Talk5Conditions, NULL, 0xAD },
    { actor4Talk6Conditions, actor4Talk6Actions, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x280 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x2CB },
    { actor6Talk1Conditions, NULL, 0x2CC },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0xA7 },
    { actor7Talk1Conditions, NULL, 0xA7 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0xA7 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0xA7 },
    { actor7Talk4Conditions, NULL, 0xA7 },
    { actor7Talk5Conditions, NULL, 0xA7 },
    { actor7Talk6Conditions, actor7Talk6Actions, 0xA7 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 4), 0, CODES_END };
u16 actor1Conditions[] = { SPECIAL(4), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(9), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(3), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1A), 0, SPECIAL(9), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(6, 7), 1, ITEM(0, 0x23), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 1569, 642, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x32, 5, 1345, 225, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x32, 5, 1345, 225, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x32, 5, 1345, 225, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x32, 5, 1345, 225, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x32, 5, 1345, 225, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x7F, 6, 305, 281, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 7, 1345, 225, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 665, 621, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 1102, 881, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 1232, 197, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 1668, 433, 0, 0 },
    { 1, 0, 0x64, 2, 9, 0, 0, 0, 0, 0, 29, 246, 0, 0 },
    { 1, 0, 0x64, 2, 0xA, 0, 0, 0, 0, 0, 30, 321, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 391, 857, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 615, 820, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 632, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 664, 966, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 708, 828, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 804, 888, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1037, 765, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1113, 492, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1321, 622, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1392, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1427, 532, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1577, 549, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1693, 707, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 52, 229, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 158, 422, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 206, 840, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 264, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 408, 861, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 958, 570, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1212, 539, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1312, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1447, 533, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1467, 678, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 727, 829, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 796, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 923, 864, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1546, 718, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1592, 300, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1678, 629, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 450, 189, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 614, 453, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 739, 309, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 849, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 939, 336, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1006, 804, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1099, 261, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1106, 204, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1281, 849, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1351, 965, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1373, 906, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1515, 148, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1638, 263, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 487, 418, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 615, 738, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 650, 168, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 726, 724, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 753, 138, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 824, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 919, 759, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1024, 129, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1070, 887, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 448, 657, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 481, 570, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 496, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 499, 893, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 623, 462, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 654, 771, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 729, 295, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 789, 504, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 806, 100, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 821, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 821, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 900, 723, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 946, 346, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 995, 394, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1042, 140, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1084, 901, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1270, 536, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1295, 946, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1326, 147, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1435, 1007, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 124, 163, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 268, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 367, 150, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 381, 194, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 491, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 500, 597, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 522, 343, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 584, 290, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 644, 607, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 689, 964, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 742, 662, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 789, 927, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 814, 673, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 822, 887, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 850, 658, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 940, 627, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 942, 864, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1028, 955, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1055, 523, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1055, 764, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1059, 515, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1062, 755, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1128, 198, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1178, 288, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1184, 281, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1281, 291, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1309, 391, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1533, 140, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1568, 715, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1628, 413, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1629, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1641, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1699, 627, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 61, 371, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 76, 156, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 111, 502, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 177, 675, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 215, 912, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 227, 135, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 270, 479, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 349, 725, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 380, 125, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 472, 371, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 539, 550, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 645, 71, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 660, 485, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 697, 754, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 727, 802, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 741, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 815, 657, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 862, 133, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 870, 300, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 925, 595, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 939, 777, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 954, 913, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1040, 166, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1068, 486, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1138, 820, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1157, 911, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1164, 207, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1171, 545, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1343, 282, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1346, 642, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1381, 970, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1384, 858, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1421, 456, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1428, 127, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1446, 956, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1488, 682, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1609, 121, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1623, 498, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1637, 334, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1740, 602, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 294, 561, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 298, 104, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 305, 774, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 535, 30, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 634, 887, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 727, 249, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 773, 71, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 849, 883, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 924, 465, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 961, 383, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 977, 93, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1133, 96, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1219, 951, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1270, 589, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1312, 412, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1380, 577, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1455, 655, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1484, 908, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1513, 169, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1615, 540, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1647, 699, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1691, 390, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1737, 761, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1738, 188, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 1425, 433, 443, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 1041, 394, 430, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 918, 546, 581, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x233, 0x80, 0x220, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x237, 0x49C, 0x246, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x235, 0xF8, 0xA4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23A, 0x560, 0x440, 1, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 1, 2 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x50, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x50, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFB8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x50, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1279, script1279, EVENT_TEXT(0x19), NULL, endEvent1279 },
    { 1280, script1280, EVENT_TEXT(0x1A), NULL, endEvent1280 },
    { -1, NULL, 0, NULL, NULL },
};
