/*
 * WSTAG400: Kicking Forest, East Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object when flag 0x1A26 is set and 0x1C4B is not */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (FLAGS_00.checkCondition(FLAG(0x1A, 0x26), 1) && FLAGS_00.checkCondition(FLAG(0x1C, 0x4B), 0)) {
            children[0] = FIELDSTG_startEvent(0x514);
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

/* After event 1300, with the player: keeps it from playing again */
void endEvent1300(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x4B), 1);
}

/* After event 1301, with Agumon: sets flag 0x1C4C (which decides whether actors 1 and 2 appear) */
void endEvent1301(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x4C), 1);
}

/*
 * After event 1302, with Agumon: sets flag 0x1C4D (which decides whether
 * actors 12 and 13 appear)
 */
void endEvent1302(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x4D), 1);
}

/*
 * After event 1303, with Agumon: sets flag 0x1C4E (which decides whether
 * actors 14 and 15 appear)
 */
void endEvent1303(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x4E), 1);
}

/*
 * After event 1304, with Veemon: lets event 1305 of WSTAG395 start and
 * clears flag 0x1A26 (which decides whether actors 1 to 3 and 12 to 15
 * appear and what actor 2 in WSTAG395 says)
 */
void endEvent1304(void) {
    FLAGS_00.applyAction(FLAG(0x1A, 0x27), 1);
    FLAGS_00.applyAction(FLAG(0x1A, 0x26), 0);
}

/*
 * After events 1507 to 1509, where the player gives up looking for Veemon
 * and leaves the Kicking Forest: clears flags 0x1A26 and 0x1C4B to 0x1C4E,
 * so the search starts over
 */
void endEvents1507To1509(void) {
    FLAGS_00.applyAction(FLAG(0x1A, 0x26), 0);
    FLAGS_00.applyAction(FLAG(0x1C, 0x4B), 0);
    FLAGS_00.applyAction(FLAG(0x1C, 0x4C), 0);
    FLAGS_00.applyAction(FLAG(0x1C, 0x4D), 0);
    FLAGS_00.applyAction(FLAG(0x1C, 0x4E), 0);
}

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x3FA
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x40A
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV05_00; battle list 0 before story step 0xE, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12C00, 0x12C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2D;
    FIELDSTG_state.music = MUSIC(0x2D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0xE) {
        FIELDSTG_state.battles = stageBattles0;
    } else {
        FIELDSTG_state.battles = stageBattles1;
    }
}

