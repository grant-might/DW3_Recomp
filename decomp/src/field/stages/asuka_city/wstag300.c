/*
 * WSTAG300: Admin Center 2F, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of progress 0x16, which depends on flags 0x404A to 0x404C */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (GAME.progress == 0x16 && FLAGS_00.checkCondition(FLAG(0x40, 0x4A), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x4B), 0)) {
            children[0] = FIELDSTG_startEvent(0x23B);
        } else if (GAME.progress == 0x16 && FLAGS_00.checkCondition(FLAG(0x40, 0x4B), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x4C), 0)) {
            children[0] = FIELDSTG_startEvent(0x23C);
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
 * After event 570, with Soldier: starts the stage's event battle 1, sets
 * flag 0x0C11 (which decides whether actor 15 appears) and keeps it from
 * playing again
 */
void endEvent570(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x4A), 1);
    FLAGS_00.applyAction(FLAG(0xC, 0x11), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 571, with the player: starts the stage's event battle 2 and
 * keeps it from playing again
 */
void endEvent571(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x4B), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(2), 1);
}

/* After event 572, with the player: keeps it from playing again */
void endEvent572(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x4C), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x340
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x34F
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
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x49B00, 0x20900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
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

s16 script349[] = {
    SCRIPT_FOLLOW(0, 0x6A),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_PLACE(0x14A, 0x370, 0xE1),
    SCRIPT_POSE(0x14A, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x14A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0x390, 0xF1, 3),
    SCRIPT_POSE(0x14A, 1, 7),
    SCRIPT_COMMAND(0x323, 0x326, 0x14A),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x14A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 8, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_POSE(0x14A, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x14A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6A, 1, 4),
    SCRIPT_POSE(0x14A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0x390, 0xD0, 4),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_WALK(0x6A, 0x3A8, 0xC4, 5),
    SCRIPT_WALK(0x14A, 0x3A8, 0xC4, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(0), 0x6A),
    SCRIPT_LEAVE(0x218, 0x58, 0x18C, 5),
    SCRIPT_END,
};
s16 script570[] = {
    SCRIPT_WALK(2, 0x370, 0xE0, 3),
    SCRIPT_PLACE(0x130, 0x3A1, 0xC9),
    SCRIPT_POSE(0x130, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x130),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x130, 0x389, 0xD5, 1),
    SCRIPT_WAIT_WALK(0x130),
    SCRIPT_TALK(0, 1, 0x130, 2),
    SCRIPT_POSE(0x130, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script571[] = {
    SCRIPT_PLACE(2, 0x370, 0xE0),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(0), 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x76, 0x3B9, 0xBD),
    SCRIPT_POSE(0x76, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x76, 0x389, 0xD5, 1),
    SCRIPT_WAIT_WALK(0x76),
    SCRIPT_TALK(0, 1, 0x76, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x76, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x76, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x76, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x76, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x76, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(0x76, 1, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x76, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x76, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x76, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x76, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x76, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script572[] = {
    SCRIPT_PLACE(2, 0x370, 0xE0),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x76, 0x389, 0xD5),
    SCRIPT_POSE(0x76, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(0), 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 2, 0x76, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0x76, 0x3C9, 0xB5, 5),
    SCRIPT_WAIT_WALK(0x76),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_PLACE(0x76, 0, 0),
    SCRIPT_POSE(0x76, 1, 0),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHOW_OBJECTS(0), 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_WAIT_BOX,
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
AreaBattle area3Battle1 = { 190, 15, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 322, 15, MUSIC(2, 0) };
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
    { 136, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B8, 0x100, 0x1E0, 0, 0x170, 0x1F9 },
    { 0x140, 0x100, 0x172, 0x1B1, 0xC8, 0xB1, 0x170, 0x1F8 },
    { 0x180, 0x100, 0x1B0, 0x100, 0x1C0, 0, 0x170, 0x1F7 },
    { 0x180, 0x100, 0x1B0, 0x130, 0x1C0, 0x30, 0x170, 0x1F6 },
    { 0x180, 0x100, 0x1A0, 0x197, 0x180, 0x97, 0x150, 0x1F5 },
    { 0x180, 0x100, 0x190, 0x19C, 0x140, 0x9C, 0x160, 0x1F5 },
    { 0x180, 0x100, 0x1A8, 0x167, 0x1A0, 0x67, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x190, 0x174, 0x140, 0x74, 0x150, 0x1F4 },
    { 0x180, 0x100, 0x198, 0x174, 0x160, 0x74, 0x160, 0x1F4 },
    { 0x180, 0x100, 0x180, 0x17F, 0x100, 0x7F, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x188, 0x17F, 0x120, 0x7F, 0x150, 0x1F3 },
    { 0x180, 0x100, 0x1A8, 0x18F, 0x1A0, 0x8F, 0x160, 0x1F3 },
    { 0x180, 0x100, 0x1B0, 0x190, 0x1C0, 0x90, 0x170, 0x1F3 },
    { 0x180, 0x100, 0x198, 0x19C, 0x160, 0x9C, 0x150, 0x1F2 },
    { 0x180, 0x100, 0x1B0, 0x160, 0x1C0, 0x60, 0x160, 0x1F2 },
    { 0x180, 0x100, 0x1A0, 0x167, 0x180, 0x67, 0x170, 0x1F2 },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x14), 1, ITEM(1, 0x2B), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor13Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0xF), 1, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0xC, 0x10), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor15Talk0Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xC, 0x11), 1, CODES_END };
