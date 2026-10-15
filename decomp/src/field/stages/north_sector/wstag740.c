/*
 * WSTAG740: Fire Dungeon, North Sector.
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

/* Back from the battle after event 800, starts event 801, with Fire Knight, unless it has played */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (FLAGS_00.checkCondition(FLAG(0x40, 0x3F), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0x40), 0)) {
            children[0] = FIELDSTG_startEvent(0x321);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#include "common/step_state_animation_once.inc.c"

#include "common/draw_pair_sprite.inc.c"

#include "common/update_sprite_pair.inc.c"

#include "common/create_sprite_pair.inc.c"

/*
 * After event 800, with Fire Knight: starts the stage's event battle 0 and
 * keeps it from playing again
 */
void endEvent800(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x3F), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 801, with Fire Knight: gives the player the Divine Rod and
 * keeps it from playing again
 */
void endEvent801(void) {
    FLAGS_00.applyAction(ITEM(2, 0x88), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0x40), 1);
}

/* After the trap of event 9000 goes off: keeps it from going off again */
void endEvent9000(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAA), 1);
}

/* After the trap of event 9001 goes off: keeps it from going off again */
void endEvent9001(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAB), 1);
}

/* After the trap of event 9002 goes off: keeps it from going off again */
void endEvent9002(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAC), 1);
}

/* After the trap of event 9003 goes off: keeps it from going off again */
void endEvent9003(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAD), 1);
}

/* After the trap of event 9004 goes off: keeps it from going off again */
void endEvent9004(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAE), 1);
}

/* After the trap of event 9005 goes off: keeps it from going off again */
void endEvent9005(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xAF), 1);
}

/* After the trap of event 9006 goes off: keeps it from going off again */
void endEvent9006(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB0), 1);
}

/* After the trap of event 9007 goes off: keeps it from going off again */
void endEvent9007(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB1), 1);
}

/* After the trap of event 9008 goes off: keeps it from going off again */
void endEvent9008(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB2), 1);
}

/* After the trap of event 9009 goes off: keeps it from going off again */
void endEvent9009(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB3), 1);
}

/* After the trap of event 9010 goes off: keeps it from going off again */
void endEvent9010(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB4), 1);
}

/* After the trap of event 9011 goes off: keeps it from going off again */
void endEvent9011(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB5), 1);
}

/* After the trap of event 9012 goes off: keeps it from going off again */
void endEvent9012(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB6), 1);
}

/* After the trap of event 9013 goes off: keeps it from going off again */
void endEvent9013(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB7), 1);
}

/* After the trap of event 9014 goes off: keeps it from going off again */
void endEvent9014(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB8), 1);
}

/* After the trap of event 9015 goes off: keeps it from going off again */
void endEvent9015(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xB9), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xDB)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x6CE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xD3)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x6DD
#endif
/* Sets the stage up: its map, actors, battles and events, playing BGM_0023 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x12200, 0x2F400};
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

s16 script800[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x215, 0x2AC, 3),
    SCRIPT_PLACE(0xC9, 0x200, 0x2A1),
    SCRIPT_POSE(0xC9, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xC9, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xC9, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC9, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script801[] = {
    SCRIPT_FOLLOW(1, 0xC9),
    SCRIPT_PLACE(2, 0x215, 0x2AC),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0xC9, 0x200, 0x2A1),
    SCRIPT_POSE(0xC9, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0xC9, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xC9, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC9, 0x1D0, 0x288, 3),
    SCRIPT_WAIT_WALK(0xC9),
    SCRIPT_POSE(0xC9, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x200, 0x2A1, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0xC9, 2),
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
AreaBattle area0Battle0 = { 119, 24, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 119, 24, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 119, 24, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 168, 24, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 168, 24, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 168, 24, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 111, 24, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 111, 24, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 0, 24, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 0, 24, MUSIC(2, 0) };
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
AreaBattle area3Battle0 = { 20, 24, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle5 = { 168, 24, MUSIC(2, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 0, 0, MUSIC(1, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 88, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x180, 0x158, 0x100, 0x58, 0x170, 0x1FC },
    { 0x180, 0x100, 0x1A6, 0x158, 0x198, 0x58, 0x140, 0x1FB },
};
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x208 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x209 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x207 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x206 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x206 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(0x40, 0x40), 0, SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0x40, 0x40), 1, SPECIAL(0x19), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x9D, 4, 464, 648, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0xC9, 5, 464, 648, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0xC9, 5, 464, 648, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0xC9, 5, 512, 673, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xC9, 5, 464, 648, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26A, 0x70, 0x2EC, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26C, 0x448, 0x288, 1, 0, 0, 0 },
    { { { FLAG(0x40, 0xAA), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2328, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xAB), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2329, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xAC), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232A, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xAD), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232B, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xAE), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232C, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xAF), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232D, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB0), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232E, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB1), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x232F, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB2), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2330, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB3), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2331, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB4), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2332, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB5), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2333, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB6), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2334, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB7), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2335, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB8), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2336, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xB9), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2337, 0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 0x11), 1 }, { FLAG(0x40, 0x3F), 0 } }, SLOT_EVENT, 0x320, 0, 0, 0, 0, 0, 0 },
    { { { FLAG(0, FIELD_FLAG_ENCOUNTERED), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2338, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 800, script800, EVENT_TEXT(0xC), NULL, endEvent800 },
    { 801, script801, EVENT_TEXT(0xD), NULL, endEvent801 },
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
    { 9016, NULL, 0, FIELDSTG_startEventBattle5, NULL },
    { -1, NULL, 0, NULL, NULL },
};
