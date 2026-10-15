/*
 * WSTAG285: Admin Center 1F, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of the story so far, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        do {
            if (FLAGS_00.checkCondition(FLAG(0x40, 5), 0)) {
                children[0] = FIELDSTG_startEvent(0x28);
                break;
            }
            if (GAME.progress == 0xB) {
                children[0] = FIELDSTG_startEvent(0x110);
                break;
            }
            if (GAME.progress == 0xD && FLAGS_00.checkCondition(FLAG(0x1C, 0xC), 0)) {
                children[0] = FIELDSTG_startEvent(0x154);
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

/* Clears the flag of the story so far */
void endEvent272(void) {
    FLAGS_00.applyAction(FLAG(0x40, 5), 0);
}

/* After event 340, with DO Guard: keeps it from playing again */
void endEvent340(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0xC), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x2A6
#define STAGE_ARCHIVE 0x31E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x2B5
#define STAGE_ARCHIVE 0x32D
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing KANRIBGM;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x23900, 0x29F00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x14 && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script40[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x1E3, 0x2C6),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x25, 0x230, 0x2A1),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_WALK(2, 0x208, 0x2B4, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x25),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x25),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x25, 0x220, 0x2A7, 1),
    SCRIPT_WAIT_WALK(0x25),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x25, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1DA, 0x2CC, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LOOK_AT(1, 0x1DA, 0x2CC),
    SCRIPT_PLACE(2, 0, 0),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x203, 0x14A, 0xC4, 1),
    SCRIPT_END,
};
s16 script272[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x1E3, 0x2C6),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x25, 0x230, 0x2A1),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1F4, 0x2BE, 5),
    SCRIPT_PLACE(0xB, 0x1E3, 0x2C6),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x208, 0x2B4, 5),
    SCRIPT_WALK(0xB, 0x1F4, 0x2BE, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x25),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_COMMAND(0x323, 0x326, 0x25),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x25, 0x220, 0x2A7, 1),
    SCRIPT_WAIT_WALK(0x25),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x25, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1FB, 0x2BB, 1),
    SCRIPT_WALK(0xB, 0x1E3, 0x2C6, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x1DA, 0x2CC, 1),
    SCRIPT_PLACE(0xB, 0, 0),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LOOK_AT(1, 0x1DA, 0x2CC),
    SCRIPT_PLACE(2, 0, 0),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x204, 0x12A, 0x12B, 1),
    SCRIPT_END,
};
s16 script340[] = {
    SCRIPT_FOLLOW(1, 0x6A),
    SCRIPT_PLACE(0x25, 0x230, 0x2A1),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_PLACE(0x6A, 0x1E3, 0x2C6),
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0x208, 0x2B4, 5),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x25),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x25),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x25, 0x220, 0x2A7, 1),
    SCRIPT_WAIT_WALK(0x25),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x25, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x6A, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 0x25),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x25),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x25, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x25, 0x230, 0x2A1, 5),
    SCRIPT_WAIT_WALK(0x25),
    SCRIPT_POSE(0x25, 1, 1),
    SCRIPT_WAIT(0x1E),
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
    0,
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
AreaBattle area3Battle0 = { 189, 15, MUSIC(2, 0) };
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
    { 134, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B2, 0x1B0, 0x1C8, 0xB0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x170, 0x130, 0xC0, 0x30, 0x170, 0x1FE },
    { 0x140, 0x100, 0x172, 0x1B1, 0xC8, 0xB1, 0x150, 0x1FD },
    { 0x180, 0x100, 0x180, 0x1BB, 0x100, 0xBB, 0x160, 0x1FD },
    { 0x180, 0x100, 0x1B6, 0x160, 0x1D8, 0x60, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1B6, 0x188, 0x1D8, 0x88, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1A2, 0x1A2, 0x188, 0xA2, 0x160, 0x1FC },
    { 0x180, 0x100, 0x1AA, 0x1A2, 0x1A8, 0xA2, 0x170, 0x1FC },
    { 0x180, 0x100, 0x1B2, 0x100, 0x1C8, 0, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1B2, 0x130, 0x1C8, 0x30, 0x160, 0x1FB },
};
u16 actor1Talk0Actions[] = { FLAG(2, 0x15), 1, ITEM(2, 0xCF), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0xC, 5), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor8Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 6), 1, CODES_END };
u16 actor9Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 7), 1, CODES_END };
u16 actor10Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 7), 1, CODES_END };
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x452 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xA6 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, actor9Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, actor10Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xA7 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xA8 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor1Conditions[] = { FLAG(2, 0x15), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x23), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 5), 0, CODES_END };
u16 actor8Conditions[] = { FLAG(0xC, 6), 0, PROGRESS(0x16), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 7), 0, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 7), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x23), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x23), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0xB, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x21, 5, 825, 285, 1 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0x25, 6, 560, 673, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x25, 6, 560, 673, 1 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0x26, 7, 823, 819, 3 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x27, 8, 438, 244, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x6A, 9, 0, 0, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x132, 0xA, 823, 819, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x133, 0xB, 438, 244, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x134, 0xC, 370, 574, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x135, 0xD, 349, 586, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x17D, 0xE, 823, 819, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x17E, 0xF, 438, 244, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x36, 2, 0, 5, 8, 0, 437, 121, 0, 0 },
    { 1, 0, 0x78, 2, 0x32, 2, 0, 1, 4, 0, 121, 400, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 104, 383, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 212, 501, 0, 0 },
    { 1, 0, 0x40, 2, 0x34, 2, 0, 1, 4, 0, 264, 240, 0, 0 },
    { 1, 0, 0x78, 6, 0x33, 2, 0, 1, 4, 0, 116, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 104, 334, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 212, 215, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 263, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 76, 291, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 152, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 203, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 284, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 5, 8, 0, 748, 208, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 269, 421, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 348, 461, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 429, 501, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 509, 540, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 517, 161, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 597, 201, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 653, 613, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 733, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 813, 693, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 837, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 5, 8, 0, 893, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 2, 0, 5, 8, 0, 554, 542, 0, 0 },
    { 1, 0x64, 0x40, 6, 0xD, 0, 0, 0, 0, 0, 384, 133, 0, 0 },
    { 1, 0, 0x78, 4, 0x32, 2, 0, 1, 4, 0, 172, 373, 385, 0 },
    { 1, 0, 0x40, 4, 0x34, 2, 0, 1, 4, 0, 156, 358, 385, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 833, 737, 808, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 850, 744, 800, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 753, 706, 752, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 672, 666, 712, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 448, 554, 600, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 368, 514, 560, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 288, 474, 520, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 174, 336, 385, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 608, 250, 296, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 528, 210, 256, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 449, 161, 233, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 466, 168, 224, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 408, 135, 186, 0 },
    { 1, 0, 0x78, 4, 0x33, 2, 0, 1, 4, 0, 167, 255, 385, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x217, 0x318, 0x1DC, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x203, 0x14A, 0xC4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x215, 0xC8, 0x7C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 40, script40, EVENT_TEXT(0xA), NULL, NULL },
    { 272, script272, EVENT_TEXT(0x19), NULL, endEvent272 },
    { 340, script340, EVENT_TEXT(0x1F), NULL, endEvent340 },
    { -1, NULL, 0, NULL, NULL },
};
