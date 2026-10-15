/*
 * WSTAG951: Duel Island, East Sector, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 663 starts it
 * instead of WSTAG386.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x91D
#define STAGE_FILE_8 0x513
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x8F2
#define STAGE_FILE_8 0x503
#endif
/* Sets the stage up: its map, actors and events, playing BGM_0008 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x3C500, 0x3D200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xA;
    FIELDSTG_state.music = MUSIC(0xA, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

/* Defined below, after the tables that use them */
extern s16 script1620[];
extern s16 script1622[];
extern s16 script1624[];
extern s16 script1626[];
extern s16 script1628[];

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1AC, 0x150, 0x1B0, 0x50, 0x170, 0x1F8 },
    { 0x140, 0x100, 0x172, 0x1B3, 0xC8, 0xB3, 0x140, 0x1F7 },
    { 0x140, 0x100, 0x16A, 0x1C5, 0xA8, 0xC5, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x1AC, 0x100, 0x1B0, 0, 0x170, 0x1F7 },
    { 0x180, 0x100, 0x1B4, 0x150, 0x1D0, 0x50, 0x140, 0x1F6 },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x150, 0x1F6 },
    { 0x180, 0x100, 0x1AC, 0x170, 0x1B0, 0x70, 0x170, 0x1F6 },
    { 0x180, 0x100, 0x1B4, 0x170, 0x1D0, 0x70, 0x140, 0x1F5 },
    { 0x180, 0x100, 0x180, 0x178, 0x100, 0x78, 0x150, 0x1F5 },
    { 0x180, 0x100, 0x188, 0x178, 0x120, 0x78, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x140, 0x1F4 },
    { 0x180, 0x100, 0x1AC, 0x128, 0x1B0, 0x28, 0x150, 0x1F4 },
    { 0x180, 0x100, 0x1B4, 0x128, 0x1D0, 0x28, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x190, 0x178, 0x140, 0x78, 0x140, 0x1F3 },
    { 0x180, 0x100, 0x198, 0x178, 0x160, 0x78, 0x150, 0x1F3 },
};
u16 actor5Talk0Conditions[] = { FLAG(0x10, 0x11), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x10, 0x11), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0x10, 0x11), 0, FLAG(0, 0x11), 0, FLAG(0, 0), 1, CODES_END };
u16 actor5Talk2Actions[] = { CARD_BATTLE(0x48, 1), 1, CODES_END };
u16 actor5Talk3Conditions[] = {
    FLAG(0x10, 0x11), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor5Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor5Talk4Conditions[] = {
    FLAG(0x10, 0x11), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor5Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 0), 0,
    FLAG(0x10, 0x11), 1,
    ITEM(0, 0x24), 1,
    SPECIAL(0x13), 1,
    CODES_END,
};
u16 actor8Talk0Conditions[] = { FLAG(0x10, 0x12), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0x10, 0x12), 0, FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0x10, 0x12), 0, FLAG(0, 0x11), 0, FLAG(0, 1), 1, CODES_END };
u16 actor8Talk2Actions[] = { CARD_BATTLE(0x4F, 1), 1, CODES_END };
u16 actor8Talk3Conditions[] = {
    FLAG(0x10, 0x12), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor8Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 1), 0, CODES_END };
u16 actor8Talk4Conditions[] = {
    FLAG(0x10, 0x12), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor8Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 1), 0,
    FLAG(0x10, 0x12), 1,
    START_EVENT(0x78), 1,
    CODES_END,
};
u16 actor9Talk0Conditions[] = { FLAG(0x10, 0x13), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0x10, 0x13), 0, FLAG(0, 0x11), 0, FLAG(0, 2), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0x10, 0x13), 0, FLAG(0, 0x11), 0, FLAG(0, 2), 1, CODES_END };
u16 actor9Talk2Actions[] = { CARD_BATTLE(0x4C, 1), 1, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0x10, 0x13), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 2), 0, CODES_END };
u16 actor9Talk4Conditions[] = {
    FLAG(0x10, 0x13), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor9Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 2), 0,
    FLAG(0x10, 0x13), 1,
    START_EVENT(0x77), 1,
    CODES_END,
};
u16 actor10Talk0Conditions[] = { FLAG(0x10, 0x14), 1, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0x10, 0x14), 0, FLAG(0, 0x11), 0, FLAG(0, 3), 0, CODES_END };
u16 actor10Talk1Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0x10, 0x14), 0, FLAG(0, 0x11), 0, FLAG(0, 3), 1, CODES_END };
u16 actor10Talk2Actions[] = { CARD_BATTLE(0x50, 1), 1, CODES_END };
u16 actor10Talk3Conditions[] = {
    FLAG(0x10, 0x14), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor10Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 3), 0, CODES_END };
