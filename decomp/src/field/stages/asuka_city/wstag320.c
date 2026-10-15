/*
 * WSTAG320: Asuka Sewers, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

/* Hides the map objects with animation 10 from story progress 0x16 on */
void updateStage(StageTask *task) {
    StageTile *tile;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress >= 0x16) {
            for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
                if (tile->anim == 10) {
                    tile->visible = 0;
                }
            }
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#include "common/start_stage.inc.c"

/* After event 290, with Mischievous Mat: gives the player the Agumon Suit */
void endEvent290(void) {
    FLAGS_00.applyAction(ITEM(0, 0xF), 1);
}

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x295
#define STAGE_ARCHIVE 0x317
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x2A4
#define STAGE_ARCHIVE 0x326
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0003;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x26F00, 0x1EF00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 6;
    FIELDSTG_state.music = MUSIC(6, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x14 && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script290[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x181, 0xC1, 7),
    SCRIPT_PLACE(0x37, 0x1A1, 0xD1),
    SCRIPT_POSE(0x37, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 0),
    SCRIPT_POSE(2, 7, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x199, 0xB5, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x15D, 0x77, 3),
    SCRIPT_WAIT(6),
    SCRIPT_LEAVE(0x20B, 0x240, 0x140, 3),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 85, 7, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 85, 7, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 85, 7, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 85, 7, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 86, 7, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 86, 7, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 86, 7, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 86, 7, MUSIC(2, 0) };
BattleList area0Battles = {
    4,
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
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
    { 49, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B8, 0x100, 0x1E0, 0, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x194, 0x150, 0x150, 0x50, 0x140, 0x1F4 },
    { 0x180, 0x100, 0x19E, 0x130, 0x178, 0x30, 0x160, 0x1F4 },
    { 0x180, 0x100, 0x19C, 0x150, 0x170, 0x50, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x1A4, 0x170, 0x190, 0x70, 0x140, 0x1F3 },
};
u16 actor0Talk0Actions[] = { SPECIAL(0x8C), 1, FLAG(2, 0), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0x16), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0x15), 1, CODES_END };
u16 actor1Talk1Conditions[] = { ITEM(0, 0xF), 0, FLAG(0x1A, 0x16), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(0x21), 1, CODES_END };
u16 actor1Talk2Conditions[] = { ITEM(0, 0xF), 1, FLAG(0x1A, 0x16), 1, CODES_END };
u16 actor11Talk0Actions[] = { SPECIAL(0x13), 1, FLAG(2, 7), 1, ITEM(2, 0x5E), 1, CODES_END };
u16 actor14Talk0Conditions[] = { ITEM(0, 0x15), 0, CODES_END };
u16 actor14Talk0Actions[] = { ITEM(0, 0x15), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor14Talk1Conditions[] = { ITEM(0, 0x15), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x453 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x54 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x55 },
    { actor1Talk2Conditions, NULL, 0x455 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x455 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x456 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x45A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x458 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x45D },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x455 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x459 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x45B },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x457 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, actor11Talk0Actions, 0x44D },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x89 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x53 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x56 },
    { actor14Talk1Conditions, NULL, 0x57 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x45C },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { FLAG(2, 0), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0x14), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x15), 1, CODES_END };
