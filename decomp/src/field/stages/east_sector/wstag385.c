/*
 * WSTAG385: Duel Island, East Sector.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };

/* Back from the battle after event 1275, starts event 1276, with Armormon, unless it has played */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x2D), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x2E), 0)) {
            children[0] = FIELDSTG_startEvent(0x4FC);
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

/* After event 1275, with Armormon: lets event 1276 start and starts the stage's event battle 0 */
void endEvent1275(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x2D), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1276, with Armormon: gives the player the Monmon DDNA and
 * keeps it from playing again
 */
void endEvent1276(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x2E), 1);
    FLAGS_00.applyAction(ITEM(0, 0x168), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x4E8
#define STAGE_FILE_8 0x503
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x4F8
#define STAGE_FILE_8 0x513
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0008 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x3C500, 0x3D200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xA;
    FIELDSTG_state.music = MUSIC(0xA, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1275[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xBB, 0x406, 1),
    SCRIPT_PLACE(0x64, 0x9B, 0x416),
    SCRIPT_POSE(0x64, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x64, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1276[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0xBB, 0x406),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x64, 0x9B, 0x416),
    SCRIPT_POSE(0x64, 1, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x64, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
s16 script1421[] = {
    SCRIPT_WALK(2, 0x1F7, 0x214, 3),
    SCRIPT_PLACE(0x142, 0x1E0, 0x209),
    SCRIPT_POSE(0x142, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x142, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x142, 0x1D5, 0x202, 3),
    SCRIPT_WAIT_WALK(0x142),
    SCRIPT_POSE(0x142, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x142, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x142, 0x1F8, 0x1F0, 5),
    SCRIPT_WAIT_WALK(0x142),
    SCRIPT_POSE(0x142, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x142, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x142, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1423[] = {
    SCRIPT_WALK(2, 0x191, 0x1E1, 1),
    SCRIPT_PLACE(0x141, 0x177, 0x1ED),
    SCRIPT_POSE(0x141, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x141, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x141, 0x165, 0x1F5, 1),
    SCRIPT_WAIT_WALK(0x141),
    SCRIPT_POSE(0x141, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x141, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x141, 0x141, 0x1E5, 3),
    SCRIPT_WAIT_WALK(0x141),
    SCRIPT_POSE(0x141, 1, 3),
    SCRIPT_POSE(0x141, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x141, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x141, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1425[] = {
    SCRIPT_WALK(2, 0x128, 0x1FC, 3),
    SCRIPT_PLACE(0x140, 0x110, 0x1F1),
    SCRIPT_POSE(0x140, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x140, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x140, 0xCC, 0x1CE, 3),
    SCRIPT_WAIT_WALK(0x140),
    SCRIPT_POSE(0x140, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x140, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x140, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1427[] = {
    SCRIPT_WALK(2, 0x140, 0x180, 3),
    SCRIPT_PLACE(0x128, 0x129, 0x175),
    SCRIPT_POSE(0x128, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x128, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x128, 0x155, 0x15D, 5),
    SCRIPT_WAIT_WALK(0x128),
    SCRIPT_POSE(0x128, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x128, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x128, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1429[] = {
    SCRIPT_WALK(2, 0xF0, 0x118, 3),
    SCRIPT_PLACE(0x127, 0xD8, 0x10D),
    SCRIPT_POSE(0x127, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x127, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x127, 0x104, 0xF7, 5),
    SCRIPT_WAIT_WALK(0x127),
    SCRIPT_POSE(0x127, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x127, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x127, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1431[] = {
    SCRIPT_WALK(2, 0xB7, 0xBD, 3),
    SCRIPT_PLACE(0x63, 0x9F, 0xB1),
    SCRIPT_POSE(0x63, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x63, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x63, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x63, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xBF, 0xC1, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LEAVE(0x228, 0x250, 0x278, 7),
    SCRIPT_END,
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
AreaBattle area3Battle0 = { 270, 19, MUSIC(0x22, 0) };
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
    { 169, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1AC, 0x100, 0x1B0, 0, 0x170, 0x1F8 },
    { 0x180, 0x100, 0x1AC, 0x130, 0x1B0, 0x30, 0x140, 0x1F7 },
    { 0x180, 0x100, 0x1AC, 0x160, 0x1B0, 0x60, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x180, 0x178, 0x100, 0x78, 0x170, 0x1F7 },
    { 0x180, 0x100, 0x18A, 0x178, 0x128, 0x78, 0x140, 0x1F6 },
    { 0x180, 0x100, 0x194, 0x178, 0x150, 0x78, 0x150, 0x1F6 },
    { 0x180, 0x100, 0x19E, 0x178, 0x178, 0x78, 0x170, 0x1F6 },
    { 0x140, 0x100, 0x16A, 0x1C5, 0xA8, 0xC5, 0x140, 0x1F5 },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x150, 0x1F5 },
    { 0x180, 0x100, 0x1B2, 0x1C0, 0x1C8, 0xC0, 0x170, 0x1F5 },
    { 0x140, 0x100, 0x172, 0x1B3, 0xC8, 0xB3, 0x140, 0x1F4 },
    { 0x180, 0x100, 0x1A8, 0x190, 0x1A0, 0x90, 0x150, 0x1F4 },
    { 0x180, 0x100, 0x1B2, 0x190, 0x1C8, 0x90, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x180, 0x1A8, 0x100, 0xA8, 0x140, 0x1F3 },
    { 0x180, 0x100, 0x18A, 0x1A8, 0x128, 0xA8, 0x150, 0x1F3 },
    { 0x180, 0x100, 0x194, 0x1A8, 0x150, 0xA8, 0x160, 0x1F3 },
    { 0x180, 0x100, 0x19E, 0x1A8, 0x178, 0xA8, 0x170, 0x1F3 },
    { 0x180, 0x100, 0x1A8, 0x1C0, 0x1A0, 0xC0, 0x140, 0x1F2 },
};
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x55), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x55), 0, CODES_END };
u16 actor9Talk1Actions[] = { CARD_BATTLE(0xA, 1), 1, CODES_END };
u16 actor9Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1C, 0x55), 0,
    CODES_END,
};
u16 actor9Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1C, 0x55), 0,
    CODES_END,
};
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0x1C, 0x55), 1, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0x1C, 0x53), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x53), 0, CODES_END };
u16 actor10Talk1Actions[] = { CARD_BATTLE(0xA, 0), 1, CODES_END };
u16 actor10Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1C, 0x53), 0,
    CODES_END,
};
u16 actor10Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1C, 0x53), 0,
    CODES_END,
};
u16 actor10Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    ITEM(0, 0x12), 1,
    FLAG(0x1C, 0x53), 1,
    FLAG(0x1A, 0x3B), 0,
    FLAG(0x1A, 0x3A), 0,
    FLAG(0x1A, 0x39), 0,
    FLAG(0x1A, 0x38), 0,
    FLAG(0x1A, 0x37), 0,
    START_EVENT(0x6B), 1,
    CODES_END,
};
u16 actor11Talk0Conditions[] = { FLAG(0x1C, 0x17), 0, CODES_END };
u16 actor11Talk0Actions[] = { START_EVENT(0x2A), 1, FLAG(0x1C, 0x17), 1, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0x1C, 0x17), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3B), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3B), 0, CODES_END };
u16 actor12Talk1Actions[] = { CARD_BATTLE(0xB, 0), 1, CODES_END };
u16 actor12Talk2Conditions[] = {
    FLAG(0x1A, 0x3B), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor12Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3B), 0,
    CODES_END,
};
u16 actor12Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x54), 1,
    FLAG(0x1A, 0x3B), 1,
    CODES_END,
};
u16 actor13Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3B), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3B), 0, CODES_END };
u16 actor13Talk1Actions[] = { CARD_BATTLE(0xB, 1), 1, CODES_END };
u16 actor13Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3B), 0,
    CODES_END,
};
u16 actor13Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3B), 0,
    CODES_END,
};
u16 actor13Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x54), 1,
    FLAG(0x1A, 0x3B), 1,
    CODES_END,
};
u16 actor14Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3A), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3A), 0, CODES_END };
u16 actor14Talk1Actions[] = { CARD_BATTLE(0xC, 0), 1, CODES_END };
u16 actor14Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3A), 0,
    CODES_END,
};
u16 actor14Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3A), 0,
    CODES_END,
};
u16 actor14Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x53), 1,
    FLAG(0x1A, 0x3A), 1,
    CODES_END,
};
u16 actor15Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3A), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3A), 0, CODES_END };
u16 actor15Talk1Actions[] = { CARD_BATTLE(0xC, 1), 1, CODES_END };
u16 actor15Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3A), 0,
    CODES_END,
};
u16 actor15Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3A), 0,
    CODES_END,
};
u16 actor15Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x53), 1,
    FLAG(0x1A, 0x3A), 1,
    CODES_END,
};
u16 actor16Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x39), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x39), 0, CODES_END };
u16 actor16Talk1Actions[] = { CARD_BATTLE(0xD, 0), 1, CODES_END };
u16 actor16Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x39), 0,
    CODES_END,
};
u16 actor16Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor16Talk3Conditions[] = {
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x39), 0,
    FLAG(0, 0x11), 1,
    CODES_END,
};
u16 actor16Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x52), 1,
    FLAG(0x1A, 0x39), 1,
    CODES_END,
};
u16 actor17Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x39), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x39), 0, CODES_END };
u16 actor17Talk1Actions[] = { CARD_BATTLE(0xD, 1), 1, CODES_END };
u16 actor17Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x39), 0,
    CODES_END,
};
u16 actor17Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor17Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x39), 0,
    CODES_END,
};
u16 actor17Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x52), 1,
    FLAG(0x1A, 0x39), 1,
    CODES_END,
};
u16 actor18Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x38), 1, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x38), 0, CODES_END };
u16 actor18Talk1Actions[] = { CARD_BATTLE(0xE, 0), 1, CODES_END };
u16 actor18Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x38), 0,
    CODES_END,
};
u16 actor18Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0x1A, 0x38), 0,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor18Talk3Actions[] = {
    FLAG(0, 0x10), 0,
    START_EVENT(0x51), 1,
    FLAG(0x1A, 0x38), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor19Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x38), 1, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x38), 0, CODES_END };
