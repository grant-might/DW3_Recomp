/*
 * WSTAG420: Seiryu City, East Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of progress 4 when flag 0x4012 is set */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 4 && FLAGS_00.checkCondition(FLAG(0x40, 0x12), 1)) {
            children[0] = FIELDSTG_startEvent(0x47);
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

/* After event 70, with Seiryu Leader: lets event 71 start and starts the stage's event battle 1 */
void endEvent70(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x12), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/*
 * After event 71, with Seiryu Leader: gives the player the Silver ID and the
 * Seiryu Badge and moves the story on to its step 5
 */
void endEvent71(void) {
    FLAGS_00.applyAction(ITEM(0, 0x28), 1);
    FLAGS_00.applyAction(ITEM(0, 9), 1);
    GAME.progress = 5;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x775
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x784
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing SEIR_BGM;
 * BGM_0029 instead at story steps 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1D100, 0x1F600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xC;
    FIELDSTG_state.music = MUSIC(0xC, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script70[] = {
    SCRIPT_WALK(2, 0x180, 0xDA, 1),
    SCRIPT_PLACE(0x3C, 0x160, 0xEA),
    SCRIPT_POSE(0x3C, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x3C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script71[] = {
    SCRIPT_PLACE(2, 0x180, 0xDA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x3C, 0x160, 0xEA),
    SCRIPT_POSE(0x3C, 1, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x3C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x3C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x3C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 6, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x3C, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1E8, 0xA4, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x230, 0xC8, 0x1A4, 5),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x882D,
#elif VERSION_JP
    0x0111,
#endif
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
AreaBattle area3Battle0 = { 209, 20, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 3, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle2 = { 301, 18, MUSIC(0x23, 0) };
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
    { 155, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x1D0, 0x1C0, 0xD0, 0x150, 0x1FF },
    { 0x180, 0x100, 0x1A4, 0x140, 0x190, 0x40, 0x160, 0x1FF },
    { 0x1C0, 0x100, 0x1D0, 0x100, 0x240, 0, 0x170, 0x1FF },
    { 0x1C0, 0x100, 0x1D8, 0x100, 0x260, 0, 0x140, 0x1FE },
    { 0x1C0, 0x100, 0x1E0, 0x100, 0x280, 0, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1E8, 0x100, 0x2A0, 0, 0x170, 0x1FE },
    { 0x1C0, 0x100, 0x1D2, 0x128, 0x248, 0x28, 0x140, 0x1FD },
    { 0x1C0, 0x100, 0x1DA, 0x128, 0x268, 0x28, 0x150, 0x1FD },
    { 0x1C0, 0x100, 0x1E2, 0x128, 0x288, 0x28, 0x160, 0x1FD },
    { 0x1C0, 0x100, 0x1EA, 0x132, 0x2A8, 0x32, 0x170, 0x1FD },
    { 0x1C0, 0x100, 0x1F2, 0x132, 0x2C8, 0x32, 0x140, 0x1FC },
    { 0x1C0, 0x100, 0x1C0, 0x13E, 0x200, 0x3E, 0x150, 0x1FC },
    { 0x1C0, 0x100, 0x1C8, 0x148, 0x220, 0x48, 0x160, 0x1FC },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 0x2E), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 0x2E), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 0x2E), 1, SPECIAL(0x2C), 0, CODES_END };
u16 actor0Talk2Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 0,
    CODES_END,
};
u16 actor0Talk3Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 1,
    SPECIAL(0x27), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { FLAG(6, 1), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 1,
    SPECIAL(0x27), 1,
    CODES_END,
};
u16 actor1Talk0Conditions[] = { SPECIAL(0xC), 0, CODES_END };
u16 actor1Talk0Actions[] = { SPECIAL(0x33), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk1Conditions[] = { SPECIAL(0xC), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x2E), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1A, 0x2E), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0x2E), 1, SPECIAL(0x2C), 0, CODES_END };
u16 actor2Talk2Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 0,
    CODES_END,
};
u16 actor2Talk3Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 1,
    SPECIAL(0x27), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { FLAG(6, 1), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0x1A, 0x2E), 1,
    SPECIAL(0x2C), 1,
    SPECIAL(0x25), 1,
    SPECIAL(0x27), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { SPECIAL(0xC), 0, CODES_END };
