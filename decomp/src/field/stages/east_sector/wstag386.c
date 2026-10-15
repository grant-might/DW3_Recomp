/*
 * WSTAG386: Duel Island, East Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG385.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x57D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x58D
#endif
/* Sets the stage up: its map, actors and events, playing BGM_0008 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x32500, 0x36900};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0xA;
    FIELDSTG_state.music = MUSIC(0xA, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1436[] = {
    SCRIPT_WALK(2, 0x1F7, 0x214, 3),
    SCRIPT_PLACE(0x145, 0x1E0, 0x209),
    SCRIPT_POSE(0x145, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x145, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x145, 0x1D5, 0x202, 3),
    SCRIPT_WAIT_WALK(0x145),
    SCRIPT_POSE(0x145, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x145, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x145, 0x1F8, 0x1F0, 5),
    SCRIPT_WAIT_WALK(0x145),
    SCRIPT_POSE(0x145, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x145, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x145, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1438[] = {
    SCRIPT_WALK(2, 0x191, 0x1E1, 1),
    SCRIPT_PLACE(0x144, 0x177, 0x1ED),
    SCRIPT_POSE(0x144, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x144, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x144, 0x165, 0x1F5, 1),
    SCRIPT_WAIT_WALK(0x144),
    SCRIPT_POSE(0x144, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x144, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x144, 0x141, 0x1E5, 3),
    SCRIPT_WAIT_WALK(0x144),
    SCRIPT_POSE(0x144, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x144, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x144, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1440[] = {
    SCRIPT_WALK(2, 0x128, 0x1FC, 3),
    SCRIPT_PLACE(0x143, 0x110, 0x1F1),
    SCRIPT_POSE(0x143, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x143, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x143, 0xCC, 0x1CE, 3),
    SCRIPT_WAIT_WALK(0x143),
    SCRIPT_POSE(0x143, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x143, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x143, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1442[] = {
    SCRIPT_WALK(2, 0x140, 0x180, 3),
    SCRIPT_PLACE(0x129, 0x129, 0x175),
    SCRIPT_POSE(0x129, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x129, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x129, 0x155, 0x15D, 5),
    SCRIPT_WAIT_WALK(0x129),
    SCRIPT_POSE(0x129, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x129, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x129, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1444[] = {
    SCRIPT_WALK(2, 0xF0, 0x118, 3),
    SCRIPT_PLACE(0x12A, 0xD8, 0x10D),
    SCRIPT_POSE(0x12A, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x12A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x12A, 0x104, 0xF7, 5),
    SCRIPT_WAIT_WALK(0x12A),
    SCRIPT_POSE(0x12A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x12A, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x12A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1446[] = {
    SCRIPT_WALK(2, 0xB7, 0xBD, 3),
    SCRIPT_PLACE(0xC0, 0x9F, 0xB1),
    SCRIPT_POSE(0xC0, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0xC0, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xC0, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xC0, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xBF, 0xC1, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_LEAVE(0x297, 0x250, 0x278, 7),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x172, 0x1B3, 0xC8, 0xB3, 0x170, 0x1F8 },
    { 0x180, 0x100, 0x1AC, 0x100, 0x1B0, 0, 0x140, 0x1F7 },
    { 0x140, 0x100, 0x16A, 0x1C5, 0xA8, 0xC5, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x180, 0x178, 0x100, 0x78, 0x170, 0x1F7 },
    { 0x180, 0x100, 0x188, 0x178, 0x120, 0x78, 0x140, 0x1F6 },
    { 0x180, 0x100, 0x190, 0x178, 0x140, 0x78, 0x150, 0x1F6 },
    { 0x180, 0x100, 0x198, 0x178, 0x160, 0x78, 0x170, 0x1F6 },
    { 0x180, 0x100, 0x1A0, 0x178, 0x180, 0x78, 0x140, 0x1F5 },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x150, 0x1F5 },
    { 0x180, 0x100, 0x1A8, 0x178, 0x1A0, 0x78, 0x170, 0x1F5 },
    { 0x180, 0x100, 0x1B0, 0x178, 0x1C0, 0x78, 0x140, 0x1F4 },
    { 0x180, 0x100, 0x180, 0x1A0, 0x100, 0xA0, 0x150, 0x1F4 },
    { 0x180, 0x100, 0x188, 0x1A0, 0x120, 0xA0, 0x170, 0x1F4 },
    { 0x180, 0x100, 0x190, 0x1A0, 0x140, 0xA0, 0x140, 0x1F3 },
    { 0x180, 0x100, 0x198, 0x1A0, 0x160, 0xA0, 0x150, 0x1F3 },
    { 0x180, 0x100, 0x1A0, 0x1A0, 0x180, 0xA0, 0x160, 0x1F3 },
    { 0x180, 0x100, 0x1A8, 0x1A0, 0x1A0, 0xA0, 0x170, 0x1F3 },
};
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x54), 1, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x54), 0, CODES_END };
u16 actor8Talk1Actions[] = { CARD_BATTLE(0x25, 0), 1, CODES_END };
u16 actor8Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1C, 0x54), 0,
    CODES_END,
};
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk3Conditions[] = {
    FLAG(0x1C, 0x54), 0,
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    CODES_END,
};
u16 actor8Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    ITEM(0, 0x14), 1,
    FLAG(0x1C, 0x54), 1,
    FLAG(0x1A, 0x40), 0,
    FLAG(0x1A, 0x3F), 0,
    FLAG(0x1A, 0x3E), 0,
    FLAG(0x1A, 0x3D), 0,
    FLAG(0x1A, 0x3C), 0,
    START_EVENT(0x6C), 1,
    CODES_END,
};
u16 actor9Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x56), 1, CODES_END };
u16 actor9Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1C, 0x56), 0, CODES_END };
u16 actor9Talk1Actions[] = { CARD_BATTLE(0x25, 1), 1, CODES_END };
u16 actor9Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1C, 0x56), 0,
    CODES_END,
};
u16 actor9Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor9Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1C, 0x56), 0,
    CODES_END,
};
u16 actor9Talk3Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, FLAG(0x1C, 0x56), 1, CODES_END };
u16 actor11Talk0Conditions[] = { FLAG(0x1A, 0x3F), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor11Talk1Conditions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0x1A, 0x3F), 0,
    ITEM(0, 0x18D), 0,
    CODES_END,
};
u16 actor11Talk1Actions[] = { FLAG(0x1C, 0x49), 1, FLAG(0x1C, 0x42), 1, CODES_END };
u16 actor11Talk2Conditions[] = {
    FLAG(0, 0x11), 0,
    ITEM(0, 0x18D), 1,
    FLAG(0x1A, 0x3F), 0,
    FLAG(0x1C, 0x4A), 0,
    CODES_END,
};
u16 actor11Talk2Actions[] = { CARD_BATTLE(0x27, 0), 1, FLAG(0x1C, 0x4A), 1, CODES_END };
u16 actor11Talk3Conditions[] = {
    FLAG(0, 0x11), 0,
    ITEM(0, 0x18D), 1,
    FLAG(0x1C, 0x4A), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3F), 0,
    CODES_END,
};
u16 actor11Talk3Actions[] = { CARD_BATTLE(0x27, 0), 1, CODES_END };
u16 actor11Talk4Conditions[] = {
    FLAG(0, 0x11), 1,
    ITEM(0, 0x18D), 1,
    FLAG(0x1C, 0x4A), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3F), 0,
    CODES_END,
};
u16 actor11Talk4Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor11Talk5Conditions[] = {
    FLAG(0, 0x11), 1,
    ITEM(0, 0x18D), 1,
    FLAG(0x1C, 0x4A), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3F), 0,
    CODES_END,
};
u16 actor11Talk5Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3F), 1,
    START_EVENT(0x59), 1,
    CODES_END,
};
u16 actor12Talk0Conditions[] = { FLAG(0x1A, 0x3F), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3F), 0, CODES_END };
u16 actor12Talk1Actions[] = { CARD_BATTLE(0x27, 1), 1, CODES_END };
u16 actor12Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3F), 0,
    CODES_END,
};
u16 actor12Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor12Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3F), 0,
    CODES_END,
};
u16 actor12Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x59), 1,
    FLAG(0x1A, 0x3F), 1,
    CODES_END,
};
u16 actor13Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x40), 1, CODES_END };
u16 actor13Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x40), 0, CODES_END };
u16 actor13Talk1Actions[] = { CARD_BATTLE(0x26, 0), 1, CODES_END };
u16 actor13Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x40), 0,
    CODES_END,
};
u16 actor13Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor13Talk3Conditions[] = {
    FLAG(0, 0x10), 1,
    FLAG(0, 0x11), 1,
    FLAG(0x1A, 0x40), 0,
    CODES_END,
};
u16 actor13Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x5A), 1,
    FLAG(0x1A, 0x40), 1,
    CODES_END,
};
u16 actor14Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x40), 1, CODES_END };
u16 actor14Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x40), 0, CODES_END };
u16 actor14Talk1Actions[] = { CARD_BATTLE(0x26, 1), 1, CODES_END };
u16 actor14Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x40), 0,
    CODES_END,
};
u16 actor14Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x40), 0,
    CODES_END,
};
u16 actor14Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x5A), 1,
    FLAG(0x1A, 0x40), 1,
    CODES_END,
};
u16 actor15Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3E), 1, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3E), 0, CODES_END };
u16 actor15Talk1Actions[] = { CARD_BATTLE(0x28, 0), 1, CODES_END };
u16 actor15Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3E), 0,
    CODES_END,
};
u16 actor15Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3E), 0,
    CODES_END,
};
u16 actor15Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x58), 1,
    FLAG(0x1A, 0x3E), 1,
    CODES_END,
};
u16 actor16Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3E), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3E), 0, CODES_END };
u16 actor16Talk1Actions[] = { CARD_BATTLE(0x28, 1), 1, CODES_END };
u16 actor16Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3E), 0,
    CODES_END,
};
u16 actor16Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor16Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3E), 0,
    CODES_END,
};
u16 actor16Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x58), 1,
    FLAG(0x1A, 0x3E), 1,
    CODES_END,
};
u16 actor17Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3D), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3D), 0, CODES_END };
u16 actor17Talk1Actions[] = { CARD_BATTLE(0x29, 0), 1, CODES_END };
u16 actor17Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3D), 0,
    CODES_END,
};
u16 actor17Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor17Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3D), 0,
    CODES_END,
};
u16 actor17Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x57), 1,
    FLAG(0x1A, 0x3D), 1,
    CODES_END,
};
u16 actor18Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3D), 1, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3D), 0, CODES_END };
u16 actor18Talk1Actions[] = { CARD_BATTLE(0x29, 1), 1, CODES_END };
u16 actor18Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3D), 0,
    CODES_END,
};
u16 actor18Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor18Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3D), 0,
    CODES_END,
};
u16 actor18Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x57), 1,
    FLAG(0x1A, 0x3D), 1,
    CODES_END,
};
u16 actor19Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3C), 1, CODES_END };
u16 actor19Talk1Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3C), 0, CODES_END };
u16 actor19Talk1Actions[] = { CARD_BATTLE(0x2A, 0), 1, CODES_END };
u16 actor19Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3C), 0,
    CODES_END,
};
u16 actor19Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3C), 0,
    CODES_END,
};
u16 actor19Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x56), 1,
    FLAG(0x1A, 0x3C), 1,
    CODES_END,
};
u16 actor20Talk0Conditions[] = { FLAG(0, 0x11), 0, FLAG(0x1A, 0x3C), 1, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0x1A, 0x3C), 0, FLAG(0, 0x11), 0, CODES_END };
u16 actor20Talk1Actions[] = { CARD_BATTLE(0x2A, 1), 1, CODES_END };
u16 actor20Talk2Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 0,
    FLAG(0x1A, 0x3C), 0,
    CODES_END,
};
u16 actor20Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor20Talk3Conditions[] = {
    FLAG(0, 0x11), 1,
    FLAG(0, 0x10), 1,
    FLAG(0x1A, 0x3C), 0,
    CODES_END,
};
u16 actor20Talk3Actions[] = {
    FLAG(0, 0x11), 0,
    FLAG(0, 0x10), 0,
    START_EVENT(0x56), 1,
    FLAG(0x1A, 0x3C), 1,
    CODES_END,
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x34B },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x330 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x349 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x34A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x339 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x336 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0x337 },
    { actor8Talk3Conditions, actor8Talk3Actions, 0x338 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x33D },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x33A },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x33B },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x33C },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x341 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x330 },
    { actor11Talk1Conditions, actor11Talk1Actions, 0x332 },
    { actor11Talk2Conditions, actor11Talk2Actions, 0x333 },
    { actor11Talk3Conditions, actor11Talk3Actions, 0x334 },
    { actor11Talk4Conditions, actor11Talk4Actions, 0x32E },
    { actor11Talk5Conditions, actor11Talk5Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x330 },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x32F },
    { actor12Talk2Conditions, actor12Talk2Actions, 0x32E },
    { actor12Talk3Conditions, actor12Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x341 },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x33E },
    { actor13Talk2Conditions, actor13Talk2Actions, 0x33F },
    { actor13Talk3Conditions, actor13Talk3Actions, 0x340 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x341 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x33E },
    { actor14Talk2Conditions, actor14Talk2Actions, 0x33F },
    { actor14Talk3Conditions, actor14Talk3Actions, 0x340 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0x330 },
    { actor15Talk1Conditions, actor15Talk1Actions, 0x32F },
    { actor15Talk2Conditions, actor15Talk2Actions, 0x32E },
    { actor15Talk3Conditions, actor15Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, NULL, 0x330 },
    { actor16Talk1Conditions, actor16Talk1Actions, 0x32F },
    { actor16Talk2Conditions, actor16Talk2Actions, 0x32E },
    { actor16Talk3Conditions, actor16Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, NULL, 0x330 },
    { actor17Talk1Conditions, actor17Talk1Actions, 0x32F },
    { actor17Talk2Conditions, actor17Talk2Actions, 0x32E },
    { actor17Talk3Conditions, actor17Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, NULL, 0x330 },
    { actor18Talk1Conditions, actor18Talk1Actions, 0x32F },
    { actor18Talk2Conditions, actor18Talk2Actions, 0x32E },
    { actor18Talk3Conditions, actor18Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0x330 },
    { actor19Talk1Conditions, actor19Talk1Actions, 0x32F },
    { actor19Talk2Conditions, actor19Talk2Actions, 0x32E },
    { actor19Talk3Conditions, actor19Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, NULL, 0x330 },
    { actor20Talk1Conditions, actor20Talk1Actions, 0x32F },
    { actor20Talk2Conditions, actor20Talk2Actions, 0x32E },
    { actor20Talk3Conditions, actor20Talk3Actions, 0x335 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x34C },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x34C },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor2Conditions[] = { FLAG(0x1A, 0x3F), 1, CODES_END };
u16 actor3Conditions[] = { FLAG(0x1A, 0x3E), 1, CODES_END };
u16 actor4Conditions[] = { FLAG(0x1A, 0x3D), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x1A, 0x3C), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(8), 1, CODES_END };
u16 actor8Conditions[] = { ITEM(0, 0x14), 0, CODES_END };
u16 actor9Conditions[] = { ITEM(0, 0x14), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1A, 0x40), 1, CODES_END };
u16 actor11Conditions[] = { FLAG(0x1A, 0x3F), 0, ITEM(0, 0x14), 0, CODES_END };
u16 actor12Conditions[] = { FLAG(0x1A, 0x3F), 0, ITEM(0, 0x14), 1, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1A, 0x40), 0, ITEM(0, 0x14), 0, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1A, 0x40), 0, ITEM(0, 0x14), 1, CODES_END };
u16 actor15Conditions[] = { FLAG(0x1A, 0x3E), 0, ITEM(0, 0x14), 0, CODES_END };
u16 actor16Conditions[] = { ITEM(0, 0x14), 1, FLAG(0x1A, 0x3E), 0, CODES_END };
u16 actor17Conditions[] = { FLAG(0x1A, 0x3D), 0, ITEM(0, 0x14), 0, CODES_END };
u16 actor18Conditions[] = { FLAG(0x1A, 0x3D), 0, ITEM(0, 0x14), 1, CODES_END };
u16 actor19Conditions[] = { FLAG(0x1A, 0x3C), 0, ITEM(0, 0x14), 0, CODES_END };
u16 actor20Conditions[] = { FLAG(0x1A, 0x3C), 0, ITEM(0, 0x14), 1, CODES_END };
u16 actor21Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor22Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0x4B, 4, 588, 1083, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x5B, 5, 993, 913, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x61, 6, 341, 349, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x62, 7, 204, 462, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0xB8, 8, 321, 485, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0xB9, 9, 504, 496, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0xBA, 0xA, 769, 209, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0xBB, 0xB, 929, 417, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0xC0, 0xC, 159, 177, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0xC0, 0xC, 159, 177, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0xC1, 0xD, 260, 247, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x129, 0xE, 297, 373, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x129, 0xE, 297, 373, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x12A, 0xF, 216, 269, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x12A, 0xF, 216, 269, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x143, 0x10, 272, 497, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x143, 0x10, 272, 497, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x144, 0x11, 375, 493, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x144, 0x11, 375, 493, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x145, 0x12, 480, 521, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x145, 0x12, 480, 521, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x16B, 0x13, 501, 549, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x16C, 0x14, 522, 538, 7 };
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
    { 1, 0, 0x40, 2, 0x47, 2, 0, 5, 8, 0, 85, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 83, 135, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 91, 131, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 99, 127, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 107, 123, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 115, 119, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 123, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 131, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 89, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 139, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 85, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 147, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 81, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 155, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 77, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 5, 8, 0, 163, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 115, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 87, 133, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 95, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 107, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 103, 125, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 103, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 111, 121, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 99, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 119, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 95, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 127, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 91, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 135, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 87, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 143, 105, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 83, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 151, 101, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 159, 97, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 75, 0, 0 },
    { 1, 0, 0x40, 2, 0x49, 2, 0, 5, 8, 0, 167, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0x4A, 2, 0, 7, 4, 0, 103, 201, 0, 0 },
    { 1, 0, 0x40, 2, 0x4B, 2, 0, 7, 4, 0, 105, 205, 0, 0 },
    { 1, 0, 0x80, 2, 1, 0, 0, 0, 0, 0, 768, 128, 0, 0 },
    { 1, 0, 0x78, 2, 3, 0, 0, 0, 0, 0, 896, 256, 0, 0 },
    { 1, 0, 0x55, 2, 4, 0, 0, 0, 0, 0, 984, 299, 0, 0 },
    { 1, 0, 0x33, 2, 5, 0, 0, 0, 0, 0, 278, 813, 0, 0 },
    { 1, 0, 0x1E, 2, 6, 0, 0, 0, 0, 0, 323, 829, 0, 0 },
    { 1, 0, 0x45, 2, 2, 0, 0, 0, 0, 0, 880, 156, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 220, 180, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 232, 138, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 297, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x4A, 2, 0, 7, 4, 0, 300, 284, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 168, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 220, 184, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 234, 143, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 235, 299, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 280, 241, 0, 0 },
    { 1, 0, 0x40, 6, 0x4B, 2, 0, 7, 4, 0, 300, 270, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 318, 861, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 427, 415, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 543, 989, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 766, 529, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 903, 778, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 81, 1002, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 449, 833, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 518, 250, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 885, 1060, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1056, 440, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1108, 931, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 477, 577, 626, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0xC0, 0xC2, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xCF, 0x106, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x110, 0x12B, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x11F, 0x16E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x100, 0x192, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0xF1, 0x1D6, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x330, 0xEA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x33F, 0x12E, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x2DE, 0x141, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x2CE, 0x186, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 0x12, 1 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E0, 0x240, 0xD8, 1, 0, 0x12, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1436, script1436, EVENT_TEXT(0x2C), NULL, NULL },
    { 1438, script1438, EVENT_TEXT(0x2D), NULL, NULL },
    { 1440, script1440, EVENT_TEXT(0x2E), NULL, NULL },
    { 1442, script1442, EVENT_TEXT(0x2F), NULL, NULL },
    { 1444, script1444, EVENT_TEXT(0x30), NULL, NULL },
    { 1446, script1446, EVENT_TEXT(0x32), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