u16 actor19Talk1Actions[] = { CARD_BATTLE(0xE, 1), 1, CODES_END };
u16 actor19Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x38), 0,
    CODES_END,
};
u16 actor19Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x38), 0,
    CODES_END,
};
u16 actor19Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x51), 1,
    FLAG(0x1A, 0x38), 1,
    CODES_END,
};
u16 actor20Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x37), 1, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x37), 0, CODES_END };
u16 actor20Talk1Actions[] = { CARD_BATTLE(0xF, 0), 1, CODES_END };
u16 actor20Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x37), 0,
    CODES_END,
};
u16 actor20Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor20Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x37), 0,
    CODES_END,
};
u16 actor20Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x50), 1,
    FLAG(0x1A, 0x37), 1,
    CODES_END,
};
u16 actor21Talk0Conditions[] = { FLAG(0x1A, 0x37), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x37), 0, CODES_END };
u16 actor21Talk1Actions[] = { CARD_BATTLE(0xF, 1), 1, CODES_END };
u16 actor21Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x37), 0,
    CODES_END,
};
u16 actor21Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor21Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x37), 0,
    CODES_END,
};
u16 actor21Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x50), 1,
    FLAG(0x1A, 0x37), 1,
    CODES_END,
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x316 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x238 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x1DD },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x231 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x31A },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x31F },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x320 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x321 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0x31A },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x317 },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x318 },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x319 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, actor11Talk0Actions, 0x2C7 },
    { actor11Talk1Conditions, NULL, 0x2C8 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x316 },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x313 },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x314 },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x316 },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x313 },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x314 },
    { actor13Talk3Conditions, actor13Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x316 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x313 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0x314 },
    { actor14Talk3Conditions, actor14Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0x316 },
    { actor15Talk1Conditions, actor15Talk1Actions, 0x313 },
    { actor15Talk2Conditions, actor15Talk2Actions, 0x314 },
    { actor15Talk3Conditions, actor15Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, NULL, 0x316 },
    { actor16Talk1Conditions, actor16Talk1Actions, 0x313 },
    { actor16Talk2Conditions, actor16Talk2Actions, 0x314 },
    { actor16Talk3Conditions, actor16Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, NULL, 0x316 },
    { actor17Talk1Conditions, actor17Talk1Actions, 0x313 },
    { actor17Talk2Conditions, actor17Talk2Actions, 0x314 },
    { actor17Talk3Conditions, actor17Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0x316 },
    { actor18Talk1Conditions, actor18Talk1Actions, 0x313 },
    { actor18Talk2Conditions, actor18Talk2Actions, 0x314 },
    { actor18Talk3Conditions, actor18Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0x316 },
    { actor19Talk1Conditions, actor19Talk1Actions, 0x313 },
    { actor19Talk2Conditions, actor19Talk2Actions, 0x314 },
    { actor19Talk3Conditions, actor19Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, NULL, 0x316 },
    { actor20Talk1Conditions, actor20Talk1Actions, 0x313 },
    { actor20Talk2Conditions, actor20Talk2Actions, 0x314 },
    { actor20Talk3Conditions, actor20Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0x316 },
    { actor21Talk1Conditions, actor21Talk1Actions, 0x313 },
    { actor21Talk2Conditions, actor21Talk2Actions, 0x314 },
    { actor21Talk3Conditions, actor21Talk3Actions, 0x315 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x33C },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x33C },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x93), 1, CODES_END };
