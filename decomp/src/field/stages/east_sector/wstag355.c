/*
 * WSTAG355: East Wire Forest, East Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 0x50
#include "common/start_stage.inc.c"

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
extern FieldBattles stageBattles2[];
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x1AD
#define STAGE_ARCHIVE 0x3C5
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x1BB
#define STAGE_ARCHIVE 0x3D5
#endif
/*
 * Sets the stage up: its map, actors and battles, playing the ambience
 * ENV05_00; battle list 0 before story step 0xB, 1 before story step 0x18, 2
 * from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x13800, 0x27100};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2D;
    FIELDSTG_state.music = MUSIC(0x2D, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0xB) {
        FIELDSTG_state.battles = stageBattles0;
    } else if (GAME.progress < 0x18) {
        FIELDSTG_state.battles = stageBattles1;
    } else {
        FIELDSTG_state.battles = stageBattles2;
    }
}

AreaBattle battles0Area0Battle0 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle1 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle2 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle3 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle4 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 41, 13, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area1Battles = {
    0,
    { &battles0Area1Battle0, &battles0Area1Battle1, &battles0Area1Battle2, &battles0Area1Battle3,
      &battles0Area1Battle4, &battles0Area1Battle5, &battles0Area1Battle6, &battles0Area1Battle7 },
};
AreaBattle battles0Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area2Battles = {
    0,
    { &battles0Area2Battle0, &battles0Area2Battle1, &battles0Area2Battle2, &battles0Area2Battle3,
      &battles0Area2Battle4, &battles0Area2Battle5, &battles0Area2Battle6, &battles0Area2Battle7 },
};
AreaBattle battles0Area3Battle0 = { 204, 13, MUSIC(3, 0) };
AreaBattle battles0Area3Battle1 = { 205, 13, MUSIC(3, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 45, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 45, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 57, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 57, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 57, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 57, 13, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area1Battles = {
    0,
    { &battles1Area1Battle0, &battles1Area1Battle1, &battles1Area1Battle2, &battles1Area1Battle3,
      &battles1Area1Battle4, &battles1Area1Battle5, &battles1Area1Battle6, &battles1Area1Battle7 },
};
AreaBattle battles1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area2Battles = {
    0,
    { &battles1Area2Battle0, &battles1Area2Battle1, &battles1Area2Battle2, &battles1Area2Battle3,
      &battles1Area2Battle4, &battles1Area2Battle5, &battles1Area2Battle6, &battles1Area2Battle7 },
};
AreaBattle battles1Area3Battle0 = { 204, 13, MUSIC(3, 0) };
AreaBattle battles1Area3Battle1 = { 205, 13, MUSIC(3, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
AreaBattle battles2Area0Battle0 = { 45, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle1 = { 45, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle2 = { 57, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle3 = { 57, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle4 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle5 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle6 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle7 = { 146, 13, MUSIC(2, 0) };
BattleList battles2Area0Battles = {
    3,
    { &battles2Area0Battle0, &battles2Area0Battle1, &battles2Area0Battle2, &battles2Area0Battle3,
      &battles2Area0Battle4, &battles2Area0Battle5, &battles2Area0Battle6, &battles2Area0Battle7 },
};
AreaBattle battles2Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles2Area1Battles = {
    0,
    { &battles2Area1Battle0, &battles2Area1Battle1, &battles2Area1Battle2, &battles2Area1Battle3,
      &battles2Area1Battle4, &battles2Area1Battle5, &battles2Area1Battle6, &battles2Area1Battle7 },
};
AreaBattle battles2Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles2Area2Battles = {
    0,
    { &battles2Area2Battle0, &battles2Area2Battle1, &battles2Area2Battle2, &battles2Area2Battle3,
      &battles2Area2Battle4, &battles2Area2Battle5, &battles2Area2Battle6, &battles2Area2Battle7 },
};
AreaBattle battles2Area3Battle0 = { 204, 13, MUSIC(3, 0) };
AreaBattle battles2Area3Battle1 = { 205, 13, MUSIC(3, 0) };
AreaBattle battles2Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles2Area3Battles = {
    0,
    { &battles2Area3Battle0, &battles2Area3Battle1, &battles2Area3Battle2, &battles2Area3Battle3,
      &battles2Area3Battle4, &battles2Area3Battle5, &battles2Area3Battle6, &battles2Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 6, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 23, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
FieldBattles stageBattles2[] = {
    { 52, 2, 0, { &battles2Area0Battles, &battles2Area1Battles, &battles2Area2Battles, &battles2Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16C, 0x146, 0xB0, 0x46, 0x150, 0x1FF },
    { 0x140, 0x100, 0x174, 0x146, 0xD0, 0x46, 0x160, 0x1FF },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x170, 0x16E, 0xC0, 0x6E, 0x170, 0x1FF },
    { 0x140, 0x100, 0x170, 0x18E, 0xC0, 0x8E, 0x150, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, SPECIAL(3), 1, CODES_END };
u16 actor0Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor0Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, SPECIAL(4), 1, CODES_END };
u16 actor0Talk3Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor0Talk4Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor0Talk4Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor0Talk5Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk6Actions[] = { CARD_BATTLE(9, 0), 1, CODES_END };
u16 actor0Talk7Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 5), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk7Actions[] = { FLAG(0xE, 5), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor0Talk8Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 5), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk9Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 5), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk10Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 5), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor0Talk10Actions[] = { CARD_BATTLE(9, 1), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(9, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    PARTY_STAT(3), 1,
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 5), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { FLAG(0xE, 5), 1, EVENT_BATTLE(1), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor1Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(9, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(9, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 5), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 5), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor2Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(9, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, SPECIAL(3), 1, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, SPECIAL(4), 1, CODES_END };
u16 actor5Talk3Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk4Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor5Talk4Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk5Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk6Actions[] = { CARD_BATTLE(8, 0), 1, CODES_END };
u16 actor5Talk7Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 4), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk7Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 4), 1, CODES_END };
u16 actor5Talk8Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    PARTY_STAT(1), 1,
    FLAG(0xE, 4), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk9Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk10Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk10Actions[] = { CARD_BATTLE(8, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(8, 0), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 4), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor7Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 4), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor7Talk5Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor7Talk6Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor7Talk6Actions[] = { CARD_BATTLE(8, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor8Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor8Talk2Actions[] = { CARD_BATTLE(8, 0), 1, CODES_END };
u16 actor8Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 4), 0,
    CODES_END,
};
u16 actor8Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 4), 1, CODES_END };
u16 actor8Talk4Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor8Talk5Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor8Talk6Conditions[] = {
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor8Talk6Actions[] = { CARD_BATTLE(8, 1), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor11Talk2Actions[] = { CARD_BATTLE(8, 0), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    PARTY_STAT(1), 1,
    FLAG(0xE, 4), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor11Talk3Actions[] = { FLAG(0xE, 4), 1, CODES_END };
u16 actor11Talk4Conditions[] = {
    ITEM(0, 0x12), 0,
    FLAG(0xE, 4), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor11Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 4), 1,
    CODES_END,
};
u16 actor11Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 4), 1,
    CODES_END,
};
u16 actor11Talk6Actions[] = { CARD_BATTLE(8, 1), 1, CODES_END };
u16 actor12Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor12Talk2Actions[] = { CARD_BATTLE(9, 0), 1, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 5), 0,
    CODES_END,
};
u16 actor12Talk3Actions[] = { FLAG(0xE, 5), 1, CODES_END };
u16 actor12Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor12Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor12Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 5), 1,
    CODES_END,
};
u16 actor12Talk6Actions[] = { CARD_BATTLE(9, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x5D },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x5C },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x52 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x53 },
    { actor0Talk4Conditions, actor0Talk4Actions, 0x5B },
    { actor0Talk5Conditions, NULL, 0x56 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0x57 },
    { actor0Talk7Conditions, actor0Talk7Actions, 0x58 },
    { actor0Talk8Conditions, NULL, 0x59 },
    { actor0Talk9Conditions, NULL, 0x5A },
    { actor0Talk10Conditions, actor0Talk10Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x53 },
    { actor1Talk1Conditions, NULL, 0x56 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x57 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x58 },
    { actor1Talk4Conditions, NULL, 0x59 },
    { actor1Talk5Conditions, NULL, 0x5A },
    { actor1Talk6Conditions, actor1Talk6Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x5B },
    { actor2Talk1Conditions, NULL, 0x56 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x57 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x58 },
    { actor2Talk4Conditions, NULL, 0x59 },
    { actor2Talk5Conditions, NULL, 0x5A },
    { actor2Talk6Conditions, actor2Talk6Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x52 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x5C },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x5D },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x274 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x51 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x50 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x46 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x47 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0x48 },
    { actor5Talk5Conditions, NULL, 0x4B },
    { actor5Talk6Conditions, actor5Talk6Actions, 0x4C },
    { actor5Talk7Conditions, actor5Talk7Actions, 0x4D },
    { actor5Talk8Conditions, NULL, 0x4E },
    { actor5Talk9Conditions, NULL, 0x4F },
    { actor5Talk10Conditions, actor5Talk10Actions, 0x78 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x46 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x50 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x51 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, actor7Talk0Actions, 0x47 },
    { actor7Talk1Conditions, NULL, 0x4B },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x4C },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x4D },
    { actor7Talk4Conditions, NULL, 0x4E },
    { actor7Talk5Conditions, NULL, 0x4F },
    { actor7Talk6Conditions, actor7Talk6Actions, 0x78 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, actor8Talk0Actions, 0x48 },
    { actor8Talk1Conditions, NULL, 0x4B },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x4C },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x4D },
    { actor8Talk4Conditions, NULL, 0x4E },
    { actor8Talk5Conditions, NULL, 0x4F },
    { actor8Talk6Conditions, actor8Talk6Actions, 0x78 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x273 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xF0 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x49 },
    { actor11Talk1Conditions, NULL, 0x49 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x49 },
    { actor11Talk3Conditions, actor11Talk3Actions, 0x49 },
    { actor11Talk4Conditions, NULL, 0x49 },
    { actor11Talk5Conditions, NULL, 0x49 },
    { actor11Talk6Conditions, actor11Talk6Actions, 0x49 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x54 },
    { actor12Talk1Conditions, NULL, 0x54 },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x54 },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x54 },
    { actor12Talk4Conditions, NULL, 0x54 },
    { actor12Talk5Conditions, NULL, 0x54 },
    { actor12Talk6Conditions, actor12Talk6Actions, 0x54 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor1Conditions[] = {
    ITEM(0, 0x192), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
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
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 0x192), 1, CODES_END };
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
u16 actor8Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor9Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x35, 4, 731, 350, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x35, 4, 731, 350, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x35, 4, 731, 350, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 4, 731, 350, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 4, 731, 350, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x38, 5, 545, 249, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x38, 5, 545, 249, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x38, 5, 545, 249, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x38, 5, 545, 249, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x38, 5, 545, 249, 1 };
FieldActorEntry actor10 = { NULL, actor10Talks, 0x3F, 6, 319, 600, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9D, 7, 545, 249, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 8, 731, 350, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 68, 778, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 6, 0, 159, 733, 0, 0 },
    { 1, 0, 0x80, 2, 4, 0, 0, 0, 0, 0, 13, 758, 0, 0 },
    { 1, 0x64, 0x40, 6, 2, 0, 0, 0, 0, 0, 89, 730, 0, 0 },
    { 1, 0, 0x72, 4, 0, 0, 0, 0, 0, 0, 0, 657, 795, 0 },
    { 1, 0, 0x76, 4, 1, 0, 0, 0, 0, 0, 40, 677, 795, 0 },
    { 1, 0, 0x60, 4, 3, 0, 0, 0, 0, 0, 290, 502, 595, 0 },
    { 1, 0, 0x80, 4, 5, 0, 0, 0, 0, 0, 63, 783, 795, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 103, 347, 347, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 119, 387, 387, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 156, 539, 539, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 168, 339, 339, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 169, 579, 579, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 184, 298, 298, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 185, 379, 379, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 206, 603, 603, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 232, 356, 356, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 238, 215, 215, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 249, 635, 635, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 280, 667, 667, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 296, 723, 723, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 312, 219, 219, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 360, 198, 198, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 392, 323, 323, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 400, 631, 631, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 423, 219, 219, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 424, 291, 291, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 440, 603, 603, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 472, 347, 347, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 487, 307, 307, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 552, 451, 451, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 595, 479, 479, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 597, 595, 595, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 597, 691, 691, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 600, 203, 203, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 608, 823, 823, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 623, 231, 231, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 625, 723, 723, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 642, 763, 763, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 648, 803, 803, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 680, 570, 570, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 728, 603, 603, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 760, 195, 195, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 833, 659, 659, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 840, 267, 267, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 845, 571, 571, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 871, 315, 315, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x221, 0x598, 0x260, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x227, 0x154, 0x6A, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x225, 0x80, 0x31C, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x223, 0x1B2, 0x156, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
