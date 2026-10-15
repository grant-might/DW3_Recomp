/*
 * WSTAG441: South Station, South Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG440.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define STAGE_FILE 0x6ED
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define STAGE_FILE 0x6FD
#endif
/*
 * Sets the stage up: its map and actors, playing BGM_0012; BGM_0029 instead
 * except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x9100, 0x16C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xE;
    FIELDSTG_state.music = MUSIC(0xE, 0);
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
    { 0x140, 0x100, 0x168, 0x130, 0xA0, 0x30, 0x170, 0x1FE },
    { 0x140, 0x100, 0x140, 0x14A, 0, 0x4A, 0x160, 0x1FD },
    { 0x140, 0x100, 0x150, 0x14A, 0x40, 0x4A, 0x170, 0x1FD },
    { 0x140, 0x100, 0x158, 0x150, 0x60, 0x50, 0x160, 0x1FC },
    { 0x140, 0x100, 0x160, 0x150, 0x80, 0x50, 0x170, 0x1FC },
    { 0x140, 0x100, 0x170, 0x150, 0xC0, 0x50, 0x160, 0x1FB },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0xA2 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xA5 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x9E },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xA0 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xA3 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x31, 4, 145, 329, 5 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x32, 5, 177, 313, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x39, 6, 177, 313, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x3A, 7, 145, 329, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9D, 8, 177, 313, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x9D, 8, 177, 313, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x9E, 9, 145, 329, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9E, 9, 145, 329, 5 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 9, 0, 435, 247, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 1, 0x33, 0x38, 9, 0, 443, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 1, 0x3A, 0x3F, 6, 0, 377, 384, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 482, 177, 200, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2A1, 0x208, 0x9C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
