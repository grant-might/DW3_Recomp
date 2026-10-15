/*
 * WSTAG230: Arena Front Desk, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define STAGE_FILE 0x193
#define STAGE_ARCHIVE 0x386
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define STAGE_FILE 0x1A1
#define STAGE_ARCHIVE 0x396
#endif
/*
 * Sets the stage up: its map and actors, playing KANRIBGM; BGM_0029 instead
 * at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0xDE00, 0xDE00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x14 && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x148, 0xD0, 0x48, 0x170, 0x1FF },
    { 0x140, 0x100, 0x15C, 0x161, 0x70, 0x61, 0x160, 0x1FE },
    { 0x140, 0x100, 0x164, 0x161, 0x90, 0x61, 0x170, 0x1FE },
    { 0x140, 0x100, 0x174, 0x170, 0xD0, 0x70, 0x150, 0x1FD },
    { 0x140, 0x100, 0x140, 0x175, 0, 0x75, 0x160, 0x1FD },
    { 0x140, 0x100, 0x148, 0x175, 0x20, 0x75, 0x170, 0x1FD },
    { 0x140, 0x100, 0x150, 0x175, 0x40, 0x75, 0x150, 0x1FC },
    { 0x140, 0x100, 0x16C, 0x181, 0xB0, 0x81, 0x160, 0x1FC },
    { 0x140, 0x100, 0x158, 0x189, 0x60, 0x89, 0x170, 0x1FC },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x1C9 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x1B2 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x1B3 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x151 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x1B4 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1B5 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x1B6 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x1B7 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x1B8 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x1B9 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1C6 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1C8 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x1CA },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x1D0 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x1CB },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x1CD },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x1DB },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x1EE },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x1EF },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x1CC },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x1CE },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x1CF },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x1F1 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x1F2 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x1F8 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x1F3 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x1F5 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x1F9 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x1FA },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x1FB },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x1F4 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x1F6 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x1F7 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x1FD },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x1FC },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x1F0 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x1C7 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor31Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor41Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x20, 4, 277, 344, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x20, 4, 272, 344, 7 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x24, 5, 240, 329, 3 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x24, 5, 240, 329, 3 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x24, 5, 240, 329, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x34, 6, 352, 239, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x34, 6, 237, 202, 3 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x39, 7, 237, 202, 3 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x39, 7, 240, 327, 3 };
FieldActorEntry actor37 = { actor37Conditions, NULL, 0x6A, 8, 0, 0, 1 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x9D, 9, 237, 202, 3 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x9E, 0xA, 352, 239, 1 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x9F, 0xB, 272, 344, 7 };
FieldActorEntry actor41 = { actor41Conditions, NULL, 0xA0, 0xC, 240, 329, 3 };
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
    { 1, 0, 0x40, 2, 3, 0, 0, 0, 0, 0, 192, 256, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 144, 104, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 168, 92, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 224, 64, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 127, 112, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 127, 144, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 159, 96, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 159, 128, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 191, 80, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 191, 112, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 223, 64, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 0xB, 8, 0, 223, 96, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 0xB, 8, 0, 196, 289, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 0xB, 8, 0, 226, 274, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 0xB, 8, 0, 212, 266, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 0xB, 8, 0, 212, 281, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 0xB, 8, 0, 196, 275, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 0xB, 8, 0, 226, 260, 0, 0 },
    { 1, 0, 0x40, 6, 0x37, 1, 0x37, 0x39, 0xA, 0, 200, 290, 0, 0 },
    { 1, 0, 0x40, 6, 2, 0, 0, 0, 0, 0, 256, 336, 0, 0 },
    { 1, 0, 0x40, 4, 0x3A, 1, 0x3A, 0x3C, 0xA, 0, 256, 286, 315, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 304, 312, 329, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 256, 280, 315, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x203, 0x200, 0x11C, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