u16 actor3Talk0Actions[] = { SPECIAL(0x33), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor3Talk1Conditions[] = { SPECIAL(0xC), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor14Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor15Talk2Actions[] = { CARD_BATTLE(0x13, 0), 1, CODES_END };
u16 actor15Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 0,
    CODES_END,
};
u16 actor15Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 9), 1, CODES_END };
u16 actor15Talk4Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor15Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor15Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor15Talk6Actions[] = { CARD_BATTLE(0x13, 1), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(3), 0, PARTY_STAT(1), 1, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(0x13, 0), 1, CODES_END };
u16 actor16Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 0,
    CODES_END,
};
u16 actor16Talk3Actions[] = { FLAG(0xE, 9), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor16Talk4Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor16Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor16Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor16Talk6Actions[] = { CARD_BATTLE(0x13, 1), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor18Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor18Talk1Conditions[] = { PARTY_STAT(1), 0, FLAG(0, 0), 1, CODES_END };
u16 actor18Talk2Conditions[] = { PARTY_STAT(1), 1, FLAG(0, 0), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor18Talk2Actions[] = { CARD_BATTLE(0x13, 0), 1, CODES_END };
u16 actor18Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 0,
    CODES_END,
};
u16 actor18Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 9), 1, CODES_END };
u16 actor18Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    CODES_END,
};
u16 actor18Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor18Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 9), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor18Talk6Actions[] = { CARD_BATTLE(0x13, 1), 1, CODES_END };
u16 actor31Talk0Conditions[] = {
    SPECIAL(0x20), 1,
    PROGRESS(0x20), 0,
    PROGRESS(0x21), 0,
    FLAG(0, 5), 0,
    CODES_END,
};
u16 actor31Talk0Actions[] = { FLAG(0, 5), 1, CODES_END };
u16 actor31Talk1Conditions[] = {
    FLAG(0, 5), 1,
    SPECIAL(0x20), 1,
    PROGRESS(0x20), 0,
    PROGRESS(0x21), 0,
    CODES_END,
};
u16 actor31Talk2Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor31Talk3Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor31Talk4Conditions[] = { SPECIAL(0x19), 1, SPECIAL(0x20), 0, CODES_END };
u16 actor38Talk0Actions[] = { START_EVENT(0x2E), 1, CODES_END };
u16 actor41Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor41Talk1Conditions[] = { PARTY_STAT(1), 0, FLAG(0, 0), 1, CODES_END };
u16 actor41Talk2Conditions[] = { PARTY_STAT(1), 1, FLAG(0, 0), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor41Talk2Actions[] = { CARD_BATTLE(0x13, 0), 1, CODES_END };
u16 actor41Talk3Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor41Talk3Actions[] = { FLAG(0xE, 9), 1, CODES_END };
u16 actor41Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    CODES_END,
};
u16 actor41Talk5Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    PARTY_STAT(5), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor41Talk6Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 9), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor41Talk6Actions[] = { CARD_BATTLE(0x13, 1), 1, CODES_END };
u16 actor45Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor45Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor58Talk0Conditions[] = { FLAG(0x1C, 0x47), 0, CODES_END };
u16 actor58Talk0Actions[] = { FLAG(0x1C, 0x47), 1, CODES_END };
u16 actor58Talk1Conditions[] = { FLAG(0x1C, 0x47), 1, ITEM(0, 0x18E), 0, CODES_END };
u16 actor58Talk2Conditions[] = { ITEM(0, 0x18E), 1, FLAG(0x1C, 0x47), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x114 },
    { actor0Talk1Conditions, NULL, 0x11A },
    { actor0Talk2Conditions, NULL, 0x11C },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x115 },
    { actor0Talk4Conditions, NULL, 0x11B },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x116 },
    { actor1Talk1Conditions, NULL, 0x119 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x114 },
    { actor2Talk1Conditions, NULL, 0x11A },
    { actor2Talk2Conditions, NULL, 0x11C },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x115 },
    { actor2Talk4Conditions, NULL, 0x11B },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x116 },
    { actor3Talk1Conditions, NULL, 0x119 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x29 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xA },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x10 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x33 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x2E },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x15 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x24 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0xD5 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0xE0 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0xE1 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0xD6 },
    { actor15Talk1Conditions, NULL, 0xDA },
    { actor15Talk2Conditions, actor15Talk2Actions, 0xDB },
    { actor15Talk3Conditions, actor15Talk3Actions, 0xDC },
    { actor15Talk4Conditions, NULL, 0xDD },
    { actor15Talk5Conditions, NULL, 0xDE },
    { actor15Talk6Conditions, actor15Talk6Actions, 0xDF },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xD7 },
    { actor16Talk1Conditions, NULL, 0xDA },
    { actor16Talk2Conditions, actor16Talk2Actions, 0xDB },
    { actor16Talk3Conditions, actor16Talk3Actions, 0xDC },
    { actor16Talk4Conditions, NULL, 0xDD },
    { actor16Talk5Conditions, NULL, 0xDE },
    { actor16Talk6Conditions, actor16Talk6Actions, 0xDF },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x118 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, actor18Talk0Actions, 0xD5 },
    { actor18Talk1Conditions, NULL, 0xDA },
    { actor18Talk2Conditions, actor18Talk2Actions, 0xDB },
    { actor18Talk3Conditions, actor18Talk3Actions, 0xDC },
    { actor18Talk4Conditions, NULL, 0xDD },
    { actor18Talk5Conditions, NULL, 0xDE },
    { actor18Talk6Conditions, actor18Talk6Actions, 0xDF },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x11 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x3E },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x2F },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x2A },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x16 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x25 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x2B },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x35 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { actor31Talk0Conditions, actor31Talk0Actions, 0x30 },
    { actor31Talk1Conditions, NULL, 6 },
    { actor31Talk2Conditions, NULL, 0x2B },
    { actor31Talk3Conditions, NULL, 0x2B },
    { actor31Talk4Conditions, NULL, 0x2B },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x12 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x17 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x1C },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x21 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x26 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x3F },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, actor38Talk0Actions, 0x127 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x3A },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { actor41Talk0Conditions, NULL, 0xD8 },
    { actor41Talk1Conditions, NULL, 0xD8 },
    { actor41Talk2Conditions, actor41Talk2Actions, 0xD8 },
    { actor41Talk3Conditions, actor41Talk3Actions, 0xD8 },
    { actor41Talk4Conditions, NULL, 0xD8 },
    { actor41Talk5Conditions, NULL, 0xD8 },
    { actor41Talk6Conditions, actor41Talk6Actions, 0xD8 },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x38 },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x117 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { actor45Talk0Conditions, NULL, 0x23 },
    { actor45Talk1Conditions, NULL, 0xE },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0xC },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0xF },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x37 },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0x32 },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { NULL, NULL, 0x2D },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { NULL, NULL, 0x28 },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0x14 },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { NULL, NULL, 0x19 },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor55Talks[] = {
    { NULL, NULL, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor56Talks[] = {
    { NULL, NULL, 0xF },
    { NULL, NULL, 0 },
};
FieldTalk actor57Talks[] = {
    { NULL, NULL, 0x175 },
    { NULL, NULL, 0 },
};
FieldTalk actor58Talks[] = {
    { actor58Talk0Conditions, actor58Talk0Actions, 0x16A },
    { actor58Talk1Conditions, NULL, 0x16B },
    { actor58Talk2Conditions, NULL, 0x16D },
    { NULL, NULL, 0 },
};
FieldTalk actor59Talks[] = {
    { NULL, NULL, 0xD },
    { NULL, NULL, 0 },
};
FieldTalk actor60Talks[] = {
    { NULL, NULL, 0x13 },
    { NULL, NULL, 0 },
};
FieldTalk actor61Talks[] = {
    { NULL, NULL, 0x40 },
    { NULL, NULL, 0 },
};
FieldTalk actor62Talks[] = {
    { NULL, NULL, 0x3B },
    { NULL, NULL, 0 },
};
FieldTalk actor63Talks[] = {
    { NULL, NULL, 0x36 },
    { NULL, NULL, 0 },
};
FieldTalk actor64Talks[] = {
    { NULL, NULL, 0x31 },
    { NULL, NULL, 0 },
};
FieldTalk actor65Talks[] = {
    { NULL, NULL, 0x18 },
    { NULL, NULL, 0 },
};
FieldTalk actor66Talks[] = {
    { NULL, NULL, 0x1D },
    { NULL, NULL, 0 },
};
FieldTalk actor67Talks[] = {
    { NULL, NULL, 0x22 },
    { NULL, NULL, 0 },
};
FieldTalk actor68Talks[] = {
    { NULL, NULL, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor69Talks[] = {
    { NULL, NULL, 0x27 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x22), 1, PROGRESS(4), 0, ITEM(0, 0x22), 0, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x22), 1, PROGRESS(4), 0, ITEM(0, 0x22), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x22), 0, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x22), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor14Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(9), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor15Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(4), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x26), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor17Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor18Conditions[] = { FLAG(0, 0x11), 0, SPECIAL(3), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor31Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor36Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(4), 1, FLAG(0x1C, 0x11), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor41Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor42Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor43Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor44Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor45Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor46Conditions[] = { PROGRESS(4), 1, FLAG(0x1C, 0x11), 1, CODES_END };
u16 actor47Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, CODES_END };
u16 actor48Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor49Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor50Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor51Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor52Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor53Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor54Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor55Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor56Conditions[] = { PROGRESS(8), 1, FLAG(0x1A, 0x17), 0, CODES_END };
u16 actor57Conditions[] = { PROGRESS(8), 1, FLAG(0x1A, 0x17), 1, CODES_END };
u16 actor58Conditions[] = { FLAG(0x1C, 0x46), 1, PROGRESS(6), 1, CODES_END };
u16 actor59Conditions[] = { PROGRESS(4), 1, FLAG(0x1C, 0x11), 1, CODES_END };
u16 actor60Conditions[] = { PROGRESS(6), 0, SPECIAL(0x15), 1, PROGRESS(8), 0, CODES_END };
u16 actor61Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor62Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor63Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor64Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor65Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor66Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor67Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor68Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor69Conditions[] = { SPECIAL(0x17), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2B, 4, 1089, 402, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2B, 4, 1089, 402, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2B, 4, 1089, 402, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2B, 4, 1089, 402, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2E, 5, 882, 585, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2F, 6, 449, 481, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2F, 6, 449, 481, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2F, 6, 449, 481, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2F, 6, 449, 481, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x2F, 6, 449, 481, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x32, 7, 552, 459, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x32, 7, 914, 344, 7 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x3C, 8, 352, 234, 1 };
FieldActorEntry actor39 = { actor39Conditions, NULL, 0x67, 9, 0, 0, 0 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x9D, 0xA, 352, 234, 1 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x9E, 0xB, 449, 481, 1 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x9F, 0xC, 914, 344, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0xA0, 0xD, 882, 585, 3 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0xA1, 0xE, 1089, 402, 1 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor55 = { actor55Conditions, actor55Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor56 = { actor56Conditions, actor56Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor57 = { actor57Conditions, actor57Talks, 0x16D, 0xF, 632, 406, 1 };
FieldActorEntry actor58 = { actor58Conditions, actor58Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor59 = { actor59Conditions, actor59Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor60 = { actor60Conditions, actor60Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor61 = { actor61Conditions, actor61Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor62 = { actor62Conditions, actor62Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor63 = { actor63Conditions, actor63Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor64 = { actor64Conditions, actor64Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor65 = { actor65Conditions, actor65Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor66 = { actor66Conditions, actor66Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor67 = { actor67Conditions, actor67Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor68 = { actor68Conditions, actor68Talks, 0x171, 0x10, 833, 257, 5 };
FieldActorEntry actor69 = { actor69Conditions, actor69Talks, 0x171, 0x10, 833, 257, 5 };
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
    &actor25,
    &actor26,
    &actor27,
    &actor28,
    &actor29,
    &actor30,
    &actor31,
    &actor32,
    &actor33,
    &actor34,
    &actor35,
    &actor36,
    &actor37,
    &actor38,
    &actor39,
    &actor40,
    &actor41,
    &actor42,
    &actor43,
    &actor44,
    &actor45,
    &actor46,
    &actor47,
    &actor48,
    &actor49,
    &actor50,
    &actor51,
    &actor52,
    &actor53,
    &actor54,
    &actor55,
    &actor56,
    &actor57,
    &actor58,
    &actor59,
    &actor60,
    &actor61,
    &actor62,
    &actor63,
    &actor64,
    &actor65,
    &actor66,
    &actor67,
    &actor68,
    &actor69,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 441, 290, 0, 0 },
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 817, 114, 0, 0 },
    { 1, 0, 0x4A, 2, 0, 1, 0, 3, 6, 0, 949, 528, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 366, 274, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 446, 241, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 7, 4, 0, 1129, 333, 0, 0 },
    { 1, 0, 0x80, 2, 0x24, 0, 0, 0, 0, 0, 469, 191, 0, 0 },
    { 1, 0, 0x80, 2, 0x25, 0, 0, 0, 0, 0, 256, 161, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 411, 424, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 538, 379, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 568, 341, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 607, 347, 0, 0 },
    { 1, 0, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 976, 215, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 544, 363, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 579, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 884, 243, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 913, 244, 0, 0 },
    { 1, 0, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 1020, 218, 0, 0 },
    { 1, 0, 0x40, 6, 0x10, 1, 0x10, 0x12, 4, 0, 906, 526, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x1B, 0, 0, 0, 0, 0, 721, 200, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x1C, 0, 0, 0, 0, 0, 451, 367, 0, 0 },
    { 1, 0, 0x40, 4, 8, 1, 8, 0xB, 4, 0, 448, 405, 445, 0 },
    { 1, 0, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 465, 438, 464, 0 },
    { 1, 0, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 737, 256, 281, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 898, 552, 576, 0 },
    { 1, 0, 0x40, 4, 0x19, 0, 0, 0, 0, 0, 727, 260, 280, 0 },
    { 1, 0, 0x40, 4, 0x1A, 0, 0, 0, 0, 0, 447, 417, 444, 0 },
    { 1, 0, 0x64, 4, 0x1D, 0, 0, 0, 0, 0, 784, 496, 560, 0 },
    { 1, 0, 0x64, 4, 0x1E, 0, 0, 0, 0, 0, 768, 488, 552, 0 },
    { 1, 0, 0x64, 4, 0x1F, 0, 0, 0, 0, 0, 752, 480, 544, 0 },
    { 1, 0, 0x64, 4, 0x20, 0, 0, 0, 0, 0, 736, 472, 536, 0 },
    { 1, 0, 0x64, 4, 0x21, 0, 0, 0, 0, 0, 720, 464, 528, 0 },
    { 1, 0, 0x64, 4, 0x22, 0, 0, 0, 0, 0, 704, 456, 520, 0 },
    { 1, 0, 0x64, 4, 0x23, 0, 0, 0, 0, 0, 672, 456, 496, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x229, 0x448, 0xF8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x230, 0xC8, 0x1A4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x230, 0x1B8, 0x204, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22F, 0x208, 0x1AC, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x231, 0x1A7, 0x254, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x400, 0x120, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x410, 0x188, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 70, script70, EVENT_TEXT(2), NULL, endEvent70 },
    { 71, script71, EVENT_TEXT(3), NULL, endEvent71 },
    { -1, NULL, 0, NULL, NULL },
};
