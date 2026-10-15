/*
 * WSTAG551: South Badland, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG550.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x5C2
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x5D2
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0018;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xCA00, 0x9800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x14;
    FIELDSTG_state.music = MUSIC(0x14, 0);
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

s16 script1226[] = {
    SCRIPT_WALK(2, 0x190, 0xE0, 3),
    SCRIPT_PLACE(0x15, 0x171, 0xD1),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC0E, 0, 0, 0),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 132, 3, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 133, 3, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 133, 3, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 169, 3, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 169, 3, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 3, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 3, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 234, 3, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 282, 3, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 334, 2, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 169, 3, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 180, 2, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 105, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x160, 0x145, 0x80, 0x45, 0x150, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x18D, 0xB8, 0x8D, 0x160, 0x1FF },
};
u16 actor0Talk0Actions[] = { START_EVENT(0x12), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(7), 0, CODES_END };
u16 actor1Talk2Conditions[] = { PARTY_STAT(7), 1, FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x32, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x25), 0,
    PARTY_STAT(7), 1,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x25), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    PARTY_STAT(7), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x25), 1,
    CODES_END,
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PARTY_STAT(0xA), 0, FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x46), 0, PARTY_STAT(0xA), 1, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x46), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    PARTY_STAT(0xA), 1,
    FLAG(0, 0), 1,
    FLAG(0xE, 0x46), 1,
    PARTY_STAT(0xC), 0,
    CODES_END,
};
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    FLAG(0xE, 0x46), 1,
    PARTY_STAT(0xC), 1,
    PARTY_STAT(0xA), 1,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x32, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x244 },
    { actor1Talk1Conditions, NULL, 0x245 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x246 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x247 },
    { actor1Talk4Conditions, NULL, 0x248 },
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
    { actor3Talk0Conditions, NULL, 0x39D },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x249 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor1Conditions[] = {
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor3Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x15, 4, 369, 209, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 752, 690, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 752, 690, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 752, 690, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 752, 690, 5 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 40, 512, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 472, 160, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 362, 998, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 634, 946, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 964, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1102, 947, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1169, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 202, 926, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 495, 1034, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 986, 1028, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1213, 654, 0, 0 },
    { 1, 0, 0x40, 6, 0, 0, 0, 0, 0, 0, 678, 464, 0, 0 },
    { 1, 0, 0x4A, 6, 2, 0, 0, 0, 0, 0, 1178, 782, 0, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 697, 464, 478, 0 },
    { 1, 0, 0x4B, 4, 1, 0, 0, 0, 0, 0, 160, 219, 243, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B2, 0x610, 0x540, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 3, 5 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 6, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0xB, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x479, 0x2B0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x469, 0x318, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0x13, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFE0, 0x2C, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x20, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xC, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFEC, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFE0, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 0x13, 1 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1226, script1226, EVENT_TEXT(0xB), NULL, NULL },
    { 9000, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
