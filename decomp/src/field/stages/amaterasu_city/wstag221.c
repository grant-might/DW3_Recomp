/*
 * WSTAG221: Digimon Lab, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG220.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x4D0
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x4E0
#endif
/*
 * Sets the stage up: its map and actors, playing KANRIBGM; BGM_0029 instead
 * except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x10500, 0x15F00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
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
    { 0x140, 0x100, 0x158, 0x180, 0x60, 0x80, 0x150, 0x1F7 },
    { 0x140, 0x100, 0x160, 0x190, 0x80, 0x90, 0x160, 0x1F7 },
    { 0x140, 0x100, 0x168, 0x190, 0xA0, 0x90, 0x170, 0x1F7 },
    { 0x140, 0x100, 0x170, 0x190, 0xC0, 0x90, 0x150, 0x1F6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x154, 0x1A8, 0x50, 0xA8, 0x160, 0x1F6 },
    { 0x140, 0x100, 0x140, 0x1AE, 0, 0xAE, 0x170, 0x1F6 },
    { 0x140, 0x100, 0x148, 0x1AE, 0x20, 0xAE, 0x140, 0x1F5 },
    { 0x140, 0x100, 0x15C, 0x1B8, 0x70, 0xB8, 0x150, 0x1F5 },
    { 0x140, 0x100, 0x178, 0x190, 0xE0, 0x90, 0x160, 0x1F5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor15Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor16Talk0Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor16Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor16Talk1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor16Talk1Actions[] = { 0x7C00, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x51 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x56 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x52 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x57 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x59 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x4D },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x53 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x4E },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x54 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, actor15Talk0Actions, 0x55 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0x4F },
    { actor16Talk1Conditions, actor16Talk1Actions, 0x50 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x58 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x94), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x94), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 187, 215, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 187, 215, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x24, 5, 220, 232, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x24, 5, 220, 232, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x31, 6, 256, 385, 3 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x59, 7, 300, 217, 5 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x59, 7, 300, 217, 5 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x70, 8, 169, 224, 1 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x71, 9, 202, 241, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x9D, 0xA, 187, 215, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x9D, 0xA, 187, 215, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x9E, 0xB, 220, 232, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x9E, 0xB, 220, 232, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x9F, 0xC, 300, 217, 5 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x9F, 0xC, 300, 217, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0xA0, 0xD, 399, 248, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0xFE, 0xE, 399, 248, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0xFE, 0xE, 399, 248, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x10E, 0xF, 169, 224, 1 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x10E, 0xF, 169, 224, 1 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x10F, 0x10, 202, 241, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x10F, 0x10, 202, 241, 1 };
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
    { 1, 0, 0x40, 2, 0x45, 2, 0, 2, 0xA, 0, 376, 237, 0, 0 },
    { 1, 0, 0x40, 2, 5, 0, 0, 0, 0, 0, 368, 277, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 0, 204, 0, 0 },
    { 1, 0, 0x40, 2, 0x46, 2, 0, 1, 4, 0, 38, 197, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 110, 341, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 198, 149, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 221, 148, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 235, 149, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 249, 297, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 271, 168, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 398, 350, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 102, 333, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 200, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 213, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 242, 167, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 248, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 254, 280, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 265, 153, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 391, 343, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 1, 0x4A, 0x59, 0xA, 0, 124, 331, 0, 0 },
    { 1, 0, 0x40, 6, 0x14, 1, 0x14, 0x1E, 0xA, 0, 349, 336, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x39, 0xE, 0, 318, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x2F, 1, 0x2F, 0x31, 4, 0, 305, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3F, 8, 0, 354, 209, 0, 0 },
    { 1, 0, 0x40, 6, 0x40, 2, 0, 9, 8, 0, 336, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 2, 0, 2, 0x10, 0, 328, 232, 0, 0 },
    { 1, 0x64, 0x40, 6, 4, 0, 0, 0, 0, 0, 79, 186, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 2, 0, 2, 6, 0, 136, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x43, 2, 0, 2, 0x10, 0, 306, 231, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 2, 0, 2, 0xA, 0, 321, 223, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 1, 4, 0, 140, 123, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 1, 4, 0, 307, 122, 0, 0 },
    { 1, 0, 0x40, 6, 0x49, 2, 0, 1, 4, 0, 346, 144, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 143, 287, 340, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 326, 288, 340, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 398, 240, 263, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 33, 201, 239, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0x2C8, 0x24C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x410, 0x200, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
