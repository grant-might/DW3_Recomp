/*
 * WSTAG335: Wire Forest Entrance, Central Sector.
 */

#include "common.h"
#include "field/stage.h"

/* The stage's task: a loop over its 14 children with nothing left in it (no object is created) */
void updateStage(StageTask *task, void **children) {
    s32 i;

    switch (task->state) {
    case TASK_INIT:
    default:
        for (i = 0; i < 14; i++) {
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x38
#include "common/start_stage.inc.c"

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
extern FieldBattles stageBattles2[];
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x1A9
#define STAGE_ARCHIVE 0x2C3
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x1B7
#define STAGE_ARCHIVE 0x2D2
#endif
/*
 * Sets the stage up: its map, actors and battles, playing FIELDBGM; BGM_0029
 * instead at story steps 0x27 to 0x28; battle list 0 before story step 0xB,
 * 1 before story step 0x18, 2 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x19000, 0x12C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 9;
    FIELDSTG_state.music = MUSIC(9, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
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

AreaBattle battles0Area0Battle0 = { 34, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle1 = { 34, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle2 = { 34, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle3 = { 34, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle4 = { 35, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 35, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 35, 1, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 35, 1, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle1 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle2 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle3 = { 34, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle4 = { 35, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle5 = { 35, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle6 = { 35, 13, MUSIC(2, 0) };
AreaBattle battles0Area1Battle7 = { 35, 13, MUSIC(2, 0) };
BattleList battles0Area1Battles = {
    3,
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
AreaBattle battles0Area3Battle0 = { 202, 13, MUSIC(3, 0) };
AreaBattle battles0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles0Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 35, 1, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 35, 1, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle1 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle2 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle3 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle4 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle5 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle6 = { 35, 13, MUSIC(2, 0) };
AreaBattle battles1Area1Battle7 = { 35, 13, MUSIC(2, 0) };
BattleList battles1Area1Battles = {
    3,
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
AreaBattle battles1Area3Battle0 = { 202, 13, MUSIC(3, 0) };
AreaBattle battles1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles1Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
AreaBattle battles2Area0Battle0 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle1 = { 48, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle2 = { 146, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle3 = { 146, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle4 = { 146, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle5 = { 146, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle6 = { 146, 1, MUSIC(2, 0) };
AreaBattle battles2Area0Battle7 = { 146, 1, MUSIC(2, 0) };
BattleList battles2Area0Battles = {
    3,
    { &battles2Area0Battle0, &battles2Area0Battle1, &battles2Area0Battle2, &battles2Area0Battle3,
      &battles2Area0Battle4, &battles2Area0Battle5, &battles2Area0Battle6, &battles2Area0Battle7 },
};
AreaBattle battles2Area1Battle0 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle1 = { 48, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle2 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle3 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle4 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle5 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle6 = { 146, 13, MUSIC(2, 0) };
AreaBattle battles2Area1Battle7 = { 146, 13, MUSIC(2, 0) };
BattleList battles2Area1Battles = {
    3,
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
AreaBattle battles2Area3Battle0 = { 202, 13, MUSIC(3, 0) };
AreaBattle battles2Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle3 = { 327, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle battles2Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles2Area3Battle6 = { 49, 13, MUSIC(2, 0) };
AreaBattle battles2Area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList battles2Area3Battles = {
    0,
    { &battles2Area3Battle0, &battles2Area3Battle1, &battles2Area3Battle2, &battles2Area3Battle3,
      &battles2Area3Battle4, &battles2Area3Battle5, &battles2Area3Battle6, &battles2Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 2, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 25, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
FieldBattles stageBattles2[] = {
    { 50, 2, 0, { &battles2Area0Battles, &battles2Area1Battles, &battles2Area2Battles, &battles2Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16C, 0x100, 0xB0, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x160, 0x1FF },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor0Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(6, 0), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PARTY_STAT(9), 0, FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { PARTY_STAT(9), 1, FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(6, 0), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(6, 0), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 2), 0, CODES_END };
u16 actor4Talk1Actions[] = { FLAG(0xE, 2), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 2), 1, PARTY_STAT(0xD), 0, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 2), 1, PARTY_STAT(0xD), 1, CODES_END };
u16 actor4Talk3Actions[] = { CARD_BATTLE(6, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x10), 1, CARD(0xD), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor5Talk2Actions[] = {
    CARD(0xD), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x13), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor5Talk3Conditions[] = { FLAG(0, 0x10), 1, CARD(0xD), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor5Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(6, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x6C },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x29 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x2A },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x6C },
    { actor1Talk1Conditions, NULL, 0x70 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x71 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x6D },
    { actor2Talk1Conditions, NULL, 0x70 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x71 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x6E },
    { actor3Talk1Conditions, NULL, 0x70 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x71 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x74 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x75 },
    { actor4Talk2Conditions, NULL, 0x76 },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x77 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x6C },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x2B },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x2C },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x2D },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x271 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x6F },
    { actor7Talk1Conditions, NULL, 0x6F },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x6F },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    FLAG(0, 0x11), 1,
    ITEM(0, 0x192), 1,
    SPECIAL(0xA), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor1Conditions[] = {
    SPECIAL(3), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    SPECIAL(4), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    PROGRESS(0x26), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor4Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x12), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor5Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    ITEM(0, 0x12), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor6Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2F, 4, 593, 289, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 5, 593, 289, 1 };
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
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 199, 607, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 353, -16, 0, 0 },
    { 1, 0, 0x40, 2, 0, 1, 0, 5, 8, 0, 513, 285, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 113, 393, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 153, 554, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 167, 539, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 192, 494, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 225, 493, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 251, 483, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 313, 468, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 86, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 169, 500, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 193, 527, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 216, 519, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 235, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 248, 454, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 253, 491, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 264, 469, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 58, 404, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 130, 393, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 214, 356, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 233, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 204, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 273, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 219, 483, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 289, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 307, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 317, 385, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 323, 421, 0, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 184, 226, 226, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 232, 250, 250, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 243, 544, 544, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 267, 602, 602, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 271, 135, 135, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 277, 521, 521, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 320, 159, 159, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 399, 167, 167, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 422, 187, 187, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 431, 568, 568, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 489, 501, 501, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 506, 203, 203, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 542, 228, 228, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 641, 286, 286, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21D, 0x5F4, 0x39E, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x221, 0x138, 0x7A, 7, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC8, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
