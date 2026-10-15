/*
 * WSTAG676: Bai Hu Dome, Bai Hu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG675.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of story progress 0x22 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (GAME.progress == 0x22 && FLAGS_00.checkCondition(FLAG(0x40, 0x5A), 1)) {
            children[0] = FIELDSTG_startEvent(0x385);
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

/*
 * After event 900, with Bai Hu Chief: starts the stage's event battle 0 and
 * keeps it from playing again
 */
void endEvent900(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x5A), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 901, with the player: moves the story on to its step 0x23 and
 * gives the player the White ID Pass
 */
void endEvent901(void) {
    GAME.progress = 0x23;
    FLAGS_00.applyAction(ITEM(0, 0x18), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x652
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x662
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0021;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x14E00, 0x1E900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x17;
    FIELDSTG_state.music = MUSIC(0x17, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script900[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x160, 0xA9, 3),
    SCRIPT_PLACE(0xD2, 0x140, 0x99),
    SCRIPT_POSE(0xD2, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0xD2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0xD2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x2407,
#elif VERSION_EU
    0x3,
#elif VERSION_JP
    0x178,
#endif
};
s16 script901[] = {
    SCRIPT_PLACE(2, 0x160, 0xA9),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x13C, 0x140, 0x99),
    SCRIPT_POSE(0x13C, 1, 5),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x148, 0x9D, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x13C, 0, 0),
    SCRIPT_POSE(0x13C, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1A0, 0xC9, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x2C6, 0x46C, 0xAA, 1),
    SCRIPT_END,
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
    0,
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
AreaBattle area3Battle0 = { 25, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 307, 18, MUSIC(0x23, 0) };
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
    { 152, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B4, 0x128, 0x1D0, 0x28, 0x170, 0x1FE },
    { 0x180, 0x100, 0x19C, 0x128, 0x170, 0x28, 0x140, 0x1FD },
    { 0x180, 0x100, 0x1A4, 0x128, 0x190, 0x28, 0x150, 0x1FD },
    { 0x180, 0x100, 0x180, 0x150, 0x100, 0x50, 0x160, 0x1FD },
    { 0x180, 0x100, 0x188, 0x150, 0x120, 0x50, 0x170, 0x1FD },
    { 0x180, 0x100, 0x198, 0x158, 0x160, 0x58, 0x140, 0x1FC },
    { 0x180, 0x100, 0x194, 0x128, 0x150, 0x28, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1AC, 0x128, 0x1B0, 0x28, 0x160, 0x1FC },
    { 0x180, 0x100, 0x190, 0x150, 0x140, 0x50, 0x170, 0x1FC },
    { 0x140, 0x100, 0x148, 0x186, 0x20, 0x86, 0x140, 0x1FB },
    { 0x140, 0x100, 0x152, 0x186, 0x48, 0x86, 0x150, 0x1FB },
    { 0x140, 0x100, 0x16C, 0x1E8, 0xB0, 0xE8, 0x160, 0x1FB },
};
u16 actor3Talk0Conditions[] = { FLAG(0x40, 0x5A), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x40, 0x5A), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0x40, 0x5A), 0, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0x40, 0x5A), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0x40, 0x5A), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x40, 0x5A), 1, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0x40, 0x5A), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0x40, 0x5A), 1, CODES_END };
u16 actor9Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor9Talk1Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
u16 actor9Talk1Actions[] = { 0x7A49, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x157 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x158 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x159 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x1EE },
    { actor3Talk1Conditions, NULL, 0x1EF },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, NULL, 0x1F0 },
    { actor4Talk1Conditions, NULL, 0x1F1 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x1F2 },
    { actor5Talk1Conditions, NULL, 0x1F3 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x1F4 },
    { actor6Talk1Conditions, NULL, 0x1F5 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x15A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x156 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x20D },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x1A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x155 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x94), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x40, 0x5A), 0, PROGRESS(0x22), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x22), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x25, 4, 345, 470, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x26, 5, 296, 445, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x27, 6, 297, 470, 5 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x46, 7, 345, 470, 3 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x47, 8, 296, 445, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x48, 9, 297, 470, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x49, 0xA, 344, 446, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x6F, 0xB, 344, 446, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x91, 0xC, 320, 153, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xCE, 0xD, 509, 563, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xD2, 0xE, 320, 153, 7 };
FieldActorEntry actor11 = { actor11Conditions, NULL, 0x13C, 0xF, 0, 0, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x50, 2, 0x2F, 0, 0, 0, 0, 0, 718, 505, 0, 0 },
    { 1, 0, 0x50, 2, 0x2F, 0, 0, 0, 0, 0, 815, 456, 0, 0 },
    { 1, 0, 0x40, 2, 0x30, 2, 0, 1, 0xA, 0, 625, 376, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x35, 0xA, 0, 719, 451, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 1, 0x32, 0x35, 0xA, 0, 815, 343, 0, 0 },
    { 1, 0, 0x40, 2, 0x3E, 1, 0x3E, 0x47, 6, 0, 637, 147, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 1, 0x48, 0x51, 6, 0, 769, 352, 0, 0 },
    { 1, 0, 0x50, 6, 0x2E, 0, 0, 0, 0, 0, 182, 516, 0, 0 },
    { 1, 0, 0x40, 6, 0x30, 2, 0, 1, 0xA, 0, 157, 372, 0, 0 },
    { 1, 0, 0x40, 6, 0x30, 2, 0, 1, 0xA, 0, 213, 344, 0, 0 },
    { 1, 0, 0x40, 6, 0x30, 2, 0, 1, 0xA, 0, 221, 194, 0, 0 },
    { 1, 0, 0x40, 6, 0x30, 2, 0, 1, 0xA, 0, 549, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x30, 2, 0, 1, 0xA, 0, 742, 201, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x35, 0xA, 0, 182, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x36, 1, 0x36, 0x39, 8, 0, 91, 349, 0, 0 },
    { 1, 0, 0x40, 6, 0x52, 1, 0x52, 0x5B, 6, 0, 487, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 0xA, 0, 661, 151, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 0xA, 0, 780, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 1, 0x60, 0x63, 0xA, 0, 504, 449, 0, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 249, 99, 171, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 287, 425, 455, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 511, 525, 552, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C6, 0x46C, 0xAA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x2C8, 0xA8, 0xA4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x2C0, 0x100, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x2B0, 0x148, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x210, 0x118, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x200, 0x160, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1C0, 0x180, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1B0, 0x1C8, 0, 0, 0, 0 },
    { { { PROGRESS(0x22), 1 }, { FLAG(0x40, 0x5A), 0 } }, SLOT_EVENT, 0x384, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 900, script900, EVENT_TEXT(0x19), NULL, endEvent900 },
    { 901, script901, EVENT_TEXT(0x1A), NULL, endEvent901 },
    { -1, NULL, 0, NULL, NULL },
};
