/*
 * WSTAG330: Central Park, Central Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x1A7
#define STAGE_ARCHIVE 0x319
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x1B5
#define STAGE_ARCHIVE 0x328
#endif
/*
 * Sets the stage up: its map, actors and battles, playing FIELDBGM; BGM_0029
 * instead at story steps 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x38400, 0x3E800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 9;
    FIELDSTG_state.music = MUSIC(9, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

AreaBattle area0Battle0 = { 34, 1, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 34, 1, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 34, 1, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 34, 1, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 34, 1, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 35, 1, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 35, 1, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 35, 1, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 34, 1, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 34, 1, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 34, 1, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 34, 1, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 34, 1, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 35, 1, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 35, 1, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 35, 1, MUSIC(2, 0) };
BattleList area1Battles = {
    1,
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
AreaBattle area3Battle0 = { 201, 1, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 327, 1, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 49, 1, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 1, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x1C4, 0xD8, 0xC4, 0x160, 0x1FF },
    { 0x140, 0x100, 0x178, 0x130, 0xE0, 0x30, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B6, 0x11B, 0x1D8, 0x1B, 0x140, 0x1FE },
    { 0x180, 0x100, 0x180, 0x11F, 0x100, 0x1F, 0x150, 0x1FE },
    { 0x180, 0x100, 0x188, 0x11F, 0x120, 0x1F, 0x160, 0x1FE },
    { 0x180, 0x100, 0x190, 0x11F, 0x140, 0x1F, 0x170, 0x1FE },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x150, 0x1FD },
    { 0x180, 0x100, 0x1A0, 0x11F, 0x180, 0x1F, 0x160, 0x1FD },
    { 0x180, 0x100, 0x190, 0x13F, 0x140, 0x3F, 0x170, 0x1FD },
    { 0x180, 0x100, 0x198, 0x13F, 0x160, 0x3F, 0x140, 0x1FC },
    { 0x180, 0x100, 0x1A0, 0x13F, 0x180, 0x3F, 0x150, 0x1FC },
};
u16 actor0Talk0Conditions[] = { ITEM(0, 0x28), 0, CODES_END };
u16 actor0Talk0Actions[] = { 0x9400, 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(0, 0x29), 0, ITEM(0, 0x28), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x9401, 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 actor0Talk2Actions[] = { 0x9406, 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x940D, 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(2, 1), 1, ITEM(1, 0x2B), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1A, 0x1F), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0x1A, 0x1F), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1A, 0x1F), 1, SPECIAL(0x2D), 0, CODES_END };
u16 actor3Talk2Conditions[] = {
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x2D), 1,
    SPECIAL(0x26), 0,
    CODES_END,
};
u16 actor3Talk3Conditions[] = {
    SPECIAL(0x26), 1,
    SPECIAL(0x2D), 1,
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x28), 0,
    CODES_END,
};
u16 actor3Talk3Actions[] = { FLAG(6, 2), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x2D), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 1,
    CODES_END,
};
u16 actor4Talk0Conditions[] = { SPECIAL(0xE), 0, CODES_END };
u16 actor4Talk0Actions[] = { SPECIAL(0x34), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor4Talk1Conditions[] = { SPECIAL(0xE), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0x1A, 0x1F), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0x1A, 0x1F), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x1A, 0x1F), 1, SPECIAL(0x2D), 0, CODES_END };
u16 actor5Talk2Conditions[] = {
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x2D), 1,
    SPECIAL(0x26), 0,
    CODES_END,
};
u16 actor5Talk3Conditions[] = {
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x2D), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 0,
    CODES_END,
};
u16 actor5Talk3Actions[] = { FLAG(6, 2), 1, CODES_END };
u16 actor5Talk4Conditions[] = {
    FLAG(0x1A, 0x1F), 1,
    SPECIAL(0x2D), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 1,
    CODES_END,
};
u16 actor6Talk0Conditions[] = { SPECIAL(0xE), 0, CODES_END };
u16 actor6Talk0Actions[] = { SPECIAL(0x34), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor6Talk1Conditions[] = { SPECIAL(0xE), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0x1A, 0x20), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0x1A, 0x20), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0x1A, 0x20), 1, SPECIAL(0x36), 0, CODES_END };
u16 actor7Talk2Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 0,
    CODES_END,
};
u16 actor7Talk3Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 0,
    CODES_END,
};
u16 actor7Talk3Actions[] = { FLAG(6, 3), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 1,
    CODES_END,
};
u16 actor8Talk0Conditions[] = { SPECIAL(0xF), 0, CODES_END };
u16 actor8Talk0Actions[] = { SPECIAL(0x35), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor8Talk1Conditions[] = { SPECIAL(0xF), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0x1A, 0x20), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0x1A, 0x20), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0x1A, 0x20), 1, SPECIAL(0x36), 0, CODES_END };
u16 actor9Talk2Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 0,
    CODES_END,
};
u16 actor9Talk3Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 0,
    CODES_END,
};
u16 actor9Talk3Actions[] = { FLAG(6, 3), 1, CODES_END };
u16 actor9Talk4Conditions[] = {
    FLAG(0x1A, 0x20), 1,
    SPECIAL(0x36), 1,
    SPECIAL(0x26), 1,
    SPECIAL(0x28), 1,
    CODES_END,
};
u16 actor10Talk0Conditions[] = { SPECIAL(0xF), 0, CODES_END };
u16 actor10Talk0Actions[] = { SPECIAL(0x35), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor10Talk1Conditions[] = { SPECIAL(0xF), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor11Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor11Talk2Actions[] = { CARD_BATTLE(5, 0), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 1), 0,
    CODES_END,
};
u16 actor11Talk3Actions[] = { FLAG(0xE, 1), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor11Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor11Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor11Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor11Talk6Actions[] = { CARD_BATTLE(5, 1), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor12Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor13Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor13Talk2Actions[] = { CARD_BATTLE(5, 0), 1, CODES_END };
u16 actor13Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 1), 0,
    CODES_END,
};
u16 actor13Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 1), 1, CODES_END };
u16 actor13Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor13Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor13Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor13Talk6Actions[] = { CARD_BATTLE(5, 1), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor14Talk2Actions[] = { CARD_BATTLE(5, 0), 1, CODES_END };
u16 actor14Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(0), 1,
    FLAG(0xE, 1), 0,
    CODES_END,
};
u16 actor14Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 1), 1, CODES_END };
u16 actor14Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor14Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor14Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor14Talk6Actions[] = { CARD_BATTLE(5, 1), 1, CODES_END };
u16 actor27Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor27Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor37Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor37Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor37Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor37Talk2Actions[] = { CARD_BATTLE(5, 0), 1, CODES_END };
u16 actor37Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 1), 0,
    CODES_END,
};
u16 actor37Talk3Actions[] = { FLAG(0xE, 1), 1, CODES_END };
u16 actor37Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor37Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 0,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor37Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(2), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 1), 1,
    CODES_END,
};
u16 actor37Talk6Actions[] = { CARD_BATTLE(5, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x1B },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x1B },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x16D },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x277 },
    { actor3Talk1Conditions, NULL, 0x282 },
    { actor3Talk2Conditions, NULL, 0x278 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x279 },
    { actor3Talk4Conditions, NULL, 0x28C },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x27A },
    { actor4Talk1Conditions, NULL, 0x27C },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x277 },
    { actor5Talk1Conditions, NULL, 0x282 },
    { actor5Talk2Conditions, NULL, 0x278 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x279 },
    { actor5Talk4Conditions, NULL, 0x28C },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x27A },
    { actor6Talk1Conditions, NULL, 0x27C },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x285 },
    { actor7Talk1Conditions, NULL, 0x28B },
    { actor7Talk2Conditions, NULL, 0x286 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x287 },
    { actor7Talk4Conditions, NULL, 0x28D },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x288 },
    { actor8Talk1Conditions, NULL, 0x28A },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x285 },
    { actor9Talk1Conditions, NULL, 0x28B },
    { actor9Talk2Conditions, NULL, 0x286 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x287 },
    { actor9Talk4Conditions, NULL, 0x28D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x288 },
    { actor10Talk1Conditions, NULL, 0x28A },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, actor11Talk0Actions, 0x2E },
    { actor11Talk1Conditions, NULL, 0x33 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x34 },
    { actor11Talk3Conditions, actor11Talk3Actions, 0x35 },
    { actor11Talk4Conditions, NULL, 0x36 },
    { actor11Talk5Conditions, NULL, 0x37 },
    { actor11Talk6Conditions, actor11Talk6Actions, 0x6A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x2E },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x38 },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0x32 },
    { actor13Talk1Conditions, NULL, 0x33 },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x34 },
    { actor13Talk3Conditions, actor13Talk3Actions, 0x35 },
    { actor13Talk4Conditions, NULL, 0x36 },
    { actor13Talk5Conditions, NULL, 0x37 },
    { actor13Talk6Conditions, actor13Talk6Actions, 0x6A },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x30 },
    { actor14Talk1Conditions, NULL, 0x33 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0x34 },
    { actor14Talk3Conditions, actor14Talk3Actions, 0x35 },
    { actor14Talk4Conditions, NULL, 0x36 },
    { actor14Talk5Conditions, NULL, 0x37 },
    { actor14Talk6Conditions, actor14Talk6Actions, 0x6A },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x270 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0xEA },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0xFE },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x1A },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0xFD },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x102 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x102 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x103 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x102 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x101 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x101 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { actor27Talk0Conditions, NULL, 0x102 },
    { actor27Talk1Conditions, NULL, 0x10 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0xE9 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x334 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { actor37Talk0Conditions, NULL, 0x31 },
    { actor37Talk1Conditions, NULL, 0x31 },
    { actor37Talk2Conditions, actor37Talk2Actions, 0x31 },
    { actor37Talk3Conditions, actor37Talk3Actions, 0x31 },
    { actor37Talk4Conditions, NULL, 0x31 },
    { actor37Talk5Conditions, NULL, 0x31 },
    { actor37Talk6Conditions, actor37Talk6Actions, 0x31 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x104 },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x27B },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x289 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(2, 1), 0, CODES_END };
u16 actor3Conditions[] = { ITEM(0, 0x27), 0, SPECIAL(0x24), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x27), 1, SPECIAL(0x24), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x27), 0, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 0x27), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x24), 1, ITEM(0, 0x26), 0, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x24), 1, ITEM(0, 0x26), 1, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x26), 0, PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x26), 1,
    CODES_END,
};
u16 actor11Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(4), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor15Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor16Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x20), 1, PROGRESS(0x21), 0, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x1B, 4, 923, 374, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x1B, 4, 923, 374, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x21, 5, 738, 394, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2B, 6, 737, 954, 3 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2B, 6, 737, 954, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2B, 6, 737, 954, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2B, 6, 737, 954, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2C, 7, 913, 233, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2C, 7, 913, 233, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2C, 7, 913, 233, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2C, 7, 913, 233, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x30, 8, 1152, 793, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x30, 8, 1152, 793, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x30, 8, 1152, 793, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x30, 8, 1152, 793, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x30, 8, 1152, 793, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x3A, 9, 1065, 997, 1 };
FieldActorEntry actor28 = { NULL, actor28Talks, 0x3F, 0xA, 961, 681, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x66, 0xB, 400, 784, 7 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x9D, 0xC, 1152, 793, 1 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x9E, 0xD, 1065, 997, 1 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x9F, 0xE, 737, 954, 3 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0xA0, 0xF, 913, 233, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 6, 1, 6, 8, 8, 0, 787, 422, 0, 0 },
    { 1, 0, 0x40, 2, 9, 1, 9, 0xE, 8, 0, 202, 246, 0, 0 },
    { 1, 0, 0x40, 2, 9, 1, 9, 0xE, 8, 0, 410, 878, 0, 0 },
    { 1, 0, 0x40, 2, 9, 1, 9, 0xE, 8, 0, 1322, 886, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 150, 820, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 500, 1060, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 575, 940, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 681, 1030, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 715, 1004, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 870, 843, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 910, 455, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1271, 1057, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1520, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 97, 735, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 195, 304, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 268, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 584, 151, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 668, 1135, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 738, 892, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 742, 475, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 750, 999, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1053, 1074, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 1331, 461, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 182, 792, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 645, 1050, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 867, 469, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 869, 1066, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1399, 423, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1458, 398, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 97, 892, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 138, 930, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 145, 273, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 180, 706, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 184, 407, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 293, 496, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 386, 648, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 495, 624, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 527, 75, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 530, 1028, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 611, 1160, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 643, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 910, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1039, 341, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1365, 429, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1565, 333, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 212, 806, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 232, 560, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 250, 329, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 367, 607, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 673, 194, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1070, 347, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1172, 1074, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1298, 459, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 208, 525, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 261, 826, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 317, 582, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 468, 997, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 579, 1121, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 786, 1012, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 990, 1080, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1076, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1417, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1513, 371, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 78, 891, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 113, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 133, 615, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 249, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 277, 870, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 459, 1003, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 554, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 569, 1062, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 675, 1037, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 711, 518, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 727, 511, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 751, 903, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 878, 1051, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 939, 488, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1088, 1054, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1279, 482, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1391, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1410, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1486, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1589, 336, 0, 0 },
    { 1, 0, 0x72, 4, 0, 0, 0, 0, 0, 0, 765, 572, 665, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 951, 632, 679, 0 },
    { 1, 0, 0x7D, 4, 2, 0, 0, 0, 0, 0, 816, 122, 245, 0 },
    { 1, 0, 0x7D, 4, 3, 0, 0, 0, 0, 0, 800, 114, 238, 0 },
    { 1, 0, 0x7D, 4, 4, 0, 0, 0, 0, 0, 784, 106, 230, 0 },
    { 1, 0, 0x7D, 4, 5, 0, 0, 0, 0, 0, 767, 99, 223, 0 },
    { 1, 0, 0x46, 4, 0xF, 0, 0, 0, 0, 0, 1224, 424, 484, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 243, 754, 754, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 266, 606, 606, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 275, 778, 778, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 314, 630, 630, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 338, 538, 538, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 363, 654, 654, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 410, 678, 678, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 483, 930, 930, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 531, 906, 906, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 547, 674, 674, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 579, 882, 882, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 595, 650, 650, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 626, 474, 474, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 627, 858, 858, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 643, 994, 994, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 667, 454, 454, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 691, 490, 490, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 723, 826, 826, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 771, 802, 802, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 787, 938, 938, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 819, 778, 778, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 835, 194, 194, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 835, 914, 914, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 883, 890, 890, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 883, 938, 938, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 931, 914, 914, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 979, 378, 378, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 979, 890, 890, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1027, 402, 402, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1075, 426, 426, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1314, 642, 642, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1315, 690, 690, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1315, 738, 738, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 570, 570, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 618, 618, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 666, 666, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 714, 714, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 762, 762, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x202, 0x100, 0x1E0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21E, 0x7C, 0x7E, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21F, 0x30A, 0x98, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x220, 0x4AC, 0x326, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0x2CE, 0x124, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0x2DF, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x324, 0x170, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x333, 0x11A, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 1, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x11, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 3, 3 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 1, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x60, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x40, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x10, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFE0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0xFFF8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x14, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 1, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
