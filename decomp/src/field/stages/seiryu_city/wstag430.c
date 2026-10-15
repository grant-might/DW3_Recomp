/*
 * WSTAG430: Seiryu Tower, Seiryu City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x213
#define STAGE_ARCHIVE 0x316
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x222
#define STAGE_ARCHIVE 0x325
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x18300, 0x1E600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 7;
    FIELDSTG_state.music = MUSIC(7, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1210[] = {
    SCRIPT_WALK(2, 0x178, 0x1AC, 5),
    SCRIPT_PLACE(0x15, 0x198, 0x19E),
    SCRIPT_POSE(0x15, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC02, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x166, 0x16E, 0x98, 0x6E, 0x160, 0x1FD },
    { 0x140, 0x100, 0x172, 0x16E, 0xC8, 0x6E, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1AA, 0x175, 0x1A8, 0x75, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1A6, 0x100, 0x198, 0, 0x160, 0x1FC },
    { 0x180, 0x100, 0x1AE, 0x100, 0x1B8, 0, 0x170, 0x1FC },
    { 0x140, 0x100, 0x176, 0x148, 0xD8, 0x48, 0x140, 0x1FB },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1B6, 0x120, 0x1D8, 0x20, 0x160, 0x1FB },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 9), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 9), 1, CODES_END };
u16 actor0Talk1Actions[] = { START_EVENT(0xC), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0x33), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0x33), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 0x33), 1, CODES_END };
u16 actor1Talk1Actions[] = { 0x7C00, 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1C, 0x11), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1C, 0xF), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1C, 0x11), 1, CODES_END };
u16 actor24Talk0Conditions[] = { FLAG(0x40, 0x11), 0, CODES_END };
u16 actor24Talk1Conditions[] = { FLAG(0x40, 0x11), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 3 },
    { actor0Talk1Conditions, actor0Talk1Actions, 2 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x122 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x4F },
    { actor2Talk1Conditions, NULL, 0x126 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x5C },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x176 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x5F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x62 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x6B },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x53 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x50 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x59 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x4D },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x5D },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x51 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x6C },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x66 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x63 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x60 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x54 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x57 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x5A },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x4E },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x61 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { actor24Talk0Conditions, NULL, 0x15D },
    { actor24Talk1Conditions, NULL, 0x52 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x6D },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x67 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x64 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x55 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x58 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x5B },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x5E },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x6A },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x69 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x68 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor31Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor33Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x15, 4, 408, 414, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2D, 6, 513, 241, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2D, 6, 171, 403, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x35, 7, 256, 511, 3 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x35, 7, 513, 241, 3 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x36, 8, 481, 225, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x36, 8, 481, 225, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x36, 8, 481, 225, 5 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x9D, 9, 481, 225, 5 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x9E, 0xA, 513, 241, 3 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x9F, 0xB, 171, 403, 5 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 0xA, 0, 283, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 0xA, 0, 428, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 0xA, 0, 307, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 1, 0xA, 0, 287, 459, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 1, 0xA, 0, 297, 464, 0, 0 },
    { 1, 0, 0x40, 6, 4, 1, 4, 9, 4, 0, 350, 338, 0, 0 },
    { 1, 0, 0x40, 6, 0xA, 1, 0xA, 0xD, 4, 0, 293, 279, 0, 0 },
    { 1, 0, 0x40, 6, 0xE, 1, 0xE, 0x11, 4, 0, 298, 381, 0, 0 },
    { 1, 0, 0x40, 6, 0x12, 1, 0x12, 0x15, 4, 0, 293, 427, 0, 0 },
    { 1, 0, 0x40, 4, 0x35, 2, 0, 1, 0xA, 0, 333, 483, 503, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 501, 393, 406, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 451, 436, 456, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 436, 464, 482, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 320, 474, 503, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 438, 274, 292, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 464, 277, 302, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22E, 0x2F8, 0x104, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22E, 0x1E8, 0xA4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1210, script1210, EVENT_TEXT(0x10), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
