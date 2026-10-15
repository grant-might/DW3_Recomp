/*
 * WSTAG421: Qing Long City, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG420.
 */

#include "common.h"
#include "field/stage.h"

/*
 * Cycles the frames of the map objects with animation 1 if flag
 * 0x1A0A, and creates the event object of story progress 0x1B
 */
void updateStage(StageTask *task, void **children) {
    StageTile *tile;
    s32 set;

    switch (task->state) {
    case TASK_INIT:
    default:
        set = FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 1) != 0;
        for (tile = FIELDSTG_state.objects; tile->margin != 0; tile++) {
            if (tile->anim == 1) {
                tile->cycle = set;
            }
        }
        if (GAME.progress == 0x1B) {
            if (FLAGS_00.checkCondition(FLAG(0x40, 0xA3), 1) && FLAGS_00.checkCondition(FLAG(0x40, 0xA7), 0)) {
                children[0] = FIELDSTG_startEvent(0x2E5);
            } else if (FLAGS_00.checkCondition(FLAG(0x40, 0xA7), 1)) {
                children[0] = FIELDSTG_startEvent(0x2E7);
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

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/*
 * After event 740, with Qing Long Chief: starts the stage's event battle 0
 * and keeps it from playing again
 */
void endEvent740(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xA3), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

/*
 * After event 741, with the player: gives the player the Blue ID Pass and
 * keeps it from playing again
 */
void endEvent741(void) {
    FLAGS_00.applyAction(ITEM(0, 0x16), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0xA7), 1);
}

/* After event 743, with the player: moves the story on to its step 0x1C */
void endEvent743(void) {
    GAME.progress = 0x1C;
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x778
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x787
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing SEIR_BGM;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1D100, 0x1F600};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xC;
    FIELDSTG_state.music = MUSIC(0xC, 0);
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

s16 script740[] = {
    SCRIPT_WALK(2, 0x180, 0xDA, 1),
    SCRIPT_PLACE(0xD2, 0x160, 0xEA),
    SCRIPT_POSE(0xD2, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xD2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xD2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xD2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script741[] = {
    SCRIPT_PLACE(2, 0x180, 0xDA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_PLACE(0x13C, 0x160, 0xEA),
    SCRIPT_POSE(0x13C, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x168, 0xE6, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_PLACE(0x13C, 0, 0),
    SCRIPT_POSE(0x13C, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 0),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x2D7, 1, 1, 0),
    SCRIPT_END,
};
s16 script743[] = {
    SCRIPT_PLACE(2, 0x180, 0xDA),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1E8, 0xA4, 5),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x29E, 0xC8, 0x1A4, 5),
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
AreaBattle area3Battle0 = { 14, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 305, 18, MUSIC(0x23, 0) };
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
    { 150, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x180, 0x1BB, 0x100, 0xBB, 0x140, 0x1FF },
    { 0x180, 0x100, 0x188, 0x1BB, 0x120, 0xBB, 0x150, 0x1FF },
    { 0x1C0, 0x100, 0x1DE, 0x100, 0x278, 0, 0x160, 0x1FF },
    { 0x180, 0x100, 0x1AA, 0x1BB, 0x1A8, 0xBB, 0x170, 0x1FF },
    { 0x180, 0x100, 0x1B2, 0x1BB, 0x1C8, 0xBB, 0x140, 0x1FE },
    { 0x180, 0x100, 0x190, 0x1CF, 0x140, 0xCF, 0x150, 0x1FE },
    { 0x180, 0x100, 0x198, 0x1CF, 0x160, 0xCF, 0x160, 0x1FE },
    { 0x1C0, 0x100, 0x1E6, 0x100, 0x298, 0, 0x170, 0x1FE },
    { 0x1C0, 0x100, 0x1EE, 0x100, 0x2B8, 0, 0x140, 0x1FD },
    { 0x1C0, 0x100, 0x1C0, 0x100, 0x200, 0, 0x150, 0x1FD },
    { 0x1C0, 0x100, 0x1C6, 0x100, 0x218, 0, 0x160, 0x1FD },
    { 0x1C0, 0x100, 0x1CE, 0x100, 0x238, 0, 0x170, 0x1FD },
    { 0x1C0, 0x100, 0x1D6, 0x100, 0x258, 0, 0x140, 0x1FC },
    { 0x1C0, 0x100, 0x1F6, 0x100, 0x2D8, 0, 0x150, 0x1FC },
    { 0x1C0, 0x100, 0x1DE, 0x120, 0x278, 0x20, 0x160, 0x1FC },
    { 0x1C0, 0x100, 0x1E6, 0x120, 0x298, 0x20, 0x170, 0x1FC },
    { 0x1C0, 0x100, 0x1EE, 0x120, 0x2B8, 0x20, 0x140, 0x1FB },
    { 0x1C0, 0x100, 0x1F6, 0x120, 0x2D8, 0x20, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1A0, 0x1AC, 0x180, 0xAC, 0x160, 0x1FB },
    { 0x180, 0x100, 0x1B0, 0x12E, 0x1C0, 0x2E, 0x170, 0x1FB },
};
u16 actor11Talk0Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor11Talk1Conditions[] = { SPECIAL(0x1F), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x10D },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x10F },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x108 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x115 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x111 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x114 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x116 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1CF },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x1D0 },
    { actor11Talk1Conditions, NULL, 0x1D1 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x104 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x10E },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x110 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x10A },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x107 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x106 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x10C },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x109 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x103 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor16Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor17Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor21Conditions[] = { FLAG(0x40, 0xA3), 0, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x25, 4, 449, 481, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x26, 5, 833, 257, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2E, 6, 632, 406, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x31, 7, 914, 344, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x32, 8, 449, 481, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x34, 9, 833, 257, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x38, 0xA, 882, 585, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x39, 0xB, 882, 585, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x3A, 0xC, 914, 344, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x66, 0xD, 553, 461, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x73, 0xE, 449, 481, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x74, 0xF, 833, 257, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x8F, 0x10, 352, 234, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x9D, 0x11, 882, 585, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x9D, 0x11, 449, 481, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x9E, 0x12, 833, 257, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9E, 0x12, 632, 406, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9F, 0x13, 914, 344, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9F, 0x13, 882, 585, 3 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0xA0, 0x14, 632, 406, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0xA1, 0x15, 914, 344, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0xD2, 0x16, 352, 234, 1 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x13C, 0x17, 0, 0, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 1, 0x4A, 2, 0, 1, 0, 3, 6, 0, 441, 290, 0, 0 },
    { 1, 1, 0x4A, 2, 0, 1, 0, 3, 6, 0, 817, 114, 0, 0 },
    { 1, 1, 0x4A, 2, 0, 1, 0, 3, 6, 0, 949, 528, 0, 0 },
    { 1, 1, 0x40, 2, 4, 1, 4, 7, 4, 0, 366, 274, 0, 0 },
    { 1, 1, 0x40, 2, 4, 1, 4, 7, 4, 0, 446, 241, 0, 0 },
    { 1, 1, 0x40, 2, 4, 1, 4, 7, 4, 0, 1129, 333, 0, 0 },
    { 1, 0, 0x80, 2, 0x24, 0, 0, 0, 0, 0, 469, 191, 0, 0 },
    { 1, 0, 0x80, 2, 0x25, 0, 0, 0, 0, 0, 256, 161, 0, 0 },
    { 1, 1, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 411, 424, 0, 0 },
    { 1, 1, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 538, 379, 0, 0 },
    { 1, 1, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 568, 341, 0, 0 },
    { 1, 1, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 607, 347, 0, 0 },
    { 1, 1, 0x40, 6, 8, 1, 8, 0xB, 4, 0, 976, 215, 0, 0 },
    { 1, 1, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 544, 363, 0, 0 },
    { 1, 1, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 579, 380, 0, 0 },
    { 1, 1, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 884, 243, 0, 0 },
    { 1, 1, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 913, 244, 0, 0 },
    { 1, 1, 0x40, 6, 0xC, 1, 0xC, 0xF, 4, 0, 1020, 218, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x1B, 0, 0, 0, 0, 0, 721, 200, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x1C, 0, 0, 0, 0, 0, 451, 367, 0, 0 },
    { 1, 0, 0x78, 6, 0x26, 0, 0, 0, 0, 0, 721, 200, 0, 0 },
    { 1, 0, 0x80, 6, 0x27, 0, 0, 0, 0, 0, 451, 367, 0, 0 },
    { 1, 1, 0x40, 4, 8, 1, 8, 0xB, 4, 0, 448, 405, 445, 0 },
    { 1, 1, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 465, 438, 464, 0 },
    { 1, 1, 0x40, 4, 0xC, 1, 0xC, 0xF, 4, 0, 737, 256, 281, 0 },
    { 1, 0, 0x64, 4, 0x1D, 0, 0, 0, 0, 0, 784, 496, 560, 0 },
    { 1, 0, 0x64, 4, 0x1E, 0, 0, 0, 0, 0, 768, 488, 552, 0 },
    { 1, 0, 0x64, 4, 0x1F, 0, 0, 0, 0, 0, 752, 480, 544, 0 },
    { 1, 0, 0x64, 4, 0x20, 0, 0, 0, 0, 0, 736, 472, 536, 0 },
    { 1, 0, 0x64, 4, 0x21, 0, 0, 0, 0, 0, 720, 464, 528, 0 },
    { 1, 0, 0x64, 4, 0x22, 0, 0, 0, 0, 0, 704, 456, 520, 0 },
    { 1, 0, 0x64, 4, 0x23, 0, 0, 0, 0, 0, 672, 456, 496, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 898, 552, 576, 0 },
    { 1, 0, 0x40, 4, 0x19, 0, 0, 0, 0, 0, 727, 258, 280, 0 },
    { 1, 0, 0x40, 4, 0x1A, 0, 0, 0, 0, 0, 447, 408, 444, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x298, 0x448, 0xF8, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29E, 0xC8, 0x1A4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29E, 0x1B8, 0x204, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29D, 0x208, 0x1AC, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x29F, 0x1A7, 0x254, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 6, 0x400, 0x120, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 6, 0x410, 0x188, 0, 0, 0, 0 },
    { { { FLAG(0x40, 0xA3), 0 }, { CODES_END, 0 } }, SLOT_EVENT, 0x2E4, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 740, script740, EVENT_TEXT(0x29), NULL, endEvent740 },
    { 741, script741, EVENT_TEXT(0x2A), NULL, endEvent741 },
    { 743, script743, EVENT_TEXT(0x2B), NULL, endEvent743 },
    { -1, NULL, 0, NULL, NULL },
};
