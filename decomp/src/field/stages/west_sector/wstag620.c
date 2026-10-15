/*
 * WSTAG620: Secret Room, West Sector: the rebels' hideout. Off the Sewers
 * (WSTAG615): Lisa, Kail, Keith and Nick, who looks for the pass code, and the
 * Byakko Leader.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern StageSeqStep updateTileSeqs8Steps[4][4];

/*
 * stepTileAnimation (common/step_tile_animation.inc.c) for a StageTileSeq8:
 * steps OBJ's animation through FRAMES and returns its frame, looping at the
 * end or returning 0xFF if once
 */
s32 stepTileAnimation(StageTileSeq8 *obj, AnimFrame *frames, s32 once, s32 depth) {
    AnimFrame *frame = &frames[obj->anim.index];
    s32 dt = GFX.funcs.getFrameTime();

    if (dt > 4) {
        dt = 4;
    }
    if (depth == 0) {
        obj->anim.timer -= dt;
    }
    if (obj->anim.timer <= 0) {
        frame++;
        obj->anim.index++;
        obj->anim.timer += frame->duration;
        if (once) {
            if (frame->frame == 0xFF) {
                return 0xFF;
            }
        } else if (frame->frame == 0xFF) {
            frame = frames;
            obj->anim.index = 0;
            obj->anim.timer += frame->duration;
        }
        stepTileAnimation(obj, frames, once, depth + 1);
    }
    return frame->frame;
}

/*
 * Plays the steps of updateTileSeqs8Steps on the map objects with animations
 * 1 to 4, which go on to their next step at the end, the last with a sound
 */
void updateTileSeqs8(StageTileSeqs8 *task) {
    StageTile *object;
    s32 n;
    StageTile *tile;
    s32 i;
    s32 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        n = 0;
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim >= 1 && object->anim <= 4) {
                task->entries[n].tile = object;
                n++;
            }
        }
        task->entries[3].setFrame = 1;
        task->entries[3].sound = 1;
        task->entries[0].seq = 2;
        task->entries[1].seq = 2;
        task->entries[0].setFrame = 0;
        task->entries[1].setFrame = 0;
        task->entries[2].setFrame = 0;
        task->entries[2].seq = 1;
        task->entries[3].seq = 1;
        task->entries[0].anim.index = 0;
        task->entries[0].anim.timer = updateTileSeqs8Steps[0][2].frames->duration;
        task->entries[1].anim.index = 0;
        task->entries[1].anim.timer = updateTileSeqs8Steps[1][2].frames->duration;
        task->entries[2].anim.index = 0;
        task->entries[2].anim.timer = updateTileSeqs8Steps[2][1].frames->duration;
        task->entries[3].anim.index = 0;
        task->entries[3].anim.timer = updateTileSeqs8Steps[3][1].frames->duration;
        break;
    case TASK_RUN:
        for (i = 0; i < 4; i++) {
            tile = task->entries[i].tile;
            if (task->entries[i].seq != 0) {
                frame = stepTileAnimation(&task->entries[i], updateTileSeqs8Steps[i][task->entries[i].seq].frames,
                                      updateTileSeqs8Steps[i][task->entries[i].seq].once, 0);
                switch (frame) {
                case 0xFF:
                    if (updateTileSeqs8Steps[i][task->entries[i].seq].next == 0) {
                        task->entries[i].seq = 0;
                        tile->visible = 0;
                    } else {
                        task->entries[i].seq = updateTileSeqs8Steps[i][task->entries[i].seq].next;
                        task->entries[i].anim.index = 0;
                        task->entries[i].anim.timer = updateTileSeqs8Steps[i][task->entries[i].seq].frames->duration;
                    }
                    break;
                case 0x12C:
                    tile->visible = 0;
                    break;
                default:
                    tile->visible = 1;
                    if (task->entries[i].setFrame == 0) {
                        tile->clutRow = frame;
                    } else {
                        if (frame == 0x34 && task->entries[i].sound == 1) {
                            SOUND.playSound(SOUND_DIGITAMA);
                            task->entries[i].sound = 0;
                        }
                        tile->frame = frame;
                    }
                    break;
                }
            } else {
                tile->visible = 0;
            }
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/*
 * Command 849 of the event scripts: the map objects of event 532, at Lucky
 * Mouse's hideout (updateTileSeqs8)
 */
void *createCommand849(s32 arg) {
    return createTaskWithId(updateTileSeqs8, sizeof(StageTileSeqs8), 0, arg);
}

/* Creates the event object of the story so far, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 0x13 && FLAGS_00.checkCondition(FLAG(0x40, 0x3B), 0)) {
            children[0] = FIELDSTG_startEvent(0x214);
        } else if (GAME.progress == 0x13 && FLAGS_00.checkCondition(FLAG(0x40, 0x3B), 1)) {
            children[0] = FIELDSTG_startEvent(0x216);
        } else if (FLAGS_00.checkCondition(FLAG(0x40, 0x3A), 0) && FLAGS_00.checkCondition(FLAG(0x1C, 0x1B), 1)) {
            children[0] = FIELDSTG_startEvent(0x21C);
        } else if (GAME.progress == 0x14 && FLAGS_00.checkCondition(FLAG(0x40, 3), 1)) {
            children[0] = FIELDSTG_startEvent(0x227);
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

/*
 * After event 532, with Lucky Mouse, Lisa, Kail, General and Attacker:
 * starts the stage's event battle 0 and keeps it from playing again
 */
void endEvent532(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x3B), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/* After event 534, with Attacker, Lisa, Kail and Kurt: moves the story on to its step 0x14 */
void endEvent534(void) {
    GAME.progress = 0x14;
}

/*
 * After event 540, with Kail, Kurt and Lisa: lets event 550 of WSTAG525
 * start and keeps it from playing again
 */
void endEvent540(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x1C), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0x3A), 1);
}

