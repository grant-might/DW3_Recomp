/*
 * WSTAG500: Suzaku City, South Sector.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

/*
 * After event 250, with Woman and Lisa: sets flag 0x1A32 (which decides
 * whether actor 32 appears) and keeps it from playing again
 */
void endEvent250(void) {
    FLAGS_00.applyAction(FLAG(0x40, 0xA), 1);
    FLAGS_00.applyAction(FLAG(0x1A, 0x32), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xD4)
#define EVENT_TEXT_FILE 0x135
#define STAGE_FILE 0x3FE
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xCC)
#define EVENT_TEXT_FILE 0x13C
#define STAGE_FILE 0x40E
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing BGM_0016;
 * BGM_0029 instead at story steps 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x11100, 0x35300};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x12;
    FIELDSTG_state.music = MUSIC(0x12, 0);
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
}

s16 script250[] = {
    SCRIPT_WALK(2, 0x118, 0x194, 3),
    SCRIPT_PLACE(0x65, 0xF8, 0x184),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x65),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x65),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x65, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x65, 0xE8, 0x18C, 1),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_WALK(0x65, 0x120, 0x1A8, 7),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_POSE(0x65, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0x65, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x65, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WALK(0x65, 0x17E, 0x220, 7),
    SCRIPT_WAIT_WALK(0x65),
    SCRIPT_TALK(0, 0xA, 2, 0),
    SCRIPT_PLACE(0x65, 0, 0),
    SCRIPT_POSE(0x65, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x6004,
#elif VERSION_EU || VERSION_JP
    0x6008,
#endif
};
s16 script270[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(1, 0x202, 0x1B8, 1),
    SCRIPT_WALK(2, 0x178, 0x153, 1),
    SCRIPT_PLACE(0xB, 0x126, 0x153),
    SCRIPT_POSE(0xB, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(6),
    SCRIPT_COMMAND(0x323, 0x325, 0xB),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xB),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WALK(0xB, 0x15E, 0x153, 6),
    SCRIPT_WAIT_WALK(0xB),
    SCRIPT_TALK(0, 1, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 2),
    SCRIPT_POSE(1, 7, 2),
    SCRIPT_POSE(2, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xB, 0),
    SCRIPT_POSE(0xB, 7, 6),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0xB, 1, 6),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 2),
    SCRIPT_POSE(1, 7, 2),
    SCRIPT_POSE(2, 7, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(1, 1, 2),
    SCRIPT_POSE(2, 1, 2),
    SCRIPT_LEAVE(0x204, 0x50, 0x198, 5),
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
AreaBattle area3Battle0 = { 215, 20, MUSIC(3, 0) };
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
    { 157, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x148, 0x19D, 0x20, 0x9D, 0x160, 0x1FF },
    { 0x140, 0x100, 0x16E, 0x174, 0xB8, 0x74, 0x170, 0x1FF },
    { 0x140, 0x100, 0x156, 0x18C, 0x58, 0x8C, 0x150, 0x1FE },
    { 0x140, 0x100, 0x140, 0x16D, 0, 0x6D, 0x160, 0x1FE },
    { 0x140, 0x100, 0x178, 0x19C, 0xE0, 0x9C, 0x170, 0x1FE },
    { 0x140, 0x100, 0x150, 0x1AC, 0x40, 0xAC, 0x150, 0x1FD },
    { 0x140, 0x100, 0x176, 0x174, 0xD8, 0x74, 0x160, 0x1FD },
    { 0x140, 0x100, 0x168, 0x19C, 0xA0, 0x9C, 0x160, 0x1FC },
    { 0x140, 0x100, 0x158, 0x1AC, 0x60, 0xAC, 0x170, 0x1FC },
    { 0x140, 0x100, 0x160, 0x1B9, 0x80, 0xB9, 0x140, 0x1FB },
    { 0x140, 0x100, 0x170, 0x1BC, 0xC0, 0xBC, 0x150, 0x1FB },
    { 0x140, 0x100, 0x140, 0x1BD, 0, 0xBD, 0x160, 0x1FB },
    { 0x140, 0x100, 0x14C, 0x16D, 0x30, 0x6D, 0x170, 0x1FB },
};
u16 actor1Talk0Actions[] = { 0x7A09, 1, CODES_END };
u16 actor2Talk0Actions[] = { 0x7A07, 1, CODES_END };
u16 actor3Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0x1C, 0x1C), 0, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0x1C, 0x1C), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor15Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor15Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(0x19, 0), 1, CODES_END };
u16 actor16Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    PARTY_STAT(2), 1,
    FLAG(0xE, 0xF), 0,
    CODES_END,
};
u16 actor16Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xF), 1, CODES_END };
u16 actor16Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor16Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor16Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor16Talk6Actions[] = { CARD_BATTLE(0x19, 1), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor17Talk2Actions[] = { CARD_BATTLE(0x19, 0), 1, CODES_END };
u16 actor17Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 0,
    CODES_END,
};
u16 actor17Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xF), 1, CODES_END };
u16 actor17Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor17Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor17Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor17Talk6Actions[] = { CARD_BATTLE(0x19, 1), 1, CODES_END };
u16 actor18Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor18Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor18Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor18Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor18Talk2Actions[] = { CARD_BATTLE(0x19, 0), 1, CODES_END };
u16 actor18Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 0,
    CODES_END,
};
u16 actor18Talk3Actions[] = { EVENT_BATTLE(0), 1, FLAG(0xE, 0xF), 1, CODES_END };
u16 actor18Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor18Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor18Talk6Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor18Talk6Actions[] = { CARD_BATTLE(0x19, 1), 1, CODES_END };
u16 actor34Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor34Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor34Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(2), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor34Talk2Actions[] = { CARD_BATTLE(0x19, 0), 1, CODES_END };
u16 actor34Talk3Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 0,
    CODES_END,
};
u16 actor34Talk3Actions[] = { FLAG(0xE, 0xF), 1, CODES_END };
u16 actor34Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 0,
    CODES_END,
};
u16 actor34Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    FLAG(0xE, 0xF), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 0,
    CODES_END,
};
u16 actor34Talk6Conditions[] = {
    FLAG(0xE, 0xF), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(2), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x12), 1,
    PARTY_STAT(6), 1,
    CODES_END,
};
u16 actor34Talk6Actions[] = { CARD_BATTLE(0x19, 1), 1, CODES_END };
u16 actor36Talk0Actions[] = { 0x7A08, 1, CODES_END };
u16 actor37Talk0Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor37Talk0Actions[] = { 0x7A3B, 1, CODES_END };
u16 actor37Talk1Conditions[] = { SPECIAL(0x3E), 1, CODES_END };
u16 actor37Talk1Actions[] = { 0x7A3B, 1, CODES_END };
u16 actor37Talk2Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor37Talk2Actions[] = { 0x7A3C, 1, CODES_END };
u16 actor37Talk3Conditions[] = { SPECIAL(0x20), 1, CODES_END };
u16 actor37Talk3Actions[] = { 0x7A3C, 1, CODES_END };
u16 actor37Talk4Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor37Talk4Actions[] = { 0x7A3D, 1, CODES_END };
u16 actor37Talk5Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor37Talk5Actions[] = { 0x7A3E, 1, CODES_END };
u16 actor37Talk6Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor37Talk6Actions[] = { 0x7A3E, 1, CODES_END };
u16 actor37Talk7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor37Talk7Actions[] = { 0x7A3E, 1, CODES_END };
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0xF3 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0xF2 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x4C },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x71 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x79 },
    { actor5Talk1Conditions, NULL, 0x3C },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x81 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x7F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x7B },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x77 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x73 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x7D },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x7D },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0xE2 },
    { actor15Talk1Conditions, actor15Talk1Actions, 0xED },
    { actor15Talk2Conditions, actor15Talk2Actions, 0xEE },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0xE2 },
    { actor16Talk1Conditions, NULL, 0xE7 },
    { actor16Talk2Conditions, actor16Talk2Actions, 0xE8 },
    { actor16Talk3Conditions, actor16Talk3Actions, 0xE9 },
    { actor16Talk4Conditions, NULL, 0xEA },
    { actor16Talk5Conditions, NULL, 0xEB },
    { actor16Talk6Conditions, actor16Talk6Actions, 0xEC },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0xE4 },
    { actor17Talk1Conditions, NULL, 0xE7 },
    { actor17Talk2Conditions, actor17Talk2Actions, 0xE8 },
    { actor17Talk3Conditions, actor17Talk3Actions, 0xE9 },
    { actor17Talk4Conditions, NULL, 0xEA },
    { actor17Talk5Conditions, NULL, 0xEB },
    { actor17Talk6Conditions, actor17Talk6Actions, 0xEC },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { actor18Talk0Conditions, actor18Talk0Actions, 0xE3 },
    { actor18Talk1Conditions, NULL, 0xE7 },
    { actor18Talk2Conditions, actor18Talk2Actions, 0xE8 },
    { actor18Talk3Conditions, actor18Talk3Actions, 0xE9 },
    { actor18Talk4Conditions, NULL, 0xEA },
    { actor18Talk5Conditions, NULL, 0xEB },
    { actor18Talk6Conditions, actor18Talk6Actions, 0xEC },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x113 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x70 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x84 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x7C },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x72 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x75 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x76 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x78 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x80 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x7A },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x7C },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x120 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x82 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { actor34Talk0Conditions, NULL, 0xE5 },
    { actor34Talk1Conditions, NULL, 0xE5 },
    { actor34Talk2Conditions, actor34Talk2Actions, 0xE5 },
    { actor34Talk3Conditions, actor34Talk3Actions, 0xE5 },
    { actor34Talk4Conditions, NULL, 0xE5 },
    { actor34Talk5Conditions, NULL, 0xE5 },
    { actor34Talk6Conditions, actor34Talk6Actions, 0xE5 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x83 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, actor36Talk0Actions, 0xF4 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { actor37Talk0Conditions, actor37Talk0Actions, 1 },
    { actor37Talk1Conditions, actor37Talk1Actions, 1 },
    { actor37Talk2Conditions, actor37Talk2Actions, 1 },
    { actor37Talk3Conditions, actor37Talk3Actions, 1 },
    { actor37Talk4Conditions, actor37Talk4Actions, 1 },
    { actor37Talk5Conditions, actor37Talk5Actions, 1 },
    { actor37Talk6Conditions, actor37Talk6Actions, 1 },
    { actor37Talk7Conditions, actor37Talk7Actions, 1 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x174 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x14), 1, PROGRESS(0xB), 0, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x14), 1, PROGRESS(0xB), 0, CODES_END };