u16 actor11Conditions[] = { FLAG(2, 7), 0, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0x14), 0, CODES_END };
u16 actor14Conditions[] = { PROGRESS(9), 1, FLAG(0x1A, 0x1B), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x21, 4, 465, 169, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x37, 5, 417, 209, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x4D, 6, 272, 217, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x61, 7, 417, 209, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x61, 7, 417, 209, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x61, 7, 417, 209, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9D, 8, 417, 209, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 395, 311, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 871, 533, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 1, 0x47, 0x4C, 4, 0, 1037, 703, 0, 0 },
    { 1, 0, 0x40, 2, 0x4D, 2, 0, 3, 4, 0, 1012, 671, 0, 0 },
    { 1, 0, 0x40, 2, 0x52, 2, 0, 3, 8, 0, 442, 116, 0, 0 },
    { 1, 0, 0x40, 2, 0x52, 2, 0, 3, 8, 0, 473, 132, 0, 0 },
    { 1, 0, 0x40, 2, 0x53, 2, 0, 3, 4, 0, 299, 39, 0, 0 },
    { 1, 0, 0x40, 2, 0x5A, 0, 0, 0, 0, 0, 472, 396, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 417, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 449, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 481, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 513, 0, 0 },
    { 1, 0, 0x40, 2, 0x5E, 2, 0, 5, 4, 0, 871, 544, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 1, 0x47, 0x4C, 4, 0, 735, 465, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 328, 658, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 616, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 760, 730, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 2, 0, 3, 6, 0, 904, 802, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 351, 643, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 639, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 783, 715, 0, 0 },
    { 1, 0, 0x40, 6, 0x4F, 2, 0, 3, 6, 0, 927, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 324, 632, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 612, 776, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 756, 704, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 2, 0, 3, 6, 0, 900, 776, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 305, 643, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 593, 788, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 737, 715, 0, 0 },
    { 1, 0, 0x40, 6, 0x50, 2, 0, 3, 6, 0, 881, 787, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 202, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 210, 420, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 218, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 226, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 234, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 242, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 250, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 258, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 266, 352, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 274, 356, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 282, 360, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 290, 364, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 298, 368, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 306, 372, 0, 0 },
    { 1, 0, 0x40, 6, 0x54, 2, 0, 3, 8, 0, 314, 376, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 3, 8, 0, 322, 380, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 270, 444, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 278, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 286, 436, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 294, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 302, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 310, 424, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 358, 400, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 366, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 374, 392, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 382, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x56, 2, 0, 3, 8, 0, 390, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 3, 8, 0, 349, 405, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 3, 4, 0, 439, 395, 0, 0 },
    { 1, 0, 0x40, 6, 0x59, 2, 0, 3, 4, 0, 540, 398, 0, 0 },
    { 1, 0, 0x40, 6, 0x5B, 2, 0, 3, 4, 0, 612, 416, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 646, 530, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 688, 509, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 790, 602, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 790, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 832, 581, 0, 0 },
    { 1, 0, 0x40, 6, 0x5F, 2, 0, 1, 4, 0, 832, 725, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 358, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 400, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 519, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 535, 458, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 560, 733, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 2, 0, 1, 4, 0, 576, 437, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 648, 724, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 690, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 792, 653, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 833, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 936, 725, 0, 0 },
    { 1, 0, 0x40, 6, 0x61, 2, 0, 1, 4, 0, 978, 746, 0, 0 },
    { 1, 0, 0x40, 6, 0x62, 2, 0, 1, 4, 0, 654, 640, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 102, 448, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 159, 484, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 174, 374, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 273, 382, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 390, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 128, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 177, 460, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 209, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 94, 418, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 194, 530, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 98, 495, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 546, 532, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 366, 486, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 251, 381, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 70, 467, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 76, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 129, 505, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 179, 534, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 186, 472, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 187, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 263, 395, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 352, 473, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 393, 415, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 553, 516, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 956, 689, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 966, 668, 0, 0 },
    { 1, 0xA, 0xA0, 6, 0, 0, 0, 0, 0, 0, 792, 381, 0, 0 },
    { 1, 0, 0x60, 6, 1, 0, 0, 0, 0, 0, 721, 691, 0, 0 },
    { 1, 0, 0x60, 6, 1, 0, 0, 0, 0, 0, 865, 763, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 369, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 465, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 2, 0, 5, 4, 0, 775, 497, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 6, 0, 546, 665, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x20, 1, 0x20, 0x22, 6, 0, 833, 798, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2A, 1, 0x2A, 0x2D, 6, 0, 545, 558, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2A, 1, 0x2A, 0x2D, 6, 0, 834, 701, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, -12, 814, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 0, 364, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 100, 758, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 212, 702, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x23, 1, 0x23, 0x25, 6, 0, 324, 646, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, -14, 713, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 99, 658, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 210, 602, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x2E, 1, 0x2E, 0x31, 0xA, 0, 322, 546, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x26, 1, 0x26, 0x29, 6, 0, 11, 408, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20B, 0x242, 0x13E, 3, 0, 0, 0 },
    { { { SPECIAL(0x3F), 1 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21A, 0x68, 0x304, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x21C, 0x88, 0x1B4, 5, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 2, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x130, 0xF8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x120, 0x140, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 2, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 290, script290, EVENT_TEXT(2), NULL, endEvent290 },
    { -1, NULL, 0, NULL, NULL },
};
