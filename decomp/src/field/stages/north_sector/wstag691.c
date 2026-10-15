/*
 * WSTAG691: Boot Mountain, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG690.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x620
#define STAGE_ARCHIVE 0x618
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x630
#define STAGE_ARCHIVE 0x628
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0021 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 4;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x47400, 0x2B500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3D;
    FIELDSTG_state.music = MUSIC(0x3D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 136, 4, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 136, 4, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 136, 4, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 136, 4, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 183, 4, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 183, 4, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 183, 4, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 183, 4, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 243, 4, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 244, 4, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 291, 4, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 333, 4, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 175, 4, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 116, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x170, 0x175, 0xC0, 0x75, 0x160, 0x1FE },
    { 0x140, 0x100, 0x174, 0x125, 0xD0, 0x25, 0x170, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x145, 0xA8, 0x45, 0x160, 0x1FD },
    { 0x140, 0x100, 0x160, 0x14D, 0x80, 0x4D, 0x170, 0x1FD },
    { 0x140, 0x100, 0x172, 0x14D, 0xC8, 0x4D, 0x160, 0x1FC },
    { 0x140, 0x100, 0x168, 0x16D, 0xA0, 0x6D, 0x170, 0x1FC },
    { 0x140, 0x100, 0x160, 0x175, 0x80, 0x75, 0x140, 0x1FB },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x50), 1, ITEM(1, 0x2C), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 0), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk4Conditions[] = {
    PARTY_STAT(0xA), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x3C, 0), 1, CODES_END };
u16 actor3Talk5Conditions[] = {
    FLAG(0xE, 0x2F), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk5Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x2F), 1, CODES_END };
u16 actor3Talk6Conditions[] = {
    FLAG(0xE, 0x2F), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor3Talk7Conditions[] = {
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x2F), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk8Conditions[] = {
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x2F), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk8Actions[] = { CARD_BATTLE(0x3C, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 1, FLAG(0xE, 0x4F), 0, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0xE, 0x4F), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x4F), 1,
    PARTY_STAT(0xD), 0,
    CODES_END,
};
u16 actor4Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x4F), 1,
    PARTY_STAT(0xD), 1,
    CODES_END,
};
u16 actor4Talk4Actions[] = { CARD_BATTLE(0x3C, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor7Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 1), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk4Conditions[] = {
    PARTY_STAT(0xA), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk4Actions[] = { CARD_BATTLE(0x3B, 0), 1, CODES_END };
u16 actor7Talk5Conditions[] = {
    FLAG(0xE, 0x2E), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk5Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x2E), 1, CODES_END };
u16 actor7Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x2E), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk7Conditions[] = {
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x2E), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk8Conditions[] = {
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x2E), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk8Actions[] = { CARD_BATTLE(0x3B, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xB), 1, FLAG(0xE, 0x4E), 0, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0xE, 0x4E), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor8Talk3Conditions[] = {
    PARTY_STAT(0xD), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x4E), 1,
    CODES_END,
};
u16 actor8Talk4Conditions[] = {
    FLAG(0xE, 0x4E), 1,
    PARTY_STAT(0xD), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor8Talk4Actions[] = { CARD_BATTLE(0x3B, 1), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor9Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x17D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x222 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x220 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x24A },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x249 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x244 },
    { actor3Talk3Conditions, NULL, 0x245 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x246 },
    { actor3Talk5Conditions, actor3Talk5Actions, 0x247 },
    { actor3Talk6Conditions, NULL, 0x248 },
    { actor3Talk7Conditions, NULL, 0x245 },
    { actor3Talk8Conditions, actor3Talk8Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x24C },
    { actor4Talk1Conditions, NULL, 0x24E },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x24D },
    { actor4Talk3Conditions, NULL, 0x248 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x39C },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x249 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x22A },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x229 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x224 },
    { actor7Talk3Conditions, NULL, 0x225 },
    { actor7Talk4Conditions, actor7Talk4Actions, 0x226 },
    { actor7Talk5Conditions, actor7Talk5Actions, 0x227 },
    { actor7Talk6Conditions, NULL, 0x228 },
    { actor7Talk7Conditions, NULL, 0x225 },
    { actor7Talk8Conditions, actor7Talk8Actions, 0x226 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x22C },
    { actor8Talk1Conditions, NULL, 0x22E },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x22D },
    { actor8Talk3Conditions, NULL, 0x228 },
    { actor8Talk4Conditions, actor8Talk4Actions, 0x22F },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x39D },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x229 },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x22A },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x22B },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x223 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x223 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x221 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x221 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x50), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 432, 521, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2F, 5, 737, 673, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x32, 6, 945, 249, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 7, 617, 589, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 7, 617, 589, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 7, 617, 589, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x45, 7, 617, 589, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x46, 8, 851, 490, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x46, 8, 851, 490, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x46, 8, 851, 490, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x46, 8, 851, 490, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 9, 737, 673, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9D, 9, 737, 673, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9E, 0xA, 945, 249, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9E, 0xA, 945, 249, 1 };
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
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 323, 826, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 690, 795, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 450, 288, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 745, 533, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 210, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 277, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 788, 245, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 3, 6, 0, 960, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 1, 0x37, 0x3C, 6, 0, 850, 660, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 1, 0x37, 0x3C, 6, 0, 939, 699, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 1, 0x37, 0x3C, 6, 0, 950, 624, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 1, 0x37, 0x3C, 6, 0, 997, 651, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 3, 8, 0, 893, 649, 0, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 130, 97, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 434, 412, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 480, 645, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 513, 10, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 611, 294, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 628, 630, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 893, 695, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 1101, 650, 896, 0 },
    { 1, 0, 0xC8, 4, 0x33, 1, 0x33, 0x36, 8, 0, 1232, -8, 896, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 224, 527, 527, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 512, 703, 703, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 816, 375, 375, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 991, 207, 207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1088, 207, 207, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CD, 0x4F0, 0x2B8, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CA, 0xA0, 0x1D0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CA, 0xA0, 0x240, 7, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x17, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 9, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 5, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0xD, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x27F, 0x280, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x26E, 0x2E8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x4C0, 0x1A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x4B0, 0x208, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0xA, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