u16 actor3Conditions[] = { SPECIAL(0x14), 1, PROGRESS(0xB), 0, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor9Conditions[] = { SPECIAL(0x17), 1, PROGRESS(0x14), 0, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor15Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, SPECIAL(9), 1, CODES_END };
u16 actor16Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, SPECIAL(3), 1, CODES_END };
u16 actor17Conditions[] = { ITEM(0, 0x192), 1, PROGRESS(0x26), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor18Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, SPECIAL(4), 1, CODES_END };
u16 actor19Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x17), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xB), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xA), 1, FLAG(0x1A, 0x32), 0, CODES_END };
u16 actor33Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(0xB), 0, SPECIAL(0x14), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(0xB), 0, SPECIAL(0x14), 1, ITEM(0, 0x192), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x14), 1, PROGRESS(0xB), 0, ITEM(0, 0x192), 0, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, NULL, 0xB, 4, 294, 339, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x16, 5, 923, 892, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x17, 6, 816, 521, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x18, 7, 1189, 666, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2D, 8, 751, 650, 1 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2E, 9, 405, 650, 1 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2E, 9, 405, 650, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2E, 9, 405, 650, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x2E, 9, 405, 650, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x2E, 9, 405, 650, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x32, 0xA, 751, 650, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x32, 0xA, 677, 184, 1 };
FieldActorEntry actor32 = { actor32Conditions, NULL, 0x65, 0xB, 248, 388, 7 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x9D, 0xC, 677, 184, 1 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x9E, 0xD, 405, 650, 1 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x9F, 0xE, 751, 650, 1 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0xB7, 0xF, 990, 767, 1 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0xCE, 0x10, 1216, 913, 3 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0xCE, 0x10, 1216, 913, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 8, 0, 160, 342, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 8, 0, 234, 305, 0, 0 },
    { 1, 0, 0x78, 2, 0xC, 0, 0, 0, 0, 0, 542, 517, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 167, 863, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 352, 645, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 729, 598, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 856, 344, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 922, 739, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 922, 965, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 945, 639, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 968, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 986, 166, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1080, 774, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1104, 915, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1110, 241, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1229, 378, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1241, 948, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1289, 877, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 1307, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 313, 667, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 335, 942, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 481, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 275, 687, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 324, 580, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 465, 512, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 737, 837, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 989, 706, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1214, 965, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 1328, 860, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 360, 862, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 790, 877, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 858, 672, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 958, 982, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 984, 514, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1038, 947, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1060, 261, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1130, 343, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1134, 935, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1193, 519, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 1200, 744, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 634, 800, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1050, 432, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 1155, 503, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 379, 735, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 570, 831, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 712, 838, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 831, 895, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1031, 534, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1157, 726, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1188, 373, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 1232, 539, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 246, 922, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 306, 894, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 390, 891, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 451, 885, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 501, 842, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 606, 970, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 793, 728, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 885, 667, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 956, 726, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 995, 970, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1030, 448, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1046, 753, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1072, 526, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1076, 934, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1084, 919, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1107, 513, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1113, 506, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1156, 929, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1227, 385, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1251, 754, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1304, 520, 0, 0 },
    { 1, 0x64, 0x40, 6, 9, 0, 0, 0, 0, 0, 769, 100, 0, 0 },
    { 1, 0x65, 0x40, 6, 0xA, 0, 0, 0, 0, 0, 574, 93, 0, 0 },
    { 1, 0x66, 0x40, 6, 0xB, 0, 0, 0, 0, 0, 186, 320, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 606, 118, 136, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 156, 353, 380, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 318, 390, 407, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 400, 341, 354, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 501, 625, 649, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 660, 626, 649, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 537, 158, 166, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 1206, 636, 682, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 1120, 656, 670, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23B, 0x630, 0x88, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x23F, 0x2C0, 0x1B0, 3, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x240, 0xC8, 0x164, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x240, 0x258, 0x1C2, 3, 0x64, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 7, 1 },
    { { { PROGRESS(0xA), 1 }, { FLAG(0x40, 0xA), 0 } }, SLOT_EVENT, 0xFA, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xB), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x10E, 0, 0, 0, 0, 0, 0 },
    { { { SPECIAL(0x93), 1 }, { CODES_END, 0 } }, SLOT_WARP0, 0x2E2, 0xB0, 0x148, 7, 0, 7, 1 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 270, script270, EVENT_TEXT(3), NULL, NULL },
    { 250, script250, EVENT_TEXT(0), NULL, endEvent250 },
    { -1, NULL, 0, NULL, NULL },
};
