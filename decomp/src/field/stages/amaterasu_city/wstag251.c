/*
 * WSTAG251: Junk Shop, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG250.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x4D4
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x4E4
#endif
/* Sets the stage up: its map and actors, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12100, 0x17D00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 7;
    FIELDSTG_state.music = MUSIC(7, 0);
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
    { 0x140, 0x100, 0x158, 0x1C8, 0x60, 0xC8, 0x140, 0x1FE },
    { 0x180, 0x100, 0x180, 0x100, 0x100, 0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x178, 0x1D8, 0xE0, 0xD8, 0x160, 0x1FE },
    { 0x180, 0x100, 0x18A, 0x100, 0x128, 0, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1A6, 0x100, 0x198, 0, 0x140, 0x1FD },
    { 0x180, 0x100, 0x1AE, 0x100, 0x1B8, 0, 0x150, 0x1FD },
};
u16 actor0Talk0Actions[] = { 0x7A13, 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7A12, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x1A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1AA },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x73 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x75 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x76 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x74 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 451, 178, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 382, 331, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 6, 533, 171, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x36, 7, 533, 171, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 8, 195, 402, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3A, 9, 195, 402, 1 };
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
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x3D, 4, 0, 224, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x4D, 1, 0x4D, 0x50, 8, 0, 73, 439, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x60, 0xA, 0, 90, 278, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x60, 0xA, 0, 133, 435, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x48, 0xA, 0, 67, 338, 0, 0 },
    { 1, 0, 0x40, 6, 0, 0, 0, 0, 0, 0, 314, 248, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27C, 0x150, 0x98, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27C, 0x60, 0x150, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
