/*
 * WSTAG490: Ether Jungle, South Sector.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

/* Creates the event object that the flags call for, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x25), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x26), 0)) {
            children[0] = FIELDSTG_startEvent(0x4F4);
        } else if (FLAGS_00.checkCondition(FLAG(0x40, 0x2F), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x30), 0)) {
            children[0] = FIELDSTG_startEvent(0x4FE);
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
 * After event 1267, with WarGrowlmon: lets event 1268 start and starts the
 * stage's event battle 0
 */
void endEvent1267(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x25), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1268, with WarGrowlmon: gives the player the Guilmon DDNA and
 * keeps it from playing again
 */
void endEvent1268(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x26), 1);
    FLAGS_00.applyAction(ITEM(0, 0x22), 1);
}

/* After event 1277, with GrapLeomon: lets event 1278 start and starts the stage's event battle 1 */
void endEvent1277(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x2F), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 1278, with GrapLeomon: gives the player the Kumamon DDNA and
 * keeps it from playing again
 */
void endEvent1278(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x30), 1);
    FLAGS_00.applyAction(ITEM(0, 0x18B), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x3EF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x3FF
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0013 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x33900, 0x3D200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x35;
    FIELDSTG_state.music = MUSIC(0x35, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1267[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x3E5, 0x187, 5),
    SCRIPT_PLACE(0x7E, 0x405, 0x177),
    SCRIPT_POSE(0x7E, 1, 1),
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
    SCRIPT_TALK(0, 2, 0x7E, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1268[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x3E5, 0x187),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x7E, 0x405, 0x177),
    SCRIPT_POSE(0x7E, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x7E, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
s16 script1277[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x138, 0x2D4, 3),
    SCRIPT_PLACE(0x81, 0x118, 0x2C4),
    SCRIPT_POSE(0x81, 1, 7),
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
    SCRIPT_TALK(0, 2, 0x81, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1278[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x138, 0x2D4),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x81, 0x118, 0x2C4),
    SCRIPT_POSE(0x81, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x81, 2),
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
AreaBattle area0Battle0 = { 152, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 152, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 152, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 152, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 145, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 145, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 58, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 58, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 265, 8, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 269, 8, MUSIC(0x22, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 329, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 152, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 59, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 20, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F6, 0x158, 0x2D8, 0x58, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1F0, 0x158, 0x2C0, 0x58, 0x170, 0x1FE },
    { 0x180, 0x100, 0x19E, 0x133, 0x178, 0x33, 0x150, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x1CC, 0x1C0, 0xCC, 0x160, 0x1FD },
    { 0x1C0, 0x100, 0x1C8, 0x15A, 0x220, 0x5A, 0x170, 0x1FD },
};
u16 actor6Talk0Conditions[] = { FLAG(0x1C, 0x13), 0, CODES_END };
u16 actor6Talk0Actions[] = { START_EVENT(0x26), 1, FLAG(0x1C, 0x13), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0x1C, 0x13), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0x1C, 0x18), 0, CODES_END };
u16 actor7Talk0Actions[] = { START_EVENT(0x2B), 1, FLAG(0x1C, 0x18), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0x1C, 0x18), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x22 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x144 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x147 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x145 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x143 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x2BF },
    { actor6Talk1Conditions, NULL, 0x2C0 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x2C9 },
    { actor7Talk1Conditions, NULL, 0x2CA },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x146 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(6, 1), 1, ITEM(0, 0x22), 0, CODES_END };
u16 actor7Conditions[] = { FLAG(6, 5), 1, ITEM(0, 0x18B), 0, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2E, 4, 561, 458, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 4, 561, 458, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 4, 561, 458, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 4, 561, 458, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 4, 561, 458, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x66, 5, 776, 996, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x7E, 6, 1029, 375, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x81, 7, 280, 708, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 8, 561, 458, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 330, 130, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 555, 650, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 1017, 793, 0, 0 },
    { 1, 0, 0x70, 2, 0xA, 0, 0, 0, 0, 0, 896, 548, 0, 0 },
    { 1, 0, 0x78, 2, 0xB, 0, 0, 0, 0, 0, 768, 265, 0, 0 },
    { 1, 0, 0x40, 6, 0x62, 2, 0, 0xD, 0xA, 0, 902, 118, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 324, 437, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 480, 1007, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 1, 0x58, 0x61, 0xA, 0, 1091, 911, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 259, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 452, 984, 0, 0 },
    { 1, 0, 0x40, 6, 0x28, 1, 0x28, 0x31, 0xA, 0, 1034, 904, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 70, 825, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 200, 1009, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 200, 1054, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 562, 1103, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 683, 1086, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 260, 993, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 155, 855, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 281, 1130, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 30, 836, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 220, 983, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 301, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 438, 1073, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 530, 1107, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 651, 1054, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 743, 1116, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 842, 1095, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 29, 789, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 148, 1013, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 162, 889, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 471, 1092, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 701, 1043, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 802, 1118, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 109, 831, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 166, 952, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 241, 980, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 336, 1112, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 377, 1083, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 502, 1067, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 588, 1055, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 599, 1017, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 659, 1036, 0, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 103, 696, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 287, 790, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 319, 634, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 331, 860, 1152, 0 },
    { 1, 0, 0x40, 4, 0x4F, 1, 0x4F, 0x57, 0xA, 0, 425, 935, 1152, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 566, 586, 673, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 455, 303, 335, 0 },
    { 1, 0, 0x50, 4, 1, 0, 0, 0, 0, 0, 1013, 384, 452, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 959, 427, 502, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 310, 860, 900, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 455, 303, 335, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 448, 513, 569, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 592, 744, 782, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 992, 151, 200, 0 },
    { 1, 0, 0x64, 4, 0x12, 0, 0, 0, 0, 0, 445, 977, 1021, 0 },
    { 1, 0, 0x64, 4, 0x13, 0, 0, 0, 0, 0, 896, 756, 810, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 80, 759, 759, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 224, 191, 191, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 256, 223, 223, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 247, 247, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 368, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 447, 647, 647, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 439, 439, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 464, 615, 615, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 496, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 512, 559, 559, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 613, 879, 879, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 624, 839, 839, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 640, 447, 447, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 487, 487, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 887, 887, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 704, 543, 543, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 752, 935, 935, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 871, 871, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 816, 823, 823, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23D, 0x372, 0x272, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x242, 0x90, 0x128, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23B, 0xA0, 0x80, 7, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 4, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 1, 2 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 4, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x48, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x20, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 4, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1267, script1267, EVENT_TEXT(0x15), NULL, endEvent1267 },
    { 1268, script1268, EVENT_TEXT(0x16), NULL, endEvent1268 },
    { 1277, script1277, EVENT_TEXT(0x17), NULL, endEvent1277 },
    { 1278, script1278, EVENT_TEXT(0x18), NULL, endEvent1278 },
    { -1, NULL, 0, NULL, NULL },
};
