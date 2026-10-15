/*
 * WSTAG351: West Wire Forest, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG350.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x698
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x6A8
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing FIELDBGM;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x2F400, 0x9C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 9;
    FIELDSTG_state.music = MUSIC(9, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

AreaBattle area0Battle0 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 93, 13, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 95, 13, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 95, 13, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 95, 13, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 95, 13, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 13, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 13, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 225, 13, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 226, 13, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 273, 13, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 329, 13, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 330, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 93, 13, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 93, 13, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 180, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 62, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x166, 0x118, 0x98, 0x18, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x118, 0xB8, 0x18, 0x140, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk4Actions[] = { CARD_BATTLE(0x23, 0), 1, CODES_END };
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1C), 0,
    CODES_END,
};
u16 actor0Talk5Actions[] = { FLAG(0xE, 0x1C), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1C), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor0Talk7Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1C), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 0,
    CODES_END,
};
u16 actor0Talk8Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1C), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    CODES_END,
};
u16 actor0Talk8Actions[] = { CARD_BATTLE(0x23, 1), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, FLAG(0xE, 0x3D), 0, CODES_END };
u16 actor1Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x3D), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    PARTY_STAT(0xB), 0,
    FLAG(0xE, 0x3D), 1,
    CODES_END,
};
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x3D), 1,
    CODES_END,
};
u16 actor1Talk4Actions[] = { CARD_BATTLE(0x23, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor4Talk4Actions[] = { CARD_BATTLE(0x24, 0), 1, CODES_END };
u16 actor4Talk5Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1D), 0,
    CODES_END,
};
u16 actor4Talk5Actions[] = { FLAG(0xE, 0x1D), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor4Talk6Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1D), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor4Talk7Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1D), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 0,
    CODES_END,
};
u16 actor4Talk8Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x1D), 1,
    PARTY_STAT(9), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor4Talk8Actions[] = { CARD_BATTLE(0x24, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(9), 1, FLAG(0xE, 0x3E), 0, CODES_END };
u16 actor5Talk2Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x3E), 1, CODES_END };
u16 actor5Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x3E), 1,
    PARTY_STAT(0xB), 0,
    CODES_END,
};
u16 actor5Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x3E), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor5Talk4Actions[] = { CARD_BATTLE(0x24, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x24A },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x249 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x24C },
    { actor0Talk3Conditions, NULL, 0x24E },
    { actor0Talk4Conditions, actor0Talk4Actions, 0x246 },
    { actor0Talk5Conditions, actor0Talk5Actions, 0x247 },
    { actor0Talk6Conditions, NULL, 0x228 },
    { actor0Talk7Conditions, NULL, 0x225 },
    { actor0Talk8Conditions, actor0Talk8Actions, 0x246 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x24C },
    { actor1Talk1Conditions, NULL, 0x24E },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x24D },
    { actor1Talk3Conditions, NULL, 0x248 },
    { actor1Talk4Conditions, actor1Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x39B },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x249 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x22A },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x229 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x224 },
    { actor4Talk3Conditions, NULL, 0x225 },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x226 },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x227 },
    { actor4Talk6Conditions, NULL, 0x228 },
    { actor4Talk7Conditions, NULL, 0x225 },
    { actor4Talk8Conditions, actor4Talk8Actions, 0x226 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x22C },
    { actor5Talk1Conditions, NULL, 0x22E },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x22D },
    { actor5Talk3Conditions, NULL, 0x228 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0x22F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x22B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x39D },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x229 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x22A },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x45, 4, 722, 161, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 4, 722, 161, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 4, 722, 161, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 4, 722, 161, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x46, 5, 786, 643, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x46, 5, 786, 643, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x46, 5, 786, 643, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x46, 5, 786, 643, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 311, 249, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 728, 359, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 930, 44, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 145, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 239, 425, 0, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 441, 339, 339, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 488, 363, 363, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 527, 207, 207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 576, 230, 230, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 584, 131, 131, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 632, 107, 107, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 679, 83, 83, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 687, 551, 551, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 760, 83, 83, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 551, 551, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 808, 107, 107, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 856, 131, 131, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 880, 551, 551, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 896, 151, 151, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 944, 463, 463, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 991, 440, 440, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1037, 410, 410, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1079, 395, 395, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1144, 387, 387, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1192, 410, 410, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1240, 435, 435, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1280, 455, 455, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1304, 499, 499, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1328, 335, 335, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1336, 531, 531, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1368, 355, 355, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1380, 553, 553, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1411, 376, 376, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28D, 0x28C, 0x33C, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x291, 0x8C, 0xCE, 7, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x50, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0x38, 0, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
