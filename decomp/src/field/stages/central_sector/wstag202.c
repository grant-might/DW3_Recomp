/*
 * WSTAG202: Asuka City, Central Sector: the streets by the Cargo Tower. Exits
 * to the Underground Path (WSTAG237), the Yellow Cruiser (270), El Dorado
 * (280) and the Cargo Tower (mode 528, WSTAG260).
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#include "common/start_stage.inc.c"

const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xCD)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x32A
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xC5)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x339
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing ASKA_BGM;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_FILE - 2;
    FIELDSTG_state.start = (Vec2){0x13500, 0x12500};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 4;
    FIELDSTG_state.music = MUSIC(4, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
    if (GAME.progress >= 0x14 && GAME.progress < 0x18) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
    if (GAME.progress >= 0x27 && GAME.progress < 0x29) {
        FIELDSTG_state.soundBank = 0x1F;
        FIELDSTG_state.music = MUSIC(0x1F, 0);
    }
}

s16 script220[] = {
    SCRIPT_WALK(2, 0x220, 0x1B9, 3),
    SCRIPT_PLACE(0x61, 0x200, 0x1A9),
    SCRIPT_POSE(0x61, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x61, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 3),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x61, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x61, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x61, 0x170, 0x165, 3),
    SCRIPT_WAIT_WALK(0x61),
    SCRIPT_TALK(0, 5, 2, 3),
    SCRIPT_PLACE(0x61, 0, 0),
    SCRIPT_POSE(0x61, 1, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x8FB1,
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
AreaBattle area3Battle0 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle1 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle2 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle3 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle4 = { 328, 8, MUSIC(2, 0) };
AreaBattle area3Battle5 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle6 = { 0, 0, MUSIC(1, 0) };
AreaBattle area3Battle7 = { 54, 8, MUSIC(2, 0) };
BattleList area3Battles = {
    0,
    { &area3Battle0, &area3Battle1, &area3Battle2, &area3Battle3,
      &area3Battle4, &area3Battle5, &area3Battle6, &area3Battle7 },
};
FieldBattles stageBattles[] = {
    { 375, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B6, 0x100, 0x1D8, 0, 0x170, 0x1FE },
    { 0x180, 0x100, 0x1B6, 0x128, 0x1D8, 0x28, 0x170, 0x1FD },
    { 0x180, 0x100, 0x18C, 0x171, 0x130, 0x71, 0x170, 0x1FC },
    { 0x180, 0x100, 0x180, 0x175, 0x100, 0x75, 0x140, 0x1FB },
    { 0x180, 0x100, 0x1B0, 0x150, 0x1C0, 0x50, 0x150, 0x1FB },
    { 0x180, 0x100, 0x1B6, 0x178, 0x1D8, 0x78, 0x160, 0x1FB },
    { 0x180, 0x100, 0x194, 0x183, 0x150, 0x83, 0x170, 0x1FB },
    { 0x180, 0x100, 0x19C, 0x183, 0x170, 0x83, 0x140, 0x1FA },
    { 0x180, 0x100, 0x1A4, 0x18B, 0x190, 0x8B, 0x150, 0x1FA },
    { 0x180, 0x100, 0x188, 0x191, 0x120, 0x91, 0x160, 0x1FA },
    { 0x180, 0x100, 0x1A6, 0x163, 0x198, 0x63, 0x170, 0x1FA },
    { 0x180, 0x100, 0x180, 0x195, 0x100, 0x95, 0x140, 0x1F9 },
    { 0x180, 0x100, 0x1AC, 0x198, 0x1B0, 0x98, 0x150, 0x1F9 },
    { 0x180, 0x100, 0x1B4, 0x198, 0x1D0, 0x98, 0x160, 0x1F9 },
    { 0x180, 0x100, 0x190, 0x1A3, 0x140, 0xA3, 0x170, 0x1F9 },
    { 0x180, 0x100, 0x1AE, 0x178, 0x1B8, 0x78, 0x140, 0x1F8 },
};
u16 actor6Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor6Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor21Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
u16 actor42Talk0Actions[] = { FLAG(0x1A, 0x1A), 1, CODES_END };
u16 actor43Talk0Actions[] = { FLAG(0x1A, 0x1B), 1, START_EVENT(0x5D), 1, CODES_END };
u16 actor82Talk0Actions[] = { FLAG(0x1A, 0x14), 1, CODES_END };
u16 actor83Talk0Actions[] = { FLAG(0x1A, 0x14), 1, CODES_END };
u16 actor90Talk0Conditions[] = { FLAG(0x1A, 0x18), 0, CODES_END };
u16 actor90Talk1Conditions[] = { FLAG(0x1A, 0x18), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x12 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x279 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x27A },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x27B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x27C },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x27D },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x279 },
    { actor6Talk1Conditions, NULL, 0x4A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x27E },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x27F },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x280 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x4A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0xB },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x2C7 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x2C8 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x2C9 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x2CA },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x2CB },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x4A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x2CC },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x2CD },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x2CE },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, NULL, 0x2C7 },
    { actor21Talk1Conditions, NULL, 0x4A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x16 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x286 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x282 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x283 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x284 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x285 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x28B },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x287 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x288 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x289 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x28D },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x28E },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x28F },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x290 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x17 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x291 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x296 },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x292 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x293 },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x294 },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, actor42Talk0Actions, 0x51 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, actor43Talk0Actions, 0xBA },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x28A },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { NULL, NULL, 0x295 },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0x281 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0x2CF },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor55Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor56Talks[] = {
    { NULL, NULL, 0x48B },
    { NULL, NULL, 0 },
};
FieldTalk actor57Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor58Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor59Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor60Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor61Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor62Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor63Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor64Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor65Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor66Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor67Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor68Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor69Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor70Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor71Talks[] = {
    { NULL, NULL, 0x65 },
    { NULL, NULL, 0 },
};
FieldTalk actor72Talks[] = {
    { NULL, NULL, 0xBE },
    { NULL, NULL, 0 },
};
FieldTalk actor73Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor74Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor75Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor76Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor77Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor78Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor79Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor80Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor81Talks[] = {
    { NULL, NULL, 0x48F },
    { NULL, NULL, 0 },
};
FieldTalk actor82Talks[] = {
    { NULL, actor82Talk0Actions, 0x427 },
    { NULL, NULL, 0 },
};
FieldTalk actor83Talks[] = {
    { NULL, actor83Talk0Actions, 0x428 },
    { NULL, NULL, 0 },
};
FieldTalk actor84Talks[] = {
    { NULL, NULL, 0x18 },
    { NULL, NULL, 0 },
};
FieldTalk actor85Talks[] = {
    { NULL, NULL, 0x2D1 },
    { NULL, NULL, 0 },
};
FieldTalk actor86Talks[] = {
    { NULL, NULL, 0x2D2 },
    { NULL, NULL, 0 },
};
FieldTalk actor87Talks[] = {
    { NULL, NULL, 0x2D3 },
    { NULL, NULL, 0 },
};
FieldTalk actor88Talks[] = {
    { NULL, NULL, 0x2D4 },
    { NULL, NULL, 0 },
};
FieldTalk actor89Talks[] = {
    { NULL, NULL, 0x2D5 },
    { NULL, NULL, 0 },
};
FieldTalk actor90Talks[] = {
    { actor90Talk0Conditions, NULL, 0x2D1 },
    { actor90Talk1Conditions, NULL, 0x4A6 },
    { NULL, NULL, 0 },
};
FieldTalk actor91Talks[] = {
    { NULL, NULL, 0x2D6 },
    { NULL, NULL, 0 },
};
FieldTalk actor92Talks[] = {
    { NULL, NULL, 0x2D7 },
    { NULL, NULL, 0 },
};
FieldTalk actor93Talks[] = {
    { NULL, NULL, 0x2D8 },
    { NULL, NULL, 0 },
};
FieldTalk actor94Talks[] = {
    { NULL, NULL, 0x2D9 },
    { NULL, NULL, 0 },
};
FieldTalk actor95Talks[] = {
    { NULL, NULL, 0x4A6 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor4Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor7Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor12Conditions[] = { SPECIAL(0x15), 1, PROGRESS(8), 0, PROGRESS(9), 0, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor15Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor19Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0xC), 0, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor32Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0xC), 0, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor39Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(8), 1, FLAG(0x1A, 0x19), 1, CODES_END };
u16 actor43Conditions[] = { PROGRESS(9), 1, FLAG(0x1A, 0x1B), 0, CODES_END };
u16 actor44Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor45Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor46Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor47Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor48Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor49Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor50Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor51Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor52Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor53Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor54Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor55Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor56Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor57Conditions[] = { PROGRESS(5), 1, CODES_END };
u16 actor58Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor59Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor60Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor61Conditions[] = { PROGRESS(0x10), 1, CODES_END };
u16 actor62Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor63Conditions[] = { PROGRESS(0x18), 1, CODES_END };
u16 actor64Conditions[] = { PROGRESS(0x1A), 1, CODES_END };
u16 actor65Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor66Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor67Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor68Conditions[] = { PROGRESS(0x22), 1, CODES_END };
u16 actor69Conditions[] = { PROGRESS(0x24), 1, CODES_END };
u16 actor70Conditions[] = { PROGRESS(0x25), 1, CODES_END };
u16 actor71Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor72Conditions[] = { PROGRESS(0x27), 1, CODES_END };
u16 actor73Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor74Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor75Conditions[] = { PROGRESS(0x1C), 1, CODES_END };
u16 actor76Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor77Conditions[] = { PROGRESS(0x1E), 1, CODES_END };
u16 actor78Conditions[] = { PROGRESS(0x1F), 1, CODES_END };
u16 actor79Conditions[] = { PROGRESS(0x20), 1, CODES_END };
u16 actor80Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor81Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor82Conditions[] = { PROGRESS(0xC), 1, FLAG(0x1A, 0xC), 1, CODES_END };
u16 actor83Conditions[] = { FLAG(0x1A, 0xC), 1, PROGRESS(0xC), 1, CODES_END };
u16 actor84Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor85Conditions[] = {
    SPECIAL(0x15), 1,
    PROGRESS(6), 0,
    PROGRESS(8), 0,
    PROGRESS(9), 0,
    CODES_END,
};
u16 actor86Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor87Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor88Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor89Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor90Conditions[] = { PROGRESS(8), 1, CODES_END };
u16 actor91Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor92Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor93Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor94Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor95Conditions[] = { PROGRESS(9), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2F, 4, 576, 160, 1 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x33, 5, 494, 187, 5 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x39, 6, 208, 344, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x39, 6, 680, 380, 1 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x3A, 7, 494, 187, 1 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x3A, 7, 648, 396, 5 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x61, 8, 512, 425, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x61, 8, 512, 425, 7 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0x9D, 9, 680, 380, 1 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x9E, 0xA, 648, 396, 5 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x9F, 0xB, 576, 160, 1 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0xA0, 0xC, 494, 187, 5 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor55 = { actor55Conditions, actor55Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor56 = { actor56Conditions, actor56Talks, 0xB2, 0xD, 534, 435, 5 };
FieldActorEntry actor57 = { actor57Conditions, actor57Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor58 = { actor58Conditions, actor58Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor59 = { actor59Conditions, actor59Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor60 = { actor60Conditions, actor60Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor61 = { actor61Conditions, actor61Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor62 = { actor62Conditions, actor62Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor63 = { actor63Conditions, actor63Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor64 = { actor64Conditions, actor64Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor65 = { actor65Conditions, actor65Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor66 = { actor66Conditions, actor66Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor67 = { actor67Conditions, actor67Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor68 = { actor68Conditions, actor68Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor69 = { actor69Conditions, actor69Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor70 = { actor70Conditions, actor70Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor71 = { actor71Conditions, actor71Talks, 0x118, 0xE, 864, 249, 1 };
FieldActorEntry actor72 = { actor72Conditions, actor72Talks, 0x119, 0xF, 864, 249, 1 };
FieldActorEntry actor73 = { actor73Conditions, actor73Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor74 = { actor74Conditions, actor74Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor75 = { actor75Conditions, actor75Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor76 = { actor76Conditions, actor76Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor77 = { actor77Conditions, actor77Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor78 = { actor78Conditions, actor78Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor79 = { actor79Conditions, actor79Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor80 = { actor80Conditions, actor80Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor81 = { actor81Conditions, actor81Talks, 0x13D, 0x10, 512, 424, 5 };
FieldActorEntry actor82 = { actor82Conditions, actor82Talks, 0x149, 0x11, 680, 380, 1 };
FieldActorEntry actor83 = { actor83Conditions, actor83Talks, 0x14A, 0x12, 648, 396, 5 };
FieldActorEntry actor84 = { actor84Conditions, actor84Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor85 = { actor85Conditions, actor85Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor86 = { actor86Conditions, actor86Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor87 = { actor87Conditions, actor87Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor88 = { actor88Conditions, actor88Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor89 = { actor89Conditions, actor89Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor90 = { actor90Conditions, actor90Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor91 = { actor91Conditions, actor91Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor92 = { actor92Conditions, actor92Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor93 = { actor93Conditions, actor93Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor94 = { actor94Conditions, actor94Talks, 0x171, 0x13, 208, 344, 5 };
FieldActorEntry actor95 = { actor95Conditions, actor95Talks, 0x171, 0x13, 208, 344, 5 };
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
    &actor48,
    &actor49,
    &actor50,
    &actor51,
    &actor52,
    &actor53,
    &actor54,
    &actor55,
    &actor56,
    &actor57,
    &actor58,
    &actor59,
    &actor60,
    &actor61,
    &actor62,
    &actor63,
    &actor64,
    &actor65,
    &actor66,
    &actor67,
    &actor68,
    &actor69,
    &actor70,
    &actor71,
    &actor72,
    &actor73,
    &actor74,
    &actor75,
    &actor76,
    &actor77,
    &actor78,
    &actor79,
    &actor80,
    &actor81,
    &actor82,
    &actor83,
    &actor84,
    &actor85,
    &actor86,
    &actor87,
    &actor88,
    &actor89,
    &actor90,
    &actor91,
    &actor92,
    &actor93,
    &actor94,
    &actor95,
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 174, 210, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 244, 175, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 840, 188, 0, 0 },
    { 1, 0, 0x40, 2, 0x47, 2, 0, 3, 6, 0, 886, 211, 0, 0 },
    { 1, 0, 0x40, 2, 0x48, 2, 0, 3, 6, 0, 341, 183, 0, 0 },
    { 1, 0, 0x40, 2, 4, 1, 4, 9, 8, 0, 98, 114, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 168, 256, 0, 0 },
    { 1, 0, 0x40, 2, 0xE, 0, 0, 0, 0, 0, 846, 150, 0, 0 },
    { 1, 0, 0x58, 2, 0xF, 0, 0, 0, 0, 0, 426, 256, 0, 0 },
    { 1, 0, 0x50, 2, 0x10, 0, 0, 0, 0, 0, 323, 384, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 0, 0, 0, 0, 0, 640, 256, 0, 0 },
    { 1, 0, 0x60, 2, 0x12, 0, 0, 0, 0, 0, 548, 202, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 676, 384, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 386, 160, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 3, 6, 0, 498, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 123, 295, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 247, 351, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 571, 465, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 632, 171, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 654, 417, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 684, 140, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 719, 379, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 726, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 772, 330, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 793, 216, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x34, 0xA, 0, 994, 283, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 705, 485, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 706, 129, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 718, 471, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 741, 104, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 757, 70, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 764, 405, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 799, 58, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 855, 294, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 1, 0x35, 0x37, 0xA, 0, 912, 279, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 119, 307, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 203, 386, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 217, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 283, 332, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 598, 453, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 647, 259, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 689, 401, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 691, 480, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 726, 257, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 742, 457, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 771, 233, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 798, 48, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 810, 366, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 819, 305, 0, 0 },
    { 1, 0, 0x40, 6, 0x38, 1, 0x38, 0x3A, 0xA, 0, 895, 276, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 84, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 116, 353, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 277, 443, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 306, 336, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 399, 464, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 610, 94, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 673, 147, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 769, 210, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 776, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 802, 323, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 835, 403, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 984, 292, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 199, 281, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 416, 464, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 533, 476, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 589, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 659, 135, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 749, 190, 0, 0 },
    { 1, 0, 0x40, 6, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 790, 408, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 97, 297, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 143, 268, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 169, 362, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 185, 388, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 311, 450, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 465, 474, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 631, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 708, 421, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 748, 169, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 758, 250, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 803, 312, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 820, 208, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 821, 53, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 822, 406, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 1, 0x41, 0x43, 0xA, 0, 961, 283, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 140, 346, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 256, 443, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 360, 456, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 400, 290, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 422, 282, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 579, 126, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 594, 125, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 594, 201, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 596, 187, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 610, 144, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 611, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 620, 136, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 633, 468, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 669, 271, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 699, 272, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 737, 433, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 738, 396, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 756, 153, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 771, 144, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 782, 389, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 784, 320, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 819, 213, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 863, 281, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 908, 242, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 1, 0x44, 0x46, 0xA, 0, 1023, 270, 0, 0 },
    { 1, 0x64, 0x40, 6, 0xA, 0, 0, 0, 0, 0, 350, 152, 0, 0 },
    { 1, 0x65, 0x40, 6, 0xB, 0, 0, 0, 0, 0, 463, 106, 0, 0 },
    { 1, 0x66, 0x40, 6, 0xC, 0, 0, 0, 0, 0, 878, 199, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 224, 268, 289, 0 },
    { 1, 0, 0x41, 4, 1, 0, 0, 0, 0, 0, 336, 159, 216, 0 },
    { 1, 0, 0x46, 4, 2, 0, 0, 0, 0, 0, 448, 103, 162, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 891, 200, 247, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20B, 0x68, 0x134, 3, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x213, 0x218, 0xF4, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x211, 0x216, 0xE2, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x210, 0x98, 0x14A, 5, 0x66, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x40, 0xFFF0, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0x20, 0x18, 0, 0, 0, 0, 0 },
    { { { ITEM(0, 5), 1 }, { CODES_END, 0 } }, SLOT_GAUGE, 0xFFE0, 0x1C, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 220, script220, EVENT_TEXT(0x17), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
