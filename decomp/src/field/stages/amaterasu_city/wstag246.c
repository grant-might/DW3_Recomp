/*
 * WSTAG246: Wedge's Shop, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG245.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x4D7
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x4E7
#endif
/* Sets the stage up: its map and actors, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x15C00, 0x18B00};
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
    { 0x180, 0x100, 0x1B0, 0x16E, 0x1C0, 0x6E, 0x150, 0x1FF },
    { 0x180, 0x100, 0x1B0, 0x196, 0x1C0, 0x96, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1B0, 0x1DE, 0x1C0, 0xDE, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B0, 0x1B6, 0x1C0, 0xB6, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1F2, 0x100, 0x2C8, 0, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1F2, 0x120, 0x2C8, 0x20, 0x170, 0x1FE },
    { 0x1C0, 0x100, 0x1E2, 0x13F, 0x288, 0x3F, 0x150, 0x1FD },
    { 0x180, 0x100, 0x1A0, 0x1C4, 0x180, 0xC4, 0x160, 0x1FD },
    { 0x1C0, 0x100, 0x1EA, 0x13F, 0x2A8, 0x3F, 0x170, 0x1FD },
};
u16 actor0Talk0Actions[] = { 0x7A11, 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7A0F, 1, CODES_END };
u16 actor9Talk0Actions[] = { 0x7A10, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x1A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x6F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x6D },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x71 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x70 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x72 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x6C },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, actor9Talk0Actions, 0x1A6 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1BB },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x69 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x6B },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x1BA },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x68 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x6A },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1C), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1C), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 8), 1, CODES_END };
u16 actor11Conditions[] = { ITEM(0, 8), 0, PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor12Conditions[] = { ITEM(0, 8), 0, PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, ITEM(0, 8), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 449, 281, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 304, 369, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 6, 287, 433, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 6, 287, 433, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x34, 7, 480, 338, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9D, 8, 480, 338, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 8, 480, 338, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9E, 9, 287, 433, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9E, 9, 287, 433, 5 };
FieldActorEntry actor9 = { NULL, actor9Talks, 0xB7, 0xA, 359, 308, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xFF, 0xB, 144, 352, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0xFF, 0xB, 152, 397, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0xFF, 0xB, 152, 397, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xFF, 0xB, 144, 352, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x119, 0xC, 152, 397, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x119, 0xC, 152, 397, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 384, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 492, 137, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 548, 165, 0, 0 },
    { 1, 0x64, 0x40, 6, 5, 0, 0, 0, 0, 0, 304, 99, 0, 0 },
    { 1, 0x65, 0x40, 6, 6, 0, 0, 0, 0, 0, 64, 285, 0, 0 },
    { 1, 0, 0x40, 6, 7, 0, 0, 0, 0, 0, 485, 145, 0, 0 },
    { 1, 0, 0x40, 6, 7, 0, 0, 0, 0, 0, 541, 173, 0, 0 },
    { 1, 0, 0xA0, 4, 0, 0, 0, 0, 0, 0, 144, 257, 380, 0 },
    { 1, 0, 0x78, 4, 1, 0, 0, 0, 0, 0, 256, 257, 337, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 384, 262, 287, 0 },
    { 1, 0, 0x64, 4, 3, 0, 0, 0, 0, 0, 240, 73, 150, 0 },
    { 1, 0, 0x64, 4, 4, 0, 0, 0, 0, 0, 0, 255, 335, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x1E0, 0x1DA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x240, 0x1AA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27D, 0x218, 0xE4, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27D, 0x128, 0x19C, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x11F, 0xBA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x110, 0xFE, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
