/*
 * WSTAG331: Central Park, Central Sector. On the Amaterasu server; its Asuka
 * server twin is WSTAG330.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* After event 920, with Teddy: keeps it from playing again */
void endEvent920(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x5C), 1);
}

/* After event 970, with Kail and Kurt: keeps it from playing again */
void endEvent970(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0x6A), 1);
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xE9)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x559
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE1)
#define EVENT_TEXT_FILE 0x127
#define STAGE_FILE 0x569
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing FIELDBGM;
 * BGM_0029 instead except at story step 0x26 once flag 0x1A0A is set
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x49B00, 0x1E300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 9;
    FIELDSTG_state.music = MUSIC(9, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_AREAS, STAGE_FILE << 16 | 3);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress != 0x26 || FLAGS_00.checkCondition(FLAG(0x1A, 0xA), 0) != 0) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script920[] = {
    SCRIPT_WALK(2, 0x514, 0x184, 5),
    SCRIPT_PLACE(0xC, 0x460, 0x200),
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0xC, 0x4A6, 0x1DC, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_TALK(0, 2, 0xC, 2),
    SCRIPT_POSE(0xC, 0x42, 7),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(0xC, 0x4B6, 0x1D4, 5),
    SCRIPT_WAIT_WALK(0xC),
    SCRIPT_WALK(0xC, 0x4F4, 0x194, 5),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0xC, 1),
    SCRIPT_POSE(0xC, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xC, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x272, 0x2DA, 0xEC, 4),
    SCRIPT_END,
};
s16 script970[] = {
    SCRIPT_WALK(2, 0x572, 0x142, 5),
#if VERSION_US
    SCRIPT_PLACE(0xB, 0x5A8, 0xEC),
    SCRIPT_POSE(0xB, 1, 7),
#endif
    SCRIPT_PLACE(0xB2, 0x5A8, 0xEC),
    SCRIPT_POSE(0xB2, 1, 7),
    SCRIPT_PLACE(0x13D, 0x5C0, 0xE0),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
#if VERSION_EU || VERSION_JP
    SCRIPT_FOLLOW(0, 0xB2),
    SCRIPT_POSE(0xB2, 1, 1),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 0xB2),
    SCRIPT_WAIT(0x3C),
#endif
    SCRIPT_WALK(2, 0x5C0, 0xF8, 5),
#if VERSION_US
    SCRIPT_POSE(0xB, 1, 0),
#endif
    SCRIPT_POSE(0xB2, 1, 0),
#if VERSION_US
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(0xB, 1, 7),
#elif VERSION_EU || VERSION_JP
    SCRIPT_COMMAND(0x323, 0x326, 0xB2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
#endif
    SCRIPT_POSE(0xB2, 1, 7),
    SCRIPT_POSE(0x13D, 1, 0),
#if VERSION_US
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
#endif
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 1, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 1, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0x13D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 3, 0xB, 0),
    SCRIPT_POSE(0xB, 0xC, 7),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 3, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
#if VERSION_US
    SCRIPT_POSE(0xB, 1, 7),
#endif
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 5, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 5, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 7, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 7, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 0x13, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0x13, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 0xA, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0xA, 0xB2, 0),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_FOLLOW(0, 0xB),
#elif VERSION_EU || VERSION_JP
    SCRIPT_FOLLOW(0, 0xB2),
#endif
    SCRIPT_WALK(2, 0x5E0, 0xE8, 5),
#if VERSION_US
    SCRIPT_POSE(0xB, 1, 6),
#endif
    SCRIPT_POSE(0xB2, 1, 6),
    SCRIPT_POSE(0x13D, 1, 7),
#if VERSION_US
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
#elif VERSION_EU || VERSION_JP
    SCRIPT_COMMAND(0x323, 0x325, 0xB2),
#endif
    SCRIPT_WAIT_WALK(2),
#if VERSION_US
    SCRIPT_TALK(0, 0xC, 0xB, 0),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0xC, 0xB2, 0),
#endif
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 2, 0),
    SCRIPT_POSE(2, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_COMMAND(0x323, 0x327, 0xB),
#elif VERSION_EU || VERSION_JP
    SCRIPT_COMMAND(0x323, 0x327, 0xB2),
#endif
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 0xE, 0xB, 1),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0xE, 0xB2, 1),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 2, 2),
    SCRIPT_POSE(2, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x678, 0x9C, 5),
