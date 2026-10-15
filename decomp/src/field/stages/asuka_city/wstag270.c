/*
 * WSTAG270: Yellow Cruiser, Asuka City.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"

/* The text file of the menus, which the versions number differently */
#if VERSION_US || VERSION_JP
#define MENU_TEXT TEXT_FILE(0x11A)
#elif VERSION_EU
#define MENU_TEXT 0x120
#endif

/* A two-option menu: creates the event object of the chosen option */
void updateEvent1518(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x5)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x5)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x3B : 0x5EF);
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
 * Starts event 1518, which asks whether to listen to the basic card rules:
 * yes starts event 59, no event 1519
 */
void *startEvent1518(void) {
    return createTask(updateEvent1518, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* A two-option menu: creates the event object of the chosen option */
void updateEvent1520(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x6)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x6)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x41 : 0x5F1);
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
 * Starts event 1520, which asks whether to listen to the card rules: yes
 * starts event 65, no event 1521
 */
void *startEvent1520(void) {
    return createTask(updateEvent1520, sizeof(StageMenu), sizeof(ChoiceChildren));
}

#include "common/update_stage.inc.c"

#include "common/start_stage.inc.c"

/*
 * After event 1415, with Divermon, Tamer Nacky, Tamer Steve, Tamer Gloria
 * and Tamer Wong: gives the player the Folder Bag and sets flags 0x1A1C
 * (which decides what actor 30 says and what actor 15 in WSTAG425 says) and
 * 0x40A1
 */