u16 actor1Conditions[] = { FLAG(0x1A, 0x3B), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1A, 0x3A), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(0x1A, 0x39), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0x1A, 0x38), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x1A, 0x37), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x93), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x93), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x93), 1, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x12), 1, CODES_END };
u16 actor10Conditions[] = { ITEM(0, 0x12), 0, CODES_END };
u16 actor11Conditions[] = { FLAG(6, 6), 1, ITEM(0, 0x168), 0, CODES_END };
u16 actor12Conditions[] = { FLAG(0x1A, 0x3B), 0, ITEM(0, 0x12), 0, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1A, 0x3B), 0, ITEM(0, 0x12), 1, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1A, 0x3A), 0, ITEM(0, 0x12), 0, CODES_END };
u16 actor15Conditions[] = { FLAG(0x1A, 0x3A), 0, ITEM(0, 0x12), 1, CODES_END };
u16 actor16Conditions[] = { FLAG(0x1A, 0x39), 0, ITEM(0, 0x12), 0, CODES_END };
u16 actor17Conditions[] = { FLAG(0x1A, 0x39), 0, ITEM(0, 0x12), 1, CODES_END };
u16 actor18Conditions[] = { FLAG(0x1A, 0x38), 0, ITEM(0, 0x12), 0, CODES_END };
u16 actor19Conditions[] = { FLAG(0x1A, 0x38), 0, ITEM(0, 0x12), 1, CODES_END };
u16 actor20Conditions[] = { FLAG(0x1A, 0x37), 0, ITEM(0, 0x12), 0, CODES_END };
u16 actor21Conditions[] = { FLAG(0x1A, 0x37), 0, ITEM(0, 0x12), 1, CODES_END };
u16 actor22Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor23Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0x4B, 4, 588, 1083, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x5B, 5, 260, 247, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x5C, 6, 341, 349, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x5D, 7, 204, 462, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x5E, 8, 321, 485, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x5F, 9, 504, 496, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x60, 0xA, 993, 913, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x61, 0xB, 769, 209, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x62, 0xC, 929, 417, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x63, 0xD, 159, 177, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x63, 0xD, 159, 177, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x64, 0xE, 155, 1046, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x127, 0xF, 216, 269, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x127, 0xF, 216, 269, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x128, 0x10, 297, 373, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x128, 0x10, 297, 373, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x140, 0x11, 272, 497, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x140, 0x11, 272, 497, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x141, 0x12, 375, 493, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x141, 0x12, 375, 493, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x142, 0x13, 480, 521, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x142, 0x13, 480, 521, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x169, 0x14, 501, 549, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x16A, 0x15, 522, 538, 7 };
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
    { 1, 0, 0x40, 2, 0x47, 2, 0, 5, 8, 0, 85, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 135, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 131, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 127, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 123, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 119, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 89, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 85, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 77, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 133, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 125, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 121, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 91, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 87, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 83, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 75, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x4A, 2, 0, 7, 4, 0, 103, 201, 0, 0 },
    { 1, 0, 0x40, 2, 0x4B, 2, 0, 7, 4, 0, 105, 205, 0, 0 },
    { 1, 0, 0x80, 2, 1, 0, 0, 0, 0, 0, 768, 128, 0, 0 },
    { 1, 0, 0x45, 2, 2, 0, 0, 0, 0, 0, 880, 156, 0, 0 },
    { 1, 0, 0x78, 2, 3, 0, 0, 0, 0, 0, 896, 256, 0, 0 },
    { 1, 0, 0x55, 2, 4, 0, 0, 0, 0, 0, 984, 299, 0, 0 },
    { 1, 0, 0x33, 2, 5, 0, 0, 0, 0, 0, 278, 813, 0, 0 },
    { 1, 0, 0x1E, 2, 6, 0, 0, 0, 0, 0, 323, 829, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 220, 180, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 232, 138, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 297, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 300, 284, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 168, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 220, 184, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 234, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 235, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 280, 241, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 300, 270, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 203, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 236, 1022, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 237, 973, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 238, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 273, 861, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 309, 635, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 320, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 396, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 575, 884, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 630, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 769, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1138, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 281, 1085, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 320, 983, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 359, 980, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 503, 793, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 657, 718, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 95, 790, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 117, 1016, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 145, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 178, 1068, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 185, 1000, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 212, 747, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 288, 998, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 302, 1076, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 391, 819, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 541, 902, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 773, 705, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 800, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 850, 469, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 967, 574, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 101, 951, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 275, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 461, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 509, 339, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 534, 794, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 600, 441, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 683, 709, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 715, 508, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 874, 539, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 917, 557, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 131, 1058, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 273, 953, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 19, 963, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 106, 1043, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 122, 952, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 249, 923, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 367, 919, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 545, 804, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 582, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 742, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 931, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1086, 533, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 56, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 117, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 158, 846, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 171, 949, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 216, 834, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 221, 1050, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 227, 1041, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 235, 750, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 280, 713, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 286, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 287, 1094, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 305, 665, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 332, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 333, 845, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 363, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 378, 309, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 380, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 390, 431, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 391, 981, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 398, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 412, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 430, 770, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 470, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 474, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 499, 344, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 515, 599, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 517, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 526, 345, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 534, 910, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 546, 377, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 591, 852, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 616, 1076, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 622, 1085, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 633, 1077, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 694, 498, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 706, 727, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 775, 559, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 785, 551, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 793, 661, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 797, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 798, 492, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 838, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 916, 780, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 922, 772, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 929, 778, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1026, 1107, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1032, 1114, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1039, 1107, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1047, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1066, 898, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1149, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1161, 892, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 477, 577, 626, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xC0, 0xC2, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xCF, 0x106, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x110, 0x12B, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x11F, 0x16E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x100, 0x192, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xF1, 0x1D6, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x330, 0xEA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x33F, 0x12E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x2DE, 0x141, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x2CE, 0x186, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 8, 1 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 8, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1275, script1275, EVENT_TEXT(0x1E), NULL, endEvent1275 },
    { 1276, script1276, EVENT_TEXT(0x1F), NULL, endEvent1276 },
    { 1421, script1421, EVENT_TEXT(0x27), NULL, NULL },
    { 1423, script1423, EVENT_TEXT(0x28), NULL, NULL },
    { 1425, script1425, EVENT_TEXT(0x29), NULL, NULL },
    { 1427, script1427, EVENT_TEXT(0x2A), NULL, NULL },
    { 1429, script1429, EVENT_TEXT(0x2B), NULL, NULL },
    { 1431, script1431, EVENT_TEXT(0x31), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
