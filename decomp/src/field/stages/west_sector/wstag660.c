/*
 * WSTAG660: Byakko City, West Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x4B7
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x4C7
#endif
/*
 * Sets the stage up: its map, actors and events, playing BGM_0021; BGM_0029
 * instead at story steps 0x27 to 0x28 and 0xF to 0x17
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x1F800, 0x2E200};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x17;
    FIELDSTG_state.music = MUSIC(0x17, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
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

s16 script415[] = {
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WALK(2, 0x186, 0x320, 5),
    SCRIPT_PLACE(0x45, 0x127, 0x266),
    SCRIPT_POSE(0x45, 1, 7),
    SCRIPT_PLACE(0x46, 0x1E0, 0x2D1),
    SCRIPT_POSE(0x46, 1, 1),
    SCRIPT_PLACE(0x47, 0x201, 0x2E1),
    SCRIPT_POSE(0x47, 1, 1),
    SCRIPT_PLACE(0x48, 0x221, 0x2F1),
    SCRIPT_POSE(0x48, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1BD, 0x320, 6),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x190, 0x282),
    SCRIPT_COMMAND(0x323, 0x325, 0x45),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x45),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x45, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x45, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0x202, 0x2CA),
    SCRIPT_POSE(0x46, 1, 2),
    SCRIPT_POSE(0x48, 1, 0),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 4, 0x46, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x47, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x48, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x46, 1, 1),
    SCRIPT_POSE(0x48, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x248, 0x1F0, 0x68, 1),
    SCRIPT_END,
};
s16 script1231[] = {
    SCRIPT_WALK(2, 0x43E, 0x204, 5),
    SCRIPT_PLACE(0x15, 0x45F, 0x1F6),
    SCRIPT_POSE(0x15, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x15, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x15, 0x36, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_PLAY_SAVEDEMO, 2),
    SCRIPT_WAIT_ANIM(0x15),
    SCRIPT_POSE(0x15, 0x37, 1),
    SCRIPT_WAIT(0x5A),
    SCRIPT_LEAVE(0xC06, 0, 0, 0),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU
    0x6004,
#endif
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1E4, 0x1B0, 0x290, 0xB0, 0x150, 0x1FB },
    { 0x180, 0x100, 0x180, 0x1CF, 0x100, 0xCF, 0x160, 0x1FB },
    { 0x1C0, 0x100, 0x1F2, 0x128, 0x2C8, 0x28, 0x170, 0x1FB },
    { 0x1C0, 0x100, 0x1E2, 0x132, 0x288, 0x32, 0x150, 0x1FA },
    { 0x1C0, 0x100, 0x1EE, 0x100, 0x2B8, 0, 0x160, 0x1FA },
    { 0x140, 0x100, 0x170, 0x1CE, 0xC0, 0xCE, 0x170, 0x1FA },
    { 0x1C0, 0x100, 0x1C0, 0x138, 0x200, 0x38, 0x140, 0x1F9 },
    { 0x1C0, 0x100, 0x1EC, 0x1B0, 0x2B0, 0xB0, 0x150, 0x1F9 },
    { 0x1C0, 0x100, 0x1D6, 0x144, 0x258, 0x44, 0x160, 0x1F9 },
    { 0x1C0, 0x100, 0x1C8, 0x14D, 0x220, 0x4D, 0x170, 0x1F9 },
    { 0x1C0, 0x100, 0x1EC, 0x150, 0x2B0, 0x50, 0x140, 0x1F8 },
    { 0x1C0, 0x100, 0x1F4, 0x150, 0x2D0, 0x50, 0x150, 0x1F8 },
    { 0x1C0, 0x100, 0x1DE, 0x152, 0x278, 0x52, 0x160, 0x1F8 },
    { 0x1C0, 0x100, 0x1F4, 0x1B0, 0x2D0, 0xB0, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1D0, 0x1B2, 0x240, 0xB2, 0x140, 0x1F7 },
    { 0x1C0, 0x100, 0x1D8, 0x1B2, 0x260, 0xB2, 0x150, 0x1F7 },
};
u16 actor0Talk0Actions[] = { 0x7A24, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x14), 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7A0C, 1, CODES_END };
u16 actor3Talk0Actions[] = { 0x7A0B, 1, CODES_END };
u16 actor4Talk0Actions[] = { 0x7C00, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 5 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 2 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0xF3 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x155 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x154 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x151 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x152 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x147 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x14B },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x148 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x149 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x14C },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x150 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x14D },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x14E },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x145 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x142 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x144 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x146 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x143 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x14A },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x14F },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x153 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor3Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor17Conditions[] = { FLAG(0x1C, 0x26), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor19Conditions[] = { FLAG(0x1C, 0x26), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor21Conditions[] = { FLAG(0x1C, 0x26), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor23Conditions[] = { FLAG(0x1C, 0x26), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x14, 4, 908, 596, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x15, 5, 1119, 502, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x16, 6, 834, 470, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x17, 7, 690, 493, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x18, 8, 1329, 411, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2D, 9, 897, 289, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2D, 9, 767, 529, 3 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2D, 9, 897, 289, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2D, 9, 897, 289, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x36, 0xA, 993, 531, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x36, 0xA, 897, 289, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x36, 0xA, 993, 531, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x36, 0xA, 993, 531, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x3A, 0xB, 767, 529, 3 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x3A, 0xB, 993, 531, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x3A, 0xB, 767, 529, 3 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x3A, 0xB, 767, 529, 3 };
FieldActorEntry actor17 = { actor17Conditions, NULL, 0x45, 0xC, 295, 614, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x45, 0xC, 834, 470, 7 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x46, 0xD, 480, 721, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x46, 0xD, 1123, 499, 1 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x47, 0xE, 513, 737, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x47, 0xE, 690, 493, 1 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x48, 0xF, 545, 753, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x48, 0xF, 908, 596, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x49, 0x10, 1329, 411, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x9D, 0x11, 993, 531, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x9E, 0x12, 767, 529, 3 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x9F, 0x13, 897, 289, 5 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0xC8, 2, 0x5A, 1, 0x5A, 0x61, 0xA, 0, 1314, 270, 0, 0 },
    { 1, 0, 0x48, 2, 0, 0, 0, 0, 0, 0, 444, 419, 0, 0 },
    { 1, 0, 0x58, 2, 0xD, 0, 0, 0, 0, 0, 683, 345, 0, 0 },
    { 1, 0, 0x4C, 2, 0xE, 0, 0, 0, 0, 0, 947, 492, 0, 0 },
    { 1, 0, 0x60, 2, 0x10, 0, 0, 0, 0, 0, 640, 515, 0, 0 },
    { 1, 0, 0x74, 2, 0x11, 0, 0, 0, 0, 0, 624, 524, 0, 0 },
    { 1, 0, 0x44, 2, 0x12, 0, 0, 0, 0, 0, 304, 556, 0, 0 },
    { 1, 0, 0x40, 2, 7, 1, 7, 0xC, 4, 0, 379, 317, 0, 0 },
    { 1, 0, 0x40, 2, 7, 1, 7, 0xC, 4, 0, 924, 587, 0, 0 },
    { 1, 0, 0x41, 2, 3, 0, 0, 0, 0, 0, 960, 418, 0, 0 },
    { 1, 0, 0x62, 2, 0xF, 0, 0, 0, 0, 0, 938, 609, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 8, 0, 1378, 348, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 8, 0, 1391, 358, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 5, 8, 0, 1250, 357, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 5, 8, 0, 1263, 350, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x38, 4, 0, 1217, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x38, 4, 0, 1417, 367, 0, 0 },
    { 1, 0, 0x40, 6, 0x39, 2, 0, 3, 6, 0, 1310, 259, 0, 0 },
    { 1, 0, 0xC8, 6, 0x57, 1, 0x57, 0x59, 6, 0, 1144, 428, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x3E, 0xA, 0, 939, 765, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x3E, 0xA, 0, 1005, 684, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x3E, 0xA, 0, 1163, 605, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x3E, 0xA, 0, 1324, 525, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x41, 0xA, 0, 971, 701, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x41, 0xA, 0, 1013, 599, 0, 0 },
    { 1, 0, 0x40, 6, 0x3F, 1, 0x3F, 0x41, 0xA, 0, 1160, 527, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 337, 398, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 411, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 489, 399, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 701, 294, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 815, 743, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 995, 695, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 1025, 674, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 1079, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 1139, 536, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 1182, 595, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 1, 0x42, 0x44, 0xA, 0, 1306, 536, 0, 0 },
    { 1, 0, 0x40, 6, 0x45, 1, 0x45, 0x47, 0xA, 0, 551, 531, 0, 0 },
    { 1, 0, 0x40, 6, 0x45, 1, 0x45, 0x47, 0xA, 0, 733, 621, 0, 0 },
    { 1, 0, 0x40, 6, 0x45, 1, 0x45, 0x47, 0xA, 0, 873, 609, 0, 0 },
    { 1, 0, 0x40, 6, 0x45, 1, 0x45, 0x47, 0xA, 0, 886, 745, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x4A, 0xA, 0, 345, 477, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x4A, 0xA, 0, 575, 542, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x4A, 0xA, 0, 814, 580, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 1, 0x48, 0x4A, 0xA, 0, 896, 701, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 1, 0x4B, 0x4D, 0xA, 0, 869, 689, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 273, 423, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 289, 417, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 297, 409, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 310, 410, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 510, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 531, 377, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 719, 281, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 804, 560, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 817, 561, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 922, 500, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 964, 753, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 977, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 1101, 351, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 1171, 513, 0, 0 },
    { 1, 0, 0x40, 6, 0x4E, 1, 0x4E, 0x50, 0xA, 0, 1174, 506, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0x51, 1, 0x51, 0x53, 6, 0, 799, 318, 0, 0 },
    { 1, 0, 0xC8, 0xA, 0x54, 1, 0x54, 0x56, 6, 0, 1052, 371, 0, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 944, 515, 569, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 582, 424, 454, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 1056, 269, 327, 0 },
    { 1, 0, 0x41, 4, 4, 0, 0, 0, 0, 0, 369, 703, 751, 0 },
    { 1, 0, 0x41, 4, 4, 0, 0, 0, 0, 0, 449, 743, 791, 0 },
    { 1, 0, 0x60, 4, 2, 0, 0, 0, 0, 0, 914, 233, 303, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x25E, 0x100, 0x220, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x248, 0x1F0, 0x68, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x170, 0x268, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x180, 0x2B0, 0, 0, 0, 0 },
    { { { FLAG(0x1C, 0x26), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x19F, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1231, script1231, EVENT_TEXT(7), NULL, NULL },
    { 415, script415, EVENT_TEXT(0xE), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
