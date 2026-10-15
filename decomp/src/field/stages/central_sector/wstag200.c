/*
 * WSTAG200: Asuka City, Central Sector: the streets by the bridge. Exits to
 * Asuka Bridge (WSTAG205), the Main Lobby (210), the Digimon Lab (220), the
 * Inn (235), Smith's Shop (245) and Lamb Chop (255).
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of progress 3, which depends on flag 0x400D */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 3) {
            children[0] = FIELDSTG_startEvent(FLAGS_00.checkCondition(FLAG(0x40, 0xD), 0) ? 0x32 : 0x34);
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
 * After event 50, with Tamer Genji: starts the stage's event battle 1 and
 * keeps it from playing again
 */
void endEvent50(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xD), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(1), 1);
}

/* After event 52, with Tamer Genji: moves the story on to its step 4 */
void endEvent52(void) {
    GAME.progress = 4;
}

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xCD)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x189
#define STAGE_ARCHIVE 0x30F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xC5)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x197
#define STAGE_ARCHIVE 0x31E
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing ASKA_BGM;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x3E800, 0xEC00};
    FIELDSTG_state.startDir = 1;
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 4;
    FIELDSTG_state.music = MUSIC(4, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
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

s16 script50[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x418, 0xD4),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_PLACE(0x39, 0x448, 0x11E),
    SCRIPT_POSE(0x39, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_WALK(1, 0x3E8, 0xEC, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x325, 0x39),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x39),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x39, 0x408, 0xFE, 3),
    SCRIPT_WAIT_WALK(0x39),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0x39, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script52[] = {
    SCRIPT_PLACE(1, 0x3E8, 0xEC),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_PLACE(0x39, 0x408, 0xFE),
    SCRIPT_POSE(0x39, 1, 3),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 1, 0),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 1, 0),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 1, 0),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 1, 0),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x39, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x206, 0x70, 0xF0, 7),
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
AreaBattle area3Battle0 = { 188, 18, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 200, 18, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 272, 20, MUSIC(3, 0) };
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
    { 131, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F6, 0x17C, 0x2D8, 0x7C, 0x170, 0x1F2 },
    { 0x1C0, 0x100, 0x1C0, 0x185, 0x200, 0x85, 0x170, 0x1F1 },
    { 0x1C0, 0x100, 0x1F6, 0x154, 0x2D8, 0x54, 0x170, 0x1F0 },
    { 0x1C0, 0x100, 0x1D4, 0x185, 0x250, 0x85, 0x170, 0x1EF },
    { 0x1C0, 0x100, 0x1DC, 0x185, 0x270, 0x85, 0x170, 0x1EE },
    { 0x1C0, 0x100, 0x1E4, 0x18C, 0x290, 0x8C, 0x170, 0x1ED },
    { 0x1C0, 0x100, 0x1C8, 0x192, 0x220, 0x92, 0x170, 0x1EC },
    { 0x1C0, 0x100, 0x1EC, 0x19C, 0x2B0, 0x9C, 0x170, 0x1EB },
    { 0x1C0, 0x100, 0x1F4, 0x19C, 0x2D0, 0x9C, 0x170, 0x1EA },
    { 0x1C0, 0x100, 0x1C0, 0x1A5, 0x200, 0xA5, 0x170, 0x1E9 },
};
u16 actor11Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor23Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor23Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor23Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor23Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor23Talk2Actions[] = { CARD_BATTLE(0, 0), 1, CODES_END };
u16 actor23Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 0,
    CODES_END,
};
u16 actor23Talk3Actions[] = { EVENT_BATTLE(2), 1, FLAG(0xE, 0), 1, CODES_END };
u16 actor23Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor23Talk5Conditions[] = {
    ITEM(0, 0x12), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    PARTY_STAT(4), 0,
    CODES_END,
};
u16 actor23Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 1,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor23Talk6Actions[] = { CARD_BATTLE(0, 1), 1, CODES_END };
u16 actor24Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor24Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor24Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor24Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor24Talk2Actions[] = { CARD_BATTLE(0, 0), 1, CODES_END };
u16 actor24Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 0,
    CODES_END,
};
u16 actor24Talk3Actions[] = { EVENT_BATTLE(2), 1, FLAG(0xE, 0), 1, CODES_END };
u16 actor24Talk4Conditions[] = {
    ITEM(0, 0x12), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 1,
    PARTY_STAT(2), 1,
    CODES_END,
};
u16 actor24Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    CODES_END,
};
u16 actor24Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor24Talk6Actions[] = { CARD_BATTLE(0, 1), 1, CODES_END };
u16 actor26Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor26Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor26Talk1Conditions[] = { PARTY_STAT(0), 0, FLAG(0, 0), 1, CODES_END };
u16 actor26Talk2Conditions[] = { PARTY_STAT(0), 1, FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor26Talk2Actions[] = { CARD_BATTLE(0, 0), 1, CODES_END };
u16 actor26Talk3Conditions[] = {
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    CODES_END,
};
u16 actor26Talk3Actions[] = { FLAG(0xE, 0), 1, EVENT_BATTLE(2), 1, CODES_END };
u16 actor26Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor26Talk5Conditions[] = {
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 1,
    PARTY_STAT(4), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor26Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 1,
    CODES_END,
};
u16 actor26Talk6Actions[] = { CARD_BATTLE(0, 1), 1, CODES_END };
u16 actor27Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor27Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor27Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor27Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor27Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor29Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor29Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor29Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, PARTY_STAT(0), 1, CODES_END };
u16 actor29Talk2Actions[] = { CARD_BATTLE(0, 0), 1, CODES_END };
u16 actor29Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 0), 0,
    CODES_END,
};
u16 actor29Talk3Actions[] = { FLAG(0xE, 0), 1, CODES_END };
u16 actor29Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    CODES_END,
};
u16 actor29Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0), 1,
    CODES_END,
};
u16 actor29Talk6Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0xE, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor29Talk6Actions[] = { CARD_BATTLE(0, 1), 1, CODES_END };
u16 actor32Talk0Conditions[] = { FLAG(0x1A, 0xC), 0, CODES_END };
u16 actor32Talk1Conditions[] = { FLAG(0x1A, 0xC), 1, CODES_END };
u16 actor50Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor50Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x13 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x26F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x270 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x271 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x26E },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x272 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x4A3 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x273 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x274 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x275 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x26E },
    { actor11Talk1Conditions, NULL, 0x4A3 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xF },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x299 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x29A },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x29B },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x29C },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x29D },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0x299 },
    { actor18Talk1Conditions, NULL, 0x4A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x29E },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x29F },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x2A0 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x4A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { actor23Talk0Conditions, actor23Talk0Actions, 0x69 },
    { actor23Talk1Conditions, NULL, 0x6D },
    { actor23Talk2Conditions, actor23Talk2Actions, 0x6E },
    { actor23Talk3Conditions, actor23Talk3Actions, 0x6F },
    { actor23Talk4Conditions, NULL, 0x81 },
    { actor23Talk5Conditions, NULL, 0x82 },
    { actor23Talk6Conditions, actor23Talk6Actions, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { actor24Talk0Conditions, actor24Talk0Actions, 0x68 },
    { actor24Talk1Conditions, NULL, 0x6D },
    { actor24Talk2Conditions, actor24Talk2Actions, 0x6E },
    { actor24Talk3Conditions, actor24Talk3Actions, 0x6F },
    { actor24Talk4Conditions, NULL, 0x81 },
    { actor24Talk5Conditions, NULL, 0x82 },
    { actor24Talk6Conditions, actor24Talk6Actions, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { actor26Talk0Conditions, actor26Talk0Actions, 0x6A },
    { actor26Talk1Conditions, NULL, 0x6D },
    { actor26Talk2Conditions, actor26Talk2Actions, 0x6E },
    { actor26Talk3Conditions, actor26Talk3Actions, 0x6F },
    { actor26Talk4Conditions, NULL, 0x81 },
    { actor26Talk5Conditions, NULL, 0x82 },
    { actor26Talk6Conditions, actor26Talk6Actions, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { actor27Talk0Conditions, NULL, 0x68 },
    { actor27Talk1Conditions, actor27Talk1Actions, 0x7F },
    { actor27Talk2Conditions, actor27Talk2Actions, 0x80 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x41A },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { actor29Talk0Conditions, NULL, 0x6B },
    { actor29Talk1Conditions, NULL, 0x6D },
    { actor29Talk2Conditions, actor29Talk2Actions, 0x6E },
    { actor29Talk3Conditions, actor29Talk3Actions, 0x6F },
    { actor29Talk4Conditions, NULL, 0x81 },
    { actor29Talk5Conditions, NULL, 0x82 },
    { actor29Talk6Conditions, actor29Talk6Actions, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x2A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x276 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { actor32Talk0Conditions, NULL, 0x4D },
    { actor32Talk1Conditions, NULL, 0x4E },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x15 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x25A },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x25B },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x25C },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x25D },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x25E },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x263 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x25F },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x260 },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x261 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x262 },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x14 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { NULL, NULL, 0x265 },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0x266 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0x267 },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x264 },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0x268 },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { actor50Talk0Conditions, NULL, 0x264 },
    { actor50Talk1Conditions, NULL, 0x4A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { NULL, NULL, 0x269 },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0x26A },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { NULL, NULL, 0x26B },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0x26C },
    { NULL, NULL, 0 },
};
FieldTalk actor55Talks[] = {
    { NULL, NULL, 0x4A4 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(3), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(4), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor24Conditions[] = { SPECIAL(3), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor25Conditions[] = { PROGRESS(3), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x26), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor27Conditions[] = { FLAG(0, 0x11), 1, SPECIAL(9), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 0, ITEM(0, 0x192), 0, SPECIAL(9), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor31Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0x15), 0, CODES_END };
u16 actor33Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor41Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor43Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor44Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor45Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor46Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor47Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor48Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor49Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor50Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor51Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor52Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor53Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor54Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor55Conditions[] = { PROGRESS(9), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 5, 564, 558, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x2F, 6, 555, 474, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x39, 7, 1088, 288, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x39, 7, 1088, 288, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0x39, 7, 1096, 286, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x39, 7, 1088, 288, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x39, 7, 1088, 288, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x39, 7, 1088, 288, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x9D, 8, 1088, 288, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x9E, 9, 555, 474, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x9F, 0xA, 564, 558, 5 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0xB2, 0xB, 305, 616, 1 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x16D, 0xC, 464, 640, 1 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x16D, 0xC, 719, 235, 7 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0x173, 0xD, 595, 542, 1 };
FieldActorEntry actor55 = { actor55Conditions, actor55Talks, 0x173, 0xD, 595, 542, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 1048, 211, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 3, 6, 0, 437, 437, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 3, 6, 0, 533, 389, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 3, 6, 0, 1231, 344, 0, 0 },
    { 1, 0, 0x40, 2, 0x4A, 2, 0, 3, 6, 0, 623, 331, 0, 0 },
    { 1, 0, 0x40, 2, 0x10, 0, 0, 0, 0, 0, 268, 384, 0, 0 },
    { 1, 0, 0x48, 2, 0x11, 0, 0, 0, 0, 0, 274, 440, 0, 0 },
    { 1, 0, 0x40, 2, 0x12, 0, 0, 0, 0, 0, 1044, 416, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 1088, 384, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 1, 0xA, 0xF, 8, 0, 123, 518, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 1, 0xA, 0xF, 8, 0, 355, 626, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 430, 722, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 501, 687, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 945, 160, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 1096, 700, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 1142, 723, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 483, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 597, 695, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 642, 672, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 754, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x49, 2, 0, 3, 6, 0, 1023, 463, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 3, 6, 0, 669, 308, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 379, 807, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 503, 863, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 888, 683, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 940, 652, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 982, 613, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1028, 842, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1049, 728, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1205, 438, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1250, 795, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 252, 805, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 962, 641, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 997, 616, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1013, 582, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1055, 570, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1111, 806, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1117, 352, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1168, 791, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 207, 510, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 277, 265, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 375, 819, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 473, 878, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 539, 844, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 903, 771, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 982, 769, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1027, 745, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1054, 560, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1066, 878, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1075, 817, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1151, 788, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1197, 422, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 99, 649, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 340, 734, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 372, 865, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 562, 848, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 866, 606, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 929, 659, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1025, 722, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1032, 734, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1058, 835, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1240, 804, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 210, 645, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 455, 793, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 845, 597, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 915, 647, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1005, 702, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1183, 406, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 225, 814, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 235, 656, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 322, 260, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 353, 809, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 399, 780, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 425, 874, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 887, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1004, 681, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1014, 762, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1059, 824, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1076, 720, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1077, 565, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1161, 394, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1217, 795, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 43, 657, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 124, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 182, 805, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 192, 797, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 212, 468, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 217, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 298, 247, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 396, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 656, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 678, 794, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 835, 638, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 850, 713, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 852, 699, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 866, 656, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 867, 705, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 876, 648, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 895, 628, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 925, 783, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 955, 784, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1012, 665, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1027, 656, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1040, 832, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1051, 411, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1061, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1064, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1075, 725, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1119, 793, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1279, 782, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x18, 0, 0, 0, 0, 0, 1045, 458, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x17, 0, 0, 0, 0, 0, 638, 321, 0, 0 },
    { 1, 0x66, 0x40, 6, 0x14, 0, 0, 0, 0, 0, 277, 489, 0, 0 },
    { 1, 0x67, 0x40, 6, 0x15, 0, 0, 0, 0, 0, 446, 416, 0, 0 },
    { 1, 0x68, 0x40, 6, 0x16, 0, 0, 0, 0, 0, 542, 367, 0, 0 },
    { 1, 0, 0x68, 6, 6, 1, 6, 8, 4, 0, 879, 428, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 6, 0, 241, 396, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 6, 0, 226, 450, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x53, 1, 0x53, 0x56, 6, 0, 1091, 316, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x57, 1, 0x57, 0x5A, 6, 0, 1103, 338, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 303, 495, 544, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 409, 416, 472, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 510, 375, 423, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 609, 327, 375, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1051, 192, 246, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 1073, 463, 512, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { SPECIAL(7), 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x202, 0x308, 0xE0, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20F, 0xE0, 0x14E, 5, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20D, 0x178, 0x19C, 3, 0x67, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20D, 0x218, 0x14C, 3, 0x68, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20A, 0x116, 0xDA, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x203, 0x60, 0x190, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x206, 0xD8, 0x178, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 50, script50, EVENT_TEXT(0xB), NULL, endEvent50 },
    { 52, script52, EVENT_TEXT(0xC), NULL, endEvent52 },
    { -1, NULL, 0, NULL, NULL },
};
