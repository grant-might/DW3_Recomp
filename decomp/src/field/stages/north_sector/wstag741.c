/*
 * WSTAG741: Fire Dungeon, North Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG740.
 */

#include "common.h"
#include "field/stage.h"
const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };

/* Defined below, after the code that uses them */
extern s32 partyActorIds[];
void *createSpritePair(s32 x, s32 y);
extern AnimFrame updateSpritePairFrames_0[];
extern AnimFrame updateSpritePairFrames_1[];
extern AnimFrame *updateSpritePairFrames[];

#include "common/update_event9015.inc.c"

#include "common/start_events9000_to9015.inc.c"

/*
 * Back from the battle after event 1295, starts event 1296, with Fire
 * Master, unless it has played
 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x84), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x85), 0)) {
            children[1] = FIELDSTG_startEvent(0x510);
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

#include "common/step_state_animation_once.inc.c"

#include "common/draw_pair_sprite.inc.c"

#include "common/update_sprite_pair.inc.c"

#include "common/create_sprite_pair.inc.c"

/*
 * After event 1295, with Fire Master: starts the stage's event battle 0 and
 * keeps it from playing again
 */
void endEvent1295(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x84), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 1296, with Fire Master: gives the player the Mind Chip and
 * keeps it from playing again
 */
void endEvent1296(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x85), 1);
    FLAGS_00.applyAction(ITEM(1, 0x33), 1);
}

/* After the trap of event 9000 goes off: keeps it from going off again */
void endEvent9000(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBA), 1);
}

/* After the trap of event 9001 goes off: keeps it from going off again */
void endEvent9001(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBB), 1);
}

/* After the trap of event 9002 goes off: keeps it from going off again */
void endEvent9002(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBC), 1);
}

/* After the trap of event 9003 goes off: keeps it from going off again */
void endEvent9003(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBD), 1);
}

/* After the trap of event 9004 goes off: keeps it from going off again */
void endEvent9004(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBE), 1);
}

/* After the trap of event 9005 goes off: keeps it from going off again */
void endEvent9005(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xBF), 1);
}

/* After the trap of event 9006 goes off: keeps it from going off again */
void endEvent9006(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC0), 1);
}

/* After the trap of event 9007 goes off: keeps it from going off again */
void endEvent9007(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC1), 1);
}

/* After the trap of event 9008 goes off: keeps it from going off again */
void endEvent9008(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC2), 1);
}

/* After the trap of event 9009 goes off: keeps it from going off again */
void endEvent9009(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC3), 1);
}

/* After the trap of event 9010 goes off: keeps it from going off again */
void endEvent9010(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC4), 1);
}

/* After the trap of event 9011 goes off: keeps it from going off again */
void endEvent9011(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC5), 1);
}

/* After the trap of event 9012 goes off: keeps it from going off again */
void endEvent9012(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC6), 1);
}

/* After the trap of event 9013 goes off: keeps it from going off again */
void endEvent9013(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC7), 1);
}

/* After the trap of event 9014 goes off: keeps it from going off again */
void endEvent9014(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC8), 1);
}

