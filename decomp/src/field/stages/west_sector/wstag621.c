/*
 * WSTAG621: Secret Room, West Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG620.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x607
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x617
#endif
/* Sets the stage up: its map and actors, playing BGM_0019 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x21C00, 0x18300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x15;
    FIELDSTG_state.music = MUSIC(0x15, 0);
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
    { 0x140, 0x100, 0x174, 0x150, 0xD0, 0x50, 0x160, 0x1F6 },
    { 0x140, 0x100, 0x14C, 0x178, 0x30, 0x78, 0x160, 0x1F5 },
    { 0x140, 0x100, 0x154, 0x178, 0x50, 0x78, 0x170, 0x1F5 },
    { 0x140, 0x100, 0x16C, 0x190, 0xB0, 0x90, 0x140, 0x1F4 },
    { 0x140, 0x100, 0x140, 0x198, 0, 0x98, 0x160, 0x1F4 },
    { 0x140, 0x100, 0x174, 0x198, 0xD0, 0x98, 0x170, 0x1F4 },
    { 0x140, 0x100, 0x148, 0x1A0, 0x20, 0xA0, 0x140, 0x1F3 },
    { 0x140, 0x100, 0x150, 0x1A0, 0x40, 0xA0, 0x150, 0x1F3 },
    { 0x140, 0x100, 0x158, 0x1A0, 0x60, 0xA0, 0x160, 0x1F3 },
    { 0x140, 0x100, 0x15C, 0x178, 0x70, 0x78, 0x170, 0x1F3 },
};
u16 actor0Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor0Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor0Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor1Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor1Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor1Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor2Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Talk0Conditions[] = { ITEM(3, 0x68), 1, CODES_END };
u16 actor9Talk1Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk2Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 1, ITEM(2, 0x64), 0, CODES_END };
u16 actor9Talk3Conditions[] = { ITEM(3, 0x68), 0, FLAG(0, 0), 1, ITEM(2, 0x64), 1, CODES_END };
u16 actor9Talk3Actions[] = {
    ITEM(3, 0x68), 1,
    ITEM(3, 0x67), 0,
    ITEM(2, 0x64), 0,
    SPECIAL(0x13), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0x11F },
    { actor0Talk1Conditions, NULL, 0x120 },
    { actor0Talk2Conditions, NULL, 0x127 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x122 },
    { actor1Talk1Conditions, NULL, 0x123 },
    { actor1Talk2Conditions, NULL, 0x128 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x125 },
    { actor2Talk1Conditions, NULL, 0x126 },
    { actor2Talk2Conditions, NULL, 0x129 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x11E },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x121 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x124 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x2FE },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x2FF },
    { actor9Talk2Conditions, NULL, 0x300 },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x301 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor9Conditions[] = {
    SPECIAL(0x42), 1,
    SPECIAL(0x4A), 1,
    ITEM(3, 0x67), 1,
    ITEM(3, 0x68), 0,
    CODES_END,
};
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 385, 417, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x24, 5, 449, 360, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x59, 6, 515, 414, 7 };
FieldActorEntry actor3 = { NULL, NULL, 0x9A, 7, 641, 350, 0 };
FieldActorEntry actor4 = { NULL, NULL, 0x9B, 8, 593, 325, 1 };
FieldActorEntry actor5 = { NULL, NULL, 0x9C, 9, 545, 301, 2 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9D, 0xA, 385, 417, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9E, 0xB, 449, 360, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x9F, 0xC, 515, 414, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xAC, 0xD, 400, 232, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x40, 2, 0, 3, 6, 0, 521, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x40, 2, 0, 3, 6, 0, 569, 236, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 3, 6, 0, 617, 260, 0, 0 },
    { 1, 0, 0x40, 2, 0x43, 2, 0, 5, 6, 0, 392, 163, 0, 0 },
    { 1, 0, 0x40, 2, 0x44, 2, 0, 5, 6, 0, 369, 205, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 2, 0, 3, 6, 0, 360, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 2, 0, 5, 6, 0, 361, 186, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 377, 371, 399, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2BE, 0x2D4, 0xC4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x1D0, 0xF8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x1E0, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
