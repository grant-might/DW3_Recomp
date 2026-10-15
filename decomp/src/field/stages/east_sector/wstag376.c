/*
 * WSTAG376: Protocol Ruins, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG375.
 */

#include "common.h"
#include "field/stage.h"

/*
 * Back from the battle after event 1283, starts event 1284, with BK
 * MegaGargomon, unless it has played
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x78), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x79), 0)) {
            children[0] = FIELDSTG_startEvent(0x504);
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
 * After event 1283, with BK MegaGargomon: lets event 1284 start and starts
 * the stage's event battle 1
 */
void endEvent1283(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x78), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 1284, with BK MegaGargomon: gives the player the Wild Helmet
 * and keeps it from playing again
 */
void endEvent1284(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x79), 1);
    FLAGS_00.applyAction(ITEM(5, 0xF2), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x579
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x589
#endif
/* Sets the stage up: its map, actors, battles and events, playing PYRA_BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x11A00, 0x3AD00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xB;
    FIELDSTG_state.music = MUSIC(0xB, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1283[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x411, 0xA1, 5),
    SCRIPT_PLACE(0x106, 0x431, 0x91),
    SCRIPT_POSE(0x106, 1, 1),
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
    SCRIPT_TALK(0, 2, 0x106, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x106, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1284[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x411, 0xA1),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x106, 0x431, 0x91),
    SCRIPT_POSE(0x106, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x106, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x106, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x106, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 96, 7, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 97, 7, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 97, 7, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 97, 7, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 97, 7, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 7, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 7, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 13, 19, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 316, 19, MUSIC(0x22, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 97, 7, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 68, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14E, 0x100, 0x38, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x140, 0x100, 0, 0, 0x150, 0x1FF },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x8E), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 0), 1, ITEM(2, 0x8A), 0, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(3, 0x8E), 0, FLAG(0, 0), 1, ITEM(2, 0x8A), 1, CODES_END };
u16 actor0Talk3Actions[] = {
    ITEM(3, 0x8E), 1,
    ITEM(3, 0x8D), 0,
    ITEM(2, 0x8A), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor1Talk0Conditions[] = { FLAG(0xA, 7), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0xA, 7), 1, START_EVENT(0x39), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0xA, 7), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0xA, 7), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0xA, 7), 1, START_EVENT(0x39), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0xA, 7), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0xA, 7), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0xA, 7), 1, START_EVENT(0x39), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0xA, 7), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0xA, 7), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0xA, 7), 1, START_EVENT(0x39), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0xA, 7), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0xA, 7), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0xA, 7), 1, START_EVENT(0x39), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0xA, 7), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x2F6 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x2F7 },
    { actor0Talk2Conditions, NULL, 0x2F8 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x2F9 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x2D2 },
    { actor1Talk1Conditions, NULL, 0x49 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x2D2 },
    { actor2Talk1Conditions, NULL, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x2D2 },
    { actor3Talk1Conditions, NULL, 0x4A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x2D2 },
    { actor4Talk1Conditions, NULL, 0x4B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x2D2 },
    { actor5Talk1Conditions, NULL, 0x4D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x48), 1,
    SPECIAL(0x50), 1,
    ITEM(3, 0x8D), 1,
    ITEM(3, 0x8E), 0,
    CODES_END,
};
u16 actor1Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x1C, 4, 97, 242, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x106, 5, 1073, 145, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x106, 5, 1073, 145, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x106, 5, 1073, 145, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x106, 5, 1073, 145, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x106, 5, 1073, 145, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    NULL,
};
StageTile stageObjects[] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x294, 0x510, 0x88, 1, 0, 0, 0 },
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
    { 1283, script1283, EVENT_TEXT(0x22), NULL, endEvent1283 },
    { 1284, script1284, EVENT_TEXT(0x23), NULL, endEvent1284 },
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
