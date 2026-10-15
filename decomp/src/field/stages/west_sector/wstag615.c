/*
 * WSTAG615: Sewers, West Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of story progress 0x12 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 0x12 && FLAGS_00.checkCondition(FLAG(0x40, 0x39), 1)) {
            children[0] = FIELDSTG_startEvent(0x213);
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
 * After event 530, with WaruMonzaemon and Lisa: starts the stage's event
 * battle 0 and keeps it from playing again
 */
void endEvent530(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x39), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 531, with Lisa, Numemon and Lucky Mouse: moves the story on to its step 0x13 */
void endEvent531(void) {
    GAME.progress = 0x13;
}

/* Defined below, after the code that uses them */
extern StageSlot stageSlots0[];
extern StageSlot stageSlots1[];
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x54D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x55D
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV_0018; slot list 0 before story step 0x15, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xBF00, 0x2AF00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3A;
    FIELDSTG_state.music = MUSIC(0x3A, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0x15) {
        FIELDSTG_state.slots = stageSlots0;
    } else {
        FIELDSTG_state.slots = stageSlots1;
    }
}

s16 script530[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x231, 0x172, 7),
    SCRIPT_PLACE(0x65, 0x280, 0x238),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_PLACE(0x97, 0x29C, 0x22A),
    SCRIPT_POSE(0x97, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x291, 0x1A1, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x291, 0x1F1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 0x97, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 5, 0x65, 0),
    SCRIPT_POSE(0x65, 0x4D, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 0),
    SCRIPT_POSE(0x65, 0x4C, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_POSE(0x65, 0x4D, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(2, 0x340, 0x1F9, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x2C0, 0x239, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x97, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x97, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script531[] = {
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_PLACE(1, 0x2AF, 0x240),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_PLACE(0x65, 0x28F, 0x230),
    SCRIPT_POSE(0x65, 3, 7),
    SCRIPT_PLACE(0x69, 0x2B8, 0x1B2),
    SCRIPT_POSE(0x69, 1, 6),
    SCRIPT_PLACE(0x97, 0x261, 0x221),
    SCRIPT_POSE(0x97, 0x10, 1),
    SCRIPT_PLACE(0x11F, 0x261, 0x222),
    SCRIPT_POSE(0x11F, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_PIYOPIYO, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x65, 0x4B, 7),
    SCRIPT_WAIT_ANIM(0x65),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x65, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x69, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_WALK(0x69, 0x340, 0x1F8, 7),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_WALK(0x69, 0x320, 0x208, 1),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_TALK(0, 7, 0x65, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x69, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x69, 0x47, 1),
    SCRIPT_WAIT(0x12),
    SCRIPT_PLACE(0x68, 0x320, 0x207),
    SCRIPT_POSE(0x68, 0x47, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_WEAR_OFF, 1),
    SCRIPT_WAIT_ANIM(0x68),
    SCRIPT_PLACE(0x69, 0, 0),
    SCRIPT_POSE(0x69, 1, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 9, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x65, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 1, 3),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0x68, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x68, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x68, 0x340, 0x1F8, 5),
    SCRIPT_WAIT_WALK(0x68),
    SCRIPT_WALK(1, 0x2D0, 0x230, 5),
    SCRIPT_WALK(0x65, 0x2B0, 0x240, 7),
    SCRIPT_WALK(0x68, 0x320, 0x1E7, 3),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(1, 0x33F, 0x1F7, 5),
    SCRIPT_WALK(0x65, 0x33F, 0x1F7, 5),
    SCRIPT_WALK(0x68, 0x26F, 0x190, 3),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0x255, 0x1C0, 0x189, 1),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 81, 12, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 81, 12, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 81, 12, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 81, 12, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 82, 12, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 82, 12, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 82, 12, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 82, 12, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 8, 19, MUSIC(0x22, 0) };
AreaBattle area3Battle1 = { 312, 19, MUSIC(0x22, 0) };
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
    { 48, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1A8, 0x168, 0x1A0, 0x68, 0x160, 0x1FE },
    { 0x180, 0x100, 0x1B6, 0x130, 0x1D8, 0x30, 0x170, 0x1FE },
    { 0x140, 0x100, 0x179, 0x100, 0xE4, 0, 0x140, 0x1FD },
    { 0x180, 0x100, 0x180, 0x16E, 0x100, 0x6E, 0x150, 0x1FD },
    { 0x140, 0x100, 0x170, 0x18B, 0xC0, 0x8B, 0x160, 0x1FD },
    { 0x140, 0x100, 0x178, 0x168, 0xE0, 0x68, 0x170, 0x1FD },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x156 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x15A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x157 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x158 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x159 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x15B },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x155 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x12), 1, FLAG(0x40, 0x39), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x12), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0x65, 5, 640, 570, 5 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0x68, 6, 0, 0, 1 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0x69, 7, 0, 0, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x97, 8, 624, 544, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x97, 8, 668, 554, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x11F, 9, 0, 0, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x56, 2, 0, 1, 4, 0, 660, 140, 0, 0 },
    { 1, 0, 0x48, 6, 0, 0, 0, 0, 0, 0, 143, 793, 0, 0 },
    { 1, 0, 0x44, 6, 1, 0, 0, 0, 0, 0, 298, 816, 0, 0 },
    { 1, 0, 0x40, 6, 2, 0, 0, 0, 0, 0, 512, 816, 0, 0 },
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
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 200, 474, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 215, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 422, 463, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 477, 351, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 493, 353, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 755, 721, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0x14, 0, 764, 711, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 39, 789, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 120, 516, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 268, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 282, 682, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 341, 409, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 436, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 726, 215, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 829, 116, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0x14, 0, 859, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0x14, 0, 206, 865, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0x14, 0, 222, 867, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0x14, 0, 284, 673, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0x14, 0, 17, 551, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0x14, 0, 497, 703, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0x14, 0, 577, 412, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0x14, 0, 597, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0x14, 0, 664, 468, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0x14, 0, 217, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0x14, 0, 509, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0x14, 0, 682, 371, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0x14, 0, 913, 92, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 424, 668, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 434, 677, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 482, 843, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 490, 851, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 683, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 706, 379, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0x14, 0, 880, 477, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 107, 521, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 198, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 206, 427, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 262, 450, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 360, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 432, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 439, 462, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 498, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 683, 390, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 686, 524, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 692, 394, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 698, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 714, 219, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 734, 836, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 739, 826, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 792, 179, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 803, 181, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 819, 714, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 833, 707, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 865, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 875, 481, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 878, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0x14, 0, 882, 98, 0, 0 },
    { 1, 0x64, 0x40, 6, 3, 0, 0, 0, 0, 0, 687, 136, 0, 0 },
    { 1, 0, 0x40, 6, 5, 1, 5, 0xA, 4, 0, 304, 820, 0, 0 },
    { 1, 0, 0x40, 6, 0xB, 1, 0xB, 0x10, 4, 0, 157, 799, 0, 0 },
    { 1, 0, 0x40, 6, 0xB, 1, 0xB, 0x10, 4, 0, 430, 744, 0, 0 },
    { 1, 0, 0x40, 6, 0xB, 1, 0xB, 0x10, 4, 0, 527, 823, 0, 0 },
    { 1, 0, 0x40, 6, 0x11, 1, 0x11, 0x16, 4, 0, 563, 592, 0, 0 },
    { 1, 0, 0x40, 6, 0x11, 1, 0x11, 0x16, 4, 0, 602, 612, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 1, 0x17, 0x1C, 4, 0, 70, 455, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 1, 0x17, 0x1C, 4, 0, 326, 327, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 1, 0x17, 0x1C, 4, 0, 366, 307, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 1, 0x17, 0x1C, 4, 0, 582, 199, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 1, 0x17, 0x1C, 4, 0, 622, 179, 0, 0 },
    { 1, 0, 0x78, 6, 0x1D, 0, 0, 0, 0, 0, 810, 120, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2C, 1, 0x2C, 0x2E, 0x14, 0, 691, 754, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 681, 794, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 705, 816, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 853, 676, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x58, 1, 0x58, 0x5B, 0x14, 0, 949, 526, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 6, 803, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 22, 594, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 229, 490, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 449, 422, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 471, 311, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 587, 259, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 648, 810, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 687, 352, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 724, 471, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 808, 662, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 844, 541, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 893, 386, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0x14, 0, 925, 508, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 46, 550, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 106, 587, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 177, 523, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 258, 699, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 319, 464, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 383, 643, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 420, 742, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 455, 384, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 549, 414, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 620, 365, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 626, 460, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 671, 285, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 769, 744, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 830, 460, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 0x14, 0, 859, 156, 0, 0 },
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
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 646, 144, 191, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots0[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x253, 0x90, 0x240, 7, 0, 0, 0 },
    { { { SPECIAL(0x41), 1 }, { CODES_END, 0 } }, SLOT_EXIT, 0x255, 0x268, 0x1AC, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0xB3, 0x238, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0xC3, 0x2A0, 0, 0, 0, 0 },
    { { { PROGRESS(0x12), 1 }, { FLAG(0x40, 0x39), 0 } }, SLOT_EVENT, 0x212, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots1[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x253, 0x90, 0x240, 7, 0, 0, 0 },
    { { { SPECIAL(0x41), 1 }, { CODES_END, 0 } }, SLOT_EXIT, 0x256, 0x268, 0x1AC, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0xB3, 0x238, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0xC3, 0x2A0, 0, 0, 0, 0 },
    { { { PROGRESS(0x12), 1 }, { FLAG(0x40, 0x39), 0 } }, SLOT_EVENT, 0x212, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 530, script530, EVENT_TEXT(0x15), NULL, endEvent530 },
    { 531, script531, EVENT_TEXT(5), NULL, endEvent531 },
    { -1, NULL, 0, NULL, NULL },
};
