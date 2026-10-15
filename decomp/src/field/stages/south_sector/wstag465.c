/*
 * WSTAG465: Tranquil Swamp, South Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x1B5
#define STAGE_ARCHIVE 0x3D1
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x1C3
#define STAGE_ARCHIVE 0x3E1
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0012 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x37F00, 0x1F600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x34;
    FIELDSTG_state.music = MUSIC(0x34, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 53, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 53, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 53, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 147, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 147, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 147, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 54, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 54, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 213, 8, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 329, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 147, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 66, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 15, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x148, 0x1D4, 0x20, 0xD4, 0x170, 0x1FF },
    { 0x180, 0x100, 0x198, 0x100, 0x160, 0, 0x140, 0x1FE },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x150, 0x1FE },
    { 0x180, 0x100, 0x1A8, 0x100, 0x1A0, 0, 0x160, 0x1FE },
};
u16 actor14Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor14Talk2Actions[] = { CARD_BATTLE(0x17, 0), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor15Talk2Actions[] = { CARD_BATTLE(0x17, 0), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(0x17, 0), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0xD), 0, CODES_END };
u16 actor17Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xD), 1, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0xD), 1, PARTY_STAT(0xD), 0, CODES_END };
u16 actor17Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0xD), 1, PARTY_STAT(0xD), 1, CODES_END };
u16 actor17Talk3Actions[] = { CARD_BATTLE(0x17, 1), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor18Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor18Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor19Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor19Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk2Conditions[] = { FLAG(0, 0x10), 1, CARD(0x13), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor19Talk2Actions[] = {
    CARD(0x13), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x13), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor19Talk3Conditions[] = { FLAG(0, 0x10), 1, CARD(0x13), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor19Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor21Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor21Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor21Talk2Actions[] = { CARD_BATTLE(0x17, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x133 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x133 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x12D },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x12E },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x12F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x130 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x137 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x135 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x131 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x132 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x134 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0xBE },
    { actor14Talk1Conditions, NULL, 0xC2 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0xC3 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0xBF },
    { actor15Talk1Conditions, NULL, 0xC2 },
    { actor15Talk2Conditions, actor15Talk2Actions, 0xC3 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xC0 },
    { actor16Talk1Conditions, NULL, 0xC2 },
    { actor16Talk2Conditions, actor16Talk2Actions, 0xC3 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0xC4 },
    { actor17Talk1Conditions, actor17Talk1Actions, 0xC5 },
    { actor17Talk2Conditions, NULL, 0xC6 },
    { actor17Talk3Conditions, actor17Talk3Actions, 0xC7 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0xBE },
    { actor18Talk1Conditions, actor18Talk1Actions, 0xCA },
    { actor18Talk2Conditions, actor18Talk2Actions, 0xC9 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0xBE },
    { actor19Talk1Conditions, actor19Talk1Actions, 0xC8 },
    { actor19Talk2Conditions, actor19Talk2Actions, 0xCB },
    { actor19Talk3Conditions, actor19Talk3Actions, 0xC9 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x283 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0xC1 },
    { actor21Talk1Conditions, NULL, 0xC1 },
    { actor21Talk2Conditions, actor21Talk2Actions, 0xC1 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x136 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor14Conditions[] = {
    SPECIAL(3), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor15Conditions[] = {
    SPECIAL(4), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor16Conditions[] = {
    PROGRESS(0x26), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor17Conditions[] = {
    ITEM(0, 0x12), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor18Conditions[] = {
    SPECIAL(0xA), 1,
    FLAG(0, 0x11), 1,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor19Conditions[] = {
    ITEM(0, 0x12), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor20Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1A), 0, SPECIAL(9), 1, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x33, 4, 897, 514, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x3A, 5, 690, 251, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9D, 6, 690, 251, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x9E, 7, 897, 514, 5 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 1098, 580, 0, 0 },
    { 1, 0, 0x40, 2, 0x12, 0, 0, 0, 0, 0, 1107, 563, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 1187, 603, 0, 0 },
    { 1, 0, 0x40, 2, 0x14, 0, 0, 0, 0, 0, 545, 152, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 255, 371, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 476, 316, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 482, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 656, 359, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 815, 365, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 960, 347, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 978, 599, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 319, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 474, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 606, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 707, 423, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 784, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 999, 583, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1177, 467, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 373, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 712, 325, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 797, 287, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3A, 0x3D, 0xA, 0, 290, 429, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 415, 553, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 528, 419, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 702, 495, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1084, 593, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 211, 355, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 312, 442, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 436, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 504, 517, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 195, 355, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 290, 442, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 431, 551, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 544, 418, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 668, 475, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 747, 327, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 125, 305, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 128, 296, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 245, 93, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 252, 98, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 262, 91, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 299, 379, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 366, 425, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 392, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 416, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 507, 441, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 521, 46, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 526, 292, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 532, 49, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 562, 496, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 584, 131, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 599, 479, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 633, 525, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 642, 298, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 643, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 650, 514, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 651, 522, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 672, 157, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 718, 410, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 758, 167, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 812, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 820, 409, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 821, 417, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 835, 409, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 908, 370, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 971, 337, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1080, 569, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1268, 556, 0, 0 },
    { 1, 0, 0x40, 6, 0x15, 0, 0, 0, 0, 0, 185, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x16, 0, 0, 0, 0, 0, 233, 298, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 0, 0, 0, 0, 0, 281, 322, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 164, 311, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 192, 229, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 393, 331, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 511, 381, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 514, 482, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 541, 92, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 639, 447, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 660, 178, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 731, 383, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 767, 326, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 791, 457, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 843, 243, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 861, 362, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x4A, 0xA, 0, 1153, 592, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 139, 211, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 349, 379, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 467, 497, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 504, 336, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 567, 337, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 569, 438, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 577, 275, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 613, 264, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 650, 386, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 730, 179, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 735, 458, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 762, 229, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 776, 165, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 912, 408, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 919, 329, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1140, 529, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1234, 356, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 0xA, 0, 1336, 600, 0, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 1072, 384, 422, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 1104, 368, 412, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 1120, 368, 404, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 1136, 368, 396, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 1152, 352, 389, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 1168, 352, 381, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 400, 144, 191, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 384, 160, 197, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 446, 290, 318, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 766, 258, 287, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 486, 160, 189, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 1099, 491, 511, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 207, 286, 286, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 255, 310, 310, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 303, 334, 334, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 735, 581, 581, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 799, 581, 581, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 864, 470, 470, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 912, 446, 446, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 960, 422, 422, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x234, 0x340, 0x90, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x239, 0xA8, 0x132, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x238, 0x1F2, 0x164, 3, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x48, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x38, 0xFFF8, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
