/*
 * WSTAG276: Water Tunnel, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG275.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x51F
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x52F
#endif
/* Sets the stage up: its map and actors, playing the ambience ENV_0004 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10100, 0x1C800};
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
    { 0x180, 0x100, 0x198, 0x128, 0x160, 0x28, 0x160, 0x1FD },
    { 0x180, 0x100, 0x1A0, 0x128, 0x180, 0x28, 0x170, 0x1FD },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x8C },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x8B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x8D },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x33, 4, 256, 443, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x9D, 5, 256, 443, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x9D, 5, 256, 443, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x9F, 2, 0x37, 1, 0x37, 0x39, 6, 0, 450, 296, 0, 0 },
    { 1, 0, 0xBE, 2, 0x3A, 1, 0x3A, 0x3C, 6, 0, 560, 270, 0, 0 },
    { 1, 0, 0x8F, 2, 0x1D, 1, 0x1D, 0x31, 8, 0, 584, 352, 0, 0 },
    { 1, 0, 0xB6, 2, 6, 1, 6, 0x1C, 8, 0, 471, 277, 0, 0 },
    { 1, 0, 0xB6, 2, 6, 1, 6, 0x1C, 8, 0, 535, 294, 0, 0 },
    { 1, 0, 0x40, 2, 0x53, 0, 0, 0, 0, 0, 90, 366, 0, 0 },
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
    { 1, 0, 0x40, 6, 0, 0, 0, 0, 0, 0, 403, 378, 0, 0 },
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