/* After event 551, with Lisa, Keith, Nick and Kail: moves the story on to its step 0x15 */
void endEvent551(void) {
    GAME.progress = 0x15;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x479
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x489
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0019 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x20200, 0x17800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x15;
    FIELDSTG_state.music = MUSIC(0x15, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script532[] = {
    SCRIPT_FOLLOW(1, 1),
    SCRIPT_PLACE(1, 0x1C0, 0x189),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_PLACE(0x65, 0x1C1, 0x168),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_PLACE(0x68, 0x190, 0x178),
    SCRIPT_POSE(0x68, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(1, 0x3A, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 1, 1, 3),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x1A4, 0x178),
    SCRIPT_TALK(0, 3, 0x68, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x68, 0),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 1, 3),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x21, 0x68, 0),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x68, 0),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0x68, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x68, 0),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 1, 4),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_DEMO_BGM, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0x65),
    SCRIPT_COMMAND(0x325, 0x325, 0x68),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0x65),
    SCRIPT_COMMAND(0x325, 0x326, 0x68),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x1C0, 0x193),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x68, 1, 7),
    SCRIPT_PLACE(0x98, 0x288, 0x1BC),
    SCRIPT_POSE(0x98, 1, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0xB, 0x288, 0x1BC),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_WALK(0x98, 0x268, 0x1AC, 3),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_WALK(0xB, 0x268, 0x1AC, 3),
    SCRIPT_WALK(0x98, 0x248, 0x19C, 3),
    SCRIPT_PLACE(0xC2, 0x288, 0x1BC),
    SCRIPT_POSE(0xC2, 1, 3),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_WALK(0xB, 0x248, 0x19C, 3),
    SCRIPT_PLACE(0x45, 0x288, 0x1BC),
    SCRIPT_POSE(0x45, 1, 3),
    SCRIPT_WALK(0x98, 0x228, 0x18C, 3),
    SCRIPT_WALK(0xC2, 0x268, 0x1AC, 3),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_WALK(0xB, 0x228, 0x18C, 3),
    SCRIPT_WALK(0x45, 0x268, 0x1AC, 3),
    SCRIPT_WALK(0x98, 0x208, 0x19C, 1),
    SCRIPT_WALK(0xC2, 0x248, 0x19C, 3),
    SCRIPT_WAIT_WALK(0x45),
    SCRIPT_WALK(0xB, 0x208, 0x19C, 1),
    SCRIPT_WALK(0x45, 0x248, 0x19C, 3),
    SCRIPT_WALK(0x98, 0x1F0, 0x1A7, 3),
    SCRIPT_WALK(0xC2, 0x228, 0x18C, 1),
    SCRIPT_WAIT_WALK(0x45),
    SCRIPT_WALK(0xB, 0x202, 0x19E, 3),
    SCRIPT_WALK(0x45, 0x228, 0x18C, 3),
    SCRIPT_POSE(0x98, 1, 3),
    SCRIPT_WALK(0xC2, 0x215, 0x195, 3),
    SCRIPT_WAIT_WALK(0x45),
    SCRIPT_TALK(1, 0xD, 0x68, 2),
    SCRIPT_TALK(0, 0xE, 1, 1),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_POSE(0x45, 1, 3),
    SCRIPT_POSE(0x98, 1, 3),
    SCRIPT_POSE(0xC2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0xB, 1),
    SCRIPT_POSE(0xB, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 1, 2),
    SCRIPT_POSE(1, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x68, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 0x98, 1),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 0x68, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_COMMAND(0x324, 0x325, 0xB),
    SCRIPT_COMMAND(0x325, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_COMMAND(0x324, 0x326, 0xB),
    SCRIPT_COMMAND(0x325, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x68, 0x48, 7),
    SCRIPT_COMMAND(0x323, 0x327, 0x68),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x68),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 0x15, 0x68, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x68, 0x1B0, 0x188, 7),
    SCRIPT_WAIT_WALK(0x68),
    SCRIPT_POSE(1, 1, 1),
    SCRIPT_POSE(0xB, 1, 2),
    SCRIPT_POSE(0x45, 1, 2),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WALK(0x68, 0x180, 0x1A0, 1),
    SCRIPT_POSE(0x98, 1, 2),
    SCRIPT_POSE(0xC2, 1, 2),
    SCRIPT_WAIT_WALK(0x68),
    SCRIPT_POSE(0x68, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x68, 0x49, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x351, 0x335, 1),
    SCRIPT_WAIT(0x8A),
    SCRIPT_POSE(1, 1, 6),
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_POSE(0x45, 1, 3),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x68, 1, 6),
    SCRIPT_POSE(0x98, 1, 4),
    SCRIPT_PLACE(0x99, 0x201, 0x176),
    SCRIPT_POSE(0x99, 1, 0),
    SCRIPT_POSE(0xC2, 1, 3),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x98),
    SCRIPT_TALK(0, 0x16, 0x98, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x99, 0, 0),
    SCRIPT_POSE(0x99, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xB, 1, 3),
    SCRIPT_POSE(0x45, 1, 7),
    SCRIPT_POSE(0x98, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x98, 0x208, 0x1B4, 7),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_WALK(0x98, 0x240, 0x198, 5),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_WALK(0x45, 0x281, 0x1B9, 7),
    SCRIPT_WALK(0x98, 0x290, 0x1C0, 7),
    SCRIPT_POSE(0xC2, 1, 2),
    SCRIPT_WAIT_WALK(0x45),
    SCRIPT_TALK(0, 0x17, 0xC2, 0),
    SCRIPT_PLACE(0x45, 0, 0),
    SCRIPT_POSE(0x45, 1, 0),
    SCRIPT_PLACE(0x98, 0, 0),
    SCRIPT_POSE(0x98, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x1B0, 0x190, 5),
    SCRIPT_WALK(0xB, 0x1C8, 0x184, 1),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WALK(0x68, 0x198, 0x194, 5),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_TALK(0, 0x18, 0xB, 2),
    SCRIPT_POSE(0xB, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_LOOK_AT(0, 0x1B0, 0x190),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x190, 0x181, 3),
    SCRIPT_WALK(0xB, 0x1A8, 0x174, 3),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x68, 1, 4),
    SCRIPT_WALK(0xC2, 0x202, 0x19E, 3),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_WALK(1, 0x1A0, 0x178, 7),
    SCRIPT_WALK(0xB, 0x1B0, 0x170, 7),
    SCRIPT_POSE(0xC2, 1, 3),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_TALK(0, 0x19, 0xC2, 1),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_POSE(0x68, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x68),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x68),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1A, 0x68, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0x68, 0x1C0, 0x180, 5),
    SCRIPT_WAIT_WALK(0x68),
    SCRIPT_WALK(0x68, 0x1D0, 0x188, 7),
    SCRIPT_POSE(0xC2, 0x4A, 3),
    SCRIPT_WAIT_WALK(0x68),
    SCRIPT_PLACE(0x14B, 0x202, 0x19E),
    SCRIPT_POSE(0x14B, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_BEAM_SHT, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x14B, 0, 0),
    SCRIPT_POSE(0x14B, 1, 0),
    SCRIPT_PLACE(0x14C, 0x1D0, 0x188),
    SCRIPT_POSE(0x14C, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_BEAM_HIT, 1),
    SCRIPT_WAIT(0x30),
    SCRIPT_TALK(1, 0x1C, 0xB, 0),
    SCRIPT_TALK(0, 0x1B, 0x68, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_PLACE(0x68, 0, 0),
    SCRIPT_POSE(0x68, 1, 0),
    SCRIPT_PLACE(0x13D, 0x1D0, 0x187),
    SCRIPT_POSE(0x13D, 1, 7),
    SCRIPT_WAIT(0x96),
    SCRIPT_PLACE(0x14C, 0, 0),
    SCRIPT_POSE(0x14C, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_STOP_BEAM_HIT, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1D, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1E, 0xC2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC2, 0x4A, 3),
    SCRIPT_WAIT(0x24),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH02, 1),
    SCRIPT_WAIT(0x30),
    SCRIPT_POSE(0xC2, 0x4A, 3),
    SCRIPT_WAIT(0x18),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH02, 1),
    SCRIPT_WAIT(0xC),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SWITCH02, 1),
    SCRIPT_WAIT(0x30),
    SCRIPT_TALK(0, 0x1F, 0xC2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x1B8, 0x184, 7),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_TALK(0, 0x20, 1, 1),
    SCRIPT_POSE(1, 0xC, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_END,
};
s16 script534[] = {
    SCRIPT_PLACE(1, 0x1C0, 0x188),
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_PLACE(0xB, 0x1B0, 0x170),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_PLACE(0x65, 0x1C0, 0x168),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_PLACE(0x9D, 0x1A0, 0x178),
    SCRIPT_POSE(0x9D, 1, 7),
    SCRIPT_PLACE(0xC2, 0x202, 0x19E),
    SCRIPT_POSE(0xC2, 1, 3),
    SCRIPT_PLACE(0x13D, 0x1A0, 0x178),
    SCRIPT_POSE(0x13D, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0xC2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0xC2, 0x228, 0x18C, 5),
    SCRIPT_WAIT_WALK(0xC2),
    SCRIPT_WALK(0xC2, 0x280, 0x1B8, 7),
    SCRIPT_WAIT_WALK(0xC2),
    SCRIPT_LOOK_AT(0, 0x1C0, 0x172),
    SCRIPT_PLACE(0xC2, 0, 0),
    SCRIPT_POSE(0xC2, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xB, 0),
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_POSE(0xB, 7, 1),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x13D, 1),
    SCRIPT_POSE(0x9D, 1, 5),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 1, 3),
    SCRIPT_POSE(1, 0xC, 3),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_POSE(0x9D, 1, 7),
    SCRIPT_POSE(0x13D, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 1, 1),
    SCRIPT_POSE(1, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 2),
    SCRIPT_POSE(1, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(1, 0x210, 0x1B0, 7),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_WALK(1, 0x240, 0x198, 7),
    SCRIPT_WAIT_WALK(1),
    SCRIPT_WALK(1, 0x280, 0x1B8, 7),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x254, 0x2D4, 0xC4, 7),
    SCRIPT_END,
};
s16 script540[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x268, 0x1AC),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0xB, 0x1D8, 0x17D),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_PLACE(0x65, 0x1B0, 0x104),
    SCRIPT_POSE(0x65, 1, 3),
    SCRIPT_PLACE(0x13D, 0x1C1, 0x188),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x228, 0x18C, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xB, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xB, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x13D),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x13D),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x13D, 3),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xB, 2),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0xB, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x1F2, 0x140),
    SCRIPT_TALK(0, 0x14, 0x65, 3),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0xB, 2),
    SCRIPT_POSE(0xB, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 0x13D, 2),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0xB, 2),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 2, 2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 2, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 0x65, 3),
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 2, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_POSE(0x13D, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x268, 0x1AC, 7),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x254, 0x2D4, 0xC4, 7),
    SCRIPT_END,
};
s16 script551[] = {
    SCRIPT_LOOK_AT(1, 0x1AE, 0x131),
    SCRIPT_PLACE(2, 0x228, 0x18C),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0xB, 0x1D8, 0x17D),
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_PLACE(0x65, 0x1B0, 0x104),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_PLACE(0x66, 0x1EE, 0xE8),
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_PLACE(0x67, 0x182, 0x1A0),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_PLACE(0x13D, 0x1C1, 0x188),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x65, 1),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_POSE(0xB, 1, 1),
    SCRIPT_POSE(0x65, 1, 1),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x65, 1),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_POSE(0x13D, 1, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 3, 0x66, 3),
    SCRIPT_TALK(0, 4, 0x67, 2),
    SCRIPT_POSE(0x66, 1, 1),
    SCRIPT_POSE(0x67, 1, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x66, 1, 5),
    SCRIPT_POSE(0x67, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x65, 3),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xB, 2),
    SCRIPT_POSE(0xB, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_POSE(0x65, 1, 5),
    SCRIPT_POSE(0x13D, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0xB),
    SCRIPT_TALK(0, 9, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 2, 1),
    SCRIPT_POSE(2, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x256, 0x228, 0x18C, 3),
    SCRIPT_END,
};
AnimFrame updateTileSeqs8Steps_0_0[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 1, 6 },
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 1, 6 },
    { 255, 0 },
};
AnimFrame updateTileSeqs8Steps_0_1[] = {
    { 0, 6 }, { 0, 6 }, { 0, 6 }, { 1, 6 },
    { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 },
    { 6, 6 }, { 7, 6 }, { 6, 6 }, { 5, 6 },
    { 4, 6 }, { 3, 6 }, { 2, 6 }, { 1, 6 },
    { 0, 6 }, { 0, 6 }, { 255, 0x3E7 },
};
AnimFrame updateTileSeqs8Steps_0_2[] = {
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 1, 6 },
    { 0, 6 }, { 1, 6 }, { 2, 6 }, { 1, 6 },
    { 255, 0 },
};
AnimFrame updateTileSeqs8Steps_1_0[] = {
    { 0, 12 }, { 1, 12 }, { 2, 12 }, { 1, 12 },
    { 255, 0 },
};
AnimFrame updateTileSeqs8Steps_1_1[] = {
    { 2, 12 }, { 3, 12 }, { 4, 12 }, { 5, 12 },
    { 6, 12 }, { 7, 12 }, { 8, 12 }, { 9, 12 },
    { 10, 12 }, { 255, 0x3E7 },
};
AnimFrame updateTileSeqs8Steps_1_2[] = {
    { 9, 12 }, { 10, 12 }, { 9, 12 }, { 8, 12 },
    { 255, 0 },
};
AnimFrame updateTileSeqs8Steps_2[] = {
    { 0x12C, 90 }, { 0, 6 }, { 1, 6 }, { 2, 6 },
    { 3, 6 }, { 4, 6 }, { 3, 6 }, { 4, 6 },
    { 3, 6 }, { 4, 6 }, { 3, 6 }, { 2, 6 },
    { 1, 6 }, { 2, 6 }, { 1, 6 }, { 2, 6 },
    { 1, 6 }, { 255, 0x3E7 },
};
AnimFrame updateTileSeqs8Steps_3[] = {
    { 0x12C, 108 }, { 50, 6 }, { 51, 6 }, { 52, 6 },
    { 53, 6 }, { 54, 6 }, { 56, 6 }, { 57, 6 },
    { 58, 6 }, { 59, 6 }, { 255, 0x3E7 },
};
StageSeqStep updateTileSeqs8Steps[4][4] = {
    { { NULL, 0, 0 }, { updateTileSeqs8Steps_0_0, 0, 0 }, { updateTileSeqs8Steps_0_1, 1, 3 }, { updateTileSeqs8Steps_0_2, 0, 0 } },
    { { NULL, 0, 0 }, { updateTileSeqs8Steps_1_0, 0, 0 }, { updateTileSeqs8Steps_1_1, 1, 3 }, { updateTileSeqs8Steps_1_2, 0, 0 } },
    { { NULL, 0, 0 }, { updateTileSeqs8Steps_2, 1, 0 }, { NULL, 0, 0 }, { NULL, 0, 0 } },
    { { NULL, 0, 0 }, { updateTileSeqs8Steps_3, 1, 0 }, { NULL, 0, 0 }, { NULL, 0, 0 } },
};
AreaBattle area0Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area0Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area1Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area1Battles = {
    0,
    { &area1Battle0, &area1Battle1, &area1Battle2, &area1Battle3,
      &area1Battle4, &area1Battle5, &area1Battle6, &area1Battle7 },
};
AreaBattle area2Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area2Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area2Battles = {
    0,
    { &area2Battle0, &area2Battle1, &area2Battle2, &area2Battle3,
      &area2Battle4, &area2Battle5, &area2Battle6, &area2Battle7 },
};
AreaBattle area3Battle0 = { 188, 18, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 172, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x1A0, 0xD8, 0xA0, 0x160, 0x1F6 },
    { 0x140, 0x100, 0x158, 0x1A8, 0x60, 0xA8, 0x160, 0x1F5 },
    { 0x140, 0x100, 0x174, 0x150, 0xD0, 0x50, 0x170, 0x1F5 },
    { 0x140, 0x100, 0x158, 0x180, 0x60, 0x80, 0x160, 0x1F4 },
    { 0x140, 0x100, 0x160, 0x180, 0x80, 0x80, 0x170, 0x1F4 },
    { 0x140, 0x100, 0x140, 0x198, 0, 0x98, 0x140, 0x1F3 },
    { 0x140, 0x100, 0x178, 0x128, 0xE0, 0x28, 0x150, 0x1F3 },
    { 0x140, 0x100, 0x148, 0x198, 0x20, 0x98, 0x160, 0x1F3 },
    { 0x140, 0x100, 0x160, 0x1B8, 0x80, 0xB8, 0x170, 0x1F3 },
    { 0x140, 0x100, 0x140, 0x1C0, 0, 0xC0, 0x140, 0x1F2 },
    { 0x140, 0x100, 0x148, 0x1C0, 0x20, 0xC0, 0x150, 0x1F2 },
    { 0x140, 0x100, 0x150, 0x1C0, 0x40, 0xC0, 0x160, 0x1F2 },
    { 0x140, 0x100, 0x150, 0x198, 0x40, 0x98, 0x140, 0x1F1 },
    { 0x140, 0x100, 0x170, 0x1C0, 0xC0, 0xC0, 0x150, 0x1F1 },
    { 0x140, 0x100, 0x146, 0x1E0, 0x18, 0xE0, 0x160, 0x1F1 },
    { 0x140, 0x100, 0x15C, 0x150, 0x70, 0x50, 0x170, 0x1F1 },
};
u16 actor2Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor19Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x2FE },
    { actor2Talk1Conditions, NULL, 0x301 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x2FD },
    { actor5Talk1Conditions, NULL, 0x300 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x337 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x304 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x339 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0x2FF },
    { actor19Talk1Conditions, NULL, 0x302 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(7), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x15), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x15), 1, FLAG(0x40, 0x3D), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x15), 1, FLAG(0x40, 0x3D), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(7), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x13), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x13), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 1, 4, 0, 0, 0 };