u16 actor10Talk4Conditions[] = {
    FLAG(0x10, 0x14), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor10Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 3), 0,
    FLAG(0x10, 0x14), 1,
    START_EVENT(0x76), 1,
    CODES_END,
};
u16 actor11Talk0Conditions[] = { FLAG(0x10, 0x15), 1, CODES_END };
u16 actor11Talk1Conditions[] = { FLAG(0x10, 0x15), 0, FLAG(0, 0x11), 0, FLAG(0, 4), 0, CODES_END };
u16 actor11Talk1Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor11Talk2Conditions[] = { FLAG(0x10, 0x15), 0, FLAG(0, 0x11), 0, FLAG(0, 4), 1, CODES_END };
u16 actor11Talk2Actions[] = { CARD_BATTLE(0x4A, 1), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    FLAG(0x10, 0x15), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    CODES_END,
};
u16 actor11Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 4), 0, CODES_END };
u16 actor11Talk4Conditions[] = {
    FLAG(0x10, 0x15), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor11Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 4), 0,
    FLAG(0x10, 0x15), 1,
    START_EVENT(0x75), 1,
    CODES_END,
};
u16 actor12Talk0Conditions[] = { FLAG(0x10, 0x16), 1, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0x10, 0x16), 0, FLAG(0, 0x11), 0, FLAG(0, 5), 0, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 5), 1, CODES_END };
u16 actor12Talk2Conditions[] = { FLAG(0x10, 0x16), 0, FLAG(0, 0x11), 0, FLAG(0, 5), 1, CODES_END };
u16 actor12Talk2Actions[] = { CARD_BATTLE(0x4E, 1), 1, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x10, 0x16), 0,
    CODES_END,
};
u16 actor12Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 5), 0, CODES_END };
u16 actor12Talk4Conditions[] = {
    FLAG(0x10, 0x16), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor12Talk4Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0, 5), 0,
    FLAG(0x10, 0x16), 1,
    START_EVENT(0x74), 1,
    CODES_END,
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x5A },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x6E },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x69 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x64 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x5F },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x55 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x51 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x52 },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x53 },
    { actor5Talk4Conditions, actor5Talk4Actions, 0x54 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x89 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x5A },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x56 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x57 },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x58 },
    { actor8Talk4Conditions, actor8Talk4Actions, 0x59 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x5F },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x5B },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x5C },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x5D },
    { actor9Talk4Conditions, actor9Talk4Actions, 0x5E },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0x64 },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x60 },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x61 },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x62 },
    { actor10Talk4Conditions, actor10Talk4Actions, 0x63 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x69 },
    { actor11Talk1Conditions, actor11Talk1Actions, 0x65 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x66 },
    { actor11Talk3Conditions, actor11Talk3Actions, 0x67 },
    { actor11Talk4Conditions, actor11Talk4Actions, 0x68 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x6E },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x6A },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x6B },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x6C },
    { actor12Talk4Conditions, actor12Talk4Actions, 0x6D },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x88 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x8B },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x10, 0x12), 1, CODES_END };
