/*
 * WSTAG370: Protocol Forest, East Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* Defined below, after the code that uses them */
extern FieldBattles stageBattles0[];
extern FieldBattles stageBattles1[];
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define STAGE_FILE 0x1AF
#define STAGE_ARCHIVE 0x3C8
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define STAGE_FILE 0x1BD
#define STAGE_ARCHIVE 0x3D8
#endif
/*
 * Sets the stage up: its map, actors and battles, playing the ambience
 * ENV10_00; battle list 0 before story step 0xE, 1 from then on
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x19B00, 0x23500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x32;
    FIELDSTG_state.music = MUSIC(0x32, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress < 0xE) {
        FIELDSTG_state.battles = stageBattles0;
    } else {
        FIELDSTG_state.battles = stageBattles1;
    }
}

AreaBattle battles0Area0Battle0 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle1 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle2 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle3 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle4 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle5 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle6 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles0Area0Battle7 = { 44, 3, MUSIC(2, 0) };
BattleList battles0Area0Battles = {
    3,
    { &battles0Area0Battle0, &battles0Area0Battle1, &battles0Area0Battle2, &battles0Area0Battle3,
      &battles0Area0Battle4, &battles0Area0Battle5, &battles0Area0Battle6, &battles0Area0Battle7 },
};
AreaBattle battles0Area1Battle0 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle1 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle2 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle3 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle4 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle5 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle6 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles0Area1Battle7 = { 45, 8, MUSIC(2, 0) };
BattleList battles0Area1Battles = {
    1,
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
AreaBattle battles0Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles0Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles0Area3Battles = {
    0,
    { &battles0Area3Battle0, &battles0Area3Battle1, &battles0Area3Battle2, &battles0Area3Battle3,
      &battles0Area3Battle4, &battles0Area3Battle5, &battles0Area3Battle6, &battles0Area3Battle7 },
};
AreaBattle battles1Area0Battle0 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle1 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle2 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle3 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle4 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle5 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle6 = { 44, 3, MUSIC(2, 0) };
AreaBattle battles1Area0Battle7 = { 44, 3, MUSIC(2, 0) };
BattleList battles1Area0Battles = {
    3,
    { &battles1Area0Battle0, &battles1Area0Battle1, &battles1Area0Battle2, &battles1Area0Battle3,
      &battles1Area0Battle4, &battles1Area0Battle5, &battles1Area0Battle6, &battles1Area0Battle7 },
};
AreaBattle battles1Area1Battle0 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle1 = { 45, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle2 = { 57, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle3 = { 57, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle4 = { 57, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle5 = { 57, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle6 = { 57, 8, MUSIC(2, 0) };
AreaBattle battles1Area1Battle7 = { 57, 8, MUSIC(2, 0) };
BattleList battles1Area1Battles = {
    1,
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
AreaBattle battles1Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle battles1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList battles1Area3Battles = {
    0,
    { &battles1Area3Battle0, &battles1Area3Battle1, &battles1Area3Battle2, &battles1Area3Battle3,
      &battles1Area3Battle4, &battles1Area3Battle5, &battles1Area3Battle6, &battles1Area3Battle7 },
};
FieldBattles stageBattles0[] = {
    { 10, 0, 0, { &battles0Area0Battles, &battles0Area1Battles, &battles0Area2Battles, &battles0Area3Battles } },
};
FieldBattles stageBattles1[] = {
    { 29, 1, 0, { &battles1Area0Battles, &battles1Area1Battles, &battles1Area2Battles, &battles1Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x160, 0x100, 0x80, 0, 0x140, 0x1FF },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(5), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(6), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(7), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0xB2, 4, 320, 736, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0xB2, 4, 320, 736, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xB2, 4, 320, 736, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 1024, 448, 0, 0 },
    { 1, 0, 0x40, 2, 1, 0, 0, 0, 0, 0, 1088, 448, 0, 0 },
    { 1, 0, 0x40, 2, 2, 0, 0, 0, 0, 0, 896, 576, 0, 0 },
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 960, 576, 0, 0 },
    { 1, 0, 0x40, 2, 4, 0, 0, 0, 0, 0, 1024, 576, 0, 0 },
    { 1, 0, 0x40, 2, 5, 0, 0, 0, 0, 0, 1088, 576, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 1024, 640, 0, 0 },
    { 1, 0, 0x40, 2, 7, 0, 0, 0, 0, 0, 1088, 640, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x226, 0x5E, 0x40E, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x222, 0x374, 0x9B, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
