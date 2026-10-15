/*
 * WSTAG715: Lake of Ice, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define STAGE_FILE 0x726
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define STAGE_FILE 0x736
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0022 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x36300, 0x1C700};
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

AreaBattle area0Battle0 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 115, 14, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 115, 14, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 333, 14, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 332, 14, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 114, 14, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 179, 14, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 85, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x168, 0x1A5, 0xA0, 0xA5, 0x160, 0x1FF },
    { 0x140, 0x100, 0x170, 0x1A5, 0xC0, 0xA5, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15C, 0x1A8, 0x70, 0xA8, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x1B1, 0, 0xB1, 0x160, 0x1FE },
    { 0x140, 0x100, 0x148, 0x1B1, 0x20, 0xB1, 0x170, 0x1FE },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x213 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x216 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x214 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x217 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x218 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x21A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x21B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x21E },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x21D },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x21C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x215 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x219 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x31, 4, 433, 393, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x31, 4, 256, 431, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x31, 4, 433, 393, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 5, 272, 209, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 5, 272, 209, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x35, 5, 512, 519, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x40, 6, 529, 392, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x40, 6, 272, 209, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x40, 6, 529, 392, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x40, 6, 529, 392, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 7, 433, 393, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 8, 272, 209, 5 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 49, 138, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 128, 512, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 222, 441, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 600, 352, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 615, 376, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x38, 4, 0, 640, 368, 0, 0 },
    { 1, 0, 0x40, 2, 0x39, 2, 0, 5, 6, 0, 21, 133, 0, 0 },
    { 1, 0, 0x40, 2, 3, 1, 3, 8, 4, 0, 801, 513, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x38, 4, 0, 243, 53, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x38, 4, 0, 270, 56, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 5, 6, 0, 234, 51, 0, 0 },
    { 1, 0, 0x40, 4, 0x3B, 0, 0, 0, 0, 0, 406, 413, 478, 0 },
    { 1, 0, 0x40, 4, 0x3C, 0, 0, 0, 0, 0, 360, 401, 458, 0 },
    { 1, 0, 0x40, 4, 0x46, 1, 0x46, 0x51, 0xC, 0, 381, 406, 458, 0 },
    { 1, 0, 0x40, 4, 0x3E, 0, 0, 0, 0, 0, 406, 388, 438, 0 },
    { 1, 0, 0x40, 4, 0x3F, 0, 0, 0, 0, 0, 433, 401, 458, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 128, 120, 175, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 235, 368, 374, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 665, 307, 322, 0 },
    { 1, 0, 0x4F, 4, 9, 0, 0, 0, 0, 0, 65, 377, 445, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 304, 152, 152, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 352, 128, 128, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 592, 199, 199, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 656, 183, 183, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 704, 207, 207, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 736, 239, 239, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 784, 263, 263, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x265, 0xB4, 0x98, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x267, 0x298, 0x134, 3, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 6, 1 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 5, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x170, 0x98, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x180, 0xE0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x48, 0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x48, 0xFFDC, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFC8, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
