/*
 * WSTAG675: Byakko Dome, Byakko City.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of story progress 0xF */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (GAME.progress == 0xF && FLAGS_00.checkCondition(FLAG(0x40, 0x1E), 1)) {
            children[0] = FIELDSTG_startEvent(0x19B);
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

/* After event 390: keeps it from playing again */
void endEvent390(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x17), 1);
}

/* After event 400: keeps it from playing again */
void endEvent400(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x1A), 1);
}

/*
 * After event 410, with Byakko Leader: lets event 415 of WSTAG660 start,
 * starts the stage's event battle 0 and keeps it from playing again
 */
void endEvent410(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x1E), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 0x26), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x4A0
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x4B0
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing the
 * ambience ENV_0021; BGM_0029 instead at story steps 0x27 to 0x28 and 0xF to
 * 0x17
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x17000, 0x1E000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x3D;
    FIELDSTG_state.music = MUSIC(0x3D, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0xF && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script390[] = {
    SCRIPT_WALK(2, 0x1E9, 0x16D, 5),
    SCRIPT_PLACE(0x133, 0x1F9, 0x165),
    SCRIPT_POSE(0x133, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x133, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x133, 0x219, 0x175, 7),
    SCRIPT_WAIT_WALK(0x133),
    SCRIPT_POSE(0x133, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0xA622,
#elif VERSION_EU
    0x1F9,
#elif VERSION_JP
    0x301,
#endif
};
s16 script400[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x27F, 0x141, 7),
    SCRIPT_PLACE(0x4A, 0x29F, 0x151),
    SCRIPT_POSE(0x4A, 1, 3),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x4A, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x4A, 0x2BF, 0x161, 7),
    SCRIPT_WAIT_WALK(0x4A),
    SCRIPT_POSE(0x4A, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script410[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x181, 0xBA, 3),
    SCRIPT_PLACE(0x98, 0x161, 0xAA),
    SCRIPT_POSE(0x98, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x98, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x98, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script411[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_PLACE(2, 0x181, 0xBA),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x98, 0x161, 0xAA),
    SCRIPT_POSE(0x98, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x98, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x98, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x98, 0x181, 0x9A, 1),
    SCRIPT_WAIT_WALK(0x98),
    SCRIPT_POSE(0x98, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x141, 0x9A, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x101, 0x7A, 3),
    SCRIPT_LEAVE(0x25F, 0x64, 0x64, 0),
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
AreaBattle area3Battle0 = { 5, 18, MUSIC(0x23, 0) };
AreaBattle area3Battle1 = { 303, 18, MUSIC(0x23, 0) };
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
    { 139, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x19C, 0x128, 0x170, 0x28, 0x160, 0x1FE },
    { 0x180, 0x100, 0x18A, 0x128, 0x128, 0x28, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1A4, 0x128, 0x190, 0x28, 0x140, 0x1FD },
    { 0x180, 0x100, 0x1AC, 0x128, 0x1B0, 0x28, 0x150, 0x1FD },
    { 0x180, 0x100, 0x1B4, 0x128, 0x1D0, 0x28, 0x160, 0x1FD },
    { 0x180, 0x100, 0x192, 0x148, 0x148, 0x48, 0x170, 0x1FD },
    { 0x180, 0x100, 0x180, 0x150, 0x100, 0x50, 0x140, 0x1FC },
    { 0x180, 0x100, 0x19A, 0x150, 0x168, 0x50, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1AA, 0x150, 0x1A8, 0x50, 0x170, 0x1FC },
    { 0x180, 0x100, 0x1B2, 0x150, 0x1C8, 0x50, 0x140, 0x1FB },
    { 0x180, 0x100, 0x188, 0x158, 0x120, 0x58, 0x150, 0x1FB },
    { 0x180, 0x100, 0x190, 0x170, 0x140, 0x70, 0x160, 0x1FB },
    { 0x140, 0x100, 0x158, 0x186, 0x60, 0x86, 0x170, 0x1FB },
    { 0x180, 0x100, 0x1A0, 0x178, 0x180, 0x78, 0x140, 0x1FA },
    { 0x180, 0x100, 0x1A8, 0x178, 0x1A0, 0x78, 0x150, 0x1FA },
    { 0x180, 0x100, 0x188, 0x180, 0x120, 0x80, 0x160, 0x1FA },
    { 0x180, 0x100, 0x1B0, 0x180, 0x1C0, 0x80, 0x170, 0x1FA },
    { 0x140, 0x100, 0x16E, 0x1A0, 0xB8, 0xA0, 0x140, 0x1F9 },
    { 0x180, 0x100, 0x180, 0x178, 0x100, 0x78, 0x150, 0x1F9 },
    { 0x180, 0x100, 0x198, 0x178, 0x160, 0x78, 0x160, 0x1F9 },
};
u16 actor32Talk0Conditions[] = {
    SPECIAL(0x20), 1,
    PROGRESS(0x20), 0,
    PROGRESS(0x21), 0,
    FLAG(0, 5), 0,
    CODES_END,
};
u16 actor32Talk0Actions[] = { FLAG(0, 5), 1, CODES_END };
u16 actor32Talk1Conditions[] = {
    SPECIAL(0x20), 1,
    PROGRESS(0x20), 0,
    PROGRESS(0x21), 0,
    FLAG(0, 5), 1,
    CODES_END,
};
u16 actor32Talk2Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor32Talk3Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor32Talk4Conditions[] = { SPECIAL(0x19), 1, SPECIAL(0x20), 0, CODES_END };
u16 actor39Talk0Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor39Talk0Actions[] = { 0x7A3F, 1, CODES_END };
u16 actor39Talk1Conditions[] = { SPECIAL(0x20), 1, CODES_END };
u16 actor39Talk1Actions[] = { 0x7A40, 1, CODES_END };
u16 actor39Talk2Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor39Talk2Actions[] = { 0x7A40, 1, CODES_END };
u16 actor39Talk3Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor39Talk3Actions[] = { 0x7A41, 1, CODES_END };
u16 actor39Talk4Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Talk4Actions[] = { 0x7A41, 1, CODES_END };
u16 actor39Talk5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor39Talk5Actions[] = { 0x7A41, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0xCF },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0xC5 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xC0 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xC1 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xC6 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xD0 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xBD },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xD1 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xC7 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xC2 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xBE },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xD2 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0xC3 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0xC8 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x157 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x158 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0xB7 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x15A },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x156 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x156 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x16E },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0xBA },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0xCE },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0xC4 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { actor32Talk0Conditions, actor32Talk0Actions, 0xBF },
    { actor32Talk1Conditions, NULL, 7 },
    { actor32Talk2Conditions, NULL, 0xBF },
    { actor32Talk3Conditions, NULL, 0xBF },
    { actor32Talk4Conditions, NULL, 0xBF },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0xC9 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0xCB },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0xCC },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0xCD },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0xCA },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { actor39Talk0Conditions, actor39Talk0Actions, 1 },
    { actor39Talk1Conditions, actor39Talk1Actions, 1 },
    { actor39Talk2Conditions, actor39Talk2Actions, 1 },
    { actor39Talk3Conditions, actor39Talk3Actions, 1 },
    { actor39Talk4Conditions, actor39Talk4Actions, 1 },
    { actor39Talk5Conditions, actor39Talk5Actions, 1 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x174 },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x15B },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x159 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x159 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xF), 1, FLAG(0x40, 0x1A), 0, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0xF), 1, FLAG(0x40, 0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor36Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(0xF), 1, FLAG(0x40, 0x17), 0, CODES_END };
