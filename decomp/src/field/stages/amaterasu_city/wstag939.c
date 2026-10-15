/*
 * WSTAG939: Control Room, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 651 starts it
 * instead of WSTAG326.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x905
#define STAGE_FILE_8 0x764
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8CE
#define STAGE_FILE_8 0x754
#endif
/* Sets the stage up: its map and actors, playing the ambience ENV_0003 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0xF500, 0x17A00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x2B;
    FIELDSTG_state.music = MUSIC(0x2B, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
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
    { 0x140, 0x100, 0x168, 0x100, 0xA0, 0, 0x140, 0x1EE },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x140, 0x1ED },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x140, 0x1EC },
};
u16 actor2Talk0Conditions[] = { SPECIAL(0x54), 0, CODES_END };
u16 actor2Talk1Conditions[] = { SPECIAL(0x54), 1, FLAG(0x10, 0xC), 0, CODES_END };
u16 actor2Talk2Conditions[] = { SPECIAL(0x54), 1, FLAG(0x10, 0xC), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x6D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x6A },
    { actor2Talk1Conditions, NULL, 0x6B },
    { actor2Talk2Conditions, NULL, 0x6C },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x20, 4, 368, 264, 5 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x24, 5, 440, 253, 5 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x68, 6, 407, 269, 5 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 374, 301, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 390, 293, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 406, 285, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 422, 277, 0, 0 },
    { 1, 0, 0x40, 2, 0x36, 2, 0, 3, 4, 0, 458, 262, 0, 0 },
    { 1, 0, 0x49, 6, 0, 1, 0, 3, 4, 0, 84, 193, 0, 0 },
    { 1, 0, 0x5D, 6, 4, 1, 4, 7, 4, 0, 68, 252, 0, 0 },
    { 1, 0, 0x5D, 6, 5, 1, 5, 8, 4, 0, 131, 227, 0, 0 },
    { 1, 0, 0x5D, 6, 0xB, 1, 0xB, 0xE, 4, 0, 182, 153, 0, 0 },
    { 1, 0, 0x49, 6, 0, 1, 0, 3, 4, 0, 227, 178, 0, 0 },
    { 1, 0, 0x5D, 6, 7, 1, 7, 0xA, 4, 0, 272, 108, 0, 0 },
    { 1, 1, 0x40, 6, 0x32, 2, 0, 2, 0x10, 0, 438, 220, 0, 0 },
    { 1, 3, 0x40, 6, 0x33, 2, 0, 1, 0x10, 0, 438, 220, 0, 0 },
    { 1, 2, 0x40, 6, 0x34, 2, 0, 3, 4, 0, 444, 202, 0, 0 },
    { 1, 4, 0x40, 6, 0x35, 2, 0, 3, 4, 0, 444, 202, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 294, 261, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 310, 253, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 326, 245, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 342, 237, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 3, 4, 0, 374, 220, 0, 0 },
    { 1, 0, 0x40, 6, 0xF, 0, 0, 0, 0, 0, 448, 207, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28A, 0x3F8, 0x2DC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
