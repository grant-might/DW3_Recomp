/*
 * WSTAG280: El Dorado, Asuka City.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"

/* The text file of the menus, which the versions number differently */
#if VERSION_US || VERSION_JP
#define MENU_TEXT TEXT_FILE(0x10C)
#elif VERSION_EU
#define MENU_TEXT 0x112
#endif

/* A two-option menu: creates the event object of the chosen option */
void updateMenu(StageMenu *task, ChoiceChildren *children) {
    SpriteDrawer drawer;
    s32 prev;
    s32 i;
    s32 j;
    s32 k;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->tween.duration = 10;
        for (i = 0; i < 2; i++) {
            children->options[i] = createTextWindow(FIELD_LAYER_MAP, 1, 0x1C, 0xBE + i * 14);
            children->options[i]->setDepth(children->options[i], 1);
        }
        children->cursor = createCursor(FIELD_LAYER_MAP, 1, 0x12, 0xBE);
        children->cursor->setVisible(children->cursor, 0);
        children->title = createTextWindow(FIELD_LAYER_MAP, 1, 0x12, 0xB0);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            stageFuncs.start(&task->tween, 1);
            task->substate++;
            break;
        case 1:
            if (stageFuncs.update(&task->tween)) {
                for (j = 0; j < 2; j++) {
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x39)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x39)), 1);
                task->substate++;
            }
            break;
        case 2:
            prev = task->cursor;
            if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
                if (--task->cursor < 0) {
                    task->cursor = 0;
                }
            } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
                task->cursor++;
                if (task->cursor > 1) {
                    task->cursor = 1;
                }
            }
            if (prev != task->cursor) {
                SOUND.playSound(SOUND_CURSOR);
                children->cursor->setPos(children->cursor, 0x12, task->cursor * 14 + 0xBE);
                break;
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                task->substate = 10;
                task->step = 1;
            }
            break;
        case 3:
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x39 : 0x5F3);
            task->substate++;
            break;
        case 4:
            if (children->event == NULL) {
                task->state = TASK_KILL;
            }
            break;
        case 10:
            for (k = 0; k < 2; k++) {
                children->options[k]->setVisible(children->options[k], 0);
            }
            children->cursor->setVisible(children->cursor, 0);
            children->title->setVisible(children->title, 0);
            stageFuncs.start(&task->tween, 0);
            task->substate++;
            break;
        case 11:
            if (stageFuncs.update(&task->tween)) {
                if (task->step == 1) {
                    task->substate = 3;
                } else {
                    task->state = TASK_KILL;
                }
            }
            break;
        }
        initSpriteDrawer(&drawer);
        drawer.setLayerId(FIELD_LAYER_MAP, 2);
        drawer.setTexture(0x140, 0);
        drawer.setFollowScroll(0);
        if (task->tween.level != 0) {
            if (task->tween.level != 0x1000) {
                drawer.setScale(task->tween.level, 0x1000, 0x1000);
                drawer.setPivot(0, 0xC3);
            }
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), 0x45, 0, 0xAC);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/*
 * Starts event 1522, which asks whether to listen to basic item information:
 * yes starts event 57, no event 1523
 */