s16 script1300[] = {
    SCRIPT_PLACE(2, 0xEF, 0xE1),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x170, 0x120, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1301[] = {
    SCRIPT_WALK(2, 0x3F8, 0x105, 7),
    SCRIPT_WALK(0x22, 0x418, 0x114, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(0x22),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0x22, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x22, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1302[] = {
    SCRIPT_WALK(2, 0x464, 0x41F, 7),
    SCRIPT_WALK(0xE2, 0x484, 0x42F, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(0xE2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0xE2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xE2, 2),
    SCRIPT_TALK(0, 1, 0xE2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1303[] = {
    SCRIPT_WALK(2, 0xB8, 0x34C, 1),
    SCRIPT_WALK(0xE3, 0x99, 0x35C, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(0xE3),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_POSE(0xE3, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xE3, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1304[] = {
    SCRIPT_WALK(2, 0x148, 0x1AC, 7),
    SCRIPT_WALK(0x23, 0x168, 0x1BD, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(0x23),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0x23, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x23, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x229, 0x5CF, 0x3E8, 7),
    SCRIPT_END,
};
s16 script1507[] = {
    SCRIPT_WALK(2, 0xB0, 0xC1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x229, 0x654, 0x442, 3),
    SCRIPT_END,
};
s16 script1508[] = {
    SCRIPT_WALK(2, 0x648, 0xCD, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x22B, 0xB0, 0x4F8, 5),
    SCRIPT_END,
};
s16 script1509[] = {
    SCRIPT_WALK(2, 0xD0, 0x2FD, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x22C, 0x384, 0x160, 3),
    SCRIPT_END,
};
AreaBattle battles0Area0Battle0 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle1 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle2 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle3 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle4 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 37, 8, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle1 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle2 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle3 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle4 = { 40, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle5 = { 40, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle6 = { 40, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle7 = { 40, 13, MUSIC(2, 0) };
BattleList battles0Area1Battles = {
    3,
    { &battles0Area1Battle0, &battles0Area1Battle1, &battles0Area1Battle2, &battles0Area1Battle3,
      &battles0Area1Battle4, &battles0Area1Battle5, &battles0Area1Battle6, &battles0Area1Battle7 },
};
AreaBattle battles0Area2Battle0 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle1 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle2 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle3 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle4 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle5 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle6 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles0Area2Battle7 = { 0, 4, MUSIC(2, 0) };
BattleList battles0Area2Battles = {
    0,
    { &battles0Area2Battle0, &battles0Area2Battle1, &battles0Area2Battle2, &battles0Area2Battle3,
      &battles0Area2Battle4, &battles0Area2Battle5, &battles0Area2Battle6, &battles0Area2Battle7 },
};
AreaBattle battles0Area3Battle0 = { 207, 13, MUSIC(3, 0) };
AreaBattle battles0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles0Area3Battle5 = { 101, 4, MUSIC(2, 0) };
AreaBattle battles0Area3Battle6 = { 147, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 37, 8, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 37, 8, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle1 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle2 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle3 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle4 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle5 = { 145, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle6 = { 40, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle7 = { 40, 13, MUSIC(2, 0) };
BattleList battles1Area1Battles = {
    3,
    { &battles1Area1Battle0, &battles1Area1Battle1, &battles1Area1Battle2, &battles1Area1Battle3,
      &battles1Area1Battle4, &battles1Area1Battle5, &battles1Area1Battle6, &battles1Area1Battle7 },
};
AreaBattle battles1Area2Battle0 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle1 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle2 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle3 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle4 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle5 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle6 = { 0, 4, MUSIC(2, 0) };
AreaBattle battles1Area2Battle7 = { 0, 4, MUSIC(2, 0) };
BattleList battles1Area2Battles = {
    0,
    { &battles1Area2Battle0, &battles1Area2Battle1, &battles1Area2Battle2, &battles1Area2Battle3,
      &battles1Area2Battle4, &battles1Area2Battle5, &battles1Area2Battle6, &battles1Area2Battle7 },
};
AreaBattle battles1Area3Battle0 = { 207, 13, MUSIC(3, 0) };
AreaBattle battles1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles1Area3Battle5 = { 101, 4, MUSIC(2, 0) };
AreaBattle battles1Area3Battle6 = { 147, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 9, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 28, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B8, 0x140, 0x1E0, 0x40, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B4, 0x120, 0x1D0, 0x20, 0x140, 0x1FE },
    { 0x180, 0x100, 0x1A8, 0x125, 0x1A0, 0x25, 0x150, 0x1FE },
    { 0x180, 0x100, 0x1B0, 0x140, 0x1C0, 0x40, 0x160, 0x1FE },
    { 0x180, 0x100, 0x1B0, 0x160, 0x1C0, 0x60, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1B0, 0x180, 0x1C0, 0x80, 0x140, 0x1FD },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x28), 1, ITEM(5, 0xFE), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x31), 1, FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk0Actions[] = { START_EVENT(0x34), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x11, 0), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(0x11, 0), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor8Talk2Actions[] = { CARD_BATTLE(0x11, 0), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0x10), 1, CARD(7), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor9Talk2Actions[] = {
    FLAG(0, 0x11), 0,
    CARD(7), 1,
    SPECIAL(0x13), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor9Talk3Conditions[] = { FLAG(0, 0x10), 1, CARD(7), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 7), 0, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0xE, 7), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 7), 1, PARTY_STAT(0xD), 0, CODES_END };
u16 actor10Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 7), 1, PARTY_STAT(0xD), 1, CODES_END };
u16 actor10Talk3Actions[] = { CARD_BATTLE(0x11, 1), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor11Talk2Actions[] = { CARD_BATTLE(0x11, 0), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor12Talk0Actions[] = { START_EVENT(0x32), 1, FLAG(0, 1), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 2), 0, CODES_END };
u16 actor14Talk0Actions[] = { START_EVENT(0x33), 1, FLAG(0, 2), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 2), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x26D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x2E2 },
    { actor1Talk1Conditions, NULL, 0x32F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x32F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x2E2 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x7B },
    { actor4Talk1Conditions, NULL, 0x80 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x81 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x27D },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x7B },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x85 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x86 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x7C },
    { actor7Talk1Conditions, NULL, 0x80 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x81 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x7D },
    { actor8Talk1Conditions, NULL, 0x80 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x81 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x7B },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x87 },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x88 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x89 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x7F },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x82 },
    { actor10Talk2Conditions, NULL, 0x83 },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x84 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x7E },
    { actor11Talk1Conditions, NULL, 0x7E },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0x2E2 },
    { actor12Talk1Conditions, NULL, 0x32F },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x32F },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x2E2 },
    { actor14Talk1Conditions, NULL, 0x32F },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x32F },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x28), 0, CODES_END };
