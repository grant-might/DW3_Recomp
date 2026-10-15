/*
 * WSTAG616: Sewers, West Sector. On the Amaterasu server; its Asuka server
 * twin is WSTAG615.
 */

#include "common.h"
#include "field/stage.h"

/*
 * Back from the battle after event 1291, starts event 1292, with BK
 * KingNumemon, unless it has played
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x80), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x81), 0)) {
            children[0] = FIELDSTG_startEvent(0x50C);
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
 * After event 1291, with BK KingNumemon: lets event 1292 start and starts
 * the stage's event battle 0
 */
void endEvent1291(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x80), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1292, with BK KingNumemon: gives the player the BIT Adapter
 * and keeps it from playing again
 */
void endEvent1292(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x81), 1);
    FLAGS_00.applyAction(ITEM(7, 0x142), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x613
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x623
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0018 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xB700, 0x2B200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3A;
    FIELDSTG_state.music = MUSIC(0x3A, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1291[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x290, 0x230, 3),
    SCRIPT_PLACE(0x10A, 0x270, 0x220),
    SCRIPT_POSE(0x10A, 1, 7),
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
    SCRIPT_TALK(0, 2, 0x10A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x10A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US || VERSION_JP
    0x6004,
#elif VERSION_EU
    0x325,
#endif
};
s16 script1292[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x290, 0x230),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x10A, 0x270, 0x220),
    SCRIPT_POSE(0x10A, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x10A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x10A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x10A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 83, 12, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 83, 12, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 83, 12, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 83, 12, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 84, 12, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 84, 12, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 84, 12, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 84, 12, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 24, 19, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 320, 19, MUSIC(0x22, 0) };
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
    { 113, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x156, 0x177, 0x58, 0x77, 0x170, 0x1FE },
    { 0x140, 0x100, 0x160, 0x170, 0x80, 0x70, 0x140, 0x1FD },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x9A), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 1, ITEM(2, 0x96), 0, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(3, 0x9A), 0, FLAG(0, 0), 1, ITEM(2, 0x96), 1, CODES_END };
u16 actor0Talk3Actions[] = {
    ITEM(3, 0x9A), 1,
    ITEM(3, 0x99), 0,
    ITEM(2, 0x96), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor1Talk0Conditions[] = { FLAG(0xA, 0xC), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0xA, 0xC), 1, START_EVENT(0x3D), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0xA, 0xC), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0xA, 0xC), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0xA, 0xC), 1, START_EVENT(0x3D), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0xA, 0xC), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0xA, 0xC), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0xA, 0xC), 1, START_EVENT(0x3D), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0xA, 0xC), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0xA, 0xC), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0xA, 0xC), 1, START_EVENT(0x3D), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0xA, 0xC), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0xA, 0xC), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0xA, 0xC), 1, START_EVENT(0x3D), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0xA, 0xC), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x2EA },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x2EB },
    { actor0Talk2Conditions, NULL, 0x2EC },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x2ED },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x2D6 },
    { actor1Talk1Conditions, NULL, 0x119 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x2D6 },
    { actor2Talk1Conditions, NULL, 0x11D },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x2D6 },
    { actor3Talk1Conditions, NULL, 0x11C },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x2D6 },
    { actor4Talk1Conditions, NULL, 0x11B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x2D6 },
    { actor5Talk1Conditions, NULL, 0x11A },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x47), 1,
    SPECIAL(0x4F), 1,
    ITEM(3, 0x99), 1,
    ITEM(3, 0x9A), 0,
    CODES_END,
};
u16 actor1Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x25), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0xA9, 4, 464, 320, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x10A, 5, 624, 544, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x10A, 5, 624, 544, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x10A, 5, 624, 544, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x10A, 5, 624, 544, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x10A, 5, 624, 544, 7 };
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
    { 1, 0, 0x40, 2, 0x56, 2, 0, 1, 4, 0, 660, 140, 0, 0 },
    { 1, 0, 0x40, 6, 2, 0, 0, 0, 0, 0, 512, 816, 0, 0 },
    { 1, 0, 0x44, 6, 3, 0, 0, 0, 0, 0, 298, 816, 0, 0 },
    { 1, 0, 0x48, 6, 4, 0, 0, 0, 0, 0, 143, 793, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 1, 4, 0, 395, 684, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 1, 4, 0, 552, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 1, 4, 0, 96, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 196, 576, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 236, 352, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 436, 252, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 532, 204, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 724, 108, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 1, 4, 0, 789, 575, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 476, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 652, 624, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 684, 276, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 836, 352, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 908, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 1, 4, 0, 988, 664, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x41, 0xA, 0, 227, 467, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x41, 0xA, 0, 676, 506, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x41, 0xA, 0, 838, 140, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x4C, 0xA, 0, 410, 672, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x4C, 0xA, 0, 517, 306, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x4C, 0xA, 0, 686, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x4C, 0xA, 0, 756, 723, 0, 0 },
    { 1, 0x64, 0x40, 6, 0, 0, 0, 0, 0, 0, 687, 136, 0, 0 },
    { 1, 0, 0x78, 6, 0x1D, 0, 0, 0, 0, 0, 810, 120, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2C, 1, 0x2C, 0x2E, 0x14, 0, 691, 754, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 681, 794, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 705, 816, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 853, 676, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 949, 526, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x60, 1, 0x60, 0x62, 0x14, 0, 80, 601, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x60, 1, 0x60, 0x62, 0x14, 0, 80, 702, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0x14, 0, 295, 495, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2F, 1, 0x2F, 0x31, 0x14, 0, 298, 606, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 92, 757, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 190, 431, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 329, 641, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 483, 287, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 806, 127, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 872, 90, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x5C, 1, 0x5C, 0x5F, 0x14, 0, 922, 390, 0, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 646, 144, 191, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2BD, 0x90, 0x240, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2BF, 0x268, 0x1AC, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0xB3, 0x238, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0xC3, 0x2A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1291, script1291, EVENT_TEXT(0xA), NULL, endEvent1291 },
    { 1292, script1292, EVENT_TEXT(0xB), NULL, endEvent1292 },
    { -1, NULL, 0, NULL, NULL },
};
