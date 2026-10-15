/*
 * WSTAG535: Catacomb, South Sector: before story point 0x1A. The Jungle Shrine
 * (WSTAG530) leads here (mode 579) until GAME.progress reaches 0x1A.
 */

#include "common.h"
#include "field/stage.h"

/* Back from the battle after event 1262, starts event 1263, with Zanbamon, unless it has played */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x55), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x56), 0)) {
            children[0] = FIELDSTG_startEvent(0x4EF);
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

/* After event 1262, with Zanbamon: lets event 1263 start and starts the stage's event battle 0 */
void endEvent1262(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x55), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1263, with Zanbamon: gives the player the Rusty Katana and
 * keeps it from playing again
 */
void endEvent1263(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x56), 1);
    FLAGS_00.applyAction(ITEM(3, 0x66), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x285
#define STAGE_ARCHIVE 0x3CE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x294
#define STAGE_ARCHIVE 0x3DE
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0015 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x19E00, 0x11300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x11;
    FIELDSTG_state.music = MUSIC(0x11, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1262[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x357, 0x93, 5),
    SCRIPT_PLACE(0x85, 0x377, 0x85),
    SCRIPT_POSE(0x85, 1, 1),
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
    SCRIPT_TALK(0, 2, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x85, 0),
    SCRIPT_TALK(0, 0, 0, 0),
    SCRIPT_TALK(0, 4, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_JP
    0x800A,
#endif
};
s16 script1263[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x357, 0x93),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x85, 0x377, 0x85),
    SCRIPT_POSE(0x85, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x85, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 91, 6, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 91, 6, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 6, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 11, 19, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 91, 6, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 59, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x180, 0x100, 0x100, 0, 0x170, 0x1F5 },
};
u16 actor0Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor5Talk0Actions[] = { START_EVENT(0x36), 1, FLAG(0xA, 2), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0xA, 2), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0xA, 2), 1, START_EVENT(0x36), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0xA, 2), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x2F8 },
    { actor0Talk1Conditions, NULL, 0x1E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x2F8 },
    { actor1Talk1Conditions, NULL, 0x1E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x2F8 },
    { actor2Talk1Conditions, NULL, 0x1E4 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x2F8 },
    { actor3Talk1Conditions, NULL, 0x1E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x2F8 },
    { actor4Talk1Conditions, NULL, 0x1EA },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x2F8 },
    { actor5Talk1Conditions, NULL, 0x1E9 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x2F8 },
    { actor6Talk1Conditions, NULL, 0x1E8 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x2F8 },
    { actor7Talk1Conditions, NULL, 0x1E7 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x2F8 },
    { actor8Talk1Conditions, NULL, 0x1E6 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x2F8 },
    { actor9Talk1Conditions, NULL, 0x1E5 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xC), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x85, 4, 887, 133, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x85, 4, 887, 133, 1 };
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
    { 1, 0, 0x70, 2, 0x36, 1, 0x36, 0x3B, 6, 0, 949, 142, 0, 0 },
    { 1, 0, 0x70, 2, 0x3C, 1, 0x3C, 0x41, 6, 0, 941, 141, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 999, 304, 0, 0 },
    { 1, 0, 0x78, 6, 0x32, 2, 0, 0xD, 8, 0, 1000, 393, 0, 0 },
    { 1, 0, 0x78, 6, 0x33, 2, 0, 0xD, 8, 0, 993, 389, 0, 0 },
    { 1, 0, 0x70, 6, 0x34, 2, 0, 0xD, 8, 0, 642, 153, 0, 0 },
    { 1, 0, 0x70, 6, 0x35, 2, 0, 0xD, 8, 0, 634, 144, 0, 0 },
    { 1, 0, 0x70, 6, 0x42, 1, 0x42, 0x47, 6, 0, 955, 71, 0, 0 },
    { 1, 0, 0x70, 6, 0x48, 1, 0x48, 0x4D, 6, 0, 946, 71, 0, 0 },
    { 1, 0, 0x70, 6, 0x4E, 1, 0x4E, 0x53, 6, 0, 618, 227, 0, 0 },
    { 1, 0, 0x70, 6, 0x4E, 1, 0x4E, 0x53, 6, 0, 730, 171, 0, 0 },
    { 1, 0, 0x70, 6, 0x54, 2, 0, 3, 6, 0, 614, 232, 0, 0 },
    { 1, 0, 0x70, 6, 0x54, 2, 0, 3, 6, 0, 726, 176, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 258, 102, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 531, 190, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 758, 111, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 806, 87, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 827, 275, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 859, 292, 0, 0 },
    { 1, 0, 0x70, 6, 0x55, 2, 0, 3, 4, 0, 1002, 301, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 266, 105, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 539, 193, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 766, 114, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 814, 90, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 835, 279, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 867, 295, 0, 0 },
    { 1, 0, 0x70, 6, 0x56, 2, 0, 3, 4, 0, 1010, 304, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x242, 0x3E0, 0x1B0, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x3A0, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x3B0, 0x1E8, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1262, script1262, EVENT_TEXT(0xC), NULL, endEvent1262 },
    { 1263, script1263, EVENT_TEXT(0xD), NULL, endEvent1263 },
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
