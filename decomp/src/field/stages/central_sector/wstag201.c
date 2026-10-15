/*
 * WSTAG201: Amaterasu City, Central Sector: the streets by the bridge. On the
 * Amaterasu server; its Asuka server twin is WSTAG200.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x512
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x522
#endif
/*
 * Sets the stage up: its map, actors and battles, playing ASKA_BGM; BGM_0029
 * instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE + 1;
    FIELDSTG_state.start = (Vec2){0x16B00, 0x28500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 4;
    FIELDSTG_state.music = MUSIC(4, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

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
AreaBattle area3Battle0 = { 195, 18, MUSIC(2, 0) };
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
    { 142, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1A2, 0x15A, 0x188, 0x5A, 0x170, 0x1FD },
    { 0x180, 0x100, 0x18E, 0x185, 0x138, 0x85, 0x170, 0x1FC },
    { 0x180, 0x100, 0x196, 0x186, 0x158, 0x86, 0x140, 0x1FB },
    { 0x1C0, 0x100, 0x1F8, 0x121, 0x2E0, 0x21, 0x150, 0x1FB },
    { 0x1C0, 0x100, 0x1DE, 0x130, 0x278, 0x30, 0x160, 0x1FB },
    { 0x1C0, 0x100, 0x1C0, 0x100, 0x200, 0, 0x170, 0x1FB },
    { 0x1C0, 0x100, 0x1C8, 0x100, 0x220, 0, 0x140, 0x1FA },
    { 0x1C0, 0x100, 0x1D0, 0x100, 0x240, 0, 0x150, 0x1FA },
    { 0x1C0, 0x100, 0x1CA, 0x140, 0x228, 0x40, 0x160, 0x1FA },
    { 0x1C0, 0x100, 0x1D2, 0x140, 0x248, 0x40, 0x170, 0x1FA },
    { 0x1C0, 0x100, 0x1D8, 0x100, 0x260, 0, 0x140, 0x1F9 },
    { 0x1C0, 0x100, 0x1E0, 0x100, 0x280, 0, 0x150, 0x1F9 },
    { 0x1C0, 0x100, 0x1E6, 0x100, 0x298, 0, 0x160, 0x1F9 },
    { 0x180, 0x100, 0x1AA, 0x188, 0x1A8, 0x88, 0x170, 0x1F9 },
    { 0x1C0, 0x100, 0x1E6, 0x148, 0x298, 0x48, 0x140, 0x1F8 },
    { 0x1C0, 0x100, 0x1DA, 0x150, 0x268, 0x50, 0x150, 0x1F8 },
    { 0x1C0, 0x100, 0x1EE, 0x154, 0x2B8, 0x54, 0x160, 0x1F8 },
    { 0x1C0, 0x100, 0x1F6, 0x154, 0x2D8, 0x54, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1C0, 0x158, 0x200, 0x58, 0x140, 0x1F7 },
    { 0x1C0, 0x100, 0x1C8, 0x160, 0x220, 0x60, 0x150, 0x1F7 },
    { 0x1C0, 0x100, 0x1D0, 0x160, 0x240, 0x60, 0x160, 0x1F7 },
    { 0x1C0, 0x100, 0x1E2, 0x168, 0x288, 0x68, 0x170, 0x1F7 },
    { 0x1C0, 0x100, 0x1D8, 0x170, 0x260, 0x70, 0x140, 0x1F6 },
    { 0x1C0, 0x100, 0x1F2, 0x174, 0x2C8, 0x74, 0x150, 0x1F6 },
    { 0x1C0, 0x100, 0x1C0, 0x178, 0x200, 0x78, 0x160, 0x1F6 },
    { 0x1C0, 0x100, 0x1C8, 0x180, 0x220, 0x80, 0x170, 0x1F6 },
};
u16 actor0Talk0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor0Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor1Talk0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor1Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor2Talk0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor2Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor12Talk0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor12Talk1Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0), 0, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Talk1Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor12Talk2Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor12Talk3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor14Talk0Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 1), 0, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor14Talk1Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 1), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor14Talk2Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor15Talk0Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor15Talk1Conditions[] = { PROGRESS(0x26), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, NULL, 0xB5 },
    { actor0Talk1Conditions, NULL, 0xB6 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0xB8 },
    { actor1Talk1Conditions, NULL, 0xB9 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0xBB },
    { actor2Talk1Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0xC4 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0xC2 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0xCA },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xC8 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xC6 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xD2 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xD3 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xCD },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xD0 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x1B6 },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x1FC },
    { actor12Talk2Conditions, NULL, 0x1FD },
    { actor12Talk3Conditions, NULL, 0x20A },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xA2 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x1FE },
    { actor14Talk1Conditions, NULL, 0x1FF },
    { actor14Talk2Conditions, NULL, 0x20B },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0xBE },
    { actor15Talk1Conditions, NULL, 0xBF },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0xCC },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0xCE },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0xCF },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0xD1 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0xC7 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xC5 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xCB },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xC9 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0xC3 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0xC1 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0xB7 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0xBA },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0xBD },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0xC0 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0xD4 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0xD5 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0xD6 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1A, 0xA), 1, PROGRESS(0x26), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x1C), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor21Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1C), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor25Conditions[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1C), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x25, 4, 1088, 288, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x26, 5, 736, 392, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x27, 6, 555, 474, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 7, 305, 616, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2D, 7, 268, 586, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2E, 8, 784, 264, 3 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x30, 9, 944, 584, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x30, 9, 944, 584, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x32, 0xA, 563, 559, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x38, 0xB, 905, 253, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x39, 0xC, 564, 558, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x3A, 0xD, 595, 542, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x65, 0xE, 257, 704, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x66, 0xF, 905, 253, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x67, 0x10, 225, 720, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x6F, 0x11, 464, 640, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9D, 0x12, 564, 558, 5 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9D, 0x12, 564, 558, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9E, 0x13, 595, 542, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x9E, 0x13, 595, 542, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9F, 0x14, 944, 584, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9F, 0x14, 944, 584, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0xA0, 0x15, 784, 264, 3 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0xA0, 0x15, 784, 264, 3 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0xA1, 0x16, 268, 586, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0xA1, 0x16, 268, 586, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0xA2, 0x17, 1088, 288, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0xAE, 0x18, 736, 392, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0xAF, 0x19, 555, 474, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0xB0, 0x1A, 464, 640, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x177, 0x1B, 736, 393, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x179, 0x1C, 464, 641, 5 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x17B, 0x1D, 596, 543, 1 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0xA, 0, 0, 0, 0, 0, 120, 520, 0, 0 },
    { 1, 0, 0x78, 2, 0x19, 0, 0, 0, 0, 0, 130, 528, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 1048, 211, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 3, 6, 0, 437, 437, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 3, 6, 0, 533, 389, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 3, 6, 0, 1231, 344, 0, 0 },
    { 1, 0, 0x40, 2, 0x4A, 2, 0, 3, 6, 0, 623, 331, 0, 0 },
    { 1, 0, 0x40, 2, 0xA, 0, 0, 0, 0, 0, 352, 629, 0, 0 },
    { 1, 0, 0x40, 2, 0x10, 0, 0, 0, 0, 0, 268, 384, 0, 0 },
    { 1, 0, 0x48, 2, 0x11, 0, 0, 0, 0, 0, 274, 440, 0, 0 },
    { 1, 0, 0x40, 2, 0x12, 0, 0, 0, 0, 0, 1044, 416, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 1088, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 430, 722, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 501, 687, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 945, 160, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 1096, 700, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 3, 6, 0, 1142, 723, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 483, 414, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 597, 695, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 642, 672, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 754, 617, 0, 0 },
    { 1, 0, 0x40, 6, 0x49, 2, 0, 3, 6, 0, 1023, 463, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 3, 6, 0, 669, 308, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 145, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 290, 767, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 845, 621, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 903, 713, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 40, 693, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 191, 816, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 441, 795, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1030, 595, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1148, 406, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x14, 0, 0, 0, 0, 0, 277, 489, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x15, 0, 0, 0, 0, 0, 446, 416, 0, 0 },
    { 1, 0x66, 0x40, 6, 0x16, 0, 0, 0, 0, 0, 542, 367, 0, 0 },
    { 1, 0x67, 0x40, 6, 0x17, 0, 0, 0, 0, 0, 638, 321, 0, 0 },
    { 1, 0x68, 0x40, 6, 0x18, 0, 0, 0, 0, 0, 1045, 458, 0, 0 },
    { 1, 0, 0x68, 6, 6, 0, 0, 0, 0, 0, 879, 428, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4B, 1, 0x4B, 0x4E, 6, 0, 241, 396, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4F, 1, 0x4F, 0x52, 6, 0, 226, 450, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x53, 1, 0x53, 0x56, 6, 0, 1091, 316, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x57, 1, 0x57, 0x5A, 6, 0, 1103, 338, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 303, 495, 544, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 409, 416, 472, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 510, 375, 423, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 609, 327, 375, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 1051, 192, 246, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 1073, 463, 512, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x272, 0x308, 0xE0, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27E, 0xE0, 0x14E, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27C, 0x178, 0x19C, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x27C, 0x218, 0x14C, 3, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x279, 0x116, 0xDA, 3, 0x67, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0x60, 0x190, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x275, 0xD8, 0x178, 5, 0x68, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
