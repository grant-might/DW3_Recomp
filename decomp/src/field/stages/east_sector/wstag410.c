/*
 * WSTAG410: East Station, East Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

/* After event 90, with the player: moves the story on to its step 6 */
void endEvent90(void) {
    GAME.progress = 6;
}

const CVECTOR stageColor = { 0x80, 0x80, 0x80, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF7)
#define EVENT_TEXT_FILE 0x12E
#define STAGE_FILE 0x38D
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xEF)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x39D
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
    FIELDSTG_state.start = (Vec2){0x26F00, 0xA800};
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

s16 script90[] = {
    SCRIPT_WALK(2, 0x1DE, 0x140, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x20E, 0x128, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x22A, 0xD0, 0x2FC, 7),
    SCRIPT_END,
};
s16 script140[] = {
    SCRIPT_WALK(2, 0x1DE, 0x140, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x324, 0x325, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(0x324, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1FF, 0x130, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script142[] = {
    SCRIPT_WALK(2, 0x1DE, 0x140, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(2, 7, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x1EF, 0x139, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x19F, 0x111, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x13F, 0x141, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x22D, 0x114, 0x157, 1),
    SCRIPT_END,
};
s16 script143[] = {
    SCRIPT_WALK(2, 0x1DE, 0x140, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x78),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x18E, 0x118, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x14E, 0x138, 1),
    SCRIPT_WAIT(0x3C),
    SCRIPT_LEAVE(0x232, 0x1C0, 0xD0, 1),
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
AreaBattle area3Battle0 = { 208, 20, MUSIC(3, 0) };
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
    { 154, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x174, 0x100, 0xD0, 0, 0x170, 0x1FF },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x174, 0x128, 0xD0, 0x28, 0x160, 0x1FE },
};
u16 actor0Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor0Talk2Actions[] = { CARD_BATTLE(0x12, 0), 1, CODES_END };
u16 actor0Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 0,
    CODES_END,
};
u16 actor0Talk3Actions[] = { FLAG(0xE, 8), 1, EVENT_BATTLE(0), 1, CODES_END };
u16 actor0Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 8), 1,
    CODES_END,
};
u16 actor0Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0xE, 8), 1,
    CODES_END,
};
u16 actor0Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor0Talk6Actions[] = { CARD_BATTLE(0x12, 1), 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor1Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor1Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor1Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor2Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor2Talk2Actions[] = { CARD_BATTLE(0x12, 0), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 0,
    CODES_END,
};
u16 actor2Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 8), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 0,
    FLAG(0xE, 8), 1,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor2Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(0x12, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor3Talk2Actions[] = { CARD_BATTLE(0x12, 0), 1, CODES_END };
u16 actor3Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 0,
    CODES_END,
};
u16 actor3Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 8), 1, CODES_END };
u16 actor3Talk4Conditions[] = {
    ITEM(0, 0x12), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    CODES_END,
};
u16 actor3Talk5Conditions[] = {
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 0,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor3Talk6Conditions[] = {
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    FLAG(0xE, 8), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    CODES_END,
};
u16 actor3Talk6Actions[] = { CARD_BATTLE(0x12, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { ITEM(0, 0x18E), 1, CODES_END };
u16 actor5Talk0Actions[] = { START_EVENT(0xA), 1, CODES_END };
u16 actor5Talk1Conditions[] = { ITEM(0, 0x18E), 0, ITEM(0, 0x18F), 0, CODES_END };
u16 actor5Talk2Conditions[] = {
    ITEM(0, 0x18E), 0,
    ITEM(0, 0x18F), 1,
    FLAG(0x1C, 0x50), 0,
    CODES_END,
};
u16 actor5Talk2Actions[] = {
    FLAG(0x1C, 0x46), 1,
    START_EVENT(9), 1,
    FLAG(0x1C, 0x50), 1,
    CODES_END,
};
u16 actor5Talk3Conditions[] = {
    ITEM(0, 0x18F), 1,
    ITEM(0, 0x18E), 0,
    FLAG(0x1C, 0x50), 1,
    CODES_END,
};
u16 actor6Talk0Actions[] = { START_EVENT(0x5F), 1, CODES_END };
u16 actor7Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor7Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor7Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(1), 1, PARTY_STAT(3), 0, CODES_END };
u16 actor7Talk2Actions[] = { CARD_BATTLE(0x12, 0), 1, CODES_END };
u16 actor7Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 0,
    CODES_END,
};
u16 actor7Talk3Actions[] = { FLAG(0xE, 8), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor7Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(3), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(1), 1,
    FLAG(0xE, 8), 1,
    PARTY_STAT(5), 0,
    CODES_END,
};
u16 actor7Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(1), 1,
    PARTY_STAT(3), 1,
    FLAG(0xE, 8), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(5), 1,
    CODES_END,
};
u16 actor7Talk6Actions[] = { CARD_BATTLE(0x12, 1), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x5E },
    { actor0Talk1Conditions, NULL, 0x63 },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x64 },
    { actor0Talk3Conditions, actor0Talk3Actions, 0x65 },
    { actor0Talk4Conditions, NULL, 0x66 },
    { actor0Talk5Conditions, NULL, 0x67 },
    { actor0Talk6Conditions, actor0Talk6Actions, 0x7A },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, NULL, 0x5E },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x68 },
    { actor1Talk2Conditions, actor1Talk2Actions, 0x69 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x5F },
    { actor2Talk1Conditions, NULL, 0x63 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x64 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x65 },
    { actor2Talk4Conditions, NULL, 0x66 },
    { actor2Talk5Conditions, NULL, 0x67 },
    { actor2Talk6Conditions, actor2Talk6Actions, 0x7A },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 0x60 },
    { actor3Talk1Conditions, NULL, 0x63 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x64 },
    { actor3Talk3Conditions, actor3Talk3Actions, 0x65 },
    { actor3Talk4Conditions, NULL, 0x66 },
    { actor3Talk5Conditions, NULL, 0x67 },
    { actor3Talk6Conditions, actor3Talk6Actions, 0x7A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x276 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x32A },
    { actor5Talk1Conditions, NULL, 0x2E3 },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x2E4 },
    { actor5Talk3Conditions, NULL, 0x2E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x2E5 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x61 },
    { actor7Talk1Conditions, NULL, 0x61 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0x61 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x61 },
    { actor7Talk4Conditions, NULL, 0x61 },
    { actor7Talk5Conditions, NULL, 0x61 },
    { actor7Talk6Conditions, actor7Talk6Actions, 0x61 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(3), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(9), 1, FLAG(0, 0x11), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(4), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x26), 1, FLAG(0, 0x11), 0, ITEM(0, 0x192), 1, CODES_END };
u16 actor4Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor5Conditions[] = { PROGRESS(6), 1, CODES_END };
u16 actor6Conditions[] = { SPECIAL(0x14), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x30, 4, 624, 152, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x30, 4, 624, 152, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x30, 4, 624, 152, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x30, 4, 624, 152, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x30, 4, 624, 152, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x3F, 5, 463, 328, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x3F, 5, 463, 328, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x9D, 6, 624, 152, 7 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
    &actor7,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x80, 2, 0, 0, 0, 0, 0, 0, 218, 322, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 5, 6, 0, 429, 308, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 5, 6, 0, 674, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 1, 0x34, 0x39, 4, 0, 776, 392, 0, 0 },
    { 1, 0, 0x80, 6, 1, 0, 0, 0, 0, 0, 219, 367, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x22A, 0xD0, 0x2FC, 7, 0, 0, 0 },
    { { { PROGRESS(5), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x5A, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 90, script90, EVENT_TEXT(0x1D), NULL, endEvent90 },
    { 140, script140, EVENT_TEXT(0x1E), NULL, NULL },
    { 142, script142, EVENT_TEXT(0x1F), NULL, NULL },
    { 143, script143, EVENT_TEXT(0x20), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
