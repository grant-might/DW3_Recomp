/*
 * WSTAG255: Lamb Chop, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define STAGE_FILE 0x19D
#define STAGE_ARCHIVE 0x3C0
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define STAGE_FILE 0x1AB
#define STAGE_ARCHIVE 0x3D0
#endif
/* Sets the stage up: its map and actors, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x10B00, 0x12C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x16E, 0x178, 0xB8, 0x78, 0x150, 0x1FF },
    { 0x140, 0x100, 0x140, 0x158, 0, 0x58, 0x160, 0x1FF },
    { 0x140, 0x100, 0x176, 0x178, 0xD8, 0x78, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15A, 0x162, 0x68, 0x62, 0x150, 0x1FE },
    { 0x140, 0x100, 0x152, 0x17A, 0x48, 0x7A, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x180, 0, 0x80, 0x170, 0x1FE },
    { 0x140, 0x100, 0x164, 0x183, 0x90, 0x83, 0x150, 0x1FD },
    { 0x140, 0x100, 0x15A, 0x18A, 0x68, 0x8A, 0x160, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0xC), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0xC), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 0xC), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0x1A, 0x25), 0, CODES_END };
u16 actor13Talk0Actions[] = { FLAG(0x1A, 0x25), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0x1A, 0x25), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0x1A, 0x25), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0x1A, 0x25), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0x1A, 0x25), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x243 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x23D },
    { actor1Talk1Conditions, NULL, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x23E },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x23F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x23C },
    { actor4Talk1Conditions, NULL, 0x4A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x240 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x241 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x23C },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x242 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x4A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x22D },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x229 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, actor13Talk0Actions, 0xB7 },
    { actor13Talk1Conditions, NULL, 0xB8 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x22E },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x2D },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xB7 },
    { actor16Talk1Conditions, NULL, 0xB8 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x22F },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x230 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x22A },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x22B },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x22C },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x232 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x236 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x239 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x2E },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x23B },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x228 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x237 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x238 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x233 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x234 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x235 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x244 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x23A },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x231 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x16), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x15), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor13Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    FLAG(0x1C, 8), 1,
    CODES_END,
};
u16 actor14Conditions[] = { SPECIAL(0x18), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor15Conditions[] = { PROGRESS(4), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x22), 1, FLAG(0x1C, 8), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x19), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xE), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x16), 1, FLAG(0x1C, 8), 0, CODES_END };
u16 actor22Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    FLAG(0x1C, 8), 0,
    CODES_END,
};
u16 actor23Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor36Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor41Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x35, 6, 101, 203, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x35, 6, 217, 257, 5 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x44, 7, 307, 240, 5 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0x44, 7, 307, 240, 5 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x9D, 8, 275, 255, 1 };
FieldActorEntry actor36 = { actor36Conditions, NULL, 0x9E, 9, 307, 240, 5 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x9F, 0xA, 217, 257, 5 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0xA0, 0xB, 256, 321, 1 };
FieldActorEntry actor39 = { actor39Conditions, NULL, 0xE0, 0xC, 259, 263, 1 };
FieldActorEntry actor40 = { actor40Conditions, NULL, 0xE0, 0xC, 259, 263, 1 };
FieldActorEntry actor41 = { actor41Conditions, NULL, 0x10E, 0xD, 259, 263, 1 };
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
    &actor24,
    &actor25,
    &actor26,
    &actor27,
    &actor28,
    &actor29,
    &actor30,
    &actor31,
    &actor32,
    &actor33,
    &actor34,
    &actor35,
    &actor36,
    &actor37,
    &actor38,
    &actor39,
    &actor40,
    &actor41,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 115, 131, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 163, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 227, 75, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 280, 73, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 328, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 376, 121, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 2, 0, 2, 4, 0, 117, 147, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 2, 0, 2, 4, 0, 165, 124, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 2, 0, 2, 4, 0, 229, 92, 0, 0 },
    { 1, 0, 0x40, 2, 0xB, 2, 0, 2, 4, 0, 259, 90, 0, 0 },
    { 1, 0, 0x40, 2, 0xB, 2, 0, 2, 4, 0, 307, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0xB, 2, 0, 2, 4, 0, 355, 139, 0, 0 },
    { 1, 0x64, 0x40, 6, 0xF, 0, 0, 0, 0, 0, 350, 125, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 192, 271, 301, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 184, 263, 288, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 152, 216, 233, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 178, 254, 277, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 280, 256, 280, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 264, 248, 271, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 248, 240, 264, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 232, 232, 256, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 217, 222, 248, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 312, 231, 263, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 332, 222, 253, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x112, 0x220, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20A, 0x68, 0x15A, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
