/*
 * WSTAG696: Snow Mountain, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG695.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5F3
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x603
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0007 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xBA00, 0x24900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2F;
    FIELDSTG_state.music = MUSIC(0x2F, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 160, 14, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 245, 14, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 246, 14, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 293, 14, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 333, 14, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 187, 14, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 115, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x14C, 0x1C8, 0x30, 0xC8, 0x170, 0x1FE },
    { 0x140, 0x100, 0x154, 0x1C8, 0x50, 0xC8, 0x170, 0x1FD },
    { 0x180, 0x100, 0x180, 0x100, 0x100, 0, 0x170, 0x1FC },
    { 0x180, 0x100, 0x188, 0x100, 0x120, 0, 0x160, 0x1FB },
    { 0x180, 0x100, 0x190, 0x100, 0x140, 0, 0x170, 0x1FB },
    { 0x180, 0x100, 0x198, 0x100, 0x160, 0, 0x160, 0x1FA },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 1, FLAG(0xE, 0x51), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0xE, 0x51), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x51), 1,
    PARTY_STAT(0xD), 0,
    CODES_END,
};
u16 actor2Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x51), 1,
    PARTY_STAT(0xD), 1,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x3E, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 0), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk4Conditions[] = {
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x3E, 0), 1, CODES_END };
u16 actor3Talk5Conditions[] = {
    FLAG(0xE, 0x31), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk5Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x31), 1, CODES_END };
u16 actor3Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x31), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk7Conditions[] = {
    FLAG(0xE, 0x31), 1,
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk8Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x31), 1,
    PARTY_STAT(8), 1,
    CODES_END,
};
u16 actor3Talk8Actions[] = { CARD_BATTLE(0x3E, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xB), 1, FLAG(0xE, 0x52), 0, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0xE, 0x52), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x52), 1,
    PARTY_STAT(0xD), 0,
    CODES_END,
};
u16 actor6Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x52), 1,
    PARTY_STAT(0xD), 1,
    CODES_END,
};
u16 actor6Talk4Actions[] = { CARD_BATTLE(0x3D, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor7Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 1), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk4Conditions[] = {
    PARTY_STAT(0xA), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk4Actions[] = { CARD_BATTLE(0x3D, 0), 1, CODES_END };
u16 actor7Talk5Conditions[] = {
    FLAG(0xE, 0x30), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk5Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x30), 1, CODES_END };
u16 actor7Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x30), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk7Conditions[] = {
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x30), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk8Conditions[] = {
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x30), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk8Actions[] = { CARD_BATTLE(0x3D, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x264 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x266 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x24C },
    { actor2Talk1Conditions, NULL, 0x24E },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x24D },
    { actor2Talk3Conditions, NULL, 0x248 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x24F },
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
    { actor3Talk7Conditions, NULL, 0x24B },
    { actor3Talk8Conditions, actor3Talk8Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x39B },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x249 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x22C },
    { actor6Talk1Conditions, NULL, 0x22E },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x22D },
    { actor6Talk3Conditions, NULL, 0x228 },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x22F },
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
    { actor7Talk7Conditions, NULL, 0x22C },
    { actor7Talk8Conditions, actor7Talk8Actions, 0x226 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x39D },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x229 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x22A },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x267 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x267 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x265 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x265 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x31, 4, 656, 409, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x37, 5, 272, 825, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 6, 721, 569, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 6, 721, 569, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 6, 721, 569, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x45, 6, 721, 569, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x46, 7, 353, 320, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x46, 7, 353, 320, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x46, 7, 353, 320, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x46, 7, 353, 320, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 8, 272, 825, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 8, 272, 825, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 9, 656, 409, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9E, 9, 656, 409, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 2, 0, 5, 6, 0, 472, 329, 0, 0 },
    { 1, 0, 0xC8, 2, 0x34, 1, 0x34, 0x37, 8, 0, 542, 48, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 90, 310, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 584, 768, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 1032, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xD, 6, 0, 579, 336, 0, 0 },
    { 1, 0x64, 0x40, 6, 1, 0, 0, 0, 0, 0, 546, 340, 0, 0 },
    { 1, 0, 0x48, 4, 0, 0, 0, 0, 0, 0, 517, 329, 393, 0 },
    { 1, 0, 0x53, 4, 8, 0, 0, 0, 0, 0, 448, 290, 368, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 192, 783, 783, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 400, 263, 263, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 672, 175, 175, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 720, 199, 199, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C9, 0x500, 0x290, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C9, 0x500, 0x300, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CC, 0x238, 0x444, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CB, 0x108, 0x12C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x1CE, 0xAA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x1BF, 0x112, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x3B0, 0x16A, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x3BF, 0x1D0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x380, 0x253, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x38F, 0x2EA, 0, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x14, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 7, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
