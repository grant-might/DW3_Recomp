/*
 * WSTAG395: Wind Prairie, East Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of progress 5 while flag 0x4011 is clear, or else the one of flags 0x1A27 and 0x1A28 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        do {
            if (GAME.progress == 5 && FLAGS_00.checkCondition(FLAG(0x40, 0x11), 0)) {
                children[0] = FIELDSTG_startEvent(0x50);
                break;
            }
            if (FLAGS_00.checkCondition(FLAG(0x1A, 0x27), 1) && FLAGS_00.checkCondition(FLAG(0x1A, 0x28), 0)) {
                children[0] = FIELDSTG_startEvent(0x519);
                break;
            }
        } while (0);
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

/* After event 80, with Teddy: keeps it from playing again */
void endEvent80(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x11), 1);
}

/*
 * After event 1305, with Veemon: gives the player the Tree Boots, clears
 * flag 0x1C08 (which decides whether actors 11 to 22 in WSTAG255 appear) and
 * keeps it from playing again
 */
void endEvent1305(void) {
    FLAGS_00.applyAction(ITEM(0, 4), 1);
    FLAGS_00.applyAction(FLAG(0x1A, 0x28), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 8), 0);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x20C
#define STAGE_ARCHIVE 0x310
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x21B
#define STAGE_ARCHIVE 0x31F
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0006 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x30200, 0x23500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2E;
    FIELDSTG_state.music = MUSIC(0x2E, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script80[] = {
    SCRIPT_LOOK_AT(1, 0x439, 0x103),
    SCRIPT_PLACE(1, 0x45E, 0xE9),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_PLACE(2, 0x458, 0xF5),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x430, 0x101, 1),
    SCRIPT_WALK(2, 0x439, 0x103, 1),
    SCRIPT_PLACE(0xC, 0x393, 0x1A9),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0xC, 0x3D4, 0x152, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xC),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(0xC, 0x3F7, 0x124, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_WALK(0xC, 0x420, 0x10F, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 2),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 2),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(0x324, 0x325, 0xC),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_INFO_SIG, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_COMMAND(0x324, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0x3F7, 0x124, 1),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 0xA, 2, 2),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0x394, 0x1A9, 1),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 0xB, 2, 2),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0xC, 0, 0),
    SCRIPT_POSE(0xC, 1, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
s16 script1299[] = {
    SCRIPT_WALK(2, 0x5D0, 0x3E8, 5),
    SCRIPT_POSE(0x23, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x23),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x23),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x23, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x23, 0x610, 0x3E9, 7),
    SCRIPT_WAIT_WALK(0x23),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WALK(0x23, 0x5EF, 0x3F8, 0),
    SCRIPT_WAIT_WALK(0x23),
    SCRIPT_WALK(0x23, 0x670, 0x438, 7),
    SCRIPT_WAIT_WALK(0x23),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x23, 0, 0),
    SCRIPT_POSE(0x23, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 2),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 2),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x670, 0x438, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x22A, 0xEF, 0xE1, 7),
    SCRIPT_END,
};
s16 script1305[] = {
    SCRIPT_PLACE(2, 0x5CF, 0x3E8),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_PLACE(0x23, 0x5EF, 0x3F8),
    SCRIPT_POSE(0x23, 1, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x23, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x23, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x23, 0x670, 0x438, 7),
    SCRIPT_WAIT_WALK(0x23),
    SCRIPT_PLACE(0x23, 0, 0),
    SCRIPT_POSE(0x23, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 42, 1, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 42, 1, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 42, 1, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 42, 1, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 43, 1, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 43, 1, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 43, 1, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 43, 1, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 42, 1, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 42, 1, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 42, 1, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 42, 1, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 43, 1, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 43, 1, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 43, 1, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 43, 1, MUSIC(2, 0) };
BattleList area1Battles = {
    1,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle1 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle2 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle3 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle4 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle5 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle6 = { 153, 4, MUSIC(2, 0) };
AreaBattle area2Battle7 = { 153, 4, MUSIC(2, 0) };
BattleList area2Battles = {
    3,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 206, 1, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 327, 1, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 49, 1, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 8, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x18A, 0x15A, 0x128, 0x5A, 0x140, 0x1FF },
    { 0x180, 0x100, 0x1B8, 0x14C, 0x1E0, 0x4C, 0x150, 0x1FF },
    { 0x180, 0x100, 0x192, 0x15A, 0x148, 0x5A, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1A8, 0x13C, 0x1A0, 0x3C, 0x170, 0x1FF },
    { 0x180, 0x100, 0x19A, 0x164, 0x168, 0x64, 0x140, 0x1FE },
};
u16 actor1Talk0Actions[] = { FLAG(2, 0x27), 1, ITEM(5, 0x10B), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x26), 0, CODES_END };
u16 actor2Talk0Actions[] = {
    FLAG(0x1A, 0x26), 1,
    START_EVENT(0x30), 1,
    FLAG(0x1A, 0x25), 0,
    CODES_END,
};
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0x26), 1, CODES_END };
u16 actor2Talk1Actions[] = { START_EVENT(0x30), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(0x10, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 6), 0,
    CODES_END,
};
u16 actor4Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 6), 1, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor4Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor4Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor4Talk6Actions[] = { CARD_BATTLE(0x10, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor5Talk2Actions[] = { CARD_BATTLE(0x10, 0), 1, CODES_END };
u16 actor5Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 0,
    CODES_END,
};
u16 actor5Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 6), 1, CODES_END };
u16 actor5Talk4Conditions[] = {
    FLAG(0xE, 6), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor5Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor5Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor5Talk6Actions[] = { CARD_BATTLE(0x10, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(0x10, 0), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 0,
    CODES_END,
};
u16 actor6Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 6), 1, CODES_END };
u16 actor6Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor6Talk5Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 6), 1,
    PARTY_STAT(5), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor6Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor6Talk6Actions[] = { CARD_BATTLE(0x10, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor9Talk1Conditions[] = { PARTY_STAT(1), 0, FLAG(0, 0), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor9Talk2Actions[] = { CARD_BATTLE(0x10, 0), 1, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 0,
    CODES_END,
};
u16 actor9Talk3Actions[] = { FLAG(0xE, 6), 1, CODES_END };
u16 actor9Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor9Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor9Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor9Talk6Actions[] = { CARD_BATTLE(0x10, 1), 1, CODES_END };
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x26C },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x2DF },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x2E0 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x2E1 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0xDC },
    { actor4Talk1Conditions, NULL, 0xE1 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0xE2 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0xE3 },
    { actor4Talk4Conditions, NULL, 0xE4 },
    { actor4Talk5Conditions, NULL, 0xE5 },
    { actor4Talk6Conditions, actor4Talk6Actions, 0xE6 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0xDD },
    { actor5Talk1Conditions, NULL, 0xE1 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0xE2 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0xE3 },
    { actor5Talk4Conditions, NULL, 0xE4 },
    { actor5Talk5Conditions, NULL, 0xE5 },
    { actor5Talk6Conditions, actor5Talk6Actions, 0xE6 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0xDE },
    { actor6Talk1Conditions, NULL, 0xE1 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0xE2 },
    { actor6Talk3Conditions, actor6Talk3Actions, 0xE3 },
    { actor6Talk4Conditions, NULL, 0xE4 },
    { actor6Talk5Conditions, NULL, 0xE5 },
    { actor6Talk6Conditions, actor6Talk6Actions, 0xE6 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0xDC },
    { actor7Talk1Conditions, actor7Talk1Actions, 0xE7 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0xE8 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x275 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0xDF },
    { actor9Talk1Conditions, NULL, 0xDF },
    { actor9Talk2Conditions, actor9Talk2Actions, 0xDF },
    { actor9Talk3Conditions, actor9Talk3Actions, 0xDF },
    { actor9Talk4Conditions, NULL, 0xDF },
    { actor9Talk5Conditions, NULL, 0xDF },
    { actor9Talk6Conditions, actor9Talk6Actions, 0xDF },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