#if VERSION_US
    SCRIPT_POSE(0xB, 1, 5),
#endif
    SCRIPT_POSE(0xB2, 1, 5),
    SCRIPT_POSE(0x13D, 1, 5),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(1, 0x16, 0x13D, 0),
#if VERSION_US
    SCRIPT_TALK(0, 0x10, 0xB, 1),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0x10, 0xB2, 1),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 0x11, 0xB, 1),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0x11, 0xB2, 1),
#endif
    SCRIPT_WAIT_BOX,
#if VERSION_US
    SCRIPT_POSE(0xB, 1, 7),
#endif
    SCRIPT_POSE(0xB2, 1, 7),
    SCRIPT_POSE(0x13D, 1, 1),
    SCRIPT_WAIT(0x1E),
#if VERSION_US
    SCRIPT_TALK(0, 0x17, 0xB, 3),
#elif VERSION_EU || VERSION_JP
    SCRIPT_TALK(0, 0x17, 0xB2, 3),
#endif
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x2D7, 1, 1, 0),
    SCRIPT_END,
};
AreaBattle area0Battle0 = { 127, 1, MUSIC(2, 0) };
AreaBattle area0Battle1 = { 127, 1, MUSIC(2, 0) };
AreaBattle area0Battle2 = { 127, 1, MUSIC(2, 0) };
AreaBattle area0Battle3 = { 127, 1, MUSIC(2, 0) };
AreaBattle area0Battle4 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle5 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle6 = { 126, 1, MUSIC(2, 0) };
AreaBattle area0Battle7 = { 126, 1, MUSIC(2, 0) };
BattleList area0Battles = {
    3,
    { &area0Battle0, &area0Battle1, &area0Battle2, &area0Battle3,
      &area0Battle4, &area0Battle5, &area0Battle6, &area0Battle7 },
};
AreaBattle area1Battle0 = { 127, 1, MUSIC(2, 0) };
AreaBattle area1Battle1 = { 127, 1, MUSIC(2, 0) };
AreaBattle area1Battle2 = { 127, 1, MUSIC(2, 0) };
AreaBattle area1Battle3 = { 127, 1, MUSIC(2, 0) };
AreaBattle area1Battle4 = { 126, 1, MUSIC(2, 0) };
AreaBattle area1Battle5 = { 126, 1, MUSIC(2, 0) };
AreaBattle area1Battle6 = { 126, 1, MUSIC(2, 0) };
AreaBattle area1Battle7 = { 126, 1, MUSIC(2, 0) };
BattleList area1Battles = {
    1,
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
AreaBattle area3Battle0 = { 224, 1, MUSIC(2, 0) };
AreaBattle area3Battle1 = { 272, 1, MUSIC(3, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 329, 1, MUSIC(2, 0) };
AreaBattle area3Battle4 = { 330, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 93, 1, MUSIC(2, 0) };
AreaBattle area3Battle7 = { 180, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 92, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
#if VERSION_US
    { 0x180, 0x100, 0x180, 0x128, 0x100, 0x28, 0x150, 0x1FF },
#endif
    { 0x180, 0x100, 0x188, 0x128, 0x120, 0x28, 0x160, 0x1FF },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x170, 0x1FF },
    { 0x140, 0x100, 0x174, 0x180, 0xD0, 0x80, 0x140, 0x1FE },
    { 0x140, 0x100, 0x16A, 0x1B0, 0xA8, 0xB0, 0x150, 0x1FE },
    { 0x140, 0x100, 0x178, 0x120, 0xE0, 0x20, 0x160, 0x1FE },
    { 0x180, 0x100, 0x190, 0x128, 0x140, 0x28, 0x170, 0x1FE },
    { 0x180, 0x100, 0x188, 0x100, 0x120, 0, 0x150, 0x1FD },
    { 0x180, 0x100, 0x190, 0x100, 0x140, 0, 0x160, 0x1FD },
    { 0x180, 0x100, 0x198, 0x100, 0x160, 0, 0x170, 0x1FD },
    { 0x180, 0x100, 0x1A0, 0x100, 0x180, 0, 0x140, 0x1FC },
    { 0x180, 0x100, 0x198, 0x128, 0x160, 0x28, 0x150, 0x1FC },
    { 0x180, 0x100, 0x1A0, 0x128, 0x180, 0x28, 0x160, 0x1FC },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x180, 0x100, 0x1A8, 0x100, 0x1A0, 0, 0x170, 0x1FC },
    { 0x180, 0x100, 0x1B0, 0x100, 0x1C0, 0, 0x140, 0x1FB },
    { 0x180, 0x100, 0x1A8, 0x128, 0x1A0, 0x28, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1B0, 0x128, 0x1C0, 0x28, 0x160, 0x1FB },
    { 0x180, 0x100, 0x180, 0x148, 0x100, 0x48, 0x170, 0x1FB },
    { 0x180, 0x100, 0x188, 0x148, 0x120, 0x48, 0x140, 0x1FA },
    { 0x180, 0x100, 0x190, 0x148, 0x140, 0x48, 0x150, 0x1FA },
    { 0x180, 0x100, 0x198, 0x148, 0x160, 0x48, 0x160, 0x1FA },
    { 0x180, 0x100, 0x1A0, 0x148, 0x180, 0x48, 0x170, 0x1FA },
    { 0x180, 0x100, 0x1A8, 0x148, 0x1A0, 0x48, 0x140, 0x1F9 },
};
/* The Super Charge the player finds: taken (flag 0x021D), given and special action 0x13 */
u16 superChargeActions[] = { FLAG(2, 0x1D), 1, ITEM(1, 0x2C), 1, SPECIAL(0x13), 1, CODES_END };
u16 talks1Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 talks1Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 talks1Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 talks1Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 talks1Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 talks3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 talks3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 talks3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 0, CODES_END };
u16 talks3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(9), 1, PARTY_STAT(0xB), 0, CODES_END };
u16 talks3Talk2Actions[] = { CARD_BATTLE(0x22, 0), 1, CODES_END };
u16 talks3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x1B), 0,
    CODES_END,
};
u16 talks3Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x1B), 1, CODES_END };
u16 talks3Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(9), 1,
    PARTY_STAT(0xB), 1,
    FLAG(0xE, 0x1B), 1,
    CODES_END,
};
u16 talks4Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 talks4Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 talks4Talk1Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x3C), 0, CODES_END };
u16 talks4Talk1Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0x3C), 1, CODES_END };
u16 talks4Talk2Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x3C), 1, PARTY_STAT(0xD), 0, CODES_END };
u16 talks4Talk3Conditions[] = { FLAG(0, 0), 1, FLAG(0xE, 0x3C), 1, PARTY_STAT(0xD), 1, CODES_END };
u16 talks4Talk3Actions[] = { CARD_BATTLE(0x22, 1), 1, CODES_END };
u16 talks5Talk0Actions[] = { 0x940D, 1, CODES_END };
u16 talks6Talk0Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 0, CODES_END };
u16 talks6Talk0Actions[] = { 0x9407, 1, CODES_END };
u16 talks6Talk1Conditions[] = { ITEM(0, 0x28), 1, ITEM(0, 0x29), 1, CODES_END };
u16 talks6Talk1Actions[] = { 0x9408, 1, CODES_END };
FieldTalk talks0[] = {
#if VERSION_US
    { NULL, NULL, 0x39A },
#elif VERSION_EU || VERSION_JP
    { NULL, superChargeActions, 0x17D },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
#if VERSION_US
    { NULL, superChargeActions, 0x17D },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x21 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x21 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x24 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x24 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x1B },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x1B },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x1E },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x1E },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x26 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x26 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x2A },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x2A },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x28 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x28 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x29 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x29 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x15 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x15 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x18 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x18 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x27 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
