/*
 * WSTAG923: Main Lobby, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 627 starts it
 * instead of WSTAG211.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8E2
#define STAGE_FILE_8 0x19A
#define STAGE_ARCHIVE 0x8E0
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x897
#define STAGE_FILE_8 0x18C
#define STAGE_ARCHIVE 0xC2
#endif
/* Sets the stage up: its map, actors and events, playing KANRIBGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 3;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x11C00, 0x13400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 4);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 5);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F0, 0x120, 0x2C0, 0x20, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1F0, 0x148, 0x2C0, 0x48, 0x140, 0x1F7 },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x150, 0x1F7 },
    { 0x140, 0x100, 0x178, 0x139, 0xE0, 0x39, 0x160, 0x1F7 },
    { 0x1C0, 0x100, 0x1C0, 0x16D, 0x200, 0x6D, 0x170, 0x1F7 },
    { 0x1C0, 0x100, 0x1C8, 0x16D, 0x220, 0x6D, 0x140, 0x1F6 },
    { 0x1C0, 0x100, 0x1F0, 0x170, 0x2C0, 0x70, 0x150, 0x1F6 },
    { 0x1C0, 0x100, 0x1D0, 0x18A, 0x240, 0x8A, 0x160, 0x1F6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x1C0, 0x100, 0x1D8, 0x18A, 0x260, 0x8A, 0x170, 0x1F6 },
    { 0x1C0, 0x100, 0x1E0, 0x18A, 0x280, 0x8A, 0x140, 0x1F5 },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x18 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x19 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x1D },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x1A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x1C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x21 },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x25, 6, 344, 188, 1 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x2D, 7, 193, 257, 7 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x30, 8, 328, 260, 5 };
FieldActorEntry actor5 = { NULL, actor5Talks, 0x35, 9, 391, 359, 5 };
FieldActorEntry actor6 = { NULL, actor6Talks, 0x39, 0xA, 529, 505, 7 };
FieldActorEntry actor7 = { NULL, actor7Talks, 0x3A, 0xB, 617, 501, 1 };
FieldActorEntry actor8 = { NULL, NULL, 0x70, 0xC, 360, 268, 1 };
FieldActorEntry actor9 = { NULL, NULL, 0x71, 0xD, 450, 254, 7 };
FieldActorEntry actor10 = { NULL, actor10Talks, 0x173, 0xE, 308, 305, 3 };
FieldActorEntry actor11 = { NULL, actor11Talks, 0x179, 0xF, 285, 295, 7 };
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
    { 1, 0, 0x40, 2, 0x36, 2, 0, 1, 4, 0, 243, 69, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 1, 4, 0, 802, 278, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 4, 0, 457, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 339, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 372, 196, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 403, 244, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 435, 228, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 386, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 546, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 674, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 573, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 613, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 151, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 411, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 649, 200, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 10, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 42, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 74, 137, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 290, 69, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 322, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 354, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 482, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 514, 181, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 706, 213, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 738, 229, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 770, 245, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 7, 0x12, 0, 119, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 135, 232, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 155, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 175, 212, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 195, 202, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 215, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 235, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 255, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 275, 162, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 295, 152, 0, 0 },
    { 1, 0x66, 0x40, 6, 4, 0, 0, 0, 0, 0, 335, 141, 0, 0 },
    { 1, 0x65, 0x40, 6, 5, 0, 0, 0, 0, 0, 511, 229, 0, 0 },
    { 1, 0x64, 0x40, 6, 6, 0, 0, 0, 0, 0, 736, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 4, 0, 114, 179, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 4, 0, 264, 130, 0, 0 },
    { 1, 0, 0x40, 6, 0x59, 2, 0, 3, 4, 0, 429, 176, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x54, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 6, 0x4D, 1, 0x4D, 0x50, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 28, 519, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 81, 492, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 183, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 237, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 656, 492, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 752, 431, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 55, 506, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 100, 493, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 151, 461, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 202, 445, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 296, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 657, 504, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 259, 428, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 366, 433, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 582, 411, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 661, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 674, 537, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 694, 538, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 778, 417, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 449, 517, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 501, 544, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 530, 355, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 546, 354, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 636, 592, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 659, 600, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 87, 303, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 130, 322, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 682, 614, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 61, 292, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 113, 328, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 474, 531, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 529, 554, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 437, 427, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 455, 421, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 608, 405, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 626, 396, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 654, 525, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 691, 424, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 144, 110, 160, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 359, 148, 193, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 535, 236, 281, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 759, 332, 376, 0 },
    { 1, 0, 0x50, 4, 7, 0, 0, 0, 0, 0, 441, 345, 450, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 464, 444, 496, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 595, 442, 478, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 384, 259, 279, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 401, 250, 271, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 368, 251, 270, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 417, 243, 264, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 352, 242, 262, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 433, 233, 255, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 336, 229, 255, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 449, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 320, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 337, 219, 240, 0 },
    { 1, 0, 0x40, 4, 0x14, 0, 0, 0, 0, 0, 353, 208, 231, 0 },
    { 1, 0, 0x40, 4, 0x15, 0, 0, 0, 0, 0, 369, 203, 223, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 385, 192, 215, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 401, 187, 207, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 499, 317, 334, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x3E8, 0xEC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x275, 0x70, 0xF0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x276, 0x58, 0x1CC, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x277, 0x69, 0x134, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
FieldEvent stageEvents[] = {
    { -1, NULL, 0, NULL, NULL },
};