void endEvent1415(void) {
    FLAGS_00.applyAction(ITEM(0, 0x192), 1);
    FLAGS_00.applyAction(FLAG(0x1A, 0x1C), 1);
    FLAGS_00.applyAction(FLAG(0x40, 0xA1), 1);
}

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xCD)
#define EVENT_TEXT_FILE 0x119
#define STAGE_FILE 0x1A1
#define STAGE_ARCHIVE 0x3C2
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xC5)
#define EVENT_TEXT_FILE 0x120
#define STAGE_FILE 0x1AF
#define STAGE_ARCHIVE 0x3D2
#endif
/* Sets the stage up: its map, actors and events, playing SHOP2BGM */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x1A400, 0xC800};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 8;
    FIELDSTG_state.music = MUSIC(8, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

s16 script59[] = {
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x33, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x33, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x33, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x4644,
#elif VERSION_EU
    0x104,
#endif
};
s16 script65[] = {
    SCRIPT_WALK(2, 0x117, 0xBD, 3),
    SCRIPT_POSE(0x2D, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x2D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x2D, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x2D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1415[] = {
    SCRIPT_WALK(2, 0x19B, 0xA3, 3),
    SCRIPT_POSE(0x2F, 1, 7),
    SCRIPT_POSE(0x32, 1, 7),
    SCRIPT_POSE(0x34, 1, 3),
    SCRIPT_POSE(0x37, 1, 3),
    SCRIPT_POSE(0xCE, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_TALK(0, 1, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xCE),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0xCE),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_LOOK_AT(0, 0xD0, 0xE1),
    SCRIPT_WAIT(0x96),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0x34),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x34),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x34, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x34, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0x2F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 0x34, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 0x2F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(1, 0xC, 0x2F, 2),
    SCRIPT_TALK(0, 0xB, 0x34, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x325, 0x34),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x323, 0x326, 0x34),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0x34, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 0x2F, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 0x34),
    SCRIPT_WAIT(0xB4),
    SCRIPT_TALK(0, 0xF, 0x32, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x326, 0x34),
    SCRIPT_COMMAND(0x324, 0x325, 0x37),
    SCRIPT_WAIT(0x5A),
    SCRIPT_COMMAND(0x324, 0x326, 0x37),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0x32, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x12, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(0x323, 0x327, 0x37),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x13, 0x32, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x37),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x14, 0x37, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x15, 0x32, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 2),
    SCRIPT_WAIT(0x5A),
    SCRIPT_TALK(0, 0x16, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON3, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x17, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x18, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x19, 2, 1),
    SCRIPT_POSE(2, 7, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x1A, 0xCE, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1519[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x33, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1521[] = {
    SCRIPT_WALK(2, 0x117, 0xBD, 3),
    SCRIPT_POSE(0x2D, 1, 7),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x2D, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_WAIT(0x1E),
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
    { 0x180, 0x100, 0x1B8, 0x159, 0x1E0, 0x59, 0x160, 0x1FB },
    { 0x180, 0x100, 0x1B0, 0x159, 0x1C0, 0x59, 0x170, 0x1FB },
    { 0x180, 0x100, 0x1A0, 0x15D, 0x180, 0x5D, 0x150, 0x1FA },
    { 0x180, 0x100, 0x1A8, 0x15D, 0x1A0, 0x5D, 0x160, 0x1FA },
    { 0x180, 0x100, 0x190, 0x171, 0x140, 0x71, 0x170, 0x1FA },
    { 0x180, 0x100, 0x198, 0x171, 0x160, 0x71, 0x140, 0x1F9 },
    { 0x140, 0x100, 0x140, 0x1D8, 0, 0xD8, 0x150, 0x1F9 },
    { 0x180, 0x100, 0x180, 0x172, 0x100, 0x72, 0x160, 0x1F9 },
    { 0x180, 0x100, 0x188, 0x172, 0x120, 0x72, 0x170, 0x1F9 },
    { 0x180, 0x100, 0x1B0, 0x181, 0x1C0, 0x81, 0x140, 0x1F8 },
    { 0x180, 0x100, 0x1A0, 0x185, 0x180, 0x85, 0x150, 0x1F8 },
    { 0x180, 0x100, 0x1A8, 0x185, 0x1A0, 0x85, 0x160, 0x1F8 },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x170, 0x1F8 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Actions[] = { START_EVENT(0x1C), 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x1C), 1, CODES_END };
u16 actor2Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor2Talk1Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor2Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk2Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor2Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk3Conditions[] = { FLAG(0, 2), 0, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor2Talk3Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor2Talk4Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(0), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk5Conditions[] = {
    PARTY_STAT(4), 0,
    PARTY_STAT(0), 1,
    FLAG(0, 2), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk5Actions[] = { CARD_BATTLE(3, 0), 1, CODES_END };
u16 actor2Talk6Conditions[] = {
    FLAG(0, 2), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor2Talk6Actions[] = { CARD_BATTLE(3, 1), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor3Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor3Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor3Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0, 2), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor4Talk2Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor4Talk2Actions[] = { CARD_BATTLE(3, 0), 1, CODES_END };
u16 actor4Talk3Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor4Talk3Actions[] = { CARD_BATTLE(3, 1), 1, CODES_END };
u16 actor5Talk0Conditions[] = { FLAG(0, 2), 0, CODES_END };
u16 actor5Talk0Actions[] = { FLAG(0, 2), 1, CODES_END };
u16 actor5Talk1Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor5Talk2Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor5Talk2Actions[] = { CARD_BATTLE(3, 0), 1, CODES_END };
u16 actor5Talk3Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor5Talk3Actions[] = { CARD_BATTLE(3, 1), 1, CODES_END };
u16 actor7Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor7Talk1Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor7Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk2Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor7Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk3Conditions[] = { FLAG(0, 3), 0, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor7Talk3Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor7Talk4Conditions[] = {
    FLAG(0, 3), 1,
    PARTY_STAT(0), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk5Conditions[] = {
    FLAG(0, 3), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(4), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk5Actions[] = { CARD_BATTLE(4, 0), 1, CODES_END };
u16 actor7Talk6Conditions[] = {
    FLAG(0, 3), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor7Talk6Actions[] = { CARD_BATTLE(4, 1), 1, CODES_END };
u16 actor8Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor8Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor8Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor8Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor9Talk0Conditions[] = { FLAG(0, 3), 0, CODES_END };
u16 actor9Talk0Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor9Talk1Conditions[] = { PARTY_STAT(0), 0, FLAG(0, 3), 1, CODES_END };
u16 actor9Talk2Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor9Talk2Actions[] = { CARD_BATTLE(4, 0), 1, CODES_END };
u16 actor9Talk3Conditions[] = { PARTY_STAT(4), 1, FLAG(0, 3), 1, PARTY_STAT(0), 1, CODES_END };
u16 actor9Talk3Actions[] = { CARD_BATTLE(4, 1), 1, CODES_END };
u16 actor10Talk0Conditions[] = { FLAG(0, 3), 0, CODES_END };
u16 actor10Talk0Actions[] = { FLAG(0, 3), 1, CODES_END };
u16 actor10Talk1Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor10Talk2Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor10Talk2Actions[] = { CARD_BATTLE(4, 0), 1, CODES_END };
u16 actor10Talk3Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor10Talk3Actions[] = { CARD_BATTLE(4, 1), 1, CODES_END };
u16 actor12Talk0Actions[] = { START_EVENT(0x1B), 1, CODES_END };
u16 actor13Talk0Actions[] = { START_EVENT(0x1B), 1, CODES_END };
u16 actor14Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor14Talk1Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor14Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk2Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor14Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk3Conditions[] = { FLAG(0, 0), 0, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor14Talk3Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor14Talk4Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(0), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor14Talk5Conditions[] = {
    FLAG(0, 0), 1,
    PARTY_STAT(4), 0,
    PARTY_STAT(0), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor14Talk5Actions[] = { CARD_BATTLE(1, 0), 1, CODES_END };
u16 actor14Talk6Conditions[] = {
    PARTY_STAT(0), 1,
    FLAG(0, 0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor14Talk6Actions[] = { CARD_BATTLE(1, 1), 1, CODES_END };
u16 actor15Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor15Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor15Talk2Conditions[] = { FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor15Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor16Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor16Talk2Actions[] = { CARD_BATTLE(1, 0), 1, CODES_END };
u16 actor16Talk3Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor16Talk3Actions[] = { CARD_BATTLE(1, 1), 1, CODES_END };
u16 actor17Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor17Talk0Actions[] = { FLAG(0, 0), 1, CODES_END };
u16 actor17Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor17Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(4), 0, PARTY_STAT(0), 1, CODES_END };
u16 actor17Talk2Actions[] = { CARD_BATTLE(1, 0), 1, CODES_END };
u16 actor17Talk3Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor17Talk3Actions[] = { CARD_BATTLE(1, 1), 1, CODES_END };
u16 actor19Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor19Talk1Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 1, CODES_END };
u16 actor19Talk1Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor19Talk2Conditions[] = { ITEM(0, 0x192), 1, FLAG(0, 0x11), 1, FLAG(0, 0x10), 0, CODES_END };
u16 actor19Talk2Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk3Conditions[] = { FLAG(0, 1), 0, ITEM(0, 0x192), 1, FLAG(0, 0x11), 0, CODES_END };
u16 actor19Talk3Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor19Talk4Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor19Talk5Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(4), 0,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor19Talk5Actions[] = { CARD_BATTLE(2, 0), 1, CODES_END };
u16 actor19Talk6Conditions[] = {
    FLAG(0, 1), 1,
    PARTY_STAT(0), 1,
    PARTY_STAT(4), 1,
    ITEM(0, 0x192), 1,
    FLAG(0, 0x11), 0,
    CODES_END,
};
u16 actor19Talk6Actions[] = { CARD_BATTLE(2, 1), 1, CODES_END };
u16 actor20Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor20Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor20Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor20Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor20Talk2Actions[] = { CARD_BATTLE(2, 0), 1, CODES_END };
u16 actor20Talk3Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor20Talk3Actions[] = { CARD_BATTLE(2, 1), 1, CODES_END };
u16 actor21Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor21Talk0Actions[] = { FLAG(0, 1), 1, CODES_END };
u16 actor21Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor21Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor21Talk2Actions[] = { CARD_BATTLE(2, 0), 1, CODES_END };
u16 actor21Talk3Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(4), 1, PARTY_STAT(0), 1, CODES_END };
u16 actor21Talk3Actions[] = { CARD_BATTLE(2, 1), 1, CODES_END };
u16 actor22Talk0Conditions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor22Talk1Conditions[] = { FLAG(0, 0x10), 0, FLAG(0, 0x11), 1, CODES_END };
u16 actor22Talk1Actions[] = { FLAG(0, 0x11), 0, CODES_END };
u16 actor22Talk2Conditions[] = { FLAG(0, 0x10), 1, FLAG(0, 0x11), 1, CODES_END };
u16 actor22Talk2Actions[] = { FLAG(0, 0x11), 0, FLAG(0, 0x10), 0, CODES_END };
u16 actor25Talk0Conditions[] = { FLAG(0, 2), 0, CODES_END };
u16 actor25Talk1Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor25Talk2Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor25Talk2Actions[] = { CARD_BATTLE(3, 0), 1, CODES_END };
u16 actor25Talk3Conditions[] = { FLAG(0, 2), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor25Talk3Actions[] = { CARD_BATTLE(3, 1), 1, CODES_END };
u16 actor26Talk0Conditions[] = { FLAG(0, 0), 0, CODES_END };
u16 actor26Talk1Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor26Talk2Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor26Talk2Actions[] = { CARD_BATTLE(1, 0), 1, CODES_END };
u16 actor26Talk3Conditions[] = { FLAG(0, 0), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor26Talk3Actions[] = { CARD_BATTLE(1, 1), 1, CODES_END };
u16 actor27Talk0Conditions[] = { FLAG(0, 3), 0, CODES_END };
u16 actor27Talk1Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor27Talk2Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor27Talk2Actions[] = { CARD_BATTLE(4, 0), 1, CODES_END };
u16 actor27Talk3Conditions[] = { FLAG(0, 3), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor27Talk3Actions[] = { CARD_BATTLE(4, 1), 1, CODES_END };
u16 actor28Talk0Conditions[] = { FLAG(0, 1), 0, CODES_END };
u16 actor28Talk1Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 0, CODES_END };
u16 actor28Talk2Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 0, CODES_END };
u16 actor28Talk2Actions[] = { CARD_BATTLE(2, 0), 1, CODES_END };
u16 actor28Talk3Conditions[] = { FLAG(0, 1), 1, PARTY_STAT(0), 1, PARTY_STAT(4), 1, CODES_END };
u16 actor28Talk3Actions[] = { CARD_BATTLE(2, 1), 1, CODES_END };
u16 actor30Talk0Conditions[] = { PROGRESS(4), 1, FLAG(0x1A, 0x1C), 0, CODES_END };
u16 actor30Talk0Actions[] = { FLAG(0x1A, 0x1C), 1, CODES_END };
u16 actor30Talk1Conditions[] = { PROGRESS(4), 1, FLAG(0x1A, 0x1C), 1, CODES_END };
u16 actor30Talk1Actions[] = { 0x7A31, 1, CODES_END };
u16 actor30Talk2Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor30Talk2Actions[] = { 0x7A31, 1, CODES_END };
u16 actor30Talk3Conditions[] = { SPECIAL(0x3E), 1, CODES_END };
u16 actor30Talk3Actions[] = { 0x7A32, 1, CODES_END };
u16 actor30Talk4Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor30Talk4Actions[] = { 0x7A33, 1, CODES_END };
u16 actor30Talk5Conditions[] = { SPECIAL(0x20), 1, CODES_END };
u16 actor30Talk5Actions[] = { 0x7A34, 1, CODES_END };
u16 actor30Talk6Conditions[] = { SPECIAL(0x21), 1, PROGRESS(0x26), 0, CODES_END };
u16 actor30Talk6Actions[] = { 0x7A35, 1, CODES_END };
u16 actor30Talk7Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor30Talk7Actions[] = { 0x7A36, 1, CODES_END };
u16 actor30Talk8Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Talk8Actions[] = { 0x7A36, 1, CODES_END };
u16 actor30Talk9Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor30Talk9Actions[] = { 0x7A36, 1, CODES_END };
u16 actor31Talk0Conditions[] = { ITEM(0, 0x192), 0, CODES_END };
u16 actor31Talk0Actions[] = { FLAG(0x1C, 0x48), 1, START_EVENT(0x5C), 1, CODES_END };
u16 actor31Talk1Conditions[] = { ITEM(0, 0x192), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x248 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x31 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, NULL, 0x41D },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x97 },
    { actor2Talk2Conditions, actor2Talk2Actions, 0x96 },
    { actor2Talk3Conditions, actor2Talk3Actions, 0x7A },
    { actor2Talk4Conditions, NULL, 0x7C },
    { actor2Talk5Conditions, actor2Talk5Actions, 0x7D },
    { actor2Talk6Conditions, actor2Talk6Actions, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, NULL, 0x7A },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x96 },
    { actor3Talk2Conditions, actor3Talk2Actions, 0x97 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x7B },
    { actor4Talk1Conditions, NULL, 0x7C },
    { actor4Talk2Conditions, actor4Talk2Actions, 0x7D },
    { actor4Talk3Conditions, actor4Talk3Actions, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x93 },
    { actor5Talk1Conditions, NULL, 0x7C },
    { actor5Talk2Conditions, actor5Talk2Actions, 0x7D },
    { actor5Talk3Conditions, actor5Talk3Actions, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x41D },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { actor7Talk0Conditions, NULL, 0x41E },
    { actor7Talk1Conditions, actor7Talk1Actions, 0xA1 },
    { actor7Talk2Conditions, actor7Talk2Actions, 0xA0 },
    { actor7Talk3Conditions, actor7Talk3Actions, 0x98 },
    { actor7Talk4Conditions, NULL, 0x9D },
    { actor7Talk5Conditions, actor7Talk5Actions, 0x9E },
    { actor7Talk6Conditions, actor7Talk6Actions, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { actor8Talk0Conditions, NULL, 0x98 },
    { actor8Talk1Conditions, actor8Talk1Actions, 0xA0 },
    { actor8Talk2Conditions, actor8Talk2Actions, 0xA1 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { actor9Talk0Conditions, actor9Talk0Actions, 0x99 },
    { actor9Talk1Conditions, NULL, 0x9D },
    { actor9Talk2Conditions, actor9Talk2Actions, 0x9E },
    { actor9Talk3Conditions, actor9Talk3Actions, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { actor10Talk0Conditions, actor10Talk0Actions, 0x9A },
    { actor10Talk1Conditions, NULL, 0x9D },
    { actor10Talk2Conditions, actor10Talk2Actions, 0x9E },
    { actor10Talk3Conditions, actor10Talk3Actions, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x41E },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, actor12Talk0Actions, 0x30 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, actor13Talk0Actions, 0x30 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { actor14Talk0Conditions, NULL, 0x41B },
    { actor14Talk1Conditions, actor14Talk1Actions, 0x88 },
    { actor14Talk2Conditions, actor14Talk2Actions, 0x87 },
    { actor14Talk3Conditions, actor14Talk3Actions, 0x70 },
    { actor14Talk4Conditions, NULL, 0x72 },
    { actor14Talk5Conditions, actor14Talk5Actions, 0x73 },
    { actor14Talk6Conditions, actor14Talk6Actions, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { actor15Talk0Conditions, NULL, 0x70 },
    { actor15Talk1Conditions, actor15Talk1Actions, 0x87 },
    { actor15Talk2Conditions, actor15Talk2Actions, 0x88 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 0x71 },
    { actor16Talk1Conditions, NULL, 0x72 },
    { actor16Talk2Conditions, actor16Talk2Actions, 0x73 },
    { actor16Talk3Conditions, actor16Talk3Actions, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { actor17Talk0Conditions, actor17Talk0Actions, 0x84 },
    { actor17Talk1Conditions, NULL, 0x72 },
    { actor17Talk2Conditions, actor17Talk2Actions, 0x73 },
    { actor17Talk3Conditions, actor17Talk3Actions, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x41B },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { actor19Talk0Conditions, NULL, 0x41C },
    { actor19Talk1Conditions, actor19Talk1Actions, 0x92 },
    { actor19Talk2Conditions, actor19Talk2Actions, 0x91 },
    { actor19Talk3Conditions, actor19Talk3Actions, 0x75 },
    { actor19Talk4Conditions, NULL, 0x77 },
    { actor19Talk5Conditions, actor19Talk5Actions, 0x78 },
    { actor19Talk6Conditions, actor19Talk6Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor20Talks[] = {
    { actor20Talk0Conditions, actor20Talk0Actions, 0x76 },
    { actor20Talk1Conditions, NULL, 0x77 },
    { actor20Talk2Conditions, actor20Talk2Actions, 0x78 },
    { actor20Talk3Conditions, actor20Talk3Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { actor21Talk0Conditions, actor21Talk0Actions, 0x8D },
    { actor21Talk1Conditions, NULL, 0x77 },
    { actor21Talk2Conditions, actor21Talk2Actions, 0x78 },
    { actor21Talk3Conditions, actor21Talk3Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { actor22Talk0Conditions, NULL, 0x75 },
    { actor22Talk1Conditions, actor22Talk1Actions, 0x91 },
    { actor22Talk2Conditions, actor22Talk2Actions, 0x92 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x41C },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x251 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { actor25Talk0Conditions, NULL, 0x94 },
    { actor25Talk1Conditions, NULL, 0x7C },
    { actor25Talk2Conditions, actor25Talk2Actions, 0x7D },
    { actor25Talk3Conditions, actor25Talk3Actions, 0x7E },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { actor26Talk0Conditions, NULL, 0x85 },
    { actor26Talk1Conditions, NULL, 0x72 },
    { actor26Talk2Conditions, actor26Talk2Actions, 0x73 },
    { actor26Talk3Conditions, actor26Talk3Actions, 0x74 },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { actor27Talk0Conditions, NULL, 0x9B },
    { actor27Talk1Conditions, NULL, 0x9D },
    { actor27Talk2Conditions, actor27Talk2Actions, 0x9E },
    { actor27Talk3Conditions, actor27Talk3Actions, 0x9F },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { actor28Talk0Conditions, NULL, 0x8F },
    { actor28Talk1Conditions, NULL, 0x77 },
    { actor28Talk2Conditions, actor28Talk2Actions, 0x78 },
    { actor28Talk3Conditions, actor28Talk3Actions, 0x79 },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x247 },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { actor30Talk0Conditions, actor30Talk0Actions, 0x2F },
    { actor30Talk1Conditions, actor30Talk1Actions, 0x2A5 },
    { actor30Talk2Conditions, actor30Talk2Actions, 0x2A5 },
    { actor30Talk3Conditions, actor30Talk3Actions, 0x2A5 },
    { actor30Talk4Conditions, actor30Talk4Actions, 0x2A5 },
    { actor30Talk5Conditions, actor30Talk5Actions, 0x2A5 },
    { actor30Talk6Conditions, actor30Talk6Actions, 0x2A5 },
    { actor30Talk7Conditions, actor30Talk7Actions, 0x2A5 },
    { actor30Talk8Conditions, actor30Talk8Actions, 0x2A5 },
    { actor30Talk9Conditions, actor30Talk9Actions, 0x2A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { actor31Talk0Conditions, actor31Talk0Actions, 0x63 },
    { actor31Talk1Conditions, NULL, 0x64 },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x4A2 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor2Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor3Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor4Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor5Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor6Conditions[] = {
    ITEM(0, 0x192), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor7Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor8Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor9Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor10Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor11Conditions[] = {
    ITEM(0, 0x192), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor12Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor15Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor16Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor17Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor18Conditions[] = {
    ITEM(0, 0x192), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor19Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor20Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor21Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor22Conditions[] = {
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    ITEM(0, 0x192), 1,
    CODES_END,
};
u16 actor23Conditions[] = {
    ITEM(0, 0x192), 0,
    PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1,
    CODES_END,
};
u16 actor24Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { ITEM(0, 0x192), 1, SPECIAL(9), 1, CODES_END };
u16 actor31Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor32Conditions[] = { ITEM(0, 0x192), 0, SPECIAL(0x1A), 1, CODES_END };
u16 actor34Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor35Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x2D, 4, 255, 176, 3 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x2D, 4, 255, 176, 3 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x2F, 5, 198, 219, 7 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x2F, 5, 198, 219, 7 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x2F, 5, 198, 219, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x2F, 5, 198, 219, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x2F, 5, 198, 219, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x32, 6, 173, 231, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x32, 6, 173, 231, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x32, 6, 173, 231, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x32, 6, 173, 231, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x32, 6, 173, 231, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x33, 7, 348, 147, 1 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x33, 7, 348, 147, 1 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x34, 8, 250, 244, 3 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x34, 8, 250, 244, 3 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x34, 8, 250, 244, 3 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x34, 8, 250, 244, 3 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x34, 8, 250, 244, 3 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x37, 9, 224, 257, 3 };
FieldActorEntry actor20 = { actor20Conditions, actor20Talks, 0x37, 9, 224, 257, 3 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x37, 9, 224, 257, 3 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x37, 9, 224, 257, 3 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x37, 9, 224, 257, 3 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x9D, 0xA, 348, 147, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x9E, 0xB, 198, 219, 7 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x9F, 0xC, 250, 244, 3 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0xA0, 0xD, 173, 231, 7 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0xA1, 0xE, 224, 257, 3 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0xA2, 0xF, 255, 176, 3 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0xCE, 0x10, 379, 147, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0xCE, 0x10, 379, 147, 7 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0xCE, 0x10, 379, 147, 7 };
FieldActorEntry actor33 = { NULL, NULL, 0xDC, 0x11, 379, 163, 7 };
FieldActorEntry actor34 = { actor34Conditions, NULL, 0xE1, 0x12, 347, 158, 1 };
FieldActorEntry actor35 = { actor35Conditions, NULL, 0x10E, 0x13, 347, 158, 1 };
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x201, 0x1F0, 0xA0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
FieldEvent stageEvents[] = {
    { 59, script59, EVENT_TEXT(0), NULL, NULL },
    { 65, script65, EVENT_TEXT(1), NULL, NULL },
    { 1415, script1415, EVENT_TEXT(2), NULL, endEvent1415 },
    { 1518, NULL, EVENT_TEXT(5), startEvent1518, NULL },
    { 1519, script1519, EVENT_TEXT(3), NULL, NULL },
    { 1520, NULL, EVENT_TEXT(6), startEvent1520, NULL },
    { 1521, script1521, EVENT_TEXT(4), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
