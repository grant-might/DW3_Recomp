/*
 * WSTAG710: Kulon Mine, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x299
#define STAGE_ARCHIVE 0x30E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x2A8
#define STAGE_ARCHIVE 0x31D
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0007 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x43C00, 0x25900};
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

AreaBattle area0Battle0 = { 161, 4, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 161, 4, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 161, 4, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 110, 4, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 110, 4, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 153, 4, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 153, 4, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 153, 4, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 220, 4, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 221, 4, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 333, 4, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 127, 4, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 84, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B4, 0x140, 0x1D0, 0x40, 0x160, 0x1FF },
    { 0x140, 0x100, 0x174, 0x1C1, 0xD0, 0xC1, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x160, 0x1FE },
    { 0x180, 0x100, 0x1B4, 0x120, 0x1D0, 0x20, 0x170, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 0, SPECIAL(4), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 0), 0, PROGRESS(0x26), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk3Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk4Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk5Actions[] = { CARD_BATTLE(0x1E, 0), 1, CODES_END };
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x14), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk6Actions[] = { FLAG(0xE, 0x14), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor0Talk7Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk8Conditions[] = {
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk9Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk9Actions[] = { CARD_BATTLE(0x1E, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x1E, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 0,
    CODES_END,
};
u16 actor3Talk4Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x14), 1, CODES_END };
u16 actor3Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(0x1E, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, SPECIAL(4), 1, CODES_END };
u16 actor4Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor4Talk3Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor4Talk4Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(6), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor4Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(8), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk5Actions[] = { CARD_BATTLE(0x1F, 0), 1, CODES_END };
u16 actor4Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    FLAG(0xE, 0x15), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk6Actions[] = { FLAG(0xE, 0x15), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor4Talk7Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk8Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk9Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor4Talk9Actions[] = { CARD_BATTLE(0x1F, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(0x1F, 0), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor7Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 0,
    CODES_END,
};
u16 actor7Talk4Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x15), 1, CODES_END };
u16 actor7Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor7Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor7Talk6Actions[] = { CARD_BATTLE(0x1F, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0), 1, ITEM(0, 0x14), 0, PARTY_STAT(6), 1, CODES_END };
u16 actor8Talk2Actions[] = { CARD_BATTLE(0x1E, 0), 1, CODES_END };
u16 actor8Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor8Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 0,
    CODES_END,
};
u16 actor8Talk4Actions[] = { FLAG(0xE, 0x14), 1, CODES_END };
u16 actor8Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor8Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x14), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor8Talk6Actions[] = { CARD_BATTLE(0x1E, 1), 1, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(6), 0, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(6), 1, ITEM(0, 0x14), 0, CODES_END };
u16 actor9Talk2Actions[] = { CARD_BATTLE(0x1F, 0), 1, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 0,
    CODES_END,
};
u16 actor9Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 0,
    CODES_END,
};
u16 actor9Talk4Actions[] = { FLAG(0xE, 0x15), 1, CODES_END };
u16 actor9Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 0,
    CODES_END,
};
u16 actor9Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(6), 1,
    ITEM(0, 0x14), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x15), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor9Talk6Actions[] = { CARD_BATTLE(0x1F, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x187 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x186 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x17C },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x17D },
    { actor0Talk4Conditions, NULL, 0x180 },
    { actor0Talk5Conditions, actor0Talk5Actions, 0x181 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0x183 },
    { actor0Talk7Conditions, NULL, 0x182 },
    { actor0Talk8Conditions, NULL, 0x184 },
    { actor0Talk9Conditions, actor0Talk9Actions, 0x185 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x297 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x17C },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x186 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x187 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x17D },
    { actor3Talk1Conditions, NULL, 0x180 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x181 },
    { actor3Talk3Conditions, NULL, 0x182 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x183 },
    { actor3Talk5Conditions, NULL, 0x184 },
    { actor3Talk6Conditions, actor3Talk6Actions, 0x185 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x193 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x192 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x188 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x189 },
    { actor4Talk4Conditions, NULL, 0x18C },
    { actor4Talk5Conditions, actor4Talk5Actions, 0x18D },
    { actor4Talk6Conditions, actor4Talk6Actions, 0x18F },
    { actor4Talk7Conditions, NULL, 0x18E },
    { actor4Talk8Conditions, NULL, 0x190 },
    { actor4Talk9Conditions, actor4Talk9Actions, 0x191 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x298 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x188 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x192 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x193 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x189 },
    { actor7Talk1Conditions, NULL, 0x18C },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x18D },
    { actor7Talk3Conditions, NULL, 0x18E },
    { actor7Talk4Conditions, actor7Talk4Actions, 0x18F },
    { actor7Talk5Conditions, NULL, 0x190 },
    { actor7Talk6Conditions, actor7Talk6Actions, 0x191 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x17E },
    { actor8Talk1Conditions, NULL, 0x17E },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x17E },
    { actor8Talk3Conditions, NULL, 0x17E },
    { actor8Talk4Conditions, actor8Talk4Actions, 0x17E },
    { actor8Talk5Conditions, NULL, 0x17E },
    { actor8Talk6Conditions, actor8Talk6Actions, 0x17E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x18A },
    { actor9Talk1Conditions, NULL, 0x18A },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x18A },
    { actor9Talk3Conditions, NULL, 0x18A },
    { actor9Talk4Conditions, actor9Talk4Actions, 0x18A },
    { actor9Talk5Conditions, NULL, 0x18A },
    { actor9Talk6Conditions, actor9Talk6Actions, 0x18A },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x22), 1, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor2Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor4Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor6Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor7Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 464, 193, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 464, 193, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 4, 464, 193, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 4, 464, 193, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 5, 1088, 234, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x39, 5, 1088, 234, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x39, 5, 1088, 234, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x39, 5, 1088, 234, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 6, 464, 193, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9E, 7, 1088, 234, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 2, 0, 1, 6, 0, 603, 223, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 1, 6, 0, 952, 485, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 6, 0, 567, 357, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 6, 0, 604, 375, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 6, 0, 857, 450, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 1, 6, 0, 529, 660, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 201, 398, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 8, 0, 215, 598, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 140, 499, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 476, 616, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 569, 552, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 827, 661, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 408, 122, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 479, 678, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 772, 301, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 869, 528, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 919, 263, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 929, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 1166, 732, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 6, 0, 1218, 758, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 6, 0, 986, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 6, 0, 1025, 319, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 6, 0, 1195, 732, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 1, 6, 0, 522, 632, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 2, 0, 1, 6, 0, 599, 379, 0, 0 },
    { 1, 0, 0xC4, 4, 0, 0, 0, 0, 0, 0, 413, 523, 715, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 699, 190, 233, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 138, 383, 410, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 161, 328, 364, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 542, 196, 217, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 208, 311, 311, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 672, 719, 719, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 688, 455, 455, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 720, 743, 743, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 736, 479, 479, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 864, 175, 175, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 928, 351, 351, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 944, 167, 167, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x266, 0x390, 0x1E8, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x261, 0x80, 0x90, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x268, 0x390, 0x118, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x20F, 0xF8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x21F, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x34F, 0x106, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x35F, 0x16F, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x440, 0x161, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x430, 0x1C8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x3EF, 0x2C9, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x3E1, 0x32F, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x150, 0x258, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x15F, 0x2BE, 0, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 5, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0xA, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 6, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x16, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
