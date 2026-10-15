/*
 * WSTAG431: Qing Long Tower, Qing Long City. On the Amaterasu server; its
 * Asuka server twin is WSTAG430.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x58E
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x59E
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x19100, 0x1E700};
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

s16 script1211[] = {
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
    SCRIPT_LEAVE(0xC0C, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x150, 0x100, 0x40, 0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x15C, 0x100, 0x70, 0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x148, 0x126, 0x20, 0x26, 0x150, 0x1FC },
    { 0x140, 0x100, 0x15C, 0x128, 0x70, 0x28, 0x160, 0x1FC },
    { 0x140, 0x100, 0x176, 0x100, 0xD8, 0, 0x170, 0x1FC },
    { 0x140, 0x100, 0x140, 0x126, 0, 0x26, 0x140, 0x1FB },
    { 0x140, 0x100, 0x140, 0x156, 0, 0x56, 0x150, 0x1FB },
    { 0x140, 0x100, 0x150, 0x130, 0x40, 0x30, 0x160, 0x1FB },
    { 0x140, 0x100, 0x172, 0x130, 0xC8, 0x30, 0x170, 0x1FB },
    { 0x140, 0x100, 0x164, 0x142, 0x90, 0x42, 0x140, 0x1FA },
    { 0x140, 0x100, 0x16C, 0x142, 0xB0, 0x42, 0x150, 0x1FA },
    { 0x140, 0x100, 0x148, 0x14E, 0x20, 0x4E, 0x160, 0x1FA },
    { 0x140, 0x100, 0x158, 0x150, 0x60, 0x50, 0x170, 0x1FA },
    { 0x140, 0x100, 0x150, 0x158, 0x40, 0x58, 0x140, 0x1F9 },
    { 0x140, 0x100, 0x172, 0x158, 0xC8, 0x58, 0x150, 0x1F9 },
    { 0x140, 0x100, 0x160, 0x16A, 0x80, 0x6A, 0x160, 0x1F9 },
    { 0x140, 0x100, 0x168, 0x172, 0xA0, 0x72, 0x170, 0x1F9 },
    { 0x140, 0x100, 0x140, 0x176, 0, 0x76, 0x140, 0x1F8 },
    { 0x140, 0x100, 0x148, 0x176, 0x20, 0x76, 0x150, 0x1F8 },
};
u16 actor0Talk0Actions[] = { START_EVENT(0xD), 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor3Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor4Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor5Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor15Talk0Actions[] = { FLAG(0x1A, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x1A2 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xF7 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xFD },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xF3 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xF5 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xF0 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x100 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xFB },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, actor15Talk0Actions, 0xFA },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x1D2 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x1D3 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0xEF },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0xF4 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0xF6 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xF1 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xFC },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0xF8 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0xFE },
    { NULL, NULL, 0 },
};
u16 actor1Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor3Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x15, 4, 408, 414, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x18, 5, 352, 488, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x20, 6, 481, 225, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x24, 7, 500, 294, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x25, 8, 171, 403, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x26, 9, 257, 272, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 0xA, 257, 513, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 0xA, 171, 403, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x31, 0xB, 500, 294, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x33, 0xC, 401, 233, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x37, 0xD, 481, 225, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x66, 0xE, 320, 515, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x73, 0xF, 171, 403, 5 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x74, 0x10, 500, 294, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9D, 0x11, 513, 241, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x9D, 0x11, 171, 403, 5 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9E, 0x12, 481, 225, 5 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9E, 0x12, 257, 272, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x9F, 0x13, 257, 513, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0xA0, 0x14, 481, 225, 5 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0xA1, 0x15, 401, 233, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0xA2, 0x16, 500, 294, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 0xA, 0, 283, 466, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 0xA, 0, 428, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 1, 0xA, 0, 307, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 1, 0xA, 0, 287, 459, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 2, 0, 1, 0xA, 0, 297, 464, 0, 0 },
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29C, 0x2F8, 0x104, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29C, 0x1E8, 0xA4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1211, script1211, EVENT_TEXT(0x11), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