#if VERSION_US
    { NULL, NULL, 0x27 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x13 },
#endif
    { NULL, NULL, 0 },
};
/* The sign's talks: the USA version's actor 14, the European actor 13 (actor13Talks) */
#if VERSION_US
FieldTalk signTalks[] = {
    { NULL, NULL, 0x13 },
    { NULL, NULL, 0 },
};
#endif
FieldTalk talks1[] = {
    { talks1Talk0Conditions, NULL, 0x248 },
    { talks1Talk1Conditions, talks1Talk1Actions, 0x249 },
    { talks1Talk2Conditions, talks1Talk2Actions, 0x24A },
    { NULL, NULL, 0 },
};
FieldTalk talks2[] = {
    { NULL, NULL, 0x24B },
    { NULL, NULL, 0 },
};
FieldTalk talks3[] = {
    { talks3Talk0Conditions, talks3Talk0Actions, 0x244 },
    { talks3Talk1Conditions, NULL, 0x245 },
    { talks3Talk2Conditions, talks3Talk2Actions, 0x246 },
    { talks3Talk3Conditions, talks3Talk3Actions, 0x247 },
    { talks3Talk4Conditions, NULL, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk talks4[] = {
    { talks4Talk0Conditions, talks4Talk0Actions, 0x24C },
    { talks4Talk1Conditions, talks4Talk1Actions, 0x24D },
    { talks4Talk2Conditions, NULL, 0x24E },
    { talks4Talk3Conditions, talks4Talk3Actions, 0x24F },
    { NULL, NULL, 0 },
};
FieldTalk talks5[] = {
    { NULL, talks5Talk0Actions, 0x2D8 },
    { NULL, NULL, 0 },
};
FieldTalk talks6[] = {
    { talks6Talk0Conditions, talks6Talk0Actions, 0x2D8 },
    { talks6Talk1Conditions, talks6Talk1Actions, 0x2D8 },
    { NULL, NULL, 0 },
};
FieldTalk talks7[] = {
    { NULL, NULL, 0x22 },
    { NULL, NULL, 0 },
};
FieldTalk talks8[] = {
    { NULL, NULL, 0x20 },
    { NULL, NULL, 0 },
};
FieldTalk talks9[] = {
    { NULL, NULL, 0x25 },
    { NULL, NULL, 0 },
};
FieldTalk talks10[] = {
    { NULL, NULL, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk talks11[] = {
    { NULL, NULL, 0x16 },
    { NULL, NULL, 0 },
};
FieldTalk talks12[] = {
    { NULL, NULL, 0x14 },
    { NULL, NULL, 0 },
};
FieldTalk talks13[] = {
    { NULL, NULL, 0x19 },
    { NULL, NULL, 0 },
};
FieldTalk talks14[] = {
    { NULL, NULL, 0x17 },
    { NULL, NULL, 0 },
};
FieldTalk talks15[] = {
    { NULL, NULL, 0x1F },
    { NULL, NULL, 0 },
};
FieldTalk talks16[] = {
    { NULL, NULL, 0x1D },
    { NULL, NULL, 0 },
};
FieldTalk talks17[] = {
    { NULL, NULL, 0x1C },
    { NULL, NULL, 0 },
};
FieldTalk talks18[] = {
    { NULL, NULL, 0x1A },
    { NULL, NULL, 0 },
};
FieldTalk talks19[] = {
#if VERSION_US
    { NULL, NULL, 0x346 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x39A },
#endif
    { NULL, NULL, 0 },
};
FieldTalk talks20[] = {
#if VERSION_US
    { NULL, NULL, 0x347 },
#elif VERSION_EU || VERSION_JP
    { NULL, NULL, 0x346 },
#endif
    { NULL, NULL, 0 },
};
FieldTalk talks21[] = {
    { NULL, NULL, 0x347 },
    { NULL, NULL, 0 },
};
/* Here the USA version has Kail's conditions, the European one Kurt's talks */
#if VERSION_US
u16 kailConditions[] = { PROGRESS(0x27), 1, CODES_END };
#elif VERSION_EU || VERSION_JP
FieldTalk kurtTalks[] = {
    { NULL, NULL, 0x347 },
    { NULL, NULL, 0 },
};
#endif
u16 actorConditions0[] = { PROGRESS(0x24), 1, CODES_END };
u16 actorConditions1[] = { FLAG(2, 0x1D), 0, CODES_END };
u16 actorConditions2[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions3[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions4[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions5[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions6[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actorConditions7[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actorConditions8[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actorConditions9[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actorConditions10[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions11[] = { PROGRESS(0x26), 1, FLAG(0x1A, 0xA), 1, CODES_END };
u16 actorConditions12[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actorConditions13[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(4), 1, CODES_END };
u16 actorConditions14[] = { ITEM(0, 0x192), 0, SPECIAL(4), 1, CODES_END };
u16 actorConditions15[] = {
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    SPECIAL(4), 1,
    ITEM(0, 0x14), 0,
    CODES_END,
};
u16 actorConditions16[] = {
    SPECIAL(4), 1,
    FLAG(0, 0x11), 0,
    ITEM(0, 0x192), 1,
    ITEM(0, 0x14), 1,
    CODES_END,
};
u16 actorConditions17[] = { ITEM(0, 0x2A), 1, CODES_END };
u16 actorConditions18[] = { ITEM(0, 0x2A), 0, CODES_END };
u16 actorConditions19[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions20[] = { SPECIAL(0x1E), 1, FLAG(0x1A, 0xA), 0, CODES_END };
u16 actorConditions21[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions22[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actorConditions23[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions24[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actorConditions25[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions26[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actorConditions27[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions28[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
u16 actorConditions29[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actorConditions30[] = { FLAG(0x1A, 0xA), 0, SPECIAL(0x1E), 1, CODES_END };
#if VERSION_EU || VERSION_JP
u16 kailConditions[] = { PROGRESS(0x27), 1, CODES_END };
#endif
u16 actor33Conditions[] = { PROGRESS(0x28), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0x28), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0x27), 1, CODES_END };
#if VERSION_US
FieldActorEntry actor0 = { kailConditions, talks0, 0xB, 4, 1448, 236, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor0 = { actorConditions0, NULL, 0xC, 4, 0, 0, 1 };
#endif
#if VERSION_US
FieldActorEntry actor1 = { actorConditions0, NULL, 0xC, 5, 0, 0, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor1 = { actorConditions1, talks0, 0x21, 5, 737, 954, 1 };
#endif
#if VERSION_US
FieldActorEntry actor2 = { actorConditions1, actor2Talks, 0x21, 6, 737, 954, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor2 = { actorConditions2, actor2Talks, 0x25, 6, 1072, 665, 1 };
#endif
#if VERSION_US
FieldActorEntry actor3 = { actorConditions2, actor3Talks, 0x25, 7, 1072, 665, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor3 = { actorConditions3, actor3Talks, 0x26, 7, 864, 577, 5 };
#endif
#if VERSION_US
FieldActorEntry actor4 = { actorConditions3, actor4Talks, 0x26, 8, 864, 577, 5 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor4 = { actorConditions4, actor4Talks, 0x2D, 8, 688, 633, 1 };
#endif
#if VERSION_US
FieldActorEntry actor5 = { actorConditions4, actor5Talks, 0x2D, 9, 688, 633, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor5 = { actorConditions5, actor5Talks, 0x2E, 9, 913, 233, 3 };
#endif
#if VERSION_US
FieldActorEntry actor6 = { actorConditions5, actor6Talks, 0x2E, 0xA, 913, 233, 3 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor6 = { actorConditions6, actor6Talks, 0x30, 0xA, 401, 785, 1 };
#endif
#if VERSION_US
FieldActorEntry actor7 = { actorConditions6, actor7Talks, 0x30, 0xB, 401, 785, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor7 = { actorConditions7, actor7Talks, 0x31, 0xB, 545, 385, 7 };
#endif
#if VERSION_US
FieldActorEntry actor8 = { actorConditions7, actor8Talks, 0x31, 0xC, 545, 385, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor8 = { actorConditions8, actor8Talks, 0x34, 0xC, 560, 728, 7 };
#endif
#if VERSION_US
FieldActorEntry actor9 = { actorConditions8, actor9Talks, 0x34, 0xD, 560, 728, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor9 = { actorConditions9, actor9Talks, 0x38, 0xD, 864, 577, 5 };
#endif
#if VERSION_US
FieldActorEntry actor10 = { actorConditions9, actor10Talks, 0x38, 0xE, 864, 577, 5 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor10 = { actorConditions10, actor10Talks, 0x39, 0xE, 560, 728, 7 };
#endif
#if VERSION_US
FieldActorEntry actor11 = { actorConditions10, actor11Talks, 0x39, 0xF, 560, 728, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor11 = { actorConditions11, actor11Talks, 0x3A, 0xF, 401, 785, 1 };
#endif
#if VERSION_US
FieldActorEntry actor12 = { actorConditions11, actor12Talks, 0x3A, 0x10, 401, 785, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor12 = { actorConditions12, actor12Talks, 0x3A, 0xF, 1072, 665, 1 };
#endif
#if VERSION_US
FieldActorEntry actor13 = { actorConditions12, actor13Talks, 0x3A, 0x10, 1072, 665, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor13 = { NULL, actor13Talks, 0x3F, 0x10, 961, 681, 1 };
#endif
#if VERSION_US
FieldActorEntry actor14 = { NULL, signTalks, 0x3F, 0x11, 961, 681, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor14 = { actorConditions13, talks1, 0x45, 0x11, 1152, 793, 1 };
#endif
#if VERSION_US
FieldActorEntry actor15 = { actorConditions13, talks1, 0x45, 0x12, 1152, 793, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor15 = { actorConditions14, talks2, 0x45, 0x11, 1152, 793, 1 };
#endif
#if VERSION_US
FieldActorEntry actor16 = { actorConditions14, talks2, 0x45, 0x12, 1152, 793, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor16 = { actorConditions15, talks3, 0x45, 0x11, 1152, 793, 1 };
#endif
#if VERSION_US
FieldActorEntry actor17 = { actorConditions15, talks3, 0x45, 0x12, 1152, 793, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor17 = { actorConditions16, talks4, 0x45, 0x11, 1152, 793, 1 };
#endif
#if VERSION_US
FieldActorEntry actor18 = { actorConditions16, talks4, 0x45, 0x12, 1152, 793, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor18 = { actorConditions17, talks5, 0x89, 0x12, 923, 374, 7 };
#endif
#if VERSION_US
FieldActorEntry actor19 = { actorConditions17, talks5, 0x89, 0x13, 923, 374, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor19 = { actorConditions18, talks6, 0x89, 0x12, 923, 374, 7 };
#endif
#if VERSION_US
FieldActorEntry actor20 = { actorConditions18, talks6, 0x89, 0x13, 923, 374, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor20 = { actorConditions19, talks7, 0x9D, 0x13, 1072, 665, 1 };
#endif
#if VERSION_US
FieldActorEntry actor21 = { actorConditions19, talks7, 0x9D, 0x14, 1072, 665, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor21 = { actorConditions20, talks8, 0x9D, 0x13, 1072, 665, 1 };
#endif
#if VERSION_US
FieldActorEntry actor22 = { actorConditions20, talks8, 0x9D, 0x14, 1072, 665, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor22 = { actorConditions21, talks9, 0x9E, 0x14, 864, 577, 5 };
#endif
#if VERSION_US
FieldActorEntry actor23 = { actorConditions21, talks9, 0x9E, 0x15, 864, 577, 5 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor23 = { actorConditions22, talks10, 0x9E, 0x14, 864, 577, 5 };
#endif
#if VERSION_US
FieldActorEntry actor24 = { actorConditions22, talks10, 0x9E, 0x15, 864, 577, 5 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor24 = { actorConditions23, talks11, 0x9F, 0x15, 560, 728, 7 };
#endif
#if VERSION_US
FieldActorEntry actor25 = { actorConditions23, talks11, 0x9F, 0x16, 560, 728, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor25 = { actorConditions24, talks12, 0x9F, 0x15, 560, 728, 7 };
#endif
#if VERSION_US
FieldActorEntry actor26 = { actorConditions24, talks12, 0x9F, 0x16, 560, 728, 7 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor26 = { actorConditions25, talks13, 0xA0, 0x16, 401, 785, 1 };
#endif
#if VERSION_US
FieldActorEntry actor27 = { actorConditions25, talks13, 0xA0, 0x17, 401, 785, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor27 = { actorConditions26, talks14, 0xA0, 0x16, 401, 785, 1 };
#endif
#if VERSION_US
FieldActorEntry actor28 = { actorConditions26, talks14, 0xA0, 0x17, 401, 785, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor28 = { actorConditions27, talks15, 0xA1, 0x17, 913, 233, 3 };
#endif
#if VERSION_US
FieldActorEntry actor29 = { actorConditions27, talks15, 0xA1, 0x18, 913, 233, 3 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor29 = { actorConditions28, talks16, 0xA1, 0x17, 913, 233, 3 };
#endif
#if VERSION_US
FieldActorEntry actor30 = { actorConditions28, talks16, 0xA1, 0x18, 913, 233, 3 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor30 = { actorConditions29, talks17, 0xA2, 0x18, 688, 633, 1 };
#endif
#if VERSION_US
FieldActorEntry actor31 = { actorConditions29, talks17, 0xA2, 0x19, 688, 633, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor31 = { actorConditions30, talks18, 0xA2, 0x18, 688, 633, 1 };
#endif
#if VERSION_US
FieldActorEntry actor32 = { actorConditions30, talks18, 0xA2, 0x19, 688, 633, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor32 = { kailConditions, talks19, 0xB2, 0x19, 1448, 236, 7 };
#endif
#if VERSION_US
FieldActorEntry actor33 = { actor33Conditions, talks19, 0xB2, 0x1A, 1110, 595, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor33 = { actor33Conditions, talks20, 0xB2, 0x19, 1110, 595, 1 };
#endif
#if VERSION_US
FieldActorEntry actor34 = { actor34Conditions, talks20, 0x13D, 0x1B, 1088, 584, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor34 = { actor34Conditions, talks21, 0x13D, 0x1A, 1088, 584, 1 };
#endif
#if VERSION_US
FieldActorEntry actor35 = { actor35Conditions, talks21, 0x13D, 0x1B, 1472, 224, 1 };
#elif VERSION_EU || VERSION_JP
FieldActorEntry actor35 = { actor35Conditions, kurtTalks, 0x13D, 0x1A, 1472, 224, 1 };
#endif
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 787, 422, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 200, 248, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 408, 880, 0, 0 },
    { 1, 0, 0x40, 2, 9, 0, 0, 0, 0, 0, 1320, 887, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 51, 661, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 159, 547, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1084, 1101, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1091, 247, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 1, 0x2C, 0x3B, 0xA, 0, 1517, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 92, 596, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 226, 835, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 532, 1050, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 590, 68, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 881, 481, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 913, 1106, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1108, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x3C, 1, 0x3C, 0x46, 0xA, 0, 1357, 486, 0, 0 },
    { 1, 0, 0x72, 4, 0, 0, 0, 0, 0, 0, 765, 572, 665, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 951, 632, 679, 0 },
    { 1, 0, 0x7D, 4, 2, 0, 0, 0, 0, 0, 816, 122, 245, 0 },
    { 1, 0, 0x7D, 4, 3, 0, 0, 0, 0, 0, 800, 114, 238, 0 },
    { 1, 0, 0x7D, 4, 4, 0, 0, 0, 0, 0, 784, 106, 230, 0 },
    { 1, 0, 0x7D, 4, 5, 0, 0, 0, 0, 0, 767, 99, 223, 0 },
    { 1, 0, 0x46, 4, 0xF, 0, 0, 0, 0, 0, 1224, 424, 484, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 243, 754, 754, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 266, 606, 606, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 275, 778, 778, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 314, 630, 630, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 338, 538, 538, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 363, 654, 654, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 410, 678, 678, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 483, 930, 930, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 531, 906, 906, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 547, 674, 674, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 579, 882, 882, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 595, 650, 650, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 626, 474, 474, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 627, 858, 858, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 643, 994, 994, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 667, 454, 454, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 691, 490, 490, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 723, 826, 826, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 771, 802, 802, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 787, 938, 938, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 819, 778, 778, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 835, 194, 194, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 835, 914, 914, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 883, 890, 890, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 883, 938, 938, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 931, 914, 914, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 979, 378, 378, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 979, 890, 890, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1027, 402, 402, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1075, 426, 426, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1314, 642, 642, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1315, 690, 690, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1315, 738, 738, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 570, 570, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 618, 618, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 666, 666, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 714, 714, 0 },
    { 1, 0xFF, 0x64, 4, 0x38, 0, 0, 0, 0, 0, 1363, 762, 762, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x272, 0x100, 0x1E0, 5, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28D, 0x7C, 0x7E, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28E, 0x30A, 0x98, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x28F, 0x4AC, 0x326, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0x2CE, 0x124, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0x2DF, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 5, 0x324, 0x170, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 5, 0x333, 0x11A, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 0xB, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 0x1C, 1 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 3, 4 },
    { { { SPECIAL(0x94), 1 }, { CODES_END, 0 } }, SLOT_WARP1, 0x2E8, 0x240, 0xD0, 1, 0, 8, 2 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x60, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x30, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0x40, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFB0, 0x10, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0xFFE8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0, 0xFFD8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFE0, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x30, 0x28, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFD0, 0xFFF8, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFC0, 0x14, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0x24), 1 }, { FLAG(0x40, 0x5C), 0 } }, SLOT_EVENT, 0x398, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0x27), 1 }, { FLAG(0x40, 0x6A), 0 } }, SLOT_EVENT, 0x3CA, 0, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E5, 0x240, 0x120, 1, 0, 0xB, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 920, script920, EVENT_TEXT(0xE), NULL, endEvent920 },
    { 970, script970, EVENT_TEXT(0x16), NULL, endEvent970 },
    { -1, NULL, 0, NULL, NULL },
};
