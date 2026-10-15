/*
 * WSTAG256: Lamb Chop, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG255.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x282
#define STAGE_ARCHIVE 0x3C1
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x291
#define STAGE_ARCHIVE 0x3D1
#endif
/* Sets the stage up: its map and actors, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0xFB00, 0x13500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
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
    { 0x140, 0x100, 0x16A, 0x162, 0xA8, 0x62, 0x150, 0x1FF },
    { 0x140, 0x100, 0x140, 0x15A, 0, 0x5A, 0x160, 0x1FF },
    { 0x140, 0x100, 0x154, 0x172, 0x50, 0x72, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15C, 0x180, 0x70, 0x80, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x182, 0, 0x82, 0x160, 0x1FE },
    { 0x140, 0x100, 0x164, 0x18A, 0x90, 0x8A, 0x170, 0x1FE },
    { 0x140, 0x100, 0x148, 0x193, 0x20, 0x93, 0x150, 0x1FD },
    { 0x140, 0x100, 0x172, 0x160, 0xC8, 0x60, 0x160, 0x1FD },
    { 0x140, 0x100, 0x150, 0x19A, 0x40, 0x9A, 0x170, 0x1FD },
    { 0x140, 0x100, 0x158, 0x1A8, 0x60, 0xA8, 0x140, 0x1FC },
    { 0x140, 0x100, 0x140, 0x1AA, 0, 0xAA, 0x150, 0x1FC },
    { 0x140, 0x100, 0x160, 0x1AA, 0x80, 0xAA, 0x160, 0x1FC },
    { 0x140, 0x100, 0x168, 0x1AA, 0xA0, 0xAA, 0x170, 0x1FC },
    { 0x140, 0x100, 0x170, 0x1AA, 0xC0, 0xAA, 0x140, 0x1FB },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x78 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x7A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x7C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x84 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x80 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x82 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x87 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x89 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x77 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x7B },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x7D },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x86 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x88 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x85 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x7F },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x81 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x19, 4, 275, 255, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x1A, 5, 256, 321, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 6, 101, 203, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x36, 7, 133, 187, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x36, 7, 217, 257, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x38, 8, 217, 257, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x39, 9, 101, 203, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x3A, 0xA, 133, 187, 1 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x44, 0xB, 307, 240, 5 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x44, 0xB, 307, 240, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9D, 0xC, 275, 255, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9D, 0xC, 275, 255, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9E, 0xD, 256, 321, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9E, 0xD, 256, 321, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x9F, 0xE, 307, 240, 5 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x9F, 0xE, 307, 240, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0xA0, 0xF, 217, 257, 5 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0xA0, 0xF, 217, 257, 5 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0xA1, 0x10, 101, 203, 5 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0xA1, 0x10, 101, 203, 5 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0xA2, 0x11, 133, 187, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0xA2, 0x11, 133, 187, 1 };
FieldActorEntry actor24 = { actor24Conditions, NULL, 0xE0, 0x12, 267, 266, 1 };
FieldActorEntry actor25 = { actor25Conditions, NULL, 0xE0, 0x12, 267, 266, 1 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x10E, 0x13, 267, 266, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x10E, 0x13, 267, 266, 1 };
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
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 178, 254, 277, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 152, 216, 233, 0 },
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x112, 0x220, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x279, 0x68, 0x15A, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
