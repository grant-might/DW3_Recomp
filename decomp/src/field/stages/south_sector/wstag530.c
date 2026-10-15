/*
 * WSTAG530: Jungle Shrine, South Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* Defined below, after the code that uses them */
extern StageSlot stageSlots0[];
extern StageSlot stageSlots1[];
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x26A
#define STAGE_ARCHIVE 0x3CD
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x279
#define STAGE_ARCHIVE 0x3DD
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0015;
 * slot list 0 before story step 0x1A, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x10800, 0xED00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x11;
    FIELDSTG_state.music = MUSIC(0x11, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0x1A) {
        FIELDSTG_state.slots = stageSlots0;
    } else {
        FIELDSTG_state.slots = stageSlots1;
    }
}

AreaBattle area0Battle0 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 165, 6, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 165, 6, MUSIC(2, 0) };
BattleList area0Battles = {
    5,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 6, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 6, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 165, 6, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 58, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x150, 0x1FA },
};
u16 actor0Talk0Actions[] = { FLAG(2, 5), 1, ITEM(1, 0x2B), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x16D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 5), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 671, 176, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 477, 371, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 508, 386, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 512, 353, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 543, 368, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 574, 419, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 604, 434, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 608, 401, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 3, 4, 0, 639, 416, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 438, 68, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 469, 367, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 500, 383, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 502, 120, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 504, 349, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 522, 110, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 535, 364, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 566, 415, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 596, 431, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 600, 397, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 631, 412, 0, 0 },
    { 1, 0, 0x40, 2, 0x3A, 2, 0, 3, 4, 0, 822, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x37, 6, 0, 788, 446, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 2, 0, 5, 6, 0, 768, 423, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 426, 82, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 446, 72, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 510, 124, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 530, 114, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 747, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 830, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 830, 475, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 4, 0, 913, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 4, 0, 418, 79, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 4, 0, 739, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 4, 0, 822, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 4, 0, 905, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 2, 0, 3, 4, 0, 514, 60, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 2, 0, 3, 4, 0, 510, 56, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 3, 4, 0, 429, 28, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 3, 4, 0, 423, 20, 0, 0 },
    { 1, 0, 0x50, 4, 0, 0, 0, 0, 0, 0, 474, 362, 406, 0 },
    { 1, 0, 0x50, 4, 1, 0, 0, 0, 0, 0, 571, 410, 456, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots0[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23C, 0x3C8, 0xC4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x243, 0x98, 0x8C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x221, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x211, 0x158, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots1[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23C, 0x3C8, 0xC4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x244, 0x98, 0x8C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x221, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x211, 0x158, 0, 0, 0, 0 },
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
