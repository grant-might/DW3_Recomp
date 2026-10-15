/*
 * WSTAG501: Zhu Que City, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG500.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x5B6
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x5C6
#endif
/*
 * Sets the stage up: its map, actors and battles, playing BGM_0016; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x20000, 0x2AA00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x12;
    FIELDSTG_state.music = MUSIC(0x12, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
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
AreaBattle area3Battle0 = { 232, 18, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 233, 18, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 280, 18, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 281, 18, MUSIC(3, 0) };
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
    { 160, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x195, 0, 0x95, 0x160, 0x1FF },
    { 0x140, 0x100, 0x148, 0x19D, 0x20, 0x9D, 0x170, 0x1FF },
    { 0x140, 0x100, 0x140, 0x16D, 0, 0x6D, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x1D5, 0, 0xD5, 0x160, 0x1FE },
    { 0x140, 0x100, 0x14A, 0x1BD, 0x28, 0xBD, 0x170, 0x1FE },
    { 0x140, 0x100, 0x152, 0x1A4, 0x48, 0xA4, 0x150, 0x1FD },
    { 0x140, 0x100, 0x15A, 0x1A4, 0x68, 0xA4, 0x160, 0x1FD },
    { 0x140, 0x100, 0x16E, 0x1A4, 0xB8, 0xA4, 0x170, 0x1FD },
    { 0x140, 0x100, 0x162, 0x1C1, 0x88, 0xC1, 0x150, 0x1FC },
    { 0x140, 0x100, 0x176, 0x1C4, 0xD8, 0xC4, 0x160, 0x1FC },
    { 0x140, 0x100, 0x152, 0x1CC, 0x48, 0xCC, 0x170, 0x1FC },
    { 0x140, 0x100, 0x15A, 0x1CC, 0x68, 0xCC, 0x140, 0x1FB },
    { 0x140, 0x100, 0x14C, 0x16D, 0x30, 0x6D, 0x150, 0x1FB },
};
u16 actor0Talk0Actions[] = { 0x7A18, 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7A16, 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor6Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, PARTY_STAT(5), 0, CODES_END };
u16 actor6Talk4Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor6Talk4Actions[] = { CARD_BATTLE(0x31, 0), 1, CODES_END };
u16 actor6Talk5Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x23), 0,
    CODES_END,
};
u16 actor6Talk5Actions[] = { FLAG(0xE, 0x23), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor6Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x23), 1,
    ITEM(0, 0x14), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor6Talk7Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x23), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 0,
    CODES_END,
};
u16 actor6Talk8Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x23), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(9), 1,
    CODES_END,
};
u16 actor6Talk8Actions[] = { CARD_BATTLE(0x31, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor7Talk2Conditions[] = { PARTY_STAT(9), 1, FLAG(0, 0), 1, FLAG(0xE, 0x44), 0, CODES_END };
u16 actor7Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x44), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0xE, 0x44), 1,
    PARTY_STAT(0xB), 0,
    PARTY_STAT(9), 1,
    CODES_END,
};
u16 actor7Talk4Conditions[] = {
    PARTY_STAT(9), 1,
    FLAG(0, 0), 1,
    FLAG(0xE, 0x44), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor7Talk4Actions[] = { CARD_BATTLE(0x31, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor10Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor10Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 1, PARTY_STAT(5), 0, CODES_END };
u16 actor10Talk4Conditions[] = {
    PARTY_STAT(8), 0,
    PARTY_STAT(5), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk4Actions[] = { CARD_BATTLE(0x30, 0), 1, CODES_END };
u16 actor10Talk5Conditions[] = {
    FLAG(0xE, 0x24), 0,
    PARTY_STAT(8), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk5Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x24), 1, CODES_END };
u16 actor10Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x24), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk7Conditions[] = {
    PARTY_STAT(9), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x24), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk8Conditions[] = {
    PARTY_STAT(9), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x24), 1,
    PARTY_STAT(8), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor10Talk8Actions[] = { CARD_BATTLE(0x30, 1), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor11Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor11Talk1Conditions[] = { PARTY_STAT(9), 0, FLAG(0, 1), 1, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0, 1), 1, FLAG(0xE, 0x45), 0, PARTY_STAT(9), 1, CODES_END };
u16 actor11Talk2Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x45), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x45), 1,
    PARTY_STAT(0xB), 0,
    FLAG(0, 1), 1,
    CODES_END,
};
u16 actor11Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x45), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor11Talk4Actions[] = { CARD_BATTLE(0x30, 1), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor12Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor20Talk0Actions[] = { 0x7A17, 1, CODES_END };
u16 actor21Talk0Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor21Talk1Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor21Talk1Actions[] = { 0x7A48, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x1A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x14E },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x154 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x153 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x131 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x130 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x12B },
    { actor6Talk3Conditions, NULL, 0x12C },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x12D },
    { actor6Talk5Conditions, actor6Talk5Actions, 0x12E },
    { actor6Talk6Conditions, NULL, 0x132 },
    { actor6Talk7Conditions, NULL, 0x12C },
    { actor6Talk8Conditions, actor6Talk8Actions, 0x12D },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x133 },
    { actor7Talk1Conditions, NULL, 0x135 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x134 },
    { actor7Talk3Conditions, NULL, 0x12F },
    { actor7Talk4Conditions, actor7Talk4Actions, 0x136 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x205 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x130 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x131 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x132 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x183 },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x182 },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x133 },
    { actor10Talk3Conditions, NULL, 0x135 },
    { actor10Talk4Conditions, actor10Talk4Actions, 0x136 },
    { actor10Talk5Conditions, actor10Talk5Actions, 0x134 },
    { actor10Talk6Conditions, NULL, 0x181 },
    { actor10Talk7Conditions, NULL, 0x187 },
    { actor10Talk8Conditions, actor10Talk8Actions, 0x188 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, actor11Talk0Actions, 0x185 },
    { actor11Talk1Conditions, NULL, 0x187 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x186 },
    { actor11Talk3Conditions, NULL, 0x181 },
    { actor11Talk4Conditions, actor11Talk4Actions, 0x188 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x206 },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x182 },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x183 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x184 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x151 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x152 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x14F },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x150 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x14C },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x14D },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, actor20Talk0Actions, 0x1A6 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0x20D },
    { actor21Talk1Conditions, actor21Talk1Actions, 0x1A7 },
    { NULL, NULL, 0 },
};
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 923, 892, 3 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 816, 521, 1 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x18, 6, 1189, 666, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 7, 645, 199, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 8, 432, 249, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x36, 9, 304, 369, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x45, 0xA, 545, 673, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x45, 0xA, 545, 673, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x45, 0xA, 545, 673, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x45, 0xA, 545, 673, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x46, 0xB, 687, 682, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x46, 0xB, 687, 682, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x46, 0xB, 687, 682, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x46, 0xB, 687, 682, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9D, 0xC, 304, 369, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9D, 0xC, 304, 369, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9E, 0xD, 432, 249, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9E, 0xD, 432, 249, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9F, 0xE, 645, 199, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x9F, 0xE, 645, 199, 1 };
FieldActorEntry actor20 = { NULL, actor20Talks, 0xB7, 0xF, 990, 767, 1 };
FieldActorEntry actor21 = { NULL, actor21Talks, 0xCE, 0x10, 1216, 913, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 8, 0, 160, 342, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 8, 0, 234, 305, 0, 0 },
    { 1, 0, 0x78, 2, 0xC, 0, 0, 0, 0, 0, 542, 517, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 70, 849, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 614, 820, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1038, 280, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1089, 931, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 462, 518, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 832, 752, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1124, 522, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1343, 870, 0, 0 },
    { 1, 0x64, 0x40, 6, 9, 0, 0, 0, 0, 0, 769, 100, 0, 0 },
    { 1, 0x65, 0x40, 6, 0xA, 0, 0, 0, 0, 0, 574, 93, 0, 0 },
    { 1, 0x66, 0x40, 6, 0xB, 0, 0, 0, 0, 0, 186, 320, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 606, 118, 136, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 156, 353, 380, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 318, 390, 407, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 400, 341, 354, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 501, 625, 649, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 660, 626, 649, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 537, 158, 166, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 1206, 636, 682, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 1120, 656, 670, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A8, 0x630, 0x88, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AC, 0x2C0, 0x1B0, 3, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AD, 0xC8, 0x164, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2AD, 0x258, 0x1C2, 3, 0x64, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0x11, 1 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0x11, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
