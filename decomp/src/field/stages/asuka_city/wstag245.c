/*
 * WSTAG245: Smith's Shop, Asuka City.
 */

#include "common.h"
#include "field/stage.h"

#include "common/update_stage.inc.c"
#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x199
#define STAGE_ARCHIVE 0x314
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x1A7
#define STAGE_ARCHIVE 0x323
#endif
/* Sets the stage up: its map, actors and events, playing SHOP1BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x15A00, 0x18E00};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 7;
    FIELDSTG_state.music = MUSIC(7, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

s16 script1250[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xB0, 0x198, 3),
    SCRIPT_PLACE(0x1E, 0x98, 0x18D),
    SCRIPT_POSE(0x1E, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x1E),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x1E, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x1E, 0x68, 0x174, 3),
    SCRIPT_WAIT_WALK(0x1E),
    SCRIPT_WALK(0x1E, 0x90, 0x160, 5),
    SCRIPT_WAIT_WALK(0x1E),
    SCRIPT_POSE(0x1E, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1251[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0xB0, 0x198, 3),
    SCRIPT_PLACE(0x126, 0x98, 0x18D),
    SCRIPT_POSE(0x126, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x126),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x126),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x126, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x126, 0x68, 0x174, 3),
    SCRIPT_WAIT_WALK(0x126),
    SCRIPT_WALK(0x126, 0x90, 0x160, 5),
    SCRIPT_WAIT_WALK(0x126),
    SCRIPT_POSE(0x126, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x1C0, 0x100, 0x1F4, 0x140, 0x2D0, 0x40, 0x150, 0x1FF },
    { 0x140, 0x100, 0x162, 0x1DC, 0x88, 0xDC, 0x160, 0x1FF },
    { 0x1C0, 0x100, 0x1CE, 0x1A4, 0x238, 0xA4, 0x170, 0x1FF },
    { 0x140, 0x100, 0x178, 0x100, 0xE0, 0, 0x150, 0x1FE },
    { 0x1C0, 0x100, 0x1D6, 0x1A9, 0x258, 0xA9, 0x160, 0x1FE },
    { 0x140, 0x100, 0x174, 0x1DC, 0xD0, 0xDC, 0x170, 0x1FE },
    { 0x1C0, 0x100, 0x1C0, 0x1AE, 0x200, 0xAE, 0x140, 0x1FD },
    { 0x1C0, 0x100, 0x1E6, 0x1AF, 0x298, 0xAF, 0x150, 0x1FD },
    { 0x1C0, 0x100, 0x1EE, 0x1AF, 0x2B8, 0xAF, 0x160, 0x1FD },
    { 0x140, 0x100, 0x178, 0x120, 0xE0, 0x20, 0x170, 0x1FD },
    { 0x1C0, 0x100, 0x1DE, 0x1A9, 0x278, 0xA9, 0x140, 0x1FC },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 0xF), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 0xF), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 0xF), 1, CODES_END };
u16 actor0Talk1Actions[] = { 0x7A02, 1, CODES_END };
u16 actor1Talk0Conditions[] = { FLAG(0x1A, 0xE), 0, CODES_END };
u16 actor1Talk0Actions[] = { FLAG(0x1A, 0xE), 1, CODES_END };
u16 actor1Talk1Conditions[] = { FLAG(0x1A, 0xE), 1, CODES_END };
u16 actor1Talk1Actions[] = { 0x7A00, 1, CODES_END };
u16 actor2Talk0Conditions[] = { FLAG(0x1A, 0x11), 0, CODES_END };
u16 actor2Talk0Actions[] = { FLAG(0x1A, 0x11), 1, CODES_END };
u16 actor2Talk1Conditions[] = { FLAG(0x1A, 0x11), 1, ITEM(0, 8), 0, CODES_END };
u16 actor2Talk2Conditions[] = {
    FLAG(0x1A, 0x11), 1,
    ITEM(0, 8), 1,
    FLAG(0x1C, 0x1E), 0,
    CODES_END,
};
u16 actor2Talk2Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor2Talk3Conditions[] = {
    FLAG(0x1A, 0x11), 1,
    ITEM(0, 8), 1,
    FLAG(0x1C, 0x1E), 1,
    CODES_END,
};
u16 actor3Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor3Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor3Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor3Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0x1A, 0x11), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0x1A, 0x11), 1, CODES_END };
u16 actor4Talk1Conditions[] = { ITEM(0, 8), 0, FLAG(0x1A, 0x11), 1, CODES_END };
u16 actor4Talk2Conditions[] = {
    ITEM(0, 8), 1,
    FLAG(0x1A, 0x11), 1,
    FLAG(0x1C, 0x1E), 0,
    CODES_END,
};
u16 actor4Talk2Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor4Talk3Conditions[] = {
    FLAG(0x1A, 0x11), 1,
    ITEM(0, 8), 1,
    FLAG(0x1C, 0x1E), 1,
    CODES_END,
};
u16 actor5Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor5Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor5Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor5Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor6Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor6Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor6Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor6Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor7Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor7Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor7Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor7Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor8Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor8Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor8Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor8Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor9Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor9Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor9Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor9Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor10Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor10Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor10Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor10Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor11Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor11Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor11Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor11Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor12Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor12Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor12Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor12Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor13Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor13Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor13Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor13Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor14Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor14Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor14Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor14Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor15Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor15Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor15Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor15Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor16Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor16Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor16Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor16Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor32Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor32Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor32Talk1Actions[] = { START_EVENT(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor32Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor76Talk0Conditions[] = { FLAG(0x1A, 0x10), 0, CODES_END };
u16 actor76Talk0Actions[] = { FLAG(0x1A, 0x10), 1, CODES_END };
u16 actor76Talk1Conditions[] = { FLAG(0x1A, 0x10), 1, CODES_END };
u16 actor76Talk1Actions[] = { 0x7A01, 1, CODES_END };
u16 actor78Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor78Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor78Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor78Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor79Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor79Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor79Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor79Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor80Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor80Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor80Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor80Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor81Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor81Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor81Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor81Talk2Conditions[] = { ITEM(0, 8), 1, CODES_END };
u16 actor82Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor82Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor82Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor82Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor83Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor83Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor83Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor83Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor84Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor84Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor84Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor84Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor85Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor85Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor85Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor85Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor86Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor86Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor86Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor86Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor87Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor87Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor87Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor87Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor88Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor88Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor88Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor88Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor89Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor89Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor89Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor89Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor90Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor90Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor90Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor90Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor91Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor91Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor91Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor91Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor92Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor92Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor92Talk1Actions[] = { START_EVENT(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor92Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x42 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x26 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x43 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x25 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x45 },
    { actor2Talk1Conditions, NULL, 0x46 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x47 },
    { actor2Talk3Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x46 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x47 },
    { actor3Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x45 },
    { actor4Talk1Conditions, NULL, 0x46 },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x47 },
    { actor4Talk3Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, NULL, 0x46 },
    { actor5Talk1Conditions, actor5Talk1Actions, 0x47 },
    { actor5Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { actor6Talk0Conditions, NULL, 0x46 },
    { actor6Talk1Conditions, actor6Talk1Actions, 0x47 },
    { actor6Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x46 },
    { actor7Talk1Conditions, actor7Talk1Actions, 0x47 },
    { actor7Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x46 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0x47 },
    { actor8Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, NULL, 0x46 },
    { actor9Talk1Conditions, actor9Talk1Actions, 0x47 },
    { actor9Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, NULL, 0x46 },
    { actor10Talk1Conditions, actor10Talk1Actions, 0x47 },
    { actor10Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { actor11Talk0Conditions, NULL, 0x46 },
    { actor11Talk1Conditions, actor11Talk1Actions, 0x47 },
    { actor11Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { actor12Talk0Conditions, NULL, 0x46 },
    { actor12Talk1Conditions, actor12Talk1Actions, 0x47 },
    { actor12Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { actor13Talk0Conditions, NULL, 0x46 },
    { actor13Talk1Conditions, actor13Talk1Actions, 0x47 },
    { actor13Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x46 },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x47 },
    { actor14Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0x46 },
    { actor15Talk1Conditions, actor15Talk1Actions, 0x47 },
    { actor15Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, NULL, 0x46 },
    { actor16Talk1Conditions, actor16Talk1Actions, 0x47 },
    { actor16Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { actor32Talk0Conditions, NULL, 0x46 },
    { actor32Talk1Conditions, actor32Talk1Actions, 0x47 },
    { actor32Talk2Conditions, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0xBB },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x21C },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x213 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x214 },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x215 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x216 },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x217 },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x218 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0x219 },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor55Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor56Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor57Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor58Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor59Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor60Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor61Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor62Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor63Talks[] = {
    { NULL, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor64Talks[] = {
    { NULL, NULL, 0x212 },
    { NULL, NULL, 0 },
};
FieldTalk actor65Talks[] = {
    { NULL, NULL, 0x211 },
    { NULL, NULL, 0 },
};
FieldTalk actor66Talks[] = {
    { NULL, NULL, 0x208 },
    { NULL, NULL, 0 },
};
FieldTalk actor67Talks[] = {
    { NULL, NULL, 0x209 },
    { NULL, NULL, 0 },
};
FieldTalk actor68Talks[] = {
    { NULL, NULL, 0x20A },
    { NULL, NULL, 0 },
};
FieldTalk actor69Talks[] = {
    { NULL, NULL, 0x20B },
    { NULL, NULL, 0 },
};
FieldTalk actor70Talks[] = {
    { NULL, NULL, 0x20C },
    { NULL, NULL, 0 },
};
FieldTalk actor71Talks[] = {
    { NULL, NULL, 0x20D },
    { NULL, NULL, 0 },
};
FieldTalk actor72Talks[] = {
    { NULL, NULL, 0x20E },
    { NULL, NULL, 0 },
};
FieldTalk actor73Talks[] = {
    { NULL, NULL, 0x20F },
    { NULL, NULL, 0 },
};
FieldTalk actor74Talks[] = {
    { NULL, NULL, 0x436 },
    { NULL, NULL, 0 },
};
FieldTalk actor75Talks[] = {
    { NULL, NULL, 0x210 },
    { NULL, NULL, 0 },
};
FieldTalk actor76Talks[] = {
    { actor76Talk0Conditions, actor76Talk0Actions, 0x44 },
    { actor76Talk1Conditions, actor76Talk1Actions, 0x27 },
    { NULL, NULL, 0 },
};
FieldTalk actor77Talks[] = {
    { NULL, NULL, 0x436 },
    { NULL, NULL, 0 },
};
FieldTalk actor78Talks[] = {
    { actor78Talk0Conditions, NULL, 0x48 },
    { actor78Talk1Conditions, actor78Talk1Actions, 0x49 },
    { actor78Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor79Talks[] = {
    { actor79Talk0Conditions, NULL, 0x48 },
    { actor79Talk1Conditions, actor79Talk1Actions, 0x49 },
    { actor79Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor80Talks[] = {
    { actor80Talk0Conditions, NULL, 0x48 },
    { actor80Talk1Conditions, actor80Talk1Actions, 0x49 },
    { actor80Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor81Talks[] = {
    { actor81Talk0Conditions, NULL, 0x48 },
    { actor81Talk1Conditions, actor81Talk1Actions, 0x49 },
    { actor81Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor82Talks[] = {
    { actor82Talk0Conditions, NULL, 0x48 },
    { actor82Talk1Conditions, actor82Talk1Actions, 0x49 },
    { actor82Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor83Talks[] = {
    { actor83Talk0Conditions, NULL, 0x48 },
    { actor83Talk1Conditions, actor83Talk1Actions, 0x49 },
    { actor83Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor84Talks[] = {
    { actor84Talk0Conditions, NULL, 0x48 },
    { actor84Talk1Conditions, actor84Talk1Actions, 0x49 },
    { actor84Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor85Talks[] = {
    { actor85Talk0Conditions, NULL, 0x48 },
    { actor85Talk1Conditions, actor85Talk1Actions, 0x49 },
    { actor85Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor86Talks[] = {
    { actor86Talk0Conditions, NULL, 0x48 },
    { actor86Talk1Conditions, actor86Talk1Actions, 0x49 },
    { actor86Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor87Talks[] = {
    { actor87Talk0Conditions, NULL, 0x48 },
    { actor87Talk1Conditions, actor87Talk1Actions, 0x49 },
    { actor87Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor88Talks[] = {
    { actor88Talk0Conditions, NULL, 0x48 },
    { actor88Talk1Conditions, actor88Talk1Actions, 0x49 },
    { actor88Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor89Talks[] = {
    { actor89Talk0Conditions, NULL, 0x48 },
    { actor89Talk1Conditions, actor89Talk1Actions, 0x49 },
    { actor89Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor90Talks[] = {
    { actor90Talk0Conditions, NULL, 0x48 },
    { actor90Talk1Conditions, actor90Talk1Actions, 0x49 },
    { actor90Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor91Talks[] = {
    { actor91Talk0Conditions, NULL, 0x48 },
    { actor91Talk1Conditions, actor91Talk1Actions, 0x49 },
    { actor91Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor92Talks[] = {
    { actor92Talk0Conditions, NULL, 0x48 },
    { actor92Talk1Conditions, actor92Talk1Actions, 0x49 },
    { actor92Talk2Conditions, NULL, 0xBC },
    { NULL, NULL, 0 },
};
FieldTalk actor93Talks[] = {
    { NULL, NULL, 0x24 },
    { NULL, NULL, 0 },
};
u16 actor2Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(4), 1, CODES_END };
u16 actor3Conditions[] = {
    FLAG(0x1C, 0x1E), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor4Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(6), 1, CODES_END };
u16 actor5Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(7), 1, CODES_END };
u16 actor6Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(9), 1, CODES_END };
u16 actor7Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0xA), 1, CODES_END };
u16 actor8Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0xF), 1, CODES_END };
u16 actor9Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x11), 1, CODES_END };
u16 actor10Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x12), 1, CODES_END };
u16 actor11Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x14), 1, CODES_END };
u16 actor12Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x15), 1, CODES_END };
u16 actor13Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x19), 1, CODES_END };
u16 actor14Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1B), 1, CODES_END };
u16 actor15Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1D), 1, CODES_END };
u16 actor16Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x21), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(6), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor18Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    FLAG(0x1C, 0x1E), 1,
    CODES_END,
};
u16 actor19Conditions[] = { PROGRESS(4), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(7), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(9), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0xA), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(0xF), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(0x11), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor25Conditions[] = { PROGRESS(0x12), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x14), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(0x15), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(0x19), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0x1B), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0x1D), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0x21), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0x23), 1, FLAG(0x1C, 0x1E), 0, CODES_END };
u16 actor33Conditions[] = { PROGRESS(0x23), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor36Conditions[] = { PROGRESS(6), 1, CODES_END };
u16 actor37Conditions[] = { PROGRESS(7), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(9), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(0xA), 1, CODES_END };
u16 actor40Conditions[] = { PROGRESS(0xF), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(0x11), 1, CODES_END };
u16 actor42Conditions[] = { PROGRESS(0x19), 1, CODES_END };
u16 actor43Conditions[] = { PROGRESS(0x14), 1, CODES_END };
u16 actor44Conditions[] = { PROGRESS(0x12), 1, CODES_END };
u16 actor45Conditions[] = { PROGRESS(0x21), 1, CODES_END };
u16 actor46Conditions[] = { PROGRESS(0x1D), 1, CODES_END };
u16 actor47Conditions[] = { PROGRESS(0x1B), 1, CODES_END };
u16 actor48Conditions[] = { PROGRESS(0x15), 1, CODES_END };
u16 actor49Conditions[] = { PROGRESS(5), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor50Conditions[] = { PROGRESS(0x26), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor51Conditions[] = { PROGRESS(8), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor52Conditions[] = { FLAG(0x1C, 0x1E), 1, PROGRESS(0xC), 1, CODES_END };
u16 actor53Conditions[] = { PROGRESS(0xE), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor54Conditions[] = { PROGRESS(0x10), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor55Conditions[] = { PROGRESS(0x16), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor56Conditions[] = { PROGRESS(0x1A), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor57Conditions[] = { PROGRESS(0x1C), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor58Conditions[] = { PROGRESS(0x1E), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor59Conditions[] = { PROGRESS(0x1F), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor60Conditions[] = { PROGRESS(0x22), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor61Conditions[] = { PROGRESS(0x24), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor62Conditions[] = { PROGRESS(0x25), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor63Conditions[] = { PROGRESS(0x18), 1, FLAG(0x1C, 0x1E), 1, CODES_END };
u16 actor64Conditions[] = { PROGRESS(0x23), 1, CODES_END };
u16 actor65Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor66Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor67Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor68Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor69Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor70Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor71Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor72Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor73Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor74Conditions[] = { SPECIAL(0x1A), 1, ITEM(0, 8), 1, CODES_END };
u16 actor75Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor77Conditions[] = { SPECIAL(0x1A), 1, ITEM(0, 8), 0, CODES_END };
u16 actor78Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0xE), 1, CODES_END };
u16 actor79Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x16), 1, CODES_END };
u16 actor80Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x18), 1, CODES_END };
u16 actor81Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1A), 1, CODES_END };
u16 actor82Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x10), 1, CODES_END };
u16 actor83Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1C), 1, CODES_END };
u16 actor84Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1E), 1, CODES_END };
u16 actor85Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(8), 1, CODES_END };
u16 actor86Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x25), 1, CODES_END };
u16 actor87Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x1F), 1, CODES_END };
u16 actor88Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(5), 1, CODES_END };
u16 actor89Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0xC), 1, CODES_END };
u16 actor90Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x22), 1, CODES_END };
u16 actor91Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x24), 1, CODES_END };
u16 actor92Conditions[] = { FLAG(0x1C, 0x1E), 0, PROGRESS(0x26), 1, CODES_END };
u16 actor93Conditions[] = { PROGRESS(4), 1, CODES_END };
FieldActorEntry actor0 = { NULL, actor0Talks, 0x16, 4, 449, 281, 7 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x17, 5, 304, 369, 7 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x1E, 6, 152, 397, 7 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x1E, 6, 144, 352, 1 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x2D, 7, 287, 432, 5 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor55 = { actor55Conditions, actor55Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor56 = { actor56Conditions, actor56Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor57 = { actor57Conditions, actor57Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor58 = { actor58Conditions, actor58Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor59 = { actor59Conditions, actor59Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor60 = { actor60Conditions, actor60Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor61 = { actor61Conditions, actor61Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor62 = { actor62Conditions, actor62Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor63 = { actor63Conditions, actor63Talks, 0x2D, 7, 144, 352, 1 };
FieldActorEntry actor64 = { actor64Conditions, actor64Talks, 0x2D, 7, 336, 206, 7 };
FieldActorEntry actor65 = { actor65Conditions, actor65Talks, 0x33, 8, 480, 336, 3 };
FieldActorEntry actor66 = { actor66Conditions, actor66Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor67 = { actor67Conditions, actor67Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor68 = { actor68Conditions, actor68Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor69 = { actor69Conditions, actor69Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor70 = { actor70Conditions, actor70Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor71 = { actor71Conditions, actor71Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor72 = { actor72Conditions, actor72Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor73 = { actor73Conditions, actor73Talks, 0x33, 8, 287, 432, 5 };
FieldActorEntry actor74 = { actor74Conditions, actor74Talks, 0x9D, 9, 144, 352, 1 };
FieldActorEntry actor75 = { actor75Conditions, actor75Talks, 0x9E, 0xA, 287, 432, 5 };
FieldActorEntry actor76 = { NULL, actor76Talks, 0xB7, 0xB, 359, 308, 7 };
FieldActorEntry actor77 = { actor77Conditions, actor77Talks, 0x119, 0xC, 152, 397, 7 };
FieldActorEntry actor78 = { actor78Conditions, actor78Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor79 = { actor79Conditions, actor79Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor80 = { actor80Conditions, actor80Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor81 = { actor81Conditions, actor81Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor82 = { actor82Conditions, actor82Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor83 = { actor83Conditions, actor83Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor84 = { actor84Conditions, actor84Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor85 = { actor85Conditions, actor85Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor86 = { actor86Conditions, actor86Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor87 = { actor87Conditions, actor87Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor88 = { actor88Conditions, actor88Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor89 = { actor89Conditions, actor89Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor90 = { actor90Conditions, actor90Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor91 = { actor91Conditions, actor91Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor92 = { actor92Conditions, actor92Talks, 0x126, 0xD, 152, 397, 7 };
FieldActorEntry actor93 = { actor93Conditions, actor93Talks, 0x13B, 0xE, 186, 413, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 384, 79, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 492, 137, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 2, 0, 1, 4, 0, 548, 165, 0, 0 },
    { 1, 0, 0x40, 6, 3, 1, 3, 8, 4, 0, 485, 145, 0, 0 },
    { 1, 0, 0x40, 6, 9, 1, 9, 0xE, 4, 0, 541, 172, 0, 0 },
    { 1, 0x64, 0x40, 6, 0x12, 0, 0, 0, 0, 0, 304, 99, 0, 0 },
    { 1, 0x65, 0x40, 6, 0x13, 0, 0, 0, 0, 0, 64, 285, 0, 0 },
    { 1, 0, 0xA0, 4, 0, 0, 0, 0, 0, 0, 144, 257, 380, 0 },
    { 1, 0, 0xA0, 4, 1, 0, 0, 0, 0, 0, 256, 257, 337, 0 },
    { 1, 0, 0xA0, 4, 2, 0, 0, 0, 0, 0, 384, 261, 287, 0 },
    { 1, 0, 0x60, 4, 0xF, 0, 0, 0, 0, 0, 240, 73, 150, 0 },
    { 1, 0, 0x60, 4, 0x10, 0, 0, 0, 0, 0, 0, 255, 335, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x1E0, 0x1DA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x240, 0x1AA, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20E, 0x218, 0xE4, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x20E, 0x128, 0x19C, 3, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 4, 0x11F, 0xBA, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 4, 0x110, 0xFE, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void (*stageFuncs[])(void) = {
    setupStage,
};
FieldEvent stageEvents[] = {
    { 1250, script1250, EVENT_TEXT(0x2C), NULL, NULL },
    { 1251, script1251, EVENT_TEXT(0x2D), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
