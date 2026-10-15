/*
 * WSTAG938: Amaterasu Sewer, Amaterasu City, in the extra chapter. The
 * European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 650
 * starts it instead of WSTAG321.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x903
#define STAGE_FILE_8 0x2A3
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8CB
#define STAGE_FILE_8 0x294
#endif
/* Sets the stage up: its map, actors and battles, playing BGM_0003 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x26F00, 0x1EF00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 6;
    FIELDSTG_state.music = MUSIC(6, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B8, 0x100, 0x1E0, 0, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x1A4, 0x128, 0x190, 0x28, 0x140, 0x1F4 },
    { 0x180, 0x100, 0x1AC, 0x128, 0x1B0, 0x28, 0x160, 0x1F4 },
    { 0x180, 0x100, 0x1A6, 0x158, 0x198, 0x58, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x19E, 0x130, 0x178, 0x30, 0x140, 0x1F3 },
};
u16 actor0Talk0Actions[] = { FLAG(2, 0x67), 1, ITEM(1, 0x37), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(2, 0x68), 1, ITEM(1, 0x38), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 2 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x67 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x68 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x69 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 3 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0x67), 0, CODES_END };
u16 actor4Conditions[] = { FLAG(2, 0x68), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 465, 169, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x25, 5, 336, 640, 1 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x26, 6, 839, 533, 1 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x38, 7, 600, 509, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x4D, 8, 272, 217, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 395, 311, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 871, 533, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 1037, 703, 0, 0 },
    { 1, 0, 0x40, 2, 0x4D, 2, 0, 3, 4, 0, 1012, 671, 0, 0 },
    { 1, 0, 0x40, 2, 0x52, 2, 0, 3, 8, 0, 442, 116, 0, 0 },
    { 1, 0, 0x40, 2, 0x52, 2, 0, 3, 8, 0, 473, 132, 0, 0 },
    { 1, 0, 0x40, 2, 0x53, 2, 0, 3, 4, 0, 299, 39, 0, 0 },
    { 1, 0, 0x40, 2, 0x5A, 0, 0, 0, 0, 0, 472, 396, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 417, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 449, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 481, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 513, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 544, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 1, 0x47, 0x4C, 4, 0, 735, 465, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 328, 658, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 616, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 760, 730, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 904, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 351, 643, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 639, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 783, 715, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 927, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 324, 632, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 612, 776, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 756, 704, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 900, 776, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 305, 643, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 593, 788, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 737, 715, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 881, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 202, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 210, 420, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 218, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 226, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 234, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 242, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 250, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 258, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 266, 352, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 274, 356, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 282, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 290, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 298, 368, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 306, 372, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 314, 376, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 3, 8, 0, 322, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 270, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 278, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 286, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 294, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 302, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 310, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 358, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 366, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 374, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 382, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 390, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 3, 8, 0, 349, 405, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 3, 4, 0, 439, 395, 0, 0 },
    { 1, 0, 0x40, 6, 0x59, 2, 0, 3, 4, 0, 540, 398, 0, 0 },
    { 1, 0, 0x40, 6, 0x5B, 2, 0, 3, 4, 0, 612, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 646, 530, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 688, 509, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 790, 602, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 790, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 832, 581, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 832, 725, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 358, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 400, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 519, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 535, 458, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 560, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 576, 437, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 648, 724, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 690, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 792, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 833, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 936, 725, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 978, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x62, 2, 0, 1, 4, 0, 654, 640, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 102, 448, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 159, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 174, 374, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 273, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 390, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 128, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 177, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 209, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 94, 418, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 194, 530, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 98, 495, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 546, 532, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 366, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 251, 381, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 70, 467, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 76, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 129, 505, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 179, 534, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 186, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 187, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 263, 395, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 352, 473, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 393, 415, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 553, 516, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 956, 689, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 966, 668, 0, 0 },
    { 1, 0xA, 0xA0, 6, 0, 0, 0, 0, 0, 0, 792, 381, 0, 0 },
    { 1, 0, 0x60, 6, 1, 0, 0, 0, 0, 0, 721, 691, 0, 0 },
    { 1, 0, 0x60, 6, 1, 0, 0, 0, 0, 0, 865, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 369, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 465, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 497, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 6, 0, 546, 665, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 6, 0, 833, 798, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2A, 1, 0x2A, 0x2D, 6, 0, 545, 558, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2A, 1, 0x2A, 0x2D, 6, 0, 834, 701, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, -12, 814, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 0, 364, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 100, 758, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 212, 702, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 324, 646, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, -14, 713, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 99, 658, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 210, 602, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 322, 546, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x26, 1, 0x26, 0x29, 6, 0, 11, 408, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27A, 0x242, 0x13E, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28B, 0x88, 0x1B4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E3, 0x380, 0x70, 1, 0, 1, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x130, 0xF8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x120, 0x140, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
AreaBattle area0Battle0 = { 88, 7, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 88, 7, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 88, 7, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 88, 7, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 90, 7, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 90, 7, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 90, 7, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 90, 7, MUSIC(2, 0) };
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
    { 379, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
