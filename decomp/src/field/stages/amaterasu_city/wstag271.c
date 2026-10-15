/*
 * WSTAG271: Yellow Cruiser, Amaterasu City. On the Amaterasu server; its Asuka
 * server twin is WSTAG270.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE2)
#define STAGE_FILE 0x6F9
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xDA)
#define STAGE_FILE 0x709
#endif
/* Sets the stage up: its map and actors, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0xC000, 0x11D00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x140, 0x1D8, 0, 0xD8, 0x160, 0x1FB },
    { 0x180, 0x100, 0x1B0, 0x159, 0x1C0, 0x59, 0x170, 0x1FB },
    { 0x180, 0x100, 0x1A0, 0x15D, 0x180, 0x5D, 0x150, 0x1FA },
    { 0x180, 0x100, 0x1A8, 0x15D, 0x1A0, 0x5D, 0x160, 0x1FA },
    { 0x180, 0x100, 0x190, 0x171, 0x140, 0x71, 0x170, 0x1FA },
    { 0x180, 0x100, 0x198, 0x171, 0x160, 0x71, 0x140, 0x1F9 },
    { 0x180, 0x100, 0x180, 0x172, 0x100, 0x72, 0x150, 0x1F9 },
    { 0x180, 0x100, 0x188, 0x172, 0x120, 0x72, 0x160, 0x1F9 },
    { 0x180, 0x100, 0x1B0, 0x181, 0x1C0, 0x81, 0x170, 0x1F9 },
    { 0x180, 0x100, 0x1A0, 0x185, 0x180, 0x85, 0x140, 0x1F8 },
    { 0x180, 0x100, 0x1A8, 0x185, 0x1A0, 0x85, 0x150, 0x1F8 },
    { 0x180, 0x100, 0x180, 0x192, 0x100, 0x92, 0x160, 0x1F8 },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x170, 0x1F8 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor2Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 2), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor3Talk3Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0xB), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk4Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk4Actions[] = { CARD_BATTLE(0x53, 0), 1, CODES_END };
u16 actor3Talk5Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor3Talk5Actions[] = { CARD_BATTLE(0x53, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor5Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor5Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor6Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor6Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Conditions[] = { FLAG(0, 3), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk2Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor6Talk3Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0xB), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor6Talk4Conditions[] = {
    FLAG(0, 3), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor6Talk4Actions[] = { CARD_BATTLE(0x54, 0), 1, CODES_END };
u16 actor6Talk5Conditions[] = {
    FLAG(0, 3), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor6Talk5Actions[] = { CARD_BATTLE(0x54, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 0), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk2Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor9Talk3Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0xB), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor9Talk4Actions[] = { CARD_BATTLE(0x51, 0), 1, CODES_END };
u16 actor9Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor9Talk5Actions[] = { CARD_BATTLE(0x51, 1), 1, CODES_END };
u16 actor13Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0x10), 0, CODES_END };
u16 actor13Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk2Conditions[] = { FLAG(0, 0x10), 1, CODES_END };
u16 actor13Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk0Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor14Talk0Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk2Conditions[] = { FLAG(0, 1), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk2Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor14Talk3Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0xB), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 0,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor14Talk4Actions[] = { CARD_BATTLE(0x52, 0), 1, CODES_END };
u16 actor14Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0xB), 1,
    PARTY_STAT(0xD), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor14Talk5Actions[] = { CARD_BATTLE(0x52, 1), 1, CODES_END };
u16 actor28Talk0Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 0, CODES_END };
u16 actor28Talk1Conditions[] = { SPECIAL(8), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor28Talk1Actions[] = { 0x7A43, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x26 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x25 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x1A0 },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x1A },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x1B },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x1B },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x1A },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x15 },
    { actor3Talk3Conditions, NULL, 0x17 },
    { actor3Talk4Conditions, actor3Talk4Actions, 0x18 },
    { actor3Talk5Conditions, actor3Talk5Actions, 0x19 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x1A0 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x1A1 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x21 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x22 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, actor6Talk0Actions, 0x22 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x21 },
    { actor6Talk2Conditions, actor6Talk2Actions, 0x1C },
    { actor6Talk3Conditions, NULL, 0x1E },
    { actor6Talk4Conditions, actor6Talk4Actions, 0x1F },
    { actor6Talk5Conditions, actor6Talk5Actions, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x1A1 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x19E },
    { actor8Talk1Conditions, actor8Talk1Actions, 0xC },
    { actor8Talk2Conditions, actor8Talk2Actions, 0xD },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0xD },
    { actor9Talk1Conditions, actor9Talk1Actions, 0xC },
    { actor9Talk2Conditions, actor9Talk2Actions, 7 },
    { actor9Talk3Conditions, NULL, 9 },
    { actor9Talk4Conditions, actor9Talk4Actions, 0xA },
    { actor9Talk5Conditions, actor9Talk5Actions, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x19E },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x19F },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x13 },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x14 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, actor14Talk0Actions, 0x14 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x13 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0xE },
    { actor14Talk3Conditions, NULL, 0x10 },
    { actor14Talk4Conditions, actor14Talk4Actions, 0x11 },
    { actor14Talk5Conditions, actor14Talk5Actions, 0x12 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x19F },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x27 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x2D },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x29 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x2F },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x2A },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x30 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x2B },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x31 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x2C },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x32 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x28 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x2E },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { actor28Talk0Conditions, NULL, 0x20D },
    { actor28Talk1Conditions, actor28Talk1Actions, 0x1A7 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor2Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor5Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor6Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor7Conditions[] = { ITEM(0, 0x192), 0, PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor8Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor9Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor10Conditions[] = { ITEM(0, 0x192), 0, PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor13Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor14Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor15Conditions[] = { ITEM(0, 0x192), 0, PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor21Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actor31Conditions[] = { SPECIAL(0x1C), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2E, 4, 255, 176, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2E, 4, 255, 176, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x30, 5, 198, 219, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x30, 5, 198, 219, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x30, 5, 198, 219, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x31, 6, 173, 231, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x31, 6, 173, 231, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x31, 6, 173, 231, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x33, 7, 250, 244, 3 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x33, 7, 250, 244, 3 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x33, 7, 250, 244, 3 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x34, 8, 348, 147, 1 };
FieldActorEntry actor12 = { actor12Conditions, NULL, 0x34, 8, 348, 147, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x36, 9, 224, 257, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x36, 9, 224, 257, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x36, 9, 224, 257, 3 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x9D, 0xA, 348, 147, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x9D, 0xA, 348, 147, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x9E, 0xB, 250, 244, 3 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x9E, 0xB, 250, 244, 3 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x9F, 0xC, 224, 257, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x9F, 0xC, 224, 257, 3 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0xA0, 0xD, 198, 219, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0xA0, 0xD, 198, 219, 7 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0xA1, 0xE, 173, 231, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0xA1, 0xE, 173, 231, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0xA2, 0xF, 255, 176, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0xA2, 0xF, 255, 176, 3 };
FieldActorEntry actor28 = { NULL, actor28Talks, 0xCE, 0x10, 379, 147, 7 };
FieldActorEntry actor29 = { NULL, NULL, 0xDC, 0x11, 379, 163, 7 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x100, 0x12, 347, 158, 1 };
FieldActorEntry actor31 = { actor31Conditions, NULL, 0x10E, 0x13, 347, 158, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x10E, 0x13, 347, 158, 1 };
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
    { 1, 0, 0x40, 6, 0x3B, 2, 0, 5, 8, 0, 428, 75, 0, 0 },
    { 1, 0, 0x40, 6, 0x3D, 2, 0, 3, 8, 0, 65, 176, 0, 0 },
    { 1, 0, 0x40, 6, 0x3A, 2, 0, 3, 8, 0, 425, 67, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x39, 8, 0, 428, 75, 0, 0 },
    { 1, 0, 0x40, 4, 0x3C, 2, 0, 3, 8, 0, 470, 149, 176, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 86, 239, 258, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 160, 220, 248, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 192, 204, 232, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 320, 201, 231, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 291, 185, 209, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 352, 147, 168, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 371, 140, 160, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 336, 139, 160, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 385, 131, 153, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 320, 131, 153, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 401, 123, 144, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 304, 117, 144, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 464, 149, 176, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x271, 0x1F0, 0xA0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
