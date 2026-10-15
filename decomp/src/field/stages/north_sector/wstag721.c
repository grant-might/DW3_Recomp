/*
 * WSTAG721: Legendary Gym, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG720.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x666
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x676
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0022 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x8C00, 0x2BB00};
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
AreaBattle area3Battle0 = { 249, 20, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 250, 20, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 297, 18, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 298, 18, MUSIC(3, 0) };
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
    { 163, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x172, 0x174, 0xC8, 0x74, 0x160, 0x1FF },
    { 0x140, 0x100, 0x176, 0x139, 0xD8, 0x39, 0x170, 0x1FF },
    { 0x140, 0x100, 0x148, 0x176, 0x20, 0x76, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x176, 0, 0x76, 0x170, 0x1FE },
    { 0x140, 0x100, 0x150, 0x176, 0x40, 0x76, 0x160, 0x1FD },
    { 0x140, 0x100, 0x158, 0x176, 0x60, 0x76, 0x170, 0x1FD },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x8F), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 2), 0, ITEM(3, 0x8F), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 2), 1, ITEM(3, 0x8F), 0, ITEM(2, 0x8B), 0, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 2), 1, ITEM(3, 0x8F), 0, ITEM(2, 0x8B), 1, CODES_END };
u16 actor0Talk3Actions[] = {
    ITEM(3, 0x8F), 1,
    ITEM(3, 0x8E), 0,
    ITEM(2, 0x8B), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor1Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor1Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk4Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor1Talk4Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk5Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor1Talk5Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk6Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(8), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk7Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor1Talk7Actions[] = { CARD_BATTLE(0x41, 0), 1, CODES_END };
u16 actor1Talk8Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x34), 0,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor1Talk8Actions[] = { FLAG(0xE, 0x34), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor1Talk9Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 0,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x34), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor1Talk10Conditions[] = {
    PARTY_STAT(0xC), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x34), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor1Talk11Conditions[] = {
    FLAG(0xE, 0x34), 1,
    PARTY_STAT(0xC), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor1Talk11Actions[] = { CARD_BATTLE(0x41, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk3Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk4Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Talk4Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk5Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk5Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk6Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(8), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk7Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk7Actions[] = { CARD_BATTLE(0x42, 0), 1, CODES_END };
u16 actor4Talk8Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x35), 0,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk8Actions[] = { FLAG(0xE, 0x35), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor4Talk9Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 0,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x35), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk10Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x35), 1,
    PARTY_STAT(0xC), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk11Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(8), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0xE, 0x35), 1,
    PARTY_STAT(0xC), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk11Actions[] = { CARD_BATTLE(0x42, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor6Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Talk0Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 0, CODES_END };
u16 actor7Talk0Actions[] = { 0x940B, 1, CODES_END };
u16 actor7Talk1Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor7Talk1Actions[] = { 0x940C, 1, CODES_END };
u16 actor8Talk0Actions[] = { 0x940D, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x306 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x307 },
    { actor0Talk2Conditions, NULL, 0x308 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x309 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x2A1 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x25 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x2A0 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x24 },
    { actor1Talk4Conditions, actor1Talk4Actions, 0x29A },
    { actor1Talk5Conditions, actor1Talk5Actions, 0x23 },
    { actor1Talk6Conditions, NULL, 0x29C },
    { actor1Talk7Conditions, actor1Talk7Actions, 0x29D },
    { actor1Talk8Conditions, actor1Talk8Actions, 0x29F },
    { actor1Talk9Conditions, NULL, 0x29E },
    { actor1Talk10Conditions, NULL, 0x2A3 },
    { actor1Talk11Conditions, actor1Talk11Actions, 0x2A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x2A2 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x2A0 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x2A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x2A2 },
    { actor3Talk1Conditions, NULL, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x2AE },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x2E },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x2AD },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x2D },
    { actor4Talk4Conditions, actor4Talk4Actions, 0x2A5 },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x2A5 },
    { actor4Talk6Conditions, NULL, 0x2A7 },
    { actor4Talk7Conditions, actor4Talk7Actions, 0x2A8 },
    { actor4Talk8Conditions, actor4Talk8Actions, 0x2AA },
    { actor4Talk9Conditions, NULL, 0x2AE },
    { actor4Talk10Conditions, NULL, 0x2AB },
    { actor4Talk11Conditions, actor4Talk11Actions, 0x2AC },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x2AF },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x2AD },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x2AE },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x2AF },
    { actor6Talk1Conditions, NULL, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x2DB },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x2DB },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, actor8Talk0Actions, 0x2DB },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x2B0 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x2B1 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    SPECIAL(0x49), 1,
    SPECIAL(0x51), 1,
    ITEM(3, 0x8E), 1,
    ITEM(3, 0x8F), 0,
    CODES_END,
};
u16 actor1Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor2Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor3Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x1E), 1, CODES_END };
u16 actor5Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor6Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor7Conditions[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actor8Conditions[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x1F, 4, 538, 145, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x36, 5, 320, 216, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x36, 5, 320, 216, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x36, 5, 320, 216, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 6, 203, 152, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x39, 6, 273, 89, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x39, 6, 203, 152, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x8C, 7, 124, 700, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x8C, 7, 124, 700, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 8, 320, 216, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9E, 9, 203, 152, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x76, 2, 2, 0, 0, 0, 0, 0, 15, 51, 0, 0 },
    { 1, 0, 0x76, 2, 2, 0, 0, 0, 0, 0, 367, 644, 0, 0 },
    { 1, 0, 0x48, 2, 3, 0, 0, 0, 0, 0, 184, 716, 0, 0 },
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
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 495, 232, 286, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CE, 0xC8, 0xAC, 7, 0, 0, 0 },
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
