/*
 * WSTAG845: Seabed, Underground: Seehomon and Depthmon's forge. A tunnel only
 * the other tunnels reach (WSTAG840, 850, 855, 860), with Seehomon's and
 * Depthmon's weapon forging.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places_last.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x6F5
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x705
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xE500, 0x16900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1D;
    FIELDSTG_state.music = MUSIC(0x1D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = FIELDSTG_state.findBattles(stageBattles, GAME.place);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

StagePoint placePoints1_1Point1 = { 0x2E5, 1, 2, 0x3B0, 216, 1, NULL };
StagePoint placePoints1_1Point0 = { 0x2E5, 1, 1, 224, 0x120, 5, &placePoints1_1Point1 };
StagePoints placePoints1_1 = { 1, 1, &placePoints1_1Point0 };
StagePoint placePoints4_1Point1 = { 0x2E5, 4, 1, 0x3B0, 216, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x2E3, 4, 1, 160, 0x180, 5, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point1 = { 0x2E6, 5, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x2E3, 5, 1, 160, 0x180, 5, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints6_1Point1 = { 0x2E7, 6, 1, 0x250, 232, 1, NULL };
StagePoint placePoints6_1Point0 = { 0x2E3, 6, 1, 160, 0x180, 5, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints11_1Point1 = { 0x2E5, 11, 2, 0x3B0, 216, 1, NULL };
StagePoint placePoints11_1Point0 = { 0x2E5, 11, 1, 224, 0x120, 5, &placePoints11_1Point1 };
StagePoints placePoints11_1 = { 11, 1, &placePoints11_1Point0 };
StagePoint placePoints14_1Point1 = { 0x2E5, 14, 1, 0x3B0, 216, 1, NULL };
StagePoint placePoints14_1Point0 = { 0x2E3, 14, 1, 160, 0x180, 5, &placePoints14_1Point1 };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints15_1Point1 = { 0x2E6, 15, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints15_1Point0 = { 0x2E3, 15, 1, 160, 0x180, 5, &placePoints15_1Point1 };
StagePoints placePoints15_1 = { 15, 1, &placePoints15_1Point0 };
StagePoint placePoints16_1Point1 = { 0x2E7, 16, 1, 0x250, 232, 1, NULL };
StagePoint placePoints16_1Point0 = { 0x2E3, 16, 1, 160, 0x180, 5, &placePoints16_1Point1 };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints1_1Point0 };
StagePoints *placePoints[] = {
    &placePoints1_1, &placePoints4_1, &placePoints5_1, &placePoints6_1,
    &placePoints11_1, &placePoints14_1, &placePoints15_1, &placePoints16_1,
    &placePoints0_0, NULL,
};
AreaBattle place1Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle4 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle5 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle6 = { 62, 11, MUSIC(2, 0) };
AreaBattle place1Area0Battle7 = { 62, 11, MUSIC(2, 0) };
BattleList place1Area0Battles = {
    2,
    { &place1Area0Battle0, &place1Area0Battle1, &place1Area0Battle2, &place1Area0Battle3,
      &place1Area0Battle4, &place1Area0Battle5, &place1Area0Battle6, &place1Area0Battle7 },
};
AreaBattle place1Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area1Battles = {
    0,
    { &place1Area1Battle0, &place1Area1Battle1, &place1Area1Battle2, &place1Area1Battle3,
      &place1Area1Battle4, &place1Area1Battle5, &place1Area1Battle6, &place1Area1Battle7 },
};
AreaBattle place1Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area2Battles = {
    0,
    { &place1Area2Battle0, &place1Area2Battle1, &place1Area2Battle2, &place1Area2Battle3,
      &place1Area2Battle4, &place1Area2Battle5, &place1Area2Battle6, &place1Area2Battle7 },
};
AreaBattle place1Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place1Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place1Area3Battles = {
    0,
    { &place1Area3Battle0, &place1Area3Battle1, &place1Area3Battle2, &place1Area3Battle3,
      &place1Area3Battle4, &place1Area3Battle5, &place1Area3Battle6, &place1Area3Battle7 },
};
AreaBattle place4Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle4 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle5 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle6 = { 62, 11, MUSIC(2, 0) };
AreaBattle place4Area0Battle7 = { 62, 11, MUSIC(2, 0) };
BattleList place4Area0Battles = {
    2,
    { &place4Area0Battle0, &place4Area0Battle1, &place4Area0Battle2, &place4Area0Battle3,
      &place4Area0Battle4, &place4Area0Battle5, &place4Area0Battle6, &place4Area0Battle7 },
};
AreaBattle place4Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area1Battles = {
    0,
    { &place4Area1Battle0, &place4Area1Battle1, &place4Area1Battle2, &place4Area1Battle3,
      &place4Area1Battle4, &place4Area1Battle5, &place4Area1Battle6, &place4Area1Battle7 },
};
AreaBattle place4Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area2Battles = {
    0,
    { &place4Area2Battle0, &place4Area2Battle1, &place4Area2Battle2, &place4Area2Battle3,
      &place4Area2Battle4, &place4Area2Battle5, &place4Area2Battle6, &place4Area2Battle7 },
};
AreaBattle place4Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place4Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place4Area3Battles = {
    0,
    { &place4Area3Battle0, &place4Area3Battle1, &place4Area3Battle2, &place4Area3Battle3,
      &place4Area3Battle4, &place4Area3Battle5, &place4Area3Battle6, &place4Area3Battle7 },
};
AreaBattle place5Area0Battle0 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle1 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle2 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle3 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle4 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle5 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle6 = { 63, 11, MUSIC(2, 0) };
AreaBattle place5Area0Battle7 = { 63, 11, MUSIC(2, 0) };
BattleList place5Area0Battles = {
    3,
    { &place5Area0Battle0, &place5Area0Battle1, &place5Area0Battle2, &place5Area0Battle3,
      &place5Area0Battle4, &place5Area0Battle5, &place5Area0Battle6, &place5Area0Battle7 },
};
AreaBattle place5Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area1Battles = {
    0,
    { &place5Area1Battle0, &place5Area1Battle1, &place5Area1Battle2, &place5Area1Battle3,
      &place5Area1Battle4, &place5Area1Battle5, &place5Area1Battle6, &place5Area1Battle7 },
};
AreaBattle place5Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area2Battles = {
    0,
    { &place5Area2Battle0, &place5Area2Battle1, &place5Area2Battle2, &place5Area2Battle3,
      &place5Area2Battle4, &place5Area2Battle5, &place5Area2Battle6, &place5Area2Battle7 },
};
AreaBattle place5Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place5Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place5Area3Battles = {
    0,
    { &place5Area3Battle0, &place5Area3Battle1, &place5Area3Battle2, &place5Area3Battle3,
      &place5Area3Battle4, &place5Area3Battle5, &place5Area3Battle6, &place5Area3Battle7 },
};
AreaBattle place6Area0Battle0 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle1 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle2 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle3 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle4 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle5 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle6 = { 63, 11, MUSIC(2, 0) };
AreaBattle place6Area0Battle7 = { 63, 11, MUSIC(2, 0) };
BattleList place6Area0Battles = {
    5,
    { &place6Area0Battle0, &place6Area0Battle1, &place6Area0Battle2, &place6Area0Battle3,
      &place6Area0Battle4, &place6Area0Battle5, &place6Area0Battle6, &place6Area0Battle7 },
};
AreaBattle place6Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area1Battles = {
    0,
    { &place6Area1Battle0, &place6Area1Battle1, &place6Area1Battle2, &place6Area1Battle3,
      &place6Area1Battle4, &place6Area1Battle5, &place6Area1Battle6, &place6Area1Battle7 },
};
AreaBattle place6Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area2Battles = {
    0,
    { &place6Area2Battle0, &place6Area2Battle1, &place6Area2Battle2, &place6Area2Battle3,
      &place6Area2Battle4, &place6Area2Battle5, &place6Area2Battle6, &place6Area2Battle7 },
};
AreaBattle place6Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place6Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place6Area3Battles = {
    0,
    { &place6Area3Battle0, &place6Area3Battle1, &place6Area3Battle2, &place6Area3Battle3,
      &place6Area3Battle4, &place6Area3Battle5, &place6Area3Battle6, &place6Area3Battle7 },
};
AreaBattle place11Area0Battle0 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle1 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle2 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle3 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle4 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle5 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle6 = { 103, 11, MUSIC(2, 0) };
AreaBattle place11Area0Battle7 = { 103, 11, MUSIC(2, 0) };
BattleList place11Area0Battles = {
    2,
    { &place11Area0Battle0, &place11Area0Battle1, &place11Area0Battle2, &place11Area0Battle3,
      &place11Area0Battle4, &place11Area0Battle5, &place11Area0Battle6, &place11Area0Battle7 },
};
AreaBattle place11Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area1Battles = {
    0,
    { &place11Area1Battle0, &place11Area1Battle1, &place11Area1Battle2, &place11Area1Battle3,
      &place11Area1Battle4, &place11Area1Battle5, &place11Area1Battle6, &place11Area1Battle7 },
};
AreaBattle place11Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area2Battles = {
    0,
    { &place11Area2Battle0, &place11Area2Battle1, &place11Area2Battle2, &place11Area2Battle3,
      &place11Area2Battle4, &place11Area2Battle5, &place11Area2Battle6, &place11Area2Battle7 },
};
AreaBattle place11Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place11Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place11Area3Battles = {
    0,
    { &place11Area3Battle0, &place11Area3Battle1, &place11Area3Battle2, &place11Area3Battle3,
      &place11Area3Battle4, &place11Area3Battle5, &place11Area3Battle6, &place11Area3Battle7 },
};
AreaBattle place14Area0Battle0 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle1 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle2 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle3 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle4 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle5 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle6 = { 103, 11, MUSIC(2, 0) };
AreaBattle place14Area0Battle7 = { 103, 11, MUSIC(2, 0) };
BattleList place14Area0Battles = {
    2,
    { &place14Area0Battle0, &place14Area0Battle1, &place14Area0Battle2, &place14Area0Battle3,
      &place14Area0Battle4, &place14Area0Battle5, &place14Area0Battle6, &place14Area0Battle7 },
};
AreaBattle place14Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area1Battles = {
    0,
    { &place14Area1Battle0, &place14Area1Battle1, &place14Area1Battle2, &place14Area1Battle3,
      &place14Area1Battle4, &place14Area1Battle5, &place14Area1Battle6, &place14Area1Battle7 },
};
AreaBattle place14Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area2Battles = {
    0,
    { &place14Area2Battle0, &place14Area2Battle1, &place14Area2Battle2, &place14Area2Battle3,
      &place14Area2Battle4, &place14Area2Battle5, &place14Area2Battle6, &place14Area2Battle7 },
};
AreaBattle place14Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place14Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place14Area3Battles = {
    0,
    { &place14Area3Battle0, &place14Area3Battle1, &place14Area3Battle2, &place14Area3Battle3,
      &place14Area3Battle4, &place14Area3Battle5, &place14Area3Battle6, &place14Area3Battle7 },
};
AreaBattle place15Area0Battle0 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle1 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle2 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle3 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle4 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle5 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle6 = { 178, 11, MUSIC(2, 0) };
AreaBattle place15Area0Battle7 = { 178, 11, MUSIC(2, 0) };
BattleList place15Area0Battles = {
    3,
    { &place15Area0Battle0, &place15Area0Battle1, &place15Area0Battle2, &place15Area0Battle3,
      &place15Area0Battle4, &place15Area0Battle5, &place15Area0Battle6, &place15Area0Battle7 },
};
AreaBattle place15Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area1Battles = {
    0,
    { &place15Area1Battle0, &place15Area1Battle1, &place15Area1Battle2, &place15Area1Battle3,
      &place15Area1Battle4, &place15Area1Battle5, &place15Area1Battle6, &place15Area1Battle7 },
};
AreaBattle place15Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area2Battles = {
    0,
    { &place15Area2Battle0, &place15Area2Battle1, &place15Area2Battle2, &place15Area2Battle3,
      &place15Area2Battle4, &place15Area2Battle5, &place15Area2Battle6, &place15Area2Battle7 },
};
AreaBattle place15Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place15Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place15Area3Battles = {
    0,
    { &place15Area3Battle0, &place15Area3Battle1, &place15Area3Battle2, &place15Area3Battle3,
      &place15Area3Battle4, &place15Area3Battle5, &place15Area3Battle6, &place15Area3Battle7 },
};
AreaBattle place16Area0Battle0 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle1 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle2 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle3 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle4 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle5 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle6 = { 178, 11, MUSIC(2, 0) };
AreaBattle place16Area0Battle7 = { 178, 11, MUSIC(2, 0) };
BattleList place16Area0Battles = {
    5,
    { &place16Area0Battle0, &place16Area0Battle1, &place16Area0Battle2, &place16Area0Battle3,
      &place16Area0Battle4, &place16Area0Battle5, &place16Area0Battle6, &place16Area0Battle7 },
};
AreaBattle place16Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area1Battles = {
    0,
    { &place16Area1Battle0, &place16Area1Battle1, &place16Area1Battle2, &place16Area1Battle3,
      &place16Area1Battle4, &place16Area1Battle5, &place16Area1Battle6, &place16Area1Battle7 },
};
AreaBattle place16Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area2Battles = {
    0,
    { &place16Area2Battle0, &place16Area2Battle1, &place16Area2Battle2, &place16Area2Battle3,
      &place16Area2Battle4, &place16Area2Battle5, &place16Area2Battle6, &place16Area2Battle7 },
};
AreaBattle place16Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place16Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place16Area3Battles = {
    0,
    { &place16Area3Battle0, &place16Area3Battle1, &place16Area3Battle2, &place16Area3Battle3,
      &place16Area3Battle4, &place16Area3Battle5, &place16Area3Battle6, &place16Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 176, 1, 0, { &place1Area0Battles, &place1Area1Battles, &place1Area2Battles, &place1Area3Battles } },
    { 184, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 188, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 192, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 204, 11, 0, { &place11Area0Battles, &place11Area1Battles, &place11Area2Battles, &place11Area3Battles } },
    { 212, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
    { 216, 15, 0, { &place15Area0Battles, &place15Area1Battles, &place15Area2Battles, &place15Area3Battles } },
    { 220, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B0, 0x178, 0x1C0, 0x78, 0x150, 0x1FF },
    { 0x140, 0x100, 0x170, 0x1D0, 0xC0, 0xD0, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x170, 0x1FF },
};
u16 actor0Talk0Conditions[] = { ITEM(3, 0x76), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 1, ITEM(2, 0x72), 0, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(3, 0x76), 0, FLAG(0, 0), 1, ITEM(2, 0x72), 1, CODES_END };
u16 actor0Talk3Actions[] = { ITEM(3, 0x76), 1, ITEM(3, 0x75), 0, ITEM(2, 0x72), 0, CODES_END };
u16 actor1Talk0Conditions[] = { ITEM(3, 0x9B), 1, CODES_END };
u16 actor1Talk1Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 0, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk2Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 0, CODES_END };
u16 actor1Talk3Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 1, CODES_END };
u16 actor1Talk3Actions[] = { ITEM(3, 0x9B), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x97), 0, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(3, 0x9B), 1, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 0, CODES_END };
u16 actor2Talk3Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 1, CODES_END };
u16 actor2Talk3Actions[] = { ITEM(3, 0x9B), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x97), 0, CODES_END };
u16 actor3Talk0Conditions[] = { ITEM(3, 0x9B), 1, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 0, CODES_END };
u16 actor3Talk3Conditions[] = { ITEM(3, 0x9B), 0, FLAG(0, 0), 1, ITEM(2, 0x97), 1, CODES_END };
u16 actor3Talk3Actions[] = { ITEM(3, 0x9B), 1, ITEM(3, 0x9A), 0, ITEM(2, 0x97), 0, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x36F },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x370 },
    { actor0Talk2Conditions, NULL, 0x371 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x372 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x373 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x374 },
    { actor1Talk2Conditions, NULL, 0x375 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x376 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x373 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x374 },
    { actor2Talk2Conditions, NULL, 0x375 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x376 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x373 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x374 },
    { actor3Talk2Conditions, NULL, 0x375 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x376 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = {
    WARP_ARG(0), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x44), 1,
    SPECIAL(0x4C), 1,
    ITEM(3, 0x76), 0,
    ITEM(3, 0x75), 1,
    CODES_END,
};
u16 actor1Conditions[] = {
    WARP_ARG(0xD), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x4F), 1,
    ITEM(3, 0x9A), 1,
    SPECIAL(0x47), 1,
    ITEM(3, 0x9B), 0,
    CODES_END,
};
u16 actor2Conditions[] = {
    ITEM(3, 0x9B), 0,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x45), 1,
    ITEM(3, 0x9A), 1,
    SPECIAL(0x4D), 1,
    WARP_ARG(3), 1,
    CODES_END,
};
u16 actor3Conditions[] = {
    ITEM(3, 0x9A), 1,
    SPECIAL(0x4E), 1,
    ITEM(3, 0x9B), 0,
    WARP_ARG(4), 1,
    WARP_ARG(0x1E), 1,
    SPECIAL(0x46), 1,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0xA8, 4, 544, 240, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0xAA, 5, 544, 240, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xAA, 5, 544, 240, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xAA, 5, 544, 240, 1 };
FieldActorEntry actor4 = { NULL, NULL, 0x147, 6, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 200, 265, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 560, 185, 0, 0 },
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 857, 33, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 395, 180, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 500, 39, 0, 0 },
    { 1, 0, 0xB6, 6, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 659, 0, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 176, 128, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 758, 41, 0, 0 },
    { 1, 0, 0x8A, 4, 3, 0, 0, 0, 0, 0, 416, 191, 327, 0 },
    { 1, 0, 0x52, 4, 4, 0, 0, 0, 0, 0, 384, 174, 311, 0 },
    { 1, 0, 0x83, 4, 5, 0, 0, 0, 0, 0, 352, 169, 295, 0 },
    { 1, 0, 0x8E, 4, 6, 0, 0, 0, 0, 0, 320, 154, 279, 0 },
    { 1, 0, 0x54, 4, 8, 0, 0, 0, 0, 0, 672, 177, 268, 0 },
    { 1, 0, 0x66, 4, 9, 0, 0, 0, 0, 0, 640, 162, 251, 0 },
    { 1, 0, 0x60, 4, 0xA, 0, 0, 0, 0, 0, 624, 156, 242, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E4, 0x340, 0xA0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E4, 0xC0, 0x180, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