void *startEvent1522(void) {
    return createTask(updateMenu, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* Creates the event object while flags 0x7201, 0x8008 and 0x701A are clear */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        if (FLAGS_00.checkCondition(PARTY_STAT(1), 0) && FLAGS_00.checkCondition(ITEM(0, 8), 0) && FLAGS_00.checkCondition(SPECIAL(0x1A), 0)) {
            children[0] = FIELDSTG_startEvent(0x42);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

#define STAGE_CHILDREN_SIZE 4
#include "common/start_stage.inc.c"

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xCD)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x1A5
#define STAGE_ARCHIVE 0x3C3
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xC5)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x1B3
#define STAGE_ARCHIVE 0x3D3
#endif
/* Sets the stage up: its map, actors and events, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1BB00, 0xF400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

s16 script57[] = {
    SCRIPT_WALK(2, 0x180, 0xB0, 3),
    SCRIPT_POSE(0x10B, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x10B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x10B, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x10B, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script66[] = {
    SCRIPT_PLACE(2, 0x218, 0xF4),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_PLACE(0x2D, 0x1F6, 0xE4),
    SCRIPT_POSE(0x2D, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x210, 0xF1, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x2D, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x22B, 0xFC, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_LEAVE(0x201, 0x180, 0xD8, 7),
    SCRIPT_END,
};
s16 script67[] = {
    SCRIPT_WALK(2, 0x14F, 0xD0, 3),
    SCRIPT_PLACE(0x118, 0x131, 0xC1),
    SCRIPT_POSE(0x118, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x118, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x118, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x168, 0xC4, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WALK(0x118, 0x170, 0xE0, 7),
    SCRIPT_WAIT_WALK(0x118),
    SCRIPT_WALK(2, 0x14F, 0xD0, 3),
    SCRIPT_POSE(0x118, 1, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1523[] = {
    SCRIPT_WALK(2, 0x180, 0xB0, 3),
    SCRIPT_POSE(0x10B, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x10B, 2),
    SCRIPT_WAIT_BOX,
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
    { 0x140, 0x100, 0x178, 0x17A, 0xE0, 0x7A, 0x160, 0x1FF },
    { 0x140, 0x100, 0x158, 0x182, 0x60, 0x82, 0x170, 0x1FF },
    { 0x140, 0x100, 0x170, 0x17A, 0xC0, 0x7A, 0x160, 0x1FE },
    { 0x140, 0x100, 0x140, 0x182, 0, 0x82, 0x170, 0x1FE },
    { 0x140, 0x100, 0x160, 0x189, 0x80, 0x89, 0x140, 0x1FD },
    { 0x140, 0x100, 0x168, 0x189, 0xA0, 0x89, 0x150, 0x1FD },
    { 0x140, 0x100, 0x158, 0x1A2, 0x60, 0xA2, 0x160, 0x1FD },
    { 0x140, 0x100, 0x170, 0x1A2, 0xC0, 0xA2, 0x170, 0x1FD },
    { 0x140, 0x100, 0x148, 0x182, 0x20, 0x82, 0x140, 0x1FC },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x150, 0x182, 0x40, 0x82, 0x150, 0x1FC },
    { 0x140, 0x100, 0x160, 0x1A9, 0x80, 0xA9, 0x160, 0x1FC },
};
u16 actor42Talk0Actions[] = { START_EVENT(0), 1, CODES_END };
u16 actor43Talk0Conditions[] = { FLAG(0x1A, 5), 0, CODES_END };
u16 actor43Talk0Actions[] = { FLAG(0x1A, 5), 1, CODES_END };
u16 actor43Talk1Conditions[] = { FLAG(0x1A, 5), 1, PARTY_STAT(2), 0, CODES_END };
u16 actor43Talk1Actions[] = { START_EVENT(0), 1, CODES_END };
u16 actor43Talk2Conditions[] = { FLAG(0x1A, 5), 1, PARTY_STAT(2), 1, ITEM(0, 8), 0, CODES_END };
u16 actor43Talk2Actions[] = { ITEM(0, 8), 1, SPECIAL(0x13), 1, CODES_END };
u16 actor43Talk3Conditions[] = { FLAG(0x1A, 5), 1, PARTY_STAT(2), 1, ITEM(0, 8), 1, CODES_END };
u16 actor43Talk3Actions[] = { START_EVENT(0), 1, CODES_END };
u16 actor45Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor45Talk1Conditions[] = { ITEM(0, 8), 1, FLAG(0, 0), 0, CODES_END };
u16 actor45Talk1Actions[] = { START_EVENT(0x22), 1, FLAG(0, 0), 1, CODES_END };
u16 actor45Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0, 0), 1, CODES_END };
u16 actor46Talk0Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor46Talk1Conditions[] = { FLAG(0, 0), 0, ITEM(0, 8), 1, CODES_END };
u16 actor46Talk1Actions[] = { START_EVENT(0x22), 1, FLAG(0, 0), 1, CODES_END };
u16 actor46Talk2Conditions[] = { ITEM(0, 8), 1, FLAG(0, 0), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x8A },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x469 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x471 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x46A },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x46B },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x46C },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x46D },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x473 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x46E },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x46F },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x470 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x474 },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x47C },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { NULL, NULL, 0x475 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0x476 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x477 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x478 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x47E },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x47B },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x479 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0x47A },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x45E },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x467 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x45F },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x460 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x461 },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x462 },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x468 },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x463 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x465 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x464 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x8B },
    { NULL, NULL, 0 },
};
FieldTalk actor39Talks[] = {
    { NULL, NULL, 0x466 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0x472 },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x47D },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, actor42Talk0Actions, 0x138 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { actor43Talk0Conditions, actor43Talk0Actions, 0x32 },
    { actor43Talk1Conditions, actor43Talk1Actions, 0x138 },
    { actor43Talk2Conditions, actor43Talk2Actions, 0x139 },
    { actor43Talk3Conditions, actor43Talk3Actions, 0x138 },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { actor45Talk0Conditions, NULL, 0xA3 },
    { actor45Talk1Conditions, actor45Talk1Actions, 0xA2 },
    { actor45Talk2Conditions, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { actor46Talk0Conditions, NULL, 0xA3 },
    { actor46Talk1Conditions, actor46Talk1Actions, 0xA2 },
    { actor46Talk2Conditions, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0xA4 },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0xA5 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 8), 0, PARTY_STAT(1), 0, CODES_END };
u16 actor1Conditions[] = { ITEM(0, 8), 1, SPECIAL(0x22), 1, PARTY_STAT(1), 0, CODES_END };
u16 actor2Conditions[] = {
    ITEM(0, 8), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    PARTY_STAT(1), 0,
    CODES_END,
};
u16 actor3Conditions[] = {
    ITEM(0, 8), 1,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    PARTY_STAT(1), 0,
    CODES_END,
};
u16 actor4Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 8), 0, PARTY_STAT(1), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 8), 1, PARTY_STAT(1), 1, CODES_END };
u16 actor6Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 8), 0,
    PARTY_STAT(1), 1,
    CODES_END,
};
u16 actor7Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 8), 1,
    PARTY_STAT(1), 1,
    CODES_END,
};
u16 actor8Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor14Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor16Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor18Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor26Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor29Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor33Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor36Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor38Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor39Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor40Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor41Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor42Conditions[] = { ITEM(0, 8), 1, CODES_END };
u16 actor43Conditions[] = { ITEM(0, 8), 0, CODES_END };
u16 actor45Conditions[] = { SPECIAL(0x22), 1, ITEM(0, 8), 0, CODES_END };
u16 actor46Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, ITEM(0, 8), 0, CODES_END };
u16 actor47Conditions[] = { ITEM(0, 8), 1, SPECIAL(0x22), 1, CODES_END };
u16 actor48Conditions[] = { ITEM(0, 8), 1, PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor49Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 502, 228, 7 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2D, 4, 502, 228, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x2D, 4, 471, 245, 5 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2E, 5, 352, 296, 5 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x33, 6, 216, 277, 7 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x37, 7, 264, 301, 3 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x9D, 8, 471, 245, 5 };
FieldActorEntry actor39 = { actor39Conditions, actor39Talks, 0x9F, 9, 264, 301, 3 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0xA0, 0xA, 352, 296, 5 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0xA1, 0xB, 216, 277, 7 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x10B, 0xC, 353, 160, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x10B, 0xC, 353, 160, 7 };
FieldActorEntry actor44 = { NULL, NULL, 0x10C, 0xD, 360, 172, 7 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x118, 0xE, 305, 193, 7 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x118, 0xE, 305, 193, 7 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0x118, 0xE, 368, 224, 3 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0x118, 0xE, 368, 224, 3 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x119, 0xF, 368, 224, 3 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0xA, 0, 0, 0, 0, 0, 93, 125, 0, 0 },
    { 1, 0, 0x40, 2, 0xB, 0, 0, 0, 0, 0, 126, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0xC, 0, 0, 0, 0, 0, 158, 93, 0, 0 },
    { 1, 0, 0x40, 2, 0xD, 0, 0, 0, 0, 0, 235, 67, 0, 0 },
    { 1, 0, 0x40, 2, 0xE, 0, 0, 0, 0, 0, 282, 43, 0, 0 },
    { 1, 0, 0x40, 2, 0xF, 0, 0, 0, 0, 0, 379, 34, 0, 0 },
    { 1, 0, 0x40, 2, 0x10, 0, 0, 0, 0, 0, 403, 46, 0, 0 },
    { 1, 0, 0x40, 2, 0x11, 0, 0, 0, 0, 0, 427, 90, 0, 0 },
    { 1, 0, 0x40, 2, 0x12, 0, 0, 0, 0, 0, 463, 113, 0, 0 },
    { 1, 0, 0x40, 2, 0x13, 0, 0, 0, 0, 0, 482, 109, 0, 0 },
    { 1, 0, 0x40, 2, 0x14, 0, 0, 0, 0, 0, 503, 123, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 84, 130, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 116, 114, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 148, 98, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 272, 44, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 368, 36, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 417, 91, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 453, 113, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 473, 112, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 493, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 497, 124, 0, 0 },
    { 1, 0, 0x40, 6, 0x33, 2, 0, 1, 4, 0, 292, 104, 0, 0 },
    { 1, 0, 0x40, 6, 0x15, 0, 0, 0, 0, 0, 416, 59, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 224, 68, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 392, 48, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 2, 0, 1, 4, 0, 416, 60, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 208, 263, 288, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 384, 191, 217, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 320, 169, 184, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 304, 161, 174, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 336, 161, 174, 0 },
    { 1, 0, 0x40, 4, 5, 0, 0, 0, 0, 0, 288, 153, 166, 0 },
    { 1, 0, 0x40, 4, 6, 0, 0, 0, 0, 0, 352, 153, 166, 0 },
    { 1, 0, 0x40, 4, 7, 0, 0, 0, 0, 0, 272, 144, 158, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 368, 145, 158, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x201, 0x180, 0xD8, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x212, 0x92, 0x1A2, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_DOWN, 3, 0x110, 0xB8, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_CLIMB_UP, 3, 0x100, 0xF0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
FieldEvent stageEvents[] = {
    { 57, script57, EVENT_TEXT(0x11), NULL, NULL },
    { 66, script66, EVENT_TEXT(0x13), NULL, NULL },
    { 67, script67, EVENT_TEXT(0x14), NULL, NULL },
    { 1522, NULL, EVENT_TEXT(0x39), startEvent1522, NULL },
    { 1523, script1523, EVENT_TEXT(0x34), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