u16 actor1Conditions[] = { FLAG(0x10, 0x16), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(0x10, 0x15), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(0x10, 0x14), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0x10, 0x13), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x10, 0x12), 0, CODES_END };
u16 actor9Conditions[] = { FLAG(0x10, 0x13), 0, CODES_END };
u16 actor10Conditions[] = { FLAG(0x10, 0x14), 0, CODES_END };
u16 actor11Conditions[] = { FLAG(0x10, 0x15), 0, CODES_END };
u16 actor12Conditions[] = { FLAG(0x10, 0x16), 0, CODES_END };
u16 actor13Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor14Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2E, 4, 260, 247, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x32, 5, 504, 496, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x34, 6, 321, 485, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x35, 7, 204, 462, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x39, 8, 341, 349, 1 };
FieldActorEntry actor5 = { NULL, actor5Talks, 0x66, 9, 159, 177, 7 };
FieldActorEntry actor6 = { NULL, actor6Talks, 0x16F, 0xA, 929, 417, 1 };
FieldActorEntry actor7 = { NULL, actor7Talks, 0x177, 0xB, 769, 209, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x18F, 0xC, 216, 269, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x190, 0xD, 297, 373, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x191, 0xE, 272, 497, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x192, 0xF, 375, 493, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x193, 0x10, 480, 521, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x194, 0x11, 501, 549, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x195, 0x12, 522, 538, 7 };
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
    { 1, 0, 0x40, 2, 0x47, 2, 0, 5, 8, 0, 85, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 135, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 131, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 127, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 123, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 119, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 89, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 85, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 77, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 133, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 125, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 121, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 91, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 87, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 83, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 75, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x4A, 2, 0, 7, 4, 0, 103, 201, 0, 0 },
    { 1, 0, 0x40, 2, 0x4B, 2, 0, 7, 4, 0, 105, 205, 0, 0 },
    { 1, 0, 0x80, 2, 1, 0, 0, 0, 0, 0, 768, 128, 0, 0 },
    { 1, 0, 0x45, 2, 2, 0, 0, 0, 0, 0, 880, 156, 0, 0 },
    { 1, 0, 0x78, 2, 3, 0, 0, 0, 0, 0, 896, 256, 0, 0 },
    { 1, 0, 0x55, 2, 4, 0, 0, 0, 0, 0, 984, 299, 0, 0 },
    { 1, 0, 0x33, 2, 5, 0, 0, 0, 0, 0, 278, 813, 0, 0 },
    { 1, 0, 0x1E, 2, 6, 0, 0, 0, 0, 0, 323, 829, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 220, 180, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 232, 138, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 297, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 300, 284, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 168, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 220, 184, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 234, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 235, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 280, 241, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 300, 270, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 203, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 236, 1022, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 237, 973, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 238, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 273, 861, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 309, 635, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 320, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 396, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 575, 884, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 630, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 769, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1138, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 281, 1085, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 320, 983, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 359, 980, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 503, 793, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 657, 718, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 95, 790, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 117, 1016, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 145, 858, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 178, 1068, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 185, 1000, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 212, 747, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 288, 998, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 302, 1076, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 391, 819, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 541, 902, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 773, 705, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 800, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 850, 469, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 967, 574, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 101, 951, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 275, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 461, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 509, 339, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 534, 794, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 600, 441, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 683, 709, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 715, 508, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 874, 539, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 917, 557, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 131, 1058, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 273, 953, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 19, 963, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 106, 1043, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 122, 952, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 249, 923, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 367, 919, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 545, 804, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 582, 430, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 742, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 931, 568, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1086, 533, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 56, 960, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 117, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 158, 846, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 171, 949, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 216, 834, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 221, 1050, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 227, 1041, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 235, 750, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 280, 713, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 286, 1071, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 287, 1094, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 305, 665, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 332, 909, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 333, 845, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 363, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 378, 309, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 380, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 390, 431, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 391, 981, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 398, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 412, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 430, 770, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 470, 562, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 474, 567, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 499, 344, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 515, 599, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 517, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 526, 345, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 534, 910, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 546, 377, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 591, 852, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 616, 1076, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 622, 1085, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 633, 1077, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 694, 498, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 706, 727, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 775, 559, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 785, 551, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 793, 661, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 797, 555, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 798, 492, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 838, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 916, 780, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 922, 772, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 929, 778, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1026, 1107, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1032, 1114, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1039, 1107, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1047, 546, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1066, 898, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1149, 890, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1161, 892, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 477, 577, 626, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xC0, 0xC2, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xCF, 0x106, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x110, 0x12B, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x11F, 0x16E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x100, 0x192, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xF1, 0x1D6, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x330, 0xEA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x33F, 0x12E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x2DE, 0x141, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x2CE, 0x186, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 3, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1620, script1620, EVENT_TEXT(0xA), NULL, NULL },
    { 1622, script1622, EVENT_TEXT(0xB), NULL, NULL },
    { 1624, script1624, EVENT_TEXT(0xC), NULL, NULL },
    { 1626, script1626, EVENT_TEXT(0xD), NULL, NULL },
    { 1628, script1628, EVENT_TEXT(0xE), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
s16 script1620[] = {
    SCRIPT_WALK(2, 0x1F7, 0x214, 3),
    SCRIPT_PLACE(0x193, 0x1E0, 0x209),
    SCRIPT_POSE(0x193, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x193, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x193, 0x1D5, 0x202, 3),
    SCRIPT_WAIT_WALK(0x193),
    SCRIPT_POSE(0x193, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x193, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x193, 0x1F8, 0x1F0, 5),
    SCRIPT_WAIT_WALK(0x193),
    SCRIPT_POSE(0x193, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x193, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x193, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1622[] = {
    SCRIPT_WALK(2, 0x191, 0x1E1, 1),
    SCRIPT_PLACE(0x192, 0x177, 0x1ED),
    SCRIPT_POSE(0x192, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x192, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x192, 0x165, 0x1F5, 1),
    SCRIPT_WAIT_WALK(0x192),
    SCRIPT_POSE(0x192, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x192, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x192, 0x141, 0x1E5, 3),
    SCRIPT_WAIT_WALK(0x192),
    SCRIPT_POSE(0x192, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x192, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x192, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1624[] = {
    SCRIPT_WALK(2, 0x128, 0x1FC, 3),
    SCRIPT_PLACE(0x191, 0x110, 0x1F1),
    SCRIPT_POSE(0x191, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x191, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x191, 0xCC, 0x1CE, 3),
    SCRIPT_WAIT_WALK(0x191),
    SCRIPT_POSE(0x191, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x191, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x191, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1626[] = {
    SCRIPT_WALK(2, 0x140, 0x180, 3),
    SCRIPT_PLACE(0x190, 0x129, 0x175),
    SCRIPT_POSE(0x190, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x190, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x190, 0x155, 0x15D, 5),
    SCRIPT_WAIT_WALK(0x190),
    SCRIPT_POSE(0x190, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x190, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x190, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1628[] = {
    SCRIPT_WALK(2, 0xF0, 0x118, 3),
    SCRIPT_PLACE(0x18F, 0xD8, 0x10D),
    SCRIPT_POSE(0x18F, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x18F, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x18F, 0x104, 0xF7, 5),
    SCRIPT_WAIT_WALK(0x18F),
    SCRIPT_POSE(0x18F, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x18F, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x18F, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
