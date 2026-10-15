/*
 * WSTAG250: Junk Shop, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define STAGE_FILE 0x19B
#define STAGE_ARCHIVE 0x3BF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define STAGE_FILE 0x1A9
#define STAGE_ARCHIVE 0x3CF
#endif
/* Sets the stage up: its map and actors, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x13D00, 0x16F00};
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
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x174, 0x128, 0xD0, 0x28, 0x140, 0x1FE },
    { 0x140, 0x100, 0x174, 0x148, 0xD0, 0x48, 0x150, 0x1FE },
    { 0x140, 0x100, 0x174, 0x170, 0xD0, 0x70, 0x160, 0x1FE },
    { 0x180, 0x100, 0x1B6, 0x158, 0x1D8, 0x58, 0x170, 0x1FE },
    { 0x180, 0x100, 0x180, 0x190, 0x100, 0x90, 0x140, 0x1FD },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 0x13), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 0x13), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 0x13), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A04, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0x12), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0x12), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 0x12), 1, CODES_END };
u16 actor1Talk1Actions[] = { 0x7A03, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x4A },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x28 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x4B },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x29 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x2B },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x44A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x442 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x444 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x446 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x448 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x441 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x443 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x445 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x447 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x2A },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x440 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x438 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x43A },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x43C },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x43E },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x437 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x439 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x43B },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x43D },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x43F },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x449 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(5), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 451, 178, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 382, 331, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x35, 6, 533, 171, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x36, 7, 195, 402, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x9D, 8, 195, 402, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x9E, 9, 533, 171, 5 };
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
    &actor12,
    &actor13,
    &actor14,
    &actor15,
    &actor16,
    &actor17,
    &actor18,
    &actor19,
    &actor20,
    &actor21,
    &actor22,
    &actor23,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x3D, 4, 0, 224, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x53, 0xA, 0, 95, 248, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x53, 0xA, 0, 181, 470, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 1, 0x57, 0x59, 0xA, 0, 123, 226, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 148, 288, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 153, 422, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 114, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 155, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 168, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 1, 0x47, 0x49, 0xA, 0, 105, 333, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 1, 0x47, 0x49, 0xA, 0, 126, 300, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 1, 0x47, 0x49, 0xA, 0, 219, 451, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 59, 341, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 62, 328, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 63, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 95, 378, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 126, 244, 0, 0 },
    { 1, 0, 0x40, 6, 0x4D, 1, 0x4D, 0x50, 8, 0, 73, 439, 0, 0 },
    { 1, 0, 0x40, 6, 0, 1, 0, 5, 4, 0, 299, 241, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20D, 0x150, 0x98, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20D, 0x60, 0x150, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
