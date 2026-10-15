/*
 * WSTAG525: Suzaku UG Lake, South Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Back from the battle after event 1273, starts event 1274, with Kyukimon, unless it has played */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x2B), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x2C), 0)) {
            children[0] = FIELDSTG_startEvent(0x4FA);
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

/* After event 550, with Nick and Keith: lets event 551 of WSTAG620 start */
void endEvent550(void) {
    FLAGS_00.applyAction(FLAG(0x40, 3), 1);
}

/* After event 1273, with Kyukimon: lets event 1274 start and starts the stage's event battle 0 */
void endEvent1273(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x2B), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1274, with Kyukimon: gives the player the Kotemon DDNA and
 * keeps it from playing again
 */
void endEvent1274(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x2C), 1);
    FLAGS_00.applyAction(ITEM(0, 0x13), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x3B3
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x3C3
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0015 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x18600, 0x18600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x37;
    FIELDSTG_state.music = MUSIC(0x37, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script550[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_PLACE(0x66, 0x71, 0x2D9),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_PLACE(0x67, 0x91, 0x2C9),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x66),
    SCRIPT_TALK(0, 2, 0x67, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x66, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x67, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x66, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x67, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x60, 0x2C8, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x66),
    SCRIPT_COMMAND(0x324, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_COMMAND(0x323, 0x326, 0x66),
    SCRIPT_COMMAND(0x324, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x66),
    SCRIPT_COMMAND(0x324, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x66),
    SCRIPT_COMMAND(0x324, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x66, 0x8F, 0x2D1, 7),
    SCRIPT_WALK(0x67, 0x70, 0x2C1, 3),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WALK(0x66, 0x70, 0x2C1, 3),
    SCRIPT_WALK(0x67, 0x90, 0x2B1, 5),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_WALK(0x66, 0xA0, 0x2A7, 5),
    SCRIPT_WALK(0x67, 0xC0, 0x297, 5),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_TALK(0, 0xF, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x66, 0),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x67, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_POSE(0x67, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xEA, 0x284, 5),
    SCRIPT_WALK(0x66, 0xEA, 0x284, 5),
    SCRIPT_WALK(0x67, 0xEA, 0x284, 5),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x255, 0x268, 0x1AC, 3),
    SCRIPT_END,
};
s16 script1273[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xA0, 0xB8, 3),
    SCRIPT_PLACE(0x80, 0x80, 0xA8),
    SCRIPT_POSE(0x80, 1, 7),
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
    SCRIPT_TALK(0, 2, 0x80, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1274[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0xA0, 0xB8),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x80, 0x80, 0xA8),
    SCRIPT_POSE(0x80, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x80, 2),
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
AreaBattle area0Battle0 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 65, 27, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 65, 27, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 268, 27, MUSIC(0x22, 0) };
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
    { 57, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x19A, 0x198, 0x168, 0x98, 0x170, 0x1FE },
    { 0x140, 0x100, 0x16E, 0x1C0, 0xB8, 0xC0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x174, 0x1C0, 0xD0, 0xC0, 0x170, 0x1FC },
    { 0x140, 0x100, 0x154, 0x1C0, 0x50, 0xC0, 0x170, 0x1FB },
};
u16 actor0Talk0Actions[] = { FLAG(2, 8), 1, ITEM(4, 0xC0), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1C, 0x16), 0, CODES_END };
u16 actor3Talk0Actions[] = { START_EVENT(0x29), 1, FLAG(0x1C, 0x16), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1C, 0x16), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x251 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x2C5 },
    { actor3Talk1Conditions, NULL, 0x2C6 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 8), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x14), 1, FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x14), 1, FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(6, 4), 1, ITEM(0, 0x13), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 545, 177, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0x66, 5, 113, 729, 5 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0x67, 6, 145, 713, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x80, 7, 128, 168, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x59, 3, 0, 0xB, 0xA, 0, 196, 230, 0, 0 },
    { 1, 0, 0x40, 2, 0x58, 3, 0, 0xB, 0xA, 0, 355, 190, 0, 0 },
    { 1, 0, 0x40, 2, 0x5A, 3, 0, 0xB, 0xA, 0, 220, 263, 0, 0 },
    { 1, 0, 0xC8, 6, 0x4A, 3, 0, 0xD, 0xA, 0, 256, 20, 0, 0 },
    { 1, 0, 0xC8, 6, 0x4B, 3, 0, 0xD, 0xA, 0, 580, 86, 0, 0 },
    { 1, 0, 0xC8, 6, 0x4C, 3, 0, 0xD, 0xA, 0, 444, 40, 0, 0 },
    { 1, 0, 0xC8, 6, 0x4D, 3, 0, 0xD, 0xA, 0, 562, 430, 0, 0 },
    { 1, 0, 0x64, 6, 0x4E, 3, 0, 9, 8, 0, 24, 48, 0, 0 },
    { 1, 0, 0x80, 6, 0x4F, 3, 0, 9, 8, 0, 490, 64, 0, 0 },
    { 1, 0, 0x80, 6, 0x50, 3, 0, 9, 8, 0, 188, 497, 0, 0 },
    { 1, 0, 0x80, 6, 0x51, 3, 0, 9, 8, 0, 589, 489, 0, 0 },
    { 1, 0, 0x40, 6, 0x52, 3, 0, 9, 0xA, 0, 435, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x53, 3, 0, 9, 0xC, 0, 472, 309, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 3, 0, 9, 0xC, 0, 549, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 3, 0, 0xB, 0xA, 0, 228, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 3, 0, 0xB, 0xA, 0, 245, 184, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 3, 0, 0xB, 0xA, 0, 271, 188, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 158, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 292, 1043, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 458, 1054, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 487, 547, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 582, 1050, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 633, 966, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 725, 881, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 191, 910, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 253, 1029, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 313, 1044, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 316, 981, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 415, 457, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 417, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 584, 875, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 119, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 140, 860, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 212, 1046, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 256, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 467, 509, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 615, 1035, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 679, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 728, 917, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 139, 608, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 161, 621, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 423, 1047, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 532, 1048, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 158, 607, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 117, 863, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 150, 1030, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 228, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 235, 944, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 353, 1000, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 82, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 140, 523, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 242, 537, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 274, 1005, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 299, 1048, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 373, 514, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 460, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 493, 1039, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 525, 682, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 529, 712, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 637, 805, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 637, 892, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 675, 850, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 676, 799, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 717, 667, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 728, 657, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 732, 855, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 735, 663, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 339, 392, 409, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 7, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x22F, 0x3C8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x220, 0x372, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x251, 0x2F7, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x260, 0x2A1, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 0xA, 0x1C0, 0x181, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 0xA, 0x1D0, 0xDA, 0, 0, 0, 0 },
    { { { FLAG(0x1C, 0x1C), 1 }, { PROGRESS(0x14), 1 } }, SLOT_EVENT, 0x226, 0, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 7, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 550, script550, EVENT_TEXT(0x1C), NULL, endEvent550 },
    { 1273, script1273, EVENT_TEXT(0x12), NULL, endEvent1273 },
    { 1274, script1274, EVENT_TEXT(0x13), NULL, endEvent1274 },
    { -1, NULL, 0, NULL, NULL },
};
