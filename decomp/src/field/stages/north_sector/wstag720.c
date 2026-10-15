/*
 * WSTAG720: Legendary Gym, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x65E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x66E
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0022 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xDA00, 0x2E500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x18;
    FIELDSTG_state.music = MUSIC(0x18, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

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
AreaBattle area3Battle0 = { 222, 20, MUSIC(3, 0) };
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
    { 159, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1AA, 0x100, 0x1A8, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x172, 0x189, 0xC8, 0x89, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x139, 0xD8, 0x39, 0x160, 0x1FE },
    { 0x140, 0x100, 0x176, 0x161, 0xD8, 0x61, 0x170, 0x1FE },
    { 0x140, 0x100, 0x172, 0x1A9, 0xC8, 0xA9, 0x160, 0x1FD },
    { 0x140, 0x100, 0x172, 0x1C9, 0xC8, 0xC9, 0x170, 0x1FD },
    { 0x140, 0x100, 0x162, 0x1D2, 0x88, 0xD2, 0x160, 0x1FC },
    { 0x140, 0x100, 0x16A, 0x1D2, 0xA8, 0xD2, 0x170, 0x1FC },
    { 0x180, 0x100, 0x1A2, 0x100, 0x188, 0, 0x160, 0x1FB },
};
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(0x20, 0), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor6Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 0,
    CODES_END,
};
u16 actor6Talk4Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x16), 1, CODES_END };
u16 actor6Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor6Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor6Talk6Actions[] = { CARD_BATTLE(0x20, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor9Talk2Actions[] = { CARD_BATTLE(0x20, 0), 1, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor9Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 0,
    CODES_END,
};
u16 actor9Talk4Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x16), 1, CODES_END };
u16 actor9Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor9Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor9Talk6Actions[] = { CARD_BATTLE(0x20, 1), 1, CODES_END };
u16 actor10Talk0Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 0, CODES_END };
u16 actor10Talk0Actions[] = { 0x9405, 1, CODES_END };
u16 actor10Talk1Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor10Talk1Actions[] = { 0x9406, 1, CODES_END };
u16 actor11Talk0Actions[] = { 0x940D, 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor12Talk2Actions[] = { CARD_BATTLE(0x20, 0), 1, CODES_END };
u16 actor12Talk3Conditions[] = {
    PARTY_STAT(6), 1,
    FLAG(0, 0), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor12Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 0,
    CODES_END,
};
u16 actor12Talk4Actions[] = { FLAG(0xE, 0x16), 1, CODES_END };
u16 actor12Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor12Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x16), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor12Talk6Actions[] = { CARD_BATTLE(0x20, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x223 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x226 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x224 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x21F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x222 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x220 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x194 },
    { actor6Talk1Conditions, NULL, 0x198 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x199 },
    { actor6Talk3Conditions, NULL, 0x19A },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x19B },
    { actor6Talk5Conditions, NULL, 0x19C },
    { actor6Talk6Conditions, actor6Talk6Actions, 0x19D },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x299 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x194 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x19E },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x19F },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x195 },
    { actor9Talk1Conditions, NULL, 0x198 },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x199 },
    { actor9Talk3Conditions, NULL, 0x19A },
    { actor9Talk4Conditions, actor9Talk4Actions, 0x19B },
    { actor9Talk5Conditions, NULL, 0x19C },
    { actor9Talk6Conditions, actor9Talk6Actions, 0x19D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x30A },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x30A },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, actor11Talk0Actions, 0x30A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x196 },
    { actor12Talk1Conditions, NULL, 0x196 },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x196 },
    { actor12Talk3Conditions, NULL, 0x196 },
    { actor12Talk4Conditions, actor12Talk4Actions, 0x196 },
    { actor12Talk5Conditions, NULL, 0x196 },
    { actor12Talk6Conditions, actor12Talk6Actions, 0x196 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x225 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x221 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x22E },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x22D },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x22C },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x22B },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x227 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x22A },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x228 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x229 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(0, 0x11), 0, SPECIAL(4), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor8Conditions[] = { FLAG(0, 0x11), 1, SPECIAL(9), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actor11Conditions[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 537, 145, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 120, 504, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 4, 537, 145, 3 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 5, 119, 137, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 5, 433, 567, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 5, 119, 137, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x35, 6, 321, 217, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x35, 6, 321, 217, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x35, 6, 321, 217, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x35, 6, 321, 217, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x88, 7, 124, 700, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x88, 7, 124, 700, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9D, 8, 321, 217, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9E, 9, 537, 145, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9F, 0xA, 119, 137, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xB4, 0xB, 118, 135, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0xB4, 0xB, 369, 345, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0xB4, 0xB, 369, 345, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0xB4, 0xB, 369, 345, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0xB6, 0xC, 121, 505, 5 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0xB6, 0xC, 272, 90, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0xB6, 0xC, 121, 505, 5 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0xB6, 0xC, 121, 505, 5 };
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
    { 1, 0, 0x76, 2, 2, 1, 2, 7, 4, 0, 15, 51, 0, 0 },
    { 1, 0, 0x76, 2, 2, 1, 2, 7, 4, 0, 367, 644, 0, 0 },
    { 1, 0, 0x48, 2, 8, 0, 0, 0, 0, 0, 184, 716, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 106, 91, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 115, 78, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 120, 99, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 189, 348, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 192, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 209, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 317, 142, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 326, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 341, 120, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 361, 421, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 368, 442, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 408, 678, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 421, 702, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 457, 290, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 468, 270, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 502, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 509, 451, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 519, 66, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 522, 96, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 550, 31, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 633, 234, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 703, 228, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 4, 0, 714, 256, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 103, 97, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 115, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 122, 106, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 188, 402, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 192, 354, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 207, 370, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 314, 148, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 328, 171, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 342, 125, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 360, 427, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 371, 449, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 409, 685, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 419, 707, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 454, 296, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 471, 276, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 505, 473, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 506, 456, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 517, 71, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 523, 102, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 553, 36, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 634, 239, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 702, 233, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 4, 0, 717, 264, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 257, 598, 631, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 385, 439, 488, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 495, 232, 286, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x266, 0xC8, 0xAC, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x1B0, 0xE8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x1A0, 0x140, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x100, 0x140, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0xF0, 0x198, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x161, 0x200, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x171, 0x258, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 0x12, 0xA2, 0xB0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 0x12, 0xB2, 0x1D8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0x10, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
