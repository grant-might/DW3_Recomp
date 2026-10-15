/*
 * WSTAG760: Genbu City, North Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x143
#define STAGE_FILE 0x709
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x14A
#define STAGE_FILE 0x719
#endif
/*
 * Sets the stage up: its map, actors and events, playing BGM_0024; BGM_0029
 * instead at story steps 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x27100, 0x2F000};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x1A;
    FIELDSTG_state.music = MUSIC(0x1A, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script1240[] = {
    SCRIPT_WALK(2, 0x4B0, 0x381, 5),
    SCRIPT_PLACE(0x15, 0x4D1, 0x371),
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
    SCRIPT_LEAVE(0xC09, 0, 0, 0),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x168, 0x188, 0xA0, 0x88, 0x170, 0x1FB },
    { 0x140, 0x100, 0x170, 0x100, 0xC0, 0, 0x160, 0x1FA },
    { 0x140, 0x100, 0x170, 0x158, 0xC0, 0x58, 0x170, 0x1FA },
    { 0x140, 0x100, 0x148, 0x160, 0x20, 0x60, 0x150, 0x1F9 },
    { 0x140, 0x100, 0x16E, 0x130, 0xB8, 0x30, 0x160, 0x1F9 },
    { 0x140, 0x100, 0x168, 0x134, 0xA0, 0x34, 0x170, 0x1F9 },
    { 0x140, 0x100, 0x160, 0x134, 0x80, 0x34, 0x150, 0x1F8 },
    { 0x140, 0x100, 0x140, 0x154, 0, 0x54, 0x160, 0x1F8 },
    { 0x140, 0x100, 0x168, 0x158, 0xA0, 0x58, 0x170, 0x1F8 },
    { 0x140, 0x100, 0x148, 0x1A0, 0x20, 0xA0, 0x150, 0x1F7 },
    { 0x140, 0x100, 0x170, 0x1A0, 0xC0, 0xA0, 0x160, 0x1F7 },
    { 0x140, 0x100, 0x140, 0x1A4, 0, 0xA4, 0x170, 0x1F7 },
    { 0x140, 0x100, 0x150, 0x1A4, 0x40, 0xA4, 0x150, 0x1F6 },
    { 0x140, 0x100, 0x158, 0x1A4, 0x60, 0xA4, 0x160, 0x1F6 },
    { 0x140, 0x100, 0x156, 0x134, 0x58, 0x34, 0x170, 0x1F6 },
};
u16 actor0Talk0Actions[] = { 0x7A26, 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x17), 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7A0E, 1, CODES_END };
u16 actor3Talk0Actions[] = { 0x7A0D, 1, CODES_END };
u16 actor4Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor5Talk0Actions[] = { ITEM(1, 0x46), 1, FLAG(2, 0x26), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor46Talk0Actions[] = { 0x7A42, 1, CODES_END };
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
    { NULL, actor5Talk0Actions, 0x112 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0xF7 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0xFC },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0xFA },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0xF8 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0xF9 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0xFD },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x102 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x100 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0xFE },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xFF },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x106 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x103 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x105 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x104 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x108 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x10E },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x10C },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x109 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x10A },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x10B },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0xFB },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x101 },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x107 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { NULL, NULL, 0x10D },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, actor46Talk0Actions, 1 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0x174 },
    { NULL, NULL, 0 },
};
u16 actor5Conditions[] = { FLAG(2, 0x26), 0, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor40Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor42Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor43Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor44Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor45Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor46Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
u16 actor47Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x14, 4, 1073, 798, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x15, 5, 1233, 881, 1 };
FieldActorEntry actor2 = { NULL, actor2Talks, 0x16, 6, 705, 597, 1 };
FieldActorEntry actor3 = { NULL, actor3Talks, 0x17, 7, 626, 557, 1 };
FieldActorEntry actor4 = { NULL, actor4Talks, 0x18, 8, 1283, 541, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x21, 9, 1041, 881, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x25, 0xA, 593, 702, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x26, 0xB, 745, 777, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x27, 0xC, 290, 490, 7 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x39, 0xD, 721, 256, 7 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x9D, 0xE, 593, 702, 1 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x9E, 0xF, 745, 777, 1 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0x9F, 0x10, 290, 490, 7 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0xA0, 0x11, 721, 256, 7 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0xCE, 0x12, 1041, 657, 3 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0xCE, 0x12, 1041, 657, 3 };
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
    &actor44,
    &actor45,
    &actor46,
    &actor47,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 6, 0, 1273, 492, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 0xB, 6, 0, 1277, 496, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 1, 0x34, 0x3B, 6, 0, 1184, 497, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 2, 0, 3, 6, 0, 1180, 483, 0, 0 },
    { 1, 0, 0x40, 6, 3, 0, 0, 0, 0, 0, 1272, 754, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 1104, 797, 832, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 325, 419, 441, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 388, 386, 410, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26E, 0x344, 0x196, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26E, 0x318, 0x264, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x26E, 0x404, 0x1F6, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x49E, 0x2F0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x48F, 0x348, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1240, script1240, EVENT_TEXT(6), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