#if VERSION_US
    PROGRESS(5), 1, CODES_END,
#elif VERSION_EU || VERSION_JP
    PROGRESS(5), 1, FLAG(0x40, 0x11), 0, CODES_END,
#endif
};
u16 actor1Conditions[] = { FLAG(2, 0x27), 0, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1A, 0x25), 1, FLAG(0x1A, 0x27), 0, CODES_END };
u16 actor3Conditions[] = { FLAG(0x1A, 0x27), 1, FLAG(0x1A, 0x28), 0, CODES_END };
u16 actor4Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(4), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor8Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0xC, 4, 0, 0, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x21, 5, 1566, 529, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x23, 6, 1520, 985, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x23, 6, 1520, 985, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x33, 7, 512, 554, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x33, 7, 512, 554, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x33, 7, 512, 554, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x33, 7, 512, 554, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x33, 7, 512, 554, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 8, 512, 554, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 1559, 780, 0, 0 },
    { 1, 0, 0x78, 2, 0x12, 0, 0, 0, 0, 0, 1216, 594, 0, 0 },
    { 1, 0, 0x78, 2, 0x14, 0, 0, 0, 0, 0, 1172, 388, 0, 0 },
    { 1, 0, 0x78, 2, 0x16, 0, 0, 0, 0, 0, 998, 303, 0, 0 },
    { 1, 0, 0x78, 2, 0x17, 0, 0, 0, 0, 0, 953, 355, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 86, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 621, 642, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 1, 0xC, 0x11, 8, 0, 1201, 300, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 1115, 218, 262, 0 },
    { 1, 0, 0x48, 4, 1, 0, 0, 0, 0, 0, 828, 359, 400, 0 },
    { 1, 0, 0x60, 4, 2, 0, 0, 0, 0, 0, 476, 268, 321, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 1216, 649, 682, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1169, 434, 465, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 545, 557, 583, 0 },
    { 1, 0, 0x44, 4, 6, 0, 0, 0, 0, 0, 925, 722, 766, 0 },
    { 1, 0, 0x73, 4, 7, 0, 0, 0, 0, 0, 634, 661, 767, 0 },
    { 1, 0, 0x78, 4, 0x13, 0, 0, 0, 0, 0, 1264, 489, 562, 0 },
    { 1, 0, 0x78, 4, 0x15, 0, 0, 0, 0, 0, 905, 338, 355, 0 },
    { 1, 0, 0x78, 4, 0x18, 0, 0, 0, 0, 0, 914, 450, 464, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 288, 133, 133, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 384, 135, 135, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 425, 155, 155, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 432, 311, 311, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 472, 179, 179, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 521, 203, 203, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 263, 263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 391, 391, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 574, 368, 368, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 607, 214, 214, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 617, 347, 347, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 648, 235, 235, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 695, 259, 259, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 744, 283, 283, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 792, 307, 307, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 840, 331, 331, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 799, 799, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1008, 791, 791, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1376, 751, 751, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1424, 775, 775, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1472, 799, 799, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x227, 0x4A2, 0x36A, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22A, 0xAA, 0xC3, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22E, 0x150, 0x268, 5, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 1, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 5, 4 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 80, script80, EVENT_TEXT(0), NULL, endEvent80 },
    { 1299, script1299, EVENT_TEXT(0x24), NULL, NULL },
    { 1305, script1305, EVENT_TEXT(0x25), NULL, endEvent1305 },
    { -1, NULL, 0, NULL, NULL },
};