u16 actor43Conditions[] = { PROGRESS(0xF), 1, FLAG(0x40, 0x17), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 296, 445, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 344, 446, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x20, 4, 296, 445, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x20, 4, 296, 445, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x25, 5, 345, 470, 3 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x25, 5, 345, 470, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x25, 5, 345, 470, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x25, 5, 345, 470, 3 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2F, 6, 297, 470, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2F, 6, 384, 152, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2F, 6, 297, 470, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2F, 6, 297, 470, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x30, 7, 344, 446, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x30, 7, 296, 445, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x30, 7, 344, 446, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x30, 7, 344, 446, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x45, 8, 345, 470, 3 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x46, 9, 297, 470, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x47, 0xA, 296, 445, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x48, 0xB, 344, 446, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x4A, 0xC, 671, 337, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x4A, 0xC, 703, 353, 3 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x66, 0xD, 408, 484, 3 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x7A, 0xE, 320, 154, 7 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x7A, 0xE, 320, 154, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x7A, 0xE, 320, 154, 7 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x7A, 0xE, 320, 154, 7 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x98, 0xF, 353, 170, 7 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x9D, 0x10, 320, 154, 7 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x9E, 0x11, 345, 470, 3 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x9F, 0x12, 297, 470, 5 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0xA0, 0x13, 344, 446, 1 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0xA1, 0x14, 296, 445, 7 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0xCE, 0x15, 509, 563, 3 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0xCE, 0x15, 509, 563, 3 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x132, 0x16, 544, 504, 1 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x133, 0x17, 505, 357, 1 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x133, 0x17, 537, 373, 3 };
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
    { 1, 0, 0x40, 6, 0x3A, 1, 0x3A, 0x3D, 8, 0, 447, 275, 0, 0 },
    { 1, 0, 0x40, 6, 0x52, 1, 0x52, 0x5B, 6, 0, 487, 434, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 0xA, 0, 661, 151, 0, 0 },
    { 1, 0, 0x40, 6, 0x5C, 1, 0x5C, 0x5F, 0xA, 0, 780, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x60, 1, 0x60, 0x63, 0xA, 0, 504, 449, 0, 0 },
    { 1, 0, 0x40, 6, 3, 0, 0, 0, 0, 0, 285, 98, 0, 0 },
    { 1, 0, 0x58, 4, 0, 0, 0, 0, 0, 0, 249, 99, 171, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 287, 425, 455, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 511, 525, 552, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_DROP, 5, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25D, 0x46C, 0xAA, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x260, 0xA8, 0xA4, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x2C0, 0x100, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x2B0, 0x148, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x210, 0x118, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x200, 0x160, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x1C0, 0x180, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x1B0, 0x1C8, 0, 0, 0, 0 },
    { { { PROGRESS(0xF), 1 }, { FLAG(0x40, 0x17), 0 } }, SLOT_EVENT, 0x186, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xF), 1 }, { FLAG(0x40, 0x1A), 0 } }, SLOT_EVENT, 0x190, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xF), 1 }, { FLAG(0x40, 0x1E), 0 } }, SLOT_EVENT, 0x19A, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 390, script390, EVENT_TEXT(0xC), NULL, endEvent390 },
    { 400, script400, EVENT_TEXT(0xD), NULL, endEvent400 },
    { 410, script410, EVENT_TEXT(0), NULL, endEvent410 },
    { 411, script411, EVENT_TEXT(1), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
