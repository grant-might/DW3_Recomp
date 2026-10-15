/*
 * WSTAG611: Secret Stairs, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG610.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x60B
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x61B
#endif
/* Sets the stage up: its map, actors and battles, playing the ambience ENV_0018 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xBD00, 0x25800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3A;
    FIELDSTG_state.music = MUSIC(0x3A, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

AreaBattle area0Battle0 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 116, 12, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 164, 12, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 164, 12, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 164, 12, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 164, 12, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
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
    { 112, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1A2, 0x176, 0x188, 0x76, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B2, 0x146, 0x1C8, 0x46, 0x150, 0x1FE },
    { 0x180, 0x100, 0x1B2, 0x16E, 0x1C8, 0x6E, 0x160, 0x1FE },
    { 0x180, 0x100, 0x19A, 0x176, 0x168, 0x76, 0x170, 0x1FE },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x118 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x116 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x117 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x117 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x115 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x115 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 625, 171, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x33, 5, 529, 567, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x9D, 6, 625, 171, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x9D, 6, 625, 171, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9E, 7, 529, 567, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9E, 7, 529, 567, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 1, 0, 0, 0, 0, 0, 512, 0, 0, 0 },
    { 1, 0, 0x80, 2, 2, 0, 0, 0, 0, 0, 463, 0, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x37, 4, 0, 180, 506, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x37, 4, 0, 249, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3D, 4, 0, 160, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3D, 4, 0, 229, 368, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 1, 4, 0, 388, 395, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 1, 4, 0, 429, 415, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 1, 4, 0, 441, 565, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 1, 4, 0, 468, 435, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 2, 0, 1, 4, 0, 482, 585, 0, 0 },
    { 1, 0, 0x40, 4, 0x3F, 2, 0, 1, 4, 0, 460, 138, 584, 0 },
    { 1, 0, 0x40, 4, 0x3F, 2, 0, 1, 4, 0, 492, 154, 584, 0 },
    { 1, 0, 0x40, 4, 0x3F, 2, 0, 1, 4, 0, 586, 201, 584, 0 },
    { 1, 0, 0x65, 4, 0, 0, 0, 0, 0, 0, 84, 484, 584, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 489, 136, 190, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 509, 136, 198, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 525, 136, 207, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 541, 136, 215, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2B7, 0x88, 0x22C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2BE, 0x3F0, 0x2F0, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
