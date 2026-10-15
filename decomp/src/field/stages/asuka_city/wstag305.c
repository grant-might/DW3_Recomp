/*
 * WSTAG305: Master Room, Asuka City.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame updateSoundTileFrames[];

#include "common/step_animation_once.inc.c"

#include "common/update_sound_tile.inc.c"

/* Plays the sound and starts the animation when map object 0x335 is triggered */
void handleCommand850(StageSoundTile *task, s32 id) {
    if (task != NULL && id == 0x335) {
        task->voice = SOUND.playSound(SOUND_COMCD115);
        task->obj.anim.index = 0;
        task->obj.anim.timer = updateSoundTileFrames[0].duration;
        task->setSubstate(task, 1);
    }
}

#include "common/create_sound_tile.inc.c"

/* Creates the event objects of the story so far, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        do {
            if (GAME.progress == 0xD && FLAGS_00.checkCondition(FLAG(0x1C, 0xD), 0)) {
                children[0] = FIELDSTG_startEvent(0x15E);
                break;
            }
            if (GAME.progress == 0xD && FLAGS_00.checkCondition(FLAG(0x1C, 0xD), 1)) {
                children[0] = FIELDSTG_startEvent(0x15F);
                break;
            }
            if (GAME.progress == 0x16 && FLAGS_00.checkCondition(FLAG(0x1C, 0x36), 1)) {
                children[0] = FIELDSTG_startEvent(0x294);
                break;
            }
            if (GAME.progress == 0x16 && FLAGS_00.checkCondition(FLAG(0x1C, 0x37), 1)) {
                children[0] = FIELDSTG_startEvent(0x295);
                break;
            }
            if (GAME.progress == 0x17 && FLAGS_00.checkCondition(FLAG(0x40, 0x50), 1)) {
                children[0] = FIELDSTG_startEvent(0x2AB);
                break;
            }
            if (GAME.progress == 0x21 && FLAGS_00.checkCondition(FLAG(0x1C, 0x39), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x57), 0)) {
                children[0] = FIELDSTG_startEvent(0x37A);
                break;
            }
            if (GAME.progress == 0x21 && FLAGS_00.checkCondition(FLAG(0x40, 0x59), 1) && FLAGS_00.checkCondition(FLAG(0x1C, 0x39), 0) &&
                FLAGS_00.checkCondition(FLAG(0x1A, 0x34), 1)) {
                children[0] = FIELDSTG_startEvent(0x37D);
                break;
            }
            if (GAME.progress == 0x21 && FLAGS_00.checkCondition(FLAG(0x40, 0x59), 1) && FLAGS_00.checkCondition(FLAG(0x1C, 0x39), 0) &&
                FLAGS_00.checkCondition(FLAG(0x1A, 0x34), 0)) {
                children[0] = FIELDSTG_startEvent(0x37E);
                break;
            }
            if (GAME.progress == 0x26 && FLAGS_00.checkCondition(FLAG(0x40, 0x69), 0)) {
                children[0] = FIELDSTG_startEvent(0x3C0);
                break;
            }
        } while (0);
        if (GAME.progress < 0x26) {
            children[1] = createSoundTile(0x352);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0x8
#include "common/start_stage.inc.c"

/* After event 350, with Kail, Game Master and DO Guard: keeps it from playing again */
void endEvent350(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0xD), 1);
}

/* After event 351, with Game Master and Kail: sets flag 0x1C0E */
void endEvent351(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0xE), 1);
}

/* After event 660, with the player: clears flag 0x1C36 */
void endEvent660(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x36), 0);
}

/* After event 661, with the player: moves the story on to its step 0x17 */
void endEvent661(void) {
    GAME.progress = 0x17;
}

/* After event 890, with Lisa, Teddy, Keith and Nick: keeps it from playing again */
void endEvent890(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x57), 1);
}

/* After event 893, with Lisa, Nick, Keith and Teddy: moves the story on to its step 0x22 */
void endEvent893(void) {
    GAME.progress = 0x22;
}

/* After event 894, with Lisa, Nick and Keith: keeps it from playing again */
void endEvent894(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x39), 1);
}

/* After event 895, with Lisa, Keith, Nick and Teddy: moves the story on to its step 0x22 */
void endEvent895(void) {
    GAME.progress = 0x22;
}

