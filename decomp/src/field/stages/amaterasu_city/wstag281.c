/*
 * WSTAG281: El Dorado, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG280.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x507
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x517
#endif
/* Sets the stage up: its map and actors, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12000, 0x13400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
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
    { 0x140, 0x100, 0x160, 0x1AA, 0x80, 0xAA, 0x170, 0x1FF },
    { 0x140, 0x100, 0x166, 0x182, 0x98, 0x82, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16E, 0x182, 0xB8, 0x82, 0x170, 0x1FE },
    { 0x140, 0x100, 0x176, 0x182, 0xD8, 0x82, 0x140, 0x1FD },
    { 0x140, 0x100, 0x150, 0x189, 0x40, 0x89, 0x160, 0x1FD },
    { 0x140, 0x100, 0x168, 0x1AA, 0xA0, 0xAA, 0x170, 0x1FD },
    { 0x140, 0x100, 0x170, 0x1AA, 0xC0, 0xAA, 0x140, 0x1FC },
    { 0x140, 0x100, 0x148, 0x1B1, 0x20, 0xB1, 0x150, 0x1FC },
    { 0x140, 0x100, 0x150, 0x1B1, 0x40, 0xB1, 0x160, 0x1FC },
    { 0x140, 0x100, 0x158, 0x1CA, 0x60, 0xCA, 0x170, 0x1FC },
    { 0x140, 0x100, 0x160, 0x1CA, 0x80, 0xCA, 0x140, 0x1FB },
    { 0x140, 0x100, 0x158, 0x1A2, 0x60, 0xA2, 0x160, 0x1FB },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x170, 0x1CA, 0xC0, 0xCA, 0x170, 0x1FB },
    { 0x140, 0x100, 0x140, 0x1A9, 0, 0xA9, 0x140, 0x1FA },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x8F },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x91 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x93 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xA0 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x97 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x9A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x8E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x90 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x99 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x9B },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x92 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x94 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x96 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x98 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x1F6 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x9C },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x9E },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x9D },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x95 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2E, 4, 471, 245, 5 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 4, 471, 245, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2F, 5, 216, 277, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x30, 6, 216, 277, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x35, 7, 264, 301, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x38, 8, 264, 301, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x39, 9, 352, 296, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x3A, 0xA, 352, 296, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 0xB, 471, 245, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 0xB, 471, 245, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9E, 0xC, 352, 296, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 0xC, 352, 296, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9F, 0xD, 216, 277, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9F, 0xD, 216, 277, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xA0, 0xE, 264, 301, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xA0, 0xE, 264, 301, 3 };
FieldActorEntry actor16 = { NULL, actor16Talks, 0x10B, 0xF, 353, 160, 7 };
FieldActorEntry actor17 = { NULL, NULL, 0x10C, 0x10, 360, 172, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x119, 0x11, 305, 193, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x119, 0x11, 305, 193, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x125, 0x12, 368, 224, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x125, 0x12, 305, 193, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 427, 90, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 84, 130, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 116, 114, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 148, 98, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 272, 44, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 368, 36, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 417, 91, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 453, 113, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 473, 112, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 493, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 497, 124, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 4, 0, 292, 104, 0, 0 },
    { 1, 0, 0x40, 6, 0xA, 0, 0, 0, 0, 0, 400, 59, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 224, 68, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 392, 48, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 416, 60, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 208, 263, 288, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 384, 191, 217, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 320, 169, 184, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 304, 161, 174, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 336, 161, 174, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 288, 153, 166, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 352, 153, 166, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 272, 144, 158, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 368, 145, 158, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x271, 0x180, 0xD8, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x281, 0x92, 0x1A2, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0x110, 0xB8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0x100, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