FieldActorEntry actor1 = { actor1Conditions, NULL, 0xB, 5, 0, 0, 0 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xB, 5, 472, 381, 1 };
FieldActorEntry actor3 = { actor3Conditions, NULL, 0x45, 6, 0, 0, 1 };
FieldActorEntry actor4 = { actor4Conditions, NULL, 0x65, 7, 0, 0, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x65, 7, 432, 260, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x65, 7, 432, 260, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x66, 8, 496, 424, 3 };
FieldActorEntry actor8 = { actor8Conditions, NULL, 0x66, 8, 0, 0, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x67, 9, 533, 405, 3 };
FieldActorEntry actor10 = { actor10Conditions, NULL, 0x67, 9, 0, 0, 1 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x68, 0xA, 0, 0, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x98, 0xB, 0, 0, 1 };
FieldActorEntry actor13 = { actor13Conditions, NULL, 0x99, 0xC, 0, 0, 1 };
FieldActorEntry actor14 = { NULL, NULL, 0x9A, 0xD, 641, 350, 0 };
FieldActorEntry actor15 = { NULL, NULL, 0x9B, 0xE, 593, 325, 1 };
FieldActorEntry actor16 = { NULL, NULL, 0x9C, 0xF, 545, 301, 2 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0xC2, 0x10, 0, 0, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x13D, 0x11, 0, 0, 0 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x13D, 0x11, 449, 392, 5 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x14B, 0x12, 0, 0, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x14C, 0x13, 0, 0, 1 };
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
    { 0, 1, 0x40, 2, 0x3D, 0, 0, 0, 0, 0, 335, 351, 0, 0 },
    { 0, 2, 0x40, 2, 0x3E, 0, 0, 0, 0, 0, 345, 372, 0, 0 },
    { 0, 3, 0x40, 2, 0x3C, 0, 0, 0, 0, 0, 477, 356, 0, 0 },
    { 0, 4, 0x40, 2, 0x32, 0, 0, 0, 0, 0, 489, 350, 0, 0 },
    { 1, 0, 0x40, 2, 0x40, 2, 0, 3, 6, 0, 521, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x40, 2, 0, 3, 6, 0, 569, 236, 0, 0 },
    { 1, 0, 0x40, 2, 0x41, 2, 0, 3, 6, 0, 617, 260, 0, 0 },
    { 1, 0, 0x40, 2, 0x43, 2, 0, 5, 6, 0, 392, 163, 0, 0 },
    { 1, 0, 0x40, 2, 0x44, 2, 0, 5, 6, 0, 369, 205, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 2, 0, 3, 6, 0, 360, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 2, 0, 5, 6, 0, 361, 186, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 377, 371, 399, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x254, 0x2D4, 0xC4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x1D0, 0xF8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x1E0, 0x150, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 532, script532, EVENT_TEXT(0xF), NULL, endEvent532 },
    { 534, script534, EVENT_TEXT(0x10), NULL, endEvent534 },
    { 540, script540, EVENT_TEXT(0x16), NULL, endEvent540 },
    { 551, script551, EVENT_TEXT(0x17), NULL, endEvent551 },
    { -1, NULL, 0, NULL, NULL },
};
