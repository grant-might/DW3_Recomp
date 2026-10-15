/*
 * WSTAG686: Underground Cave, Bai Hu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG685.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x5EF
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x5FF
#endif
/* Sets the stage up: its map and actors, playing the ambience ENV_0020 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x3D200, 0x13A00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3C;
    FIELDSTG_state.music = MUSIC(0x3C, 0);
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
    { 0x140, 0x100, 0x16E, 0x158, 0xB8, 0x58, 0x150, 0x1FF },
    { 0x140, 0x100, 0x176, 0x158, 0xD8, 0x58, 0x160, 0x1FF },
    { 0x140, 0x100, 0x164, 0x180, 0x90, 0x80, 0x170, 0x1FF },
    { 0x140, 0x100, 0x16C, 0x188, 0xB0, 0x88, 0x150, 0x1FE },
    { 0x140, 0x100, 0x174, 0x188, 0xD0, 0x88, 0x160, 0x1FE },
    { 0x140, 0x100, 0x14E, 0x18D, 0x38, 0x8D, 0x170, 0x1FE },
    { 0x140, 0x100, 0x156, 0x195, 0x58, 0x95, 0x140, 0x1FD },
    { 0x140, 0x100, 0x140, 0x1A6, 0, 0xA6, 0x150, 0x1FD },
    { 0x140, 0x100, 0x16C, 0x1A8, 0xB0, 0xA8, 0x160, 0x1FD },
    { 0x140, 0x100, 0x174, 0x1A8, 0xD0, 0xA8, 0x170, 0x1FD },
};
u16 actor3Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor3Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor5Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor5Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x165 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x166 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x167 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x15B },
    { actor3Talk1Conditions, NULL, 0x15C },
    { actor3Talk2Conditions, NULL, 0x15D },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x15F },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x160 },
    { actor5Talk1Conditions, NULL, 0x161 },
    { actor5Talk2Conditions, NULL, 0x162 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x164 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x168 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x168 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x169 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x169 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x16A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x16A },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x15E },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x163 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x25, 4, 825, 269, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x26, 5, 560, 304, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x27, 6, 376, 397, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x39, 7, 560, 448, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 7, 560, 448, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3A, 8, 1017, 309, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x3A, 8, 584, 436, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 9, 825, 269, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9D, 9, 825, 269, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9E, 0xA, 560, 304, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9E, 0xA, 560, 304, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9F, 0xB, 376, 397, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9F, 0xB, 376, 397, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0xA0, 0xC, 560, 448, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0xA1, 0xD, 1017, 309, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 177, 76, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 244, 43, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 6, 0, 350, 54, 0, 0 },
    { 1, 0, 0x80, 6, 4, 0, 0, 0, 0, 0, 390, 289, 0, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 1025, 223, 307, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 492, 452, 508, 0 },
    { 1, 0, 0x42, 4, 2, 0, 0, 0, 0, 0, 625, 443, 501, 0 },
    { 1, 0, 0x5B, 4, 3, 0, 0, 0, 0, 0, 639, 378, 454, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C7, 0x290, 0x1F0, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 9, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 8, 0x1C4, 0xA0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 8, 0x1D5, 0x12C, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1BF, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1AF, 0x198, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
