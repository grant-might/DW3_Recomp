/*
 * WSTAG211: Main Lobby, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG210.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/*
 * After event 925, with Attacker: starts the stage's event battle 0, sets
 * flag 0x0C23 (which decides whether actors 30 to 32 appear) and keeps it
 * from playing again
 */
void endEvent925(void) {
    FLAGS_00.applyAction(FLAG(0xC, 0x23), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0xA4), 1);
    FLAGS_00.applyAction(EVENT_BATTLE(0), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x494
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x4A4
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing KANRIBGM;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x14B00, 0x13400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
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

s16 script925[] = {
    SCRIPT_WALK(2, 0x120, 0xE1, 5),
    SCRIPT_PLACE(0xC2, 0x149, 0xCD),
    SCRIPT_POSE(0xC2, 1, 1),
    SCRIPT_PLACE(0xC3, 0x160, 0xD9),
    SCRIPT_POSE(0xC3, 1, 1),
    SCRIPT_PLACE(0xC4, 0x130, 0xC0),
    SCRIPT_POSE(0xC4, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xC2),
    SCRIPT_COMMAND(0x324, 0x325, 0xC3),
    SCRIPT_COMMAND(0x325, 0x325, 0xC4),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0xC2),
    SCRIPT_COMMAND(0x324, 0x326, 0xC3),
    SCRIPT_COMMAND(0x325, 0x326, 0xC4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC2, 0x141, 0xD1, 1),
    SCRIPT_WAIT_WALK(0xC2),
    SCRIPT_POSE(0xC2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xC2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU
    0x6961,
#elif VERSION_JP
    0x6004,
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
AreaBattle area3Battle0 = { 195, 15, MUSIC(2, 0) };
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
    { 143, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F0, 0x160, 0x2C0, 0x60, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1C0, 0x171, 0x200, 0x71, 0x140, 0x1F7 },
    { 0x180, 0x100, 0x198, 0x1B0, 0x160, 0xB0, 0x150, 0x1F7 },
    { 0x1C0, 0x100, 0x1F6, 0x100, 0x2D8, 0, 0x160, 0x1F7 },
    { 0x1C0, 0x100, 0x1F6, 0x130, 0x2D8, 0x30, 0x170, 0x1F7 },
    { 0x1C0, 0x100, 0x1F8, 0x160, 0x2E0, 0x60, 0x140, 0x1F6 },
    { 0x1C0, 0x100, 0x1C8, 0x171, 0x220, 0x71, 0x150, 0x1F6 },
    { 0x1C0, 0x100, 0x1D8, 0x174, 0x260, 0x74, 0x160, 0x1F6 },
    { 0x1C0, 0x100, 0x1E0, 0x174, 0x280, 0x74, 0x170, 0x1F6 },
    { 0x1C0, 0x100, 0x1E8, 0x174, 0x2A0, 0x74, 0x140, 0x1F5 },
    { 0x1C0, 0x100, 0x1C8, 0x199, 0x220, 0x99, 0x150, 0x1F5 },
    { 0x1C0, 0x100, 0x1D0, 0x154, 0x240, 0x54, 0x160, 0x1F5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x1C0, 0x100, 0x1D0, 0x1AC, 0x240, 0xAC, 0x170, 0x1F5 },
    { 0x1C0, 0x100, 0x1EC, 0x1B0, 0x2B0, 0xB0, 0x140, 0x1F4 },
    { 0x1C0, 0x100, 0x1F4, 0x1B0, 0x2D0, 0xB0, 0x150, 0x1F4 },
    { 0x1C0, 0x100, 0x1D8, 0x1B4, 0x260, 0xB4, 0x160, 0x1F4 },
    { 0x1C0, 0x100, 0x1E0, 0x1B4, 0x280, 0xB4, 0x170, 0x1F4 },
    { 0x1C0, 0x100, 0x1C8, 0x1B9, 0x220, 0xB9, 0x140, 0x1F3 },
    { 0x1C0, 0x100, 0x1C0, 0x1C1, 0x200, 0xC1, 0x150, 0x1F3 },
    { 0x1C0, 0x100, 0x1D0, 0x1CC, 0x240, 0xCC, 0x160, 0x1F3 },
    { 0x1C0, 0x100, 0x1D0, 0x184, 0x240, 0x84, 0x170, 0x1F3 },
    { 0x1C0, 0x100, 0x1F0, 0x188, 0x2C0, 0x88, 0x140, 0x1F2 },
    { 0x1C0, 0x100, 0x1C0, 0x199, 0x200, 0x99, 0x150, 0x1F2 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor30Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x23), 1, CODES_END };
u16 actor31Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x23), 1, CODES_END };
u16 actor32Talk0Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xC, 0x23), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x33 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x36 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x37 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x3A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x43 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x45 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x47 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x42 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x3E },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x3F },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x3B },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x4B },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x49 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x35 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x38 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x3C },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x3D },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x40 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x41 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x44 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x46 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x48 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x4A },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, actor30Talk0Actions, 0x1B7 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, actor31Talk0Actions, 0x1B8 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, actor32Talk0Actions, 0x1B9 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { FLAG(0xC, 0x23), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0x25), 1, FLAG(0xC, 0x23), 0, CODES_END };
u16 actor32Conditions[] = { FLAG(0xC, 0x23), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x94), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0x26), 0, SPECIAL(0x94), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x25, 6, 304, 193, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x26, 7, 305, 353, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x27, 8, 193, 257, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2D, 9, 529, 505, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x30, 0xA, 617, 501, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x31, 0xB, 328, 260, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x32, 0xC, 617, 501, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x37, 0xD, 391, 359, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x39, 0xE, 684, 363, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x6F, 0xF, 529, 505, 7 };
FieldActorEntry actor14 = { actor14Conditions, NULL, 0x70, 0x10, 360, 268, 1 };
FieldActorEntry actor15 = { actor15Conditions, NULL, 0x70, 0x10, 360, 268, 1 };
FieldActorEntry actor16 = { actor16Conditions, NULL, 0x71, 0x11, 450, 254, 7 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x71, 0x11, 450, 254, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9D, 0x12, 384, 255, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x9D, 0x12, 384, 255, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9E, 0x13, 434, 247, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9E, 0x13, 434, 247, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x9F, 0x14, 617, 501, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x9F, 0x14, 617, 501, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0xA0, 0x15, 328, 260, 5 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0xA0, 0x15, 328, 260, 5 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0xA1, 0x16, 304, 193, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0xA2, 0x17, 305, 303, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0xAE, 0x18, 193, 257, 5 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0xAF, 0x19, 529, 505, 7 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0xC2, 0x1A, 329, 205, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0xC3, 0x1B, 352, 217, 1 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0xC4, 0x1C, 304, 192, 1 };
FieldActorEntry actor33 = { actor33Conditions, NULL, 0x10E, 0x1D, 360, 268, 1 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0x10E, 0x1D, 360, 268, 1 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x10F, 0x1E, 450, 254, 7 };
FieldActorEntry actor36 = { actor36Conditions, NULL, 0x10F, 0x1E, 450, 254, 7 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x36, 2, 0, 1, 4, 0, 243, 69, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 1, 4, 0, 802, 278, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 4, 0, 457, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 339, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 372, 196, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 403, 244, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 435, 228, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 386, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 546, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 674, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 573, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 613, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 151, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 411, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 649, 200, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x47, 0xA, 0, 76, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x47, 0xA, 0, 561, 589, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x47, 0xA, 0, 710, 468, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x52, 0xA, 0, 225, 445, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x52, 0xA, 0, 422, 462, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x52, 0xA, 0, 748, 560, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 10, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 42, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 74, 137, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 290, 69, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 322, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 354, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 482, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 514, 181, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 706, 213, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 738, 229, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 770, 245, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 7, 0x12, 0, 119, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 135, 232, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 155, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 175, 212, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 195, 202, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 215, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 235, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 255, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 275, 162, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 295, 152, 0, 0 },
    { 1, 0x64, 0x40, 6, 4, 0, 0, 0, 0, 0, 335, 141, 0, 0 },
    { 1, 0x65, 0x40, 6, 5, 0, 0, 0, 0, 0, 511, 229, 0, 0 },
    { 1, 0x66, 0x40, 6, 6, 0, 0, 0, 0, 0, 736, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 4, 0, 114, 179, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 4, 0, 264, 130, 0, 0 },
    { 1, 0, 0x40, 6, 0x59, 2, 0, 3, 4, 0, 429, 176, 0, 0 },
    { 1, 0, 0x40, 6, 0x5E, 1, 0x5E, 0x61, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 6, 0x5A, 1, 0x5A, 0x5D, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 144, 110, 160, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 359, 148, 193, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 535, 236, 281, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 759, 332, 376, 0 },
    { 1, 0, 0x50, 4, 7, 0, 0, 0, 0, 0, 441, 345, 450, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 464, 444, 496, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 595, 442, 478, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 384, 259, 279, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 401, 250, 271, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 368, 251, 270, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 417, 243, 264, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 352, 242, 262, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 433, 233, 255, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 336, 229, 255, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 449, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 320, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 337, 219, 240, 0 },
    { 1, 0, 0x40, 4, 0x14, 0, 0, 0, 0, 0, 353, 208, 231, 0 },
    { 1, 0, 0x40, 4, 0x15, 0, 0, 0, 0, 0, 369, 203, 223, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 385, 192, 215, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 401, 187, 207, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 499, 317, 334, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x3E8, 0xEC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x275, 0x70, 0xF0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x276, 0x58, 0x1CC, 5, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x277, 0x69, 0x134, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x283, 0x1F8, 0x2BC, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x286, 0x1D8, 0x1D4, 3, 0, 0, 0 },
    { { { PROGRESS(0x25), 1 }, { FLAG(0x40, 0xA4), 0 } }, SLOT_EVENT, 0x39D, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 925, script925, EVENT_TEXT(0x28), NULL, endEvent925 },
    { -1, NULL, 0, NULL, NULL },
};