/* After event 960, with Oinkmon: keeps it from playing again */
void endEvent960(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x69), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x33C
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x34B
#endif
/* Sets the stage up: its map, actors and events, playing KANRIBGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12C00, 0x12C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script350[] = {
    SCRIPT_FOLLOW(1, 0x26),
    SCRIPT_PLACE(0x25, 0x13E, 0x102),
    SCRIPT_POSE(0x25, 1, 5),
    SCRIPT_PLACE(0x26, 0xCC, 0x139),
    SCRIPT_POSE(0x26, 1, 5),
    SCRIPT_PLACE(0x27, 0x100, 0x152),
    SCRIPT_POSE(0x27, 1, 5),
    SCRIPT_PLACE(0x45, 0x15E, 0x114),
    SCRIPT_POSE(0x45, 1, 5),
    SCRIPT_PLACE(0x46, 0x130, 0x116),
    SCRIPT_POSE(0x46, 1, 5),
    SCRIPT_PLACE(0x6A, 0x58, 0x18C),
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_PLACE(0x6C, 0x1AF, 0xB0),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_PLACE(0x6D, 0x19F, 0x9E),
    SCRIPT_POSE(0x6D, 1, 1),
    SCRIPT_PLACE(0x6E, 0x1CE, 0xB3),
    SCRIPT_POSE(0x6E, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0x78, 0x17C, 5),
    SCRIPT_PLACE(0x6B, 0x58, 0x18C),
    SCRIPT_POSE(0x6B, 1, 5),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_WALK(0x6A, 0x98, 0x16B, 5),
    SCRIPT_WALK(0x6B, 0x75, 0x17D, 5),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_POSE(0x6B, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x6B),
    SCRIPT_COMMAND(0x324, 0x325, 0x6A),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6B),
    SCRIPT_COMMAND(0x324, 0x326, 0x6A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x6B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0x6A, 0xBF, 0x180, 5),
    SCRIPT_WALK(0x6B, 0x98, 0x16B, 7),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_WALK(0x6A, 0xD9, 0x172, 3),
    SCRIPT_WALK(0x6B, 0xBF, 0x180, 5),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_POSE(0x6B, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x160, 0xDF),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 2, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x45, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x25, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0xB0, 0x160),
    SCRIPT_POSE(0x6A, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x6A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x6F, 0x58, 0x18C),
    SCRIPT_POSE(0x6F, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6F, 0x80, 0x178, 5),
    SCRIPT_WAIT_WALK(0x6F),
    SCRIPT_POSE(0x6F, 1, 5),
    SCRIPT_COMMAND(0x323, 0x325, 0x6F),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6F),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x6F, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x6F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0x26),
    SCRIPT_COMMAND(0x324, 0x325, 0x27),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x26),
    SCRIPT_COMMAND(0x324, 0x326, 0x27),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x26, 0xE3, 0x147, 1),
    SCRIPT_POSE(0x27, 1, 3),
    SCRIPT_POSE(0x45, 1, 1),
    SCRIPT_POSE(0x46, 1, 1),
    SCRIPT_POSE(0x6D, 1, 1),
    SCRIPT_WAIT_WALK(0x26),
    SCRIPT_WALK(0x26, 0xD0, 0x151, 1),
    SCRIPT_WALK(0x27, 0xE3, 0x147, 1),
    SCRIPT_WAIT_WALK(0x27),
    SCRIPT_WALK(0x26, 0xB0, 0x160, 7),
    SCRIPT_WALK(0x27, 0xD0, 0x151, 0),
    SCRIPT_WAIT_WALK(0x26),
    SCRIPT_TALK(1, 8, 0x27, 2),
    SCRIPT_TALK(0, 7, 0x26, 1),
    SCRIPT_POSE(0x26, 1, 7),
    SCRIPT_POSE(0x27, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0x6A),
    SCRIPT_COMMAND(0x324, 0x325, 0x6B),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6A),
    SCRIPT_COMMAND(0x324, 0x326, 0x6B),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_POSE(0x6B, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x6A, 1, 4),
    SCRIPT_POSE(0x6B, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x6A, 1, 1),
    SCRIPT_POSE(0x6B, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x6A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x218, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script351[] = {
    SCRIPT_LOOK_AT(1, 0x160, 0xDF),
    SCRIPT_PLACE(1, 0x13E, 0x102),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_PLACE(0xB, 0x15E, 0x114),
    SCRIPT_POSE(0xB, 1, 5),
    SCRIPT_PLACE(0x25, 0x130, 0x11B),
    SCRIPT_POSE(0x25, 1, 5),
    SCRIPT_PLACE(0x26, 0xCC, 0x139),
    SCRIPT_POSE(0x26, 1, 5),
    SCRIPT_PLACE(0x27, 0x100, 0x152),
    SCRIPT_POSE(0x27, 1, 5),
    SCRIPT_PLACE(0x6C, 0x1AF, 0xB0),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_PLACE(0x6D, 0x19F, 0x9E),
    SCRIPT_POSE(0x6D, 1, 1),
    SCRIPT_PLACE(0x6E, 0x1CE, 0xB3),
    SCRIPT_POSE(0x6E, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 1, 0),
    SCRIPT_POSE(1, 0xC, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0xB, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 1, 0),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x204, 0x179, 0xAB, 1),
    SCRIPT_END,
};
s16 script660[] = {
    SCRIPT_PLACE(2, 0x138, 0x11D),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x17F, 0xF9),
    SCRIPT_WAIT(0x96),
    SCRIPT_WAIT(0x3C),
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x218, 0x138, 0x11D, 1),
    SCRIPT_END,
};
s16 script661[] = {
    SCRIPT_PLACE(2, 0x138, 0x11D),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x17F, 0xF9),
    SCRIPT_WAIT(0x96),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(0x352, 0x335, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x17F, 0xF9, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x219, 0x58, 0x20C, 5),
    SCRIPT_END,
};
s16 script683[] = {
    SCRIPT_PLACE(1, 0x16E, 0xE8),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_PLACE(0xB, 0x188, 0xF4),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_PLACE(0xC, 0x1BA, 0x92),
    SCRIPT_POSE(0xC, 1, 3),
    SCRIPT_PLACE(0x65, 0x138, 0x12C),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_PLACE(0x67, 0x118, 0x11C),
    SCRIPT_POSE(0x67, 1, 5),
    SCRIPT_PLACE(0x6C, 0x150, 0x110),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_PLACE(0x13D, 0x1A0, 0x100),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LOOK_AT(0, 0x150, 0x114),
    SCRIPT_TALK(0, 1, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0x65),
    SCRIPT_COMMAND(0x326, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0x65),
    SCRIPT_COMMAND(0x326, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x17A, 0xBC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x325, 0xC),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xC, 0x34, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0x19A, 0xA2, 1),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xC, 3),
    SCRIPT_POSE(0xC, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 0x35, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x150, 0x114),
    SCRIPT_TALK(0, 4, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6C, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x19, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1A, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 1, 0),
    SCRIPT_POSE(1, 0xC, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0x6C, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x150, 0x114),
    SCRIPT_TALK(0, 7, 0x6C, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1B, 0x6C, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x67, 2),
    SCRIPT_POSE(0x67, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x65, 3),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1C, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_TALK(0, 0xA, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_FOLLOW(0, 0x13D),
    SCRIPT_TALK(0, 0x18, 0x13D, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x150, 0x114),
    SCRIPT_TALK(0, 0xB, 0x65, 3),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0x65, 3),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x6C, 0),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_POSE(0x6C, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_TALK(0, 0x10, 1, 0),
    SCRIPT_POSE(1, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x65),
    SCRIPT_TALK(0, 0x12, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x65, 0x150, 0x138, 7),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_POSE(0x65, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1D, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_LOOK_AT(0, 0x150, 0x114),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x67, 0x128, 0x124, 7),
    SCRIPT_WAIT_WALK(0x67),
    SCRIPT_POSE(0x67, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 0x67, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WALK(0x6C, 0x140, 0x118, 1),
    SCRIPT_WAIT_WALK(0x6C),
    SCRIPT_WALK(0x65, 0x128, 0x124, 1),
    SCRIPT_WALK(0x67, 0xF8, 0x13C, 1),
    SCRIPT_WALK(0x6C, 0x110, 0x130, 1),
    SCRIPT_WAIT_WALK(0x6C),
    SCRIPT_WALK(0x65, 0x80, 0x178, 1),
    SCRIPT_WALK(0x67, 0x80, 0x178, 1),
    SCRIPT_WALK(0x6C, 0x80, 0x178, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_LOOK_AT(0, 0x17E, 0xE4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x14E, 0xF8, 1),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_TALK(0, 0x14, 0xC, 3),
    SCRIPT_POSE(0xC, 0x3B, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 1, 3),
    SCRIPT_POSE(1, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xC, 9, 1),
    SCRIPT_WAIT_ANIM(0xC),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x16, 0xB, 3),
    SCRIPT_POSE(0xB, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xB, 0x188, 0x110, 0),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_POSE(0xC, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1E, 0xB, 2),
    SCRIPT_POSE(0xB, 0x3B, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x2D7, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script890[] = {
    SCRIPT_FOLLOW(0, 0x65),
    SCRIPT_PLACE(2, 0x90, 0x170),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(2, 0x98, 0x16C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x65, 0x14F, 0x111),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_PLACE(0x66, 0x138, 0x12C),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_PLACE(0x67, 0x170, 0x138),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_PLACE(0xB3, 0x1A0, 0xA0),
    SCRIPT_POSE(0xB3, 1, 3),
    SCRIPT_PLACE(0x17D, 0x16F, 0xE9),
    SCRIPT_POSE(0x17D, 1, 1),
    SCRIPT_PLACE(0x17E, 0x187, 0xF5),
    SCRIPT_POSE(0x17E, 1, 1),
    SCRIPT_PLACE(0x17F, 0x19F, 0x101),
    SCRIPT_POSE(0x17F, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xE0, 0x148, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x110, 0x130, 5),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_COMMAND(0x324, 0x325, 0x66),
    SCRIPT_COMMAND(0x326, 0x325, 0x67),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x128, 0x124, 5),
    SCRIPT_WALK(0x66, 0x148, 0x134, 7),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_COMMAND(0x324, 0x326, 0x66),
    SCRIPT_COMMAND(0x326, 0x326, 0x67),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x65, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 0xB3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xB3, 1),
    SCRIPT_POSE(0x66, 1, 4),
    SCRIPT_POSE(0x67, 1, 4),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 4, 0x66, 1),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_POSE(0xB3, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x67, 1),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x66, 1, 6),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x65, 0),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 4),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 0xC, 0x67, 2),
    SCRIPT_TALK(0, 8, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x65, 0),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x65, 0),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x108, 0x114, 7),
    SCRIPT_WALK(0x65, 0x128, 0x124, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(2, 0x128, 0x124, 1),
    SCRIPT_WALK(0x65, 0x108, 0x134, 1),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(2, 0xA0, 0x168, 1),
    SCRIPT_WALK(0x65, 0xA0, 0x168, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x216, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script893[] = {
    SCRIPT_FOLLOW(1, 0xB3),
    SCRIPT_PLACE(2, 0x118, 0x11C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x65, 0x14F, 0x111),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_PLACE(0x66, 0x136, 0x12C),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_PLACE(0x67, 0x170, 0x138),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_PLACE(0xB3, 0x1A0, 0xA0),
    SCRIPT_POSE(0xB3, 1, 3),
    SCRIPT_PLACE(0x17D, 0x16F, 0xE9),
    SCRIPT_POSE(0x17D, 1, 1),
    SCRIPT_PLACE(0x17E, 0x187, 0xF5),
    SCRIPT_POSE(0x17E, 1, 1),
    SCRIPT_PLACE(0x17F, 0x19F, 0x101),
    SCRIPT_POSE(0x17F, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x136, 0x12C),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 1, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x67, 1),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x66, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_POSE(2, 7, 6),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x65, 2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_COMMAND(0x324, 0x325, 0x66),
    SCRIPT_COMMAND(0x325, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_COMMAND(0x324, 0x326, 0x66),
    SCRIPT_COMMAND(0x325, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x67, 1),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 2, 0),
    SCRIPT_POSE(2, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WALK(0x66, 0x136, 0x143, 1),
    SCRIPT_COMMAND(0x323, 0x327, 0x65),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x65, 2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 4),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x16, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x17, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x67),
    SCRIPT_TALK(1, 0x11, 0x66, 1),
    SCRIPT_TALK(0, 0x12, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x164, 0xF0),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WALK(0x65, 0xF0, 0x140, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0x66, 0x110, 0x130, 3),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0x65, 0x60, 0x18A, 1),
    SCRIPT_WALK(0x66, 0x60, 0x18A, 1),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 0x13, 2, 2),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0xB3, 1),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 2, 2),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x120, 0x120, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0xB0, 0x160, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0x217, 0x398, 0xCC, 1),
    SCRIPT_END,
};
s16 script894[] = {
    SCRIPT_FOLLOW(1, 0xB3),
    SCRIPT_PLACE(2, 0x118, 0x11C),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_PLACE(0x65, 0x14F, 0x111),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_PLACE(0x66, 0x136, 0x12C),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_PLACE(0x67, 0x170, 0x138),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_PLACE(0xB3, 0x1A0, 0xA0),
    SCRIPT_POSE(0xB3, 1, 3),
    SCRIPT_PLACE(0x17D, 0x16F, 0xE9),
    SCRIPT_POSE(0x17D, 1, 1),
    SCRIPT_PLACE(0x17E, 0x187, 0xF5),
    SCRIPT_POSE(0x17E, 1, 1),
    SCRIPT_PLACE(0x17F, 0x19F, 0x101),
    SCRIPT_POSE(0x17F, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x136, 0x12C),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 1, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x67, 1),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x66, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x66, 3),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x66, 3),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x100, 0x128, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_POSE(0x66, 1, 2),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 2),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_COMMAND(0x324, 0x325, 0x66),
    SCRIPT_COMMAND(0x325, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_COMMAND(0x324, 0x326, 0x66),
    SCRIPT_COMMAND(0x325, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x110, 0x130, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0xB0, 0x160, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0x217, 0x398, 0xCC, 1),
    SCRIPT_END,
};
s16 script895[] = {
    SCRIPT_LOOK_AT(0, 0x136, 0x12C),
    SCRIPT_WALK(2, 0x118, 0x11C, 5),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_POSE(0x17D, 1, 1),
    SCRIPT_POSE(0x17E, 1, 1),
    SCRIPT_POSE(0x17F, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x65, 2),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 6),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x65, 2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_COMMAND(0x324, 0x325, 0x66),
    SCRIPT_COMMAND(0x325, 0x325, 0x67),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_COMMAND(0x324, 0x326, 0x66),
    SCRIPT_COMMAND(0x325, 0x326, 0x67),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x67, 1),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x66, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 2, 0),
    SCRIPT_POSE(2, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WALK(0x66, 0x136, 0x143, 1),
    SCRIPT_COMMAND(0x323, 0x327, 0x65),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0x65, 2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0x66, 1, 4),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x17, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x18, 0x65, 2),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x67),
    SCRIPT_TALK(1, 0x12, 0x66, 1),
    SCRIPT_TALK(0, 0x13, 0x67, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x164, 0xF0),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_WALK(0x65, 0xF0, 0x140, 1),
    SCRIPT_POSE(0x66, 1, 3),
    SCRIPT_POSE(0x67, 1, 2),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0x66, 0x110, 0x130, 3),
    SCRIPT_WAIT_WALK(0x66),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0x65, 0x60, 0x18A, 1),
    SCRIPT_WALK(0x66, 0x60, 0x18A, 1),
    SCRIPT_POSE(0x67, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 0x14, 2, 2),
    SCRIPT_POSE(2, 1, 6),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0xB3, 1),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x16, 2, 2),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x120, 0x120, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0xB0, 0x160, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0x217, 0x398, 0xCC, 1),
    SCRIPT_END,
};
s16 script960[] = {
    SCRIPT_PLACE(2, 0x4F, 0x191),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xD0, 0x150, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, 1, 0),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(1, 0x1B0, 0x121),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 4, 0x23, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 5, 0x119, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 6, 0x13A, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script965[] = {
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
    SCRIPT_LEAVE(0x219, 0x58, 0x20C, 5),
    SCRIPT_END,
};
s16 script1450[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x218, 0x138, 0x11D, 5),
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
    { 0x180, 0x100, 0x1B4, 0x13C, 0x1D0, 0x3C, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1A8, 0x152, 0x1A0, 0x52, 0x160, 0x1FB },
    { 0x180, 0x100, 0x180, 0x158, 0x100, 0x58, 0x170, 0x1FB },
    { 0x140, 0x100, 0x176, 0x18C, 0xD8, 0x8C, 0x140, 0x1FA },
    { 0x180, 0x100, 0x180, 0x100, 0x100, 0, 0x150, 0x1FA },
    { 0x180, 0x100, 0x188, 0x100, 0x120, 0, 0x160, 0x1FA },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x1B0, 0x114, 0x1C0, 0x14, 0x170, 0x1FA },
    { 0x180, 0x100, 0x180, 0x130, 0x100, 0x30, 0x140, 0x1F9 },
    { 0x180, 0x100, 0x188, 0x130, 0x120, 0x30, 0x150, 0x1F9 },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x160, 0x1F9 },
    { 0x180, 0x100, 0x190, 0x130, 0x140, 0x30, 0x170, 0x1F9 },
    { 0x180, 0x100, 0x188, 0x158, 0x120, 0x58, 0x140, 0x1F8 },
    { 0x180, 0x100, 0x190, 0x158, 0x140, 0x58, 0x150, 0x1F8 },
    { 0x140, 0x100, 0x178, 0x130, 0xE0, 0x30, 0x160, 0x1F8 },
    { 0x180, 0x100, 0x198, 0x130, 0x160, 0x30, 0x170, 0x1F8 },
    { 0x180, 0x100, 0x1A0, 0x130, 0x180, 0x30, 0x140, 0x1F7 },
    { 0x180, 0x100, 0x190, 0x100, 0x140, 0, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x198, 0x158, 0x160, 0x58, 0x160, 0x1F7 },
    { 0x180, 0x100, 0x1A0, 0x158, 0x180, 0x58, 0x170, 0x1F7 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x1B0, 0x15C, 0x1C0, 0x5C, 0x140, 0x1F6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x1A8, 0x172, 0x1A0, 0x72, 0x150, 0x1F6 },
    { 0x180, 0x100, 0x180, 0x178, 0x100, 0x78, 0x160, 0x1F6 },
    { 0x180, 0x100, 0x198, 0x100, 0x160, 0, 0x170, 0x1F6 },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x140, 0x1F5 },
    { 0x180, 0x100, 0x1A8, 0x100, 0x1A0, 0, 0x150, 0x1F5 },
};
u16 actor8Talk0Conditions[] = { FLAG(0x1C, 0x27), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0), 0, FLAG(0x1C, 0x27), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor8Talk2Conditions[] = { SPECIAL(0x71), 0, FLAG(0, 0), 1, FLAG(0x1C, 0x27), 0, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0x1C, 0x2E), 1, FLAG(0x1C, 0x27), 1, CODES_END };
u16 actor8Talk3Conditions[] = {
    FLAG(0x1C, 0x27), 0,
    FLAG(0, 0), 1,
    SPECIAL(0x71), 1,
    SPECIAL(0x70), 0,
    CODES_END,
};
u16 actor8Talk3Actions[] = {
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    FLAG(0x1C, 0x36), 1,
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x27), 0,
    CODES_END,
};
u16 actor8Talk4Conditions[] = {
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0, 0), 1,
    SPECIAL(0x71), 1,
    CODES_END,
};
u16 actor8Talk4Actions[] = { FLAG(0x1C, 0x27), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0x1A, 0x34), 0, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0x1A, 0x34), 1, CODES_END };
u16 actor13Talk1Actions[] = { START_EVENT(0x4B), 1, CODES_END };
u16 actor35Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor35Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor35Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor37Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor37Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor37Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor38Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor38Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor38Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor39Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor39Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor39Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor40Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor40Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor40Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor41Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor41Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor41Talk1Conditions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor45Talk0Conditions[] = { FLAG(0x1C, 0x28), 1, CODES_END };
u16 actor45Talk1Conditions[] = { FLAG(0, 1), 0, FLAG(0x1C, 0x28), 0, CODES_END };
u16 actor45Talk1Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor45Talk2Conditions[] = { FLAG(0, 1), 1, SPECIAL(0x70), 0, FLAG(0x1C, 0x28), 0, CODES_END };
u16 actor45Talk2Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor45Talk3Conditions[] = {
    FLAG(0, 1), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x28), 0,
    CODES_END,
};
u16 actor45Talk3Actions[] = { FLAG(0x1C, 0x28), 1, CODES_END };
u16 actor45Talk4Conditions[] = {
    FLAG(0, 1), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x2E), 1,
    FLAG(0x1C, 0x28), 0,
    CODES_END,
};
u16 actor45Talk4Actions[] = { FLAG(0x1C, 0x28), 1, FLAG(0x1C, 0x2F), 1, CODES_END };
u16 actor47Talk0Conditions[] = { FLAG(0x1C, 0x29), 1, CODES_END };
u16 actor47Talk1Conditions[] = { FLAG(0x1C, 0x29), 0, FLAG(0, 2), 0, CODES_END };
u16 actor47Talk1Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor47Talk2Conditions[] = { FLAG(0x1C, 0x29), 0, FLAG(0, 2), 1, SPECIAL(0x70), 0, CODES_END };
u16 actor47Talk2Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor47Talk3Conditions[] = {
    FLAG(0x1C, 0x29), 0,
    FLAG(0, 2), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x2F), 0,
    CODES_END,
};
u16 actor47Talk3Actions[] = { FLAG(0x1C, 0x29), 1, CODES_END };
u16 actor47Talk4Conditions[] = {
    FLAG(0x1C, 0x29), 0,
    FLAG(0, 2), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x2F), 1,
    CODES_END,
};
u16 actor47Talk4Actions[] = { FLAG(0x1C, 0x29), 1, FLAG(0x1C, 0x30), 1, CODES_END };
u16 actor51Talk0Conditions[] = { FLAG(0x1C, 0x2A), 1, CODES_END };
u16 actor51Talk1Conditions[] = { FLAG(0, 3), 0, FLAG(0x1C, 0x2A), 0, CODES_END };
u16 actor51Talk1Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor51Talk2Conditions[] = { FLAG(0x1C, 0x2A), 0, FLAG(0, 3), 1, SPECIAL(0x70), 0, CODES_END };
u16 actor51Talk2Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor51Talk3Conditions[] = {
    FLAG(0x1C, 0x2A), 0,
    FLAG(0, 3), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x30), 0,
    CODES_END,
};
u16 actor51Talk3Actions[] = { FLAG(0x1C, 0x2A), 1, CODES_END };
u16 actor51Talk4Conditions[] = {
    FLAG(0x1C, 0x2A), 0,
    FLAG(0, 3), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x30), 1,
    CODES_END,
};
u16 actor51Talk4Actions[] = { FLAG(0x1C, 0x2A), 1, FLAG(0x1C, 0x31), 1, CODES_END };
u16 actor53Talk0Conditions[] = { FLAG(0x1C, 0x2B), 1, CODES_END };
u16 actor53Talk1Conditions[] = { FLAG(0x1C, 0x2B), 0, FLAG(0, 4), 0, CODES_END };
u16 actor53Talk1Actions[] = { FLAG(0, 4), 1, CODES_END };
u16 actor53Talk2Conditions[] = { FLAG(0x1C, 0x2B), 0, FLAG(0, 4), 1, SPECIAL(0x70), 0, CODES_END };
u16 actor53Talk2Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor53Talk3Conditions[] = {
    FLAG(0x1C, 0x2B), 0,
    FLAG(0, 4), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x31), 0,
    CODES_END,
};
u16 actor53Talk3Actions[] = { FLAG(0x1C, 0x2B), 1, CODES_END };
u16 actor53Talk4Conditions[] = {
    FLAG(0x1C, 0x2B), 0,
    FLAG(0, 4), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x31), 1,
    CODES_END,
};
u16 actor53Talk4Actions[] = { FLAG(0x1C, 0x2B), 1, FLAG(0x1C, 0x32), 1, CODES_END };
u16 actor55Talk0Conditions[] = { FLAG(0x1C, 0x2C), 1, CODES_END };
u16 actor55Talk1Conditions[] = { FLAG(0x1C, 0x2C), 0, FLAG(0, 5), 0, CODES_END };
u16 actor55Talk1Actions[] = { FLAG(0, 5), 1, CODES_END };
u16 actor55Talk2Conditions[] = { FLAG(0x1C, 0x2C), 0, FLAG(0, 5), 1, SPECIAL(0x70), 0, CODES_END };
u16 actor55Talk2Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor55Talk3Conditions[] = {
    FLAG(0x1C, 0x2C), 0,
    FLAG(0, 5), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x32), 0,
    CODES_END,
};
u16 actor55Talk3Actions[] = { FLAG(0x1C, 0x2C), 1, CODES_END };
u16 actor55Talk4Conditions[] = {
    FLAG(0x1C, 0x2C), 0,
    FLAG(0, 5), 1,
    SPECIAL(0x70), 1,
    FLAG(0x1C, 0x32), 1,
    CODES_END,
};
u16 actor55Talk4Actions[] = { FLAG(0x1C, 0x2C), 1, FLAG(0x1C, 0x33), 1, CODES_END };
u16 actor57Talk0Conditions[] = { FLAG(0x1C, 0x2D), 1, CODES_END };
u16 actor57Talk1Conditions[] = { FLAG(0x1C, 0x2D), 0, FLAG(0, 6), 0, CODES_END };
u16 actor57Talk1Actions[] = { FLAG(0, 6), 1, CODES_END };
u16 actor57Talk2Conditions[] = { FLAG(0x1C, 0x2D), 0, FLAG(0, 6), 1, SPECIAL(0x73), 0, CODES_END };
u16 actor57Talk2Actions[] = { FLAG(0x1C, 0x2D), 1, CODES_END };
u16 actor57Talk3Conditions[] = {
    FLAG(0x1C, 0x33), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0, 6), 1,
    SPECIAL(0x73), 1,
    CODES_END,
};
u16 actor57Talk3Actions[] = {
    START_EVENT(0x61), 1,
    FLAG(0x1C, 0x36), 1,
    FLAG(0x1C, 0x27), 0,
    FLAG(0x1C, 0x28), 0,
    FLAG(0x1C, 0x29), 0,
    FLAG(0x1C, 0x2A), 0,
    FLAG(0x1C, 0x2B), 0,
    FLAG(0x1C, 0x2C), 0,
    FLAG(0x1C, 0x2D), 0,
    FLAG(0x1C, 0x2E), 0,
    FLAG(0x1C, 0x2F), 0,
    FLAG(0x1C, 0x30), 0,
    FLAG(0x1C, 0x31), 0,
    FLAG(0x1C, 0x32), 0,
    FLAG(0x1C, 0x33), 0,
    CODES_END,
};
u16 actor57Talk4Conditions[] = {
    FLAG(0x1C, 0x2D), 0,
    FLAG(0, 6), 1,
    SPECIAL(0x73), 1,
    FLAG(0x1C, 0x33), 1,
    CODES_END,
};
u16 actor57Talk4Actions[] = { START_EVENT(0x61), 1, FLAG(0x1C, 0x37), 1, CODES_END };
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x21E },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x5A },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x21D },
    { actor8Talk3Conditions, actor8Talk3Actions, 0xBD },
    { actor8Talk4Conditions, actor8Talk4Actions, 0x21D },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0xB0 },
    { actor13Talk1Conditions, actor13Talk1Actions, 0xB1 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x48E },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x8C },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x8E },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x493 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x493 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0xAF },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0xAF },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { actor35Talk0Conditions, actor35Talk0Actions, 0x48D },
    { actor35Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x49B },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { actor37Talk0Conditions, actor37Talk0Actions, 0x48D },
    { actor37Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { actor38Talk0Conditions, actor38Talk0Actions, 0x48D },
    { actor38Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { actor39Talk0Conditions, actor39Talk0Actions, 0x48D },
    { actor39Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { actor40Talk0Conditions, actor40Talk0Actions, 0x48D },
    { actor40Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { actor41Talk0Conditions, actor41Talk0Actions, 0x48D },
    { actor41Talk1Conditions, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x48D },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x494 },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x494 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { actor45Talk0Conditions, NULL, 0x220 },
    { actor45Talk1Conditions, actor45Talk1Actions, 0x5B },
    { actor45Talk2Conditions, actor45Talk2Actions, 0xBD },
    { actor45Talk3Conditions, actor45Talk3Actions, 0x21F },
    { actor45Talk4Conditions, actor45Talk4Actions, 0x21F },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { actor47Talk0Conditions, NULL, 0x222 },
    { actor47Talk1Conditions, actor47Talk1Actions, 0x5C },
    { actor47Talk2Conditions, actor47Talk2Actions, 0xBD },
    { actor47Talk3Conditions, actor47Talk3Actions, 0x221 },
    { actor47Talk4Conditions, actor47Talk4Actions, 0x221 },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0xAD },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { NULL, NULL, 0xAD },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { actor51Talk0Conditions, NULL, 0x224 },
    { actor51Talk1Conditions, actor51Talk1Actions, 0x5D },
    { actor51Talk2Conditions, actor51Talk2Actions, 0xBD },
    { actor51Talk3Conditions, actor51Talk3Actions, 0x223 },
    { actor51Talk4Conditions, actor51Talk4Actions, 0x223 },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { actor53Talk0Conditions, NULL, 0x226 },
    { actor53Talk1Conditions, actor53Talk1Actions, 0x5E },
    { actor53Talk2Conditions, actor53Talk2Actions, 0xBD },
    { actor53Talk3Conditions, actor53Talk3Actions, 0x225 },
    { actor53Talk4Conditions, actor53Talk4Actions, 0x225 },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor55Talks[] = {
    { actor55Talk0Conditions, NULL, 0x21A },
    { actor55Talk1Conditions, actor55Talk1Actions, 0x5F },
    { actor55Talk2Conditions, actor55Talk2Actions, 0xBD },
    { actor55Talk3Conditions, actor55Talk3Actions, 0x227 },
    { actor55Talk4Conditions, actor55Talk4Actions, 0x227 },
    { NULL, NULL, 0 },
};
FieldTalk actor56Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor57Talks[] = {
    { actor57Talk0Conditions, NULL, 0x153 },
    { actor57Talk1Conditions, actor57Talk1Actions, 0x60 },
    { actor57Talk2Conditions, actor57Talk2Actions, 0x152 },
    { actor57Talk3Conditions, actor57Talk3Actions, 0xBD },
    { actor57Talk4Conditions, actor57Talk4Actions, 0xBD },
    { NULL, NULL, 0 },
};
FieldTalk actor58Talks[] = {
    { NULL, NULL, 0x49A },
    { NULL, NULL, 0 },
};
FieldTalk actor59Talks[] = {
    { NULL, NULL, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor60Talks[] = {
    { NULL, NULL, 0xAE },
    { NULL, NULL, 0 },
};
FieldTalk actor62Talks[] = {
    { NULL, NULL, 0xB2 },
    { NULL, NULL, 0 },
};
FieldTalk actor63Talks[] = {
    { NULL, NULL, 0xB2 },
    { NULL, NULL, 0 },
};
FieldTalk actor64Talks[] = {
    { NULL, NULL, 0xB2 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 0, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 0, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor40Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor43Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor44Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor45Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor46Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor47Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor48Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor49Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor50Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor51Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor52Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor53Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor54Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor55Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor56Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor57Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor58Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor59Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor60Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor61Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor62Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor63Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor64Conditions[] = { PROGRESS(0x21), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 1, 4, 0, 0, 1 };
FieldActorEntry actor2 = { actor2Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0xB, 5, 0, 0, 1 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0xC, 6, 0, 0, 1 };
FieldActorEntry actor5 = { actor5Conditions, NULL, 0x25, 7, 0, 0, 1 };
FieldActorEntry actor6 = { actor6Conditions, NULL, 0x26, 8, 0, 0, 1 };
FieldActorEntry actor7 = { actor7Conditions, NULL, 0x27, 9, 0, 0, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x3F, 0xA, 121, 260, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x3F, 0xA, 121, 260, 7 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x45, 0xB, 0, 0, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x46, 0xC, 0, 0, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x65, 0xD, 335, 273, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x65, 0xD, 416, 160, 3 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x65, 0xD, 0, 0, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x66, 0xE, 310, 300, 5 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x67, 0xF, 0, 0, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x67, 0xF, 368, 312, 3 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x67, 0xF, 279, 284, 5 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x67, 0xF, 279, 284, 5 };
FieldActorEntry actor26 = { actor26Conditions, NULL, 0x6A, 0x10, 0, 0, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0x6B, 0x11, 0, 0, 1 };
FieldActorEntry actor28 = { actor28Conditions, NULL, 0x6C, 0x12, 0, 0, 1 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x6C, 0x12, 0, 0, 1 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x6D, 0x13, 0, 0, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x6E, 0x14, 0, 0, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x6F, 0x15, 0, 0, 1 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x9D, 0x16, 416, 297, 5 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x9D, 0x16, 416, 297, 5 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0xB3, 0x17, 416, 160, 3 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0xB3, 0x17, 441, 146, 3 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0xC6, 0x18, 185, 229, 7 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0xC6, 0x18, 185, 229, 7 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0xC7, 0x19, 249, 197, 7 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0xC7, 0x19, 249, 197, 7 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x119, 0x1A, 428, 278, 1 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0x119, 0x1A, 428, 278, 1 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0x121, 0x1B, 376, 188, 1 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0x121, 0x1B, 376, 188, 1 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0x122, 0x1C, 360, 380, 3 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0x122, 0x1C, 360, 380, 3 };
FieldActorEntry actor55 = { actor55Conditions, actor55Talks, 0x123, 0x1D, 424, 348, 3 };
FieldActorEntry actor56 = { actor56Conditions, actor56Talks, 0x123, 0x1D, 424, 348, 3 };
FieldActorEntry actor57 = { actor57Conditions, actor57Talks, 0x124, 0x1E, 488, 316, 3 };
FieldActorEntry actor58 = { actor58Conditions, actor58Talks, 0x124, 0x1E, 488, 316, 3 };
FieldActorEntry actor59 = { actor59Conditions, actor59Talks, 0x13A, 0x1F, 451, 290, 1 };
FieldActorEntry actor60 = { actor60Conditions, actor60Talks, 0x13A, 0x1F, 451, 290, 1 };
FieldActorEntry actor61 = { actor61Conditions, NULL, 0x13D, 0x20, 0, 0, 1 };
FieldActorEntry actor62 = { actor62Conditions, actor62Talks, 0x17D, 0x21, 367, 233, 1 };
FieldActorEntry actor63 = { actor63Conditions, actor63Talks, 0x17E, 0x22, 391, 245, 1 };
FieldActorEntry actor64 = { actor64Conditions, actor64Talks, 0x17F, 0x23, 415, 257, 1 };
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
    &actor42,
    &actor43,
    &actor44,
    &actor45,
    &actor46,
    &actor47,
    &actor48,
    &actor49,
    &actor50,
    &actor51,
    &actor52,
    &actor53,
    &actor54,
    &actor55,
    &actor56,
    &actor57,
    &actor58,
    &actor59,
    &actor60,
    &actor61,
    &actor62,
    &actor63,
    &actor64,
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x217, 0x398, 0xCC, 1, 0, 0, 0 },
    { { { SPECIAL(0x94), 1 }, { SPECIAL(0x19), 0 } }, SLOT_EVENT, 0x3C5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 350, script350, EVENT_TEXT(0), NULL, endEvent350 },
    { 351, script351, EVENT_TEXT(1), NULL, endEvent351 },
    { 660, script660, EVENT_TEXT(2), NULL, endEvent660 },
    { 661, script661, EVENT_TEXT(3), NULL, endEvent661 },
    { 683, script683, EVENT_TEXT(4), NULL, NULL },
    { 890, script890, EVENT_TEXT(5), NULL, endEvent890 },
    { 893, script893, EVENT_TEXT(6), NULL, endEvent893 },
    { 894, script894, EVENT_TEXT(7), NULL, endEvent894 },
    { 895, script895, EVENT_TEXT(8), NULL, endEvent895 },
    { 960, script960, EVENT_TEXT(9), NULL, endEvent960 },
    { 965, script965, EVENT_TEXT(0xA), NULL, NULL },
    { 1450, script1450, EVENT_TEXT(0xA), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
