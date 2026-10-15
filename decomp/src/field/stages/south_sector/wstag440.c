/*
 * WSTAG440: South Station, South Sector.
 */

#include "common.h"
#include "field/stage.h"

/* Creates the event object of story progress 6 */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        if (GAME.progress == 6) {
            children[0] = FIELDSTG_startEvent(0x98);
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

/* After event 152, with the player: moves the story on to its step 7 */
void endEvent152(void) {
    GAME.progress = 7;
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x36D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x37D
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0012;
 * BGM_0029 instead at story steps 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1A000, 0x21C00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xE;
    FIELDSTG_state.music = MUSIC(0xE, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script144[] = {
    SCRIPT_WALK(2, 0x1BF, 0x120, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1A0, 0x130, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x150, 0x109, 3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x22C, 0x140, 0x140, 5),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x8E02,
#elif VERSION_EU || VERSION_JP
    0x128,
#endif
};
s16 script152[] = {
    SCRIPT_LOOK_AT(1, 0x1AF, 0xD9),
    SCRIPT_PLACE(2, 0x1EF, 0xB9),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x16F, 0xF9, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x233, 0x208, 0x9C, 7),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x8FBF,
#elif VERSION_EU || VERSION_JP
    0x1FB,
#endif
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
AreaBattle area3Battle0 = { 210, 20, MUSIC(3, 0) };
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
    { 156, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x158, 0x150, 0x60, 0x50, 0x170, 0x1FE },
    { 0x140, 0x100, 0x140, 0x150, 0, 0x50, 0x160, 0x1FD },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x148, 0x150, 0x20, 0x50, 0x170, 0x1FD },
    { 0x140, 0x100, 0x150, 0x150, 0x40, 0x50, 0x160, 0x1FC },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor0Talk2Actions[] = { CARD_BATTLE(0x14, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xA), 0,
    PARTY_STAT(4), 1,
    CODES_END,
};
u16 actor0Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xA), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xA), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xA), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xA), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor0Talk6Actions[] = { CARD_BATTLE(0x14, 1), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(4), 0, PARTY_STAT(2), 1, CODES_END };
u16 actor1Talk2Actions[] = { CARD_BATTLE(0x14, 0), 1, CODES_END };
u16 actor1Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xA), 0,
    CODES_END,
};
u16 actor1Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xA), 1, CODES_END };
u16 actor1Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor1Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor1Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor1Talk6Actions[] = { CARD_BATTLE(0x14, 1), 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x14, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xA), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor2Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(0x14, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor19Talk0Actions[] = { START_EVENT(0x60), 1, CODES_END };
u16 actor20Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor20Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor20Talk2Actions[] = { CARD_BATTLE(0x14, 0), 1, CODES_END };
u16 actor20Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 0,
    CODES_END,
};
u16 actor20Talk3Actions[] = { FLAG(0xE, 0xA), 1, CODES_END };
u16 actor20Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor20Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xA), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor20Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(6), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    FLAG(0xE, 0xA), 1,
    CODES_END,
};
u16 actor20Talk6Actions[] = { CARD_BATTLE(0x14, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x8A },
    { actor0Talk1Conditions, NULL, 0x8F },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x90 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x91 },
    { actor0Talk4Conditions, NULL, 0x92 },
    { actor0Talk5Conditions, NULL, 0x93 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0x94 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x8B },
    { actor1Talk1Conditions, NULL, 0x8F },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x90 },
    { actor1Talk3Conditions, actor1Talk3Actions, 0x91 },
    { actor1Talk4Conditions, NULL, 0x92 },
    { actor1Talk5Conditions, NULL, 0x93 },
    { actor1Talk6Conditions, actor1Talk6Actions, 0x94 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x8C },
    { actor2Talk1Conditions, NULL, 0x8F },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x90 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x91 },
    { actor2Talk4Conditions, NULL, 0x92 },
    { actor2Talk5Conditions, NULL, 0x93 },
    { actor2Talk6Conditions, actor2Talk6Actions, 0x94 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x8A },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x95 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x96 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x27E },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xF8 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xFC },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x1E },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xF3 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xF4 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0xF5 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0xF6 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0xF7 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0xFA },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0xF8 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, actor19Talk0Actions, 0x32B },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, NULL, 0x8D },
    { actor20Talk1Conditions, NULL, 0x8D },
    { actor20Talk2Conditions, actor20Talk2Actions, 0x8D },
    { actor20Talk3Conditions, actor20Talk3Actions, 0x8D },
    { actor20Talk4Conditions, NULL, 0x8D },
    { actor20Talk5Conditions, NULL, 0x8D },
    { actor20Talk6Conditions, actor20Talk6Actions, 0x8D },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xFB },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(4), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(9), 1, ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor5Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 177, 313, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 177, 313, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 4, 177, 313, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 4, 177, 313, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2D, 4, 177, 313, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x2E, 5, 145, 329, 5 };
FieldActorEntry actor19 = { NULL, actor19Talks, 0x3F, 6, 463, 280, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9D, 7, 177, 313, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9E, 8, 145, 329, 5 };
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
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 9, 0, 435, 247, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 1, 0x33, 0x38, 9, 0, 443, 254, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 5, 6, 0, 443, 348, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 1, 0x3A, 0x3F, 6, 0, 377, 384, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 482, 177, 200, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x233, 0x208, 0x9C, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 152, script152, EVENT_TEXT(6), NULL, endEvent152 },
    { 144, script144, EVENT_TEXT(0x21), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
