/*
 * WSTAG936: Water Tunnel, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 641 starts it
 * instead of WSTAG276.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8FF
#define STAGE_FILE_8 0x1B0
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8C5
#define STAGE_FILE_8 0x1A2
#endif
/* Sets the stage up: its map and actors, playing the ambience ENV_0004 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xFD00, 0x1CD00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2C;
    FIELDSTG_state.music = MUSIC(0x2C, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x18, 6), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x18, 6), 0, FLAG(0, 0), 0, CODES_END };
u16 actor0Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0x18, 6), 0, FLAG(0, 0), 1, FLAG(0, 1), 0, CODES_END };
u16 actor0Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor0Talk3Conditions[] = {
    FLAG(0x18, 6), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0x18, 6), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 0,
    CODES_END,
};
u16 actor0Talk4Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor0Talk5Conditions[] = {
    FLAG(0x18, 6), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 4), 0,
    FLAG(0, 3), 1,
    CODES_END,
};
u16 actor0Talk5Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor0Talk6Conditions[] = {
    FLAG(0x18, 6), 0,
    FLAG(0, 0), 1,
    FLAG(0, 1), 1,
    FLAG(0, 2), 1,
    FLAG(0, 3), 1,
    FLAG(0, 4), 1,
    CODES_END,
};
u16 actor0Talk6Actions[] = { FLAG(0x18, 6), 1, ITEM(0, 0x25), 1, SPECIAL(0x13), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x58 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x55 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x56 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x56 },
    { actor0Talk4Conditions, actor0Talk4Actions, 0x56 },
    { actor0Talk5Conditions, actor0Talk5Actions, 0x56 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0x57 },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x3F, 4, 175, 408, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x9F, 2, 0x37, 1, 0x37, 0x39, 6, 0, 450, 296, 0, 0 },
    { 1, 0, 0xBE, 2, 0x3A, 1, 0x3A, 0x3C, 6, 0, 560, 270, 0, 0 },
    { 1, 0, 0x8F, 2, 0x1D, 1, 0x1D, 0x31, 8, 0, 584, 352, 0, 0 },
    { 1, 0, 0xB6, 2, 6, 1, 6, 0x1C, 8, 0, 471, 277, 0, 0 },
    { 1, 0, 0xB6, 2, 6, 1, 6, 0x1C, 8, 0, 535, 294, 0, 0 },
    { 1, 0, 0x70, 2, 4, 0, 0, 0, 0, 0, 374, 310, 0, 0 },
    { 1, 0, 0xD0, 2, 5, 0, 0, 0, 0, 0, 438, 232, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 609, 253, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 4, 0, 633, 246, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 4, 0, 640, 242, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 1, 4, 0, 652, 281, 0, 0 },
    { 1, 0, 0x9F, 6, 0x37, 1, 0x37, 0x39, 6, 0, 366, 170, 0, 0 },
    { 1, 0, 0x67, 6, 0x3D, 1, 0x3D, 0x52, 8, 0, 507, 185, 0, 0 },
    { 1, 0, 0xB6, 6, 6, 1, 6, 0x1C, 8, 0, 421, 106, 0, 0 },
    { 1, 0x64, 0x40, 6, 2, 0, 0, 0, 0, 0, 91, 362, 0, 0 },
    { 1, 0x65, 0x40, 6, 3, 0, 0, 0, 0, 0, 770, 115, 0, 0 },
    { 1, 0, 0x90, 4, 0, 0, 0, 0, 0, 0, 0, 256, 410, 0 },
    { 1, 0, 0xB0, 4, 1, 0, 0, 0, 0, 0, 768, 0, 165, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x282, 0x13A, 0x144, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27F, 0xE8, 0x1A2, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
