/*
 * WSTAG350: West Wire Forest, East Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
extern FieldBattles stageBattles2[];
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x1AB
#define STAGE_ARCHIVE 0x3C4
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x1B9
#define STAGE_ARCHIVE 0x3D4
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV05_00; BGM_0029 instead at story steps 0x27 to 0x28; battle
 * list 0 before story step 0xB, 1 before story step 0x18, 2 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x2D600, 0x8000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2D;
    FIELDSTG_state.music = MUSIC(0x2D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
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
AreaBattle battles0Area0Battle4 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 41, 13, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle1 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle2 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle3 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle4 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle5 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle6 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle7 = { 0, 13, MUSIC(2, 0) };
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
AreaBattle battles0Area3Battle0 = { 203, 13, MUSIC(3, 0) };
AreaBattle battles0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles0Area3Battle5 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 48, 13, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle1 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle2 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle3 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle4 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle5 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle6 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle7 = { 0, 13, MUSIC(2, 0) };
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
AreaBattle battles1Area3Battle0 = { 203, 13, MUSIC(3, 0) };
AreaBattle battles1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles1Area3Battle5 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
AreaBattle battles2Area0Battle0 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle1 = { 39, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle2 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle3 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle4 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle5 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle6 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area0Battle7 = { 146, 13, MUSIC(2, 0) };
BattleList battles2Area0Battles = {
    3,
    { &battles2Area0Battle0, &battles2Area0Battle1, &battles2Area0Battle2, &battles2Area0Battle3,
      &battles2Area0Battle4, &battles2Area0Battle5, &battles2Area0Battle6, &battles2Area0Battle7 },
};
AreaBattle battles2Area1Battle0 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle1 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle2 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle3 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle4 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle5 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle6 = { 0, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle7 = { 0, 13, MUSIC(2, 0) };
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
AreaBattle battles2Area3Battle0 = { 203, 13, MUSIC(3, 0) };
AreaBattle battles2Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles2Area3Battle5 = { 41, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles2Area3Battles = {
    0,
    { &battles2Area3Battle0, &battles2Area3Battle1, &battles2Area3Battle2, &battles2Area3Battle3,
      &battles2Area3Battle4, &battles2Area3Battle5, &battles2Area3Battle6, &battles2Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 5, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 22, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
FieldBattles stageBattles2[] = {
    { 51, 2, 0, { &battles2Area0Battles, &battles2Area1Battles, &battles2Area2Battles, &battles2Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x100, 0xB0, 0, 0x160, 0x1FF },
    { 0x140, 0x100, 0x174, 0x160, 0xD0, 0x60, 0x170, 0x1FF },
};
u16 actor0Talk0Actions[] = { FLAG(2, 2), 1, ITEM(5, 0x113), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(7, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 3), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { FLAG(0xE, 3), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor1Talk5Conditions[] = {
    PARTY_STAT(5), 0,
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 3), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(7, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(7, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 3), 0,
    CODES_END,
};
u16 actor3Talk3Actions[] = { FLAG(0xE, 3), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 3), 1,
    CODES_END,
};
u16 actor3Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 3), 1,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(3), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(7, 1), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(7, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 3), 0,
    CODES_END,
};
u16 actor4Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 3), 1, CODES_END };
u16 actor4Talk4Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor4Talk5Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor4Talk6Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor4Talk6Actions[] = { CARD_BATTLE(7, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor6Talk2Actions[] = { CARD_BATTLE(7, 0), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 3), 0,
    CODES_END,
};
u16 actor6Talk3Actions[] = { FLAG(0xE, 3), 1, CODES_END };
u16 actor6Talk4Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor6Talk5Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor6Talk6Conditions[] = {
    FLAG(0xE, 3), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor6Talk6Actions[] = { CARD_BATTLE(7, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x3A },
    { actor1Talk1Conditions, NULL, 0x3F },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x40 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x41 },
    { actor1Talk4Conditions, NULL, 0x42 },
    { actor1Talk5Conditions, NULL, 0x43 },
    { actor1Talk6Conditions, actor1Talk6Actions, 0x6B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x3A },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x44 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x45 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x3B },
    { actor3Talk1Conditions, NULL, 0x3F },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x40 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x41 },
    { actor3Talk4Conditions, NULL, 0x42 },
    { actor3Talk5Conditions, NULL, 0x43 },
    { actor3Talk6Conditions, actor3Talk6Actions, 0x6B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x3C },
    { actor4Talk1Conditions, NULL, 0x3F },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x40 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x41 },
    { actor4Talk4Conditions, NULL, 0x42 },
    { actor4Talk5Conditions, NULL, 0x43 },
    { actor4Talk6Conditions, actor4Talk6Actions, 0x6B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x272 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x3D },
    { actor6Talk1Conditions, NULL, 0x3D },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x3D },
    { actor6Talk3Conditions, actor6Talk3Actions, 0x3D },
    { actor6Talk4Conditions, NULL, 0x3D },
    { actor6Talk5Conditions, NULL, 0x3D },
    { actor6Talk6Conditions, actor6Talk6Actions, 0x3D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 2), 0, CODES_END };
u16 actor1Conditions[] = { SPECIAL(3), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(4), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 1353, 405, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 5, 241, 297, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 5, 241, 297, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 5, 241, 297, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 5, 241, 297, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 5, 241, 297, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 6, 241, 297, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 313, 257, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 730, 365, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 932, 55, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 161, 378, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 83, 403, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 50, 403, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 182, 431, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 196, 377, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 208, 446, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 188, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 230, 394, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 273, 422, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 129, 383, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x44, 0x46, 0xA, 0, 196, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 248, 483, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 272, 461, 0, 0 },
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21E, 0x28C, 0x33C, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x222, 0x8C, 0xCE, 7, 0, 0, 0 },
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