u16 actor16Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0xB), 1, CODES_END };
u16 actor17Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0xC), 1, CODES_END };
u16 actor18Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0xD), 1, CODES_END };
u16 actor19Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0xE), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2B6 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xAC },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xAA },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xAC },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xAA },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xAC },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, actor13Talk0Actions, 0x417 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, actor14Talk0Actions, 0x417 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, actor15Talk0Actions, 0x1ED },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, actor16Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, actor17Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, actor18Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, actor19Talk0Actions, 0xA9 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x1E6 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xAA },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xAB },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0xA7 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x14), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x20), 1, PROGRESS(0x21), 0, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x20), 1, PROGRESS(0x21), 0, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xF), 0, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0x10), 0, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0x11), 0, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xB), 0, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xC), 0, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xD), 0, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x16), 1, FLAG(0xC, 0xE), 0, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x20), 1, PROGRESS(0x21), 0, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0xD), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 700, 283, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x25, 5, 880, 225, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x25, 5, 880, 225, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x26, 6, 905, 188, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x26, 6, 905, 188, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x26, 6, 848, 209, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x26, 6, 848, 209, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x27, 7, 953, 213, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x27, 7, 953, 213, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x27, 7, 912, 240, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x27, 7, 912, 240, 7 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x6A, 8, 0, 0, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x76, 9, 0, 0, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x12B, 0xA, 992, 282, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x12F, 0xB, 769, 169, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x130, 0xC, 929, 201, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x132, 0xD, 936, 477, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x133, 0xE, 1225, 429, 3 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x134, 0xF, 321, 201, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x135, 0x10, 1120, 281, 1 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x14A, 0x11, 880, 225, 5 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x17D, 0x12, 1225, 429, 3 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x17D, 0x12, 929, 201, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x17D, 0x12, 929, 201, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x17E, 0x13, 318, 199, 7 };
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
    &actor24,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x36, 2, 0, 9, 8, 0, 918, 96, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 9, 0xA, 0, 918, 96, 0, 0 },
    { 1, 0, 0xFF, 2, 0xB, 0, 0, 0, 0, 0, 992, 126, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 166, 118, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 541, 90, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 1115, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 332, 103, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1009, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1211, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1294, 168, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 5, 8, 0, 860, 89, 0, 0 },
    { 1, 0, 0x80, 6, 0x35, 3, 0, 0xF, 0xE, 0, 660, 434, 0, 0 },
    { 1, 0x64, 0x40, 6, 0, 0, 0, 0, 0, 0, 929, 148, 0, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 768, 107, 151, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 992, 218, 264, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 1088, 218, 264, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1152, 186, 230, 0 },
    { 1, 0, 0x60, 4, 5, 0, 0, 0, 0, 0, 816, 333, 432, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 945, 441, 462, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 960, 190, 231, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 480, 321, 342, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 497, 256, 272, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 953, 144, 200, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x205, 0x158, 0xA4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x218, 0x56, 0x18A, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x203, 0xC8, 0x9C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x214, 0x177, 0xBD, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x205, 0x372, 0x132, 7, 0, 0, 0 },
    { { { PROGRESS(0x16), 1 }, { FLAG(0x40, 0x4A), 0 } }, SLOT_EVENT, 0x23A, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xD), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x15D, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 349, script349, EVENT_TEXT(3), NULL, NULL },
    { 570, script570, EVENT_TEXT(7), NULL, endEvent570 },
    { 571, script571, EVENT_TEXT(8), NULL, endEvent571 },
    { 572, script572, EVENT_TEXT(9), NULL, endEvent572 },
    { -1, NULL, 0, NULL, NULL },
};
