/*
 * WSTAG466: Tranquil Swamp, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG465.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5A2
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5B2
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0012 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x3A700, 0x1D400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x34;
    FIELDSTG_state.music = MUSIC(0x34, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 159, 8, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 159, 8, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 231, 8, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 279, 8, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 331, 8, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 332, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 177, 8, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 106, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 72, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x149, 0xD8, 0x49, 0x150, 0x1FF },
    { 0x140, 0x100, 0x176, 0x171, 0xD8, 0x71, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x199, 0xD8, 0x99, 0x140, 0x1FE },
};
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(5), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(5), 1, PARTY_STAT(8), 0, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x2F, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x22), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x22), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(5), 1,
    PARTY_STAT(8), 1,
    FLAG(0xE, 0x22), 1,
    CODES_END,
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, FLAG(0xE, 0x43), 0, CODES_END };
u16 actor2Talk2Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x43), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x43), 1,
    PARTY_STAT(0xB), 0,
    CODES_END,
};
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    FLAG(0xE, 0x43), 1,
    PARTY_STAT(0xB), 1,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x2F, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0xEA },
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
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xE9 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xEB },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    ITEM(0, 0x192), 1,
    SPECIAL(0x19), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actor3Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x36, 4, 897, 514, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 690, 251, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 690, 251, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 690, 251, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 690, 251, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9D, 6, 897, 514, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 6, 897, 514, 1 };
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
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 1100, 594, 0, 0 },
    { 1, 0, 0x40, 2, 0x12, 0, 0, 0, 0, 0, 1107, 563, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 1187, 603, 0, 0 },
    { 1, 0, 0x40, 2, 0x14, 0, 0, 0, 0, 0, 545, 152, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 273, 403, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 799, 391, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1031, 584, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 101, 298, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 401, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 429, 522, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 534, 322, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 619, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 821, 283, 0, 0 },
    { 1, 0, 0x40, 6, 0x15, 0, 0, 0, 0, 0, 185, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x16, 0, 0, 0, 0, 0, 233, 298, 0, 0 },
    { 1, 0, 0x40, 6, 0x17, 0, 0, 0, 0, 0, 281, 322, 0, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 1072, 384, 422, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 1104, 368, 412, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 1120, 368, 404, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 1136, 368, 396, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 1152, 352, 389, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 1168, 352, 381, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 400, 144, 191, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 384, 160, 197, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 446, 290, 318, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 766, 258, 287, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 486, 160, 189, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 1099, 491, 511, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 207, 286, 286, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 255, 310, 310, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 303, 334, 334, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 735, 581, 581, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 799, 581, 581, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 864, 470, 470, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 912, 446, 446, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 960, 422, 422, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A2, 0x340, 0x90, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A6, 0xA8, 0x132, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A5, 0x1F2, 0x164, 3, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x48, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x38, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x38, 0xFFF8, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
