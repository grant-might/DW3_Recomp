/*
 * WSTAG750: Chamber Room, North Sector: the emergency Matrix Chamber. Its
 * gates lead to the Real World and the Ice Dungeon (WSTAG735); the trip to the
 * Real World (its 180 seconds, the Juggernaut taken) plays here.
 */

#include "common.h"
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern AnimFrame *updateTileEffectFrames[];
extern StageEffectSpot updateStageSpots[];
extern s8 tileEffectSortY[];
extern s8 tileEffectY[];
extern s16 tileEffectPlaces[][2];

#include "common/step_animation_once.inc.c"

#include "common/reset_tile_effect.inc.c"

/* Moves its map objects to (x, y) and animates them when the substate is set to 1 */
void updateTileEffect(StageTileEffect *task) {
    StageTile *tile;
    StageTile *t;
    s32 i;
    s32 n;
    s32 j;
    s32 frame;
    s32 done;

    switch (task->state) {
    case TASK_INIT:
    default:
        resetTileEffect(task);
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            switch (task->step) {
            case 0:
                n = 0;
                for (t = FIELDSTG_state.objects; t->margin != 0; t++) {
                    if (t->anim >= 1 && t->anim <= 3) {
                        task->anims[n].tile = t;
                        t->x = task->x;
                        t->y = task->y + tileEffectY[n];
                        t->sortY = task->y + tileEffectSortY[n];
                        n++;
                    }
                }
                SOUND.playSound(SOUND_ENTRY_02);
                task->nextStep(task);
                /* fallthrough */
            case 1:
                done = 0;
                for (i = 0; i < 3; i++) {
                    tile = task->anims[i].tile;
                    frame = stepAnimationOnce(&task->anims[i], updateTileEffectFrames[i], 0);
                    switch (frame) {
                    case 0xFF:
                        done++;
                        tile->visible = 0;
                        tile->frame = 0;
                        break;
                    case 0x12C:
                        tile->visible = 0;
                        tile->frame = 0;
                        break;
                    default:
                        tile->visible = 1;
                        tile->frame = frame;
                        break;
                    }
                }
                if (done < 3) {
                    break;
                }
                task->nextStep(task);
                /* fallthrough */
            case 2:
                for (j = 0; j < 3; j++) {
                    task->anims[j].tile->visible = 0;
                }
                resetTileEffect(task);
                task->setSubstate(task, 0);
                break;
            }
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Starts the effect at the place of map 0x335 */
void handleCommand846(StageTileEffect *task, s32 id) {
    s32 i;

    if (task != NULL) {
        i = 0;
        switch (id) {
        case 0x335:
            task->x = tileEffectPlaces[i][0];
            task->y = tileEffectPlaces[i][1];
            task->setSubstate(task, 1);
            break;
        }
    }
}

#include "common/create_tile_effect.inc.c"

/* Creates the stage's effect, the event object of story progress 0x20 that applies, and another object */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (updateStageSpots[0].kind == 0) {
            children[0] = createStageEffect(updateStageSpots[0].x, updateStageSpots[0].y, updateStageSpots[0].frame);
        }
        if (GAME.progress == 0x20 && FLAGS_00.checkCondition(FLAG(0x40, 0x48), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x46), 0) &&
            FLAGS_00.checkCondition(FLAG(0x40, 0x66), 0)) {
            children[2] = FIELDSTG_startEvent(0x35C);
        } else if (GAME.progress == 0x20 && FLAGS_00.checkCondition(FLAG(0x40, 0x46), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x48), 0) &&
                   FLAGS_00.checkCondition(FLAG(0x40, 0x47), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x66), 0)) {
            children[2] = FIELDSTG_startEvent(0x373);
        } else if (GAME.progress == 0x20 && FLAGS_00.checkCondition(FLAG(0x40, 0x48), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x46), 0) &&
                   FLAGS_00.checkCondition(FLAG(0x40, 0x62), 0) && FLAGS_00.checkCondition(FLAG(0x40, 0x66), 0)) {
            children[2] = FIELDSTG_startEvent(0x374);
        } else if (GAME.progress == 0x20 && FLAGS_00.checkCondition(FLAG(0x40, 0x66), 1)) {
            children[2] = FIELDSTG_startEvent(0x376);
        }
        children[1] = createTileEffect(0x34E);
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 0xC
#include "common/start_stage.inc.c"
#include "common/step_animation.inc.c"
#include "common/draw_stage_effect.inc.c"
#include "common/is_on_screen.inc.c"
#include "common/update_stage_effect.inc.c"
#include "common/create_stage_effect.inc.c"

/* After event 820, with Chamber Officer: moves the story on to its step 0x20 */
void endEvent820(void) {
    GAME.progress = 0x20;
}

/* After event 860, with Chamber Office: keeps it from playing again */
void endEvent860(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x48), 1);
}

