/*
 * WSTAG306: Master Room, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG305.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateSoundTileFrames[];

#include "common/step_animation_once.inc.c"

#include "common/update_sound_tile.inc.c"

/*
 * Command 851 of the event scripts: told 0x335 in event 930, opens the
 * secret door, with its sound
 */
void handleCommand851(StageSoundTile *task, s32 id) {
    if (task != NULL && id == 0x335) {
        task->voice = SOUND.playSound(SOUND_COMCD115);
        task->obj.anim.index = 0;
        task->obj.anim.timer = updateSoundTileFrames[0].duration;
        task->setSubstate(task, 1);
    }
}

#include "common/create_sound_tile.inc.c"

/* Creates the event object of progress 0x25 or 0x27, and the stage helper task before progress 0x27 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        switch (GAME.progress) {
        case 0x25:
            children[1] = FIELDSTG_startEvent(0x3A2);
            break;
        case 0x27:
            children[1] = FIELDSTG_startEvent(0x3D4);
            break;
        }
        if (GAME.progress < 0x27) {
            children[0] = createSoundTile(0x353);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x536
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x546
#endif
/*
 * Sets the stage up: its map, actors and events, playing KANRIBGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x13900, 0x11B00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script930[] = {
    SCRIPT_PLACE(1, 0x4F, 0x191),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x148, 0x115, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0xB4),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 0x29, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 0x2A, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(1, 0x29, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x353, 0x335, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x190, 0xF0, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x288, 0x58, 0x20C, 5),
    SCRIPT_END,
};
s16 script966[] = {
    SCRIPT_WALK(2, 0x188, 0xF5, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x288, 0x58, 0x20C, 5),
    SCRIPT_END,
};
s16 script980[] = {
    SCRIPT_PLACE(1, 0x4F, 0x190),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0x9D, 0x168, 0x115),
    SCRIPT_POSE(0x9D, 1, 5),
    SCRIPT_PLACE(0x9E, 0x148, 0x104),
    SCRIPT_POSE(0x9E, 1, 5),
    SCRIPT_PLACE(0x119, 0x14F, 0x128),
    SCRIPT_POSE(0x119, 1, 5),
    SCRIPT_PLACE(0x13A, 0x120, 0x110),
    SCRIPT_POSE(0x13A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0xD0, 0x150, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x9D, 1, 1),
    SCRIPT_POSE(0x9E, 1, 1),
    SCRIPT_POSE(0x119, 1, 1),
    SCRIPT_POSE(0x13A, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x117, 0x12C, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x136, 0x11B),
    SCRIPT_POSE(0x119, 1, 2),
    SCRIPT_POSE(0x13A, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 2, 0x13A, 0),
    SCRIPT_TALK(0, 1, 0x119, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 4, 0x9E, 0),
    SCRIPT_TALK(0, 3, 0x9D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x119, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 1, 0),
    SCRIPT_POSE(1, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WALK(1, 0x13F, 0x118, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_WALK(1, 0x171, 0xFF, 5),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x9D, 1, 5),
    SCRIPT_POSE(0x9E, 1, 5),
    SCRIPT_POSE(0x119, 1, 5),
    SCRIPT_POSE(0x13A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x13F, 0x118),
    SCRIPT_TALK(1, 0xA, 0x13A, 0),
    SCRIPT_TALK(0, 9, 0x119, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 0xC, 0x9E, 0),
    SCRIPT_TALK(0, 0xB, 0x9D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_POSE(1, 9, 1),
    SCRIPT_WAIT_ANIM(1),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x1A0, 0xE8, 5),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x288, 0x60, 0x208, 5),
    SCRIPT_END,
};
AnimFrame updateSoundTileFrames[] = {
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 8, 4 },
    { 9, 4 }, { 255, 0x3E7 },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x18C, 0xD8, 0x8C, 0x140, 0x1FB },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x19A, 0x100, 0x168, 0, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1A2, 0x100, 0x188, 0, 0x160, 0x1FB },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x180, 0x114, 0x100, 0x14, 0x150, 0x1FA },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x188, 0x116, 0x120, 0x16, 0x160, 0x1FA },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 6 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x1F9 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 4 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 5 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x27), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x3F, 5, 121, 260, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x9D, 6, 463, 264, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x9E, 7, 367, 312, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xC6, 8, 185, 229, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xC7, 9, 249, 197, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x119, 0xA, 428, 278, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x121, 0xB, 376, 188, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x122, 0xC, 360, 380, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x123, 0xD, 424, 348, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x124, 0xE, 488, 316, 3 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x13A, 0xF, 451, 290, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x3B, 6, 0, 374, 136, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x45, 8, 0, 408, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x46, 1, 0x46, 0x57, 8, 0, 418, 102, 0, 0 },
    { 0, 1, 0x40, 6, 5, 0, 5, 0xA, 4, 0, 382, 196, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 453, 185, 199, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 449, 190, 204, 0 },
    { 1, 0, 0x58, 4, 2, 0, 0, 0, 0, 0, 351, 136, 218, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 215, 302, 353, 0 },
    { 1, 0, 0x48, 4, 4, 0, 0, 0, 0, 0, 144, 262, 322, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x286, 0x398, 0xCC, 1, 0, 0, 0 },
    { { { SPECIAL(0x1A), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x3C6, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 930, script930, EVENT_TEXT(0xF), NULL, NULL },
    { 966, script966, EVENT_TEXT(0x15), NULL, NULL },
    { 980, script980, EVENT_TEXT(0x17), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
