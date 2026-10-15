/*
 * WSTAG425: Zephyr Tower, Seiryu City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define STAGE_FILE 0x245
#define STAGE_FILE_8 0x23D
#define STAGE_ARCHIVE 0x321
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define STAGE_FILE 0x254
#define STAGE_FILE_8 0x24C
#define STAGE_ARCHIVE 0x330
#endif
/* Sets the stage up: its map and actors, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1A200, 0x17A00};
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
    { 0x140, 0x100, 0x15C, 0x1A7, 0x70, 0xA7, 0x170, 0x1FF },
    { 0x140, 0x100, 0x176, 0x173, 0xD8, 0x73, 0x170, 0x1FE },
    { 0x140, 0x100, 0x160, 0x147, 0x80, 0x47, 0x170, 0x1FD },
    { 0x180, 0x100, 0x190, 0x11A, 0x140, 0x1A, 0x170, 0x1FC },
    { 0x180, 0x100, 0x198, 0x11A, 0x160, 0x1A, 0x170, 0x1FB },
    { 0x180, 0x100, 0x180, 0x11B, 0x100, 0x1B, 0x170, 0x1FA },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x170, 0x1F9 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 8), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 8), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A20, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0xF), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0xF), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 0xF), 1, CODES_END };
u16 actor1Talk1Actions[] = { 0x7A06, 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0xE), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1A, 0xE), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0xE), 1, CODES_END };
u16 actor2Talk1Actions[] = { 0x7A05, 1, CODES_END };
u16 actor13Talk0Conditions[] = { ITEM(0, 0x18F), 0, CODES_END };
u16 actor13Talk0Actions[] = { ITEM(0, 0x18F), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor13Talk1Conditions[] = { ITEM(0, 0x18F), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0x1A, 0x1C), 0, PROGRESS(4), 1, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0x1A, 0x1C), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0x1A, 0x1C), 1, PROGRESS(4), 1, CODES_END };
u16 actor15Talk1Actions[] = { 0x7A37, 1, CODES_END };
u16 actor15Talk2Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor15Talk2Actions[] = { 0x7A37, 1, CODES_END };
u16 actor15Talk3Conditions[] = { SPECIAL(0x3E), 1, CODES_END };
u16 actor15Talk3Actions[] = { 0x7A37, 1, CODES_END };
u16 actor15Talk4Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor15Talk4Actions[] = { 0x7A38, 1, CODES_END };
u16 actor15Talk5Conditions[] = { SPECIAL(0x20), 1, CODES_END };
u16 actor15Talk5Actions[] = { 0x7A38, 1, CODES_END };
u16 actor15Talk6Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor15Talk6Actions[] = { 0x7A39, 1, CODES_END };
u16 actor15Talk7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor15Talk7Actions[] = { 0x7A3A, 1, CODES_END };
u16 actor15Talk8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Talk8Actions[] = { 0x7A3A, 1, CODES_END };
u16 actor15Talk9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor15Talk9Actions[] = { 0x7A3A, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 4 },
    { actor0Talk1Conditions, actor0Talk1Actions, 5 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x121 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0xF3 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x123 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x41 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x43 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x49 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x48 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x47 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x46 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x45 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x44 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x42 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x4B },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0x128 },
    { actor13Talk1Conditions, NULL, 0x16C },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x4A },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, actor15Talk0Actions, 0x124 },
    { actor15Talk1Conditions, actor15Talk1Actions, 1 },
    { actor15Talk2Conditions, actor15Talk2Actions, 1 },
    { actor15Talk3Conditions, actor15Talk3Actions, 1 },
    { actor15Talk4Conditions, actor15Talk4Actions, 1 },
    { actor15Talk5Conditions, actor15Talk5Actions, 1 },
    { actor15Talk6Conditions, actor15Talk6Actions, 1 },
    { actor15Talk7Conditions, actor15Talk7Actions, 1 },
    { actor15Talk8Conditions, actor15Talk8Actions, 1 },
    { actor15Talk9Conditions, actor15Talk9Actions, 1 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x174 },
    { NULL, NULL, 0 },
};
u16 actor3Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1C, 0x45), 1, PROGRESS(6), 1, FLAG(0x1C, 0x46), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
u16 actor16Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 345, 197, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x16, 5, 408, 341, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x17, 6, 330, 380, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 7, 384, 418, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2E, 7, 384, 418, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x40, 8, 207, 152, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9D, 9, 384, 418, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xCE, 0xA, 128, 201, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0xCE, 0xA, 128, 201, 7 };
FieldActorEntry actor17 = { NULL, NULL, 0xDB, 0xB, 345, 213, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 304, 189, 222, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 290, 166, 222, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 328, 195, 215, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 336, 187, 207, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 352, 180, 199, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 367, 164, 199, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 472, 281, 331, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 496, 352, 399, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 271, 382, 399, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 289, 374, 391, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 336, 350, 367, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 353, 343, 358, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 366, 323, 351, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 416, 309, 326, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 435, 292, 319, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 176, 155, 211, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 160, 148, 199, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 144, 140, 191, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 128, 132, 184, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 118, 127, 179, 0 },
    { 1, 0, 0x64, 4, 0x14, 0, 0, 0, 0, 0, 448, 251, 310, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22E, 0x1E8, 0x1AC, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x100, 0xB0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x10F, 0xF6, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xB0, 0xD8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xBF, 0x11E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
