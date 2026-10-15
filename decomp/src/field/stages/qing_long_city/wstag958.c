/*
 * WSTAG958: Qing Long Tower, Qing Long City, in the extra chapter. The
 * European version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 670
 * starts it instead of WSTAG431.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 1646, with Guardromon: applies 0x7C0E, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1646(void) {
    FLAGS_00.applyAction(0x7C0E, 1);
}

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x92B
#define STAGE_FILE_8 0x221
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x907
#define STAGE_FILE_8 0x212
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 1;
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

/* Defined below, after the tables that use them */
extern s16 script1646[];

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x166, 0x16E, 0x98, 0x6E, 0x160, 0x1FD },
    { 0x140, 0x100, 0x172, 0x16E, 0xC8, 0x6E, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1A6, 0x100, 0x198, 0, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1AE, 0x100, 0x1B8, 0, 0x160, 0x1FC },
    { 0x140, 0x100, 0x176, 0x148, 0xD8, 0x48, 0x170, 0x1FC },
};
u16 actor0Talk0Actions[] = { START_EVENT(0x79), 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7C00, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x7B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x7D },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x7C },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x7E },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x15, 4, 408, 414, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x18, 5, 320, 515, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x30, 6, 171, 403, 7 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x32, 7, 513, 241, 5 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x39, 8, 352, 488, 3 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29C, 0x2F8, 0x104, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29C, 0x1E8, 0xA4, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1646, script1646, EVENT_TEXT(0xF), NULL, endEvent1646 },
    { -1, NULL, 0, NULL, NULL },
};
s16 script1646[] = {
    SCRIPT_WALK(2, 0x178, 0x1AC, 5),
    SCRIPT_PLACE(0x15, 0x198, 0x19E),
    SCRIPT_POSE(0x15, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x24),
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
