/*
 * WSTAG955: East Station, East Sector, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 667 starts it
 * instead of WSTAG411.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0x104)
#define STAGE_FILE 0x925
#define STAGE_FILE_8 0x39C
#elif VERSION_JP
#define STAGE_TEXT 0x89B
#define STAGE_FILE 0x8FE
#define STAGE_FILE_8 0x38C
#endif
/* Sets the stage up: its map and actors, playing BGM_0012 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
    FIELDSTG_state.start = (Vec2){0x26F00, 0xA800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xE;
    FIELDSTG_state.music = MUSIC(0xE, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.spriteColor = stageColor;
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
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x174, 0x130, 0xD0, 0x30, 0x160, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x14E, 0xA8, 0x4E, 0x170, 0x1FE },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 0x11), 0, FLAG(0, 0), 1, CODES_END };
u16 actor2Talk3Actions[] = { CARD_BATTLE(0x38, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x93 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x4D },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x4F },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x50 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x4D },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x4E },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x94 },
    { NULL, NULL, 0 },
};
u16 actor1Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor2Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x25, 4, 479, 320, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 5, 624, 152, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 5, 624, 152, 7 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x16D, 6, 528, 209, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 0, 0, 0, 0, 0, 0, 218, 322, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 6, 0, 429, 308, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 6, 0, 674, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 1, 0x34, 0x39, 4, 0, 776, 392, 0, 0 },
    { 1, 0, 0x80, 6, 1, 0, 0, 0, 0, 0, 219, 367, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x299, 0xD0, 0x2FC, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
