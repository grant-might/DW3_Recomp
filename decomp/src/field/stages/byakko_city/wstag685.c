/*
 * WSTAG685: Underground Cave, Byakko City.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of story progress 0xF */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 0xF) {
            children[0] = FIELDSTG_startEvent(0x19D);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/* After event 413, with the player: moves the story on to its step 0x10 */
void endEvent413(void) {
    GAME.progress = 0x10;
}

/* After event 421, with Numemon: sets flag 0x4020 */
void endEvent421(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x20), 1);
}

/* After event 690, with General: moves the story on to its step 0x19 */
void endEvent690(void) {
    GAME.progress = 0x19;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x4B4
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x4C4
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0020 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x23700, 0x1AF00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3C;
    FIELDSTG_state.music = MUSIC(0x3C, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script413[] = {
    SCRIPT_PLACE(1, 0x330, 0x108),
    SCRIPT_POSE(1, 0x10, 7),
    SCRIPT_PLACE(0x11F, 0x340, 0x10D),
    SCRIPT_POSE(0x11F, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_PIYOPIYO, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 0x27, 7),
    SCRIPT_WAIT_ANIM(1),
    SCRIPT_POSE(1, 0x26, 7),
    SCRIPT_PLACE(0x11F, 0, 0),
    SCRIPT_POSE(0x11F, 1, 0),
    SCRIPT_WAIT_ANIM(1),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x3A, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 1, 1, 2),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_LEAVE(0x260, 0x330, 0x108, 1),
    SCRIPT_END,
};
s16 script421[] = {
    SCRIPT_WALK(2, 0x3D5, 0x147, 5),
    SCRIPT_PLACE(0x69, 0x415, 0x127),
    SCRIPT_POSE(0x69, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x69),
    SCRIPT_TALK(0, 2, 0x69, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x69, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x69, 0x3F5, 0x137, 1),
    SCRIPT_WAIT_WALK(0x69),
    SCRIPT_TALK(0, 4, 0x69, 0),
    SCRIPT_POSE(0x69, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x69, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x69, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x41E, 0x122, 5),
    SCRIPT_WALK(0x69, 0x41E, 0x122, 5),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x24C, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script690[] = {
    SCRIPT_WALK(2, 0x248, 0x1B4, 1),
    SCRIPT_PLACE(0x98, 0x230, 0x1C1),
    SCRIPT_POSE(0x98, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 0x98),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x98),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x98, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(2, 0x248, 0x164, 4),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(2, 0x1C8, 0x106, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x25E, 0x290, 0x1F0, 1),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x1A6, 0, 0xA6, 0x150, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x158, 0xB8, 0x58, 0x160, 0x1FF },
    { 0x140, 0x100, 0x176, 0x158, 0xD8, 0x58, 0x170, 0x1FF },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x164, 0x180, 0x90, 0x80, 0x150, 0x1FE },
    { 0x140, 0x100, 0x16C, 0x188, 0xB0, 0x88, 0x160, 0x1FE },
    { 0x140, 0x100, 0x15E, 0x1A8, 0x78, 0xA8, 0x170, 0x1FE },
    { 0x140, 0x100, 0x174, 0x188, 0xD0, 0x88, 0x140, 0x1FD },
    { 0x140, 0x100, 0x166, 0x1B0, 0x98, 0xB0, 0x150, 0x1FD },
    { 0x140, 0x100, 0x16E, 0x1B0, 0xB8, 0xB0, 0x160, 0x1FD },
    { 0x140, 0x100, 0x176, 0x1B0, 0xD8, 0xB0, 0x170, 0x1FD },
    { 0x140, 0x100, 0x148, 0x1B5, 0x20, 0xB5, 0x140, 0x1FC },
    { 0x140, 0x100, 0x150, 0x1BD, 0x40, 0xBD, 0x150, 0x1FC },
    { 0x140, 0x100, 0x168, 0x1D0, 0xA0, 0xD0, 0x160, 0x1FC },
    { 0x140, 0x100, 0x14E, 0x18D, 0x38, 0x8D, 0x170, 0x1FC },
    { 0x140, 0x100, 0x156, 0x195, 0x58, 0x95, 0x140, 0x1FB },
    { 0x140, 0x100, 0x140, 0x1C6, 0, 0xC6, 0x150, 0x1FB },
    { 0x140, 0x100, 0x158, 0x1C8, 0x60, 0xC8, 0x160, 0x1FB },
    { 0x140, 0x100, 0x160, 0x1D0, 0x80, 0xD0, 0x170, 0x1FB },
};
u16 actor5Talk0Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor5Talk1Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor6Talk0Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor6Talk1Conditions[] = { PROGRESS(0x18), 0, SPECIAL(0x18), 1, CODES_END };
u16 actor9Talk0Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor9Talk1Conditions[] = { SPECIAL(0x18), 1, PROGRESS(0x18), 0, CODES_END };
u16 actor13Talk0Conditions[] = { PROGRESS(0x18), 0, CODES_END };
u16 actor13Talk1Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor13Talk1Actions[] = { START_EVENT(0x43), 1, CODES_END };
u16 actor22Talk0Actions[] = { FLAG(0x1A, 3), 1, CODES_END };
u16 actor23Talk0Actions[] = { FLAG(0x1A, 3), 1, CODES_END };
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x13A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x13C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x13B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x13D },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x140 },
    { actor5Talk1Conditions, NULL, 0x12A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x172 },
    { actor6Talk1Conditions, NULL, 0x12B },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x12E },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x131 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x173 },
    { actor9Talk1Conditions, NULL, 0x12C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x12F },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x132 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x12D },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x171 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x130 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x133 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x136 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x134 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x135 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x13E },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x13F },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, actor22Talk0Actions, 0x129 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, actor23Talk0Actions, 0x15C },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x137 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x138 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x139 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(4), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(4), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x25, 5, 386, 129, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x25, 5, 386, 129, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x26, 6, 415, 145, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x26, 6, 415, 145, 7 };
FieldActorEntry actor5 = { NULL, actor5Talks, 0x3F, 7, 1040, 298, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x45, 8, 561, 305, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x45, 8, 561, 305, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x45, 8, 561, 305, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x46, 9, 993, 321, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x46, 9, 993, 321, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x46, 9, 993, 321, 5 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x69, 0xA, 1045, 295, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x98, 0xB, 560, 449, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x98, 0xB, 560, 449, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x98, 0xB, 560, 449, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9D, 0xC, 560, 449, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9E, 0xD, 561, 305, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9F, 0xE, 993, 321, 5 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0xA0, 0xF, 386, 129, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0xA1, 0x10, 415, 145, 7 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x11F, 0x11, 0, 0, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x132, 0x12, 328, 142, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x133, 0x13, 354, 128, 7 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x16D, 0x14, 825, 269, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x16F, 0x15, 560, 449, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x171, 0x16, 561, 305, 1 };
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25E, 0x290, 0x1F0, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 9, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 8, 0x1C4, 0xA0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 8, 0x1D5, 0x12C, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1BF, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1AF, 0x198, 0, 0, 0, 0 },
    { { { FLAG(0x1A, 3), 1 }, { PROGRESS(0x10), 1 } }, SLOT_EVENT, 0x1A5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 413, script413, EVENT_TEXT(3), NULL, endEvent413 },
    { 421, script421, EVENT_TEXT(4), NULL, endEvent421 },
    { 690, script690, EVENT_TEXT(0x1B), NULL, endEvent690 },
    { -1, NULL, 0, NULL, NULL },
};
