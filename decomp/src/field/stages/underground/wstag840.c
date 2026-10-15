/*
 * WSTAG840: Seabed, Underground: below Shell Beach, Plug Cape, South Cape and
 * the Lake of Ice. A tunnel under Shell Beach, Plug Cape, South Cape and the
 * Lake of Ice; it joins WSTAG845 and 855.
 */

#include "common.h"
#include "field/stage.h"

#include "common/copy_place_points.inc.c"
#include "common/update_stage_places_last.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x01 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define STAGE_FILE 0x73B
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define STAGE_FILE 0x74B
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0027 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x15300, 0x13900};
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

StagePoint placePoints3_1Point1 = { 0x2E6, 3, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints3_1Point0 = { 0x220, 0, 0, 112, 184, 0, &placePoints3_1Point1 };
StagePoints placePoints3_1 = { 3, 1, &placePoints3_1Point0 };
StagePoint placePoints4_1Point1 = { 0x2E4, 4, 1, 0x340, 160, 1, NULL };
StagePoint placePoints4_1Point0 = { 0x21F, 0, 0, 224, 0x208, 0, &placePoints4_1Point1 };
StagePoints placePoints4_1 = { 4, 1, &placePoints4_1Point0 };
StagePoint placePoints5_1Point1 = { 0x2E4, 5, 1, 0x340, 160, 1, NULL };
StagePoint placePoints5_1Point0 = { 0x266, 0, 0, 0x2A0, 0x228, 0, &placePoints5_1Point1 };
StagePoints placePoints5_1 = { 5, 1, &placePoints5_1Point0 };
StagePoint placePoints6_1Point1 = { 0x2E4, 6, 1, 0x340, 160, 1, NULL };
StagePoint placePoints6_1Point0 = { 0x266, 0, 0, 0x1C0, 0x108, 0, &placePoints6_1Point1 };
StagePoints placePoints6_1 = { 6, 1, &placePoints6_1Point0 };
StagePoint placePoints9_1Point1 = { 0x2E6, 9, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints9_1Point0 = { 0x23D, 0, 0, 0x240, 216, 0, &placePoints9_1Point1 };
StagePoints placePoints9_1 = { 9, 1, &placePoints9_1Point0 };
StagePoint placePoints13_1Point1 = { 0x2E6, 13, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints13_1Point0 = { 0x28F, 0, 0, 112, 184, 0, &placePoints13_1Point1 };
StagePoints placePoints13_1 = { 13, 1, &placePoints13_1Point0 };
StagePoint placePoints14_1Point1 = { 0x2E4, 14, 1, 0x340, 160, 1, NULL };
StagePoint placePoints14_1Point0 = { 0x28E, 0, 0, 224, 0x208, 0, &placePoints14_1Point1 };
StagePoints placePoints14_1 = { 14, 1, &placePoints14_1Point0 };
StagePoint placePoints15_1Point1 = { 0x2E4, 15, 1, 0x340, 160, 1, NULL };
StagePoint placePoints15_1Point0 = { 0x2CE, 0, 0, 0x2A0, 0x228, 0, &placePoints15_1Point1 };
StagePoints placePoints15_1 = { 15, 1, &placePoints15_1Point0 };
StagePoint placePoints16_1Point1 = { 0x2E4, 16, 1, 0x340, 160, 1, NULL };
StagePoint placePoints16_1Point0 = { 0x2CE, 0, 0, 0x1C0, 0x108, 0, &placePoints16_1Point1 };
StagePoints placePoints16_1 = { 16, 1, &placePoints16_1Point0 };
StagePoint placePoints19_1Point1 = { 0x2E6, 19, 1, 0x450, 0x248, 1, NULL };
StagePoint placePoints19_1Point0 = { 0x2AA, 0, 0, 0x240, 216, 0, &placePoints19_1Point1 };
StagePoints placePoints19_1 = { 19, 1, &placePoints19_1Point0 };
StagePoints placePoints0_0 = { 0, 0, &placePoints3_1Point0 };
StagePoints *placePoints[] = {
    &placePoints3_1, &placePoints4_1, &placePoints5_1, &placePoints6_1,
    &placePoints9_1, &placePoints13_1, &placePoints14_1, &placePoints15_1,
    &placePoints16_1, &placePoints19_1, &placePoints0_0, NULL,
};
AreaBattle place3Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle4 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle5 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle6 = { 61, 11, MUSIC(2, 0) };
AreaBattle place3Area0Battle7 = { 61, 11, MUSIC(2, 0) };
BattleList place3Area0Battles = {
    3,
    { &place3Area0Battle0, &place3Area0Battle1, &place3Area0Battle2, &place3Area0Battle3,
      &place3Area0Battle4, &place3Area0Battle5, &place3Area0Battle6, &place3Area0Battle7 },
};
AreaBattle place3Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area1Battles = {
    0,
    { &place3Area1Battle0, &place3Area1Battle1, &place3Area1Battle2, &place3Area1Battle3,
      &place3Area1Battle4, &place3Area1Battle5, &place3Area1Battle6, &place3Area1Battle7 },
};
AreaBattle place3Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area2Battles = {
    0,
    { &place3Area2Battle0, &place3Area2Battle1, &place3Area2Battle2, &place3Area2Battle3,
      &place3Area2Battle4, &place3Area2Battle5, &place3Area2Battle6, &place3Area2Battle7 },
};
AreaBattle place3Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place3Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place3Area3Battles = {
    0,
    { &place3Area3Battle0, &place3Area3Battle1, &place3Area3Battle2, &place3Area3Battle3,
      &place3Area3Battle4, &place3Area3Battle5, &place3Area3Battle6, &place3Area3Battle7 },
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
AreaBattle place9Area0Battle0 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle1 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle2 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle3 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle4 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle5 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle6 = { 61, 11, MUSIC(2, 0) };
AreaBattle place9Area0Battle7 = { 61, 11, MUSIC(2, 0) };
BattleList place9Area0Battles = {
    3,
    { &place9Area0Battle0, &place9Area0Battle1, &place9Area0Battle2, &place9Area0Battle3,
      &place9Area0Battle4, &place9Area0Battle5, &place9Area0Battle6, &place9Area0Battle7 },
};
AreaBattle place9Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area1Battles = {
    0,
    { &place9Area1Battle0, &place9Area1Battle1, &place9Area1Battle2, &place9Area1Battle3,
      &place9Area1Battle4, &place9Area1Battle5, &place9Area1Battle6, &place9Area1Battle7 },
};
AreaBattle place9Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area2Battles = {
    0,
    { &place9Area2Battle0, &place9Area2Battle1, &place9Area2Battle2, &place9Area2Battle3,
      &place9Area2Battle4, &place9Area2Battle5, &place9Area2Battle6, &place9Area2Battle7 },
};
AreaBattle place9Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place9Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place9Area3Battles = {
    0,
    { &place9Area3Battle0, &place9Area3Battle1, &place9Area3Battle2, &place9Area3Battle3,
      &place9Area3Battle4, &place9Area3Battle5, &place9Area3Battle6, &place9Area3Battle7 },
};
AreaBattle place13Area0Battle0 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle1 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle2 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle3 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle4 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle5 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle6 = { 104, 11, MUSIC(2, 0) };
AreaBattle place13Area0Battle7 = { 104, 11, MUSIC(2, 0) };
BattleList place13Area0Battles = {
    3,
    { &place13Area0Battle0, &place13Area0Battle1, &place13Area0Battle2, &place13Area0Battle3,
      &place13Area0Battle4, &place13Area0Battle5, &place13Area0Battle6, &place13Area0Battle7 },
};
AreaBattle place13Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area1Battles = {
    0,
    { &place13Area1Battle0, &place13Area1Battle1, &place13Area1Battle2, &place13Area1Battle3,
      &place13Area1Battle4, &place13Area1Battle5, &place13Area1Battle6, &place13Area1Battle7 },
};
AreaBattle place13Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area2Battles = {
    0,
    { &place13Area2Battle0, &place13Area2Battle1, &place13Area2Battle2, &place13Area2Battle3,
      &place13Area2Battle4, &place13Area2Battle5, &place13Area2Battle6, &place13Area2Battle7 },
};
AreaBattle place13Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place13Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place13Area3Battles = {
    0,
    { &place13Area3Battle0, &place13Area3Battle1, &place13Area3Battle2, &place13Area3Battle3,
      &place13Area3Battle4, &place13Area3Battle5, &place13Area3Battle6, &place13Area3Battle7 },
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
AreaBattle place19Area0Battle0 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle1 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle2 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle3 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle4 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle5 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle6 = { 104, 11, MUSIC(2, 0) };
AreaBattle place19Area0Battle7 = { 104, 11, MUSIC(2, 0) };
BattleList place19Area0Battles = {
    3,
    { &place19Area0Battle0, &place19Area0Battle1, &place19Area0Battle2, &place19Area0Battle3,
      &place19Area0Battle4, &place19Area0Battle5, &place19Area0Battle6, &place19Area0Battle7 },
};
AreaBattle place19Area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area1Battles = {
    0,
    { &place19Area1Battle0, &place19Area1Battle1, &place19Area1Battle2, &place19Area1Battle3,
      &place19Area1Battle4, &place19Area1Battle5, &place19Area1Battle6, &place19Area1Battle7 },
};
AreaBattle place19Area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area2Battles = {
    0,
    { &place19Area2Battle0, &place19Area2Battle1, &place19Area2Battle2, &place19Area2Battle3,
      &place19Area2Battle4, &place19Area2Battle5, &place19Area2Battle6, &place19Area2Battle7 },
};
AreaBattle place19Area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle place19Area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList place19Area3Battles = {
    0,
    { &place19Area3Battle0, &place19Area3Battle1, &place19Area3Battle2, &place19Area3Battle3,
      &place19Area3Battle4, &place19Area3Battle5, &place19Area3Battle6, &place19Area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 180, 3, 0, { &place3Area0Battles, &place3Area1Battles, &place3Area2Battles, &place3Area3Battles } },
    { 183, 4, 0, { &place4Area0Battles, &place4Area1Battles, &place4Area2Battles, &place4Area3Battles } },
    { 187, 5, 0, { &place5Area0Battles, &place5Area1Battles, &place5Area2Battles, &place5Area3Battles } },
    { 191, 6, 0, { &place6Area0Battles, &place6Area1Battles, &place6Area2Battles, &place6Area3Battles } },
    { 198, 9, 0, { &place9Area0Battles, &place9Area1Battles, &place9Area2Battles, &place9Area3Battles } },
    { 208, 13, 0, { &place13Area0Battles, &place13Area1Battles, &place13Area2Battles, &place13Area3Battles } },
    { 211, 14, 0, { &place14Area0Battles, &place14Area1Battles, &place14Area2Battles, &place14Area3Battles } },
    { 215, 15, 0, { &place15Area0Battles, &place15Area1Battles, &place15Area2Battles, &place15Area3Battles } },
    { 219, 16, 0, { &place16Area0Battles, &place16Area1Battles, &place16Area2Battles, &place16Area3Battles } },
    { 226, 19, 0, { &place19Area0Battles, &place19Area1Battles, &place19Area2Battles, &place19Area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x1A1, 0, 0xA1, 0x160, 0x1FF },
};
FieldActorEntry actor0 = { NULL, NULL, 0x147, 4, 0, 0, 0 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x8F, 2, 0x37, 1, 0x37, 0x4B, 0xA, 0, 948, 29, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 311, 207, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 636, 93, 0, 0 },
    { 1, 0, 0xB6, 2, 0x4C, 1, 0x4C, 0x62, 0xA, 0, 871, 27, 0, 0 },
    { 1, 0, 0x8B, 2, 0, 0, 0, 0, 0, 0, 720, 158, 0, 0 },
    { 1, 0, 0xA1, 2, 1, 0, 0, 0, 0, 0, 688, 136, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 0, 0, 0, 0, 0, 262, 340, 0, 0 },
    { 1, 0, 0x40, 2, 0xB, 0, 0, 0, 0, 0, 320, 330, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 384, 299, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 448, 266, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 373, 118, 0, 0 },
    { 1, 0, 0x8F, 6, 0x37, 1, 0x37, 0x4B, 0xA, 0, 490, 66, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 148, 233, 0, 0 },
    { 1, 0, 0x68, 6, 0x34, 1, 0x34, 0x36, 0xA, 0, 753, 1, 0, 0 },
    { 1, 0, 0xFF, 4, 0x32, 2, 0, 7, 0x18, 0, 860, -124, 120, 0 },
    { 1, 0, 0x8B, 4, 2, 0, 0, 0, 0, 0, 656, 118, 250, 0 },
    { 1, 0, 0x8A, 4, 3, 0, 0, 0, 0, 0, 624, 104, 234, 0 },
    { 1, 0, 0x79, 4, 4, 0, 0, 0, 0, 0, 592, 89, 218, 0 },
    { 1, 0, 0x98, 4, 5, 0, 0, 0, 0, 0, 560, 70, 202, 0 },
    { 1, 0, 0x55, 4, 7, 0, 0, 0, 0, 0, 400, 249, 338, 0 },
    { 1, 0, 0x67, 4, 8, 0, 0, 0, 0, 0, 368, 233, 322, 0 },
    { 1, 0, 0x5E, 4, 9, 0, 0, 0, 0, 0, 352, 228, 314, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E3, 0x380, 0x70, 4, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2E3, 0xA0, 0x180, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
