/*
 * WSTAG486: Phoenix Bay, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG485.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* After event 745, with Trooper: sets flag 0x40A8 */
void endEvent745(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xA8), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x5AE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x5BE
#endif
/* Sets the stage up: its map, actors, battles and events, playing the ambience ENV_0014 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x19500, 0xD000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x36;
    FIELDSTG_state.music = MUSIC(0x36, 0);
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

s16 script745[] = {
    SCRIPT_WALK(2, 0x501, 0xD9, 7),
    SCRIPT_PLACE(0x132, 0x521, 0xE9),
    SCRIPT_POSE(0x132, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_COMMAND(0x323, 0x325, 0x132),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x132),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x132, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x132, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x132, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x132, 0x560, 0x109, 7),
    SCRIPT_WAIT_WALK(0x132),
    SCRIPT_POSE(0x132, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x132, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x548, 0xFC, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x588, 0xDC, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 109, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 109, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 109, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 176, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 176, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 176, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 157, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 157, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 331, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 332, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 177, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 106, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 75, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x170, 0xE0, 0x70, 0x170, 0x1FF },
    { 0x140, 0x100, 0x164, 0x1B0, 0x90, 0xB0, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16C, 0x1B0, 0xB0, 0xB0, 0x170, 0x1FE },
    { 0x140, 0x100, 0x174, 0x1B0, 0xD0, 0xB0, 0x160, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x156, 0x1C0, 0x58, 0xC0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x140, 0x1CB, 0, 0xCB, 0x160, 0x1FC },
    { 0x140, 0x100, 0x176, 0x1D8, 0xD8, 0xD8, 0x170, 0x1FC },
    { 0x180, 0x100, 0x194, 0x100, 0x150, 0, 0x150, 0x1FB },
    { 0x140, 0x100, 0x148, 0x1CB, 0x20, 0xCB, 0x160, 0x1FB },
};
u16 actor14Talk0Conditions[] = { FLAG(0x1A, 0x35), 0, CODES_END };
u16 actor14Talk0Actions[] = { START_EVENT(0x4A), 1, FLAG(0x1A, 0x35), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0x1A, 0x35), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x102 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x101 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xFC },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xFA },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x327 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x326 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x324 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x32A },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xFB },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xFD },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xFE },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x100 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x324 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x325 },
    { actor14Talk1Conditions, NULL, 0x326 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x1C), 0, SPECIAL(0x1F), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x1C), 1, FLAG(0x1A, 0x35), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1F), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1A, 0x35), 0, PROGRESS(0x1C), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 944, 185, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x31, 5, 1394, 778, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x33, 6, 1394, 778, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x34, 7, 944, 185, 1 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x3F, 8, 529, 249, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 9, 1376, 265, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x45, 9, 1376, 265, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x46, 0xA, 1344, 208, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x46, 0xA, 1344, 208, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 0xB, 944, 185, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 0xB, 944, 185, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 0xC, 1394, 778, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 0xC, 1394, 778, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x132, 0xD, 1313, 233, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x132, 0xD, 1313, 233, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x4B, 2, 0, 3, 6, 0, 1729, 477, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1400, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1448, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1456, 145, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1496, 69, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1504, 121, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1544, 45, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1552, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1592, 21, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1600, 73, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1640, -3, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1648, 49, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1688, -27, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1696, 25, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1744, 1, 0, 0 },
    { 1, 0, 0x40, 2, 0x4E, 2, 0, 5, 8, 0, 1792, -23, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 308, 158, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 924, 626, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 1732, 319, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 1620, 640, 0, 0 },
    { 1, 0, 0x4E, 2, 0xA, 0, 0, 0, 0, 0, 1664, 384, 0, 0 },
    { 1, 0, 0x50, 2, 2, 0, 0, 0, 0, 0, 1536, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 3, 6, 0, 1083, 205, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 3, 6, 0, 1107, 467, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 3, 6, 0, 1648, 565, 0, 0 },
    { 1, 0, 0x40, 6, 0x4C, 2, 0, 3, 6, 0, 1800, 729, 0, 0 },
    { 1, 0, 0x40, 6, 0x4D, 2, 0, 3, 6, 0, 1255, 43, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 460, 369, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 914, 808, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 935, 499, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 962, 255, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1379, 661, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1508, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 661, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 683, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 941, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1136, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1322, 331, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 516, 208, 247, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 326, 181, 289, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 400, 175, 175, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 448, 159, 159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 496, 183, 183, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 544, 207, 207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 640, 191, 191, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 703, 183, 183, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 816, 159, 159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 912, 151, 151, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 976, 599, 599, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1056, 143, 143, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 143, 143, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1456, 711, 711, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1488, 743, 743, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1503, 687, 687, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1536, 719, 719, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A9, 0x48E, 0x3F8, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AB, 0xA0, 0x390, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A7, 0x3E8, 0x9C, 7, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 0xE, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFD0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x20, 0x20, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x30, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 0xE, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 745, script745, EVENT_TEXT(0x2C), NULL, endEvent745 },
    { -1, NULL, 0, NULL, NULL },
};