/* After event 883, with Chamber Officer: keeps it from playing again */
void endEvent883(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x47), 1);
}

/* After event 884, with the player: keeps it from playing again */
void endEvent884(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x62), 1);
}

/* After event 886, with the player: moves the story on to its step 0x21 */
void endEvent886(void) {
    GAME.progress = 0x21;
}

/*
 * After event 1245, with Guardromon: applies 0x7C0A, an action that does
 * nothing (applyAction's mode changes 0x7C are only 0 and 1)
 */
void endEvent1245(void) {
    FLAGS_00.applyAction(0x7C0A, 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x6B2
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x6C1
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0024 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12A00, 0x1D200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x40;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.music = MUSIC(0x40, 1);
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script820[] = {
    SCRIPT_WALK(2, 0xA0, 0x128, 3),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xD, 9, 7),
    SCRIPT_WAIT_ANIM(0xD),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xD, 0x6F, 0x110, 3),
    SCRIPT_WAIT_WALK(0xD),
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xD, 2, 3),
    SCRIPT_WAIT(0x78),
    SCRIPT_POSE(0xD, 1, 3),
    SCRIPT_WAIT(0x5A),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xD, 0x81, 0x119, 7),
    SCRIPT_WAIT_WALK(0xD),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x160, 0xC8, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LOOK_AT(1, 0x160, 0xC8),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x34E, 0x335, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_PLACE(2, 0, 0),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x2DA, 0x45F, 0x37D, 1),
    SCRIPT_END,
};
s16 script860[] = {
    SCRIPT_LOOK_AT(1, 0xF0, 0xE0),
    SCRIPT_PLACE(2, 0, 0),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_PLACE(0xD, 0x81, 0x119),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_LOOK_AT(0, 0xD0, 0xF0),
    SCRIPT_COMMAND(0x323, 0x325, 0xD),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xD),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xD, 0x6F, 0x110, 3),
    SCRIPT_WAIT_WALK(0xD),
    SCRIPT_POSE(0xD, 2, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0xF0, 0xE0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x34E, 0x335, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_PLACE(2, 0x15E, 0xCA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LOOK_AT(0, 0xD0, 0xF0),
    SCRIPT_TALK(0, 2, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(0xD, 0x81, 0x119, 7),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_LEAVE(0x2DC, 1, 1, 0),
    SCRIPT_END,
};
s16 script883[] = {
    SCRIPT_LOOK_AT(1, 0xF0, 0xE0),
    SCRIPT_PLACE(2, 0, 0),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_PLACE(0xD, 0x81, 0x119),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 3, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_LOOK_AT(0, 0xD0, 0xF0),
    SCRIPT_COMMAND(0x323, 0x325, 0xD),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xD),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xD, 0x6F, 0x110, 3),
    SCRIPT_WAIT_WALK(0xD),
    SCRIPT_POSE(0xD, 2, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LOOK_AT(0, 0xF0, 0xE0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x34E, 0x335, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_PLACE(2, 0x15E, 0xCA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LOOK_AT(0, 0xD0, 0xF0),
    SCRIPT_TALK(0, 1, 0xD, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x3C),
    SCRIPT_WALK(0xD, 0x81, 0x119, 7),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x2D7, 0, 0, 1),
    SCRIPT_END,
};
s16 script884[] = {
    SCRIPT_PLACE(2, 0x15E, 0xCA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0xD, 0x81, 0x119),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x2D7, 0x64, 0x64, 0),
    SCRIPT_END,
};
s16 script886[] = {
    SCRIPT_PLACE(2, 0x15E, 0xCA),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_PLACE(0xD, 0x81, 0x119),
    SCRIPT_POSE(0xD, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xA0, 0x128, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x26A, 0x3C0, 0x50, 7),
    SCRIPT_END,
};
s16 script1245[] = {
    SCRIPT_WALK(2, 0x100, 0x1C8, 3),
    SCRIPT_PLACE(0x15, 0xE0, 0x1B9),
    SCRIPT_POSE(0x15, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 7),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC08, 0, 0, 0),
    SCRIPT_END,
};
AnimFrame updateTileEffectFrames_0[] = {
    { 70, 4 }, { 71, 4 }, { 70, 4 }, { 71, 4 },
    { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_2[] = {
    { 0x12C, 56 }, { 85, 6 }, { 86, 6 }, { 87, 4 },
    { 88, 4 }, { 89, 4 }, { 90, 4 }, { 88, 4 },
    { 89, 4 }, { 90, 4 }, { 88, 4 }, { 89, 4 },
    { 90, 4 }, { 88, 4 }, { 89, 4 }, { 90, 4 },
    { 88, 4 }, { 89, 4 }, { 90, 4 }, { 255, 0x3E7 },
};
AnimFrame updateTileEffectFrames_1[] = {
    { 0x12C, 16 }, { 72, 4 }, { 73, 4 }, { 74, 4 },
    { 73, 4 }, { 75, 6 }, { 76, 6 }, { 77, 14 },
    { 78, 114 }, { 79, 6 }, { 80, 6 }, { 81, 6 },
    { 82, 6 }, { 83, 6 }, { 84, 8 }, { 255, 0x3E7 },
};
AnimFrame *updateTileEffectFrames[] = {
    updateTileEffectFrames_0, updateTileEffectFrames_1, updateTileEffectFrames_2,
};
s8 tileEffectSortY[] = {
    30, 30, 30, 0,
};
s8 tileEffectY[] = {
    0, -0x2F, -0x2F, 0,
};
s16 tileEffectPlaces[][2] = {
    { 0x140, 186 },
};
StageEffectSpot updateStageSpots[] = {
    { 28, 0, 108, 0x144 },
};
AnimFrame effectClutFrames[] = {
    { 0, 6 }, { 1, 6 }, { 2, 68 }, { 1, 4 },
    { 0, 4 }, { 255, 0 },
};
AnimFrame effectFrames[] = {
    { 0x12C, 18 }, { 1, 6 }, { 2, 6 }, { 3, 6 },
    { 4, 6 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
    { 5, 4 }, { 6, 4 }, { 7, 4 }, { 5, 4 },
    { 6, 4 }, { 7, 4 }, { 5, 4 }, { 6, 4 },
    { 7, 4 }, { 4, 4 }, { 3, 4 }, { 2, 4 },
    { 1, 4 }, { 255, 0x3E7 },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x176, 0x1B0, 0xD8, 0xB0, 0x160, 0x1F8 },
    { 0x140, 0x100, 0x164, 0x1C8, 0x90, 0xC8, 0x170, 0x1F8 },
    { 0x140, 0x100, 0x14A, 0x1B0, 0x28, 0xB0, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x194, 0x140, 0x150, 0x40, 0x160, 0x1F7 },
    { 0x180, 0x100, 0x19C, 0x148, 0x170, 0x48, 0x170, 0x1F7 },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { ITEM(0, 0xC), 0, FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Conditions[] = { ITEM(0, 0xC), 1, FLAG(0x1C, 2), 0, FLAG(0, 0), 1, CODES_END };
u16 actor0Talk2Actions[] = { START_EVENT(0x45), 1, FLAG(0x1C, 2), 1, CODES_END };
u16 actor0Talk3Conditions[] = { ITEM(0, 0xC), 1, FLAG(0x1C, 2), 1, FLAG(0, 0), 1, CODES_END };
u16 actor1Talk0Actions[] = { 0x7A27, 1, CODES_END };
u16 actor2Talk0Actions[] = { START_EVENT(0x19), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x30B },
    { actor0Talk1Conditions, NULL, 0x30C },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x30D },
    { actor0Talk3Conditions, NULL, 0x30E },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x16B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x168 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x210 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x211 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x20E },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x20E },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x312 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0xA), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0xD, 4, 129, 281, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x14, 5, 162, 473, 7 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x15, 6, 224, 441, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x23, 7, 306, 456, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9D, 8, 129, 281, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 0, 1, 0x50, 6, 0x46, 0, 0, 0, 0, 0, 320, 186, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 1, 0x60, 0x63, 6, 0, 324, 169, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 0xB, 8, 0, 78, 233, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 0xB, 8, 0, 93, 214, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 0xB, 8, 0, 109, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 0xB, 8, 0, 78, 221, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 0xB, 8, 0, 93, 225, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 0xB, 8, 0, 109, 205, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 82, 234, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 139, 230, 0, 0 },
    { 0, 3, 0x50, 4, 0x55, 0, 0, 0, 0, 0, 320, 139, 206, 0 },
    { 0, 2, 0x50, 4, 0x48, 0, 0, 0, 0, 0, 320, 139, 213, 0 },
    { 1, 0, 0x40, 4, 0x5C, 1, 0x5C, 0x5F, 6, 0, 324, 185, 209, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26C, 0xB8, 0xB4, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_LAUNCH_OUT, 0x26A, 0x3C0, 0x50, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 820, script820, EVENT_TEXT(0x19), NULL, endEvent820 },
    { 860, script860, EVENT_TEXT(0x1C), NULL, endEvent860 },
    { 883, script883, EVENT_TEXT(0x1D), NULL, endEvent883 },
    { 884, script884, EVENT_TEXT(0x1E), NULL, endEvent884 },
    { 886, script886, EVENT_TEXT(0x1F), NULL, endEvent886 },
    { 1245, script1245, EVENT_TEXT(8), NULL, endEvent1245 },
    { -1, NULL, 0, NULL, NULL },
};
