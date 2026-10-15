/*
 * WSTAG481: Jungle Grave, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG480.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern StageTileFrame **updateTileCursorsFrames[];

#include "common/step_tile_sequences.inc.c"

#include "common/update_tile_cursors.inc.c"

#include "common/create_tile_cursors.inc.c"

/*
 * The stage task: creates the sequences of the map objects with animations 1
 * to 5 (updateTileCursors)
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children[0] = createTileCursors();
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0 };

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x5EB
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x5FB
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0015 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x48600, 0xE600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x11;
    FIELDSTG_state.music = MUSIC(0x11, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR1, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFirstMap(0);
}

StageTileFrame updateTileCursorsFrames_0_0[] = {
    { 64, 10, 0, 0 }, { 64, 10, 1, 0 }, { 64, 10, 2, 0 }, { 64, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_0_12[] = {
    { 65, 4, 2, 0 }, { 65, 4, 4, 0 }, { 65, 4, 6, 0 }, { 65, 4, 4, 0 },
    { 65, 4, 2, 0 }, { 65, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_1_0[] = {
    { 66, 10, 0, 0 }, { 66, 10, 1, 0 }, { 66, 10, 2, 0 }, { 66, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_1_12[] = {
    { 67, 4, 2, 0 }, { 67, 4, 4, 0 }, { 67, 4, 6, 0 }, { 67, 4, 4, 0 },
    { 67, 4, 2, 0 }, { 67, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_2_0[] = {
    { 68, 10, 0, 0 }, { 68, 10, 1, 0 }, { 68, 10, 2, 0 }, { 68, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_2_12[] = {
    { 69, 4, 2, 0 }, { 69, 4, 4, 0 }, { 69, 4, 6, 0 }, { 69, 4, 4, 0 },
    { 69, 4, 2, 0 }, { 69, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_3_0[] = {
    { 70, 10, 0, 0 }, { 70, 10, 1, 0 }, { 70, 10, 2, 0 }, { 70, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_3_12[] = {
    { 71, 4, 2, 0 }, { 71, 4, 4, 0 }, { 71, 4, 6, 0 }, { 71, 4, 4, 0 },
    { 71, 4, 2, 0 }, { 71, 4, 0, 1 },
};
StageTileFrame updateTileCursorsFrames_4_0[] = {
    { 72, 10, 0, 0 }, { 72, 10, 1, 0 }, { 72, 10, 2, 0 }, { 72, 10, 1, 1 },
};
StageTileFrame updateTileCursorsFrames_4_12[] = {
    { 73, 4, 2, 0 }, { 73, 4, 4, 0 }, { 73, 4, 6, 0 }, { 73, 4, 4, 0 },
    { 73, 4, 2, 0 }, { 73, 4, 0, 1 },
};
StageTileFrame *updateTileCursorsFrames_0[] = {
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0, updateTileCursorsFrames_0_0,
    updateTileCursorsFrames_0_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_1[] = {
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0, updateTileCursorsFrames_1_0,
    updateTileCursorsFrames_1_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_2[] = {
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0, updateTileCursorsFrames_2_0,
    updateTileCursorsFrames_2_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_3[] = {
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0, updateTileCursorsFrames_3_0,
    updateTileCursorsFrames_3_12, NULL,
};
StageTileFrame *updateTileCursorsFrames_4[] = {
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0, updateTileCursorsFrames_4_0,
    updateTileCursorsFrames_4_12, NULL,
};
StageTileFrame **updateTileCursorsFrames[] = {
    updateTileCursorsFrames_0, updateTileCursorsFrames_1, updateTileCursorsFrames_2, updateTileCursorsFrames_3,
    updateTileCursorsFrames_4,
};
AreaBattle area0Battle0 = { 107, 9, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 107, 9, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 107, 9, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 107, 9, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 155, 9, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 155, 9, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 155, 9, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 155, 9, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 105, 8, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 105, 8, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 105, 8, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 105, 8, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 159, 8, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 159, 8, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 159, 8, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 159, 8, MUSIC(2, 0) };
BattleList area1Battles = {
    2,
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
AreaBattle area3Battle3 = { 331, 9, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 332, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 177, 9, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 106, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 74, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
};
StageTile stageObjects[] = {
    { 1, 1, 0x40, 2, 0x40, 0, 0, 0, 0, 0, 310, 197, 0, 0 },
    { 1, 2, 0x40, 2, 0x42, 0, 0, 0, 0, 0, 381, 208, 0, 0 },
    { 1, 3, 0x40, 2, 0x44, 0, 0, 0, 0, 0, 416, 204, 0, 0 },
    { 1, 5, 0x40, 2, 0x48, 0, 0, 0, 0, 0, 464, 258, 0, 0 },
    { 1, 4, 0x40, 2, 0x46, 0, 0, 0, 0, 0, 468, 206, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 191, 408, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 564, 488, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 594, 498, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 724, 120, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 772, 419, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 804, 410, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x37, 8, 0, 901, 541, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 977, 774, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 986, 772, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 995, 783, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1010, 771, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1017, 771, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1024, 784, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1047, 799, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1057, 791, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1066, 796, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1081, 795, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1088, 789, 0, 0 },
    { 1, 0, 0x40, 2, 0x38, 1, 0x38, 0x3D, 8, 0, 1096, 796, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 79, 617, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 262, 882, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 431, 638, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 702, 664, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 931, 679, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 1151, 642, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 2, 0, 0xF, 6, 0, 1293, 701, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 707, 1103, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 732, 964, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1051, 956, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1076, 1132, 0, 0 },
    { 1, 0, 0x40, 2, 0x3F, 2, 0, 0xF, 6, 0, 1290, 929, 0, 0 },
    { 1, 0, 0x40, 2, 0x4B, 1, 0x4B, 0x56, 0x10, 0, 871, 187, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 1, 0x57, 0x62, 0x10, 0, 256, 462, 0, 0 },
    { 1, 0, 0xFF, 2, 0x1A, 0, 0, 0, 0, 0, 1280, 250, 0, 0 },
    { 1, 0, 0x80, 2, 0x1B, 0, 0, 0, 0, 0, 640, 640, 0, 0 },
    { 1, 0, 0x80, 2, 0x1C, 0, 0, 0, 0, 0, 1152, 640, 0, 0 },
    { 1, 0, 0x80, 2, 0x1D, 0, 0, 0, 0, 0, 1280, 640, 0, 0 },
    { 1, 0, 0x40, 2, 0, 0, 0, 0, 0, 0, 1125, 1033, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 200, 271, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 299, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 379, 1321, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 388, 925, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 436, 759, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 565, 1172, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 653, 974, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 871, 170, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 883, 1267, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 885, 889, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 900, 760, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 1079, 128, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 1322, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x22, 1, 0x22, 0x31, 0xA, 0, 1341, 990, 0, 0 },
    { 1, 0, 0xFF, 6, 0x63, 0, 0, 0, 0, 0, 280, 280, 0, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 1041, 566, 603, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 1133, 585, 596, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 862, 659, 666, 0 },
    { 1, 0, 0x80, 4, 0xD, 0, 0, 0, 0, 0, 1022, 313, 367, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 693, 475, 479, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 661, 583, 589, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 1029, 891, 899, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 1011, 936, 945, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 321, 1025, 1031, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 821, 1047, 1057, 0 },
    { 1, 0, 0x40, 4, 0x14, 0, 0, 0, 0, 0, 261, 1055, 1060, 0 },
    { 1, 0, 0x40, 4, 0x15, 0, 0, 0, 0, 0, 612, 1088, 1097, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 993, 1122, 1127, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 804, 1152, 1160, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 401, 1225, 1231, 0 },
    { 1, 0, 0x40, 4, 0x19, 0, 0, 0, 0, 0, 608, 1249, 1255, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 496, 807, 807, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 528, 951, 951, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 560, 743, 743, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 577, 359, 359, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 607, 768, 768, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 704, 847, 847, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 800, 847, 847, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1072, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1120, 447, 447, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1136, 487, 487, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1152, 527, 527, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1175, 441, 441, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1184, 559, 559, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1200, 471, 471, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1248, 495, 495, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1303, 605, 605, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1312, 431, 431, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1344, 623, 623, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A8, 0x6C8, 0x1D4, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A2, 0x140, 0x350, 5, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 3, 2 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x1A, 1 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x28, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 0, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_MAP, 1, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
