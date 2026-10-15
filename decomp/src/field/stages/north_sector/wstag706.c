/*
 * WSTAG706: Freeze Mountain, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG705.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x674
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x684
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0022 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x4BE00, 0x8800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3E;
    FIELDSTG_state.music = MUSIC(0x3E, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 184, 14, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 160, 14, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 160, 14, MUSIC(2, 0) };
BattleList area0Battles = {
    2,
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
AreaBattle area3Battle0 = { 247, 14, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 248, 14, MUSIC(2, 0) };
AreaBattle area3Battle2 = { 295, 14, MUSIC(3, 0) };
AreaBattle area3Battle3 = { 296, 14, MUSIC(3, 0) };
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
    { 114, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16A, 0x100, 0xA8, 0, 0x140, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x100, 0x68, 0, 0x150, 0x1FF },
    { 0x140, 0x100, 0x162, 0x100, 0x88, 0, 0x160, 0x1FF },
};
u16 actor0Talk0Actions[] = { SPECIAL(0x90), 1, FLAG(2, 0x52), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor1Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 0), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk4Conditions[] = {
    PARTY_STAT(0xA), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk4Actions[] = { CARD_BATTLE(0x3F, 0), 1, CODES_END };
u16 actor2Talk5Conditions[] = {
    FLAG(0xE, 0x32), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk5Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x32), 1, CODES_END };
u16 actor2Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x32), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk7Conditions[] = {
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x32), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk8Conditions[] = {
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x32), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 0), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk8Actions[] = { CARD_BATTLE(0x3F, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PARTY_STAT(0xB), 0, FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { PARTY_STAT(0xB), 1, FLAG(0, 0), 1, FLAG(0xE, 0x53), 0, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0xE, 0x53), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    PARTY_STAT(0xB), 1,
    FLAG(0, 0), 1,
    FLAG(0xE, 0x53), 1,
    PARTY_STAT(0xD), 0,
    CODES_END,
};
u16 actor3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x53), 1,
    PARTY_STAT(0xD), 1,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x3F, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor5Talk3Conditions[] = { PARTY_STAT(8), 0, FLAG(0, 1), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk4Conditions[] = {
    PARTY_STAT(0xA), 0,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk4Actions[] = { CARD_BATTLE(0x40, 0), 1, CODES_END };
u16 actor5Talk5Conditions[] = {
    FLAG(0xE, 0x33), 0,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk5Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x33), 1, CODES_END };
u16 actor5Talk6Conditions[] = {
    ITEM(0, 0x14), 0,
    FLAG(0xE, 0x33), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk7Conditions[] = {
    PARTY_STAT(0xB), 0,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x33), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk8Conditions[] = {
    PARTY_STAT(0xB), 1,
    ITEM(0, 0x14), 1,
    FLAG(0xE, 0x33), 1,
    PARTY_STAT(0xA), 1,
    PARTY_STAT(8), 1,
    FLAG(0, 1), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor5Talk8Actions[] = { CARD_BATTLE(0x40, 1), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 1, FLAG(0xE, 0x54), 0, CODES_END };
u16 actor6Talk2Actions[] = { EVENT_BATTLE(1), 1, FLAG(0xE, 0x54), 1, CODES_END };
u16 actor6Talk3Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x54), 1,
    PARTY_STAT(0xD), 0,
    CODES_END,
};
u16 actor6Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x54), 1,
    PARTY_STAT(0xD), 1,
    CODES_END,
};
u16 actor6Talk4Actions[] = { CARD_BATTLE(0x40, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x345 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x39C },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x249 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x22A },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x229 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x224 },
    { actor2Talk3Conditions, NULL, 0x225 },
    { actor2Talk4Conditions, actor2Talk4Actions, 0x226 },
    { actor2Talk5Conditions, actor2Talk5Actions, 0x227 },
    { actor2Talk6Conditions, NULL, 0x228 },
    { actor2Talk7Conditions, NULL, 0x225 },
    { actor2Talk8Conditions, actor2Talk8Actions, 0x226 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x24C },
    { actor3Talk1Conditions, NULL, 0x24E },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x24D },
    { actor3Talk3Conditions, NULL, 0x248 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x24A },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x249 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x244 },
    { actor5Talk3Conditions, NULL, 0x245 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0x246 },
    { actor5Talk5Conditions, actor5Talk5Actions, 0x247 },
    { actor5Talk6Conditions, NULL, 0x248 },
    { actor5Talk7Conditions, NULL, 0x24B },
    { actor5Talk8Conditions, actor5Talk8Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x22C },
    { actor6Talk1Conditions, NULL, 0x22E },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x22D },
    { actor6Talk3Conditions, NULL, 0x228 },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x22F },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x39D },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x229 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x22A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x22B },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x52), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(0x19), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x19), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x19), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 1281, 401, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x45, 5, 561, 841, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x45, 5, 561, 841, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x45, 5, 561, 841, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x45, 5, 561, 841, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x46, 6, 992, 273, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x46, 6, 992, 273, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x46, 6, 992, 273, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x46, 6, 992, 273, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 834, 768, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 889, 248, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 998, 143, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 1192, 110, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2D5, 0xE0, 0x3A8, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2CA, 0x3A0, 0xA0, 1, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x18, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E9, 0xB0, 0xF8, 7, 0, 0xD, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 5, 3 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0xB, 2 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x420, 0x140, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x410, 0x1D8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x460, 0x2A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x450, 0x338, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x430, 0x288, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x420, 0x2F0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x36F, 0x1D8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x37F, 0x240, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 9, 0x32F, 0x288, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 9, 0x33F, 0x320, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x1F0, 0x338, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x1E0, 0x3A0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0xD, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 7, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 0xA, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
