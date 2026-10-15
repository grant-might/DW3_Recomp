/*
 * WSTAG650: Mirage Hall, West Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1458, with Guardromon: applies 0x7C15, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1458(void) {
    FLAGS_00.applyAction(0x7C15, 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x53E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x54E
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0020;
 * BGM_0029 instead at story steps 0x27 to 0x28 and 0xF to 0x17
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xEF00, 0x25800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x16;
    FIELDSTG_state.music = MUSIC(0x16, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0xF && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script1458[] = {
    SCRIPT_WALK(2, 0xD7, 0x129, 3),
    SCRIPT_PLACE(0x15, 0xB7, 0x119),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC13, 0, 0, 0),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x3C04,
#elif VERSION_EU
    0x323,
#elif VERSION_JP
    0x3E0,
#endif
};
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
AreaBattle area3Battle0 = { 216, 20, MUSIC(3, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
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
    { 158, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x164, 0x1A7, 0x90, 0xA7, 0x160, 0x1FF },
    { 0x140, 0x100, 0x150, 0x178, 0x40, 0x78, 0x170, 0x1FF },
    { 0x140, 0x100, 0x140, 0x18B, 0, 0x8B, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16C, 0x1A5, 0xB0, 0xA5, 0x170, 0x1FE },
    { 0x140, 0x100, 0x174, 0x1A5, 0xD0, 0xA5, 0x160, 0x1FD },
    { 0x140, 0x100, 0x15C, 0x1A7, 0x70, 0xA7, 0x170, 0x1FD },
    { 0x140, 0x100, 0x14A, 0x1A8, 0x28, 0xA8, 0x160, 0x1FC },
};
u16 actor0Talk0Actions[] = { 0x7A2D, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x12), 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7A0A, 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor13Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor13Talk2Actions[] = { CARD_BATTLE(0x1A, 0), 1, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor14Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor15Talk2Actions[] = { CARD_BATTLE(0x1A, 0), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(0x1A, 0), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x10), 0, CODES_END };
u16 actor17Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x10), 1, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x10), 1, PARTY_STAT(0xD), 0, CODES_END };
u16 actor17Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x10), 1, PARTY_STAT(0xD), 1, CODES_END };
u16 actor17Talk3Actions[] = { CARD_BATTLE(0x1A, 1), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor18Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk2Conditions[] = { FLAG(0, 0x10), 1, CARD(1), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor18Talk2Actions[] = {
    CARD(1), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(0x13), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor18Talk3Conditions[] = { FLAG(0, 0x10), 1, CARD(1), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor18Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor20Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor20Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, CODES_END };
u16 actor20Talk2Actions[] = { CARD_BATTLE(0x1A, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16B },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x168 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x2D7 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x27 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x15F },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x160 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x161 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x160 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x28 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x15C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x15D },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x15D },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x15E },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0xCD },
    { actor13Talk1Conditions, NULL, 0xD1 },
    { actor13Talk2Conditions, actor13Talk2Actions, 0xD2 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0xCD },
    { actor14Talk1Conditions, actor14Talk1Actions, 0xD7 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0xD8 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0xCE },
    { actor15Talk1Conditions, NULL, 0xD1 },
    { actor15Talk2Conditions, actor15Talk2Actions, 0xD2 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xCF },
    { actor16Talk1Conditions, NULL, 0xD1 },
    { actor16Talk2Conditions, actor16Talk2Actions, 0xD2 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0xD3 },
    { actor17Talk1Conditions, actor17Talk1Actions, 0xD4 },
    { actor17Talk2Conditions, NULL, 0xD5 },
    { actor17Talk3Conditions, actor17Talk3Actions, 0xD6 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0xCD },
    { actor18Talk1Conditions, actor18Talk1Actions, 0xD9 },
    { actor18Talk2Conditions, actor18Talk2Actions, 0xDA },
    { actor18Talk3Conditions, actor18Talk3Actions, 0xDB },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x284 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, NULL, 0xD0 },
    { actor20Talk1Conditions, NULL, 0xD0 },
    { actor20Talk2Conditions, actor20Talk2Actions, 0xD0 },
    { NULL, NULL, 0 },
};
u16 actor3Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x11), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x11), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor13Conditions[] = {
    FLAG(0, 0x11), 0,
    SPECIAL(3), 1,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor14Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    SPECIAL(0xA), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor15Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    SPECIAL(4), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor16Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    PROGRESS(0x26), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor17Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x12), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor18Conditions[] = {
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 1,
    ITEM(0, 0x12), 1,
    SPECIAL(0x22), 1,
    CODES_END,
};
u16 actor19Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 321, 593, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 183, 281, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x17, 6, 528, 305, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x20, 7, 353, 442, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x20, 7, 353, 442, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x20, 7, 353, 442, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x20, 7, 353, 442, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x20, 7, 353, 442, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x30, 8, 537, 493, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x30, 8, 537, 493, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x30, 8, 537, 493, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x30, 8, 537, 493, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x30, 8, 537, 493, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x33, 9, 401, 529, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9D, 0xA, 401, 529, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 7, 0xE, 0, 252, 269, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 7, 0xE, 0, 482, 625, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 195, 353, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 339, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 7, 0xE, 0, 539, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 7, 0xE, 0, 366, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 7, 0xE, 0, 475, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 2, 0, 0xB, 0xA, 0, 164, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 0xB, 0xA, 0, 164, 354, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 0xB, 0xA, 0, 508, 337, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 0xB, 0xA, 0, 512, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 2, 0, 0xB, 0xA, 0, 568, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 2, 0, 0xB, 0xA, 0, 481, 426, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 0xB, 0xA, 0, 453, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 0xB, 0xA, 0, 346, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 2, 0, 0xB, 0xA, 0, 342, 488, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 311, 96, 151, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 318, 579, 600, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 335, 572, 595, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25C, 0xA8, 0x1B4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25A, 0x298, 0x104, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xFD, 0xD0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xEC, 0x118, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 8, 0x133, 0xC8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 8, 0x142, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x192, 0x148, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1A2, 0x190, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1C2, 0x1A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1D2, 0x1E8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x25C, 0x160, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x24C, 0x1A8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1AC, 0x228, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x19C, 0x270, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 9, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1458, script1458, EVENT_TEXT(0x1E), NULL, endEvent1458 },
    { -1, NULL, 0, NULL, NULL },
};
