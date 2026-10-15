/*
 * WSTAG651: Mirage Hall, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG650.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1462, with Guardromon: applies 0x7C19, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1462(void) {
    FLAGS_00.applyAction(0x7C19, 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x61F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x62F
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0020 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1CB00, 0x21100};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x16;
    FIELDSTG_state.music = MUSIC(0x16, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1462[] = {
    SCRIPT_WALK(2, 0xD7, 0x129, 3),
    SCRIPT_PLACE(0x15, 0xB7, 0x119),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC17, 0, 0, 0),
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
AreaBattle area3Battle0 = { 240, 20, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 241, 20, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 288, 18, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 289, 18, MUSIC(3, 0) };
AreaBattle area3Battle4 = { 242, 18, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 290, 18, MUSIC(3, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 161, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x164, 0x1A7, 0x90, 0xA7, 0x160, 0x1FF },
    { 0x140, 0x100, 0x150, 0x178, 0x40, 0x78, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x1A5, 0xB0, 0xA5, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x18B, 0, 0x8B, 0x170, 0x1FE },
    { 0x140, 0x100, 0x156, 0x1C8, 0x58, 0xC8, 0x160, 0x1FD },
    { 0x140, 0x100, 0x150, 0x1A8, 0x40, 0xA8, 0x170, 0x1FD },
    { 0x140, 0x100, 0x148, 0x18B, 0x20, 0x8B, 0x160, 0x1FC },
    { 0x140, 0x100, 0x176, 0x1A5, 0xD8, 0xA5, 0x170, 0x1FC },
    { 0x140, 0x100, 0x15C, 0x1A7, 0x70, 0xA7, 0x160, 0x1FB },
    { 0x140, 0x100, 0x148, 0x1B3, 0x20, 0xB3, 0x170, 0x1FB },
    { 0x140, 0x100, 0x140, 0x1BB, 0, 0xBB, 0x160, 0x1FA },
    { 0x140, 0x100, 0x16C, 0x1C5, 0xB0, 0xC5, 0x170, 0x1FA },
    { 0x140, 0x100, 0x164, 0x1C7, 0x90, 0xC7, 0x160, 0x1F9 },
};
u16 actor0Talk0Actions[] = { 0x7A2D, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x68), 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7A19, 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk4Conditions[] = { FLAG(0, 2), 0, FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk4Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor4Talk5Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 2), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk5Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor4Talk6Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(7), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk7Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk7Actions[] = { CARD_BATTLE(0x3A, 0), 1, CODES_END };
u16 actor4Talk8Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(7), 1,
    FLAG(0xE, 0x2D), 0,
    PARTY_STAT(9), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk8Actions[] = { FLAG(0xE, 0x2D), 1, EVENT_BATTLE(4), 1, CODES_END };
u16 actor4Talk9Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 0,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2D), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk10Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2D), 1,
    PARTY_STAT(0xB), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk11Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2D), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk11Actions[] = { CARD_BATTLE(0x3A, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor5Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor7Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor7Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor9Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor10Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor10Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk4Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor10Talk4Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor10Talk5Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor10Talk5Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor10Talk6Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk7Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk7Actions[] = { CARD_BATTLE(0x38, 0), 1, CODES_END };
u16 actor10Talk8Conditions[] = {
    FLAG(0xE, 0x2B), 0,
    PARTY_STAT(9), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk8Actions[] = { FLAG(0xE, 0x2B), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor10Talk9Conditions[] = {
    ITEM(0, 0x14), 0,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2B), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk10Conditions[] = {
    FLAG(0xE, 0x2B), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    PARTY_STAT(0xB), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk11Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2B), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk11Actions[] = { CARD_BATTLE(0x38, 1), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor12Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor12Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor12Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk4Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor12Talk4Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor12Talk5Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor12Talk5Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor12Talk6Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(7), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk7Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor12Talk7Actions[] = { CARD_BATTLE(0x39, 0), 1, CODES_END };
u16 actor12Talk8Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(7), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2C), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor12Talk8Actions[] = { FLAG(0xE, 0x2C), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor12Talk9Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 0,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2C), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor12Talk10Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2C), 1,
    PARTY_STAT(0xB), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor12Talk11Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(7), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x2C), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor12Talk11Actions[] = { CARD_BATTLE(0x39, 1), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor13Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor13Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor14Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2CE },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x2D0 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x151 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x14F },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x150 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x150 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x21 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x14F },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x20 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x148 },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x1F },
    { actor4Talk6Conditions, NULL, 0x149 },
    { actor4Talk7Conditions, actor4Talk7Actions, 0x14A },
    { actor4Talk8Conditions, actor4Talk8Actions, 0x14C },
    { actor4Talk9Conditions, NULL, 0x14B },
    { actor4Talk10Conditions, NULL, 0x14D },
    { actor4Talk11Conditions, actor4Talk11Actions, 0x14E },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x151 },
    { actor5Talk1Conditions, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x147 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x143 },
    { actor7Talk1Conditions, NULL, 0x144 },
    { actor7Talk2Conditions, NULL, 0x145 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x134 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x132 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x133 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x134 },
    { actor9Talk1Conditions, NULL, 0x17 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x133 },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x19 },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x132 },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x18 },
    { actor10Talk4Conditions, actor10Talk4Actions, 0x12A },
    { actor10Talk5Conditions, actor10Talk5Actions, 0x17 },
    { actor10Talk6Conditions, NULL, 0x12C },
    { actor10Talk7Conditions, actor10Talk7Actions, 0x12D },
    { actor10Talk8Conditions, actor10Talk8Actions, 0x12F },
    { actor10Talk9Conditions, NULL, 0x12E },
    { actor10Talk10Conditions, NULL, 0x130 },
    { actor10Talk11Conditions, actor10Talk11Actions, 0x131 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x146 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, actor12Talk0Actions, 0x13E },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x1D },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x13D },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x1C },
    { actor12Talk4Conditions, actor12Talk4Actions, 0x135 },
    { actor12Talk5Conditions, actor12Talk5Actions, 0x1B },
    { actor12Talk6Conditions, NULL, 0x137 },
    { actor12Talk7Conditions, actor12Talk7Actions, 0x138 },
    { actor12Talk8Conditions, actor12Talk8Actions, 0x139 },
    { actor12Talk9Conditions, NULL, 0x13A },
    { actor12Talk10Conditions, NULL, 0x13B },
    { actor12Talk11Conditions, actor12Talk11Actions, 0x13C },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x13F },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x13D },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x13E },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x135 },
    { actor14Talk1Conditions, NULL, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x141 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x140 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x142 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x2DC },
    { NULL, NULL, 0 },
};
u16 actor3Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor8Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor9Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor10Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor13Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor14Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 321, 593, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 183, 281, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x17, 6, 528, 305, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x25, 7, 239, 177, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x25, 7, 239, 177, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x25, 7, 239, 177, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2D, 8, 537, 493, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2E, 9, 537, 493, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2F, 0xA, 352, 440, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2F, 0xA, 352, 440, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2F, 0xA, 352, 440, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x33, 0xB, 209, 304, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x37, 0xC, 401, 528, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x37, 0xC, 401, 528, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x37, 0xC, 401, 528, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9D, 0xD, 401, 528, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9E, 0xE, 352, 440, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9F, 0xF, 537, 493, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0xA0, 0x10, 239, 177, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 0xE, 0, 252, 269, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 0xE, 0, 482, 625, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 195, 353, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 339, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 539, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 7, 0xE, 0, 366, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 7, 0xE, 0, 475, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 2, 0, 0xB, 0xA, 0, 164, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 0xB, 0xA, 0, 164, 354, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 0xB, 0xA, 0, 508, 337, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 0xB, 0xA, 0, 512, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 2, 0, 0xB, 0xA, 0, 568, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 2, 0, 0xB, 0xA, 0, 481, 426, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 0xB, 0xA, 0, 453, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 0xB, 0xA, 0, 346, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 2, 0, 0xB, 0xA, 0, 342, 488, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 311, 96, 151, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 318, 579, 600, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 335, 572, 595, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C5, 0xA8, 0x1B4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C3, 0x298, 0x104, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xFD, 0xD0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xEC, 0x118, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 8, 0x133, 0xC8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 8, 0x142, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x192, 0x148, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1A2, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1C2, 0x1A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1D2, 0x1E8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x25C, 0x160, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x24C, 0x1A8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1AC, 0x228, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x19C, 0x270, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 9, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1462, script1462, EVENT_TEXT(0x1F), NULL, endEvent1462 },
    { -1, NULL, 0, NULL, NULL },
};