u16 actor1Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4C), 0, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4C), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(0x1A, 0x26), 1, CODES_END };
u16 actor4Conditions[] = {
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    SPECIAL(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor6Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    SPECIAL(0x22), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor7Conditions[] = {
    FLAG(0, 0x11), 0,
    SPECIAL(4), 1,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor8Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    PROGRESS(0x26), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor9Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    ITEM(0, 0x12), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor10Conditions[] = {
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4D), 0, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4D), 1, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4E), 0, CODES_END };
u16 actor15Conditions[] = { FLAG(0x1A, 0x26), 1, FLAG(0x1C, 0x4E), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 1407, 809, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x22, 5, 977, 238, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x22, 5, 1048, 276, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x23, 6, 255, 391, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x39, 7, 1655, 1299, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 8, 1655, 1299, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xE2, 9, 1064, 1024, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xE2, 9, 1156, 1071, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xE3, 0xA, 246, 813, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xE3, 0xA, 153, 860, 1 };
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
    { 1, 0, 0x78, 2, 0, 0, 0, 0, 0, 0, 432, 785, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 730, 539, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 791, 950, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 792, 87, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 303, 734, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 351, 829, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 374, 627, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 500, 726, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 571, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 631, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 639, 644, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 862, 571, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 954, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1127, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 341, 879, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 374, 929, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 485, 453, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 684, 535, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 784, 488, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 928, 521, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1013, 479, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1117, 497, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 303, 945, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 329, 932, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 390, 785, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 438, 796, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 445, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 450, 586, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 519, 438, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 620, 642, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 663, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 825, 587, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 986, 492, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1456, 664, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 513, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 687, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1189, 636, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1215, 553, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1344, 618, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 444, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 473, 730, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 544, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 669, 458, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 760, 581, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1052, 481, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1152, 493, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1272, 585, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1397, 643, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 434, 724, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 446, 530, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 588, 580, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 730, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 788, 597, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 807, 504, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1087, 490, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1204, 634, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1237, 569, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1284, 600, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 343, 853, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 354, 839, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 359, 937, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 368, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 372, 910, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 398, 765, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 415, 714, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 445, 783, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 456, 726, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 627, 650, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 652, 632, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 824, 605, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 834, 506, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 883, 535, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 974, 510, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1030, 1476, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1144, 525, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1155, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1159, 517, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1276, 774, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1287, 778, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1294, 769, 0, 0 },
    { 1, 0x64, 0x40, 6, 2, 0, 0, 0, 0, 0, 177, 719, 0, 0 },
    { 1, 0, 0xE0, 0xA, 0x4A, 1, 0x4A, 0x4C, 8, 0, 184, 992, 0, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 134, 729, 771, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 139, 776, 813, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 166, 303, 303, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 184, 248, 248, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 204, 335, 335, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 208, 279, 279, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 208, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 240, 311, 311, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 240, 503, 503, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 246, 871, 871, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 351, 351, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 447, 447, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 264, 911, 911, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 272, 391, 391, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 272, 535, 535, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 272, 791, 791, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 288, 479, 479, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 327, 327, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 423, 423, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 320, 367, 367, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 345, 212, 212, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 367, 263, 263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 399, 239, 239, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 432, 279, 279, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 432, 951, 951, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 432, 999, 999, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 458, 252, 252, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 480, 303, 303, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 480, 975, 975, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 506, 276, 276, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 999, 999, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 544, 1135, 1135, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 560, 295, 295, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 576, 975, 975, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 271, 271, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 823, 823, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 1015, 1015, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 1159, 1159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 608, 319, 319, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 633, 291, 291, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 640, 1039, 1039, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 640, 1183, 1183, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 647, 827, 827, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 672, 1071, 1071, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 681, 315, 315, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 1207, 1207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 703, 815, 815, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 720, 1047, 1047, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 729, 339, 339, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 736, 1087, 1087, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 752, 711, 711, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 752, 807, 807, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 768, 1119, 1119, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 783, 843, 843, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 743, 743, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 1159, 1159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 800, 1255, 1255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 832, 815, 815, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 832, 1135, 1135, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 848, 759, 759, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 864, 1167, 1167, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 864, 1231, 1231, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 880, 791, 791, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 911, 970, 970, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 912, 1143, 1143, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 927, 1241, 1241, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 271, 271, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 799, 799, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 1183, 1183, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 935, 858, 858, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 960, 943, 943, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 976, 295, 295, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 976, 983, 983, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 976, 1159, 1159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 989, 1034, 1034, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 991, 1262, 1262, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 997, 201, 201, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1024, 959, 959, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1040, 999, 999, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1040, 1143, 1143, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1042, 1251, 1251, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1055, 215, 215, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1064, 1083, 1083, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1087, 1222, 1222, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1088, 1119, 1119, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1104, 231, 231, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 319, 319, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 911, 911, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 1007, 1007, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1145, 1251, 1251, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1152, 255, 255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1152, 943, 943, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1168, 295, 295, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1168, 983, 983, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1184, 335, 335, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1184, 1279, 1279, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1200, 919, 919, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1200, 1239, 1239, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1216, 367, 367, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1216, 959, 959, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1223, 1188, 1188, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1232, 1303, 1303, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1248, 1263, 1263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1264, 343, 343, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1264, 935, 935, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1264, 1175, 1175, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1280, 975, 975, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1280, 1327, 1327, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1311, 1182, 1182, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1312, 327, 327, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1312, 367, 367, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1321, 1146, 1146, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1328, 951, 951, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1344, 991, 991, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1344, 1118, 1118, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1344, 1311, 1311, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1360, 343, 343, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1376, 1023, 1023, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1392, 1327, 1327, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1398, 1051, 1051, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1400, 227, 227, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1440, 207, 207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1440, 1063, 1063, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1456, 1319, 1319, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1488, 407, 407, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1503, 1295, 1295, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1504, 223, 223, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1525, 443, 443, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1542, 298, 298, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1568, 1279, 1279, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1584, 279, 279, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1601, 464, 464, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22B, 0xB0, 0x4F8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x229, 0x654, 0x442, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22C, 0x384, 0x160, 3, 0x64, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { FLAG(0x1A, 0x26), 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 1, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 2, 1 },
    { { { SPECIAL(0x94), 1 }, { FLAG(0x1A, 0x26), 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 3, 4 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 7, 0x34D, 0xFA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 7, 0x33F, 0x16C, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x59E, 0x111, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x58F, 0x163, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 7, 0x2DE, 0x4E3, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 7, 0x2D0, 0x550, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0x40, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0xFFC0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0, 0x50, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0x60, 0x10, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0x50, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0xFFC0, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { ITEM(0, 4), 1 } }, SLOT_GAUGE, 0xFFD0, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x1A, 0x26), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x5E3, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x1A, 0x26), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x5E4, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x1A, 0x26), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x5E5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1300, script1300, EVENT_TEXT(0x22), NULL, endEvent1300 },
    { 1301, script1301, EVENT_TEXT(0x23), NULL, endEvent1301 },
    { 1302, script1302, EVENT_TEXT(0x24), NULL, endEvent1302 },
    { 1303, script1303, EVENT_TEXT(0x25), NULL, endEvent1303 },
    { 1304, script1304, EVENT_TEXT(0x26), NULL, endEvent1304 },
    { 1507, script1507, EVENT_TEXT(0x2E), NULL, endEvents1507To1509 },
    { 1508, script1508, EVENT_TEXT(0x2F), NULL, endEvents1507To1509 },
    { 1509, script1509, EVENT_TEXT(0x30), NULL, endEvents1507To1509 },
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