/* After the trap of event 9015 goes off: keeps it from going off again */
void endEvent9015(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xC9), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xFE)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x741
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xF6)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x751
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0023 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1CD00, 0x29A00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x19;
    FIELDSTG_state.music = MUSIC(0x19, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1295[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x215, 0x2AC, 3),
    SCRIPT_PLACE(0xCC, 0x200, 0x2A1),
    SCRIPT_POSE(0xCC, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xCC, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xCC, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1296[] = {
    SCRIPT_FOLLOW(1, 0xCC),
    SCRIPT_PLACE(2, 0x215, 0x2AC),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0xCC, 0x200, 0x2A1),
    SCRIPT_POSE(0xCC, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0xCC, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_POSE(0xCC, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xCC, 0x1D0, 0x288, 3),
    SCRIPT_WAIT_WALK(0xCC),
    SCRIPT_POSE(0xCC, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x200, 0x2A1, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xCC, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
/* The actor ids of the party's three partners, as FIELDSTG's partner lists */
s32 partyActorIds[] = {
    2, 4, 8,
};
AnimFrame updateSpritePairFrames_0[] = {
    { 50, 4 }, { 51, 4 }, { 52, 4 }, { 53, 4 },
    { 54, 4 }, { 55, 4 }, { 255, 0x3E7 },
};
AnimFrame updateSpritePairFrames_1[] = {
    { 0x12C, 4 }, { 56, 4 }, { 57, 4 }, { 58, 4 },
    { 59, 4 }, { 60, 4 }, { 61, 4 }, { 62, 4 },
    { 63, 4 }, { 64, 4 }, { 65, 4 }, { 66, 4 },
    { 67, 4 }, { 255, 0x3E7 },
};
AnimFrame *updateSpritePairFrames[] = {
    updateSpritePairFrames_0, updateSpritePairFrames_1,
};
AreaBattle area0Battle0 = { 136, 24, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 136, 24, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 136, 24, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 183, 24, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 183, 24, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 183, 24, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 118, 24, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 118, 24, MUSIC(2, 0) };
BattleList area0Battles = {
    5,
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
AreaBattle area3Battle0 = { 28, 24, MUSIC(0x23, 0) };
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
    { 121, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x180, 0x158, 0x100, 0x58, 0x140, 0x1FB },
    { 0x180, 0x100, 0x1A6, 0x158, 0x198, 0x58, 0x150, 0x1FB },
};
u16 actor2Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor2Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Talk0Conditions[] = { SPECIAL(0x1D), 1, CODES_END };
u16 actor3Talk1Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor3Talk2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Talk3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x214 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x214 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x211 },
    { actor2Talk1Conditions, NULL, 0x212 },
    { actor2Talk2Conditions, NULL, 0x213 },
    { actor2Talk3Conditions, NULL, 0x206 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x211 },
    { actor3Talk1Conditions, NULL, 0x212 },
    { actor3Talk2Conditions, NULL, 0x213 },
    { actor3Talk3Conditions, NULL, 0x215 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(0x40, 0x85), 0, SPECIAL(0x1A), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x1A), 1, FLAG(0x40, 0x85), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(8), 1, FLAG(0x40, 0x85), 0, SPECIAL(0x1A), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(8), 1, FLAG(0x40, 0x85), 1, SPECIAL(0x1A), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x9D, 4, 512, 673, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x9D, 4, 464, 648, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xCC, 5, 512, 673, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xCC, 5, 464, 648, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 6, 0, 224, 601, 638, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 5, 0, 253, 334, 371, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 5, 0, 465, 242, 274, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 6, 0, 607, 402, 441, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 4, 0, 647, 343, 371, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 4, 0, 699, 740, 781, 0 },
    { 1, 0, 0x40, 4, 0x50, 1, 0x50, 0x63, 4, 0, 760, 579, 615, 0 },
    { 1, 0, 0x40, 4, 0xA, 1, 0xA, 0x1D, 5, 0, 213, 576, 638, 0 },
    { 1, 0, 0x40, 4, 0xA, 1, 0xA, 0x1D, 6, 0, 267, 297, 371, 0 },
    { 1, 0, 0x40, 4, 0xA, 1, 0xA, 0x1D, 4, 0, 472, 403, 466, 0 },
    { 1, 0, 0x40, 4, 0xA, 1, 0xA, 0x1D, 5, 0, 616, 385, 441, 0 },
    { 1, 0, 0x40, 4, 0xA, 1, 0xA, 0x1D, 6, 0, 855, 692, 756, 0 },
    { 1, 0, 0x40, 4, 0x1E, 1, 0x1E, 0x31, 5, 0, 331, 427, 493, 0 },
    { 1, 0, 0x40, 4, 0x1E, 1, 0x1E, 0x31, 6, 0, 465, 211, 274, 0 },
    { 1, 0, 0x40, 4, 0x1E, 1, 0x1E, 0x31, 4, 0, 698, 429, 493, 0 },
    { 1, 0, 0x40, 4, 0x1E, 1, 0x1E, 0x31, 5, 0, 699, 715, 781, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2D2, 0x70, 0x2EC, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2D4, 0x448, 0x288, 1, 0, 0, 0 },
    { { { FLAG(0x40, 0xBA), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xBB), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2329, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xBC), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232A, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xBD), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232B, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xBE), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232C, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xBF), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232D, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC0), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232E, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC1), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232F, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC2), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2330, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC3), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2331, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC4), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2332, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC5), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2333, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC6), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2334, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC7), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2335, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC8), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2336, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xC9), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2337, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0x84), 0 }, { SPECIAL(0x1A), 0 } }, SLOT_EVENT, 0x50F, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1295, script1295, EVENT_TEXT(0x15), NULL, endEvent1295 },
    { 1296, script1296, EVENT_TEXT(0x16), NULL, endEvent1296 },
    { 9000, NULL, 0, startEvents9000To9015, endEvent9000 },
    { 9001, NULL, 0, startEvents9000To9015, endEvent9001 },
    { 9002, NULL, 0, startEvents9000To9015, endEvent9002 },
    { 9003, NULL, 0, startEvents9000To9015, endEvent9003 },
    { 9004, NULL, 0, startEvents9000To9015, endEvent9004 },
    { 9005, NULL, 0, startEvents9000To9015, endEvent9005 },
    { 9006, NULL, 0, startEvents9000To9015, endEvent9006 },
    { 9007, NULL, 0, startEvents9000To9015, endEvent9007 },
    { 9008, NULL, 0, startEvents9000To9015, endEvent9008 },
    { 9009, NULL, 0, startEvents9000To9015, endEvent9009 },
    { 9010, NULL, 0, startEvents9000To9015, endEvent9010 },
    { 9011, NULL, 0, startEvents9000To9015, endEvent9011 },
    { 9012, NULL, 0, startEvents9000To9015, endEvent9012 },
    { 9013, NULL, 0, startEvents9000To9015, endEvent9013 },
    { 9014, NULL, 0, startEvents9000To9015, endEvent9014 },
    { 9015, NULL, 0, startEvents9000To9015, endEvent9015 },
    { -1, NULL, 0, NULL, NULL },
};
