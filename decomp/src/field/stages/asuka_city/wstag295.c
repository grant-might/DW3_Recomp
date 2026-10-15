/*
 * WSTAG295: Prison Tower, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of progress 0x21, which depends on flags 0x1C39, 0x4057 and 0x4058 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (GAME.progress == 0x21) {
            if (FLAGS_00.checkCondition(FLAG(0x1C, 0x39), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x57), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x58), 0)) {
                children[0] = FIELDSTG_startEvent(0x37B);
            } else if (FLAGS_00.checkCondition(FLAG(0x1C, 0x39), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x58), 1)) {
                children[0] = FIELDSTG_startEvent(0x37C);
            }
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
 * After event 580, with Teddy, Dr. Kadomatsu, DO Operator and DO Guard: sets
 * flag 0x1C1A (which decides whether actors 0, 2 and 20 appear) and keeps it
 * from playing again
 */
void endEvent580(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x1A), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0x4D), 1);
}

/* After event 891, with Lisa, Game Master and Teddy: keeps it from playing again */
void endEvent891(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x58), 1);
}

/*
 * After event 892, with Game Master, Lisa, Teddy and Nick: lets events 893
 * and 894 of WSTAG305 start
 */
void endEvent892(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x59), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x5FF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x60F
#endif
/* Sets the stage up: its map, actors and events, playing BGM_0029 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x18400, 0xE400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1F;
    FIELDSTG_state.music = MUSIC(0x1F, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script580[] = {
    SCRIPT_WALK(2, 0x100, 0xA8, 3),
    SCRIPT_PLACE(0xC, 0x7B, 0x6E),
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_PLACE(0x20, 0xB2, 0x4A),
    SCRIPT_POSE(0x20, 1, 7),
    SCRIPT_PLACE(0x25, 0xD2, 0x4A),
    SCRIPT_POSE(0x25, 1, 7),
    SCRIPT_PLACE(0x43, 0x92, 0x61),
    SCRIPT_POSE(0x43, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_COMMAND(0x323, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xC, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0xBD, 0x8F, 7),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_WALK(0xC, 0xC5, 0x8B, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_TALK(0, 3, 0xC, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_POSE(0x20, 1, 0),
    SCRIPT_POSE(0x25, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_TALK(0, 5, 0x43, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x20, 3),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_POSE(0xC, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x25, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 8, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_COMMAND(0x323, 0x325, 0xC),
    SCRIPT_COMMAND(0x324, 0x325, 0x43),
    SCRIPT_COMMAND(0x325, 0x325, 0x20),
    SCRIPT_COMMAND(0x326, 0x325, 0x25),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xC),
    SCRIPT_COMMAND(0x324, 0x326, 0x43),
    SCRIPT_COMMAND(0x325, 0x326, 0x20),
    SCRIPT_COMMAND(0x326, 0x326, 0x25),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x20, 3),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_POSE(0xC, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x25, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x43, 3),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 0xC, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_TALK(0, 0xD, 0xC, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 0xE, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script891[] = {
    SCRIPT_LOOK_AT(1, 0x100, 0xA8),
    SCRIPT_PLACE(2, 0x1CE, 0x110),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x45, 0xD2, 0x4A),
    SCRIPT_POSE(0x45, 1, 1),
    SCRIPT_PLACE(0x46, 0xB2, 0x4A),
    SCRIPT_POSE(0x46, 1, 3),
    SCRIPT_PLACE(0x65, 0x1AE, 0x100),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_PLACE(0x6C, 0x92, 0x61),
    SCRIPT_POSE(0x6C, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x120, 0xB8, 3),
    SCRIPT_WALK(0x65, 0x100, 0xA8, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x6C, 3),
    SCRIPT_POSE(0x45, 1, 7),
    SCRIPT_POSE(0x46, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0x6C),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x6C, 3),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6C, 0x80, 0x68, 1),
    SCRIPT_WAIT_WALK(0x6C),
    SCRIPT_WALK(0x6C, 0xC4, 0x8A, 7),
    SCRIPT_WAIT_WALK(0x6C),
    SCRIPT_TALK(0, 0xA, 0x6C, 2),
    SCRIPT_POSE(0x6C, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x45, 1, 4),
    SCRIPT_POSE(0x46, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(0x324, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_COMMAND(0x324, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 0x6C),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6C),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0x6C, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0xC, 0x1B0, 0x100),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0x170, 0xE0, 3),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 0xF, 0xC, 0),
    SCRIPT_POSE(0xC, 0x42, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(0x324, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_COMMAND(0x324, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WALK(0xC, 0x150, 0xD0, 3),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 0x10, 0xC, 0),
    SCRIPT_POSE(0xC, 0x34, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 2, 1),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_LEAVE(0xE04, 0x120, 0xB8, 3),
#elif VERSION_EU || VERSION_JP
    SCRIPT_LEAVE(0xE05, 0x120, 0xB8, 3),
#endif
    SCRIPT_END,
};
s16 script892[] = {
    SCRIPT_FOLLOW(1, 0x65),
    SCRIPT_PLACE(2, 0x120, 0xB8),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_PLACE(0xC, 0x150, 0xD0),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_PLACE(0x45, 0xD2, 0x4A),
    SCRIPT_POSE(0x45, 1, 1),
    SCRIPT_PLACE(0x46, 0xB2, 0x4A),
    SCRIPT_POSE(0x46, 1, 3),
    SCRIPT_PLACE(0x65, 0x100, 0xA8),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x6C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(0x324, 0x325, 0xC),
    SCRIPT_COMMAND(0x325, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_COMMAND(0x324, 0x326, 0xC),
    SCRIPT_COMMAND(0x325, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x6C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(1, 5, 2, 2),
    SCRIPT_TALK(0, 6, 0xC, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_LEAVE(0x218, 0x64, 0x64, 5),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x148, 0x100, 0x20, 0, 0x170, 0x1FE },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x170, 0x1FC },
    { 0x140, 0x100, 0x150, 0x100, 0x40, 0, 0x140, 0x1FB },
    { 0x140, 0x100, 0x158, 0x100, 0x60, 0, 0x150, 0x1FB },
    { 0x140, 0x100, 0x160, 0x100, 0x80, 0, 0x160, 0x1FB },
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x170, 0x1FB },
    { 0x140, 0x100, 0x170, 0x120, 0xC0, 0x20, 0x140, 0x1FA },
    { 0x140, 0x100, 0x148, 0x128, 0x20, 0x28, 0x150, 0x1FA },
    { 0x140, 0x100, 0x150, 0x128, 0x40, 0x28, 0x160, 0x1FA },
    { 0x140, 0x100, 0x158, 0x128, 0x60, 0x28, 0x170, 0x1FA },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x59 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x492 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x1C, 0x1A), 0, PROGRESS(0x16), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x16), 1, FLAG(0x1C, 0x1A), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x18), 1, PROGRESS(0x1A), 0, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1E), 1, PROGRESS(0x21), 0, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x21), 1, FLAG(0x40, 0x58), 0, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x21), 1, FLAG(0x40, 0x58), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x16), 1, FLAG(0x1C, 0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0xC, 4, 123, 110, 7 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0xC, 4, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xC, 4, 197, 139, 7 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0x20, 5, 178, 74, 6 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0x25, 6, 210, 74, 2 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x43, 7, 146, 97, 7 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x45, 8, 210, 74, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x45, 8, 210, 74, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x46, 9, 178, 74, 3 };
FieldActorEntry actor9 = { actor9Conditions, NULL, 0x46, 9, 178, 74, 3 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x65, 0xA, 0, 0, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x67, 0xB, 352, 192, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x67, 0xB, 0, 0, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x6C, 0xC, 146, 97, 7 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x6C, 0xC, 146, 97, 7 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x6C, 0xC, 146, 97, 7 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x6C, 0xC, 196, 138, 7 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x9D, 0xD, 146, 97, 7 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x9E, 0xE, 210, 74, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x9F, 0xF, 178, 74, 3 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x120, 0x10, 233, 156, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 320, 53, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 384, 85, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 424, 104, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 455, 145, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 480, 180, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 3, 4, 0, 528, 205, 0, 0 },
    { 1, 0, 0x40, 2, 0x35, 2, 0, 3, 4, 0, 104, 68, 0, 0 },
    { 1, 0, 0x40, 2, 0x35, 2, 0, 3, 4, 0, 140, 69, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 104, 131, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 198, 84, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 4, 0, 382, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 3, 6, 0, 403, 168, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x205, 0x70, 0xE4, 7, 0, 0, 0 },
    { { { PROGRESS(0x16), 1 }, { FLAG(0x40, 0x4D), 0 } }, SLOT_EVENT, 0x244, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 580, script580, EVENT_TEXT(0x21), NULL, endEvent580 },
    { 891, script891, EVENT_TEXT(0x24), NULL, endEvent891 },
    { 892, script892, EVENT_TEXT(0x25), NULL, endEvent892 },
    { -1, NULL, 0, NULL, NULL },
};
